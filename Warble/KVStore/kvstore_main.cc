#include <iostream>
#include <getopt.h>
#include <string>
#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>

#include "kvstore_server.h"

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  KeyValueStoreImpl service; //TODO: implement KVStoreHandler class

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Func server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char *argv[])
{
  RunServer();
  return 0;
}