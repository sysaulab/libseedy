/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "ftime.h"
#include "stat.h"

double get_minimum_nanosleep_delay()
{
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 1;
    double start = ftime();
    nanosleep(&t, NULL);
    return ftime() - start;
}

double get_minimum_usleep_delay()
{
    struct timespec t;
    t.tv_sec = 0;
    t.tv_nsec = 1;
    double start = ftime();
    usleep(1);
    return ftime() - start;
}

int main(int argc, char** argv)
{
    printf("Distributed under Affero GNU Public Licence version 3\n\n");
    fstat_t stats;

    fstat_init(&stats);
    for(int x = 0; x < 1000000; x++)
        fstat_analyze(&stats, get_minimum_usleep_delay());

    printf("usleep(1) min :    %1.1fµs\n", fstat_min(&stats) * 1000000);
    printf("usleep(1) avg :    %1.1fµs\n", fstat_avg(&stats) * 1000000);
    printf("usleep(1) max :    %1.1fµs\n", fstat_max(&stats) * 1000000);

    fstat_init(&stats);
    for(int x = 0; x < 100000; x++)
        fstat_analyze(&stats, get_minimum_nanosleep_delay());

    printf("nanosleep(1) min : %1.1fµs\n", fstat_min(&stats) * 1000000);
    printf("nanosleep(1) avg : %1.1fµs\n", fstat_avg(&stats) * 1000000);
    printf("nanosleep(1) max : %1.1fµs\n", fstat_max(&stats) * 1000000);
    return 0;
}