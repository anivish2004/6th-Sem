#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file;
    char filename[100];
    int ch;
    int lines = 0, characters = 0;

    printf("Enter the filename: ");
    scanf("%s", filename);

    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return 1;
    }

    while ((ch = fgetc(file)) != EOF) {
        characters++;              
        if (ch == '\n') {
            lines++;               
        }
    }

    if (characters > 0) {
        lines++;
    }

    printf("Number of characters: %d\n", characters);
    printf("Number of lines: %d\n", lines);

    fclose(file);

    return 0;
}
