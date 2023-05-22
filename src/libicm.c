/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#ifndef ___libicm_c
#define ___libicm_c

#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "libicm.h"
#include "config.h"

int ___libicm_modify( uint64_t* in, uint64_t* out )
{
    if ( in == NULL || out == NULL )
        return 1;

    uint64_t acc = 1;                                   /* newvalue : 128 unique prime numbers will be multiplied together in this accumulator */
    uint64_t primes[128] =                              /* primes : list of prime numbers, 256 first prime numbers. */
    #include "primes.large.txt"

    for( int x = 0; x < 64; x++ )
    {
        *in = ( *in << 13 ) + ( *in >> ( 64 - 13 ) );   /* source is rotated by 13 bits... */
//        acc = ( acc << x ) + ( acc >> ( 64 - x ) );     /* smooth out bit distribution in acc */
        acc *= primes[ (2 * x) + (1 & *in) ];           /* accumulate a unique prime for this run... */
        *out += acc;                                    /* add uncertainty in the sink too as we go... */
    }
    *out ^= acc;                                        /* finally XOR the accumulated product to the sink */
    return 0;
}

void icm_stop(icm_state_t* icm)
{
    for( int x = 0; x < _SSRNG_THREADS; x++ )
    {
        pthread_mutex_lock(&(icm->threads[x].mutx));
        icm->threads[x].pause = 1;
    }
}

void icm_go(icm_state_t* icm)
{
    for( int x = 0; x < _SSRNG_THREADS; x++ )
    {
        pthread_mutex_unlock(&(icm->threads[x].mutx));
    }
}

void icm_join(icm_state_t* icm)
{
    for( int x = 0; x < _SSRNG_THREADS; x++ )
    {
        pthread_join(icm->threads[x].thr, NULL);
    }
}

void* ___libicm_threadwork(void* raw)
{
    icm_thread_t* state = (icm_thread_t*) raw;

    int is_last_node = state->source > state->sink;

    while(state->run)
    {
        ___libicm_modify(state->source, state->sink);
        if(state->pause)
        {
            state->pause = 0;
            pthread_mutex_lock(&state->mutx); //waiting for restart
            pthread_mutex_unlock(&state->mutx); //unlocking the mutex right away...
        }
#ifdef ICM_EXPERIMENTAL
        state->fresh++;
        if(is_last_node)
        {
            uint64_t val = 0;
            int ok = 1;

            for ( int x = 0; x < _SSRNG_THREADS; x++ )
                if ( _SSRNG_EXP_SKIP > *state->freshes[x] ) 
                    ok = 0;
            
            if(ok)
            {
            for ( int x = 0; x < _SSRNG_THREADS; x++ )
            {
                *state->freshes[x] = 0;
                val ^= state->nodes[x];
            }
            fwrite(&val, sizeof(uint64_t), 1, stdout);
            }

        }
#endif
    }
    return NULL;
}

void icm_init(icm_state_t* state)
{
#ifdef ICM_EXPERIMENTAL
    pthread_t threads[_SSRNG_THREADS];
#endif
    state->delay.tv_sec = 0;
    state->delay.tv_nsec = 5000;
    for( int i = 0; i < _SSRNG_THREADS; i++ )
    {
        state->nodes[i] = 0;
#ifdef ICM_EXPERIMENTAL
        for(int n = 0; n <_SSRNG_THREADS; n++)
        {
            state->threads[i].freshes[n] = &state->threads[n].fresh;
        }
        state->threads[i].fresh = 0;
        state->threads[i].nodes = state->nodes;
#endif
        state->threads[i].source = &(state->nodes[i]);
        state->threads[i].sink = &(state->nodes[(i + 1) % _SSRNG_THREADS]);
        state->threads[i].run = 1;
        state->threads[i].pause = 1;
        pthread_mutex_init(&(state->threads[i].mutx), NULL);
        pthread_mutex_lock(&(state->threads[i].mutx));
        pthread_create(&(state->threads[i].thr), NULL, &___libicm_threadwork, &(state->threads[i]));
    }
    icm_go(state);
    usleep(state->delay.tv_nsec);
    icm_stop(state);
}

void icm_get(icm_state_t* state, uint64_t* destination, uint64_t count)
{
    icm_go(state);
    uint64_t answer = 0;
    for( int i = 0; i < count; i++ )
    {
        nanosleep( &state->delay, &state->rem );
        for( int y = 0; y < _SSRNG_THREADS; y++ )
            answer ^= state->nodes[y];
        destination[i] = answer;
    }
    icm_stop(state);
}
#endif
