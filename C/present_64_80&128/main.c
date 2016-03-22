
#include <stdio.h>
#include "constants.h"
#include "./../common/cipher.h"

int main () {
	#if defined(PRESENT_KEY_SIZE_80)
		/*
		 * PRESENT 64/80
		 */
		u8 inputKey[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
		u8 keys[PRESENT_BLOCK_SIZE_80/8*(PRESENT_ROUNDS+1)];
		u8 plainText[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

		encryptKeySchedule80(inputKey, keys);

		printf("PlainText: %x, %x, %x, %x, %x, %x, %x, %x\n", 
			plainText[0], plainText[1], plainText[2], plainText[3], 
			plainText[4], plainText[5], plainText[6], plainText[7]);

		encrypt(plainText, keys);
		// cipherText is: e72c46c0 f5945049
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
