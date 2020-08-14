// *****************************************************************************
//
// Copyright(c) 2015,Hisilicon Technologies Co., Ltd.
// All Rights Reserved
//
// Project name : VDP
// File Name    : coef.cpp
// Author       : z00294247
// Date         : 2015/7/17
//
// Module Name  : COEF.CPP
// -----------------------------------------------------------------------------
// Abstract     :
//code_template.v*
//
// *****************************************************************************
// Called By    : TOP
// Modification History
// -----------------------------------------------------------------------------
// $Log :     $
//
//
// --=========================================================================--


#include "vdp_drv_vdm_coef.h"
//#include "vdm_process.h"
#include "vdp_drv_coef.h"
#include "vdp_define.h"
#include "vdp_fpga_define.h"
#include "vdp_hal_comm.h"
#include "vdp_hal_ip_vdm.h"
#include "VdpHiHDR_cfg.h"
#include "vdp_drv_comm.h"

/************* DeGamma Luts **************/
#define VDP_COEF_COMPILE
HI_U32 u32TMLutHisi2SDR[32] = {                //U8.8 [0,65535]
    6399,
    6246,
    6097,
    5953,
    5814,
    5679,
    5548,
    5421,
    5298,
    4843,
    4441,
    4085,
    3769,
    3488,
    3237,
    3013,
    2812,
    2631,
    2469,
    2322,
    2189,
    2068,
    1958,
    1353,
    1022,
    819 ,
    683 ,
    512 ,
    410 ,
    341 ,
    293 ,
    256
};
/************* DeGamma Luts **************/
HI_U32 u32DePQLut[64] = {
    0      ,
    11     ,
    62     ,
    208    ,
    542    ,
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

// DeSLF Curve: AVS HDR
HI_U32 u32DeSLFLut[64] = {                           //U20.0 [0,1048575]
    0        ,
    0        ,
    0        ,
    1        ,
    1        ,
    2        ,
    4        ,
    7        ,
    10       ,
    16       ,
    24       ,
    35       ,
    51       ,
    71       ,
    99       ,
    136      ,
    184      ,
    246      ,
    325      ,
    425      ,
    550      ,
    706      ,
    899      ,
    1135     ,
    1422     ,
    1771     ,
    2191     ,
    2694     ,
    3293     ,
    4005     ,
    4846     ,
    5837     ,
    6999     ,
    8356     ,
    9938     ,
    11776    ,
    13905    ,
    16365    ,
    19202    ,
    22466    ,
    26216    ,
    30518    ,
    35445    ,
    41085    ,
    47535    ,
    54910    ,
    63341    ,
    72982    ,
    84015    ,
    96655    ,
    111156   ,
    127829   ,
    147049   ,
    169280   ,
    195096   ,
    225228   ,
    260609   ,
    302458   ,
    352398   ,
    412641   ,
    486278   ,
    577771   ,
    693812   ,
    1048575
};
// DeHLG Curve: BBC HDR
HI_U32 u32DeHLGLut[64] = {                           //U20.0 [0,1048575]
    0      ,
    85     ,
    341    ,
    768    ,
    1365   ,
    2133   ,
    3072   ,
    4181   ,
    5461   ,
    6912   ,
    8533   ,
    10325  ,
    12288  ,
    14421  ,
    16725  ,
    19200  ,
    21845  ,
    24661  ,
    27648  ,
    30805  ,
    34133  ,
    37632  ,
    41301  ,
    45141  ,
    49152  ,
    53333  ,
    57685  ,
    62208  ,
    66901  ,
    71765  ,
    76800  ,
    82005  ,
    87381  ,
    93088  ,
    99316  ,
    106113 ,
    113530 ,
    121625 ,
    130458 ,
    140098 ,
    150619 ,
    162099 ,
    174628 ,
    188301 ,
    203223 ,
    219506 ,
    237277 ,
    256670 ,
    277833 ,
    300929 ,
    326133 ,
    353639 ,
    383656 ,
    416414 ,
    452163 ,
    491175 ,
    533750 ,
    580211 ,
    630915 ,
    686248 ,
    746633 ,
    812532 ,
    884447 ,
    1048575
};

// DeGamma Curve: SDR
HI_U32 u32DeGmmLut[64] = {                           //U20.0 [0,1048575]
        0  ,
    3641   ,
    7282   ,
    10923  ,
    14564  ,
    18204  ,
    21907  ,
    26051  ,
    30591  ,
    35537  ,
    40892  ,
    46664  ,
    52857  ,
    59478  ,
    66530  ,
    74019  ,
    81950  ,
    90327  ,
    99155  ,
    108437 ,
    118178 ,
    128382 ,
    139053 ,
    150193 ,
    161808 ,
    173901 ,
    186474 ,
    199532 ,
    213077 ,
    227114 ,
    241644 ,
    256671 ,
    272199 ,
    288230 ,
    304766 ,
    321811 ,
    339368 ,
    357439 ,
    376027 ,
    395135 ,
    414764 ,
    434919 ,
    455600 ,
    476812 ,
    498555 ,
    520833 ,
    543647 ,
    567001 ,
    590895 ,
    615334 ,
    640318 ,
    665850 ,
    691933 ,
    718567 ,
    745756 ,
    773502 ,
    801805 ,
    830670 ,
    860096 ,
    890087 ,
    920645 ,
    951770 ,
    983466 ,
    1048575
};

// DeHisi Curve: Hisi HDR
HI_U32 u32DeHisiLut[64] = {                          //U20.0 [0,1048575]
    0        ,
    0        ,
    0        ,
    1        ,
    1        ,
    2        ,
    4        ,
    7        ,
    10       ,
    16       ,
    24       ,
    35       ,
    51       ,
    71       ,
    99       ,
    136      ,
    184      ,
    246      ,
    325      ,
    425      ,
    550      ,
    706      ,
    899      ,
    1135     ,
    1422     ,
    1771     ,
    2191     ,
    2694     ,
    3293     ,
    4005     ,
    4846     ,
    5837     ,
    6999     ,
    8356     ,
    9938     ,
    11776    ,
    13905    ,
    16365    ,
    19202    ,
    22466    ,
    26216    ,
    30518    ,
    35445    ,
    41085    ,
    47535    ,
    54910    ,
    63341    ,
    72982    ,
    84015    ,
    96655    ,
    111156   ,
    127829   ,
    147049   ,
    169280   ,
    195096   ,
    225228   ,
    260609   ,
    302458   ,
    352398   ,
    412641   ,
    486278   ,
    577771   ,
    693812   ,
    1048575
};



/************* ToneMapping Luts **************/
// ToneMapping Lut: Default
HI_U32 u32TMLut[64] = {//U8.8 [0,65535]
    11127,
    11127,
    11123,
    11119,
    11116,
    11101,
    11086,
    11072,
    11057,
    11042,
    11028,
    11013,
    10955,
    10897,
    10783,
    10671,
    10560,
    10451,
    10237,
    10028,
    9826 ,
    9629 ,
    9437 ,
    8721 ,
    8078 ,
    7500 ,
    6980 ,
    6511 ,
    6087 ,
    5703 ,
    5354 ,
    5038 ,
    4750 ,
    4487 ,
    4247 ,
    4027 ,
    3825 ,
    3167 ,
    2684 ,
    2320 ,
    2039 ,
    1816 ,
    1637 ,
    1489 ,
    1365 ,
    1260 ,
    1170 ,
    1092 ,
    1024 ,
    964  ,
    910  ,
    862  ,
    819  ,
    683  ,
    585  ,
    512  ,
    455  ,
    410  ,
    372  ,
    341  ,
    315  ,
    293  ,
    273  ,
    256
/*
4160,
4160,
4160,
4160,
4160,
4160,
3771,
3418,
3134,
2899,
2700,
2531,
2383,
2254,
2140,
2038,
1946,
1788,
1638,
1489,
1365,
1260,
1170,
1092,
1024,
819 ,
683 ,
512 ,
410 ,
341 ,
293 ,
256
*/
};

// ToneMapping Lut: HDR10  to SDR
HI_U32 u32TMLutHDR102SDR[64] = {                   //U9.7 [0,65535]
    37694,
    37694,
    35601,
    33818,
    32273,
    27682,
    24599,
    22352,
    20623,
    19243,
    18109,
    17158,
    14482,
    12802,
    10757,
    9526 ,
    8687 ,
    8068 ,
    7203 ,
    6611 ,
    6170 ,
    5823 ,
    5538 ,
    4740 ,
    4214 ,
    3818 ,
    3499 ,
    3232 ,
    3002 ,
    2801 ,
    2624 ,
    2465 ,
    2323 ,
    2195 ,
    2079 ,
    1973 ,
    1876 ,
    1561 ,
    1330 ,
    1155 ,
    1020 ,
    910  ,
    819  ,
    745  ,
    683  ,
    630  ,
    585  ,
    546  ,
    512  ,
    482  ,
    455  ,
    431  ,
    410  ,
    341  ,
    293  ,
    256  ,
    228  ,
    205  ,
    186  ,
    171  ,
    158  ,
    146  ,
    137  ,
    128
};

// ToneMapping Lut: AVS to SDR
HI_U32 u32TMLutAVS2SDR[64] = {                 //U9.7 [0,65535]
    30773,
    30773,
    28984,
    27460,
    26140,
    22218,
    19586,
    17667,
    16192,
    15015,
    14049,
    13239,
    10963,
    9539 ,
    7813 ,
    6784 ,
    6089 ,
    5583 ,
    4886 ,
    4422 ,
    4086 ,
    3828 ,
    3623 ,
    3083 ,
    2758 ,
    2530 ,
    2354 ,
    2211 ,
    2091 ,
    1986 ,
    1893 ,
    1810 ,
    1734 ,
    1665 ,
    1601 ,
    1542 ,
    1487 ,
    1299 ,
    1150 ,
    1029 ,
    928  ,
    844  ,
    772  ,
    710  ,
    657  ,
    611  ,
    571  ,
    535  ,
    503  ,
    475  ,
    450  ,
    427  ,
    407  ,
    341  ,
    293  ,
    256  ,
    228  ,
    205  ,
    186  ,
    171  ,
    158  ,
    146  ,
    137  ,
    128
};

// ToneMapping Lut: AVS to HDR10
HI_U32 u32TMLutAVS2HDR10[64] = {                   //U8.8 [0,65535]
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

// ToneMapping Lut: HDR10 to BBC
HI_U32 u32TMLutHDR102BBC[64] = {                   //U6.10 [0,65535]
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

// ToneMapping Lut: BBC to HDR10
HI_U32 u32TMLutBBC2HDR10[64] = {                   //U1.15 [0,65535]
    357 ,
    357 ,
    410 ,
    444 ,
    471 ,
    540 ,
    586 ,
    621 ,
    649 ,
    673 ,
    694 ,
    713 ,
    773 ,
    819 ,
    888 ,
    941 ,
    984 ,
    1021,
    1081,
    1130,
    1172,
    1209,
    1242,
    1347,
    1426,
    1491,
    1547,
    1595,
    1638,
    1677,
    1713,
    1746,
    1777,
    1805,
    1832,
    1858,
    1882,
    1968,
    2041,
    2105,
    2162,
    2213,
    2261,
    2304,
    2345,
    2382,
    2418,
    2452,
    2483,
    2514,
    2543,
    2570,
    2597,
    2693,
    2777,
    2853,
    2921,
    2983,
    3040,
    3094,
    3144,
    3190,
    3235,
    3277
};

// ToneMapping Lut: BBC to SDR
HI_U32 u32TMLutBBC2SDR[64] = {                 //U1.15 [0,65535]
    40715,
    40715,
    40165,
    39847,
    39623,
    39088,
    38778,
    38560,
    38392,
    38255,
    38139,
    38040,
    37738,
    37526,
    37229,
    37020,
    36858,
    36726,
    36520,
    36360,
    36231,
    36121,
    36027,
    35741,
    35540,
    35385,
    35259,
    35153,
    35061,
    34980,
    34908,
    34842,
    34783,
    34728,
    34678,
    34631,
    34587,
    34436,
    34313,
    34210,
    34120,
    34042,
    33971,
    33908,
    33850,
    33797,
    33748,
    33702,
    33660,
    33620,
    33582,
    33547,
    33513,
    33393,
    33292,
    33205,
    33129,
    33060,
    32999,
    32942,
    32891,
    32843,
    32799,
    32757
};

// ToneMapping Lut: Hisi to HDR10
HI_U32 u32TMLutHisi2HDR10[32] = {                  //U8.8 [0,65535]
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



/************* Gamma Luts **************/
HI_U32 u32GmmLut0[64] = {//U12.0 [0,4095]
0   ,
72  ,
144 ,
216 ,
288 ,
360 ,
426 ,
486 ,
541 ,
592 ,
641 ,
686 ,
730 ,
772 ,
812 ,
850 ,
887 ,
923 ,
957 ,
991 ,
1024,
1055,
1086,
1116,
1146,
1174,
1203,
1230,
1257,
1284,
1310,
1335,
1360,
1409,
1456,
1502,
1547,
1590,
1632,
1673,
1713,
1753,
1791,
1829,
1866,
1902,
1937,
1972,
2006,
2138,
2261,
2378,
2489,
2595,
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

/******************* Gmm-8 Lut Params *************************/
HI_U32 u32PQLut[64] = {            //U12.0 [0,4095]
    0   ,
    304 ,
    400 ,
    466 ,
    517 ,
    657 ,
    749 ,
    820 ,
    878 ,
    927 ,
    970 ,
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

// PQ Curve
HI_U32 u32PQLut2500[64] = {            //U12.0 [0,4095]
    0   ,
    167 ,
    227 ,
    270 ,
    304 ,
    400 ,
    466 ,
    517 ,
    559 ,
    596 ,
    628 ,
    657 ,
    749 ,
    820 ,
    927 ,
    1008,
    1074,
    1129,
    1220,
    1293,
    1354,
    1407,
    1454,
    1600,
    1708,
    1794,
    1865,
    1926,
    1980,
    2028,
    2071,
    2110,
    2146,
    2179,
    2210,
    2239,
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
    2896,
    2922,
    2946,
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
};

//HLG Curve
HI_U32 u32HLGLut[64] = {            //U12.0 [0,4095]
    0   ,
    28  ,
    39  ,
    48  ,
    55  ,
    78  ,
    96  ,
    111 ,
    124 ,
    136 ,
    147 ,
    157 ,
    192 ,
    222 ,
    271 ,
    313 ,
    350 ,
    384 ,
    443 ,
    496 ,
    543 ,
    586 ,
    627 ,
    768 ,
    887 ,
    991 ,
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

// Gamma Curve
HI_U32 u32GmmLut[64] = {            //U12.0 [0,4095]
    0   ,
    0   ,
    1   ,
    1   ,
    1   ,
    2   ,
    3   ,
    4   ,
    6   ,
    7   ,
    8   ,
    9   ,
    13  ,
    18  ,
    27  ,
    36  ,
    45  ,
    54  ,
    72  ,
    90  ,
    108 ,
    126 ,
    144 ,
    216 ,
    288 ,
    360 ,
    426 ,
    486 ,
    541 ,
    592 ,
    641 ,
    686 ,
    730 ,
    772 ,
    812 ,
    850 ,
    887 ,
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



extern VDP_COEF_ADDR_S gstVdpCoefBufAddr;
HI_VOID VDP_DRV_SetBt2020GammaCoef(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG   *HiHdrCfg)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
    HI_U32 addr = 0;
#else
    HI_U8  *addr    = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_VDM_GAMMA] ;//add by hyx

#endif

    VDP_COEF_SEND_CFG   stCoefSend;


    HI_S32 tcLut_s32[64];


    void*  p_coef_array[]    = {tcLut_s32};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {12};
    FILE*   fp_hihdr_vdm_degmm_coef=NULL;


    VDP_VID_SetVdmVdmParaGmmAddr     (VDP_LAYER_VP0,        gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_VDM_GAMMA]);


    if(enHiHdrMode == HIHDR_HDR10_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else if(enHiHdrMode == HIHDR_BT2020_IN_709_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BT2020_IN_709_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else if(enHiHdrMode == HIHDR_HDR10_IN_HLG_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");

        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == Default !\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }

    stCoefSend.coef_addr         = addr                     ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ;
    stCoefSend.fp_coef           = fp_hihdr_vdm_degmm_coef  ;
    stCoefSend.lut_num           = 1                        ;
    stCoefSend.burst_num         = 1                        ;
    stCoefSend.cycle_num         = 10                       ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
    VDP_VID_SetVdmVdmParaGmmUpd         (VDP_LAYER_VP0, 1);

#endif

}

HI_VOID Vdp_Drv_SetVdmCoef_gmm(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG   *HiHdrCfg)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
    HI_U32 addr = 0;
#else
    HI_U8  *addr    = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_VDM_GAMMA] ;//add by hyx

#endif

    VDP_COEF_SEND_CFG   stCoefSend;


    HI_S32 tcLut_s32[64];


    void*  p_coef_array[]    = {tcLut_s32};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {12};
    FILE*   fp_hihdr_vdm_degmm_coef=NULL;


    VDP_VID_SetVdmVdmParaGmmAddr     (VDP_LAYER_VP0,        gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_VDM_GAMMA]);


#if CBB_CFG_NOT_FROM_PQ
    if(enHiHdrMode == HIHDR_HDR10_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else if(enHiHdrMode == HIHDR_BT2020_IN_709_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BT2020_IN_709_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32GmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else if(enHiHdrMode == HIHDR_HDR10_IN_HLG_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");

        p_coef_array[0]    = (HI_S32*)u32PQLut     ;
    }
    else
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == Default !\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
#else
    p_coef_array[0]    = (HI_S32*)HiHdrCfg->stGmm.pu32LUT;
#endif

    stCoefSend.coef_addr         = addr                     ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ;
    stCoefSend.fp_coef           = fp_hihdr_vdm_degmm_coef  ;
    stCoefSend.lut_num           = 1                        ;
    stCoefSend.burst_num         = 1                        ;
    stCoefSend.cycle_num         = 10                       ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
    VDP_VID_SetVdmVdmParaGmmUpd         (VDP_LAYER_VP0, 1);

#endif

}


HI_VOID VDP_DRV_SetBt2020DegmmaCoef(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
    HI_U32 addr = 0;
#else
    HI_U8  *addr    = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_VDM_DEGAMMA] ;//add by hyx

#endif

    VDP_COEF_SEND_CFG   stCoefSend;


    HI_S32 tcLut_s32[64];


    void*  p_coef_array[]    = {tcLut_s32};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {20};
    FILE*   fp_hihdr_vdm_degmm_coef=NULL;


    VDP_VID_SetVdmVdmParaDegmmAddr  (VDP_LAYER_VP0,        gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_VDM_DEGAMMA]);


    if(enHiHdrMode == HIHDR_HDR10_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_HDR10_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeHLGLut     ;
    }
    else if(enHiHdrMode == HIHDR_BT2020_IN_709_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BT2020_IN_709_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeGmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeHLGLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeSLFLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_HDR10_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeSLFLut     ;
    }
    else if(enHiHdrMode == HIHDR_HDR10_IN_HLG_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");

        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
    else
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == Default !\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }

    stCoefSend.coef_addr         = addr                     ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM        ;
    stCoefSend.fp_coef           = fp_hihdr_vdm_degmm_coef              ;
    stCoefSend.lut_num           = 1              ;
    stCoefSend.burst_num         = 1            ;
    stCoefSend.cycle_num         = 6            ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
    VDP_VID_SetVdmVdmParaDegmmUpd     (VDP_LAYER_VP0, 1);

#endif

}

HI_VOID Vdp_Drv_SetVdmCoef_Degmm(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
    HI_U32 addr = 0;
#else
    HI_U8  *addr    = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_VDM_DEGAMMA] ;//add by hyx

#endif

    VDP_COEF_SEND_CFG   stCoefSend;


    HI_S32 tcLut_s32[64];


    void*  p_coef_array[]    = {tcLut_s32};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {20};
    FILE*   fp_hihdr_vdm_degmm_coef=NULL;


    VDP_VID_SetVdmVdmParaDegmmAddr  (VDP_LAYER_VP0,        gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_VDM_DEGAMMA]);


#if CBB_CFG_NOT_FROM_PQ
    if(enHiHdrMode == HIHDR_HDR10_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_HDR10_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeHLGLut     ;
    }
    else if(enHiHdrMode == HIHDR_BT2020_IN_709_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BT2020_IN_709_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeGmmLut     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeHLGLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_SDR_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeSLFLut     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_HDR10_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32DeSLFLut     ;
    }
    else if(enHiHdrMode == HIHDR_HDR10_IN_HLG_OUT)
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");

        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
    else
    {
        //VDP_PRINT("DeGamma Coef : VdmSceneMode == Default !\n");
        p_coef_array[0]    = (HI_S32*)u32DePQLut     ;
    }
#else
        p_coef_array[0]    = (HI_S32*)HiHdrCfg->stDeGmm.pu32LUT;
#endif

    stCoefSend.coef_addr         = addr                     ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM        ;
    stCoefSend.fp_coef           = fp_hihdr_vdm_degmm_coef              ;
    stCoefSend.lut_num           = 1              ;
    stCoefSend.burst_num         = 1            ;
    stCoefSend.cycle_num         = 6            ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
    VDP_VID_SetVdmVdmParaDegmmUpd     (VDP_LAYER_VP0, 1);

#endif

}


HI_VOID Vdp_Drv_SetVdmCoef_Tmapping(HIHDR_SCENE_MODE_E  enHiHdrMode,HI_PQ_HDR_CFG  *HiHdrCfg)
{
#ifdef VDP_COEF_COMPILE

#if UT_TEST
    HI_U32 addr = 0;
#else
    HI_U8  *addr    = gstVdpCoefBufAddr.pu8CoefVirAddr[VDP_COEF_BUF_VDM_TONEMAP] ;//add by hyx

#endif

    VDP_COEF_SEND_CFG   stCoefSend;


    HI_S32 tcLut_s32[64];


    void*  p_coef_array[]    = {tcLut_s32};
    HI_U32 lut_length[]      = {64};
    HI_U32 coef_bit_length[] = {16};
    FILE*   fp_hihdr_vdm_degmm_coef=NULL;


    VDP_VID_SetVdmVdmParaTmapAddr   (VDP_LAYER_VP0,        gstVdpCoefBufAddr.u32CoefPhyAddr[VDP_COEF_BUF_VDM_TONEMAP]);



#if CBB_CFG_NOT_FROM_PQ
    if(enHiHdrMode == HIHDR_HDR10_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32TMLutHDR102SDR     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32TMLutBBC2HDR10     ;
    }
    else if(enHiHdrMode == HIHDR_HLG_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_BBC_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32TMLutBBC2SDR     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_SDR_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_SDR_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32TMLutAVS2SDR     ;
    }
    else if(enHiHdrMode == HIHDR_AVS_IN_HDR10_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_AVS_IN_HDR10_OUT\n");
        p_coef_array[0]    = (HI_S32*)u32TMLutAVS2HDR10     ;
    }
    else if(enHiHdrMode == HIHDR_HDR10_IN_HLG_OUT)
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == VDM_HDR10_IN_BBC_OUT\n");

        p_coef_array[0]    = (HI_S32*)u32TMLutHDR102BBC     ;
    }
    else
    {
        VDP_PRINT("DeGamma Coef : VdmSceneMode == Default !\n");
        p_coef_array[0]    = (HI_S32*)u32TMLut     ;
    }
#else
            p_coef_array[0]    = (HI_S32*)HiHdrCfg->stTMAP.pu32LUTTM;
#endif


    stCoefSend.coef_addr         = addr                     ;
    stCoefSend.sti_type          = STI_FILE_COEF_DM         ;
    stCoefSend.fp_coef           = fp_hihdr_vdm_degmm_coef  ;
    stCoefSend.lut_num           = 1                        ;
    stCoefSend.burst_num         = 1                        ;
    stCoefSend.cycle_num         = 8                        ;
    stCoefSend.p_coef_array      = p_coef_array             ;
    stCoefSend.lut_length        = lut_length               ;
    stCoefSend.coef_bit_length   = coef_bit_length          ;
    stCoefSend.data_type         = COEF_DATA_TYPE_S32       ;

    VDP_FUNC_SendCoef(&stCoefSend);
    VDP_VID_SetVdmVdmParaTmapUpd        (VDP_LAYER_VP0, 1);

#endif

}






