#include "../kvstore/warble_code.h"
#include "../kvstore/warble.pb.h"
#include <gtest/gtest.h>

using grpc::Status;

TEST(Warble, CreateUser) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));

  warble::RegisteruserRequest newuserrequest;
  newuserrequest.set_username("Aya");
  google::protobuf::Any requestpayload;
  requestpayload.PackFrom(newuserrequest);

  google::protobuf::Any replypayload;

  grpc::Status status = warblecode.CreateUser(requestpayload, &replypayload);
  EXPECT_TRUE(status.ok());
}

TEST(Warble, CreateExistingUser) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));

  warble::RegisteruserRequest newuserrequest;
  newuserrequest.set_username("Aya");
  google::protobuf::Any requestpayload;
  requestpayload.PackFrom(newuserrequest);

  google::protobuf::Any replypayload;

  grpc::Status status_user1 = warblecode.CreateUser(requestpayload, &replypayload);
  grpc::Status status_user2 = warblecode.CreateUser(requestpayload, &replypayload);
  EXPECT_TRUE(!status_user2.ok());
}

class CreateUser : public ::testing::Test {
protected:
  void SetUp() override {
    warble::RegisteruserRequest newuserrequest;
    newuserrequest.set_username("Aya");
    requestpayload.PackFrom(newuserrequest);
  }

  google::protobuf::Any requestpayload;
  google::protobuf::Any replypayload;

};

TEST_F(CreateUser, CreateWarbleInvalidUser) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username("AyaShimizu");
  newwarblerequest.set_text("My new warble");
  newwarblerequest.set_parent_id("-1");

  google::protobuf::Any warblepayload;
  warblepayload.PackFrom(newwarblerequest);

  google::protobuf::Any warblereplypayload;

  grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "User doesn't exist");
}

TEST_F(CreateUser, CreateWarbleInvalidParentIdLow) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble");
  newwarblerequest.set_parent_id("-5");

  google::protobuf::Any warblepayload;
  warblepayload.PackFrom(newwarblerequest);

  google::protobuf::Any warblereplypayload;

  grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "Parent id is invalid");
}

TEST_F(CreateUser, CreateWarbleInvalidParentIdHigh) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble");
  newwarblerequest.set_parent_id("5");

  google::protobuf::Any warblepayload;
  warblepayload.PackFrom(newwarblerequest);

  google::protobuf::Any warblereplypayload;

  grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "Parent id is invalid");
}

TEST_F(CreateUser, CreateWarble) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble");
  newwarblerequest.set_parent_id("-1");

  google::protobuf::Any warblepayload;
  warblepayload.PackFrom(newwarblerequest);

  warble::WarbleReply warblereply;
  google::protobuf::Any warblereplypayload;

  grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
  bool success = warblereplypayload.UnpackTo(&warblereply);
  EXPECT_TRUE(success);
  warble::Warble warble = warblereply.warble();
  EXPECT_TRUE(status.ok());
  EXPECT_EQ(warble.id(), "0");
  EXPECT_EQ(warble.username(), "Aya");
  EXPECT_EQ(warble.text(), "My new warble");
}

TEST_F(CreateUser, CreateMultipleWarbles) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  for(int i=0; i<5; i++){
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble " + std::to_string(i));
    newwarblerequest.set_parent_id("-1");

    google::protobuf::Any warblepayload;
    warblepayload.PackFrom(newwarblerequest);

    warble::WarbleReply warblereply;
    google::protobuf::Any warblereplypayload;

    grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
    bool success = warblereplypayload.UnpackTo(&warblereply);
    EXPECT_TRUE(success);
    warble::Warble warble = warblereply.warble();
    EXPECT_TRUE(status.ok());
    EXPECT_EQ(warble.id(), std::to_string(i));
    EXPECT_EQ(warble.username(), "Aya");
    EXPECT_EQ(warble.text(), "My new warble " + std::to_string(i));
  }
}

TEST_F(CreateUser, CreateWarbleReplyChain) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  for(int i=-1; i<5; i++){
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble " + std::to_string(i));
    newwarblerequest.set_parent_id(std::to_string(i));

    google::protobuf::Any warblepayload;
    warblepayload.PackFrom(newwarblerequest);

    warble::WarbleReply warblereply;
    google::protobuf::Any warblereplypayload;

    grpc::Status status = warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);
    bool success = warblereplypayload.UnpackTo(&warblereply);
    EXPECT_TRUE(success);
    warble::Warble warble = warblereply.warble();
    EXPECT_TRUE(status.ok());
    EXPECT_EQ(warble.id(), std::to_string(i+1));
    EXPECT_EQ(warble.username(), "Aya");
    EXPECT_EQ(warble.text(), "My new warble " + std::to_string(i));
  }
}

 
int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}
