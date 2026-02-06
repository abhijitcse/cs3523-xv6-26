#include "kernel/types.h"
#include "user/user.h"

void helloTest(void)
{
    printf("Testing single hello\n\n");
    hello();
    printf("\nTesting multiple hello's\n\n");
    for (int i = 0; i < 5; i++)
    {
        hello();
    }
    printf("Hello system call tests passed\n\n");
}

int main(){
    helloTest();
    exit(0);
}