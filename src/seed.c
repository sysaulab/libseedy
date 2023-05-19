/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libicm.h"
#include "config.h"

#ifndef _SSRNG_CORE
void fillbuffer(icm_state_t* state, uint64_t* buffer, uint64_t buffer_size, uint64_t first)
{
    if(!first)
    {
        icm_get(state, buffer, buffer_size);
    }
    else
    { 
        for( uint64_t y = 0; y < buffer_size; y++ )
        {
            for( int x = 0; x < _SSRNG_THREADS; x++ )
            {
                state->nodes[x] = 0;
            }
            icm_get(state, buffer + y, 1);
        }
    }
}
#endif

int main(int argc, const char * argv[])
{
    uint64_t buffer[_SSRNG_BUFLEN];

#ifndef _SSRNG_CORE
    uint32_t cfg_debug = 0;
    uint32_t cfg_reset = 0;
    for( int x = 1; x < argc; x++ )
    {
        if(!strcmp("debug", argv[x]))
            cfg_debug = 1;
        else if(!strcmp("reset", argv[x]))
            cfg_reset = 1;
        else
        {
            printf("Distributed under Affero GNU Public Licence version 3\n\n");
            printf("usage : seed [debug reset] | output\n");
            return EXIT_FAILURE;
        }
    }
#endif
    icm_state_t state;
    icm_init(&state);
    icm_go(&state);

#ifdef _SSRNG_EXPERIMENTAL //Enable in libicm.h. Output from library, thread function.
    fprintf(stderr, "!!! WARNING !!! Experimental features in use\n");
    icm_join(&state);
#else
    while( 1 )
    {
#ifndef _SSRNG_CORE
        fillbuffer(&state, buffer, _SSRNG_BUFLEN, cfg_reset);
#else
        icm_get(&state, buffer, _SSRNG_BUFLEN, 0);
#endif
        
#ifndef _SSRNG_CORE
        if( cfg_debug )
        {
            for( int x = 0; x < _SSRNG_THREADS; x++)
                fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
            for( int x = 0; x < _SSRNG_THREADS; x++)
                fprintf(stderr, "%016llx", state.nodes[x]);
        }
#endif
    }
#endif

    icm_stop(&state);
    return EXIT_SUCCESS;
}