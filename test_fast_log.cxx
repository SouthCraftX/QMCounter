#include <cstdio>
#include <cstdint>

unsigned char quick_log10(unsigned long long x)
{
    unsigned char                           ret = 0;
    if (x < 10ULL)                          { ret = 1; }
    else if (x < 100ULL)                    { ret = 2; }
    else if (x < 1000ULL)                   { ret = 3; }
    else if (x < 10000ULL)                  { ret = 4; }
    else if (x < 100000ULL)                 { ret = 5; }
    else if (x < 1000000ULL)                { ret = 6; }
    else if (x < 10000000ULL)               { ret = 7; }
    else if (x < 100000000ULL)              { ret = 8; }
    else if (x < 1000000000ULL)             { ret = 9; }
    else if (x < 10000000000ULL)            { ret = 10; }
    else if (x < 100000000000ULL)           { ret = 11; }
    else if (x < 1000000000000ULL)          { ret = 12; }
    else if (x < 10000000000000ULL)         { ret = 13; }
    else if (x < 100000000000000ULL)        { ret = 14; }
    else if (x < 1000000000000000ULL)       { ret = 15; }
    else if (x < 10000000000000000ULL)      { ret = 16; }
    else if (x < 100000000000000000ULL)     { ret = 17; }
    else if (x < 1000000000000000000ULL)    { ret = 18; }
    else                                    { ret = 19; }
    return                                  ret;        
}

int main()
{
    std::int32_t x;
    while (true)
    {
        std::puts("Input:");
        std::scanf("%i",&x);
        std::printf("quick_log10(%i)=%i\n",x,quick_log10(x));
    }
    
}