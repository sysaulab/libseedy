#include <stdio.h>
#include <time.h>
#include <dos.h>

unsigned long int seedy()
{
    unsigned long int h = 0;
    int j = 0;
    while(j++ < 192)
    {
        delay(13);
        h = ( h << 1 ) | ( h >> 31 );
        h ^= (unsigned long int)clock()&1;
    }
    return h;
}

int main()
{

    fprintf(stdout, "%08lx", seedy());
    return 1;
}


