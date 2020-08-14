#ifndef __MPI_ADEC_ASSEMBLY_H__
#define __MPI_ADEC_ASSEMBLY_H__

#ifndef MAX_32
 #define MAX_32 (int)0x7fffffffL
#endif

#ifndef MIN_32
 #define MIN_32 (int)0x80000000L
#endif

#ifndef MAX_16
 #define MAX_16 (short)0x7fff
#endif

#ifndef MIN_16
 #define MIN_16 (short)0x8000
#endif

/*
 * target architecture: arm64
 */
#if defined (__GNUC__) && defined (__aarch64__)

static __inline int MULSHIFT32(int x, int y)
{
    y = (int)((((long long) x) * y) >> 32);

    return y;
}

static __inline int Integer_Mult(int x, int y)
{
    return x * y;
}

static __inline int MUL32_16T(int x, int y)
{
    return MULSHIFT32(x, y & 0xffff0000L);
}

static __inline int MUL32_16B(int x, int y)
{
    return MULSHIFT32(x, y << 16);
}

static __inline int MULF32_16T(int x, int y)
{
    return MULSHIFT32(x, y & 0xffff0000L) << 1;
}

static __inline int MULF32_16B(int x, int y)
{
    return MULSHIFT32(x, y << 16) << 1;
}

static __inline int MLA32_16T(int ret, int x, int y)
{
    return ret + MUL32_16T(x, y);
}

static __inline int MLA32_16B(int ret, int x, int y)
{
    return ret + MUL32_16B(x, y);
}

static __inline int MUL16BB(int x, int y)
{
    return MULSHIFT32(x << 16, y << 16);
}

static __inline int MUL16BT( int x, int y)
{
    return MULSHIFT32(x << 16, y & 0xffff0000L);
}

static __inline int MUL16TB( int x, int y)
{
    return MULSHIFT32(x & 0xffff0000L, y << 16);
}

static __inline int MUL16TT( int x, int y)
{
    return MULSHIFT32(x & 0xffff0000L, y & 0xffff0000L);
}

static __inline int MLA16BB(int ret, int x, int y)
{
    return ret + MUL16BB(x, y);
}

static __inline int MLA16BT(int ret, int x, int y)
{
    return ret + MUL16BT(x, y);
}

static __inline int MLA16TB(int ret, int x, int y)
{
    return ret + MUL16TB(x, y);
}

static __inline int MLA16TT(int ret, int x, int y)
{
    return ret + MUL16TT(x, y);
}

static __inline int QADD(int x, int y)
{
    int out;

    out = x + y;
    if (((x ^ y) & MIN_32) == 0)
    {
        if ((out ^ x) & MIN_32)
        {
            out = (x < 0) ? MIN_32 : MAX_32;
        }
    }

    return (out);
}

static __inline int QSUB (int x, int y)
{
    int out;

    out = x - y;
    if (((x ^ y) & MIN_32) != 0)
    {
        if ((out ^ x) & MIN_32)
        {
            out = (x < 0L) ? MIN_32 : MAX_32;
        }
    }

    return (out);
}

static __inline int QDADD(int x, int y)
{
    return QADD(x, QADD(y, y));
}

static __inline int QDSUB(int x, int y)
{
    return QSUB(x, QADD(y, y));
}

static __inline short CLIPTOSHORT(int x)
{
    int sign;

    /* clip to [-32768, 32767] */
    sign = x >> 31;
    if (sign != (x >> 15))
    {
        x = sign ^ ((1 << 15) - 1);
    }

    return (short)x;
}

static __inline int FASTABS(int x)
{
    int y;

    y  = x - (((unsigned int)x) >> 31);
    y ^= (y >> 31);
    return y;
}

#if 0
static __inline int MUL32_16T(int x, int y)
{
    __asm {
        smulwt x, x, y
    }
    return x;
}

static __inline int MUL32_16B(int x, int y)
{
    __asm {
        smulwb x, x, y
    }
    return x;
}

