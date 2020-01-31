#include <iostream>

#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "kvstore.pb.h"

//Service implementations for kvstore
class KeyValueStoreImpl final : public KeyValueStore::Service {
  public:
  // rpc put (PutRequest) returns (PutReply) {}
  Status put(ServerContext* context, const PutRequest request, PutReply reply) override {
  	return Status::OK;
  }

  // rpc get (stream GetRequest) returns (stream GetReply) {}
  Status get(ServerContext* context, const stream GetRequest request, ServerWriter<GetReply>* writer) override {
  	return Status::OK;
  }

  // rpc remove (RemoveRequest) returns (RemoveReply) 
  Status remove(ServerContext* context, const RemoveRequest request, RemoveReply reply) override {
  	return Status::OK;
  }

  

}