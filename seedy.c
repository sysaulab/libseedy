#include "seedy.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct seed_thread_s{
    volatile seedy_t *source;
    volatile seedy_t *sink;
    int run;
#ifdef _WIN32
    HANDLE thr;
#else
    pthread_t thr;
#endif
} seed_thread;

typedef struct seed_state_s{
    volatile seedy_t nodes[3];
    seed_thread threads[3];
} seed_state;

seedy_t read_state(seed_state* state)
{
    return state->nodes[0] ^ state->nodes[1] ^ state->nodes[2];
}

int seed_modify( volatile seedy_t* in, volatile seedy_t* out )
{
    seedy_t acc = 1;                                   
    /* 
     *   newvalue : 128 unique prime numbers will be multiplied together in this accumulator
     *   The original hash function used a set of smallest prime numbers. (2, 3, 5, ...)
     *   Replacing by a personal selection of prime numbers found on the internet improved the quality of the stream.
     *   I think this is mostly because it forces to changes more bits at every addition in the collision multiplier.
     *   No other sets has been tested by the original author. Please modify this comment if you do.
     */
    seedy_t primes[] = 
    #if (SEEDY_WIDTH > 4)
        #include "primes64.inc"

    #elif (SEEDY_WIDTH > 2)
        #include "primes32.inc"

    #elif (SEEDY_WIDTH > 1)
        #include "primes16.inc"

    #else
        #include "primes8.inc"
        
    #endif
    ;

    int x = 0;
    while( x < SEEDY_WIDTH * 8 ) {
        *in = (*in << 7) | (*in >> ((SEEDY_WIDTH * 8) - 7));           /* source is rotated by 13 bits... now 7 to accomodate potential 8 bit fundamentals */
        acc = (acc << x) | (acc >> ((SEEDY_WIDTH * 8) - x));           /* smooth out bit distribution in acc */
        acc *= primes[(2 * x) + (1 & *in)];                            /* pick the prime[x][set?], do it 64 times (we rotate above) */

        /* 
         *   This addition increase the number of unresolved race conditions / computation cycle.
         *   Before this addition I had to wait and verify for a change in the state to avoid polling too soon.
         *   This addition instruction allowed continuous polling during most of the development and testing phases.
         *   Basic dafeguard has been reintroduced for production before the last round of testing on the iBook G4.
         */
        *out += acc ^ *in;
        x++;
    }
    *out ^= acc;
    return 0;
}

void* seed_thread_main(void* raw)
{
    seed_thread* state = (seed_thread*) raw;
    while(1)
    {
        seed_modify(state->source, state->sink);
        if(state->run == 0)
        {
            return NULL;
        }
    }
    return NULL;
}

void start_seeder(seed_state* state)
{
    int i = 0;
    state->nodes[0] = 0;
    state->nodes[1] = 0;
    state->nodes[2] = 0;
    state->threads[0].source = &(state->nodes[0]);
    state->threads[1].source = &(state->nodes[1]);
    state->threads[2].source = &(state->nodes[2]);
    state->threads[0].sink = &(state->nodes[1]);
    state->threads[1].sink = &(state->nodes[2]);
    state->threads[2].sink = &(state->nodes[0]);
    state->threads[0].run = 1;
    state->threads[1].run = 1;
    state->threads[2].run = 1;
#ifdef _WIN32
    state->threads[0].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main, &(state->threads[0]), (DWORD)0, NULL);
    state->threads[1].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main, &(state->threads[1]), (DWORD)0, NULL);
    state->threads[2].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main, &(state->threads[2]), (DWORD)0, NULL);
#else
    pthread_create(&(state->threads[0].thr), NULL, &seed_thread_main, &(state->threads[0]));
    pthread_create(&(state->threads[1].thr), NULL, &seed_thread_main, &(state->threads[1]));
    pthread_create(&(state->threads[2].thr), NULL, &seed_thread_main, &(state->threads[2]));
#endif

    while( i < 3 )
    {
        state->nodes[i] = 0;
        state->threads[i].source = &(state->nodes[i]);
        state->threads[i].sink = &(state->nodes[(i + 1) % 3]);
        state->threads[i].run = 1;
        i++;
    }
}

void stop_seeder(seed_state* state)
{
    state->threads[0].run = 0;
    state->threads[1].run = 0;
    state->threads[2].run = 0;
#ifdef _WIN32
    WaitForSingleObject(state->threads[0].thr, 2000000000);
    WaitForSingleObject(state->threads[1].thr, 2000000000);
    WaitForSingleObject(state->threads[2].thr, 2000000000);
#else
    pthread_join(state->threads[0].thr, NULL);
    pthread_join(state->threads[1].thr, NULL);
    pthread_join(state->threads[2].thr, NULL);
#endif
}

/*
 *   AJUST FOR PORTS...
 */
void* parseinputgen(int argc, char** argv)
{
    if(argc > 1)
    {
    #ifdef _WIN32

        TODO Windows Crypto here BREAK BUIULD ON PURPOSE

    #else

        if(strcmp(argv[1], "arc4") == 0)
            return arc4random;

    #endif

        if(strcmp(argv[1], "stdin") == 0)
            return stdinput;
        
        if(strcmp(argv[1], "seedy") == 0)
            return seedy;

    }

    return seedy;
}

void stdinput(uint8_t* buffer, size_t bytes)
{
    fread(buffer, bytes, sizeof(uint8_t), stdin);
}

void seedy(uint8_t* buffer, size_t bytes)
{
    size_t i = 0;
    int j = 0;
    seedy_t last_pick;
    seedy_t next_pick;
    seed_state state;
    size_t blocks; 
    size_t partial; 
    blocks = bytes / SEEDY_WIDTH;
    partial = bytes % SEEDY_WIDTH;

    start_seeder(&state);
    last_pick = read_state(&state);

    while( i < (SEEDY_WIDTH * blocks) )
    {
        wait_us(500);
        next_pick = read_state(&state);
        if(next_pick != last_pick)
        {
            *(seedy_t*)&(buffer[i]) = next_pick;
            last_pick = next_pick;
            i = i + (int)SEEDY_WIDTH;
        }
    }

    while( i < bytes )
    {
        wait_us(500);
        next_pick = read_state(&state);
        if(next_pick != last_pick)
        {
            buffer[i++] = ((uint8_t*)&next_pick)[j++];
            last_pick = next_pick;
        }
    }

    stop_seeder(&state);
}












