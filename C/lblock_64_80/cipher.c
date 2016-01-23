
/*
 * LBLOCK_64_80
 */

#include <stdint.h>

#include "./../common/cipher.h"
#include "constants.h"

static u8 sbox0[16] = {14, 9, 15, 0, 13, 4, 10, 11, 1, 2, 8, 3, 7, 6, 12, 5};
static u8 sbox1[16] = {4, 11, 14, 9, 15, 13, 0, 10, 7, 12, 5, 6, 2, 8, 1, 3};
static u8 sbox2[16] = {1, 14, 7, 12, 15, 13, 0, 6, 11, 5, 9, 3, 2, 4, 8, 10};
static u8 sbox3[16] = {7, 6, 8, 11, 0, 15, 3, 14, 9, 10, 12, 13, 5, 2, 4, 1};
static u8 sbox4[16] = {14, 5, 15, 0, 7, 2, 12, 13, 1, 8, 4, 9, 11, 10, 6, 3};
static u8 sbox5[16] = {2, 13, 11, 12, 15, 14, 0, 9, 7, 10, 6, 3, 1, 8, 4, 5};
static u8 sbox6[16] = {11, 9, 4, 14, 0, 15, 10, 13, 6, 12, 5, 7, 3, 8, 1, 2};
static u8 sbox7[16] = {13, 10, 15, 0, 14, 4, 9, 11, 2, 1, 8, 3, 7, 5, 12, 6};

static u8 sbox8[16] = {8, 7, 14, 5, 15, 13, 0, 6, 11, 12, 9, 10, 2, 4, 1, 3};
static u8 sbox9[16] = {11, 5, 15, 0, 7, 2, 9, 13, 4, 8, 1, 12, 14, 10, 3, 6};
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


