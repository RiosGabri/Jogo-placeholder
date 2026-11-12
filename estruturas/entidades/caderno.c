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

void registrar_depoimento(Caderno *caderno, int id_suspeito, int dia, char* quem_falou, char* local, char* periodo, char* sobre_quem_fala, enum Categorias categoria, char* texto) {
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
    depoimento->quem_falou = strdup(quem_falou);
    depoimento->local = strdup(local);
    depoimento->periodo = strdup(periodo);
    depoimento->sobre_quem_fala = strdup(sobre_quem_fala);
    depoimento->texto = strdup(texto);
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
void registrar_pista(Caderno *caderno, int dia, char* local, char* objeto, char* descricao, ListaDupla *suspeitos_relacionados) {
    //nó do novo depoimento que fica na seção de depoimentos do caderno
    NoListaDupla *nova_pista = malloc(sizeof(NoListaDupla));
    if (nova_pista == NULL) {
        return; // Falha ao alocar memória
    }

    //pista em si, que será armazenado no nó da lista dupla
    Pista *pista = malloc(sizeof(Pista));
    if (pista == NULL) {
        return; // Falha ao alocar memória
    }
    pista->dia = dia;
    pista->local = strdup(local);
    pista->objeto = strdup(objeto);
    pista->descricao = strdup(descricao);
    pista->suspeitos_relacionados = suspeitos_relacionados;

    //adicionando o depoimento na seção de depoimentos do caderno
    nova_pista->dado = pista;
    nova_pista->prox = NULL;
    nova_pista->ant = NULL;

    if (caderno->pistas.fim == NULL) {
        caderno->pistas.inicio = nova_pista;
    } else {
        caderno->pistas.fim->prox = nova_pista;
        nova_pista->ant = caderno->pistas.fim;
    }
    caderno->pistas.fim = nova_pista;
    caderno->pistas.tamanho++;
}
void registrar_reacao(Caderno *caderno, int dia, int id_depoimento, char* quem_reagiu, enum TipoReacao tipo_reacao) {
    //nó da nova reação que fica na seção de reações do caderno
    NoListaDupla *nova_reacao = malloc(sizeof(NoListaDupla));
    if (nova_reacao == NULL) {
        return; // Falha ao alocar memória
    }

    //reação em si, que será armazenada no nó da lista dupla
    Reacao *reacao = malloc(sizeof(Reacao));
    if (reacao == NULL) {
        return; // Falha ao alocar memória
    }
    reacao->dia = dia;
    reacao->id_depoimento = id_depoimento;
    reacao->quem_reagiu = strdup(quem_reagiu);
    reacao->tipo_reacao = tipo_reacao;

    //adicionando a reação na seção de reações do caderno
    nova_reacao->dado = reacao;
    nova_reacao->prox = NULL;
    nova_reacao->ant = NULL;

    if (caderno->reacoes.fim == NULL) {
        caderno->reacoes.inicio = nova_reacao;
    } else {
        caderno->reacoes.fim->prox = nova_reacao;
        nova_reacao->ant = caderno->reacoes.fim;
    }
    caderno->reacoes.fim = nova_reacao;
    caderno->reacoes.tamanho++;
}

void avancar_secao(Caderno *caderno){
    if (caderno->depoimentos.atual != NULL && caderno->depoimentos.atual->prox != NULL) {
        caderno->depoimentos.atual = caderno->depoimentos.atual->prox;
    }else if (caderno->pistas.atual != NULL && caderno->pistas.atual->prox != NULL) {
        caderno->pistas.atual = caderno->pistas.atual->prox;
    } else if (caderno->reacoes.atual != NULL && caderno->reacoes.atual->prox != NULL) {
        caderno->reacoes.atual = caderno->reacoes.atual->prox;
    }
}

void voltar_secao(Caderno *caderno){
    if (caderno->depoimentos.atual != NULL && caderno->depoimentos.atual->ant != NULL) {
        caderno->depoimentos.atual = caderno->depoimentos.atual->ant;
    }else if (caderno->pistas.atual != NULL && caderno->pistas.atual->ant != NULL) {
        caderno->pistas.atual = caderno->pistas.atual->ant;
    } else if (caderno->reacoes.atual != NULL && caderno->reacoes.atual->ant != NULL) {
        caderno->reacoes.atual = caderno->reacoes.atual->ant;
    }
}

NoListaDupla *obter_item_atual(Caderno *caderno){
    if (caderno->depoimentos.atual != NULL) {
        return caderno->depoimentos.atual;
    } else if (caderno->pistas.atual != NULL) {
        return caderno->pistas.atual;
    } else if (caderno->reacoes.atual != NULL) {
        return caderno->reacoes.atual;
    }
    return NULL; // Nenhum item atual
}
void ler_item_atual(Caderno *caderno){
    if (caderno->depoimentos.atual != NULL) {
        Depoimento *depoimento = (Depoimento *)caderno->depoimentos.atual->dado;
        //printf por enquanto que nao tem graficos
        printf("Depoimento do suspeito %d: %s\n", depoimento->id_suspeito, depoimento->texto);
    } else if (caderno->pistas.atual != NULL) {
        Pista *pista = (Pista *)caderno->pistas.atual->dado;
        printf("Pista encontrada no dia %d: %s\n", pista->dia, pista->descricao);
    } else if (caderno->reacoes.atual != NULL) {
        Reacao *reacao = (Reacao *)caderno->reacoes.atual->dado;
        printf("Reação de %s ao depoimento %d: %d\n", reacao->quem_reagiu, reacao->id_depoimento, reacao->tipo_reacao);
    } else {
        printf("Nenhum item atual para ler.\n");
    }
}

Depoimento* buscar_depoimento_por_id_reacao(Caderno *caderno, int id_suspeito) {
    NoListaDupla *atual = caderno->depoimentos.inicio;
    NoListaDupla *reacao_atual = caderno->reacoes.inicio;
    while (atual != NULL) {
        Depoimento *depoimento = (Depoimento *)atual->dado;
        if (depoimento->id_suspeito == id_suspeito) {
            while (reacao_atual != NULL) {
                Reacao *reacao = (Reacao *)reacao_atual->dado;
                if (reacao->id_depoimento == id_suspeito) {
                    return depoimento;
                }
                reacao_atual = reacao_atual->prox;
            }
        }
        atual = atual->prox;
    }
    return NULL; // Depoimento não encontrado
}

void liberar_caderno(Caderno *caderno) {
    // Liberar depoimentos
    NoListaDupla *atual = caderno->depoimentos.inicio;
    while (atual != NULL) {
        NoListaDupla *proximo = atual->prox;
        free(atual->dado); // Liberar o depoimento
        free(atual);       // Liberar o nó da lista
        atual = proximo;
    }

    // Liberar pistas
    atual = caderno->pistas.inicio;
    while (atual != NULL) {
        NoListaDupla *proximo = atual->prox;
        Pista *pista = (Pista *)atual->dado;
        liberar_lista_dupla(pista->suspeitos_relacionados); // Liberar a lista de suspeitos relacionados
        free(pista);       // Liberar a pista
        free(atual);       // Liberar o nó da lista
        atual = proximo;
    }

    // Liberar reações
    atual = caderno->reacoes.inicio;
    while (atual != NULL) {
        NoListaDupla *proximo = atual->prox;
        free(atual->dado); // Liberar a reação
        free(atual);       // Liberar o nó da lista
        atual = proximo;
    }

    free(caderno); // Liberar o caderno em si
}