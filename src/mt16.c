/* An implementation of the MT19937 Algorithm for the Mersenne Twister
 * by Evan Sultanik.  Based upon the pseudocode in: M. Matsumoto and
 * T. Nishimura, "Mersenne Twister: A 623-dimensionally
 * equidistributed uniform pseudorandom number generator," ACM
 * Transactions on Modeling and Computer Simulation Vol. 8, No. 1,
 * January pp.3-30 1998.
 *
 * http://www.sultanik.com/Mersenne_twister
 *
 * Adapted to libseedy by Sylvain Saucier
 *
 * THIS 16 BIT PORT IS NOT OUTPUTTING GOOD QUALITY
 * EXPERIMENT TO SEE IF WE CAN PORT IT TO MICROS
 */

#define UPPER_MASK		0x8000
#define LOWER_MASK		0x7fff
#define TEMPERING_MASK_B	0x5680
#define TEMPERING_MASK_C	0x0

#include "seedy.h"


void insert_16_4357(uint8_t* b, size_t n)
{
    *(uint16_t*)b = 4357;
}

void mt16_init(MT16* rand, void* f)
{
    uint16_t seed;
    ((void(*)(uint8_t* b, size_t n))f) ( (uint8_t*)&seed, sizeof(seed) );
    rand->mt[0] = seed & 0xffff;
    for(rand->index=1; rand->index<STATE_VECTOR_LENGTH; rand->index++)
    {
        rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffff;
    }
}

uint16_t mt16_next(MT16* rand)
{
    uint16_t y;
    static uint16_t mag[2] = {0x0, 0xb0df};
    if(rand->index >= STATE_VECTOR_LENGTH || rand->index < 0)
    {
        int32_t kk;
        if(rand->index >= STATE_VECTOR_LENGTH+1 || rand->index < 0)
        {
                mt16_init(rand, insert_16_4357);
        }
        for(kk=0; kk<STATE_VECTOR_LENGTH-STATE_VECTOR_M; kk++)
        {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
        }
        for(; kk<STATE_VECTOR_LENGTH-1; kk++)
        {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk+1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+(STATE_VECTOR_M-STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
        }
        y = (rand->mt[STATE_VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[STATE_VECTOR_LENGTH-1] = rand->mt[STATE_VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
    y = rand->mt[rand->index++];
    y ^= (y >> 5);
    y ^= (y << 1) & TEMPERING_MASK_B;
    y ^= (y << 9) & TEMPERING_MASK_C;
    y ^= (y >> 12);
    return y;
}

void mt16_fill(MT16* rand, uint8_t* b, size_t n)
{
    size_t pos = 0;
    size_t blocks = n / 2;
    size_t bytes = n % 2;
    uint16_t last;
    uint16_t next;
    while(pos < blocks)
    {
        next = mt16_next(rand);
        ((uint16_t*)b)[pos] = next;
        pos = pos + 1;
    }
    if(bytes)
    {
        next = mt16_next(rand);
        pos = 0;
        while(pos < blocks)
        {
            b[(blocks * 2) + pos] = ((uint8_t*)&next)[pos];
            pos = pos + 1;
        }
    }
}
