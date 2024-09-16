#ifndef LIBSEEDY_QX16_H_
#define LIBSEEDY_QX16_H_

#include <libseedy/common.h>

/*
 *   QX16 (Quad XOR, 16 bit port)
 */

typedef struct QX16_s
{
    void* feeder;
    uint16_t step;
    uint16_t iter;
    uint16_t pool[2][4096];//16kb
}
QX16;

void qx16_init(QX16* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 12347;
    q->step = 0;
    q->feeder = f;
}

uint16_t qx16_at(QX16* q, uint16_t i)
{
    uint16_t pos, pos1, pos2;
	uint16_t rot1, rot2;
    pos = i * q->iter;
    rot1 = pos & 0x000f;
    pos1 = pos >> 4;
    rot2 = pos >> 12;
    pos2 = pos & 0x0fff;

    return  ( ROT16(q->pool[0][pos1],rot1) ) ^
            ( ROT16(q->pool[1][pos2],rot2) ) ;
}

uint16_t qx16_next(QX16* q)
{
    uint16_t next, pos;
    uint16_t* pos16 = (uint16_t*)&pos;
    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, uint16_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;/* Skip z */
    }
    next = qx16_at(q, q->step);
    q->step++;

    return next;
}

void qx16_fill(QX16* rand, uint8_t* b, size_t n)
{
    size_t pos = 0;
    size_t blocks = n / 2;
    size_t bytes = n % 2;
    uint16_t last;
    uint16_t next;
    while(pos < blocks)
    {
        next = qx16_next(rand);
        ((uint16_t*)b)[pos] = next;
        pos = pos + 1;
    }
    if(bytes)
    {
        next = qx16_next(rand);
        pos = 0;
        while(pos < blocks)
        {
            b[(blocks * 2) + pos] = ((uint8_t*)&next)[pos];
            pos = pos + 1;
        }
    }
}

#endif
