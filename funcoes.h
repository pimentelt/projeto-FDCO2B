#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_DICAS 5
#define TAM_MAX_DICA 200
#define TAM_MAX_RESPOSTA 102
#define TAM_MAX_CATEGORIA 102

typedef enum {MUITOFACIL = 1, FACIL, MEDIO, DIFICIL, MUITODIFICIL}Dificuldade;

/**
 * @brief Estrutura de dados que armazena todas as informações de um item no sistema.
 * 
 */
typedef struct{
    char resposta[TAM_MAX_RESPOSTA];
    char dica[TAM_MAX_DICA];
    char categoria[TAM_MAX_CATEGORIA];
    int quantidadeDicas[MAX_DICAS][TAM_MAX_DICA];
    int totalDicas;
    Dificuldade nivel;  
}Item;

/**
 * @brief strutura que representa o banco de dados dinâmico de todos os itens cadastrados.
 *
 * Utilizada para gerenciar a coleção de itens, permitindo que o número
 * de itens cresça ou diminua dinamicamente na memória, sem limites fixos
 * (além da memória disponível).
 */
typedef struct{
    Item *itens;
    int totalItens;
    int capacidadeArmazenamento;
}BancoInformacoes;

#endif
