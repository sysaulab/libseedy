#include "seedy.h"
#include <stdlib.h>

int main(int argc, char** argv)
{
    uint32_t start = 0;
    if (argc > 1)
        start = atoi(argv[1]);
    start = prime32(start);
    printf("%u\n", start);
    return 1;
}

