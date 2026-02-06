#include "kernel/types.h"
#include "user/user.h"


void getsyscounttest(void)
{
    printf("Testing getsyscounttest\n\n");

    printf("Testing syscount increment\n");
    int a = getsyscount();
    int b = getsyscount();
    int c = getsyscount();
    if (!(a < b && b < c)) {
        printf("FAIL: syscount not strictly increasing (%d %d %d)\n", a, b, c);
        exit(1);
    }
    printf("Testing syscount increment Passed\n");

    printf("\nTesting syscount independence for child\n");
    int fork_before = getsyscount();
    int pid = fork();
    
    if (pid == 0) {
        int child_count = getsyscount();
        if (child_count >= fork_before) {
            printf("FAIL: child syscount not independent\n");
            exit(1);
        }
        exit(0);
    }
    wait(0);
    int fork_after = getsyscount();
    if (fork_after <= fork_before) {
        printf("FAIL: fork not counted in parent\n");
        exit(1);
    }
    
    printf("Testing syscount independence for child Passed\n");


    printf("\nMultiple getsyscount() pressure\n");
    for (int i = 0; i < 5; i++) {
        if (fork() == 0) {
            for (int j = 0; j < 500; j++) {
                getpid2();
                getsyscount();
            }
            exit(0);
        }
    }
    
    for (int i = 0; i < 5; i++)
    wait(0);
    printf("Multiple getsyscount() pressure Passed\n");
    
    printf("All tests Passed\n"); 
}


int main()
{
    getsyscounttest();
    exit(0);
}