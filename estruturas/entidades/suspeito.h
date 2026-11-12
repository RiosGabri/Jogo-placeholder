#ifndef SUSPEITO_H
#define SUSPEITO_H
#include "listas/lista-dupla-circular.h"

enum Situacao {
    VIVO,
    PRESO,
    MORTO
};

enum Papel {
    INOCENTE,
    VITIMA,
    ASSASSINO
};

typedef struct {
    int id;
    char *nome;
    char *cargo;
    enum Situacao situacao; 
    enum Papel papel; 
} Suspeito;

void carregar_suspeito(ListaDuplaCircular *lista, int id, char *nome, char *cargo, enum Situacao situacao, enum Papel papel);
int contar_vivos(ListaDuplaCircular *lista);
void liberar_suspeitos(ListaDuplaCircular *lista);
#endif