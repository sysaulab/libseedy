#include <libseedy/libseedy32.h>
#include "algos/cc2032.h"
#include "algos/qx32.h"


void mt_seeder(uint8_t* b, size_t n)
{
    CC2032 CCprng;
    cc2032_init(&CCprng, seedy32);
//fprintf(stderr, "buffer:%016lx, length:%zu\n", b, n);
    cc2032_fill(&CCprng,b,n);
//fprintf(stderr, ".4\n");
}

int main(int argc, char** argv)
{
    int x = 0;
    uint8_t buf[10240];
    QX32 QXprng;
    qx32_init(&QXprng, (void*)&mt_seeder);

    while (1)
    {
        qx32_fill(&QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


