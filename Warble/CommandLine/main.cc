#include <iostream>
#include <getopt.h>
#include <string>

#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>
#include <glog/logging.h>
#include <google/protobuf/any.h>

#include "main.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

//to compile: g++ main.cc -o main -lgflags -pthread -lprotobuf -lpthread

//gflags
DEFINE_string(registeruser, "", "Registers the given username. Enter as -registeruser <username>");
DEFINE_string(user, "", "Logs in as the given username. Enter as -user <username>");
DEFINE_string(warble, "", "Creates a new warble with the given text. Enter as -warble <warble text>");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id. Enter as -reply <reply warble id>");
DEFINE_string(follow, "", "Starts following the given username. Enter as -follow <username>");
DEFINE_string(read, "", "Reads the warble thread starting at the given id. Enter as -read <warble id>");
DEFINE_string(profile, "", "Gets the user’s profile of following and followers. Enter as -profile");

void FuncClient::RegisterUser(const std::string& username) {
  // Objects being passing into stub
  grpc::ClientContext context;
  func::EventRequest request;
  func::EventReply reply;

  // Warble objects
  warble::RegisteruserRequest newuserrequest;
  warble::RegisteruserReply newuserreply;

  // populate warble new user request object and pack event request
  newuserrequest.set_username(username);

  request.set_event_type(kRegisterUser);
  google::protobuf::Any payload;
  payload.PackFrom(newuserrequest);
  *request.mutable_payload() = payload;
  
  // TODO - unpack eventreply into registeruserreply
  grpc::Status status = stub_->event(&context, request, &reply);
  //return status;
  if(status.ok()) {
    std::cout << "worked\n";
  } else {
    std::cout << status.error_code() << ": " << status.error_message() << std::endl; 
  }
}

// If a new independent warble is created - the parent warble id will be -1
void FuncClient::CreateWarble(const std::string& username, const std::string& text) {
  FuncClient::CreateWarbleReply(username, text, "-1", kReply);
}

void FuncClient::CreateWarbleReply(const std::string& username, const std::string& text, const std::string& id, const WarbleTypes type) {
  grpc::ClientContext context;
  // Create new warble request object 
  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username(username);
  newwarblerequest.set_text(text);
  newwarblerequest.set_parent_id(id);

  // Pack registeruserrequest into an eventrequest payload 
  func::EventRequest request;
  request.set_event_type(type);
  google::protobuf::Any payload;
  payload.PackFrom(newwarblerequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC 
  func::EventReply reply;
  warble::WarbleReply warblereply;
  // TODO - unpack eventreply into warblereply
 
  grpc::Status status = stub_->event(&context, request, &reply);
}

void FuncClient::Follow(const std::string& username, const std::string& usernametofollow) {
  grpc::ClientContext context;
  
  warble::FollowRequest followrequest;
  followrequest.set_username(username);
  followrequest.set_to_follow(usernametofollow);

  // Pack followrequest into an eventrequest payload 
  func::EventRequest request;
  request.set_event_type(kFollow);
  google::protobuf::Any payload;
  payload.PackFrom(followrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC 
  func::EventReply reply;
  warble::FollowReply followreply;
  // TODO - Unpack eventreply into followreply 
 
  grpc::Status status = stub_->event(&context, request, &reply);
}

void FuncClient::Read(const std::string& warbleid) {
  grpc::ClientContext context;
  
  warble::ReadRequest readrequest;
  readrequest.set_warble_id(warbleid);

  // Pack readrequest into an eventrequest payload 
  func::EventRequest request;
  request.set_event_type(kRead);
  google::protobuf::Any payload;
  payload.PackFrom(readrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC 
  func::EventReply reply;
  warble::ReadReply readreply;
  // TODO - unpack eventreply into readreply
 
  grpc::Status status = stub_->event(&context, request, &reply);
}

void FuncClient::Profile(const std::string& username) {
  grpc::ClientContext context;
  
  warble::ProfileRequest profrequest;
  profrequest.set_username(username);

  // Pack readrequest into an eventrequest payload 
  func::EventRequest request;
  request.set_event_type(kProfile);
  google::protobuf::Any payload;
  payload.PackFrom(profrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC 
  func::EventReply reply;
  warble::ProfileReply profreply;
  //TODO - unpack eventreply into profreply
 
  grpc::Status status = stub_->event(&context, request, &reply);
}

int main(int argc, char *argv[]) {
  FuncClient func_client(grpc::CreateChannel( "localhost:50000", grpc::InsecureChannelCredentials()));
  
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  /// Error check - user was not defined for flags that require it (all except registeruser)
  if (FLAGS_user.empty() && FLAGS_registeruser.empty()) {
    LOG(INFO) << "User not defined";
  	return 1;
  }

  // If registeruser flag triggered with argument - add user
  if (!FLAGS_registeruser.empty()) {
    func_client.RegisterUser(FLAGS_registeruser);
  }
  else if(!FLAGS_warble.empty()) {
    func_client.CreateWarble(FLAGS_user, FLAGS_warble);
  }
  else if(!FLAGS_warble.empty() && !FLAGS_reply.empty()) {
    func_client.CreateWarbleReply(FLAGS_user, FLAGS_warble, FLAGS_reply, kWarble);
  }
  else if(!FLAGS_read.empty()) {
    func_client.Read(FLAGS_read);
  }
  else if(!FLAGS_profile.empty()) {
    func_client.Profile(FLAGS_user);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
