
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

### NEON projects
+ [Ne10](https://github.com/projectNe10/Ne10)

### References
+ [neon-simple-guide](https://github.com/thenifty/neon-guide)
+ [NEON and VFP编程](http://infocenter.arm.com/help/topic/com.arm.doc.dui0204ic/DUI0204IC_rvct_assembler_guide.pdf)指令含义中文介绍, page 323
+ [Cortex-A9 NEON Media Processing Engine Revision: r4p1](http://infocenter.arm.com/help/topic/com.arm.doc.ddi0409i/DDI0409I_cortex_a9_neon_mpe_r4p1_trm.pdf)指令时间说明
+ [ARM中文社区](https://community.arm.com/cn/b/blog)
+ [ARM NEON optimization](https://community.arm.com/android-community/b/android/posts/arm-neon-optimization), ARM社区
  - 示例：Remove data dependencies
  - 示例：Reduce branched
  - **NEON assembly and intrinsic优劣势比较**
+ [ARM NEON programming quick reference](https://community.arm.com/android-community/b/android/posts/arm-neon-programming-quick-reference), ARM社区
  - AMMv7-A, ARMv8-A AArch32, ARMv8-A AArch64寄存器、指令结构
  - NEON汇编示例：**Assembly files** or **Inline assembly**
+ [Coding for NEON - Part 1: load and stores](https://community.arm.com/processors/b/blog/posts/coding-for-neon---part-1-load-and-stores)
+ [Coding for NEON - Part 2: Dealing With Leftovers](https://community.arm.com/processors/b/blog/posts/coding-for-neon---part-2-dealing-with-leftovers)
+ [Coding for NEON - Part 4: Shifting Left and Right](https://community.arm.com/processors/b/blog/posts/coding-for-neon---part-4-shifting-left-and-right)
+ [Coding for NEON - Part 5: Rearranging Vectors](https://community.arm.com/processors/b/blog/posts/coding-for-neon---part-5-rearranging-vectors)
+ [ARM NEON编程初探——一个简单的BGR888转YUV444实例详解](http://galoisplusplus.coding.me/blog/2017/06/10/use-arm-neon-to-accelerate-bgr888-to-yuv444/)
+ [ARM NEON Programmer's Reading Guide](https://github.com/yszheda/wiki/wiki/ARM-NEON-Programmer%E2%80%99s-Guide-Reading-Notes)
+ [ARM NEON tips](https://github.com/yszheda/wiki/wiki/ARM-NEON-tips)
+ [An Introduction to ARM NEON](http://peterdn.com/post/an-introduction-to-ARM-NEON.aspx)
+ [ARM NEON support in the ARM compiler](https://www.arm.com/files/pdf/NEON_Support_in_the_ARM_Compiler.pdf) White Paper
+ [ARM Compiler Version 5.06 armasm user guide](http://infocenter.arm.com/help/topic/com.arm.doc.dui0473m/DUI0473M_armasm_user_guide.pdf)指令描述更加详细，有图示
+ [objdump](https://en.wikipedia.org/wiki/Objdump) is a program for displaying various information about object files on Unix-like systems
+ [How to check the existence of NEON on arm?](https://stackoverflow.com/questions/26701262/how-to-check-the-existence-of-neon-on-arm)

### ARMv8-a ARM64 ASIMD
+ [AARCH64 ARMV8 NEON的变动](https://my.oschina.net/rinehart/blog/354523)
  - v8-v15在子函数调用时必须要保留，但是只保留低64bits
+ [Procedure Call Standard for the ARM 64-bit Architecture](http://infocenter.arm.com/help/topic/com.arm.doc.ihi0055b/IHI0055B_aapcs64.pdf)
+ [Getting to know ARM64 NEON](https://neondsp.wordpress.com/2013/12/01/getting-to-know-arm64-neon/)一个向量加法示例

### ARMv7-a
+ [从一个复数点积算法看NEON的汇编优化](https://community.arm.com/cn/b/blog/posts/neon-assemble-optimization-2013)向量乘法加法示例
