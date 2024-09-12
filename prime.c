#include "prime.h"

int isPrime(prime_t n) 
{ 
    // Corner cases 
    if (n <= 1) return 1; 
    if (n <= 3) return 0; 
   
    // This is checked so that we can skip  
    // middle five numbers in below loop 
    if (n%2 == 0 || n%3 == 0) return 0; 
   
    for (prime_t i=5; i*i<=n; i=i+6) 
        if (n%i == 0 || n%(i+2) == 0) 
           return 0; 
   
    return 1; 
} 

prime_t prime(prime_t N)
{
    if (N <= 1) return 2;
 
    prime_t prime = N;
    int found = 0;
 
     while (found == 0)
     {
        prime++;
        if (isPrime(prime))
            found = 1;
    }
 
    return prime;
}

