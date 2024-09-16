#ifndef LIBSEEDY_SS64_H_ 
#define LIBSEEDY_SS64_H_

#include "shishua/shishua.h"

/*
 *   SS64 (SHISHUA - Fastest non-cryptographic PRNG. AVAILABLE ON MODERN COMPILERS ONLY
 *
 *   The fake_prng object serve to allow the definition to be here without having to link to shoshua.h header everywhere.
 *   MUST MATCH THE DEFINITION FOUND IN shishua/shishua.h.
 */

typedef struct SS64_s
{
    void* feeder;
    uint64_t pool[4];
    prng_state shishua;
}
SS64;

void ss64_init(SS64* s, void* f)
{
    s->feeder = f;
    ((void(*)(uint8_t*b,size_t n))f)((uint8_t*)&s->pool,sizeof(s->pool));
    prng_init((prng_state*)&s->shishua, s->pool);
}

void ss64_fill(SS64* s, uint8_t* b, size_t n)
{
    prng_gen((prng_state*)&s->shishua, b, n);
}

#endif
