%option noyywrap
%{
#include <stdio.h>
int pos_int = 0, neg_int = 0, pos_float = 0, neg_float = 0;
%}

%%
[+]?[0-9]+              { pos_int++; }                // positive integers
-[0-9]+                 { neg_int++; }                // negative integers
[+]?[0-9]+"."[0-9]+     { pos_float++; }              // positive floating-point numbers
-[0-9]+"."[0-9]+        { neg_float++; }              // negative floating-point numbers
.|\n                    { /* ignore other characters */ }
%%

int main() {
    printf("Enter numbers (Ctrl+D to end input):\n");
    yylex();
    printf("Positive integers: %d\n", pos_int);
    printf("Negative integers: %d\n", neg_int);
    printf("Positive floating-point numbers: %d\n", pos_float);
    printf("Negative floating-point numbers: %d\n", neg_float);
    return 0;
}
