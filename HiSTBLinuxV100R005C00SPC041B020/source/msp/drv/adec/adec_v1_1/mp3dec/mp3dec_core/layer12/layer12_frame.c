/**************************************************************************************
* layer12_frame.c  decode one frame sample Layer I or II  from a bitstream
**************************************************************************************/

#if defined (MPEG_LAYER12_SUPPORT)
#include "layer12.h"

/* MPEG Header Definitions - Mode Values */
#define MPEG1 0
#define MPEG2 1
#define MPEG25 2

#define MPG_MD_STEREO 0
#define MPG_MD_JOINT_STEREO 1
#define MPG_MD_DUAL_CHANNEL 2
#define MPG_MD_MONO 3

#define NOUSE(d) d

#define MAD_F_FRACBITS 24

typedef signed int layer2_fixed_t;

#ifndef _M_IX86
typedef unsigned long long HI_U64;
typedef long long          HI_S64;
#else
typedef __int64 HI_U64;
typedef __int64 HI_S64;
#endif

typedef struct _Layer2_Quant
{
    unsigned short steps;
    unsigned char  group_bits;
    unsigned char  bits;
    layer2_fixed_t C;
    layer2_fixed_t D;
} Layer2_Quant;

#define SPEED_OPTIMIZE
#ifdef SPEED_OPTIMIZE
 #define layer2_fracmul(x, y) (((x) >> 12) * ((y) >> 12))

#else
 #define  layer2_fracmul(x, y) (layer2_fixed_t)(((HI_S64)(x) * y) >> 24)
#endif

#define frac_invert(x, frac_nb) ((x) ^ (1 << (frac_nb)))
#define frac_signextend(x, frac_nb) ((x) | (-(x & (1 << (frac_nb)))))
#define frac_norm(x, frac_nb) ((x) << (MAD_F_FRACBITS - (frac_nb)))  /* frac_nb is src fracbits, MAD_F_FRACBITS dst fracbits */

//#define CONFIG_GENERATE_TABS_FLOAT
#ifdef CONFIG_GENERATE_TABS_FLOAT
 #define M8_24 0x01000000

typedef struct _Layer2_QuantClass_F
{
    unsigned short steps;
    unsigned char  group_bits;
    unsigned char  bits;
    float          C;
    float          D;
} Layer2_QuantClass_F;

