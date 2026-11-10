#include <suspeito.h>
#include <lista-dupla-circular.h>
#include <stdlib.h>
#include <string.h>

void carregar_suspeito(ListaDuplaCircular *lista, const char *nome, const char *cargo, int situacao, int papel){
    Suspeito *novo_suspeito = malloc(sizeof(Suspeito));
    if (novo_suspeito == NULL) {
        return; // Falha ao alocar memória
    }

    // Atribuir valores aos campos do suspeito
    novo_suspeito->id = rand(); // Gera um ID aleatório para o suspeito
    strncpy(novo_suspeito->nome, nome, sizeof(novo_suspeito->nome) - 1);
    strncpy(novo_suspeito->cargo, cargo, sizeof(novo_suspeito->cargo) - 1);
    novo_suspeito->nome[sizeof(novo_suspeito->nome) - 1] = '\0';
    novo_suspeito->cargo[sizeof(novo_suspeito->cargo) - 1] = '\0';
    novo_suspeito->situacao = situacao;
    novo_suspeito->papel = papel;

    // Inserir o suspeito na lista
    lista_dupla_circular_inserir_no_fim(lista, novo_suspeito->id, novo_suspeito);
}