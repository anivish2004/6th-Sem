#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// bring in lexer definitions
extern int row, col;
struct token {
    char lexeme[100];
    char type[20];
    int row, col;
};
extern struct token getNextToken(FILE *f);

struct token tkn;
FILE *f1;

// grammar functions
void program();
void declarations();
void datatype();
void idlist();
void idlistprime();
void assignstat();
void statementlist();
void statement();
void expn();
void eprime();
void simpleexp();
void seprime();
void term();
void tprime();
void factor();
void decisionstat();
void dprime();
void loopingstat();
void relop();
void addop();
void mulop();

// operator arrays
char *rel[] = {"==", "!=", "<=", ">=", ">", "<"};
char *add[] = {"+", "-"};
char *mul[] = {"*", "/", "%"};

int isrel(char *w) { for (int i=0;i<6;i++) if (!strcmp(w,rel[i])) return 1; return 0; }
int isadd(char *w) { for (int i=0;i<2;i++) if (!strcmp(w,add[i])) return 1; return 0; }
int ismul(char *w) { for (int i=0;i<3;i++) if (!strcmp(w,mul[i])) return 1; return 0; }
int isdtype(char *lexeme) { return (!strcmp(lexeme,"int")||!strcmp(lexeme,"char")); }

int main() {
    f1 = fopen("q9output.c", "r");
    if (!f1) { printf("Error! File cannot be opened!\n"); return 0; }

    while ((tkn = getNextToken(f1)).row != -1) {
        if (!strcmp(tkn.lexeme,"main")) { program(); break; }
    }

    printf("Compiled successfully\n");
    fclose(f1);
    return 0;
}

void program() {
    if (!strcmp(tkn.lexeme,"main")) {
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"(")) { printf("( missing\n"); exit(1); }
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,")")) { printf(") missing\n"); exit(1); }
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"{")) { printf("{ missing\n"); exit(1); }

        tkn = getNextToken(f1);
        declarations();
        statementlist();

        if (strcmp(tkn.lexeme,"}")) { printf("} missing\n"); exit(1); }
    }
}

void declarations() {
    if (!isdtype(tkn.lexeme)) return;
    datatype();
    idlist();
    if (!strcmp(tkn.lexeme,";")) {
        tkn = getNextToken(f1);
        declarations();
    } else { printf("; missing\n"); exit(1); }
}

void datatype() {
    if (isdtype(tkn.lexeme)) { tkn = getNextToken(f1); return; }
    printf("Datatype missing\n"); exit(1);
}

void idlist() {
    if (!strcmp(tkn.type,"IDENTIFIER")) {
        tkn = getNextToken(f1);
        idlistprime();
    } else { printf("Missing IDENTIFIER\n"); exit(1); }
}

void idlistprime() {
    if (!strcmp(tkn.lexeme,",")) {
        tkn = getNextToken(f1);
        idlist();
    } else if (!strcmp(tkn.lexeme,"[")) {
        tkn = getNextToken(f1);
        if (!strcmp(tkn.type,"NUMBER")) {
            tkn = getNextToken(f1);
            if (!strcmp(tkn.lexeme,"]")) {
                tkn = getNextToken(f1);
                if (!strcmp(tkn.lexeme,",")) {
                    tkn = getNextToken(f1);
                    idlist();
                }
            } else { printf("] missing\n"); exit(1); }
        }
    }
}

void statementlist() {
    if (strcmp(tkn.type,"IDENTIFIER") && strcmp(tkn.lexeme,"if")
        && strcmp(tkn.lexeme,"while") && strcmp(tkn.lexeme,"for")) return;
    statement();
    statementlist();
}

void statement() {
    if (!strcmp(tkn.type,"IDENTIFIER")) {
        assignstat();
        if (!strcmp(tkn.lexeme,";")) { tkn = getNextToken(f1); return; }
        else { printf("; missing\n"); exit(1); }
    } else if (!strcmp(tkn.lexeme,"if")) {
        decisionstat();
    } else if (!strcmp(tkn.lexeme,"while") || !strcmp(tkn.lexeme,"for")) {
        loopingstat();
    }
}

