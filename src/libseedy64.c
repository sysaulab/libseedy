#include "libseedy64.h"

uint64_t read_state_64(SEEDY64* state)
{
    return state->nodes[0] ^ state->nodes[1] ^ state->nodes[2];
}

int seed_modify_64( volatile uint64_t* in, volatile uint64_t* out )
{
    uint64_t acc = 1;                                   
    /* 
     *   newvalue : 128 unique prime numbers will be multiplied together in this accumulator
     *   The original hash function used a set of smallest prime numbers. (2, 3, 5, ...)
     *   Replacing by a personal selection of prime numbers found on the internet improved the quality of the stream.
     *   I think this is mostly because it forces to changes more bits at every addition in the collision multiplier.
     *   No other sets has been tested by the original author. Please modify this comment if you do.
     */
    uint64_t primes[] = 
        #include "primes64.inc"
    ;

    int x = 0;
    while( x < 8 * 8 ) {
        *in = (*in << 7) | (*in >> ((8 * 8) - 7));           /* source is rotated by 13 bits... now 7 to accomodate potential 8 bit fundamentals */
        acc = (acc << x) | (acc >> ((8 * 8) - x));           /* smooth out bit distribution in acc */
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

void* seed_thread_main_64(void* raw)
{
    seed_thread* state = (seed_thread*) raw;
    while(1)
    {
        seed_modify_64(state->source, state->sink);
        if(state->run == 0)
        {
            return NULL;
        }
    }
    return NULL;
}

void start_seeder_64(SEEDY64* state)
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
        state->threads[0].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_64, &(state->threads[0]), (DWORD)0, NULL);
        state->threads[1].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_64, &(state->threads[1]), (DWORD)0, NULL);
        state->threads[2].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_64, &(state->threads[2]), (DWORD)0, NULL);
    #else
        pthread_create(&(state->threads[0].thr), NULL, &seed_thread_main_64, &(state->threads[0]));
        pthread_create(&(state->threads[1].thr), NULL, &seed_thread_main_64, &(state->threads[1]));
        pthread_create(&(state->threads[2].thr), NULL, &seed_thread_main_64, &(state->threads[2]));
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

void stop_seeder_64(SEEDY64* state)
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



void seedy64(uint8_t* buffer, size_t bytes)
{
    size_t i = 0;
    int j = 0;
    uint64_t last_pick;
    uint64_t next_pick;
    clock_t old_report;
    clock_t new_report;
    SEEDY64 state;
    size_t blocks; 
    size_t partial; 
    blocks = bytes / 8;
    partial = bytes % 8;

    start_seeder_64(&state);
    wait_ms(SEEDY_INIT_MS_);

    last_pick = read_state_64(&state);
    old_report = NOW();

    while( i < (8 * blocks) )
    {
        wait_us(SEEDY_INTERVAL_US_);
        next_pick = read_state_64(&state);
        if(next_pick != last_pick)
        {
            *(uint64_t*)&(buffer[i]) = next_pick;
            last_pick = next_pick;
            i = i + (int)8;
        }
        new_report = NOW();
        if(new_report > old_report)
        {
            old_report = new_report;
            fprintf(stderr, "\b\b\b\b\b\b%zu %% ", (i*100)/((8 * blocks)));
        }
    }

    while( i < bytes )
    {
        wait_us(SEEDY_INTERVAL_US_);
        next_pick = read_state_64(&state);
        if(next_pick != last_pick)
        {
            buffer[i++] = ((uint8_t*)&next_pick)[j++];
            last_pick = next_pick;
        }
    }

    stop_seeder_64(&state);
}












