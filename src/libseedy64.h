/*
 *   64 bit specific header.
 */
#ifndef __SEEDY64_H_
#define __SEEDY64_H_

#include "common.h"

/*
 *   SEEDY64
 */

typedef struct seed_thread64_s
{
    volatile uint64_t *source;
    volatile uint64_t *sink;
    int run;
#   ifdef _WIN32
        HANDLE thr;
#   else
        pthread_t thr;
#   endif
}
seed_thread;

typedef struct seed_state64_s
{
    volatile uint64_t nodes[3];
    seed_thread threads[3];
}
SEEDY64;

void start_seeder_64(SEEDY64* state);
void stop_seeder_64(SEEDY64* state);
uint64_t read_state_64(SEEDY64* state);
void seedy64(uint8_t* buffer, size_t bytes);

#endif