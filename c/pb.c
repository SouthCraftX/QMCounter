#include <stdio.h>
#include <unistd.h>
int main()
{
    for (size_t i = 0; i < 50; i++)
    {
        printf("[\033[0m\033[9m\033[34m");
        size_t j = 0;
        for (; j < i; j++)
        {
            printf("━");
        }
        printf("\033[0m\033[9m\033[35m");
        for (; j < 50 ; j++)
        {
            printf("━");
        }
        fflush(stdout);
        sleep(1);
        putchar('\r');
    }
    puts("\033[0m");
    
}