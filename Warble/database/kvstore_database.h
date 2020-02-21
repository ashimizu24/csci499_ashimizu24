#include <string>
#include <unordered_map>
#include <iostream>
#include <functional>
#include <vector>

// Database class - holds unordered map with all warble information
// key - prefix (what type of data) + id (user id/warble id) 
// value - struct object serialized into string
class KVStoreDb {

public:
  void Put(const std::string key, const std::optional<std::vector<std::string>> value); // Insert given key and value pair into the database
  std::pair<bool, std::vector<std::string> > Get(const std::string key); // Based on a given key - get the value associated from the database
  void Remove(std::string key); // Remove data from the database from a given key

private:
  // TODO - find out do we need to be able to have duplicate keys?
  std::unordered_map<std::string, std::string> db; //database mapping key (prefix+id/username) to serialized struct
};

