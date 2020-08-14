/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18204_InstanceCustom.h
 *
 *                %version: 1 %
 *
 * \date          %modify_time%
 *
 * \author        Bruno LEMESLE
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18204_Driver_User_Guide.pdf
 *
 * \section info Change Information
 *
 * \par Changelog
 *  -1.0.0.26 Change way to calculate AGC2 NB contribution in RSSI, now using 2 polynoms
 *  -1.0.0.25 Tighten IF filter for OOB standard (8% offset)
 *  -1.0.0.24 Add support for OOB standard (HP enabled, IF=4.75MHz)
 *  -1.0.0.23 Update coefficients for RSSI NB ES2 equation
 *  -1.0.0.22 Update coefficients for RSSI NB ES2 equation
 *  -1.0.0.21 Set bAgc2Active flag in pObj for multiple instance purpose potential bug
 *     Change implementation of AGC2 NB TOP programmation with a state machine + addition of a medium level of AGC2 NB TOP
 *     PR#549 Set AGC3_NB_Do_step = 2.044ms, AGC3_NB_UP_STEP= 66ms, AGCK_mode=8.192ms
 *  -1.0.0.20 Set DC_Notch_IF Enabled
 *  -1.0.0.19 Set AGC2 NB TOP Down/Up to 100/99 if 244 MHz < RF < 600 MHz else set AGC2 NB TOP Down/Up to 103/102
 *  -1.0.0.18 Enable AGC2 external config
 *  -1.0.0.17 Update new coefficients of NB reference max gain for RSSI, after latest release
 *  -1.0.0.16 Update new coefficients of NB reference max gain for RSSI, after latest release
 *  -1.0.0.15 Change type of agc2WBCtrl to support internal or external AGC2 loop
 *  -1.0.0.14 Set AGC2 TOP DN from 100 to 102
 *     Set AGC2 TOP UP from 98 to 101
 *  -1.0.0.13 Add configuration of AGC2 depending on RF frequency
 *  -1.0.0.12 Improve power level calculation by changing equation order of AGC2 NB coefficients for ES2
 *  -1.0.0.11 Remove AGCK_Time_Constant as configurable parameter, in order not to be reset when doing a tune
 *  -1.0.0.10 PR#338 : Chande decoding of XTout
 *  -1.0.0.9 Update coefficients for RSSI WB ES1 equation
 *  -1.0.0.8 by default start TDA18204 in TDA18204_PowerSciNbNormalMode
 *  -1.0.0.7 Take into account new registers in ES2
 *     Symbol AGC2_NB_Gain_Control_En and AGC2_NB_Gain_Control_Speed renamed in AGC2_Gain_Control_En and AGC2_Gain_Control_Speed in ES2
 *  -1.0.0.6 Set AGC1_TOP default value to 103-97
 *     Set AGC1_TOP_STRATEGY default value to top constant hysteresis 4dB
 *     RSSI, take into account that coefficients are different between ES1 and ES2
 *     RSSI, update ES2 coefficients for WB and NB paths
 *  -1.0.0.5 Force LNA_Zin to 1 in any mode
 *  -1.0.0.4 PR#459 : FVCO max drift versus temperature
 *     PR#460 : TOP AGC2_NB versus CSO/CTB
 *  -1.0.0.3 PR#456 : rework W_Filter decoding
 *  -1.0.0.2 PR#453 : Uncor on WB when switching NB AGC2 high-pass
 *  -1.0.0.1 Rework AGC2_WB contribution in RSSI
 *  -1.0.0.0 Set AGC2_NB_Gain_Control_En to O at POR
 *  -0.0.0.20 Error in Power Level for WB (AGC2 coefficients)
 *  -0.0.0.19 Rework RSSI and Power Level for WB and NB path (AGC2 contribution)
 *  -0.0.0.18 PR#444 : Adjust AGC3_TOP and IF_LEVEL
 *  -0.0.0.17 Implement RSSI and Power Level for NB path (back to previous AGC3 contribution)
 *  -0.0.0.17 PR#444 : Adjust TEQ input parameters
 *  -0.0.0.16 tmbslTDA18204 : Implement RSSI and Power Level for WB path (AGC2 contribution)
 *  -0.0.0.15 Implement RSSI and Power Level for NB path (AGC3 contribution)
 *  -0.0.0.14 Rssi NB, remove iTDA18204_LvlRFFilterContribution() and add iTDA18204_GetRefMaxGainNB()
 *  -0.0.0.13 Implement RSSI and Power Level for NB path
 *  -0.0.0.12 Implement RSSI and Power Level for NB path
 *  -0.0.0.11 Programmation of RF filters for NB path
 *  -0.0.0.10 AGC2_TOP_DOWN (WB) 96.3 -> 97.3
 *     AGC2_TOP_UP (WB) 95.8 -> 96.8
 *     Set AGC2 WB gain change speed to 6.86
 *  -0.0.0.9 Make AGC2 WB gain change speed configurable
 *     Implementation of NB RSSI
 *  -0.0.0.8 Rework AGC1 contribution in RSSI
 *  -0.0.0.7 Set IF_LPF_offset to 4%, to reduce tilt (PR424)
 *  -0.0.0.6 Change AGC1 contribution equation in RSSI calculation
 *     Set AGC1 smooth time constant to 8;
 *     Set AGC2_NB_TOP_DO to 96 dBuVms and AGC2_NB_TOP_UP to 94 dBuVms
 *  -0.0.0.5 Change TEQ contribution equation in RSSI calculation
 *     Error in LT_Gain_Read values decoding
 *  -0.0.0.5 Change TEQ contribution equation in RSSI calculation
 *     Error in LT_Gain_Read values decoding
 *  -0.0.0.4 AGC2_TOP_DOWN (WB) 99.8 -> 96.3
 *     AGC2_TOP_UP (WB) 99.3 -> 95.8
 *     Force PSM mode to 1 at initialization
 *  -0.0.0.4 AGC2_TOP_DOWN (WB) 99.8 -> 96.3
 *     AGC2_TOP_UP (WB) 99.3 -> 95.8
 *     Force PSM mode to 1 at initialization
 *  -0.0.0.3 - TDA18204 must be in mode TDA18204_PowerSciNormalMode by default
 *     - Rework TDA18204 RSSI equation
 *  -0.0.0.3 - TDA18204 must be in mode TDA18204_PowerSciNormalMode by default
 *     - Rework TDA18204 RSSI equation
 *  -0.0.0.2 Set IF Output Level to 0.8v
 *  -0.0.0.2 Set IF Output Level to 0.8v
 *  -0.0.0.1 initial config for system OM3968B
 *
 * \par Version_Definition
 *  VERSION_TAG:TDA18204_SETTINGS_CUSTOMER_NUM.TDA18204_SETTINGS_PROJECT_NUM.TDA18204_SETTINGS_MAJOR_VER.TDA18204_SETTINGS_MINOR_VER
