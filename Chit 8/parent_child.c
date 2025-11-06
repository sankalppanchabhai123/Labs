#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_SIZE 20

// Function to display array
void display_array(int arr[], int n, char *process_name)
{
    printf("%s: ", process_name);
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

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

int main()
{
    int arr[MAX_SIZE];
    int n;
    pid_t pid;

    printf("=== PARENT ACCEPTS ARRAY, CHILD SORTS IT ===\n");

    // Parent accepts array from user
    printf("Parent: Enter number of elements (max %d): ", MAX_SIZE);
    scanf("%d", &n);

    if (n <= 0 || n > MAX_SIZE)
    {
        printf("Invalid array size!\n");
        return 1;
    }

    printf("Parent: Enter %d integers: ", n);
    for (int i = 0; i < n; i++)
    {
        scanf("%d", &arr[i]);
    }

    // Display original array
    display_array(arr, n, "Parent - Original array");

    printf("\nParent: Creating child process using fork()...\n");

    // Create child process
    pid = fork();

    if (pid < 0)
    {
        // Fork failed
        perror("fork failed");
        exit(1);
    }
    else if (pid == 0)
    {
        // Child process
        printf("\n--- CHILD PROCESS ---\n");
        printf("Child: My PID = %d\n", getpid());
        printf("Child: Parent PID = %d\n", getppid());

        // Sort the array
        printf("Child: Sorting the array...\n");
        sort_array(arr, n);

        // Display sorted array
        display_array(arr, n, "Child - Sorted array");

        printf("Child: My work is done. Exiting...\n");
        exit(0); // Child exits after completing its task
    }
    else
    {
        // Parent process
        printf("\n--- PARENT PROCESS ---\n");
        printf("Parent: My PID = %d\n", getpid());
        printf("Parent: Created child with PID = %d\n", pid);

        // Parent waits for child to complete
        printf("Parent: Waiting for child to finish sorting...\n");
        wait(NULL); // Wait for any child to finish

        printf("Parent: Child process has completed.\n");

        // Display array in parent (still original due to copy-on-write)
        display_array(arr, n, "Parent - Array after child (unchanged)");

        printf("Parent: Exiting...\n");
    }

    return 0;
}