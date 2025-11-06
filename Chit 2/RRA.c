#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10

struct Process
{
    int pid;        // Process ID
    int arrival;    // Arrival time
    int burst;      // Burst time
    int remaining;  // Remaining burst time
    int completion; // Completion time
    int waiting;    // Waiting time
    int turnaround; // Turnaround time
    bool completed; // Completion status
};

// Function to implement Round Robin scheduling
void roundRobin(struct Process processes[], int n, int timeQuantum)
{
    int currentTime = 0;
    int completed = 0;
    int totalWaiting = 0;
    int totalTurnaround = 0;

    // Initialize processes
    for (int i = 0; i < n; i++)
    {
        processes[i].remaining = processes[i].burst;
        processes[i].completed = false;
        processes[i].completion = 0;
        processes[i].waiting = 0;
        processes[i].turnaround = 0;
    }

    printf("\n=== ROUND ROBIN SCHEDULING (Time Quantum: %d) ===\n", timeQuantum);
    printf("\nGantt Chart:\n");
    printf("Time -> Process\n");

    int prevProcess = -1;
    bool executionStarted = false;

    // Continue until all processes are completed
    while (completed < n)
    {
        bool processExecuted = false;

        for (int i = 0; i < n; i++)
        {
            // Skip if process is completed or hasn't arrived yet
            if (processes[i].completed || processes[i].arrival > currentTime)
            {
                continue;
            }

            // Execute the process for time quantum or until completion
            int executionTime = (processes[i].remaining < timeQuantum) ? processes[i].remaining : timeQuantum;

            // Print Gantt chart
            if (prevProcess != processes[i].pid)
            {
                if (executionStarted)
                {
                    printf("%d] ", currentTime);
                }
                printf("[%d -> P%d ", currentTime, processes[i].pid);
                executionStarted = true;
            }

            // Update current time and remaining time
            currentTime += executionTime;
            processes[i].remaining -= executionTime;
            processExecuted = true;
            prevProcess = processes[i].pid;

            // Check if process is completed
            if (processes[i].remaining == 0)
            {
                processes[i].completion = currentTime;
                processes[i].turnaround = processes[i].completion - processes[i].arrival;
                processes[i].waiting = processes[i].turnaround - processes[i].burst;
                processes[i].completed = true;
                completed++;

                totalWaiting += processes[i].waiting;
                totalTurnaround += processes[i].turnaround;

                printf("%d] ", currentTime);
                prevProcess = -1;
            }

            // Break after executing one process in this cycle
            break;
        }

        // If no process was executed in this cycle, move time forward
        if (!processExecuted)
        {
            // Find the next arrival time
            int nextArrival = __INT_MAX__;
            for (int i = 0; i < n; i++)
            {
                if (!processes[i].completed && processes[i].arrival > currentTime)
                {
                    if (processes[i].arrival < nextArrival)
                    {
                        nextArrival = processes[i].arrival;
                    }
                }
            }

            if (nextArrival != __INT_MAX__)
            {
                if (executionStarted)
                {
                    printf("%d] ", currentTime);
                }
                printf("[%d -> IDLE ", currentTime);
                currentTime = nextArrival;
                printf("%d] ", currentTime);
                prevProcess = -1;
            }
        }
    }

    // Display results
    printf("\n\nProcess Details:\n");
    printf("PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
    printf("---\t-------\t-----\t----------\t-------\t----------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\t%d\t\t%d\t%d\n",
               processes[i].pid,
               processes[i].arrival,
               processes[i].burst,
               processes[i].completion,
               processes[i].waiting,
               processes[i].turnaround);
    }

    printf("\nAverage Waiting Time: %.2f\n", (float)totalWaiting / n);
    printf("Average Turnaround Time: %.2f\n", (float)totalTurnaround / n);
}

// Function to sort processes by arrival time
void sortByArrival(struct Process processes[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].arrival > processes[j + 1].arrival)
            {
                // Swap processes
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

// Function to display process table
void displayProcessTable(struct Process processes[], int n)
{
    printf("\nInput Processes:\n");
    printf("PID\tArrival\tBurst\n");
    printf("---\t-------\t-----\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d\t%d\t%d\n", processes[i].pid, processes[i].arrival, processes[i].burst);
    }
}

int main()
{
    int n, timeQuantum;
    struct Process processes[MAX_PROCESSES];

    printf("=== ROUND ROBIN CPU SCHEDULING ALGORITHM ===\n");

    // Get number of processes
    printf("Enter number of processes: ");
    scanf("%d", &n);

    if (n <= 0 || n > MAX_PROCESSES)
    {
        printf("Error: Number of processes must be between 1 and %d.\n", MAX_PROCESSES);
        return 1;
    }

    // Get time quantum
    printf("Enter time quantum: ");
    scanf("%d", &timeQuantum);

    if (timeQuantum <= 0)
    {
        printf("Error: Time quantum must be positive.\n");
        return 1;
    }

    // Input process details
    printf("\nEnter process details:\n");
    for (int i = 0; i < n; i++)
    {
        processes[i].pid = i + 1;
        printf("\nProcess P%d:\n", i + 1);
        printf("Enter arrival time: ");
        scanf("%d", &processes[i].arrival);
        printf("Enter burst time: ");
        scanf("%d", &processes[i].burst);

        if (processes[i].burst <= 0)
        {
            printf("Error: Burst time must be positive.\n");
            return 1;
        }
    }

    // Display input processes
    displayProcessTable(processes, n);

    // Sort processes by arrival time
    sortByArrival(processes, n);

    // Execute Round Robin scheduling
    roundRobin(processes, n, timeQuantum);

    return 0;
}