
/*
 *
 * The input is from right to left. 
 * The rightmost byte is the least significant byte.
 * The leftmost byte is the most significant byte.
 */

#ifndef SPECK_H
#define SPECK_H

#define SPECK_BLOCK_SIZE    (64)
#define SPECK_WORD_SIZE     (32)
#define SPECK_A             (8)
#define SPECK_B             (3)
#define SPECK_KEY_WORDS_96  (3)
#define SPECK_ROUNDS_96     (26)
#define SPECK_KEY_WORDS_128 (4)
#define SPECK_ROUNDS_128    (27)

#include "types.h"

/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void speck_64_96_key_schedule(const u8 * inputKey, u8 * keys );

void speck_64_128_key_schedule(const u8 * inputKey, u8 * keys );

/*
 * encrypt
 * plainText: plainText has just one block.
 * keys: round keys
 */
void speck_64_96_encrypt(u8 * plainText, const u8 * keys );

void speck_64_128_encrypt(u8 * plainText, const u8 * keys );

/*
 * decrypt
 * cipherText: cipherText has just one block.
 * keys: round keys
 */
void speck_64_96_decrypt(u8 * cipherText, const u8 * keys );

void speck_64_128_decrypt(u8 * cipherText, const u8 * keys );

#endif
