#include <stdlib.h>

#include "tools.h"
#if defined(LIBSEEDY_OPT64)
#   include <libseedy/libseedy64.h>
#elif defined(LIBSEEDY_OPT32)
#   include <libseedy/libseedy32.h>
#elif defined(LIBSEEDY_OPT16)
#   include <libseedy/libseedy16.h>
#endif

#include "algos/cc2032.h"
#include "algos/qx32.h"



CC2032* CCprng;
QX32* QXprng;
void mt_seeder(uint8_t* b, size_t n)
{
fprintf(stderr, "mt_seeder()");
    cc2032_fill(CCprng, b, n);
}

int main(int argc, char** argv)
{
    uint8_t buf[1024];
    QXprng = malloc(sizeof(QX32));
    CCprng = malloc(sizeof(CC2032));
    if( (QXprng == NULL) || (CCprng == NULL) )
    {
        fprintf(stderr, "Out of memory.\n");
        exit(0);
    }
#if defined(LIBSEEDY_OPT64)
    cc2032_init(CCprng, seedy64);
#elif defined(LIBSEEDY_OPT32)
    cc2032_init(CCprng, seedy32);
#elif defined(LIBSEEDY_OPT16)
    cc2032_init(CCprng, seedy16);
#endif
    qx32_init(QXprng, mt_seeder);
    
    while (1)
    {
        qx32_fill(QXprng, buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


