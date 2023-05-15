/******************************************************************************
 |                                                                            |
 |  Copyright 2023, All rights reserved, Sylvain Saucier                      |
 |  sylvain@sysau.com                                                         |
 |  Covered by agpl-v3                                                        |
 |  Commercial licence available upon request                                 |
 |                                                                            |
 ******************************************************************************/
 
 #include <time.h>
double ftime()
{
#ifdef __APPLE__
    return (double)(clock_gettime_nsec_np(CLOCK_REALTIME)) / 1000000000.0;
#else
    struct timespec _linux_ts;
    clock_gettime(CLOCK_REALTIME, &_linux_ts);
    double _linux_time = _linux_ts.tv_sec;
    _linux_time += _linux_ts.tv_nsec / 1000000000.0;
    return _linux_time;
#endif
}
