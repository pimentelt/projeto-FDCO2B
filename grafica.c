#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "grafica.h"

/**
 * @author Maria Julia Ferraz Rocha 
 * @brief Implementação da interface gráfica do jogo Perfil utilizando a biblioteca Raylib.
 * @file grafica.c
 * 
 */
typedef enum { TELA_MENU, TELA_JOGO, TELA_RESULTADO } EstadoGrafico;

void iniciarJogoGrafico(BancoInformacoes* banco) {
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Perfil - Trabalho Final");
    SetTargetFPS(60);

    EstadoGrafico estado = TELA_MENU;
    int indiceSorteado = 0;
    int dicasAbertas = 1;
    char inputTexto[100] = "\0";
    int letrasCount = 0;
    char feedback[100] = "";

    while (!WindowShouldClose()) {
        switch (estado) {
            case TELA_MENU:
                if (IsKeyPressed(KEY_ENTER)) {
                    if (banco->totalItens > 0) indiceSorteado = GetRandomValue(0, banco->totalItens - 1);
                    dicasAbertas = 1;
                    letrasCount = 0;
                    inputTexto[0] = '\0';
                    strcpy(feedback, "Digite e aperte ENTER");
                    estado = TELA_JOGO;
                }
                break;

            case TELA_JOGO:
                int key = GetCharPressed();
                while (key > 0) {
                    if ((key >= 32) && (key <= 125) && (letrasCount < 99)) {
                        inputTexto[letrasCount] = (char)key;
                        inputTexto[letrasCount+1] = '\0';
                        letrasCount++;
                    }
                    key = GetCharPressed();
                }
                if (IsKeyPressed(KEY_BACKSPACE)) {
                    letrasCount--;
                    if (letrasCount < 0) letrasCount = 0;
                    inputTexto[letrasCount] = '\0';
                }
                if (IsKeyPressed(KEY_TAB)) {
                    if (dicasAbertas < 5) dicasAbertas++;
                }
                if (IsKeyPressed(KEY_ENTER)) {
                    Item *item = &banco->itens[indiceSorteado];
                    char inputLow[100], respLow[100];
                    strcpy(inputLow, inputTexto);
                    strcpy(respLow, item->resposta);
                    for(int i=0; inputLow[i]; i++) inputLow[i] = tolower(inputLow[i]);
                    for(int i=0; respLow[i]; i++) respLow[i] = tolower(respLow[i]);

                    if (strcmp(inputLow, respLow) == 0) estado = TELA_RESULTADO;
                    else {
                        strcpy(feedback, "ERROU! Tente de novo.");
                        letrasCount = 0; inputTexto[0] = '\0';
                    }
                }
                break;

            case TELA_RESULTADO:
                if (IsKeyPressed(KEY_ENTER)) estado = TELA_MENU;
                break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estado == TELA_MENU) {
            DrawText("JOGO PERFIL", 250, 200, 50, DARKBLUE);
            DrawText("Pressione ENTER para comecar", 230, 300, 20, DARKGRAY);
            DrawText(TextFormat("Itens: %d", banco->totalItens), 10, 570, 20, LIGHTGRAY);
        }
        else if (estado == TELA_JOGO) {
            Item *item = &banco->itens[indiceSorteado];
            DrawText("QUEM SOU EU?", 50, 30, 20, BLACK);
            int y = 70;
            if (dicasAbertas >= 1) DrawText(TextFormat("1. %s", item->dica1), 50, y, 20, DARKGRAY); y+=30;
            if (dicasAbertas >= 2) DrawText(TextFormat("2. %s", item->dica2), 50, y, 20, DARKGRAY); y+=30;
            if (dicasAbertas >= 3) DrawText(TextFormat("3. %s", item->dica3), 50, y, 20, DARKGRAY); y+=30;
            if (dicasAbertas >= 4) DrawText(TextFormat("4. %s", item->dica4), 50, y, 20, DARKGRAY); y+=30;
            if (dicasAbertas >= 5) DrawText(TextFormat("5. %s", item->dica5), 50, y, 20, DARKGRAY);
            DrawText("Sua Resposta (TAB para dica):", 50, 400, 20, BLACK);
            DrawRectangleLines(50, 430, 600, 40, BLACK);
            DrawText(inputTexto, 55, 440, 20, MAROON);
            DrawText(feedback, 50, 500, 20, RED);
        }
        else if (estado == TELA_RESULTADO) {
            DrawText("ACERTOU!", 280, 250, 40, GREEN);
            DrawText("Enter para voltar", 300, 400, 20, GRAY);
        }
        EndDrawing();
    }
    CloseWindow();
}