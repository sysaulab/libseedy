#include "tools.h"
#include "algos/nm64.h"

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    NM64 prng;
    nm64_init(&prng);
    
    while (1)
    {
        nm64_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

