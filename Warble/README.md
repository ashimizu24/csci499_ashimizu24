CSCI499 - Warble
======

### Aya Shimizu - ashimizu@usc.edu

Install VM
------

[Download Vagrant](https://www.vagrantup.com/downloads.html)
```
$ vagrant init hashicorp-vagrant/ubuntu-16.04
```
Create Shared Folder
------
Uncomment 
```config.vm.synced_folder ```
Add **/vagrant_data** as shared folder name
```config.vm.synced_folder ".", "/vagrant_data"```
