#include "../include/funcoes_de_print.h"

void print_busca(int buscas){
    printf(BUSCA, (buscas + 1));
}

void print_num_pag_discos(int num_pag_disco){
    printf(NUM_PAG_DISCO, num_pag_disco);
}

void print_falha_processamento_arquivo(){
    printf(ERRO_FALHA_PROCESSAMENTO_ARQUIVO);
}

void print_registro_inexistente(){
    printf(ERRO_REG_INEXISTENTE);
}

/*

Função responsável por imprimir os dados de um registro conforme requisitado nas especificações do trabalho. Recebe um
ponteiro para registro de dados. Antes de cada impressão, é verificado em cada caso em que o campo pode ser nulo, se o valor
contido na struct não é referente à um campo nulo. Pois caso seja, não ocorre a impressão deste campo.

*/
void printa_registro(reg_dados* reg){

    printf("Identificador do ponto: %d\n", reg->idConecta);
    if(reg->nomePoPs[0] != '\0') printf("Nome do ponto: %s\n", reg->nomePoPs);//confere se não é nulo
    if(reg->nomePais[0] != '\0') printf("Pais de localizacao: %s\n", reg->nomePais);
    if(reg->siglaPais[0] != '$') printf("Sigla do pais: %s\n", reg->siglaPais);
    if(reg->idPoPsConectado != -1) printf("Identificador do ponto conectado: %d\n", reg->idPoPsConectado);
    if(reg->velocidade != -1) printf("Velocidade de transmissao: %d %sbps\n", reg->velocidade, reg->unidadeMedida);
    printf("\n");
}