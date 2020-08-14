/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name    : pq_hal_hdr.c
  Version       : Initial Draft
  Author        : w00357637
                      p00203646
  Created      : 2016/06/15
  Description  :

******************************************************************************/
#include <linux/string.h>

#include "hi_type.h"
#include "hi_math.h"

#include "pq_hal_comm.h"
#include "pq_hal_hdr.h"

static PQ_PARAM_S* sg_pstHalHdrPqParam = HI_NULL;
static PQ_HAL_HDR_MODE_E sg_enHDRMode = PQ_HAL_HDR_MODE_HDR10_TO_SDR;

/* YUV to RGB params: 10bit in, 10bit out */
static const HI_U16 u16ScaleY2R = 13;		 /*U4.0  [0,15] */
static const HI_S16 as16M33Y2R_2020[3][3] =  /* S2.13 [-2^15, 2^15-1] = [-32768, 32767]*/
{
    /*BT2020 */
    {9567 ,          0 ,      13792},
    {9567 ,      -1539 ,      -5344},
    {9567 ,      17597 ,          0}
};

static const HI_S16 as16M33Y2R_709[3][3] =
{
    /* BT709 */
    {9567,           0,       14729},
    {9567,       -1752,       -4379},
    {9567,       17356,           0}
};

static const HI_S16 as16DcInY2R[3] =       /* S10.0  [-1024, 1023] */
{
    -64,
    -512,
    -512
};

static const HI_S16 as16DcOutY2R[3] =      /*  S10.0 [-1024, 1023] */
{
    0,
    0,
    0
};

static const HI_U16 u16ClipMinY2R = 0;	   /* U10.0  [0,1023] */
static const HI_U16 u16ClipMaxY2R = 1023;  /* U10.0  [0,1023] */


/* ToneMapping Params: 20bit in, 20bit out */
static const HI_U16 u16ScaleLumaCal      = 15;                   /* U4.0  [0,15] */
static const HI_U16 au16M3LumaCal[3]     = {8608, 22217, 1943};  /* U1.15 [0, 65535]     {0.2627, 0.6780, 0.0593}<<u16ScaleLumaCal */
static const HI_U16 u16ScaleCoefTM       = 8;		             /*  U4.0  [0,15] */
static const HI_U32 u32ClipMinTM         = 0;                    /*  U20.0  [0,1048575] */
static const HI_U32 u32ClipMaxTM         = 1048575;              /*  U20.0  [0,1048575] */
static const HI_S32 as32TMOff[3]         = {0, 0, 0};            /*  S20.0  [-524288,524287] */
static const HI_S32 as32TMOff_HLG2SDR[3] = {350, 350, 350};      /*  S20.0  [-524288,524287] */


/* GamutMapping Params: 20bit in, 20bit out  */
static const HI_U16 u16ScaleGMap = 14;	                        /*  U4.0  [0,15] */
static const HI_S16 as16M33GMap_2020to709[3][3] =               /*  S1.14 [-2^15, 2^15-1] = [-32768, 32767] */
{
    /* 2020 to 709 */
    {27209 ,      -9632  ,     -1194 },
    { -2045 ,      18565  ,      -138 },
    { -297  ,     -1648   ,    18330	 }
};
static const HI_S16 as16M33GMap_709to2020[3][3] =               /*   S1.14 [-2^15, 2^15-1] = [-32768, 32767]  */
{
    /* 709 to 2020  */
    {10279 ,       5396  ,       710},
    {1134  ,     15064   ,      187	},
    {268   ,     1442    ,   14673	}
};

static const HI_S16 as16M33GMap_linear[3][3] =                  /*  S1.14 [-2^15, 2^15-1] = [-32768, 32767] */
{
    /* Linear */
    { 16384,       0  ,       0 },
    {     0,     16384,       0 },
    {     0,       0  ,   16384 }
};
static const HI_U32 u32ClipMinGMap = 0;		                    /*   U20.0 [0,1048575] */
static const HI_U32 u32ClipMaxGMap = 1048575;                   /*   U20.0 [0,1048575] */


/* RGB to YUV Params */
static const HI_U16 u16ScaleR2Y = 14;                           /* U4.0  [0,15] */
static const HI_S16 as16M33R2Y_709[3][3] =                      /* S1.14 [-32768, 32767] */
{
    /* BT709 */
    {2984  ,     10034   ,     1013  },
    { -1643 ,      -5531  ,      7175 },
    {7175  ,     -6518   ,     -659  }
};

static const HI_S16 as16M33R2Y_709_S90[3][3] =                  /* S1.14 [-32768, 32767] */
{
    /* BT709_SAT90 */
    {2984  ,     10034   ,     1013  },
    { -1479 ,      -4978  ,      6457 },
    {6457  ,     -5866   ,     -593  }
};

static const HI_S16 as16M33R2Y_709_S80[3][3] =                  /* S1.14 [-32768, 32767] */
{
    /* BT709_SAT80 */
    {2984  ,     10034   ,     1013  },
    { -1315 ,      -4425  ,      5740 },
    {5740  ,     -5214   ,     -527  }
};