static const Layer2_QuantClass_F layer2_qc_tab_f[17] = {
    /*
     * Table B.4 of ISO/IEC 11172-3.
     */

    {     3, 2,  5,
          1.33333333333 /* 1.33333333333 => 1.33333333209, e	0.00000000124 */,
          0.50000000000, /* 0.50000000000 => 0.50000000000, e	0.00000000000 */},
    {     5, 3,  7,
          1.60000000000 /* 1.60000000000 => 1.60000000149, e -0.00000000149 */,
          0.50000000000 /* 0.50000000000 => 0.50000000000, e	0.00000000000 */},
    {     7, 0,  3,
          1.14285714286 /* 1.14285714286 => 1.14285714179, e	0.00000000107 */,
          0.25000000000 /* 0.25000000000 => 0.25000000000, e	0.00000000000 */},
    {     9, 4, 10,
          1.77777777777 /* 1.77777777777 => 1.77777777612, e	0.00000000165 */,
          0.50000000000 /* 0.50000000000 => 0.50000000000, e	0.00000000000 */},
    {    15, 0,  4,
         1.06666666666 /* 1.06666666666 => 1.06666666642, e	0.00000000024 */,
         0.12500000000 /* 0.12500000000 => 0.12500000000, e	0.00000000000 */ },
    {    31, 0,  5,
         1.03225806452 /* 1.03225806452 => 1.03225806355, e	0.00000000097 */,
         0.06250000000 /* 0.06250000000 => 0.06250000000, e	0.00000000000 */ },
    {    63, 0,  6,
         1.01587301587 /* 1.01587301587 => 1.01587301493, e	0.00000000094 */,
         0.03125000000 /* 0.03125000000 => 0.03125000000, e	0.00000000000 */ },
    {	127, 0,  7,
        1.00787401575 /* 1.00787401575 => 1.00787401572, e	0.00000000003 */,
        0.01562500000 /* 0.01562500000 => 0.01562500000, e	0.00000000000 */},
    {	255, 0,  8,
        1.00392156863 /* 1.00392156863 => 1.00392156839, e	0.00000000024 */,
        0.00781250000 /* 0.00781250000 => 0.00781250000, e	0.00000000000 */},
    {	511, 0,  9,
        1.00195694716 /* 1.00195694716 => 1.00195694715, e	0.00000000001 */,
        0.00390625000 /* 0.00390625000 => 0.00390625000, e	0.00000000000 */},
    {  1023, 0, 10,
       1.00097751711 /* 1.00097751711 => 1.00097751617, e	0.00000000094 */,
       0.00195312500 /* 0.00195312500 => 0.00195312500, e	0.00000000000 */},
    {  2047, 0, 11,
       1.00048851979 /* 1.00048851979 => 1.00048851967, e	0.00000000012 */,
       0.00097656250 /* 0.00097656250 => 0.00097656250, e	0.00000000000 */},
    {  4095, 0, 12,
       1.00024420024 /* 1.00024420024 => 1.00024420023, e	0.00000000001 */,
       0.00048828125 /* 0.00048828125 => 0.00048828125, e	0.00000000000 */},
    {  8191, 0, 13,
       1.00012208522 /* 1.00012208522 => 1.00012208521, e	0.00000000001 */,
       0.00024414063 /* 0.00024414063 => 0.00024414062, e	0.00000000000 */},
    { 16383, 0, 14,
      1.00006103888 /* 1.00006103888 => 1.00006103888, e -0.00000000000 */,
      0.00012207031 /* 0.00012207031 => 0.00012207031, e -0.00000000000 */ },
    { 32767, 0, 15,
      1.00003051851 /* 1.00003051851 => 1.00003051758, e	0.00000000093 */,
      0.00006103516 /* 0.00006103516 => 0.00006103516, e	0.00000000000 */},
    { 65535, 0, 16,
      1.00001525902 /* 1.00001525902 => 1.00001525879, e	0.00000000023 */,
      0.00003051758 /* 0.00003051758 => 0.00003051758, e	0.00000000000 */}
};

static const float layer12_sf_tab_float[64] =
{
    /*
     Table B.1 of ISO/IEC 11172-3.
     */

    2.000000000000, 1.587401051968, 1.259921049895, 1.000000000000, 0.793700525984, 0.629960524947, 0.500000000000,
    0.396850262992,

    0.314980262474, 0.250000000000, 0.198425131496, 0.157490131237, 0.125000000000, 0.099212565748, 0.078745065618,
    0.062500000000,

    0.049606282874, 0.039372532809, 0.031250000000, 0.024803141437, 0.019686266405, 0.015625000000, 0.012401570719,
    0.009843133202,

    0.007812500000, 0.006200785359, 0.004921566601, 0.003906250000, 0.003100392680, 0.002460783301, 0.001953125000,
    0.001550196340,
    0.001230391650, 0.000976562500, 0.000775098170, 0.000615195825, 0.000488281250, 0.000387549085, 0.000307597913,
    0.000244140625,

    0.000193774542, 0.000153798956, 0.000122070313, 0.000096887271, 0.000076899478, 0.000061035156, 0.000048443636,
    0.000038449739,

    0.000030517578, 0.000024221818, 0.000019224870, 0.000015258789, 0.000012110909, 0.000009612435, 0.000007629395,
    0.000006055454,

    0.000004806217, 0.000003814697, 0.000003027727, 0.000002403109, 0.000001907349, 0.000001513864, 0.000001201554,
    0,
};
static layer2_fixed_t NormAndRound(float x)
{
    float val;

    if (x >= 0.0)
    {
        val = (x * M8_24 + 0.5);
    }
    else
    {
        val = (x * M8_24 - 0.5);
    }

    return (layer2_fixed_t)(val);
}

