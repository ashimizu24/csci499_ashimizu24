#include <grpcpp/grpcpp.h>

#include "kvstore.pb.h"
#include "kvstore.grpc.pb.h"
#include "warble_code.h"
#include "protobuf-3.11.2/src/google/protobuf/stubs/status.h"

//Service implementations for kvstore
class KeyValueStoreImpl final : public kvstore::KeyValueStore::Service {
  public:
  // Returns a result that indicates whether the put was successful
  grpc::Status put(grpc::ServerContext* context, const kvstore::PutRequest request, kvstore::PutReply reply);

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
  grpc::Status get(grpc::ServerContext* context, const kvstore::GetRequest request, grpc::ServerWriter<kvstore::GetReply>* writer);

  // deletes all previously stored values under that key
  grpc::Status remove(grpc::ServerContext* context, const kvstore::RemoveRequest request, kvstore::RemoveReply reply);
 
};