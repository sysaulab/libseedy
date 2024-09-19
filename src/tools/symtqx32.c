#include <libseedy/libseedy32.h>
#include "algos/mt32.h"
#include "algos/qx32.h"

MT32 CCprng;
void (mt_seeder)(uint8_t* b, size_t n)
{
    mt32_fill(&CCprng, b, n);
}

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    QX32 QXprng;
    mt32_init(&CCprng, &seedy32);
    qx32_init(&QXprng, &mt_seeder);
    
    while (1)
    {
        qx32_fill(&QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


