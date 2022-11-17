#include "../include/btree_rw_e_io.h"

void escrever_no_arquivo_cabecalho_arvore(FILE* arquivo, reg_cabecalho_arvore* reg){
    
    escreve_string_no_arquivo(reg->status, arquivo, 1);
    fwrite(&reg->noRaiz, sizeof(int), 1, arquivo);
    fwrite(&reg->nroChavesTotal, sizeof(int), 1, arquivo);
    fwrite(&reg->alturaArvore, sizeof(int), 1, arquivo);
    fwrite(&reg->RRNproxNo, sizeof(int), 1, arquivo);
    
    for(int i = 0; i < (TAM_PAG_ARVORE - TAM_REG_CABECALHO_ARVORE); i++){
        fwrite("$", sizeof(char), 1, arquivo);
    }
}