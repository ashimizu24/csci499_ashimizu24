#include "../kvstore/warble_code.h"
#include "../kvstore/warble.pb.h"
#include <gtest/gtest.h>

using grpc::Status;

TEST(User, CreateUser) {
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

TEST(User, CreateExistingUser) {
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

class Warble : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up user so warble functionality can be tested
    warble::RegisteruserRequest newuserrequest;
    newuserrequest.set_username("Aya");
    requestpayload.PackFrom(newuserrequest);
  }

  google::protobuf::Any requestpayload;
  google::protobuf::Any replypayload;

};

TEST_F(Warble, CreateWarbleInvalidUser) {
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

TEST_F(Warble, CreateWarbleInvalidParentIdLow) {
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

TEST_F(Warble, CreateWarbleInvalidParentIdHigh) {
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

TEST_F(Warble, CreateWarble) {
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

TEST_F(Warble, CreateMultipleWarbles) {
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

TEST_F(Warble, CreateWarbleReplyChain) {
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

class WarbleReply : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up user so warble functionality can be tested
    warble::RegisteruserRequest newuserrequest;
    newuserrequest.set_username("Aya");
    requestpayload.PackFrom(newuserrequest);

    // Set up warble
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble -1");
    newwarblerequest.set_parent_id("-1");
    warblepayload.PackFrom(newwarblerequest);
  }

  google::protobuf::Any requestpayload;
  google::protobuf::Any replypayload;

  google::protobuf::Any warblepayload;
  google::protobuf::Any warblereplypayload;

  google::protobuf::Any replywarblepayload;
  google::protobuf::Any replywarblereplypayload;
};

TEST_F(WarbleReply, ReadWarble) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warblecode.CreateWarble(warblepayload, &warblereplypayload);

  warble::ReadRequest readrequest;
  readrequest.set_warble_id("0");
  replywarblepayload.PackFrom(readrequest);

  warble::ReadReply readreply;

  grpc::Status status = warblecode.Read(replywarblepayload, &replywarblereplypayload);
  bool success = replywarblereplypayload.UnpackTo(&readreply);
  EXPECT_TRUE(success);
  std::vector<warble::Warble> warbles;
  std::copy(readreply.warbles().begin(), readreply.warbles().end(),
            std::back_inserter(warbles));

  for (warble::Warble warble : warbles) {
    EXPECT_TRUE(status.ok());
    EXPECT_EQ(warble.id(), "0");
    EXPECT_EQ(warble.username(), "Aya");
    EXPECT_EQ(warble.text(), "My new warble -1");
  }
}

TEST_F(WarbleReply, ReadWarbleInvalidId) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warblecode.CreateWarble(warblepayload, &warblereplypayload);

  warble::ReadRequest readrequest;
  readrequest.set_warble_id("-2");
  replywarblepayload.PackFrom(readrequest);

  warble::ReadReply readreply;

  grpc::Status status = warblecode.Read(replywarblepayload, &replywarblereplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "Invalid warble id");
}

TEST_F(WarbleReply, ReadWarbleReplyChain) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);


  warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);
  for(int i=0; i<5; i++){
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble " + std::to_string(i));
    newwarblerequest.set_parent_id(std::to_string(i));

    google::protobuf::Any warblepayload;
    warblepayload.PackFrom(newwarblerequest);

    warble::WarbleReply warblereply;
    google::protobuf::Any warblereplypayload;

    warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);
  }

  warble::ReadRequest readrequest;
  readrequest.set_warble_id("0");
  replywarblepayload.PackFrom(readrequest);

  warble::ReadReply readreply;

  grpc::Status status = warblecode.Read(replywarblepayload, &replywarblereplypayload);
  EXPECT_TRUE(status.ok());
  bool success = replywarblereplypayload.UnpackTo(&readreply);
  EXPECT_TRUE(success);
  std::vector<warble::Warble> warbles;
  std::copy(readreply.warbles().begin(), readreply.warbles().end(),
            std::back_inserter(warbles));

  for(int i=-1; i<warbles.size()-1; i++){
    warble::Warble warble = warbles[i];
    EXPECT_EQ(warble.id(), std::to_string(i));
    EXPECT_EQ(warble.username(), "Aya");
    EXPECT_EQ(warble.text(), "My new warble " + std::to_string(i));
  }
}

