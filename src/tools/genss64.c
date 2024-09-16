#include "tools.h"
#include "ss64.h"

int main(int argc, char** argv)
{
    uint8_t buf[102400];
    SS64 prng;
    ss64_init(&prng, parseinputgen(argc, argv));

    while (1)
    {
        ss64_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

