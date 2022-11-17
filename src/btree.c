#include "btree.h"


int busca_arvore(reg_dados_indice* reg, int chave){

    if (reg !=NULL){

    int i = 0;

    while (i<reg->nroChavesNo && chave > reg->chaveBusca[i]) i++;
    
    if (i<reg->nroChavesNo && chave == reg->chaveBusca[i]) return reg->RRNdoRegistro[i]; //chave encontrada

    else if (reg->folha == '1') return ; //n sei oq retornar scr

    else return busca_arvore(reg->ponteiroSubarvore[i], chave);

    }
