# How to create and maintenance backup service on restic

This repositorio is intended to guide in a basic way to use restic to backup your environment.
### Features: 

* Installation restic
* Create remote repository
* Create local folder for backups
* Configuration scripts for restic
* Understanding files
* Run script
* Logs
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

> Create folder backup

   `` mkdir /backup`` 

> Create folder for store databases files temp

  `` mkdir /backup/databases``

> Create folder for store files temp

  ``mkdir /backup/files``

### 4. Configuration scripts for restic

* This script requires 4(four) files:
1. backup.sh
2. backup-restic.inc
3. .pass
4. restic (link for binary)

* Download files

Download the latest version files `backup.sh`, `backup-restic.inc` and `.pass` from the [repo](https://github.com/prefeitura-chapeco/backup-restic). Put the file here `/backup` or here other directory conform your like.

* Create simbolic link for restic 

This script runs in the **/backup** folder so it is necessary to create a link to restic bin. Run bellow command inside the folder **/backup**.

> ``ln -s /usr/bin/restic restic``

### 5. Understanding files

* File backup.sh

This script file where you have all the instructions to back up, compress, and send the files to the remote repository. 
This file should be edited for your backup task

* File backup-restic.inc

File containing all business logic and functions that are executed by the script. this file doesn't need to be edited by default.

* File .pass

This file contains the password of the remote repository. Your access should be restricted.

* File restic link

This files is a link to the restic binary. Is necessary but script check if exist.

### 6. Run Script 

* Run Manually

To run manually runs the script from the folder ``**/backup**``. you need to give execution permission to the file ``backup.sh``.
Run command:

> ``./backup.sh`` 

* Run Automatic

Edit the crontab file and set the best time to run backup.

### 7. Logs

* Log files

This script creates a log folder and log files are stored by one file for day of the week.



