#include "raylib.h"

#define LARGURA_TELA 960
#define ALTURA_TELA 540
#define TITULO_JOGO "Último Turno"
#define TAMANHO_TITULO 64

#define TOTAL_BOTOES 4
#define LARGURA_BOTAO 300
#define ALTURA_BOTAO 56
#define ESPACO_BOTOES 20
#define TAMANHO_TEXTO_BOTAO 24

static const char *ROTULOS[TOTAL_BOTOES] = {"Iniciar", "Configurações", "Trilha", "Sair"};

static Rectangle obter_retangulo_botao(int indice) {
    float x = (LARGURA_TELA - LARGURA_BOTAO) / 2.0f;
    float y = 200.0f + indice * (ALTURA_BOTAO + ESPACO_BOTOES);
    return (Rectangle){x, y, LARGURA_BOTAO, ALTURA_BOTAO};
}

static void desenhar_titulo(void) {
    int largura = MeasureText(TITULO_JOGO, TAMANHO_TITULO);
    DrawText(TITULO_JOGO, (LARGURA_TELA - largura) / 2, 90, TAMANHO_TITULO, RAYWHITE);
}

static void desenhar_botao(int indice, bool destacado) {
    Rectangle retangulo = obter_retangulo_botao(indice);
    Color fundo = destacado ? (Color){80, 80, 110, 255} : (Color){45, 45, 60, 255};
    DrawRectangleRec(retangulo, fundo);
    DrawRectangleLinesEx(retangulo, 2, destacado ? RAYWHITE : GRAY);

    int largura_texto = MeasureText(ROTULOS[indice], TAMANHO_TEXTO_BOTAO);
    int x_texto = (int)(retangulo.x + (retangulo.width - largura_texto) / 2);
    int y_texto = (int)(retangulo.y + (retangulo.height - TAMANHO_TEXTO_BOTAO) / 2);
    DrawText(ROTULOS[indice], x_texto, y_texto, TAMANHO_TEXTO_BOTAO, RAYWHITE);
}

static int atualizar_selecao(int selecionado) {
    if (IsKeyPressed(KEY_DOWN)) {
        selecionado = (selecionado + 1) % TOTAL_BOTOES;
    }
    if (IsKeyPressed(KEY_UP)) {
        selecionado = (selecionado <= 0) ? TOTAL_BOTOES - 1 : selecionado - 1;
    }

    Vector2 movimento = GetMouseDelta();
    if (movimento.x != 0.0f || movimento.y != 0.0f) {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < TOTAL_BOTOES; i++) {
            if (CheckCollisionPointRec(mouse, obter_retangulo_botao(i))) {
                selecionado = i;
            }
        }
    }

    return selecionado;
}

int main(void) {
    InitWindow(LARGURA_TELA, ALTURA_TELA, TITULO_JOGO);
    SetTargetFPS(60);

    int selecionado = -1;

    while (!WindowShouldClose()) {
        selecionado = atualizar_selecao(selecionado);

        BeginDrawing();
        ClearBackground((Color){20, 20, 28, 255});
        desenhar_titulo();
        for (int i = 0; i < TOTAL_BOTOES; i++) {
            desenhar_botao(i, i == selecionado);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}