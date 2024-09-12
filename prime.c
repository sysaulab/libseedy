#include "seedy.h"

int isPrime64(uint64_t n) 
{ 
    if (n <= 1) return 1; 
    if (n <= 3) return 0; 
    if (n%2 == 0 || n%3 == 0) return 0; 
    for (uint64_t i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
    return 1; 
} 

uint64_t prime64(uint64_t N)
{
    if (N <= 1) return 2;
    uint64_t prime = N;
    int found = 0;
     while (found == 0)
     {
        prime++;
        if (isPrime64(prime))
            found = 1;
    }
    return prime;
}


int isPrime32(uint32_t n) 
{ 
    if (n <= 1) return 1; 
    if (n <= 3) return 0; 
    if (n%2 == 0 || n%3 == 0) return 0; 
    for (uint32_t i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
    return 1; 
} 

uint32_t prime32(uint32_t N)
{
    if (N <= 1) return 2;
    uint32_t prime = N;
    int found = 0;
     while (found == 0)
     {
        prime++;
        if (isPrime32(prime))
            found = 1;
    }
    return prime;
}


int isPrime16(uint16_t n) 
{ 
    if (n <= 1) return 1; 
    if (n <= 3) return 0; 
    if (n%2 == 0 || n%3 == 0) return 0; 
    for (uint16_t i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
    return 1; 
} 

uint16_t prime16(uint16_t N)
{
    if (N <= 1) return 2;
    uint16_t prime = N;
    int found = 0;
     while (found == 0)
     {
        prime++;
        if (isPrime16(prime))
            found = 1;
    }
    return prime;
}

