#include <stdio.h>
#include <stdlib.h>

int main()
{
    volatile int temp = 0;
    char *buf = (char *)malloc(sizeof("Hello\r\n"));
    
    sprintf(buf, "Hello\r\n");
    while (1)
    {
        temp++;
    }
}