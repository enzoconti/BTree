#ifndef __FUNCIONALIDADES_H__
#define __FUNCIONALIDADES_H__

#include "topologiaRede.h"
#include "funcoes_auxiliares.h"
#include "funcoes_checagem.h"
#include "funcoes_de_abertura.h"
#include "funcoes_de_busca.h"
#include "funcoes_de_escrita.h"
#include "funcoes_de_leitura.h"
#include "funcoes_fornecidas.h"
#include "btree.h"
#include "btree_rw_e_io.h"
#include "funcoes_de_print.h"

void comando1();
void comando2();
void comando3();
void comando4();
void comando5();
void comando6();
void comando7();
void comando8();
void comando9();
int juncao(reg_dados* reg1, FILE* arquivo_entrada, reg_cabecalho_arvore* cabecalho_arvore, reg_dados_indice* reg_indice, FILE* arquivo_procura, FILE* arquivo_indice, reg_dados*reg_procura);
void comando10();
void printa_juncao(reg_dados* reg1, reg_dados* reg2);

#endif