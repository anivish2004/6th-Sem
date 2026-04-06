#include <stdio.h>
#include <ctype.h>
#include <string.h>

struct token {
    char lexeme[100];
    char type[20];
    int row, col;
};

int row = 1, col = 0;   // defined here

struct token getNextToken(FILE *f) {
    struct token t;
    int c;
    int i = 0;

    c = fgetc(f);
    col++;

    // Skip whitespace
    while (isspace(c)) {
        if (c == '\n') { row++; col = 0; }
        c = fgetc(f);
        col++;
    }

    // End of file
    if (c == EOF) {
        t.row = -1;
        return t;
    }

    // Identifiers or keywords
    if (isalpha(c)) {
        while (isalnum(c)) {
            t.lexeme[i++] = c;
            c = fgetc(f);
            col++;
        }
        t.lexeme[i] = '\0';
        ungetc(c, f);

        if (strcmp(t.lexeme, "int") == 0 || strcmp(t.lexeme, "char") == 0)
            strcpy(t.type, "DATATYPE");
        else if (strcmp(t.lexeme, "main") == 0)
            strcpy(t.type, "MAIN");
        else if (strcmp(t.lexeme, "if") == 0 || strcmp(t.lexeme, "else") == 0)
            strcpy(t.type, "KEYWORD");
        else if (strcmp(t.lexeme, "while") == 0 || strcmp(t.lexeme, "for") == 0)
            strcpy(t.type, "KEYWORD");
        else
            strcpy(t.type, "IDENTIFIER");

        t.row = row; t.col = col;
        return t;
    }

    // Numbers
    if (isdigit(c)) {
        while (isdigit(c)) {
            t.lexeme[i++] = c;
            c = fgetc(f);
            col++;
        }
        t.lexeme[i] = '\0';
        ungetc(c, f);
        strcpy(t.type, "NUMBER");
        t.row = row; t.col = col;
        return t;
    }

    // Operators and punctuation
    t.lexeme[0] = c;
    t.lexeme[1] = '\0';
    t.row = row; t.col = col;

    switch (c) {
        case '+': case '-': strcpy(t.type, "ADDOP"); break;
        case '*': case '/': case '%': strcpy(t.type, "MULOP"); break;
        case '=':
            c = fgetc(f);
            if (c == '=') { strcpy(t.lexeme, "=="); strcpy(t.type, "RELOP"); col++; }
            else { ungetc(c, f); strcpy(t.type, "ASSIGN"); }
            break;
        case '!':
            c = fgetc(f);
            if (c == '=') { strcpy(t.lexeme, "!="); strcpy(t.type, "RELOP"); col++; }
            else { ungetc(c, f); strcpy(t.type, "UNKNOWN"); }
            break;
        case '<':
            c = fgetc(f);
            if (c == '=') { strcpy(t.lexeme, "<="); strcpy(t.type, "RELOP"); col++; }
            else { ungetc(c, f); strcpy(t.type, "RELOP"); }
            break;
        case '>':
            c = fgetc(f);
            if (c == '=') { strcpy(t.lexeme, ">="); strcpy(t.type, "RELOP"); col++; }
            else { ungetc(c, f); strcpy(t.type, "RELOP"); }
            break;
        case ';': case ',': case '(': case ')':
        case '{': case '}': case '[': case ']':
            strcpy(t.type, "DELIM"); break;
        default:
            strcpy(t.type, "UNKNOWN"); break;
    }

    return t;
}
