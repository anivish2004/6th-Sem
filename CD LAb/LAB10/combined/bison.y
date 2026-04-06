%{
#include <stdio.h>
#include <stdlib.h>

/* Forward declarations */
int yylex(void);
int yyerror(char *msg);
%}

/* Tokens */
%token INT FLOAT CHAR IF ELSE ID NUMBER RELOP NL
%left '+' '-' '*' '/' '^' 'n'

%%
/* Top-level: choose which kind of statement */
input : decl_stmt
      | decision_stmt
      | postfix_input
      ;

/* ---------------- Declaration ---------------- */
decl_stmt : type decl_list ';' NL { printf("Valid Declaration Statement\n"); exit(0); }
          ;

type : INT
     | FLOAT
     | CHAR
     ;

decl_list : decl_list ',' ID
          | ID
          ;

/* ---------------- Decision Making ---------------- */
decision_stmt : if_stmt NL { printf("Valid Decision Statement\n"); exit(0); }
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

/* ---------------- Postfix Calculator ---------------- */
postfix_input : /* empty */
              | postfix_input line
              ;

line : NL
     | exp NL { printf("Valid Postfix Expression\n"); exit(0); }
     ;

exp  : NUMBER
     | exp exp '+'
     | exp exp '-'
     | exp exp '*'
     | exp exp '/'
     | exp exp '^'
     | exp 'n'
     ;
%%

int yyerror(char *msg) {
    printf("Invalid Statement\n");
    exit(0);
}

int main() {
    printf("Enter input:\n");
    yyparse();
    return 0;
}