void Layer2_generate_tab(void)
{
 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <math.h>

    static int generate_flag = 0;
    FILE  *ftab = NULL;
    int i, n;
    float f, gain;
    float f0, f1;
    if (!generate_flag)
    {
        if (!(ftab = fopen("f:\\layer2_tab.c", "w")))
        {
            exit(1);
        }

        /*
         * scalefactor table generate
         */

        fprintf(ftab, " static const layer2_fixed_t layer12_scalefactor_tab[64] = {\n");

        fprintf(ftab, "/* \n Table B.1 of ISO/IEC 11172-3. \n */ \n");

        for (i = 0; i < 64; i++)
        {
            if (i == 63)
            {
                fprintf(ftab, "NOUSE(0), ");
            }
            else
            {
                fprintf(ftab, "0x%.8x, ", NormAndRound(layer12_sf_tab_float[i]));
            }

            if (!((i + 1) & 0x7))
            {
                fprintf(ftab, "\n ");
            }
        }

        fprintf(ftab, "\n};\n");

        /*
         *   layer1_quant_tab table generate
         */
        fprintf(ftab, " static const layer2_fixed_t layer1_quant_tab[16] = {\n");
        fprintf(ftab, "/* table = (2^nb / (2^nb - 1)), nb=0~15  */ \n");
        for (i = 0; i < 16; i++)
        {
            if (i < 2)
            {
                fprintf(ftab, "NOUSE(0),\n ");
            }
            else
            {
                f0 = pow(2, i);
                f1 = (pow(2, i) - 1.0);
                f = f0 / f1;
                fprintf(ftab, "0x%.8x,\n ", NormAndRound(f));
            }
        }

        fprintf(ftab, "\n};\n");

        /*
         * layer2 qc_tab table generate
         */
        fprintf(ftab, " static const Layer2_Quant layer2_quant_tab[17] = {\n");
        fprintf(ftab, "/* Table B.4 of ISO/IEC 11172-3  */ \n");
        fprintf(ftab, "/*    steps, group, bits, C, D */ \n");

        for (i = 0; i < 17; i++)
        {
            fprintf(ftab, "{\t");
            fprintf(ftab, "%5d,", layer2_qc_tab_f[i].steps);
            fprintf(ftab, "%1d,", layer2_qc_tab_f[i].group_bits);
            fprintf(ftab, "%2d,", layer2_qc_tab_f[i].bits);
            fprintf(ftab, "0x%.8x,", NormAndRound(layer2_qc_tab_f[i].C));
            fprintf(ftab, "0x%.8x },\n ", NormAndRound(layer2_qc_tab_f[i].D));
        }

        fprintf(ftab, "\n};\n");

        fclose(ftab);
        generate_flag = 1;
    }
}

#endif

/* --- Layer 1 &  Layer 2 ------------------------------------------------------------- */

/* table = [version][samplerate index]
 * sample rate of frame (Hz)
 */
static const int samplerateTab[3][3] = {
    {44100, 48000, 32000},      /* MPEG-1 */
    {22050, 24000, 16000},      /* MPEG-2 */
    {11025, 12000,	8000},      /* MPEG-2.5 */
};

/* table = [version&layer][bitrate index]
 */
static const short bitrateTab[5][15] = {
    /* MPEG-1 */
    {  0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448},     /* Layer 1 */
    {  0, 32, 48, 56,  64,	80,  96, 112, 128, 160, 192, 224, 256, 320, 384},     /* Layer 2 */
    {  0, 32, 40, 48,  56,	64,  80,  96, 112, 128, 160, 192, 224, 256, 320},     /* Layer 3 */
    /* MPEG-2 & 2.5 */
    {  0, 32, 48, 56,  64,	80,  96, 112, 128, 144, 160, 176, 192, 224, 256},     /* Layer 1 */
    {  0,  8, 16, 24,  32,	40,  48,  56,  64,	80,  96, 112, 128, 144, 160},     /* Layer 2&3 */
};

