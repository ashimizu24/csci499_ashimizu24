#include <iostream>
#include <grpcpp/grpcpp.h>
#include <grpc/grpc.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <grpcpp/security/server_credentials.h>
#include <google/protobuf/any.pb.h>
#include <google/protobuf/util/time_util.h>
#include <thread>
#include <sstream>

#include "../kvstore/kvstore.grpc.pb.h"
#include "../database/kvstore_database.h"

class KVBase{
public:
  virtual void PutRequest(const std::string key, const std::string value) = 0;
  virtual std::string GetRequest(const std::string key) = 0;
  virtual void RemoveRequest(const std::string key) = 0;
};

// KVStore that interacts straight with the database
class KVStoreLocal : public KVBase { 
public:
  void PutRequest(const std::string key, const std::string value) override {
    kvstore_.Put(key, value);
  }

  std::string GetRequest(const std::string key) override {
    return kvstore_.Get(key);
  }

  void RemoveRequest(const std::string key) override {
    kvstore_.Remove(key);
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

  	std::shared_ptr<grpc::ClientReaderWriter<kvstore::GetRequest, kvstore::GetReply>> stream(stub_->get(&context));

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
  
    if(!status.ok()) {
      std::cout << status.error_code() << ": " << status.error_message() << std::endl; 
    }
  }

private:
  std::unique_ptr<kvstore::KeyValueStore::Stub> stub_;
};