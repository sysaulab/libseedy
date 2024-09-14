#include "seedy.h"

int main(int argc, char** argv)
{
    int x = 0;
    uint32_t buf[1024];
    QX32 prng;
    qx32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        while ( x < 1024 )
        {
            buf[x] = qx32_next(&prng);
            x++;
        }
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


