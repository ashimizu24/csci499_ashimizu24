#include <google/protobuf/any.pb.h>
#include <google/protobuf/util/time_util.h>
#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#include "../database/kvstore_database.h"
#include "../kvstore/kvstore.grpc.pb.h"

// Wrapper class for calling the KVStore either directly or via GRPC
class KVBase {
public:
  virtual void PutRequest(const std::string key, const std::string value) = 0;
  virtual std::string GetRequest(const std::string key) = 0;
  virtual void RemoveRequest(const std::string key) = 0;
  virtual std::vector<std::string> StreamGetRequest(std::string hashtag) = 0;
  virtual void StreamPutRequest(std::vector<std::string> splited_warble_texts, 
                                std::string serialized_warble) = 0;
};

// KVStore that interacts straight with the database for testing
class KVStoreLocal : public KVBase {
public:
  void PutRequest(const std::string key, const std::string value) override {
    kvstore_.Put(key, value);
  }

  std::string GetRequest(const std::string key) override {
    return kvstore_.Get(key);
  }

  void RemoveRequest(const std::string key) override { kvstore_.Remove(key); }

  std::vector<std::string> StreamGetRequest(std::string hashtag) {
    // to be completed
  }

  void StreamPutRequest(std::vector<std::string> splited_warble_texts, std::string serialized_warble) {
    //to be completed
  }

private:
  KVStoreDb kvstore_;
};

// KVStore that interacts through GRPC to the KVstore Server
class KVStoreRemote : public KVBase {
public:
  KVStoreRemote(std::shared_ptr<grpc::Channel> channel)
      : stub_(kvstore::KeyValueStore::NewStub(channel)) {}

  void PutRequest(const std::string key, const std::string value) override {
    grpc::ClientContext context;
    kvstore::PutRequest request;
    request.set_key(key);
    request.set_value(value);
    kvstore::PutReply reply;

    grpc::Status status = stub_->put(&context, request, &reply);
  }

  std::string GetRequest(const std::string key) override {
    grpc::ClientContext context;

    std::shared_ptr<
        grpc::ClientReaderWriter<kvstore::GetRequest, kvstore::GetReply>>
        stream(stub_->get(&context));

    // add request to stream
    std::thread write_to_stream([&key, &stream]() {
      kvstore::GetRequest request;
      request.set_key(key);
      stream->Write(request);
      stream->WritesDone();
    });

    // get value from stream
    kvstore::GetReply reply;
    stream->Read(&reply);
    std::string val = reply.value();
    write_to_stream.join();
    grpc::Status status = stream->Finish();

    return val;
  }

  void RemoveRequest(const std::string key) override {
    grpc::ClientContext context;
    kvstore::RemoveRequest request;
    request.set_key(key);
    kvstore::RemoveReply reply;

    grpc::Status status = stub_->remove(&context, request, &reply);

    if (!status.ok()) {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
    }
  }

  // StreamGetRequest method is used to query warbles that 
  // contain hashtag from kvstore server.
  // Parameters: hashtag.
  // Return: A vector of warbles, which contain hashtag in their text.
  std::vector<std::string> StreamGetRequest(std::string hashtag) {
    // to be completed
  }

  // StreamPutRequest method is used to add warble to hashtag. 
  // If the text of warble contains some hashtags, then the warble will be added to 
  // corresponding vector of the hashtag.
  // Parameters: splited_warble_texts: warble_texts after spliting by space.
  //             serialized_warble: warble after serialization.
  void StreamPutRequest(std::vector<std::string> splited_warble_texts, std::string serialized_warble) {
    //to be completed
  }

private:
  std::unique_ptr<kvstore::KeyValueStore::Stub> stub_;
};