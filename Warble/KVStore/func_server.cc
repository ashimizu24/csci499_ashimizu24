#include "func_server.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.h>
// Hook processes messages and calls certain event types
// hook done by operator (who's running the warble service) like warble hook is 17
grpc::Status FuncHandler::hook(grpc::ServerContext* context, const func::HookRequest* request, func::HookReply* reply)  
{
  return AddFunc(request);
}

grpc::Status FuncHandler::unhook(grpc::ServerContext* context, const func::UnhookRequest* request, func::UnhookReply* reply)  
{
	// TODO
  return grpc::Status::OK;
}

// Recieves incoming event requests from the commandline
grpc::Status FuncHandler::event(grpc::ServerContext* context, const func::EventRequest* request, func::EventReply* response)  {
	
  int eventtype = request->event_type();
  auto it2 = typetofuncmap_.find(eventtype);
  if(it2 != typetofuncmap_.end()){
    int i = it2->first;
    std::function<grpc::Status(const WarbleCode, const google::protobuf::Any&, google::protobuf::Any&)> func = it2->second;
    ///wc_.func();
    //auto x = std::bind(&func, request->payload(), *response->mutable_payload());
    //std::cout << "bind " << x << std::endl;
   // return &func(wc_request->payload(), *response->mutable_payload());
  }
	
  return grpc::Status::CANCELLED;
}

grpc::Status FuncHandler::AddFunc(const func::HookRequest* request){
  auto it2 = nametofuncmap_.find(request->event_function());
  if(it2 != nametofuncmap_.end()){
    typetofuncmap_.insert( {request->event_type(), it2->second} );

    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

void FuncHandler::PopulateMap(){
  nametofuncmap_.insert( {"Register User", &WarbleCode::CreateUser} );
  nametofuncmap_.insert( {"Create Warble", &WarbleCode::CreateWarble} );
  nametofuncmap_.insert( {"Follow User", &WarbleCode::Follow} );
  nametofuncmap_.insert( {"Read Warble", &WarbleCode::Read} );
  nametofuncmap_.insert( {"User Profile", &WarbleCode::Profile} );
  nametofuncmap_.insert( {"Create Warble Reply", &WarbleCode::CreateWarbleReply} );
}

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreRemote>(grpc::CreateChannel( "localhost:50002", grpc::InsecureChannelCredentials()));
 // std::unique_ptr<KVBase> kvstore(new KVStoreRemote(grpc::CreateChannel( "localhost:50002", grpc::InsecureChannelCredentials())));
  //KVStoreRemote kvstore(grpc::CreateChannel( "localhost:50002", grpc::InsecureChannelCredentials()))
  FuncHandler service(std::move(kvstore));
  service.PopulateMap();

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