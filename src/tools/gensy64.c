#include "tools.h"
#include "../libseedy/libseedy64.h"

int main(int argc, char** argv)
{
    uint64_t pick;
    SEEDY64 state;
    start_seeder_64(&state);
    wait_ms(SEEDY_INIT_MS_);
    while (1)
    {
        wait_us(SEEDY_INTERVAL_US_);
        pick = read_state_64(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

