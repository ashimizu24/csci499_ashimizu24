#include "../kvstore/KVWrapper.cc"

#include <gtest/gtest.h>

// The fixture for testing class Foo.
class DBTest : public ::testing::Test {
protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  DBTest() {
    // You can do set-up work for each test here.
  }

  ~DBTest() override {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  void SetUp() override {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  void TearDown() override {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
};

TEST(Database, User) {
  KVStoreLocal kvstore;
  EXPECT_EQ(kvstore.GetRequest("ayashimizu"), "does not exist");

  kvstore.PutRequest("ayashimizu", "ayashimizu data");
  EXPECT_EQ(kvstore.GetRequest("ayashimizu"), "ayashimizu data");

  kvstore.RemoveRequest("ayashimizu");
  EXPECT_EQ(kvstore.GetRequest("ayashimizu"), "does not exist");

  kvstore.PutRequest("ayashimizu", "ayashimizu data0");
  kvstore.PutRequest("notaya", "hello");
  kvstore.PutRequest("ayashimizu3", "ayashimizu data1");

  EXPECT_EQ(kvstore.GetRequest("ayashimizu"), "ayashimizu data0");
  EXPECT_EQ(kvstore.GetRequest("notaya"), "hello");
  EXPECT_EQ(kvstore.GetRequest("ayashimizu3"), "ayashimizu data1");
  EXPECT_EQ(kvstore.GetRequest("ayashizu3"), "does not exist");

  kvstore.RemoveRequest("ayashimizu3");
  EXPECT_EQ(kvstore.GetRequest("ayashizu3"), "does not exist");
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}