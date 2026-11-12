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
    char nome[100];
    char cargo[50];
    enum Situacao situacao; 
    enum Papel papel; 
} Suspeito;

void carregar_suspeito(ListaDuplaCircular *lista, const char *nome, const char *cargo, int situacao, int papel);
int contar_vivos(ListaDuplaCircular *lista);
#endif