
#ifndef SIMON
#define SIMON

#include <stdint.h>

#define SIMON_BLOCK_SIZE (64)
#define SIMON_WORD_SIZE (32)
#define SIMON_CONST_C (0xfffffffc)
#define SIMON_KEY_WORDS (3)
#if (SIMON_KEY_WORDS==3)
	#define SIMON_SEQUENCE_NUMBER (2)
	#define SIMON_ROUNDS (42)
#elif (SIMON_KEY_WORDS==4)
	#define SIMON_SEQUENCE_NUMBER (3)
	#define SIMON_ROUNDS (44)
#endif

typedef  uint8_t u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;

#endif
