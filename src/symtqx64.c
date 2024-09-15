#include "seedy.h"

MT32* MTprng;
QX64* QXprng;

void mt_seeder(uint8_t* b, size_t n)
{
    mt32_fill(MTprng, b, n);
}

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    int x = 0;

    MTprng = malloc(sizeof(MT32));
    QXprng = malloc(sizeof(QX64));
    mt32_init(MTprng, &seedy32);
    qx64_init(QXprng, &mt_seeder);
    
    while (1)
    {
		qx64_fill(QXprng, (uint8_t*)&buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
    free(MTprng);
    free(QXprng);
}


