#include "btree.h"


int busca_arvore(reg_dados_indice* reg, int chave){

    if (reg !=NULL){

    int i = 0;

    while (i<reg->nroChavesNo && chave > reg->chaveBusca[i]) i++;
    
    if (i<reg->nroChavesNo && chave == reg->chaveBusca[i]) return reg->RRNdoRegistro[i]; //chave encontrada

    else if (reg->folha == '1') return ; //n sei oq retornar scr

    else return busca_arvore(reg->ponteiroSubarvore[i], chave);

    }
}

int insercao_btree(FILE* fp, int current_index_rrn, int key, int data_rrn_4insertion, int* promoted_child, int* promoted_key, int* promoted_data_rrn){
    reg_dados_indice *reg_arvore;
    int posicao_key, flag_retorno;

    if(current_index_rrn == -1){ // atingimos uma pagina inexistente - rrn vazio
        *promoted_key = key;
        *promoted_data_rrn = data_rrn_4insertion;
        *promoted_child = -1; // chave promovida vai sem nenhum filho
        return PROMOTION; // promovemos a key para a pagina pai na qual sera inserida
    }else{
        int byteoffset = current_index_rrn * TAM_PAG_ARVORE + TAM_PAG_ARVORE; // uma pagina a mais para header
        fseek(fp, byteoffset,SEEK_SET);
        ler_dados_indice(fp, &reg_arvore);

        posicao_key = busca_na_pagina(key, reg_arvore);
    }

    flag_retorno =  insercao_btree(fp,reg_arvore->ponteiroSubarvore[posicao_key],key,data_rrn_4insertion,promoted_child,promoted_key,promoted_data_rrn);


    if(flag_retorno == NO_PROMOTION || flag_retorno == INSERTION_ERROR){
        return flag_retorno;
    }
    // flag_retorno == PROMOTION a partir daqui
    if(reg_arvore->nroChavesNo < ORDEM_ARVORE_B - 1){
        insere_na_pagina(reg_arvore,*promoted_key,*promoted_data_rrn,*promoted_child);
        return NO_PROMOTION;
    }else{
        // split case
    }
    
}

void insere_na_pagina(reg_dados_indice* r,int insert_key, int insert_data_rrn, int insert_child){
    for(int i=0;i<r->nroChavesNo;i++){
        if() // implementar
    }
}

// pelo amor de Deus ja esqueci icc2 depois eu termino <3
void insertion_sort(int *v, int tam){
  // declara um contador da funcao e uma variavel de referencia
  int referencia, j;

// insertion sort
  for(int i=1;i<tam;i++){
    referencia = v[i];

    j = i-1;

    while(j>=0 && v[j] > referencia){
      v[j+1] = v[j];
      j--;

    }
    v[j+1] = referencia;
    }
}
