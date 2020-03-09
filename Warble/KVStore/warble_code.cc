#include "warble_code.h"

grpc::Status WarbleCode::CreateUser(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  // If user already doesn't exist
  // Create new user request object from unpacking payload 
  warble::RegisteruserRequest newuserrequest;
  if (request.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.username();
    if(ValExists(key)){
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
  std::string warble_cnt_str = std::to_string(warble_cnt);

  // Create new warble from unpacking warble request and adding parameters
  warble::Warble new_warble; 
  warble::WarbleRequest warble_request;
  if (request.UnpackTo(&warble_request)) {
    // Add elements that were not in the WarbleRequest
    std::string key = WARB_PRE + std::to_string(warble_cnt);

    new_warble.set_id(warble_cnt_str);
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
    std::string usrname_key = USR_PRE + followrequest.username();
    std::string tofollow_key = USR_PRE + followrequest.to_follow();
    // Check if both users exists
    if(!ValExists(usrname_key) || !ValExists(tofollow_key)){
      return grpc::Status::CANCELLED;
    }
    // Check if users are the same 
    if(usrname_key.compare(tofollow_key) == 0){
      return grpc::Status::CANCELLED;
    }

    //Check if users are already following each other
    std::string check = kvstore_->GetRequest(tofollow_key);
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
    std::string usrname_val = kvstore_->GetRequest(usrname_key);
    usrname_val+=followrequest.to_follow();
    usrname_val+=",";
    kvstore_->RemoveRequest(usrname_key);
    kvstore_->PutRequest(usrname_key, usrname_val);

    // Add follower to user
    std::string tofollow_val = kvstore_->GetRequest(tofollow_key);
    tofollow_val = followrequest.username() + "," + tofollow_val; 
    kvstore_->RemoveRequest(tofollow_key);
    kvstore_->PutRequest(tofollow_key, tofollow_val);
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Read(const google::protobuf::Any& request, google::protobuf::Any& reply) {
  warble::ReadRequest readrequest;
  warble::ReadReply readreply;
  if (request.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.warble_id();
    bool proceed = true;
    int count = 0;
    while(proceed){
      std::string val = kvstore_->GetRequest(key);

      if(val == "does not exist") { // Warble doesn't exist in kvstore
        return grpc::Status::CANCELLED;
      }

      warble::Warble warble; 
      warble.ParseFromString(val);
      *readreply.add_warbles() = warble;
      std::cout << warble.parent_id() << std::endl;
      if(warble.parent_id().compare("-1") == 0) {
        proceed = false;
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
    if(!ValExists(key)){
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

bool WarbleCode::ValExists(const std::string key) {
  std::string val = kvstore_->GetRequest(key);
  if(val == "does not exist"){ // User doesn't exists
    return false;
  }
  return true;
}

