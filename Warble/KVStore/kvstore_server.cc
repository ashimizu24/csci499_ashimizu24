#include "kvstore.pb.h"
#include "kvstore.cc"
#include "kvstore_server.h"
#include "protobuf-3.11.2/src/google/protobuf/stubs/status.h"

  // Returns a result that indicates whether the put was successful
Status put(ServerContext* context, const PutRequest request, PutReply reply) override {
  // TODO - implement getting request and putting it in kvstore
  UserMap umap = new UserMap();
  umap.adduser(context.)
  return Status::OK;
}

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
Status get(ServerContext* context, const stream GetRequest request, ServerWriter<GetReply>* writer) override {
	// TODO - implement gett
	// replylist should be list of values under that key
  for (const GetReply& r : replylist) {
    if (f.location().longitude() >= left &&
      f.location().longitude() <= right &&
      f.location().latitude() >= bottom &&
      f.location().latitude() <= top) {
      writer->Write(f);
    }
  }
  return Status::OK;
}

  // deletes all previously stored values under that key
Status remove(ServerContext* context, const RemoveRequest request, RemoveReply reply) override {
	return Status::OK;
}

// Adapt this code to warble - for example right now
void RunServer(const std::string& db_path) {
  std::string server_address("0.0.0.0:50051");
  RouteGuideImpl service(db_path);

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}