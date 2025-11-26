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
    char dica1[TAM_MAX_DICA];
    char dica2[TAM_MAX_DICA];
    char dica3[TAM_MAX_DICA];
    char dica4[TAM_MAX_DICA];
    char dica5[TAM_MAX_DICA];
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

/**
 * @brief Lê o arquivo "dados_jogoadvinhacao.csv" e salva os dados nas estruturas
 */
void leArquivoCSV(BancoInformacoes* banco);

/**
 * @brief Lê uma string do usuário de forma segura, convertendo-a para minúsculas.
 */
void lerString(char texto[], int tamanho);

/**
 * @brief Inicializa a estrutura BancoInformacoes alocando memória dinâmica.
 */
BancoInformacoes* inicializarBanco(void);

/**
 * @brief Libera toda a memória alocada pelo BancoInformacoes.
 */
void liberarBanco(BancoInformacoes *banco);

/**
 * @brief Insere um novo item no banco de informações.
 */
void inserirItem(BancoInformacoes *banco);

/**
 * @brief Lista todos os itens armazenados no banco.
 */
void listarItens(BancoInformacoes *banco);

/**
 * @brief Altera os dados de um item existente no banco.
 */
void alterarItem(BancoInformacoes *banco);

/**
 * @brief Pesquisa um item pelo nome (resposta).
 */
void pesquisaItem(BancoInformacoes *banco);

/**
 * @brief Exclui um item do banco de informações.
 */
void excluirItem(BancoInformacoes *banco);

/**
 * @brief Salva os itens do banco em um arquivo binário.
 */
void salvarItensBinario(BancoInformacoes *banco);

void processar_resposta(char* input, char* mensagem);

/**
 * @brief Carrega os itens de um arquivo binário para a memória.
 */
BancoInformacoes* carregarItensBinario(const char *nomeArquivo);

#endif




