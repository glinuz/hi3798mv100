#include <stdio.h>
#include <stdlib.h>

//#include <math.h>
#include "hi_type.h"
#include "mpi_adec_assembly.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846            /* pi */
#endif

/* Table of normalised fixed point common logarithms */

HI_U16 FXlog10_table[] =
{
    0x0000, 0x00DD, 0x01B9, 0x0293, 0x036B, 0x0442, 0x0517, 0x05EB,
    0x06BD, 0x078E, 0x085D, 0x092A, 0x09F6, 0x0AC1, 0x0B8A, 0x0C51,
    0x0D18, 0x0DDD, 0x0EA0, 0x0F63, 0x1024, 0x10E3, 0x11A2, 0x125F,
    0x131B, 0x13D5, 0x148F, 0x1547, 0x15FE, 0x16B4, 0x1769, 0x181C,
    0x18CF, 0x1980, 0x1A30, 0x1ADF, 0x1B8D, 0x1C3A, 0x1CE6, 0x1D91,
    0x1E3B, 0x1EE4, 0x1F8C, 0x2033, 0x20D9, 0x217E, 0x2222, 0x22C5,
    0x2367, 0x2409, 0x24A9, 0x2548, 0x25E7, 0x2685, 0x2721, 0x27BD,
    0x2858, 0x28F3, 0x298C, 0x2A25, 0x2ABD, 0x2B54, 0x2BEA, 0x2C7F,
    0x2D14, 0x2DA8, 0x2E3B, 0x2ECD, 0x2F5F, 0x2FF0, 0x3080, 0x310F,
    0x319E, 0x322C, 0x32B9, 0x3345, 0x33D1, 0x345C, 0x34E7, 0x3571,
    0x35FA, 0x3682, 0x370A, 0x3792, 0x3818, 0x389E, 0x3923, 0x39A8,
    0x3A2C, 0x3AB0, 0x3B32, 0x3BB5, 0x3C36, 0x3CB7, 0x3D38, 0x3DB8,
    0x3E37, 0x3EB6, 0x3F34, 0x3FB2, 0x402F, 0x40AC, 0x4128, 0x41A3,
    0x421E, 0x4298, 0x4312, 0x438C, 0x4405, 0x447D, 0x44F5, 0x456C,
    0x45E3, 0x4659, 0x46CF, 0x4744, 0x47B9, 0x482E, 0x48A2, 0x4915,
    0x4988, 0x49FB, 0x4A6D, 0x4ADE, 0x4B50, 0x4BC0, 0x4C31, 0x4CA0,
    0x4D10,
};

/* Table of fixed point common logarithms for the powers of 2 */

HI_U32 FXlog2_table[] =
{
    0x00000000L, 0x00004D10L, 0x00009A20L, 0x0000E730L,
    0x00013441L, 0x00018151L, 0x0001CE61L, 0x00021B72L,
    0x00026882L, 0x0002B592L, 0x000302A3L, 0x00034FB3L,
    0x00039CC3L, 0x0003E9D3L, 0x000436E4L, 0x000483F4L,
    0x0004d104L,
};

typedef long Frac1616type;            /* 16.16 format                 */

static Frac1616type Fmul(Frac1616type f, Frac1616type g)
{
    Frac1616type a_whole, b_whole;
    Frac1616type a_frac, b_frac;

    // Extract the whole and fractional parts of the numbers. We strip the
    // sign bit from the fractional parts but leave it intact for the
    // whole parts. This ensures that the sign of the result will be correct.

    a_frac  = f & 0x0000FFFF;
    a_whole = f >> 16;
    b_frac  = g & 0x0000FFFF;
    b_whole = g >> 16;

    // We round the result by adding 2^(-17) before we shift the
    // fractional part of the result left 16 bits.

    return ((a_whole * b_whole) << 16) +
           (a_whole * b_frac) +
           (a_frac * b_whole) +
           ((a_frac * b_frac + 0x8000) >> 16);
}


