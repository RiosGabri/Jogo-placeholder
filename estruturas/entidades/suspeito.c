#define _POSIX_C_SOURCE 200809L //para o strdup funcionar no linux
#include "entidades/suspeito.h"
#include "listas/lista-dupla-circular.h"
#include <stdlib.h>
#include <string.h>

void carregar_suspeito(ListaDuplaCircular *lista,int id, char *nome, char *cargo, enum Situacao situacao, enum Papel papel){
    Suspeito *novo_suspeito = malloc(sizeof(Suspeito));
    if (novo_suspeito == NULL) {
        return; // Falha ao alocar memória
    }

    // Atribuir valores aos campos do suspeito
    novo_suspeito->id = id;
    novo_suspeito->nome = strdup(nome);
    novo_suspeito->cargo = strdup(cargo);
    novo_suspeito->situacao = situacao;
    novo_suspeito->papel = papel;

    // Inserir o suspeito na lista
    lista_dupla_circular_inserir_no_fim(lista, novo_suspeito->id, novo_suspeito);
}

int contar_vivos(ListaDuplaCircular *lista){
    if (lista == NULL) {
        return 0; // Lista nula, não há suspeitos vivos
    }

    int contador = 0;
    NoListaDuplaCircular *atual = lista->inicio;

    if (atual == NULL) {
        return 0; // Lista vazia, não há suspeitos vivos
    }

    do {
        Suspeito *suspeito = (Suspeito *)atual->dado;
        if (suspeito != NULL && suspeito->situacao == 0) { // 0 = vivo
            contador++;
        }
        atual = atual->prox;
    } while (atual != lista->inicio);

    return contador;
}

void liberar_suspeitos(ListaDuplaCircular *lista) {
    if (!lista || !lista->inicio) { liberar_lista_dupla_circular(lista); return; }
    NoListaDuplaCircular *atual = lista->inicio;
    do {
        Suspeito *s = atual->dado;
        free(s->nome);
        free(s->cargo);
        free(s);
        atual = atual->prox;
    } while (atual != lista->inicio);
    liberar_lista_dupla_circular(lista);
}