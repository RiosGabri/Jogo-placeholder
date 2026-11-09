#include "lista.h"
#include <stdlib.h>

Lista *criar_lista(void) {
    Lista *lista = malloc(sizeof(Lista));
    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

int inserir_no_fim(Lista *lista, int id, void *dado) {
    if (lista == NULL || dado == NULL) {
        return 0;
    }

    NoLista *novo = malloc(sizeof(NoLista));
    if (novo == NULL) {
        return 0;
    }

    novo->id = id;
    novo->dado = dado;
    novo->prox = NULL;

    if (lista->fim == NULL) {
        lista->inicio = novo;
    } else {
        lista->fim->prox = novo;
    }
    lista->fim = novo;

    return 1;
}

void *buscar_por_id(const Lista *lista, int id) {
    if (lista == NULL) {
        return NULL;
    }

    for (const NoLista *atual = lista->inicio; atual != NULL; atual = atual->prox) {
        if (atual->id == id) {
            return atual->dado;
        }
    }

    return NULL;
}

void *remover(Lista *lista, int id) {
    if (lista == NULL) {
        return NULL;
    }

    NoLista *anterior = NULL;
    NoLista *atual = lista->inicio;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        return NULL;
    }

    if (anterior == NULL) {
        lista->inicio = atual->prox;
    } else {
        anterior->prox = atual->prox;
    }

    if (atual == lista->fim) {
        lista->fim = anterior;
    }

    void *dado = atual->dado;
    free(atual);
    return dado;
}

void listar(const Lista *lista, void (*imprimir)(const void *dado)) {
    if (lista == NULL || imprimir == NULL) {
        return;
    }

    for (const NoLista *atual = lista->inicio; atual != NULL; atual = atual->prox) {
        imprimir(atual->dado);
    }
}

void liberar_lista(Lista *lista) {
    if (lista == NULL) {
        return;
    }

    NoLista *atual = lista->inicio;
    while (atual != NULL) {
        NoLista *prox = atual->prox;
        free(atual);
        atual = prox;
    }

    free(lista);
}
