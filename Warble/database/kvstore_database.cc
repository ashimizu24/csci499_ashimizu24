#include "kvstore_database.h"

// Insert given key and value pair into the database
void KVStoreDb::put(std::string key, std::string value) {
  db.insert ( {key, value} );
}

// Based on a given key - get the value associated from the database
std::string KVStoreDb::get(std::string key) {
	// TODO check if element is not found
  try {
    db.at (key);
  }catch(const std::out_of_range &e) { 
    std::cout << key << " not found\n"; 
  }  

  return db.at (key);
}

// Remove data from the database from a given key
void KVStoreDb::remove(std::string key) {
  // TODO error check - what if key doesn't exist
  db.erase (key);
}

