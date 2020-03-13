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
  // Hook links event types to warble code functions
  // Parameters: GRPC Server context, hook request, hook reply
  // Return: GRPC status to indicate whether hooking was successful
  grpc::Status hook(grpc::ServerContext *context,
                    const func::HookRequest *request,
                    func::HookReply *reply) override;

  // Unhook unlinks the event types to warble code functions
  // Parameters: GRPC Server context, unhook request (single event type), unhook reply
  // Return: GRPC status to indicate whether unhooking was successful
  grpc::Status unhook(grpc::ServerContext *context,
                      const func::UnhookRequest *request,
                      func::UnhookReply *reply) override;

  // Events are sent by the commandline via GRPC and this function 
  // figures out what type of event it is and directs it to the correct function
  // Parameters: GRPC Server context, event request (with event type name/payload), event reply
  // Return: GRPC status to indicate whether event was successfully executed
  grpc::Status event(grpc::ServerContext *context,
                     const func::EventRequest *request,
                     func::EventReply *response) override;

  // Helper function to populate map of event type names to the functions
  void PopulateMap();

  // Helper function that finds the correct function based on event type
  // Parameter: Hook request so event type can be parsed
  // Return: GRPC status to indicate whether adding the function to hook was successful
  grpc::Status AddFunc(const func::HookRequest *request);

private:
  WarbleCode wc_; 
  // Map of event name to function
  std::unordered_map<
      std::string,
      std::function<grpc::Status(WarbleCode &, const google::protobuf::Any &,
                                 google::protobuf::Any *)>>
      nametofuncmap_;
  // Map of event type to function
  std::unordered_map<int, std::function<grpc::Status(
                              WarbleCode &, const google::protobuf::Any &,
                              google::protobuf::Any *)>>
      typetofuncmap_;
};