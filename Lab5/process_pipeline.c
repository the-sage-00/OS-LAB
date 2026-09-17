#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE 4096

// Function for Process 2: Formats text according to the rules
void format_text(const char *src, char *dest) {
    int i = 0, j = 0;
    int len = strlen(src);

    // Skip leading whitespace
    while (i < len && isspace((unsigned char)src[i])) i++;

    while (i < len) {
        if (isspace((unsigned char)src[i])) {
            // Look ahead to find the next non-space character
            int next = i;
            while (next < len && isspace((unsigned char)src[next])) next++;
            if (next >= len) {
                break; // Trailing whitespace, stop
            }
            // Rule: No blank space before punctuation marks
            if (ispunct((unsigned char)src[next])) {
                i = next;
                continue;
            }
            // Rule: Only one blank space between two words
            if (j > 0 && dest[j - 1] != ' ') {
                dest[j++] = ' ';
            }
            i = next;
        } else if (ispunct((unsigned char)src[i])) {
            // If there is an existing space before punctuation, remove it
            if (j > 0 && dest[j - 1] == ' ') {
                j--;
            }
            dest[j++] = src[i];
            i++;
            // Skip any spaces immediately after punctuation
            while (i < len && isspace((unsigned char)src[i])) i++;
            // Rule: Exactly one blank space after punctuation if another character follows
            if (i < len && !isspace((unsigned char)src[i])) {
                dest[j++] = ' ';
            }
        } else {
            dest[j++] = src[i++];
        }
    }
    // Remove trailing space if any
    while (j > 0 && isspace((unsigned char)dest[j - 1])) j--;
    dest[j] = '\0';
}

// Function for Process 3: Converts case and counts words
int convert_case_and_count(const char *src, char *dest) {
    int i = 0;
    int word_count = 0;
    int in_word = 0;

    while (src[i] != '\0') {
        char c = src[i];
        if (isalnum((unsigned char)c)) {
            if (!in_word) {
                // First character of word -> Uppercase
                dest[i] = (char)toupper((unsigned char)c);
                in_word = 1;
                word_count++;
            } else {
                // Remaining characters of word -> Lowercase
                dest[i] = (char)tolower((unsigned char)c);
            }
        } else {
            // Punctuation marks and spaces are preserved
            dest[i] = c;
            in_word = 0;
        }
        i++;
    }
    dest[i] = '\0';
    return word_count;
}

