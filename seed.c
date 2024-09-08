#include <stdio.h>
#include <stdlib.h>
#include "seedy.h"

#if defined(_WIN32) && (_MSC_VER < 1940)
#include "stdint.h"
#else
#include <stdint.h>
#endif

int main(int argc, const char * argv[])
{
    /* Bits to bytes with remainder */
    int stop = argc > 1 ? atoi( argv[1] ) / 64 + ( atoi( argv[1] ) % 64 ? 1 : 0 ) : 1;

    /* We need a buffer to store the seed of indefinite size */
    uint64_t* buffer = malloc( sizeof(uint64_t) * stop );
    if ( buffer == NULL ) exit( EXIT_FAILURE );

    /* Get the seed */
    seedy( buffer, stop );
    
    /* Write seed to output */
    fwrite( buffer, sizeof(uint64_t), stop, stdout );

    /* Free your memory. */
    free( buffer );

    return EXIT_SUCCESS;
}