Frac1616type Flog10(Frac1616type f)
{
    HI_S16 e, index;                // Exponent and index into table
    Frac1616type r, diff, interpolant;

    if (f <= 0)
    {
        return 0;
    }

    // Search for the index of the first 1 bit in the number (start of
    // the mantissa. Note that we are only working with positive numbers
    // here, so we ignore the sign bit (bit 31).
    e = 14;                         // Exponent for number with 1 in bit
    // position 30
    while ((f & 0x40000000) == 0)
    {
        // Isolate first bit
        f <<= 1;                    // Shift all left 1
        e--;                        // Decrement exponent for number
    }

    // At this stage our number is in the following format:
    //
    //   bits 23-30        15-22       0-14
    //  +-------------+-------------+---------+
    //  |.table index.|.interpolant.|.ignored.|
    //  +-------------+-------------+---------+
    //
    // We compute the index into the table by shifting the mantissa
    // so that the first 1 bit ends up in bit position 7, and mask it
    // out. The interpolant factor that we use is the bottom 16
    // bits left in the original number after the index is extracted out,
    // and is used to linearly interpolate the results between the two
    // consecutive entries in the table.
    index = (HI_S16)(f >> 23) & 0x7F;
    interpolant = (f >> 7) & 0xFFFF;

    // Lookup the values for the 7 bits of mantissa in the table, and
    // linearly interpolate between the two entries.
    diff = FXlog10_table[index + 1] - (r = FXlog10_table[index]);
    r += Fmul(diff, interpolant);

    // Now find the appropriate power of 2 logarithm to add to the final
    // result.
    if (e < 0)
    {
        r -= FXlog2_table[-e];
    }
    else
    {
        r += FXlog2_table[e];
    }

    return r;
}

int MY_LOG10(int x)
{
    if (x <= 0)
    {
        return 0;
    }
    else
    {
        return (10 * (Flog10((int)x) + 0x4d104)) >> 16;
    }
}

typedef struct
{
    double peak;
    int    index;
} PEAK_DATA;

#define PEAK_COUNT 16

#define SQRT1_2 0x5a82799a  /* sqrt(1/2) in Q31 */

#define swap_cplx(p0, p1) \
    t = p0; t1 = *(&(p0) + 1); p0 = p1; *(&(p0) + 1) = *(&(p1) + 1); p1 = t; *(&(p1) + 1) = t1

static  unsigned char g_u8FftBitrevTab[129] =
{
    /* nfft = 512 */
    0x01, 0x40, 0x02, 0x20, 0x03, 0x60, 0x04, 0x10, 0x05, 0x50, 0x06, 0x30, 0x07, 0x70, 0x09, 0x48,
    0x0a, 0x28, 0x0b, 0x68, 0x0c, 0x18, 0x0d, 0x58, 0x0e, 0x38, 0x0f, 0x78, 0x11, 0x44, 0x12, 0x24,
    0x13, 0x64, 0x15, 0x54, 0x16, 0x34, 0x17, 0x74, 0x19, 0x4c, 0x1a, 0x2c, 0x1b, 0x6c, 0x1d, 0x5c,
    0x1e, 0x3c, 0x1f, 0x7c, 0x21, 0x42, 0x23, 0x62, 0x25, 0x52, 0x26, 0x32, 0x27, 0x72, 0x29, 0x4a,
    0x2b, 0x6a, 0x2d, 0x5a, 0x2e, 0x3a, 0x2f, 0x7a, 0x31, 0x46, 0x33, 0x66, 0x35, 0x56, 0x37, 0x76,
    0x39, 0x4e, 0x3b, 0x6e, 0x3d, 0x5e, 0x3f, 0x7e, 0x43, 0x61, 0x45, 0x51, 0x47, 0x71, 0x4b, 0x69,
    0x4d, 0x59, 0x4f, 0x79, 0x53, 0x65, 0x57, 0x75, 0x5b, 0x6d, 0x5f, 0x7d, 0x67, 0x73, 0x6f, 0x7b,
    0x00, 0x08, 0x14, 0x1c, 0x22, 0x2a, 0x36, 0x3e, 0x41, 0x49, 0x55, 0x5d, 0x63, 0x6b, 0x77, 0x7f,
    0x00,
};

