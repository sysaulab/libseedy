#ifndef _SEEDY_H
#define _SEEDY_H

/*****************************************************************************
 *                                                                           *
 *   STANDARD INT SUPPORT AND SHIMS.                                         *
 *                                                                           *
 *****************************************************************************/

/*
 *  MISCROSOFT COMPILERS EXCEPTIONS APPLIED FIRST, THEN C99 IS ASSUMED.
 */

#if defined(_MSC_VER) /* MICROSOFT COMPILER */

    /* NO LONG LONG, ANCIENT WIN16 COMPILER ASSUMING WIN16 TARGET */
    #if (_MSC_VER < 1200)

        #define SEEDY_WIDTH 2
        typedef unsigned short seedy_t;
        typedef unsigned char uint8_t;

    /* NO LONG LONG, ASSUMING WIN32 TARGET */
    #elif (_MSC_VER < 1310)

        #include "stdintms.h"
        #define SEEDY_WIDTH 4
        typedef uint32_t seedy_t;

    /* NO STDINT but LONG LONG is available */
    #elif (_MSC_VER < 1930) 

        #include "stdintms.h"
        #define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;

    /* STDINT is available... no shim needed. */
    #else

        #include <stdint.h>
        #define SEEDY_WIDTH 8
        typedef uint64_t seedy_t;

    #endif

#else /* ASSUMING MODERN COMPILER */

    #include <stdint.h>
    #define SEEDY_WIDTH 8
    typedef uint64_t seedy_t;

#endif /* END STANDARD INT SUPPORT AND SHIMS.*/

/*
 *  SLEEP AND THREADS. TODO: FIND BETTER TIMER FOR WINDOWS
 */

#if defined(_WIN32)
    #include <windows.h>
    #define wait_ms(ms) Sleep(ms)
    #define wait_us(ms) Sleep(ms)
#else
    #include <pthread.h>
    #include <unistd.h>
    #include <sys/random.h>
    #define wait_ms(ms) usleep(ms*1000)
    #define wait_us(us) usleep(us)
#endif

#define rot64(n,i) ((n<<(i%64))|(n>>(64-(i%64))))
#define rot32(n,i) ((n<<(i%32))|(n>>(32-(i%32))))
#define rot16(n,i) ((n<<(i%16))|(n>>(16-(i%16))))

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <inttypes.h>
#include <time.h>

void seedy(uint8_t* buffer, size_t bytes);
void stdinput(uint8_t* buffer, size_t bytes);
void* parseinputgen(int argc, char** argv);





/*****************************************************************************
 *                                                                           *
 *   ADDITIONAL ALGORITHMS DISTRIBUTED WITH THE LIBRARY.                     *
 *                                                                           *
 *****************************************************************************/



/*
 *   Return the next prime number, if the starting point is prime, it is retuned.
 */

uint64_t prime64(uint64_t start);
uint32_t prime32(uint32_t start);
uint16_t prime16(uint16_t start);



/*
 *   QXO64 (Quad XOR, 64 bits)
 */

typedef struct QXO64_s 
{
    void* feeder;
    uint64_t step;
    uint64_t iter;
    uint64_t pool[4][65536];
} QXO64;
void qxo64_init(QXO64* q, void* f);
uint64_t qxo64_next(QXO64* q);
uint64_t qxo64_at(QXO64* q, uint64_t i);



/*
 *   QXO32 (Quad XOR, 32 bit port)
 */

typedef struct QXO32_s
{
    void* feeder;
    uint32_t step;
    uint32_t iter;
    uint32_t pool[2][65536];
} QXO32;
void qxo32_init(QXO32* q, void* f);
uint32_t qxo32_next(QXO32* q);
uint32_t qxo32_at(QXO32* q, uint32_t i);


/*
 *   QXO16 (Quad XOR, 16 bit port)
 */

typedef struct QXO16_s
{
    void* feeder;
    uint16_t step;
    uint16_t iter;
    uint16_t pool[2][256];//2kb
} QXO16;
void  qxo16_init(QXO16* q, void* f);
uint16_t qxo16_next(QXO16* q);
uint16_t qxo16_at  (QXO16* q, uint16_t i);



/*
 *   MT32 (Mersenne Twister - 32 Bits)
 */

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */
typedef struct tagMTRand
{
  uint32_t mt[STATE_VECTOR_LENGTH];
  size_t index;
} MT32;
void mt32_init(MT32* rand, void* f);
uint32_t mt32_next(MT32* rand);



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

#endif // _SEEDY_H
