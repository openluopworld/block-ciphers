
/*
 *
 * The input is from right to left. 
 * The rightmost byte is the least significant byte.
 * The leftmost byte is the most significant byte.
 */

#ifndef CIPHER
#define CIPHER

#include <stdint.h>

typedef  uint8_t u8;
typedef  uint16_t u16;
typedef  uint32_t u32;
typedef  uint64_t u64;

/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void encryptKeySchedule(const u8 * inputKey, u8 * keys );

/*
 * encrypt
 * plainText: plainText has just one block.
 * keys: round keys
 */
void encrypt(u8 * plainText, const u8 * keys );

/*
 * decrypt
 * cipherText: cipherText has just one block.
 * keys: round keys
 */
void decrypt(u8 * cipherText, const u8 * keys );

#endif
