#include "tools.h"
#include "algos/cc32.h"

int main(int argc, char** argv)
{
    uint8_t buf[10240];
    CC32 prng;
    cc32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        cc32_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }

    return 0;
}


