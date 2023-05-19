/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"

void* prng(void * raw)
{
    uint64_t buffer[_SSRNG_BUFLEN];
    uint64_t prng_state[4][256];
    uint32_t x = 0;
    uint8_t* x8 = (uint8_t*)&x;
    while(1)
    {
        for( uint64_t y = 0; y < _SSRNG_BUFLEN; y++ )
        {
            if(x == 0)
            {
                fread( prng_state, sizeof(uint64_t), 4 * 256, stdin);
            }

            buffer[y] = prng_state[0][x8[0]] ^ prng_state[1][x8[1]] ^ prng_state[2][x8[2]] ^ prng_state[3][x8[3]];
            x++;
        }
        fwrite(buffer, sizeof(uint64_t), _SSRNG_BUFLEN, stdout);
    }
    return NULL;
}

int main(int argc, char** argv)
{
    pthread_t threads[2];

    for (int x = 0; x < 2; x++)
        pthread_create(&threads[x], NULL, prng, NULL);

    for (int x = 0; x < 2; x++)
        pthread_join(threads[x], NULL);

    return EXIT_SUCCESS;
}