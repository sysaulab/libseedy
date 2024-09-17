#ifndef LIBSEEDY_CC32_H_
#define LIBSEEDY_CC32_H_

/*
 *   https://cr.yp.to/streamciphers/timings/estreambench/submissions/salsa20/chacha8/ref/chacha.c
 */

#include <libseedy/common.h>
#include <string.h>
#include <stdlib.h>
#include "chacha8/chacha8.h"

typedef struct CC832_s
{
    void* feeder;
    ECRYPT_ctx pool;
    size_t step;
	uint8_t text[64];
	uint8_t next[64];
	int used;
}
CC832;

void cc832_init(CC832* s, void* f)
{
    s->feeder = f;
    ((void(*)(uint8_t*,size_t))f) ( (uint8_t*)&s->pool, sizeof(s->pool) );
	s->step = 0;
	s->used = 64;
	memset(&s->text, 0, 64);
}

void cc832_refill(CC832* q)
{
    if( (q->step&0xffffFFFFffffFFFF) == 0 ) /* reseed at the end of the period, 2^70 bytes - 2^6 bytes according to a comment in line #1006. The mask is there to prevent machines with >64bit word length to repeat the sequence */
    {
        ((void(*)(uint8_t* b, uint64_t n))q->feeder)((uint8_t*)&q->pool, sizeof(q->pool));
    }
	
	ECRYPT_encrypt_bytes( &q->pool, q->text, q->next, 64 );
	q->used = 0;
    q->step++;
}

void cc832_fill(CC832* s, uint8_t* b, size_t n)
{
	size_t requested = n;
	size_t chunk_size;
    size_t written = 0;
    size_t available = 0;
    size_t needed = 0;
    while(written < requested) /* There is work to do */
    {
        available = sizeof(s->next) - s->used;
        needed = n - written;
        if ( available == 0 )
		{
			cc832_refill(s);
		}
        chunk_size = MIN(available, needed); /* How many bytes are available? How many do I need? Return the smallest. It is the size we can copy now...*/
        memcpy(&b[written], &s->next[s->used], chunk_size); /* copy what we need for now */
        written += chunk_size; /* Increase our position by the amount written */
        s->used += chunk_size; /* Increase our consumption by the amount written */
    }
}

#endif
