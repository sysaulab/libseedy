#include <libseedy/libseedy64.h>
#include "algos/cc2032.h"
#include "algos/qx64.h"


void mt_seeder(uint8_t* b, size_t n)
{
    CC2032 CCprng;
    cc2032_init(&CCprng, seedy64);
//fprintf(stderr, "buffer:%016lx, length:%zu\n", b, n);
    cc2032_fill(&CCprng,b,n);
//fprintf(stderr, ".4\n");
}

int main(int argc, char** argv)
{
    int x = 0;
    uint8_t buf[10240];
    QX64 QXprng;
    qx64_init(&QXprng, (void*)&mt_seeder);

    while (1)
    {
        qx64_fill(&QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


