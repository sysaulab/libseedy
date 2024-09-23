#include "tools.h"
#include "algos/nm80.h"

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    NM80 prng;
    nm80_init(&prng, "nm80_test.bin");
    
    while (1)
    {
        nm80_fill(&prng, buf, sizeof(buf), 0, 0);
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

