#include "../include/topologiaRede.h"
#include "../include/funcionalidades.h"

/*
Rafael Freitas Garcia - N°USP 11222374 - 100%
Beatriz Aimee Teixeira Furtado Braga - N°USP 12547934 - 100%
*/

int main(){

    int comando = 0;

    scanf("%d", &comando);
    //scanf("%s", nome_do_arquivo_entrada);

    switch(comando){
        case 1:
            comando1();
            break;
        case 2:
            comando2();
            break;
        case 3:
            comando3();
            break;
        case 4:
            comando4();
            break;
        case 5:
            comando5();
            break;
        case 6:
            comando6();
            break;
        case 7:
            //comando7();
            break;
        case 8:
            comando8();
            break;
        case 9:
            comando9();
            break;
        case 10:
            //comando10();
            break;
    }
    return 0;
}
