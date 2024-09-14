#include "seedy.h"

int main(int argc, char** argv)
{
    uint32_t pick;
    SEEDY32 state;
    start_seeder_32(&state);
    wait_ms(1);
    while (1)
    {
        wait_us(30);
        pick = read_state_32(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