*/

#pragma once
#ifndef _TDA18204_INSTANCE_CUSTOM_H
#define _TDA18204_INSTANCE_CUSTOM_H


#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/
/* Driver settings version definition */
#define TDA18204_SETTINGS_CUSTOMER_NUM 1                     /* SW Settings Customer Number */
#define TDA18204_SETTINGS_PROJECT_NUM  0                     /* SW Settings Project Number  */
#define TDA18204_SETTINGS_MAJOR_VER    0                     /* SW Settings Major Version   */
#define TDA18204_SETTINGS_MINOR_VER    26                     /* SW Settings Minor Version   */

/*======================================================================================================*/
/* Custom Driver: Get Power Level                                                                       */
/*======================================================================================================*/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_MAX 12

/* {     X     ,    X    ,  X  ,   X      ,          A           },                */
/* {     0     ,    1    ,  2  ,   3      ,          G           },                */
/* {           ,         ,     ,          ,          C           },                */
/* {     x     ,    x    ,  x  ,   x      ,	         1           },                */
/* {     1     ,    1    ,  1  ,   1      ,                      },                */
/* {     0     ,    0    ,  0  ,   0      ,                      },                */
/* {     0     ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/***********************************************************************************/
/**********                      ES1, ES1MF1                              **********/
/***********************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_ES1    				            \
{                                                                                   \
   {-2983,        1282,   -340,   192,    TDA18204_AGC1_Gain_MINUS11_VALUE_0   },   \
   {-2716,        1600,   -606,   414,    TDA18204_AGC1_Gain_MINUS8_VALUE_1    },   \
   {-2394,         281,   -634,   520,    TDA18204_AGC1_Gain_MINUS5_VALUE_2    },   \
   {-1980,         739,   -592,   449,    TDA18204_AGC1_Gain_MINUS2_VALUE_3    },   \
   {-1672,         608,   -668,   609,    TDA18204_AGC1_Gain_1_VALUE_4         },   \
   {-1482,         404,   -642,   522,    TDA18204_AGC1_Gain_4_VALUE_5         },   \
   {-1210,        1122,   -336,   192,    TDA18204_AGC1_Gain_7_VALUE_6         },   \
   { -914,        1264,   -355,   220,    TDA18204_AGC1_Gain_10_VALUE_7        },   \
   { -624,        1087,   -294,   204,    TDA18204_AGC1_Gain_13_VALUE_8        },   \
   { -317,         994,   -286,   182,    TDA18204_AGC1_Gain_16_VALUE_9        },   \
   {    0,         -19,     11,    -7,    TDA18204_AGC1_Gain_19_VALUE_10       },   \
   {    0,           0,      0,     0,    TDA18204_AGC1_Gain_22_VALUE_11       }    \
}

/***********************************************************************************/
/**********                          ES2                                  **********/
/***********************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1_ES2						        \
{                                                                                   \
   {-3405,         2226,   -548,   302,    TDA18204_AGC1_Gain_MINUS11_VALUE_0   }, \
   {-3117,         1516,   -597,   390,    TDA18204_AGC1_Gain_MINUS8_VALUE_1    }, \
   {-2789,           93,   -612,   503,    TDA18204_AGC1_Gain_MINUS5_VALUE_2    }, \
   {-2379,         1024,   -651,   472,    TDA18204_AGC1_Gain_MINUS2_VALUE_3    }, \
   {-2056,          899,   -601,   553,    TDA18204_AGC1_Gain_1_VALUE_4         }, \
   {-1871,          413,   -659,   523,    TDA18204_AGC1_Gain_4_VALUE_5         }, \
   {-1561,          221,   -580,   477,    TDA18204_AGC1_Gain_7_VALUE_6         }, \
   {-1266,           22,   -525,   469,    TDA18204_AGC1_Gain_10_VALUE_7        }, \
   { -954,          -36,   -519,   462,    TDA18204_AGC1_Gain_13_VALUE_8        }, \
   { -700,         1365,   -394,   249,    TDA18204_AGC1_Gain_16_VALUE_9        }, \
   { -377,          461,   -144,   116,    TDA18204_AGC1_Gain_19_VALUE_10       }, \
   {    0,            0,      0,     0,    TDA18204_AGC1_Gain_22_VALUE_11       }  \
}

#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2_MAX 13
/* {     X     ,    X    ,  X  ,   X      ,          A           },    */
/* {     0     ,    1    ,  2  ,   3      ,          G           },    */
/* {           ,         ,     ,          ,          C           },    */
/* {     x     ,    x    ,  x  ,   x      ,	         2           },    */
/* {     1     ,    1    ,  1  ,   1      ,                      },    */
/* {     0     ,    0    ,  0  ,   0      ,                      },    */
/* {     0     ,    0    ,  0  ,   0      ,                      },    */
/* {           ,    0    ,  0  ,   0      ,                      },    */
/* {           ,    0    ,  0  ,   0      ,                      },    */
/* {           ,    0    ,  0  ,   0      ,                      },    */
/* {           ,         ,  0  ,   0      ,                      },    */
/* {           ,         ,  0  ,   0      ,                      },    */
/* {           ,         ,  0  ,   0      ,                      },    */
/* {           ,         ,     ,   0      ,                      },    */
/* {           ,         ,     ,   0      ,                      },    */
/* {           ,         ,     ,   0      ,                      },    */
/***********************************************************************/
/**********                 ES1, ES1MF1                       **********/
/***********************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2WB_ES1  \
{                                                       \
   {    -2337,     -145,    316,  -13,     10   },      \
   {    -2130,     -186,    385,  -60,     21   },      \
   {    -1938,     -113,    214,   36,     32   },      \
   {    -1737,     -121,    210,   41,     43   },      \
   {    -1536,     -116,    197,   43,     54   },      \
   {    -1340,      -76,    104,   95,     65   },      \
   {    -1147,      -57,     78,   98,     76   },      \
   {     -947,      -80,    126,   58,     87   },      \
   {     -748,     -119,    222,  -14,     98   },      \
   {     -562,      -47,     59,   69,    109   },      \
   {     -374,      -31,     49,   45,    120   },      \
   {     -188,        7,    -34,   66,    131   },      \
   {        0,        0,      0,    0,    142   }       \
}

/***********************************************************************/
/**********                   ES2                             **********/
/***********************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2WB_ES2  \
{                                                       \
   {    -2357,     -134,    261,     3,     10   },     \
   {    -2155,     -129,    241,     9,     21   },     \
   {    -1962,     -103,    168,    49,     32   },     \
   {    -1758,     -127,    210,    23,     43   },     \
   {    -1563,      -76,     92,    90,     54   },     \
   {    -1360,     -102,    150,    52,     65   },     \
   {    -1164,      -99,    163,    29,     76   },     \
   {     -969,      -87,    146,    30,     87   },     \
   {     -771,     -107,    191,    -9,     98   },     \
   {     -576,      -94,    174,   -19,    109   },     \
   {     -373,      -87,    190,   -61,    120   },     \
   {     -175,     -159,    396,  -227,    131   },     \
   {        0,        0,      0,     0,    142   }      \
}

#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_MAX 7

/* {     X     ,    X    ,  X  ,   X      ,          T           },                */
/* {     0     ,    1    ,  2  ,   3      ,          E           },                */
/* {           ,         ,     ,          ,          Q           },                */
/* {     x     ,    x    ,  x  ,   x      ,	                     },                */
/* {     1     ,    1    ,  1  ,   1      ,                      },                */
/* {     0     ,    0    ,  0  ,   0      ,                      },                */
/* {     0     ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,    0    ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,  0  ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/* {           ,         ,     ,   0      ,                      },                */
/***********************************************************************************/
/**********                      ES1, ES1MF1                              **********/
/***********************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_ES1                                 \
{                                                                                   \
   {2120,         191,    3980,  -2697,    TDA18204_TEQ_SELECT_15_VALUE_0      },   \
   {2564,         552,    2143,  -1713,    TDA18204_TEQ_SELECT_10_VALUE_1      },   \
   {3031,         911,    -183,   -187,    TDA18204_TEQ_SELECT_5_VALUE_2       },   \
   {3256,         627,    -240,    -71,    TDA18204_TEQ_SELECT_3_VALUE_3       },   \
   {3743,        -457,    1146,   -834,    TDA18204_TEQ_SELECT_0_DEF_VALUE_4   },   \
   {3734,        -327,     121,   -315,    TDA18204_TEQ_SELECT_MINUS5_VALUE_5  },   \
   {3639,         576,   -3136,   1721,    TDA18204_TEQ_SELECT_MINUS10_VALUE_6 }    \
}

/***********************************************************************************/
/**********                           ES2                                 **********/
/***********************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_TEQ_ES2                                 \
{                                                                                   \
   {2536,         -103,    5097,   -3477,    TDA18204_TEQ_SELECT_15_VALUE_0      }, \
   {2981,          367,    2958,   -2311,    TDA18204_TEQ_SELECT_10_VALUE_1      }, \
   {3427,          976,     -27,    -344,    TDA18204_TEQ_SELECT_5_VALUE_2       }, \
   {3690,          472,     332,    -463,    TDA18204_TEQ_SELECT_3_VALUE_3       }, \
   {4165,         -573,    1627,   -1165,    TDA18204_TEQ_SELECT_0_DEF_VALUE_4   }, \
   {4139,         -286,     360,    -600,    TDA18204_TEQ_SELECT_MINUS5_VALUE_5  }, \
   {4000,         1076,   -4150,    2286,    TDA18204_TEQ_SELECT_MINUS10_VALUE_6 }  \
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
/*************************************************************************/
/**********                  ES1, ES1MF1                        **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_ES1					         \
{                                                                                \
   {-294239610,        2936,    25,      TDA18204_AGC1_Gain_MINUS11_VALUE_0   }, \
   {-266656723,       -6416,    86,      TDA18204_AGC1_Gain_MINUS8_VALUE_1    }, \
   {-234389773,      -25593,   220,      TDA18204_AGC1_Gain_MINUS5_VALUE_2    }, \
   {-193772377,      -16106,   147,      TDA18204_AGC1_Gain_MINUS2_VALUE_3    }, \
   {-162016310,      -39965,   318,      TDA18204_AGC1_Gain_1_VALUE_4         }, \
   {-143042707,      -25756,   214,      TDA18204_AGC1_Gain_4_VALUE_5         }, \
   {-112933207,      -25448,   220,      TDA18204_AGC1_Gain_7_VALUE_6         }, \
   { -84213911,      -25668,   242,      TDA18204_AGC1_Gain_10_VALUE_7        }, \
   { -53800802,      -25873,   216,      TDA18204_AGC1_Gain_13_VALUE_8        }, \
   { -29021107,         442,    -1,      TDA18204_AGC1_Gain_16_VALUE_9        }, \
   {         0,           0,     0,      TDA18204_AGC1_Gain_19_VALUE_10       }, \
   {         0,           0,     0,      TDA18204_AGC1_Gain_22_VALUE_11       }  \
}

/*************************************************************************/
/**********                     ES2                             **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC1NB_ES2					         \
{                                                                                \
   {-326257977,	    -18896,	    195,     TDA18204_AGC1_Gain_MINUS11_VALUE_0   }, \
   {-298926480,	    -25895,	    238,     TDA18204_AGC1_Gain_MINUS8_VALUE_1    }, \
   {-267589787,	    -40476,	    342,     TDA18204_AGC1_Gain_MINUS5_VALUE_2    }, \
   {-226644817,	    -32739,	    284,     TDA18204_AGC1_Gain_MINUS2_VALUE_3    }, \
   {-195358723,	    -52614,	    425,     TDA18204_AGC1_Gain_1_VALUE_4         }, \
   {-176020533,	    -39825,	    332,     TDA18204_AGC1_Gain_4_VALUE_5         }, \
   {-145391430,	    -39725,	    348,     TDA18204_AGC1_Gain_7_VALUE_6         }, \
   {-116412327,	    -40164,	    386,     TDA18204_AGC1_Gain_10_VALUE_7        }, \
   { -85521731,	    -41584,	    392,     TDA18204_AGC1_Gain_13_VALUE_8        }, \
   { -60637948,	    -15852,	    184,     TDA18204_AGC1_Gain_16_VALUE_9        }, \
   { -32631494,	     -9603,	    144,     TDA18204_AGC1_Gain_19_VALUE_10       }, \
   {         0,          0,       0,     TDA18204_AGC1_Gain_22_VALUE_11       }  \
}

/*************************************************************************/
/**********                  ES1, ES1MF1                        **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB_ES1  \
{                                                    \
   {  -265751,    2103,    94,       10    },        \
   {  -267898,    2094,    85,       21    },        \
   {  -269586,    2056,    81,       32    },        \
   {  -272219,    2058,    78,       43    },        \
   {  -275537,    2075,    76,       54    },        \
   {  -276588,    2051,    74,       65    },        \
   {  -279104,    2053,    71,       76    },        \
   {  -280439,    2037,    65,       87    },        \
   {  -281504,    2024,    60,       98    },        \
   {  -282908,    2016,    52,       109   },        \
   {  -283364,    2008,    39,       120   },        \
   {  -280080,    1976,    23,       131   },        \
   {  -276237,    1945,     0,       142   }         \
}

/*************************************************************************/
/**********                     ES2                             **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC2NB_ES2                        \
{                                                                             \
   { 2070726,	-256447459,	-47907,	97,	-190910150,	-153656,	81,  10   },  \
   { 2072239,	-258963250,	-45454,	92,	-187339807,	-168239,	89,  21   },  \
   { 2038086,	-260911310,	-43982,	90,	-191598076,	-162750,	86,  32   },  \
   { 2051314,	-263994633,	-42843,	87,	-192939715,	-167769,	89,  43   },  \
   { 2045275,	-266127670,	-41590,	84,	-195202355,	-166691,	89,  54   },  \
   { 2052755,	-269043901,	-39606,	80,	-197883881,	-165791,	89,  65   },  \
   { 2042134,	-271174604,	-38110,	77,	-200898927,	-162514,	88,  76   },  \
   { 2035172,	-273558125,	-35358,	71,	-218149436,	-124049,	66,  87   },  \
   { 2029093,	-275685106,	-33305,	65,	-231978770,	 -96374,	51,  98   },  \
   { 2037086,	-279610994,	-28767,	55,	-243403344,	 -78522,	41, 109   },  \
   { 2016548,	-279880844,	-21417,	41,	-252279945,	 -58032,	30, 120   },  \
   { 1964639,	-275922805,	-11884,	22,	-254523569,	 -40881,	21, 131   },  \
   { 1899120,	-269107840,	 -4784,  6,	-268750902,	  -5134,     4, 142   }   \
}

#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3_MAX 8

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
/*************************************************************************/
/**********                  ES1, ES1MF1                        **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB_ES1  				        \
{                                                                               \
   {-208243333,   218,     -6,    TDA18204_AGC3_NB_GAIN_READ_MINUS6_VALUE_0  }, \
   {-177762807,   252,     -4,    TDA18204_AGC3_NB_GAIN_READ_MINUS3_VALUE_1  }, \
   {-148958103,   -47,     -2,    TDA18204_AGC3_NB_GAIN_READ_0_VALUE_2       }, \
   {-118259877,   201,     -3,    TDA18204_AGC3_NB_GAIN_READ_3_VALUE_3       }, \
   { -88502333,    -6,     -1,    TDA18204_AGC3_NB_GAIN_READ_6_VALUE_4       }, \
   { -58357336,   -38,      0,    TDA18204_AGC3_NB_GAIN_READ_9_VALUE_5       }, \
   { -29251335,   -33,      0,    TDA18204_AGC3_NB_GAIN_READ_12_VALUE_6      }, \
   {         0,     0,      0,    TDA18204_AGC3_NB_GAIN_READ_15_VALUE_7      }  \
}

/*************************************************************************/
/**********                     ES2                             **********/
/*************************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_AGC3NB_ES2					        \
{                                                                               \
   {-207546837,	-1558,	  12,    TDA18204_AGC3_NB_GAIN_READ_MINUS6_VALUE_0  }, \
   {-177461130,	-1459,	  12,    TDA18204_AGC3_NB_GAIN_READ_MINUS3_VALUE_1  }, \
   {-148394723,	-1474,	  12,    TDA18204_AGC3_NB_GAIN_READ_0_VALUE_2       }, \
   {-117932910,	-1286,	  10,    TDA18204_AGC3_NB_GAIN_READ_3_VALUE_3       }, \
   { -88105934,	-1385,	  11,    TDA18204_AGC3_NB_GAIN_READ_6_VALUE_4       }, \
   { -58033867,	-1174,	   9,    TDA18204_AGC3_NB_GAIN_READ_9_VALUE_5       }, \
   { -28958061,  -666,	   5,    TDA18204_AGC3_NB_GAIN_READ_12_VALUE_6      }, \
   {         0,     0,     0,    TDA18204_AGC3_NB_GAIN_READ_15_VALUE_7      }  \
}

#define TDA18204_RF_MAX         1200000000 

#define TDA18204_BP_FILTER_1      39424000 
#define TDA18204_BP_FILTER_2      61952000
#define TDA18204_BP_FILTER_3      86528000
#define TDA18204_BP_FILTER_4     123392000
#define TDA18204_BP_FILTER_5     172544000
#define TDA18204_BP_FILTER_6     244224000
#define TDA18204_BP_FILTER_7     320000000

#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX 19
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
/******************************************************************/
/**********               ES1, ES1MF1                    **********/
/******************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB_ES1    \
{                                                              \
   { 36365207,   345691,  -28228,     TDA18204_BP_FILTER_1  }, \
   { 36365207,   345691,  -28228,     TDA18204_BP_FILTER_2  }, \
   { 37414420,   245971,  -14388,     TDA18204_BP_FILTER_3  }, \
   { 38863417,   164196,   -7208,     TDA18204_BP_FILTER_4  }, \
   { 40121954,   112291,   -3882,     TDA18204_BP_FILTER_5  }, \
   { 43510312,    42214,   -1056,     TDA18204_BP_FILTER_6  }, \
   { 48527164,     1270,     -24,     TDA18204_BP_FILTER_7  }, \
   { 53529948,   -45170,     661,     335360000             }, \
   { 43917902,    10525,    -212,     390656000             }, \
   { 68502796,  -110850,    1283,     430592000             }, \
   { 39782421,    28612,    -360,     473600000             }, \
   { 54584463,   -37187,     333,     535040000             }, \
   { 30460199,    56912,    -552,     562688000             }, \
   { 70833546,   -85332,     667,     600000000             }, \
   { 17213787,    86609,    -690,     654848000             }, \
   { 89405232,  -131507,     960,     720384000             }, \
   { 21785777,    64300,    -425,     793088000             }, \
   { 77102565,   -72718,     404,     865792000             }, \
   { 40723586,    12676,     -79,     TDA18204_RF_MAX}         \
}

/******************************************************************/
/**********                     ES2                      **********/
/******************************************************************/
#define TDA18204_INSTANCE_CUSTOM_COMMON_LVL_REF_GAIN_MAX_NB_ES2    \
{                                                              \
   { 36946101,	323729,	-26891,    TDA18204_BP_FILTER_1  }, \
   { 36946101,	323729,	-26891,    TDA18204_BP_FILTER_2  }, \
   { 38126238,	224189,	-13523,    TDA18204_BP_FILTER_3  }, \
   { 41056400,	112599,	 -4723,    TDA18204_BP_FILTER_4  }, \
   { 37068050,	102381,	 -3152,    TDA18204_BP_FILTER_5  }, \
   { 44405715,	   464,	    82,    TDA18204_BP_FILTER_6  }, \
   { 46351012,	 -1435,	    41,    TDA18204_BP_FILTER_7  }, \
   { 27636079,	118118,	 -1871,    335360000             }, \
   { 45678885,	  4859,	  -156,    390656000             }, \
   { 52936057,	-31011,	   289,    430592000             }, \
   { 37218677,	 42613,	  -536,    473600000             }, \
   { 46533800,	 -1627,	   -50,    535040000             }, \
   { 18765507,	102242,	  -994,    562688000             }, \
   { 71678092,	-87906,	   682,    600000000             }, \
   { 38991012,	 11427,	  -113,    654848000             }, \
   { 41444676,	  3113,	   -33,    720384000             }, \
   { 32974408,	 28650,	  -198,    793088000             }, \
   { 46043331,	 -1199,	   -51,    865792000             }, \
   { 27069967,	 38025,	  -229,    TDA18204_RF_MAX}         \
}