static __inline int MULF32_16T(int x, int y)
{
    __asm {
        smulwt x, x, y
        qadd x, x, x
    }
    return x;
}

static __inline int MULF32_16B(int x, int y)
{
    __asm {
        smulwb x, x, y
        qadd x, x, x
    }
    return x;
}

static __inline int MLA32_16T(int ret, int x, int y)
{
    __asm {
        smlawt ret, x, y, ret
    }
    return ret;
}

static __inline int MLA32_16B(int ret, int x, int y)
{
    __asm {
        smlawb ret, x, y, ret
    }
    return ret;
}

static __inline int QDADD(int x, int y)
{
    __asm {
        qdadd x, x, y
    }
    return x;
}

static __inline int MLA16BB(int ret, int x, int y)
{
    __asm {
        smlabb ret, x, y, ret
    }
    return ret;
}

static __inline int MLA16BT(int ret, int x, int y)
{
    __asm {
        smlabt ret, x, y, ret
    }
}

static __inline int MLA16TT(int ret, int x, int y)
{
    __asm {
        smlatt ret, x, y, ret
    }
}

static __inline int MLA16TB(int ret, int x, int y)
{
    __asm {
        smlatb ret, x, y, ret
    }
}

static __inline int QDSUB(int x, int y)
{
    __asm {
        qdsub x, x, y
    }
    return x;
}

static __inline int QADD(int x, int y)
{
    __asm {
        qadd x, x, y
    }
    return x;
}

static __inline int QSUB(int x, int y)
{
    __asm {
        qsub x, x, y
    }
    return x;
}

 #ifdef FASK_MULSHIFT32
static __inline int MULSHIFT32_FASK(int x, int y)
{
    __asm {
        smulwt x, x, y
    }
    return x;
}

 #else
  #define MULSHIFT32_FASK MULSHIFT32
 #endif

static __inline unsigned int SWAP_QUAD_BYTE(unsigned int v)
{
    int t;

    __asm {
        EOR t, v, v, ROR # 16
        MOV t, t, LSR # 8
        BIC t, t, # 0xFF00
        EOR v, t, v, ROR # 8
    }
    return v;
}

static __inline short CLIPTOSHORT(int x)
{
    int sign;

    /* clip to [-32768, 32767] */
    sign = x >> 31;
    if (sign != (x >> 15))
    {
        x = sign ^ ((1 << 15) - 1);
    }

    return (short)x;
}

static __inline int FASTABS(int x)
{
    int y;

    __asm {
        SUB y, x, x, LSR # 31
        EOR y, y, y, ASR # 31
    }
    return y;
}

static __inline int CLZ(int x)
{
    int numZeros;

    __asm {
        CLZ numZeros, x
    }
    return numZeros;
}
#endif

static __inline int CLZ(int x)
{
    int Num;
    short numZeros;

    Num = x;

    if (Num == 0)
    {
        return 32;
    }
    else if (Num < 0)
    {
        Num = ~Num;
    }

    /* count leading zeros with binary search */
    numZeros = 1;
    if (!((unsigned int)Num >> 16))  { numZeros += 16; Num <<= 16; }
    if (!((unsigned int)Num >> 24))  { numZeros +=  8; Num <<=  8; }
    if (!((unsigned int)Num >> 28))  { numZeros +=  4; Num <<=  4; }
    if (!((unsigned int)Num >> 30))  { numZeros +=  2; Num <<=  2; }

    numZeros -= ((unsigned int)Num >> 31);

    return (short)(numZeros);
}

#elif defined (__GNUC__) && defined (__arm__)

static __inline__ int MULSHIFT32(int x, int y)
{
    int zlow;

    __asm__ volatile ("smull %0,%1,%2,%3" : "=&r" (zlow), "=r" (y) : "r" (x), "1" (y) : "cc");
    return y;
}

static __inline__ int Integer_Mult(int x, int y)
{
    int zlow;

    __asm__ volatile ("smull %0,%1,%2,%3" : "=&r" (zlow), "=r" (y) : "r" (x), "1" (y) : "cc");
    return zlow;
}

