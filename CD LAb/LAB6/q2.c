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
void parseU(Parser *p);
void parseV(Parser *p);
void parseW(Parser *p);

void parseS(Parser *p) {
    printf("S -> U V W\n");
    parseU(p);
    parseV(p);
    parseW(p);
}

void parseU(Parser *p) {
    char current = peek(p);
    if (current == '(') {
        consume(p, '(');
        printf("U -> ( S )\n");
        parseS(p);
        consume(p, ')');
    } else if (current == 'a') {
        consume(p, 'a');
        printf("U -> a S b\n");
        parseS(p);   // parse the inner S
        consume(p, 'b'); // consume the closing b for outer U
    } else if (current == 'd') {
        consume(p, 'd');
        printf("U -> d\n");
    } else {
        error("Unexpected symbol in U", p->pos);
    }
}

void parseV(Parser *p) {
    char current = peek(p);
    if (current == 'a') {
        consume(p, 'a');
        printf("V -> a V\n");
        parseV(p);
    } else {
        printf("V -> ε\n");
    }
}

void parseW(Parser *p) {
    char current = peek(p);
    if (current == 'c') {
        consume(p, 'c');
        printf("W -> c W\n");
        parseW(p);
    } else {
        printf("W -> ε\n");
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
