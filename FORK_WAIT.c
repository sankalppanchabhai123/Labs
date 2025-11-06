#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
// #include <sys/wait.h>

// Function to sort an array using bubble sort
void bubbleSort(int arr[], int n)
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

// Function to display array elements
void displayArray(int arr[], int n)
{
    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main()
{
    int n;
    int *arr;
    pid_t pid;

    printf("=== PARENT SORTS, CHILD DISPLAYS ===\n");

    // Get array size from user
    printf("Enter the number of integers: ");
    scanf("%d", &n);

    // Validate input
    if (n <= 0)
    {
        printf("Error: Number of integers must be positive.\n");
        return 1;
    }

    // Allocate memory for array
    arr = (int *)malloc(n * sizeof(int));
    if (arr == NULL)
    {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Get array elements from user
    printf("Enter %d integers:\n", n);
    for (int i = 0; i < n; i++)
    {
        printf("Element %d: ", i + 1);
        scanf("%d", &arr[i]);
    }

    // Display original array
    printf("\nOriginal array: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Create a child process
    pid = fork();

    if (pid < 0)
    {
        // Fork failed
        printf("Fork failed!\n");
        free(arr);
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("\n--- CHILD PROCESS (PID: %d) ---\n", getpid());
        printf("Child: Waiting for parent to sort the array...\n");

        // Child process waits a bit to ensure parent completes sorting
        sleep(1);

        // Display the sorted array
        displayArray(arr, n);
        printf("Child: Display completed. Exiting...\n");
    }
    else
    {
        // Parent process
        printf("\n--- PARENT PROCESS (PID: %d) ---\n", getpid());
        printf("Parent: Sorting the array...\n");

        // Sort the array
        bubbleSort(arr, n);

        printf("Parent: Sorting completed.\n");
        printf("Parent: Waiting for child to finish...\n");

        // Parent waits for child to complete
        wait(NULL);

        printf("Parent: Child process finished.\n");

        // Display sorted array from parent's perspective too
        printf("Parent: Final sorted array: ");
        for (int i = 0; i < n; i++)
        {
            printf("%d ", arr[i]);
        }
        printf("\n");

        // Clean up
        free(arr);
        printf("Parent: Exiting...\n");
    }

    return 0;
}