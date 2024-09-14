#include "seedy.h"

int main(int argc, char ** argv)
{
    int i = 0;
    uint8_t* buffer = NULL;
    int stop = argc > 1 ? atoi( argv[1] ) : SEEDY_WIDTH;

    /* We need a buffer to store the seed of indefinite size */
    buffer = malloc( sizeof(seedy_t) * stop );
    if ( buffer == NULL ) exit( EXIT_FAILURE );

    /* Get the seed */
#   if defined(_OPT64)
        seedy64( buffer, stop );
#   endif
#   if defined(_OPT32)      
        seedy32( buffer, stop );
#   endif
#   if defined(_OPT16) 
        seedy16( buffer, stop );
#   endif
    
    /* Write seed to output */
    while(i < stop)
        fprintf(stdout, "%02x", buffer[i++]);
    
    /* Free your memory. */
    free( buffer );

    return EXIT_SUCCESS;
}