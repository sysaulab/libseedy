/**
* @file softrng.c
* @author Sylvain Saucier <sylvain@sysau.com>
* @version 0.4.0
* @section LICENSE *
* This program is free software; you can redistribute it and/or
* modify it under the terms of the Affero GNU Public Licence version 3.
* Other licences available upon request.
* @section DESCRIPTION *
* Main program */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "libicm.h"
#include "common.h"
#include "sr_config.h"

//CLASS FLAGS 
#define I 1 //requires input from source
#define O 2 //requires target tp receive output 
#define P 4 //prng generator, seed goes in sequence goes out
#define T 8 //target commands, where data goes to die, typical test or file...
#define S 16 //seed commands, my or may not require input.
#define F 32 //filter command, middle of crypto chain
#define E 128 //external command, depends on another package


typedef uint16_t item_id_t;
typedef uint8_t item_class_t;

typedef struct {
    item_id_t id;
    item_class_t class;
    char* name;
    char* desc;
    char* cmd;
} item_t;

typedef struct {
    size_t capacity;
    size_t size;
    item_t* list;
} item_list_t;

item_list_t db;
item_id_t nextid = 0;
int save_config = 0; //requires root access

item_class_t class_from_str(char* str)
{
    item_class_t answer = 0;
    for(int x=0;!str[x];x++)
    {
        switch (str[x]) {
        case 'I': answer |= I; break;
        case 'O': answer |= O; break;
        case 'P': answer |= P; break;
        case 'T': answer |= T; break;
        case 'S': answer |= S; break;
        case 'F': answer |= F; break;
        case 'E': answer |= E; break;
        }
    }
    return answer;
}

char* dup_str(char* src)
{
    int l = strnlen(src, 1024);
    char* answer = malloc((1 + l) * sizeof(char));
    memcpy(answer, src, l);
    return answer;
}

item_id_t add_item_to_list(item_class_t class, char* name, char* desc, char* cmd, item_list_t* list)
{
    if(list->capacity < 1){
        list->capacity = 16;
        list->list = malloc(list->capacity * sizeof(item_t));
        if(list->list == NULL) exit(EXIT_FAILURE);}
    if(list->capacity - list->size < 1){
        list->capacity *= 2;
        list->list = realloc(list->list, list->capacity * sizeof(item_t));
        if(list->list == NULL) exit(EXIT_FAILURE);}
    list->list[list->size].id = nextid++;
    list->list[list->size].class = class;
    list->list[list->size].name = dup_str(name);
    list->list[list->size].cmd = dup_str(cmd);
    list->list[list->size].desc = dup_str(desc);
    list->size++;
    return nextid;
}

void manual()
{
    printf("sofTRNG is composed of simple specialized commands.\n");
    printf("This program is here to help you learn to use the tools properly.\n");
    printf("There are commands to generate a seed, filter a stream, PRNGs.\n");
    printf("When combined they can be powerful tools.\n");
    printf("\n");
    printf("PREFIX           DESCRIPTION           SHORTS\n");
    printf("srs_             Seeding               seed\n");
    printf("srg_             Generators            prng\n");
    printf("srf_             Filters               bench keep void hex\n");
    printf("srt_             Tests                 bspec32\n");
    printf("sr_              Non-chainable tools   \n");
    printf("                 Complete solution     rng\n");
    printf("\n");
    printf("COMMAND          DESCRIPTION\n");
    printf("srs_icm64        ICM seed generator.\n");
    printf("srs_icm64mix     ICM seed generator combined with standard input.\n");
    printf("srg_roxo64-8k    Rotate XOR 64bit +8k - Smaller (1mb)\n");
    printf("srg_roxo64-8kc   Rotate XOR 64bit +8k - Faster\n");
    printf("srf_bench        Display speed and size of piped data (stdin > stdout)\n");
    printf("srf_hex          Portable hex viewer, no formatting\n");
    printf("srf_limitsize    Read input and write a maximum amount of bytes\n");
    printf("srf_void         Portable replacement for /dev/null in case it's missing\n");
    printf("srt_bspec32      Used to test ICM in reset mode, finds all 32bits integers\n");
    printf("srt_bspec32_ref  Computes the perfect probability curve of bspec32\n");
    printf("sr_sleepinfo     Simple test collecting information about sleep functions\n");
    printf("\n");
}

