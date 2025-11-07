/**
 * @file funcoes.c
 * @author Tarsila Paiva Pimentel
 * @brief Implementações das funções de gerenciamento de itens e banco do jogo Perfil.
 * Este arquivo contém funções para:
 * - Ler strings do usuário de forma segura.
 * - Inicializar e liberar o banco dinâmico de itens.
 * - Inserir novos itens no banco, incluindo nome, categoria, dicas e dificuldade.
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "funcoes.h"

/**
 * @brief Lê uma linha de texto da entrada padrão (stdin), converte para minúsculas e remove a quebra de linha.
 *
 * Esta função realiza as seguintes operações:
 * 1. Limpa o buffer de entrada (`stdin`).
 * 2. Lê uma string (incluindo espaços) até o tamanho máximo especificado.
 * 3. Converte todos os caracteres da string lida para minúsculas.
 * 4. Remove o caractere de nova linha ('\n') adicionado pelo fgets, substituindo-o por um terminador nulo ('\0').
 *
 * @note Requer as bibliotecas <stdio.h>, <string.h> e <ctype.h> para funcionar corretamente.
 *
 * @param texto Ponteiro para o array de caracteres (string) onde o texto lido será armazenado.
 * @param tamanho O tamanho máximo do array 'texto', que limita o número de caracteres a serem lidos.
 *
 * @return void Esta função não retorna valor.
 */
void lerString(char texto[], int tamanho){
    fgets(texto, tamanho, stdin);
    for (int i = 0; texto[i]; i++){
        texto[i] = tolower((unsigned char)texto[i]);
    }
    texto[strcspn(texto, "\n")] = '\0';
    setbuf(stdin, NULL);
}

/**
 * @brief Inicializa e aloca dinamicamente a memória para a estrutura BancoInformacoes.
 *
 * A função aloca o espaço para a estrutura BancoInformacoes e, em seguida,
 * aloca o array dinâmico de Itens com uma capacidade inicial arbitrária (10).
 * O total de itens é inicializado como zero (0).
 *
 * @return BancoInformacoes* Retorna um ponteiro para a estrutura BancoInformacoes
 * alocada e inicializada com sucesso, ou NULL em caso de falha na alocação de memória.
 */
BancoInformacoes* incializarBanco(){
    BancoInformacoes *banco = malloc(sizeof(BancoInformacoes));
    if (banco == NULL){
        printf("[Erro] Houve um erro na alocação de memória para o banco.\n");//Se possível marcar como [Erro], pois fica mais legível
        return NULL;
    }
    
    banco->capacidadeArmazenamento = 10;//Valor abitrário de capacidade inicial
    banco->totalItens = 0;
    banco->itens = malloc(banco->capacidadeArmazenamento * sizeof(Item));

    if (banco->itens == NULL){
        printf("[Erro] Houve um erro na alocação de memória para os itens.\n");
        free(banco);
        return NULL;
    }
    return banco;
}

/**
 * @brief Libera a memória alocada dinamicamente para a estrutura BancoInformacoes.
 *
 * Esta função garante que o array dinâmico de itens (`banco->itens`) e, em seguida,
 * a própria estrutura `banco` sejam desalocados da memória, prevenindo vazamentos de memória.
 * Os ponteiros são definidos como NULL após a liberação.
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes a ser liberada.
 * A função verifica se o ponteiro é válido antes de tentar liberar a memória.
 *
 * @return void Esta função não retorna valor.
 */
void liberarBanco(BancoInformacoes *banco){
    if (banco){
        free(banco->itens);
        banco->itens = NULL;
        free(banco);
        banco = NULL;
    }
}

/**
 * @brief Insere um novo Item no BancoInformacoes, gerenciando a alocação dinâmica.
 *
 * Esta função realiza as seguintes etapas:
 * 1. Verifica se a capacidade do banco foi atingida e, se necessário, duplica a capacidade
 * do array `itens` utilizando `realloc`.
 * 2. Solicita ao usuário as informações do novo item (resposta e nível de dificuldade).
 * 3. Valida a entrada do nível de dificuldade, garantindo que esteja entre 1 e 5.
 * 4. Adiciona o novo item ao final do array (`banco->itens[banco->totalItens]`).
 * 5. Incrementa a contagem de `banco->totalItens`.
 *
 * @note Requer a definição da estrutura BancoInformacoes, a enum/typedef Dificuldade,
 * a função 'lerString' e as constantes de tamanho (ex: TAM_MAX_RESPOSTA).
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes onde o item será inserido.
 *
 * @return void Esta função não retorna valor.
 */
void inserirItem(BancoInformacoes *banco){
    if (*banco == NULL) return;
    if (banco->totalItens >= banco->capacidadeArmazenamento){
        int novaCapacidade = banco->capacidadeArmazenamento * 2;
        Item *novoArray = realloc(banco->totalItens, novaCapacidade * sizeof(Item));
        if (novoArray == NULL){
            printf("[Erro] Houve um erro na realocação de memória para expandir o banco.\n");
            return;
        }
        banco->itens = novoArray;
        banco->capacidadeArmazenamento = novaCapacidade;
    }
    Item novoItem;
    
    printf("Digite a resposta (nome) do item:\n");
    lerString(novoItem.resposta, TAM_MAX_RESPOSTA);
    
    int nivelTemporario;
    do{
        printf("Escolha a dificuldade do item: \n1-Muito Fácil 2-Fácil 3-Médio 4-Difícil 5-Muito Difícil\n");
        scanf("%d", &nivelTemporario);
        setbuf(stdin, NULL);
        if (nivelTemporario < 1 || nivelTemporario > 5){
            printf("[Erro] O nível deve estar entre 1 e 5. Digite novamente:\n")
        }
        
    } while (nivelTemporario < 1 || nivelTemporario > 5);
    novoItem.nivel = (Dificuldade)nivelTemporario;

    banco->itens[banco->totalItens] = novoItem;
    banco->totalItens++;
    printf("[OK] Item inserido com sucesso! Total de itens: %d\n", banco->totalItens);
}

