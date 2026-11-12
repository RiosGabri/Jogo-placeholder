#include "personagens.h"
#include <stdlib.h>

int sortear_papeis(int *ids, int quantidade, SorteioPapeis *sorteio) {
    if (ids == NULL || sorteio == NULL || quantidade < 2) {
        return 0;   // precisa de pelo menos 2 suspeitos
    }

    int i = rand() % quantidade; //assassino 
    int j = rand() % (quantidade - 1);//cúmplice, entre os "restantes"

    /* O truque: se o número do cúmplice for >= ao do assassino,
       pula uma posição. Assim ele nunca cai no mesmo id. */
    if (j >= i) {
        j++;
    }

    sorteio->id_assassino = ids[i];
    sorteio->id_cumplice  = ids[j];
    return 1;
}

enum Papel papel_do_suspeito(int id, SorteioPapeis sorteio) {
    if (id == sorteio.id_assassino) {
        return ASSASSINO;
    }
    if (id == sorteio.id_cumplice) {
        return CUMPLICE;
    }
    return INOCENTE;
}