# libSEEDY

## Install

### Windows

Get programs and a DLL pre-compiled for various Windows platforms.

- [Windows 64 bits](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.1/seedy-windows-x64.zip) ( Vista+ )
- [Windows 32 bits]() ( Vista+ )
- [Windows for ARM](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.1/seedy-windows-ARM64.zip) ( 8+ )
- [win32](https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.1/seedy-windows-win32.zip) ( 95+ )

## Build

### Windows

Get programs and a DLL pre-compiled for various Windows platforms.

- Visual Studio project is located in VS2022/seedy

### Posix (Linux, MacOS, BSD's, etc.)

1. Download source [seedy-1.0.1]([https://github.com/sysaulab/libseedy/releases/download/libseedy-1.0.1/seedy-windows-x64.zip](https://github.com/sysaulab/libseedy/archive/refs/tags/libseedy-1.0.1.tar.gz))
2. make
3. make install

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
appeared in my head was a system that was interdependent, 
but loosely coupled.

I used many hash function as active processes and unmitigated shared 
memory to provide the “slack” in the coupling. I experimented with 
different topologies before adopting the obvious choice: a ring. 
The physical analogy that comes to my mind, is one of a circle in 
which three balls are launched at high velocity towards the centre. 

In a perfect universe, we can imagine the balls would hit each other 
precisely at the same point in space and time when they get to the 
centre, then rebound back precisely and hit the wall behind them, then 
go back towards the centre again and repeat the same motions forever. 
In an imperfect universe, we would observe divergences after a few cycles.

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

The prandom program provided here uses the ICM-ROXO-QXO combination 
which provides additional filtering and requires less initial 
entropy. I hope this demo was as fun to run as it was fun to write.

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
