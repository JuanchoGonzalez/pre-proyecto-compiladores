#ifndef INTERPRETE_H
#define INTERPRETE_H

#include "../arbol-sintactico/arbol.h"
#include "../tabla-simbolos/tabla_simbolos.h"

int interpretar(nodo *raiz, tabla_simbolos *ts);

#endif