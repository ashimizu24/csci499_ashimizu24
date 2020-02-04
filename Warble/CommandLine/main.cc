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

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  GOOGLE_PROTOBUF_VERIFY_VERSION;
  
  /// Error check - user was not defined for flags that require it (all except registeruser)
  if (FLAGS_user == "" && FLAGS_registeruser == "")
  {
  	std::cout << "User not defined" << std::endl;
  	return 1;
  }

  // If registeruser flag triggered with argument - add user
  if (FLAGS_registeruser != "")
  {
    warble::RegisteruserRequest* newuser = new warble::RegisteruserRequest();
  	newuser->set_username(FLAGS_registeruser);
    // TODO - GRPC call
  	
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
