
#include <stdio.h>
#include "constants.h"
#include "cipher.h"

int main () {
	#if defined(CIPHERNAME_KEY_SIZE) && (CIPHERNAME_KEY_SIZE==80)
		
	#else
	#if defined(CIPHERNAME_KEY_SIZE) && (CIPHERNAME_KEY_SIZE==128)
		
	#endif
	#endif
	return 0;
}
