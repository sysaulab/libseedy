#include "tools.h"
#include "../libseedy/libseedy32.h"

int main(int argc, char** argv)
{
    uint32_t pick;
    SEEDY32 state;
    start_seeder_32(&state);
    wait_ms(SEEDY_INIT_MS_);
    while (1)
    {
        wait_us(SEEDY_INTERVAL_US_);
        pick = read_state_32(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

