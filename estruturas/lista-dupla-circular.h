#ifndef LISTA_DUPLA_CIRCULAR_H
#define LISTA_DUPLA_CIRCULAR_H

typedef struct NoListaDuplaCircular {
    int id;
    void *dado;
    struct NoListaDuplaCircular *prox;
    struct NoListaDuplaCircular *ant;
} NoListaDuplaCircular;

typedef struct {
    NoListaDuplaCircular *inicio;
    NoListaDuplaCircular *fim;
} ListaDuplaCircular;

ListaDuplaCircular *criar_lista_dupla_circular(void);
int lista_dupla_circular_inserir_no_fim(ListaDuplaCircular *lista, int id, void *dado);
void *lista_dupla_circular_buscar_por_id(const ListaDuplaCircular *lista, int id);
void *lista_dupla_circular_remover(ListaDuplaCircular *lista, int id);
void lista_dupla_circular_listar(const ListaDuplaCircular *lista);
void lista_dupla_circular_liberar_lista(ListaDuplaCircular *lista);
void trocar_dados(ListaDuplaCircular *lista, int id1, int id2);

#endif