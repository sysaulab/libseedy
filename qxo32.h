#ifndef _QXO32_H__kjhf
#define _QXO32_H__kjhf

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 *   EXPERIMENTAL !!! Practrand > , DO NOT USE.
 */

typedef uint32_t qxo_t;

typedef struct QXO32_s {
    void* feeder;
    qxo_t step;
    qxo_t iter;
    qxo_t pool[2][65536];
} QXO32;

void qxo32_init(QXO32* q);
qxo_t qxo32_next(QXO32* q);
qxo_t qxo32_at(QXO32* q, qxo_t i);


#endif
