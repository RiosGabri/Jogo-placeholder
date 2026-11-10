#include "lista-simples.h"
#include <stdlib.h>

Lista *criar_lista_simples(void) {
    Lista *lista = malloc(sizeof(Lista));
    if (lista == NULL) {
        return NULL;
    }

    lista->inicio = NULL;
    lista->fim = NULL;
    return lista;
}

int lista_simples_inserir_no_fim(Lista *lista, int id, void *dado) {
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

void *lista_simples_buscar_por_id(const Lista *lista, int id) {
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

void *lista_simples_remover(Lista *lista, int id) {
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

void lista_simples_listar(const Lista *lista, void (*imprimir)(const void *dado)) {
    if (lista == NULL || imprimir == NULL) {
        return;
    }

    for (const NoLista *atual = lista->inicio; atual != NULL; atual = atual->prox) {
        imprimir(atual->dado);
    }
}

void liberar_lista_simples(Lista *lista) {
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

void lista_simples_trocar_dados(Lista *lista, int id1, int id2) {
    if (lista == NULL) {
        return;
    }

    NoLista *no1 = NULL;
    NoLista *no2 = NULL;

    NoLista *atual = lista->inicio;
    while (atual != NULL) {
        if (atual->id == id1) {
            no1 = atual;
        } else if (atual->id == id2) {
            no2 = atual;
        }
        atual = atual->prox;
    }

    if (no1 != NULL && no2 != NULL) {
        void *temp = no1->dado;
        no1->dado = no2->dado;
        no2->dado = temp;
    }
}
