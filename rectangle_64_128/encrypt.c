/*
 *
 * Chinese Academy of Sciences
 * State Key Laboratory of Information Security, 
 * Institute of Information Engineering
 *
 * Copyright (C) 2016 Chinese Academy of Sciences
 *
 * Written in 2016 by Luo Peng <luopeng@iie.ac.cn>
 *
 */
#include <stdint.h>

#include "cipher.h"
#include "constants.h"

#ifdef OPT_FLASH
void Encrypt(uint8_t *block, uint8_t *roundKeys)
{
	uint16_t *block16 = (uint16_t*)block;
	uint32_t *roundKeys32 = (uint32_t*)roundKeys;

	uint16_t w0 = *block16;     // first line
	uint16_t w1 = *(block16+1); // second line
	uint16_t w2 = *(block16+2); // third line
	uint16_t w3 = *(block16+3); // forth line

	uint32_t temprk;
	uint16_t sbox0, sbox1;
	uint8_t i;
	for ( i = 0; i <= NUMBER_OF_ROUNDS; i++ ) {
		/* AddRoundKey */
		temprk = READ_ROUND_KEY_DOUBLE_WORD(*roundKeys32);
		w0 ^= (uint16_t)temprk;
		w1 ^= (uint16_t)(temprk>>16);
		temprk = READ_ROUND_KEY_DOUBLE_WORD(*(roundKeys32+1));
		w2 ^= (uint16_t)temprk;
		w3 ^= (uint16_t)(temprk>>16);
		if ( NUMBER_OF_ROUNDS == i ) {break;}
		roundKeys32 += 2;
		/* SubColumn */	
		sbox0 =  w2;
		w2    ^= w1;
		w1    =  ~w1;
		sbox1 =  w0;
		w0    &= w1;
		w1    |= w3;
		w1    ^= sbox1;
		w3    ^= sbox0;
		w0    ^= w3;
		w3    &= w1;
		w3    ^= w2;
		w2    |= w0;
		w2    ^= w1;
		w1    ^= sbox0;
		/* ShiftRow */
		w1 = (w1<<1  | w1 >> 15);
		w2 = (w2<<12 | w2 >> 4);
		w3 = (w3<<13 | w3 >> 3);
	}
	/* store cipher text */
	*block16 = w0;
	*(block16+1) = w1;
	*(block16+2) = w2;
	*(block16+3) = w3;
}

#else
#ifdef OPT_RAM
void Encrypt(uint8_t *block, uint8_t *roundKeys)
{
	uint16_t *block16 = (uint16_t*)block;
	uint16_t *roundKeys16 = (uint16_t*)roundKeys;

	uint16_t w0 = *block16;     // first line
	uint16_t w1 = *(block16+1); // second line
	uint16_t w2 = *(block16+2); // third line
	uint16_t w3 = *(block16+3); // forth line
	
	uint16_t k0, k3;
	uint32_t k12;
	k0 = READ_ROUND_KEY_WORD(*(roundKeys16++));

	uint16_t sbox0, sbox1;
	uint8_t i;
	for ( i = 0; i <= NUMBER_OF_ROUNDS; i++ ) {
		/* AddRoundKey */
		k12 = READ_ROUND_KEY_DOUBLE_WORD(*(uint32_t*)roundKeys16);
		k3 = READ_ROUND_KEY_WORD(*(roundKeys16+2));
		w0 ^= ( (k3&0xff00) | (((uint16_t)k12)&0x00ff) );
		w1 ^= ( (k0<<8) | (((uint16_t)k12)>>8) );
		w2 ^= ( (k3<<8) | (((uint16_t)(k12>>16))&0x00ff) );
		w3 ^= ( (k0&0xff00) | ((uint16_t)(k12>>24)) );
		if ( NUMBER_OF_ROUNDS == i ) {break;}
		roundKeys16 += 3;
		k0 = k3;
		/* SubColumn */
		sbox0 =  w2;
		w2    ^= w1;
		w1    =  ~w1;
		sbox1 =  w0;
		w0    &= w1;
		w1    |= w3;
		w1    ^= sbox1;
		w3    ^= sbox0;
		w0    ^= w3;
		w3    &= w1;
		w3    ^= w2;
		w2    |= w0;
		w2    ^= w1;
		w1    ^= sbox0;
		/* ShiftRow */
		w1 = (w1<<1  | w1 >> 15);
		w2 = (w2<<12 | w2 >> 4);
		w3 = (w3<<13 | w3 >> 3);
	}
	/* store cipher text */
	*block16 = w0;
	*(block16+1) = w1;
	*(block16+2) = w2;
	*(block16+3) = w3;
}
#endif
#endif
