#ifndef LIBSEEDY_nm64_H_
#define LIBSEEDY_nm64_H_

#include "../../libseedy/common.h"
#include "../../libseedy/libseedy64.h"
#include "prime64sub16.h"
#include "cc2032.h"

#include <stdio.h>

/*
 *   NM64 (NoiseMap, 64 bits addressable)
 */

typedef struct NM64_s 
{
    uint64_t seek_pos;
    uint64_t noise[4][65536];
    uint64_t iter;
}
NM64;

void nm64_init(NM64* s)
{
    CC2032 seed_gen;
    cc2032_init( &seed_gen, (void*)seedy64 );
    cc2032_fill( &seed_gen, (uint8_t*)&s->noise, sizeof(s->noise) );
    s->iter = prime64sub16();
    s->seek_pos = 0;
}

void nm64_set_seek64(NM64* s, uint64_t b)
{
    s->seek_pos = b;
}

uint64_t nm64_block(NM64* s, uint64_t i)
{
    uint64_t pos64;
    uint16_t* pos16 = (uint16_t*)&pos64;
    pos64 = i * s->iter;

    return  ( s->noise[0][pos16[0]] ) ^
            ( s->noise[1][pos16[1]] ) ^
            ( s->noise[2][pos16[2]] ) ^
            ( s->noise[3][pos16[3]] ) ;
}

void nm64_fill(NM64* s, uint8_t* b, size_t n)
{
    size_t t = 0;
    uint64_t next = 0;

    next = nm64_block(s, s->seek_pos >> 3);
    while( (s->seek_pos % 8) && (t < n) )
    {
        b[t] = ((uint8_t*)&next)[s->seek_pos % 8];
        t++;
        s->seek_pos++;
    }

    while( (n - t) >> 3 )
    {
        *(uint64_t*)(&b[t]) = nm64_block(s, s->seek_pos >> 3);
        t = t + 8;
        s->seek_pos = s->seek_pos + 8;
    }

    next = nm64_block(s, s->seek_pos >> 3);
    while( n - t )
    {
        b[t] = ((uint8_t*)&next)[s->seek_pos % 8];
        t++;
        s->seek_pos++;
    }
}

#endif
