%{
#include "bison.tab.h"
%}

%%
[0-9]+          { return NUMBER; }
\n              { return NL; }
[+\-*/^n]       { return yytext[0]; }
[ \t]           ;   /* ignore whitespace */
.               { return yytext[0]; }
%%

int yywrap() {
    return 1;
}
