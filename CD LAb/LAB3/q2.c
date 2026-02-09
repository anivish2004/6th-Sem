#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char *keywords[] = {
    "int","float","char","double","if","else","while","for","do",
    "return","switch","case","break","continue","void","static",
    "struct","typedef","union","long","short","signed","unsigned",
    "const","goto","default","sizeof"
};
int keywordCount = sizeof(keywords)/sizeof(keywords[0]);

int isKeyword(const char *word) {
    for (int i = 0; i < keywordCount; i++) {
        if (strcmp(word, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int isSpecialSymbol(char c) {
    char symbols[] = {';',',','(',')','{','}','[',']','#'};
    for (int i = 0; i < sizeof(symbols); i++) {
        if (c == symbols[i]) return 1;
    }
    return 0;
}

int isNumber(const char *word) {
    int i = 0;
    if (!isdigit(word[i]) && word[i] != '.') return 0;
    for (i = 1; word[i] != '\0'; i++) {
        if (!isdigit(word[i]) && word[i] != '.') return 0;
    }
    return 1;
}

int isStringLiteral(const char *word) { 
    int len = strlen(word);
    return (len >= 2 && word[0] == '"' && word[len-1] == '"');
}

int isIdentifier(const char *word) {
    if (!(isalpha(word[0]) || word[0] == '_')) return 0;
    for (int i = 1; word[i] != '\0'; i++) {
        if (!(isalnum(word[i]) || word[i] == '_')) return 0;
    }
    return 1;
}

int main() {
    FILE *fp = fopen("q2.c", "r");
    if (fp == NULL) {
        printf("Cannot open file\n");
        exit(0);
    }

    char word[100];
    while (fscanf(fp, "%s", word) != EOF) {
        if (isKeyword(word)) {
            printf("\n Keyword: %s", word);
        } else if (isNumber(word)) {
            printf("\n Numerical constant: %s", word);
        } else if (isStringLiteral(word)) {
            printf("\n String literal: %s", word);
        } else if (isIdentifier(word)) {
            printf("\n Identifier: %s", word);
        } else {
            for (int i = 0; word[i] != '\0'; i++) {
                if (isSpecialSymbol(word[i])) {
                    printf("\n Special symbol: %c", word[i]);
                }
            }
        }
    }

    fclose(fp);
    return 0;
}
