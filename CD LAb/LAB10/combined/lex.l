%{
#include "bison.tab.h"
%}

%%
int                         { return INT; }
float                       { return FLOAT; }
char                        { return CHAR; }
if                          { return IF; }
else                        { return ELSE; }
[0-9]+                      { return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]*      { return ID; }
"=="|">="|"<="|">"|"<"|"!=" { return RELOP; }
";"                         { return ';'; }
","                         { return ','; }
"("                         { return '('; }
")"                         { return ')'; }
[+\-*/^n]                   { return yytext[0]; }
\n                          { return NL; }
[ \t]                       ;   /* ignore whitespace */
.                           { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
