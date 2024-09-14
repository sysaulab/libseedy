#include "seedy.h"

int main(int argc, char** argv)
{
    uint32_t buf[1024];
    int x = 0;
    MT32 prng;
    mt32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        while( x < 1024 )
        {
            buf[x] = mt32_next(&prng);
            x++;
        }
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


