#include "seedy.h"
#include "mtwister/mtwister.h"
#include "mtwister/mtwister.c"


void mt32_init(MT32* s, void* f)
{
    s->feeder = f;
    ((void(*)(uint8_t*,size_t))f) ( (uint8_t*)&s->pool, sizeof(s->pool) );
    m_seedRand(&s->gen_mt32, s->pool);
}

uint32_t mt32_next(MT32* s)
{
    return genRandLong(&s->gen_mt32);
}

void mt32_fill(MT32* rand, uint8_t* b, size_t n)
{
    size_t pos = 0;
    size_t blocks = n / 4;
    size_t bytes = n % 4;
    uint32_t next;
    while(pos < blocks)
    {
        next = mt32_next(rand);
        ((uint32_t*)b)[pos] = next;
        pos = pos + 1;
    }
    if(bytes)
    {
        next = mt32_next(rand);
        pos = 0;
        while(pos < blocks)
        {
            b[(blocks * 4) + pos] = ((uint8_t*)&next)[pos];
            pos = pos + 1;
        }
    }
}


