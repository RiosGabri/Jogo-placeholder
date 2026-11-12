
#include "dia.h"

void iniciar_dia(EstadoDia *estado) {
    estado->dia_atual = 1;
    estado->fase_atual = FASE_MANHA;
}

void avancar_fase(EstadoDia *estado) {
    switch (estado->fase_atual) {

        case FASE_MANHA:
            estado->fase_atual = FASE_DEBATE;
            break;

        case FASE_DEBATE:
            estado->fase_atual = FASE_JULGAMENTO;
            break;

        case FASE_JULGAMENTO:
            if (estado->dia_atual >= TOTAL_DIAS) {
                estado->fase_atual = FASE_FINALIZADO;
            } else {
                estado->fase_atual = FASE_NOITE;
            }
            break;

        case FASE_NOITE:
            if (estado->dia_atual < TOTAL_DIAS) {
                estado->dia_atual++;
                estado->fase_atual = FASE_MANHA;
            } else {
                estado->fase_atual = FASE_FINALIZADO;
            }
            break;

        case FASE_FINALIZADO:
            break;
    }
}
