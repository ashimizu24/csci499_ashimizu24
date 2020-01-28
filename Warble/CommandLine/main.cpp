#include <iostream>
#include <getopt.h>
#include <string>

using namespace std;

//g++ main.cpp

int main(int argc, char *argv[]) {
  int opt= 0;
  static struct option long_options[] = {
    {"registeruser", required_argument, 0,  'r' },
    {"user",         required_argument, 0,  'u' },
    {"warble",       required_argument, 0,  'w' },
    {"reply",        required_argument, 0,  'y' },
    {"follow",       required_argument, 0,  'f' },
    {"read",         required_argument, 0,  'd' },
    {"profile",      no_argument,       0,  'p' },
    {0,              0,                 0,   0  }
  };

  int long_index =0;
  bool user_specified = false;
  while ((opt = getopt_long(argc, argv,"r:u:w:y:f:d:p", long_options, &long_index )) != -1) {

    switch (opt) {
      case 'r' : printf("Register %s\n", optarg);
        user_specified = true;
        break;
      case 'u' : printf("Logged in as %s\n", optarg);
        user_specified = true;
        break;
      case 'w' : 
        if(user_specified){
          printf("Create new Warble '%s'\n", optarg);
        }
        break;
      case 'y' : 
        if(user_specified){
          printf("Replying to Warble %s\n", optarg);
        }
        break;
      case 'f' : 
        if(user_specified){
          printf("Following user %s\n", optarg);
        }
        break;
      case 'd' : 
        if(user_specified){
          printf("Reading Warble thread %s\n", optarg);
        }
        break;
      case 'p' : 
        if(user_specified){
          printf("profile\n");
        }
        break;
      case ':':
        /* missing option argument */
        printf("HI %c", optopt);
        //printf("%s: option '-%c' requires an argument\n", argv[0], optopt);
        std::string flag(sizeof(optopt), optopt);
       
        if(flag.compare("--user")){
          printf("user not specified");
          user_specified = false;
        }
        break;
    }

    //Check is user was specific (except for register user)
  if (!user_specified) {
    printf("--user <username> must be specific for each action except for --registeruser\n");
  }

    
  }

  return 0;
}
