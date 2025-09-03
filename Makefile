# Compilador y flags
CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = $(DIR_LEXICO)/a.out

# Directorios principales
DIR_LEXICO = analisis-lexico-sintactico
DIR_ARBOL = arbol-sintactico
DIR_TABLA = tabla-simbolos
DIR_SEMANTICO = analisis-semantico
DIR_INTERPRETE = interprete
DIR_ASSEMBLER = generador-assembly

# Archivos de entrada para Bison y Flex
LEX_SRC = $(DIR_LEXICO)/calc-lexico-pre-proyecto.l
YACC_SRC = $(DIR_LEXICO)/calc-sintaxis-pre-proyecto.y

# Archivos generados por Bison y Flex
LEX_OUT = $(DIR_LEXICO)/lex.yy.c
YACC_C = $(DIR_LEXICO)/calc-sintaxis-pre-proyecto.tab.c
YACC_H = $(DIR_LEXICO)/calc-sintaxis-pre-proyecto.tab.h

# Archivos fuente adicionales
CFILES = \
    $(DIR_ARBOL)/arbol.c \
    $(DIR_TABLA)/tabla_simbolos.c \
    $(DIR_SEMANTICO)/analisis_semantico.c \
    $(DIR_INTERPRETE)/interprete.c \
    $(DIR_ASSEMBLER)/generador_assembly.c

# Includes para headers
INCLUDES = \
    -I$(DIR_ARBOL) \
    -I$(DIR_TABLA) \
    -I$(DIR_SEMANTICO) \
    -I$(DIR_INTERPRETE) \
    -I$(DIR_ASSEMBLER) \
    -I$(DIR_LEXICO)

# Regla principal
all: $(TARGET)

# Compilación final
$(TARGET): $(YACC_C) $(LEX_OUT) $(CFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(LEX_OUT) $(YACC_C) $(CFILES) $(INCLUDES) -lm

# Generar archivos de Bison (.tab.c y .tab.h) directamente en analisis-lexico-sintactico/
$(YACC_C) $(YACC_H): $(YACC_SRC)
	bison --output=$(YACC_C) --defines=$(YACC_H) $(YACC_SRC)

# Generar archivo de Flex (lex.yy.c) dentro de analisis-lexico-sintactico/
$(LEX_OUT): $(LEX_SRC) $(YACC_H)
	flex -o $(LEX_OUT) $(LEX_SRC)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET) $(DIR_LEXICO)/input-pre-proyecto.txt

# Limpiar archivos generados
clean:
	rm -f $(TARGET) $(LEX_OUT) $(YACC_C) $(YACC_H)

.PHONY: all run debug clean
