#include <google/protobuf/text_format.h>
#include <iostream>
#include <string.h>
#include <google/protobuf/any.h>

#include <grpcpp/grpcpp.h>

#include "func.grpc.pb.h"
#include "func.pb.h"
#include "warble_code.h"
#include "../commandline/events.h" 
#include "kvstore.grpc.pb.h"

//using func::KeyValueStore;
using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

//TODO - compile error saying KeyValueStore has not been declared

// Server that takes in processes requests from the commandline and redirects them to the kvstore
class FuncHandler final : public func::KeyValueStore::Service { 
public:
   FuncHandler(std::shared_ptr<grpc::Channel> channel) : wc_(channel) {}
  // Hook processes messages and calls certain event types
  grpc::Status hook(grpc::ServerContext* context, const func::HookRequest request, func::HookReply reply);

  grpc::Status unhook(grpc::ServerContext* context, const func::UnhookRequest request, func::UnhookReply reply);

  // Where all the inputs come in 
  //grpc::Status event(grpc::ServerContext* context, const func::EventRequest request, func::EventReply reply) override;
  grpc::Status event(grpc::ServerContext* context, const func::EventRequest* request, func::EventReply* response) override;

private:
  WarbleCode wc_;
};