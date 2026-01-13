#include <stdio.h>
#include <stdlib.h>

int main() {
 FILE *fp;
 char ch;
 int lines = 0, characters = 0;

 fp = fopen("source.txt", "r");

 if (fp == NULL) {
 printf("File cannot be opened.\n");
 exit(1);
 }

 while ((ch = fgetc(fp)) != EOF) {
 characters++;
 if (ch == '\n') {
 lines++;
 }
 }

 if (characters > 0) {
 lines++; // count last line if file does not end with newline
 }

 printf("Number of lines = %d\n", lines);
 printf("Number of characters = %d\n", characters);

 fclose(fp);
 return 0;
}
