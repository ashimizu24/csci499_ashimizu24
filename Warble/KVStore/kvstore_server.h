#include <grpcpp/grpcpp.h>

#include "../database/kvstore_database.h"
#include "kvstore.grpc.pb.h"
#include "kvstore.pb.h"

using grpc::Status;

// Service implementations for kvstore
class KeyValueStoreImpl final : public kvstore::KeyValueStore::Service {

public:
  // Returns a result that indicates whether the put was successful
  grpc::Status put(grpc::ServerContext *context,
                   const kvstore::PutRequest *request,
                   kvstore::PutReply *reply) override;

  // Returns a previously stored value or values under that key or nothing if
  // the key is not present in the store
  grpc::Status
  get(grpc::ServerContext *context,
      grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest> *stream)
      override;

  // deletes all previously stored values under that key

  grpc::Status remove(grpc::ServerContext *context,
                      const kvstore::RemoveRequest *request,
                      kvstore::RemoveReply *reply) override;

private:
  KVStoreDb kvstore_;
};