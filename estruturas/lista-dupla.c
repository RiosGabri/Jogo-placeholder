#include "lista-dupla.h"
#include <stdlib.h>
#include <stdio.h>


ListaDupla *criar_lista(void){
    ListaDupla *lista = malloc(sizeof(ListaDupla));
    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

int inserir_no_fim(ListaDupla *lista, int id, void *dado) {
    if (lista == NULL || dado == NULL) {
        return 0;
    }

    NoListaDupla *novo = malloc(sizeof(NoListaDupla));
    if (novo == NULL) {
        return 0;
    }

    novo->id = id;
    novo->dado = dado;
    novo->prox = NULL;
    novo->ant = lista->fim;

    if (lista->fim == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->prox = novo;
    }
    lista->fim = novo;

    return 1;
}

void *buscar_por_id(const ListaDupla *lista, int id) {
    if (lista == NULL) {
        return NULL;
    }

    for (const NoListaDupla *atual = lista->inicio; atual != NULL; atual = atual->prox) {
        if (atual->id == id) {
            return atual->dado;
        }
    }

    return NULL;
}

void *remover(ListaDupla *lista, int id) {
    if (lista == NULL) {
        return NULL;
    }

    NoListaDupla *atual = lista->inicio;

    while (atual != NULL && atual->id != id) {
        atual = atual->prox;
    }

    if (atual == NULL) {
        return NULL;
    }

    if (atual->ant == NULL) {
        lista->inicio = atual->prox;
    } else {
        atual->ant->prox = atual->prox;
    }

    if (atual->prox == NULL) {
        lista->fim = atual->ant;
    } else {
        atual->prox->ant = atual->ant;
    }

    void *dado = atual->dado;
    free(atual);
    return dado;
}

void listar(const ListaDupla *lista) {
    NoListaDupla *atual = lista->inicio;
    while (atual != NULL) {
        printf("ID: %d, Dado: %p\n", atual->id, atual->dado);
        atual = atual->prox;
    }
}

void liberar_lista(ListaDupla *lista) {
    if (lista == NULL) {
        return;
    }

    NoListaDupla *atual = lista->inicio;
    while (atual != NULL) {
        NoListaDupla *proximo = atual->prox;
        free(atual);
        atual = proximo;
    }

    free(lista);
}

void trocar_dados(ListaDupla *lista, int id1, int id2) {
    if (lista == NULL) {
        return;
    }

    NoListaDupla *no1 = NULL;
    NoListaDupla *no2 = NULL;

    for (NoListaDupla *atual = lista->inicio; atual != NULL; atual = atual->prox) {
        if (atual->id == id1) {
            no1 = atual;
        } else if (atual->id == id2) {
            no2 = atual;
        }
        if (no1 != NULL && no2 != NULL) {
            break;
        }
    }

    if (no1 != NULL && no2 != NULL) {
        void *temp = no1->dado;
        no1->dado = no2->dado;
        no2->dado = temp;
    }
}