#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define SIZE 5000

int main() {

    char filename[200];
    char text[SIZE];

    printf("Enter file name: ");
    fflush(stdout);
    scanf("%s", filename);

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("File cannot be opened\n");
        return 1;
    }

    int n = fread(text, 1, SIZE - 1, fp);
    text[n] = '\0';
    fclose(fp);

    int pipe1[2];
    int pipe2[2];
    int pipe3[2];

    pipe(pipe1);
    pipe(pipe2);
    pipe(pipe3);

    pid_t p2, p3;

    p2 = fork();

    if(p2 == 0) {

        // ---------------- PROCESS 2 (Text Formatter) ----------------

        char text[SIZE];
        char formatted[SIZE];

        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe3[0]);
        close(pipe3[1]);

        int n = read(pipe1[0], text, SIZE - 1);
        text[n] = '\0';

        int j = 0;
        int space = 0;

        for(int i = 0; text[i] != '\0'; i++) {

            if(isspace(text[i])) {
                space = 1;
            }
            else if(ispunct(text[i])) {

                // No blank space before punctuation marks
                if(j > 0 && formatted[j - 1] == ' ') {
                    j--;
                }

                formatted[j++] = text[i];

                // Exactly one space after punctuation mark
                if(text[i + 1] != '\0' && !isspace(text[i + 1])) {
                    formatted[j++] = ' ';
                }

                space = 0;
            }
            else {

                // Only one blank space between two words
                if(space && j > 0 && formatted[j - 1] != ' ') {
                    formatted[j++] = ' ';
                }

                formatted[j++] = text[i];
                space = 0;
            }
        }

        formatted[j] = '\0';

        write(pipe2[1], formatted, strlen(formatted) + 1);

        close(pipe1[0]);
        close(pipe2[1]);

        exit(0);
    }

    p3 = fork();

    if(p3 == 0) {

        // ---------------- PROCESS 3 (Case Converter & Word Counter) ----------------

        char text[SIZE];

        close(pipe1[0]);
        close(pipe1[1]);
        close(pipe2[1]);
        close(pipe3[0]);

        int n = read(pipe2[0], text, SIZE - 1);
        text[n] = '\0';

        int word = 1;
        int count = 0;

        for(int i = 0; text[i] != '\0'; i++) {

            if(isalpha(text[i])) {

                if(word) {
                    text[i] = toupper(text[i]);
                    count++;
                    word = 0;
                }
                else {
                    text[i] = tolower(text[i]);
                }
            }
            else {
                word = 1;
            }
        }

        printf("\nFinal Text:\n%s\n", text);
        fflush(stdout);

        write(pipe3[1], &count, sizeof(count));

        close(pipe2[0]);
        close(pipe3[1]);

        exit(0);
    }

    // ---------------- PROCESS 1 (File Reader) ----------------

    close(pipe1[0]);
    close(pipe2[0]);
    close(pipe2[1]);
    close(pipe3[1]);

    write(pipe1[1], text, strlen(text) + 1);
    close(pipe1[1]);

    int count;
    read(pipe3[0], &count, sizeof(count));
    close(pipe3[0]);

    printf("\nTotal words = %d\n", count);

    wait(NULL);
    wait(NULL);

    return 0;
}
