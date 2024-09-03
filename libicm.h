#include <stdint.h>
#include <pthread.h>

typedef struct{
    volatile uint64_t *source;
    volatile uint64_t *sink;
    int run;
    pthread_mutex_t mutx;
    pthread_t thr;
} icm_thread;

typedef struct{
    volatile uint64_t nodes[3];
    icm_thread threads[3];
} icm_state_t;

void icm(icm_state_t* state, uint64_t* buffer, uint64_t count);