/* hann window */
static int g_s32FttWindowTab[512] =
{
    0x00000000, 0x00020001, 0x00070004, 0x000f000b, 0x00180013, 0x0025001e, 0x0034002c, 0x0045003c,
    0x0059004e, 0x006f0063, 0x0087007b, 0x00a20095, 0x00c000b1, 0x00e000d0, 0x010200f1, 0x01270114,
    0x014e013a, 0x01780163, 0x01a4018e, 0x01d201bb, 0x020301ea, 0x0236021d, 0x026c0251, 0x02a40288,
    0x02de02c1, 0x031b02fd, 0x035a033a, 0x039c037b, 0x03df03bd, 0x04250402, 0x046e0449, 0x04b80493,
    0x050504df, 0x0555052d, 0x05a6057d, 0x05fa05d0, 0x06500624, 0x06a8067c, 0x070206d5, 0x075f0730,
    0x07be078e, 0x081f07ee, 0x08820850, 0x08e708b4, 0x094e091a, 0x09b80983, 0x0a2309ed, 0x0a910a5a,
    0x0b010ac9, 0x0b720b39, 0x0be60bac, 0x0c5c0c21, 0x0cd40c98, 0x0d4e0d10, 0x0dc90d8b, 0x0e470e08,
    0x0ec60e86, 0x0f480f07, 0x0fcb0f89, 0x1050100e, 0x10d71094, 0x1160111c, 0x11eb11a5, 0x12771231,
    0x130512be, 0x1395134d, 0x142713de, 0x14ba1470, 0x154f1505, 0x15e6159a, 0x167e1632, 0x171816cb,
    0x17b31765, 0x18501802, 0x18ef189f, 0x198f193f, 0x1a3019df, 0x1ad31a82, 0x1b781b25, 0x1c1d1bca,
    0x1cc51c71, 0x1d6d1d19, 0x1e171dc2, 0x1ec21e6c, 0x1f6f1f18, 0x201c1fc5, 0x20cb2074, 0x217b2123,
    0x222d21d4, 0x22df2286, 0x23932339, 0x244723ed, 0x24fd24a2, 0x25b42558, 0x266c2610, 0x272526c8,
    0x27de2781, 0x2899283c, 0x295528f7, 0x2a1129b3, 0x2ace2a70, 0x2b8c2b2d, 0x2c4b2bec, 0x2d0b2cab,
    0x2dcb2d6b, 0x2e8d2e2c, 0x2f4e2eed, 0x30112faf, 0x30d43072, 0x31973135, 0x325c31f9, 0x332032be,
    0x33e53383, 0x34ab3448, 0x3571350e, 0x363835d4, 0x36ff369b, 0x37c63762, 0x388d3829, 0x395538f1,
    0x3a1d39b9, 0x3ae63a81, 0x3bae3b4a, 0x3c773c12, 0x3d403cdb, 0x3e083da4, 0x3ed13e6d, 0x3f9a3f36,
    0x40643fff, 0x412d40c8, 0x41f64191, 0x42be425a, 0x43874323, 0x445043ec, 0x451844b4, 0x45e1457d,
    0x46a94645, 0x4771470d, 0x483847d5, 0x48ff489c, 0x49c64963, 0x4a8d4a2a, 0x4b534af0, 0x4c194bb6,
    0x4cde4c7b, 0x4da24d40, 0x4e674e05, 0x4f2a4ec9, 0x4fed4f8c, 0x50b0504f, 0x51715111, 0x523351d2,
    0x52f35293, 0x53b35353, 0x54725412, 0x553054d1, 0x55ed558e, 0x56a9564b, 0x57655707, 0x582057c2,
    0x58d9587d, 0x59925936, 0x5a4a59ee, 0x5b015aa6, 0x5bb75b5c, 0x5c6b5c11, 0x5d1f5cc5, 0x5dd15d78,
    0x5e835e2a, 0x5f335edb, 0x5fe25f8a, 0x608f6039, 0x613c60e6, 0x61e76192, 0x6291623c, 0x633962e5,
    0x63e1638d, 0x64866434, 0x652b64d9, 0x65ce657c, 0x666f661f, 0x670f66bf, 0x67ae675f, 0x684b67fc,
    0x68e66899, 0x69806933, 0x6a1869cc, 0x6aaf6a64, 0x6b446af9, 0x6bd76b8e, 0x6c696c20, 0x6cf96cb1,
    0x6d876d40, 0x6e136dcd, 0x6e9e6e59, 0x6f276ee2, 0x6fae6f6a, 0x70336ff0, 0x70b67075, 0x713870f7,
    0x71b77178, 0x723571f6, 0x72b07273, 0x732a72ee, 0x73a27366, 0x741873dd, 0x748c7452, 0x74fd74c5,
    0x756d7535, 0x75db75a4, 0x76467611, 0x76b0767b, 0x771776e4, 0x777c774a, 0x77df77ae, 0x78407810,
    0x789f7870, 0x78fc78ce, 0x79567929, 0x79ae7982, 0x7a0479da, 0x7a587a2e, 0x7aa97a81, 0x7af97ad1,
    0x7b467b1f, 0x7b907b6b, 0x7bd97bb5, 0x7c1f7bfc, 0x7c627c41, 0x7ca47c83, 0x7ce37cc4, 0x7d207d01,
    0x7d5a7d3d, 0x7d927d76, 0x7dc87dad, 0x7dfb7de1, 0x7e2c7e14, 0x7e5a7e43, 0x7e867e70, 0x7eb07e9b,
    0x7ed77ec4, 0x7efc7eea, 0x7f1e7f0d, 0x7f3e7f2e, 0x7f5c7f4d, 0x7f777f69, 0x7f8f7f83, 0x7fa57f9b,
    0x7fb97fb0, 0x7fca7fc2, 0x7fd97fd2, 0x7fe67fe0, 0x7fef7feb, 0x7ff77ff3, 0x7ffc7ffa, 0x7ffe7ffd,
    0x7ffe7fff, 0x7ffc7ffd, 0x7ff77ffa, 0x7fef7ff3, 0x7fe67feb, 0x7fd97fe0, 0x7fca7fd2, 0x7fb97fc2,
    0x7fa57fb0, 0x7f8f7f9b, 0x7f777f83, 0x7f5c7f69, 0x7f3e7f4d, 0x7f1e7f2e, 0x7efc7f0d, 0x7ed77eea,
    0x7eb07ec4, 0x7e867e9b, 0x7e5a7e70, 0x7e2c7e43, 0x7dfb7e14, 0x7dc87de1, 0x7d927dad, 0x7d5a7d76,
    0x7d207d3d, 0x7ce37d01, 0x7ca47cc4, 0x7c627c83, 0x7c1f7c41, 0x7bd97bfc, 0x7b907bb5, 0x7b467b6b,
    0x7af97b1f, 0x7aa97ad1, 0x7a587a81, 0x7a047a2e, 0x79ae79da, 0x79567982, 0x78fc7929, 0x789f78ce,
    0x78407870, 0x77df7810, 0x777c77ae, 0x7717774a, 0x76b076e4, 0x7646767b, 0x75db7611, 0x756d75a4,
    0x74fd7535, 0x748c74c5, 0x74187452, 0x73a273dd, 0x732a7366, 0x72b072ee, 0x72357273, 0x71b771f6,
    0x71387178, 0x70b670f7, 0x70337075, 0x6fae6ff0, 0x6f276f6a, 0x6e9e6ee2, 0x6e136e59, 0x6d876dcd,
    0x6cf96d40, 0x6c696cb1, 0x6bd76c20, 0x6b446b8e, 0x6aaf6af9, 0x6a186a64, 0x698069cc, 0x68e66933,
    0x684b6899, 0x67ae67fc, 0x670f675f, 0x666f66bf, 0x65ce661f, 0x652b657c, 0x648664d9, 0x63e16434,
    0x6339638d, 0x629162e5, 0x61e7623c, 0x613c6192, 0x608f60e6, 0x5fe26039, 0x5f335f8a, 0x5e835edb,
    0x5dd15e2a, 0x5d1f5d78, 0x5c6b5cc5, 0x5bb75c11, 0x5b015b5c, 0x5a4a5aa6, 0x599259ee, 0x58d95936,
    0x5820587d, 0x576557c2, 0x56a95707, 0x55ed564b, 0x5530558e, 0x547254d1, 0x53b35412, 0x52f35353,
    0x52335293, 0x517151d2, 0x50b05111, 0x4fed504f, 0x4f2a4f8c, 0x4e674ec9, 0x4da24e05, 0x4cde4d40,
    0x4c194c7b, 0x4b534bb6, 0x4a8d4af0, 0x49c64a2a, 0x48ff4963, 0x4838489c, 0x477147d5, 0x46a9470d,
    0x45e14645, 0x4518457d, 0x445044b4, 0x438743ec, 0x42be4323, 0x41f6425a, 0x412d4191, 0x406440c8,
    0x3f9a3fff, 0x3ed13f36, 0x3e083e6d, 0x3d403da4, 0x3c773cdb, 0x3bae3c12, 0x3ae63b4a, 0x3a1d3a81,
    0x395539b9, 0x388d38f1, 0x37c63829, 0x36ff3762, 0x3638369b, 0x357135d4, 0x34ab350e, 0x33e53448,
    0x33203383, 0x325c32be, 0x319731f9, 0x30d43135, 0x30113072, 0x2f4e2faf, 0x2e8d2eed, 0x2dcb2e2c,
    0x2d0b2d6b, 0x2c4b2cab, 0x2b8c2bec, 0x2ace2b2d, 0x2a112a70, 0x295529b3, 0x289928f7, 0x27de283c,
    0x27252781, 0x266c26c8, 0x25b42610, 0x24fd2558, 0x244724a2, 0x239323ed, 0x22df2339, 0x222d2286,
    0x217b21d4, 0x20cb2123, 0x201c2074, 0x1f6f1fc5, 0x1ec21f18, 0x1e171e6c, 0x1d6d1dc2, 0x1cc51d19,
    0x1c1d1c71, 0x1b781bca, 0x1ad31b25, 0x1a301a82, 0x198f19df, 0x18ef193f, 0x1850189f, 0x17b31802,
    0x17181765, 0x167e16cb, 0x15e61632, 0x154f159a, 0x14ba1505, 0x14271470, 0x139513de, 0x1305134d,
    0x127712be, 0x11eb1231, 0x116011a5, 0x10d7111c, 0x10501094, 0x0fcb100e, 0x0f480f89, 0x0ec60f07,
    0x0e470e86, 0x0dc90e08, 0x0d4e0d8b, 0x0cd40d10, 0x0c5c0c98, 0x0be60c21, 0x0b720bac, 0x0b010b39,
    0x0a910ac9, 0x0a230a5a, 0x09b809ed, 0x094e0983, 0x08e7091a, 0x088208b4, 0x081f0850, 0x07be07ee,
    0x075f078e, 0x07020730, 0x06a806d5, 0x0650067c, 0x05fa0624, 0x05a605d0, 0x0555057d, 0x0505052d,
    0x04b804df, 0x046e0493, 0x04250449, 0x03df0402, 0x039c03bd, 0x035a037b, 0x031b033a, 0x02de02fd,
    0x02a402c1, 0x026c0288, 0x02360251, 0x0203021d, 0x01d201ea, 0x01a401bb, 0x0178018e, 0x014e0163,
    0x0127013a, 0x01020114, 0x00e000f1, 0x00c000d0, 0x00a200b1, 0x00870095, 0x006f007b, 0x00590063,
    0x0045004e, 0x0034003c, 0x0025002c, 0x0018001e, 0x000f0013, 0x0007000b, 0x00020004, 0x00000001,

};

