
#include <stdio.h>

#include "present.h"

void present_64_80_test () {
	/*
	 * PRESENT 64/80
	 */
	u8 inputKey[10] = {0x00};
	u8 keys[PRESENT_KEY_SIZE_80/8*(PRESENT_ROUNDS+1)];
	u8 plainText[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

	present_64_80_key_schedule(inputKey, keys);

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

	present_encrypt(plainText, keys);
	printf("After encryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);
	
	present_decrypt(plainText, keys);
	printf("After decryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);
}

int main () {

	present_64_80_test();

	return 0;

}