static const layer2_fixed_t layer12_scalefactor_tab[64] = {
    /*
     Table B.1 of ISO/IEC 11172-3.
     */
    0x02000000, 0x01965fea, 0x01428a30, 0x01000000, 0x00cb2ff6, 0x00a14518, 0x00800000, 0x006597fb,
    0x0050a28c, 0x00400000, 0x0032cbfd, 0x00285146, 0x00200000, 0x001965ff, 0x001428a3, 0x00100000,
    0x000cb2ff, 0x000a1452, 0x00080000, 0x00065980, 0x00050a29, 0x00040000, 0x00032cc0, 0x00028514,
    0x00020000, 0x00019660, 0x0001428a, 0x00010000, 0x0000cb30, 0x0000a145, 0x00008000, 0x00006598,
    0x000050a3, 0x00004000, 0x000032cc, 0x00002851, 0x00002000, 0x00001966, 0x00001429, 0x00001000,
    0x00000cb3, 0x00000a14, 0x00000800, 0x00000659, 0x0000050a, 0x00000400, 0x0000032d, 0x00000285,
    0x00000200, 0x00000196, 0x00000143, 0x00000100, 0x000000cb, 0x000000a1, 0x00000080, 0x00000066,
    0x00000051, 0x00000040, 0x00000033, 0x00000028, 0x00000020, 0x00000019, 0x00000014, NOUSE(0),
};
static const layer2_fixed_t layer1_quant_tab[16] = {
    /* table = (2^nb / (2^nb - 1)), nb=0~15  */
    NOUSE(0),
    NOUSE(0),
    0x01555556,
    0x0124924a,
    0x01111112,
    0x01084210,
    0x01041042,
    0x01020408,
    0x01010102,
    0x01008040,
    0x01004010,
    0x01002004,
    0x01001002,
    0x01000800,
    0x01000400,
    0x01000200,
};
static const Layer2_Quant layer2_quant_tab[17] = {
    /* Table B.4 of ISO/IEC 11172-3  */
    /*    steps, group, bits, C, D */
    {		3, 2,  5, 0x01555556, 0x00800000 },
    {		5, 3,  7, 0x0199999a, 0x00800000 },
    {		7, 0,  3, 0x0124924a, 0x00400000 },
    {		9, 4, 10, 0x01c71c72, 0x00800000 },
    {      15, 0,  4, 0x01111112, 0x00200000 },
    {      31, 0,  5, 0x01084210, 0x00100000 },
    {      63, 0,  6, 0x01041042, 0x00080000 },
    {     127, 0,  7, 0x01020408, 0x00040000 },
    {     255, 0,  8, 0x01010102, 0x00020000 },
    {     511, 0,  9, 0x01008040, 0x00010000 },
    {    1023, 0, 10, 0x01004010, 0x00008000 },
    {    2047, 0, 11, 0x01002004, 0x00004000 },
    {    4095, 0, 12, 0x01001002, 0x00002000 },
    {    8191, 0, 13, 0x01000800, 0x00001000 },
    {	16383, 0, 14, 0x01000400, 0x00000800 },
    {	32767, 0, 15, 0x01000200, 0x00000400 },
    {	65535, 0, 16, 0x01000100, 0x00000200 },
};

/*
 layer2_sblimit_table -> layer2_sbquant_nbal_table ->   nval
 layer2_sblimit_table -> layer2_sbquant_offset_table -> layer2_offset_table(not standard) -> layer2_quant_tab  -> s'' = C * (s''' + D)
 */
static const int layer2_sblimit_table[5] = { 27, 30, 8, 12, 30 };

