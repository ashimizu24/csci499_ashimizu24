#include <google/protobuf/text_format.h>
#include <iostream>
#include <string.h>
#include <google/protobuf/any.h>

#include <grpcpp/grpcpp.h>
// #include <gflags/gflags.h>
// #include <glog/logging.h>

#include "func.grpc.pb.h"
#include "func.pb.h"
#include "warble_code.h"
#include "../commandline/events.h" 
#include "kvstore.grpc.pb.h"
// #include "KVWrapper.cc"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

//TODO - compile error saying KeyValueStore has not been declared

// Server that takes in processes requests from the commandline and redirects them to the kvstore
class FuncHandler final : public func::KeyValueStore::Service { 
public:
   FuncHandler(std::unique_ptr<KVBase> kvstore) : wc_(std::move(kvstore)) {}
  // Hook processes messages and calls certain event types
  grpc::Status hook(grpc::ServerContext* context, const func::HookRequest* request, func::HookReply* reply) override;

  grpc::Status unhook(grpc::ServerContext* context, const func::UnhookRequest* request, func::UnhookReply* reply) override;

  // Where all the inputs from commandline come in to 
  grpc::Status event(grpc::ServerContext* context, const func::EventRequest* request, func::EventReply* response) override;
  
  // Helper function to populate map of event type names to the functions
  void PopulateMap();
  grpc::Status AddFunc(const func::HookRequest* request);
private:
  WarbleCode wc_;
  std::unordered_map<std::string, std::function<grpc::Status(const WarbleCode, const google::protobuf::Any&, google::protobuf::Any&)>> nametofuncmap_;
  //std::unordered_map<std::string, int> nametotypemap_;
  std::unordered_map<int, std::function<grpc::Status(const WarbleCode, const google::protobuf::Any&, google::protobuf::Any&)>> typetofuncmap_;
};