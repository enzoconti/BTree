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

void ler_reg_cabecalho_arvore(FILE* arquivo, reg_cabecalho_arvore* reg){
    
    fread(&reg->status, sizeof(char), 1, arquivo);
    fread(&reg->noRaiz, sizeof(int), 1, arquivo);
    fread(&reg->nroChavesTotal, sizeof(int), 1, arquivo);
    fread(&reg->alturaArvore, sizeof(int), 1, arquivo);
    fread(&reg->RRNproxNo, sizeof(int), 1, arquivo);
    
    fseek(arquivo, TAM_PAG_ARVORE - TAM_REG_CABECALHO_ARVORE, SEEK_CUR);
}

void escrever_dados_indice_porRRN(FILE* arq, int RRN, reg_dados_indice* reg){

    int byte_offset = TAM_PAG_ARVORE + RRN*TAM_REG_DADOS_ARVORE;
    fseek(arq, byte_offset , SEEK_SET);

    fwrite(reg->folha[0], sizeof(char), 1, arq);
    fwrite(&reg->nroChavesNo, sizeof(int), 1, arq);
    fwrite(&reg->RRNdoNo, sizeof(int), 1, arq);

    for(int i = 0; i < ORDEM_ARVORE_B; i++){
        fwrite(reg->ponteiroSubarvore[i], sizeof(int), 1, arq);
    }
    for(int i = 0; i < ORDEM_ARVORE_B-1; i++){
        fwrite(reg->chaveBusca[i], sizeof(int), 1, arq);
    }
        for(int i = 0; i < ORDEM_ARVORE_B-1; i++){
        fwrite(reg->RRNdoRegistro[i], sizeof(int), 1, arq);
    }
}


void ler_dados_indice_porRRN(FILE* arq, int RRN, reg_dados_indice* reg){

    int byte_offset = TAM_REG_CABECALHO_ARVORE + RRN*TAM_REG_DADOS_ARVORE;
    fseek(arq, byte_offset , SEEK_SET);

    fread(reg->folha[0], sizeof(char), 1, arq);
    reg->folha[1] = '\0';

    fread(&reg->nroChavesNo, sizeof(int), 1, arq);
    fread(&reg->RRNdoNo, sizeof(int), 1, arq);

    for(int i = 0; i < ORDEM_ARVORE_B; i++){
        fread(reg->ponteiroSubarvore[i], sizeof(int), 1, arq);
    }
    for(int i = 0; i < ORDEM_ARVORE_B-1; i++){
        fread(reg->chaveBusca[i], sizeof(int), 1, arq);
    }
        for(int i = 0; i < ORDEM_ARVORE_B-1; i++){
        fread(reg->RRNdoRegistro[i], sizeof(int), 1, arq);
    }
}