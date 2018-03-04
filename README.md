
## Some Lightweight Block Ciphers

Implementations of some lightweight block ciphers in C

* HIGHT 64/128
* [PRESENT 64/80](https://www.iacr.org/archive/ches2007/47270450/47270450.pdf)
* LBlock 64/80
* LED 64/80
* Piccolo 64/80
* TWINE 64/80
* PRINCE 64/128
* LEA 128/128
* [SIMON 64/96&128](https://eprint.iacr.org/2013/404.pdf)
* [SPECK 64/96&128](https://eprint.iacr.org/2013/404.pdf)
* Chaskey 128/128
* Fantomas 128/128
* [PRIDE 64/128](https://eprint.iacr.org/2014/453.pdf)
* Robin 128/128
* [RECTANGLE 64/80&128](https://eprint.iacr.org/2014/084.pdf)
  + Reference and some assembler implementations can be found in [FELICS](https://www.cryptolux.org/index.php/FELICS)
  + May add an implementation of ARM NEON
* [SKINNY 128/128](https://eprint.iacr.org/2016/660.pdf)

## ARM NEON

### NEON intrinsics
* load
  + vld4\_16
```C
  uint16x4x4 vld4_16(const uint16_t *)
  Form of expected instructions: vld4.16 {d0, d1, d2, d3}, [r0]
```
 + vld4q\_u16
```C
  uint16x8x4_t vld4q_u16 (const uint16_t *) 
  Form of expected instruction(s): vld4.16 {d0, d1, d2, d3}, [r0]
```
 + vld4\lane\u16
```C
 uit16x4x4_t vld4_lane_u16 (const uint16_t *, uint16x4x4_t, const int) 
 Form of expected instruction(s): vld4.16 {d0[0], d1[0], d2[0], d3[0]}, [r0]
```

### NEON projects or references
* projects
 + [Ne10](https://github.com/projectNe10/Ne10)
* references
 + [neon-simple-guide](https://github.com/thenifty/neon-guide)
 + [ARM NEON Programmer's Reading Guide](https://github.com/yszheda/wiki/wiki/ARM-NEON-Programmer%E2%80%99s-Guide-Reading-Notes)
