#ifndef _QXO16_H__kjhf
#define _QXO16_H__kjhf

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint16_t qxo_t;

typedef struct QXO16_s {
    void* feeder;
    qxo_t step;
    qxo_t iter;
    qxo_t pool[2][256];//2kb
} QXO16;

void  qxo16_init(QXO16* q, void* f);
qxo_t qxo16_next(QXO16* q);
qxo_t qxo16_at  (QXO16* q, qxo_t i);

#endif
