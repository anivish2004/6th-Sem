#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *src, *dest;
    char ch, next;

    src = fopen("q1.c", "r");
    dest = fopen("output.c", "w");

    if (src == NULL || dest == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while ((ch = fgetc(src)) != EOF) {

        if (ch == '/') {
            next = fgetc(src);

            /* Single-line comment */
            if (next == '/') {
                while ((ch = fgetc(src)) != '\n' && ch != EOF);
                fputc('\n', dest);
            }

            /* Multi-line comment */
            else if (next == '*') {
                while (1) {
                    ch = fgetc(src);
                    next = fgetc(src);
                    if (ch == '*' && next == '/')
                        break;
                    fseek(src, -1, SEEK_CUR);
                }
            }

            /* Not a comment */
            else {
                fputc(ch, dest);
                fputc(next, dest);
            }
        }

        else {
            fputc(ch, dest);
        }
    }

    fclose(src);
    fclose(dest);

    printf("Comments removed successfully.\n");
    return 0;
}
