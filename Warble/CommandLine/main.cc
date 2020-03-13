#include <getopt.h>
#include <iostream>
#include <string>
#include <gflags/gflags.h>
#include <glog/logging.h>
#include <google/protobuf/any.h>

#include "main.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

// Flag definitions
DEFINE_string(
    registeruser, "",
    "Registers the given username. Enter as -registeruser <username>");
DEFINE_string(user, "",
              "Logs in as the given username. Enter as -user <username>");
DEFINE_string(
    warble, "",
    "Creates a new warble with the given text. Enter as -warble <warble text>");
DEFINE_string(reply, "",
              "Indicates that the new warble is a reply to the given id. Enter "
              "as -reply <reply warble id>");
DEFINE_string(
    follow, "",
    "Starts following the given username. Enter as -follow <username>");
DEFINE_string(read, "",
              "Reads the warble thread starting at the given id. Enter as "
              "-read <warble id>");
DEFINE_bool(
    profile, false,
    "Gets the user’s profile of following and followers. Enter as -profile");
DEFINE_bool(hook, false, "Initializes all functions. Enter as -hook");
DEFINE_bool(unhook, false, "Uninitializes all functions. Enter as -unhook");

void FuncClient::RegisterUser(const std::string &username) {
  // Objects being passing into stub
  grpc::ClientContext context;
  func::EventRequest request;
  func::EventReply reply;

  // Warble objects
  warble::RegisteruserRequest newuserrequest;
  warble::RegisteruserReply newuserreply;

  // populate warble new user request object and pack event request
  newuserrequest.set_username(username);

  request.set_event_type(kRegisterUser);
  google::protobuf::Any payload;
  payload.PackFrom(newuserrequest);
  *request.mutable_payload() = payload;

  grpc::Status status = stub_->event(&context, request, &reply);

  if (status.ok()) {
    std::cout << "New User Created: " << username << std::endl;
  } else {
    std::cout << "User was not able to be created\n";
    std::cout << status.error_message() << std::endl;
  }
}

// If a new independent warble is created - the parent warble id will be -1
void FuncClient::CreateWarble(const std::string &username,
                              const std::string &text) {
  FuncClient::CreateWarbleReply(username, text, "-1", kReply);
}

