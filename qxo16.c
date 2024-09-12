#include "qxo16.h"
#include "seedy.h"
#include "prime.h"
#include <string.h>
#include <unistd.h>

#define rot16(n,i) ((n<<(i%16))|(n>>(16-(i%16))))

void qxo16_init(QXO16* q, void* f)
{
    int x = 0;
    int y = 0;
    q->iter = 12347;
    q->step = 0;
    q->feeder = f;
}

qxo_t qxo16_at(QXO16* q, qxo_t i)
{
    qxo_t pos;
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

qxo_t qxo16_next(QXO16* q)
{
    qxo_t next, pos;
    uint16_t* pos16 = (uint16_t*)&pos;
    if( q->step == 0 )
    {
        ((void(*)(uint8_t* b, qxo_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
        q->step++;/* Skip z */
    }
    next = qxo16_at(q, q->step);
    q->step++;

    return next;
}
