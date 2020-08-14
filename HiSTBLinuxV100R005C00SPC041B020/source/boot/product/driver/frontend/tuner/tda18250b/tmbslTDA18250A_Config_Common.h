/*
Copyright (C) 2010 NXP B.V., All Rights Reserved.
This source code and any compilation or derivative thereof is the proprietary
information of NXP B.V. and is confidential in nature. Under no circumstances
is this software to be  exposed to or placed under an Open Source License of
any type without the expressed written permission of NXP B.V.
*
* \file          tmbslTDA18250A_Config.h
*
* \date          %modify_time%
*
* \author        Alexandre TANT
*
* \brief         Describe briefly the purpose of this file.
*
* REFERENCE DOCUMENTS :
*                TDA18250A_Driver_User_Guide.pdf
*
* \section info Change Information
*
*/

#ifndef _TDA18250A_CONFIG_COMMON_H
#define _TDA18250A_CONFIG_COMMON_H


#ifdef __cplusplus
extern "C"
{
#endif

    /*============================================================================*/
    /* Types and defines:                                                         */
    /*============================================================================*/

    /* Driver settings version definition */
#define TDA18250A_SETTINGS_CUSTOMER_NUM      (0)                     /* SW Settings Customer Number */
#define TDA18250A_SETTINGS_PROJECT_NUM       (0)                     /* SW Settings Project Number  */
#define TDA18250A_SETTINGS_MAJOR_VER         (1)                     /* SW Settings Major Version   */
#define TDA18250A_SETTINGS_MINOR_VER         (5)                     /* SW Settings Minor Version   */

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX 19
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX 12
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX 14
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX 8
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_S2D_MAX 3

#define TDA18250A_RF_MAX         1200000000 

#define TDA18250A_BP_FILTER_1      39424000 
#define TDA18250A_BP_FILTER_2      61952000
#define TDA18250A_BP_FILTER_3      86528000
#define TDA18250A_BP_FILTER_4     123392000
#define TDA18250A_BP_FILTER_5     172544000
#define TDA18250A_BP_FILTER_6     244222000
#define TDA18250A_BP_FILTER_7     320000000

/* {    X     ,    X    ,   X  ,           R           },         */
/* {    0     ,    1    ,   2  ,           F           },         */
/* {          ,         ,      ,                       },         */
/* {    x     ,    x    ,   x  ,                       },         */
/* {    1     ,    1    ,   1  ,           M           },         */
/* {    0     ,    0    ,   0  ,           H           },         */
/* {    0     ,    0    ,   0  ,           z           },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {    0     ,    0    ,   0  ,                       },         */
/* {          ,         ,   0  ,                       },         */
/* {          ,         ,      ,                       },         */
/* {          ,         ,      ,                       },         */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB    \
{                                                               \
   {  48246965,	 345882,	-29629,     TDA18250A_BP_FILTER_1  }, \
   {  48246965,	 345882,	-29629,     TDA18250A_BP_FILTER_2  }, \
   {  52714209,	 156842,	 -9521,     TDA18250A_BP_FILTER_3  }, \
   {  49922821,	 177703,	 -8232,     TDA18250A_BP_FILTER_4  }, \
   {  52037099,	 108895,	 -3929,     TDA18250A_BP_FILTER_5  }, \
   {  59606844,	  -5223,	   107,     TDA18250A_BP_FILTER_6  }, \
   {  52162259,	  45035,	  -762,     TDA18250A_BP_FILTER_7  }, \
   { 118037960,	-339851,	  4977,     335360000              }, \
   {  64602586,	 -22242,	   197,     390656000              }, \
   {  70976293,	 -52604,	   556,     430592000              }, \
   {  51351035,	  40168,	  -506,     473500000              }, \
   {  58157869,	   8858,	  -176,     535040000              }, \
   {  60964665,	  -2013,	   -47,     562688000              }, \
   {  48258159,	  42283,	  -459,     600000000              }, \
   {  80088833,	 -78392,	   592,     654848000              }, \
   {  42410399,	  41578,	  -357,     720384000              }, \
   {  48432980,	  24600,	  -217,     793088000              }, \
   { 206714600,	-355940,	  2046,     865792000              }, \
   {  41824186,	  30871,	  -206,     TDA18250A_RF_MAX}         \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           1           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_OFF        \
{                                                                                \
   { -343284930,	  9989,	 -218,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -318924050,  	 885,	  -39,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   { -289296030,	-19239,	  227,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   { -259484110,	-10871,	  129,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   { -224391360,	  4834,	  -55,      TDA18250AAGC1_GAIN_Plus_1dB     }, \
   { -199120150,	-15351,	  204,      TDA18250AAGC1_GAIN_Plus_4dB     }, \
   { -167449130,	-16834,	  231,      TDA18250AAGC1_GAIN_Plus_7dB     }, \
   { -136916620,	-17650,	  268,      TDA18250AAGC1_GAIN_Plus_10dB    }, \
   { -103409030,	-19602,	  265,      TDA18250AAGC1_GAIN_Plus_13dB    }, \
   {  -75696074,	 11814,	 -122,      TDA18250AAGC1_GAIN_Plus_16dB    }, \
   {  -42736725,	  7907,	  -42,      TDA18250AAGC1_GAIN_Plus_19dB    }, \
   {          0,	     0,	    0,      TDA18250AAGC1_GAIN_Plus_22dB    }  \
}

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_OFF_ST_ON          \
{                                                                                \
   { -343247050,	  2996,	 -157,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -319536860,	 -1643,	  -24,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   { -290436330,	-18578,	  221,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   { -260603260,	-11817,	  141,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   { -224230270,	 -1873,	    3,      TDA18250AAGC1_GAIN_Plus_1dB     }, \
   { -199080890,	-20882,	  244,      TDA18250AAGC1_GAIN_Plus_4dB     }, \
   { -167544820,	-22750,	  270,      TDA18250AAGC1_GAIN_Plus_7dB     }, \
   { -136789190,	-26421,	  327,      TDA18250AAGC1_GAIN_Plus_10dB    }, \
   { -102939750,	-32904,	  359,      TDA18250AAGC1_GAIN_Plus_13dB    }, \
   {  -74877349,	 -6020,	    3,      TDA18250AAGC1_GAIN_Plus_16dB    }, \
   {  -41146315,	-18451,	  142,      TDA18250AAGC1_GAIN_Plus_19dB    }, \
   {    2081896,	-40688,	  303,      TDA18250AAGC1_GAIN_Plus_22dB    }  \
}

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_TUNER_LT_ON_ST_ON          \
{                                                                                \
   { -345153940,	  6831,	 -186,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -320198940,	 -2076,	  -16,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   { -290915180,	-19955,	  220,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   { -261024800,	-14742,	  146,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   { -225307120,	 -2499,	  -11,      TDA18250AAGC1_GAIN_Plus_1dB     }, \
   { -199712490,	-24499,	  249,      TDA18250AAGC1_GAIN_Plus_4dB     }, \
   { -168025740,	-28263,	  278,      TDA18250AAGC1_GAIN_Plus_7dB     }, \
   { -137015850,	-31284,	  323,      TDA18250AAGC1_GAIN_Plus_10dB    }, \
   { -102901760,	-38703,	  367,      TDA18250AAGC1_GAIN_Plus_13dB    }, \
   {  -74513886,	-13294,	   30,      TDA18250AAGC1_GAIN_Plus_16dB    }, \
   {  -40582265,	-26118,	  182,      TDA18250AAGC1_GAIN_Plus_19dB    }, \
   {    2804342,	-50262,	  379,      TDA18250AAGC1_GAIN_Plus_22dB    }  \
}

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_OFF_ST_ON          \
{                                                                                \
   { -148849960,	  1328,	 -274,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -124420860,	 -6213,	 -138,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   {  -95314662,	-24103,	  100,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   {  -65175760,	-17308,	   -3,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   {  -29212184,	 -5983,	 -162,      TDA18250AAGC1_GAIN_Plus_1dB     }, \
   {   -3495200,	-26907,	   88,      TDA18250AAGC1_GAIN_Plus_4dB     }, \
   {   28023489,	-28012,	  102,      TDA18250AAGC1_GAIN_Plus_7dB     }, \
   {   58660735,	-29931,	  137,      TDA18250AAGC1_GAIN_Plus_10dB    }, \
   {   92273775,	-33491,	  134,      TDA18250AAGC1_GAIN_Plus_13dB    }, \
   {  120569010,	 -5418,	 -235,      TDA18250AAGC1_GAIN_Plus_16dB    }, \
   {  154362300,	-14428,	 -138,      TDA18250AAGC1_GAIN_Plus_19dB    }, \
   {  197425560,	-28760,	  -74,      TDA18250AAGC1_GAIN_Plus_22dB    }  \
}

#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_MASTER_LT_ON_ST_ON          \
{                                                                                \
   { -151387640,	  -1628,	-252,      TDA18250AAGC1_GAIN_Minus_11dB   }, \
   { -127209950,	  -6859,	-160,      TDA18250AAGC1_GAIN_Minus_8dB    }, \
   {  -97483906,	 -28409,	 119,      TDA18250AAGC1_GAIN_Minus_5dB    }, \
   {  -67312475,	 -22655,	  18,      TDA18250AAGC1_GAIN_Minus_2dB    }, \
   {  -31169910,	 -12161,	-139,      TDA18250AAGC1_GAIN_Plus_1dB     }, \
   {   -5585317,	 -32456,	  95,      TDA18250AAGC1_GAIN_Plus_4dB     }, \
   {   26101617,	 -35154,	 106,      TDA18250AAGC1_GAIN_Plus_7dB     }, \
   {   57434780,	 -37036,	 141,      TDA18250AAGC1_GAIN_Plus_10dB    }, \
   {   91636366,	 -41741,	 155,      TDA18250AAGC1_GAIN_Plus_13dB    }, \
   {  120337980,	 -14660,	-194,      TDA18250AAGC1_GAIN_Plus_16dB    }, \
   {  154486300,	 -23925,	 -84,      TDA18250AAGC1_GAIN_Plus_19dB    }, \
   {  197933630,	 -40351,	  19,      TDA18250AAGC1_GAIN_Plus_22dB    }  \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           2           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB  \
{                                                    \
   { -258427,	2058,	-112,       10    },       \
   { -260262,	2060,	-112,       21    },       \
   { -262421,	2045,	-108,       32    },       \
   { -265338,	2054,	-105,       43    },       \
   { -266910,	2036,	-101,       54    },       \
   { -269641,	2044,	 -97,       65    },       \
   { -271233,	2023,	 -80,       76    },       \
   { -274006,	2021,	 -62,       87    },       \
   { -275925,	2010,	 -43,       98    },       \
   { -276919,	1990,	 -26,      109    },       \
   { -277342,	1977,	 -15,      120    },       \
   { -275241,	1949,	  -8,      131    },       \
   { -271646,	1913,	   0,      142    }        \
}

/* {    X     ,    X    ,   X  ,           A           },                */
/* {    0     ,    1    ,   2  ,           G           },                */
/* {          ,         ,      ,           C           },                */
/* {    x     ,    x    ,   x  ,           3           },                */
/* {    1     ,    1    ,   1  ,           N           },                */
/* {    0     ,    0    ,   0  ,           B           },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB						        \
{                                                                               \
   { -206266320,	-301,	  0,    TDA18250AAGC3_GAIN_Minus_6dB      }, \
   { -176965600,	-201,	 -2,    TDA18250AAGC3_GAIN_Minus_3dB      }, \
   { -146895830,	-233,	 -1,    TDA18250AAGC3_GAIN_0dB            }, \
   { -116950370,	-111,	 -2,    TDA18250AAGC3_GAIN_Plus_3dB       }, \
   {  -87059349,	-203,	 -1,    TDA18250AAGC3_GAIN_Plus_6dB       }, \
   {  -57821720,	  57,	 -3,    TDA18250AAGC3_GAIN_Plus_9dB       }, \
   {  -28752654,	  97,	 -3,    TDA18250AAGC3_GAIN_Plus_12dB      }, \
   {          0,	   0,	  0,    TDA18250AAGC3_GAIN_Plus_15dB      }  \
}

/* {    X     ,    X    ,   X  ,           S           },                */
/* {    0     ,    1    ,   2  ,           2           },                */
/* {          ,         ,      ,           D           },                */
/* {    x     ,    x    ,   x  ,                       },                */
/* {    1     ,    1    ,   1  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {    0     ,    0    ,   0  ,                       },                */
/* {          ,         ,   0  ,                       },                */
/* {          ,         ,      ,                       },                */
#define TDA18250A_INSTANCE_CUSTOM_COMMON_LVL_S2DNB						  \
{                                                                         \
   { -55110151,	-8517,	  36,              TDA18250A_S2D_Gain_3dB      }, \
   { -27613532,	-4141,	  11,              TDA18250A_S2D_Gain_6dB      }, \
   {         0,	    0,	   0,              TDA18250A_S2D_Gain_9dB      }, \
}

/* Default configuration */
#define TDA18250A_CONFIG_CURPOWERSTATE_DEF tmPowerMax
#define TDA18250A_CONFIG_CURLLPOWERSTATE_DEF TDA18250A_PowerMax
#define TDA18250A_CONFIG_RF_DEF 0
#define TDA18250A_CONFIG_PROG_RF_DEF 0
#define TDA18250A_CONFIG_STANDARDMODE_DEF TDA18250A_StandardMode_Max

    /* Power settings */
#define TDA18250A_CONFIG_POWER_DEF_MASTER \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn,           /* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithLtOnWithXtalOn            /* tmPowerOff (D3) */     \
}

#define TDA18250A_CONFIG_POWER_DEF_SLAVE \
    TDA18250A_CONFIG_CURPOWERSTATE_DEF,                     /* curPowerState */       \
    TDA18250A_CONFIG_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */     \
    {                                                       /* mapLLPowerState */     \
    TDA18250A_PowerNormalMode,                          /* tmPowerOn (D0) */      \
    TDA18250A_PowerStandbyWithXtalOn,			        /* tmPowerStandby (D1) */ \
    TDA18250A_PowerStandbyWithXtalOn,					/* tmPowerSuspend (D2) */ \
    TDA18250A_PowerStandbyWithXtalOn					/* tmPowerOff (D3) */     \
}

    /* Standard Presets Aggregation: */
#define TDA18250A_CONFIG_STD_DEF_VCO_PULLING_MINUS \
    { \
    TDA18250A_CONFIG_STD_QAM_6MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_QAM_8MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_64QAM_6MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_256QAM_6MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_64QAM_8MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_256QAM_8MHZ_VCO_PULLING_MINUS,    \
    TDA18250A_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_MINUS, \
    TDA18250A_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_MINUS,   \
    TDA18250A_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_MINUS,   \
    TDA18250A_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_MINUS,   \
    TDA18250A_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_MINUS,  \
    TDA18250A_CONFIG_STD_DTMB_8MHz_VCO_PULLING_MINUS,   \
    TDA18250A_CONFIG_STD_ISDBT_6MHZ_VCO_PULLING_MINUS,  \
    TDA18250A_CONFIG_STD_ATSC_6MHZ_VCO_PULLING_MINUS,   \
}

#define TDA18250A_CONFIG_STD_DEF_VCO_PULLING_PLUS \
    { \
    TDA18250A_CONFIG_STD_QAM_6MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_QAM_8MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_64QAM_6MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_256QAM_6MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_64QAM_8MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_256QAM_8MHZ_VCO_PULLING_PLUS,    \
    TDA18250A_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_PLUS, \
    TDA18250A_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_PLUS,  \
    TDA18250A_CONFIG_STD_DTMB_8MHz_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_ISDBT_6MHZ_VCO_PULLING_PLUS,  \
    TDA18250A_CONFIG_STD_ATSC_6MHZ_VCO_PULLING_PLUS,   \
}

#define TDA18250A_CONFIG_STD_DEF_VCO_PULLING_PLUS_2 \
    { \
    TDA18250A_CONFIG_STD_QAM_6MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_QAM_8MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_64QAM_6MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_256QAM_6MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_64QAM_8MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_256QAM_8MHZ_VCO_PULLING_PLUS_2,    \
    TDA18250A_CONFIG_STD_DVBT_1_7MHZ_VCO_PULLING_PLUS, \
    TDA18250A_CONFIG_STD_DVBT_6MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_7MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_8MHZ_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_DVBT_10MHZ_VCO_PULLING_PLUS,  \
    TDA18250A_CONFIG_STD_DTMB_8MHz_VCO_PULLING_PLUS,   \
    TDA18250A_CONFIG_STD_ISDBT_6MHZ_VCO_PULLING_PLUS,  \
    TDA18250A_CONFIG_STD_ATSC_6MHZ_VCO_PULLING_PLUS,   \
}


#define TDA18250A_CONFIG_REGMAP_DEF \
    { \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0           \
}


#define TDA18250A_CONFIG_MASTER \
    TDA18250A_CONFIG_POWER_DEF_MASTER, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    False,                                                  /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Enabled,                          /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF_VCO_PULLING_MINUS, \
    TDA18250A_CONFIG_REGMAP_DEF

#define TDA18250A_CONFIG_SLAVE \
    TDA18250A_CONFIG_POWER_DEF_SLAVE, \
    TDA18250A_CONFIG_RF_DEF,                                /* uRF */                            \
    TDA18250A_CONFIG_PROG_RF_DEF,                           /* uProgRF */                        \
    TDA18250A_CONFIG_STANDARDMODE_DEF,                      /* StandardMode */                   \
    Null,                                                   /* pStandard */                      \
    True,                                                   /* bBufferMode */                    \
    False,                                                  /* Manual PLL Calc */                \
    TDA18250A_LoopThrough_Disabled,                         /* single app 18274 */               \
    TDA18250A_Mode_Dual,                                    /* single app */                     \
    TDA18250A_XtalFreq_16000000,                            /* eXtalFreq */                      \
    TDA18250A_XTOUT_Amp_400mV,                              /* eXTOutAmp */                      \
    TDA18250A_XTOUT_Sinusoidal,                             /* eXTOut */                         \
    TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */                     \
    TDA18250A_IRQ_POLARITY_LOW,                             /* eIRQPolarity */                   \
    TDA18250A_HwState_InitNotDone,                          /* eHwState */                       \
    TDA18250A_CONFIG_STD_DEF_VCO_PULLING_PLUS, \
    TDA18250A_CONFIG_REGMAP_DEF

#define TDA18250A_CONFIG_SECOND_SLAVE \
	TDA18250A_CONFIG_POWER_DEF_SLAVE, \
	TDA18250A_CONFIG_RF_DEF,								/* uRF */							 \
	TDA18250A_CONFIG_PROG_RF_DEF,							/* uProgRF */						 \
	TDA18250A_CONFIG_STANDARDMODE_DEF,						/* StandardMode */					 \
	Null,													/* pStandard */ 					 \
	True,													/* bBufferMode */					 \
	False,													/* Manual PLL Calc */				 \
	TDA18250A_LoopThrough_Disabled, 						/* single app 18274 */				 \
	TDA18250A_Mode_Dual,									/* single app */					 \
	TDA18250A_XtalFreq_16000000,							/* eXtalFreq */ 					 \
	TDA18250A_XTOUT_Amp_400mV,								/* eXTOutAmp */ 					 \
	TDA18250A_XTOUT_Sinusoidal, 							/* eXTOut */						 \
	TDA18250A_LNA_RANGE_Minus8dB_Plus22dB,					/* eLNA_Range */					 \
	TDA18250A_IRQ_POLARITY_LOW, 							/* eIRQPolarity */					 \
	TDA18250A_HwState_InitNotDone,							/* eHwState */						 \
	TDA18250A_CONFIG_STD_DEF_VCO_PULLING_PLUS_2, \
	TDA18250A_CONFIG_REGMAP_DEF


    /* Custom Driver Instance Parameters: (Path 0) */
#define TDA18250A_CONFIG_0 \
    TDA18250A_CONFIG_MASTER

    /* Custom Driver Instance Parameters: (Path 1) */
#define TDA18250A_CONFIG_1 \
    TDA18250A_CONFIG_SLAVE

    /* Custom Driver Instance Parameters: (Path 2) */
#define TDA18250A_CONFIG_2 \
    TDA18250A_CONFIG_SECOND_SLAVE


#ifdef __cplusplus
}
#endif

#endif /* _TDA18250A_CONFIG_COMMON_H */

