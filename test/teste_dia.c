
#include <stdio.h>
#include <assert.h>
#include "dia.h"

void testar_jogo_finalizado(void) {
    EstadoDia estado;

    estado.dia_atual = 4;
    estado.fase_atual = FASE_FINALIZADO;

    avancar_fase(&estado);

    assert(estado.dia_atual == 4);
    assert(estado.fase_atual == FASE_FINALIZADO);

    printf("Teste 1 - Jogo finalizado: PASSOU!\n");
}

void testar_noite_ultimo_dia(void) {
    EstadoDia estado;

    estado.dia_atual = 4;
    estado.fase_atual = FASE_NOITE;

    avancar_fase(&estado);

    assert(estado.dia_atual == 4);
    assert(estado.fase_atual == FASE_FINALIZADO);

    printf("Teste 2 - Noite no ultimo dia: PASSOU!\n");
}

int main(void) {
    EstadoDia estado;

    iniciar_dia(&estado);

    while (estado.fase_atual != FASE_FINALIZADO) {
        printf("Dia: %d | Fase: %d\n",
               estado.dia_atual,
               estado.fase_atual);

        avancar_fase(&estado);
    }

    printf("Jogo finalizado no dia %d!\n",
           estado.dia_atual);

    testar_jogo_finalizado();
    testar_noite_ultimo_dia();

    return 0;
}