static const HI_S16 as16M33R2Y_709_S78[3][3] =                  /* S1.14 [-32768, 32767] */
{
    /* BT709_SAT78 */
    {2984  ,     10034   ,     1013  },
    { -1282 ,      -4314  ,      5596 },
    {5596  ,     -5084   ,     -514  }
};


static const HI_S16 as16M33R2Y_2020[3][3] =                     /*S1.14 [-32768, 32767] */
{
    /* BT2020 */
    {3685  ,      9512  ,       832  },
    { -2004 ,      -5171 ,       7175 },
    {7175  ,     -6598  ,      -577	 }
};
static const HI_S16 as16DcInR2Y[3] =                            /* S10.0  [-1024, 1023] */
{
    0,
    0,
    0
};
static const HI_S16 as16DcOutR2Y[3] = { 64,  512,  512 };      /* s10.0  [-1024,1023] */
static const HI_U16 u16ClipMinR2Y = 0;    					   /* U10.0  [0,1023] */
static const HI_U16 u16ClipMaxR2Y = 1023; 					   /* U10.0  [0,1023] */


/************* DeGamma Params **************/
static const HI_U32 au32DeGmm_step[4] = {6, 4, 2, 0};		   /*U4.0  [0,15] */
static const HI_U32 au32DeGmm_pos[4]  = {512, 928, 1020, 1023};/*U10.0 [0,1023] */
static const HI_U32 au32DeGmm_num[4]  = {8, 26, 23, 4};		   /*U6.0  [0,63] */
/* DePQ curve: HDR10 */
static const HI_U32 au32DeGmmLut_PQ[64] =  					   /*U20.0 [0,1048575] */
{
    0	   ,
    11	   ,
    62	   ,
    208	   ,
    542	   ,
    1236   ,
    2587   ,
    5115   ,
    9720   ,
    11359  ,
    13254  ,
    15442  ,
    17968  ,
    20883  ,
    24244  ,
    28120  ,
    32586  ,
    37733  ,
    43663  ,
    50495  ,
    58366  ,
    67434  ,
    77884  ,
    89927  ,
    103810 ,
    119820 ,
    138289 ,
    159603 ,
    184212 ,
    212642 ,
    245506 ,
    283519 ,
    327520 ,
    378492 ,
    437588 ,
    453785 ,
    470597 ,
    488048 ,
    506164 ,
    524971 ,
    544496 ,
    564768 ,
    585818 ,
    607675 ,
    630374 ,
    653948 ,
    678432 ,
    703863 ,
    730279 ,
    757722 ,
    786232 ,
    815853 ,
    846632 ,
    878615 ,
    911852 ,
    946395 ,
    982299 ,
    1019619,
    1029178,
    1038829,
    1048575,
    1048575,
    1048575,
    1048575

};
/* DeHLG Curve: BBC HDR */
static const HI_U32 au32DeGmmLut_HLG[64] =  			 /* U20.0 [0,1048575] */
{
    0	   ,
    1368   ,
    5472   ,
    12312  ,
    21888  ,
    34200  ,
    49248  ,
    67032  ,
    87552  ,
    93281  ,
    99533  ,
    106356 ,
    113803 ,
    121931 ,
    130802 ,
    140483 ,
    151049 ,
    162581 ,
    175167 ,
    188903 ,
    203894 ,
    220256 ,
    238113 ,
    257602 ,
    278872 ,
    302087 ,
    327423 ,
    355074 ,
    385253 ,
    418190 ,
    454138 ,
    493371 ,
    536189 ,
    582921 ,
    633924 ,
    647388 ,
    661148 ,
    675213 ,
    689589 ,
    704283 ,
    719301 ,
    734652 ,
    750341 ,
    766378 ,
    782769 ,
    799522 ,
    816646 ,
    834148 ,
    852037 ,
    870322 ,
    889011 ,
    908113 ,
    927637 ,
    947592 ,
    967989 ,
    988837 ,
    1010145,
    1031925,
    1037445,
    1042995,
    1048575,
    1048575,
    1048575,
    1048575
};
/* DeGamma Curve: SDR */
static const HI_U32 au32DeGmmLut_Gmm[64] =  			 /* U20.0 [0,1048575] */
{
    0	   ,
    14578  ,
    30629  ,
    52932  ,
    82078  ,
    118373 ,
    162087 ,
    213455 ,
    272693 ,
    288755 ,
    305324 ,
    322403 ,
    339995 ,
    358102 ,
    376728 ,
    395873 ,
    415543 ,
    435737 ,
    456461 ,
    477715 ,
    499502 ,
    521825 ,
    544685 ,
    568086 ,
    592030 ,
    616518 ,
    641553 ,
    667138 ,
    693273 ,
    719962 ,
    747207 ,
    775009 ,
    803371 ,
    832294 ,
    861781 ,
    869242 ,
    876737 ,
    884268 ,
    891834 ,
    899436 ,
    907073 ,
    914746 ,
    922454 ,
    930198 ,
    937978 ,
    945793 ,
    953644 ,
    961531 ,
    969453 ,
    977411 ,
    985405 ,
    993435 ,
    1001500,
    1009602,
    1017739,
    1025913,
    1034122,
    1042367,
    1044434,
    1046504,
    1048575,
    1048575,
    1048575,
    1048575

};

