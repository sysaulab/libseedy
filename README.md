# libSEEDY

Portable seedless random number generator.

## Why?

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to operating systems lacking such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.
5. Learn to code C in a portable way.

Read more about the [algorithms](src/help/algorithms.md), [commands](src/help/commands.md) and some of the [tests](src/help/testing.md) I conducted in the [src/help](src/help) folder.

## Latest version
You can download the latest stable version of this packages from [github](https://github.com/sysaulab/libseedy).

## Downloads

### Windows

Modern windows build.

- [Windows 64 bits](https://github.com/sysaulab/libseedy/blob/main/Programs/seedy-windows-x64.zip) ( Vista+ )
- [Windows 32 bits](https://github.com/sysaulab/libseedy/blob/main/Programs/seedy-windows-x86.zip) ( Vista+ )
- [Windows for ARM64](https://github.com/sysaulab/libseedy/blob/main/Programs/seedy-windows-ARM64.zip) ( 8+ )

### Window Legacy

Legacy Windows for compatibility.

- [win32](https://github.com/sysaulab/libseedy/blob/main/Programs/seedy-windows-win32.zip) ( 95+ )

### DOS

The DOS port uses a crude timer-based entropy collection method. 
The seed generator itself has not been tested with serious statistical 
suites. It is very slow (32 bits per second) but provide a reliable method 
to initialize a generator. The Mersenne Twister generator provides a cryptographic quality generator 
to DOS. Copy to c:\dev\random.exe if you want to be fancy! Tested with 
(Compaq) Dos 2.11 on 86box.

- [DOS](https://github.com/sysaulab/libseedy/blob/main/Programs/seedy-DOS.zip)

### Unix/Posix

Build from the source using the terminal.
The Makefile and readme are located in [Unix/](Unix/)

## Visual Studio

The solutions used to compile the Windows binaries. Visual Studio 6 is used to produce the legacy binaries. Visual Studio 2022 is used to produce the modern Windows binaries.

Visual Studio 2022 Solution: [Windows/](Windows/libseedy/libseedy.sln).

Visual Studio 6 Solution: [Win95/](Win95/seedy/seedy.dsw).

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

It was new year's eve and I was returning from Yellowknife. 
I was listening to a podcast about cryptography. 
A bird flying in the sky inspired me to theorize 
about the nature of unpredictability ny imagining 
the turbulences behind the wings. It seemed to me that loose 
concurrency would be key to achieve my goal. Could I replicate 
it using software? The image that appeared in my head was one 
of a system that was interdependent and loosely coupled.
Un-coordinated race conditions offers the possibility of loose coupling.
