#include <google/protobuf/any.h>

#include "func_server.h"
#include "protobuf-3.11.2/src/google/protobuf/stubs/status.h"

// Hook processes messages and calls certain event types
// hook done by operator (who's running the warble service) like warble hook is 17
grpc::Status FuncHandler::hook(grpc::ServerContext* context, const func::HookRequest request, func::HookReply reply)  
{
	// TODO
	// get what event function it is by request.event_function();
	// where does event function get set?

  return grpc::Status::OK;
}

grpc::Status FuncHandler::unhook(grpc::ServerContext* context, const func::UnhookRequest request, func::UnhookReply reply)  
{
	// TODO
  return grpc::Status::OK;
}

// QUESTION - does this automatically get triggered when main sends the stub? or do we have to call it somewhere
grpc::Status FuncHandler::event(grpc::ServerContext* context, const func::EventRequest request, func::EventReply reply) 
{
	// TODO - all this does is take in the call and find the corresponding function call from the table and then sending
	// it to the warble code 
  switch(request.event_type()) {
	case kRegisterUser:
      warblecode::CreateUser(request.payload());
      break;
	case kWarble:
	  wwarblecode::CreateWarble();
      break;
	case kFollow:
	  warblecode::Follow();
      break;
  	case kRead:
  	  warblecode::Read();
      break;
  	case kProfile:
  	  warblecode::Profile();
      break;
  	case kReply:
  	  warblecode::CreateWarbleReply();
      break;
    default:
      return grpc::Status::NOT_FOUND;
  }
	
  return grpc::Status::OK;
}

//only sends function call 
//warble code separate class needs to KVClient
// for unit test create an abstract kvdb class and kvdb inherit
// warble code parses payload so for warble request - creates a new warble, copies data, and then gets new data
// has the increment counters and such

// has the int of type of function and message (payload)
	// .h file with the ints ofr the user
	// all this does is take 