#include <google/protobuf/any.h>
#include <google/protobuf/text_format.h>
#include <iostream>
#include <string.h>

#include <grpcpp/grpcpp.h>

#include "../commandline/events.h"
#include "func.grpc.pb.h"
#include "func.pb.h"
#include "kvstore.grpc.pb.h"
#include "warble_code.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Server that takes in processes requests from the commandline and redirects
// them to the kvstore
class FuncHandler final : public func::KeyValueStore::Service {
public:
  FuncHandler(std::unique_ptr<KVBase> kvstore) : wc_(std::move(kvstore)) {}
  // Hook processes messages and calls certain event types
  grpc::Status hook(grpc::ServerContext *context,
                    const func::HookRequest *request,
                    func::HookReply *reply) override;

  grpc::Status unhook(grpc::ServerContext *context,
                      const func::UnhookRequest *request,
                      func::UnhookReply *reply) override;

  // Where all the inputs from commandline come in to
  grpc::Status event(grpc::ServerContext *context,
                     const func::EventRequest *request,
                     func::EventReply *response) override;

  // Helper function to populate map of event type names to the functions
  void PopulateMap();
  grpc::Status AddFunc(const func::HookRequest *request);

private:
  WarbleCode wc_;
  std::unordered_map<
      std::string,
      std::function<grpc::Status(WarbleCode &, const google::protobuf::Any &,
                                 google::protobuf::Any *)>>
      nametofuncmap_;
  std::unordered_map<int, std::function<grpc::Status(
                              WarbleCode &, const google::protobuf::Any &,
                              google::protobuf::Any *)>>
      typetofuncmap_;
};