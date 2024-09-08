#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seed.h"

#if defined(_WIN32) && (_MSC_VER < 1940)
#include "stdint.h"
#include <windows.h>
#else
#include <stdint.h>
#include "shishua.h"
#endif

#define buffer_length (1024*16)

int main(int argc, const char * argv[])
{
#if defined(_WIN32) && (_MSC_VER < 1940)
    uint8_t buffer;
    uint64_t newseed;
    seed(&newseed, 1);
    srand((unsigned int)newseed);
#else
    uint8_t buffer[buffer_length];
    uint64_t newseed[4];
	prng_state s;
    seed(newseed, 4);
    prng_init(&s, newseed);
#endif

    while(1)
    {
#if defined(_WIN32) && (_MSC_VER < 1940)
        buffer = (uint8_t)rand();
        fwrite(&buffer, sizeof(uint8_t), 1, stdout);
#else
        prng_gen(&s, buffer, buffer_length);
        fwrite(buffer, sizeof(uint8_t), buffer_length, stdout);
#endif
    }

    return EXIT_SUCCESS;
}
