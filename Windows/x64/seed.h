#ifdef _WIN32
#if (_MSC_VER < 1940)
#include "stdint.h"
#else
#include <stdint.h>
#endif
#include <windows.h>
#else
#include <stdint.h>
#include <pthread.h>
#endif

void seed(uint64_t* buffer, uint64_t count);