#define FFT_FIXPOIND_OPT

#ifdef FFT_FIXPOIND_OPT
#define float_t long
#define FFT_MUL30(a, b) ((int)(MULSHIFT32((int)a, (int)b) << 2))
static unsigned int sincostab[] =
{
    0x00000000, 0x00000000, 0xc0000001, 0x3fffffff,
    0xc0000001, 0x00000000, 0x00000000, 0xc0000001,
    0xd2bec334, 0xc0000001, 0x2d413ccc, 0x00000000,
    0xe7821d5a, 0xd2bec334, 0x3b20d79d, 0x2d413ccc,
    0xf383a3e2, 0xe7821d5a, 0x3ec52f9e, 0x3b20d79d,
    0xf9ba1652, 0xf383a3e2, 0x3fb11b46, 0x3ec52f9e,
    0xfcdc1342, 0xf9ba1652, 0x3fec43c5, 0x3fb11b46,
    0xfe6deaa1, 0xfcdc1342, 0x3ffb10c0, 0x3fec43c5,
    0xff36f171, 0xfe6deaa1, 0x3ffec42c, 0x3ffb10c0,
};
#endif

static void window_calc (float_t* buf, short* win_tbl, int len)
{
    int i;

    for (i = 0; i < len; i++)
    {
        buf[i] = MUL32_16B((int)buf[i], (int)(win_tbl[i]));
    }
}

