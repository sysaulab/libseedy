# Testing and observations

PractRand and TestU01 are standard test suites used to measure the 
statistical "randomness" of PRNG. I use it is verify the 
characteristics of the chaotic generator in various environment.

As I tested it in different environments, it became clear to me 
that the synthetic chaos effect appear only when the threads are 
physically concurrent. When the coupling is not maintained, the 
strength of the effect is diminished. In the case of partial 
physical concurrency, the quality is diminished, but not completely.

The chaotic generator unfiltered passes both PractRand and Test U01 
Small Crush, when running on multicore architecture. This live 
testing has been conducted on a M1 cpu. Using it to generate a 2mb 
static map for the QXO64 generator (final stage of prandom). QXO 
uses a naive combining (xor) alrgorithm that reflects the quality 
of it's input. By using it, I tested ICM+QXO to 32TB with success.

I also generated seed files on an old iBook G4 (1 core) and core 
i5 first generation (2 cores, 4 threads). The results are excellent, 
the loss of cohesion (simulteneous treads) in the G4 is reflected in 
the abysmal results from PractRand and TestU01 Small Crunch. The fast 
tests (FPR FSC) is running the QXO generator and testing 2mb of seed 
at a time, or is ran live from my M1 Mac. The results given is the 
number of FAST tests with flawless results.

    ICM....................................
                    M1       G4        i5
    PractRand       PASS     FAIL      PASS
    Test U01        PASS     FAIL      PASS
    
    ICM+QXO................................
                    M1       G4        i5
    PractRand 1GB   94%      95%       93%
    Test U01 S.C.   97%      99%       96%

The fact that a monocore G4 produces such results by filtering 
its faulty output using only QXO surprises me. I expected the 
results to be much worse. Generating chaotic streams from user 
space has been quite an adventure. I hope these results will 
help alleviate some of the skepticism I encounter.

I published the results (folder with plenty of stuff) of the tests 
with the seeds collected from the G4 and old i5. on Github. The G4 
seed is truncated as it was too large for github (3gb). Look into 
the seed.txt files for the source URL.

[https://github.com/sysaulab/softrng](https://github.com/sysaulab/softrng)

[https://github.com/sysaulab/softrng-test](https://github.com/sysaulab/softrng-test)

These results, coupled with the results I observed last year (video), 
make me confident about the valitdity of the approach.

[https://www.youtube.com/watch?v=rrr7k4lb8Dg](https://www.youtube.com/watch?v=rrr7k4lb8Dg)

[https://github.com/sysaulab/libseedy](https://github.com/sysaulab/libseedy)
