#include "seedy.h"
#include "shishua/shishua.h"

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