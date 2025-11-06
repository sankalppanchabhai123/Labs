#include <stdio.h>
#include <stdlib.h>

// Simple SSTF implementation
void sstf(int requests[], int n, int head)
{
    int visited[100] = {0}; // Static array instead of dynamic allocation
    int total_seek = 0;
    int current = head;

    printf("SSTF Disk Scheduling\n");
    printf("Initial Head Position: %d\n", head);
    printf("Request Queue: ");
    for (int i = 0; i < n; i++)
    {
        printf("%d ", requests[i]);
    }
    printf("\n\nSeek Sequence: %d", head);

    for (int i = 0; i < n; i++)
    {
        int min_distance = 10000; // Large number
        int next_index = -1;

        // Find closest request
        for (int j = 0; j < n; j++)
        {
            if (!visited[j])
            {
                int distance = abs(requests[j] - current);
                if (distance < min_distance)
                {
                    min_distance = distance;
                    next_index = j;
                }
            }
        }

        visited[next_index] = 1;
        total_seek += min_distance;
        current = requests[next_index];
        printf(" -> %d", current);
    }

    printf("\n\nTotal Seek Time: %d\n", total_seek);
    printf("Average Seek Time: %.2f\n", (float)total_seek / n);
}

int main()
{
    // Sample data
    int requests[] = {98, 183, 37, 122, 14, 124, 65, 67};
    int n = 8;
    int head = 53;

    sstf(requests, n, head);
    return 0;
}
