#include "seedy.h"

/*
 *   AJUST FOR PORTS...
 */




#if defined(_WIN32)
void wincrypt(uint8_t* buffer, size_t bytes)
{
    HCRYPTPROV hCryptProv;
    if(CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0));
    else
    {
        exit(1);
    }
    if(CryptGenRandom(hCryptProv, (DWORD)bytes, buffer));
    else
    {
        fprintf(stderr, "Error in CryptGenRandom()\n");
        exit(1);
    }
}
#endif






void stdinput(uint8_t* buffer, size_t bytes)
{
    fread(buffer, bytes, sizeof(uint8_t), stdin);
}

void* parseinputgen(int argc, char** argv)
{
    if(argc > 1)
    {
        #ifdef _WIN32
            if(strcmp(argv[1], "wincrypt") == 0)
                return wincrypt;

#       endif

#       if defined(_OPTARC4)
            if(strcmp(argv[1], "arc4") == 0)
                return arc4random_buf;
#       endif

#       if defined(_OPT64)
            if(strcmp(argv[1], "seedy64") == 0)
                return seedy64;
#       endif
#       if defined(_OPT32)
            if(strcmp(argv[1], "seedy32") == 0)
                return seedy32;
#       endif
#       if defined(_OPT16)
            if(strcmp(argv[1], "seedy16") == 0)
                return seedy16;
#       endif

        if(strcmp(argv[1], "stdin") == 0)
            return stdinput;
        
    }

#   if defined(_OPT64)
        return seedy64;
#   elif defined(_OPT32)
        return seedy32;
#   elif defined(_OPT16)
        return seedy16;
#   else
        return stdinput;
#   endif
}
