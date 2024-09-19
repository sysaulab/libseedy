# Prebuilt programs

I prepared three commands to demonstrate the effectiveness of libseedy:

- seedy: libseedy unfiltered.
- fastrand: libseedy with Shishua.
- saferand: libseedy with ChaCha-20.

## Windows

Windows Vista and up. ( libseedy -> shishua )

- [Windows/ARM64/](Windows/ARM64/)
- [Windows/x64/](Windows/x64/)
- [Windows/x86/](Windows/x86/)

## Window Legacy

Windows 95 and up. ( libseedy -> mersenne twister -> qx32 )

- [win32/](win32/)

## Haiku OS

x64 build of a GUI program showing a seed computed by libseedy. A basic driver for a true RNG driver named seedy has compiled with success but remains untested.

- [Haiku/](Haiku/) (64 bit build)

## DOS

Tested with (Compaq) Dos 2.11 on 86box.

- [DOS/](DOS/)