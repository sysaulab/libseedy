#include "seedy.h"

int main(int argc, char** argv)
{
    uint16_t pick;
    SEEDY16 state;
    start_seeder_16(&state);
    wait_ms(1);
    while (1)
    {
        wait_us(30);
        pick = read_state_16(&state);
        fwrite(&pick, sizeof(pick), 1, stdout);
    }
    return 0;
}

