#include "seedy.h"

int main(int argc, char** argv)
{
    uint64_t pick;
    SEEDY64 state;
    start_seeder_64(&state);
    wait_ms(1);
    while (1)
    {
        wait_us(30);
        pick = read_state_64(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

