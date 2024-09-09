#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seedy.h"

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
    unsigned int newseed;
    seedy((uint8_t*)&newseed, sizeof(newseed));
    srand((unsigned int)newseed);
#else
    uint8_t buffer[buffer_length];
    uint64_t newseed[4];
	prng_state s;
    seedy((uint8_t*)newseed, 4 * sizeof(uint64_t));
    prng_init(&s, newseed);
#endif

    while(1)
    {
#if defined(_WIN32) && (_MSC_VER < 1940)
        buffer = (char)rand();
        fwrite(&buffer, sizeof(char), 1, stdout);
#else
        prng_gen(&s, buffer, buffer_length);
        fwrite(buffer, sizeof(char), buffer_length, stdout);
#endif
    }

    return EXIT_SUCCESS;
}
