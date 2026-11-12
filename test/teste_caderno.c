#include "entidades/caderno.h"
#include "listas/lista-dupla.h"
#include "listas/lista-simples.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Ids fixos dos suspeitos usados no exemplo D1..D5.
 * Eles são os mesmos que vão em id_falante / id_assunto dos depoimentos.
 */
enum {
    ID_MARPLE = 0,
    ID_POIROT = 1,
    ID_ENGENHEIRO = 2,
    ID_OPERARIA = 3,
    ID_VIGIA = 4,
    ID_GERENTE = 5
};

static int falhas = 0;

static void verificar(int condicao, const char *mensagem) {
    if (!condicao) {
        printf("FALHOU: %s\n", mensagem);
        falhas++;
    }
}

/* Confere os ids dos depoimentos do resultado, na ordem, e os ids dos nós. */
static void verificar_resultado(const Lista *resultado, const int *esperados,
                                int quantidade, const char *mensagem) {
    int encontrados = 0;
    int ok = (resultado != NULL);

    if (ok) {
        for (const NoLista *no = resultado->inicio; no != NULL; no = no->prox) {
            if (encontrados >= quantidade) {
                ok = 0;
                break;
            }

            const Depoimento *depoimento = (const Depoimento *)no->dado;
            if (depoimento == NULL || depoimento->id_depoimento != esperados[encontrados]) {
                ok = 0;
            }
            if (no->id != encontrados + 1) { /* ids do resultado são 1, 2, 3... */
                ok = 0;
            }
            encontrados++;
        }

        if (encontrados != quantidade) {
            ok = 0;
        }
    }

    verificar(ok, mensagem);

    if (!ok) {
        printf("  esperados %d depoimento(s), encontrados %d\n", quantidade, encontrados);
        if (quantidade > 0) {
            printf("  esperados:");
            for (int i = 0; i < quantidade; i++) {
                printf(" %d", esperados[i]);
            }
            printf("\n  obtidos:");
            if (resultado != NULL) {
                for (const NoLista *no = resultado->inicio; no != NULL; no = no->prox) {
                    const Depoimento *depoimento = (const Depoimento *)no->dado;
                    printf(" %d", depoimento->id_depoimento);
                }
            }
            printf("\n");
        }
    }
}

static void filtrar_e_verificar(const Caderno *caderno, int falante, int assunto,
                                int dia, int periodo, const int *esperados,
                                int quantidade, const char *mensagem) {
    CriterioFiltro *criterio = criar_criterio(falante, assunto, dia, periodo);
    verificar(criterio != NULL, "criar_criterio deve alocar o criterio");
    if (criterio == NULL) {
        return;
    }

    Lista *resultado = filtrar_depoimentos((Caderno *)caderno, criterio);
    verificar_resultado(resultado, esperados, quantidade, mensagem);

    liberar_resultado_filtro(resultado);
    free(criterio);
}

/*
 * Exemplo D1..D5 combinado:
 *   D1: Engenheiro fala sobre Engenheiro, dia 1, tarde
 *   D2: Operária fala sobre Engenheiro, dia 1, tarde
 *   D3: Vigia fala sobre Operária, dia 2, noite
 *   D4: Engenheiro fala sobre Gerente, dia 2, manhã
 *   D5: Gerente fala sobre Engenheiro, dia 3, manhã
 */
static Caderno *criar_caderno_exemplo(void) {
    Caderno *caderno = inicializar_caderno(ID_MARPLE);
    if (caderno == NULL) {
        return NULL;
    }

    registrar_depoimento(caderno, 1, ID_ENGENHEIRO, "Fundição", TARDE, ID_ENGENHEIRO,
                         TESTEMUNHO, "Eu vi o vigia rondando a fundição.");
    registrar_depoimento(caderno, 1, ID_OPERARIA, "Refeitório", TARDE, ID_ENGENHEIRO,
                         TESTEMUNHO, "O engenheiro saiu mais cedo ontem.");
    registrar_depoimento(caderno, 2, ID_VIGIA, "Portão", NOITE, ID_OPERARIA,
                         ALIBI, "A operária estava no portão à noite.");
    registrar_depoimento(caderno, 2, ID_ENGENHEIRO, "Escritório", MANHA, ID_GERENTE,
                         RELACAO, "O gerente pediu os relatórios.");
    registrar_depoimento(caderno, 3, ID_GERENTE, "Sala de máquinas", MANHA, ID_ENGENHEIRO,
                         CENA_DO_CRIME, "O engenheiro conhecia bem a máquina.");

    return caderno;
}

