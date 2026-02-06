#include "kernel/types.h"
#include "user/user.h"

void getppidtest(void)
{
    printf("Testing getppid\n\n");

    printf("Printing ppid of current process\n");
    int ppid = getppid();
    int pid = getpid2();
    
    printf("PID=%d PPID=%d\n", pid, ppid);
    if (ppid <= 0 || pid <= 0)
    {
        printf("invalid pid/ppid\n");
        exit(1);
    }
    printf("Printing ppid of current process Passed \n\n");
    
    printf("Testing ppid of multiple forks\n");
    for (int i = 0; i < 20; i++)
    {
        int c = fork();
        if (c == 0)
        {
            int myppid = getppid();
            if (myppid != ppid && myppid <= 0)
            {
                printf("Children not allocated proper parent id\n");
                exit(1);
            }
            exit(0);
        }
    }
    for (int i = 0; i < 20; i++)
    {
        wait(0);
    }
    printf("Testing ppid of multiple forks passed!\n\n");

    printf("Testing grandchild\n");
    int c1 = fork();
    if (c1 == 0)
    {
        int c2 = fork();
        if (c2 == 0)
        {
            int p = getppid();
            printf(" PID of grandchild = %d PPID of grandchild =%d\n", getpid2(), p);
            if (p <= 0)
            {
                printf("FAIL: invalid PPID in grandchild\n");
                exit(1);
            }
            exit(0);
        }
        wait(0);
        exit(0);
    }
    wait(0);
    printf("Testing grandchild Passed\n\n");

    printf("Testing reallocation of child after parent exits\n");
    int ch = fork();
    if (ch == 0)
    {
        pause(5);
        int newppid = getppid();
        printf("child PID=%d new PPID=%d\n", getpid2(), newppid);
        if (newppid != 1)
        {
            printf("FAIL: child not reparented to init\n");
            exit(1);
        }
        printf("All getppid test passed! Please press enter to get a better shell line.\n\n");
        exit(0);
    }
    else
    {
        exit(0);
    }
}


int main()
{
    getppidtest();
    exit(0);
}