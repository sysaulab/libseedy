#include "seedy.h"

int main(int argc, char** argv)
{
    MT16 prng;
    mt16_init(&prng, parseinputgen(argc, argv));

    while (1)
    {
        uint16_t buf[1024];

        for ( int x = 0; x < 1024; x++)
            buf[x] = mt16_next(&prng);

        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}