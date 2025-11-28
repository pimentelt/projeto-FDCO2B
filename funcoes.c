/**
 * @file funcoes.c
 * @author Tarsila Paiva Pimentel e Érika Fernanda Santos de Souza
 * @brief Implementações das funções de gerenciamento de itens e do banco de dados do jogo *Perfil*.
 * 
 * Este módulo contém a implementação completa das rotinas responsáveis por gerenciar o banco dinâmico
 * de itens utilizados no jogo *Perfil*. Abrange desde a leitura de dados e manipulação de memória até
 * a persistência em arquivos binários.
 * 
 * As principais funcionalidades incluem:
 * 
 * - Leitura segura de strings a partir da entrada padrão, com normalização de caixa.
 * - Inicialização e liberação do banco de informações dinâmico.
 * - Inserção, listagem, pesquisa, alteração e exclusão de itens no banco.
 * - Leitura do arquivo CSV
 * - Salvamento e carregamento dos dados em arquivos binários, preservando o estado do jogo.
 * 
 * O arquivo faz uso de alocação dinâmica de memória (`malloc`, `realloc`, `free`) e controle de erros
 * básico, garantindo maior robustez durante as operações de inserção e expansão do banco de dados.
 * 
 * @note Requer o cabeçalho correspondente **funcoes.h**, que define as estruturas, tipos e constantes
 * utilizados em todas as funções aqui implementadas.
 * 
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
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
 * @brief Remove todos os caracteres de espaço (' ') de uma string.
 *
 * Esta função percorre a string, copiando apenas os caracteres que não são espaço
 * para o início da mesma string, efetivamente eliminando os espaços.
 *
 * @param str A string a ser processada.
 * @return void
 */
