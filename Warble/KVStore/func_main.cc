#include <iostream>
#include <getopt.h>
#include <string>
#include <gflags/gflags.h>

#include "func_server.h"

//to compile: g++ func_main.cc -o func_main -lgflags -pthread -lprotobuf -lpthread

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  FuncHandler service; //TODO: implement FuncHandler class

  ServerBuilder builder;
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