TEST_F(WarbleReply, ReadWarbleReplyTree) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);
  // Create warble reply chain
  for(int i=0; i<3; i++){
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble " + std::to_string(i));
    newwarblerequest.set_parent_id(std::to_string(i));

    google::protobuf::Any warblepayload;
    warblepayload.PackFrom(newwarblerequest);

    warble::WarbleReply warblereply;
    google::protobuf::Any warblereplypayload;

    warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);
  }
  warble::WarbleRequest newwarblerequest;
  google::protobuf::Any warblepayload;

  // Create warble that replies to root 
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble 3");
  newwarblerequest.set_parent_id("0");
  warblepayload.PackFrom(newwarblerequest);
  warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);

  // Create warble that replies to 1
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble 4");
  newwarblerequest.set_parent_id("1");
  warblepayload.PackFrom(newwarblerequest);
  warblecode.CreateWarbleReply(warblepayload, &warblereplypayload);

  // Request to read warbles 
  warble::ReadRequest readrequest;
  readrequest.set_warble_id("0");
  replywarblepayload.PackFrom(readrequest);

  warble::ReadReply readreply;

  grpc::Status status = warblecode.Read(replywarblepayload, &replywarblereplypayload);
  EXPECT_TRUE(status.ok());
  bool success = replywarblereplypayload.UnpackTo(&readreply);
  EXPECT_TRUE(success);
  std::vector<warble::Warble> warbles;
  std::copy(readreply.warbles().begin(), readreply.warbles().end(),
            std::back_inserter(warbles));

  for(int i=-1; i<warbles.size()-1; i++){
    warble::Warble warble = warbles[i];
    EXPECT_EQ(warble.id(), std::to_string(i));
    EXPECT_EQ(warble.username(), "Aya");
    EXPECT_EQ(warble.text(), "My new warble " + std::to_string(i));
  }
}

class Follow : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up user so warble functionality can be tested
    warble::RegisteruserRequest newuserrequest1;
    newuserrequest1.set_username("Aya");
    requestpayload1.PackFrom(newuserrequest1);

    warble::RegisteruserRequest newuserrequest2;
    newuserrequest2.set_username("Maya");
    requestpayload2.PackFrom(newuserrequest2);
  }

  google::protobuf::Any requestpayload1;
  google::protobuf::Any requestpayload2;
  google::protobuf::Any followreplypayload;

};
 
TEST_F(Follow, FollowUser) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload1, &followreplypayload);
  warblecode.CreateUser(requestpayload2, &followreplypayload);

  warble::FollowRequest followrequest;
  followrequest.set_username("Aya");
  followrequest.set_to_follow("Maya");

  google::protobuf::Any followpayload;
  followpayload.PackFrom(followrequest);

  grpc::Status status = warblecode.Follow(followpayload, &followreplypayload);
  EXPECT_TRUE(status.ok());
}

 
TEST_F(Follow, FollowUserInvalidUsername) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload1, &followreplypayload);
  warblecode.CreateUser(requestpayload2, &followreplypayload);

  warble::FollowRequest followrequest;
  followrequest.set_username("Sarah");
  followrequest.set_to_follow("Maya");

  google::protobuf::Any followpayload;
  followpayload.PackFrom(followrequest);

  grpc::Status status = warblecode.Follow(followpayload, &followreplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "One or more of the users are invalid");
}

TEST_F(Follow, FollowUserInvalidFollower) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload1, &followreplypayload);
  warblecode.CreateUser(requestpayload2, &followreplypayload);

  warble::FollowRequest followrequest;
  followrequest.set_username("Aya");
  followrequest.set_to_follow("Sarah");

  google::protobuf::Any followpayload;
  followpayload.PackFrom(followrequest);

  grpc::Status status = warblecode.Follow(followpayload, &followreplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "One or more of the users are invalid");
}

