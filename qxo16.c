#include "seedy.h"



void qxo16_init(QXO16* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 12347;
    q->step = 0;
    q->feeder = f;
}

uint16_t qxo16_at(QXO16* q, uint16_t i)
{
    uint16_t pos;
    pos = i * q->iter;
    uint8_t pos0 = pos;
    uint8_t pos1 = pos >> 3;
    uint8_t pos2 = pos >> 5;
    uint8_t pos3 = pos >> 8;

    return  ( q->pool[0][pos0] ) ^
            ( q->pool[0][pos2] >> 1 ) ^
            ( q->pool[1][pos1] << 3 ) ^
            ( q->pool[1][pos3] ) ;
}

uint16_t qxo16_next(QXO16* q)
{
    uint16_t next, pos;
    uint16_t* pos16 = (uint16_t*)&pos;
    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, uint16_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;/* Skip z */
    }
    next = qxo16_at(q, q->step);
    q->step++;

    return next;
}