/**
 * @brief Exibe no console todos os itens cadastrados na estrutura BancoInformacoes.
 *
 * * A função percorre o array dinâmico de itens e imprime as informações essenciais
 * de cada um, como Nome (resposta), Categoria e Nível de Dificuldade.
 * * É usado um switch/case para converter o valor enumérico de dificuldade
 * (MUITOFACIL, FACIL, etc.) para sua representação em texto.
 *
 * @note Requer que a enumeração de Dificuldade (MUITOFACIL, etc.) e o campo
 * 'nivel' na struct Item estejam definidos.
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes contendo os itens a serem listados.
 *
 * @return void Esta função não retorna valor.
 */
void listarItens(BancoInformacoes *banco){
    if (banco == NULL){
        printf("[Erro] Não foi encontrado nenhum item cadastrado.\n");
        return;
    }
    
    printf("====== LISTA DE ITENS CADASTRADOS ======\n");
    for (int i = 0; i < banco->totalItens; i++){
        Item *item = &banco->itens[i];
        printf("Item %d:\n", i + 1);
        printf("Nome: %s\n", item->resposta);
        printf("Categoria:\n", item->categoria);
        printf("Nível de Dificuldade: ");
        switch (item->nivel){
            case MUITOFACIL:   printf("Muito Fácil\n"); break;
            case FACIL:        printf("Fácil\n"); break;
            case MEDIO:        printf("Médio\n"); break;
            case DIFICIL:      printf("Difícil\n"); break;
            case MUITODIFICIL: printf("Muito Difícil\n"); break;
            default:           printf("[Indefinido]\n"); break;
        }
    }
    printf("Total de dicas: %d\n", item->totalDicas);
    printf("======================================\n\n");
}

/**
 * @brief Localiza um Item por sua resposta e permite ao usuário alterar seus dados.
 *
 * A função executa as seguintes etapas:
 * 1. Verifica se o banco está inicializado e se contém itens.
 * 2. Solicita ao usuário a 'resposta' (nome) do item a ser buscado.
 * 3. Percorre o array de itens, comparando a resposta de busca.
 * 4. Se o item for encontrado, exibe seus dados atuais e pergunta
 * interativamente ao usuário quais campos (resposta, categoria, nível) deseja alterar.
 * 5. Garante a validação da entrada para o novo nível de dificuldade.
 * 6. Finaliza a execução após a primeira alteração bem-sucedida.
 *
 * @note Requer as funções de entrada (ex: 'lerString'), a estrutura BancoInformacoes,
 * e a enum/typedef Dificuldade.
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes contendo os itens.
 *
 * @return void Esta função não retorna valor.
 */
void alterarItem(BancoInformacoes *banco){
    if (banco == NULL || banco->totalItens == 0){
        printf("[Erro] Não há itens cadastrados para alterar.\n");
        return;
    }

    char busca[TAM_MAX_RESPOSTA];
    printf("Digite o nome (resposta) do item que deseja alterar:\n");
    lerString(busca, TAM_MAX_RESPOSTA);
    for (int i = 0; i < banco->totalItens; i++){
        if (strcmp(banco->itens[i].resposta, busca) == 0){
            printf("\nItem encontrado com sucesso! Informações atuais:\n");
            printf("Resposta: %s\n", banco->itens[i].resposta);
            printf("Categoria: %s\n", banco->itens[i].categoria);
            printf("Nível de Dificuldade: %d\n", banco->itens[i].nivel);

            printf("\nDeseja alterar a resposta? (s/n):\n");
            char opcao;
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) == 's'){
                printf("Nova resposta: ");
                lerString(banco->itens[i].resposta, TAM_MAX_RESPOSTA);
            }

            printf("Deseja alterar a categoria? (s/n):\n");
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) == 's'){
                printf("Nova categoria:\n");
                lerString(banco->itens[i].categoria, TAM_MAX_CATEGORIA);
            }

            printf("Deseja alterar o nível de dificuldade? (s/n):\n");
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) == 's'){
                int novoNivel;
                do{
                    printf("Escolha o novo nível: \n1-Muito Fácil 2-Fácil 3-Médio 4-Difícil 5-Muito Difícil\n");
                    scanf("%d", &novoNivel);
                    setbuf(stdin, NULL);
                    if (novoNivel < 1 || novoNivel > 5)
                        printf("[Erro] Valor inválido. Digite um número entre 1 e 5.\n");
                } while (novoNivel < 1 || novoNivel > 5);
                banco->itens[i].nivel = (Dificuldade)novoNivel;
            }
            printf("\n[OK] Item alterado com sucesso!\n");
            return;
        }
    }
    printf("[Aviso] Nenhum item encontrado com o nome informado.\n");
}