void removerEspacos(char str[]){
    int i, j = 0;
    // Percorre a string
    for (i = 0; str[i] != '\0'; i++) {
        // Se o caractere atual NÃO for um espaço, ele é copiado
        if (str[i] != ' ') {
            str[j] = str[i];
            j++;
        }
    }
    // Finaliza a nova string no ponto correto
    str[j] = '\0';
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
BancoInformacoes* inicializarBanco(){
    BancoInformacoes *banco = malloc(sizeof(BancoInformacoes));
    if (banco == NULL){
        printf("[Erro] Houve um erro na alocação de memória para o banco.\n");//Se possível marcar como [Erro], pois fica mais legível
        return NULL;
    }
    
    banco->capacidadeArmazenamento = 40;//Valor abitrário de capacidade inicial
    banco->totalItens = 0;
    banco->itens = malloc(banco->capacidadeArmazenamento * sizeof(Item));
    printf("Deu certo função inicializarBanco\n");

    if (banco->itens == NULL){
        printf("[Erro] Houve um erro na alocação de memória para os itens.\n");
        free(banco);
        return NULL;
    }
    return banco;
}

/**
 * @brief Obtém um item aleatório do banco de informações para o jogo.
 *
 * A função inicializa a semente de números aleatórios (apenas uma vez)
 * e calcula um índice aleatório válido dentro do total de itens carregados.
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes.
 * @return Item* Retorna um ponteiro para o item escolhido, ou NULL se o banco estiver vazio.
 */
Item* obterItemAleatorio(BancoInformacoes *banco){
    if (banco == NULL || banco->totalItens == 0){
        printf("[Aviso] O banco de informaçôes está vazio.\n");
        return NULL;
    }
    
    //Inicialização da semente (isso aqui me deu dor de cabeça de entender)
    static int seed_initialized = 0;
    if (!seed_initialized) {
        srand(time(NULL)); // Inicializa o gerador com o tempo atual.
        seed_initialized = 1;//A biblioteca de tempo vai ser utilizada pra gerar um número pseudo-aleatório.
    }
    int indiceAleatorio = rand() % banco->totalItens;//Aqui é gerado o valor pseudo-aleatório.
    return &banco->itens[indiceAleatorio];

}

/**
 * @brief Carrega os dados de itens de um arquivo CSV para a estrutura BancoInformacoes.
 * * Esta função abre o arquivo "dados_jogoadvinhacao.csv", pula o cabeçalho
 * e lê sequencialmente os registros de itens (resposta, nível, 5 dicas).
 * A leitura utiliza o formato CSV (ou TSV) onde os campos são separados por
 * ponto e vírgula (`;`). Os itens são carregados no array dinâmico
 * 'banco->itens' até o limite da capacidade alocada.
 *
 * O total de itens lidos é atualizado em 'banco->totalItens'.
 *
 * @note Em caso de falha na abertura do arquivo, a função exibe uma mensagem
 * de erro e encerra o programa com exit(1).
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes, onde os dados
 * do arquivo serão armazenados. Deve ser um banco inicializado e com memória
 * alocada para 'itens'.
 * * @return void Esta função não retorna valor.
 */
void leArquivoCSV(BancoInformacoes* banco){
    FILE* arquivoLeCSV = fopen("dados_jogoadvinhacao.csv", "r");
    if (arquivoLeCSV == NULL) {
        printf("[Erro] Não foi possivel abrir o arquivo CSV.\n");
        exit(1);
    }

    // Buffer para pular a primeira linha (cabeçalho)
    char buffer[1024];
    fgets(buffer, 1024, arquivoLeCSV); 

    int i = 0;
    int nivelTemporario; // Variável temporária para ler o nível

    // O formato "%[^;]" lê tudo até o próximo ponto e vírgula.
    while (i < banco->capacidadeArmazenamento && 
           fscanf(arquivoLeCSV, " %[^;];%d;%[^;];%[^;];%[^;];%[^;];%[^\n]", 
           banco->itens[i].resposta,
           &nivelTemporario, // Lê no endereço da variável int temporária
           banco->itens[i].dica1,
           banco->itens[i].dica2,
           banco->itens[i].dica3,
           banco->itens[i].dica4,
           banco->itens[i].dica5) == 7) 
    {
        banco->itens[i].nivel = (Dificuldade)nivelTemporario;
        banco->totalItens++;
        i++;
    }
    printf("Deu certo função leArquivoCSV\n");
    fclose(arquivoLeCSV);
    printf("[OK] Dados carregados do CSV. Total de itens: %d\n", banco->totalItens);
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
void inserirItem(BancoInformacoes* banco){
    if (banco == NULL) return;
    if (banco->totalItens >= banco->capacidadeArmazenamento){
        int novaCapacidade = banco->capacidadeArmazenamento * 2;
        Item* novoArray = realloc(banco->itens, novaCapacidade * sizeof(Item));

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
            printf("[Erro] O nível deve estar entre 1 e 5. Digite novamente:\n");
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
        printf("[Aviso] O banco de informaçôes está vazio.\n");
        return;
    }
    
    printf("====== LISTA DE ITENS CADASTRADOS ======\n");
    for (int i = 0; i < banco->totalItens; i++){
        Item *item = &banco->itens[i];
        printf("Item %d:\n", i + 1);
        printf("Nome: %s\n", item->resposta);
        printf("Categoria: %s\n", item->categoria);
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
        printf("[Aviso] O banco de informaçôes está vazio para alterações.\n");
        return;
    }

    char busca[TAM_MAX_RESPOSTA];
    printf("Digite o nome (resposta) do item que deseja alterar: ");
    lerString(busca, TAM_MAX_RESPOSTA);
    for (int i = 0; i < banco->totalItens; i++){
        if (strcmp(banco->itens[i].resposta, busca) == 0){
            printf("\nItem encontrado com sucesso! Informações atuais:\n");
            printf("Resposta: %s\n", banco->itens[i].resposta);
            printf("Categoria: %s\n", banco->itens[i].categoria);
            printf("Nível de Dificuldade: %d\n", banco->itens[i].nivel);

            printf("\nDeseja alterar a resposta? (digite s para sim e n para não): ");
            char opcao;
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) == 's'){
                printf("Nova resposta: ");
                lerString((*banco).itens[i].resposta, TAM_MAX_RESPOSTA);
            }

            printf("Deseja alterar a categoria? (digite s para sim e n para não): ");
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) == 's'){
                printf("Nova categoria:\n");
                lerString((*banco).itens[i].categoria, TAM_MAX_CATEGORIA);
            }

            printf("Deseja alterar o nível de dificuldade? (digite s para sim e n para não): ");
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
                banco->itens[i].nivel = (Dificuldade) novoNivel;
            }
            printf("\n[OK] Item alterado com sucesso!\n");
            return;
        }
    }
    printf("[Aviso] Nenhum item encontrado com o nome informado.\n");
}

