
#ifndef INTERFACE
#define INTERFACE

#include<stdint.h>

void encKeySchedule(uint8_t *keys, uint8_t *roundKeys);

void encrypt(uint8_t *block, uint8_t *roundKeys);

void encrypt(uint8_t *block, uint8_t *roundKeys);

#endif
