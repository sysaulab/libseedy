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

## Prebuilt programs

I prepared two commands to demonstrate the effectiveness of libseedy: seedy and fastrand. Seedy exposes a seedy generator directly to the user while fastrand uses it to initiate a fast prng.

Prebuilt programs are kept in the [Programs/](Programs/README.md) folder.

## Build from source

Makefile in [Unix/](Unix/).

Visual Studio 2022: [VS2022/](VS2022/).

Visual Studio 6: [VS6/](VS6/).

## About

Seedy create and observe an unstable complex system built with threads and race conditions to create chaotic sequences in software. It works flawlessly on multicore CPU and offers acceptable quality on old platforms.

This allow a user to bypass or supplement traditionnal entropy supply channels who may be monitored. With enough paranoia, any threat become credible. In a world of secure algorithms, I would try to sit on my adversary critical entropy supply stream. 

In case a flaw is discovered in the hardware generators, this supplement can act as a failsafe. For portability, programs can access entropy on older platforms that otherwise lack the hardware of software capability.

## History

It was new year's eve and I was returning from Yellowknife. I was listening to a podcast about cryptography. A bird flying in the sky inspired me to theorize about the nature of unpredictability ny imagining the turbulences behind the wings. It seemed to me that loose concurrency would be key to achieve my goal. Could I replicate it using software? The image that appeared in my head was one of a system that was interdependent and loosely coupled. Un-coordinated race conditions offers the possibility of loose coupling.
