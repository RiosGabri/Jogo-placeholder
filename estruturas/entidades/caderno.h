#ifndef CADERNO_H
#define CADERNO_H
#include "listas/lista-dupla.h"
#include "listas/lista-simples.h"

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

enum Periodo{
    MANHA,
    TARDE,
    NOITE
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
    int prox_id_depoimento; // Próximo ID de depoimento a ser atribuído e começa em 1
    int prox_id_pista; // Próximo ID de pista a ser atribuído e começa em 1
    int prox_id_reacao; // Próximo ID de reação a ser atribuído e começa em 1
} Caderno;

//secoes
typedef struct {
    int id_depoimento;
    int dia;
    int id_falante; //quem falou
    int id_assunto; //sobre quem fala
    char *local;
    enum Periodo periodo;
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
int registrar_depoimento(Caderno *caderno, int dia, int quem_falou, char* local, enum Periodo periodo, int sobre_quem_fala, enum Categorias categoria, char* texto);
void registrar_pista(Caderno *caderno, int dia, char* local, char* objeto, char* descricao, ListaDupla *suspeitos_relacionados);
void registrar_reacao(Caderno *caderno, int dia, int id_depoimento, char* quem_reagiu, enum TipoReacao tipo_reacao);
void avancar_secao(Caderno *caderno); //move o atual para o próximo ou anterior da seção, parando nas pontas
void voltar_secao(Caderno *caderno);
NoListaDupla *obter_item_atual(Caderno *caderno); //retorna o nó atual da seção, ou NULL se não houver
void ler_item_atual(Caderno *caderno);
Depoimento* buscar_depoimento_por_id_reacao(Caderno *caderno, int id_suspeito); //acha o depoimento ao qual uma reação se refere (linka um com o outro)
void liberar_caderno(Caderno *caderno); //libera a memória alocada para o caderno e suas seções

//filtro vai funcionar como se fosse um filtro de site, onde eu seleciono o que quero ver, e o resto é desabilitado
#define DESABILITADO -1
typedef struct {
    int falante;   // id de quem falou, ou DESABILITADO
    int assunto;   // id de sobre quem se fala, ou DESABILITADO
    int dia;       // 1 a 7, ou DESABILITADO
    int periodo;   // manhã, tarde ou noite, ou DESABILITADO
} CriterioFiltro;

CriterioFiltro* criar_criterio_vazio(void); //cria um filtro vazio, com todos os campos desabilitados
CriterioFiltro* criar_criterio(int falante, int assunto, int dia, int periodo);
int comparar_depoimentos_criterio(Depoimento *depoimento, CriterioFiltro *criterio); //retorna 1 se o depoimento satisfaz o criterio, 0 caso contrário
Lista *filtrar_depoimentos(Caderno *caderno, CriterioFiltro *criterio);
void liberar_resultado_filtro(Lista *resultado); //libera a memória alocada para o resultado do filtro (lembrar de fazer o free no criterio depois de usar essa funcao)

Lista* depoimentos_do_suspeito(Caderno *caderno, int id_suspeito); //retorna uma lista de depoimentos do suspeito, ou NULL se não houver
Lista* depoimentos_sobre_suspeito(Caderno *caderno, int id_suspeito); //retorna uma lista de depoimentos sobre o suspeito, ou NULL se não houver
#endif