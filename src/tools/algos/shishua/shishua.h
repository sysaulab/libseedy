#ifndef SHISHUA_H
#define SHISHUA_H

#define SHISHUA_TARGET_SCALAR 0
#define SHISHUA_TARGET_AVX2 1
#define SHISHUA_TARGET_SSE2 2
#define SHISHUA_TARGET_NEON 3



#ifndef SHISHUA_TARGET
#  if defined(__AVX2__) && (defined(__x86_64__) || defined(_M_X64))
#    define SHISHUA_TARGET SHISHUA_TARGET_AVX2
#  elif defined(__x86_64__) || defined(_M_X64) || defined(__SSE2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2)
#    define SHISHUA_TARGET SHISHUA_TARGET_SSE2
#  elif (defined(__ARM_NEON) || defined(__ARM_NEON__)) && defined(__clang__)
#    define SHISHUA_TARGET SHISHUA_TARGET_NEON
#  else
#    define SHISHUA_TARGET SHISHUA_TARGET_SCALAR
#  endif
#endif

#if SHISHUA_TARGET == SHISHUA_TARGET_AVX2
#  include "shishua-avx2.h"
#elif SHISHUA_TARGET == SHISHUA_TARGET_SSE2
#  include "shishua-sse2.h"
#elif SHISHUA_TARGET == SHISHUA_TARGET_NEON
#  include "shishua-neon.h"
#else 

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

typedef struct prng_state {
  uint64_t state[16];  
  uint64_t output[16]; 
  uint64_t counter[4]; 
} prng_state;

#if defined(__GNUC__) || defined(_MSC_VER)
#  define SHISHUA_RESTRICT __restrict
#else
#  define SHISHUA_RESTRICT
#endif

void prng_write_le64(void *dst, uint64_t val) {
#if defined(SHISHUA_NATIVE_ENDIAN) \
   || defined(_WIN32) \
   || (defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) \
   || defined(__LITTLE_ENDIAN__)
  memcpy(dst, &val, sizeof(uint64_t));
#else
  uint8_t *d = (uint8_t *)dst;
  for (size_t i = 0; i < 8; i++) {
    d[i] = (uint8_t)(val & 0xff);
    val >>= 8;
  }
#endif
}

void prng_gen(prng_state *SHISHUA_RESTRICT state, uint8_t *SHISHUA_RESTRICT buf, size_t size)
{
  size_t i, j, k;
  uint64_t u_lo, u_hi;
  const uint8_t shuf_offsets[16] = { 2,3,0,1,5,6,7,4,3,0,1,2,6,7,4,5 };
  uint8_t *b = buf;
  assert((size % 128 == 0) && "buf's size must be a multiple of 128 bytes.");

  for (i = 0; i < size; i += 128) {
    if (buf != NULL) {
      for (j = 0; j < 16; j++) {
        prng_write_le64(b, state->output[j]); b += 8;
      }
    }
    for (j = 0; j < 2; j++) {
      uint64_t *s = &state->state[j * 8];  
      uint64_t *o = &state->output[j * 4]; 
      uint64_t t[8]; 

      for (k = 0; k < 4; k++) {
        s[k + 4] += state->counter[k];
      }


      for (k = 0; k < 8; k++) {
        t[k] = (s[shuf_offsets[k]] >> 32) | (s[shuf_offsets[k + 8]] << 32);
      }

      for (k = 0; k < 4; k++) {
      
        u_lo = s[k + 0] >> 1;
        u_hi = s[k + 4] >> 3;

        s[k + 0] = u_lo + t[k + 0];
        s[k + 4] = u_hi + t[k + 4];

        o[k] = u_lo ^ t[k + 4];
      }
    }

    for (j = 0; j < 4; j++) {
      state->output[j +  8] = state->state[j + 0] ^ state->state[j + 12];
      state->output[j + 12] = state->state[j + 8] ^ state->state[j +  4];
      state->counter[j] += 7 - (j * 2); // 7, 5, 3, 1
    }
  }
}
#undef SHISHUA_RESTRICT

static uint64_t phi[16] = {
  0x9E3779B97F4A7C15, 0xF39CC0605CEDC834, 0x1082276BF3A27251, 0xF86C6A11D0C18E95,
  0x2767F0B153D27B7F, 0x0347045B5BF1827F, 0x01886F0928403002, 0xC1D64BA40F335E36,
  0xF06AD7AE9717877E, 0x85839D6EFFBD7DC6, 0x64D325D1C5371682, 0xCADD0CCCFDFFBBE1,
  0x626E33B8D04B4331, 0xBBF73C790D94F79D, 0x471C4AB3ED3D82A5, 0xFEC507705E4AE6E5,
};

void prng_init(prng_state *s, uint64_t seed[4])
{
  size_t i, j;
  memset(s, 0, sizeof(prng_state));
# define STEPS 1
# define ROUNDS 13

  memcpy(s->state, phi, sizeof(phi));
  for (i = 0; i < 4; i++) {
    s->state[i * 2 + 0] ^= seed[i];
    s->state[i * 2 + 8] ^= seed[(i + 2) % 4];
  }
  for ( i = 0; i < ROUNDS; i++) {
    prng_gen(s, NULL, 128 * STEPS);
    for ( j = 0; j < 4; j++) {
       s->state[j+ 0] = s->output[j+12];
       s->state[j+ 4] = s->output[j+ 8];
       s->state[j+ 8] = s->output[j+ 4];
       s->state[j+12] = s->output[j+ 0];
    }
  }
# undef STEPS
# undef ROUNDS
}
#endif
#endif