static void testar_inicializacao_e_caderno_vazio(void) {
    Caderno *vazio = inicializar_caderno(ID_POIROT);
    verificar(vazio != NULL, "inicializar_caderno deve alocar o caderno");
    if (vazio == NULL) {
        return;
    }

    verificar(vazio->id == ID_POIROT, "caderno guarda o id recebido");
    verificar(vazio->depoimentos.inicio == NULL, "secao de depoimentos começa vazia");
    verificar(vazio->depoimentos.fim == NULL, "fim dos depoimentos começa NULL");
    verificar(vazio->depoimentos.atual == NULL, "atual dos depoimentos começa NULL");
    verificar(vazio->depoimentos.tamanho == 0, "tamanho dos depoimentos começa em 0");
    verificar(vazio->pistas.tamanho == 0, "secao de pistas começa vazia");
    verificar(vazio->reacoes.tamanho == 0, "secao de reacoes começa vazia");
    verificar(vazio->prox_id_depoimento == 1, "primeiro id de depoimento e 1");
    verificar(vazio->prox_id_pista == 1, "primeiro id de pista e 1");
    verificar(vazio->prox_id_reacao == 1, "primeiro id de reacao e 1");

    /* Buscar em caderno vazio não pode travar. */
    verificar(buscar_depoimento_por_id(vazio, 1) == NULL, "buscar em caderno vazio retorna NULL");
    verificar(buscar_depoimento_por_id(vazio, 99) == NULL, "buscar id inexistente em caderno vazio");
    verificar(buscar_depoimento_por_id(NULL, 1) == NULL, "buscar em caderno NULL retorna NULL");

    /* Caderno vazio + qualquer filtro = lista vazia, sem travar. */
    filtrar_e_verificar(vazio, DESABILITADO, DESABILITADO, DESABILITADO, DESABILITADO,
                        NULL, 0, "caderno vazio com criterio vazio retorna lista vazia");
    filtrar_e_verificar(vazio, ID_ENGENHEIRO, DESABILITADO, 1, TARDE,
                        NULL, 0, "caderno vazio com qualquer filtro retorna lista vazia");

    liberar_caderno(vazio);
}

static void testar_criterios(void) {
    CriterioFiltro *vazio = criar_criterio_vazio();
    verificar(vazio != NULL, "criar_criterio_vazio deve alocar");
    if (vazio != NULL) {
        verificar(vazio->falante == DESABILITADO, "criterio vazio: falante desabilitado");
        verificar(vazio->assunto == DESABILITADO, "criterio vazio: assunto desabilitado");
        verificar(vazio->dia == DESABILITADO, "criterio vazio: dia desabilitado");
        verificar(vazio->periodo == DESABILITADO, "criterio vazio: periodo desabilitado");
        free(vazio);
    }

    CriterioFiltro *criterio = criar_criterio(ID_ENGENHEIRO, ID_OPERARIA, 2, NOITE);
    verificar(criterio != NULL, "criar_criterio deve alocar");
    if (criterio != NULL) {
        verificar(criterio->falante == ID_ENGENHEIRO, "criar_criterio copia o falante");
        verificar(criterio->assunto == ID_OPERARIA, "criar_criterio copia o assunto");
        verificar(criterio->dia == 2, "criar_criterio copia o dia");
        verificar(criterio->periodo == NOITE, "criar_criterio copia o periodo");
        free(criterio);
    }
}