static const char layer2_sbquant_nbal_table[5][32] = {
    {4, 4, 4, 4, 4, 4, 4, 4,		4, 4, 4, 3,        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, NOUSE(0), 0, 0,
     0, 0},                                                                                                  /* ISO/IEC 11172-3  tab B.2a */
    {4, 4, 4, 4, 4, 4, 4, 4,		4, 4, 4, 3,        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2,        2, 2, 2,
     NOUSE(0), 0},                                                                                           /* ISO/IEC 11172-3  tab B.2b */
    {4, 4, 3, 3, 3, 3, 3, 3, NOUSE(0), 0, 0, 0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0, 0, 0,
     0, 0},                                                                                                  /* ISO/IEC 11172-3  tab B.2c */
    {4, 4, 3, 3, 3, 3, 3, 3,		3, 3, 3, 3, NOUSE(0), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0, 0, 0,
     0, 0},                                                                                                  /* ISO/IEC 11172-3  tab B.2d */
    {4, 4, 4, 4, 3, 3, 3, 3,		3, 3, 3, 2,        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,        2, 2, 2,
     NOUSE(0), 0},                                                                                           /* ISO/IEC 13818-3  tab B.1 */
};

static char const layer2_sbquant_offset_table[5][32] = {
  { 0,        0, 0, 1, 1, 1, 1, 1,        1, 1, 1, 2,        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, NOUSE(0), 0,
    0,
    0,        0, /* tab B.2a */},
  { 0,        0, 0, 1, 1, 1, 1, 1,        1, 1, 1, 2,        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3,        3, 3,
    3,
    NOUSE(0), 0, /* tab B.2b */},
  { 4,        4, 4, 4, 4, 4, 4, 4, NOUSE(0), 0, 0, 0,        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0, 0,
    0,
    0,        0, /* tab B.2c */},
  { 4,        4, 4, 4, 4, 4, 4, 4,        4, 4, 4, 4, NOUSE(0), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        0, 0,
    0,
    0,        0, /* tab B.2d */},
  { 5,        5, 5, 5, 4, 4, 4, 4,        4, 4, 4, 4,        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,        4, 4,
    4,
    NOUSE(0), 0, /* tab B.1 */},
};

/* offsets of  table layer2_quant_tab */
static const unsigned char layer2_offset_table[6][15] = {
    /* B.2a/2b sb[00-02] */
    { 0, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 },   /* 0 */
    /* B.2a/2b sb[03-10]£»13818-3 B.1 sb[04-29] */
    { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 16 },   /* 1 */
    /* B.2a/2b sb[11-22] */
    { 0, 1, 2, 3, 4, 5,16},/* 2 */
    /* B.2a sb[23-26];B.2b sb[23-29] */
    { 0, 1,16},/* 3 */
    /* B.2c sb[00-07]; B.2d sb[00-11];*/
    { 0, 1, 3, 4, 5, 6, 7, 8,  9, 10, 11, 12, 13, 14, 15 },   /* 4 */
    /* ISO/IEC 13818-3 B.1 sb[00-3] */
    { 0, 1, 2, 3, 4, 5, 6, 7,  8,  9, 10, 11, 12, 13, 14 },   /* 5 */
};

/*
 * brief:	decode one requantized Layer I sample from a bitstream
 */
layer2_fixed_t Layer1_sample(tmp3Bits *pMainData, unsigned int nb)
{
    layer2_fixed_t sample;
    layer2_fixed_t sample_integer, sample_decimal;

    sample = getNbits(pMainData, nb);

    /* invert most significant bit, extend sign, then scale to fixed format */
    /* requantize the sample */
    /* s'' = (2^nb / (2^nb - 1)) * (s''' + 2^(-nb + 1)) */

    /*
         sample_integer = s''' normalize  to  MAD_F_FRACBITS
     */
    sample_integer = sample;
    sample_integer = frac_invert(sample_integer, nb - 1);
    sample_integer = frac_signextend(sample_integer, nb - 1);
    sample_integer = frac_norm(sample_integer, nb - 1);

    /*
         sample_decimal = 2^(-nb + 1) normalize  to  MAD_F_FRACBITS
     */
    sample_decimal = frac_norm(1, nb - 1);
    sample = sample_integer + sample_decimal;

    return layer2_fracmul(sample, layer1_quant_tab[nb]);

    /* s' = factor * s'' */
    /* (to be performed by caller) */
}

