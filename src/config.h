/*  Copyright 2023, All rights reserved, Sylvain Saucier
    sylvain@sysau.com
    Distributed under Affero GNU Public Licence version 3
    Other licences available upon request */
    
#define _SSRNG_BUFLEN 2048          // buffer size in number of uint64_t
#define _SSRNG_BUFSIZE 16384        // buffer size in bytes (support commands)
#define _SSRNG_THREADS 3            // number of threads and nodes
#define _PRNG_THREADS 3            // number of generators in the fast prng (quick ssd secure erase)


#define _SSRNG_CORE 1               // remove all non-necessary code for production distribution

//#define _SSRNG_EXPERIMENTAL 1 //uncomment to enable alternate output mode
#ifdef _SSRNG_EXPERIMENTAL
#undef _SSRNG_CORE
#define _SSRNG_EXP_SKIP 3         //required for alternate output
#endif