#include <iostream>
#include <getopt.h>
#include <string>
#include <gflags/gflags.h>

#include "../kvstore/protobuf-3.11.2/warble.pb.h"

//to compile: g++ main.cc -o main -lgflags

//gflags
DEFINE_string(registeruser, "", "Registers the given username. Enter as -registeruser <username>\n");
DEFINE_string(user, "", "Logs in as the given username. Enter as -user <username>\n");
DEFINE_string(warble, "", "Creates a new warble with the given text. Enter as -warble <warble text>\n");
DEFINE_string(reply, "", "Indicates that the new warble is a reply to the given id. Enter as -reply <reply warble id>\n");
DEFINE_string(follow, "", "Starts following the given username. Enter as -follow <username>\n");
DEFINE_string(read, "", "Reads the warble thread starting at the given id. Enter as -read <warble id>\n");
DEFINE_string(profile, "", "Gets the user’s profile of following and followers. Enter as -profile\n");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  GOOGLE_PROTOBUF_VERIFY_VERSION;
  
  ///user was not defined for flags that require it (all except registeruser)
  if(FLAGS_user == "" && FLAGS_registeruser == ""){
  	std::cout << "User not defined" << std::endl;
  	return 0;
  }

  //If registeruser flag triggered with argument - add user
  if(FLAGS_registeruser != ""){
  	warble::RegisteruserRequest* newuser;
  	std::cout << FLAGS_registeruser << std::endl;
  	newuser->set_username(FLAGS_registeruser);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
