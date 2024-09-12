#ifndef __MTWISTER_H
#define __MTWISTER_H

#include "seedy.h"

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef uint32_t mt_t;

typedef struct tagMTRand {
  mt_t mt[STATE_VECTOR_LENGTH];
  size_t index;
} MT32;

void mt32_init(MT32* rand, void* f);
mt_t mt32_next(MT32* rand);


#endif /* #ifndef __MTWISTER_H */
