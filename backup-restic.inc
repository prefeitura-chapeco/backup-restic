#!/bin/bash
# suporte.infra@chapeco.sc.gov.br
# gerenteti@chapeco.sc.gov.br
# update em  09/11/2022

# Variaveis usadas no script

# HOST_REMOTO="IP_HOST_REMOTO"
# DIR_REMOTO="/backup/nome-do-host"

ARQ_LOG="/backup/log/`date +%w-%a`.log"
ARQ_PASS="/backup/.pass"

# Funcoes de backup

function log_inicio
{
  echo "[=] Backup Iniciado em `date +%d/%m/%Y` as `date +%Hh%M`" > ${ARQ_LOG} 2>&1
}

function log_fim
{
  echo "[=] Backup Finalizado em `date +%d/%m/%Y` as `date +%Hh%M`" >> ${ARQ_LOG} 2>&1
}

#$1=diretorio
function access_dir
{
 echo "[=] Acessando diretorio \"${1}\"" >> ${ARQ_LOG} 2>&1
 cd ${1}

}


# $1=destino, $2=usuario, $3=senha, $4=banco (se nao indicar, exporta todos os bancos)
function dump_mysql
{
  BANCO="--all-databases"
  [ ! -z "${4}" ] && BANCO="${4}"

  echo "[=] Exportando Banco Mysql \"${BANCO}\" para \"${1}\"" >> ${ARQ_LOG} 2>&1
  mysqldump -u ${2} -p${3} ${BANCO} > ${1}
}

#$1=parametros $2=arquivo_compactado, $3=arquivo original
function compactar_arquivos
{
  PARAMETROS="-czvf"
  [ ! -z "${1}" ] && PARAMETROS="-czvf"

    echo "[=] Compactando Arquivos \"${3}\" para \"${2}\"" >> ${ARQ_LOG} 2>&1
    tar ${1} ${2} ${3}
    echo "[=] Compactação finalizada em `date +%d/%m/%Y` as `date +%Hh%M`" >> ${ARQ_LOG} 2>&1
}

# $1=destino, $2=banco
function dump_postgres
{
  echo "[=] Exportando Banco Postgres \"${2}\" para \"${1}\"" >> ${ARQ_LOG} 2>&1

  sudo -u postgres pg_dump ${2} > ${1}
}

# $1=destino, $2=banco $3=porta
function dump_postgres_gz
{
  echo "[=] Exportando Banco Postgres \"${2}\" para \"${1}\"" >> ${ARQ_LOG} 2>&1

  PORT=""
  [ ! -z "${3}" ] && PORT="-p ${3}"

  sudo -u postgres pg_dump ${PORT} -F d -f ${1} ${2}
}

# 1=repositorio 2=arquivos_e_pastas 3=retencao(opcional)
function enviar_restic
{
  RESTIC_BIN="`which restic`"
  [ -z "$RESTIC_BIN" ] && RESTIC_BIN="/backup/restic"

  RESTIC_FORGET="--keep-daily 5 --keep-monthly 1"
  [ ! -z "${3}" ] && RESTIC_FORGET="${3}"

  if [ -z "${DIR_REMOTO}" ]
  then
    echo "É preciso definir: DIR_REMOTO" >> ${ARQ_LOG} 2>&1
    return -1
  fi

  if [ -z "${HOST_REMOTO}" ]
  then
    echo "É preciso definir: HOST_REMOTO" >> ${ARQ_LOG} 2>&1
    return -1
  fi

  export RESTIC_REPOSITORY="sftp:backup@${HOST_REMOTO}:${DIR_REMOTO}/${1}"
  export RESTIC_PASSWORD_FILE="${ARQ_PASS}"

  #echo "[=] Criando repositorio \"${RESTIC_REPOSITORY}\"" >> ${ARQ_LOG} 2>&1
  #${RESTIC_BIN} init

  echo "[=] Eliminando backups antigos em \"${RESTIC_REPOSITORY}\"" >> ${ARQ_LOG} 2>&1
  ${RESTIC_BIN} forget --prune ${RESTIC_FORGET} -p ${RESTIC_PASSWORD_FILE} >> ${ARQ_LOG} 2>&1
 
  echo "[=] Fazendo backup de \"${2}\" em \"${RESTIC_REPOSITORY}\"" >> ${ARQ_LOG} 2>&1
  ${RESTIC_BIN} backup ${2} -p ${RESTIC_PASSWORD_FILE} >> ${ARQ_LOG} 2>&1
}

#$1=PARAMETRO $2=aquivos
function remover_arquivos
{
  PARAMETROS="-vrf"
  [ ! -z "${1}" ] && PARAMETROS="-vrf"

  echo "[=] Removendo o arquivo \"${2}\" após o backup remoto." >> ${ARQ_LOG} 2>&1

  rm ${1} ${2}

}