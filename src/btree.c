#include "../include/btree.h"

/**
 * @brief Função responsável por inicializar a busca recursiva de um registro de dados no arquivo de índice da Árvore B
 * 
 * @param reg Ponteiro de registro de cabeçalho para registro de cabeçalho da árvore
 * @param pos Ponteiro de inteiro que marca a posição num nó da árvore (?) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * @param reg_arvore_encontrado Ponteiro para registro de dados do índice que recebe os dados de um registro encontrado na busca
 * @param chave Inteiro referente à chave de busca, isto é, o campo de busca indexada "idConecta" 
 * @param arq Ponteiro para o arquivo de índice em que será realizado a busca
 * @return int Responsável por informar se foi ou não encontrado o registro de dados buscado a partir da chave
 */

int busca_arvore(reg_cabecalho_arvore* reg, int* pos, reg_dados_indice *reg_arvore_encontrado, int chave, FILE* arq){

    if(reg->noRaiz == -1) return NAO_ENCONTRADO;//caso base, não existe arvore

    //Inicializa-se a recursão a partir do RRN do nó da raiz da árvore
    return _busca_arvore(reg->noRaiz, pos, reg_arvore_encontrado, chave, arq);
}

/**
 * @brief Função recursiva de busca por um registro de dados no arquivo de índice da Árvore B
 * 
 * @param RRN RRN do nó em que se está fazendo a busca
 * @param pos onteiro de inteiro que marca a posição num nó da árvore (?) !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 * @param reg_arvore_encontrado Ponteiro para registro de dados do índice que recebe os dados de um registro encontrado na busca
 * @param chave Inteiro referente à chave de busca, isto é, o campo de busca indexada "idConecta" 
 * @param arq Ponteiro para o arquivo de índice em que será realizado a busca
 * @return int Responsável por informar se foi ou não encontrado o registro de dados buscado a partir da chave
 */
int _busca_arvore(int RRN, int *pos, reg_dados_indice *reg_arvore_encontrado, int chave, FILE* arq){

    if(RRN == -1) return NAO_ENCONTRADO;//Caso base, nó pai folha

    reg_dados_indice* novo_reg_dados = cria_registro_dados_indice();
    ler_dados_indice_porRRN(arq, RRN, novo_reg_dados);//lê o nó atual para RAM

    int flag_de_retorno = busca_na_pagina(chave, pos, novo_reg_dados); //busca na pagina (nó) atual
    if(flag_de_retorno == ENCONTRADO){//se encontrado
        *reg_arvore_encontrado = *novo_reg_dados; // reg_arvore_encontrado se torna o novo_reg_dados 
        return ENCONTRADO;
    }
    else{
        //Chama busca recursiva, com o novo RRN referente ao ponteiro para subArvore na posição "pos"
        return(_busca_arvore(novo_reg_dados->ponteiroSubarvore[*pos], pos, reg_arvore_encontrado, chave, arq));
    }
}

/**
 * @brief Função responsável por fazer a busca indexada de um registro de dados a partir de uma chave e um arquivo de
 * índice Árvore B
 * 
 * @param valor Buffer de inteiro para armazenar o o valor do campo idConecta
 * @param num_registros_encontrados Variável responsávael por marcar o número de registros encontrados
 * @param novo_reg_cabecalho_arvore Ponteiro para um registro de cabeçalho da Árvore B
 * @param novo_reg_encontrado Ponteiro de um registro de dados da árvore B para armazenar o registro encontrado na busca
 * @param arquivo_dados Ponteiro para o arquivo de dados com os registros
 * @param arquivo_indice Ponteiro para o arquivo de índice
 * @param novo_reg_dados Ponteiro para registro de dados que armazenará as informações do registro encontrado no arquivo de dados
 */
void busca_indexada(int valor, int num_registros_encontrados, reg_cabecalho_arvore *novo_reg_cabecalho_arvore, reg_dados_indice *novo_reg_encontrado, FILE* arquivo_dados, FILE* arquivo_indice, reg_dados *novo_reg_dados){
    int* pos;//posicao no arquivo de indice

    scanf("%d", &valor);
    num_registros_encontrados = 0;

    int flag_retorno = busca_arvore(novo_reg_cabecalho_arvore, pos, novo_reg_encontrado, valor, arquivo_indice);
    if(flag_retorno != 0){//encontrou registro
    fseek(arquivo_dados, TAM_PAG_DISCO + (*novo_reg_encontrado->RRNdoRegistro)*TAM_REG_DADOS, SEEK_SET);
    le_registro(novo_reg_dados, arquivo_dados);

    if (novo_reg_dados->removido[0] != '1'){
        printa_registro(novo_reg_dados);
        num_registros_encontrados++;
    }
    else{
        if (num_registros_encontrados == 0) printf("Registro inexistente.\n\n");
    }
    }
}

