#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TableLength 30

enum tokenType {
    EOFILE = -1,
    LESS_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN,
    GREATER_THAN_OR_EQUAL,
    EQUAL,
    NOT_EQUAL
};

struct token {
    char *lexeme;
    int index;
    unsigned int rowno, colno;
    enum tokenType type;
};

struct ListElement {
    struct token tok;
    struct ListElement *next;
};

struct ListElement *TABLE[TableLength];

void Initialize() {
    for (int i = 0; i < TableLength; i++) {
        TABLE[i] = NULL;
    }
}

int HASH(char *str) {
    unsigned long hash = 0;
    int p = 31;  
    while (*str) {
        hash = (hash * p + *str) % TableLength;
        str++;
    }
    return (int)hash;
}

int SEARCH(char *str) {
    int val = HASH(str);
    struct ListElement *ele = TABLE[val];
    while (ele != NULL) {
        if (strcmp(ele->tok.lexeme, str) == 0) {
            return 1; 
        }
        ele = ele->next;
    }
    return 0; 
}

void INSERT(struct token tk) {
    if (SEARCH(tk.lexeme) == 1) {
        return; 
    }
    int val = HASH(tk.lexeme);
    struct ListElement *cur = (struct ListElement *)malloc(sizeof(struct ListElement));
    cur->tok = tk;
    cur->next = NULL;
    if (TABLE[val] == NULL) {
        TABLE[val] = cur; 
    } else {
        struct ListElement *ele = TABLE[val];
        while (ele->next != NULL) {
            ele = ele->next; 
        }
        ele->next = cur;
    }
}

void Display() {
    for (int i = 0; i < TableLength; i++) {
        struct ListElement *ele = TABLE[i];
        if (ele != NULL) {
            printf("Bucket[%d]: ", i);
            while (ele != NULL) {
                printf("Lexeme: %s | Index: %d | Row: %u | Col: %u | Type: %d -> ",
                       ele->tok.lexeme, ele->tok.index, ele->tok.rowno, ele->tok.colno, ele->tok.type);
                ele = ele->next;
            }
            printf("NULL\n");
        }
    }
}

void processFile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error opening file %s\n", filename);
        return;
    }

    char ch;
    int row = 1, col = 1, index = 1;
    while ((ch = fgetc(fp)) != EOF) {
        struct token t;
        t.lexeme = NULL;
        t.index = index;
        t.rowno = row;
        t.colno = col;
        t.type = EOFILE;

        if (ch == '<') {
            char next = fgetc(fp);
            if (next == '=') {
                t.lexeme = strdup("<=");
                t.type = LESS_THAN_OR_EQUAL;
                col++;
            } else {
                ungetc(next, fp);
                t.lexeme = strdup("<");
                t.type = LESS_THAN;
            }
        } else if (ch == '>') {
            char next = fgetc(fp);
            if (next == '=') {
                t.lexeme = strdup(">=");
                t.type = GREATER_THAN_OR_EQUAL;
                col++;
            } else {
                ungetc(next, fp);
                t.lexeme = strdup(">");
                t.type = GREATER_THAN;
            }
        } else if (ch == '=') {
            char next = fgetc(fp);
            if (next == '=') {
                t.lexeme = strdup("==");
                t.type = EQUAL;
                col++;
            }
        } else if (ch == '!') {
            char next = fgetc(fp);
            if (next == '=') {
                t.lexeme = strdup("!=");
                t.type = NOT_EQUAL;
                col++;
            }
        }

        if (t.lexeme != NULL) {
            INSERT(t);
            index++;
        }

        if (ch == '\n') {
            row++;
            col = 1;
        } else {
            col++;
        }
    }

    fclose(fp);
}

int main() {
    Initialize();

    processFile("example.c");   
    Display();

    return 0;
}
