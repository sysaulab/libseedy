#include "common.h"

#if defined(LIBSEEDY_OPT64)
#   include "libseedy64.h"
#elif defined(LIBSEEDY_OPT32)      
#   include "libseedy32.h"
#elif defined(LIBSEEDY_OPT16) 
#   include "libseedy16.h"
#endif

#include <stdlib.h>

int main(int argc, char ** argv)
{
    int i = 0;
    uint8_t* buffer = NULL;
    int stop = argc > 1 ? atoi( argv[1] ) : 8;

    /* We need a buffer to store a seed of indefinite size */
    buffer = malloc( sizeof(uint8_t) * stop );
    if ( buffer == NULL ) exit( EXIT_FAILURE );

    /* Get the seed */
#   if defined(LIBSEEDY_OPT64)
        seedy64( buffer, stop );
#   elif defined(LIBSEEDY_OPT32)      
        seedy32( buffer, stop );
#   elif defined(LIBSEEDY_OPT16) 
        seedy16( buffer, stop );
#   endif
    
    /* Write seed to output */
    while(i < stop)
        fprintf(stdout, "%02x", buffer[i++]);
    
    /* Free your memory. */
    free( buffer );

    return EXIT_SUCCESS;
}
