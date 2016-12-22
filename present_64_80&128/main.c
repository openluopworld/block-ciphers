
#include <stdio.h>
#include "constants.h"
#include "cipher.h"


#define CASE (1)

int main () {
	#if (1==CASE)
		/*
		 * PRESENT 64/80
		 */
		u8 inputKey[10] = {0x00};
		u8 keys[PRESENT_KEY_SIZE_80/8*(PRESENT_ROUNDS+1)];
		u8 plainText[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		encryptKeySchedule80(inputKey, keys);

		int i, j;
		for ( i = 0; i < 16; i++ ) {
			for ( j = 0; j < 16; j++ ) {
				printf("%2x ", keys[i*16+j]);
			}
			printf("\n");
		}

		printf("PlainText: %x, %x, %x, %x, %x, %x, %x, %x\n", 
			plainText[0], plainText[1], plainText[2], plainText[3], 
			plainText[4], plainText[5], plainText[6], plainText[7]);

		encrypt(plainText, keys);
		printf("After encryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
			plainText[0], plainText[1], plainText[2], plainText[3], 
			plainText[4], plainText[5], plainText[6], plainText[7]);
		
		decrypt(plainText, keys);
		printf("After decryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
			plainText[0], plainText[1], plainText[2], plainText[3], 
			plainText[4], plainText[5], plainText[6], plainText[7]);
	#else
	#if defined(PRESENT_KEY_SIZE_128)
		/*
		 * PRESENT 64/128
		 */
	#endif
	#endif
	return 0;
}
