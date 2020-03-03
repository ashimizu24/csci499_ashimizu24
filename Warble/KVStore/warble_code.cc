#include "warble_code.h"

bool WarbleCode::CreateUser(google::protobuf::Any any) {
  std::cout << "create " << std::endl;
  // If user already doesn't exist
  // Create new user request object from unpacking payload 
  warble::RegisteruserRequest newuserrequest;
  if (any.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.username();
    if(UserExists(key)){
      return 0;
    }
    
    PutRequest(key, "/"); //User data will be in FOLLOWERS/FOLLOWING form
  }
  return 1;
}

void WarbleCode::CreateWarble(google::protobuf::Any any) {
  warble::Timestamp *timestamp = new warble::Timestamp();
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  timestamp->set_seconds(seconds);
  timestamp->set_useconds(useconds);
  // convert int warble id to char*
  std::string s = std::to_string(warble_cnt);
  char const *pchar = s.c_str();

  // Create new warble from unpacking warble request and adding parameters
  warble::Warble new_warble; 
  warble::WarbleRequest warble_request;
  if (any.UnpackTo(&warble_request)) {
    // Add elements that were not in the WarbleRequest
    std::string key = WARB_PRE + std::to_string(warble_cnt);

    new_warble.set_id(pchar);
    new_warble.set_allocated_timestamp(timestamp); 
    new_warble.set_username(warble_request.username());  
    new_warble.set_text(warble_request.text()); 
    new_warble.set_parent_id(warble_request.parent_id()); 

    warble_cnt++;

    std::string value;
    new_warble.SerializeToString(&value);

    PutRequest(key, value);
  }
}

void WarbleCode::CreateWarbleReply(google::protobuf::Any any) {
  CreateWarble(any);
}

void WarbleCode::Follow(google::protobuf::Any any) {
  warble::FollowRequest followrequest;
  if (any.UnpackTo(&followrequest)) {
    std::string key1 = USR_PRE + followrequest.username();
    std::string key2 = USR_PRE + followrequest.to_follow();
    // Check if both users exists
    if(!UserExists(key1) || !UserExists(key2)){
      return;
    }
    // TODO Check if users are the same 
    if(key1.compare(key2) == 0){
      return;
    }
    // Add following to user
    std::string val1 = GetRequest(key1);
    val1+=followrequest.to_follow();
    val1+=",";
    RemoveRequest(key1);
    PutRequest(key1, val1);

    // Add follower to user
    std::string val2 = GetRequest(key2);
    std::string delimiter = "/";
    std::string followers = val2.substr(0, val2.find(delimiter));
    std::string following = val2.substr(1, val2.find(delimiter));
    followers+=",";
    followers+=followrequest.username();
    val2 = followers+"/"+following;
    RemoveRequest(key2);
    PutRequest(key2, val2);
  }
}

google::protobuf::Any WarbleCode::Read(google::protobuf::Any any) {
  warble::ReadRequest readrequest;
  warble::ReadReply readreply;
  if (any.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.warble_id();
    
    bool end = false;
    int count = 0;
    while(end){
      std::string val = GetRequest(key);
      warble::Warble warble; 
      warble.ParseFromString(val);
      *readreply.mutable_warbles(count) = warble;

      if(warble.parent_id().compare("-1") == 0) {
        end = true;
      }
      else{
        key = WARB_PRE + warble.parent_id();
      }
      count++;
    }
  }
  google::protobuf::Any payload;
  payload.PackFrom(readreply);
  return payload;
}

void WarbleCode::Profile(google::protobuf::Any any, google::protobuf::Any& anyreply) {
  warble::ProfileRequest profilerequest;
  warble::ProfileReply profilereply;
  if (any.UnpackTo(&profilerequest)) {
    std::string key = USR_PRE + profilerequest.username();
    // Check if user exists
    if(!UserExists(key)){
      return;
    }
    // Get user's followers and who they're following
    std::string val = GetRequest(key);
    std::cout << profilerequest.username() << " : " << val << std::endl;
    std::string delimiter = "/";
    std::string followers = val.substr(0, val.find(delimiter));
    std::stringstream ss(followers);
    std::string token;
    int index = 1;
    while(std::getline(ss, token, ',')) {
      profilereply.set_followers(index, token);
      index++;
    }

    std::string following = val.substr(1, val.find(delimiter));
    std::stringstream ss1(following);
    index = 1;
    while(std::getline(ss1, token, ',')) {
      profilereply.set_following(index, token);
      index++;
    }
  }
  anyreply.PackFrom(profilereply);
}

std::string WarbleCode::GetRequest(std::string key) {
   grpc::ClientContext context;

  std::shared_ptr<grpc::ClientReaderWriter<kvstore::GetRequest, kvstore::GetReply>> stream(stub_->get(&context));

  // add request to stream
  std::thread write_to_stream([&key, &stream]() {
    kvstore::GetRequest request;
    request.set_key(key);
    stream->Write(request);
    stream->WritesDone();
  });

  // get value from stream
  kvstore::GetReply reply;
  stream->Read(&reply);
  std::string val = reply.value();
  write_to_stream.join();
  grpc::Status status = stream->Finish();

  return val;
}

void WarbleCode::PutRequest(std::string key, std::string value) {
  grpc::ClientContext context;
  kvstore::PutRequest request;
  request.set_key(key);
  request.set_value(value);
  kvstore::PutReply reply;

  grpc::Status status = stub_->put(&context, request, &reply);
  //return status;
  // if(status.ok()) {
  //   std::cout << "worked\n";
  // } else {
  //   std::cout << status.error_code() << ": " << status.error_message() << std::endl; 
  // }
}

void WarbleCode::RemoveRequest(std::string key) {
  grpc::ClientContext context;
  kvstore::RemoveRequest request;
  request.set_key(key);
  kvstore::RemoveReply reply;

  grpc::Status status = stub_->remove(&context, request, &reply);
  //return status;
  // if(status.ok()) {
  //   std::cout << "worked\n";
  // } else {
  //   std::cout << status.error_code() << ": " << status.error_message() << std::endl; 
  // }
}

bool WarbleCode::UserExists(const std::string key) {
  std::string val = GetRequest(key);
    if(val == "does not exist"){ // User doesn't exists
      return false;
  }
  return true;
}