void FuncClient::CreateWarbleReply(const std::string &username,
                                   const std::string &text,
                                   const std::string &id,
                                   const WarbleTypes type) {
  grpc::ClientContext context;
  // Create new warble request object
  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username(username);
  newwarblerequest.set_text(text);
  newwarblerequest.set_parent_id(id);

  // Pack registeruserrequest into an eventrequest payload
  func::EventRequest request;
  request.set_event_type(type);
  google::protobuf::Any payload;
  payload.PackFrom(newwarblerequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC
  func::EventReply reply;
  warble::WarbleReply warblereply;

  grpc::Status status = stub_->event(&context, request, &reply);
  if(status.ok()) {
    if(reply.payload().UnpackTo(&warblereply)) {
      warble::Warble warble = warblereply.warble();
      std::cout << "New Warble Created!\n";
      std::cout << "ID: " << warble.id() << std::endl;
      std::cout << "Author: " << warble.username() << std::endl;
      std::cout << "Time Posted: " << warble.timestamp().seconds() << std::endl;
      std::cout << "Text: " << warble.text() << std::endl;
    }
  } else {
    std::cout << "Warble was not able to be created\n";
    std::cout << status.error_message() << std::endl;
  }
}

void FuncClient::Read(const std::string &warbleid) {
  grpc::ClientContext context;

  warble::ReadRequest readrequest;
  readrequest.set_warble_id(warbleid);

  // Pack readrequest into an eventrequest payload
  func::EventRequest request;
  request.set_event_type(kRead);
  google::protobuf::Any payload;
  payload.PackFrom(readrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC
  func::EventReply reply;
  warble::ReadReply readreply;

  grpc::Status status = stub_->event(&context, request, &reply);

  if(status.ok()) {
    if(reply.payload().UnpackTo(&readreply)) {
      std::vector<warble::Warble> warbles;
      std::copy(readreply.warbles().begin(), readreply.warbles().end(),
                std::back_inserter(warbles));

      for (warble::Warble warble : warbles) {
        std::cout << "WARBLE " << warble.id() << std::endl;
        if(warble.parent_id().compare("-1") != 0){
          std::cout << "Replying to Warble " << warble.parent_id() << std::endl;
        }
        std::cout << "[" << warble.timestamp().seconds() << "] Author: " << warble.username() << std::endl;
        std::cout << warble.text() << std::endl;
        std::cout << "---------------------------" << std::endl;
      }
    }
  } else {
    std::cout << "Warble was not able to be read\n";
    std::cout << status.error_message() << std::endl;
  }
}

void FuncClient::Follow(const std::string &username,
                        const std::string &usernametofollow) {
  grpc::ClientContext context;

  warble::FollowRequest followrequest;
  followrequest.set_username(username);
  followrequest.set_to_follow(usernametofollow);

  // Pack followrequest into an eventrequest payload
  func::EventRequest request;
  request.set_event_type(kFollow);
  google::protobuf::Any payload;
  payload.PackFrom(followrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC
  func::EventReply reply;
  warble::FollowReply followreply;

  grpc::Status status = stub_->event(&context, request, &reply);
  if (status.ok()) {
    std::cout << username << " followed " << usernametofollow << std::endl;
  } else {
    std::cout << "Follow request was not able to be made\n";
    std::cout << status.error_message() << std::endl;
  }
}

void FuncClient::Profile(const std::string &username) {
  grpc::ClientContext context;

  warble::ProfileRequest profrequest;
  profrequest.set_username(username);

  // Pack readrequest into an eventrequest payload
  func::EventRequest request;
  request.set_event_type(kProfile);
  google::protobuf::Any payload;
  payload.PackFrom(profrequest);
  *request.mutable_payload() = payload;

  // Unpack response from GRPC
  func::EventReply reply;
  warble::ProfileReply profreply;
  grpc::Status status = stub_->event(&context, request, &reply);

  if (status.ok()) {
    if (reply.payload().UnpackTo(&profreply)) {
      std::cout << username << "'s Profile\n";
      // Get and print followers
      std::vector<std::string> followers;
      std::copy(profreply.followers().begin(), profreply.followers().end(),
                std::back_inserter(followers));
      std::cout << username << "Followers: ";
      for (std::string follower : followers) {
        std::cout << follower << std::endl;
      }
      std::cout << "\n";
      // Get and print following
      std::vector<std::string> following;
      std::copy(profreply.following().begin(), profreply.following().end(),
                std::back_inserter(following));
      std::cout << "Following: ";
      for (std::string following : following) {
        std::cout << following << std::endl;
      }
      std::cout << "\n\n";
    }
  } else {
    std::cout << "Profile was not able to be viewed\n";
    std::cout << status.error_message() << std::endl;
  }
}

void FuncClient::HookEvents() {
  typemap_.insert({"Register User", kRegisterUser});
  typemap_.insert({"Create Warble", kWarble});
  typemap_.insert({"Follow", kFollow});
  typemap_.insert({"Read Warble", kRead});
  typemap_.insert({"Profile", kProfile});
  typemap_.insert({"Create Warble Reply", kReply});

  grpc::Status status;

  for (auto it = typemap_.begin(); it != typemap_.end(); ++it) {
    grpc::ClientContext context;
    func::HookRequest request;
    func::HookReply reply;
    request.set_event_type(it->second);
    request.set_event_function(it->first);
    grpc::Status status = stub_->hook(&context, request, &reply);
  }

  if(status.ok()) {
    std::cout << "All events were hooked successfully\n";
  }
  else {
    LOG(WARNING) << "Hooking events was unsuccessful\n";
    std::cout << "Hooking events was unsuccessful\n";
  }

}

void FuncClient::UnhookEvents() {
  typemap_.insert({"Register User", kRegisterUser});
  typemap_.insert({"Create Warble", kWarble});
  typemap_.insert({"Follow", kFollow});
  typemap_.insert({"Read Warble", kRead});
  typemap_.insert({"Profile", kProfile});
  typemap_.insert({"Create Warble Reply", kReply});
  grpc::Status status = grpc::Status::CANCELLED;

  for (auto it = typemap_.begin(); it != typemap_.end(); ++it) {
    grpc::ClientContext context;
    func::UnhookRequest request;
    func::UnhookReply reply;
    request.set_event_type(it->second);
    status = stub_->unhook(&context, request, &reply);
  }

  if(status.ok()) {
    std::cout << "All events were unhooked successfully\n";
  }
  else {
    std::cout << "Unhooking events was unsuccessful\n";
  }
}

int main(int argc, char *argv[]) {
  FuncClient func_client(grpc::CreateChannel(
      "localhost:50000", grpc::InsecureChannelCredentials()));

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  // Initialize Google's logging library.
  google::InitGoogleLogging(argv[0]);

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  // Hook all functions call
  if (FLAGS_hook) {
    func_client.HookEvents();
    return 1;
  }

  // Unhook all functions call
  if (FLAGS_unhook) {
    func_client.UnhookEvents();
    return 1;
  }

  /// Error check - user was not defined for flags that require it (all except
  /// registeruser)
  if (FLAGS_user.empty() && FLAGS_registeruser.empty()) {
    LOG(INFO) << "User not defined\n";
    return 1;
  }

  // If registeruser flag triggered with argument - add user
  if (!FLAGS_registeruser.empty()) {
    func_client.RegisterUser(FLAGS_registeruser);
  } else if (!FLAGS_warble.empty() && FLAGS_reply.empty()) {
    func_client.CreateWarble(FLAGS_user, FLAGS_warble);
  } else if (!FLAGS_warble.empty() && !FLAGS_reply.empty()) {
    func_client.CreateWarbleReply(FLAGS_user, FLAGS_warble, FLAGS_reply,
                                  kWarble);
  } else if (!FLAGS_read.empty()) {
    func_client.Read(FLAGS_read);
  } else if (FLAGS_profile) {
    func_client.Profile(FLAGS_user);
  } else if (!FLAGS_follow.empty()) {
    func_client.Follow(FLAGS_user, FLAGS_follow);
  }

  google::protobuf::ShutdownProtobufLibrary();
  return 0;
}
