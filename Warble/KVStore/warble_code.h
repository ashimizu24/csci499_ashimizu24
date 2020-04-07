#include "../kvstore/kvstore.grpc.pb.h"
#include "KVWrapper.cc"
#include "warble.pb.h"
#include <bitset>
#include <fstream>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/util/time_util.h>
#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <sstream>
#include <thread>

using google::protobuf::Any;

// Macros for prefixes
#define USR_PRE "USR"
#define WARB_PRE "WAR"
#define CHILDREN_PRE "CHILDREN"
#define WARB_ID "ID"

class WarbleCode {
public:
  // Creates the Func Client and instantiates a new stub to send requests
  WarbleCode(std::unique_ptr<KVBase> kvstore) : kvstore_(std::move(kvstore)) {}

  // Unpacks any response into a warble message
  // Parameters: request object containing message information that unpacks into
  // a warble object, reply to pack response if necessary Return: grpc Status to
  // indicate if creating warble was successful (OK) or unsuccessful (CANCELLED)
  grpc::Status CreateWarble(const google::protobuf::Any &request,
                            google::protobuf::Any *reply);

  // Unpacks any response into new user
  // Parameters: request object containing message information that unpacks into
  // a user object, reply to pack response if necessary Return: grpc Status to
  // indicate if creating user was successful (OK) or unsuccessful (CANCELLED)
  grpc::Status CreateUser(const google::protobuf::Any &request,
                          google::protobuf::Any *reply);

  // Unpacks any response into reply warble (warble)
  // Parameters: request object containing message information that unpacks into
  // a warble object, reply to pack response if necessary Return: grpc Status to
  // indicate if creating warble reply was successful (OK) or unsuccessful
  // (CANCELLED)
  grpc::Status CreateWarbleReply(const google::protobuf::Any &request,
                                 google::protobuf::Any *reply);

  // Unpacks any response into follow request
  // Parameters: request object containing message information that unpacks into
  // a follow object (which user want to follow who), reply to pack response if
  // necessary Return: grpc Status to indicate if following user was successful
  // (OK) or unsuccessful (CANCELLED)
  grpc::Status Follow(const google::protobuf::Any &request,
                      google::protobuf::Any *reply);

  // Unpacks any response into id of warble user wants to read
  // Parameters: request object containing id of which warble user wants to
  // read, reply has warble object to display to user Return: grpc Status to
  // indicate if getting warble was successful (OK) or unsuccessful (CANCELLED)
  grpc::Status Read(const google::protobuf::Any &request,
                    google::protobuf::Any *reply);

  // Unpacks ay response into username of profile to display
  // Parameters: request object containing id of which profile user wants to
  // see, reply has user's follow/follower information Return: grpc Status to
  // indicate if getting user was successful (OK) or unsuccessful (CANCELLED)
  grpc::Status Profile(const google::protobuf::Any &request,
                       google::protobuf::Any *reply);

  // Function to check whether key already exists in the database
  // Parameters: key to search in the database
  // Return: boolean indicating whether key/value exists (true) or if there is
  // no entry yet (false)
  bool ValExists(const std::string key);

private:
  std::unique_ptr<KVBase> kvstore_; // ptr to wrapper class of kvstore
};
