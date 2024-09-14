#include "seedy.h"



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
    uint16_t pos;
    pos = i * q->iter;
    uint8_t pos0 = (uint8_t)pos;
    uint8_t pos1 = (uint8_t)pos >> 3;
    uint8_t pos2 = (uint8_t)pos >> 5;
    uint8_t pos3 = (uint8_t)pos >> 8;

    return  ( q->pool[0][pos0] ) ^
            ( q->pool[0][pos2] >> 1 ) ^
            ( q->pool[1][pos1] << 3 ) ^
            ( q->pool[1][pos3] ) ;
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
