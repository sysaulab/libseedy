#include "seedy.h"

uint16_t read_state_16(SEEDY16* state)
{
    return state->nodes[0] ^ state->nodes[1] ^ state->nodes[2];
}

int seed_modify_16( volatile uint16_t* in, volatile uint16_t* out )
{
    uint16_t acc = 1;
    uint16_t primes[] = 
#include "primes16.inc"
    ;

    int x = 0;
    while( x < 2 * 8 ) {
        *in = (*in << 7) | (*in >> ((2 * 8) - 3));
        acc = (acc << x) | (acc >> ((2 * 8) - x));
        acc *= primes[(2 * x) + (1 & *in)];
        *out += acc ^ *in;
        x++;
    }
    *out ^= acc;
    return 0;
}

void* seed_thread_main_16(void* raw)
{
    seed_thread_16* state = (seed_thread_16*) raw;
    while(1)
    {
        seed_modify_16(state->source, state->sink);
        if(state->run == 0)
        {
            return NULL;
        }
    }
    return NULL;
}

void start_seeder_16(SEEDY16* state)
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
        state->threads[0].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_16, &(state->threads[0]), (DWORD)0, NULL);
        state->threads[1].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_16, &(state->threads[1]), (DWORD)0, NULL);
        state->threads[2].thr = CreateThread((LPSECURITY_ATTRIBUTES)NULL, 0, (LPTHREAD_START_ROUTINE)&seed_thread_main_16, &(state->threads[2]), (DWORD)0, NULL);
    #else
        pthread_create(&(state->threads[0].thr), NULL, &seed_thread_main_16, &(state->threads[0]));
        pthread_create(&(state->threads[1].thr), NULL, &seed_thread_main_16, &(state->threads[1]));
        pthread_create(&(state->threads[2].thr), NULL, &seed_thread_main_16, &(state->threads[2]));
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

void stop_seeder_16(SEEDY16* state)
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

void seedy16(uint8_t* buffer, size_t bytes)
{
    size_t i = 0;
    int j = 0;
    uint16_t last_pick;
    uint16_t next_pick;
    clock_t old_report;
    clock_t new_report;
    SEEDY16 state;
    size_t blocks; 
    size_t partial; 
    blocks = bytes / 2;
    partial = bytes % 2;

    start_seeder_16(&state);
    wait_ms(SEEDY_INIT_MS_);

    last_pick = read_state_16(&state);
    old_report = NOW();

    while( i < (2 * blocks) )
    {
        wait_us(SEEDY_INTERVAL_US_);
        next_pick = read_state_16(&state);
        if(next_pick != last_pick)
        {
            *(uint16_t*)&(buffer[i]) = next_pick;
            last_pick = next_pick;
            i = i + (int)2;
        }
        new_report = NOW();
        if(new_report > old_report)
        {
            old_report = new_report;
            fprintf(stderr, "\b\b\b\b\b\b%zu %% ", (i*100)/((2 * blocks)));
        }
    }

    while( i < bytes )
    {
        wait_us(SEEDY_INTERVAL_US_);
        next_pick = read_state_16(&state);
        if(next_pick != last_pick)
        {
            buffer[i++] = ((uint8_t*)&next_pick)[j++];
            last_pick = next_pick;
        }
    }

    stop_seeder_16(&state);
}












