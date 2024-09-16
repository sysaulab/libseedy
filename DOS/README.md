# DOS port

## Notes

The DOS port uses a timer-based entropy gathering method as threads are not availables. The seed generator itself has not been tested with serious statistical suites. It is very slow (~32 bits per second) but provides a seemingly reliable method to initialize a generator. The Mersenne Twister generator provides a proof of concept of a port to DOS. Copy to c:\dev\random.exe if you want to get fancy.

I consider creating a Terminate and Stay Resident version of it.