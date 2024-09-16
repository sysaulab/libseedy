/*
 *   32 bit specific header.
 */
#ifndef __SEEDY32_H_
#define __SEEDY32_H_



/*
 *   SEEDY32
 */
 
 
 

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
#define rot32(n,i) ((n<<(i%32))|(n>>(32-(i%32))))

/*
 *   Return the next prime number, if the starting point is prime, it is retuned.
 */
 
uint32_t prime32(uint32_t start);



/*
 *   QX32 (Quad XOR, 32 bit port)
 */

typedef struct QX32_s
{
    void* feeder;
    uint32_t step;
    uint32_t iter;
    uint32_t pool[3][65536];
} QX32;
void qx32_init(QX32* q, void* f);
void qx32_fill(QX32* q, uint8_t* b, size_t n);
uint32_t qx32_next(QX32* q);
uint32_t qx32_at(QX32* q, uint32_t i);



/*
 *   MT32 (Mersenne Twister - 32 Bits)
 */
#include "mtwister/mtwister.h"
typedef struct MT32_s {
    void* feeder;
    uint32_t pool;
    MTRand gen_mt32;
    size_t index;
} MT32;

void mt32_init(MT32* rand, void* f);
uint32_t mt32_next(MT32* rand);
void mt32_fill(MT32* rand, uint8_t* b, size_t n);





#endif