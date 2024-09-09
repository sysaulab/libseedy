#include <stdio.h>
#include <stdlib.h>

#define SHISHUA 1
#define MTWISTER 2
#define RAND 3

#if defined(_MSC_VER)

    /* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING WIN16 TARGET */
    #if (_MSC_VER < 1200)
        #define PRNG RAND

    /* NO LONG LONG, ASSUMING WIN32 TARGET */
    #elif (_MSC_VER < 1310)
        #define PRNG MTWISTER

    /* NO STDINT but LONG LONG is available */
    #elif (_MSC_VER < 1930) 
        #define PRNG MTWISTER

    /* STDINT is available... modern compiler */
    #else
        #define PRNG SHISHUA
    
    #endif

/* POSIX */
#else
    #define PRNG SHISHUA
#endif

#if (PRNG == SHISHUA)
    #include "shishua.h"

#elif (PRNG == MTWISTER)
    #include "mtwist.h"

#elif (PRNG == RAND)
    #include "shishua.h"

#endif



#include "seedy.h"

#define buffer_length (1024*16)

int main(int argc, const char * argv[])
{
    #if (PRNG == SHISHUA)

        uint8_t buffer[buffer_length];
        uint64_t newseed[4];
        prng_state s;
        seedy((uint8_t*)newseed, 4 * sizeof(uint64_t));
        prng_init(&s, newseed);
        while(1)
        {
            prng_gen(&s, buffer, buffer_length);
            fwrite(buffer, sizeof(char), buffer_length, stdout);
        }

    #elif (PRNG == MTWISTER)

        MTRand mt;
		uint32_t buffer;
        uint32_t newseed;
        seedy((uint8_t*)&newseed, sizeof(newseed));
        mt = seedRand(newseed);
        while(1)
        {
            buffer = genRandLong(&mt);
            fwrite(&buffer, sizeof(uint32_t), 1, stdout);
        }

    #elif (PRNG == RAND)

        uint8_t buffer;
        unsigned int newseed;
        seedy((uint8_t*)&newseed, sizeof(newseed));
        srand((unsigned int)newseed);
        while(1)
        {
            buffer = (char)rand();
            fwrite(&buffer, sizeof(char), 1, stdout);
        }

    #endif

    return 1;
}
