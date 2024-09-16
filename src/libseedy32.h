#ifndef __SEEDY32_H_
#define __SEEDY32_H_

#include "common.h"

typedef struct seed_thread32_s{
    volatile uint32_t *source;
    volatile uint32_t *sink;
    int run;
#ifdef _WIN32
    HANDLE thr;
#else
    pthread_t thr;
#endif
} seed_thread_32;

typedef struct seed_state32_s{
    volatile uint32_t nodes[3];
    seed_thread_32 threads[3];
} SEEDY32;

uint32_t read_state_32(SEEDY32* state);
void start_seeder_32(SEEDY32* state);
void stop_seeder_32(SEEDY32* state);

void seedy32(uint8_t* buffer, size_t bytes);

#endif