int hard_alias(char* a, char* c)
{
    char cmd[200];
    char local_bin[200] = "/usr/local/bin/";
    strncat(local_bin, a, 20);
    FILE* alias_file = fopen(local_bin, "w");
    if(!alias_file) return 0;
    fprintf(alias_file, "#!/bin/sh\n%s", c);
    fclose(alias_file);
    strncpy(cmd, "sudo chmod +x ", 20);
    strncat(cmd, local_bin, 20);
    system(cmd);
    return 1;
}

int soft_alias(char* a, char* c)
{
    char cmd[200];
    strncpy(cmd, "alias ", 7);
    strncat(cmd, a, 20);
    strncat(cmd, "=\"", 3);
    strncat(cmd, c, 20);
    strncat(cmd, "\"", 2);
    system(cmd);
    return 1;
}

int alias(char* a, char* c)
{
    if(!hard_alias(a, c))
        return soft_alias(a, c);
    return 0;
}

int cmd_exist(char* c){
    char cmd[1024] = "command -v ";
    strncat(cmd, c, 1000);
    FILE* out = popen(cmd, "r");
    fread(&cmd, 1023, sizeof(uint8_t), out);
    pclose(out);
    if( cmd[0] == '\0' ) return 0;
    if( cmd[0] == '/' ) return 1;
    return -1;
}



void remove_newline(char* str)
{
    int x = 0;
    int run = 1;
    while (run){
        switch (str[x]){
            case '\n':
            case '\r':
            case '\0':
                str[x] = '\0';
                run = 0;}
        x++;}
}

void escape_space(char* str, char* dest)
{
    int x, y = 0;
    while (str[x] != '\0')
    {
        if(str[x] == ' ')
            dest[y++] = '\\';
        dest[y++] = str[x++];
    }
    dest[y] = '\0';
}

int scan(item_list_t* list, char* db_path)
{
    FILE* db_file = fopen(db_path, "r");
    if(!db_file)
    {
        fprintf(stderr, "Error opening database file, please reinstall softrng.\n");
        return 0;
    }
    ssize_t read;
    uint64_t errors = 0;
    size_t len = 4096;
    char* line = malloc(len + 1);
    if(!line) exit(EXIT_FAILURE);
    printf("#COMMANDS");

    char* target_cmd = calloc(1025, sizeof(char)); if(!target_cmd) exit(EXIT_FAILURE);

    char* alias_name = calloc(1025, sizeof(char)); if(!alias_name) exit(EXIT_FAILURE);
    char* alias_class= calloc(1025, sizeof(char)); if(!alias_class) exit(EXIT_FAILURE);
    char* alias_cmd  = calloc(1025, sizeof(char)); if(!alias_cmd) exit(EXIT_FAILURE);
    char* alias_cmd_dirty  = calloc(1025, sizeof(char)); if(!alias_cmd) exit(EXIT_FAILURE);
    char* alias_desc = calloc(1025, sizeof(char)); if(!alias_desc) exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, db_file)) != -1)
    {
        if(line[0] == ' ')
        {
            int x = 1;

            for(int start = x;line[x] != '\0';)
                if(line[x] != '|'){
                    alias_name[x-start] = line[x];
                    x++;}
                else{
                    alias_name[x-start] = '\0';
                    x++;
                    break;}

            for(int start = x;line[x] != '\0';)
                if(line[x] != '|'){
                    alias_class[x-start] = line[x];
                    x++;}
                else{
                    alias_name[x-start] = '\0';
                    x++;
                    break;}

            for(int start = x;line[x] != '\0';)
                if(line[x] != '|'){
                    alias_cmd_dirty[x-start] = line[x];
                    x++;}
                else{
                    alias_name[x-start] = '\0';
                    x++;
                    break;}

            for(int start = x;line[x] != '\0';)
                if(line[x] != '|'){
                    alias_desc[x-start] = line[x];
                    x++;}
                else{
                    alias_name[x-start] = '\0';
                    x++;
                    break;}

            escape_space(alias_cmd_dirty, alias_cmd);
            printf("  name %s\n", alias_name);
            printf("  class %s\n", alias_class);
            printf("  cmd %s\n", alias_cmd_dirty);
            printf("  cmdc %s\n", alias_cmd);
            printf(" %s\n", alias_name);
            if(!strncmp(target_cmd, alias_name, 1024))
            {
                soft_alias(alias_name, alias_cmd);
                if(save_config)
                    hard_alias(alias_name, alias_cmd);
            }
            add_item_to_list(class_from_str(alias_class), alias_name, alias_desc, alias_cmd, &db);
        }
        else
        {
            remove_newline(line);
            strncpy(target_cmd, line, 1024);
fprintf(stderr, "TARGET:%s\n", target_cmd);
            continue;
        }
    }
    printf("\n");
    if(errors > 0) fprintf(stderr, "To correct errors, reinstall softrng.\n");
    return !errors;
}

