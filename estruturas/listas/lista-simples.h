#ifndef LISTA_SIMPLES_H
#define LISTA_SIMPLES_H

//lestruturas para lista simples
typedef struct NoLista {
    int id;
    void *dado;
    struct NoLista *prox;
} NoLista;

typedef struct {
    NoLista *inicio;
    NoLista *fim;
} Lista;

Lista *criar_lista_simples(void);
int lista_simples_inserir_no_fim(Lista *lista, int id, void *dado);
void *lista_simples_buscar_por_id(const Lista *lista, int id);
void *lista_simples_remover(Lista *lista, int id);
void lista_simples_listar(const Lista *lista, void (*imprimir)(const void *dado));
void liberar_lista_simples(Lista *lista);
void lista_simples_trocar_dados(Lista *lista, int id1, int id2);

#endif
