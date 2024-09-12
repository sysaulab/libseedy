#include "qxo64.h"
#include "prime.h"
#include "seedy.h"
#include <string.h>

void qxo64_init(QXO64* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 12345678912345678943ULL;
    q->step = 0;
    q->feeder = f;
}

qxo_t qxo64_at(QXO64* q, qxo_t i)
{
    qxo_t pos64;
    uint16_t* pos16 = (uint16_t*)&pos64;
    pos64 = i * q->iter;

    return  ( q->pool[0][pos16[0]] ) ^
            ( q->pool[1][pos16[1]] ) ^
            ( q->pool[2][pos16[2]] ) ^
            ( q->pool[3][pos16[3]] ) ;
}

qxo_t qxo64_next(QXO64* q)
{
    qxo_t next, pos64;
    uint16_t* pos16 = (uint16_t*)&pos64;

    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, qxo_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;
    }
    next = qxo64_at(q, q->step);
    q->step++;

    return next;
}
