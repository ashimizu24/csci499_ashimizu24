#include "kvstore_server.h"

  // Returns a result that indicates whether the put was successful
grpc::Status KeyValueStoreImpl::put(grpc::ServerContext* context, const kvstore::PutRequest* request, kvstore::PutReply* reply) {
  // TODO - implement getting request and putting it in kvstore
  kvstore_.Put(request->key(), request->value());
  return grpc::Status::OK;
}

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
grpc::Status KeyValueStoreImpl::get(grpc::ServerContext* context, grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest> *stream) {
	// replylist should be list of values under that key
  kvstore::GetRequest request;
  kvstore::GetReply reply;

  while (stream->Read(&request)) {
    // Get response from database
    std::string ret = kvstore_.Get(request.key());
    reply.set_value(ret);

    stream->Write(reply);
  }
  return grpc::Status::OK;
}

  // deletes all previously stored values under that key
grpc::Status KeyValueStoreImpl::remove(grpc::ServerContext* context, const kvstore::RemoveRequest* request, kvstore::RemoveReply* reply) {
	kvstore_.Remove(request->key());
  return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("0.0.0.0:50002");
  KeyValueStoreImpl service; 

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "kvstore server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char *argv[])
{
  RunServer();
  return 0;
}