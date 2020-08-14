#ifndef __BITOPS_H
#define __BITOPS_H

/*
 * On ARMv5 and above those functions can be implemented around the
 * clz instruction for much better code efficiency.  __clz returns
 * the number of leading zeros, zero input will return 32, and
 * 0x80000000 will return 0.
 */
static inline unsigned int __clz(unsigned int x)
{
	unsigned int ret;

	asm("clz\t%0, %1" : "=r" (ret) : "r" (x));

	return ret;
}

/*
 * fls() returns zero if the input is zero, otherwise returns the bit
 * position of the last set bit, where the LSB is 1 and MSB is 32.
 */
static inline int fls(int x)
{
	return 32 - __clz(x);
}

/*
 * __fls() returns the bit position of the last bit set, where the
 * LSB is 0 and MSB is 31.  Zero input is undefined.
 */
static inline unsigned long __fls(unsigned long x)
{
	return fls(x) - 1;
}

/*
 * ffs() returns zero if the input was zero, otherwise returns the bit
 * position of the first set bit, where the LSB is 1 and MSB is 32.
 */
static inline int ffs(int x)
{
	return fls(x & -x);
}

/*
 * __ffs() returns the bit position of the first bit set, where the
 * LSB is 0 and MSB is 31.  Zero input is undefined.
 */
static inline unsigned long __ffs(unsigned long x)
{
	return ffs(x) - 1;
}

#define ffz(x) __ffs( ~(x) )

#define BITS_PER_LONG (BITS_PER_BYTE * sizeof(long))

#define BITOP_WORD(nr)          ((nr) / BITS_PER_LONG)

#define __ALIGN_MASK(x, mask)    (((x) + (mask)) & ~(mask))



#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

#define BITS_PER_BYTE           8

#define BITS_TO_LONGS(nr)       DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))

#define BIT_WORD(nr)            ((nr) / BITS_PER_LONG)
#define BITMAP_FIRST_WORD_MASK(start) (~0UL << ((start) % BITS_PER_LONG))
#define BITMAP_LAST_WORD_MASK(nbits)  \
( 								\
	((nbits) % BITS_PER_LONG) ? 				\
		(1UL<<((nbits) % BITS_PER_LONG))-1 : ~0UL 	\
)                





#endif
