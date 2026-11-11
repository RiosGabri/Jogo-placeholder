#ifndef SUSPEITO_H
#define SUSPEITO_H
#include "lista-dupla-circular.h"

typedef struct {
    int id;
    char nome[100];
    char cargo[50];
    int situacao; // 0 - vivo, 1 - preso, 2 - morto
    int papel; // 0 - inocente, 1 - vitma, 2 - assassino
} Suspeito;

void carregar_suspeito(ListaDuplaCircular *lista, const char *nome, const char *cargo, int situacao, int papel);
int contar_vivos(ListaDuplaCircular *lista);
#endif