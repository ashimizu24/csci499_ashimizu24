#include <grpcpp/grpcpp.h>

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
  
  // Function that calls kvstore to get serialized warbles from db
  // Parameters: GRPC server context, GetStream request, GetStream reply
  // Return: GRPC status indicating correctness of getting warbles.
  grpc::Status StreamGet(grpc::ServerContext *context,
                         const kvstore::GetStreamRequest *request,
                         kvstore::GetStreamReply *reply) override;
  
  // Function that calls kvstore to put serialized warbles to db
  // if the word is a hashtag, then kvstore_ will put 
  // serialized warble to db.
  // Parameters: GRPC server context, PutStream request, PutStream reply
  // Return: GRPC status indicating whether put process is successful.
  grpc::Status StreamPut(grpc::ServerContext *context,
                         const kvstore::PutStreamRequest *request,
                         kvstore::PutStreamReply *reply) override;

private:
  KVStoreDb kvstore_;
};