#include "tools.h"
#include "../libseedy/libseedy16.h"

int main(int argc, char** argv)
{
    uint16_t pick;
    SEEDY16 state;
    start_seeder_16(&state);
    wait_ms(SEEDY_INIT_MS_);
    while (1)
    {
        wait_us(SEEDY_INTERVAL_US_);
        pick = read_state_16(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

