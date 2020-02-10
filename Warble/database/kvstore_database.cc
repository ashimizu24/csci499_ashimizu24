#include "kvstore_database.h"

std::unordered_map<std::string,std::string> GetDb()
{
  return db;
}

// Takes in username of the user that should be added to the db
void AddUser(std::string username)
{
	User user = new User(username, USER_CNT);
	std::unordered_map<std::string,std::string> db = GetDb();
	db.insert(std::make_pair(USR_PRE + to_string(USER_CNT), user.to_string()));

	USER_CNT++;
}

// Takes in username of the user and searches in the database for user
User GetUser(std::string username)
{
	std::string key = USR_PRE + username;
	std::unordered_map<std::string,std::string> db = GetDb();
	std::unordered_map<std::string,std::string>::const_iterator it = db.find(key);

	LOG_IF(INFO, it == db.end()) << username << " not found";
   	if(it == db.end()){
   		std::cout << username << " not found\n";
   	}
   	else {
   		std::string userstring = it->second;
   		std::vector<std::string> splitstring = split(userstring, ';');
   		return User(splitstring[0], splitstring[1]);
   	}

    return null;
}

int GetUserId(std::string username)
{
	std::string key = USR_PRE + username;
	std::unordered_map<std::string,std::string> db = GetDb();
	std::unordered_map<std::string,std::string>::const_iterator it = db.find(key);

	LOG_IF(INFO, it == db.end()) << username << " not found";
   	if(it == db.end()){
   		std::cout << username << " not found\n";
   	}
   	else {
   		return it->second;
   	}

    return null;
}

void AddWarble(std::string message, std::string username)
{
  Warble warble = new Warble(WARBLE_CNT, message, username);
  std::unordered_map<std::string,std::string> db = GetDb();
  db.insert(std::make_pair(WARB_PRE + to_string(WARBLE_CNT), warble.to_string()));

  WARBLE_CNT++;
}