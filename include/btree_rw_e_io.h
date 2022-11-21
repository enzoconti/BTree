// esse arquivo contera todas as funcoes de leitura e escrita(rw) ou de input e output(io)
// relacionadas a arvore b e suas funcionalidades

#ifndef BTREERW_H
#define BTREERW_H
#include "topologiaRede.h"
#include "btree_rw_e_io.h"

void escrever_no_arquivo_cabecalho_arvore(FILE*, reg_cabecalho_arvore*);

//escrita do indice
void escrever_dados_indice_porRRN(FILE*, int, reg_dados_indice*);

//leitura do indice
void ler_dados_indice_porRRN(FILE*, int, reg_dados_indice*);
void ler_reg_cabecalho_arvore(FILE*, reg_cabecalho_arvore*);

#endif