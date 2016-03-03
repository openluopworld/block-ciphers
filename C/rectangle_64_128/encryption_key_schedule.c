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

#ifdef OPT_FLASH
void RunEncryptionKeySchedule(uint8_t *key, uint8_t *roundKeys)
{
	uint8_t key8[16];
	/* the master key can not be modified. */
	uint8_t i;
	*((uint32_t*)key8) = *((uint32_t*)key);
	*((uint32_t*)key8+1) = *((uint32_t*)key+1);
	*((uint32_t*)key8+2) = *((uint32_t*)key+2);
	*((uint32_t*)key8+3) = *((uint32_t*)key+3);

	uint16_t *key16 = (uint16_t*)key8;
	uint16_t *roundKeys16 = (uint16_t*)roundKeys;

	/* the first round keys */
	roundKeys16[0] = key16[0];
	roundKeys16[1] = key16[2];
	roundKeys16[2] = key16[4];
	roundKeys16[3] = key16[6];

	/* key schedule */
	uint8_t sbox0, sbox1;
	uint16_t halfRow2;
	uint32_t tempRow0;
	for ( i = 1; i <= NUMBER_OF_ROUNDS; i++) {
		/* S box */
		sbox0    =  key8[8];
		key8[8]  ^= key8[4];
		key8[4]  =  ~key8[4];
		sbox1    =  key8[0];
		key8[0]  &= key8[4];
		key8[4]  |= key8[12];
		key8[4]  ^= sbox1;
		key8[12] ^= sbox0;
		key8[0]  ^= key8[12];
		key8[12] &= key8[4];
		key8[12] ^= key8[8];
		key8[8]  |= key8[0];
		key8[8]  ^= key8[4];
		key8[4]  ^= sbox0;
		/* row */
		tempRow0 = *((uint32_t*)key8);
		*((uint32_t*)key8) = *((uint32_t*)key8+1);
		*((uint32_t*)key8+1) = *((uint32_t*)key8+2);
		*((uint32_t*)key8+2) = *((uint32_t*)key8+3);
		*((uint32_t*)key8+3) = tempRow0;
		*((uint32_t*)key8) ^= (tempRow0<<8 | tempRow0>>24);
		tempRow0 = *((uint32_t*)key8+1);
		*((uint32_t*)key8+2) ^= (tempRow0<<16 | tempRow0>>16);
		/* round const */
		*key8 ^= READ_RC_BYTE(RC[i-1]);
		/* store round key */
		roundKeys16[4*i] = key16[0];
		roundKeys16[4*i+1] = key16[2];
		roundKeys16[4*i+2] = key16[4];
		roundKeys16[4*i+3] = key16[6];
	}
}

#else
#ifdef OPT_RAM
void RunEncryptionKeySchedule(uint8_t *key, uint8_t *roundKeys)
{
	uint8_t key8[16];
	/* the master key can not be modified. */
	uint8_t i;
	for ( i = 0; i < KEY_SIZE; i++) {
		key8[i] = key[i];
	}

	uint16_t* key16 = (uint16_t*)key8;

	/* the first round keys */
	roundKeys[0] = key8[5];
	roundKeys[1] = key8[13];
	roundKeys[2] = key8[0];
	roundKeys[3] = key8[4];
	roundKeys[4] = key8[8];
	roundKeys[5] = key8[12];
	roundKeys[6] = key8[9];
	roundKeys[7] = key8[1];

	/* key schedule */
	uint8_t sbox0, sbox1;
	uint16_t halfRow2;
	uint32_t tempRow0;
	uint8_t index = 8;
	for ( i = 1; i <= NUMBER_OF_ROUNDS; i++) {
		/* S box */
		sbox0    =  key8[8];
		key8[8]  ^= key8[4];
		key8[4]  =  ~key8[4];
		sbox1    =  key8[0];
		key8[0]  &= key8[4];
		key8[4]  |= key8[12];
		key8[4]  ^= sbox1;
		key8[12] ^= sbox0;
		key8[0]  ^= key8[12];
		key8[12] &= key8[4];
		key8[12] ^= key8[8];
		key8[8]  |= key8[0];
		key8[8]  ^= key8[4];
		key8[4]  ^= sbox0;
		/* row */
		tempRow0 = *((uint32_t*)key8);
		*((uint32_t*)key8) = (tempRow0<<8 | tempRow0>>24) ^ *((uint32_t*)key8+1);
		*((uint32_t*)key8+1) = *((uint32_t*)key8+2);
		halfRow2 = *(key16+4);
		*(key16+4) = *(key16+5) ^ *(key16+6);
		*(key16+5) = halfRow2 ^ *(key16+7);
		*((uint32_t*)key8+3) = tempRow0;
		/* round const */
		*key8 ^= READ_RC_BYTE(RC[i-1]);
		/* store round key */
		roundKeys[index++] = key8[0];
		roundKeys[index++] = key8[4];
		roundKeys[index++] = key8[8];
		roundKeys[index++] = key8[12];
		roundKeys[index++] = key8[9];
		roundKeys[index++] = key8[1];
	}
}
#endif
#endif
