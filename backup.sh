#!/bin/bash
# suporte.infra@chapeco.sc.gov.br
# gerenteti@chapeco.sc.gov.br
# update in 05/11/2022
# version 1.0

#VARIAVEIS

#Host remoto qual ira armazenar os backups
HOST_REMOTO="10.0.1.4"

#Pasta remota no servidor de destino que ira armazenar os backups
#A pasta faz menção ao servidor para facilitar  
DIR_REMOTO="/backup/suporte"

#Diretorios temporario para a operação e envio do backup no servidor local
DIR_FILES="/backup/files/"
DIR_BANCO="/backup/banco/"

#Variaveis para facilitar a manipulação dos arquivos
ETC_FILES_GZ="teste_etc_files.tar.gz"
FILES_GZ="_php.tar.gz"
CONFIG_GZ="_config.tar.gz"
DOCS_GZ="_files.tar.gz"
ARQ_BANCO_SQL="dump_db_"
ARQ_BANCO_GZ="dump_db_"

#Arquivo de instruição para o script onde tem todas as funções declaradas
. /backup/backup-restic.inc

log_inicio

#acessar diretorio files
access_dir ${DIR_FILES}

#compactar os arquivos
## Função
## compactar_arquivos -zcvf nome_arquivo_ou_variavel pasta_ou_arquivo_a_ser_compactado
##exemplo
compactar_arquivos -zcvf ${ETC_FILES_GZ} /etc

#enviar o backup para o repositorio remoto
## Função
## enviar_restic repositorio_remoto arquivo_enviar
enviar_restic files.restic "/backup/backup.sh"

#remoção dos arquivos e backup
access_dir ${DIR_FILES}
remover_arquivos -vrf nome_arquivo_ou_variavel
remover_arquivos -vrf glpi_homologacao${GLPI_FILES_GZ}
remover_arquivos -vrf glpi_producao${GLPI_CONFIG_GZ}
remover_arquivos -vrf glpi_homologacao${GLPI_CONFIG_GZ}
remover_arquivos -vrf glpi_producao${GLPI_DOCS_GZ}

#acessar diretorio de backup do banco
access_dir ${DIR_BANCO}

#efetuar o dump do banco de dados
dump_mysql ${ARQ_BANCO_SQL}glpi_producao.sql backup sKEMjJmUQvfmVFeQ glpiweb
dump_mysql ${ARQ_BANCO_SQL}glpi_homologacao.sql backup sKEMjJmUQvfmVFeQ glpi_cco_v10

#acessar o diretorio do banco de dados
access_dir ${DIR_BANCO}

#compactação dos arquivos
compactar_arquivos -zcvf ${ARQ_BANCO_GZ}glpi_producao.tar.gz ${ARQ_BANCO_SQL}glpi_producao.sql
compactar_arquivos -zcvf ${ARQ_BANCO_GZ}glpi_homologacao.tar.gz ${ARQ_BANCO_SQL}glpi_homologacao.sql

#envia o backup do banco para o repositorio remoto
enviar_restic banco.restic ${ARQ_BANCO_GZ}glpi_producao.tar.gz
enviar_restic banco.restic ${ARQ_BANCO_GZ}glpi_homologacao.tar.gz

#Remoção dos arquivos apos envio ao repositorio remoto
access_dir ${DIR_BANCO}
remover_arquivos -vrf ${ARQ_BANCO_GZ}glpi_producao.sql
remover_arquivos -vrf ${ARQ_BANCO_GZ}glpi_homologacao.sql
remover_arquivos -vrf ${ARQ_BANCO_GZ}glpi_producao.tar.gz
remover_arquivos -vrf ${ARQ_BANCO_GZ}glpi_homologacao.tar.gz

log_fim