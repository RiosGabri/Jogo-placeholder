#ifndef LISTA_H
#define LISTA_H

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

#endif
