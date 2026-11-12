#include <caderno.h>
#include <lista-dupla.h>
#include <stdlib.h>
#include <string.h>

Caderno* inicializar_caderno(int id) {
    Caderno *caderno = malloc(sizeof(Caderno));
    if (caderno == NULL) {
        return NULL; // Falha ao alocar memória
    }

    caderno->id = id;
    //inicializar as seções do caderno todas vazias
    caderno->depoimentos.inicio = NULL;
    caderno->depoimentos.fim = NULL;
    caderno->depoimentos.atual = NULL;
    caderno->depoimentos.tamanho = 0;

    caderno->pistas.inicio = NULL;
    caderno->pistas.fim = NULL;
    caderno->pistas.atual = NULL;
    caderno->pistas.tamanho = 0;

    caderno->reacoes.inicio = NULL;
    caderno->reacoes.fim = NULL;
    caderno->reacoes.atual = NULL;
    caderno->reacoes.tamanho = 0;

    //caderno->caderno.inicio = NULL;
    //caderno->caderno.fim = NULL;

    return caderno;
}

void registrar_depoimento(Caderno *caderno, int id_suspeito, int dia, char* quem_falou, char* local, char* sobre_quem_fala, enum Categorias categoria, char* texto) {
    //nó do novo depoimento que fica na seção de depoimentos do caderno
    NoListaDupla *novo_depoimento = malloc(sizeof(NoListaDupla));
    if (novo_depoimento == NULL) {
        return; // Falha ao alocar memória
    }

    //depoimento em si, que será armazenado no nó da lista dupla
    Depoimento *depoimento = malloc(sizeof(Depoimento));
    if (depoimento == NULL) {
        return; // Falha ao alocar memória
    }
    depoimento->id_suspeito = id_suspeito;
    depoimento->dia = dia;
    strncpy(depoimento->quem_falou, quem_falou, sizeof(depoimento->quem_falou) - 1);
    depoimento->quem_falou[sizeof(depoimento->quem_falou) - 1] = '\0';
    strncpy(depoimento->local, local, sizeof(depoimento->local) - 1);
    depoimento->local[sizeof(depoimento->local) - 1] = '\0';
    strncpy(depoimento->sobre_quem_fala, sobre_quem_fala, sizeof(depoimento->sobre_quem_fala) - 1);
    depoimento->sobre_quem_fala[sizeof(depoimento->sobre_quem_fala) - 1] = '\0';
    depoimento->categoria = categoria;
    strncpy(depoimento->texto, texto, sizeof(depoimento->texto) - 1);
    depoimento->texto[sizeof(depoimento->texto) - 1] = '\0';

    //adicionando o depoimento na seção de depoimentos do caderno
    novo_depoimento->dado = depoimento;
    novo_depoimento->prox = NULL;
    novo_depoimento->ant = NULL;

    if (caderno->depoimentos.fim == NULL) {
        caderno->depoimentos.inicio = novo_depoimento;
    } else {
        caderno->depoimentos.fim->prox = novo_depoimento;
        novo_depoimento->ant = caderno->depoimentos.fim;
    }
    caderno->depoimentos.fim = novo_depoimento;
    caderno->depoimentos.tamanho++;
}