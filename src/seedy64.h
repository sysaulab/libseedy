/*
 *   64 bit specific header.
 */
#ifndef __SEEDY64_H_
#define __SEEDY64_H_



/*
 *   SEEDY64
 */



typedef struct seed_thread64_s{
    volatile uint64_t *source;
    volatile uint64_t *sink;
    int run;
#ifdef _WIN32
    HANDLE thr;
#else
    pthread_t thr;
#endif
} seed_thread;

typedef struct seed_state64_s{
    volatile uint64_t nodes[3];
    seed_thread threads[3];
} SEEDY64;

void start_seeder_64(SEEDY64* state);
void stop_seeder_64(SEEDY64* state);
uint64_t read_state_64(SEEDY64* state);
void seedy64(uint8_t* buffer, size_t bytes);
#define rot64(n,i) ((n<<(i%64))|(n>>(64-(i%64))))





/*
 *   Return the next prime number, if the starting point is prime, it is retuned.
 */

uint64_t prime64(uint64_t start);



/*
 *   QX64 (Quad XOR, 64 bits)
 */

typedef struct QX64_s 
{
    void* feeder;
    uint64_t step;
    uint64_t iter;
    uint64_t pool[4][65536];
} QX64;
void qx64_init(QX64* q, void* f);
uint64_t qx64_next(QX64* q);
uint64_t qx64_at(QX64* q, uint64_t i);



/*
 *   SS64 (SHISHUA - Fastest non-cryptographic PRNG. AVAILABLE ON MODERN COMPILERS ONLY
 *
 *   The fake_prng object serve to allow the definition to be here without having to link to shoshua.h header everywhere.
 *   MUST MATCH THE DEFINITION FOUND IN shishua/shishua.h.
 */
typedef struct fake_prng_state {
  uint64_t state[16];  
  uint64_t output[16]; 
  uint64_t counter[4]; 
} fake_prng_state;

typedef struct SS64_s {
    void* feeder;
    uint64_t pool[4];
    fake_prng_state shishua;
} SS64;
void ss64_init(SS64* s, void* f);
void ss64_fill(SS64* s, uint8_t* b, size_t n);







#endif