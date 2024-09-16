# libSEEDY

Portable seedless random number generator.

## Files

- [src/](src/): Source files of the library itself
- [src/tools/](src/tools/): Source files of optional tools and prngs
- [Unix/](Unix/): Makefile folder to build the program in Linux, MacOS, *BSD, etc...
- [Unix/tools/](Unix/tools/): Makefile folder to build the optional tools.
- [Windows/](Windows/): Folder containing various Visual Studio projects.
- [Programs/](Programs/): Prebuilt programs for DOS and Windows

## Why?

1. Bring secret entropy to spies and hackers.
2. Provide entropy to devices lacking the capability.
3. Provide entropy to operating systems lacking such capability.
4. Supplement entropy in case of hardware generators failure or vulnerability.
5. Learn to code C in a portable way.

Read more about the algorithm in the [source code](src/)'s [README](src/README.md).

Many [tools](src/tools) and some of the [tests](src/help/testing.md) I conducted in the [src/help](src/help) folder.

## Prebuilt programs

Prebuilt programs are kept in 

## Build from source

Makefile in [Unix/](Unix/).

Visual Studio 2022: [VS2022/](VS2022/).

Visual Studio 6: [VS6/](VS6/).

## Gitgub

[sysaulab/libseedy](https://github.com/sysaulab/libseedy).
