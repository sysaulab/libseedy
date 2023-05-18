/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Commercial licence available upon request */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libicm.h"

#define ___icm_BLOCKSIZE (4096) // size of uint64_t array, size in mb is 8 times greater
#define ___icm_BLOCK_BYTES (___icm_BLOCKSIZE * sizeof(uint64_t))

void flush_buffer_bin(uint64_t* buffer, uint64_t bytes)
{
    fwrite(buffer, sizeof(uint8_t), bytes, stdout);
}

void fillbuffer(icm_state_t* state, uint64_t* buffer, uint64_t buffer_size, uint64_t first)
{
    if(!first)
    {
        icm_get(state, buffer, buffer_size, 0);
    }
    else
    { 
        uint64_t y = 0;
        while( y < buffer_size )
        {
            for( int x = 0; x < NUM_THREADS; x++ )
            {
                state->nodes[x] = 0;
            }
            icm_get(state, buffer + y, 1, 0);
            y++;
        }
    }
}

int main(int argc, const char * argv[])
{
    uint64_t current_arg = argc - 1;
    uint32_t cfg_debug = 0;
    uint32_t cfg_first = 0;
    uint64_t* buffer;
    
    buffer = (uint64_t*) calloc(___icm_BLOCKSIZE, sizeof(uint64_t));

    if( buffer == NULL )
    {
        fprintf(stderr, "icm - ERROR - cannot allocate memory buffer");
        return 1;
    }

    for( int x = 1; x < argc; x++ )
    {
        if(!strcmp("info", argv[x]) || !strcmp("i", argv[x])){
            cfg_debug = 1;
        }
        else if(!strcmp("first", argv[x]) || !strcmp("f", argv[x])){
            cfg_first = 1;
        }
    }

    icm_state_t state;
    icm_init(&state);
    icm_go(&state);

#ifdef ICM_EXPERIMENTAL
    fprintf(stderr, "EXPERIMENTAL FEATURES ENABLED\n");
    fprintf(stderr, "EXPERIMENTAL FEATURES ENABLED\n");
    fprintf(stderr, "EXPERIMENTAL FEATURES ENABLED\n");
    icm_join(&state);
#else
    while( 1 )
    {
        fillbuffer(&state, buffer, ___icm_BLOCKSIZE, cfg_first);
        flush_buffer_bin( buffer, ___icm_BLOCK_BYTES );

        if( cfg_debug )
        {
            fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            for( int x = 0; x < NUM_THREADS; x++)
            {
                fprintf(stderr, "%016llx", state.nodes[x]);
            }
        }
    }
#endif

    icm_stop(&state);
    free(buffer);

    return 0;
}