void insercao_btree(FILE*fp, reg_cabecalho_arvore*h, int key, int data_rrn_4insertion){
    int flag_retorno;
    int *promoted_child, *promoted_key, *promoted_data_rrn;
    reg_dados_indice* new_root;
    reg_dados_indice* reg_buscado = cria_registro_dados_indice();
    int* found_pos, flag_busca;

    flag_busca = busca_arvore(h, found_pos, reg_buscado, key, fp);
    printf("has searched tree, returned flag_busca=%d, found_pos=%d and reg_buscado: \n",flag_busca,*found_pos);
    printa_registro_arvore(reg_buscado);

    if(flag_busca == ENCONTRADO) return; // chave ja presente na arvore, nao insere

    printf("calling _insercao_btree first recursion\n");
    flag_retorno = _insercao_btree(fp,h,reg_buscado,key,data_rrn_4insertion,promoted_child,promoted_key,promoted_data_rrn);
    printf("flag_retorno has returned _insercao_btree as %d\n",flag_retorno);

    if(flag_retorno == PROMOTION){ // houve split na raiz ou a arvore estava vazia
        printf("inside promotional conditional\n");
        // nova raiz
        new_root = cria_registro_dados_indice();

        new_root->alturaNo = h->alturaArvore + 1;

        new_root->nroChavesNo = 1;

        new_root->RRNdoNo = h->RRNproxNo;

        new_root->chaveBusca[0] = *promoted_key;
        new_root->RRNdoRegistro[0] = *promoted_data_rrn;

        new_root->ponteiroSubarvore[0] = h->noRaiz;
        new_root->ponteiroSubarvore[1] = *promoted_child;

        // atualiza header
        h->alturaArvore++;
        h->noRaiz = new_root->RRNdoNo;
        h->RRNproxNo++;
        printf("writing data_btree record(new root):\n");
        printa_registro_arvore(new_root);
        escrever_dados_indice_porRRN(fp, new_root->RRNdoNo,new_root); // CRIAR ESSA FUNCAO
    }

}


/**
 * @brief funcao recursiva que insere os nodes internos da btree
 * 
 * @param fp arquivo de indice onde esta a btree
 * @param h cabecalho do arquivo ja lido
 * @param current_index_rrn rrn no arquivo de indice da pagina da btree em que queremos inserir
 * @param key chave a ser inserida
 * @param data_rrn_4insertion rrn no arquivo de dados que deve ser inserido
 * @param promoted_child rrn do filho promovido daqui pro nivel superior
 * @param promoted_key chave promovida daqui pro nivel superior
 * @param promoted_data_rrn rrn no arquivo de dados promovido daqui pro nivel superior
 * @return ** int retorna se houve promocao ou nao, com uma flag extra para erro em caso de chaves repetidas
 */
