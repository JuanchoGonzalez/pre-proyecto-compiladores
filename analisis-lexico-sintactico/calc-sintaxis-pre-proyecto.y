%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../arbol-sintactico/arbol.h"
#include "../tabla-simbolos/tabla_simbolos.h"
#include "../analisis-semantico/analisis_semantico.h"
#include "../interprete/interprete.h"
#include "../generador-assembly/generador_assembly.h"

nodo* raiz = NULL;
tabla_simbolos *ts = NULL;

int yylex();
void yyerror(const char *s);

void init() {
    ts = malloc(sizeof(tabla_simbolos));
    if (!ts) {
        fprintf(stderr, "Error: no se pudo asignar memoria para la tabla\n");
        exit(1);
    }
    inicializar(ts);
}

%}

%code requires {
    #include "../arbol-sintactico/arbol.h"
}

%union {
    char *id;
    int nro;
    nodo* nodo_val;
}

%token TRUE FALSE
%token INT BOOL VOID RETURN MAIN ASIG SUM MEN MULT AND OR NOT EQ LE GE LT GT PYM PA PC LA LC
%token <nro> NRO 
%token <id> VAR

%type <nodo_val> prog decls decl sents sent e tipo

%left OR
%left AND
%right NOT
%left SUM MEN
%left MULT

%%

prog: 
    tipo MAIN PA PC LA decls sents LC {
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("MAIN");
        ninfo->tipo_token = T_MAIN;
        ninfo->tipo_info = $1->valor->tipo_info;

        $$ = crearArbol(ninfo ,$6, $7);
        raiz = $$;
        
        recorrerArbol(raiz, ts);        
        printf("\n");

        // Crear el archivo de salida
        FILE *fp = fopen("analisis-lexico-sintactico/assembly.asm", "w");
        if (!fp) {
            perror("Error al crear el archivo");
            exit(1);
        }

        // Generar pseudo-assembly
        generarAssembly(raiz, fp);

        fclose(fp);

        printf("\nArchivo 'assembly.asm' generado correctamente.\n");


        int result = interpretar(raiz, ts);
        printf("Resultado: %d", result);
        printf("\n");
        printf("\n");

        /*
            descomentar si se quiere mostrar el arbol, 
            para la jerarquia ,no muy lindo a la vista
        */
        mostrarArbol(raiz, 0);

        liberarArbol(raiz);
    }
    ;

tipo: 
    INT  { 
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("INT");
        ninfo->tipo_info = TIPO_INT; 
        ninfo->tipo_token = T_INT;
        
        $$ = crearNodo(ninfo);
    }

    | BOOL { 
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("BOOL");
        ninfo->tipo_info = TIPO_BOOL; 
        ninfo->tipo_token = T_BOOL;

        $$ = crearNodo(ninfo);
    }

    | VOID { 
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("VOID");
        ninfo->tipo_info = TIPO_VOID; 
        ninfo->tipo_token = T_VOID;

        $$ = crearNodo(ninfo);
    }
    ;

decls: 
    decl decls {
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("DECLS");
        ninfo->tipo_token = T_DECLS;

        $$ = crearArbol(ninfo, $1, $2);
    }

    | decl { $$ = $1; }
    ;

decl: 
    tipo VAR PYM {
        info *ninfo = malloc(sizeof(info));
        ninfo->tipo_info = $1->valor->tipo_info;
        ninfo->tipo_token = T_DECL;

        info *ninfoVar = malloc(sizeof(info));
        ninfoVar->id = strdup($2);
        ninfoVar->tipo_info = $1->valor->tipo_info;
        ninfoVar->tipo_token = T_VAR;
        nodo* nodoVar = crearNodo(ninfoVar);

        if (!insertar(ts, ninfoVar)) {
            // redeclaraciones
            yyerror("Error sintactico: variable ya declarada");
            exit(1);
        }

        $$ = crearArbol(ninfo, $1, nodoVar);
    }
    ;

sents: 
    sent sents {
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("SENTS");
        ninfo->tipo_token = T_SENTS;

        $$ = crearArbol(ninfo, $1, $2);
    }

    | sent { $$ = $1; }

    | RETURN e PYM {
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("RETURN");
        ninfo->tipo_token = T_RETURN;

        $$ = crearArbol(ninfo, $2, NULL);    
    }

    | RETURN PYM {
        info *ninfo = malloc(sizeof(info));
        ninfo->pal_res_tipo = strdup("RETURN");
        ninfo->tipo_token = T_RETURN;

        $$ = crearArbol(ninfo, NULL, NULL);
    }

    ;

sent: 
    VAR ASIG e PYM {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("=");
        ninfo->tipo_token = T_ASIG;

        info *ninfoVar = malloc(sizeof(info));
        ninfoVar->id = strdup($1);
        ninfoVar->tipo_token = T_VAR;
       
        nodo* nodoVar = crearNodo(ninfoVar);

        $$ = crearArbol(ninfo, nodoVar, $3);
    }
    ;

e:
    e SUM e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("+");
        ninfo->tipo_token = T_SUM;
        ninfo->tipo_info = TIPO_INT;

        $$ = crearArbol(ninfo, $1, $3);
    }

    | e MULT e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("*");
        ninfo->tipo_token = T_MULT;
        ninfo->tipo_info = TIPO_INT;

        $$ = crearArbol(ninfo, $1, $3);
    }

    | e MEN e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("-");
        ninfo->tipo_token = T_MEN;
        ninfo->tipo_info = TIPO_INT;

        $$ = crearArbol(ninfo, $1, $3);
    }

    | e AND e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("&&");
        ninfo->tipo_token = T_AND;
        ninfo->tipo_info = TIPO_BOOL;

        $$ = crearArbol(ninfo, $1, $3);
    }

    | e OR e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("||");
        ninfo->tipo_token = T_OR;
        ninfo->tipo_info = TIPO_BOOL;

        $$ = crearArbol(ninfo, $1, $3);
    }
    
    | NOT e {
        info *ninfo = malloc(sizeof(info));
        ninfo->op = strdup("!");
        ninfo->tipo_token = T_NOT;
        ninfo->tipo_info = TIPO_BOOL;

        $$ = crearArbol(ninfo, $2, NULL);
    }

    | PA e PC {
        $$ = $2;
    }

    | NRO {
        info *ninfo = malloc(sizeof(info));
        ninfo->nro = $1;
        ninfo->tipo_token = T_NRO;

        $$ = crearNodo(ninfo);
    }

    | TRUE {
        info *ninfo = malloc(sizeof(info));
        ninfo->nro = 1;
        ninfo->tipo_token = T_TRUE;
        ninfo->tipo_info = TIPO_BOOL;

        $$ = crearNodo(ninfo);
    }

    | FALSE {
        info *ninfo = malloc(sizeof(info));
        ninfo->nro = 0;
        ninfo->tipo_token = T_FALSE;
        ninfo->tipo_info = TIPO_BOOL;

        $$ = crearNodo(ninfo);
    }

    | VAR {
        info *ninfo = malloc(sizeof(info));
        ninfo->id = strdup($1);
        ninfo->tipo_token = T_VAR;
        
        $$ = crearNodo(ninfo);
    }


;


%%