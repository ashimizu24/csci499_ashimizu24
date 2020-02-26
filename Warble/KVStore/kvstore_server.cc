#include "kvstore_server.h"

  // Returns a result that indicates whether the put was successful
grpc::Status KeyValueStoreImpl::put(grpc::ServerContext* context, const kvstore::PutRequest request, kvstore::PutReply reply) {
  // TODO - implement getting request and putting it in kvstore
  return grpc::Status::OK;
}

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
grpc::Status KeyValueStoreImpl::get(grpc::ServerContext* context, const kvstore::GetRequest request, grpc::ServerWriter<kvstore::GetReply>* writer) {
	// TODO - implement gett
	// replylist should be list of values under that key
  return grpc::Status::OK;
}

  // deletes all previously stored values under that key
grpc::Status KeyValueStoreImpl::remove(grpc::ServerContext* context, const kvstore::RemoveRequest request, kvstore::RemoveReply reply) {
	return grpc::Status::OK;
}

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  KeyValueStoreImpl service; //TODO: implement KVStoreHandler class

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
  return 0;
}