/**
 * @author Érika Fernanda Santos de Souza
 * @brief Inicializa alocações, salva dados nas estruturas e apresenta menu de opções ao usuário
 * @file menu_principal.c
 * 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "funcoes.h"

int main(){
    // 1. INICIALIZAR: Chama a função que dá o malloc e prepara a memória
    BancoInformacoes* banco = inicializarBanco(); 
    
    // Verificação de segurança 
    if (banco == NULL){
        exit(1); // Sai se falhou a alocação
    }
    
    // 2. salvando dados do arquivo 'dados_jogoadvinhacao.csv' nas estrutura 'Item'
    leArquivoCSV(banco);
    printf("-------- BEM-VINDO(A)!! --------\n");//mensagem inicial de incentivo
    int opcao;
    do{
        printf(" 1 - Inserir  \n");
        printf(" 2 - Listar   \n");
        printf(" 3 - Alterar \n" );
        printf(" 4 - Pesquisar \n" );
        printf(" 5 - Apagar \n" );
        printf(" 6 - Jogar \n");
        printf(" 7 - Listar Ranking \n");
        printf(" 8 - Excluir Ranking \n");
        printf(" 0 - Sair \n");
        printf("Escolha uma opção: \n>");
        scanf("%d",&opcao);
        getchar();
        switch (opcao){
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
        printf("Iniciando o modo Perfil...\n");
        jogarMultiplayer(banco); 
        break;
        case 7:
            listarRanking(); 
            break;
        case 8:
            formatarRanking();
            break;
        case 0:
            printf("Salvando dados do jogo para um arquivo binário...\n");
            salvarItensBinario(banco);
            break;
        default:
            printf("[Aviso] Opção inválida, tente novamente!\n");
            break;
        }//switch
    }//do 
    while (opcao != 0 );
    liberarBanco(banco);
    return 0;
}
