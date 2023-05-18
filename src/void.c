/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Commercial licence available upon request */

#include <stdio.h>

#define BUFSIZE 16384

int main(int argc, char** argv)
{
    char buffer[BUFSIZE];
    while(fread(&buffer, sizeof(char), BUFSIZE, stdin))
    {;}
}