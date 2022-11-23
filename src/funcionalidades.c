#include "../include/funcionalidades.h"


/**
 * @brief Função referente à funcionalidade 1, que lê um arquivo csv registros e grava os dados em um arquivo binário.
 * Exibindo o output da função binarioNaTela no final da execução.
 *
 * @param nome_do_arquivo_entrada uma string contendo o nome do arquivo csv de entrada.
 * @param nome_do_arquivo_saida  uma string contendo o nome do arquivo binário de saída.
 */
void comando1(char *nome_do_arquivo_entrada, char *nome_do_arquivo_saida)
{

  FILE *arquivo_entrada = abrir_leitura(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL)
    return;
  FILE *arquivo_saida = abrir_escrita_binario(nome_do_arquivo_saida);
  if (arquivo_saida == NULL)
    return;

  char buffer[TAM_REG_DADOS]; // buffer temporário para armazenar as linhas do arquivo csv
  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  int cont_registros = 0;

  escrever_no_arquivo_cabecalho(arquivo_saida, novo_reg_cabecalho);
  fgets(buffer, 64, arquivo_entrada); // pula primeira linha
  fgets(buffer, 64, arquivo_entrada); // pula primeira linha

  while (fgets(buffer, 64, arquivo_entrada))
  { // enquanto não chegar no fim do arquivo csv (fgets retorna NULL)
    cont_registros++;
    ler_dados(buffer, novo_reg_dados); // lê dados do arquivo csv e grava na struct
    escrever_no_arquivo_dados(arquivo_saida, novo_reg_dados);
  }

  novo_reg_cabecalho->nroPagDisco = calcula_pag_disco(cont_registros);
  strcpy(novo_reg_cabecalho->status, "1"); // atualiza status do arquivo para "1" (não corrompido)
  novo_reg_cabecalho->proxRRN = cont_registros;

  fseek(arquivo_saida, 0, SEEK_SET); // retorna para o início do arquivo

  escrever_no_arquivo_cabecalho(arquivo_saida, novo_reg_cabecalho);

  free(novo_reg_dados);
  free(novo_reg_cabecalho);

  fclose(arquivo_entrada);
  fclose(arquivo_saida);

  binarioNaTela(nome_do_arquivo_saida);
}

/**
 * @brief Função referente à funcionalidade 2, que lê um arquivo binário e exibe todos seus registros.
 *
 * @param nome_do_arquivo_entrada  uma string contendo o nome do arquivo binário de entrada.
 */
void comando2(char *nome_do_arquivo_entrada)
{

  FILE *arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL) return;

  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0)
  {
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }
  if (novo_reg_cabecalho->nroPagDisco == 1)
  { // se não existirem registros no arquivo
    printf("Registro inexistente.\n\n");
    printf("Numero de paginas de disco: %d\n\n", novo_reg_cabecalho->nroPagDisco);

    free(novo_reg_dados);
    free(novo_reg_cabecalho);

    fclose(arquivo_entrada);
    return;
  }

  while (confere_remocao(novo_reg_dados, arquivo_entrada) != 0)
    ; // enquanto o confere remoçao não retornar 0, ainda há registros a serem lidos

  printf("Numero de paginas de disco: %d", novo_reg_cabecalho->nroPagDisco);
  printf("\n\n");

  free(novo_reg_dados);
  free(novo_reg_cabecalho);

  fclose(arquivo_entrada);
}

/**
 * @brief Função referente à funcionalidade 3, que lê um arquivo binário e exibe todos registros encontrados em uma busca.
 *
 * @param nome_do_arquivo_entrada uma string contendo o nome do arquivo binário de entrada.
 */
