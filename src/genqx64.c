#include "seedy.h"

int main(int argc, char** argv)
{
    int x = 0;
    uint64_t buf[1024];
    QX64 prng;
    qx64_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        while ( x < 1024 )
        {
            buf[x] = qx64_next(&prng);
            x++;
        }
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

