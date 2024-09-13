#include "seedy.h"

int main(int argc, char** argv)
{
    SS64 prng;
    ss64_init(&prng, parseinputgen(argc, argv));
    uint64_t buf[1024];

    while (1)
    {
        ss64_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

