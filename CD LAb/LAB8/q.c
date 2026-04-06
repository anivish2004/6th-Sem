#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char input[200];
int pos = 0;

char nextChar() {
    return input[pos];
}

bool consume(char c) {
    if (nextChar() == c) {
        pos++;
        return true;
    }
    return false;
}

bool matchID() {
    if (isalpha(nextChar())) {
        while (isalnum(nextChar())) pos++;
        return true;
    }
    return false;
}

bool matchNUM() {
    if (isdigit(nextChar())) {
        while (isdigit(nextChar())) pos++;
        return true;
    }
    return false;
}

// Forward declarations
bool Program();
bool Declarations();
bool IdentifierList();
bool StatementList();
bool Statement();
bool AssignStat();
bool Expn();
bool Eprime();
bool SimpleExpn();
bool Seprime();
bool Term();
bool Tprime();
bool Factor();
bool Relop();
bool Addop();
bool Mulop();

bool Program() {
    return (matchID() && consume('(') && consume(')') &&
            consume('{') && Declarations() && StatementList() && consume('}'));
}

bool Declarations() {
    return IdentifierList();
}

bool IdentifierList() {
    if (!matchID()) return false;
    if (consume('[')) {
        if (!matchNUM() || !consume(']')) return false;
    }
    if (consume(',')) return IdentifierList();
    return true;
}

bool StatementList() {
    if (Statement()) return StatementList();
    return true; // ε
}

bool Statement() {
    return AssignStat() && consume(';');
}

bool AssignStat() {
    int save = pos;
    if (matchID() && consume('=')) {
        if (Expn()) return true;
    }
    pos = save;
    return false;
}

bool Expn() {
    return SimpleExpn() && Eprime();
}

bool Eprime() {
    int save = pos;
    if (Relop() && SimpleExpn()) return true;
    pos = save;
    return true; // ε
}

bool SimpleExpn() {
    return Term() && Seprime();
}

bool Seprime() {
    int save = pos;
    if (Addop() && Term() && Seprime()) return true;
    pos = save;
    return true; // ε
}

bool Term() {
    return Factor() && Tprime();
}

bool Tprime() {
    int save = pos;
    if (Mulop() && Factor() && Tprime()) return true;
    pos = save;
    return true; // ε
}

bool Factor() {
    int save = pos;
    if (matchID()) return true;
    pos = save;
    if (matchNUM()) return true;
    pos = save;
    return false;
}

bool Relop() {
    char c = nextChar();
    if (c == '=' || c == '>' || c == '<') { pos++; return true; }
    if (c == '!') { pos++; if (consume('=')) return true; }
    if (c == '<' || c == '>') {
        pos++;
        if (consume('=')) return true;
    }
    return false;
}

bool Addop() {
    if (nextChar() == '+' || nextChar() == '-') { pos++; return true; }
    return false;
}

bool Mulop() {
    if (nextChar() == '*' || nextChar() == '/' || nextChar() == '%') { pos++; return true; }
    return false;
}

int main() {
    printf("Enter input string: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';

    pos = 0;
    if (Program() && pos == strlen(input)) {
        printf("Parsing Successful\n");
    } else {
        printf("Parsing Unsuccessful at position %d\n", pos);
    }
    return 0;
}

