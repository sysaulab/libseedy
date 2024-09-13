#include "seedy.h"

int main(int argc, char** argv)
{
    uint8_t buf[1024*1024];

    while (1)
    {
        seedy(buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}

