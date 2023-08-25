#include <cstdio>

unsigned nv_getVLi_u32(unsigned char**p)
{
	unsigned VLnum = **p & 0x7Fu;

	while (*(*p)++ & 0x80u)
	{
		(VLnum <<= 7);
        VLnum |= **p & 0x7Fu;
	}

	return VLnum;
}

unsigned vlq_to_u32_move(unsigned char** x)
{
    unsigned result = 0;
    unsigned char b;
    do
    {
        b = *(*x++);
        result = (result << 7) | (b & 0x7F);
    } while (b & 0b10000000);
    return result;
}

unsigned char vlq[] = {0xFF,0xFF,0xFF,0x7F};

int main()
{
    unsigned char* p = vlq;
    printf("Convert:%u,%u",nv_getVLi_u32(&p),vlq_to_u32_move(&p));

}