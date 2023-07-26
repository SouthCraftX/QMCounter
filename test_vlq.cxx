#include <cstdio>

unsigned vlq_to_u32(unsigned char* p_vlq)
{
    unsigned n = *p_vlq & 0x7FU;
    while ((*p_vlq)++ & 0x80u)
    {
        (n <<= 7) |= *p_vlq & 0x7FU;
    }
    return n;
}

int main()
{
    unsigned char vlq[] = {0xC0,0x80,0x80};
    printf("Convert:%u",vlq_to_u32(vlq));
}