# Prebuilt programs

I prepared two commands to demonstrate the effectiveness of libseedy:

- seedy: libseedy is sent to output unfiltered.
- fastrand: libseedy initiate a fast prng that is sent to the output.

## Windows

Windows Vista and up. ( libseedy -> shishua )

- [Windows/ARM64/](Windows/ARM64/)
- [Windows/x64/](Windows/x64/)
- [Windows/x86/](Windows/x86/)

## Window Legacy

Windows 95 and up. ( libseedy -> mersenne twister -> qx32 )

- [win32/](win32/)

## DOS

The DOS port uses a timer-based entropy gathering method as threads are not availables. The seed generator itself has not been tested with serious statistical suites. It is very slow (~32 bits per second) but provides a 
seemingly reliable method to initialize a generator. The Mersenne Twister generator provides a proof of concept of a port to DOS. Copy to c:\dev\random.exe if you want to be fancy! Tested with (Compaq) Dos 2.11 on 86box.

- [DOS/](DOS/)