int _insercao_btree(FILE* fp,reg_cabecalho_arvore* h, reg_dados_indice* reg_arvore_atual, int key, int data_rrn_4insertion, int* promoted_child, int* promoted_key, int* promoted_data_rrn){
    int posicao_key, flag_retorno, flag_insercao=0;

    if(reg_arvore_atual == NULL){ // atingimos uma pagina inexistente - rrn vazio
        *promoted_key = key;
        *promoted_data_rrn = data_rrn_4insertion;
        *promoted_child = -1; // chave promovida vai sem nenhum filho
        return PROMOTION; // promovemos a key para a pagina pai na qual sera inserida
    }else{
        flag_insercao = busca_na_pagina(key, &posicao_key, reg_arvore_atual);
    }
    if(flag_insercao == ENCONTRADO) return INSERTION_ERROR; // retorna erro se ja possui a chave

    // as informacoes possivelmente promovidas de baixo sao atribuidas nessas variaveis
    int* promoted_below_child;
    int* promoted_below_key;
    int* promoted_below_data_rrn;
    if(reg_arvore_atual->ponteiroSubarvore[posicao_key] == -1){ // caso o RRN seja -1 passamos NULL para servir como caso base da recursao
        flag_retorno = _insercao_btree(fp,h,NULL,key,data_rrn_4insertion,promoted_below_child,promoted_below_key,promoted_below_data_rrn);
    }else{
        reg_dados_indice *reg_arvore_proximo;
        reg_arvore_proximo = cria_registro_dados_indice();
        ler_dados_indice_porRRN(fp, reg_arvore_atual->ponteiroSubarvore[posicao_key], reg_arvore_proximo);
        flag_retorno =  _insercao_btree(fp,h,reg_arvore_proximo,key,data_rrn_4insertion,promoted_below_child,promoted_below_key,promoted_below_data_rrn);
    }

    if(flag_retorno == NO_PROMOTION || flag_retorno == INSERTION_ERROR){
        return flag_retorno;
    }
    // flag_retorno == PROMOTION a partir daqui, pois NO_PROMOTION e INSERTION_ERROR foram descartados
    if(reg_arvore_atual->nroChavesNo < ORDEM_ARVORE_B - 1){ // significa que esse no tem espaco
        flag_insercao = insere_na_pagina(reg_arvore_atual,*promoted_below_key,*promoted_below_data_rrn,*promoted_below_child);
        escrever_dados_indice_porRRN(fp,reg_arvore_atual->RRNdoNo,reg_arvore_atual); // escreve os dados atualizados da pagina // CRIAR ESSA FUNCAO
        

        if(flag_insercao == ENCONTRADO) return INSERTION_ERROR;
        else return NO_PROMOTION;
    }else{
        // cria nova pagina
        reg_dados_indice *newreg_arvore;
        newreg_arvore = cria_registro_dados_indice();
        newreg_arvore->RRNdoNo = h->RRNproxNo;
        h->RRNproxNo++;
        strcpy(newreg_arvore->folha,reg_arvore_atual->folha);
        newreg_arvore->alturaNo = reg_arvore_atual->alturaNo;

        split(*promoted_below_key,*promoted_below_data_rrn,*promoted_below_child,promoted_key,promoted_data_rrn,promoted_child,reg_arvore_atual,newreg_arvore);

        // reescreve os dados de ambas as paginas da btree
        escrever_dados_indice_porRRN(fp,reg_arvore_atual->RRNdoNo,reg_arvore_atual); // CRIAR ESSA FUNCAO
        escrever_dados_indice_porRRN(fp,newreg_arvore->RRNdoNo,newreg_arvore); // CRIAR ESSA FUNCAO

        return PROMOTION;
    }
    
}
    /* 
        um array auxiliar aux[TAM+1] recebe o array original a[TAM] + o valor pra insercao 
        [a1 , a2 , ... , an] + valor_insercao = [aux1, aux2, ..., aux(n+1) ]
        sendo que essa insercao eh feita pelo algoritmo de insercao ordenada similar ao da funcao insere_na_pagina
        nesse caso, como a ordem eh impar, teremos a seguinte construcao pros arrays de chave e de data_rrn:
        [a1, a2, a3, a4] + valor_insercao = [aux1, aux2, aux3, aux4, aux5]
        assim, promovemos a chave aux3 para distribuir mais igualmente as chaves, isto eh, [aux1,aux2] ficam na pagina original e [aux4,aux5] na nova pagina
        para o array de subarvores filhas, temos que a insercao so ocorre em nos folha
        logo, todos filhos sao -1 na pagina original e na nova pagina
        e o filho promovido eh o rrn da nova pagina(rrn_np no esquema)

        assim teremos a construcao:

                                pagina de cima
            [...,        chavedecimai,     chavedecima(i+1), ...]    
                   /                    \              \
                  /   promocao:  ^       \             ...
                 /    chave3     |        \
                /   data_rrn3    |         \
               /    rrn_np       |          \
              /                  |           \
            pagina original                nova pagina
            [chave1,chave2]               [chave4,chave5]
           /       |       \            /        |      \
        [NIL,      NIL,      NIL]     [NIL,      NIL,    NIL]

        apesar de no esquema assumirmos ORDEM_ARVORE_B como impar(i.e numero de chave eh par), isso nao faz diferenca no algoritmo
        caso tenhamos 4 chaves(ordem 5) + 1 de insercao e devemos promover uma, promovemos a chave3(posicao 2)
        caso tenhamos 3 chaves(ordem 4) + 1 de insercao e devemos promover uma(optando por deixar o no da esquerda com mais chaves), promovemos a chave3 (posicao 2)
        em ambos casos, a posicao(=2) eh igual ao resultado da divisao inteira da ordem por 2(5/2 = 2 ou 4/2 = 2)
    */