void comando3(char *nome_do_arquivo_entrada)
{

  FILE *arquivo_entrada = abrir_leitura_binario(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL) return;

  int num_buscas = 0;
  int pos_campo = -1;
  int valor = 0;   // buffer de inteiro para o campo recebido
  char buffer[24]; // buffer de string para o campo recebido
  int num_RRN = -1; // necessário para argumento da função le_arquivo
  int num_registros = 0;

  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0)
  {
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  scanf("%d", &num_buscas);
  for (int i = 0; i < num_buscas; i++)
  { // enquanto as buscas não acabarem
    printf("Busca %d\n", (i + 1));
    int num_registros = 0; // zera o contador de registros encontrados

    pos_campo = ler_campo();

   if (pos_campo == 0 || pos_campo == 2 || pos_campo == 4) //  se for um campo de inteiro
    { 
      scanf("%d", &valor);
      while (le_arquivo(novo_reg_dados, arquivo_entrada, &num_RRN)!=0){
        if (busca_inteiro(novo_reg_dados, arquivo_entrada, pos_campo, &num_registros, valor) == ENCONTRADO) printa_registro(novo_reg_dados);
      }
    } 
    else
    {
      scan_quote_string(buffer);
      while (le_arquivo(novo_reg_dados, arquivo_entrada, &num_RRN)!=0){
        if (busca_string(novo_reg_dados, arquivo_entrada, pos_campo, &num_registros, buffer) == ENCONTRADO) printa_registro(novo_reg_dados);
      }
    }
    
    if (num_registros == NAO_ENCONTRADO) printf("Registro inexistente.\n\n");

    printf("Numero de paginas de disco: %d\n\n", novo_reg_cabecalho->nroPagDisco);
    fseek(arquivo_entrada, TAM_PAG_DISCO, SEEK_SET); // volta pro inicio do arquivo após o registro de cabeçalho para nova busca
  
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);  
  }
}

/**
 * @brief Função referente à funcionalidade 4, que lê um arquivo binário e apaga todos registros encontrados em uma busca,
 * cujo output é a função binarioNaTela
 *
 * @param nome_do_arquivo_entrada uma string contendo o nome do arquivo binário de entrada.
 */
void comando4(char *nome_do_arquivo_entrada)
{

  FILE *arquivo_entrada = abrir_leitura_e_escrita_binario(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL)
    return;

  int num_buscas = 0;
  int pos_campo = -1;
  int valor = 0;   // buffer de inteiro para o campo recebido
  char buffer[24]; // buffer de string para o campo recebido

  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();
  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0)
  {
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  scanf("%d", &num_buscas);
  int num_registros = 0;
  
  for (int i = 0; i < num_buscas; i++)
  { // enquanto as buscas não acabarem
    pos_campo = ler_campo();
    int num_RRN = -1; //reinicia o RRN para cada busca
    

    if (pos_campo == 0 || pos_campo == 2 || pos_campo == 4) //  se for um campo de inteiro
    { 
      scanf("%d", &valor);
      while (le_arquivo(novo_reg_dados, arquivo_entrada, &num_RRN)!=0){
        if (busca_inteiro(novo_reg_dados, arquivo_entrada, pos_campo, &num_registros, valor) == ENCONTRADO) apaga_registro(arquivo_entrada, novo_reg_dados, novo_reg_cabecalho, &num_RRN);
      }
    } 
    else
    {
      scan_quote_string(buffer);
      while (le_arquivo(novo_reg_dados, arquivo_entrada, &num_RRN)!=0){
        if (busca_string(novo_reg_dados, arquivo_entrada, pos_campo, &num_registros, buffer) == ENCONTRADO) apaga_registro(arquivo_entrada, novo_reg_dados, novo_reg_cabecalho, &num_RRN);
      }
    }
    fseek(arquivo_entrada, TAM_PAG_DISCO, SEEK_SET);
  }
  
  novo_reg_cabecalho->nroRegRem += num_registros;
  fseek(arquivo_entrada, 0, SEEK_SET);
  escrever_no_arquivo_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  free(novo_reg_dados);
  free(novo_reg_cabecalho);

  fclose(arquivo_entrada);

  binarioNaTela(nome_do_arquivo_entrada);
}

/**
 * @brief Função referente à funcionalidade 5, que lê um arquivo binário e insere um registro no arquivo, cujo output é
 * a função binárioNaTela
 *
 * @param nome_do_arquivo_entrada  uma string contendo o nome do arquivo binário de entrada.
 */
