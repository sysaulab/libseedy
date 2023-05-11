/*----------------------------------------------------------------------------*\
 |                                                                            |
 |  libstat.c                                                                 |
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |                                                                            |
 |  Institutional licence available upon request                              |
 |                                                                            |
\*----------------------------------------------------------------------------*/

#ifndef ___libstat_h
#define ___libstat_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stat.h"

uint64_t count_bits_fast(uint16_t value, uint8_t* bits)
{
    return bits[value];
}

uint8_t count_bits_slow(uint16_t value)
{
    uint8_t answer = 0;
    while( value != 0 )
    {
        if( value & 1 )
            answer++;
        value = value >> 1;
    }
    return answer;
}

void init_bits(uint8_t* bits)
{
    int bit = 0;
    while ( bit < 0x10000 )
    {
        bits[bit] = count_bits_slow(bit);
        bit++;
    }
}

void u8_update_unique(distribution_8_t* dist)
{
    fprintf(stderr, "u8_update_unique(%p)", dist);
    dist->unique = 0;
    dist->unique_diff = 0;
    dist->count = 0;
    uint64_t x = 0;
    while( x < 0x100 )
    {
        if ( dist->value[x] > 0 )
            dist->unique++;

        if ( dist->diff[x] > 0 )
            dist->unique_diff++;

        dist->count += dist->value[x];
        x++;
    }
    fprintf(stderr, "u8_update_unique() -> %llu %llu, ", dist->unique, dist->unique_diff);
}

void count_u32_bits( distribution_32_t* dist )
{
    uint8_t* cache = dist->bits_cache;
    if(dist->bits_cache_ready == 0)
    {
        init_bits(cache);
    }

    uint64_t unique = 0;
    uint64_t segment = 0;
    while( segment < 0x100000000/64 )
    {
        uint16_t* value = (uint16_t*)&dist->data[segment];
        unique += (cache[value[0]] + cache[value[1]]) + (cache[value[2]] + cache[value[3]]);
        segment++;
    }
    dist->unique = unique;
}

double deviation( double a, double b )
{
    if (a < b) return a / b;
    return b / a;
}

void u8_update_score(distribution_8_t* dist)
{
    double target_dist = dist->count;
    if ( target_dist > U8_MAX_VALUE+1L ) 
        target_dist = U8_MAX_VALUE+1L;

    u8_update_unique(dist);
    dist->score_uniqueness = deviation(target_dist, dist->unique);
    dist->score_uniqueness_diff = deviation(target_dist, dist->unique_diff);
}

void u32_update_score(distribution_32_t* dist)
{
    double target_unique = dist->count;

    if ( target_unique > U32_MAX_VALUE+1L ) 
        target_unique = U32_MAX_VALUE+1L;
        
    count_u32_bits(dist);
    dist->score_unique = deviation(target_unique, dist->unique);
}

double update_score(distribution* dist)
{
    double score = 1.0;

    u32_update_score(&dist->u32_A);
    score *= dist->u32_A.score_unique;

    return score;
}

uint8_t difference(uint8_t a, uint8_t b)
{
    if ( a < b )
        return b - a;
    else
        return a - b;
}

void add_u32 (distribution_32_t* stat32, uint32_t value)
{
    uint64_t prefix = value / 64;
    uint64_t suffix = value % 64;
    stat32->data[prefix] |= (1LL << suffix);
    stat32->count++;
}

void add_u64( distribution* dist, uint64_t value )
{
    add_u32(&dist->u32_A, (uint32_t)(value));
    add_u32(&dist->u32_A, (uint32_t)(value >> 32));
}

#endif