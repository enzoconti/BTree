#include "../include/topologiaRede.h"

/*

Função responsável por criar um ponteiro de registro de dados, primeiramente alocando espaço, verificando se a alocação
foi de sucesso, e inicializando os valores conforme especificado.

*/
reg_dados* cria_registro_dados(){

    reg_dados* novo_reg;
    novo_reg = (reg_dados*)malloc(sizeof(reg_dados));

    if(novo_reg == NULL){
        return NULL;
    }

    strcpy(novo_reg->removido, "0");
    novo_reg->encadeamento = -1;

    return novo_reg;
}

/*

Função responsável por criar um ponteiro de registro de cabeçalho, primeiramente alocando espaço, verificando se a alocação
foi de sucesso, e inicializando os valores conforme especificado.

*/
reg_cabecalho* cria_registro_cabecalho(){

    reg_cabecalho* novo_reg;
    novo_reg = (reg_cabecalho*)malloc(sizeof(reg_cabecalho));

    if(novo_reg == NULL){
        return NULL;
    }

    strcpy(novo_reg->status, "0");
    novo_reg->topo = -1;
    novo_reg->proxRRN = 0;
    novo_reg->nroRegRem = 0;
    novo_reg->nroPagDisco = 0;
    novo_reg->qttCompacta = 0;

    return novo_reg;
}

reg_cabecalho_arvore* cria_registro_cabecalho_arvore(){

    reg_cabecalho_arvore* novo_reg;
    novo_reg = (reg_cabecalho_arvore*)malloc(sizeof(reg_cabecalho_arvore));

    if(novo_reg == NULL){
        return NULL;
    }

    strcpy(novo_reg->status, "0");
    novo_reg->noRaiz = -1;
    novo_reg->nroChavesTotal = 0;
    novo_reg->alturaArvore = 0;
    novo_reg->RRNproxNo = 0;

    return novo_reg;
}


reg_dados_indice* cria_registro_dados_indice(){

    reg_dados_indice* novo_reg;
    novo_reg = (reg_dados_indice*)malloc(sizeof(reg_dados_indice));

    if(novo_reg == NULL){
        return NULL;
    }

    strcpy(novo_reg->folha, "1");
    novo_reg->nroChavesNo = 0;
    novo_reg->alturaNo = 1;
    novo_reg->RRNdoNo = -1;

    novo_reg->ponteiroSubarvore[0] = -1;
    for(int i=0;i<ORDEM_ARVORE_B-1;i++){
        novo_reg->ponteiroSubarvore[i+1] = -1;
        novo_reg->chaveBusca[i] = -1;
        novo_reg->RRNdoRegistro[i] = -1;
    }

    return novo_reg;
}
    