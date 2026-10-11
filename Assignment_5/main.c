#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define SIZE 2048

int main() {
    char filename[100], text[SIZE];
    printf("Enter file name: ");
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("File cannot be opened\n");
        return 1;
    }
    int n = fread(text, 1, SIZE - 1, fp);
    text[n] = '\0';
    fclose(fp);

    int p1[2], p2[2], p3[2];
    pipe(p1); pipe(p2); pipe(p3);

    // Process 2: Text Formatter
    if (fork() == 0) {
        char in[SIZE], out[SIZE];
        close(p1[1]); close(p2[0]); close(p3[0]); close(p3[1]);

        int len = read(p1[0], in, SIZE - 1);
        in[len] = '\0';

        int j = 0, space = 0;
        for (int i = 0; in[i]; i++) {
            if (isspace(in[i])) {
                space = 1;
            } else if (ispunct(in[i])) {
                if (j > 0 && out[j - 1] == ' ') j--; // No space before punctuation
                out[j++] = in[i];
                if (in[i + 1] && !isspace(in[i + 1])) out[j++] = ' '; // One space after punctuation
                space = 0;
            } else {
                if (space && j > 0 && out[j - 1] != ' ') out[j++] = ' ';
                out[j++] = in[i];
                space = 0;
            }
        }
        out[j] = '\0';

        write(p2[1], out, strlen(out) + 1);
        close(p1[0]); close(p2[1]);
        exit(0);
    }

    // Process 3: Case Converter & Word Counter
    if (fork() == 0) {
        char in[SIZE];
        close(p1[0]); close(p1[1]); close(p2[1]); close(p3[0]);

        int len = read(p2[0], in, SIZE - 1);
        in[len] = '\0';

        int word = 1, count = 0;
        for (int i = 0; in[i]; i++) {
            if (isalpha(in[i])) {
                if (word) {
                    in[i] = toupper(in[i]);
                    count++;
                    word = 0;
                } else {
                    in[i] = tolower(in[i]);
                }
            } else {
                word = 1;
            }
        }

        printf("\nFinal Text:\n%s\n", in);
        write(p3[1], &count, sizeof(count));
        close(p2[0]); close(p3[1]);
        exit(0);
    }

    // Process 1: File Reader & Word Count Display
    close(p1[0]); close(p2[0]); close(p2[1]); close(p3[1]);

    write(p1[1], text, strlen(text) + 1);
    close(p1[1]);

    int count;
    read(p3[0], &count, sizeof(count));
    close(p3[0]);

    printf("\nTotal words = %d\n", count);

    wait(NULL);
    wait(NULL);
    return 0;
}
