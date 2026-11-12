
#ifndef DIA_H
#define DIA_H

#define TOTAL_DIAS 4

typedef enum {
    FASE_MANHA,
    FASE_DEBATE,
    FASE_JULGAMENTO,
    FASE_NOITE,
    FASE_FINALIZADO
} FaseDia;

typedef struct {
    int dia_atual;
    FaseDia fase_atual;
} EstadoDia;

void iniciar_dia(EstadoDia *estado);
void avancar_fase(EstadoDia *estado);

#endif
