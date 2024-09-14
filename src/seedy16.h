/*
 *   16 bit specific header.
 */
#ifndef __SEEDY16_H_
#define __SEEDY16_H_



/*
 *   SEEDY16.
 */
 
 
 
 typedef struct seed_thread16_s{
    volatile uint16_t *source;
    volatile uint16_t *sink;
    int run;
#ifdef _WIN32
    HANDLE thr;
#else
    pthread_t thr;
#endif
} seed_thread_16;

typedef struct seed_state16_s{
    volatile uint16_t nodes[3];
    seed_thread_16 threads[3];
} SEEDY16;

uint16_t read_state_16(SEEDY16* state);
void start_seeder_16(SEEDY16* state);
void stop_seeder_16(SEEDY16* state);
void seedy16(uint8_t* buffer, size_t bytes);
#define rot16(n,i) ((n<<(i%16))|(n>>(16-(i%16))))

/*
 *   Return the next prime number, if the starting point is prime, it is retuned.
 */

uint16_t prime16(uint16_t start);



/*
 *   QX16 (Quad XOR, 16 bit port)
 */

typedef struct QX16_s
{
    void* feeder;
    uint16_t step;
    uint16_t iter;
    uint16_t pool[2][256];//2kb
} QX16;
void  qx16_init(QX16* q, void* f);
uint16_t qx16_next(QX16* q);
uint16_t qx16_at  (QX16* q, uint16_t i);



/*
 *   MT16 (Mersenne Twister - 16 Bits EXPERIMENTAL PORT FOR SMALL FOOTPRINT) FAILS PRACTRAND
 */

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */
typedef struct tagMTRand16
{
  uint16_t mt[STATE_VECTOR_LENGTH];
  size_t index;
} MT16;
uint16_t mt16_next(MT16* rand);
void mt16_init(MT16* rand, void* f);



#endif