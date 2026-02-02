#include "kernel/types.h"
#include "user/user.h"

void helloTest(void)
{
    printf("Testing single hello\n");
    if (hello() != 0)
    {
        printf("Hello failed!\n");
        exit(1);
    }
    printf("Testing multiple hello's\n");

    for (int i = 0; i < 5; i++)
    {
        if (hello() != 0)
        {
            printf("Hello failed!\n");
            exit(1);
        }
    }
    printf("Hello system call tests passed\n\n");
}

void getpid2test(void)
{
    printf("Testing getpid2\n\n");
    printf("Getting pid of the current process\n");
    int pid1 = getpid2();
    printf("PID from getpid2(): %d\n", pid1);

    if (pid1 <= 0)
    {
        printf("FAIL: Invalid PID\n");
        exit(1);
    }
    int pid2 = getpid2();
    if (pid1 != pid2)
    {
        printf("Process id not same as before!");
        exit(1);
    }

    printf("Testing getpid2 for child and parent process.\n");
    int ppid = getpid2();
    int chpid = fork();
    if (chpid < 0)
    {
        printf("Fork failed");
        exit(1);
    }
    if (chpid == 0)
    {
        int pid = getpid2();
        printf("PID from getpid2() for child process: %d\n", pid);
        if (chpid == ppid)
        {
            printf("FAIL: Child PID equals parent PID\n");
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
            printf("FAIL: Parent PID changed\n");
            exit(1);
        }
        printf("getpid2 TestCases passed!\n\n");
    }
}

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

    printf("Testting grandchild\n");
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

    printf("Testing reallocation of child after parent exits\n");
    int ch = fork();
    if (ch == 0)
    {
        pause(10);
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

void getnumchildtest(void)
{
    printf("Testing getnumchild\n");
    printf("\nNo child process test\n");
    if (getnumchild() != 0)
    {
        printf("No child process test failed\n");
        exit(1);
    }
    printf("No child process test passed\n");

    printf("\nSingle child test\n");
    int pid = fork();
    if (pid == 0)
    {
        pause(50);
        exit(0);
    }
    pause(10);
    if (getnumchild() != 1)
    {
        printf("Single child test failed\n");
        exit(1);
    }
    wait(0);
    printf("Single child test passed\n");

    printf("\nMultiple children test\n");
    int n = 10;
    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            pause(100);
            exit(0);
        }
    }
    pause(10);
    if (getnumchild() != 10)
    {
        printf("Multiple children test failed\n");
        exit(1);
    }
    for (int i = 0; i < n; i++)
    {
        wait(0);
    }
    printf("Multiple children test passed\n");

    printf("\nZombie exclusion test\n");
    pid = fork();
    if (pid == 0)
        exit(0);

    pause(20); // child is ZOMBIE
    if (getnumchild() != 0)
    {
        printf("Zombie exclusion test failed\n");
        exit(1);
    }
    wait(0);
    printf("Zombie exclusion test passed\n");

    printf("\nFork/exit in bulk test\n");
    for (int i = 0; i < 50; i++)
    {
        pid = fork();
        if (pid == 0)
            exit(0);
        getnumchild(); // concurrent inspection
    }
    while (wait(0) >= 0)
        ;
    printf("Fork/exit in bulk test passed\n");

    printf("\nConcurrent getnumchild readers\n");
    for (int i = 0; i < n; i++)
    {
        if (fork() == 0)
        {
            for (int j = 0; j < 100; j++)
                getnumchild();
            exit(0);
        }
    }
    for (int i = 0; i < n; i++)
    {
        wait(0);
    }
    printf("Concurrent getnumchild readers passed\n");

    printf("\nGrandchildren counting test\n");
    pid = fork();
    if (pid == 0)
    {
        fork();
        fork();
        pause(50);
        exit(0);
    }
    pause(10);
    if (getnumchild() != 1)
    {
        printf("Grandchildren counting test failed\n");
        exit(1);
    }
    wait(0);
    printf("Grandchildren counting test passed\n");

    printf("\nReparenting test\n");
    pid = fork();
    if (pid == 0)
    {
        pause(50);
        if (getppid() != 1)
        {
            printf("Reparenting test failed\n");
            exit(1);
        }
        printf("Reparenting test passed\n");
        printf("All tests passed!\n\n");
        exit(0);
    }
    exit(0);
}
int main()
{
    helloTest();
    getpid2test();
    // getppidtest();
    getnumchildtest();
    exit(0);
}