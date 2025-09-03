# pre-proyecto-compiladores

# Integrante: Juan Cruz Gonzalez, DNI:41073525

Este proyecto implementa un compilador/interprete simple para un lenguaje que soporta variables, operaciones aritméticas y booleanas, declaración de funciones `main`, y generación de pseudo-assembly. Está desarrollado en **C** utilizando **Flex** y **Bison**.

## Lenguaje pequenio


- Declaración de variables (`int`, `bool`, `void`)
- Operaciones aritméticas (`+`, `-`, `*`)
- Operaciones booleanas (`&&`, `||`, `!`)
- Función principal `main`
- Retorno de valores
- Generación de pseudo-assembly
- Evaluación semántica de tipos y variables

## Requisitos

- GCC (compilador de C)
- Flex
- Bison
- Linux o entorno compatible con la línea de comandos

---

## Compilación

Para compilar todo el proyecto, basta con ejecutar:

> make

en la raiz del proyecto, esto genera un a.out dentro de la carpeta analisis-lexico-sintactico

## Ejecucion

> make run

tambien en la raiz del proyecto esto genera el resultado del input dado que esta dentro de la carpeta analisis-lexico-sintactico llamado input-pre-proyecto.txt (se puede poner el input que querramos respetando la gramatica y la semantica del lenguaje)

## Aclaraciones

podes hacer:

> make clean 

para borrar archivos como el ejecutable a.out y el archivo generado por flex (lex.yy.c) y bison (.tab.c y tab.h)


ademas dentro de la carpeta analisis-lexico-sintactico tambien se generara el pseudo-assembler del codigo fuente, que se puede visualizar si se quiere. 