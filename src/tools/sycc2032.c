
#include "tools.h"
#include "algos/cc2032.h"
#if defined(LIBSEEDY_OPT64)
#   include <libseedy/libseedy64.h>
#elif defined(LIBSEEDY_OPT32)
#   include <libseedy/libseedy32.h>
#elif defined(LIBSEEDY_OPT16)
#   include <libseedy/libseedy16.h>
#endif



int main(int argc, char** argv)
{
    uint8_t buf[10240];
    CC2032 prng;
#if defined(LIBSEEDY_OPT64)
    cc2032_init(&prng, seedy64);
#elif defined(LIBSEEDY_OPT32)      
    cc2032_init(&prng, seedy32);
#elif defined(LIBSEEDY_OPT16) 
    cc2032_init(&prng, seedy16);
#endif    
    while (1)
    {
        cc2032_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }

    return 0;
}


