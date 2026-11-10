#include "lista-simples.h"

#include <stdio.h>

typedef struct {
    int id;
    const char *nome;
} ItemTeste;

static int falhas = 0;

static void verificar(int condicao, const char *mensagem) {
    if (!condicao) {
        printf("FALHOU: %s\n", mensagem);
        falhas++;
    }
}

static int visitados[8];
static int quantidade_visitados = 0;

static void visitar(const void *dado) {
    const ItemTeste *item = (const ItemTeste *)dado;

    if (quantidade_visitados < 8) {
        visitados[quantidade_visitados] = item->id;
        quantidade_visitados++;
    }
}

int main(void) {
    Lista *lista = criar_lista();

    if (lista == NULL) {
        printf("FALHOU: criar_lista retornou NULL\n");
        return 1;
    }

    verificar(lista->inicio == NULL, "lista nova deve ter inicio NULL");
    verificar(lista->fim == NULL, "lista nova deve ter fim NULL");
    verificar(buscar_por_id(lista, 1) == NULL, "busca em lista vazia");
    verificar(remover(lista, 1) == NULL, "remocao em lista vazia");

    ItemTeste a = {1, "um"};
    ItemTeste b = {2, "dois"};
    ItemTeste c = {3, "tres"};

    int inseriu = 1;
    inseriu &= inserir_no_fim(lista, a.id, &a);
    inseriu &= inserir_no_fim(lista, b.id, &b);
    inseriu &= inserir_no_fim(lista, c.id, &c);
    verificar(inseriu == 1, "inserir tres itens no fim");

    if (inseriu != 1) {
        liberar_lista(lista);
        return 1;
    }

    verificar(lista->inicio->id == 1, "inicio deve ser o primeiro item");
    verificar(lista->fim->id == 3, "fim deve ser o ultimo item");
    verificar(lista->inicio->prox->id == 2, "ordem 1 -> 2");
    verificar(lista->inicio->prox->prox->id == 3, "ordem 2 -> 3");
    verificar(lista->fim->prox == NULL, "fim->prox deve ser NULL");
    verificar(buscar_por_id(lista, 1) == &a, "buscar primeiro item");
    verificar(buscar_por_id(lista, 2) == &b, "buscar item do meio");
    verificar(buscar_por_id(lista, 99) == NULL, "buscar item inexistente");

    quantidade_visitados = 0;
    listar(lista, visitar);
    verificar(quantidade_visitados == 3, "listar deve visitar tres itens");
    verificar(visitados[0] == 1 && visitados[1] == 2 && visitados[2] == 3,
              "listar deve respeitar a ordem");

    void (*callback_nulo)(const void *) = NULL;
    listar(lista, callback_nulo);
    listar(NULL, visitar);

    verificar(remover(lista, 2) == &b, "remover item do meio");
    verificar(lista->inicio->id == 1 && lista->fim->id == 3,
              "remover meio deve manter as pontas");
    verificar(lista->inicio->prox == lista->fim, "remover meio deve religar os vizinhos");
    verificar(lista->fim->prox == NULL, "remover meio nao deve quebrar o fim");

    verificar(remover(lista, 1) == &a, "remover primeiro item");
    verificar(lista->inicio == lista->fim && lista->fim->id == 3,
              "remover inicio deve atualizar o inicio");

    verificar(remover(lista, 3) == &c, "remover o unico item restante");
    verificar(lista->inicio == NULL && lista->fim == NULL,
              "lista deve ficar vazia apos remover tudo");
    verificar(remover(lista, 99) == NULL, "remover item inexistente");

    inseriu = 1;
    inseriu &= inserir_no_fim(lista, a.id, &a);
    inseriu &= inserir_no_fim(lista, b.id, &b);
    inseriu &= inserir_no_fim(lista, c.id, &c);
    verificar(inseriu == 1, "reinserir tres itens");

    if (inseriu != 1) {
        liberar_lista(lista);
        return 1;
    }

    verificar(remover(lista, 3) == &c, "remover ultimo item");
    verificar(lista->fim == lista->inicio->prox && lista->fim->id == 2,
              "remover fim deve atualizar o ponteiro fim");
    verificar(lista->fim->prox == NULL, "novo fim deve ter prox NULL");

    verificar(remover(lista, 2) == &b, "remover segundo item");
    verificar(remover(lista, 1) == &a, "remover primeiro item novamente");
    verificar(lista->inicio == NULL && lista->fim == NULL,
              "lista deve terminar vazia");

    verificar(inserir_no_fim(lista, 4, NULL) == 0, "inserir dado NULL deve falhar");
    verificar(inserir_no_fim(NULL, 4, &a) == 0, "inserir em lista NULL deve falhar");
    verificar(buscar_por_id(NULL, 1) == NULL, "buscar em lista NULL");
    verificar(remover(NULL, 1) == NULL, "remover em lista NULL");
    liberar_lista(NULL);

    liberar_lista(lista);

    if (falhas == 0) {
        printf("OK: todos os testes passaram\n");
        return 0;
    }

    printf("FALHAS: %d\n", falhas);
    return 1;
}
