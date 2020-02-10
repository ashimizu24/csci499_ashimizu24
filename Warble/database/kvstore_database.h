#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>

// Macros for prefixes 
#define USR_PRE "USR"
#define WARB_PRE "WAR"
#define REPLY_PRE "RE"

// Macros for warble id
#define WARBLE_CNT 0
#define USER_CNT 0

struct User {
  User(std::string username, int id){
    this->username = username;
    this->id = id;
  }

  std::string username;
  int id; //Do we need
  std::vector<std::string> followers; //or int for ids

  std::string toString()
  {
    std::string followers;
    for(const auto& user : followers){
      followers = followers + user + ",";
    }
    return username + ";" + std::to_string(id) + ";" + followers;
  }
};

struct Reply {
  Reply(int reply_id, int user_id, std::string message){
    this->reply_id = reply_id;
    this->user_id = user_id;
    this->message = message;
  }

  int reply_id;
  int user_id;
  std::string message;

  std::string toString()
  {
    return message + ";" + std::to_string(reply_id);
  }
};

struct Warble {
  Warble(int id, std::string message, std::string username){
    this->id = id;
    this->message = message;
    this->username = username;
  }

  int id;
  std::string message;
  std::string username;
  std::vector<Reply> replies;

  std::string toString()
  {
    return message + ";" + std::to_string(id);
  }
};

// Database class - holds unordered map with all warble information
// key - prefix (what type of data) + id (user id/warble id) 
// value - struct object serialized into string
class KVStoreDb {
public:
  std::unordered_map<std::string,int> GetDb(); // key - prefix (what type of data) + id (user id/warble id) 
                                               // value - struct object serialized into string

  User GetUser(std::string username);  
  void AddUser(std::string username);
  int GetUserId(std::string username);

  Warble GetWarble(int id);
  void AddWarble(std::string message, std::string username);


private:
  std::unordered_map<std::string, std::string> db; //database mapping key (prefix+id/username) to serialized struct

};

