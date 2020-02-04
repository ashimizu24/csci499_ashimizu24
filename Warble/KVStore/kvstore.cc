#include <string>
#include <unordered_map>

class UserMap {
public:
	std::unordered_map<std::string,int> users;

	std::unordered_map<std::string,int> getusermap(){
		return users;
	}

	int adduser(std::string username){
		std::unordered_map<std::string,int>::iterator it = users.find(username);

		if(it != users.end()){ //username already exists
			return 1;
		}
		else{
			users.insert({username, 0});
			return 0;
		}
	}

};