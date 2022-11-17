// esse arquivo contera todas as funcoes que operam na estrutura da arvore b
#ifndef BTREE_H
#define BTREE_H
#include "topologiaRede.h"

#define PROMOTION 1
#define NO_PROMOTION 0
#define INSERTION_ERROR -1

int busca_arvore(reg_dados_indice* reg, int chave);

int insercao_btree(FILE* fp, int current_index_rrn, int key, int data_rrn_4insertion, int* promoted_child, int* promoted_key, int*promoted_data_rrn);
void insere_na_pagina(reg_dados_indice* r,int insert_key, int insert_data_rrn, int insert_child);

#endif