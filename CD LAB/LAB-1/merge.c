#include <stdio.h>
#include <stdlib.h>

#define MAX 1024   

int main() {
    FILE *file1, *file2, *result;
    char file1Name[100], file2Name[100], resultName[100];
    char line[MAX];
    int turn = 1;  

    printf("Enter first filename: ");
    scanf("%s", file1Name);
    printf("Enter second filename: ");
    scanf("%s", file2Name);
    printf("Enter resultant filename: ");
    scanf("%s", resultName);

    file1 = fopen(file1Name, "r");
    if (file1 == NULL) {
        printf("Error: Could not open %s\n", file1Name);
        return 1;
    }

    file2 = fopen(file2Name, "r");
    if (file2 == NULL) {
        printf("Error: Could not open %s\n", file2Name);
        fclose(file1);
        return 1;
    }

    result = fopen(resultName, "w");
    if (result == NULL) {
        printf("Error: Could not create %s\n", resultName);
        fclose(file1);
        fclose(file2);
        return 1;
    }

    
    while (1) {
        if (turn == 1) {
            if (fgets(line, MAX, file1) != NULL) {
                fputs(line, result);
            }
            turn = 2;
        } else {
            if (fgets(line, MAX, file2) != NULL) {
                fputs(line, result);
            }
            turn = 1;
        }

        
        if (feof(file1) && feof(file2)) {
            break;
        }
    }

    printf("Lines merged alternatively into %s\n", resultName);

    
    fclose(file1);
    fclose(file2);
    fclose(result);

    return 0;
}
