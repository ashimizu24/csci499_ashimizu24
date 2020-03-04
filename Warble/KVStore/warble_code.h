#include <fstream>
#include <bitset>
#include <grpcpp/grpcpp.h>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/util/time_util.h>
#include <thread>
#include <sstream>
#include "warble.pb.h"
#include "../kvstore/kvstore.grpc.pb.h"

using google::protobuf::Any;

// Macros for prefixes 
#define USR_PRE "USR"
#define WARB_PRE "WAR"
#define FOLLOWING_PRE "FOLLOWING"
#define FOLLOWER_PRE "FOLLOWER"

// TODO - Makefile won't compile google:protobuf:any still

class WarbleCode{
public:
	// Creates the Func Client and instantiates a new stub to send requests
  WarbleCode(std::shared_ptr<grpc::Channel> channel)
      : stub_(kvstore::KeyValueStore::NewStub(channel)) {}

  void CreateWarble(google::protobuf::Any any); /// Unpacks any response into warble
  grpc::Status CreateUser(google::protobuf::Any any); // Unpacks any response into new user
  void CreateWarbleReply(google::protobuf::Any any); //Unpacks any response into reply warble (warble)
  grpc::Status Follow(google::protobuf::Any any); // Unpacks any response into who follow request
  grpc::Status Read(google::protobuf::Any any, google::protobuf::Any& anyreply);// Unpacks any response into id of warble user wants to read
  void Profile(google::protobuf::Any any, google::protobuf::Any& anyreply); // Unpacks ay response into username of profile to display

  void PutRequest(std::string key, std::string value);
  std::string GetRequest(std::string key);
  void RemoveRequest(std::string key);

  bool ValExists(const std::string key);
 
  std::unique_ptr<kvstore::KeyValueStore::Stub> stub_;

private:
  int warble_cnt = 0; // Unique warble id
};
