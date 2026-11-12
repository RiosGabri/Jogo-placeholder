#include "entidades/suspeito.h"
#include "listas/lista-dupla-circular.h"

#include <stdio.h>
#include <string.h>

static int falhas = 0;

static void verificar(int condicao, const char *mensagem) {
    if (!condicao) {
        printf("FALHOU: %s\n", mensagem);
        falhas++;
    }
}

static void testar_lista_vazia(void) {
    ListaDuplaCircular *lista = criar_lista_dupla_circular();
    verificar(lista != NULL, "criar_lista_dupla_circular deve alocar");
    if (lista == NULL) {
        return;
    }

    verificar(lista->inicio == NULL, "lista nova comeca sem inicio");
    verificar(lista->fim == NULL, "lista nova comeca sem fim");
    verificar(contar_vivos(lista) == 0, "lista vazia tem 0 vivos");
    verificar(contar_vivos(NULL) == 0, "contar_vivos(NULL) retorna 0");
    verificar(lista_dupla_circular_buscar_por_id(lista, 0) == NULL,
              "busca em lista vazia retorna NULL");

    liberar_suspeitos(lista);
}

static void testar_carga_dos_suspeitos(void) {
    ListaDuplaCircular *lista = criar_lista_dupla_circular();
    verificar(lista != NULL, "criar_lista_dupla_circular deve alocar");
    if (lista == NULL) {
        return;
    }

    const char *nomes[] = {
        "Miss Marple", "Hercule Poirot", "Engenheiro",
        "Operária", "Vigia noturno", "Gerente"
    };
    const char *cargos[] = {
        "Detetive", "Detetive", "Engenheiro",
        "Operária", "Vigia", "Gerente"
    };
    const enum Situacao situacoes[] = { VIVO, VIVO, VIVO, VIVO, PRESO, MORTO };
    const enum Papel papeis[] = {
        INOCENTE, INOCENTE, INOCENTE, INOCENTE, INOCENTE, VITIMA
    };

    for (int id = 0; id < 6; id++) {
        carregar_suspeito(lista, id, (char *)nomes[id], (char *)cargos[id],
                          situacoes[id], papeis[id]);
    }

    verificar(lista->inicio != NULL, "lista tem inicio depois da carga");
    verificar(lista->fim != NULL, "lista tem fim depois da carga");
    if (lista->inicio == NULL || lista->fim == NULL) {
        liberar_suspeitos(lista);
        return;
    }

    verificar(lista->inicio->id == 0, "inicio e o suspeito de id 0");
    verificar(lista->fim->id == 5, "fim e o suspeito de id 5");
    verificar(lista->fim->prox == lista->inicio, "lista circular: fim->prox aponta para o inicio");
    verificar(lista->inicio->ant == lista->fim, "lista circular: inicio->ant aponta para o fim");

    /* Percorre a lista circular uma vez e confere cada suspeito. */
    int visitados = 0;
    NoListaDuplaCircular *no = lista->inicio;
    do {
        Suspeito *suspeito = (Suspeito *)no->dado;
        verificar(suspeito != NULL, "cada no deve ter um suspeito");
        if (suspeito != NULL) {
            verificar(suspeito->id == visitados, "ordem dos ids dos suspeitos");
            verificar(strcmp(suspeito->nome, nomes[visitados]) == 0, "nome do suspeito");
            verificar(strcmp(suspeito->cargo, cargos[visitados]) == 0, "cargo do suspeito");
            verificar(suspeito->situacao == situacoes[visitados], "situacao do suspeito");
            verificar(suspeito->papel == papeis[visitados], "papel do suspeito");
            verificar(no->id == suspeito->id, "id do no igual ao id do suspeito");
        }

        no = no->prox;
        visitados++;
    } while (no != lista->inicio && visitados < 10);

    verificar(visitados == 6, "a volta completa visita os 6 suspeitos");
    verificar(contar_vivos(lista) == 4, "contar_vivos conta apenas VIVO (4 de 6)");

    Suspeito *engenheiro = lista_dupla_circular_buscar_por_id(lista, 2);
    verificar(engenheiro != NULL, "busca por id encontra o Engenheiro");
    if (engenheiro != NULL) {
        verificar(engenheiro->id == 2, "busca devolve o suspeito certo");
        verificar(strcmp(engenheiro->nome, "Engenheiro") == 0, "busca devolve o nome certo");
        verificar(engenheiro->situacao == VIVO, "Engenheiro esta vivo");
    }

    verificar(lista_dupla_circular_buscar_por_id(lista, 99) == NULL,
              "busca por id inexistente retorna NULL");

    liberar_suspeitos(lista);
}

static void testar_copia_dos_textos(void) {
    ListaDuplaCircular *lista = criar_lista_dupla_circular();
    verificar(lista != NULL, "criar_lista_dupla_circular deve alocar");
    if (lista == NULL) {
        return;
    }

    char nome_mutavel[32] = "Temporario";
    char cargo_mutavel[32] = "Cargo Temp";

    carregar_suspeito(lista, 7, nome_mutavel, cargo_mutavel, VIVO, INOCENTE);

    /* Se o suspeito guardou o ponteiro, mudar o buffer mudaria o nome. */
    nome_mutavel[0] = 'X';
    cargo_mutavel[0] = 'X';

    Suspeito *suspeito = lista_dupla_circular_buscar_por_id(lista, 7);
    verificar(suspeito != NULL, "suspeito temporario foi inserido");
    if (suspeito != NULL) {
        verificar(strcmp(suspeito->nome, "Temporario") == 0,
                  "nome e copiado com strdup (independente do buffer de origem)");
        verificar(strcmp(suspeito->cargo, "Cargo Temp") == 0,
                  "cargo e copiado com strdup (independente do buffer de origem)");
        verificar(suspeito->nome != nome_mutavel,
                  "nome do suspeito nao aponta para o buffer de origem");
        verificar(suspeito->cargo != cargo_mutavel,
                  "cargo do suspeito nao aponta para o buffer de origem");
    }

    verificar(contar_vivos(lista) == 1, "um unico suspeito vivo");

    liberar_suspeitos(lista);
}

static void testar_liberacoes_seguras(void) {
    /* Deve ser seguro liberar NULL e uma lista vazia. */
    liberar_suspeitos(NULL);

    ListaDuplaCircular *vazia = criar_lista_dupla_circular();
    verificar(vazia != NULL, "lista vazia deve alocar");
    liberar_suspeitos(vazia);

    verificar(1, "liberacoes de NULL e lista vazia nao travaram");
}

int main(void) {
    testar_lista_vazia();
    testar_carga_dos_suspeitos();
    testar_copia_dos_textos();
    testar_liberacoes_seguras();

    if (falhas == 0) {
        printf("OK: todos os testes de suspeitos passaram\n");
        return 0;
    }

    printf("FALHAS: %d\n", falhas);
    return 1;
}
