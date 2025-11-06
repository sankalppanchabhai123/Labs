#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define SIZE 10

// Function to sort array using bubble sort
void sort_array(int arr[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (arr[j] > arr[j + 1])
            {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Function to display array
void display_array(int arr[], int n, char *process_name)
{
    printf("%s: Array elements: ", process_name);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int arr[SIZE];
    pid_t pid;

    printf("=== Parent Sorts, Child Displays ===\n");

    // Initialize array with random values
    printf("Original array: ");
    for (int i = 0; i < SIZE; i++)
    {
        arr[i] = rand() % 100; // Random numbers between 0-99
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("Fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());

        // Wait a bit to ensure parent sorts first
        sleep(2);

        // Display the sorted array
        display_array(arr, SIZE, "Child");

        printf("Child: Exiting...\n");
        exit(0);
    }
    else
    {
        // Parent process
        printf("Parent Process: PID = %d, Child PID = %d\n", getpid(), pid);

        // Sort the array
        printf("Parent: Sorting array...\n");
        sort_array(arr, SIZE);

        // Display sorted array in parent
        display_array(arr, SIZE, "Parent");

        // Wait for child to complete
        printf("Parent: Waiting for child to finish...\n");
        wait(NULL);

        printf("Parent: Child process completed.\n");
        printf("Parent: Exiting...\n");
    }

    return 0;
}