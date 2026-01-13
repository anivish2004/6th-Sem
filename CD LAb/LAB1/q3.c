#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp1, *fp2, *fp3;
    char line1[256], line2[256];

    fp1 = fopen("reverse.txt", "r");
    fp2 = fopen("source.txt", "r");
    fp3 = fopen("result.txt", "w");

    if (fp1 == NULL || fp2 == NULL || fp3 == NULL) {
        printf("Error opening files.\n");
        exit(1);
    }

    while (fgets(line1, sizeof(line1), fp1) != NULL ||
           fgets(line2, sizeof(line2), fp2) != NULL) {

        if (!feof(fp1))
            fputs(line1, fp3);

        if (!feof(fp2))
            fputs(line2, fp3);
    }

    printf("Files merged alternately successfully.\n");

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}