/*======================================================================================================*/
/*======================================================================================================*/
/* Custom Driver Instance Parameters: AGC2WB_TOP                                                        */
/*======================================================================================================*/
/*======================================================================================================*/
#define TDA18204_INSTANCE_CUSTOM_COMMON_AGC2WB_TOP                                                         \
    {   /* agc2WBGud[] */                                                                                  \
        0x0E,                                               /* AGC2_Gud_Up    */                         \
        0x0D                                                /* AGC2_Gud_Down  */                         \
    }

/* #define TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP_NB_DEFAULT 1 */

#define TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP_NB 2

#define TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP    \
{                                              \
   { 126500000,	0x12,  0x13}, \
   { 0,         0x0E,  0x0D}  \
}

/* Standard Preset Definitions: */
#define TDA18204_INSTANCE_CUSTOM_STD_QAM_6MHZ \
	{                                                                   /* QAM 6MHz */      \
	/****************************************************************/						\
	/* IF Settings                                                  */						\
	/****************************************************************/						\
	4000000,														/* IF Frequency */		\
	/****************************************************************/						\
	/* IF SELECTIVITY Settings                                      */						\
	/****************************************************************/						\
	TDA18204_LP_Fc_7_VALUE_1,										/* LPF Cut off */		\
	TDA18204_LPFOffset_plus_4pc,									/* LPF offset */		\
	TDA18204_DC_Notch_IF_PPF_Enabled,								/* DC notch IF PPF */	\
	TDA18204_IF_HP_Fc_0_5_VALUE_0,									/* Hi Pass */			\
	TDA18204_HPFOffset_plus_12pc,									/* HPF offset */		\
    TDA18204_HPFEnable_Disabled,                                    /* HPF disabled */      \
	/****************************************************************/						\
	/* AGC TOP Settings                                             */						\
	/****************************************************************/						\
	103,															/* AGC2 TOP DN High */  \
	102,														    /* AGC2 TOP UP High */	\
	102,															/* AGC2 TOP DN Medium */\
	101,														    /* AGC2 TOP UP Medium */\
    100,															/* AGC2 TOP DN Low */   \
	99,																/* AGC2 TOP UP Low */   \
    TDA18204_AGC_DET_SPEED_62_5KHz,									/* AGC2 DET SPEED */	\
	TDA18204_DET12_Cint_200_VALUE_3,								/* DET12 CINT */		\
	108,															/* AGC3 TOP DN */		\
	103,															/* AGC3 TOP UP */		\
	TDA18204_IF_level_0_5_VALUE_7,									/* IF Output Level */	\
	TDA18204_S2D_Gain_6_VALUE_1,									/* S2D gain */			\
	/****************************************************************/						\
	/* GSK Settings                                                 */						\
	/****************************************************************/						\
	TDA18204_RSSI_HP_Fc_0_3_VALUE_0,								/* RSSI HP FC */		\
	/****************************************************************/						\
	/* RSSI Settings                                                */						\
	/****************************************************************/						\
	TDA18204_RSSI_CAP_VAL_3pF,										/* RSSI Cap Val */		\
	TDA18204_RSSI_CK_SPEED_31_25kHz,								/* RSSI Ck Speed */		\
}

