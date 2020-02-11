#include <grpcpp/grpcpp.h>

#include "func.pb.h"

//TODO - compile error saying KeyValueStore has not been declared

// Server that takes in processes requests from the commandline and redirects them to the kvstore
class FuncHandler final : public KeyValueStore::Service { 
public:
  // Hook processes messages and calls certain event types
  Status hook(ServerContext* context, const HookRequest request, HookReply reply) override;

  Status unhook(ServerContext* context, const stream UnHookRequest request, UnHookReply reply) override;

  // Where all the inputs come in 
  Status event(ServerContext* context, const EventRequest request, EventReply reply) override;

private:
 
};