void comando5(char *nome_do_arquivo_entrada)
{

  FILE *arquivo_entrada = abrir_leitura_e_escrita_binario(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL)
    return;

  //int num_registros_total = 0;
  int num_registros_adicionados = 0;
  //int byte_offset = 0;

  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();
  reg_dados *novo_reg_dados = cria_registro_dados();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0)
  {
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  scanf("%d", &num_registros_adicionados);

  for (int i = 0; i < num_registros_adicionados; i++)
  { // enquanto os registros a serem adicionados não acabarem
    ler_registros_dados_teclado(novo_reg_dados);
    int rrn_inserido=-1;
    rrn_inserido = insere_registro_dados(arquivo_entrada,novo_reg_cabecalho,novo_reg_dados);
    printf("rrn inserido = %d\n",rrn_inserido);
    /*
    // verifica se topo é -1, se é -1, então não há registros removidos
    if (novo_reg_cabecalho->topo == -1)
    {
      byte_offset = TAM_PAG_DISCO + ((novo_reg_cabecalho->proxRRN) * TAM_REG_DADOS);
      fseek(arquivo_entrada, byte_offset, SEEK_SET); // vai para nova posição
      escrever_no_arquivo_dados(arquivo_entrada, novo_reg_dados);
      novo_reg_cabecalho->proxRRN++;
      num_registros_total = (ftell(arquivo_entrada) - TAM_PAG_DISCO) / TAM_REG_DADOS;
    }
    else if (novo_reg_cabecalho->topo != -1)
    { // há registros removidos

      byte_offset = TAM_PAG_DISCO + ((novo_reg_cabecalho->topo) * TAM_REG_DADOS);
      fseek(arquivo_entrada, byte_offset, SEEK_SET);

      fread(novo_reg_dados->removido, sizeof(char), 1, arquivo_entrada);
      novo_reg_dados->removido[1] = '\0';

      if (checa_remocao(novo_reg_dados) == 0)
      {                                                                        // se o registro estiver removido
        fread(&novo_reg_dados->encadeamento, sizeof(int), 1, arquivo_entrada); // pega o encadeamento, isto é, novo espaço livre
        novo_reg_cabecalho->topo = novo_reg_dados->encadeamento;               // desempilha no topo
        fseek(arquivo_entrada, -5, SEEK_CUR);                                  // volta pro início do registro
        novo_reg_dados->removido[0] = '0';                                     // retorna status de não removido
        novo_reg_dados->encadeamento = -1;                                     // reseta o encadeamento
        novo_reg_cabecalho->nroRegRem--;
        escrever_no_arquivo_dados(arquivo_entrada, novo_reg_dados);
      }
      else
      { // se registro não estiver removido, erro
        free(novo_reg_dados);F
        free(novo_reg_cabecalho);
        fclose(arquivo_entrada);
        return;
      }
    }
  }
  if (num_registros_total != 0)
  {
    novo_reg_cabecalho->nroPagDisco = calcula_pag_disco(num_registros_total);
  }
  */
  }
  strcpy(novo_reg_cabecalho->status, "1");
  fseek(arquivo_entrada, 0, SEEK_SET);
  escrever_no_arquivo_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  free(novo_reg_dados);
  free(novo_reg_cabecalho);

  fclose(arquivo_entrada);

  binarioNaTela(nome_do_arquivo_entrada);
}

/**
 * @brief Função referente à funcionalidade 6, que lê um arquivo binário e desfragmenta o arquivo,cujo output é a função
 * binarioNaTela
 *
 * @param nome_do_arquivo_entrada  uma string contendo o nome do arquivo binário de entrada.
 */
