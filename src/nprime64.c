#include "seedy.h"

int main(int argc, char** argv)
{
    uint64_t start = 0;
    if (argc > 1)
        start = strtoull(argv[1], NULL, 10);
    start = prime64(start);
    printf("%llu\n", start);
    return 1;
}

