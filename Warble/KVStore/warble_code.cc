#include "warble_code.h"

// This method split warble text into a vector of unique words.
// Input : warlbe text in string format.
// Output: A vector with unique words, which contains 
// all unique split warble words. 
std::vector<std::string> CreateUniqueWords(const std::string& warble_text) {
  std::unordered_set<std::string> warble_word_set;
  std::vector<std::string> split_warble_text_vec;
  std::string word = "";
  for (auto ch : warble_text) {
    if (ch == ' ') {
      if (word.length() != 0) {
        warble_word_set.insert(word);
        word = "";
      }
    } else {
      word += ch;
    }
  }

  if (word.length() > 0) {
    warble_word_set.insert(word);
  }

  for (std::string warble_word : warble_word_set) {
    split_warble_text_vec.push_back(warble_word);
  }

  return split_warble_text_vec;
}

grpc::Status WarbleCode::CreateUser(const google::protobuf::Any &request,
                                    google::protobuf::Any *reply) {
  // If user already doesn't exist
  // Create new user request object from unpacking payload
  warble::RegisteruserRequest newuserrequest;
  if (request.UnpackTo(&newuserrequest)) {
    std::string key = USR_PRE + newuserrequest.username();
    if (ValExists(key)) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "User already exist");
    }

    // User data will be in FOLLOWERS/FOLLOWING form
    kvstore_->PutRequest(key, "/"); 
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::CreateWarble(const google::protobuf::Any &request,
                                      google::protobuf::Any *reply) {
  warble::Timestamp *timestamp = new warble::Timestamp();
  int64_t seconds = google::protobuf::util::TimeUtil::TimestampToSeconds(
      google::protobuf::util::TimeUtil::GetCurrentTime());
  int64_t useconds = google::protobuf::util::TimeUtil::TimestampToMicroseconds(
      google::protobuf::util::TimeUtil::GetCurrentTime());
  timestamp->set_seconds(seconds);
  timestamp->set_useconds(useconds);
  // convert int warble id to char*
  std::string warble_cnt_str = std::to_string(warble_cnt);

  // Create new warble from unpacking warble request and adding parameters
  warble::Warble new_warble;
  warble::WarbleRequest warble_request;
  if (request.UnpackTo(&warble_request)) {
    // Add elements that were not in the WarbleRequest
    std::string newwarble_key = WARB_PRE + std::to_string(warble_cnt);

    // Check parent id
    if(std::stoi(warble_request.parent_id()) >= warble_cnt || std::stoi(warble_request.parent_id()) < -1){
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Parent id is invalid");
    }

    // Check is user exists
    if(!ValExists(USR_PRE + warble_request.username())){
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "User doesn't exist");
    }

    // Add a child if the parent isn't the root
    if(std::stoi(warble_request.parent_id()) != -1) {
      std::string newchild_key = CHILDREN_PRE + warble_request.parent_id();
      std::string newchild_value = kvstore_->GetRequest(newchild_key);

       // Warble doesn't have any children yet
      if(newchild_value.compare("does not exist") == 0){ 
        kvstore_->PutRequest(newchild_key, warble_cnt_str);
      }
      else{
        newchild_value += ",";
        newchild_value += warble_cnt_str;
        kvstore_->RemoveRequest(newchild_key);
        kvstore_->PutRequest(newchild_key, newchild_value);
      }
    }

    new_warble.set_id(warble_cnt_str);
    new_warble.set_allocated_timestamp(timestamp);
    new_warble.set_username(warble_request.username());
    new_warble.set_text(warble_request.text());
    new_warble.set_parent_id(warble_request.parent_id());

    warble_cnt++; // incremement unique id for warble

    // serialize warble into byte string to store in kvstore
    std::string newwarble_value;
    new_warble.SerializeToString(&newwarble_value);

    // Add warble to database
    kvstore_->PutRequest(newwarble_key, newwarble_value);

    // After generating serialized warble, kvstore_->StreamPutRequest should
    // be called here to associate serialzed warble with hashtags.
    std::vector<std::string> split_warble_texts = CreateUniqueWords(warble_request.text());
    kvstore_->StreamPutRequest(split_warble_texts, newwarble_value);

    warble::WarbleReply warblereply;
    *warblereply.mutable_warble() = new_warble;
    reply->PackFrom(warblereply);
  }
  return grpc::Status::OK;
}

grpc::Status WarbleCode::CreateWarbleReply(const google::protobuf::Any &request,
                                           google::protobuf::Any *reply) {
  return CreateWarble(request, reply);
}