#define TDA18204_INSTANCE_CUSTOM_STD_QAM_8MHZ \
	{																		/* QAM 8MHz */	\
	/****************************************************************/						\
	/* IF Settings                                                  */						\
	/****************************************************************/						\
	5000000,														/* IF Frequency */		\
	/****************************************************************/						\
	/* IF SELECTIVITY Settings                                      */						\
	/****************************************************************/						\
	TDA18204_LP_Fc_9_VALUE_3,										/* LPF Cut off */		\
	TDA18204_LPFOffset_plus_4pc,									/* LPF offset */		\
	TDA18204_DC_Notch_IF_PPF_Enabled,								/* DC notch IF PPF */	\
	TDA18204_IF_HP_Fc_0_5_VALUE_0,									/* Hi Pass */			\
	TDA18204_HPFOffset_plus_12pc,									/* HPF offset */		\
    TDA18204_HPFEnable_Disabled,                                    /* HPF disabled */      \
	/****************************************************************/						\
	/* AGC TOP Settings                                             */						\
	/****************************************************************/						\
	103,															/* AGC2 TOP DN High */  \
	102,														    /* AGC2 TOP UP High */	\
	102,															/* AGC2 TOP DN Medium */\
	101,														    /* AGC2 TOP UP Medium */\
    100,															/* AGC2 TOP DN Low */   \
	99,																/* AGC2 TOP UP Low */   \
	TDA18204_AGC_DET_SPEED_62_5KHz,									/* AGC2 DET SPEED */	\
	TDA18204_DET12_Cint_200_VALUE_3,								/* DET12 CINT */		\
	108,															/* AGC3 TOP DN */		\
	103,															/* AGC3 TOP UP */		\
	TDA18204_IF_level_0_5_VALUE_7,									/* IF Output Level */	\
	TDA18204_S2D_Gain_6_VALUE_1,									/* S2D gain */			\
	/****************************************************************/						\
	/* GSK Settings                                                 */						\
	/****************************************************************/						\
	TDA18204_RSSI_HP_Fc_0_3_VALUE_0,								/* RSSI HP FC */		\
	/****************************************************************/						\
	/* RSSI Settings                                                */						\
	/****************************************************************/						\
	TDA18204_RSSI_CAP_VAL_3pF,										/* RSSI Cap Val */		\
	TDA18204_RSSI_CK_SPEED_31_25kHz,								/* RSSI Ck Speed */		\
	}

