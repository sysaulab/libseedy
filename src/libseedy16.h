#ifndef __SEEDY16_H_
#define __SEEDY16_H_

#include "common.h"

typedef struct seed_thread16_s
{
    volatile uint16_t *source;
    volatile uint16_t *sink;
    int run;
#   if defined(_WIN32)
        HANDLE thr;
#   else
        pthread_t thr;
#   endif
}
seed_thread_16;

typedef struct seed_state16_s
{
    volatile uint16_t nodes[3];
    seed_thread_16 threads[3];
}
SEEDY16;

uint16_t read_state_16(SEEDY16* state);
void start_seeder_16(SEEDY16* state);
void stop_seeder_16(SEEDY16* state);
void seedy16(uint8_t* buffer, size_t bytes);

#endif

