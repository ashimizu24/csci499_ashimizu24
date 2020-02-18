#include "kvstore_database.h"

// Insert given key and value pair into the database
void KVStoreDb::Put(const std::string key, const std::string value) {
  db.insert ( {key, value} );
}

// Based on a given key - get the value associated from the database
// Returns tuple - first parameter indicates whether it was successful (1) or not found (0)
// If successful (1) - second parameter contains all the values under that key
std::pair<bool, std::vector<std::string> > KVStoreDb::Get(const std::string key) {
  std::vector<std::string> values;
  bool success = 0;

  for (auto it = db.begin(); it != db.end(); it++) {
    if (it -> first == key) {
      std::string value = it->second;
      values.push_back(value);
      success = 1;
    }
  } 

  return std::make_pair (success, values);
}

// Remove data from the database from a given key
void KVStoreDb::Remove(std::string key) {
  // TODO: error check - what if key doesn't exist
  db.erase (key);
}

