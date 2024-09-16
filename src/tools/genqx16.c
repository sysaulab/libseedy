#include "tools.h"
#include "qx16.h"

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    QX16 prng;
    qx16_init(&prng, parseinputgen(argc, argv));
    
    while (1)
    {
        qx16_fill(&prng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