/* DeSLF Curve: AVS HDR */
static const HI_U32 au32DeSLFLut[64] =  					 /* U20.0 [0,1048575] */
{
    0	   ,
    1	   ,
    10	   ,
    51	   ,
    185	   ,
    553	   ,
    1430   ,
    3311   ,
    7038   ,
    8404   ,
    9995   ,
    11844  ,
    13986  ,
    16461  ,
    19315  ,
    22600  ,
    26374  ,
    30702  ,
    35662  ,
    41339  ,
    47833  ,
    55258  ,
    63747  ,
    73457  ,
    84571  ,
    97305  ,
    111918 ,
    128724 ,
    148102 ,
    170524 ,
    196572 ,
    226987 ,
    262720 ,
    305010 ,
    355514 ,
    369671 ,
    384521 ,
    400110 ,
    416489 ,
    433713 ,
    451844 ,
    470948 ,
    491098 ,
    512376 ,
    534871 ,
    558683 ,
    583921 ,
    610709 ,
    639185 ,
    669504 ,
    701839 ,
    736386 ,
    773371 ,
    813045 ,
    855701 ,
    901673 ,
    951346 ,
    1005171,
    1019335,
    1033800,
    1048575,
    1048575,
    1048575,
    1048575
};

/*************** ToneMapping Lut Params:  *******************************/
static const HI_U32 au32TMapStep[8] =        /* U5.0  [0,20] */
{
    4,	6,	8,	9,	10,	12,	14,	16
};

static const HI_U32 au32TMapPos[8]  =        /* U20.0 [0,1048575] */
{
    64,	512,	1024,	3072,	8192,	65536,	327680,	1048575
};

static const HI_U32 au32TMapNum[8]  =        /* U6.0  [0,63] */
{
    4,	11,	13,	17,	22,	36,	52,	63
};

/* ToneMapping Lut: HDR10  to SDR */
static const HI_U16 u16ScaleCoefTM_HDR10toSDR = 7;		   /* U4.0  [0,15] */
static const HI_U32 au32TMLut_HDR10toSDR[64] =  		   /* U9.7 [0,65535] */
{
    23914,
    23914,
    22658,
    21587,
    20660,
    17904,
    16054,
    14705,
    13667,
    12838,
    12157,
    11585,
    9977 ,
    8965 ,
    7732 ,
    6987 ,
    6476 ,
    6099 ,
    5567 ,
    5199 ,
    4922 ,
    4701 ,
    4518 ,
    3990 ,
    3628 ,
    3345 ,
    3110 ,
    2908 ,
    2731 ,
    2573 ,
    2430 ,
    2301 ,
    2184 ,
    2076 ,
    1977 ,
    1885 ,
    1801 ,
    1520 ,
    1307 ,
    1142 ,
    1010 ,
    903	 ,
    815	 ,
    743	 ,
    681	 ,
    630	 ,
    585	 ,
    546	 ,
    512	 ,
    482	 ,
    455	 ,
    431	 ,
    410	 ,
    341	 ,
    293	 ,
    256	 ,
    228	 ,
    205	 ,
    186	 ,
    171	 ,
    158	 ,
    146	 ,
    137	 ,
    128
};


