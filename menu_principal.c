#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "funcoes.h"
#include "grafica.h"
/**
 * @author Érika Fernanda Santos de Souza
 * @brief Inicializa alocações, salva dados nas estruturas e apresenta menu de opções ao usuário
 * @file menu_principal.c
 * 
 */
int main(){

    // 1. INICIALIZAR: Chama a função que dá o malloc e prepara a memória
    BancoInformacoes* banco = inicializarBanco(); 
    leArquivoCSV(banco);

    // Verificação de segurança 
    if (banco == NULL || banco->totalItens == 0) {
        printf("Erro ao carregar CSV.\n");
    }
    
    // 2. salvando dados do arquivo 'dados_jogoadvinhacao.csv' nas estrutura 'Item'
    //leArquivoCSV(banco);
    
   // printf("Bora advinhar!!");//mensagem inicial de incentivo
    //iniciarJogoGrafico(banco);

    
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
            inserirItem(banco);
            break;

        case 2:
            listarItens(banco);
            break;
        case 3: 
            alterarItem(banco);
            break;
        case 4:
            pesquisaItem(banco);
            break;
        case 5:
           excluirItem(banco);
            break;    
        case 6:
            printf("Vamos jogar!\n");
          iniciarJogoGrafico(banco); 
            break;
        case 0:
            printf("Salvando dados do jogo para um arquivo binario.... \n");
            salvarItensBinario(banco);
            break;
        default:
            printf("Opcao invalida, tente novamente !\n");
            break;
        }//switch
    }//do 
    while (opcao != 0 );

  liberarBanco(banco);

    return 0;
}