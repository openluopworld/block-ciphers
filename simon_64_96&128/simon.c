
#include "simon.h"
#include "rotate.h"

/*
 * function f
 */
#define f(x) ((rol32(x, 1)&rol32(x, 8)) ^ rol32(x, 2))

/*
 * const z
 * can only be used in this file
 */
static const u8 z2[64] = 
    {1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1};
static const u8 z3[64] = 
    {1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1};

/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void simon_64_96_key_schedule(const u8 * inputKey, u8 * keys ) {

	const u32 *ik = (const u32*)inputKey;
	u32 *rk = (u32*)keys;

	int i;
	for ( i = 0; i < SIMON_KEY_WORDS_96; ++i )  {
		rk[i] = ik[i];
	}

	u32 temp;
	for ( i = SIMON_KEY_WORDS_96; i < SIMON_ROUNDS_96; ++i ) {
		temp = ror32(rk[i-1], 3);
		temp ^= ror32(temp, 1);
		rk[i] = SIMON_CONST_C ^ rk[i-SIMON_KEY_WORDS_96] ^ temp;
		if ( z2[(i-SIMON_KEY_WORDS_96)%62] == 1 ) {
			rk[i] ^=  0x1;
		}
	}
}

void simon_64_128_key_schedule(const u8 * inputKey, u8 * keys ) {

	const u32 *ik = (const u32*)inputKey;
	u32 *rk = (u32*)keys;

	int i;
	for ( i = 0; i < SIMON_KEY_WORDS_128; ++i )  {
		rk[i] = ik[i];
	}

	u32 temp;
	for ( i = SIMON_KEY_WORDS_128; i < SIMON_ROUNDS_128; ++i ) {
		temp = ror32(rk[i-1], 3);
		temp ^= rk[i-3];
		temp ^= ror32(temp, 1);
		rk[i] = SIMON_CONST_C ^ rk[i-SIMON_KEY_WORDS_128] ^ temp;
		if ( z3[(i-SIMON_KEY_WORDS_128)%62] == 1 ) {
			rk[i] ^=  0x1;
		}
	}
}

/*
 * encrypt
 * plainText: plainText has just one block.
 * keys: round keys
 */
static void encrypt(u8 * plainText, const u8 * keys, int ROUNDS) {

	u32 *plain = (u32*)plainText;
	const u32 *rk = (const u32*)keys;

	int i;
	for ( i = 0; i < ROUNDS; i+=2 ) {
		plain[0] = plain[0] ^ rk[i] ^ f(plain[1]);
		plain[1] = plain[1] ^ rk[i+1] ^ f(plain[0]);
	}
}

void simon_64_96_encrypt(u8 * plainText, const u8 * keys) {
    encrypt(plainText, keys, SIMON_ROUNDS_96);
}

void simon_64_128_encrypt(u8 * plainText, const u8 * keys) {
    encrypt(plainText, keys, SIMON_ROUNDS_128);
}

/*
 * decrypt
 * cipherText: cipherText has just one block.
 * keys: round keys
 */
static void decrypt(u8 * cipherText, const u8 * keys, int ROUNDS) {

	u32 *cipher = (u32*)cipherText;
	const u32 *rk = (const u32*)keys;    
	
	int i;
	for ( i = ROUNDS-1; i >= 0; i-=2 ) {
		cipher[1] = cipher[1] ^ rk[i] ^ f(cipher[0]);
		cipher[0] = cipher[0] ^ rk[i-1] ^ f(cipher[1]);
	}
}

void simon_64_96_decrypt(u8 * cipherText, const u8 * keys) {
    decrypt(cipherText, keys, SIMON_ROUNDS_96);
}

void simon_64_128_decrypt(u8 * cipherText, const u8 * keys) {
    decrypt(cipherText, keys, SIMON_ROUNDS_128);
}