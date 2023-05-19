/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Commercial licence available upon request */

#include <stdio.h>
#include <stdlib.h>
#include "config.h"

int main(int argc, char** argv)
{
    char buffer[_SSRNG_BUFSIZE];
    while(fread(&buffer, sizeof(char), _SSRNG_BUFSIZE, stdin))
    {;}
    return EXIT_SUCCESS;
}