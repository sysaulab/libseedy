/*----------------------------------------------------------------------------*\
 |                                                                            |
 |  icm.c                                                                    |
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |                                                                            |
 |  Institutional licence available upon request                              |
 |                                                                            |
\*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>

#include "libicm.h"

#define ___icm_BLOCKSIZE (4096) // size of uint64_t array, size in mb is 8 times greater
#define ___icm_BLOCK_BYTES (___icm_BLOCKSIZE * sizeof(uint64_t))

void flush_buffer_hex(uint64_t* buffer, uint64_t bytes)
{
    int x = 0;
    int whole_int = bytes / 8;
    int partial_int = bytes % 8;

    while( x < whole_int )
        printf("%016llx", buffer[x++]);

    switch (partial_int)
    {
        case 1:
            printf("%02llx", 0xff & buffer[x++]);
            break;

        case 2:
            printf("%04llx", 0xffff & buffer[x++]);
            break;

        case 3:
            printf("%06llx", 0xffffff & buffer[x++]);
            break;

        case 4:
            printf("%08llx", 0xffffffff & buffer[x++]);
            break;

        case 5:
            printf("%010llx", 0xffffffffff & buffer[x++]);
            break;

        case 6:
            printf("%012llx", 0xffffffffffff & buffer[x++]);
            break;

        case 7:
            printf("%014llx", 0xffffffffffffff & buffer[x++]);
            break;
    }
}

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
    uint64_t cfg_forever = 1;
    uint64_t modifier = 1;
    uint64_t current_arg = argc - 1;
    double   cfg_count = 0;
    uint64_t requested_bytes = 1;
    uint32_t cfg_seed = 0;
    uint32_t cfg_hex = 0;
    uint32_t cfg_delay = 10;
    uint32_t cfg_info = 0;
    uint32_t cfg_first = 0;
    uint32_t cfg_void = 0;
    uint64_t* buffer;
    
    buffer = (uint64_t*) calloc(___icm_BLOCKSIZE, sizeof(uint64_t));

    if( buffer == NULL )
    {
        fprintf(stderr, "icm - ERROR - cannot allocate memory buffer");
        return 1;
    } 

    int x = 1;
    while( x < argc )
    {
        if(!strcmp("void", argv[x]) || !strcmp("v", argv[x])){
            cfg_void = 1;
        }
        else if(!strcmp("info", argv[x]) || !strcmp("i", argv[x])){
            cfg_info = 1;
        }
        else if(!strcmp("first", argv[x]) || !strcmp("f", argv[x])){
            cfg_first = 1;
        }
        else if(!strcmp("hex", argv[x]) || !strcmp("h", argv[x])){
            cfg_hex = 1;
        }
        else if(!strcmp("kb", argv[x]) || !strcmp("k", argv[x])){
            if(requested_bytes == 0)
                requested_bytes = 1000;
            else
                requested_bytes *= 1000;
        }
        else if(!strcmp("mb", argv[x]) || !strcmp("m", argv[x])){
            if(requested_bytes == 0)
                requested_bytes = 1000000;
            else
                requested_bytes *= 1000000;
        }
        else if(!strcmp("gb", argv[x]) || !strcmp("g", argv[x])){
            if(requested_bytes == 0)
                requested_bytes = 1000000000;
            else
                requested_bytes *= 1000000000;
        }
        else if(!strcmp("tb", argv[x]) || !strcmp("t", argv[x])){
            if(requested_bytes == 0)
                requested_bytes = 1000000000000;
            else
                requested_bytes *= 1000000000000;
        }
        else{
            cfg_count = atof(argv[x]);
            if(cfg_count > 0)
                cfg_forever = 0;
        }
        x++;
    }

    modifier = requested_bytes;
    requested_bytes = (uint64_t)(cfg_count * (double)requested_bytes);

    uint64_t block_count = requested_bytes / ___icm_BLOCK_BYTES;
    uint64_t remains = requested_bytes % ___icm_BLOCK_BYTES;


    if(cfg_info){
        fprintf(stderr, "=== icm === INFORMATION SHEET === BEGIN =======================================\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "output format : %s\n", cfg_void ? "void" : cfg_hex ? "ascii hex" : "binary");
        fprintf(stderr, "final size : %llu bytes (%f * %llu)\n", requested_bytes, cfg_count, modifier);
        fprintf(stderr, "infinite : %s\n", cfg_forever ? "yes" : "no");

        fprintf(stderr, "block size = %lu bytes\n", ___icm_BLOCK_BYTES);
        fprintf(stderr, "work required : %llu block + %llu bytes\n", block_count, remains);
        fprintf(stderr, "first draw only : %s\n", cfg_first ? "yes" : "no");

        fprintf(stderr, "\n");
    }

    icm_state_t state;
    icm_init(&state);


    if(cfg_info){
        fprintf(stderr, "--- icm --- INITIAL STATE -----------------------------------------------------\n");
        fprintf(stderr, "\n");
        fprintf(stderr, "delay: %ld\n", state.delay.tv_nsec);
        x = 0;
        while( x < NUM_THREADS )
        {
            fprintf( stderr, "node #%i %016llx\n", x, state.nodes[x] );
            x++;
        }
        fprintf(stderr, "\n");
        fprintf(stderr, "--- icm --- WORK IN PROGRESS --------------------------------------------------\n");
    }

    x = 0;

    while( cfg_forever || x < block_count )
    {
        fillbuffer(&state, buffer, ___icm_BLOCKSIZE, cfg_first);

        if(cfg_void == 0)
        {
            if( cfg_hex )
                flush_buffer_hex( buffer, ___icm_BLOCK_BYTES );
            else
                flush_buffer_bin( buffer, ___icm_BLOCK_BYTES );
        }

        if( cfg_info )
            fprintf(stderr, "·");

        x++;
    }

    if( remains > 0 )
    {
        uint64_t needed = remains / 8;

        if( remains % 8 > 0 )
            needed++;

        fillbuffer(&state, buffer, needed, cfg_first);

        if(cfg_void == 0)
        {
            if( cfg_hex )
                flush_buffer_hex(buffer, remains);

            else
                flush_buffer_bin(buffer, remains);
        }

        if( cfg_info )
            fprintf(stderr, ".");
    }

    if( cfg_info )
        fprintf(stderr, "\n");

    if(cfg_info)
        fprintf(stderr, "\n=== icm === INFORMATION SHEET === END =========================================\n");

    free(buffer);

    return 0;
}