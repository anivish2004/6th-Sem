%{
#include <stdio.h>
#include <stdlib.h>

/* Forward declarations */
int yylex(void);
int yyerror(char *msg);
%}

%token INT FLOAT CHAR ID NL

%%
stmt : type decl_list ';' NL { printf("Valid Declaration\n"); exit(0); }
     ;

type : INT
     | FLOAT
     | CHAR
     ;

decl_list : decl_list ',' ID
          | ID
          ;
%%

int yyerror(char *msg) {
    printf("Invalid Declaration\n");
    exit(0);
}

int main() {
    printf("Enter a declaration statement:\n");
    yyparse();
    return 0;
}
