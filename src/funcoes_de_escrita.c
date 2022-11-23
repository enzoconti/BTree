#include "../include/funcoes_de_escrita.h"
/*

Função que recebe uma string, um ponteiro de arquivo, e o tamanho de uma string,
escreve usando frwrite e o tamanho informado no arquivo apontado

*/
void escreve_string_no_arquivo(char* str, FILE* arquivo, int tam){
    fwrite(str, sizeof(char), tam, arquivo);
}

/*

Função que escreve os dados da struct do registro de dados no arquivo, recebe um ponteiro de arquivo e um ponteiro para
registro de dados. Chama a função escreve_string_no_arquivo para campos referentes à strings, e fwrite para campos
referentes à inteiros. 

Para os campos de tamanho variável, primeiro faz-se a conferencia se o campo está vazio na RAM ou não possui | no final, 
situações que ocorrem quando é lido um arquivo com a função le_arquivo, que tira o | na RAM. Nesses casos, o | é adicionado
antes da escrita. 

Como o registro de dados deve ocupar 64 bytes, é feito o cálculo do tamanho do registro atual, isto é, 20 (campos fixos)
+ o tamanho das strings referentes aos campos variáveis. Em seguida, o tamanho em bytes da quantidade de lixo a ser
preenchida no registro é calculada por tamanho do registro de dados - tamanho do registro atual, e o loop for é usado
para preenchimento com '$'.

*/
void escrever_no_arquivo_dados(FILE* arquivo, reg_dados* reg){

    int tam_reg_atual = 0;
    int tam_lixo = 0;

    escreve_string_no_arquivo(reg->removido, arquivo, strlen(reg->removido));
    fwrite(&reg->encadeamento, sizeof(int), 1, arquivo);
    fwrite(&reg->idConecta, sizeof(int), 1, arquivo);
    escreve_string_no_arquivo(reg->siglaPais, arquivo, strlen(reg->siglaPais));
    fwrite(&reg->idPoPsConectado, sizeof(int), 1, arquivo);
    escreve_string_no_arquivo(reg->unidadeMedida, arquivo, strlen(reg->unidadeMedida));
    fwrite(&reg->velocidade, sizeof(int), 1, arquivo);

    if(strcmp(reg->nomePoPs, "\0") == 0) strcpy(reg->nomePoPs, "|");
    if (reg->nomePoPs[strlen(reg->nomePoPs)-1] != '|') strcat(reg->nomePoPs, "|");
    escreve_string_no_arquivo(reg->nomePoPs, arquivo, strlen(reg->nomePoPs));

    if(strcmp(reg->nomePais, "\0") == 0) strcpy(reg->nomePais, "|");
    if (reg->nomePais[strlen(reg->nomePais)-1] != '|') strcat(reg->nomePais, "|");
    escreve_string_no_arquivo(reg->nomePais, arquivo, strlen(reg->nomePais));
    

    tam_reg_atual = 20 + strlen(reg->nomePoPs) + strlen(reg->nomePais);
    tam_lixo = (TAM_REG_DADOS - tam_reg_atual);

    for(int i = 0; i < tam_lixo; i++){
        fwrite("$", sizeof(char), 1, arquivo);
    }
}

/*

Função que escreve os dados da struct do registro de cabeçalho no arquivo, recebe um ponteiro de arquivo e um ponteiro para
registro de cabeçalho. Chama a função escreve_string_no_arquivo para campos referentes à strings, e fwrite para campos
referentes à inteiros. 

Como o registro de cabeçalho deve ocupar o tamanho de uma página de disco (960 bytes), é feito o cálculo do valor do tamanho
da página de disco - o tamanho do registro de cabeçalho (que é fixo), para utilizar-se como condição de parada num loop
que escreve '$' até completar o tamanho necessário.

*/
void escrever_no_arquivo_cabecalho(FILE* arquivo, reg_cabecalho* reg){
    
    escreve_string_no_arquivo(reg->status, arquivo, 1);
    fwrite(&reg->topo, sizeof(int), 1, arquivo);
    fwrite(&reg->proxRRN, sizeof(int), 1, arquivo);
    fwrite(&reg->nroRegRem, sizeof(int), 1, arquivo);
    fwrite(&reg->nroPagDisco, sizeof(int), 1, arquivo);
    fwrite(&reg->qttCompacta, sizeof(int), 1, arquivo);
    
    for(int i = 0; i < (TAM_PAG_DISCO - TAM_REG_CABECALHO); i++){
        fwrite("$", sizeof(char), 1, arquivo);
    }
}

