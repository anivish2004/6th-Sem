%option noyywrap
%{
#include <stdio.h>
int words = 0, chars = 0, blanks = 0, lines = 0;
%}

%%
[ \t]+        { blanks += yyleng; words++; }   // spaces/tabs count as blanks, also word separator
\n            { lines++; words++; }            // newline counts as line break and word separator
.             { chars++; }                     // any other character counts as a character
%%

int main() {
    printf("Enter text (Ctrl+D to end input):\n");
    yylex();
    printf("Number of characters: %d\n", chars);
    printf("Number of blanks: %d\n", blanks);
    printf("Number of words: %d\n", words);
    printf("Number of lines: %d\n", lines);
    return 0;
}