void split(int key, int data_rrn,int right_child, int* upkey, int* updata_rrn, int*upchild, reg_dados_indice *r, reg_dados_indice *newr ){
    // aqui precisamos de vetores auxiliares que serao ordenados de acordo com as keys
    int aux_keys[ORDEM_ARVORE_B];
    int aux_data_rrns[ORDEM_ARVORE_B];
    int aux_right_children[ORDEM_ARVORE_B];
    int promotion_position = ORDEM_ARVORE_B/2;

    // primeiro inserimos ordenadamente(ordenacao baseada nas keys) nos vetores auxiliares
    int has_added_value=0; // indica se o valor ja foi adicionado
    int j=0;              // contador adicional que percorre chaveBusca[] e ponteiroSubarvore[]
    for(int i=0;i<ORDEM_ARVORE_B;i++){
        if(has_added_value == 0 && key < r->chaveBusca[j]){ // se o valor ainda nao foi adicionado e encontramos a primeira chave em chaveBusca[] maior que a key, inserimos a key ao inves dessa chaveBusca[]
            has_added_value = 1;
            aux_keys[i] = key;
            aux_data_rrns[i] = data_rrn;
            aux_right_children[i] = right_child;
        }else{    // caso contrario, so adicionamos a chaveBusca[] (seja antes da key pois chaveBusca[j] < key ou seja depois pois key ja foi adicionada)
            aux_keys[i] = r->chaveBusca[j];
            aux_data_rrns[i] = r->RRNdoRegistro[j];
            aux_right_children[i] = r->ponteiroSubarvore[j+1];
            j++;
        }
    }

    // atualizando os valores a serem promovidos
    *upkey = aux_keys[promotion_position];
    *updata_rrn = aux_data_rrns[promotion_position];
    *upchild = newr->RRNdoNo;

    // atualizando os valores de chave e RRNdoRegistro das paginas da btree
    for(int i=0;i<promotion_position;i++){
        r->RRNdoRegistro[i] = aux_data_rrns[i];
        r->chaveBusca[i] = aux_keys[i];
        r->ponteiroSubarvore[i+1] = aux_right_children[i];

        newr->RRNdoRegistro[i] = aux_data_rrns[i+promotion_position+1];
        newr->chaveBusca[i] = aux_keys[i+promotion_position+1];
        newr->ponteiroSubarvore[i+1] = aux_right_children[i+promotion_position+1];
    }
    newr->ponteiroSubarvore[0] = aux_right_children[promotion_position];

    newr->nroChavesNo = promotion_position;
    r->nroChavesNo = promotion_position;
    if(ORDEM_ARVORE_B % 2 == 0) r->nroChavesNo++; // o da esquerda fica com um a mais se a ordem for par
}

int busca_na_pagina(int key, int* pos, reg_dados_indice* r){
    int i=0;
    // a posicao onde deveria estar ou esta eh antes da primeira chave maior do que a chave buscada
    for(i=0;i<r->nroChavesNo;i++){
        if(key <= r->chaveBusca[i]){
            *pos = i;

            if(r->chaveBusca[*pos] == key) return ENCONTRADO;
            else return NAO_ENCONTRADO;
        }
    }

    if(i == r->nroChavesNo){ // isso significa que nao encontrou nenhuma chave maior que ela
        *pos = i; // deveria estar por ultimo (no caso de a pagina ja estar cheia, isso estara fora dos array bounds e indica pagina cheia  )
        return NAO_ENCONTRADO;
    }

    return SEARCH_ERROR;
}

int insere_na_pagina(reg_dados_indice* r,int insert_key, int insert_data_rrn, int insert_child){
    int pos,flag_insercao; // posicao onde sera inserido no array ordenado
    
    // buscamos a posicao onde deveria estar
    flag_insercao = busca_na_pagina(insert_key,&pos,r);
    if(flag_insercao == ENCONTRADO) return ENCONTRADO;

    // esse loop percorre o array de traz pra frente deslocando os valores pra frente pra abrir espaco pra insercao
    // nesse caso desloca-se a chave de busca(chaveBusca[]), os filhos(ponteiroSubarvore[]) e os registros de dados(RRNdoRegistro[])
    // o deslocamento pode ser feito pois essa funcao so eh chamada quando ha espaco no array
    for(int i=r->nroChavesNo-2;i>pos;i--){
        r->chaveBusca[i+1] = r->chaveBusca[i];
        r->RRNdoRegistro[i+1] = r->RRNdoRegistro[i];
        r->ponteiroSubarvore[i+2] = r->ponteiroSubarvore[i+1]; // o indice eh +1 em comparacao aos outros pois esse array eh maior e desloca-se apenas o filho da direita
    }

    // insere-se as variaveis de insercao na posicao definida
    r->chaveBusca[pos] = insert_key;
    r->RRNdoRegistro[pos] = insert_data_rrn;
    r->ponteiroSubarvore[pos+1] = insert_child; // pos+1 pois eh o filho da direita

    r->nroChavesNo++;

    return NAO_ENCONTRADO;
}