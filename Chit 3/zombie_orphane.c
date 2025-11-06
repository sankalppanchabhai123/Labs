#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void demonstrate_zombie_process()
{
    printf("\n=== DEMONSTRATING ZOMBIE PROCESS ===\n");
    printf("A zombie process occurs when a child exits but parent hasn't called wait()\n");
    printf("The child process remains in process table as 'defunct' until parent collects it\n\n");

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        printf("CHILD:  My PID = %d, Parent PID = %d\n", getpid(), getppid());
        printf("CHILD:  I'm exiting now after 2 seconds...\n");
        sleep(2);
        printf("CHILD:  Exiting now! I'll become a zombie until parent collects me.\n");
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process
        printf("PARENT: My PID = %d, Created Child PID = %d\n", getpid(), pid);
        printf("PARENT: I'll sleep for 10 seconds WITHOUT calling wait()...\n");
        printf("PARENT: Run this command in another terminal to see the zombie:\n");
        printf("        'ps -eo pid,ppid,state,comm | grep %d'\n", pid);
        printf("PARENT: You'll see the child marked as 'Z' (zombie)\n\n");

        sleep(10); // Parent sleeps without calling wait() - child becomes zombie

        printf("PARENT: Woke up after 10 seconds. Now calling wait()...\n");
        int status;
        wait(&status); // Collect child's exit status
        printf("PARENT: Collected child exit status: %d\n", WEXITSTATUS(status));
        printf("PARENT: Zombie process has been cleaned up!\n");
    }
    else
    {
        perror("fork failed");
        exit(1);
    }
}

void demonstrate_orphan_process()
{
    printf("\n=== DEMONSTRATING ORPHAN PROCESS ===\n");
    printf("An orphan process occurs when parent exits before child\n");
    printf("The orphan gets adopted by init process (PID 1) and doesn't become zombie\n\n");

    pid_t pid = fork();

    if (pid == 0)
    {
        // Child process
        printf("CHILD:  My PID = %d, Original Parent PID = %d\n", getpid(), getppid());
        printf("CHILD:  My parent will exit in 3 seconds, making me an orphan...\n");
        printf("CHILD:  Sleeping for 8 seconds to demonstrate orphan state...\n\n");

        sleep(8); // Child sleeps longer than parent

        printf("CHILD:  Woke up! My new Parent PID = %d (init process)\n", getppid());
        printf("CHILD:  I've been adopted by init process (PID 1)\n");
        printf("CHILD:  Init will automatically collect me when I exit - no zombie!\n");
        printf("CHILD:  Exiting normally now.\n");
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process
        printf("PARENT: My PID = %d, Created Child PID = %d\n", getpid(), pid);
        printf("PARENT: I'll exit in 3 seconds, leaving my child as an orphan...\n");

        sleep(3); // Parent sleeps briefly then exits

        printf("PARENT: Exiting now! My child (PID %d) will become an orphan.\n", pid);
        exit(0); // Parent exits without waiting for child
    }
    else
    {
        perror("fork failed");
        exit(1);
    }
}

void check_process_states()
{
    printf("\n=== PROCESS STATE CODES ===\n");
    printf("R - Running or runnable\n");
    printf("S - Interruptible sleep\n");
    printf("D - Uninterruptible sleep\n");
    printf("Z - Zombie process\n");
    printf("T - Stopped\n");
}

int main()
{
    printf("========================================\n");
    printf("   ZOMBIE & ORPHAN PROCESS DEMONSTRATION\n");
    printf("========================================\n");
    printf("Initial Parent Process PID: %d\n\n", getpid());

    check_process_states();

    // Demonstrate zombie process first
    demonstrate_zombie_process();

    // Wait between demonstrations
    printf("\nWaiting 3 seconds before orphan demonstration...\n");
    sleep(3);

    // Demonstrate orphan process
    // Note: The parent in main() will exit after creating the orphan demo
    demonstrate_orphan_process();

    // This won't be reached for orphan demo since parent exits
    printf("\nMain program completed.\n");
    return 0;
}