/**
 * @brief 
 * 
 * @param banco 
 */
void pesquisaItem(BancoInformacoes *banco){
      
    if (banco == NULL || banco-> totalItens == 0){
        printf("[Aviso] O banco de informaçôes está vazio.\n");
        return;
    }
    char buscaTemporario[TAM_MAX_RESPOSTA];
    printf("Digite o nome do item a ser pesquisado.\n");
    lerString(buscaTemporario, TAM_MAX_RESPOSTA);
    for (int i = 0; i < (*banco).totalItens; i++){
        if (strcmp(banco->itens[i].resposta, buscaTemporario) == 0){
            printf("Item encontrado: %s\n", banco->itens[i].resposta);
        }
    }
    printf("[Aviso] Nenhum item encontrado com o nome informado.\n");
}

/**
 * @brief Exclui um item (registro) do banco de informações baseado na sua resposta.
 * * Esta função solicita ao usuário a 'resposta' (nome) do item que deseja excluir.
 * Se o item for encontrado, pede confirmação antes de realizar a exclusão.
 * A exclusão é feita deslocando os elementos subsequentes na lista para
 * preencher a lacuna e decrementando o total de itens.
 *
 * @param banco Um ponteiro para a estrutura BancoInformacoes que contém os itens.
 * Não deve ser NULL e deve ter totalItens > 0 para prosseguir.
 * * @return void
 */
void excluirItem(BancoInformacoes *banco){

    if (banco == NULL || banco->totalItens == 0){
        printf("[Aviso] O banco de informaçôes está vazio para exclusão.\n");
        return;
    }

    char busca[TAM_MAX_RESPOSTA];
    printf("Digite o nome (resposta) do item que deseja excluir:\n");
    lerString(busca, TAM_MAX_RESPOSTA);

    for (int i = 0; i < banco->totalItens; i++){
        if (strcmp(banco->itens[i].resposta, busca) == 0){
            printf("\nItem encontrado: %s\n", banco->itens[i].resposta);
            printf("Deseja realmente excluir este item? (digite s para sim e n para não): ");
            char opcao;
            scanf(" %c", &opcao);
            setbuf(stdin, NULL);
            if (tolower(opcao) != 's'){
                printf("[Aviso] Exclusão cancelada pelo usuário.\n");
                return;
            }

            // Desloca os itens seguintes para preencher o espaço
            for (int j = i; j < banco->totalItens - 1; j++){
                banco->itens[j] = banco->itens[j + 1];
            }
            banco->totalItens--;

            printf("[OK] Item excluído com sucesso! Total atual: %d\n", banco->totalItens);
            return;
        }
    }
    printf("[Aviso] Nenhum item encontrado com o nome informado.\n");
}

/**
 * @brief Salva todos os itens do banco em um arquivo binário.
 *
 * Esta função grava em disco a estrutura BancoInformacoes, incluindo:
 * - O número total de itens cadastrados.
 * - O conteúdo completo do array dinâmico de itens.
 *
 * O arquivo é salvo no modo binário ("wb") e pode ser recarregado
 * posteriormente pela função carregarItensBinario().
 *
 * @param banco Ponteiro para a estrutura BancoInformacoes a ser salva.
 * @param nomeArquivo Nome do arquivo binário (ex: "banco_itens.bin").
 *
 * @return void Esta função não retorna valor.
 */
