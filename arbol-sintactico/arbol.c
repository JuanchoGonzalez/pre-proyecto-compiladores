#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbol.h"

nodo* crearNodo(info *valorNodo) {
    nodo* n = (nodo*)malloc(sizeof(nodo));

    n->valor = valorNodo;
    n->izq = NULL;
    n->der = NULL;

    return n;
}

nodo* crearArbol(info *valorNodo, nodo *hijoIzq, nodo *hijoDer) {
    nodo* n = (nodo*)malloc(sizeof(nodo));

    n->valor = valorNodo;
    n->izq = hijoIzq;
    n->der = hijoDer;

    return n;
}


void mostrarArbol(nodo *raiz, int nivel) {
    if (!raiz) return;

    for (int i = 0; i < nivel; i++) {
        printf("   ");
    }

    switch (raiz->valor->tipo_token) {
        case T_VAR: printf("VAR: %s\n", raiz->valor->id); break;
        case T_NRO: printf("NRO: %d\n", raiz->valor->nro); break;
        case T_ASIG: printf("ASIG: %s\n", raiz->valor->op); break;
        case T_SUM: printf("SUM: %s\n", raiz->valor->op); break;
        case T_MEN: printf("MEN: %s\n", raiz->valor->op); break;
        case T_MULT: printf("MULT: %s\n", raiz->valor->op); break;
        case T_AND: printf("AND: %s\n", raiz->valor->op); break;
        case T_OR: printf("OR: %s\n", raiz->valor->op); break;
        case T_NOT: printf("NOT: %s\n", raiz->valor->op); break;
        case T_TRUE: printf("TRUE\n"); break;
        case T_FALSE: printf("FALSE\n"); break;
        case T_EQ: case T_LE: case T_GE: case T_LT: case T_GT:
            printf("OP_REL: %s\n", raiz->valor->op); break;
        case T_INT: case T_BOOL: case T_VOID:
            printf("TIPO: %s\n", raiz->valor->pal_res_tipo); break;
        case T_RETURN: printf("RETURN\n"); break;
        case T_MAIN: printf("MAIN\n"); break;
        case T_DECL: printf("DECL:\n"); break;
        case T_DECLS: printf("DECLS\n"); break;
        case T_SENTS: printf("SENTS\n"); break;
        default: printf("Nodo desconocido\n"); break;
    }

    mostrarArbol(raiz->izq, nivel + 1);
    mostrarArbol(raiz->der, nivel + 1);
}



void liberarArbol(nodo *raiz) {
    if (!raiz) return;

    liberarArbol(raiz->izq);
    liberarArbol(raiz->der);

    switch (raiz->valor->tipo_token) {
        case T_VAR: 
            if (raiz->valor->id) 
                free(raiz->valor->id);
            break;

        case T_ASIG: case T_SUM: case T_MEN: case T_MULT:
        case T_AND: case T_OR: case T_NOT:
        case T_EQ: case T_LE: case T_GE: case T_LT: case T_GT:
            if (raiz->valor->op) 
                free(raiz->valor->op);
            break;

        case T_INT: case T_BOOL: case T_VOID: case T_RETURN: case T_MAIN:
        case T_DECLS: case T_SENTS: case T_DECL: case T_SENT:
            if (raiz->valor->pal_res_tipo) 
                free(raiz->valor->pal_res_tipo);
            break;

        default: 
            break;
    }

    free(raiz);
}
