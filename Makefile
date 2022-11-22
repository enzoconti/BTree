CC = gcc
MAIN = src/main.c
AUXFILES = src/btree.c src/btree_rw_e_io.c src/funcoes_de_abertura.c src/funcoes_de_busca.c src/funcoes_de_escrita.c src/funcoes_de_leitura.c src/funcionalidades.c src/funcoes_auxiliares.c src/funcoes_checagem.c src/funcoes_fornecidas.c src/topologiaRede.c
EXECNAME = main

all:
	${CC} -o  ${EXECNAME} ${AUXFILES} ${MAIN}

run:
	./${EXECNAME}