/* ToneMapping Lut: HDR10 to BBC */
static const HI_U16 u16ScaleCoefTM_HDR10toHLG = 10;		   /* U4.0  [0,15] */
static const HI_U32 u32TMLut_HDR10toHLG[64] =  			   /*U6.10 [0,65535] */
{
    46552,
    46552,
    40832,
    37818,
    35815,
    31414,
    29095,
    27554,
    26416,
    25520,
    24787,
    24169,
    22385,
    21199,
    19634,
    18594,
    17826,
    17222,
    16310,
    15636,
    15106,
    14672,
    14306,
    13250,
    12548,
    12029,
    11622,
    11288,
    11006,
    10764,
    10551,
    10363,
    10194,
    10041,
    9901 ,
    9772 ,
    9654 ,
    9255 ,
    8941 ,
    8684 ,
    8192 ,
    7282 ,
    6554 ,
    5958 ,
    5461 ,
    5041 ,
    4681 ,
    4369 ,
    4096 ,
    3855 ,
    3641 ,
    3449 ,
    3277 ,
    2731 ,
    2341 ,
    2048 ,
    1820 ,
    1638 ,
    1489 ,
    1365 ,
    1260 ,
    1170 ,
    1092 ,
    1024
};
/* ToneMapping Lut: BBC to HDR10 */
static const HI_U16 u16ScaleCoefTM_HLGtoHDR10 = 15;		      /* U4.0  [0,15] */
static const HI_U32 au32TMLut_HLGtoHDR10[64] = 				  /*U1.15 [0,65535] */
{
    /*PQ1250*/
    2853 ,
    2853 ,
    3277 ,
    3554 ,
    3764 ,
    4324 ,
    4689 ,
    4967 ,
    5193 ,
    5386 ,
    5555 ,
    5705 ,
    6187 ,
    6554 ,
    7107 ,
    7528 ,
    7872 ,
    8164 ,
    8648 ,
    9042 ,
    9378 ,
    9672 ,
    9933 ,
    10773,
    11411,
    11931,
    12374,
    12762,
    13107,
    13420,
    13705,
    13969,
    14214,
    14444,
    14659,
    14863,
    15056,
    15743,
    16328,
    16839,
    17295,
    17707,
    18084,
    18432,
    18756,
    19059,
    19343,
    19612,
    19867,
    20109,
    20340,
    20562,
    20774,
    21545,
    22220,
    22821,
    23365,
    23863,
    24322,
    24749,
    25148,
    25524,
    25878,
    26214
};
/* ToneMapping Lut: HLG to SDR */
static const HI_U16 u16ScaleCoefTM_HLGtoSDR = 12;		   /* U4.0  [0,15] */
static const HI_U32 au32TMLut_HLGtoSDR[64] =  			   /*U1.15 [0,65535] */
{
    46988,
    46988,
    40342,
    36899,
    34636,
    29737,
    27200,
    25532,
    24308,
    23353,
    22574,
    21921,
    20050,
    18820,
    17214,
    16158,
    15384,
    14779,
    13873,
    13208,
    12689,
    12266,
    11911,
    10894,
    10226,
    9736 ,
    9354 ,
    9042 ,
    8780 ,
    8555 ,
    8359 ,
    8186 ,
    8031 ,
    7890 ,
    7763 ,
    7646 ,
    7538 ,
    7177 ,
    6895 ,
    6665 ,
    6472 ,
    6306 ,
    6162 ,
    6034 ,
    5920 ,
    5816 ,
    5722 ,
    5636 ,
    5557 ,
    5483 ,
    5414 ,
    5350 ,
    5290 ,
    5082 ,
    4913 ,
    4771 ,
    4649 ,
    4542 ,
    4448 ,
    4364 ,
    4287 ,
    4218 ,
    4155 ,
    4096
};

/* ToneMapping Lut: AVS to HDR10 */
static const HI_U16 u16ScaleCoefTM_Linear = 8;		       /* U4.0  [0,15] */
static const HI_U32 au32TMLut_Linear[64] =  			   /* U8.8 [0,65535] */
{
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256,
    256
};


/******************* Gmm-8 Lut Params *************************/
static const HI_U32 au32GmmStep[8] =        /* U5.0  [0,31] */
{
    4,	6,	8,	9,	10,	12,	14,	16
};

static const HI_U32 au32GmmPos[8]  =        /* U20.0 [0,1048575] */
{
    64,	512,  1024,	3072,	8192,	65536,	327680,	1048575

};

static const HI_U32 au32GmmNum[8]  =        /* U6.0  [0,63] */
{
    4,	11,	13,	17,	22,	36,	52,	63

};
/* PQ Curve */
static const HI_U32 au32GmmLut_PQ[64] =      /* U12.0 [0,4095] */
{
    0	,
    304	,
    400	,
    466	,
    517	,
    657	,
    749	,
    820	,
    878	,
    927	,
    970	,
    1008,
    1129,
    1220,
    1354,
    1454,
    1533,
    1600,
    1708,
    1794,
    1865,
    1926,
    1980,
    2146,
    2267,
    2361,
    2440,
    2506,
    2564,
    2616,
    2662,
    2704,
    2742,
    2777,
    2810,
    2841,
    2869,
    2969,
    3050,
    3119,
    3178,
    3231,
    3278,
    3321,
    3360,
    3395,
    3428,
    3459,
    3488,
    3515,
    3540,
    3564,
    3587,
    3668,
    3736,
    3794,
    3846,
    3892,
    3933,
    3971,
    4006,
    4038,
    4067,
    4095
};

/* PQ-1250 Curve */
static const HI_U32 au32GmmLut_PQ1250[64] =              /* U12.0 [0,4095] */
{
    0	,
    121	,
    167	,
    201	,
    227	,
    304	,
    358	,
    400	,
    435	,
    466	,
    493	,
    517	,
    596	,
    657	,
    749	,
    820	,
    878	,
    927	,
    1008,
    1074,
    1129,
    1177,
    1220,
    1354,
    1454,
    1533,
    1600,
    1657,
    1708,
    1753,
    1794,
    1831,
    1865,
    1897,
    1926,
    1954,
    1980,
    2071,
    2146,
    2210,
    2267,
    2317,
    2361,
    2402,
    2440,
    2474,
    2506,
    2536,
    2564,
    2591,
    2616,
    2639,
    2662,
    2742,
    2810,
    2869,
    2922,
    2969,
    3011,
    3050,
    3086,
    3119,
    3150,
    3178
};

