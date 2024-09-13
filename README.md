# libSEEDY

Portable seedless random number generator.

## Applications

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to an operating system that lack such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.

### Source code

- [Source Code](https://github.com/sysaulab/libseedy/archive/refs/heads/main.zip)

## Build instructions

You can read the build instructions in [INSTALL.md](INSTALL.md).

## Downloads

### Windows

These binaries implements the fast [Shishua algorithm](https://espadrine.github.io/blog/posts/shishua-the-fastest-prng-in-the-world.html) initiated with seedy() 64 bit. Compiled vith Visual Studio 2022. 

- [Windows 64 bits](https://github.com/sysaulab/libseedy/blob/main/Platforms/seedy-windows-x64.zip) ( Vista+ )
- [Windows 32 bits](https://github.com/sysaulab/libseedy/blob/main/Platforms/seedy-windows-x86.zip) ( Vista+ )
- [Windows for ARM](https://github.com/sysaulab/libseedy/blob/main/Platforms/seedy-windows-ARM64.zip) ( 8+ )

### Window Legacy

These binaries implement a portable [Mersenne Twister algorithm](https://github.com/ESultanik/mtwister) 
initiated with seedy(). Refactoring shishua.h to be "ancient C" compatible 
would require adapting the algorithm to 32 bits or reimplement using a 
software 64 bit library. Replacing it with the more conservative Mersenne 
Twister made much more sense. Aside from replacing uin32_t with "unsigned 
long" only one declaration followed an instructions and needed be declared 
at the top of the function first.

Compiled with Visual C++ 6.

- [win32](https://github.com/sysaulab/libseedy/blob/main/Platforms/seedy-windows-win32.zip) ( 95+ )

### DOS

The DOS port uses a crude timer-based entropy collection method. 
The seed generator itself has not been tested with serious statistical 
suites. It is very slow (32 bits per second) but provide a reliable method 
to initialize a generator.

The Mersenne Twister generator provides a cryptographic quality generator 
to DOS. Copy to c:\dev\random.exe if you want to be fancy! Tested with 
(Compaq) Dos 2.11 on 86box.

- [DOS](https://github.com/sysaulab/libseedy/blob/main/Platforms/seedy-DOS.zip)

## About

Seedy creates and observe an unstable complex system built with 
threads and race conditions to create chaotic sequences in software. 
It works flawlessly on multicore CPU and offers acceptable quality 
on old platforms.

This allow a user to bypass or supplement traditionnal entropy supply
channels who may be monitored. With enough paranoia, any threat 
become credible. In a world of secure algorithms, I would try to 
sit on my adversary critical entropy supply stream. 

In case a flaw is discovered in the 
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

I used many hash functions as active processes, 
and unmitigated shared memory to provide the “slack” in the coupling. 
I experimented with different topologies before adopting the obvious 
choice: a ring. 

The result is three threads constantly accessing 
each other's memory with asking for permission. The configuration
makes it extremely unstable when the threads operate consurrently.


