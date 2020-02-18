#include <iostream>
#include <getopt.h>
#include <string>

#include <gflags/gflags.h>

#include "../kvstore/warble.pb.h"

//to compile: g++ main.cc -o main -lgflags -pthread -lprotobuf -lpthread

//gflags
DEFINE_string(registeruser, "", "Registers the given username. Enter as -registeruser <username>");
DEFINE_string(user, "", "Logs in as the given username. Enter as -user <username>");
DEFINE_string(warble, "", "Creates a new warble with the given text. Enter as -warble <warble text>");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id. Enter as -reply <reply warble id>");
DEFINE_string(follow, "", "Starts following the given username. Enter as -follow <username>");
DEFINE_string(read, "", "Reads the warble thread starting at the given id. Enter as -read <warble id>");
DEFINE_string(profile, "", "Gets the user’s profile of following and followers. Enter as -profile");

void RegisterUser(const std::string username) {
  ClientContext context;
  warble::RegisteruserRequest newuserrequest;
  warble::RegisteruserReply reply;

  newuserrequest.set_username(username);
  // QUESTION - do we create and pass in an event request here?
 
  Status status = stub_->GetFeature(&context, newuserrequest, &reply);
}

int main(int argc, char *argv[]) {
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
  if (FLAGS_registeruser != "") {
    RegisterUser(FLAGS_registeruser);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
