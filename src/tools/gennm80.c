#include "tools.h"
#include "algos/nm80.h"


#if defined(__GNUC__)

typedef unsigned __int128 uint128_t;
typedef __int128 int128_t;
#define INT128_C(n) ((int128_t)n)
#define INT128_MAX ((int128_t)(UINT128_MAX >> 1))
#define INT128_MIN (-INT128_MAX - 1)

#else
#   error "Unsupported compiler"
#endif /* defined(__GNUC__) */


int main(int argc, char** argv)
{
    uint8_t buf[4096];
    NM80 prng;
    nm80_init(&prng, "nm80_test.bin");
    uint128_t written = 0;
    
    while (1)
    {
        nm80_fill(&prng, buf, sizeof(buf), written);
        fwrite(buf, sizeof(buf), 1, stdout);
        written += sizeof(buf);
    }
    return 0;
}

