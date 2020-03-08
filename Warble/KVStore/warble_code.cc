#include "warble_code.h"

grpc::Status WarbleCode::CreateUser(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  // If user already doesn't exist
  // Create new user request object from unpacking payload 
  warble::RegisteruserRequest newuserrequest;
  if (request.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.username();
    if(kvstore_->ValExists(key)){
      return grpc::Status::CANCELLED;
    }
    
    kvstore_->PutRequest(key, "/"); //User data will be in FOLLOWERS/FOLLOWING form
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::CreateWarble(const google::protobuf::Any& request, google::protobuf::Any& reply) {
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
  if (request.UnpackTo(&warble_request)) {
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

    kvstore_->PutRequest(key, value);
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::CreateWarbleReply(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  return CreateWarble(request, reply);
}

grpc::Status WarbleCode::Follow(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  warble::FollowRequest followrequest;
  if (request.UnpackTo(&followrequest)) {
    std::string key1 = USR_PRE + followrequest.username();
    std::string key2 = USR_PRE + followrequest.to_follow();
    // Check if both users exists
    if(!kvstore_->ValExists(key1) || !kvstore_->ValExists(key2)){
      return grpc::Status::CANCELLED;
    }
    // Check if users are the same 
    if(key1.compare(key2) == 0){
      return grpc::Status::CANCELLED;
    }

    //Check if users are already following each other
    std::string check = kvstore_->GetRequest(key2);
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
    std::string val1 = kvstore_->GetRequest(key1);
    val1+=followrequest.to_follow();
    val1+=",";
    kvstore_->RemoveRequest(key1);
    kvstore_->PutRequest(key1, val1);

    // Add follower to user
    std::string val2 = kvstore_->GetRequest(key2);
    val2 = followrequest.username() + "," + val2; 
    kvstore_->RemoveRequest(key2);
    kvstore_->PutRequest(key2, val2);
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Read(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  warble::ReadRequest readrequest;
  warble::ReadReply readreply;
  if (request.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.warble_id();
    bool exit = true;
    int count = 0;
    while(exit){
      std::string val = kvstore_->GetRequest(key);

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
  reply.PackFrom(readreply);
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Profile(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  warble::ProfileRequest profilerequest;
  warble::ProfileReply profilereply;
  if (request.UnpackTo(&profilerequest)) {
    std::string key = USR_PRE + profilerequest.username();
    // Check if user exists
    if(!kvstore_->ValExists(key)){
      return grpc::Status::CANCELLED;
    }
    // Get user's followers and who they're following
    bool followers = true;
    std::string val = kvstore_->GetRequest(key);
  
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
  reply.PackFrom(profilereply);
  return grpc::Status::OK;
}


