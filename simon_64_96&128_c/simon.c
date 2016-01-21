
#include "simon.h"
//#include "primitives.h"
#include "simon_macro.h"

/*
 * const z
 * can only be used in this file
 */
static u8 z[5][64] = {
	{1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0},
	{1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,0,0,1,1,0,0,0,0,1,0,1,1,0,1,0},
	{1,0,1,0,1,1,1,1,0,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,0,0,1,0,1,0,0,0,0,1,0,0,0,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,0,1,1},
	{1,1,0,1,1,0,1,1,1,0,1,0,1,1,0,0,0,1,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,1,0,0,1,1,1,0,0,1,1,0,1,0,0,0,0,1,1,1,1},
	{1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,1,1,0,1,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,1,1,1}
			};


/*
 * key schedule
 * inputKey: the original keys
 * keys: round keys
 */
void encryptKeySchedule(const u8 * inputKey, u8 * keys ) {

	const u32 *ik = (const u32*)inputKey;
	u32 *rk = (u32*)keys;

	int i;
	for ( i = 0; i < SIMON_KEY_WORDS; i++ )  {
		rk[i] = ik[i];
	}

	u32 temp;
	for ( i = SIMON_KEY_WORDS; i < SIMON_ROUNDS; i++ ) {
		temp = ror(rk[i-1], 3);
		#if defined(SIMON_KEY_WORDS) && (SIMON_KEY_WORDS==4)
			temp ^= rk[i-3];
		#endif
		temp ^= ror(temp, 1);
		rk[i] = SIMON_CONST_C ^ rk[i-SIMON_KEY_WORDS] ^ temp;
		if ( z[SIMON_SEQUENCE_NUMBER][(i-SIMON_KEY_WORDS)%62] == 1 ) {
			rk[i] ^=  0x1;
		}
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
	const u32 *rk = (const u32*)keys;

	int i;
	for ( i = 0; i < SIMON_ROUNDS; i+=2 ) {
		plain[0] = plain[0] ^ rk[i] ^ f(plain[1]);
		plain[1] = plain[1] ^ rk[i+1] ^ f(plain[0]);
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
	const u32 *rk = (const u32*)keys;    
	
	int i;
	for ( i = SIMON_ROUNDS-1; i >= 0; i-=2 ) {
		cipher[1] = cipher[1] ^ rk[i] ^ f(cipher[0]);
		cipher[0] = cipher[0] ^ rk[i-1] ^ f(cipher[1]);
	}
}
