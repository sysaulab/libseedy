#ifndef _QXO64_H__kjhf
#define _QXO64_H__kjhf

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/*
 *   STABLE !!! Terrabytes tested successfully *** with good seeding tables ***.
 *   Works best with truly chaotic data source.
 */

typedef uint64_t qxo_t;

typedef struct QXO64_s {
    void* feeder;
    qxo_t step;
    qxo_t iter;
    qxo_t pool[4][65536];
} QXO64;

void qxo64_init(QXO64* q, void* f);
qxo_t qxo64_next(QXO64* q);
qxo_t qxo64_at(QXO64* q, qxo_t i);

#endif