static void FftBitReverse(float_t* inout)
{
    float_t* part0, *part1;
    float_t a, b, t, t1;
    unsigned char* tab = g_u8FftBitrevTab;

    part0 = inout;
    part1 = inout + (1 << 9);

    while ((a = *tab++) != 0)
    {
        b = *tab++;

        swap_cplx(part0[4 * a + 0], part0[4 * b + 0]);   /* 0xxx0 <-> 0yyy0 */
        swap_cplx(part0[4 * a + 2], part1[4 * b + 0]);   /* 0xxx1 <-> 1yyy0 */
        swap_cplx(part1[4 * a + 0], part0[4 * b + 2]);   /* 1xxx0 <-> 0yyy1 */
        swap_cplx(part1[4 * a + 2], part1[4 * b + 2]);   /* 1xxx1 <-> 1yyy1 */
    }

    do
    {
        swap_cplx(part0[4 * a + 2], part1[4 * a + 0]);   /* 0xxx1 <-> 1xxx0 */
    }
    while ((a = *tab++) != 0);
}

/*
 * Complex Fast Fourier Transform
 */
static void FFT(int NumSamples, int InverseTransform, float_t* RealIn, float_t* ImagIn, float_t* RealOut,
                float_t* ImagOut)
{
    // int NumBits;                /* Number of bits needed to store indices */
    int i, j, k, n;
    int BlockSize, BlockEnd;

#ifdef FFT_FIXPOIND_OPT
    int tr, ti;               /* temp real, temp imaginary */
#endif
    int sincostab_idx = 0;

    // NumBits = BitsNeeded(NumSamples);

    BlockEnd = 1;
    for (BlockSize = 2; BlockSize <= NumSamples; BlockSize <<= 1)
    {
        int sm1 = (int)sincostab[sincostab_idx++];
        int sm2 = (int)sincostab[sincostab_idx++];
        int cm1 = (int)sincostab[sincostab_idx++];
        int cm2 = (int)sincostab[sincostab_idx++];
        int w = 2 * cm1;
        int ar0, ar1, ar2, ai0, ai1, ai2;

        for (i = 0; i < NumSamples; i += BlockSize)
        {
            ar2 = cm2;
            ar1 = cm1;

            ai2 = sm2;
            ai1 = sm1;

            for (j = i, n = 0; n < BlockEnd; j++, n++)
            {
#ifdef FFT_FIXPOIND_OPT
                ar0 = FFT_MUL30(w, ar1) - ar2;
#endif
                ar2 = ar1;
                ar1 = ar0;

#ifdef FFT_FIXPOIND_OPT
                ai0 = FFT_MUL30(w, ai1) - ai2;
#endif
                ai2 = ai1;
                ai1 = ai0;

                k = j + BlockEnd;
#ifdef FFT_FIXPOIND_OPT
                tr = FFT_MUL30(ar0, RealOut[k]) - FFT_MUL30(ai0, ImagOut[k]);
                ti = FFT_MUL30(ar0, ImagOut[k]) + FFT_MUL30(ai0, RealOut[k]);
#endif
                RealOut[k] = RealOut[j] - tr;
                ImagOut[k] = ImagOut[j] - ti;

                RealOut[j] += tr;
                ImagOut[j] += ti;
            }
        }

        BlockEnd = BlockSize;
    }

    /*
     * Need to normalize if inverse transform...
     */
#if 0    // CodeCC says this is a dead code because InverseTransform is always 0
    if (InverseTransform)
    {
        for (i = 0; i < NumSamples; i++)
        {
            RealOut[i] /= NumSamples;
            ImagOut[i] /= NumSamples;
        }
    }
#endif
}

