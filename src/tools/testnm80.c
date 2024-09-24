#include "tools.h"
#include "algos/nm80.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
    NM80 noise;
    nm80_init(&noise, "./nm80.bin");

    uint128_t user_offset;
    
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
            nm80_fill(&noise, (uint8_t*)command, count, user_offset);
            for(int x = 0; x < count; x++)
            {
                printf("%02x", (uint8_t)command[x]);
            }
            user_offset += count;
                printf("\n");
        }
        else if(command[0] == 'p')
        {
            /* PRINT */
            printf("cur_iter:%llu\n", noise.iters[GETBANK(user_offset)]);
            printf("bank:%u\n", (uint16_t)GETBANK(user_offset));
            printf("seek_pos:%016llx%016llx\n", (uint64_t)(user_offset>>64), (uint64_t)user_offset);
        }
        else if(command[0] == 's')
        {
            /* SEEK MUST ADAPT TO 128+ bits*/
            user_offset = hexU128(command+1);
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

