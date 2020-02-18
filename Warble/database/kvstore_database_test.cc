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
  EXPECT_EQ(kvstore.Get("ayashimizu").first, 0);

	kvstore.Put("ayashimizu", "ayashimizu data");
	EXPECT_EQ(kvstore.Get("ayashimizu").first, 1);
  EXPECT_EQ(kvstore.Get("ayashimizu").second.size(), 1);

  for(int i=0; i<kvstore.Get("ayashimizu").second.size(); i++) {
    EXPECT_EQ(kvstore.Get("ayashimizu").second.at(i), "ayashimizu data");
  }

  kvstore.Remove("ayashimizu");
  EXPECT_EQ(kvstore.Get("ayashimizu").first, 0);

  kvstore.Put("ayashimizu", "ayashimizu data0");
  kvstore.Put("notaya", "hello");
  kvstore.Put("ayashimizu", "ayashimizu data1");

  EXPECT_EQ(kvstore.Get("ayashimizu").second.size(), 2);

  for(int i=0; i<kvstore.Get("ayashimizu").second.size(); i++) {
    EXPECT_EQ(kvstore.Get("ayashimizu").second.at(i), "ayashimizu data" + std::to_string(i));
  }
  

  // kvstore.Put("user2", "I am user 2");
  // kvstore.Put("user3", "this is user 3's data");
  // kvstore.Put("user4", "user 4 data");
  // EXPECT_EQ(kvstore.Get("user3"), "this is user 3's data");

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}