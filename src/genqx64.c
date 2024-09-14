#include "seedy.h"

int main(int argc, char** argv)
{
    QX64 prng;
    qx64_init(&prng, parseinputgen(argc, argv));

    while (1)
    {
        uint64_t buf[1024];

        for ( int x = 0; x < 1024; x++)
            buf[x] = qx64_next(&prng);

        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

