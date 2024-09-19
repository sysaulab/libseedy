#include <stdio.h>
#include <time.h>
#include <dos.h>

#define UPPER_MASK		0x80000000
#define LOWER_MASK		0x7fffffff
#define TEMPERING_MASK_B	0x9d2c5680
#define TEMPERING_MASK_C	0xefc60000
#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef struct MTdos_s
{
    unsigned long int mt[STATE_VECTOR_LENGTH];
    long int index;
}
MTdos;

void m_seedRand(MTdos* rand, unsigned long int seed)
{
    rand->mt[0] = seed & 0xffffffff;
    for(rand->index=1; rand->index<STATE_VECTOR_LENGTH; rand->index++)
    {
      rand->mt[rand->index] = (6069 * rand->mt[rand->index-1]) & 0xffffffff;
    }
}

MTdos seedRand(unsigned long int seed)
{
    MTdos rand;
    m_seedRand(&rand, seed);
    return rand;
}


unsigned long int genRandLong(MTdos* rand) {
    unsigned long int y;
    static unsigned long int mag[2] = {0x0, 0x9908b0df};
    if(rand->index >= STATE_VECTOR_LENGTH || rand->index < 0)
    {
        int kk = 0;
        if(rand->index >= STATE_VECTOR_LENGTH + 1 || rand->index < 0)
        {
            m_seedRand(rand, 4357);
        }
        while( kk < STATE_VECTOR_LENGTH - STATE_VECTOR_M )
        {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk + 1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+STATE_VECTOR_M] ^ (y >> 1) ^ mag[y & 0x1];
            kk++;
        }
        while( kk < STATE_VECTOR_LENGTH - 1 )
        {
            y = (rand->mt[kk] & UPPER_MASK) | (rand->mt[kk + 1] & LOWER_MASK);
            rand->mt[kk] = rand->mt[kk+(STATE_VECTOR_M-STATE_VECTOR_LENGTH)] ^ (y >> 1) ^ mag[y & 0x1];
            kk++;
        }
        y = (rand->mt[STATE_VECTOR_LENGTH-1] & UPPER_MASK) | (rand->mt[0] & LOWER_MASK);
        rand->mt[STATE_VECTOR_LENGTH-1] = rand->mt[STATE_VECTOR_M-1] ^ (y >> 1) ^ mag[y & 0x1];
        rand->index = 0;
    }
  y = rand->mt[rand->index++];
  y ^= (y >> 11);
  y ^= (y << 7) & TEMPERING_MASK_B;
  y ^= (y << 15) & TEMPERING_MASK_C;
  y ^= (y >> 18);
  return y;
}

unsigned long int seedy_dos()
{
    unsigned long int h = 0;
    int j = 0;
    while(j++ < 192)
    {
        delay(13);
        h = ( h << 1 ) | ( h >> 31 );
        h ^= (unsigned long int)clock();
    }
    return h;
}

void main()
{
    MTdos mt;
    unsigned long int buffer;
    unsigned long int newseed = seedy_dos();

    mt = seedRand(newseed);
    while(1)
    {
        buffer = genRandLong(&mt);
        fwrite(&buffer, sizeof(unsigned long int), 1, stdout);
    }
}
