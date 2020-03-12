CSCI499 - Warble
======

### Aya Shimizu (ashimizu@usc.edu0

### Set up Folder
##### Create a new directory for project
```$ mkdir ashimizu_warble```
##### Go into directory
```$ cd ashimizu_warble```

#### Get project from Github
------
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
```$ sudo apt-get -y update```
```$ sudo apt-get -y install build-essential autoconf libtool pkg-config```
```$ curl -SL http://releases.llvm.org/9.0.0/clang+llvm-9.0.0-x86_64-linux-gnu-ubuntu-18.04.tar.xz | tar -xJC .```
```$ sudo apt-get -y install make```

#### Run the following commands to install GRPC
```$ git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc```
```$ cd grpc```
```$ git submodule update --init```
```$ make && sudo make install```
```$ cd third_party/protobuf```
```$ make && sudo make install```
###### cd out of everything

#### Install gflags
```$ sudo apt-get -y install libgflags-dev```

#### Install gtest
```$ sudo apt-get -y install libgoogle-glog-dev```

#### Install gtest
```$ sudo apt-get -y install libgtest-dev```


#### Get project from Github
------
```$ git clone https://github.com/ashimizu24/csci499_ashimizu24.git```

