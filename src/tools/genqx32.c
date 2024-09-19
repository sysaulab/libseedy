#include "tools.h"
#include "algos/qx32.h"

int main(int argc, char** argv)
{
    uint8_t buf[10240];
    QX32 prng;
    qx32_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        qx32_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


