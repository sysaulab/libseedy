#ifndef LIBSEEDY_CC2032_H_
#define LIBSEEDY_CC2032_H_

/*
 *   https://cr.yp.to/streamciphers/timings/estreambench/submissions/salsa20/chacha8/ref/chacha.c
 */

#include <libseedy/common.h>
#include <string.h>
#define CHACHA20_IMPLEMENTATION
#include "ChaCha20/ChaCha20.h"

typedef struct CC2032_s
{
    void* feeder;
    ChaCha20_Ctx ctx;
	size_t step;
	uint8_t text[64];
	uint8_t next[64];
	int used;
}
CC2032;

void cc2032_init(CC2032* s, void* f)
{
    s->feeder = (void*)&f;
}

void cc2032_refill(CC2032* q)
{
    if( (q->step&0xffffFFFFffffFFFF) == 0 ) /* reseed at the end of the period, 2^70 bytes for chacha8, asssuming equal or better here.  */
    {
        key256_t key;
        nonce96_t nonce;
        uint32_t count = 0x00000001;
        ((void(*)(uint8_t* b, uint64_t n))q->feeder)((uint8_t*)&key, sizeof(key));
        ((void(*)(uint8_t* b, uint64_t n))q->feeder)((uint8_t*)&nonce, sizeof(nonce));
        ChaCha20_init(&q->ctx, key, nonce, count);
    }
    memset(q->next, 0, sizeof(q->next));
	ChaCha20_xor(&q->ctx, q->next, sizeof(q->next));
	q->used = 0;
    q->step++;
}

void cc2032_fill(CC2032* s, uint8_t* b, size_t n)
{
	size_t written;
	size_t requested = n;
	size_t chunk;
    size_t pos = 0;

    while(pos < requested) /* There is work to do */
    {
		if(s->used < 64) /* There is data available */
		{
			chunk = MIN(64 - s->used, requested - pos); /* How many bytes are available? How many do I need? Return the smallest. */
			memcpy(&b[pos], &s->next[s->used], chunk); /* copy what we need for now */
			pos = pos + chunk; /* Increase our position by the amount written */
			s->used = s->used + chunk; /* Increase our consumption by the amount written */
		}
		else /* There is no data, refill */
		{
			cc2032_refill(s);
		}
    }
}

#endif