void comando6(char *nome_do_arquivo_entrada){

  int contador_reg = 0;

  FILE *arquivo_entrada = abrir_leitura_e_escrita_binario(nome_do_arquivo_entrada);
  if (arquivo_entrada == NULL)
    return;

  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();

  ler_reg_cabecalho(arquivo_entrada, novo_reg_cabecalho);

  if (checa_consistencia(novo_reg_cabecalho) != 0)
  {
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    fclose(arquivo_entrada);
    return;
  }

  if (novo_reg_cabecalho->topo == -1)
  { // se o registro de cabeçalho é -1, então não há registros removidos
    novo_reg_cabecalho->qttCompacta++;
    novo_reg_cabecalho->status[0] = '1';
    fseek(arquivo_entrada, 0, SEEK_SET); // volta pro inicio
    escrever_no_arquivo_cabecalho(arquivo_entrada, novo_reg_cabecalho);
    fclose(arquivo_entrada);
    binarioNaTela(nome_do_arquivo_entrada);
    return;
  }
  else
  {                                                          // se o registro de cabeçalho é diferente de -1, então há registros removidos
    FILE *arquivo_saida = abrir_escrita_binario("temp.bin"); // Cria um novo arquivo para escrita dos registros não-removidos
    if (arquivo_saida == NULL)
      return;

    escrever_no_arquivo_cabecalho(arquivo_saida, novo_reg_cabecalho);
    while (compacta_arquivo(novo_reg_dados, arquivo_entrada, arquivo_saida, &contador_reg) != 0)
      ; // enquanto o compacta_arquivo não retornar 0, ainda há registros a serem lidos
    atualizar_reg_cabecalho(novo_reg_cabecalho, arquivo_saida, &contador_reg);
    fclose(arquivo_saida);
    remover_arquivo("temp.bin", nome_do_arquivo_entrada);

    free(novo_reg_dados);
    free(novo_reg_cabecalho);

    fclose(arquivo_entrada);
  }
}

/**
 * @brief Função responsável por realizar a funcionalidade 8, que lê um arquivo binário e um arquivo de índice e realiza a
 * busca de um registro, utilizando o arquivo de índice no caso de buscas que utilizem o campo idConecta, que é a chave de busca.
 * e utiliza a busca linear (da funcionalidade 3), caso seja utilizado qualquer outro campo.
 * 
 */
void comando8(){
  char *nome_arquivo_dados, *nome_arquivo_indice;

  scanf("%ms", &nome_arquivo_dados);
  scanf("%ms", &nome_arquivo_indice);

  FILE* arquivo_dados = abrir_leitura_binario(nome_arquivo_dados);
  if(arquivo_dados == NULL) return;
  FILE* arquivo_indice = abrir_leitura_binario(nome_arquivo_indice);
  if(arquivo_indice == NULL) return;

  int num_buscas = 0;
  int pos_campo = -1;
  char buffer[24]; // buffer de string para o campo recebido
  int valor = 0;   // buffer de inteiro para o campo recebido
  int num_registros_encontrados = 0;

  scanf("%d", &num_buscas);

  reg_dados *novo_reg_dados = cria_registro_dados();
  reg_dados_indice *novo_reg_encontrado = cria_registro_dados_indice();
  reg_cabecalho *novo_reg_cabecalho = cria_registro_cabecalho();
  reg_cabecalho_arvore *novo_reg_cabecalho_arvore = cria_registro_cabecalho_arvore();

  ler_reg_cabecalho(arquivo_dados, novo_reg_cabecalho);
  ler_reg_cabecalho_arvore(arquivo_indice, novo_reg_cabecalho_arvore);

  if (checa_consistencia(novo_reg_cabecalho) != 0){
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    free(novo_reg_cabecalho_arvore);
    fclose(arquivo_dados);
    fclose(arquivo_indice);
    return;
  }
  if (checa_consistencia_indice(novo_reg_cabecalho_arvore) != 0){
    free(novo_reg_dados);
    free(novo_reg_cabecalho);
    free(novo_reg_cabecalho_arvore);
    fclose(arquivo_dados);
    fclose(arquivo_indice);
    return;
  }

  for (int i = 0; i < num_buscas; i++){ // enquanto as buscas não acabarem
    printf("Busca %d\n", (i + 1));
    pos_campo = ler_campo();
    if(pos_campo == 0) busca_indexada(valor, num_registros_encontrados, novo_reg_cabecalho_arvore, novo_reg_encontrado, arquivo_dados, arquivo_indice, novo_reg_dados);
    else if (pos_campo == 2 || pos_campo == 4){// se for um campo de inteiro

      int num_RRN = -1; // necessário para argumento da função le_arquivo
      scanf("%d", &valor);
      num_registros_encontrados = 0; // reseta contador para nova busca
      while (1)
      {
        if (le_arquivo(novo_reg_dados, arquivo_dados, &num_RRN) != 0)
        { // enquanto não termina o arquivo
          if (novo_reg_dados->removido[0] != '1')
          {
            if (compara_campo_inteiro(pos_campo, valor, novo_reg_dados) == 1)
            { // se o registro foi encontrado
              printa_registro(novo_reg_dados);
              num_registros_encontrados++;
            }
          }
        }
        else
        { // se ler tudo e não achar sai do loop
          if (num_registros_encontrados == 0)
            printf("Registro inexistente.\n\n");
          break;
        }
      }
    }
    else if (pos_campo == 1 || pos_campo == 3 || pos_campo == 5 || pos_campo == 6)
    { // se for campo de string
      int num_RRN = -1;
      scan_quote_string(buffer);
      num_registros_encontrados = 0;
      while (1)
      {
        if (le_arquivo(novo_reg_dados, arquivo_dados, &num_RRN) != 0)
        {
          if (novo_reg_dados->removido[0] != '1')
          {
            if (compara_campo_string(pos_campo, buffer, novo_reg_dados) == 1)
            {
              printa_registro(novo_reg_dados);
              num_registros_encontrados++;
            }
          }
        }
        else
        {
          if (num_registros_encontrados == 0)
            printf("Registro inexistente.\n\n");
          break;
        }
      }
    }
    printf("Numero de paginas de disco: %d\n\n", novo_reg_cabecalho->nroPagDisco);
    fseek(arquivo_dados, TAM_PAG_DISCO, SEEK_SET); // volta pro inicio do arquivo após o registro de cabeçalho para nova busca
    fseek(arquivo_indice, TAM_PAG_ARVORE, SEEK_SET); // volta pro inicio do arquivo após o registro de cabeçalho para nova busca
  }

  free(novo_reg_dados);
  free(novo_reg_cabecalho);
  free(novo_reg_cabecalho_arvore);
  fclose(arquivo_dados);
  fclose(arquivo_indice);
}


