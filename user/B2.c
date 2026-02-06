#include "kernel/types.h"
#include "user/user.h"

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
        pause(10);
        exit(0);
    }
    pause(5);
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
            pause(50);
            exit(0);
        }
    }
    pause(10);
    if (getnumchild() != n)
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

    pause(20);
    if (getnumchild() != 0)
    {
        printf("Zombie exclusion test failed\n");
        exit(1);
    }
    wait(0);
    printf("Zombie exclusion test passed\n");

    printf("\nFork/exit in bulk test\n");
    for (int i = 0; i < 500; i++)
    {
        pid = fork();
        if (pid == 0)
            exit(0);
        getnumchild(); 
    }
    while (wait(0) >= 0);


    printf("Fork/exit in bulk test passed\n");

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

    printf("All tests Passed\n\n");
}

int main()
{
    getnumchildtest();
    exit(0);
}