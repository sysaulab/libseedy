#ifndef LIBSEEDY_nm80_H_
#define LIBSEEDY_nm80_H_

#include "../../libseedy/common.h"
#include "../../libseedy/libseedy64.h"
#include "prime64sub16.h"
#include "cc2032.h"

#include <stdio.h>
#include <string.h>

/*
 *   NM80 (NoiseMap, 64 bits addressable)
 */

typedef struct NM80_s 
{
    uint64_t iters[8192];
    uint64_t noise[4][65536];
}
NM80;

void nm80_init(NM80* s)
{
    CC2032 seed_gen;
    cc2032_init( &seed_gen, (void*)seedy64 );
    cc2032_fill( &seed_gen, (uint8_t*)&s->noise, sizeof(s->noise) );
    /* Iterators must NULL to indicate its need for initialisation */
    memset(s->iters, 0, 8192 * sizeof(uint64_t));
}


uint64_t nm80_block(NM80* s, uint64_t i, uint16_t b)
{
    uint64_t pos64;
    uint16_t* pos16 = (uint16_t*) & pos64;
    uint16_t iter_bank = b >> 3;

    if ( s->iters[iter_bank] == 0 )
    {
//fprintf(stderr, "P");
        /* Lazy Initialisation, it takes some time making a unique iterator. Making them as needed avoid spending an hour finding prime numbers.*/
        s->iters[iter_bank] = prime64sub16();
    }

    if ( i == 0 )
    {
        /* To avoid having all index zero being the same, I redefine index zero as the binary not of the iterator.*/
        i = ~s->iters[b >> 3];
    }

    pos64 = i * s->iters[b >> 3];

    return  ( s->noise[0][pos16[0]] ) ^
            ( s->noise[1][pos16[1]] ) ^
            ( s->noise[2][pos16[2]] ) ^
            ( s->noise[3][pos16[3]] ) ;
}

void nm80_fill(NM80* s, uint8_t* buf, size_t num, uint16_t bank, uint64_t offset)
{
    size_t t = 0;
    uint64_t next = 0;

    next = nm80_block(s, bank << 13 | offset >> 3, bank);
    while( (offset % 8) && (t < num) )
    {
        buf[t] = ((uint8_t*)&next)[offset % 8];
        t++;
        offset++;
    }

    while( (num - t) >> 3 )
    {
        *(uint64_t*)(&buf[t]) = nm80_block(s, offset >> 3, bank);
        t = t + 8;
        offset = offset + 8;
    }

    next = nm80_block(s, offset >> 3, bank);
    while( num - t )
    {
        buf[t] = ((uint8_t*)&next)[offset % 8];
        t++;
        offset++;
    }
}

#endif
