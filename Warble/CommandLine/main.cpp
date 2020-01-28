#include <iostream>
#include <getopt.h>
#include <string>

using namespace std;

//g++ main.cpp

int main(int argc, char *argv[]) {
  int opt= 0;
  static struct option long_options[] = {
    {"registeruser", required_argument, 0,  'r' }, //--registeruser <username> : Registers the given username
    {"user",         required_argument, 0,  'u' }, //--user <username> : Logs in as the given username
    {"warble",       required_argument, 0,  'w' }, //--warble <warble text> : Creates a new warble with the given text
    {"reply",        required_argument, 0,  'y' }, //--reply <reply warble id> : Indicates that the new warble is a reply to the given id
    {"follow",       required_argument, 0,  'f' }, //--follow <username> : Starts following the given username
    {"read",         required_argument, 0,  'd' }, //--read <warble id> : Reads the warble thread starting at the given id
    {"profile",      no_argument,       0,  'p' }, //--profile : Gets the user’s profile of following and followers
    {0,              0,                 0,   0  }
  };

  int long_index =0;
  bool user_specified = false;
  if(argc == 1){
    printf("Enter Commandline Arguments");
    printf("--registeruser <username> : Registers the given username\n");
    printf("--user <username> : Logs in as the given username\n");
    printf("The following arguments need a --user <username> ");
    printf("--warble <warble text> : Creates a new warble with the given text\n");
    printf("--reply <reply warble id> : Indicates that the new warble is a reply to the given id\n");
    printf("--follow <username> : Starts following the given username\n");
    printf("--read <warble id> : Reads the warble thread starting at the given id\n");
    printf("--profile : Gets the user’s profile of following and followers\n");
  }

  while ((opt = getopt_long(argc, argv,"r:u:w:y:f:d:p", long_options, &long_index )) != -1) {

    switch (opt) {
      case 'r' : printf("Register %s\n", optarg);
        user_specified = true;
        break;
      case 'u' : 
        if(optarg[0] == '-'){ //user's name was not inputted by user
          printf("Username argument was not entered\n");
        }
        else{
          printf("Logged in as %s\n", optarg);
          user_specified = true;
        }
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
      case ':' :
        if (optopt == 'u')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
         abort ();
    }

    //Check is user was specific (except for register user)
    if (!user_specified) {
      printf("--user <username> must be specific for each action except for --registeruser\n");
    }

  }

  return 0;
}