TEST_F(Follow, FollowSameUser) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload1, &followreplypayload);
  warblecode.CreateUser(requestpayload2, &followreplypayload);

  warble::FollowRequest followrequest;
  followrequest.set_username("Aya");
  followrequest.set_to_follow("Aya");

  google::protobuf::Any followpayload;
  followpayload.PackFrom(followrequest);

  grpc::Status status = warblecode.Follow(followpayload, &followreplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "Users are the same");
}

TEST_F(Follow, FollowAlreadyFollowing) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload1, &followreplypayload);
  warblecode.CreateUser(requestpayload2, &followreplypayload);

  // Follow #1
  warble::FollowRequest followrequest;
  followrequest.set_username("Aya");
  followrequest.set_to_follow("Maya");
  google::protobuf::Any followpayload;
  followpayload.PackFrom(followrequest);
  warblecode.Follow(followpayload, &followreplypayload);

  // Follow #2
  grpc::Status status = warblecode.Follow(followpayload, &followreplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "Users are already following each other");
}
 
class Stream : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up user so warble functionality can be tested
    warble::RegisteruserRequest newuserrequest;
    newuserrequest.set_username("Aya");
    requestpayload.PackFrom(newuserrequest);

    // Set up warble
    warble::WarbleRequest newwarblerequest;
    newwarblerequest.set_username("Aya");
    newwarblerequest.set_text("My new warble -1");
    newwarblerequest.set_parent_id("-1");
    warblepayload.PackFrom(newwarblerequest);
  }

  google::protobuf::Any requestpayload;
  google::protobuf::Any replypayload;

  google::protobuf::Any warblepayload;
  google::protobuf::Any warblereplypayload;

  google::protobuf::Any replywarblepayload;
  google::protobuf::Any replywarblereplypayload;

  google::protobuf::Any streamreplypayload;
  google::protobuf::Any streamreplypayload2;

};

TEST_F(Stream, VerifyIfStreamReceiveNewWarble) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(requestpayload, &replypayload);

  // Stream warbles that contain hashtag----"warble".
  // By design, it will get vector of warbles before streaming
  // with size = prev_warbles_size
  warble::StreamRequest streamrequest;
  streamrequest.set_hashtag("warble");
  google::protobuf::Any streampayload;
  streampayload.PackFrom(streamrequest);
  warblecode.Stream(streampayload, &streamreplypayload);
  warble::StreamReply streamreply;
  streamreplypayload.UnpackTo(&streamreply);
  int prev_warbles_size = streamreply.warbles().size();
  
  // Post a new warble that contains "warble" in text.
  // Check whether stream can receive the new warble.
  warble::WarbleRequest newwarblerequest;
  newwarblerequest.set_username("Aya");
  newwarblerequest.set_text("My new warble");
  newwarblerequest.set_parent_id("-1");

  google::protobuf::Any warblepayload;
  warblepayload.PackFrom(newwarblerequest);

  warble::WarbleReply warblereply;
  google::protobuf::Any warblereplypayload;

  grpc::Status status = warblecode.CreateWarble(warblepayload, &warblereplypayload);
  warblereplypayload.UnpackTo(&warblereply);

  // Test whether stream method can receive the posted warble.
  warble::StreamRequest streamrequest2;
  streamrequest2.set_hashtag("warble");
  google::protobuf::Any streampayload2;
  streampayload2.PackFrom(streamrequest2);
  warblecode.Stream(streampayload2, &streamreplypayload2);
  warble::StreamReply streamreply2;
  streamreplypayload2.UnpackTo(&streamreply2);
  int current_warbles_size = streamreply2.warbles().size();
  ASSERT_EQ(prev_warbles_size + 1, current_warbles_size);
}

