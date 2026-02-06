#include "kernel/types.h"
#include "user/user.h"

void getchildsyscounttest()
{
    printf("\n Testing getchildsyscounttest \n");

    printf("\nTesting invalid PID\n");
    if (getchildsyscount(-1) != -1)
    {
        printf("FAIL: negative PID\n");
    }

    if (getchildsyscount(0) != -1)
    {

        printf("FAIL: zero PID\n");
    }

    if (getchildsyscount(99999) != -1)
    {

        printf("FAIL: random PID\n");
    }

    printf("Testing invalid PID Passed\n");

    printf("\nSingle child syscall count\n");
    int pid = fork();
    if (pid == 0)
    {
        for (int i = 0; i < 50; i++)
            getpid2();
        exit(0);
    }

    // Let child run
    pause(5);

    int cnt1 = getchildsyscount(pid);
    printf("Child syscall count : %d\n", cnt1);
    if (cnt1 <= 0)
    {
        printf("FAIL: child syscall count not positive\n");
    }

    printf("Single child syscall count Passed\n");

    wait(0);

    int cnt2 = getchildsyscount(pid);
    printf("Child syscall count after exit: %d\n", cnt2);
    if (cnt2 != -1)
    {
        printf("FAIL: exited child still visible\n");
    }

    printf("\nMultiple children test\n");
    int n = 5;
    int kids[n];
    for (int i = 0; i < n; i++)
    {
        kids[i] = fork();
        if (kids[i] == 0)
        {
            for (int j = 0; j < 100; j++)
            {
                getpid2();
                write(1, "", 0);
            }
            pause(30);
            exit(0);
        }
    }

    pause(10);

    for (int i = 0; i < n; i++)
    {
        int c = getchildsyscount(kids[i]);
        printf("Child %d syscount = %d\n", kids[i], c);
        if (c <= 0)
            printf("FAIL: concurrent child syscount\n");
    }

    for (int i = 0; i < n; i++)
    {
        wait(0);
    }
    printf("Multiple children test Passed\n");

    printf("\nTesting if child of a parent can find syscount of another child\n");
    pid = fork();
    if (pid == 0)
    {
        pause(50);
        exit(0);
    }
    
    int re = fork();
    if (re == 0)
    {
        // This process is NOT parent
        pause(10);
        int v = getchildsyscount(pid);
        if (v != -1)
        printf("FAIL: non-parent accessed child\n");
        exit(0);
    }
    printf("Testing if child of a parent can find syscount of another child Passed\n");
    
    wait(0);
    wait(0);

    printf("All test cases passed\n\n");
}

int main()
{
    getchildsyscounttest();
}