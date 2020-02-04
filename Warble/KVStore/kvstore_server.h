#include "kvstore.pb.h"

//Service implementations for kvstore
class KeyValueStoreImpl final : public KeyValueStore::Service {
  public:
  // Returns a result that indicates whether the put was successful
  Status put(ServerContext* context, const PutRequest request, PutReply reply) override;

  // Returns a previously stored value or values under that key or nothing if the key is not present in the store
  Status get(ServerContext* context, const stream GetRequest request, ServerWriter<GetReply>* writer) override;

  // deletes all previously stored values under that key
  Status remove(ServerContext* context, const RemoveRequest request, RemoveReply reply) override;
  

};