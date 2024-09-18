#include "tools.h"
#include "algos/mt32.h"

int main(int argc, char** argv)
{
    uint8_t buf[10240];
    MT32 prng;
    mt32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        mt32_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}
