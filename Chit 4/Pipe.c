#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

int count_vowels(char *str)
{
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++)
    {
        char ch = str[i];
        if (ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u' ||
            ch == 'A' || ch == 'E' || ch == 'I' || ch == 'O' || ch == 'U')
        {
            count++;
        }
    }
    return count;
}

int main()
{
    int pipefd[2]; // pipefd[0] - read end, pipefd[1] - write end
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Create pipe
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    // Create child process
    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        exit(1);
    }

    if (pid > 0)
    {
        // Parent process
        close(pipefd[0]); // Close read end in parent

        char message[] = "Hello World, This is Operating Systems Lab";

        printf("Parent: Sending string to child: \"%s\"\n", message);

        // Write string to pipe
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]); // Close write end

        printf("Parent: Waiting for child to finish...\n");
        wait(NULL); // Wait for child to finish
        printf("Parent: Child process completed.\n");
    }
    else
    {
        // Child process
        close(pipefd[1]); // Close write end in child

        // Read string from pipe
        read(pipefd[0], buffer, BUFFER_SIZE);
        close(pipefd[0]); // Close read end

        printf("Child: Received string: \"%s\"\n", buffer);

        // Count vowels
        int vowel_count = count_vowels(buffer);
        printf("Child: Number of vowels in the string: %d\n", vowel_count);

        exit(0);
    }

    return 0;
}