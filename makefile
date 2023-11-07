# Set project directory one level above of Makefile directory. $(CURDIR) is a GNU make variable containing the path to the current working directory
PROJDIR := $(PWD)
UTILSDIR := $(PROJDIR)/utils

OBJECTS-L = $(PROJDIR)/lista_adjacencias.c $(UTILSDIR)/*.c
OBJECTS-M = $(PROJDIR)/matriz_adjacencias.c $(UTILSDIR)/*.c

RESULT-L = $(PROJDIR)/lista_adj.out
RESULT-M = $(PROJDIR)/matriz_adj.out

COMPILER = gcc

all:
	$(info Compilando, lista_adjacencias.c e matriz_adjacencias.c)
	$(COMPILER) $(OBJECTS-L) -o $(RESULT-L)
	$(COMPILER) $(OBJECTS-M) -o $(RESULT-M)

lista:
	$(info Compilando, lista_adjacencias.c)
	$(COMPILER) $(OBJECTS-L) -o $(RESULT-L)

matriz:
	$(info Compilando, matriz_adjacencias.c)
	$(COMPILER) $(OBJECTS-M) -o $(RESULT-M)

debbug:
	$(info Compilando em modo Debbug, lista_adjacencias.c e matriz_adjacencias.c)
	$(COMPILER) -g $(OBJECTS-L) -o $(RESULT-L)
	$(COMPILER) -g $(OBJECTS-M) -o $(RESULT-M)

debbug-l:
	$(info Compilando em modo Debbug, lista_adjacencias.c)
	$(COMPILER) -g $(OBJECTS-L) -o $(RESULT-L)

debbug-m:
	$(info Compilando em modo Debbug, matriz_adjacencias.c)
	$(COMPILER) -g $(OBJECTS-M) -o $(RESULT-M)

clean:
	-rm $(OBJECTS-L) $(OBJECTS-M) 