void assignstat() {
    if (!strcmp(tkn.type,"IDENTIFIER")) {
        tkn = getNextToken(f1);
        if (!strcmp(tkn.lexeme,"=")) {
            tkn = getNextToken(f1);
            expn();
        } else { printf("= missing\n"); exit(1); }
    } else { printf("Missing IDENTIFIER\n"); exit(1); }
}

void expn() { simpleexp(); eprime(); }
void eprime() { if (isrel(tkn.lexeme)) { relop(); simpleexp(); } }
void simpleexp() { term(); seprime(); }
void seprime() { if (isadd(tkn.lexeme)) { addop(); term(); seprime(); } }
void term() { factor(); tprime(); }
void tprime() { if (ismul(tkn.lexeme)) { mulop(); factor(); tprime(); } }

void factor() {
    if (!strcmp(tkn.type,"IDENTIFIER") || !strcmp(tkn.type,"NUMBER")) {
        tkn = getNextToken(f1);
        return;
    }
}

void decisionstat() {
    if (!strcmp(tkn.lexeme,"if")) {
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"(")) { printf("( missing\n"); exit(1); }
        tkn = getNextToken(f1);
        expn();
        if (strcmp(tkn.lexeme,")")) { printf(") missing\n"); exit(1); }
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"{")) { printf("{ missing\n"); exit(1); }
        tkn = getNextToken(f1);
        statementlist();
        if (strcmp(tkn.lexeme,"}")) { printf("} missing\n"); exit(1); }
        tkn = getNextToken(f1);
        dprime();
    }
}

void dprime() {
    if (!strcmp(tkn.lexeme,"else")) {
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"{")) { printf("{ missing\n"); exit(1); }
        tkn = getNextToken(f1);
        statementlist();
        if (strcmp(tkn.lexeme,"}")) { printf("} missing\n"); exit(1); }
        tkn = getNextToken(f1);
    }
}

void loopingstat() {
    if (!strcmp(tkn.lexeme,"while")) {
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"(")) { printf("( missing\n"); exit(1); }
        tkn = getNextToken(f1);
        expn();
        if (strcmp(tkn.lexeme,")")) { printf(") missing\n"); exit(1); }
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"{")) { printf("{ missing\n"); exit(1); }
        tkn = getNextToken(f1);
        statementlist();
        if (strcmp(tkn.lexeme,"}")) { printf("} missing\n"); exit(1); }
        tkn = getNextToken(f1);
    } else if (!strcmp(tkn.lexeme,"for")) {
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"(")) { printf("( missing\n"); exit(1); }
        tkn = getNextToken(f1);
        assignstat();
        if (strcmp(tkn.lexeme,";")) { printf("; missing\n"); exit(1); }
        tkn = getNextToken(f1);
        expn();
        if (strcmp(tkn.lexeme,";")) { printf("; missing\n"); exit(1); }
        tkn = getNextToken(f1);
        assignstat();
        if (strcmp(tkn.lexeme,")")) { printf(") missing\n"); exit(1); }
        tkn = getNextToken(f1);
        if (strcmp(tkn.lexeme,"{")) { printf("{ missing\n"); exit(1); }
        tkn = getNextToken(f1);
        statementlist();
        if (strcmp(tkn.lexeme,"}")) { printf("} missing\n"); exit(1); }
        tkn = getNextToken(f1);
    }
}

void relop() {
    if (isrel(tkn.lexeme)) { tkn = getNextToken(f1); return; }
    printf("Relational operator missing\n"); exit(1);
}
void addop() {
    if (isadd(tkn.lexeme)) {
        tkn = getNextToken(f1);
        return;
    }
    printf("Add operator missing\n");
    exit(1);
}

void mulop() {
    if (ismul(tkn.lexeme)) {
        tkn = getNextToken(f1);
        return;
    }
    printf("Multiplication operator missing\n");
    exit(1);
}
