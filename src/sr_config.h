/**
* @file config.h
* @author Sylvain Saucier <sylvain@sysau.com>
* @version 0.4.0
* @section LICENSE *
* This program is free software; you can redistribute it and/or
* modify it under the terms of the Affero GNU Public Licence version 3.
* Other licences available upon request.
* @section DESCRIPTION *
* Compile time configuration */

#define _SSRNG_BUFLEN       20480                    // buffer size in number of uint64_t (apparent size of pipe buffers in macos)
#define _SSRNG_FPS          5                      // FPS in ui

//#define _SRG_PRODUCTION     1                     // remove all non-necessary code for production distribution, mostly disables printf's and the seed options
//#define _SSRNG_EXPERIMENTAL 1                     // uncomment to enable alternate output mode, so far it dont look promising at all...


// DO NOT EDIT BELOW THIS POINT
#ifdef _SSRNG_EXPERIMENTAL
    #undef _SRG_PRODUCTION                          // disable production mode when experimental is enabled
    #define _SSRNG_EXP_SKIP 3                       // required for alternate output
#endif

#define _SSRNG_BUFSIZE      (_SSRNG_BUFLEN*8)       // buffer size in bytes for support commands, meant to match muffer of generators
