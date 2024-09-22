#include "algos/prime64sub16.h"
#include "../libseedy/libseedy64.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_PRIMES 32

int main(int argc, char** argv)
{
    printf( "%llx", prime64sub16() );

    return 1;
}

