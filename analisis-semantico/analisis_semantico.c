#include <stdio.h>
#include <stdlib.h>
#include "analisis_semantico.h"

static tipo_info tipo_main_global = TIPO_VOID;

/*
    esta funcion es para analizar semantica ya insertado 
    en la tabla de simbolos y ya construido el arbol
*/
void recorrerArbol(nodo *raiz, tabla_simbolos *ts){
    if (!raiz) return;

    // guardar que retorna el main antes de ir a los hijos
    if (raiz->valor->tipo_token == T_MAIN) {
        tipo_main_global = raiz->valor->tipo_info;
    }

    recorrerArbol(raiz->izq, ts);
    recorrerArbol(raiz->der, ts);

    switch(raiz->valor->tipo_token) {

        case T_DECL: {
            // se verifica si es int algo; o bool algo;, no hay mas por ahora
            if (raiz->izq->valor->tipo_token != T_INT && 
                raiz->izq->valor->tipo_token != T_BOOL) {
                printf("Error: tipo de declaración inválido para variable\n");
                exit(1);
            }
            // se verifica si de lado derecho habia una variable
            if (raiz->der->valor->tipo_token != T_VAR) {
                printf("Error: se esperaba un identificador\n");
                exit(1);
            }
            break;
        }

        case T_ASIG: {
            // se verifica si de lado izquierdo habia una variable
            if (raiz->izq->valor->tipo_token != T_VAR) {
                printf("Error: lado izquierdo debe ser una variable\n");
                exit(1);
            }

            // si hay una variable, existe? y esta declarada para usarse?
            info* var_izq = buscar(ts, raiz->izq->valor->id);
            if (var_izq == NULL) {
                printf("Error semantico: variable %s no declarada\n", raiz->izq->valor->id);
                exit(1);
            }

            // var_izq tiene todo lo necesario mas el tipo, que lo busca en tabla de simbolos
            raiz->izq->valor->tipo_info = var_izq->tipo_info;

            // chequeo de tipos
            if (raiz->der->valor->tipo_info != var_izq->tipo_info) {
                printf("Error: tipos incompatibles en asignación\n");
                printf("  Variable %s es tipo %d, expresión es tipo %d\n", 
                       raiz->izq->valor->id, var_izq->tipo_info, raiz->der->valor->tipo_info);
                exit(1);
            }

            // si todo fue bien la raiz(asignacion) es del tipo de los hijos
            raiz->valor->tipo_info = var_izq->tipo_info;
            break;
        }

        case T_VAR: {
            // si tengo una variable, existe en la tabla?
            info* var_info = buscar(ts, raiz->valor->id);
            if (var_info == NULL) {
                printf("Error semantico: variable %s no declarada\n", raiz->valor->id);
                exit(1);
            }
            
            // si existe el tipo de la variable se le asigno al padre
            raiz->valor->tipo_info = var_info->tipo_info;
            break;
        }

        case T_NRO: {
            raiz->valor->tipo_info = TIPO_INT;
            break;
        }

        case T_SUM:
        case T_MULT:
        case T_MEN: {
            // ambos operandos sean int si son operaciones aritmeticas
            if (raiz->izq->valor->tipo_info != TIPO_INT || 
                raiz->der->valor->tipo_info != TIPO_INT) {
                printf("Error: operandos de operación aritmética deben ser INT\n");
                printf("  Operando izq: %d, Operando der: %d\n", 
                       raiz->izq->valor->tipo_info, raiz->der->valor->tipo_info);
                exit(1);
            }
            
            // el resultado de operaciones aritméticas es int
            raiz->valor->tipo_info = TIPO_INT;
            break;
        }

        case T_AND:
        case T_OR: {
            // ambos operandos sean bool si son operaciones booleanas
            if (raiz->izq->valor->tipo_info != TIPO_BOOL || 
                raiz->der->valor->tipo_info != TIPO_BOOL) {
                printf("Error: operandos de operación booleana deben ser BOOL\n");
                printf("  Operando izq: %d, Operando der: %d\n", 
                       raiz->izq->valor->tipo_info, raiz->der->valor->tipo_info);
                exit(1);
            }
            
            // el resultado de operaciones aritméticas es int
            raiz->valor->tipo_info = TIPO_BOOL;
            break;
        }
        
        case T_NOT: {
            if (raiz->izq->valor->tipo_info != TIPO_BOOL) {
                printf("Error: operando de NOT debe ser BOOL\n");
                exit(1);
            }
            raiz->valor->tipo_info = TIPO_BOOL;
            break;
        }

        case T_TRUE:
        case T_FALSE: {
            // Los literales booleanos son bool
            raiz->valor->tipo_info = TIPO_BOOL;
            break;
        }


        case T_RETURN: {
            if (raiz->izq) {
                // return con expresión
                if (raiz->izq->valor->tipo_info != tipo_main_global) {
                    printf("Error: tipo de retorno incompatible\n");
                    printf("  Main es tipo %d, return es tipo %d\n", 
                           tipo_main_global, raiz->izq->valor->tipo_info);
                    exit(1);
                }
                raiz->valor->tipo_info = raiz->izq->valor->tipo_info;
            } else {
                // return sin expresión
                if (tipo_main_global != TIPO_VOID) {
                    printf("Error: return vacío en función que no es void\n");
                    exit(1);
                }
                raiz->valor->tipo_info = TIPO_VOID;
            }
            break;
        }
        
        // casos donde la semantica no importa
        case T_MAIN:
        case T_DECLS:
        case T_SENTS:
        case T_SENT:
        case T_INT:
        case T_BOOL:
        case T_VOID:
        case T_EQ:
        case T_LE:
        case T_GE:
        case T_LT:
        case T_GT:
            break;

        default:
            printf("Token no declarado: %d\n", raiz->valor->tipo_token);            
    }
}