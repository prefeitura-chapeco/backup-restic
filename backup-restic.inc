#!/bin/bash
# suporte.infra@chapeco.sc.gov.br
# gerenteti@chapeco.sc.gov.br
# update em  09/11/2022

# Variables used in the script

# HOST_REMOTE="IP_HOST_REMOTO"
# DIR_REMOTE="/backup/nome-do-host"

ARQ_LOG="/backup/log/`date +%w-%a`.log"
ARQ_PASS="/backup/.pass"

# Functions

function log_begin
{
  echo "[=] Backup Start in `date +%d/%m/%Y` as `date +%Hh%M`" > ${ARQ_LOG} 2>&1
}

function log_end
{
  echo "[=] Backup Finish in `date +%d/%m/%Y` as `date +%Hh%M`" >> ${ARQ_LOG} 2>&1
}

#$1=directory
function access_dir
{
 echo "[=] Access directory \"${1}\"" >> ${ARQ_LOG} 2>&1
 cd ${1}

}


# $1=dump_file, $2=user, $3=password, $4=database (if not set export all)
function dump_mysql
{
  DATABASE="--all-databases"
  [ ! -z "${4}" ] && DATABASE="${4}"

  echo "[=] Exporting Database(s) from Mysql \"${DATABASE}\" to \"${1}\"" >> ${ARQ_LOG} 2>&1
  mysqldump -u ${2} -p${3} ${DATABASE} > ${1}
}

#$1=parameters $2=compressing_files, $3=files or directory for compressing
function compress_files
{
  PARAMETERS="-czvf"
  [ ! -z "${1}" ] && PARAMETERS="-czvf"

    echo "[=] Compressing files \"${3}\" to \"${2}\"" >> ${ARQ_LOG} 2>&1
    tar ${1} ${2} ${3}
    echo "[=] Compressing finish in `date +%d/%m/%Y` at `date +%Hh%M`" >> ${ARQ_LOG} 2>&1
}

# $1=destino, $2=banco
function dump_postgres
{
  echo "[=] Exporting Database(s) from PostgreSQL \"${2}\" to \"${1}\"" >> ${ARQ_LOG} 2>&1

  sudo -u postgres pg_dump ${2} > ${1}
}

# $1=dump_file, $2=database $3=port access database
function dump_postgres_gz
{
  echo "[=] Exporting compress Database(s) from PostgreSQL  \"${2}\" to \"${1}\"" >> ${ARQ_LOG} 2>&1

  PORT=""
  [ ! -z "${3}" ] && PORT="-p ${3}"

  sudo -u postgres pg_dump ${PORT} -F d -f ${1} ${2}
}

# 1=repository 2=files_or_directory 3=retention(optional)
function send_restic
{
  RESTIC_BIN="`which restic`"
  [ -z "$RESTIC_BIN" ] && RESTIC_BIN="/backup/restic"

  RESTIC_FORGET="--keep-daily 7 --keep-monthly 1 --keep-yearly 1"
  [ ! -z "${3}" ] && RESTIC_FORGET="${3}"

  if [ -z "${DIR_REMOTO}" ]
  then
    echo "You must define: DIR_REMOTE" >> ${ARQ_LOG} 2>&1
    return -1
  fi

  if [ -z "${HOST_REMOTO}" ]
  then
    echo "You must define: HOST_REMOTE" >> ${ARQ_LOG} 2>&1
    return -1
  fi

  export RESTIC_REPOSITORY="sftp:backup@${HOST_REMOTO}:${DIR_REMOTO}/${1}"
  export RESTIC_PASSWORD_FILE="${ARQ_PASS}"

  
  echo "[=] Deleting old backup files in \"${RESTIC_REPOSITORY}\"" >> ${ARQ_LOG} 2>&1
  ${RESTIC_BIN} forget --prune ${RESTIC_FORGET} -p ${RESTIC_PASSWORD_FILE} >> ${ARQ_LOG} 2>&1
 
  echo "[=] Creating new backup file \"${2}\" in \"${RESTIC_REPOSITORY}\"" >> ${ARQ_LOG} 2>&1
  ${RESTIC_BIN} backup ${2} -p ${RESTIC_PASSWORD_FILE} >> ${ARQ_LOG} 2>&1
}

#$1=PARAMETRO $2=aquivos
function deleting_files
{
  PARAMETERS="-vrf"
  [ ! -z "${1}" ] && PARAMETERS="-vrf"

  echo "[=] Deleting temp backup file \"${2}\" after remote backup." >> ${ARQ_LOG} 2>&1

  rm ${1} ${2}

}