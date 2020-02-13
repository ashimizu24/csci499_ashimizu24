#include "kvstore_database.h"

void KVStoreDb::KVStoreDb() {
	
}

// Insert given key and value pair into the database
void put(std::string key, std::string value)
{
  db.insert (std::make_pair<std::string, std::string>(key, value));
}

// Based on a given key - get the value associated from the database
std::string get(std::string key)
{
	// TODO check if element is not found
  try {
    db.at (key);
  }catch(const out_of_range &e) { 
    std::cout << key << " not found\n"; 
  }  

  return db.at (key);
}

// Remove data from the database from a given key
void remove(std::string key)
{
  // TODO error check - what if key doesn't exist
  db.erase (key);
}
