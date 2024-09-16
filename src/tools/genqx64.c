#include "tools.h"
#include "algos/qx64.h"

int main(int argc, char** argv)
{
    uint8_t buf[102400];
    QX64 prng;
    qx64_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        qx64_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

