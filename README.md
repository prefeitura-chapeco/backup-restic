# How to create and maintenance backup service on restic

### Features: 

- Installation restic
- Create remote repository
- Create local directory for backups
- Configuration scripts for restic 
- Configuration retention snapshots 
- Learn: 
  - backup.sh
  - backup-restic.inc

## Installation Restic

### 1. Installation restic

- Debian/ubuntu:

Install package from repository official distribuition after execute command in both servers (remote and local).

``sudo apt install restic`` 

### 2. Create remote repository

- Create Repository:

On the remote server store the backups run the command below.

> restic -r ***DIRECTORY_STORE_BACKUPS*** init
> restic -r /backup/your_directory/repository_directory init

PS.: create your directory before 

Code:

`` restic -r /backup/your_directory/


PS.: My sugestion is create directory 





Repository destination for backup's with restic
