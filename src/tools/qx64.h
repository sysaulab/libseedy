#ifndef LIBSEEDY_QX64_H_
#define LIBSEEDY_QX64_H_

#include <libseedy/common.h>

/*
 *   QX64 (Quad XOR, 64 bits)
 */

#include <libseedy/common.h>

typedef struct QX64_s 
{
    void* feeder;
    uint64_t step;
    uint64_t iter;
    uint64_t pool[4][65536];
}
QX64;

void qx64_init(QX64* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 12345678912345678943ULL;
    q->step = 0;
    q->feeder = f;
}

uint64_t qx64_at(QX64* q, uint64_t i)
{
    uint64_t pos64;
    uint16_t* pos16 = (uint16_t*)&pos64;
    pos64 = i * q->iter;

    return  ( q->pool[0][pos16[0]] ) ^
            ( q->pool[1][pos16[1]] ) ^
            ( q->pool[2][pos16[2]] ) ^
            ( q->pool[3][pos16[3]] ) ;
}

uint64_t qx64_next(QX64* q)
{
    uint64_t next, pos64;
    uint16_t* pos16 = (uint16_t*)&pos64;

    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, uint64_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;
    }
    next = qx64_at(q, q->step);
    q->step++;

    return next;
}

void qx64_fill(QX64* rand, uint8_t* b, size_t n)
{
    uint64_t next;
    size_t pos = 0;
    size_t blocks = n / 8;
    size_t bytes = n % 8;
    while(pos < blocks)
    {
        next = qx64_next(rand);
        ((uint64_t*)b)[pos] = next;
        pos = pos + 1;
    }
    if(bytes)
    {
        next = qx64_next(rand);
        pos = 0;
        while(pos < blocks)
        {
            b[(blocks * 8) + pos] = ((uint8_t*)&next)[pos];
            pos = pos + 1;
        }
    }
}

#endif
