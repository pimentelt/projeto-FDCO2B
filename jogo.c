/**
 * @file jogo.c
 * @author Tarsila Paiva Pimentel
 * @brief Implementação principal do modo de jogo multiplayer (adivinhação de itens).
 * Este arquivo contém as definições de constantes de pontuação e penalidade, 
 * e a lógica completa das funções de suporte e do fluxo principal do jogo multiplayer, 
 * incluindo a inicialização de jogadores, o ciclo de rodadas e a interação de turnos.
 * @version 1.0
 * @version 0.1
 * @date 2025-11-26
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>//Para uso do booleano mais abaixo.
#include <ctype.h> 
#include "funcoes.h"

#define PONTOS_ACERTOS 100
#define PENALIDADE_LETRA 20
#define PENALIDADE_PULAR 30
#define NUM_MAX_TENTATIVAS 5
#define MAX_ACOES 10

/**
 * @brief Revela a próxima letra oculta (sublinhado) na máscara de resposta.
 *
 * Esta função percorre a máscara de resposta (@c mascara) e localiza a primeira 
 * ocorrência de um sublinhado ('_') que não corresponde a um espaço (' ') 
 * na resposta original (@c resposta). Ao encontrar, substitui o sublinhado
 * na máscara pela letra correspondente da resposta, revelando-a ao jogador.
 *
 * @param resposta Ponteiro constante para a string que contém a resposta 
 * correta completa (a ser adivinhada).
 * @param mascara Ponteiro para a string que representa a resposta atual 
 * com letras ocultas (sublinhados). Esta string será modificada.
 *
 * @note A função prioriza a revelação de letras na ordem em que aparecem 
 * na string, da esquerda para a direita.
 * @note Espaços (' ') não são considerados caracteres a serem revelados 
 * (não são substituídos por '_').
 */
void revelarLetra(const char* resposta, char* mascara) {
    int len = strlen(resposta);
    int indice = -1;

    for (int i = 0; i < len; i++){
        if (resposta[i] != ' ' && mascara[i] == '_') {
            indice = i;
            break;
        }
    }
    if (indice != -1){
        mascara[indice] = resposta[indice];
        printf("[OK] Letra '%c' revelada!\n", mascara[indice]);
    } else {
        printf("[Aviso] Não há mais letras para revelar.\n");
    }
}

/**
 * @brief Inicia e gerencia uma sessão de jogo no modo multiplayer.
 *
 * Esta função permite que múltiplos jogadores tentem adivinhar uma 
 * resposta baseada em dicas, alternando as rodadas entre eles.
 * * O jogo prossegue em ciclos de itens (rodadas), onde um item aleatório 
 * é selecionado e os jogadores se revezam para tentar adivinhar a 
 * resposta ou pedir dicas/ajudas, até que o item seja adivinhado ou 
 * todas as tentativas esgotem.
 * * @param banco Ponteiro para a estrutura BancoInformacoes que contém 
 * todos os itens disponíveis para o jogo.
 * * @warning Requer que as constantes globais TAM_MAX_NOME, MAX_JOGADORES_SESSAO, 
 * @c PONTOS_ACERTOS, @c NUM_MAX_TENTATIVAS, @c PENALIDADE_LETRA, e 
 * @c PENALIDADE_PULAR estejam definidas. Requer as estruturas @c JogadorSessao, 
 * @c BancoInformacoes, e @c Item, além das funções @c lerString, 
 * @c setbuf, @c obterItemAleatorio, @c removerEspacos, @c revelarLetra.
 * * @note O ciclo de jogo continua até que o grupo decida sair.
 * @note Cada jogador tem um número limitado de tentativas (@c NUM_MAX_TENTATIVAS) por item.
 */