void salvarItensBinario(BancoInformacoes *banco){
       
    if (banco == NULL || banco->totalItens == 0){
        printf("[Aviso] O banco de informaçôes está vazio.\n");
        return;
    }

    FILE *arquivo = fopen("jogoadvinhacao.dat", "wb");

    if (arquivo == NULL){
        printf("[Erro] Não foi possível abrir o arquivo para escrita.\n");
        exit(1);
    }

    fwrite(&banco->totalItens, sizeof(int), 1, arquivo);

    int itensGravados = fwrite(banco->itens, sizeof(Item), banco->totalItens, arquivo);
    if (itensGravados != banco->totalItens){
        printf("[Erro] Nem todos os itens foram salvos corretamente.\n");
    } else {
        printf("[OK] %d itens salvos com sucesso em 'jogoadvinhacao'.\n", banco->totalItens);
    }

    fclose(arquivo);
}

/**
 * @brief Carrega os itens previamente salvos de um arquivo binário.
 *
 * Esta função:
 * 1. Abre o arquivo binário no modo leitura ("rb").
 * 2. Lê o total de itens armazenados.
 * 3. Aloca dinamicamente o espaço necessário para todos os itens.
 * 4. Carrega os dados diretamente na estrutura BancoInformacoes.
 *
 * Caso o arquivo não exista ou haja erro de leitura, o banco é inicializado vazio.
 *
 * @param nomeArquivo Nome do arquivo binário (ex: "banco_itens.bin").
 *
 * @return BancoInformacoes* Retorna ponteiro para a estrutura BancoInformacoes carregada.
 */
BancoInformacoes* carregarItensBinario(const char *nomeArquivo){
    
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL){
        printf("[Aviso] Arquivo '%s' não encontrado. Um novo banco será criado.\n", nomeArquivo);
        return inicializarBanco(); // cria um banco vazio
    }

    int totalItensLidos;
    if (fread(&totalItensLidos, sizeof(int), 1, arquivo) != 1){
        printf("[Erro] Falha ao ler o número total de itens.\n");
        fclose(arquivo);
        return inicializarBanco();
    }

    BancoInformacoes *banco = malloc(sizeof(BancoInformacoes));
    if (banco == NULL){
        printf("[Erro] Falha na alocação de memória para o banco.\n");
        fclose(arquivo);
        return NULL;
    }

    banco->totalItens = totalItensLidos;
    banco->capacidadeArmazenamento = (totalItensLidos > 10) ? totalItensLidos : 10;
    banco->itens = malloc(banco->capacidadeArmazenamento * sizeof(Item));

    if (banco->itens == NULL){
        printf("[Erro] Falha na alocação de memória para os itens.\n");
        free(banco);
        fclose(arquivo);
        return NULL;
    }

    size_t itensLidos = fread(banco->itens, sizeof(Item), totalItensLidos, arquivo);
    if (itensLidos != (size_t)totalItensLidos){
        printf("[Aviso] Nem todos os itens foram lidos corretamente. O banco pode estar incompleto.\n");
    } else {
        printf("[OK] %d itens carregados com sucesso de '%s'.\n", totalItensLidos, nomeArquivo);
    }

    fclose(arquivo);
    return banco;
}

/**
 * @brief Adiciona o resultado de um jogador ao ranking e salva em arquivo.
 *
 * Esta função carrega o ranking atual do arquivo binário, insere o novo registro,
 * reordena a lista por pontuação (decrescente) e grava o resultado de volta no disco.
 * @note A função mantém apenas um número fixo de melhores pontuações (ex: TOP 10).
 * Se o novo score não for alto o suficiente, ele pode ser descartado.
 *
 * @param nome Nome do jogador a ser registrado (string).
 * @param pontuacao Pontuação total acumulada na sessão.
 * * @return void
 */
