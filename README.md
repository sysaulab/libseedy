# libSEEDY

[Downloads](Programs/)

## What...

...is it? A portable seedless random number generator.

...is new? Support for BeOS!

## Where?

Look for README.md in each directories for more information.

- [src/](src/): Source files of the library itself
- [src/tools/](src/tools/): Source files of optional tools and prngs
- [src/DOS/](src/DOS/): DOS Port, Turbo-C.
- [Make/](Make/): Makefile folder to build the program in Linux, MacOS, *BSD, etc...
- [Extras/](Extras/): Makefile folder to build the optional tools.
- [Programs/](Programs/): Prebuilt programs for DOS and Windows
- VS.zip: Visual Studio project files.
- BE.zip: Haiku OS/BeOS project files.

Website: [sysaulab/libseedy](https://github.com/sysaulab/libseedy).

## Why?

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to operating systems lacking such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.
5. Learn to code C in a portable way.

## How?

By creating an unstable system and measuring its internal state at intervals.

More details in a [README](src/libseedy/README.md).

## When?

1. I had the idea December 31st 2022.
2. Coded a bad proof of concept in early january.
3. Sent my bad code to Professor Brown of Dieharder in early January.
4. Received a brilliant lecture about entropy from Professor Brown in early January.
5. I improved the algorithm until April using practrand as reference benchmark.
6. Validated "32 bit completeness" on first pick using a modified version of the library and in may 2022. The slow nature of the generator tested my own patience.
7. Major rehaul and cleanup of the code in September 2024, including native a port to Windows.

## Who?

sylvain AT sysau DOT com