grpc::Status WarbleCode::Follow(const google::protobuf::Any &request,
                                google::protobuf::Any *reply) {
  warble::FollowRequest followrequest;
  if (request.UnpackTo(&followrequest)) {
    std::string usrname_key = USR_PRE + followrequest.username();
    std::string tofollow_key = USR_PRE + followrequest.to_follow();
    // Check if both users exists
    if (!ValExists(usrname_key) || !ValExists(tofollow_key)) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "One or more of the users are invalid");
    }
    // Check if users are the same
    if (usrname_key.compare(tofollow_key) == 0) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Users are the same");
    }

    // Check if users are already following each other
    std::string check_key = kvstore_->GetRequest(tofollow_key);
    std::stringstream section(check_key);
    std::string section_token;
    while (std::getline(section, section_token, '/')) {
      std::stringstream ss(section_token);
      std::string token;
      while (std::getline(ss, token, ',')) {
        if (token.compare(followrequest.username()) == 0) { 
          return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Users are already following each other");
        }
      }
      break;
    }

    // Add following to user
    std::string usrname_val = kvstore_->GetRequest(usrname_key);
    usrname_val += followrequest.to_follow();
    usrname_val += ",";
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

grpc::Status WarbleCode::Read(const google::protobuf::Any &request,
                              google::protobuf::Any *reply) {
  warble::ReadRequest readrequest;
  warble::ReadReply readreply;
  if (request.UnpackTo(&readrequest)) {
    std::string key = WARB_PRE + readrequest.warble_id();

    // Check if warble id exists in kvstore
    if (kvstore_->GetRequest(key).compare("does not exist") == 0) { 
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Invalid warble id");
    }
    std::string children_key = CHILDREN_PRE + readrequest.warble_id();
    std::string children = kvstore_->GetRequest(children_key);
    
    std::vector<std::string> all_warbles;
    all_warbles.push_back(readrequest.warble_id());

    int num_children = 1;
    for(int i=0; i<num_children; i++){
      // Insert warble
      std::string child_key = WARB_PRE + all_warbles[i];
      std::string child_val = kvstore_->GetRequest(child_key);
      warble::Warble warble;
      warble.ParseFromString(child_val);
      *readreply.add_warbles() = warble;

      // check if warble has a child
      std::string grandchild_key = CHILDREN_PRE + all_warbles[i];
      std::string grandchild_val = kvstore_->GetRequest(grandchild_key);
      if(grandchild_val.compare("does not exist") != 0){
        std::stringstream gchild_ss(grandchild_val);
        std::string grandchild_token;
        while (std::getline(gchild_ss, grandchild_token, ',')) {
          all_warbles.push_back(grandchild_token);
          num_children++; 
        }
      }
    }

  }
  reply->PackFrom(readreply);
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Profile(const google::protobuf::Any &request,
                                 google::protobuf::Any *reply) {
  warble::ProfileRequest profilerequest;
  warble::ProfileReply profilereply;
  if (request.UnpackTo(&profilerequest)) {
    std::string key = USR_PRE + profilerequest.username();

    // Check if user exists
    if (!ValExists(key)) {
      return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "User doesn't exist");
    }
    // Get user's followers and who they're following
    bool followers = true;
    std::string val = kvstore_->GetRequest(key);

    std::stringstream section(val);
    std::string section_token;
    while (std::getline(section, section_token, '/')) {
      if (followers) {
        std::stringstream ss(section_token);
        std::string token;
        while (std::getline(ss, token, ',')) {
          *profilereply.add_followers() = token;
        }
        followers = false;
      } else {
        std::stringstream ss(section_token);
        std::string token;
        while (std::getline(ss, token, ',')) {
          *profilereply.add_following() = token;
        }
      }
    }
  }
  reply->PackFrom(profilereply);
  return grpc::Status::OK;
}

grpc::Status WarbleCode::Stream(const google::protobuf::Any &request,
                                 google::protobuf::Any *reply) {
  warble::StreamRequest streamrequest;
  warble::StreamReply streamreply;
  if (request.UnpackTo(&streamrequest)) {
    std::string hashtag = streamrequest.hashtag();
    std::vector<std::string> serialized_warbles = kvstore_->StreamGetRequest(hashtag);
    for (std::string serialized_warble : serialized_warbles) {
      warble::Warble warble;
      warble.ParseFromString(serialized_warble);
      *(streamreply.add_warbles()) = warble;
    }
    reply->PackFrom(streamreply);
    return grpc::Status::OK;
  } else {
    return grpc::Status(grpc::StatusCode::INVALID_ARGUMENT, "Wrong format for StreamRequest");
  }
}

bool WarbleCode::ValExists(const std::string key) {
  std::string val = kvstore_->GetRequest(key);
  if (val == "does not exist") { // User doesn't exists
    return false;
  }
  return true;
}
