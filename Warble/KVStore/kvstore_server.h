#include <grpcpp/grpcpp.h>
#include <gflags/gflags.h>
#include <fstream>
#include <sstream>

#include "../database/kvstore_database.h"
#include "kvstore.grpc.pb.h"
#include "kvstore.pb.h"

using grpc::Status;

// Service implementations for kvstore
class KeyValueStoreImpl final : public kvstore::KeyValueStore::Service {

public:
  // Function that calls kvstore to put key/value pair in db
  // Parameters: GRPC server context, put request (key and value payload)
  // of what to put into db, put reply
  /// Return: GRPC status indicating if putting value in db was successful
  grpc::Status put(grpc::ServerContext *context,
                   const kvstore::PutRequest *request,
                   kvstore::PutReply *reply) override;

  // Function that calls kvstore to get a value from db based on key
  // Parameters: GRPC server context, get request, get reply
  /// Return: GRPC status indicating if getting value in db was successful
  grpc::Status
  get(grpc::ServerContext *context,
      grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest> *stream)
      override;

  // Function that calls kvstore to remove value from db based on key
  // Parameters: GRPC server context, remove request, remove reply
  // Return: GRPC status indicating if removing value from db was successful
  grpc::Status remove(grpc::ServerContext *context,
                      const kvstore::RemoveRequest *request,
                      kvstore::RemoveReply *reply) override;

  void FileStorage(const std::string filename);

private:
  KVStoreDb kvstore_;
  std::string storagefile_;
  bool storage_;
};