/*
 * Real Fast Fourier Transform
 *
 * This function was based on the code in Numerical Recipes in C.
 * In Num. Rec., the inner loop is based on a single 1-based array
 * of interleaved real and imaginary numbers.  Because we have two
 * separate zero-based arrays, our indices are quite different.
 * Here is the correspondence between Num. Rec. indices and our indices:
 *
 * i1  <->  real[i]
 * i2  <->  imag[i]
 * i3  <->  real[n/2-i]
 * i4  <->  imag[n/2-i]
 */
static void RealFFT(int NumSamples, float_t* RealIn, float_t* RealOut, float_t* ImagOut)
{
    int Half = NumSamples / 2;
    int i;

    if (NumSamples != 1024)
    {
        return;
    }

    // Fast 1024 FFT FftBitReverse
    FftBitReverse(RealIn);

    for (i = 0; i < Half; i++)
    {
        RealOut[i] = RealIn[2 * i];
        ImagOut[i] = RealIn[2 * i + 1];
    }

    FFT(Half, 0, NULL, NULL, RealOut, ImagOut);

#ifdef FFT_FIXPOIND_OPT
    int wtemp = 0x003243f1; /*((sin(0.5 * M_PI / Half)) * 0x3fffffff);*/
    int wpr = -2 * FFT_MUL30(wtemp, wtemp);
    int wpi = 0x006487c3; /*((sin(M_PI / Half)) * 0x3fffffff);*/
    int wr = 0x3fffffff + wpr;
    int wi = wpi;
    int h1r, h1i, h2r, h2i;
    int i3;
#endif

    for (i = 1; i < Half / 2; i++)
    {
        i3 = Half - i;

        h1r = (int)(0.5 * (RealOut[i] + RealOut[i3]));
        h1i = (int)(0.5 * (ImagOut[i] - ImagOut[i3]));
        h2r = (int)(0.5 * (ImagOut[i] + ImagOut[i3]));
        h2i = (int)( -0.5 * (RealOut[i] - RealOut[i3]));
#ifdef FFT_FIXPOIND_OPT
        RealOut[i]  = h1r + FFT_MUL30(wr, h2r) - FFT_MUL30(wi, h2i);
        ImagOut[i]  = h1i + FFT_MUL30(wr, h2i) + FFT_MUL30(wi, h2r);
        RealOut[i3] = h1r - FFT_MUL30(wr, h2r) + FFT_MUL30(wi, h2i);
        ImagOut[i3] = -h1i + FFT_MUL30(wr, h2i) + FFT_MUL30(wi, h2r);

        wr = FFT_MUL30((wtemp = wr), wpr) - FFT_MUL30(wi, wpi) + wr;
        wi = FFT_MUL30(wi, wpr) + FFT_MUL30(wtemp, wpi) + wi;
#endif
    }

    RealOut[0] = (h1r = RealOut[0]) + ImagOut[0];
    ImagOut[0] = h1r - ImagOut[0];
}

