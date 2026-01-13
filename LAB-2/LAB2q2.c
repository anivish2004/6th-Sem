#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *src, *dest;
    char line[512];

    src = fopen("q1.c", "r");
    dest = fopen("output2.c", "w");

    if (src == NULL || dest == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while (fgets(line, sizeof(line), src) != NULL) {

        /* If line does NOT start with #, copy it */
        if (line[0] != '#') {
            fputs(line, dest);
        }
    }

    fclose(src);
    fclose(dest);

    printf("Preprocessor directives removed successfully.\n");
    return 0;
}
