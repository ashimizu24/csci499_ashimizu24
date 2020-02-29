#include "func_server.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.h>
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
grpc::Status FuncHandler::event(grpc::ServerContext* context, const func::EventRequest* request, func::EventReply* response)  {
	// TODO - all this does is take in the call and find the corresponding function call from the table and then sending
	// it to the warble code 
  //std::cout << "getting event " << grpc::print(request->event_type) << std::endl;
  std::cout << request->event_type() << std::endl;

  switch(request->event_type()) {
	case kRegisterUser:
      wc_.CreateUser(request->payload());
      break;
	case kWarble:
	    wc_.CreateWarble(request->payload());
      break;
	case kFollow:
	    wc_.Follow(request->payload());
      break;
  	case kRead:
  	  wc_.Read(request->payload());
      break;
  	case kProfile:
  	  wc_.Profile(request->payload());
      break;
  	case kReply:
  	  wc_.CreateWarbleReply(request->payload());
      break;
    default:
      return grpc::Status::OK; // Change to Status::NOT_FOUND
  }
	
  return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  FuncHandler service(grpc::CreateChannel( "localhost:50000", grpc::InsecureChannelCredentials()));

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Func server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char *argv[])
{
  RunServer();
  // TODO: populate enum values hook
  return 0;
}

//only sends function call 
//warble code separate class needs to KVClient
// for unit test create an abstract kvdb class and kvdb inherit
// warble code parses payload so for warble request - creates a new warble, copies data, and then gets new data
// has the increment counters and such

// has the int of type of function and message (payload)
	// .h file with the ints ofr the user
	// all this does is take 