/* Horizontal scale for 20-band equalizer */
static int xscale1[] =
{
    0,  1,  2,  3,   4,   5,   6, 7, 8, 11, 15, 20, 27,
    36, 47, 62, 82, 107, 141, 184, 255
};

/* Horizontal scale for 80-band equalizer */
static int xscale2[] =
{
    0,     1,   2,   3,   4,   5,   6,   7,   8,   9, 10, 11, 12, 13, 14, 15, 16,  17, 18,
    19,   20,  21,  22,  23,  24,  25,  26,  27,  28, 29, 30, 31, 32, 33, 34,
    35,   36,  37,  38,  39,  40,  41,  42,  43,  44, 45, 46, 47, 48, 49, 50, 51,
    52,   53,  54,  55,  56,  57,  58,  59,  61,  63, 67, 72, 77, 82, 87, 93, 99, 105,
    110, 115, 121, 130, 141, 152, 163, 174, 185, 200, 255
};

/**************************************************************************************
* Function:    spectrum_do
*
* Description: apply fft calc spectrum power
*
* Inputs:      PcmBuf: stereo data stored in  interlace mode
*              i_band: band number £¬valid value: 20 80 512
*              channels: 1 or 2£»
* Outputs:     EnergyOut: band energy£¬(0~96),unit:dB
*
* Return:      SUCCESS 0£¬FAILURE:-1
*
* Notes:
**************************************************************************************/
int spectrum_do(short* PcmBuf, short channels, short* EnergyOut, int i_band)
{
    int dB;
    int i, b, maxval, band_stop;

    int FftOut[1024 / 2] = {0};
    float_t RealIn[1024] = {0};
    float_t RealOut[1024 / 2]  = {0};
    float_t ImagOut[1024 / 2]  = {0};
    int* xscale = 0;

    if (!EnergyOut)
    {
        return -1;
    }

    if (!((i_band == 20) || (i_band == 80) || (i_band == 512)))
    {
        return -1;
    }

    if ((channels > 2) || (channels <= 0))
    {
        return -1;
    }

    if ((PcmBuf == 0))
    {
        return -1;
    }

    if (channels == 2)
    {
        for (i = 0; i < 1024; i++)
        {
            RealIn[i] = ((int)PcmBuf[2 * i] + PcmBuf[2 * i + 1]) / 2;
        }
    }
    else
    {
        for (i = 0; i < 1024; i++)
        {
            RealIn[i] = PcmBuf[i];
        }
    }

    if (i_band == 20)
    {
        xscale = xscale1;
    }
    else if (i_band == 80)
    {
        xscale = xscale2;
    }

    //band_start = (int)(20 * (1024) / 48000.0);
    band_stop = (int)(22000 * (1024) / 48000.0);

    window_calc(RealIn, (short*)g_s32FttWindowTab, 1024);

    RealFFT(1024, RealIn, RealOut, ImagOut);

    for (i = 0; i < 512; i++)
    {
        FftOut [i] = ((long long)RealOut[i] * RealOut[i] + (long long)ImagOut[i] * ImagOut[i]) / 8192;
    }

    //stop band
    for (i = band_stop; i < 1024 / 2; i++)
    {
        FftOut [i] = 0;
    }

#define MAX_dB 96
    if (i_band == 512)
    {
        for (i = 0; i < 1024 / 2; i++)
        {
            dB = MY_LOG10(FftOut[i]);
            if (dB > MAX_dB)
            {
                dB = MAX_dB;
            }

            if (dB < 0)
            {
                dB = 0;
            }

            EnergyOut[i] = dB;
        }
    }
    else
    {
        for (i = 0; i < i_band; i++)
        {
            if (xscale)
            {
                /* We search the maximum on one scale */
                for (b = xscale[i] * 2, maxval = 0; b < xscale[ i + 1 ] * 2; b++)
                {
                    if (FftOut[b] > maxval)
                    {
                        maxval = FftOut[b];
                    }
                }

                dB = MY_LOG10(maxval);
                if (dB > MAX_dB)
                {
                    dB = MAX_dB;
                }

                if (dB < 0)
                {
                    dB = 0;
                }

                EnergyOut[i] = dB;
            }
        }

        for (; i < 1024 / 2; i++)
        {
            EnergyOut[i] = 0;
        }
    }

    return 0;
}

