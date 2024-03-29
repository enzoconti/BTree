#ifndef __AUXILIARES_H__
#define __AUXILIARES_H__

#include "funcoes_fornecidas.h"
#include "funcoes_de_leitura.h"
#include "funcoes_de_escrita.h"
#include "topologiaRede.h"
#include "funcoes_de_print.h"
#include "btree.h"

char* separador(char*);
void gravar_dados(reg_dados*, int, char*);
int compacta_arquivo( reg_dados*, FILE*, FILE*, int*);
void apaga_registro(FILE*, reg_dados* , reg_cabecalho*, int*);
void remover_arquivo(char*, char*);
int confere_remocao(reg_dados*, FILE*);
int monta_arvore(reg_dados*, FILE* arquivo_entrada, FILE* arquivo_indice, reg_cabecalho_arvore*);
int calcula_pag_disco(int);


#endif