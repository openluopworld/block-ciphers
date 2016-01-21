
#ifndef PRIMITIVES
#define PRIMITIVES

#include "constants.h"

/*
 * rotate shift left x by n bits
 */
#define rol(x, n) ((x<<n) | (x>>(SIMON_WORD_SIZE-n)))

/*
 * rotate shift right x by n bits
 */
#define ror(x, n) ((x>>n) | (x<<(SIMON_WORD_SIZE-n)))

/*
 * function f
 */
#define f(x) ((rol(x, 1)&rol(x, 8)) ^ rol(x, 2))

#endif
