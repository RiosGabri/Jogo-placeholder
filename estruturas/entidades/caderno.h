#ifndef CADERNO_H
#define CADERNO_H
#include "listas/lista-dupla.h"

//caderno é do tipo lista dupla
//cada no representa uma seção = depoimentos, pistas e reação
// cada seção é uma lista dupla

enum Categorias{
    ALIBI,
    RELACAO,
    CENA_DO_CRIME,
    TESTEMUNHO
};

enum TipoReacao{
    CONCORDA,
    DISCORDA,
    NEUTRO
};

typedef struct {
    NoListaDupla *inicio;  // primeiro item da seção
    NoListaDupla *fim;     // último item (inserir no fim sem percorrer)
    NoListaDupla *atual;   // item que o jogador está lendo agora
    int tamanho;      // quantos itens a seção tem
} Secao;

typedef struct {
    int id; // 0 - Miss Marple, 1 - Hercule Poirot
    Secao depoimentos; // Lista de depoimentos
    Secao pistas;      // Lista de pistas
    Secao reacoes;     // Lista de reações
} Caderno;

//secoes
typedef struct {
    int id_suspeito;
    int dia;
    char *quem_falou;
    char *local;
    char *sobre_quem_fala;
    enum Categorias categoria;
    char *texto;
} Depoimento;

typedef struct {
    int dia;
    char *local;
    char *objeto;
    char *descricao;
    ListaDupla *suspeitos_relacionados; // Lista de suspeitos relacionados à pista
} Pista;

typedef struct {
    int dia;
    int id_depoimento;
    char *quem_reagiu;
    enum TipoReacao tipo_reacao;
} Reacao;

Caderno* inicializar_caderno(int id);
void registrar_depoimento(Caderno *caderno, int id_suspeito, int dia, char* quem_falou, char* local, char* sobre_quem_fala, enum Categorias categoria, char* texto);
#endif