/* HLG Curve */
static const HI_U32 au32GmmLut_HLG[64] =              /* U12.0 [0,4095] */
{
    0	,
    28	,
    39	,
    48	,
    55	,
    78	,
    96	,
    111	,
    124	,
    136	,
    147	,
    157	,
    192	,
    222	,
    271	,
    313	,
    350	,
    384	,
    443	,
    496	,
    543	,
    586	,
    627	,
    768	,
    887	,
    991	,
    1086,
    1173,
    1254,
    1330,
    1402,
    1470,
    1536,
    1598,
    1659,
    1717,
    1773,
    1982,
    2165,
    2313,
    2436,
    2541,
    2633,
    2714,
    2788,
    2854,
    2916,
    2972,
    3024,
    3073,
    3119,
    3162,
    3203,
    3346,
    3466,
    3569,
    3660,
    3740,
    3812,
    3878,
    3939,
    3995,
    4047,
    4095
};
/* Gamma Curve */
static const HI_U32 au32GmmLut_Gmm[64] =              /* U12.0 [0,4095] */
{
    0	,
    0	,
    1	,
    1	,
    1	,
    2	,
    3	,
    4	,
    6	,
    7	,
    8	,
    9	,
    13	,
    18	,
    27	,
    36	,
    45	,
    54	,
    72	,
    90	,
    108	,
    126	,
    144	,
    216	,
    288	,
    360	,
    426	,
    486	,
    541	,
    592	,
    641	,
    686	,
    730	,
    772	,
    812	,
    850	,
    887	,
    1024,
    1146,
    1257,
    1360,
    1456,
    1547,
    1632,
    1713,
    1791,
    1866,
    1937,
    2006,
    2073,
    2138,
    2200,
    2261,
    2489,
    2697,
    2889,
    3068,
    3237,
    3397,
    3549,
    3694,
    3833,
    3966,
    4095
};


