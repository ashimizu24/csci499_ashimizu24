CSCI499 - Warble
======

### Aya Shimizu (ashimizu@usc.edu)

### Set up Folder
##### Create a new directory for project
```$ mkdir ashimizu_warble```
##### Go into directory
```$ cd ashimizu_warble```

#### Get project from Github
```$ git clone https://github.com/ashimizu24/csci499_ashimizu24.git```

### Set up VM
------
#### First download vagrant using this link
[Download Vagrant](https://www.vagrantup.com/downloads.html)
#### Then initialize the VM in terminal using this command
```
$ vagrant init hashicorp-vagrant/ubuntu-16.04
```
#### Open the Vagrantfile
##### Verfiy this line is there
```config.vm.box = "hashicorp-vagrant/ubuntu-16.04" ```
##### Create Shared Folder
Uncomment ```config.vm.synced_folder ``` in Vagrantfile </br>
Add **/vagrant_data** as shared folder name</br>
```config.vm.synced_folder ".", "/vagrant_data"```</br>
#### Install box
```$ vagrant box add hashicorp-vagrant/ubuntu-16.04```
#### Boot up Vagrant
```$ vagrant up```
#### SSH into Vagrant
```$ vagrant ssh```

### Add Configurations
------
#### Run the following commands to install make
```$ sudo apt-get -y update```</br>
```$ sudo apt-get -y install build-essential autoconf libtool pkg-config```</br>
```$ curl -SL http://releases.llvm.org/9.0.0/clang+llvm-9.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar -xJC .```</br>
```$ sudo apt-get -y install make```

#### Run the following commands to install GRPC
```$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc```</br>
```$ cd grpc```</br>
```$ git submodule update --init```</br>
```$ make && sudo make install```</br>
```$ cd third_party/protobuf```</br>
```$ make && sudo make install```</br>
###### cd out of grpc directory

#### Install gflags
```$ sudo apt-get -y install libgflags-dev```

#### Install gtest
```$ sudo apt-get -y install libgoogle-glog-dev```

#### Install gtest
```$ sudo apt-get -y install libgtest-dev```

### Get into shared folder directory
------
###### Make sure you're still SSHed into vagrant still 
```$ cd ```</br>
```$ cd /vagrant_data/ashimizu_warble/warble/kvstore```</br>

### Run Project
------
##### Compile Project
```$ make```
###### 3 terminal windows need to be open to run warble
###### Run vagrant up and vagrant ssh and cd into the above directory for each terminal window
##### Run Key Value Store Server
```$ ./kvstore_server```
##### Run Func Server
```$ ./func_server```

### Warble Commands
------
###### *Main quits after every command is run*
##### Hook functions
###### This must be run first before any warble commands are run
```$ ./main -hook```

##### Unhook functions
###### This unhooks all warble functions at any time and other commands can't be run
```$ ./main -unhook```

##### Create New User
###### After user is registered, other commands are run with -user <username>
```$ ./main -registeruser <username>```

##### Write New Warble
###### Text must be "quotation marks"
```$ ./main -user <username> -warble "<text>"```

##### Write New Warble Reply
###### Text must be "quotation marks"
###### Id is just entered as a number
```$ ./main -user <username> -warble "<text>" -reply <id>```

##### Read Warble
```$ ./main -user <username> -read <id>```

##### Follow Another User
###### Username after follow is person you want to follow
```$ ./main -user <username> -follow <username>```

##### View User Profile
###### Profile displays who the user is following and their followers
```$ ./main -user <username> -profile ```

### Run Tests
------
##### Run Key Value Store Test
```$ ./kvstore_db_test```
##### Run Warble Code Test
```$ ./warble_test```