void jogarMultiplayer(BancoInformacoes* banco){
    // ETAPA 1: VERIFICAÇÃO INICIAL (COMENTEI TUDO ASSIM PORQUE ESSA FUNÇAO É ENORME)
    if (banco == NULL || banco->totalItens == 0){
        printf("[Aviso] Não há itens cadastrados para jogar.\n");
        return;
    }

    JogadorSessao jogadores[MAX_JOGADORES_SESSAO]; 
    int numeroJogadores = 0;
    
    // ETAPA 2: INICIALIZAÇÃO DE JOGADORES
    printf("\n==== INICIALIZAÇÃO DE JOGADORES ====\n");
    do {
        printf("Quantos jogadores? (1 a %d): \n> ", MAX_JOGADORES_SESSAO);
        if (scanf("%d", &numeroJogadores) != 1) { 
            numeroJogadores = 0;
        }
        setbuf(stdin, NULL);
        if (numeroJogadores < 1 || numeroJogadores > MAX_JOGADORES_SESSAO) 
            printf("[Aviso] Quantidade inválida.\n");
    } while (numeroJogadores < 1 || numeroJogadores > MAX_JOGADORES_SESSAO);

    for (int i = 0; i < numeroJogadores; i++){
        printf("Nome do Jogador %d: \n> ", i + 1);
        lerString(jogadores[i].nome, TAM_MAX_NOME); 
        jogadores[i].pontuacaoSessao = 0;
    }

    char acao[TAM_MAX_NOME];
    char tentativa[TAM_MAX_RESPOSTA];

    // ETAPA 3: LOOP PRINCIPAL DE ITENS (RODADAS)
    do {
        Item* itemAdivinhar = obterItemAleatorio(banco);
        if (itemAdivinhar == NULL) return; 

        char mascaraResposta[TAM_MAX_RESPOSTA];
        int dicaAtual = 1;
        int pontuacaoRodada = PONTOS_ACERTOS; 
        bool itemAdivinhado = false;
        
        int tamanhoResposta = strlen(itemAdivinhar->resposta);
        for (int k = 0; k < tamanhoResposta; k++){
            mascaraResposta[k] = (itemAdivinhar->resposta[k] == ' ') ? ' ' : '_';
        }
        mascaraResposta[tamanhoResposta] = '\0';
        
        for(int i = 0; i < numeroJogadores; i++){
            jogadores[i].tentativasNoItem = NUM_MAX_TENTATIVAS;
        }

        printf("\n\n==== NOVO ITEM: Rodada Base de %d pontos! ====\n", PONTOS_ACERTOS);
        printf("ADIVINHE: %s\n", mascaraResposta);

        int indiceJogadorAtual = 0; 

        // ETAPA 4: LOOP DE TURNOS DE ADIVINHAÇÃO
        while (!itemAdivinhado){
            
            // Define quem joga agora
            JogadorSessao* jogadorAtual = &jogadores[indiceJogadorAtual]; 

            // Verifica se ALGUÉM ainda tem tentativas
            int totalTentativasRestantes = 0;
            for(int i = 0; i < numeroJogadores; i++){
                totalTentativasRestantes += jogadores[i].tentativasNoItem;
            }
            if (totalTentativasRestantes == 0){
                printf("\nFIM! Ninguém tem mais tentativas. A resposta era: %s\n", itemAdivinhar->resposta);
                break;
            }

            if (jogadorAtual->tentativasNoItem <= 0){ 
                indiceJogadorAtual = (indiceJogadorAtual + 1) % numeroJogadores; 
                continue; 
            }
            
            printf("\n--- VEZ DE: **%s** ---\n", jogadorAtual->nome);
            printf("Tentativas Restantes: %d | Valendo: %d pts | Seu Total: %d\n", 
                    jogadorAtual->tentativasNoItem, pontuacaoRodada, jogadorAtual->pontuacaoSessao);
            printf("Progresso: %s\n", mascaraResposta);
            printf("=> DICA %d: ", dicaAtual);
            switch (dicaAtual){
                case 1: printf("%s\n", itemAdivinhar->dica1); break;
                case 2: printf("%s\n", itemAdivinhar->dica2); break;
                case 3: printf("%s\n", itemAdivinhar->dica3); break;
                case 4: printf("%s\n", itemAdivinhar->dica4); break;
                case 5: printf("%s\n", itemAdivinhar->dica5); break;
                default: printf("Todas as dicas foram reveladas!\n"); break;
            }

            // 4.1: Escolha de Ação (RESPONDER | DICA | PULAR)
            printf("\nEscolha sua Ação (RESPONDER | DICA | PULAR):\n> ");
            lerString(acao, TAM_MAX_NOME);
            removerEspacos(acao);
            
            if (strcmp(acao, "responder") == 0){
                printf("Qual é o seu palpite? \n> ");
                lerString(tentativa, TAM_MAX_RESPOSTA); 
                removerEspacos(tentativa);
                
                if (strcmp(tentativa, itemAdivinhar->resposta) == 0){
                    printf("\n--- PARABÉNS, %s! Você acertou: **%s** ---\n", jogadorAtual->nome, itemAdivinhar->resposta);
                    jogadorAtual->pontuacaoSessao += pontuacaoRodada;
                    itemAdivinhado = true; 
                } else {
                    printf("[Incorreto] Palpite errado.\n");
                    jogadorAtual->tentativasNoItem--;
                    if(dicaAtual < 5) dicaAtual++;
                }

            } else if (strcmp(acao, "dica") == 0){
                printf("\n--- MENU DE AJUDAS ---\n");
                printf("1. Revelar Letra (-%d pts, -1 Tentativa)\n", PENALIDADE_LETRA); 
                printf("2. Nova Dica (Libera a próxima)\n");
                printf("3. Cancelar\n");
                printf("Escolha uma opção (1-3):\n> ");
    
                int opcao_dica;
                if (scanf("%d", &opcao_dica) != 1){ setbuf(stdin, NULL); opcao_dica = -1; } else { setbuf(stdin, NULL); }
                
                if (opcao_dica == 1){
                    revelarLetra(itemAdivinhar->resposta, mascaraResposta);
                    pontuacaoRodada -= PENALIDADE_LETRA;
                    if(pontuacaoRodada < 0) pontuacaoRodada = 0;
                    jogadorAtual->tentativasNoItem--;
                } else if (opcao_dica == 2){
                    if (dicaAtual < 5){
                        dicaAtual++;
                        printf("[OK] Próxima Dica liberada.\n");
                    } else {
                        printf("[Aviso] Sem mais dicas.\n");
                    }
                } else if (opcao_dica == 3){
                    printf("[Ação] Cancelada.\n");
                } else {
                    printf("[Erro] Inválido.\n");
                }

            } else if (strcmp(acao, "pular") == 0){
                printf("[Atenção] Pular custa %d pontos do seu total. Confirmar? (s/n)\n> ", PENALIDADE_PULAR);
                lerString(acao, TAM_MAX_NOME);
                if (acao[0] == 's'){
                    jogadorAtual->pontuacaoSessao -= PENALIDADE_PULAR;
                    printf("Penalidade aplicada. Vez passada.\n");
                } else {
                    printf("[Ação] Cancelada. Sua vez continua.\n");
                    continue;
                }
            } else {
                printf("[Aviso] Ação inválida. Tente novamente.\n");
                continue;
            }

            if (!itemAdivinhado){
                indiceJogadorAtual = (indiceJogadorAtual + 1) % numeroJogadores;
            }
        }

        if (!itemAdivinhado){
            printf("\n--- A resposta correta era: **%s** ---\n", itemAdivinhar->resposta);
        }

        // ETAPA 5: FIM DO ITEM E OPÇÃO DE CONTINUIDADE
        printf("\nO grupo quer continuar? (N - Novo Personagem / S - Sair do Jogo)\n> ");
        lerString(acao, TAM_MAX_NOME);
        if (acao[0] == 's'){
            break;
        }

    // ETAPA 6: ENCERRAMENTO DA SESSÃO
    } while (1);
    printf("\n==== FIM DA SESSÃO DE JOGO ====\n");
    for (int i = 0; i < numeroJogadores; i++){
        printf("Pontuação final de **%s**: %d\n", jogadores[i].nome, jogadores[i].pontuacaoSessao);
        if (jogadores[i].pontuacaoSessao > 0){
            salvarRanking(jogadores[i].nome, jogadores[i].pontuacaoSessao);
        }
    }
}
