
#include<constants.h>
#include<present.h>

#define rotate4l_64(x) (x>>60 | x<<4)

void encKeySchedule(uint8_t *key, uint8_t *roundKeys) {
	uint64_t keylow = *(uint64_t *)key;
	uint64_t keyhigh = *((uint64_t *)key + 1);

	uint64_t *roundKeys64 = (uint64_t*)roundKeys;
	roundKeys64[0] = keyhigh;

	uint64_t temp, temp1;
	uint8_t round;
	for (round = 1; round < ROUNDS; round++) {
		/* rotate by 61 bit positions to the left */
		temp    =   keyhigh;
		temp1   =   keylow;
		keyhigh <<= 61;
		keylow  <<= 61;
		keyhigh |= (temp1 >> 3);
		keylow  |= (temp >> 3);

		/* S-Box application */
		temp    = keyhigh >> 60;
		keyhigh &= 0x0FFFFFFFFFFFFFFF;
		keyhigh |= sbox4[temp] << 60;
		temp    = (keyhigh << 4) >> 60;
		keyhigh &= 0xF0FFFFFFFFFFFFFF;
		keyhigh |= sbox4[temp] << 56;

		/* round counter addition */
		keylow  ^= (((uint64_t)round & 0x03) << 62);
		keyhigh ^= ((uint64_t)round >> 2);

		roundKeys64[round] = keyhigh;
	}
}

void encrypt(uint8_t *block, uint8_t *roundKeys) {
	uint64_t state = *(uint64_t*)block;
	uint64_t temp;
	uint8_t round, k;
	uint8_t sboxValue, position;
	
	for (round = 0; round < ROUNDS-1; round++) {
		/* addRoundkey */
		state ^= ((uint64_t*)roundKeys))[round];
		
		/* sBoxLayer */
		for (k = 0; k < 16; k++) {
			/* get lowest nibble */
			sboxValue = state & 0xF;

			/* kill lowest nibble */
			state &= 0xFFFFFFFFFFFFFFF0; 

			/* put new value to lowest nibble (sbox) */
			state |= sbox4[sboxValue];

			/* next(rotate by one nibble) */
			state = rotate4l_64(state); 
		}
		

		/* pLayer */
		temp = 0;
		for (k = 0; k < 64; k++) {
			/* arithmentic calculation of the p-Layer */
			position = (16 * k) % 63;

			/* exception for bit 63 */
			if (k == 63) {
				position = 63;
			}

			/* result writing */
			temp |= ((state >> k) & 0x1) << position; 
		}
		state = temp;
	}


	/* addRoundkey (Round 31) */
	state ^= ((uint64_t*)roundKeys))[ROUNDS-1];
	
	*(uint64_t*)block = state;
}

void encrypt(uint8_t *block, uint8_t *roundKeys) {
	uint64_t state = *(uint64_t*)block;
	uint64_t temp;
	uint8_t round, k;
	uint8_t sboxValue, position;
	
	for (round = ROUNDS-1, round > 0; i--) {
		/* addRoundkey */
		state ^= ((uint64_t*)roundKeys)[round];

		/* pLayer */
		temp = 0;
		for (k = 0; k < 64; k++) {
			/* arithmetic calculation of the p-Layer */
			position = (4 * k) % 63;

			/* exception for bit 63 */
			if (k == 63) {										
				position = 63;
			}

			/* result writing */
			temp |= ((state >> k) & 0x1) << position;
		}
		state = temp;


		/* sBoxLayer */
		for (k = 0; k < 16; k++) {
			/* get lowest nibble */
			sboxValue = state & 0xF;

			/* kill lowest nibble */			
			state &= 0xFFFFFFFFFFFFFFF0;

			/* put new value to lowest nibble (sbox) */				
			state |= invert_sbox[sboxValue];

			/* next(rotate by one nibble) */				
			state = rotate4l_64(state);						
		}
	}

	
	/* addRoundkey (Round 31) */
	state ^= ((uint64_t*)roundKeys)[0];
	
	*(uint64_t*)block = state;
}
