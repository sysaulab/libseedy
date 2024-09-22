#include "tools.h"
#include "algos/nm80.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
    NM80 noise;
    nm80_init(&noise);
    
    while (1)
    {
        char command[1024];
        uint64_t count;
        
        printf("NM80:");
        fflush(stdout);
        fgets(command, 1023, stdin);

        if(command[0] == 'g')
        {
            /* GET */
            count = MIN(strtoull(command+1, NULL, 10), 1024);
            nm80_fill(&noise, (uint8_t*)command, count, noise.seek_bank);
            for(int x = 0; x < count; x++)
            {
                printf("%02x", (uint8_t)command[x]);
            }
                printf("\n");
        }
        else if(command[0] == 'p')
        {
            /* PRINT */
            printf("cur_iter:%llu\n", noise.iters[noise.seek_bank>>3]);
            printf("bank:%u\n", noise.seek_bank);
            printf("seek_pos:%llu\n", noise.seek_pos);
        }
        else if(command[0] == 's')
        {
            /* SEEK */
            count = strtoull(command+1, NULL, 10);
            nm80_set_seek64(&noise, count);
        }
        else if(command[0] == 'b')
        {
            /* BANK */
            count = MIN(strtoull(command+1, NULL, 10), 65535);
            nm80_set_bank(&noise, count);
        }
        else if(command[0] == 'w')
        {
            /* SAVE */
            FILE* state_file = fopen("state.bin", "w");
            fwrite(&noise, sizeof(noise), 1, state_file);
            fclose(state_file);
        }
        else if(command[0] == 'r')
        {
            /* SAVE */
            FILE* state_file = fopen("state.bin", "r");
            fread(&noise, sizeof(noise), 1, state_file);
            fclose(state_file);
        }
        else if(command[0] == 'q')
        {
            /* QUIT */
            break;
        }
        else
        {
            printf("g x: get x bytes from pos (1-1024)\n");
            printf("s x: Seek to pos x (0x0 to 0xffffffffffffffff)\n");
            printf("b x: Set active bank x (0x0 to 0xffff)\n");
            printf("p: Print\n");
            printf("q: Quit\n");
        }
    }
    return 0;
}

