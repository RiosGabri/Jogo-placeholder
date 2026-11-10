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

ListaDupla *criar_lista_dupla(void);
int lista_dupla_inserir_no_fim(ListaDupla *lista, int id, void *dado);
void *lista_dupla_buscar_por_id(const ListaDupla *lista, int id);
void *lista_dupla_remover(ListaDupla *lista, int id);
void lista_dupla_listar(const ListaDupla *lista);
void lista_dupla_liberar_lista(ListaDupla *lista);
void lista_dupla_trocar_dados(ListaDupla *lista, int id1, int id2);

#endif