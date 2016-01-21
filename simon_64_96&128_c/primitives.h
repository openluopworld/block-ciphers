
#ifndef PRIMITIVES
#define PRIMITIVES

#include "constants.h"

/*
 * rotate shift left x by n bits
 */
static inline u32 rol (u32 x, const u8 n) {
	return (x<<n) | (x>>(SIMON_WORD_SIZE-n));
}

/*
 * rotate shift right x by n bits
 */
static inline u32 ror (u32 x, const u8 n) {
	return (x>>n) | (x<<(SIMON_WORD_SIZE-n));
}

/*
 * function f
 */
static inline u32 f(u32 x) {
	return (rol(x, 1) & rol(x, 8)) ^ rol(x, 2);
}

#endif
