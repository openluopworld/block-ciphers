
/*
 * inline is used to modify the function. But the compiler may ignore it.
 */

#ifndef PRIMITIVES
#define PRIMITIVES

#include "constants.h"

/*
 * rotate shift left x by n bits
 */
static inline u32 rol (u32 x, const u8 n) {
	return ((x<<n) | (x>>(SPECK_WORD_SIZE-n)));
}

/*
 * rotate shift right x by n bits
 */
static inline u32 ror (u32 x, const u8 n) {
	return ((x>>n) | (x<<(SPECK_WORD_SIZE-n)));
}

/*
 * round function
 */
static inline void roundFunction(u32 *x, u32 *y, const u32 *k) {
	*x = (ror(*x, SPECK_A) + *y) ^ *k;
	*y = rol(*y, SPECK_B) ^ *x;
}

/*
 * invert round function
 */
static inline void invertRoundFunction(u32 *x, u32 *y, const u32 *k) {
	*y = ror(*x ^ *y, SPECK_B);
	*x = rol((*x ^ *k - *y), SPECK_A);
}

#endif
