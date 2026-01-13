#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp1, *fp2;
    char ch;
    int space_flag = 0;

    fp1 = fopen("q1.c", "r");
    fp2 = fopen("output1.c", "w");

    if (fp1 == NULL || fp2 == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    while ((ch = fgetc(fp1)) != EOF) {

        /* If space or tab is found */
        if (ch == ' ' || ch == '\t') {
            if (space_flag == 0) {
                fputc(' ', fp2);   // write single space
                space_flag = 1;
            }
        }

        /* Reset flag when non-space character occurs */
        else {
            fputc(ch, fp2);
            if (ch != '\n')
                space_flag = 0;
        }
    }

    fclose(fp1);
    fclose(fp2);

    printf("Spaces and tabs replaced successfully.\n");
    return 0;
}
