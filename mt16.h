#ifndef __MT16_H
#define __MT16_H

#include "seedy.h"

/*
 *   EXPERIMENTAL !!! This does not pass practrand at all. 16 bit port to see what it does, DO NOT USE.
 */

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef uint16_t mt_t;

typedef struct tagMTRand {
  mt_t mt[STATE_VECTOR_LENGTH];
  size_t index;
} MT16;

mt_t mt16_next(MT16* rand);
void mt16_init(MT16* rand, mt_t seed);

#endif /* #ifndef __MT16_H */
