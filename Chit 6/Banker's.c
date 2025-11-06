#include <stdio.h>
#include <stdbool.h>

int main()
{
    int n = 5, m = 3; // processes, resources

    int alloc[5][3] = {{0, 1, 0}, {2, 0, 0}, {3, 0, 2}, {2, 1, 1}, {0, 0, 2}};
    int max[5][3] = {{7, 5, 3}, {3, 2, 2}, {9, 0, 2}, {2, 2, 2}, {4, 3, 3}};
    int avail[3] = {3, 3, 2};

    int need[5][3], work[3];
    bool finish[5] = {false};
    int safe[5], count = 0;

    // Calculate need matrix
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            need[i][j] = max[i][j] - alloc[i][j];

    // Initialize work
    for (int i = 0; i < m; i++)
        work[i] = avail[i];

    // Safety algorithm
    while (count < n)
    {
        bool found = false;

        for (int i = 0; i < n; i++)
        {
            if (!finish[i])
            {
                bool can = true;
                for (int j = 0; j < m; j++)
                {
                    if (need[i][j] > work[j])
                    {
                        can = false;
                        break;
                    }
                }

                if (can)
                {
                    for (int j = 0; j < m; j++)
                        work[j] += alloc[i][j];

                    safe[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found)
        {
            printf("Unsafe state!\n");
            return 1;
        }
    }

    printf("Safe sequence: ");
    for (int i = 0; i < n; i++)
        printf("P%d ", safe[i]);
    printf("\n");

    return 0;
}