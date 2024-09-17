#include <libseedy/libseedy32.h>
#include "algos/mt32.h"
#include "algos/qx64.h"
#include <stdlib.h>

MT32* CCprng;
QX64* QXprng;

void mt_seeder(uint8_t* b, size_t n)
{
    mt32_fill(CCprng, b, n);
}

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    int x = 0;

    CCprng = malloc(sizeof(MT32));
    QXprng = malloc(sizeof(QX64));
    mt32_init(CCprng, &seedy32);
    qx64_init(QXprng, &mt_seeder);
    
    while (1)
    {
		qx64_fill(QXprng, (uint8_t*)&buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
    free(CCprng);
    free(QXprng);
}


