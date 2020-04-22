#include <unistd.h>
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>

#include "../kvstore/func.grpc.pb.h"
#include "../kvstore/warble.pb.h"
#include "events.h"


class FuncClient {
public:
  // Creates the Func Client and instantiates a new stub to send requests
  FuncClient(std::shared_ptr<grpc::Channel> channel)
      : stub_(func::KeyValueStore::NewStub(channel)) {}

  // Creates a new user with given username
  // Parameter: string with username of new user to create
  // Return: void
  void RegisterUser(const std::string &username);

  // Creates a new warble with given username and text
  // Parameter: string of warble text to create and
  // user that wants to create warble
  // Return: void
  void CreateWarble(const std::string &username, const std::string &text);

  // Creates a new warble that is responding to another warble
  // Parameter: string of warble text, user that wants to create warble,
  // id of warble to respond to, warbletype enum
  // Return: void
  void CreateWarbleReply(const std::string &username, const std::string &text,
                         const std::string &id, const WarbleTypes type);

  // Adds a user to be following another user
  // Parameter: string of username who wants to follow and
  // username of user to follow
  // Return: void
  void Follow(const std::string &username, const std::string &usernametofollow);

  // Getting a warble based on id and displays for user to read
  // Parameter: string of warble id to read
  // Return: void
  void Read(const std::string &warbleid);

  // Displays who a user is following and their followers
  // Parameter: string of user of profile to view
  // Return: void
  void Profile(const std::string &username);

  // Read warbles posted by all users that contain
  // hashtag in their text.
  // Parameters: hashtag
  // Return: void
  void Stream(const std::string hashtag);

  // Sends hook request to func_server
  // Parameter, Return: void
  void HookEvents();

  // Send unhook request to func_server
  // Parameter, Return: void
  void UnhookEvents();

private:
  std::unique_ptr<func::KeyValueStore::Stub> stub_;
  std::unordered_map<std::string, WarbleTypes> typemap_;
};