static HI_S32 pq_hal_GetHDR10toHDR10Cfg(HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    pstPqHdrCfg->bY2REn			= HI_FALSE;
    pstPqHdrCfg->bDegammaEn		= HI_FALSE;
    pstPqHdrCfg->bTMapEn		= HI_FALSE;
    pstPqHdrCfg->bGMapEn		= HI_FALSE;
    pstPqHdrCfg->bGammaEn		= HI_FALSE;
    pstPqHdrCfg->bDitherEn		= HI_FALSE;
    pstPqHdrCfg->bR2YEn			= HI_FALSE;
    pstPqHdrCfg->bChromaAdjEn	= HI_FALSE;  /* no exist in 98mv200*/
    pstPqHdrCfg->bBT2020CL	    = HI_FALSE;  /* no exist in 98mv200*/

    pstPqHdrCfg->bGMapPosSel    = HI_TRUE;
    pstPqHdrCfg->bDitherMode    = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 pq_hal_GetHDR10toSDRCfg(HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    pstPqHdrCfg->bY2REn			= HI_TRUE;
    pstPqHdrCfg->bDegammaEn		= HI_TRUE;
    pstPqHdrCfg->bTMapEn		= HI_TRUE;
    pstPqHdrCfg->bGMapEn		= HI_TRUE;
    pstPqHdrCfg->bGammaEn		= HI_TRUE;
    pstPqHdrCfg->bDitherEn		= HI_TRUE;
    pstPqHdrCfg->bR2YEn			= HI_TRUE;
    pstPqHdrCfg->bChromaAdjEn	= HI_FALSE;  /* no exist in 98mv200*/
    pstPqHdrCfg->bBT2020CL	    = HI_FALSE;  /* no exist in 98mv200*/

    pstPqHdrCfg->bGMapPosSel    = HI_TRUE;
    pstPqHdrCfg->bDitherMode    = HI_TRUE;

    /* YUV2RGB */
    pstPqHdrCfg->stY2R.u16ScaleY2R = u16ScaleY2R;
    memcpy(pstPqHdrCfg->stY2R.as16M33Y2R, as16M33Y2R_2020, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stY2R.as16DcInY2R, as16DcInY2R, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stY2R.as16DcOutY2R, as16DcOutY2R, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stY2R.u16ClipMinY2R = u16ClipMinY2R;
    pstPqHdrCfg->stY2R.u16ClipMaxY2R = u16ClipMaxY2R;

    /* DeGamma */
    memcpy(pstPqHdrCfg->stDeGmm.au32Step, au32DeGmm_step, sizeof(HI_U32) * PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Pos, au32DeGmm_pos, sizeof(HI_U32) * PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Num, au32DeGmm_num, sizeof(HI_U32) * PQ_DEGMM_SEG_SIZE);
    pstPqHdrCfg->stDeGmm.pu32LUT = (HI_U32*)au32DeGmmLut_PQ;

    /* ToneMapping */
    pstPqHdrCfg->stTMAP.u16ScaleLumaCal = u16ScaleLumaCal;
    memcpy(pstPqHdrCfg->stTMAP.au16M3LumaCal, au16M3LumaCal, sizeof(HI_U16) * 3);
    pstPqHdrCfg->stTMAP.u32ClipMinTM = u32ClipMinTM;
    pstPqHdrCfg->stTMAP.u32ClipMaxTM = u32ClipMaxTM;
    memset(pstPqHdrCfg->stTMAP.as32TMOff, 0, sizeof(HI_S32) * 3);
    pstPqHdrCfg->stTMAP.u16ScaleCoefTM = u16ScaleCoefTM_HDR10toSDR;
    memcpy(pstPqHdrCfg->stTMAP.au32StepTM, au32TMapStep, sizeof(HI_U32) * PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32PosTM, au32TMapPos, sizeof(HI_U32) * PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32NumTM, au32TMapNum, sizeof(HI_U32) * PQ_TMAP_SEG_SIZE);
    pstPqHdrCfg->stTMAP.pu32LUTTM = (HI_U32*)au32TMLut_HDR10toSDR;

    /* GamutMapping */
    pstPqHdrCfg->stGMAP.u16ScaleGMAP = u16ScaleGMap;
    memcpy(pstPqHdrCfg->stGMAP.as16M33GMAP, as16M33GMap_2020to709, sizeof(HI_S16) * 9);
    pstPqHdrCfg->stGMAP.u32ClipMinGMAP = u32ClipMinGMap;
    pstPqHdrCfg->stGMAP.u32ClipMaxGMAP = u32ClipMaxGMap;

    /*  Gamma */
    memcpy(pstPqHdrCfg->stGmm.au32Step, au32GmmStep, sizeof(HI_U32) * PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Pos, au32GmmPos, sizeof(HI_U32) * PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Num, au32GmmNum, sizeof(HI_U32) * PQ_GMM_SEG_SIZE);
    pstPqHdrCfg->stGmm.pu32LUT = (HI_U32*)au32GmmLut_Gmm;

    /* RGB2YUV */
    pstPqHdrCfg->stR2Y.u16ScaleR2Y = u16ScaleR2Y;
    memcpy(pstPqHdrCfg->stR2Y.as16M33R2Y, as16M33R2Y_709_S78, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stR2Y.as16DcInR2Y, as16DcInR2Y, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stR2Y.as16DcOutR2Y, as16DcOutR2Y, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stR2Y.u16ClipMinR2Y_Y = u16ClipMinR2Y;
    pstPqHdrCfg->stR2Y.u16ClipMaxR2Y_Y = u16ClipMaxR2Y;

    return HI_SUCCESS;
}

static HI_S32 pq_hal_GetHLGtoSDRCfg(HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    pstPqHdrCfg->bY2REn 		= HI_TRUE ;
    pstPqHdrCfg->bDegammaEn 	= HI_TRUE ;
    pstPqHdrCfg->bTMapEn		= HI_TRUE ;
    pstPqHdrCfg->bGMapEn		= HI_TRUE ;
    pstPqHdrCfg->bGammaEn		= HI_TRUE ;
    pstPqHdrCfg->bDitherEn		= HI_TRUE ;
    pstPqHdrCfg->bR2YEn 		= HI_TRUE ;
    pstPqHdrCfg->bGMapPosSel	= HI_TRUE ;
    pstPqHdrCfg->bDitherMode	= HI_TRUE ;

    /* YUV2RGB */
    pstPqHdrCfg->stY2R.u16ScaleY2R = u16ScaleY2R;
    memcpy(pstPqHdrCfg->stY2R.as16M33Y2R, as16M33Y2R_2020, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stY2R.as16DcInY2R, as16DcInY2R, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stY2R.as16DcOutY2R, as16DcOutY2R, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stY2R.u16ClipMinY2R = u16ClipMinY2R;
    pstPqHdrCfg->stY2R.u16ClipMaxY2R = u16ClipMaxY2R;

    /* DeGamma */
    memcpy(pstPqHdrCfg->stDeGmm.au32Step, au32DeGmm_step, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Pos, au32DeGmm_pos, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Num, au32DeGmm_num, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    pstPqHdrCfg->stDeGmm.pu32LUT = (HI_U32*)au32DeGmmLut_HLG;

    /* ToneMapping */
    pstPqHdrCfg->stTMAP.u16ScaleLumaCal = u16ScaleLumaCal;
    memcpy(pstPqHdrCfg->stTMAP.au16M3LumaCal, au16M3LumaCal, sizeof(HI_U16) * 3);
    pstPqHdrCfg->stTMAP.u32ClipMinTM = u32ClipMinTM;
    pstPqHdrCfg->stTMAP.u32ClipMaxTM = u32ClipMaxTM;
    memset(pstPqHdrCfg->stTMAP.as32TMOff, 0, sizeof(HI_S32) * 3);
    pstPqHdrCfg->stTMAP.u16ScaleCoefTM = u16ScaleCoefTM_HLGtoSDR;
    memcpy(pstPqHdrCfg->stTMAP.au32StepTM, au32TMapStep, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32PosTM, au32TMapPos, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32NumTM, au32TMapNum, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    pstPqHdrCfg->stTMAP.pu32LUTTM = (HI_U32*)au32TMLut_HLGtoSDR;

    /* GamutMapping */
    pstPqHdrCfg->stGMAP.u16ScaleGMAP = u16ScaleGMap;
    memcpy(pstPqHdrCfg->stGMAP.as16M33GMAP, as16M33GMap_2020to709, sizeof(HI_S16) * 9);
    pstPqHdrCfg->stGMAP.u32ClipMinGMAP = u32ClipMinGMap;
    pstPqHdrCfg->stGMAP.u32ClipMaxGMAP = u32ClipMaxGMap;

    /*	Gamma */
    memcpy(pstPqHdrCfg->stGmm.au32Step, au32GmmStep, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Pos, au32GmmPos, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Num, au32GmmNum, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    pstPqHdrCfg->stGmm.pu32LUT = (HI_U32*)au32GmmLut_Gmm;

    /* RGB2YUV */
    pstPqHdrCfg->stR2Y.u16ScaleR2Y = u16ScaleR2Y;
    memcpy(pstPqHdrCfg->stR2Y.as16M33R2Y, as16M33R2Y_709, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stR2Y.as16DcInR2Y, as16DcInR2Y, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stR2Y.as16DcOutR2Y, as16DcOutR2Y, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stR2Y.u16ClipMinR2Y_Y = u16ClipMinR2Y;
    pstPqHdrCfg->stR2Y.u16ClipMaxR2Y_Y = u16ClipMaxR2Y;

    return HI_SUCCESS;
}

static HI_S32 pq_hal_GetHLGtoHDR10Cfg(HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    pstPqHdrCfg->bY2REn 		= HI_TRUE ;
    pstPqHdrCfg->bDegammaEn 	= HI_TRUE ;
    pstPqHdrCfg->bTMapEn		= HI_TRUE ;
    pstPqHdrCfg->bGMapEn		= HI_FALSE ;
    pstPqHdrCfg->bGammaEn		= HI_TRUE ;
    pstPqHdrCfg->bDitherEn		= HI_TRUE ;
    pstPqHdrCfg->bR2YEn 		= HI_TRUE ;
    pstPqHdrCfg->bGMapPosSel	= HI_TRUE ;
    pstPqHdrCfg->bDitherMode	= HI_TRUE ;

    /* YUV2RGB */
    pstPqHdrCfg->stY2R.u16ScaleY2R = u16ScaleY2R;
    memcpy(pstPqHdrCfg->stY2R.as16M33Y2R, as16M33Y2R_2020, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stY2R.as16DcInY2R, as16DcInY2R, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stY2R.as16DcOutY2R, as16DcOutY2R, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stY2R.u16ClipMinY2R = u16ClipMinY2R;
    pstPqHdrCfg->stY2R.u16ClipMaxY2R = u16ClipMaxY2R;

    /* DeGamma */
    memcpy(pstPqHdrCfg->stDeGmm.au32Step, au32DeGmm_step, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Pos, au32DeGmm_pos, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stDeGmm.au32Num, au32DeGmm_num, sizeof(HI_U32)*PQ_DEGMM_SEG_SIZE);
    pstPqHdrCfg->stDeGmm.pu32LUT = (HI_U32*)au32DeGmmLut_HLG;

    /* ToneMapping */
    pstPqHdrCfg->stTMAP.u16ScaleLumaCal = u16ScaleLumaCal;
    memcpy(pstPqHdrCfg->stTMAP.au16M3LumaCal, au16M3LumaCal, sizeof(HI_U16) * 3);
    pstPqHdrCfg->stTMAP.u32ClipMinTM = u32ClipMinTM;
    pstPqHdrCfg->stTMAP.u32ClipMaxTM = u32ClipMaxTM;
    memset(pstPqHdrCfg->stTMAP.as32TMOff, 0, sizeof(HI_S32) * 3);
    pstPqHdrCfg->stTMAP.u16ScaleCoefTM = u16ScaleCoefTM_HLGtoHDR10;
    memcpy(pstPqHdrCfg->stTMAP.au32StepTM, au32TMapStep, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32PosTM, au32TMapPos, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    memcpy(pstPqHdrCfg->stTMAP.au32NumTM, au32TMapNum, sizeof(HI_U32)*PQ_TMAP_SEG_SIZE);
    pstPqHdrCfg->stTMAP.pu32LUTTM = (HI_U32*)au32TMLut_HLGtoHDR10;

    /* GamutMapping */
    pstPqHdrCfg->stGMAP.u16ScaleGMAP = u16ScaleGMap;
    memcpy(pstPqHdrCfg->stGMAP.as16M33GMAP, as16M33GMap_linear, sizeof(HI_S16) * 9);
    pstPqHdrCfg->stGMAP.u32ClipMinGMAP = u32ClipMinGMap;
    pstPqHdrCfg->stGMAP.u32ClipMaxGMAP = u32ClipMaxGMap;

    /*	Gamma */
    memcpy(pstPqHdrCfg->stGmm.au32Step, au32GmmStep, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Pos, au32GmmPos, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    memcpy(pstPqHdrCfg->stGmm.au32Num, au32GmmNum, sizeof(HI_U32)*PQ_GMM_SEG_SIZE);
    pstPqHdrCfg->stGmm.pu32LUT = (HI_U32*)au32GmmLut_PQ1250;

    /* RGB2YUV */
    pstPqHdrCfg->stR2Y.u16ScaleR2Y = u16ScaleR2Y;
    memcpy(pstPqHdrCfg->stR2Y.as16M33R2Y, as16M33R2Y_2020, sizeof(HI_S16) * 9);
    memcpy(pstPqHdrCfg->stR2Y.as16DcInR2Y, as16DcInR2Y, sizeof(HI_S16) * 3);
    memcpy(pstPqHdrCfg->stR2Y.as16DcOutR2Y, as16DcOutR2Y, sizeof(HI_S16) * 3);
    pstPqHdrCfg->stR2Y.u16ClipMinR2Y_Y = u16ClipMinR2Y;
    pstPqHdrCfg->stR2Y.u16ClipMaxR2Y_Y = u16ClipMaxR2Y;

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetHDRCfg(HI_PQ_WIN_HDR_INFO* pstWinHdrInfo, HI_PQ_HDR_CFG* pstPqHdrCfg)
{
    HI_S32 s32Ret = HI_FAILURE;

    PQ_CHECK_NULL_PTR(pstWinHdrInfo);
    PQ_CHECK_NULL_PTR(pstPqHdrCfg);

    if ( (HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstWinHdrInfo->enSrcFrameType)
         && (HI_PQ_DISP_TYPE_HDR10 == pstWinHdrInfo->enDispType) )
    {
        sg_enHDRMode = PQ_HAL_HDR_MODE_HDR10_TO_HDR10;
        s32Ret = pq_hal_GetHDR10toHDR10Cfg(pstPqHdrCfg);
    }
    else if ( (HI_DRV_VIDEO_FRAME_TYPE_HDR10 == pstWinHdrInfo->enSrcFrameType)
              && (HI_PQ_DISP_TYPE_NORMAL == pstWinHdrInfo->enDispType) )
    {
        sg_enHDRMode = PQ_HAL_HDR_MODE_HDR10_TO_SDR;
        s32Ret = pq_hal_GetHDR10toSDRCfg(pstPqHdrCfg);
    }
    else if ( (HI_DRV_VIDEO_FRAME_TYPE_HLG == pstWinHdrInfo->enSrcFrameType)
              && (HI_PQ_DISP_TYPE_NORMAL == pstWinHdrInfo->enDispType) )
    {
        sg_enHDRMode = PQ_HAL_HDR_MODE_HLG_TO_SDR;
        s32Ret = pq_hal_GetHLGtoSDRCfg(pstPqHdrCfg);
    }
    else if ( (HI_DRV_VIDEO_FRAME_TYPE_HLG == pstWinHdrInfo->enSrcFrameType)
              && (HI_PQ_DISP_TYPE_HDR10 == pstWinHdrInfo->enDispType) )
    {
        sg_enHDRMode = PQ_HAL_HDR_MODE_HLG_TO_HDR10;
        s32Ret = pq_hal_GetHLGtoHDR10Cfg(pstPqHdrCfg);
    }

    return s32Ret;
}

static HI_VOID pq_hal_SetHDRPqParamDefault(HI_VOID)
{
    if (HI_NULL == sg_pstHalHdrPqParam)
    {
        return;
    }

    return ;
}

HI_S32 PQ_HAL_SetHDRSaturation(HI_U32 u32Saturation)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_InitHDR(PQ_PARAM_S* pstPqParam, HI_BOOL bDefault)
{
    HI_U32 u32HdrSaturation   = 50;

    sg_pstHalHdrPqParam = pstPqParam;

    PQ_CHECK_NULL_PTR(sg_pstHalHdrPqParam);

    if (HI_TRUE == bDefault)
    {
        pq_hal_SetHDRPqParamDefault();
    }
    else
    {
    }

    PQ_HAL_SetHDRSaturation(u32HdrSaturation);

    return HI_SUCCESS;
}
HI_S32 PQ_HAL_SetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut)
{
    return HI_SUCCESS;
}

HI_S32 PQ_HAL_GetHDRTmLut(HI_PQ_HDR_TM_LUT_S* pstTMSetLut)
{

    return HI_SUCCESS;
}

HI_S32 PQ_HAL_SetHDRDefaultCfg(HI_BOOL bDefault)
{
    pq_hal_SetHDRPqParamDefault();
    return HI_SUCCESS;
}


