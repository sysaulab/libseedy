/******************************************************************************
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |  Distributed under Affero GNU Public Licence version 3                     |
 |  Commercial licence available upon request                                 |
 |                                                                            |
 ******************************************************************************/

#ifndef ___icmstat_h
#define ___icmstat_h

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define      U8_AVG_TARGET (0xff/2.0)
#define     U16_AVG_TARGET (0xffff/2.0)
#define     U32_AVG_TARGET (0xffffffff/2.0)
#define     U64_AVG_TARGET (0xffffffffffffffff/2.0)

#define      U8_MAX_VALUE (0xff)
#define     U16_MAX_VALUE (0xffff)
#define     U32_MAX_VALUE (0xffffffff)
#define     U64_MAX_VALUE (0xffffffffffffffff)

typedef struct {
    uint64_t count;
    uint64_t unique;
    uint64_t unique_diff;
    uint64_t value[0x100];
    uint8_t last_value;
    uint64_t diff[0x100];
    double score_uniqueness;
    double score_uniqueness_diff;
} distribution_8_t;

typedef struct {
    uint8_t bits_cache[0x10000];
    uint8_t bits_cache_ready;
    uint64_t count;
    uint64_t unique;
    uint64_t data[0x100000000/64];
    double score_unique;
} distribution_32_t;

typedef struct {
    distribution_8_t u8_A;
    distribution_32_t u32_A;
    FILE* logfile;
    double start;
    double stage_start;
} distribution;


typedef struct {
    double min;
    double sum;
    double max;
    uint64_t count;
} fstat_t;

void fstat_init(fstat_t* fstat);
void fstat_analyze(fstat_t* fstat, double value);
double fstat_min(fstat_t* fstat);
double fstat_max(fstat_t* fstat);
double fstat_avg(fstat_t* fstat);


void distribution_init( distribution* dist );
double deviation(double a, double b);
double update_score(distribution* dist);
void add_u64(distribution* dist, uint64_t value);
void init_bits(uint8_t* bits);

#endif