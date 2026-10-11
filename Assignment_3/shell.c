#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_INPUT 100
#define MAX_ARGS 20

int main()
{

    char input[MAX_INPUT];

    while (1)
    {

        // 1. Print shell prompt
        printf("MyShell>>> ");
        fflush(stdout);

        // 2. Read command from user
        if (!fgets(input, MAX_INPUT, stdin))
            break;

        // Remove newline character
        input[strcspn(input, "\n")] = '\0';

        // 8. Exit the shell
        if (strcmp(input, "exit") == 0)
        {
            printf("Shell terminated.\n");
            break;
        }

        // If user presses Enter without a command
        if (strlen(input) == 0)
        {
            continue;
        }

        // 3. Parse command and arguments
        char *args[MAX_ARGS];
        int count = 0;

        char *token = strtok(input, " ");

        while (token != NULL)
        {
            args[count] = token;
            count++;

            token = strtok(NULL, " ");
        }

        // execvp() requires NULL at the end
        args[count] = NULL;

        // 4. Create child process
        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork failed");
            continue;
        }

        // 5. Child process
        if (pid == 0)
        {

            execvp(args[0], args);

            // execvp() reaches here only if execution fails
            perror("exec failed");
            exit(1);
        }

        // 6. Parent process
        else
        {

            wait(NULL);
        }

        // 7. Loop automatically goes back to the prompt
    }

    return 0;
}