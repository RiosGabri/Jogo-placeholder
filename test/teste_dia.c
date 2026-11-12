
#include <stdio.h>
#include "dia.h"

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

    return 0;
}
