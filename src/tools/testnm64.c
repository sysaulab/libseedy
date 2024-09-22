#include "tools.h"
#include "algos/nm64.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main(int argc, char** argv)
{
    NM64 noise;
    nm64_init(&noise);
    
    while (1)
    {
        char command[1024];
        uint64_t count;
        
        printf("NM64:");
        fflush(stdout);
        fgets(command, 1023, stdin);

        if(command[0] == 'g')
        {
            /* GET */
            count = MIN(strtoull(command+1, NULL, 10), 1024);
            nm64_fill(&noise, (uint8_t*)command, count);
            for(int x = 0; x < count; x++)
            {
                printf("%02x", (uint8_t)command[x]);
            }
                printf("\n");
        }
        else if(command[0] == 'p')
        {
            /* PRINT */
            printf("iter:%llu\n", noise.iter);
            printf("seek_pos:%llu\n", noise.seek_pos);
        }
        else if(command[0] == 's')
        {
            /* SEEK */
            count = strtoull(command+1, NULL, 10);
            nm64_set_seek64(&noise, count);
        }
        else if(command[0] == 'q')
        {
            /* QUIT */
            break;
        }
        else
        {
            printf("g N: get N bytes from pos\n");
            printf("s N: seek to pos MN\n");
            printf("p: print\n");
            printf("q: Quit\n");
        }
    }
    return 0;
}

