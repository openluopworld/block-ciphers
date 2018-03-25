
#include <stdio.h>

#include "simon.h"

void simon_64_96_test() {
	/*
	 * Simon64/96
	 */
	u8 inputKey[] = {0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x11, 0x12, 0x13};
	u8 keys[SIMON_BLOCK_SIZE/16*SIMON_ROUNDS_96];
	u8 plainText[] = {0x63, 0x6c, 0x69, 0x6e, 0x67, 0x20, 0x72, 0x6f};

	simon_64_96_key_schedule(inputKey, keys);

	printf("PlainText: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);

	simon_64_96_encrypt(plainText, keys);
	// cipherText is: 0x5ca2e27f 0x111a8fc8
	printf("After encryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);
	
	simon_64_96_decrypt(plainText, keys);
	printf("After decryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);

}

void simon_64_128_test() {
	/*
	 * Simon64/128
	 */
	u8 inputKey[] = {0x00, 0x01, 0x02, 0x03, 0x08, 0x09, 0x0a, 0x0b, 0x10, 0x11, 0x12, 0x13, 0x18, 0x19, 0x1a, 0x1b};
	u8 keys[SIMON_BLOCK_SIZE/16*SIMON_ROUNDS_128];
	// plain text: 0x656b696c 0x20646e75
	u8 plainText[] = {0x75, 0x6e, 0x64, 0x20, 0x6c, 0x69, 0x6b, 0x65};

	simon_64_128_key_schedule(inputKey, keys);

	printf("PlainText: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);

	simon_64_128_encrypt(plainText, keys);
	// cipher text: 0x44c8fc20 0xb9dfa07a
	printf("After encryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);

	simon_64_128_decrypt(plainText, keys);
	printf("After decryption: %x, %x, %x, %x, %x, %x, %x, %x\n", 
		plainText[0], plainText[1], plainText[2], plainText[3], 
		plainText[4], plainText[5], plainText[6], plainText[7]);

}

int main () {

	simon_64_96_test();
	simon_64_128_test();

	return 0;

}
