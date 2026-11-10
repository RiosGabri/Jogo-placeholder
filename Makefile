CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
BUILD := build

# ---------------------------------------------------------------------------
# Estrutura de pastas do projeto
# ---------------------------------------------------------------------------
ESTRUTURAS_DIR := estruturas
TEST_DIR := test

INCLUDES := -I. -I$(ESTRUTURAS_DIR)

# ---------------------------------------------------------------------------
# Dependências externas (git submodules)
# ---------------------------------------------------------------------------
RAYLIB_DIR ?= raylib
RAYLIB_SRC := $(RAYLIB_DIR)/src
RAYLIB_LIB := $(RAYLIB_SRC)/libraylib.a

RAYLIB_CFLAGS := -I$(RAYLIB_SRC)
# Bibliotecas do sistema necessárias para linkar a raylib estática no Linux.
RAYLIB_LIBS := -L$(RAYLIB_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# ---------------------------------------------------------------------------
# Fontes
# ---------------------------------------------------------------------------
# Jogo: arquivos na raiz e em estruturas/ (a pasta test/ fica de fora).
# Como as três listas usam os mesmos nomes de função, dá para escolher quais
# implementações linkar, por exemplo:
#   make JOGO_ESTRUTURAS=estruturas/lista-simples.c
JOGO_ESTRUTURAS ?= $(wildcard $(ESTRUTURAS_DIR)/*.c)
JOGO_SRCS := $(wildcard *.c) $(JOGO_ESTRUTURAS)
CABECALHOS := $(wildcard *.h) $(wildcard $(ESTRUTURAS_DIR)/*.h)

# Cada teste linka apenas a implementação que está testando, porque as
# listas compartilham os mesmos nomes de funções.
TESTE_LISTA_SIMPLES_SRC := $(TEST_DIR)/teste_lista_simples.c
TESTE_LISTA_DUPLA_SRC := $(TEST_DIR)/teste_lista_dupla.c
TESTE_LISTA_CIRCULAR_SRC := $(TEST_DIR)/teste_lista_circular.c

TESTE_LISTA_SIMPLES_IMPL := $(ESTRUTURAS_DIR)/lista-simples.c
TESTE_LISTA_DUPLA_IMPL := $(ESTRUTURAS_DIR)/lista-dupla.c
TESTE_LISTA_CIRCULAR_IMPL := $(ESTRUTURAS_DIR)/lista-dupla-circular.c

# Só entram no alvo test os arquivos de teste existentes e não vazios.
# Assim o Makefile continua válido enquanto os testes novos são escritos.
TESTE_BINS :=
ifneq ($(strip $(shell test -s $(TESTE_LISTA_SIMPLES_SRC) && echo 1)),)
TESTE_BINS += $(BUILD)/teste_lista_simples
endif
ifneq ($(strip $(shell test -s $(TESTE_LISTA_DUPLA_SRC) && echo 1)),)
TESTE_BINS += $(BUILD)/teste_lista_dupla
endif
ifneq ($(strip $(shell test -s $(TESTE_LISTA_CIRCULAR_SRC) && echo 1)),)
TESTE_BINS += $(BUILD)/teste_lista_circular
endif

.PHONY: all jogo test valgrind submodules raylib clean distclean FORCE

all: jogo

jogo: $(BUILD)/jogo

# ---------------------------------------------------------------------------
# raylib (submodule)
# ---------------------------------------------------------------------------
submodules:
	git submodule update --init --recursive

# Sempre chama o make da raylib: ele próprio decide se precisa recompilar.
# Assim, mudanças nos fontes do submodule também são detectadas.
$(RAYLIB_LIB): FORCE
	@if [ ! -f "$(RAYLIB_SRC)/Makefile" ]; then \
		echo "Erro: raylib não encontrada em $(RAYLIB_SRC)."; \
		echo "Rode: make submodules"; \
		exit 1; \
	fi
	$(MAKE) -C $(RAYLIB_SRC) PLATFORM=PLATFORM_DESKTOP

raylib: $(RAYLIB_LIB)

# ---------------------------------------------------------------------------
# Jogo
# ---------------------------------------------------------------------------
$(BUILD)/jogo: $(JOGO_SRCS) $(CABECALHOS) $(RAYLIB_LIB)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $(RAYLIB_CFLAGS) $(JOGO_SRCS) -o $@ $(RAYLIB_LIBS)

# ---------------------------------------------------------------------------
# Testes das estruturas
# ---------------------------------------------------------------------------
$(BUILD)/teste_lista_simples: $(TESTE_LISTA_SIMPLES_SRC) $(TESTE_LISTA_SIMPLES_IMPL) \
		$(ESTRUTURAS_DIR)/lista-simples.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(TESTE_LISTA_SIMPLES_IMPL) -o $@

$(BUILD)/teste_lista_dupla: $(TESTE_LISTA_DUPLA_SRC) $(TESTE_LISTA_DUPLA_IMPL) \
		$(ESTRUTURAS_DIR)/lista-dupla.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(TESTE_LISTA_DUPLA_IMPL) -o $@

$(BUILD)/teste_lista_circular: $(TESTE_LISTA_CIRCULAR_SRC) $(TESTE_LISTA_CIRCULAR_IMPL) \
		$(ESTRUTURAS_DIR)/lista-dupla-circular.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(INCLUDES) $< $(TESTE_LISTA_CIRCULAR_IMPL) -o $@

test: $(TESTE_BINS)
	@status=0; \
	for teste in $(TESTE_BINS); do \
		echo "== $$teste"; \
		./$$teste || status=1; \
	done; \
	exit $$status

valgrind: $(TESTE_BINS)
	@status=0; \
	for teste in $(TESTE_BINS); do \
		echo "== $$teste"; \
		valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$$teste || status=1; \
	done; \
	exit $$status

# ---------------------------------------------------------------------------
# Limpeza
# ---------------------------------------------------------------------------
clean:
	rm -rf $(BUILD)

# Remove também os artefatos compilados da raylib (o código do submodule é mantido).
distclean: clean
	-$(MAKE) -C $(RAYLIB_SRC) clean

FORCE:
