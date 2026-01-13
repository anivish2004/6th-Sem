#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* List of C keywords */
const char *keywords[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};

#define KEYWORD_COUNT 32

/* Function to check if a word is a keyword */
int isKeyword(char *word) {
    for (int i = 0; i < KEYWORD_COUNT; i++) {
        if (strcmp(word, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

/* Function to convert string to uppercase */
void toUpper(char *word) {
    for (int i = 0; word[i]; i++)
        word[i] = toupper(word[i]);
}

int main() {
    FILE *fp;
    char ch, word[50];
    int index = 0;

    fp = fopen("q1.c", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {

        /* If character is alphabetic, form a word */
        if (isalpha(ch)) {
            word[index++] = ch;
        } 
        else {
            if (index > 0) {
                word[index] = '\0';

                if (isKeyword(word)) {
                    toUpper(word);
                    printf("%s\n", word);
                }
                index = 0;
            }
        }
    }

    fclose(fp);
    return 0;
}