void salvarRanking(const char* nome, int pontuacao){
    PosicaoRanking ranking[MAX_RANKING_ENTRIES + 1];
    int totalEntries = 0;

    FILE *arq = fopen(ARQUIVO_RANKING, "rb");
    if (arq != NULL){
        totalEntries = fread(ranking, sizeof(PosicaoRanking), MAX_RANKING_ENTRIES, arq);
        fclose(arq);
    }

    PosicaoRanking novo;
    strncpy(novo.nome, nome, TAM_MAX_NOME - 1);
    novo.nome[TAM_MAX_NOME - 1] = '\0';
    novo.pontuacao = pontuacao;

    ranking[totalEntries] = novo;
    totalEntries++;

    //Aqui foi aplicado Bubble Sort (coisa de fora)
    for (int i = 0; i < totalEntries - 1; i++){
        for (int j = 0; j < totalEntries - i - 1; j++){
            if (ranking[j].pontuacao < ranking[j + 1].pontuacao){
                PosicaoRanking temp = ranking[j];
                ranking[j] = ranking[j + 1];
                ranking[j + 1] = temp;
            }
        }
    }

    //SÓ GRAVA OS TOP 10!!!
    int entriesParaGravar = (totalEntries > MAX_RANKING_ENTRIES) ? MAX_RANKING_ENTRIES : totalEntries;
    arq = fopen(ARQUIVO_RANKING, "wb");
    if (arq != NULL){
        fwrite(ranking, sizeof(PosicaoRanking), entriesParaGravar, arq);
        fclose(arq);
        printf("[Ranking] Pontuação salva! Você está na posição %d.\n", entriesParaGravar);
    } else {
        printf("[Erro] Falha ao salvar ranking.\n");
    }
}

/**
 * @brief Carrega o ranking do arquivo e exibe no terminal.
 *
 * Abre o arquivo binário de ranking, lê os registros armazenados e os imprime
 * na saída padrão em formato de tabela (Posição | Nome | Pontos).
 * Caso o arquivo não exista ou esteja vazio, exibe uma mensagem informando.
 * * @return void
 */
void listarRanking(){
    PosicaoRanking ranking[MAX_RANKING_ENTRIES];
    int totalEntries = 0;

    FILE *arq = fopen(ARQUIVO_RANKING, "rb");
    if (arq != NULL) {
        totalEntries = fread(ranking, sizeof(PosicaoRanking), MAX_RANKING_ENTRIES, arq);
        fclose(arq);
    }

    printf("\n ====== HALL DA FAMA (TOP %d) ====== \n", MAX_RANKING_ENTRIES);
    if (totalEntries == 0) {
        printf("   [ Aviso ] Nenhum registro encontrado.\n");
    } else {
        printf("Pos | %-30s | Pontos\n", "Nome");
        printf("---------------------------------------------\n");
        for (int i = 0; i < totalEntries; i++) {
            printf("#%02d | %-30s | %d\n", i + 1, ranking[i].nome, ranking[i].pontuacao);
        }
    }
    printf("=============================================\n\n");
}

/**
 * @brief Apaga permanentemente todo o histórico de ranking.
 *
 * Esta função remove o arquivo binário onde os recordes estão armazenados ("ranking.dat").
 * É uma operação irreversível utilizada para zerar o placar do jogo.
 * Geralmente inclui uma etapa de confirmação do usuário antes de deletar o arquivo.
 * * @return void
 */
void formatarRanking(){
    
    FILE* arquivoRanking = fopen(ARQUIVO_RANKING, "r+");
    char confirmacao;
    printf("\n  ATENÇÃO: Isso apagará todo o histórico de recordes.\n");
    printf("Tem certeza? (S/N): ");
    
    setbuf(stdin, NULL);
    scanf("%c", &confirmacao);
    
    if (confirmacao == 's' || confirmacao == 'S'){
        // Remove o arquivo e verifica o resultado NA MESMA LINHA
        if (remove(ARQUIVO_RANKING) == 0){
            printf("[OK] Ranking formatado com sucesso.\n");
        } else {
            printf("[Aviso] O arquivo de ranking não existia ou não pôde ser apagado.\n");
        }
    } else {
        printf("[Ação] Formatação cancelada.\n");
    }

    fclose(arquivoRanking);
   // return banco;
}

