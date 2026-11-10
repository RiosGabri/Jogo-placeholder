#ifndef LISTA_DUPLA_H
#define LISTA_DUPLA_H

typedef struct NoListaDupla {
    int id;
    void *dado;
    struct NoListaDupla *prox;
    struct NoListaDupla *ant;
} NoListaDupla;

typedef struct {
    NoListaDupla *inicio;
    NoListaDupla *fim;
} ListaDupla;

ListaDupla *criar_lista(void);
int inserir_no_fim(ListaDupla *lista, int id, void *dado);
void *buscar_por_id(const ListaDupla *lista, int id);
void *remover(ListaDupla *lista, int id);
void listar(const ListaDupla *lista);
void liberar_lista(ListaDupla *lista);
void trocar_dados(ListaDupla *lista, int id1, int id2);

#endif