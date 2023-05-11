/******************************************************************************
 |                                                                            |
 |  libicm.c                                                                  |
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |                                                                            |
 |  Institutional licence available upon request                              |
 |                                                                            |
 ******************************************************************************/

#ifndef ___libicm_c
#define ___libicm_c

#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "libicm.h"

int ___libicm_modify( uint64_t* in, uint64_t* out )
{
    if ( in == NULL || out == NULL )
        return 1;

    uint64_t acc = 1;                                               /* newvalue : 128 unique prime numbers will be multiplied together in this accumulator */
    uint64_t primes[128] =                                               /* primes : list of prime numbers, 256 first prime numbers. */
    #include "primes.large.txt"

    for( int x = 0; x < 64; x++ )
    {
        acc = ( acc << 13 ) + ( acc >> ( 64 - 13 ) );
        acc *= primes[ (2 * x) + (1 & *in) ];                /* accumulate a unique prime for this run... */
        *out += acc;                                            /* add uncertainty in the sink too as we go... */
        *in = ( *in << 13 ) + ( *in >> ( 64 - 13 ) );     /* source is rotated by 13 bits... */
    }
    *out ^= acc;                                                   /* finally XOR the accumulated product to the sink */
    return 0;
}

void ___icm_pause(icm_state_t* icm)
{
    for( int x = 0; x < NUM_THREADS; x++ )
    {
        pthread_mutex_lock(&icm->threads[x].mutx);
        icm->threads[x].pause = 1;
    }
}

void ___icm_play(icm_state_t* icm){
    for( int x = 0; x < NUM_THREADS; x++ )
    {
        pthread_mutex_unlock(&icm->threads[x].mutx);
    }
}

void* ___libicm_threadwork(void* raw)
{
    icm_thread_t* state = (icm_thread_t*) raw;
    while(state->run)
    {
        ___libicm_modify(state->source, state->sink);
        if(state->pause)
        {
            state->pause = 0;
            pthread_mutex_lock(&state->mutx); //waiting for restart
            pthread_mutex_unlock(&state->mutx); //unlocking the mutex right away...
        }
    }
    return NULL;
}

void icm_init(icm_state_t* state)
{
    pthread_t threads[NUM_THREADS];
    state->delay.tv_sec = 0;
    state->delay.tv_nsec = 1;
    for( int index = 0; index < NUM_THREADS; index++ )
    {
        state->nodes[index] = 0;
        state->threads[index].source = &state->nodes[index];
        state->threads[index].sink = &state->nodes[(index + 1)%NUM_THREADS];
        state->threads[index].pause = 0;
        pthread_mutex_init(&state->threads[index].mutx, NULL);
        pthread_mutex_lock(&state->threads[index].mutx);
        pthread_create(&threads[index], NULL, &___libicm_threadwork, &state->threads[index]);
    }
}

void icm_get(icm_state_t* state, uint64_t* destination, uint64_t count, uint64_t debug)
{
    uint64_t answer = 0;
    ___icm_play(state);
    for( int index = 0; index < count; index++ )
    {
        nanosleep( &state->delay, &state->rem );
        answer = state->nodes[0];
        for( int y = 1; y < NUM_THREADS; y++ )
            answer ^= state->nodes[y];
        destination[index] = answer;
    }
    ___icm_pause(state);
}
#endif
