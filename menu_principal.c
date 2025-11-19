#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "funcoes.h"

int main(){

    printf("Bora advinhar!!");
    BancoInformacoes iformacoes;
    BancoInformacoes* incializarBanco(void);
    Item dadosAdvinha;
    
    int opcao;

    do{
        printf(" 1 - Inserir  \n");
        printf(" 2 - Listar   \n");
        printf(" 3 - Alterar \n" );
        printf(" 4 - Pesquisar \n" );
        printf(" 5 - Apagar \n" );
        printf(" 6 - Jogar \n");
        printf(" 0 - Sair \n");
        printf("Escolha uma opcao: \n");
        scanf("%d",&opcao);
        getchar();

        switch (opcao)
        {
        case 1: 
            inserirItem(BancoInformacoes *banco);
            break;

        case 2:
            listarItens(BancoInformacoes *banco);
            break;
        case 3: 
            alterarItem(BancoInformacoes *banco);
            break;
        case 4:
            pesquisaItem(BancoInformacoes *banco);
            break;
        case 5:
           excluirItem(BancoInformacoes *banco);
            break;    
        case 6:
            printf("Vamos jogar!\n");
          //coloca a função  que roda o jogo
            break;
        case 0:
            printf("Salvando dados do jogo para um arquivo binario.... \n");
            salvarItensBinario(*banco);
            break;
        default:
            printf("Opcao invalida, tente novamente !\n");
            break;
        }//switch
    }//do 
    while (opcao != 0 );

  

    return 0;
}