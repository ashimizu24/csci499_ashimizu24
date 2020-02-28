#include <sstream>
#include <bitset>
#include <grpcpp/grpcpp.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/util/time_util.h>

#include "warble.pb.h"
#include "../kvstore/kvstore.grpc.pb.h"

using google::protobuf::Any;

// Macros for prefixes 
#define USR_PRE "USR"
#define WARB_PRE "WAR"
#define FOLLOW_PRE "FOLLOWER"

// TODO - Makefile won't compile google:protobuf:any still

class WarbleCode{
public:
	// Creates the Func Client and instantiates a new stub to send requests
  // WarbleCode(std::shared_ptr<grpc::Channel> channel)
  //     : stub_(kvstore::KeyValueStore::NewStub(channel)) {}

  void CreateWarble(google::protobuf::Any any); /// Unpacks any response into warble
  void CreateUser(google::protobuf::Any any); // Unpacks any response into new user
  void CreateWarbleReply(google::protobuf::Any any); //Unpacks any response into reply warble (warble)
  void Follow(google::protobuf::Any any); // Unpacks any response into who follow request
  void Read(google::protobuf::Any any); // Unpacks any response into id of warble user wants to read
  void Profile(google::protobuf::Any any); // Unpacks ay response into username of profile to display

private:
  int warble_cnt = 0; // Unique warble id
  //std::unique_ptr<kvstore::KeyValueStore::Stub> stub_;
};
