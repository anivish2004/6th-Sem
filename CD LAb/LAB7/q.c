#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ------------------ Token Definitions ------------------

typedef enum {
    TOK_MAIN, TOK_INT, TOK_CHAR,
    TOK_ID, TOK_NUM,
    TOK_LPAREN, TOK_RPAREN,
    TOK_LBRACE, TOK_RBRACE,
    TOK_SEMI, TOK_COMMA, TOK_ASSIGN,
    TOK_EOF, TOK_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[64];
    int row;
    int col;
} Token;

// ------------------ Globals ------------------

Token currentToken;
FILE *source;
int currentRow = 1, currentCol = 0;

// ------------------ Lexer ------------------

int nextChar() {
    int c = fgetc(source);
    if (c == '\n') {
        currentRow++;
        currentCol = 0;
    } else {
        currentCol++;
    }
    return c;
}

void ungetChar(int c) {
    if (c == EOF) return;
    ungetc(c, source);
    if (c == '\n') {
        currentRow--;
        // column reset not exact, but okay for demo
    } else {
        currentCol--;
    }
}

Token makeToken(TokenType type, const char *lexeme, int row, int col) {
    Token t;
    t.type = type;
    strncpy(t.lexeme, lexeme, sizeof(t.lexeme));
    t.lexeme[sizeof(t.lexeme)-1] = '\0';
    t.row = row;
    t.col = col;
    return t;
}

Token getNextToken() {
    int c;
    while ((c = nextChar()) != EOF) {
        if (isspace(c)) continue;

        int startRow = currentRow;
        int startCol = currentCol;

        // Identifiers / keywords
        if (isalpha(c)) {
            char buffer[64];
            int i = 0;
            buffer[i++] = c;
            while ((c = nextChar()), isalnum(c)) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            ungetChar(c);

            if (strcmp(buffer, "main") == 0) return makeToken(TOK_MAIN, buffer, startRow, startCol);
            if (strcmp(buffer, "int") == 0) return makeToken(TOK_INT, buffer, startRow, startCol);
            if (strcmp(buffer, "char") == 0) return makeToken(TOK_CHAR, buffer, startRow, startCol);
            return makeToken(TOK_ID, buffer, startRow, startCol);
        }

        // Numbers
        if (isdigit(c)) {
            char buffer[64];
            int i = 0;
            buffer[i++] = c;
            while ((c = nextChar()), isdigit(c)) {
                buffer[i++] = c;
            }
            buffer[i] = '\0';
            ungetChar(c);
            return makeToken(TOK_NUM, buffer, startRow, startCol);
        }

        // Symbols
        switch (c) {
            case '(': return makeToken(TOK_LPAREN, "(", startRow, startCol);
            case ')': return makeToken(TOK_RPAREN, ")", startRow, startCol);
            case '{': return makeToken(TOK_LBRACE, "{", startRow, startCol);
            case '}': return makeToken(TOK_RBRACE, "}", startRow, startCol);
            case ';': return makeToken(TOK_SEMI, ";", startRow, startCol);
            case ',': return makeToken(TOK_COMMA, ",", startRow, startCol);
            case '=': return makeToken(TOK_ASSIGN, "=", startRow, startCol);
            default:  return makeToken(TOK_UNKNOWN, (char[]){c,'\0'}, startRow, startCol);
        }
    }
    return makeToken(TOK_EOF, "EOF", currentRow, currentCol);
}

// ------------------ Parser Helpers ------------------

void advance() {
    currentToken = getNextToken();
}

void error(const char *expected) {
    printf("Error: Expected %s at row %d, column %d (found '%s')\n",
           expected, currentToken.row, currentToken.col, currentToken.lexeme);
    exit(1);
}

void expect(TokenType type, const char *expected) {
    if (currentToken.type == type) {
        advance();
    } else {
        error(expected);
    }
}

// ------------------ Parser Functions ------------------

void parseProgram();
void parseDeclarations();
void parseDataType();
void parseIdentifierList();
void parseAssignStat();

// Program → main () { declarations assign_stat }
void parseProgram() {
    expect(TOK_MAIN, "main");
    expect(TOK_LPAREN, "(");
    expect(TOK_RPAREN, ")");
    expect(TOK_LBRACE, "{");
    parseDeclarations();
    parseAssignStat();
    expect(TOK_RBRACE, "}");
}

// declarations → data-type identifier-list ; declarations | ε
void parseDeclarations() {
    if (currentToken.type == TOK_INT || currentToken.type == TOK_CHAR) {
        parseDataType();
        parseIdentifierList();
        expect(TOK_SEMI, ";");
        parseDeclarations(); // recursion
    }
    // epsilon → do nothing
}

// data-type → int | char
void parseDataType() {
    if (currentToken.type == TOK_INT) {
        advance();
    } else if (currentToken.type == TOK_CHAR) {
        advance();
    } else {
        error("int or char");
    }
}

// identifier-list → id | id , identifier-list
void parseIdentifierList() {
    expect(TOK_ID, "identifier");
    if (currentToken.type == TOK_COMMA) {
        advance();
        parseIdentifierList();
    }
}

// assign_stat → id = id ; | id = num ;
void parseAssignStat() {
    expect(TOK_ID, "identifier");
    expect(TOK_ASSIGN, "=");
    if (currentToken.type == TOK_ID) {
        advance();
    } else if (currentToken.type == TOK_NUM) {
        advance();
    } else {
        error("id or num");
    }
    expect(TOK_SEMI, ";");
}

// ------------------ Main ------------------

int main() {
    source = fopen("input.txt", "r");
    if (!source) {
        printf("Could not open input file.\n");
        return 1;
    }

    advance(); // load first token
    parseProgram();

    if (currentToken.type != TOK_EOF) {
        error("EOF");
    }

    printf("Parsing successful!\n");
    fclose(source);
    return 0;
}
