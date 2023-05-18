/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Commercial licence available upon request */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 16384

int main(int argc, char** argv)
{
    if(argc < 2)
        return 0;

    double   cfg_count = 0;
    uint64_t bytes_requested = 1;
    uint64_t factor = 0;
    int x = 1;
    while( x < argc )
    {
        if(!strcmp("kb", argv[x]) || !strcmp("k", argv[x])){
            if(bytes_requested == 0)
                bytes_requested = 1000;
            else
                bytes_requested *= 1000;
        }
        else if(!strcmp("mb", argv[x]) || !strcmp("m", argv[x])){
            if(bytes_requested == 0)
                bytes_requested = 1000000;
            else
                bytes_requested *= 1000000;
        }
        else if(!strcmp("gb", argv[x]) || !strcmp("g", argv[x])){
            if(bytes_requested == 0)
                bytes_requested = 1000000000;
            else
                bytes_requested *= 1000000000;
        }
        else if(!strcmp("tb", argv[x]) || !strcmp("t", argv[x])){
            if(bytes_requested == 0)
                bytes_requested = 1000000000000;
            else
                bytes_requested *= 1000000000000;
        }
        else{
            cfg_count = atof(argv[x]);
        }
        x++;
    }
    factor = bytes_requested;
    bytes_requested = (uint64_t)(cfg_count * (double)bytes_requested);

    uint64_t bytes_written = 0;

    while(1)
    {
        uint8_t buffer[BUFSIZE];
        uint64_t remains = (bytes_requested - bytes_written);
        if(!remains) break;
        uint64_t have = fread(&buffer, sizeof(uint8_t), BUFSIZE, stdin);
        if(!have) break;
        bytes_written += fwrite(&buffer, sizeof(uint8_t), have > remains ? remains : have, stdout);
    }
}