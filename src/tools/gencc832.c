#include "algos/cc832.h"
#include "tools.h"

int main(int argc, char** argv)
{
    uint8_t buf[10240];
    CC832 prng;
    cc832_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        cc832_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }

    return 0;
}


