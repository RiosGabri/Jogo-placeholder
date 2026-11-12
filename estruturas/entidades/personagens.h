#ifndef PERSONAGENS_H
#define PERSONAGENS_H
#include "entidades/suspeito.h"

enum Suspeitos {
    ID_GERENTE = 0,
    ID_CAPATAZ = 1,
    ID_ENGENHEIRO = 2,
    ID_OPERARIO = 3,
    ID_ENFERMEIRA = 4,
    ID_VIGIA = 5,
    TOTAL_SUSPEITOS = 6
};

enum Detetives{
    ID_MISS_MARPLE = 0,
    ID_HERCULE_POIROT = 1
};

#define NENHUM -1

typedef struct {
    int id_assassino;
    int id_cumplice;
} SorteioPapeis;

int sortear_papeis(int *ids, int quantidade, SorteioPapeis *sorteio);
enum Papel papel_do_suspeito(int id, SorteioPapeis sorteio);
#endif