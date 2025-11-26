#ifndef FUNCOES_H
#define FUNCOES_H

#define MAX_DICAS 5
#define TAM_MAX_DICA 200
#define TAM_MAX_RESPOSTA 102
#define TAM_MAX_CATEGORIA 102
#define MAX_JOGADORES_SESSAO 4
#define TAM_MAX_NOME 50
#define MAX_RANKING_ENTRIES 10
#define ARQUIVO_RANKING "ranking.dat"

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
    Dificuldade nivel;  
}Item;

/**
 * @brief Estrutura que representa o banco de dados dinâmico de todos os itens cadastrados.
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
 * @brief Estrutura que armazena os dados de um jogador durante uma única sessão de jogo.
 *
 * Esta estrutura é utilizada para registrar informações temporárias 
 * e específicas da sessão de um jogador, como seu nome, 
 * a pontuação acumulada na sessão atual e o número de tentativas 
 * realizadas até o momento.
 */
typedef struct{
    char nome[TAM_MAX_NOME];
    int pontuacaoSessao;
    int tentativasNoItem;
}JogadorSessao;

/**
 * @brief Representa uma entrada individual (registro) no ranking histórico do jogo.
 *
 * Esta estrutura é utilizada para manipular e persistir os dados dos recordes.
 * Ela armazena o par "Nome + Pontuação" de um jogador para ser gravado ou lido
 * do arquivo binário de ranking.
 * * @see salvarRanking
 * @see listarRanking
 */
typedef struct {
    char nome[TAM_MAX_NOME];
    int pontuacao;
}PosicaoRanking;

/**
 * @brief Obtém um item aleatório do banco de informações para o jogo.
 */
Item* obterItemAleatorio(BancoInformacoes* banco);

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

/**
 * @brief Carrega os itens de um arquivo binário para a memória.
 */
BancoInformacoes* carregarItensBinario(const char *nomeArquivo);

/**
 * @brief Executa a lógica principal do jogo (Modo Jogar).
 */
void jogar(BancoInformacoes* banco);

/**
 * @brief Salva a pontuação no ranking. Mantém apenas o Top 10.
 */
void salvarRanking(const char* nome, int pontuacao);

/**
 * @brief Exibe o Top 10 jogadores.
 */
void listarRanking();

/**
 * @brief Apaga o arquivo de ranking (Formatar).
 */
void formatarRanking(BancoInformacoes* banco);

/**
 * @brief Remove espaço de palavras reconhecendo como iguais.
 */
void removerEspacos(char str[]);

/**
 * @brief Inicia o jogo, dando a opção multijogador
 */
void jogarMultiplayer(BancoInformacoes* banco);

#endif
