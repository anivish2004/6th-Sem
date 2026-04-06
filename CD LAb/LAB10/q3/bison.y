%{
#include <stdio.h>
#include <stdlib.h>

/* Forward declarations */
int yylex(void);
int yyerror(char *msg);
%}

%token NUMBER NL
%left '+' '-' '*' '/' '^' 'n'

%%
input : /* empty */
      | input line
      ;

line  : NL
      | exp NL { printf("Valid Postfix Expression\n"); }
      ;

exp   : NUMBER
      | exp exp '+' 
      | exp exp '-' 
      | exp exp '*' 
      | exp exp '/' 
      | exp exp '^' 
      | exp 'n'
      ;
%%

int yyerror(char *msg) {
    printf("Invalid Postfix Expression\n");
    exit(0);
}

int main() {
    printf("Enter a postfix expression:\n");
    yyparse();
    return 0;
}