/*
 * brief:	decode three requantized Layer II samples from a bitstream
 */
void Layer2_samples(tmp3Bits *          pMainData,
                    Layer2_Quant const *quantclass,
                    layer2_fixed_t      output[3])
{
    unsigned int nb, s, sample[3];

    nb = quantclass->group_bits;
    if ((quantclass->group_bits))
    {
        unsigned int codeword, steps;

        /* degrouping */
        codeword = getNbits(pMainData, quantclass->bits);
        steps = quantclass->steps;

        for (s = 0; s < 3; ++s)
        {
            sample[s] = codeword % steps;
            codeword /= steps;
        }
    }
    else
    {
        nb = quantclass->bits;

        for (s = 0; s < 3; ++s)
        {
            sample[s] = getNbits(pMainData, nb);
        }
    }

    for (s = 0; s < 3; ++s)
    {
        layer2_fixed_t requantized;

        /* invert most significant bit, extend sign, then scale to fixed format */

        /*
                requantized = s''' normalize  to  MAD_F_FRACBITS
         */
        requantized = sample[s];
        requantized = frac_invert(requantized, nb - 1);
        requantized = frac_signextend(requantized, nb - 1);
        requantized = frac_norm(requantized, nb - 1);

        /* s'' = C * (s''' + D) */
        output[s] = layer2_fracmul(requantized + quantclass->D, quantclass->C);
    }
}

static int  L2_SelectTable(int bitrate, int nch, int freq, int version_x)
{
    int table;

    if (version_x > (int)MPEG1)     /* MPEG2 or MPEG2.5 */
    {
        table = 4;
    }
    else if (0 == bitrate)       /* free format */
    {
        table = (freq == 48000) ? 0 : 1;
    }
    else
    {
        unsigned long bitrate_per_channel;

        bitrate_per_channel = bitrate;
        if (nch == 2)
        {
            bitrate_per_channel >>= 1;
#if defined (OPT_STRICT)
            /*
             * ISO/IEC 11172-3 allows only single channel mode for 32, 48, 56, and
             * 80 kbps bitrates in Layer II, but some encoders ignore this
             * restriction. We enforce it if OPT_STRICT is defined.
             */
            if ((bitrate_per_channel <= 28000) || (bitrate_per_channel == 40000))
            {
                return -1;
            }
#endif

        }
        else     /* nch == 1 */
        {
#if defined (OPT_STRICT)
            if (bitrate_per_channel > 192000)
            {
                /*
                 * ISO/IEC 11172-3 does not allow single channel mode for 224, 256,
                 * 320, or 384 kbps bitrates in Layer II.
                 */

                return -1;
            }
#endif

        }

        if (bitrate_per_channel <= 48000)
        {
            table = (freq == 32000) ? 3 : 2;
        }
        else if (bitrate_per_channel <= 80000)
        {
            table = 0;
        }
        else
        {
            //free format:
            table = (freq == 48000) ? 0 : 1;
        }
    }

    return table;
}

