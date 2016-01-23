
/*
 * LBLOCK_64_80
 */

#include <stdint.h>

#include "./../common/cipher.h"
#include "constants.h"

static u8 sbox[] = {0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};
static u8 invsbox[] = {0x5, 0xe, 0xf, 0x8, 0xC, 0x1, 0x2, 0xD, 0xB, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xA};

/*
 * rotate shift right n-bit on x(a 64-bit block)
 */
#define ror(x, n) ( ((x)>>(n)) | ((x)<<(PRESENT_BLOCK_SIZE-(n))) )

/*
 * Key schedule
 * key: master key
 * roundKeys: round keys
 */
void encryptKeySchedule( const u8 *key, u8 *roundKeys) {

}

/*
 * one block is encrypted
 * plainText: one block of plain text
 * roundKeys: round keys
 *
 */
void encrypt(u8 *plainText, const u8 *roundKeys) {

}

/*
 * one block is decrypted
 * cipherText: one block of cipher text
 * roundKeys: round keys
 */
void decrypt(u8 *cipherText, const u8 *roundKeys) {

}


