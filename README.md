# libSEEDY

## Downloads

### Windows

These binaries implements the fast [Shishua algorithm](https://espadrine.github.io/blog/posts/shishua-the-fastest-prng-in-the-world.html) initiated with seedy() 64 bit. Compiled vith Visual Studio 2022. 

- [Windows 64 bits](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.2/seedy-windows-x64.zip) ( Vista+ )
- [Windows 32 bits](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.2/seedy-windows-x86.zip) ( Vista+ )
- [Windows for ARM](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.2/seedy-windows-ARM64.zip) ( 8+ )

### Window Legacy

These binaries implement a portable [Mersenne Twister algorithm](https://github.com/ESultanik/mtwister) initiated with seedy(). Refactoring shishua.h to be "ancient C" compatible would require adapting the algorithm to 32 bits or reimplement using a software 64 bit library. Replacing it with the more conservative Mersenne Twister makes much more sense. Aside from replacing uin32_t with "unsigned long" only one declaration followed an instructions and needed be declared at the top of the function first.

Compiled with Visual C++ 6.

- [win32](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.2/seedy-windows-win32.zip) ( 95+ )

### DOS

The DOS port uses a timer-based entropy collection method. The seed generator itself has not been tested with serious statistical suites. It is very slow (32 bits per second) but provide a reliable method to initialize a generator.

The Mersenne Twister generator provides a cryptographic quality generator to DOS. Copy to c:\dev\random.exe if you want to be fancy! Tested with (Compaq) Dos 2.11 on 86box.

- [DOS](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.2/seedy-DOS.zip)

### Source code

- [Source Code](https://github.com/sysaulab/libseedy/archive/refs/heads/main.zip)

## Build instructions

You can read the build instructions in [INSTALL.md](INSTALL.md).

## About

Portable seedless random number generator.

Seedy creates and observe an unstable complex system built with 
threads and race conditions to create chaotic sequences in software. 
It works flawlessly on multicore CPU and offers acceptable quality 
on old platforms.

For security, this allow a user to bypass or supplement traditionnal 
channels who might be monitored. In case a flaw is discovered in the 
hardware generators, this supplement can act as a failsafe. For 
portability, programs can access entropy on older platforms that 
otherwise lack the hardware of software capability.

## History

I was pondering the nature of chaos I saw a bird in the sky. It 
inspired me to imagine the turbulences behind the wings and 
theorize about the nature of the interactions that make turbulences 
unpredictable. Could I replicate it using software? The image that 
appeared in my head was a system that was interdependent and 
loosely coupled.

I used many hash function as active processes and unmitigated shared 
memory to provide the “slack” in the coupling. I experimented with 
different topologies before adopting the obvious choice: a ring.

## Testing

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

## Conclusion

These results, coupled with the results I observed last year (video), 
make me confident about the validity of this method to create chaotic 
sequences portably and reliably. It can bring quality entropy to 
platforms that lack such features and peace of mind to everyone 
trying to erase their hard drive using truly-impossible-to-trace-data.

[https://www.youtube.com/watch?v=rrr7k4lb8Dg](https://www.youtube.com/watch?v=rrr7k4lb8Dg)
[https://github.com/sysaulab/libseedy](https://github.com/sysaulab/libseedy)
