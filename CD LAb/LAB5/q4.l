%option noyywrap
%{
#include <stdio.h>
int scanf_count = 0, printf_count = 0;
%}

%%
"scanf"     { printf("READ"); scanf_count++; }   // replace scanf with READ
"printf"    { printf("WRITE"); printf_count++; } // replace printf with WRITE
.|\n        { ECHO; }                            // print other characters as-is
%%

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }
    yyin = fp;
    yylex();
    fclose(fp);

    printf("\nNumber of scanf: %d\n", scanf_count);
    printf("Number of printf: %d\n", printf_count);
    return 0;
}
