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

Lista *criar_lista(void);
int inserir_no_fim(Lista *lista, int id, void *dado);
void *buscar_por_id(const Lista *lista, int id);
void *remover(Lista *lista, int id);
void listar(const Lista *lista, void (*imprimir)(const void *dado));
void liberar_lista(Lista *lista);
void trocar_dados(Lista *lista, int id1, int id2);

#endif
