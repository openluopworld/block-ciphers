
/*
 * PRESENT_64_80
 */

#include <stdint.h>

#include "present.h"
#include "rotate.h"

static u8 sbox[] = {0xc, 0x5, 0x6, 0xb, 0x9, 0x0, 0xa, 0xd, 0x3, 0xe, 0xf, 0x8, 0x4, 0x7, 0x1, 0x2};
static u8 invsbox[] = {0x5, 0xe, 0xf, 0x8, 0xC, 0x1, 0x2, 0xD, 0xB, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xA};

/*
 * Key schedule for 80-bit
 * key: master key
 * roundKeys: round keys
 */
void present_64_80_key_schedule( const u8 *key, u8 *roundKeys) {
	u64 keylow = *(const u64*)key;
	u16 highBytes = *(const u16*)(key + 8);
	u64 keyhigh = ((u64)(highBytes) << 48) | (keylow >> 16);
	u64 *rk = (u64*)roundKeys;
	rk[0] = keyhigh;

	u64 temp;
	u8 i;
	

	for (i = 0; i < PRESENT_ROUNDS; i++) {
		/* 61-bit left shift */
		temp = keyhigh;
		keyhigh <<= 61;
		keyhigh |= (keylow << 45);
		keyhigh |= (temp >> 19);
		keylow = (temp >> 3) & 0xFFFF;

		/* S-Box application */
		temp = sbox[keyhigh >> 60];
		keyhigh &= 0x0FFFFFFFFFFFFFFF;
		keyhigh |= temp << 60;

		/* round counter addition */
		keylow ^= (((u64)(i + 1) & 0x01) << 15);
		keyhigh ^= ((u64)(i + 1) >> 1);

		rk[i+1] = keyhigh;
	}
}

/*
 * Key schedule for 128-bit
 * key: master key
 * roundKeys: round keys
 */
void present_64_128_key_schedule( const u8 *key, u8 *roundKeys) {
	u64 keylow = *(const u64*)key;
	u64 keyhigh = *((const u64*)key+1);
	u64 *rk = (u64*)roundKeys;
	rk[0] = keyhigh;

	u64 temp;
	u8 i;

	for (i = 0; i < PRESENT_ROUNDS; i++) {
		/* 61-bit left shift */
		temp = ( (keyhigh<<61) | (keylow>>3) );
		keylow = ( (keylow<<61) | (keyhigh>>3) );
		keyhigh = temp;

		/* S-Box application */
		temp = (sbox[keyhigh>>60]<<4) ^ (sbox[(keyhigh>>56)&0xf]);
		keyhigh &= 0x00FFFFFFFFFFFFFF;
		keyhigh |= temp<<56;

		/* round counter addition */
		temp = ((keyhigh<<2) | (keylow>>62)) ^ (u64)(i + 1);
		keyhigh = (keyhigh & 0xFFFFFFFFFFFFFFF8) ^ (temp & 0x7);
		keylow = (keylow & 0x3FFFFFFFFFFFFFFF) ^ (temp << 62);

		rk[i+1] = keyhigh;
	}
}

/*
 * one block is encrypted
 * plainText: one block of plain text
 * roundKeys: round keys
 *
 */
void present_encrypt(u8 *plainText, const u8 *roundKeys) {
	u64 state = *(u64*)plainText;
	const u64* rk = (const u64*)roundKeys;
	u64 result;
	u8 sInput; // every nibble of sbox
	u8 pLayerIndex; // the output position of every bit in pLayer
	u64 stateBit; // the input value of every bit in pLayer
	u8 i; // rounds
	u16 k;
	
	for (i = 0; i < PRESENT_ROUNDS; i++) {		
		state ^= rk[i];

		/* sbox */
		for (k = 0; k < PRESENT_BLOCK_SIZE/4; k++) {
			sInput = state & 0xF;
			state &= 0xFFFFFFFFFFFFFFF0; 
			state |= sbox[sInput];
			state = ror64(state, 4); 
		}
		
		/* pLayer */
		result = 0;
		for (k = 0; k < PRESENT_BLOCK_SIZE; k++) {
			stateBit = state & 0x1;
			state = state >> 1;
			if ( 0 != stateBit ) {
				pLayerIndex = (16 * k) % 63;
				if (63 == k) {
					pLayerIndex = 63;
				}
				/*
				 * result |= 0x1 << pLayerIndex;
				 * 0x1 is 32-bit by default
				 */
				result |= stateBit << pLayerIndex; 
			}
		}
		state = result;
	}

	state ^= rk[i];
	*(u64*)plainText = state;
}

/*
 * one block is decrypted
 * cipherText: one block of cipher text
 * roundKeys: round keys
 */
void present_decrypt(u8 *cipherText, const u8 *roundKeys) {
	u64 state = *(u64*)cipherText;
	const u64* rk = (const u64*)roundKeys;
	u64 result;
	u8 sInput; // every nibble of sbox
	u8 pLayerIndex; // the output position of every bit in pLayer
	u64 stateBit; // the input value of every bit in pLayer
	u8 i; // rounds
	u16 k;
	
	for (i = PRESENT_ROUNDS; i > 0; i--){
		state ^= rk[i];

		/* pLayer */
		result = 0;
		for (k = 0; k < PRESENT_BLOCK_SIZE; k++) {
			stateBit = state & 0x1;
			state = state >> 1;
			if ( 0 != stateBit ) {
				pLayerIndex = (4 * k) % 63;
				if (63 == k) {										
					pLayerIndex = 63;
				}
				result |= stateBit << pLayerIndex; 
			}
		}
		state = result;

		/* sbox */
		for (k = 0; k < PRESENT_BLOCK_SIZE/4; k++) {
			sInput = state & 0xF;
			state &= 0xFFFFFFFFFFFFFFF0; 
			state |= invsbox[sInput];
			state = ror64(state, 4); 
		}

	}
	
	state ^= rk[i];
	*(uint64_t*)cipherText = state;
}


