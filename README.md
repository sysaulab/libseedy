# libSEEDY

[Windows downloads](Programs/Windows/)

## What?

A portable seedless random number generator.

## Where?

Look for README.md in each directories for more information.

- [src/](src/): Source files of the library itself
- [src/tools/](src/tools/): Source files of optional tools and prngs
- [src/DOS/](src/DOS/): DOS Port, Turbo-C.
- [Make/](Make/): Makefile folder to build the program in Linux, MacOS, *BSD, etc...
- [Extras/](Extras/): Makefile folder to build the optional tools.
- [Programs/](Programs/): Prebuilt programs for DOS and Windows
- VS.zip: Some Visual Studio Project files.

Website: [sysaulab/libseedy](https://github.com/sysaulab/libseedy).

## Why?

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to operating systems lacking such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.
5. Learn to code C in a portable way.

## How?

By creating an unstable system and measuring its internal state at intervals.

More details in a [README](src/README.md) file.

## Who?

sylvain@sysau.com
