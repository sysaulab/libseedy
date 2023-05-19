/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#ifndef ___libstat_h
#define ___libstat_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stat.h"

void fstat_init(fstat_t* fstat)
{
    fstat->min = 0xffffffffffffffff;
    fstat->sum = 0;
    fstat->max = 0;
}

void fstat_analyze(fstat_t* fstat, double value)
{
    if( value < fstat->min )
        fstat->min = value;

    if( value > fstat->max )
        fstat->max = value;

    fstat->sum += value;
    fstat->count++;
}

double fstat_min(fstat_t* fstat)
{
    return fstat->min;
}

double fstat_max(fstat_t* fstat)
{
    return fstat->max;
}

double fstat_avg(fstat_t* fstat)
{
    return fstat->sum / fstat->count;
}

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

void add_u32 (distribution_32_t* stat32, uint32_t value)
{
    uint64_t prefix = value / 64;
    uint64_t suffix = value % 64;
    stat32->data[prefix] |= (1LL << suffix);
    stat32->count++;
}

void add_u64( distribution* dist, uint64_t value )
{
    uint32_t* sub = (uint32_t*)&value;
    add_u32(&dist->u32_A, sub[0]);
    add_u32(&dist->u32_A, sub[1]);
}

#endif