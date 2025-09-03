#include "interprete.h"
#include <stdio.h>
#include <stdlib.h>

int interpretar(nodo *raiz, tabla_simbolos *ts) {
    if (!raiz) return 0;

    switch (raiz->valor->tipo_token) {

        case T_MAIN:
            return interpretar(raiz->der, ts);
        
        case T_SENTS: {
            interpretar(raiz->izq, ts);
            if (raiz->der) { // si hay mas sentencias
                return interpretar(raiz->der, ts);
            }
            return 0;
        }

        case T_ASIG: {
            int result = interpretar(raiz->der, ts);
            info *var = buscar(ts, raiz->izq->valor->id);
            var->nro = result;
            return result;
        }

        case T_VAR:
            int result = buscar(ts, raiz->valor->id)->nro;
            return result;

        case T_NRO:
            return raiz->valor->nro;

        case T_TRUE:
            return 1;

        case T_FALSE:
            return 0;

        case T_SUM:
            return interpretar(raiz->izq, ts) + interpretar(raiz->der, ts);

        case T_MEN:
            return interpretar(raiz->izq, ts) - interpretar(raiz->der, ts);

        case T_MULT:
            return interpretar(raiz->izq, ts) * interpretar(raiz->der, ts);

        case T_AND:
            return interpretar(raiz->izq, ts) && interpretar(raiz->der, ts);

        case T_OR:
            return interpretar(raiz->izq, ts) || interpretar(raiz->der, ts);

        case T_NOT:
            return !interpretar(raiz->izq, ts);

        case T_RETURN:
            if (raiz->izq)
                return interpretar(raiz->izq, ts);
            else
                return 0;

        default:
            fprintf(stderr, "Error: token no valido para este lenguaje (%d)\n", raiz->valor->tipo_token);
            exit(1);
    }
}
