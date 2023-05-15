/******************************************************************************
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |  Covered by agpl-v3                                                        |
 |  Commercial licence available upon request                                 |
 |                                                                            |
 ******************************************************************************/

#ifndef ___libicm_h
#define ___libicm_h

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 3

typedef struct
{
    uint64_t        *source;
    uint64_t        *sink;
    int             pause;
    int             run;
    pthread_mutex_t mutx;
} icm_thread_t;

typedef struct
{
    uint64_t        nodes[NUM_THREADS];
    struct timespec delay;
    struct timespec rem;
    icm_thread_t    threads[NUM_THREADS];
} icm_state_t;

int ___libicm_modify(uint64_t* source, uint64_t* sink);
void* ___libicm_threadwork(void* raw);
void icm_init(icm_state_t* state);
void icm_get(icm_state_t* state, uint64_t* destination, uint64_t count, uint64_t debug);
void icm_stop(icm_state_t* icm);
void icm_go(icm_state_t* icm);

#endif