static void testar_registro_e_busca_por_id(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    verificar(caderno->depoimentos.tamanho == 5, "exemplo tem 5 depoimentos");
    verificar(caderno->prox_id_depoimento == 6, "proximo id de depoimento e 6");

    /* Ids dos depoimentos e dos nós devem ser sequenciais a partir de 1. */
    int esperado = 1;
    for (NoListaDupla *no = caderno->depoimentos.inicio; no != NULL; no = no->prox) {
        Depoimento *depoimento = (Depoimento *)no->dado;
        verificar(no->id == esperado, "id do no do depoimento e sequencial");
        verificar(depoimento->id_depoimento == esperado, "id_depoimento e sequencial");
        esperado++;
    }
    verificar(esperado == 6, "percorreu os 5 depoimentos");

    for (int id = 1; id <= 5; id++) {
        Depoimento *depoimento = buscar_depoimento_por_id(caderno, id);
        verificar(depoimento != NULL, "buscar_depoimento_por_id encontra id existente");
        if (depoimento != NULL) {
            verificar(depoimento->id_depoimento == id, "busca devolve o depoimento certo");
        }
    }

    verificar(buscar_depoimento_por_id(caderno, 0) == NULL, "buscar id 0 retorna NULL");
    verificar(buscar_depoimento_por_id(caderno, 6) == NULL, "buscar id 6 retorna NULL");
    verificar(buscar_depoimento_por_id(caderno, 99) == NULL, "buscar id 99 retorna NULL");

    liberar_caderno(caderno);
}

static void testar_comparacao_direta(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    Depoimento *d1 = buscar_depoimento_por_id(caderno, 1);
    verificar(d1 != NULL, "D1 deve existir");
    if (d1 != NULL) {
        CriterioFiltro criterio = { DESABILITADO, DESABILITADO, DESABILITADO, DESABILITADO };

        verificar(comparar_depoimentos_criterio(d1, &criterio) == 1,
                  "criterio vazio aceita qualquer depoimento");

        criterio.falante = ID_ENGENHEIRO;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 1,
                  "D1 passa com falante Engenheiro");

        criterio.falante = ID_OPERARIA;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 0,
                  "D1 reprova com falante Operaria");

        criterio.falante = DESABILITADO;
        criterio.assunto = ID_ENGENHEIRO;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 1,
                  "D1 passa com assunto Engenheiro");

        criterio.assunto = ID_VIGIA;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 0,
                  "D1 reprova com assunto Vigia");

        criterio.assunto = DESABILITADO;
        criterio.dia = 1;
        criterio.periodo = TARDE;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 1,
                  "D1 passa com dia 1 e tarde");

        criterio.dia = 2;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 0,
                  "D1 reprova no dia 2");

        criterio.dia = 1;
        criterio.periodo = NOITE;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 0,
                  "D1 reprova a noite");

        /* Um campo reprovado derruba o depoimento, mesmo que os outros passem. */
        criterio.falante = ID_ENGENHEIRO;
        criterio.assunto = ID_ENGENHEIRO;
        criterio.dia = 1;
        criterio.periodo = NOITE;
        verificar(comparar_depoimentos_criterio(d1, &criterio) == 0,
                  "basta um campo errado para reprovar");
    }

    verificar(comparar_depoimentos_criterio(NULL, NULL) == 0,
              "comparacao com ponteiros NULL retorna 0");

    liberar_caderno(caderno);
}

static void testar_tabela_de_filtros(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    const int falante_engenheiro[] = {1, 4};
    const int assunto_engenheiro[] = {1, 2, 5};
    const int engenheiro_dia1_tarde[] = {1, 2};
    const int todos[] = {1, 2, 3, 4, 5};

    filtrar_e_verificar(caderno, ID_ENGENHEIRO, DESABILITADO, DESABILITADO, DESABILITADO,
                        falante_engenheiro, 2, "falante = Engenheiro -> D1, D4");
    filtrar_e_verificar(caderno, DESABILITADO, ID_ENGENHEIRO, DESABILITADO, DESABILITADO,
                        assunto_engenheiro, 3, "assunto = Engenheiro -> D1, D2, D5");
    filtrar_e_verificar(caderno, DESABILITADO, ID_ENGENHEIRO, 1, TARDE,
                        engenheiro_dia1_tarde, 2, "assunto = Engenheiro, dia 1, tarde -> D1, D2");
    filtrar_e_verificar(caderno, DESABILITADO, DESABILITADO, DESABILITADO, DESABILITADO,
                        todos, 5, "criterio vazio -> todos os 5");
    filtrar_e_verificar(caderno, DESABILITADO, ID_VIGIA, DESABILITADO, DESABILITADO,
                        NULL, 0, "assunto = Vigia -> lista vazia");

    /* Filtros combinados que ninguém satisfaz. */
    filtrar_e_verificar(caderno, ID_ENGENHEIRO, ID_VIGIA, DESABILITADO, DESABILITADO,
                        NULL, 0, "falante Engenheiro e assunto Vigia -> lista vazia");
    filtrar_e_verificar(caderno, DESABILITADO, DESABILITADO, 7, NOITE,
                        NULL, 0, "dia 7 -> lista vazia");

    liberar_caderno(caderno);
}

