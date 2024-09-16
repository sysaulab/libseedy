#include "seedy.h"

void qx32_init(QX32* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 1234567907;
    q->step = 0;
    q->feeder = f;
}

uint32_t qx32_at(QX32* q, uint32_t i)
{
    uint32_t pos32;
    uint16_t pos1, pos2, pos3;
    pos32 = i * q->iter;
    pos1 = (uint16_t)pos32;
    pos2 = (uint16_t)(pos32 >> 8);
    pos3 = (uint16_t)(pos32 >> 16);

    return  q->pool[0][pos1] ^
            q->pool[1][pos2] ^
            q->pool[2][pos3] ;
}

uint32_t qx32_next(QX32* q)
{
    uint32_t next, pos32;
    uint16_t* pos16 = (uint16_t*)&pos32;

    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, uint32_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;
    }
    next = qx32_at(q, q->step);
    q->step++;

    return next;
}

void qx32_fill(QX32* rand, uint8_t* b, size_t n)
{
    size_t pos = 0;
    size_t blocks = n / 4;
    size_t bytes = n % 4;
    uint32_t next;
    while(pos < blocks)
    {
        next = qx32_next(rand);
        ((uint32_t*)b)[pos] = next;
        pos = pos + 1;
    }
    if(bytes)
    {
        next = qx32_next(rand);
        pos = 0;
        while(pos < blocks)
        {
            b[(blocks * 4) + pos] = ((uint8_t*)&next)[pos];
            pos = pos + 1;
        }
    }
}