#define TDA18204_INSTANCE_CUSTOM_STD_OOB_2MHZ \
    {                                                                   /* OOB 2MHz */      \
    /****************************************************************/                      \
    /* IF Settings                                                  */                      \
    /****************************************************************/                      \
    4750000,                                                         /* IF Frequency */     \
    /****************************************************************/                      \
    /* IF SELECTIVITY Settings                                      */                      \
    /****************************************************************/                      \
    TDA18204_LP_Fc_6_VALUE_0,                                       /* LPF Cut off */       \
    TDA18204_LPFOffset_min_8pc,                                     /* LPF offset */        \
    TDA18204_DC_Notch_IF_PPF_Enabled,                               /* DC notch IF PPF */   \
    TDA18204_IF_HP_Fc_3_5_DEF_VALUE_4,                              /* Hi Pass */           \
    TDA18204_HPFOffset_plus_8pc,                                    /* HPF offset */        \
    TDA18204_HPFEnable_Enabled,                                     /* HPF enabled */       \
    /****************************************************************/                      \
    /* AGC TOP Settings                                             */                      \
    /****************************************************************/                      \
    103,                                                            /* AGC2 TOP DN High */  \
    102,                                                            /* AGC2 TOP UP High */  \
    102,                                                            /* AGC2 TOP DN Medium */\
    101,                                                            /* AGC2 TOP UP Medium */\
    100,                                                            /* AGC2 TOP DN Low */   \
    99,                                                             /* AGC2 TOP UP Low */   \
    TDA18204_AGC_DET_SPEED_62_5KHz,                                 /* AGC2 DET SPEED */    \
    TDA18204_DET12_Cint_200_VALUE_3,                                /* DET12 CINT */        \
    108,                                                            /* AGC3 TOP DN */       \
    103,                                                            /* AGC3 TOP UP */       \
    TDA18204_IF_level_0_5_VALUE_7,                                  /* IF Output Level */   \
    TDA18204_S2D_Gain_6_VALUE_1,                                    /* S2D gain */          \
    /****************************************************************/                      \
    /* GSK Settings                                                 */                      \
    /****************************************************************/                      \
    TDA18204_RSSI_HP_Fc_0_3_VALUE_0,                                /* RSSI HP FC */        \
    /****************************************************************/                      \
    /* RSSI Settings                                                */                      \
    /****************************************************************/                      \
    TDA18204_RSSI_CAP_VAL_3pF,                                      /* RSSI Cap Val */      \
    TDA18204_RSSI_CK_SPEED_31_25kHz,                                /* RSSI Ck Speed */     \
}

