#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TableLength 30

// Token types
enum tokenType {
    EOFILE = -1,
    IDENTIFIER,
    KEYWORD,
    NUMBER,
    OPERATOR
};

// Token structure
struct token {
    char *lexeme;
    int index;
    unsigned int rowno, colno;
    enum tokenType type;
};

// Linked list element
struct ListElement {
    struct token tok;
    struct ListElement *next;
};

// Two symbol tables
struct ListElement *GLOBAL_TABLE[TableLength];
struct ListElement *LOCAL_TABLE[TableLength];

// Initialize table
void Initialize(struct ListElement *TABLE[]) {
    for (int i = 0; i < TableLength; i++) {
        TABLE[i] = NULL;
    }
}

// Hash function
int HASH(char *str) {
    unsigned long hash = 0;
    int p = 31;
    while (*str) {
        hash = (hash * p + *str) % TableLength;
        str++;
    }
    return (int)hash;
}

// Search
int SEARCH(struct ListElement *TABLE[], char *str) {
    int val = HASH(str);
    struct ListElement *ele = TABLE[val];
    while (ele != NULL) {
        if (strcmp(ele->tok.lexeme, str) == 0) return 1;
        ele = ele->next;
    }
    return 0;
}

// Insert
void INSERT(struct ListElement *TABLE[], struct token tk) {
    if (SEARCH(TABLE, tk.lexeme)) return;
    int val = HASH(tk.lexeme);
    struct ListElement *cur = (struct ListElement *)malloc(sizeof(struct ListElement));
    cur->tok = tk;
    cur->next = NULL;
    if (TABLE[val] == NULL) {
        TABLE[val] = cur;
    } else {
        struct ListElement *ele = TABLE[val];
        while (ele->next != NULL) ele = ele->next;
        ele->next = cur;
    }
}

// Display
void Display(struct ListElement *TABLE[], const char *name) {
    printf("\n--- %s Symbol Table ---\n", name);
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

/* ---------------------------------------------------
   Stub for getNextToken() — replace with your Lab 3 version
   --------------------------------------------------- */
struct token getNextToken(FILE *fp) {
    static int row = 1, col = 1;
    char word[64];
    if (fscanf(fp, "%63s", word) == EOF) {
        struct token t = {NULL, 0, row, col, EOFILE};
        return t;
    }

    struct token t;
    t.lexeme = strdup(word);
    t.rowno = row;
    t.colno = col;
    t.type = IDENTIFIER; // crude classification
    return t;
}

/* ---------------------------------------------------
   Lexical Analyser: builds GST and LST
   --------------------------------------------------- */
void LexicalAnalyser(FILE *fp) {
    struct token tk;
    int index = 1;
    int inFunction = 0; // flag for local scope

    while (1) {
        tk = getNextToken(fp);
        if (tk.type == EOFILE) break;

        tk.index = index++;

        if (tk.type == IDENTIFIER) {
            if (inFunction) {
                INSERT(LOCAL_TABLE, tk);
            } else {
                INSERT(GLOBAL_TABLE, tk);
            }
        }

        // crude detection: entering function scope when "main" is seen
        if (tk.type == IDENTIFIER && strcmp(tk.lexeme, "main") == 0) {
            inFunction = 1;
        }
    }
}

/* ---------------------------------------------------
   Main
   --------------------------------------------------- */
int main() {
    FILE *fp = fopen("example.c", "r");
    if (!fp) {
        printf("Error opening file\n");
        return 1;
    }

    Initialize(GLOBAL_TABLE);
    Initialize(LOCAL_TABLE);

    LexicalAnalyser(fp);

    Display(GLOBAL_TABLE, "Global");
    Display(LOCAL_TABLE, "Local");

    fclose(fp);
    return 0;
}