int LAYER12_DecodeLayer2(tmp3Bits *bsi, int mode, int version_x, int layer_description, int bitrate_index, int fs_index,
                         int mode_ext, int       *outBuf[2])
{
    int l, ch;
    int table, sblimit, nbal, nch, bound, s, sb;
    unsigned char const *val_tab, *off_tab;
    unsigned char allocation[2][32], scfsi[2][32], scalefactor[2][32][3];
    layer2_fixed_t samples[3];

    int bitrate;
    int samprate = samplerateTab[version_x][fs_index];
    int nChans = (mode == MPG_MD_MONO ? 1 : 2);

#ifdef CONFIG_GENERATE_TABS_FLOAT
    Layer2_generate_tab();
#endif
    if (version_x > (int)MPEG1)
    {
        bitrate = bitrateTab[3 + (layer_description >> 1)][bitrate_index] * 1000;
    }
    else
    {
        bitrate = bitrateTab[layer_description - 1][bitrate_index] * 1000;
    }

    nch = nChans;

    table = L2_SelectTable(bitrate, nChans, samprate, version_x);
    if ((table < 0) || (table > 4))
    {
        return -1;
    }

    sblimit = layer2_sblimit_table[table];
    val_tab = layer2_sbquant_nbal_table[table];
    off_tab = layer2_sbquant_offset_table[table];

    bound = 32;
    if (mode == (int)(MPG_MD_JOINT_STEREO))     /* intensity stereo */
    {
        bound = 4 + (mode_ext << 2);
    }

    if (bound > sblimit)
    {
        bound = sblimit;
    }

    /* parse bit allocation */
    for (sb = 0; sb < bound; ++sb)
    {
        nbal = val_tab[sb];

        for (ch = 0; ch < nch; ++ch)
        {
            allocation[ch][sb] = getNbits(bsi, nbal);
        }
    }

    for (sb = bound; sb < sblimit; ++sb)
    {
        nbal = val_tab[sb];

        allocation[0][sb] =
            allocation[1][sb] = getNbits(bsi, nbal);
    }

    /* parse scalefactor selection info */
    for (sb = 0; sb < sblimit; ++sb)
    {
        for (ch = 0; ch < nch; ++ch)
        {
            if (allocation[ch][sb])
            {
                scfsi[ch][sb] = getNbits(bsi, 2);
            }
        }
    }

    /* decode scalefactors */
    for (sb = 0; sb < sblimit; ++sb)
    {
        for (ch = 0; ch < nch; ++ch)
        {
            if (allocation[ch][sb])
            {
                switch (scfsi[ch][sb])
                {
                case 0:
                    scalefactor[ch][sb][0] = getNbits(bsi, 6);
                    scalefactor[ch][sb][1] = getNbits(bsi, 6);
                    scalefactor[ch][sb][2] = getNbits(bsi, 6);
                    break;
                case 2:
                    scalefactor[ch][sb][0] = getNbits(bsi, 6);
                    scalefactor[ch][sb][1] = scalefactor[ch][sb][0];
                    scalefactor[ch][sb][2] = scalefactor[ch][sb][0];
                    break;
                case 1:
                    scalefactor[ch][sb][0] = getNbits(bsi, 6);
                    scalefactor[ch][sb][2] = getNbits(bsi, 6);
                    scalefactor[ch][sb][1] = scalefactor[ch][sb][0];
                    break;
                case 3:
                    scalefactor[ch][sb][0] = getNbits(bsi, 6);
                    scalefactor[ch][sb][2] = getNbits(bsi, 6);
                    scalefactor[ch][sb][1] = scalefactor[ch][sb][2];
                    break;
                }

#if defined (OPT_STRICT)
                /*
                 * Scalefactor table 63 does not appear in Table B.1 of
                 * ISO/IEC 11172-3. Nonetheless, other implementations accept it,
                 * so we only reject it if OPT_STRICT is defined.
                 */
                if ((scalefactor[ch][sb][0] == 63)
                    || (scalefactor[ch][sb][1] == 63)
                    || (scalefactor[ch][sb][2] == 63))
                {
                    return -1;
                }
#endif

            }
        }
    }

    /* decode samples */
    for (l = 0; l < 12; ++l)
    {
        for (sb = 0; sb < bound; ++sb)
        {
            for (ch = 0; ch < nch; ++ch)
            {
                if ((table = allocation[ch][sb]))
                {
                    table = layer2_offset_table[off_tab[sb]][table - 1];

                    Layer2_samples(bsi, &layer2_quant_tab[table], samples);
                    for (s = 0; s < 3; ++s)
                    {
                        outBuf[ch][ 3 * l + s + 36 * sb ] =
                            layer2_fracmul(samples[s], layer12_scalefactor_tab[scalefactor[ch][sb][l / 4]]);
                    }
                }
                else
                {
                    for (s = 0; s < 3; ++s)
                    {
                        outBuf[ch][ 3 * l + s + 36 * sb ] = 0;
                    }
                }
            }
        }

        for (sb = bound; sb < sblimit; ++sb)
        {
            if ((table = allocation[0][sb]))
            {
                table = layer2_offset_table[off_tab[sb]][table - 1];

                Layer2_samples(bsi, &layer2_quant_tab[table], samples);

                for (ch = 0; ch < nch; ++ch)
                {
                    for (s = 0; s < 3; ++s)
                    {
                        outBuf[ch][ 3 * l + s + 36 * sb ] =
                            layer2_fracmul(samples[s], layer12_scalefactor_tab[scalefactor[ch][sb][l / 4]]);
                    }
                }
            }
            else
            {
                for (ch = 0; ch < nch; ++ch)
                {
                    for (s = 0; s < 3; ++s)
                    {
                        outBuf[ch][ 3 * l + s + 36 * sb ] = 0;
                    }
                }
            }
        }

        for (ch = 0; ch < nch; ++ch)
        {
            for (s = 0; s < 3; ++s)
            {
                for (sb = sblimit; sb < 32; ++sb)
                {
                    outBuf[ch][ 3 * l + s + 36 * sb ] = 0;
                }
            }
        }
    }

    return 0;
}

