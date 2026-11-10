CC := gcc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -g
BUILD := build

RAYLIB_CFLAGS := $(shell pkg-config --cflags raylib)
RAYLIB_LIBS := $(shell pkg-config --libs raylib)

JOGO_SRCS := $(filter-out teste_lista.c,$(wildcard *.c))
CABECALHOS := $(wildcard *.h)

.PHONY: all jogo test valgrind clean

all: jogo

jogo: $(BUILD)/jogo

$(BUILD)/jogo: $(JOGO_SRCS) $(CABECALHOS)
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) $(JOGO_SRCS) -o $@ $(RAYLIB_LIBS) -lm

$(BUILD)/teste_lista_simples: teste_lista.c lista-simples.c lista-simples.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) teste_lista.c lista-simples.c -o $@

$(BUILD)/teste_lista_dupla: teste_lista.c lista-dupla.c lista-dupla.h
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) teste_lista.c lista-dupla.c -o $@

test: $(BUILD)/teste_lista_simples $(BUILD)/teste_lista_dupla
	./$(BUILD)/teste_lista_simples && ./$(BUILD)/teste_lista_dupla

valgrind: $(BUILD)/teste_lista_simples $(BUILD)/teste_lista_dupla
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(BUILD)/teste_lista_simples

clean:
	rm -rf $(BUILD)
