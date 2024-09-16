/* Demonstrator of the old wincrypt.h API */

#include "windows.h"
#include "wincrypt.h"
#include <stdio.h> 

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

int main(int argc, char** argv)
{
    char buf[1024];
    while (1)
    {
        wincrypt(buf, sizeof(buf));
        fwrite(buf, sizeof(buf), 1, stdout);
    }
    return 0;
}


