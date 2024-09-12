#include "qxo16.h"
#include "seedy.h"

int main(int argc, char** argv)
{
    qxo_t seed[65536*4];
    QXO16 prng;
    qxo16_init(&prng);
    
    while (1)
    {
        qxo_t buf[1024];

        for ( int x = 0; x < 1024; x++)
            buf[x] = qxo16_next(&prng);

        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}