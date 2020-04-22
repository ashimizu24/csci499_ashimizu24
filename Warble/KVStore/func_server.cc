#include "func_server.h"

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.h>

grpc::Status FuncHandler::hook(grpc::ServerContext *context,
                               const func::HookRequest *request,
                               func::HookReply *reply) {
  return AddFunc(request);
}

grpc::Status FuncHandler::unhook(grpc::ServerContext *context,
                                 const func::UnhookRequest *request,
                                 func::UnhookReply *reply) {
  typetofuncmap_.clear();
  return grpc::Status::OK;
}

// Recieves incoming event requests from the commandline
grpc::Status FuncHandler::event(grpc::ServerContext *context,
                                const func::EventRequest *request,
                                func::EventReply *response) {

  int eventtype = request->event_type();
  auto it2 = typetofuncmap_.find(eventtype);
  if (it2 != typetofuncmap_.end()) {
    auto &func = it2->second;
    return func(this->wc_, request->payload(), response->mutable_payload());
  }

  return grpc::Status::CANCELLED;
}

grpc::Status FuncHandler::AddFunc(const func::HookRequest *request) {
  auto it2 = nametofuncmap_.find(request->event_function());
  if (it2 != nametofuncmap_.end()) {
    typetofuncmap_.insert({request->event_type(), it2->second});

    return grpc::Status::OK;
  }
  return grpc::Status::CANCELLED;
}

void FuncHandler::PopulateMap() {
  nametofuncmap_.insert({"Register User", &WarbleCode::CreateUser});
  nametofuncmap_.insert({"Create Warble", &WarbleCode::CreateWarble});
  nametofuncmap_.insert({"Follow", &WarbleCode::Follow});
  nametofuncmap_.insert({"Read Warble", &WarbleCode::Read});
  nametofuncmap_.insert({"Profile", &WarbleCode::Profile});
  nametofuncmap_.insert(
      {"Create Warble Reply", &WarbleCode::CreateWarbleReply});
  nametofuncmap_.insert({"Stream Warble", &WarbleCode::Stream});
}

void RunServer() {
  std::string server_address("0.0.0.0:50000");
  std::unique_ptr<KVBase> kvstore =
      std::make_unique<KVStoreRemote>(grpc::CreateChannel(
          "localhost:50002", grpc::InsecureChannelCredentials()));
  FuncHandler service(std::move(kvstore));
  service.PopulateMap();

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "Func server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char *argv[]) {
  RunServer();
  return 0;
}
