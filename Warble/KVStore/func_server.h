#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>

#include "func.grpc.pb.h"
#include "func.pb.h"
#include "warble_code.h"

//TODO - compile error saying KeyValueStore has not been declared

enum WarbleRequests {
  kRegisterUser = 1,
  kWarble = 2,
  kFollow = 3,
  kRead = 4,
  kProfile = 5,
  kReply = 6,
};

// Server that takes in processes requests from the commandline and redirects them to the kvstore
class FuncHandler final : public func::KeyValueStore::Service { 
public:
  // Hook processes messages and calls certain event types
  grpc::Status hook(grpc::ServerContext* context, const func::HookRequest request, func::HookReply reply);

  grpc::Status unhook(grpc::ServerContext* context, const func::UnhookRequest request, func::UnhookReply reply);

  // Where all the inputs come in 
  grpc::Status event(grpc::ServerContext* context, const func::EventRequest request, func::EventReply reply);

private:
  WarbleCode warbleserver; //instance of warble code class
};
