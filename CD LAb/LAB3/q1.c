#include <stdio.h>
#include <stdlib.h>

void checkArithmetic(FILE *fp, char c) {
    char next;
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        next = fgetc(fp);
        if ((c == '+' && next == '+') || (c == '-' && next == '-')) {
            printf("\n Increment/Decrement operator: %c%c", c, next);
        } else if (next == '=') {
            printf("\n Compound assignment operator: %c=", c);
        } else {
            printf("\n Arithmetic operator: %c", c);
            ungetc(next, fp);
        }
    }
}

void checkRelational(FILE *fp, char c) {
    char next;
    if (c == '=' || c == '<' || c == '>' || c == '!') {
        next = fgetc(fp);
        if (next == '=') {
            printf("\n Relational operator: %c=", c);
        } else {
            if (c == '=') {
                printf("\n Assignment operator: =");
            } else {
                printf("\n Relational operator: %c", c);
            }
            ungetc(next, fp);
        }
    }
}

void checkLogical(FILE *fp, char c) {
    char next;
    if (c == '&') {
        next = fgetc(fp);
        if (next == '&') {
            printf("\n Logical operator: &&");
        } else {
            ungetc(next, fp);
        }
    } else if (c == '|') {
        next = fgetc(fp);
        if (next == '|') {
            printf("\n Logical operator: ||");
        } else {
            ungetc(next, fp);
        }
    }
}

int main() {
    char c;
    FILE *fp = fopen("q1.c", "r");

    if (fp == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    while ((c = fgetc(fp)) != EOF) {
        checkArithmetic(fp, c);
        checkRelational(fp, c);
        checkLogical(fp, c);
    }

    fclose(fp);
    return 0;
}
