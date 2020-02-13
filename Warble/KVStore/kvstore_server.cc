#include "kvstore.pb.h"
#include "kvstore.cc"
#include "kvstore_server.h"
#include "protobuf-3.11.2/src/google/protobuf/stubs/status.h"

  // Returns a result that indicates whether the put was successful
Status put(ServerContext* context, const PutRequest request, PutReply reply) override {
  // TODO - implement getting request and putting it in kvstore
  return Status::OK;
}

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
Status get(ServerContext* context, const stream GetRequest request, ServerWriter<GetReply>* writer) override {
	// TODO - implement gett
	// replylist should be list of values under that key
  return Status::OK;
}

  // deletes all previously stored values under that key
Status remove(ServerContext* context, const RemoveRequest request, RemoveReply reply) override {
	return Status::OK;
}
