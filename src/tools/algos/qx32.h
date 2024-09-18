#ifndef LIBSEEDY_QX32_H_
#define LIBSEEDY_QX32_H_

#include <libseedy/common.h>

/*
 *   QX32 (Quad XOR, 32 bit port)
 */

typedef struct QX32_s
{
    void* feeder;
    uint32_t step;
    uint32_t iter;
    uint32_t pool[2][0x10000];
}
QX32;

int qx32_init(QX32* q, void* f)
{
    q->iter = 1234567907;
    q->step = 0;
    q->feeder = f;
    return 1;
}

uint32_t qx32_at(QX32* q, uint32_t i)
{
    uint32_t pos32;
    uint16_t pos1, pos2;
    uint8_t rot1, rot2;
    pos32 = i * q->iter;

    pos1 = (uint16_t)(pos32);
    rot2 = (uint8_t)(pos32) & 0x1f;

    pos2 = (uint16_t)(pos32 >> (16));
    rot1 = (uint8_t)(pos32 >> 16) & 0x1f;

    return  ROT32(q->pool[0][pos1],rot1) ^
            ROT32(q->pool[1][pos2],rot2) ;
}

uint32_t qx32_next(QX32* q)
{
    uint32_t next;

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

#endif
