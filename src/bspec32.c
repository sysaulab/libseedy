/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <unistd.h>

#include "stat.h"
#include "ftime.h"
#include "config.h"

int max_stage = 0;
int stage = 0; 

#ifndef _SSRNG_CORE
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
        double kbps = (dist->u32_A.count / ftime_elapsed)/1024;

        if( progress > 1 )
            ftime_remaining = ftime() - dist->start;

        uint32_t hours = floor(ftime_remaining / 3600);
        double f_remaining = ftime_remaining - (hours*3600);

        uint32_t mins = floor(f_remaining / 60);
        f_remaining = f_remaining - (mins*60);

        uint32_t secs = f_remaining;

        fprintf(stderr, "\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
        fprintf(stderr, "%.15f - %02uh%02um%02us - %.0fKiB/s - %u/%u", dist->u32_A.score_unique, hours, mins, secs, kbps, stage, max_stage);
        fprintf( dist->logfile, "%.9f,%.9f,%.9f\n", progress, dist->u32_A.score_unique, ftime_elapsed );
    }

    return NULL;
}
#endif

int main(int argc, const char * argv[])
{
    uint64_t* buffer;
    distribution* dist;

    uint32_t cfg_debug = 0;
    for( int x = 1; x < argc; x++ )
    {
        if(!strcmp("debug", argv[x]))
            cfg_debug = 1;
        else
        {
            printf("Distributed under Affero GNU Public Licence version 3\n\n");
            printf("usage : <stdin> | bspec {0-10}\n");
            printf("ex.: seed | bspec 10\n");
            
            return 1;
        }
    }
    
    buffer = calloc(_SSRNG_BUFLEN, sizeof(uint64_t));
    dist = calloc(1, sizeof(distribution));
    if( buffer == NULL || dist == NULL )
    {
        fprintf(stderr, "FATAL ERROR - cannot allocate memory buffer");
        return EXIT_FAILURE;
    }

    if(argc > 1)
        max_stage = atoi(argv[1]);

    if(max_stage < 0)
        max_stage = 0;

    if(max_stage > 10)
        max_stage = 10;

    dist->logfile = fopen("icm_stat.log", "a");
    fprintf(dist->logfile, "progress,score,seconds\n");

    pthread_t live;
    pthread_create(&live, NULL, &live_view, dist);

    dist->start = ftime();
    
    for(
        size_t readed = 1;
        readed > 0;
        readed = fread(buffer, sizeof(uint64_t), _SSRNG_BUFLEN, stdin)
       )
    {
        double stop = 1.0;
        for(int x = 0; x < readed; x++)
            add_u64(dist, buffer[x]);
        switch (stage)
        {
            case 0: //pre 1.0x
                if( (dist->u32_A.count / (double)0x100000000) >= 1.0 )
                {
                    update_score(dist);
                    printf("stage 0 : %0.9f\n", dist->u32_A.score_unique);
                    dist->stage_start = ftime();
                    stage++;
                    if( stage >= max_stage ) //stop after reaching testing level, default 0, max 10;
                        readed = 0;
                }
                break;

            case 1:
                stop = 0.9;
                goto end;

            case 2:
                stop = 0.99;
                goto end;

            case 3:
                stop = 0.999;
                goto end;

            case 4:
                stop = 0.9999;
                goto end;

            case 5:
                stop = 0.99999;
                goto end;

            case 6:
                stop = 0.999999;
                goto end;

            case 7:
                stop = 0.9999999;
                goto end;

            case 8:
                stop = 0.99999999;
                goto end;

            case 9:
                stop = 0.999999999;
                goto end;

            case 10:
                if( dist->u32_A.unique == 0x100000000 )
                {
                    printf("stage %u : %0.9f\n", stage, (double)(dist->u32_A.count / (double)0x100000000));
                    readed = 0;
                }
                break;

            end:
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
    return EXIT_SUCCESS;
}