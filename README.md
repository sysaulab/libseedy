# libSEEDY

## What?

A portable seedless random number generator.

## Where?

[sysaulab/libseedy](https://github.com/sysaulab/libseedy).
Look for README.md in each directories for more information.

- [src/](src/): Source files of the library itself
- [src/tools/](src/tools/): Source files of optional tools and prngs
- [Make/](Make/): Makefile folder to build the program in Linux, MacOS, *BSD, etc...
- [Make/tools/](Make/tools/): Makefile folder to build the optional tools.
- [Programs/](Programs/): Prebuilt programs for DOS and Windows

## Why?

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to operating systems lacking such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.
5. Learn to code C in a portable way.

## How?

By creating an unstable system and measuring its state at interval. Details in a [README](src/README.md) file.

## When?

Now!
