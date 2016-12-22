
#ifndef SPECK
#define SPECK

#define SPECK_BLOCK_SIZE (64)
#define SPECK_WORD_SIZE (32)
#define SPECK_A (8)
#define SPECK_B (3)
#define SPECK_KEY_WORDS (3)
#if (SPECK_KEY_WORDS==3)
	#define SPECK_ROUNDS (26)
#elif (SPECK_KEY_WORDS==4)
	#define SPECK_ROUNDS (27)
#endif

#endif
