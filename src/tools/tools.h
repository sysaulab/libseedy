#ifndef LIBSEEDY_COMMON_TOOLS_H_
#define LIBSEEDY_COMMON_TOOLS_H_

#include <libseedy/common.h>
#if defined(LIBSEEDY_OPT64)
    #include <libseedy/libseedy64.h>
#elif defined(LIBSEEDY_OPT32)
    #include <libseedy/libseedy32.h>
#elif defined(LIBSEEDY_OPT16)
    #include <libseedy/libseedy16.h>
#endif

#include <string.h>

/*
 *   AJUST FOR PORTS...
 */

void stdinput(uint8_t* buffer, size_t bytes)
{
    fread(buffer, bytes, sizeof(uint8_t), stdin);
}

void* parseinputgen(int argc, char** argv)
{
    if(argc > 1)
    {

        if(strcmp(argv[1], "stdin") == 0)
            return (void*)stdinput;
                    /*
                    The "&" removes a warning in old MS compilers,
                    The "(void*)" removes an error the & introduced in modern compilers.
                    */
    }
#   if defined(LIBSEEDY_OPT64)
        return (void*)seedy64;
#   elif defined(LIBSEEDY_OPT32)
        return (void*)seedy32;
#   elif defined(LIBSEEDY_OPT16)
        return (void*)seedy16;
#   else
        return stdinput;
#   endif
}

#endif