int LAYER12_DecodeLayer1(tmp3Bits *bsi, int mode, int version_x, int layer_description, int bitrate_index, int fs_index,
                         int mode_ext, int       *outBuf[2])
{
    unsigned int nch = (mode == MPG_MD_MONO ? 1 : 2);
    unsigned int bound, ch, l, sb, nb;
    unsigned char allocation[2][32], scalefactor[2][32];

    bound = 32;
    if (mode == (int)MPG_MD_JOINT_STEREO)
    {
        bound = 4 + (mode_ext << 2);
    }

    /* decode bit allocations */
    for (sb = 0; sb < bound; ++sb)
    {
        for (ch = 0; ch < nch; ++ch)
        {
            nb = getNbits(bsi, 4);
            if (nb == 15)
            {
                return 0;
            }

            allocation[ch][sb] = nb ? nb + 1 : 0;
        }
    }

    for (sb = bound; sb < 32; ++sb)
    {
        nb = getNbits(bsi, 4);

        if (nb == 15)
        {
            return 0;
        }

        allocation[0][sb] =
            allocation[1][sb] = nb ? nb + 1 : 0;
    }

    /* decode scalefactors */
    for (sb = 0; sb < 32; ++sb)
    {
        for (ch = 0; ch < nch; ++ch)
        {
            if (allocation[ch][sb])
            {
                scalefactor[ch][sb] = getNbits(bsi, 6);

#if defined (OPT_STRICT)
                /*
                 * Scalefactor index 63 does not appear in Table B.1 of
                 * ISO/IEC 11172-3. Nonetheless, other implementations accept it,
                 * so we only reject it if OPT_STRICT is defined.
                 */
                if (scalefactor[ch][sb] == 63)
                {
                    return -1;
                }
#endif

            }
        }
    }

    /* decode samples */
    for (l = 0; l < 12; ++l)
    {
        for (sb = 0; sb < bound; ++sb)
        {
            for (ch = 0; ch < nch; ++ch)
            {
                nb = allocation[ch][sb];

                outBuf[ch][ l + 12 * sb ] = nb ?
                                            layer2_fracmul(Layer1_sample(bsi, nb),
                                                           layer12_scalefactor_tab[scalefactor[ch][sb]]) : 0;
            }
        }

        for (sb = bound; sb < 32; ++sb)
        {
            if ((nb = allocation[0][sb]))
            {
                layer2_fixed_t sample;

                sample = Layer1_sample(bsi, nb);

                for (ch = 0; ch < nch; ++ch)
                {
                    outBuf[ch][ l + 12 * sb ] =
                        layer2_fracmul(sample, layer12_scalefactor_tab[scalefactor[ch][sb]]);
                }
            }
            else
            {
                for (ch = 0; ch < nch; ++ch)
                {
                    outBuf[ch][ l + 12 * sb ] = 0;
                }
            }
        }
    }

    return 0;
}

#endif
