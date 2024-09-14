#include "seedy.h"

/*
 *   AJUST FOR PORTS...
 */

#if defined(_OPTWINCRYPT)
void wincrypt(char* buffer, size_t bytes)
{
    HCRYPTPROV hCryptProv;
    if(FALSE == CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0))
    {
        if (NTE_BAD_KEYSET == GetLastError())
        {
            if (FALSE == CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_NEWKEYSET))
            {
                hCryptProv = NULL;
            }
        }
    }
    if(FALSE == CryptGenRandom(hCryptProv, (DWORD)bytes, buffer))
    {
        exit(1);
    }
    hCryptProv = NULL;
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
#       if defined(_OPTWINCRYPT)
            if(strcmp(argv[1], "win") == 0)
                return &wincrypt;
#       endif

#       if defined(_OPTARC4)
            if(strcmp(argv[1], "arc") == 0)
                return &arc4random_buf;
#       endif

        if(strcmp(argv[1], "stdin") == 0)
            return &stdinput;
        
    }
#   if defined(_OPT64)
        return &seedy64;
#   elif defined(_OPT32)
        return &seedy32;
#   elif defined(_OPT16)
        return &seedy16;
#   else
        return &stdinput;
#   endif
}
