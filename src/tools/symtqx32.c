#include <libseedy/libseedy32.h>
#include "algos/mt32.h"
#include "algos/qx32.h"

MT32 MTprng;
void (mt_seeder)(uint8_t* b, size_t n)
{
    mt32_fill(&MTprng, b, n);
}

int main(int argc, char** argv)
{
    int x = 0;
    uint8_t buf[1024];
    QX32 QXprng;
    mt32_init(&MTprng, &seedy32);
    qx32_init(&QXprng, &mt_seeder);
    
    while (1)
    {
        qx32_fill(&QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