static void testar_atalhos(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    const int do_engenheiro[] = {1, 4};
    const int sobre_engenheiro[] = {1, 2, 5};

    Lista *do_suspeito = depoimentos_do_suspeito(caderno, ID_ENGENHEIRO);
    verificar_resultado(do_suspeito, do_engenheiro, 2,
                        "depoimentos_do_suspeito liga so o filtro de falante");
    liberar_resultado_filtro(do_suspeito);

    Lista *sobre_suspeito = depoimentos_sobre_suspeito(caderno, ID_ENGENHEIRO);
    verificar_resultado(sobre_suspeito, sobre_engenheiro, 3,
                        "depoimentos_sobre_suspeito liga so o filtro de assunto");
    liberar_resultado_filtro(sobre_suspeito);

    Lista *de_quem_ninguem_fala = depoimentos_sobre_suspeito(caderno, ID_VIGIA);
    verificar_resultado(de_quem_ninguem_fala, NULL, 0,
                        "depoimentos_sobre_suspeito sem resultados -> lista vazia");
    liberar_resultado_filtro(de_quem_ninguem_fala);

    liberar_caderno(caderno);
}

static void testar_resultado_nao_copia_e_nao_libera_depoimentos(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    CriterioFiltro *criterio = criar_criterio_vazio();
    verificar(criterio != NULL, "criar_criterio_vazio deve alocar");
    if (criterio != NULL) {
        Lista *resultado = filtrar_depoimentos(caderno, criterio);
        verificar(resultado != NULL && resultado->inicio != NULL,
                  "filtro vazio devolve lista com itens");

        Depoimento *original = buscar_depoimento_por_id(caderno, 1);
        verificar(original != NULL, "depoimento original deve existir");
        if (resultado != NULL && resultado->inicio != NULL) {
            verificar(resultado->inicio->dado == original,
                      "o no do resultado guarda o ponteiro do depoimento original");
        }

        liberar_resultado_filtro(resultado);

        verificar(buscar_depoimento_por_id(caderno, 1) == original,
                  "liberar o resultado nao libera os depoimentos do caderno");
        verificar(caderno->depoimentos.tamanho == 5,
                  "liberar o resultado mantem o caderno intacto");

        free(criterio);
    }

    /* Caderno NULL ou critério NULL: devolve lista vazia, sem travar. */
    CriterioFiltro qualquer = { DESABILITADO, DESABILITADO, DESABILITADO, DESABILITADO };
    Lista *sem_caderno = filtrar_depoimentos(NULL, &qualquer);
    verificar_resultado(sem_caderno, NULL, 0, "caderno NULL -> lista vazia");
    liberar_resultado_filtro(sem_caderno);

    Lista *sem_criterio = filtrar_depoimentos(caderno, NULL);
    verificar_resultado(sem_criterio, NULL, 0, "criterio NULL -> lista vazia");
    liberar_resultado_filtro(sem_criterio);

    liberar_caderno(caderno);
}

