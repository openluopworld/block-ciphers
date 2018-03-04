/*
 *
 * Written in 2018
 *
 */
#include <stdio.h>
#include <stdint.h>
#include <arm_neon.h>
#include "rectangle.h"

#define ROUNDS (25)
const uint8_t rcs[25] = {
	0x01, 0x02, 0x04, 0x09, 0x12, 0x05, 0x0b, 0x16, 
	0x0c, 0x19, 0x13, 0x07, 0x0f, 0x1f, 0x1e, 0x1c, 
	0x18, 0x11, 0x03, 0x06, 0x0d, 0x1b, 0x17, 0x0e, 0x1d};


/*
 * Here, a, b, ...and p are all 16-bit values
 *
 |in disk |                4 64-bit register                      |
 |        |                                                       |
 |----------------------------------------------------------------|
 |   a    |-\
 |   b    | |
 |   c    | | first block -\
 |   d    |-/              |
 |   e    |-\              |
 |   f    | |              |      ---------------------------------
 |   g    | | second block |      |   m   |   i   |   e   |   a   |
 |   h    |-/              |--->> |   n   |   j   |   f   |   b   |
 |   i    |-\              |--->> |   o   |   k   |   g   |   c   |
 |   j    | |              |      |   p   |   l   |   h   |   d   |
 |   k    | | third block  |      ---------------------------------
 |   l    |-/              |
 |   m    |-\              |
 |   n    | |              |
 |   o    | | forth block -/
 |   p    |-/
 |----------------------------------------------------------------|
 */
void encrypt(const uint8_t *rks,
	const uint8_t *plain_text, 
	uint8_t *cipher_text)
{	
	uint16x4x4_t pt;
	uint16x4_t sbox0, sbox1;
	uint16x4_t invert = 0xffffffffffffffff;
	int i;

	vld4_lane_u16(plain_text,    pt, 0); // first line of 4 blocks
	vld4_lane_u16(plain_text+4,  pt, 1); // second ...
	vld4_lane_u16(plain_text+8,  pt, 2); // third ...
	vld4_lane_u16(plain_text+12, pt, 3); // forth line of 4 blocks

	for (i = 0; i < ROUNDS; ++i) {
		/* AddRoundKey */
		pt.val[0] = veor_u16(pt.val[0], vld1_dup_u16(rks));
		pt.val[1] = veor_u16(pt.val[1], vld1_dup_u16(rks+1);
		pt.val[2] = veor_u16(pt.val[2], vld1_dup_u16(rks+2);
		pt.val[3] = veor_u16(pt.val[3], vld1_dup_u16(rks+3);
		rks       += 4;
		/* SubColumn */	
		sbox0     =  pt.val[2];	
		pt.val[2] = veor_u16(pt.val[0], pt.val[1]);
		pt.val[1] = veor_u16(pt.val[1], invert);
		sbox1     =  pt.val[0];
		pt.val[0] = vand_u16(pt.val[0], pt.val[1]);
		pt.val[1] = vorr_u16(pt.val[1], pt.val[3]);
		pt.val[1] = veor_u16(pt.val[1], sbox1);
		pt.val[3] = veor_u16(pt.val[3], sbox0);
		pt.val[0] = veor_u16(pt.val[0], pt.val[3]);
		pt.val[3] = vand_u16(pt.val[3], pt.val[1]);
		pt.val[3] = veor_u16(pt.val[3], pt.val[2]);
		pt.val[2] = vorr_u16(pt.val[2], pt.val[0]);
		pt.val[2] = veor_u16(pt.val[2], pt.val[1]);
		pt.val[1] = veor_u16(pt.val[1], sbox0);
		/* ShiftRow */
		pt.val[1] = vorr_u16(vshl_u16(pt.val[1], 1), vshl_u16(pt.val[1], -15));
		pt.val[2] = vorr_u16(vshl_u16(pt.val[2], 12), vshl_u16(pt.val[2], -4));
		pt.val[3] = vorr_u16(vshl_u16(pt.val[3], 13), vshl_u16(pt.val[3], -3));
	}
	pt.val[0] = veor_u16(pt.val[0], vld1_dup_u16(rks));
	pt.val[1] = veor_u16(pt.val[1], vld1_dup_u16(rks+1);
	pt.val[2] = veor_u16(pt.val[2], vld1_dup_u16(rks+2);
	pt.val[3] = veor_u16(pt.val[3], vld1_dup_u16(rks+3);
	/* store cipher text */
	vst4_lane_u16(cipher_text,    pt, 0);
	vst4_lane_u16(cipher_text+4,  pt, 1);
	vst4_lane_u16(cipher_text+8,  pt, 2);
	vst4_lane_u16(cipher_text+12, pt, 3);
}
