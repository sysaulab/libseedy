
#include "tools.h"
#include "algos/cc2032.h"

#include "../libseedy/libseedy32.h"

int main(int argc, char** argv)
{
    uint8_t buf[10240];
    CC2032 prng;

    cc2032_init(&prng, seedy32);

    while (1)
    {
        cc2032_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }

    return 0;
}
