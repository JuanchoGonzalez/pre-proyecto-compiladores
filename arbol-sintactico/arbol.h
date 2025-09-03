#ifndef ARBOL_H
#define ARBOL_H

typedef enum {
    T_MAIN,
    T_DECL,
    T_DECLS,
    T_SENTS,
    T_SENT,
    T_INT,
    T_BOOL,
    T_VOID,
    T_RETURN,
    T_NRO,
    T_VAR,
    T_ASIG,
    T_SUM,
    T_MEN,
    T_MULT,
    T_AND,
    T_OR,
    T_TRUE,
    T_FALSE,
    T_NOT,
    T_EQ,
    T_LE,
    T_GE,
    T_LT,
    T_GT
} tipo_token;

typedef enum {
    TIPO_INT,
    TIPO_BOOL, 
    TIPO_VOID
} tipo_info;

typedef struct {
    int nro;
    char *id;
    char *pal_res_tipo;
    char *op;
    tipo_info tipo_info;
    tipo_token tipo_token; 
} info;

typedef struct nodo {
    info *valor;
    struct nodo *izq;
    struct nodo *der;
} nodo;

nodo* crearNodo(info *valorNodo);
nodo* crearArbol(info *valorNodo, nodo* hijoIzq, nodo* hijoDer);
void mostrarArbol(nodo *raiz, int nivel);
void liberarArbol(nodo *raiz);

#endif