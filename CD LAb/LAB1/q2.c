#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *source, *dest;
    long size;
    char ch;

    source = fopen("source.txt", "r");
    if (source == NULL) {
        printf("Cannot open source file.\n");
        exit(1);
    }

    dest = fopen("reverse.txt", "w");
    if (dest == NULL) {
        printf("Cannot open destination file.\n");
        fclose(source);
        exit(1);
    }

    /* Move file pointer to end to get size */
    fseek(source, 0, SEEK_END);
    size = ftell(source);

    printf("Size of file = %ld bytes\n", size);

    /* Read file in reverse order */
    for (long i = size - 1; i >= 0; i--) {
        fseek(source, i, SEEK_SET);
        ch = fgetc(source);
        fputc(ch, dest);
    }

    printf("File reversed successfully.\n");

    fclose(source);
    fclose(dest);

    return 0;
}
