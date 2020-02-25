#include "warble_code.h"

// TODO - warble_code needs a kvstore_server member so it can call put/get/remove via GRPC??

void CreateWarble(google::protobuf::Any any) {
  warble::Timestamp *timestamp = new warble::Timestamp();
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(google::protobuf::util::TimeUtil::GetCurrentTime());
  timestamp->set_seconds(seconds);
  timestamp->set_useconds(useconds);

  // Create new warble from unpacking warble request and adding parameters
  warble::Warble new_warble; //QUESTION - does this need to be set to = new Warble()?
  warble::WarbleRequest warble_request;
  if (any.UnpackTo(&warble_request)) {
    // Add elements that were not in the WarbleRequest
    std::string key = WARB_PRE + std::to_string(warble_cnt);
    new_warble.set_id(warble_cnt);
    new_warble.set_allocated_timestamp(timestamp); 

    new_warble.set_username(warble_request.get_username());  
    new_warble.set_text(warble_request.get_text()); 
    new_warble.set_parent_id(warble_request.get_parent_id()); 

    std::stringstream output;
    output << (std::bitset<sizeof(new_warble) * 8>(new_warble));
    output.str(); 
    //TODO: make a pair of (key, output.str() ) and send to kvstore *PUT
  }

  // increment warble id
  warble_cnt++;
}

void CreateUser(google::protobuf::Any any) {
  // Create new user request object from unpacking payload
  warble::RegisteruserRequest newuserrequest;
  if (any.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.get_username();
    std::stringstream output;

    output << (std::bitset<sizeof(newuserrequest) * 8>(newuserrequest));
    output.str(); 
    //TODO: make a pair of (key, output.str() ) and send to kvstore *PUT
  }
}

void CreateWarbleReply(google::protobuf::Any any) {
  CreateWarble(any);
}

void Follow(google::protobuf::Any any) {
  warble::FollowRequest followrequest;
  if (any.UnpackTo(&followrequest)) {
    std::string key = FOLLOW_PRE + followrequest.get_username();
    std::string payload = followrequest.get_to_follow();

    //TODO: make a pair of (key, payload) and send to kvstore *PUT
  }
}

void Read(google::protobuf::Any any) {
  warble::ReadRequest readrequest;
  if (any.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.get_warble_id();
    
    //TODO: GET request to kvstore
  }

}

void Profile(google::protobuf::Any any) {
  
}