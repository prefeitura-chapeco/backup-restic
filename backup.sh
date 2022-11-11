#!/bin/bash
# suporte.infra@chapeco.sc.gov.br
# gerenteti@chapeco.sc.gov.br
# update in 05/11/2022
# version 1.0

#VARIABLES

# Host remote that will store the backups
HOST_REMOTE="MY_IP_OR_HOSTNAME"

# Remote folder store snapshots backups
# The folder mentions the server to make it easier
# example /backup/test
DIR_REMOTE="/backup/test"

# Directory or folders for the operation and sending of the backup to the local server
DIR_FILES="/backup/files/"
DIR_BANCO="/backup/database/"

# Variables to facilitate file manipulation. You create and define
ETC_FILES_GZ="etc_files.tar.gz"
ARQ_BANCO_SQL="dump_db_test.sql"
PSQL_DUMP="dump_postgres.sql"
ARQ_BANCO_GZ="dump_db_test.tar.gz"

# Statement file for script where it has all functions declared
. /backup/backup-restic.inc

log_begin

## ACCESS FOLDERS ##
# call the function to access the $DIR_FILES
## INSTRUCTIONS ##
# access_dir path_to_folder_or_variable
access_dir ${DIR_FILES}

## COMPRESS FILES ##
# Compress files to send repository
## INSTRUCTIONS ##
# compress_files -zcvf file_name_or_variable_declared file_or_folder_for_compress
compress_files -zcvf ${ETC_FILES_GZ} /etc

## SEND REPOSITORY BACKUP ##
# Send the backup to the remote repository
## INSTRUCTIONS ##
# send_restic repository_remote files_or_variable
send_restic files.restic "/backup/backup.sh"
send_restic files.restic "/backup/backup-restic.inc"
send_restic files.restic ${ETC_FILES_GZ}

## DELETING TEMP FILES ##
# Deleting temp files after send to remote repository
## INSTRUCTIONS ##
# deleting_files options file_name_or_variable
deleting_files -vrf ${ETC_FILES_GZ}

## MANIPULATING DATABASES STEP ##

## ACCESS FOLDERS ##
access_dir ${DIR_BANCO}

## DUMP FOR MYSQL DATABASES ##
# Create dump file for all databases or specific database
## INSTRUCTIONS ##
# dump_mysql file_or_variable_to_dump username_access_db password_access_db db_name(optional databases or export all)
# Export all databases
dump_mysql ${ARQ_BANCO_SQL} user_test password_test
# Export specific datadase
dump_mysql ${ARQ_BANCO_SQL} user_test password_test my_database

## DUMP FOR POSTGRESQL DATABASES COMPRESS RECOMMEMDED ##
# Create dump file for specific database. 
# This script use is command sudo -u postgres for access to databases for other user adjust script
## INSTRUCTIONS ##
# dump_postgres_gz file_or_variable_to_dump "db_name" port_access (optional if port defaul not declare)
# Export database for default port
dump_postgres_gz ${PSQL_DUMP} "db_name"
#Exporta database for other port running on server
dump_postgres_gz ${PSQL_DUMP} "db_name" 5433

## COMPRESS FILES ##
compress_files -zcvf ${PSQL_DUMP_GZ} ${PSQL_DUMP}

## SEND REPOSITORY BACKUP ##
send_restic banco.restic ${PSQL_DUMP_GZ}

## DELETING TEMP FILES ##
deleting_files -vrf ${PSQL_DUMP_GZ}

log_end