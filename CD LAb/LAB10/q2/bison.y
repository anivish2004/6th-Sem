%{
#include <stdio.h>
#include <stdlib.h>

/* Forward declarations */
int yylex(void);
int yyerror(char *msg);
%}

%token IF ELSE ID NUMBER RELOP NL

%%
stmt : if_stmt NL { printf("Valid Decision Statement\n"); exit(0); }
     ;

if_stmt : IF '(' condition ')' stmt_body
        | IF '(' condition ')' stmt_body ELSE stmt_body
        ;

condition : ID RELOP NUMBER
          | NUMBER RELOP ID
          | ID RELOP ID
          | NUMBER RELOP NUMBER
          ;

stmt_body : ID ';'
          ;
%%

int yyerror(char *msg) {
    printf("Invalid Decision Statement\n");
    exit(0);
}

int main() {
    printf("Enter a decision statement:\n");
    yyparse();
    return 0;
}
