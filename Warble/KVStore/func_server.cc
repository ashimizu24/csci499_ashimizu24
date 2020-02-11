#include "func_server.h"
#include "protobuf-3.11.2/src/google/protobuf/stubs/status.h"

// Hook processes messages and calls certain event types
Status hook(ServerContext* context, const HookRequest request, HookReply reply) override 
{
  return Status::OK;
}

Status unhook(ServerContext* context, const stream UnHookRequest request, UnHookReply reply) override 
{
  return Status::OK;
}

// Where all the inputs come in 
Status event(ServerContext* context, const EventRequest request, EventReply reply) override
{
  return Status::OK;
}
