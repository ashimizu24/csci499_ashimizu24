#include "kvstore_database.h"

// Insert given key and value pair into the database
void KVStoreDb::Put(const std::string key, const std::string value) {
  db_.insert({key, value});
}

// Based on a given key - get the value associated from the database
// Returns tuple - first parameter indicates whether it was found (1) or not
// found (0) If successful (1) - second parameter contains all the values under
// that key
std::string KVStoreDb::Get(const std::string key) {
  auto it = db_.find(key);

  if (it != db_.end()) {
    return it->second;
  }

  return "does not exist";
}

// Remove data from the database from a given key
void KVStoreDb::Remove(std::string key) { db_.erase(key); }