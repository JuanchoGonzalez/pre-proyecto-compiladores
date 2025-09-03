%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "calc-sintaxis-pre-proyecto.tab.h"

void init();

%}

%option noyywrap
%option yylineno

%%

"int"      { return INT;    }
"bool"     { return BOOL;   }
"void"     { return VOID;   }
"return"   { return RETURN; }
"main"     { return MAIN;   }
"true"     { return TRUE;   }
"false"    { return FALSE;  }
"="        { return ASIG;   }
"+"        { return SUM;    }
"*"        { return MULT;   }
"-"        { return MEN;    }
"&&"       { return AND;    }
"||"       { return OR;     }
"!"        { return NOT;    }

[0-9]+     { yylval.nro = atoi(yytext);
		     return NRO;                
		   }

[A-Za-z][A-Za-z0-9]*  { yylval.id = strdup(yytext);
						return VAR; 			    
					  }

"=="       { return EQ;  }
"<="       { return LE;  }
">="       { return GE;  }
"<"        { return LT;  }
">"        { return GT;  }
";"        { return PYM; }
"("        { return PA;  }
")"        { return PC;  }
"{"        { return LA;  }
"}"        { return LC;  }

[ \t\r\n]+   ;  // Ignorar espacios, tabs y saltos de línea

.           { printf("Error léxico: caracter no válido '%c'\n", yytext[0]); 
			  exit(1); 
			}


%%

void yyerror(const char *s) {
    printf("-> ERROR Sintactico en la linea: %d - %s\n", yylineno, s);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	init(); // inicializada la tabla de simbolos
	yyparse();

}
