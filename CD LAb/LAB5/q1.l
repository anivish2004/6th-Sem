%option noyywrap
%{
#include <stdio.h>
int vowels = 0, consonants = 0;
%}

%%
[aeiouAEIOU]   { vowels++; }
[a-zA-Z]       { consonants++; }
.|\n           { /* ignore other characters */ }
%%

int main() {
    printf("Enter text (Ctrl+D to end input):\n");
    yylex();
    printf("Number of vowels: %d\n", vowels);
    printf("Number of consonants: %d\n", consonants);
    return 0;
}
