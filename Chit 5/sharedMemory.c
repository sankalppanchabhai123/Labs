// writer process

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/ipc.h>
// #include <sys/shm.h>
// #include <unistd.h>

// #define SHM_SIZE 1024

// int main() {
//     key_t key;
//     int shmid;
//     char *shared_memory;

//     // Generate key
//     key = 1234;

//     // Create shared memory
//     shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT);
//     if (shmid == -1) {
//         perror("shmget failed");
//         exit(1);
//     }

//     // Attach to shared memory
//     shared_memory = shmat(shmid, NULL, 0);
//     if (shared_memory == (char *)-1) {
//         perror("shmat failed");
//         exit(1);
//     }

//     // Write message to shared memory
//     char message[] = "Hello from Writer Process!";
//     strcpy(shared_memory, message);

//     printf("Writer: Written to shared memory: %s\n", shared_memory);
//     printf("Writer: Waiting for reader to read...\n");

//     // Wait until reader reads (check if first character is cleared)
//     while (shared_memory[0] != '\0') {
//         sleep(1);
//     }

//     printf("Writer: Message was read. Cleaning up...\n");

//     // Cleanup
//     shmdt(shared_memory);
//     shmctl(shmid, IPC_RMID, NULL);

//     return 0;
// }

// reader process

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1024

int main()
{
    key_t key;
    int shmid;
    char *shared_memory;

    // Generate same key
    key = 1234;

    // Get shared memory
    shmid = shmget(key, SHM_SIZE, 0666);
    if (shmid == -1)
    {
        perror("shmget failed");
        exit(1);
    }

    // Attach to shared memory
    shared_memory = shmat(shmid, NULL, 0);
    if (shared_memory == (char *)-1)
    {
        perror("shmat failed");
        exit(1);
    }

    printf("Reader: Waiting for message...\n");

    // Wait until writer writes something
    while (shared_memory[0] == '\0')
    {
        sleep(1);
    }

    printf("Reader: Read from shared memory: %s\n", shared_memory);

    // Clear the message to signal writer
    shared_memory[0] = '\0';

    printf("Reader: Signaled writer that message was read.\n");

    // Detach from shared memory
    shmdt(shared_memory);

    return 0;
}