int main() {
    int pipe1[2]; // Process 1 -> Process 2 (File Content)
    int pipe2[2]; // Process 2 -> Process 3 (Formatted Content)
    int pipe3[2]; // Process 3 -> Process 1 (Word Count Integer)

    printf("==============================================================\n");
    printf("     OPERATING SYSTEM LAB - ASSIGNMENT 5: PIPES & IPC\n");
    printf("==============================================================\n\n");
    fflush(stdout);

    // Create the three pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1 || pipe(pipe3) == -1) {
        perror("Error creating pipes");
        exit(EXIT_FAILURE);
    }

    // Process 1 asks for file name first
    char filename[256];
    printf("[Process 1: File Reader] (PID: %d)\n", getpid());
    printf("Enter input file name / path: ");
    fflush(stdout);

    if (scanf("%255s", filename) != 1) {
        fprintf(stderr, "Error reading filename.\n");
        exit(EXIT_FAILURE);
    }

    // Process 1 opens and reads file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("[Process 1] Error opening file");
        exit(EXIT_FAILURE);
    }

    char file_buffer[BUFFER_SIZE] = {0};
    size_t bytes_read = fread(file_buffer, 1, BUFFER_SIZE - 1, fp);
    file_buffer[bytes_read] = '\0';
    fclose(fp);

    printf("[Process 1] Successfully read %zu bytes from '%s'.\n", bytes_read, filename);
    printf("[Process 1] Raw File Content:\n   \"%s\"\n\n", file_buffer);
    fflush(stdout);

    // Fork Process 2
    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed for Process 2");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        // ============================================================
        // PROCESS 2: TEXT FORMATTER
        // ============================================================
        close(pipe1[1]); // Close unused write end of Pipe 1
        close(pipe2[0]); // Close unused read end of Pipe 2
        close(pipe3[0]); // Close unused read end of Pipe 3
        close(pipe3[1]); // Close unused write end of Pipe 3

        char raw_buffer[BUFFER_SIZE] = {0};
        char formatted_buffer[BUFFER_SIZE] = {0};

        // Read raw text from Process 1 via Pipe 1
        ssize_t r = read(pipe1[0], raw_buffer, BUFFER_SIZE - 1);
        if (r > 0) {
            raw_buffer[r] = '\0';
        }
        close(pipe1[0]); // Done reading

        printf("[Process 2: Text Formatter] (PID: %d)\n", getpid());
        printf("Received text from Process 1 via Pipe 1.\n");
        printf("Applying formatting rules:\n");
        printf("  - Removing multiple/unnecessary spaces\n");
        printf("  - Removing spaces before punctuation marks\n");
        printf("  - Ensuring single space after punctuation marks\n");

        format_text(raw_buffer, formatted_buffer);

        printf("Formatted Text:\n   \"%s\"\n", formatted_buffer);
        printf("Sending formatted text to Process 3 via Pipe 2...\n\n");
        fflush(stdout);

        // Send formatted text to Process 3 via Pipe 2
        if (write(pipe2[1], formatted_buffer, strlen(formatted_buffer) + 1) < 0) {
            perror("Write to Pipe 2 failed");
        }
        close(pipe2[1]); // Send EOF to Process 3

        exit(EXIT_SUCCESS);
    }

    // Fork Process 3
    pid_t pid3 = fork();
    if (pid3 < 0) {
        perror("Fork failed for Process 3");
        exit(EXIT_FAILURE);
    }

    if (pid3 == 0) {
        // ============================================================
        // PROCESS 3: CASE CONVERTER & WORD COUNTER
        // ============================================================
        close(pipe1[0]); // Close unused read end of Pipe 1
        close(pipe1[1]); // Close unused write end of Pipe 1
        close(pipe2[1]); // Close unused write end of Pipe 2
        close(pipe3[0]); // Close unused read end of Pipe 3

        char formatted_buffer[BUFFER_SIZE] = {0};
        char final_buffer[BUFFER_SIZE] = {0};

        // Read formatted text from Process 2 via Pipe 2
        ssize_t r = read(pipe2[0], formatted_buffer, BUFFER_SIZE - 1);
        if (r > 0) {
            formatted_buffer[r] = '\0';
        }
        close(pipe2[0]); // Done reading

        printf("[Process 3: Case Converter & Word Counter] (PID: %d)\n", getpid());
        printf("Received formatted text from Process 2 via Pipe 2.\n");
        printf("Converting first char of each word to uppercase & rest to lowercase...\n");

        int total_words = convert_case_and_count(formatted_buffer, final_buffer);

        printf("\n--------------------------------------------------------------\n");
        printf("[Process 3] FINAL PROCESSED TEXT:\n");
        printf("   \"%s\"\n", final_buffer);
        printf("--------------------------------------------------------------\n");
        printf("[Process 3] Word Count Calculated: %d words.\n", total_words);
        printf("Sending word count to Process 1 via Pipe 3...\n\n");
        fflush(stdout);

        // Send word count integer back to Process 1 via Pipe 3
        if (write(pipe3[1], &total_words, sizeof(total_words)) < 0) {
            perror("Write to Pipe 3 failed");
        }
        close(pipe3[1]); // Done writing to Pipe 3

        exit(EXIT_SUCCESS);
    }

    // ================================================================
    // PROCESS 1: FILE READER & PRINTS WORD COUNT
    // ================================================================
    close(pipe1[0]); // Close unused read end of Pipe 1
    close(pipe2[0]); // Close unused read end of Pipe 2
    close(pipe2[1]); // Close unused write end of Pipe 2
    close(pipe3[1]); // Close unused write end of Pipe 3

    printf("[Process 1] Sending file contents to Process 2 via Pipe 1...\n\n");
    fflush(stdout);

    // Send file contents to Process 2 via Pipe 1
    if (write(pipe1[1], file_buffer, strlen(file_buffer) + 1) < 0) {
        perror("Write to Pipe 1 failed");
    }
    close(pipe1[1]); // Close write end to signal EOF to Process 2

    // Read word count from Process 3 via Pipe 3
    int final_word_count = 0;
    ssize_t r = read(pipe3[0], &final_word_count, sizeof(final_word_count));
    if (r < 0) {
        perror("Read from Pipe 3 failed");
    }
    close(pipe3[0]); // Done reading from Pipe 3

    // Wait for all child processes to complete
    waitpid(pid2, NULL, 0);
    waitpid(pid3, NULL, 0);

    // Process 1 displays final word count
    printf("==============================================================\n");
    printf("[Process 1] SUCCESS: Full Inter-Process Pipeline Complete!\n");
    printf("[Process 1] Total Word Count Received from Process 3: %d\n", final_word_count);
    printf("==============================================================\n");
    fflush(stdout);

    return 0;
}
