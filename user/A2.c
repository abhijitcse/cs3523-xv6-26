#include "kernel/types.h"
#include "user/user.h"

void getpid2test(void)
{
    printf("Testing getpid2\n\n");
    printf("Getting pid of the current process\n");
    int pid1 = getpid2();
    printf("PID from getpid2(): %d\n", pid1);
    if (pid1 <= 0)
    {
        printf("Getting pid of the current process Failed\n");
        exit(1);
    }
    printf("Getting pid of the current process Passed\n\n");

    int pid2 = getpid2();
    if (pid1 != pid2)
    {
        printf("Process id not same as before!\n");
        exit(1);
    }

    printf("\nTesting getpid2 for child and parent process.\n");
    int ppid = getpid2();
    int chpid = fork();
    if (chpid < 0)
    {
        printf("Fork failed\n");
        exit(1);
    }
    if (chpid == 0)
    {
        int pid = getpid2();
        printf("PID from getpid2() for child process: %d\n", pid);
        if (chpid == ppid)
        {
            printf("Child PID equals parent PID\n");
            exit(1);
        }
        exit(0);
    }
    else
    {
        wait(0);
        int after_pid = getpid2();
        if (after_pid != ppid)
        {
            printf("Parent PID changed\n");
            exit(1);
        }
        printf("getpid2 TestCases passed!\n\n");
    }
}

int main()
{
    getpid2test();
    exit(0);
}