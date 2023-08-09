#include <cstdio>
#include <atomic>

int main()
{
    atomic_int x;
    int y;
    scanf("%i",&y);
    x = y;
    printf("x=%i",(int)x);
}