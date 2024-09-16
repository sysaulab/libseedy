#include "seedy.h"

int main(int argc, char** argv)
{
    uint16_t start = 0;
    if (argc > 1)
        start = atoi(argv[1]);
    start = prime16(start);
    printf("%u\n", start);
    return 1;
}

