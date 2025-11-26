# Detecta o sistema operacional
ifdef OS
	OS := $(strip $(OS))
else
	OS := $(strip $(shell uname))
endif

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC = jogo.c funcoes.c menu_principal.c
OBJ = jogo.o funcoes.o menu_principal.o

# Configurações por sistema
ifeq ($(OS),Windows_NT)
	BIN = jogo.exe
	RM = del /Q /F
	RUN = ./jogo.exe
else
	BIN = jogo
	RM = rm -f
	RUN = ./jogo
endif

# Regra principal
all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

jogo.o: jogo.c funcoes.h
	$(CC) $(CFLAGS) -c jogo.c

funcoes.o: funcoes.c funcoes.h
	$(CC) $(CFLAGS) -c funcoes.c

menu_principal.o: menu_principal.c funcoes.h
	$(CC) $(CFLAGS) -c menu_principal.c

# Executar o programa
run:
	$(RUN)

# Limpar arquivos gerados
clean:
	$(RM) $(OBJ) $(BIN)
