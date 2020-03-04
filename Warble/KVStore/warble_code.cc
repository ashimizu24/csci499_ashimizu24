#include "warble_code.h"

grpc::Status WarbleCode::CreateUser(google::protobuf::Any any) {
  // If user already doesn't exist
  // Create new user request object from unpacking payload 
  warble::RegisteruserRequest newuserrequest;
  if (any.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.username();
    if(ValExists(key)){
      return grpc::Status::CANCELLED;
    }
    
    PutRequest(key, "/"); //User data will be in FOLLOWERS/FOLLOWING form
  }
  return grpc::Status::OK;
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

    warble_cnt++; //incremement unique id for warble

    // serialize warble into byte string to store in kvstore
    std::string value;
    new_warble.SerializeToString(&value);

    PutRequest(key, value);
  }
}

void WarbleCode::CreateWarbleReply(google::protobuf::Any any) {
  CreateWarble(any);
}

grpc::Status WarbleCode::Follow(google::protobuf::Any any) {
  warble::FollowRequest followrequest;
  if (any.UnpackTo(&followrequest)) {
    std::string key1 = USR_PRE + followrequest.username();
    std::string key2 = USR_PRE + followrequest.to_follow();
    // Check if both users exists
    if(!ValExists(key1) || !ValExists(key2)){
      return grpc::Status::CANCELLED;
    }
    // Check if users are the same 
    if(key1.compare(key2) == 0){
      return grpc::Status::CANCELLED;
    }

    //Check if users are already following each other
    std::string check = GetRequest(key2);
    std::stringstream section(check);
    std::string section_token;
    while(std::getline(section, section_token, '/')) {
      std::stringstream ss(section_token);
      std::string token;
      while(std::getline(ss, token, ',')) {
        if(token.compare(followrequest.username()) == 0){ //user already followers
          return grpc::Status::CANCELLED;
        }
      }
      break;
    }
    // Add following to user
    std::string val1 = GetRequest(key1);
    val1+=followrequest.to_follow();
    val1+=",";
    RemoveRequest(key1);
    PutRequest(key1, val1);

    // Add follower to user
    std::string val2 = GetRequest(key2);
    val2 = followrequest.username() + "," + val2; 
    RemoveRequest(key2);
    PutRequest(key2, val2);
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Read(google::protobuf::Any any, google::protobuf::Any& anyreply) {
  warble::ReadRequest readrequest;
  warble::ReadReply readreply;
  if (any.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.warble_id();
    bool exit = true;
    int count = 0;
    while(exit){
      std::string val = GetRequest(key);

      if(val == "does not exist") { // Warble doesn't exist in kvstore
        return grpc::Status::CANCELLED;
      }

      warble::Warble warble; 
      warble.ParseFromString(val);
      *readreply.add_warbles() = warble;
      std::cout << warble.parent_id() << std::endl;
      if(warble.parent_id().compare("-1") == 0) {
        exit = false;
      }
      else{
        key = WARB_PRE + warble.parent_id();
      }
      count++;
    }
  }
  anyreply.PackFrom(readreply);
  return grpc::Status::OK;
}

void WarbleCode::Profile(google::protobuf::Any any, google::protobuf::Any& anyreply) {
  warble::ProfileRequest profilerequest;
  warble::ProfileReply profilereply;
  if (any.UnpackTo(&profilerequest)) {
    std::string key = USR_PRE + profilerequest.username();
    // Check if user exists
    if(!ValExists(key)){
      return;
    }
    // Get user's followers and who they're following
    bool followers = true;
    std::string val = GetRequest(key);
  
    std::stringstream section(val);
    std::string section_token;
     while(std::getline(section, section_token, '/')) {
      if(followers){
        std::stringstream ss(section_token);
        std::string token;
        while(std::getline(ss, token, ',')) {
          *profilereply.add_followers() = token;
        }
        followers = false;
      }
      else{
        std::stringstream ss(section_token);
        std::string token;
        while(std::getline(ss, token, ',')) {
          *profilereply.add_following() = token;
        }
      }
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

bool WarbleCode::ValExists(const std::string key) {
  std::string val = GetRequest(key);
    if(val == "does not exist"){ // User doesn't exists
      return false;
  }
  return true;
}
