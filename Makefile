CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
BUILD := build

# ---------------------------------------------------------------------------
# Dependências externas (git submodules)
# ---------------------------------------------------------------------------
RAYLIB_DIR ?= raylib
RAYLIB_SRC := $(RAYLIB_DIR)/src
RAYLIB_LIB := $(RAYLIB_SRC)/libraylib.a

RAYLIB_CFLAGS := -I$(RAYLIB_SRC)
# Bibliotecas do sistema necessárias para linkar a raylib estática no Linux.
RAYLIB_LIBS := -L$(RAYLIB_SRC) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

JOGO_SRCS := $(filter-out teste_lista.c,$(wildcard *.c))
CABECALHOS := $(wildcard *.h)

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
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) $(JOGO_SRCS) -o $@ $(RAYLIB_LIBS)

# ---------------------------------------------------------------------------
# Testes da lista
# ---------------------------------------------------------------------------
$(BUILD)/teste_lista: teste_lista.c lista.c lista.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) teste_lista.c lista.c -o $@

test: $(BUILD)/teste_lista
	./$(BUILD)/teste_lista

valgrind: $(BUILD)/teste_lista
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(BUILD)/teste_lista

# ---------------------------------------------------------------------------
# Limpeza
# ---------------------------------------------------------------------------
clean:
	rm -rf $(BUILD)

distclean: clean
	-$(MAKE) -C $(RAYLIB_SRC) clean

FORCE:
