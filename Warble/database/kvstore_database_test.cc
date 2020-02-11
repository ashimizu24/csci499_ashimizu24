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
	UserMap* map;
  EXPECT_EQ(map->GetUserId("ayashimizu"), "ayashimizu not found");

	map->AddUser("ayashimizu");
	EXPECT_EQ(map->GetUserId("ayashimizu"), 0);

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout << "RUNNING DB TESTS\n";
  return RUN_ALL_TESTS();
}