#define TDA18204_INSTANCE_CUSTOM_AGC1_TIME_CST_DEF TDA18204_AGC1_smooth_t_cst_8_VALUE_4
#define TDA18204_INSTANCE_CUSTOM_LT_TIME_CST_DEF TDA18204_LT_smooth_t_cst_8_VALUE_4

#define TDA18204_INSTANCE_CUSTOM_AGC1_GAIN_DEF TDA18204_AGC1_Gain_1_VALUE_4
#define TDA18204_INSTANCE_CUSTOM_AGC2WB_CTRL_DEF TDA18204_AGC2_WB_Ctrl_External
#define TDA18204_INSTANCE_CUSTOM_AGC2_UP_STEP_DEF TDA18204_AGC2_UP_STEP_6_86_VALUE_5
#define TDA18204_INSTANCE_CUSTOM_AGC2_DOWN_STEP_DEF TDA18204_AGC2_DOWN_STEP_3_42_VALUE_0
#define TDA18204_INSTANCE_CUSTOM_AGC2_ACTIVE_DEF False
#define TDA18204_INSTANCE_CUSTOM_AGC2_NB_TOP_CURRENT_DEF TDA18204_AGC2NBTOP_LevelInvalid

/* Default configuration */
#define TDA18204_INSTANCE_CUSTOM_CURPOWERSTATE_DEF tmPowerMax
#define TDA18204_INSTANCE_CUSTOM_CURLLPOWERSTATE_DEF TDA18204_PowerMax
#define TDA18204_INSTANCE_CUSTOM_POWERDOWNSMOOTHDELAY_DEF 1

#define TDA18204_INSTANCE_CUSTOM_IDENTITY_DEF 18204
#define TDA18204_INSTANCE_CUSTOM_REVISION_DEF 0x10
#define TDA18204_INSTANCE_CUSTOM_RF_DEF 0
#define TDA18204_INSTANCE_CUSTOM_VCOMAX_DEF 4834000
#define TDA18204_INSTANCE_CUSTOM_TEMPERATURE_DEF 22
#define TDA18204_INSTANCE_CUSTOM_STANDARD_DEF TDA18204_StandardMode_Max
#define TDA18204_INSTANCE_CUSTOM_TEQSELECT_DEF TDA18204_TEQSelect0dB
#define TDA18204_INSTANCE_CUSTOM_TEQDELAY_DEF 0
#define TDA18204_INSTANCE_CUSTOM_XTIN_DEF TDA18204_XTAL_IN_CTRL_BUFFER_IN_27MHZ_VALUE_5
#define TDA18204_INSTANCE_CUSTOM_XTOUT_DEF TDA18204_XTout_SYNC_VALUE_5
#define TDA18204_INSTANCE_CUSTOM_XTOUTAMP_DEF TDA18204_Xtout_Amp_200_VALUE_0

#define TDA18204_INSTANCE_CUSTOM_POWERSAVINGMODE_DEF TDA18204PowerSavingMode_Low
#define TDA18204_INSTANCE_CUSTOM_PDSMOOTHDELAYMODE_DEF 0

/* Power settings */
#define TDA18204_INSTANCE_CUSTOM_POWER_DEF \
    TDA18204_INSTANCE_CUSTOM_CURPOWERSTATE_DEF,                     /* curPowerState */ \
    TDA18204_INSTANCE_CUSTOM_CURLLPOWERSTATE_DEF,                   /* curLLPowerState */ \
    {                                                               /* mapLLPowerState */ \
        TDA18204_PowerSciNbNormalMode,                              /* tmPowerOn (D0) */ \
        TDA18204_PowerBufMtoLtoStandby,                             /* tmPowerStandby (D1) */ \
        TDA18204_PowerBufMtoLtoStandby,                             /* tmPowerSuspend (D2) */ \
        TDA18204_PowerBufMtoLtoStandby                              /* tmPowerOff (D3) */ \
    }, \
    TDA18204_INSTANCE_CUSTOM_POWERSAVINGMODE_DEF,					/* PowerSavingMode */ \
    TDA18204_INSTANCE_CUSTOM_PDSMOOTHDELAYMODE_DEF					/* PowerDownSmoothDelay */

