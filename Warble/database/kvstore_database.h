#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

// Database class - holds unordered map with all warble information
// key - prefix (what type of data) + id (user id/warble id) 
// value - struct object serialized into string
class KVStoreDb {
public:
  std::unordered_map<std::string,std::string> GetDb(); // key - prefix (what type of data) + id (user id/warble id) 
                                               // value - struct object serialized into string

private:
  std::unordered_map<std::string, std::string> db; //database mapping key (prefix+id/username) to serialized struct
};

