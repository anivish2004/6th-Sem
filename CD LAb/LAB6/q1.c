#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    const char *input;
    int pos;
} Parser;

void error(const char *msg, int pos) {
    fprintf(stderr, "Error at position %d: %s\n", pos, msg);
    exit(1);
}

char peek(Parser *p) {
    if (p->pos < (int)strlen(p->input)) {
        return p->input[p->pos];
    }
    return '\0'; // End of input
}

char consume(Parser *p, char expected) {
    char current = peek(p);
    if (current == '\0') {
        error("Unexpected end of input", p->pos);
    }
    if (expected != '\0' && current != expected) {
        char msg[64];
        snprintf(msg, sizeof(msg), "Expected '%c', found '%c'", expected, current);
        error(msg, p->pos);
    }
    p->pos++;
    return current;
}

// Forward declarations
void parseS(Parser *p);
void parseT(Parser *p);

void parseS(Parser *p) {
    char current = peek(p);
    if (current == 'a') {
        consume(p, 'a');
        printf("S -> a\n");
    } else if (current == '>') {
        consume(p, '>');
        printf("S -> >\n");
    } else if (current == '(') {
        consume(p, '(');
        printf("S -> ( T )\n");
        parseT(p);
        consume(p, ')');
    } else {
        error("Unexpected symbol in S", p->pos);
    }
}

void parseT(Parser *p) {
    printf("T -> ...\n");
    parseS(p);
    while (peek(p) == ',') {
        consume(p, ',');
        printf("T -> T , S\n");
        parseS(p);
    }
}

void parse(Parser *p) {
    parseS(p);
    if (p->pos != (int)strlen(p->input)) {
        error("Extra input remaining", p->pos);
    }
    printf("Parsing successful!\n");
}

int main() {
    char buffer[256];
    while (1) {
        printf("Enter a string to parse (or 'quit' to exit): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
        if (strcmp(buffer, "quit") == 0) break;

        Parser p = { buffer, 0 };
        parse(&p);
    }
    return 0;
}
