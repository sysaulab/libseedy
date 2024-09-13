#include "seedy.h"

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