void view_commands(item_list_t* list, item_class_t filters)
{
    if(!filters)
    {
        for(int x = 0; x < list->size; x++)
        {
            item_t* l = &(list->list[x]);
            printf("%04x %02x - %s(%s)\n", l->id, l->class, l->name, l->cmd);
        }
    }
    else
    {
        for(int x = 0; x < list->size; x++)
        {
            item_t* l = &(list->list[x]);
            printf("%02x %02x\n", filters, l->class);
            if(filters & l->class)
                printf("%04x %02x - %s(%s)\n", l->id, l->class, l->name, l->cmd);
        }
    }
fprintf(stderr, "%02x\n", filters);
    for(int x = 0; x < list->size; x++)
    {
        item_t* l = &(list->list[x]);
        fprintf(stderr, "x = %i\n", x);
        if(filters & l->class)
        {
            printf("%04x %02x - %s(%s)\n", l->id, l->class, l->name, l->cmd);
        }
    }
}

int reset_default()
{
    uint64_t err = 0;
    if(!alias("srng", "srs_icm64mix</dev/random")) err++;
    if(!alias("prng", "srg_roxo64-8kc")) err++;
    if(!alias("rng",  "srng|prng")) err++;
    if(err)
        return 0;
    return 1;
}

int main_menu()
{
    menu:
    printf("\nMain menu\n");
    printf("1. Change defaults (root)\n");
    printf("2. Help, read the manual\n");
    printf("3. View recognized commands\n");
    printf("q. Exit\n");
    printf("<enter>");
    fflush(stdout);

    while(1)
    {
        switch( getc(stdin) ) {
        case '1':
            if(!reset_default()) printf("Temporary shortcuts in effect for current session.\n");
            else printf("Permanent shortcuts saved to disk.\n");
            printf("<enter>");
            break;
        case '2':
            manual();
            printf("<enter>");
            break;
        case '3':
            view_commands(&db, 0);
            printf("<enter>");
            break;
        case '4':
            break;
        case '5':
            break;

        case '9':
            break;
        case '\n':
        case '\r':
            printf("");
            break;
        case 'q':
        case 'Q':
        case '\e':
            return EXIT_SUCCESS;
        default :
            goto menu;
        }
    }
}

int main(int argc, const char * argv[]){
    printf("softrng #VERSION 0.4.0 #LICENCE ");
    printf("This program is free software; you can redistribute it and/or modify it under the terms of the Affero GNU Public Licence version 3. ");
    scan(&db, "/etc/softrng/commands.db");
    if(!main_menu())
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}