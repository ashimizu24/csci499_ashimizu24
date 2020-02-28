#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "events.h"
#include "../kvstore/warble.pb.h"
#include "../kvstore/func.grpc.pb.h"

class FuncClient {
public:
  // Creates the Func Client and instantiates a new stub to send requests
  FuncClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(func::KeyValueStore::NewStub(channel)) {}

  // Creates a new user with given username 
  void RegisterUser(const std::string& username);   
  // Creates a new warble with given username and text  
  void CreateWarble(const std::string& username, const std::string& text); 
  // Creates a new warble that is responding to another warble
  void CreateWarbleReply(const std::string& username, const std::string& text, const std::string& id, const WarbleTypes type); 
  // Adds a user to be following another user
  void Follow(const std::string& username, const std::string& usernametofollow);
  // Getting a warble based on id and displays for user to read
  void Read(const std::string& warbleid);
  // Displays who a user is following and their followers
  void Profile(const std::string& username);

  std::unique_ptr<func::KeyValueStore::Stub> stub_;
};