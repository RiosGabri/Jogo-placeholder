#include "lista-dupla-circular.h"
#include <stdlib.h>
#include <stdio.h>

ListaDuplaCircular *criar_lista(void) {
    ListaDuplaCircular *lista = malloc(sizeof(ListaDuplaCircular));
    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

int inserir_no_fim(ListaDuplaCircular *lista, int id, void *dado) {
    if (lista == NULL || dado == NULL) {
        return 0;
    }

    NoListaDuplaCircular *novo = malloc(sizeof(NoListaDuplaCircular));
    if (novo == NULL) {
        return 0;
    }

    novo->id = id;
    novo->dado = dado;

    if (lista->inicio == NULL) {
        novo->prox = novo;
        novo->ant = novo;
        lista->inicio = novo;
        lista->fim = novo;
    } else {
        novo->prox = lista->inicio;
        novo->ant = lista->fim;
        lista->fim->prox = novo;
        lista->inicio->ant = novo;
        lista->fim = novo;
    }

    return 1;
}

void *buscar_por_id(const ListaDuplaCircular *lista, int id) {
    if (lista == NULL || lista->inicio == NULL) {
        return NULL;
    }

    NoListaDuplaCircular *atual = lista->inicio;
    do {
        if (atual->id == id) {
            return atual->dado;
        }
        atual = atual->prox;
    } while (atual != lista->inicio);

    return NULL;
}

void *remover(ListaDuplaCircular *lista, int id) {
    if (lista == NULL || lista->inicio == NULL) {
        return NULL;
    }

    NoListaDuplaCircular *atual = lista->inicio;
    do {
        if (atual->id == id) {
            if (atual->prox == atual) {
                lista->inicio = NULL;
                lista->fim = NULL;
            } else {
                atual->ant->prox = atual->prox;
                atual->prox->ant = atual->ant;
                if (atual == lista->inicio) {
                    lista->inicio = atual->prox;
                }
                if (atual == lista->fim) {
                    lista->fim = atual->ant;
                }
            }
            void *dado = atual->dado;
            free(atual);
            return dado;
        }
        atual = atual->prox;
    } while (atual != lista->inicio);

    return NULL; // Node with the given id not found
}

void listar(const ListaDuplaCircular *lista) {
    if (lista == NULL || lista->inicio == NULL) {
        return;
    }

    NoListaDuplaCircular *atual = lista->inicio;
    do {
        printf("ID: %d, Dado: %p\n", atual->id, atual->dado);
        atual = atual->prox;
    } while (atual != lista->inicio);
}

void liberar_lista(ListaDuplaCircular *lista) {
    if (lista == NULL || lista->inicio == NULL) {
        free(lista);
        return;
    }

    NoListaDuplaCircular *atual = lista->inicio;
    do {
        NoListaDuplaCircular *temp = atual;
        atual = atual->prox;
        free(temp);
    } while (atual != lista->inicio);

    free(lista);
}

void trocar_dados(ListaDuplaCircular *lista, int id1, int id2) {
    if (lista == NULL || lista->inicio == NULL) {
        return;
    }

    NoListaDuplaCircular *no1 = NULL;
    NoListaDuplaCircular *no2 = NULL;

    NoListaDuplaCircular *atual = lista->inicio;
    do {
        if (atual->id == id1) {
            no1 = atual;
        } else if (atual->id == id2) {
            no2 = atual;
        }
        atual = atual->prox;
    } while (atual != lista->inicio && (no1 == NULL || no2 == NULL));

    if (no1 != NULL && no2 != NULL) {
        void *tempDado = no1->dado;
        no1->dado = no2->dado;
        no2->dado = tempDado;
    }
}