#include <stdio.h>
#include <stdlib.h>
#include "generador_assembly.h"

void generarAssembly(nodo *raiz, FILE *fp) {
    if (!raiz || !fp) return;

    switch (raiz->valor->tipo_token) {

        case T_MAIN:
            fprintf(fp,"DECLARACIONES\n");
            generarAssembly(raiz->izq, fp);
            fprintf(fp,"SENTENCIAS\n");
            generarAssembly(raiz->der, fp);
            break;

        case T_DECLS:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp,"\n");
            break;

        case T_SENTS:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            break;

        case T_DECL:
            if (raiz->izq->valor->tipo_info == 0) {
                fprintf(fp, "DECL %s, int\n", raiz->der->valor->id);
            }else {
                fprintf(fp, "DECL %s, bool\n", raiz->der->valor->id);
            }
            break;

        case T_SENT:
            generarAssembly(raiz->der, fp);
            break;

        case T_ASIG:
            generarAssembly(raiz->der, fp);
            fprintf(fp, "Store %s\n\n", raiz->izq->valor->id);
            break;

        case T_VAR:
            fprintf(fp, "Load %s\n", raiz->valor->id);
            break;

        case T_NRO:
            fprintf(fp, "Push %d\n", raiz->valor->nro);
            break;

        case T_TRUE:
            fprintf(fp, "Push true\n");
            break;

        case T_FALSE:
            fprintf(fp, "Push false\n");
            break;

        case T_SUM:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp, "Add\n");
            break;

        case T_MEN:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp, "Sub\n");
            break;

        case T_MULT:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp, "Mult\n");
            break;

        case T_AND:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp, "And\n");
            break;

        case T_OR:
            generarAssembly(raiz->izq, fp);
            generarAssembly(raiz->der, fp);
            fprintf(fp, "Or\n");
            break;

        case T_NOT:
            generarAssembly(raiz->izq, fp);
            fprintf(fp, "Not\n");
            break;

        case T_RETURN:
            if (raiz->izq && raiz->izq->valor->tipo_token == T_VAR) {
                fprintf(fp, "Ret %s\n", raiz->izq->valor->id);
            } else if (raiz->izq && raiz->izq->valor->tipo_token == T_NRO) {
                fprintf(fp, "Ret %d\n", raiz->izq->valor->nro);
            } else {
                generarAssembly(raiz->izq, fp);
            }
            break;

        default:
            fprintf(stderr, "Error: token no válido (%d)\n", raiz->valor->tipo_token);
            exit(1);
    }
}
