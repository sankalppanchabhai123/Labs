#include <stdio.h>
#include <stdbool.h>

#define MAX_FRAMES 10

// Structure to represent a frame with timestamp
struct Frame
{
    int page;
    int timestamp; // Higher value means more recently used
};

// LRU with timestamp implementation
int lru_with_timestamp(int pages[], int n, int frame_size)
{
    struct Frame frames[MAX_FRAMES];
    int page_faults = 0;
    int time = 0;

    // Initialize frames
    for (int i = 0; i < frame_size; i++)
    {
        frames[i].page = -1;
        frames[i].timestamp = -1;
    }

    printf("LRU with Timestamp Implementation\n");
    printf("Frame Size: %d\n\n", frame_size);

    for (int i = 0; i < n; i++)
    {
        int current_page = pages[i];
        bool page_found = false;
        time++;

        // Check if page exists in frames
        for (int j = 0; j < frame_size; j++)
        {
            if (frames[j].page == current_page)
            {
                page_found = true;
                frames[j].timestamp = time; // Update timestamp
                printf("Page %2d: HIT\n", current_page);
                break;
            }
        }

        if (!page_found)
        {
            page_faults++;
            int replace_index = -1;

            // Find empty frame or least recently used
            for (int j = 0; j < frame_size; j++)
            {
                if (frames[j].page == -1)
                {
                    replace_index = j;
                    break;
                }
                if (replace_index == -1 || frames[j].timestamp < frames[replace_index].timestamp)
                {
                    replace_index = j;
                }
            }

            // Replace the page
            frames[replace_index].page = current_page;
            frames[replace_index].timestamp = time;

            printf("Page %2d: MISS - Replaced frame %d: [", current_page, replace_index);
            for (int j = 0; j < frame_size; j++)
            {
                if (frames[j].page != -1)
                {
                    printf("%d(t:%d) ", frames[j].page, frames[j].timestamp);
                }
                else
                {
                    printf(" - ");
                }
            }
            printf("]\n");
        }
    }

    return page_faults;
}