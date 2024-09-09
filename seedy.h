#ifdef _WIN32
#if (_MSC_VER < 1930)
#include "stdint.h"
#define SEEDY_WIDTH 4
typedef uint32_t seedy_t;
#else
#include <stdint.h>
#define SEEDY_WIDTH 8
typedef uint64_t seedy_t;
#endif
#include <windows.h>
#else
#include <stdint.h>
#define SEEDY_WIDTH 8
typedef uint64_t seedy_t;
#include <pthread.h>
#endif

void seedy(uint8_t* buffer, size_t bytes);