static void testar_reacao_vinculada_ao_depoimento(void) {
    Caderno *caderno = criar_caderno_exemplo();
    verificar(caderno != NULL, "criar_caderno_exemplo deve alocar");
    if (caderno == NULL) {
        return;
    }

    int id_depoimento = registrar_depoimento(caderno, 4, ID_POIROT, "Biblioteca", NOITE,
                                             ID_MARPLE, TESTEMUNHO,
                                             "Depoimento usado para linkar a reacao.");
    verificar(id_depoimento == 6, "registrar_depoimento devolve o id 6");

    registrar_reacao(caderno, 4, id_depoimento, "Miss Marple", DISCORDA);

    verificar(caderno->reacoes.tamanho == 1, "reacao foi registrada");
    verificar(caderno->reacoes.inicio != NULL, "secao de reacoes tem um no");
    if (caderno->reacoes.inicio != NULL) {
        verificar(caderno->reacoes.inicio->id == 1, "id do no de reacao e sequencial");

        Reacao *reacao = (Reacao *)caderno->reacoes.inicio->dado;
        verificar(reacao != NULL, "dado do no de reacao existe");
        if (reacao != NULL) {
            verificar(reacao->id_depoimento == id_depoimento,
                      "reacao guarda o id do depoimento");
            verificar(strcmp(reacao->quem_reagiu, "Miss Marple") == 0,
                      "reacao guarda quem reagiu");
            verificar(reacao->tipo_reacao == DISCORDA, "reacao guarda o tipo");

            Depoimento *depoimento = buscar_depoimento_por_id(caderno, reacao->id_depoimento);
            verificar(depoimento != NULL && depoimento->id_depoimento == id_depoimento,
                      "busca pelo id da reacao fecha o vinculo com o depoimento");
        }
    }

    registrar_reacao(caderno, 4, id_depoimento, "Poirot", CONCORDA);
    verificar(caderno->reacoes.tamanho == 2, "segunda reacao foi registrada");
    verificar(caderno->reacoes.fim != NULL && caderno->reacoes.fim->id == 2,
              "segundo id de reacao e 2");

    liberar_caderno(caderno);
}

static void testar_pista(void) {
    Caderno *caderno = inicializar_caderno(ID_POIROT);
    verificar(caderno != NULL, "inicializar_caderno deve alocar");
    if (caderno == NULL) {
        return;
    }

    ListaDupla *relacionados = criar_lista_dupla();
    verificar(relacionados != NULL, "lista de suspeitos relacionados deve alocar");

    int id_relacionado = ID_VIGIA;
    verificar(lista_dupla_inserir_no_fim(relacionados, ID_VIGIA, &id_relacionado) == 1,
              "suspeito relacionado foi inserido na pista");

    registrar_pista(caderno, 2, "Portão", "Chave", "Chave encontrada no portão.", relacionados);

    verificar(caderno->pistas.tamanho == 1, "pista foi registrada");
    verificar(caderno->pistas.inicio != NULL, "secao de pistas tem um no");
    if (caderno->pistas.inicio != NULL) {
        verificar(caderno->pistas.inicio->id == 1, "id do no de pista e sequencial");

        Pista *pista = (Pista *)caderno->pistas.inicio->dado;
        verificar(pista != NULL, "dado do no de pista existe");
        if (pista != NULL) {
            verificar(pista->dia == 2, "pista guarda o dia");
            verificar(strcmp(pista->local, "Portão") == 0, "pista guarda o local");
            verificar(strcmp(pista->objeto, "Chave") == 0, "pista guarda o objeto");
            verificar(strcmp(pista->descricao, "Chave encontrada no portão.") == 0,
                      "pista guarda a descricao");
            verificar(pista->suspeitos_relacionados == relacionados,
                      "pista guarda a lista de suspeitos relacionados");
        }
    }

    liberar_caderno(caderno);
}

int main(void) {
    testar_inicializacao_e_caderno_vazio();
    testar_criterios();
    testar_registro_e_busca_por_id();
    testar_comparacao_direta();
    testar_tabela_de_filtros();
    testar_atalhos();
    testar_resultado_nao_copia_e_nao_libera_depoimentos();
    testar_reacao_vinculada_ao_depoimento();
    testar_pista();

    if (falhas == 0) {
        printf("OK: todos os testes do caderno passaram\n");
        return 0;
    }

    printf("FALHAS: %d\n", falhas);
    return 1;
}
