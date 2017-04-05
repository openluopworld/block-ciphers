
#include "speck.h"
#include "rotate.h"

/*
 * round function
 */
#define roundFunction(x, y, k) \
    x = ror32(x, SPECK_A);     \
    x += y;                    \
	x ^= k;                    \
	y = rol32(y, SPECK_B);     \
	y ^= x                     \

/*
 * invert round function
 */
#define invertRoundFunction(x, y, k) \
	y ^= x;                          \
	y = ror32(y, SPECK_B);           \
	x ^= k;                          \
	x -= y;                          \
	x = rol32(x, SPECK_A)            \

/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void speck_64_96_key_schedule(const u8 * inputKey, u8 * keys ) {
	
	u32 *rk = (u32*)keys;
	const u32 *ik = (const u32*)inputKey;
	
	u32 l[SPECK_ROUNDS_96 + SPECK_KEY_WORDS_96-2];
	rk[0] = ik[0];
	l[0] = ik[1];
	l[1] = ik[2];
	
	int i;
	for ( i = 0; i < SPECK_ROUNDS_96-1; i++ ) {
		l[i+SPECK_KEY_WORDS_96-1] = (rk[i] + ror32(l[i], SPECK_A)) ^ (u32)i;
		rk[i+1] = rol32(rk[i], SPECK_B) ^ l[i+SPECK_KEY_WORDS_96-1];
	}
}

void speck_64_128_key_schedule(const u8 * inputKey, u8 * keys ) {
	
	u32 *rk = (u32*)keys;
	const u32 *ik = (const u32*)inputKey;
	
	u32 l[SPECK_ROUNDS_128 + SPECK_KEY_WORDS_128-2];
	rk[0] = ik[0];
	l[0] = ik[1];
	l[1] = ik[2];
	l[2] = ik[3];
	
	int i;
	for ( i = 0; i < SPECK_ROUNDS_128-1; i++ ) {
		l[i+SPECK_KEY_WORDS_128-1] = (rk[i] + ror32(l[i], SPECK_A)) ^ (u32)i;
		rk[i+1] = rol32(rk[i], SPECK_B) ^ l[i+SPECK_KEY_WORDS_128-1];
	}
}

/*
 * encrypt
 * plainText: plainText has just one block.
 * keys: round keys
 */
static void encrypt(u8 * plainText, const u8 * keys, int ROUNDS) {
	u32 *plain = (u32*)plainText;
	const u32* rk = (const u32*)keys;
	
	int i;
	for ( i = 0; i < ROUNDS; i++ ) {
		roundFunction(plain[1], plain[0], rk[i]);
	}
}

void speck_64_96_encrypt(u8 * plainText, const u8 * keys) {
    encrypt(plainText, keys, SPECK_ROUNDS_96);
}

void speck_64_128_encrypt(u8 * plainText, const u8 * keys) {
    encrypt(plainText, keys, SPECK_ROUNDS_128);
}

/*
 * decrypt
 * cipherText: cipherText has just one block.
 * keys: round keys
 */
static void decrypt(u8 * cipherText, const u8 * keys, int ROUNDS) {
	u32 *cipher = (u32*)cipherText;
	const u32* rk = (const u32*)keys;
	
	int i;
	for ( i = ROUNDS-1; i >= 0; i-- ) {
		invertRoundFunction(cipher[1], cipher[0], rk[i]);
		//cipher[0] = ror32(cipher[1] ^ cipher[0], SPECK_B);
		//cipher[1] = rol32(((cipher[1] ^ rk[i]) - cipher[0]), SPECK_A);
	}
}

void speck_64_96_decrypt(u8 * cipherText, const u8 * keys) {
    decrypt(cipherText, keys, SPECK_ROUNDS_96);
}

void speck_64_128_decrypt(u8 * cipherText, const u8 * keys) {
    decrypt(cipherText, keys, SPECK_ROUNDS_128);
}