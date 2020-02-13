#include "kvstore_database.h"

#include "gtest/gtest.h"

//to compile: g++ kvstore_database_test.cc -o dbtest -lgflags -pthread -lprotobuf -lpthread

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
	KVStoreDb kvstore;
  EXPECT_EQ(kvstore.get("ayashimizu"), "ayashimizu not found");

	kvstore.put("ayashimizu", "ayashimizu data");
	EXPECT_EQ(kvstore.get("ayashimizu"), "ayashimizu data");

  kvstore.put("user2", "I am user 2");
  kvstore.put("user3", "this is user 3's data");
  kvstore.put("user4", "user 4 data");
  EXPECT_EQ(kvstore.get("user3"), "this is user 3's data");

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}