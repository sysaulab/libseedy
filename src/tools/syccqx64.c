#include "tools.h"
#include "../libseedy/libseedy64.h"
#include "algos/cc2032.h"
#include "algos/qx64.h"
#include "stdio.h"

CC2032 CCprng;

void mt_seeder(uint8_t* b, size_t n)
{
    cc2032_fill(&CCprng,b,n);
}

int main(int argc, char** argv)
{
    QX64 QXprng;
    uint8_t buf[10240];

    cc2032_init(&CCprng, seedy64);
    qx64_init(&QXprng, (void*)&mt_seeder);

    while (1)
    {
        qx64_fill(&QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


