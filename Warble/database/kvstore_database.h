#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

// Database class - holds unordered map with all warble information
// key - prefix (what type of data) + id (user id/warble id)
// value - struct object serialized into string
class KVStoreDb {

public:
  // Insert given key and value pair into the database
  // Parameters: string with the key and value to insert into db
  // Return: void
  void Put(const std::string key, const std::string value);

  // Getting value from database
  // Parameter: string with the key to find from db
  // Return: string with value, key is associated with
  std::string Get(const std::string key);

  // Removing key/value pair from database
  // Parameter: string with key to remove from db
  // Return: void
  void Remove(std::string key);

private:
  // database mapping key (prefix+id/username) to serialized struct
  std::unordered_map<std::string, std::string> db_;
};