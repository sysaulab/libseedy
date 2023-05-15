/******************************************************************************
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |  Covered by agpl-v3                                                        |
 |  Commercial licence available upon request                                 |
 |                                                                            |
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <unistd.h> // usleep

#include "stat.h"
#include "ftime.h"

#define ___STAT_BLOCK_SIZE (4096) // size of uint64_t array, size in mb is 8 times greater

void* live_view(void* raw)
{
    distribution* dist = raw;

    while(1)
    {
        sleep(1);
        update_score(dist);

        double ftime_elapsed = ftime() - dist->start;
        double progress = (dist->u32_A.count / (double)0x100000000);
        double iter_per_sec = progress / ftime_elapsed;
        double iter_remain = 1 - progress;
        double ftime_remaining = iter_remain / iter_per_sec;

        if( progress > 1 )
            ftime_remaining = ftime() - dist->start;

        uint32_t hours = floor(ftime_remaining / 3600);
        double f_remaining = ftime_remaining - (hours*3600);

        uint32_t mins = floor(f_remaining / 60);
        f_remaining = f_remaining - (mins*60);

        uint32_t secs = f_remaining;

        fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        fprintf(stderr, "%.9f - %02uh%02um%02us - %.3f ", dist->u32_A.score_unique, hours, mins, secs, iter_per_sec);
        fprintf( dist->logfile, "%.9f,%.9f,%.9f\n", progress, dist->u32_A.score_unique, ftime_elapsed );
    }

    return NULL;
}

int main(int argc, const char * argv[])
{
    int max_stage = 0;
    uint64_t* buffer;
    distribution* dist;
    int stage = 0; 
    
    buffer = calloc(___STAT_BLOCK_SIZE, sizeof(uint64_t));
    dist = calloc(1, sizeof(distribution));
    if( buffer == NULL || dist == NULL )
    {
        fprintf(stderr, "FATAR ERROR - cannot allocate memory buffer");
        return 1;
    }

    if(argc > 1)
        max_stage = atoi(argv[1]);

    dist->logfile = fopen("icm_stat.log", "a");
    fprintf(dist->logfile, "time,score,progress\n");

    pthread_t live;
    pthread_create(&live, NULL, &live_view, dist);

    dist->start = ftime();
    size_t readed = 1;
    while(readed > 0)
    {
        readed = fread(buffer, sizeof(uint64_t), ___STAT_BLOCK_SIZE, stdin);
        double stop = 1.0;
        for(int x = 0; x < readed; x++)
            add_u64(dist, buffer[x]);
        switch (stage)
        {
            case 0: //pre 1.0
                if( (dist->u32_A.count / (double)0x100000000) >= 1.0 )
                {
                    update_score(dist);
                    printf("stage 0 : %0.9f\n", dist->u32_A.score_unique);
                    dist->stage_start = ftime();
                    stage++;
                    if( stage >= max_stage )
                    {
                        readed = 0;
                    }
                }
                break;

            case 1:
                stop = 0.9;
                goto next;

            case 2:
                stop = 0.99;
                goto next;

            case 3:
                stop = 0.999;
                goto next;

            case 4:
                stop = 0.9999;
                goto next;

            case 5:
                stop = 0.99999;
                goto next;

            case 6:
                stop = 0.999999;
                goto next;

            case 7:
                stop = 0.9999999;
                goto next;

            case 8:
                stop = 0.99999999;
                goto next;

            case 9:
                stop = 0.999999999;
                goto next;

            case 10:
                if( dist->u32_A.unique == 0x100000000 )
                {
                    printf("stage %u : %0.9f\n", stage, (double)(dist->u32_A.count / (double)0x100000000));
                    readed = 0;
                }
                break;

            next:
                if( dist->u32_A.score_unique > stop )
                {
                    printf("stage %u : %0.9f\n", stage, (double)(dist->u32_A.count / (double)0x100000000));
                    dist->stage_start = ftime();
                    stage++;
                    if( stage > max_stage )
                    {
                        readed = 0;
                    }
                }
            break;
        }
    }

    free(buffer);
    return 0;
}