class Profile : public ::testing::Test {
protected:
  void SetUp() override {
    // Set up user so warble functionality can be tested
    warble::RegisteruserRequest newuserrequest1;
    newuserrequest1.set_username("Aya");
    newuserrequestpayload1.PackFrom(newuserrequest1);

    warble::RegisteruserRequest newuserrequest2;
    newuserrequest2.set_username("Maya");
    newuserrequestpayload2.PackFrom(newuserrequest2);

    warble::RegisteruserRequest newuserrequest3;
    newuserrequest3.set_username("Sarah");
    newuserrequestpayload3.PackFrom(newuserrequest3);

    // Set up following
    warble::FollowRequest followrequest1;
    followrequest1.set_username("Aya");
    followrequest1.set_to_follow("Maya");
    followpayload1.PackFrom(followrequest1);

    warble::FollowRequest followrequest2;
    followrequest2.set_username("Aya");
    followrequest2.set_to_follow("Sarah");
    followpayload2.PackFrom(followrequest2);

    warble::FollowRequest followrequest3;
    followrequest3.set_username("Maya");
    followrequest3.set_to_follow("Aya");
    followpayload3.PackFrom(followrequest3);
  }

  google::protobuf::Any newuserrequestpayload1;
  google::protobuf::Any newuserrequestpayload2;
  google::protobuf::Any newuserrequestpayload3;
  google::protobuf::Any newuserreplypayload;

  google::protobuf::Any followpayload1;
  google::protobuf::Any followpayload2;
  google::protobuf::Any followpayload3;
  google::protobuf::Any followreplypayload1;
  google::protobuf::Any followreplypayload2;
  google::protobuf::Any followreplypayload3;
};

TEST_F(Profile, ViewProfile) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(newuserrequestpayload1, &newuserreplypayload);
  warblecode.CreateUser(newuserrequestpayload2, &newuserreplypayload);
  warblecode.CreateUser(newuserrequestpayload3, &newuserreplypayload);
  warblecode.Follow(followpayload1, &followreplypayload1);
  warblecode.Follow(followpayload2, &followreplypayload2);
  warblecode.Follow(followpayload3, &followreplypayload3);

  warble::ProfileRequest profrequest;
  profrequest.set_username("Aya");
  google::protobuf::Any profilerequestpayload;
  profilerequestpayload.PackFrom(profrequest);

  warble::ProfileReply profreply;
  google::protobuf::Any profilereplypayload;

  grpc::Status status = warblecode.Profile(profilerequestpayload, &profilereplypayload);
  EXPECT_TRUE(status.ok());
  bool success = profilereplypayload.UnpackTo(&profreply);
  EXPECT_TRUE(success);

  std::vector<std::string> payloadfollowers;
  std::copy(profreply.followers().begin(), profreply.followers().end(),
            std::back_inserter(payloadfollowers));

  ASSERT_EQ(payloadfollowers.size(), 1);
  EXPECT_EQ(payloadfollowers[0], "Maya");

  std::vector<std::string> payloadfollowing;
  std::copy(profreply.following().begin(), profreply.following().end(),
            std::back_inserter(payloadfollowing));
  ASSERT_EQ(payloadfollowing.size(), 2);
  EXPECT_EQ(payloadfollowing[0], "Maya");
  EXPECT_EQ(payloadfollowing[1], "Sarah");
}

TEST_F(Profile, ViewProfileInvalidUsername) {
  std::unique_ptr<KVBase> kvstore = std::make_unique<KVStoreLocal>();
  WarbleCode warblecode(std::move(kvstore));
  warblecode.CreateUser(newuserrequestpayload1, &newuserreplypayload);
  warblecode.CreateUser(newuserrequestpayload2, &newuserreplypayload);
  warblecode.CreateUser(newuserrequestpayload3, &newuserreplypayload);
  warblecode.Follow(followpayload1, &followreplypayload1);
  warblecode.Follow(followpayload2, &followreplypayload2);
  warblecode.Follow(followpayload3, &followreplypayload3);

  warble::ProfileRequest profrequest;
  profrequest.set_username("Kara");
  google::protobuf::Any profilerequestpayload;
  profilerequestpayload.PackFrom(profrequest);

  warble::ProfileReply profreply;
  google::protobuf::Any profilereplypayload;

  grpc::Status status = warblecode.Profile(profilerequestpayload, &profilereplypayload);
  EXPECT_TRUE(!status.ok());
  EXPECT_EQ(status.error_message(), "User doesn't exist");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}