/* Standard Presets Aggregation: */
#define TDA18204_INSTANCE_CUSTOM_STD_DEF \
    { \
        TDA18204_INSTANCE_CUSTOM_STD_QAM_6MHZ,                      /* Mode Tuner, Digital TV QAM 6MHz */ \
        TDA18204_INSTANCE_CUSTOM_STD_QAM_8MHZ,                      /* Mode Tuner, Digital TV QAM 8MHz */ \
        TDA18204_INSTANCE_CUSTOM_STD_OOB_2MHZ                       /* Mode Tuner, Digital TV OOB 2MHz */ \
    }

#define TDA18204_INSTANCE_CUSTOM_REGMAP_DEF \
    { \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
    }

#define TDA18204_INSTANCE_CUSTOM_MARKER 0xABCD

#define TDA18204_INSTANCE_CUSTOM_MASTER_0 \
    TDA18204_INSTANCE_CUSTOM_POWER_DEF,									/* Power settings */ \
	TDA18204_INSTANCE_CUSTOM_IDENTITY_DEF,								/* Identity */ \
	TDA18204_INSTANCE_CUSTOM_REVISION_DEF,								/* Revision */ \
	TDA18204_INSTANCE_CUSTOM_RF_DEF,									/* uRF */ \
	TDA18204_INSTANCE_CUSTOM_VCOMAX_DEF,								/* VCOmax */ \
    TDA18204_INSTANCE_CUSTOM_TEMPERATURE_DEF,							/* Temperature at VCO calibration */ \
    TDA18204_INSTANCE_CUSTOM_STANDARD_DEF,								/* StandardMode */ \
    Null,																/* pStandard */ \
    TDA18204_INSTANCE_CUSTOM_TEQSELECT_DEF,								/* Tilt */ \
	TDA18204_INSTANCE_CUSTOM_TEQDELAY_DEF,								/* TiltDelay */ \
	TDA18204_INSTANCE_CUSTOM_XTIN_DEF,									/* XTin */ \
	TDA18204_INSTANCE_CUSTOM_XTOUT_DEF,									/* XTOut */ \
	TDA18204_INSTANCE_CUSTOM_XTOUTAMP_DEF,								/* XTOutAmp */ \
	TDA18204_HwState_InitNotDone,										/* eHwState */ \
    TDA18204_INSTANCE_CUSTOM_STD_DEF, \
	TDA18204_AGC1_Top_MAX_103_97_VALUE_7,						        /* AGC1 TOP I2C DN/UP */	\
	TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_4DB_VALUE_0,	        /* AGC1 TOP STRATEGY */	\
	TDA18204_AGC_DET_SPEED_62_5KHz,										/* AGC1 DET SPEED */	\
	TDA18204_INSTANCE_CUSTOM_AGC1_TIME_CST_DEF,							/* agc1TimeCst */	\
	TDA18204_INSTANCE_CUSTOM_LT_TIME_CST_DEF,							/* ltTimeCst */	\
    TDA18204_INSTANCE_CUSTOM_COMMON_AGC2WB_TOP,							/* agc2 top */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP,                                /* agc2WBTop */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_CTRL_DEF,                           /* agc2WBCtrl */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_UP_STEP_DEF,	                        /* agc2UpStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_DOWN_STEP_DEF,                        /* agc2DownStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_ACTIVE_DEF,                           /* bAgc2Active */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_NB_TOP_CURRENT_DEF,                       /* eAGC2NBTopCurrent */ \
	TDA18204_INSTANCE_CUSTOM_REGMAP_DEF,								/* Reg map */    \
    TDA18204_INSTANCE_CUSTOM_MARKER


//    TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP,                                /* agc2WBTop */ 

#define TDA18204_INSTANCE_CUSTOM_MASTER_1 \
    TDA18204_INSTANCE_CUSTOM_POWER_DEF,									/* Power settings */ \
	TDA18204_INSTANCE_CUSTOM_IDENTITY_DEF,								/* Identity */ \
	TDA18204_INSTANCE_CUSTOM_REVISION_DEF,								/* Revision */ \
	TDA18204_INSTANCE_CUSTOM_RF_DEF,									/* uRF */ \
	TDA18204_INSTANCE_CUSTOM_VCOMAX_DEF,								/* VCOmax */ \
    TDA18204_INSTANCE_CUSTOM_TEMPERATURE_DEF,							/* Temperature at VCO calibration */ \
    TDA18204_INSTANCE_CUSTOM_STANDARD_DEF,								/* StandardMode */ \
    Null,																/* pStandard */ \
    TDA18204_INSTANCE_CUSTOM_TEQSELECT_DEF,								/* Tilt */ \
	TDA18204_INSTANCE_CUSTOM_TEQDELAY_DEF,								/* TiltDelay */ \
	TDA18204_INSTANCE_CUSTOM_XTIN_DEF,									/* XTin */ \
	TDA18204_INSTANCE_CUSTOM_XTOUT_DEF,									/* XTOut */ \
	TDA18204_INSTANCE_CUSTOM_XTOUTAMP_DEF,								/* XTOutAmp */ \
	TDA18204_HwState_InitNotDone,										/* eHwState */ \
    TDA18204_INSTANCE_CUSTOM_STD_DEF, \
	TDA18204_AGC1_Top_MAX_103_97_VALUE_7,						        /* AGC1 TOP I2C DN/UP */	\
	TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_4DB_VALUE_0,	        /* AGC1 TOP STRATEGY */	\
	TDA18204_AGC_DET_SPEED_62_5KHz,										/* AGC1 DET SPEED */	\
	TDA18204_INSTANCE_CUSTOM_AGC1_TIME_CST_DEF,							/* agc1TimeCst */	\
	TDA18204_INSTANCE_CUSTOM_LT_TIME_CST_DEF,							/* ltTimeCst */	\
    TDA18204_INSTANCE_CUSTOM_COMMON_AGC2WB_TOP,							/* agc2 top */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP,                                /* agc2WBTop */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_CTRL_DEF,                           /* agc2WBCtrl */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_UP_STEP_DEF,	                        /* agc2UpStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_DOWN_STEP_DEF,                        /* agc2DownStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_ACTIVE_DEF,                           /* bAgc2Active */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_NB_TOP_CURRENT_DEF,                       /* eAGC2NBTopCurrent */ \
    TDA18204_INSTANCE_CUSTOM_REGMAP_DEF,								/* Reg map */    \
    TDA18204_INSTANCE_CUSTOM_MARKER

