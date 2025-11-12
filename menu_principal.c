/*
 
 * Este arquivo contém a função main() e o loop de menu que permite
 * ao usuário interagir com as funções de gerenciamento de itens
 * (CRUD - Create, Read, Update, Delete) e persistência de dados.
 
 */

#include <stdio.h>
#include <stdlib.h> // Para system("clear") ou system("cls") e EXIT_SUCCESS
#include "funcoes.h" // Inclui todas as suas definições e protótipos

// Define o nome do arquivo binário onde os dados serão salvos.
// Usar uma constante facilita a manutenção.
#define NOME_ARQUIVO_BIN "banco_perfil.bin"

/**
 * @brief Limpa a tela do console de forma portável (Linux/Mac/Windows).
 */
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/**
 * @brief Exibe o menu principal de opções para o usuário.
 */
void exibirMenu() {
    printf("\n====== JOGO PERFIL - GERENCIADOR DE ITENS ======\n");
    printf("1. Inserir novo item\n");
    printf("2. Listar todos os itens\n");
    printf("3. Alterar um item\n");
    printf("4. Pesquisar um item\n");
    printf("5. Excluir um item\n");
    printf("0. Salvar e Sair\n");
    printf("==============================================\n");
    printf("Escolha uma opção: ");
}

/**
 * @brief Lê um número inteiro do usuário e limpa o buffer de entrada (stdin).
 *
 * Isso é crucial para evitar que o '\n' (Enter) deixado pelo scanf
 * atrapalhe as futuras chamadas da sua função lerString (que usa fgets).
 *
 * @return int A opção numérica escolhida pelo usuário.
 */
int obterOpcao() {
    int opcao;
    
    // Tenta ler um inteiro. Se o usuário digitar letras, scanf falha.
    if (scanf("%d", &opcao) != 1) {
        opcao = -1; // Define uma opção inválida para forçar o 'default' no switch
    }

    // Limpa qualquer caractere restante no buffer de entrada,
    // principalmente o '\n' (Enter).
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    return opcao;
}

/**
 * @brief Função principal que executa o loop do menu.
 */
int main() {
    // Tenta carregar o banco de dados do arquivo.
    // Conforme sua função, se o arquivo não existir, ele cria um banco novo.
    BancoInformacoes *banco = carregarItensBinario(NOME_ARQUIVO_BIN);

    // Verificação crítica: se a alocação de memória falhou no carregamento/inicialização
    if (banco == NULL) {
        printf("[Erro Crítico] Não foi possível alocar memória para o banco. Saindo.\n");
        return EXIT_FAILURE; // Termina o programa com código de erro
    }

    int opcao;

    do {
        limparTela();
        exibirMenu();
        opcao = obterOpcao();

        switch (opcao) {
            case 1:
                printf("\n--- Inserir Item ---\n");
                inserirItem(banco);
                break;
            case 2:
                printf("\n--- Listar Itens ---\n");
                listarItens(banco);
                break;
            case 3:
                printf("\n--- Alterar Item ---\n");
                alterarItem(banco);
                break;
            case 4:
                printf("\n--- Pesquisar Item ---\n");
                pesquisaItem(banco);
                break;
            case 5:
                printf("\n--- Excluir Item ---\n");
                excluirItem(banco);
                break;
            case 0:
                printf("\n--- Salvando e Saindo ---\n");
                break; // Apenas sai do loop
            default:
                printf("\n[Erro] Opção inválida! Por favor, tente novamente.\n");
                break;
        }

        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            getchar(); // Pausa o programa esperando o usuário
        }

    } while (opcao != 0);

    // --- Seção de Encerramento ---
    
    // 1. Salva o estado atual do banco no arquivo binário
    printf("Salvando dados em '%s'...\n", NOME_ARQUIVO_BIN);
    salvarItensBinario(banco, NOME_ARQUIVO_BIN);

    // 2. Libera toda a memória alocada
    printf("Liberando memória...\n");
    liberarBanco(banco);
    banco = NULL; // Boa prática para evitar 'dangling pointers'

    printf("Até logo!\n");
    return EXIT_SUCCESS; // Termina o programa com sucesso
}