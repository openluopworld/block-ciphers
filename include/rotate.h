
/*
 * rotate shift right n-bit on x(a 64-bit block)
 */
static inline uint64_t ror64(uint64_t x, int n) { return x>>n | x<<(64-n); }

/*
 * rotate shift left x by n bits
 */
static inline uint32_t rol32(uint32_t x, int n) { return x<<n | x>>(32-n); }

/*
 * rotate shift right x by n bits
 */
static inline uint32_t ror32(uint32_t x, int n) { return x>>n | x<<(32-n); }
