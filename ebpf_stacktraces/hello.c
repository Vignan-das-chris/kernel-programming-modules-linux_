#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    while (1)
    {
        printf("Hello, world! PID = %d\n", getpid());
        usleep(10000);
    }
    exit(EXIT_SUCCESS);
}