void comando9(){

  char *nome_arquivo_dados, *nome_arquivo_indice;
  int n_insercoes, rrn_reg_dados;
  FILE *data_fp, *btree_fp;
  reg_dados rd;
  scanf("%ms", &nome_arquivo_dados);
  scanf("%ms", &nome_arquivo_indice);
  printf("func9 has gotten arq_dados = %s and arq_indice=%s\n",nome_arquivo_dados,nome_arquivo_indice);

  data_fp = abrir_leitura_e_escrita_binario(nome_arquivo_dados);
  btree_fp = abrir_leitura_e_escrita_binario(nome_arquivo_indice);

  reg_cabecalho *h;
  h = cria_registro_cabecalho();
  ler_reg_cabecalho(data_fp, h); // CRIAR ESSA FUNCAO
  printf("data header has been readen as:\n");
  printHeader(h);

  if (checa_consistencia(h) != 0)
  {
    free(h);
    fclose(data_fp);
    return;
  }
  strcpy(h->status, "0");

  reg_cabecalho_arvore *h_btree;
  h_btree = cria_registro_cabecalho_arvore();
  ler_reg_cabecalho_arvore(btree_fp, h_btree); // CRIAR ESSA FUNCAO
  printf("btree header has been readen as:\n");
  printHeaderArvore(h_btree);
  if (checa_consistencia_indice(h_btree) != 0){
    free(h_btree);
    fclose(btree_fp);
    return;
  }
  strcpy(h_btree->status, "0");

  scanf("%d", &n_insercoes);

  for (int i = 0; i < n_insercoes; i++)
  {
    ler_registros_dados_teclado(&rd);
    printf("has gotten from keyboard the following data record:\n");
    printa_registro(&rd);

    printf("going to insert the data record on data file, header currently as:\n");
    printHeader(h);
    rrn_reg_dados = insere_registro_dados(data_fp, h, &rd);
    printf("rd has been inserted on rrn=%d\n",rrn_reg_dados);

    insercao_btree(btree_fp, h_btree, rd.idConecta, rrn_reg_dados);
  }

  strcpy(h->status, "1");
  fseek(data_fp, 0, SEEK_SET);
  escrever_no_arquivo_cabecalho(data_fp, h);

  strcpy(h_btree->status, "1");
  fseek(btree_fp, 0, SEEK_SET);
  escrever_no_arquivo_cabecalho_arvore(btree_fp, h_btree); // CRIAR ESSA FUNCAO

  fclose(data_fp);
  fclose(btree_fp);
  binarioNaTela(nome_arquivo_dados);
  binarioNaTela(nome_arquivo_indice);
}