static __inline int MUL32_16T(int x, int y)
{
    int ret;

    __asm__ volatile ("smulwt %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MUL32_16B(int x, int y)
{
    int ret;

    __asm__ volatile ("smulwb %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MULF32_16T(int x, int y)
{
    int ret;

    __asm__ volatile ("smulwt  %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x),   [y] "r" (y));
    __asm__ volatile ("qadd    %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (ret), [y] "r" (ret));
    return ret;
}

static __inline int MULF32_16B(int x, int y)
{
    int ret;

    __asm__ volatile ("smulwb  %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x),   [y] "r" (y));
    __asm__ volatile ("qadd    %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (ret), [y] "r" (ret));
    return ret;
}

static __inline int MLA32_16T(int ret, int x, int y)
{
    __asm__ volatile ("smlawt %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

static __inline int MLA32_16B(int ret, int x, int y)
{
    __asm__ volatile ("smlawb %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

static __inline int QDADD(int Lvar1, int Lvar2)
{
    __asm__ volatile ("qdadd %[Result], %[x], %[y]" : [Result] "=r" (Lvar1) : [x] "r" (Lvar1), [y] "r" (Lvar2));

    return Lvar1;
}

static __inline int QDSUB(int Lvar1, int Lvar2)
{
    __asm__ volatile ("qdsub %[Result], %[x], %[y]" : [Result] "=r" (Lvar1) : [x] "r" (Lvar1), [y] "r" (Lvar2));

    return Lvar1;
}

static __inline int QADD(int Lvar1, int Lvar2)
{
    __asm__ volatile ("qadd %[Result], %[x], %[y]" : [Result] "=r" (Lvar1) : [x] "r" (Lvar1), [y] "r" (Lvar2));

    return Lvar1;
}

static __inline int QSUB(int Lvar1, int Lvar2)
{
    __asm__ volatile ("qsub %[Result], %[x], %[y]" : [Result] "=r" (Lvar1) : [x] "r" (Lvar1), [y] "r" (Lvar2));

    return Lvar1;
}

static __inline int MUL16BB(int x, int y)
{
    int ret;

    __asm__ volatile ("smulbb %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MUL16BT( int x, int y)
{
    int ret;

    __asm__ volatile ("smulbt %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MUL16TB( int x, int y)
{
    int ret;

    __asm__ volatile ("smultb %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MUL16TT( int x, int y)
{
    int ret;

    __asm__ volatile ("smultt %[Result], %[x], %[y]" : [Result] "=r" (ret) : [x] "r" (x), [y] "r" (y));
    return ret;
}

static __inline int MLA16BB(int ret, int x, int y)
{
    __asm__ volatile ("smlabb %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

static __inline int MLA16BT(int ret, int x, int y)
{
    __asm__ volatile ("smlabt %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

static __inline int MLA16TB(int ret, int x, int y)
{
    __asm__ volatile ("smlatb %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

static __inline int MLA16TT(int ret, int x, int y)
{
    __asm__ volatile ("smlatt %[Result], %[x], %[y], %[ret]" : [Result] "=r" (ret) : [ret] "r" (ret),[x] "r" (x),
                      [y] "r" (y));
    return ret;
}

 #ifdef FASK_MULSHIFT32
static __inline int MULSHIFT32_FASK(int x, int y)
{
    __asm__ volatile ("smulwt %[Result], %[x], %[y]" : [Result] "=r" (x) : [x] "r" (x), [y] "r" (y));

    return x;
}

 #else
  #define MULSHIFT32_FASK MULSHIFT32
 #endif

static __inline short CLIPTOSHORT(int x)
{
    int sign;

    /* clip to [-32768, 32767] */
    sign = x >> 31;
    if (sign != (x >> 15))
    {
        x = sign ^ ((1 << 15) - 1);
    }

    return (short)x;
}

static __inline int FASTABS(int x)
{
    int y;

    __asm__ volatile ("SUB    %[y], %[x], %[x2], LSR#31" : [y] "=r" (y) : [x] "r" (x), [x2] "r" (x));
    __asm__ volatile ("EOR    %[y], %[x], %[x2], ASR#31" : [y] "=r" (y) : [x] "r" (y), [x2] "r" (y));

    return y;
}

static __inline int CLZ(int x)
{
    int numZeros;

    __asm__ volatile ("CLZ    %[numZeros], %[x]" : [numZeros] "=r" (numZeros) : [x] "r" (x));

    return numZeros;
}

static __inline unsigned int SWAP_QUAD_BYTE(unsigned int v)
{
    // return (((unsigned int)(v) << 24) | ((((unsigned int)(v)&0x0000ff00) << 16)>>8) | ((((unsigned int)(v)&0x00ff0000) << 8)>>16) | (((unsigned int)(v))>>24));
    int t;

    __asm__ volatile ("EOR %[Ret], %[x], %[y], ROR#16" :  [Ret] "=r" (t) : [x] "r" (v), [y] "r" (v));
    __asm__ volatile ("LSR %[Ret], %[x],       #8" :     [Ret] "=r" (t) : [x] "r" (t));
    __asm__ volatile ("BIC %[Ret], %[x],       #0xFF00" :[Ret] "=r" (t) : [x] "r" (t));
    __asm__ volatile ("EOR %[Ret], %[x], %[y], ROR#8" :  [Ret] "=r" (v) : [x] "r" (t), [y] "r" (v));
    return v;
}

typedef long long Word64;

typedef union _U64
{
    Word64 w64;
    struct
    {
        /* ARM ADS = little endian */
        unsigned int lo32;
        signed int hi32;
    } r;
} U64;

 #if 1
static __inline Word64 MADD64(Word64 sum64, int x, int y)
{
    U64 u;
    unsigned int lo32;
    signed int hi32;

    u.w64 = sum64;
    lo32 = u.r.lo32;
    hi32 = u.r.hi32;
    __asm__ ("smlal %0, %1, %2, %3"  : "+r" (lo32), "+r" (hi32)  : "%r" (x), "r" (y));
    u.r.lo32 = lo32;
    u.r.hi32 = hi32;
    return u.w64;
}

 #else
static __inline Word64 MADD64(Word64 sum64, int x, int y)
{
    U64 u;

    u.w64 = sum64;

    __asm__ volatile ("smlal %0,%1,%2,%3" : "+&r" (u.r.lo32), "+&r" (u.r.hi32) : "r" (x), "r" (y) : "cc");

    return u.w64;
}

 #endif

#else

#error "Unsupported platform in assembly.h"

#endif  /* platforms */

#ifndef MAX
 #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
 #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#define APPLY_SIGN(v, s) {(v) ^= ((signed int)(s) >> 31); (v) -= ((signed int)(s) >> 31);}

/* do y <<= n, clipping to range [-2^31, 2^31 - 1] (i.e. output has one guard bit) */
#define CLIP_MAX(y, n) {                   \
        int sign = (y) >> 31;                   \
        if (sign != (y) >> (31 - (n)))  {       \
            (y) = sign ^ (0x7fffffff);          \
        } else {                                \
            (y) = (y) << (n);                   \
        }                                       \
    }

/* do y <<= n, clipping to range [-2^30, 2^30 - 1] (i.e. output has one guard bit) */
#define CLIP_2N_SHIFT(y, n) {                   \
        int sign = (y) >> 31;                   \
        if (sign != (y) >> (30 - (n)))  {       \
            (y) = sign ^ (0x3fffffff);          \
        } else {                                \
            (y) = (y) << (n);                   \
        }                                       \
    }

static __inline short FASTABS_S(short x)
{
    return (short)(FASTABS((int)(x << 16)) >> 16);
}

static __inline unsigned int NormU32(unsigned int a)
{
    return (short) (CLZ((int)a) - 1);
}

#define MUL32 MULSHIFT32
#endif /* _ASSEMBLY_H */
