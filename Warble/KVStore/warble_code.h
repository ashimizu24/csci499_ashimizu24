#include "warble.pb.h"

// Macros for prefixes 
#define USR_PRE "USR"
#define WARB_PRE "WAR"
#define FOLLOW_PRE "FOLLOWER"

// TODO - Makefile won't compile google:protobuf:any still
namespace warblecode {
  void CreateWarble(google::protobuf::Any any); /// Unpacks any response into warble
  void CreateUser(google::protobuf::Any any); // Unpacks any response into new user
  void CreateWarbleReply(google::protobuf::Any any); //Unpacks any response into reply warble (warble)
  void Follow(google::protobuf::Any any); // Unpacks any response into who follow request
  void Read(google::protobuf::Any any); // Unpacks any response into id of warble user wants to read
  void Profile(google::protobuf::Any any); // Unpacks ay response into username of profile to display

  int warble_cnt = 0;
};