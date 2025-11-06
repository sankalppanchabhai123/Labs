#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

struct Process
{
    int pid;  
    int arrival;
    int burst;
    int remaining;  
    int completion;
    int waiting;
    int turnaround;
    bool completed; 
};


int findShortestJob(struct Process processes[], int n, int currentTime)
{
    int shortest = -1;
    int minRemaining = INT_MAX;

    for (int i = 0; i < n; i++)
    {
        if (processes[i].arrival <= currentTime &&
            !processes[i].completed &&
            processes[i].remaining > 0)
        {

            if (processes[i].remaining < minRemaining)
            {
                minRemaining = processes[i].remaining;
                shortest = i;
            }

            else if (processes[i].remaining == minRemaining)
            {
                if (processes[i].arrival < processes[shortest].arrival)
                {
                    shortest = i;
                }
            }
        }
    }
    return shortest;
}

void sjfPreemptive(struct Process processes[], int n)
{
    int completed = 0;
    int currentTime = 0;
    int totalWaiting = 0;
    int totalTurnaround = 0;
    int prev = -1;

    printf("\nGantt Chart:\n");
    printf("Time -> Process\n");

    for (int i = 0; i < n; i++)
    {
        processes[i].remaining = processes[i].burst;
        processes[i].completed = false;
    }

    while (completed < n)
    {
        int current = findShortestJob(processes, n, currentTime);

        if (current == -1)
        {
            currentTime++;
            continue;
        }


        printf("%d -> P%d ", currentTime, processes[current].pid);
        prev = current;

        processes[current].remaining--;
        currentTime++;

        if (processes[current].remaining == 0)
        {
            processes[current].completion = currentTime;
            processes[current].turnaround = processes[current].completion - processes[current].arrival;
            processes[current].waiting = processes[current].turnaround - processes[current].burst;
            processes[current].completed = true;
            completed++;

            totalWaiting += processes[current].waiting;
            totalTurnaround += processes[current].turnaround;
        }
    }

    if (prev != -1)
    {
        printf("%d - P%d\n", currentTime, processes[prev].pid);
    }

    printf("\nProcess Details:\n");
    printf("PID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n");
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

int main()
{
    int n;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    struct Process processes[n];

    for (int i = 0; i < n; i++)
    {
        processes[i].pid = i + 1;
        printf("Enter arrival time for process P%d: ", i + 1);
        scanf("%d", &processes[i].arrival);
        printf("Enter burst time for process P%d: ", i + 1);
        scanf("%d", &processes[i].burst);
    }


    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (processes[j].arrival > processes[j + 1].arrival)
            {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    sjfPreemptive(processes, n);

    return 0;
}