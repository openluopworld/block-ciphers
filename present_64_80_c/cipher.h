
/*
 * PRESENT_64_80
 */

#ifndef PRESENT
#define PRESENT

#include "constants.h"

/*
 * rotate shift right n-bit on x(a 64-bit block)
 */
#define ror(x, n) ((x>>n) | (x<<(PRESENT_BLOCK_SIZE-n)))

/*
 * Key schedule
 * key: master key
 * roundKeys: round keys
 */
void encryptKeySchedule( const u8 *key, u8 *roundKeys);

/*
 * one block is encrypted
 * plainText: one block of plain text
 * roundKeys: round keys
 */
void encrypt(u8 *plainText, const u8 *roundKeys);

/*
 * one block is decrypted
 * cipherText: one block of cipher text
 * roundKeys: round keys
 */
void decrypt(u8 *cipherText, const u8 *roundKeys);

#endif