/*

Função que atualiza os dados da struct do registro de cabeçalho na RAM e depois escreve no arquivo. 
Recebe um ponteiro de arquivo e um ponteiro para
registro de cabeçalho, alem do ponteiro de inteiro que possui a quantidade de registros. 

Passa o numero de registros removidos para 0, já que o registro acabou de ser desfragmentado. O prox RRN é 
o valor do cont_registros (já que ele começa a contagem em 1 e o RRN começa a contagem em 0, o valor do 
proxRRN e do cont_registros coincidem). O valor de compactações aumenta em 1, o status é resetado para '1' 
e o topo para -1, indicando que não há mais registros removidos. Já o novo numero de pagina de disco 
é calculado com a função calcula_pag_disco(). 

Por fim, é dado um fseek para o inicio do arquivo e o cabeçalho é 
sobrescrito com a função escrever_no_arquivo_cabecalho()

*/
void atualizar_reg_cabecalho(reg_cabecalho* reg, FILE* arquivo_saida, int*cont_registro){
    reg->nroRegRem = 0;
    reg->proxRRN = (*cont_registro);
    reg->qttCompacta++;
    reg->status[0] = '1';
    reg->topo = -1;
    reg->nroPagDisco=calcula_pag_disco(*cont_registro);
    fseek(arquivo_saida, 0, SEEK_SET);
    escrever_no_arquivo_cabecalho(arquivo_saida, reg);
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

int insere_registro_dados(FILE* arquivo_entrada, reg_cabecalho *h,reg_dados* rd){
    int byte_offset, num_registros_total;
    int byteoffset_written;
    int esta_removido,encadeamento;
     // verifica se topo é -1, se é -1, então não há registros removidos
    if(h->topo == -1){
      byte_offset = TAM_PAG_DISCO + ((h->proxRRN) * TAM_REG_DADOS);
      fseek(arquivo_entrada, byte_offset, SEEK_SET);//vai para nova posição
      byteoffset_written = byte_offset;
      escrever_no_arquivo_dados(arquivo_entrada, rd);
      h->proxRRN++;
      num_registros_total = (ftell(arquivo_entrada) - TAM_PAG_DISCO)/TAM_REG_DADOS;
    }
    else if(h->topo != -1){//há registros removidos


      byte_offset = TAM_PAG_DISCO + ((h->topo) * TAM_REG_DADOS);
      fseek(arquivo_entrada, byte_offset, SEEK_SET);

      fread(rd->removido, sizeof(char), 1, arquivo_entrada);
      rd->removido[1] = '\0';


      if(checa_remocao(rd) == 0){ // se o registro estiver removido
        fread(&rd->encadeamento, sizeof(int), 1, arquivo_entrada); // pega o encadeamento, isto é, novo espaço livre
        h->topo = rd->encadeamento;     // desempilha no topo
        fseek(arquivo_entrada, -5, SEEK_CUR); // volta pro início do registro
        rd->removido[0] = '0';    // retorna status de não removido
        rd->encadeamento = -1; //reseta o encadeamento
        h->nroRegRem--;
        byteoffset_written = ftell(arquivo_entrada);
        escrever_no_arquivo_dados(arquivo_entrada, rd);
      }
      else{ // se registro não estiver removido, erro
        free(rd);
        free(h);
        fclose(arquivo_entrada);
        return -1;
      }
    }

    if(num_registros_total != 0){
   h->nroPagDisco = calcula_pag_disco(num_registros_total);
  }

    return (byteoffset_written - TAM_PAG_DISCO)/TAM_REG_DADOS;
}

void printHeader(reg_cabecalho* h){
    printf("status: %s\n",h->status);
    printf("topo: %d\n",h->topo);
    printf("proxRRN: %d\n",h->proxRRN);
    printf("nroRegRem: %d\n", h->proxRRN);
    printf("nroPagDisco: %d\n",h->nroPagDisco);
    printf("qttCompacta: %d\n",h->qttCompacta);
    printf("\n");

}