#define TDA18204_INSTANCE_CUSTOM_MASTER_2 \
    TDA18204_INSTANCE_CUSTOM_POWER_DEF,									/* Power settings */ \
	TDA18204_INSTANCE_CUSTOM_IDENTITY_DEF,								/* Identity */ \
	TDA18204_INSTANCE_CUSTOM_REVISION_DEF,								/* Revision */ \
	TDA18204_INSTANCE_CUSTOM_RF_DEF,									/* uRF */ \
	TDA18204_INSTANCE_CUSTOM_VCOMAX_DEF,								/* VCOmax */ \
    TDA18204_INSTANCE_CUSTOM_TEMPERATURE_DEF,							/* Temperature at VCO calibration */ \
    TDA18204_INSTANCE_CUSTOM_STANDARD_DEF,								/* StandardMode */ \
    Null,																/* pStandard */ \
    TDA18204_INSTANCE_CUSTOM_TEQSELECT_DEF,								/* Tilt */ \
	TDA18204_INSTANCE_CUSTOM_TEQDELAY_DEF,								/* TiltDelay */ \
	TDA18204_INSTANCE_CUSTOM_XTIN_DEF,									/* XTin */ \
	TDA18204_INSTANCE_CUSTOM_XTOUT_DEF,									/* XTOut */ \
	TDA18204_INSTANCE_CUSTOM_XTOUTAMP_DEF,								/* XTOutAmp */ \
	TDA18204_HwState_InitNotDone,										/* eHwState */ \
    TDA18204_INSTANCE_CUSTOM_STD_DEF, \
	TDA18204_AGC1_Top_MAX_103_97_VALUE_7,						        /* AGC1 TOP I2C DN/UP */	\
	TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_4DB_VALUE_0,	        /* AGC1 TOP STRATEGY */	\
	TDA18204_AGC_DET_SPEED_62_5KHz,										/* AGC1 DET SPEED */	\
	TDA18204_INSTANCE_CUSTOM_AGC1_TIME_CST_DEF,							/* agc1TimeCst */	\
	TDA18204_INSTANCE_CUSTOM_LT_TIME_CST_DEF,							/* ltTimeCst */	\
    TDA18204_INSTANCE_CUSTOM_COMMON_AGC2WB_TOP,							/* agc2 top */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP,                                /* agc2WBTop */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_CTRL_DEF,                           /* agc2WBCtrl */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_UP_STEP_DEF,	                        /* agc2UpStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_DOWN_STEP_DEF,                        /* agc2DownStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_ACTIVE_DEF,                           /* bAgc2Active */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_NB_TOP_CURRENT_DEF,                       /* eAGC2NBTopCurrent */ \
    TDA18204_INSTANCE_CUSTOM_REGMAP_DEF,								/* Reg map */    \
    TDA18204_INSTANCE_CUSTOM_MARKER

#define TDA18204_INSTANCE_CUSTOM_MASTER_3 \
    TDA18204_INSTANCE_CUSTOM_POWER_DEF,									/* Power settings */ \
	TDA18204_INSTANCE_CUSTOM_IDENTITY_DEF,								/* Identity */ \
	TDA18204_INSTANCE_CUSTOM_REVISION_DEF,								/* Revision */ \
	TDA18204_INSTANCE_CUSTOM_RF_DEF,									/* uRF */ \
	TDA18204_INSTANCE_CUSTOM_VCOMAX_DEF,								/* VCOmax */ \
    TDA18204_INSTANCE_CUSTOM_TEMPERATURE_DEF,							/* Temperature at VCO calibration */ \
    TDA18204_INSTANCE_CUSTOM_STANDARD_DEF,								/* StandardMode */ \
    Null,																/* pStandard */ \
    TDA18204_INSTANCE_CUSTOM_TEQSELECT_DEF,								/* Tilt */ \
	TDA18204_INSTANCE_CUSTOM_TEQDELAY_DEF,								/* TiltDelay */ \
	TDA18204_INSTANCE_CUSTOM_XTIN_DEF,									/* XTin */ \
	TDA18204_INSTANCE_CUSTOM_XTOUT_DEF,									/* XTOut */ \
	TDA18204_INSTANCE_CUSTOM_XTOUTAMP_DEF,								/* XTOutAmp */ \
	TDA18204_HwState_InitNotDone,										/* eHwState */ \
    TDA18204_INSTANCE_CUSTOM_STD_DEF, \
	TDA18204_AGC1_Top_MAX_103_97_VALUE_7,						        /* AGC1 TOP I2C DN/UP */	\
	TDA18204_AGC1_Top_Strategy_TOP_CONSTANT_HYST_4DB_VALUE_0,	        /* AGC1 TOP STRATEGY */	\
	TDA18204_AGC_DET_SPEED_62_5KHz,										/* AGC1 DET SPEED */	\
	TDA18204_INSTANCE_CUSTOM_AGC1_TIME_CST_DEF,							/* agc1TimeCst */	\
	TDA18204_INSTANCE_CUSTOM_LT_TIME_CST_DEF,							/* ltTimeCst */	\
    TDA18204_INSTANCE_CUSTOM_COMMON_AGC2WB_TOP,							/* agc2 top */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_TOP,                                /* agc2WBTop */ \
    TDA18204_INSTANCE_CUSTOM_AGC2WB_CTRL_DEF,                           /* agc2WBCtrl */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_UP_STEP_DEF,	                        /* agc2UpStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_DOWN_STEP_DEF,                        /* agc2DownStep */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_ACTIVE_DEF,                           /* bAgc2Active */ \
    TDA18204_INSTANCE_CUSTOM_AGC2_NB_TOP_CURRENT_DEF,                       /* eAGC2NBTopCurrent */ \
    TDA18204_INSTANCE_CUSTOM_REGMAP_DEF,								/* Reg map */    \
    TDA18204_INSTANCE_CUSTOM_MARKER

/* Custom Driver Instance Parameters: (Path 0) */
#define TDA18204_INSTANCE_CUSTOM_0 \
    TDA18204_INSTANCE_CUSTOM_MASTER_0

/* Custom Driver Instance Parameters: (Path 1) */
#define TDA18204_INSTANCE_CUSTOM_1 \
    TDA18204_INSTANCE_CUSTOM_MASTER_1

/* Custom Driver Instance Parameters: (Path 2) */
#define TDA18204_INSTANCE_CUSTOM_2 \
    TDA18204_INSTANCE_CUSTOM_MASTER_2

/* Custom Driver Instance Parameters: (Path 3) */
#define TDA18204_INSTANCE_CUSTOM_3 \
    TDA18204_INSTANCE_CUSTOM_MASTER_3


#ifdef __cplusplus
}
#endif

#endif /* _TDA18204_INSTANCE_CUSTOM_H */

