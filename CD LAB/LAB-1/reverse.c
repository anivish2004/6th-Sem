#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *src, *dest;
    char srcFilename[100], destFilename[100];
    long size;
    char ch;

    printf("Enter source filename: ");
    scanf("%s", srcFilename);
    printf("Enter destination filename: ");
    scanf("%s", destFilename);

    src = fopen(srcFilename, "r");
    if (src == NULL) {
        printf("Error: Could not open source file %s\n", srcFilename);
        return 1;
    }

    dest = fopen(destFilename, "w");
    if (dest == NULL) {
        printf("Error: Could not open destination file %s\n", destFilename);
        fclose(src);
        return 1;
    }// Write to destination file

    fseek(src, 0, SEEK_END);   
    size = ftell(src);         
    printf("Size of file %s: %ld bytes\n", srcFilename, size);

    for (long i = size - 1; i >= 0; i--) {
        fseek(src, i, SEEK_SET);   
        ch = fgetc(src);           
        fputc(ch, dest);           
    }

    printf("File contents reversed and stored in %s\n", destFilename);

    fclose(src);
    fclose(dest);

    return 0;
}
