
#include "speck.h"
//#include "primitives.h"
#include "speck_macro.h"

/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void encryptKeySchedule(const u8 * inputKey, u8 * keys ) {
	
	u32 *rk = (u32*)keys;
	const u32 *ik = (const u32*)inputKey;
	
	u32 l[SPECK_ROUNDS+SPECK_KEY_WORDS-2];
	rk[0] = ik[0];
	l[0] = ik[1];
	l[1] = ik[2];
	#if defined(SPECK_KEY_WORDS) && (SPECK_KEY_WORDS==4)
		l[2] = ik[3];
	#endif
	
	int i;
	for ( i = 0; i < SPECK_ROUNDS-1; i++ ) {
		l[i+SPECK_KEY_WORDS-1] = (rk[i] + ror(l[i], SPECK_A)) ^ (u32)i;
		rk[i+1] = rol(rk[i], SPECK_B) ^ l[i+SPECK_KEY_WORDS-1];
	}
}

/*
 * encrypt
 * plainText: plainText[0] is the lower word(y), [1] is the higher(x). 
 *            plainText has just one block.
 * keys: round keys
 */
void encrypt(u8 * plainText, const u8 * keys ) {
	u32 *plain = (u32*)plainText;
	const u32* rk = (const u32*)keys;
	
	int i;
	for ( i = 0; i < SPECK_ROUNDS; i++ ) {
		roundFunction(plain[1], plain[0], rk[i]);
	}
}

/*
 * decrypt
 * cipherText: cipherText[0] is the lower word(y), [1] is the higher(x).
 *             cipherText has just one block.
 * keys: round keys
 */
void decrypt(u8 * cipherText, const u8 * keys ) {
	u32 *cipher = (u32*)cipherText;
	const u32* rk = (const u32*)keys;
	
	int i;
	for ( i = SPECK_ROUNDS-1; i >= 0; i-- ) {
		invertRoundFunction(cipher[1], cipher[0], rk[i]);
	}
}
