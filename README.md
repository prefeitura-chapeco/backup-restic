# How to create and maintenance backup service on restic

### Features: 

* Installation restic
* Create remote repository
* Create local directory for backups
* Configuration scripts for restic 
* Configuration retention snapshots 
* Learn: 
  * backup.sh
  * backup-restic.inc

## Installation Restic

### 1. Installation restic

- Debian/ubuntu:

Install package from repository official distribuition after execute command in both servers (remote and local).

> ``` sudo apt install restic ``` 

### 2. Create remote repository

* Create Repository:

On the remote server store the backups run the command below.

  * Sintaxe:

> restic -r /backup/hostname_this_your_ip_server/repository_directory init

  * Example:

> `restic -r /backup/app-web/database.restic init`  

  * Note:

> Create separate directories for databases and files. Remember that each director is a repository and can have its configuration. Sugestions is:
  * databases.restic;
  * files.restic

### 3. Create local directory for backups

* Create local directory

On local server create a directory `/backup` or the other location on this file server for store scripts and temporaly backups files.
This scripts 

* Download files

Download the latest version of scripts files `backup.sh` and `backup-restic.inc` from the [repo](https://github.com/prefeitura-chapeco/backup-restic). Put the file here `/backup` or here other directory conform your like.







Repository destination for backup's with restic
