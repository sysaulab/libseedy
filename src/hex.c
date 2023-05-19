/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */

#include <stdio.h>
#include <stdint.h>
#include "config.h"

int main(int argc, char** argv)
{
    uint8_t buffer[_SSRNG_BUFSIZE];
    while(1)
    {   
        uint64_t buf_received = fread(&buffer, sizeof(char), _SSRNG_BUFSIZE, stdin);
        if(!buf_received)
            break;
        for( int x = 0; x < buf_received / 8; x++ )
            fprintf(stdout, "%016llx", ((uint64_t*)buffer)[x]);
        for( int y = (buf_received / 8) * 8; y < buf_received % 8; y++ )
            fprintf(stdout, "%02x", buffer[y]);
    }
}