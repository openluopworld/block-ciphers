
#ifndef MACROS
#define MACROS

#include "constants.h"

/*
 * rotate shift left x by n bits
 */
#define rol(x, n) (((x)<<(n)) | ((x)>>(SPECK_WORD_SIZE-(n))))

/*
 * rotate shift right x by n bits
 */
#define ror(x, n) (((x)>>(n)) | ((x)<<(SPECK_WORD_SIZE-(n))))

/*
 * round function
 */
#define roundFunction(x, y, k)		\
	x = (ror((x), SPECK_A) + (y)) ^ (k);	\
	y = rol((y), SPECK_B) ^ (x)

/*
 * invert round function
 */
#define invertRoundFunction(x, y, k)	\
	y = ror( (x)^(y), SPECK_B);	\
	x = rol( (((x)^(k))-(y)), SPECK_A)
#endif
