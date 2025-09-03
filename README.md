# pre-proyecto-compiladores

**Integrante:** Juan Cruz Gonzalez  
**DNI:** 41.073.525

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

- GCC (Compilador de C)
- Flex (Analizar lexico)
- Bison (analizador sintactico)
- Linux o entorno compatible con la línea de comandos

---

## Compilación

En la **raíz del proyecto**, ejecutá:

> make

en la raiz del proyecto, esto genera un ejecutable a.out dentro de la carpeta analisis-lexico-sintactico

## Ejecucion

Por defecto si ejecutas:

> make run

el compilador correra usando el archivo de prueba test1.txt ubicado dentro de la carpeta tests

Si querés usar otro archivo de entrada, indicá el test explícitamente con:

> make run TEST=tests/test3.txt 

o 

> make run TEST=tests/testneg2.txt

para compilar el test3 y testneg2 correlativamente

## Aclaraciones

## Aclaraciones

### **Tests disponibles**

Dentro de la carpeta `tests/` se encuentran casos preparados para validar el compilador.

### **Tests positivos** ✅  
Estos tests contienen entradas **válidas**. El compilador debería procesarlos correctamente y generar el archivo `assembly.asm` dentro de la carpeta analisis-lexico-sintactico para visualizar si se quiere

- Si se corre **otro test**, el archivo **se sobrescribe** con el nuevo pseudo-assembly.

### **Tests negativos** ❌
Estos tests están diseñados para **forzar errores** y verificar que el compilador los detecte correctamente:

| Test                 | Descripción                                                     | Error que tira           |
|----------------------|-----------------------------------------------------------------|--------------------------|
| `tests/testneg1.txt` | Falta de `;` al final de la declaración de `a`.                 | Error sintáctico         |
| `tests/testneg2.txt` | Suma entre `bool` e `int` no permitida.                         | Error semántico          |
| `tests/testneg3.txt` | Variable `b` usada sin declarar.                                | Error semántico          |
| `tests/testneg4.txt` | Función `void` devuelve un valor, lo cual no está permitido.    | Error semántico          |
| `tests/testneg5.txt` | Contiene un carácter no reconocido por el lexer (`$`).          | Error léxico             |

- Si se corre alguno test obviamente no se crea el pseudo-assembly.