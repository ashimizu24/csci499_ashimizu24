#include "kvstore_server.h"

DEFINE_string(store, "",
              "Reads in data from file to store in the kvstore. Enter as "
              "-store <filename>");

grpc::Status KeyValueStoreImpl::put(grpc::ServerContext *context,
                                    const kvstore::PutRequest *request,
                                    kvstore::PutReply *reply) {
  kvstore_.Put(request->key(), request->value());

  if(storage_){
    std::ofstream outfile;
    outfile.open(storagefile_, std::ios_base::app);

    std::string warbprefix("WAR");
    auto res = std::mismatch(warbprefix.begin(), warbprefix.end(), request->key().begin());
    if (res.first == warbprefix.end()) {
      outfile << request->key() << "\n" << request->value() << "\n";
    }
    else{
      outfile << request->key() << " " << request->value() << "\n";
    }
    
  }
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreImpl::get(
    grpc::ServerContext *context,
    grpc::ServerReaderWriter<kvstore::GetReply, kvstore::GetRequest> *stream) {
  // replylist should be list of values under that key
  kvstore::GetRequest request;
  kvstore::GetReply reply;

  while (stream->Read(&request)) {
    // Get response from database
    std::string ret = kvstore_.Get(request.key());
    reply.set_value(ret);

    stream->Write(reply);
  }
  return grpc::Status::OK;
}

grpc::Status KeyValueStoreImpl::remove(grpc::ServerContext *context,
                                       const kvstore::RemoveRequest *request,
                                       kvstore::RemoveReply *reply) {
  kvstore_.Remove(request->key());
  return grpc::Status::OK;

}

void KeyValueStoreImpl::FileStorage(std::string filename) {
  std::ifstream file(filename);
  std::string line;
  storagefile_ = filename;
  storage_ = true;
  std::string warbprefix("WAR");

    // file is in the format key value
  if(!file.fail()) {
    if(file.is_open()) {
      while(std::getline(file, line)) {
          std::istringstream ss(line);
          std::string token;
          std::getline(ss, token, ' ');
          std::string key = token;
          std::string value;

          // if key is warble, then extra line to get value
          auto res = std::mismatch(warbprefix.begin(), warbprefix.end(), key.begin());
          if (res.first == warbprefix.end()) {
            std::getline(file, line);
            std::getline(file, line);
            value = "\n" + line;
          }
          else {
            std::getline(ss, token);
            value = token;
          }
          
          kvstore_.Put(key, value);
      }
        file.close();
    }
  }
}

void RunServer(std::string filename, bool storage) {
  std::string server_address("0.0.0.0:50002");
  KeyValueStoreImpl service;

  grpc::ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  std::cout << "kvstore server listening on " << server_address << std::endl;

  if(storage){
    service.FileStorage(filename);
  }

  server->Wait();
}

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::string filename = "";
  bool storage = false;

  // Specifies storage file
  if (!FLAGS_store.empty()) {
    filename = FLAGS_store;
    storage = true;
  }

  RunServer(filename, storage);
  return 0;
}