# How to create and maintenance backup service on restic

### Features: 

* Installation restic
* Create remote repository
* Create local folder for backups
* Configuration scripts for restic
* Understanding files
* Run task (cron)
* Learn: 
  * backup.sh
  * backup-restic.inc

## Installation Restic

### 1. Installation restic

* Debian/ubuntu:

Install package from repository official distribuition after execute command in both servers (remote and local).

> `` sudo apt install restic ``

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

### 3. Create local folder for backups

* Create folder

On local server create a directory `/backup` or the other location on this file server for store scripts and temporaly backups files.
This script is configured for the following directory: databases, files, log. 

Run commands:

> `` mkdir /backup`` 
> `` mkdir /backup/databases``
> ``mkdir /backup/files``

### 4. Configuration scripts for restic

* This script requires 4 files:
1. backup.sh
2. backup-restic.inc
3. .pass
4. restic (link for binary)

* Download files

Download the latest version files `backup.sh`, `backup-restic.inc` and `.pass` from the [repo](https://github.com/prefeitura-chapeco/backup-restic). Put the file here `/backup` or here other directory conform your like.

* Create simbolic link for restic 

This script runs in the **/backup** folder so it is necessary to create a link to restic bin. Run bellow command inside the folder backup.

> ``ln -s /usr/bin/restic restic

### 5. Understanding files



