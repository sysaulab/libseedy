#include <stdlib.h>
#include <stdio.h>

#include "mt32.h"
#include "seedy.h"

int main(int argc, char** argv)
{
    mt_t seed;
    MT32 prng;
    mt32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        mt_t buf[1024];

        for ( int x = 0; x < 1024; x++)
            buf[x] = mt32_next(&prng);

        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}