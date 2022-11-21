// esse arquivo contera todas as funcoes que operam na estrutura da arvore b
#ifndef BTREE_H
#define BTREE_H
#include "topologiaRede.h"

#define PROMOTION 1
#define NO_PROMOTION 0
#define INSERTION_ERROR -1



int busca_arvore(reg_cabecalho_arvore* reg, int* pos, reg_dados_indice* reg_arvore_encontrado, int chave, FILE* arq);
int _busca_arvore(int RRN, int *pos, reg_dados_indice* reg_arvore_encontrado, int chave, FILE* arq);

void insercao_btree(FILE*fp,reg_cabecalho_arvore*h,int key, int data_rrn_4insertion);
int _insercao_btree(FILE* fp,reg_cabecalho_arvore* h, reg_dados_indice* reg_arvore_atual, int key, int data_rrn_4insertion, int* promoted_child, int* promoted_key, int* promoted_data_rrn);
int insere_na_pagina(reg_dados_indice* r,int insert_key, int insert_data_rrn, int insert_child);
int busca_na_pagina(int key, int* pos, reg_dados_indice* r);

#endif