#include "seedy.h"

#define SHISHUA 1
#define MT32 2
#define RAND 3
#define buffer_length (1024*16)

#if defined(_MSC_VER)

    /* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING WIN16 TARGET */
    #if (_MSC_VER < 1200)
        #define PRNG RAND

    /* NO LONG LONG, ASSUMING WIN32 TARGET */
    #elif (_MSC_VER < 1310)
        #define PRNG MT32

    /* NO STDINT but LONG LONG is available */
    #elif (_MSC_VER < 1930) 
        #define PRNG MT32

    /* STDINT is available... modern compiler */
    #else
        #define PRNG SHISHUA
    
    #endif

/* POSIX */
#else
    #define PRNG SHISHUA
#endif



/* Lib C */



int main(int argc, char **argv)
{
    #if (PRNG == SHISHUA)

        SS64 ss;
        uint8_t buffer[buffer_length];
        ss64_init(&ss, parseinputgen(argc, argv));
        while(1)
        {
            ss64_fill(&ss, buffer, sizeof(buffer));
            fwrite(buffer, sizeof(char), buffer_length, stdout);
        }

    #elif (PRNG == MT32)

        MT32 mt;
		uint32_t buffer;
        mt32_init(&mt, parseinputgen(argc, argv));
        while(1)
        {
            buffer = mt32_next(&mt);
            fwrite(&buffer, sizeof(uint32_t), 1, stdout);
        }

    #elif (PRNG == RAND)

        int next;
        uint16_t out[16];
        unsigned int newseed;
        void* f = parseinputgen(argc, argv);
        ((void(*)(uint8_t* b, qxo_t n))f)((uint8_t*)&newseed, sizeof(newseed));
        srand((unsigned int)newseed);

        while(1)
        {
            int index = 0;
            int nindex = 1;
            int mask = 1;
            out[index] = rand() << 1; 
            while(index < 15)
            {
                nindex = index + 1;
                out[nindex] = rand();               
                out[index] |= (out[nindex] >> (14 - index)) & mask;
                mask = (mask << 1) + 1;
                index = nindex;
            }
            index = 0;
            while(index < 16)
            {
                index = index + 1;
            }
            fwrite(out, sizeof(out), 1, stdout);
        }
        
        

    #endif

    return 1;
}
