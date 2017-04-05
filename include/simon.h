
/*
 *
 * The input is from right to left. 
 * The rightmost byte is the least significant byte.
 * The leftmost byte is the most significant byte.
 */

#ifndef SIMON_H
#define SIMON_H

#define SIMON_BLOCK_SIZE          (64)
#define SIMON_WORD_SIZE           (32)
#define SIMON_CONST_C             (0xfffffffc)
#define SIMON_KEY_WORDS_96        (3)
#define SIMON_ROUNDS_96           (42)
#define SIMON_KEY_WORDS_128       (4)
#define SIMON_ROUNDS_128          (44)
#endif

#include <stdint.h>

typedef  uint8_t  u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;


/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void simon_64_96_key_schedule(const u8 * inputKey, u8 * keys );

void simon_64_128_key_schedule(const u8 * inputKey, u8 * keys );

/*
 * encrypt
 * plainText: plainText has just one block.
 * keys: round keys
 */
void simon_64_96_encrypt(u8 * plainText, const u8 * keys );

void simon_64_128_encrypt(u8 * plainText, const u8 * keys );

/*
 * decrypt
 * cipherText: cipherText has just one block.
 * keys: round keys
 */
void simon_64_96_decrypt(u8 * cipherText, const u8 * keys );

void simon_64_128_decrypt(u8 * cipherText, const u8 * keys );
