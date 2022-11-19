#include "topologiaRede.h"
#include "funcionalidades.h"

/*
Rafael Freitas Garcia - N°USP 11222374 - 100%
Beatriz Aimee Teixeira Furtado Braga - N°USP 12547934 - 100%
*/

int main(){

    char nome_do_arquivo_entrada[TAM_NOME_ARQUIVO];
    char nome_do_arquivo_saida[TAM_NOME_ARQUIVO];
    int comando = 0;

    scanf("%d", &comando);
    scanf("%s", nome_do_arquivo_entrada);

    switch(comando){
        case 1:
            scanf("%s", nome_do_arquivo_saida);
            comando1(nome_do_arquivo_entrada, nome_do_arquivo_saida);
            break;
        case 2:
            comando2(nome_do_arquivo_entrada);
            break;
        case 3:
            comando3(nome_do_arquivo_entrada);
            break;
        case 4:
            comando4(nome_do_arquivo_entrada);
            break;
        case 5:
            comando5(nome_do_arquivo_entrada);
            break;
        case 6:
            comando6(nome_do_arquivo_entrada);
            break;
        case 7:
            comando7();
            break;
        case 8:
            comando8();
            break;
        case 9:
            comando9();
            break;
        case 10:
            comando10();
            break;
    }
    return 0;
}