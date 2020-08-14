/*
  Copyright (C) 2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslHCDP_Local.h
 *
 *                %version: 8 %
 *
 * \date          %modify_time%
 *
 * \author        Alexandre TANT
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                
 *
 * TVFE SW Arch V4 Template: Author Christophe CAZETTES
 *
 * \section info Change Information
 *
 * \par Changelog
 *
 * \par Version_Definition
 *  VERSION_TAG:HCDP_COMP_NUM.HCDP_MAJOR_VER.HCDP_MINOR_VER
 *
*/

#pragma once
#ifndef _TMBSL_HCDP_LOCAL_H
#define _TMBSL_HCDP_LOCAL_H

#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

/* Maximum number of systems supported by driver */
#define HCDP_MAX_UNITS                          (8)

/* Driver version definition */
#define HCDP_COMP_NUM                           (2) /* Major protocol change - Specification change required */
#define HCDP_MAJOR_VER                          (1) /* Minor protocol change - Specification change required */
#define HCDP_MINOR_VER                          (5) /* Software update - No protocol change - No specification change required */

/* Instance macros */
#define P_OBJ_VALID                             (pObj != Null)

/* I/O Functions macros */
#define P_SIO                                   pObj->sIo
#define P_SIO_READ                              P_SIO.Read
#define P_SIO_WRITE                             P_SIO.Write
#define P_SIO_READ_VALID                        (P_OBJ_VALID && (P_SIO_READ != Null))
#define P_SIO_WRITE_VALID                       (P_OBJ_VALID && (P_SIO_WRITE != Null))

/* Time Functions macros */
#define P_STIME                                 pObj->sTime
#define P_STIME_WAIT                            P_STIME.Wait
#define P_STIME_WAIT_VALID                      (P_OBJ_VALID && (P_STIME_WAIT != Null))

/* Debug Functions macros */
#define P_SDEBUG                                pObj->sDebug
#define P_DBGPRINTEx                            P_SDEBUG.Print
#define P_DBGPRINTVALID                         (P_OBJ_VALID && (P_DBGPRINTEx != Null))

/* Mutex Functions macros */
#define P_SMUTEX                                pObj->sMutex
#ifdef _TVFE_SW_ARCH_V4
 #define P_SMUTEX_OPEN                           P_SMUTEX.Open
 #define P_SMUTEX_CLOSE                          P_SMUTEX.Close
#else
 #define P_SMUTEX_OPEN                           P_SMUTEX.Init
 #define P_SMUTEX_CLOSE                          P_SMUTEX.DeInit
#endif
#define P_SMUTEX_ACQUIRE                        P_SMUTEX.Acquire
#define P_SMUTEX_RELEASE                        P_SMUTEX.Release

#define P_SMUTEX_OPEN_VALID                     (P_OBJ_VALID && (P_SMUTEX_OPEN != Null))
#define P_SMUTEX_CLOSE_VALID                    (P_OBJ_VALID && (P_SMUTEX_CLOSE != Null))
#define P_SMUTEX_ACQUIRE_VALID                  (P_OBJ_VALID && (P_SMUTEX_ACQUIRE != Null))
#define P_SMUTEX_RELEASE_VALID                  (P_OBJ_VALID && (P_SMUTEX_RELEASE != Null))

/* Driver Mutex macros */
#define HCDP_MUTEX_TIMEOUT                  (5000)
#define P_MUTEX                                 pObj->pMutex
#define P_MUTEX_VALID                           (P_MUTEX != Null)

#ifdef _TVFE_IMPLEMENT_MUTEX
 #define _MUTEX_ACQUIRE(_NAME) \
     if(err == TM_OK) \
     { \
         /* Try to acquire driver mutex */ \
         err = i##_NAME##_MutexAcquire(pObj, _NAME##_MUTEX_TIMEOUT); \
     } \
     if(err == TM_OK) \
     {

 #define _MUTEX_RELEASE(_NAME) \
         (void)i##_NAME##_MutexRelease(pObj); \
     }
#else
 #define _MUTEX_ACQUIRE(_NAME) \
     if(err == TM_OK) \
     {

 #define _MUTEX_RELEASE(_NAME) \
     }
#endif

#define HCDP_NUMBER_AGC_DOTS  (95)


/* define the normalized area of the Siganl Quality */
/* Max BER value is 1E-8 -> 80% */
#define HCDP_SIGNAL_QUALITY_NORM_MAX       80 
/* Minimum value is (10 * QuasiErrorFree BER value) */
/* this value is transposed in percentage */
/* DVB-C -> 10 * 2E-4 */
/* J83B  -> 10 * 5E-5 */
#define HCDP_SIGNAL_QUALITY_NORM_MIN_DVBC  25
#define HCDP_SIGNAL_QUALITY_NORM_MIN_J83B  32

#define BITS_PER_SYMBOL_J83B     7
#define SYMBOLS_PER_PACKET_J83B  128
#define BITS_PER_PACKET_J83B     896
#define UNCOR_BITS_J83B          16
#define UNCOR_SYMBOLS_J83B       4

#define BITS_PER_SYMBOL_DVBC     8
#define SYMBOLS_PER_PACKET_DVBC  204
#define BITS_PER_PACKET_DVBC     1632
#define UNCOR_BITS_DVBC          36
#define UNCOR_SYMBOLS_DVBC       4

#define HCDP_TAPS_NUMBER  64

typedef enum _HCDP_ResetMode_t
{
    HCDP_ResetMode_Soft         = 0x1,
    HCDP_ResetMode_Micro        = 0x2,
    HCDP_ResetMode_Full         = 0x4,
    HCDP_ResetMode_DemodSoft    = 0x8,
    HCDP_ResetMode_DemodMicro   = 0x10,
    HCDP_ResetMode_FECSoft      = 0x20,
    HCDP_ResetMode_FECMicro     = 0x40,
    HCDP_ResetMode_CsmSoft      = 0x80,
    HCDP_ResetMode_CsmMicro     = 0x100,
    HCDP_ResetMode_Eq           = 0x200
}HCDP_ResetMode_t;

typedef enum _HCDP_BERStartResult_t
{
    HCDP_BERStartResult_Success = 0, /* Started successfully */
    HCDP_BERStartResult_Busy    = 1  /* Another measurement is on going */
}HCDP_BERStartResult_t;

typedef struct _HCDP_LogTable_t
{
    UInt32 uX;
    UInt32 uLogX;  /*100*log(x)*/
} HCDP_LogTable_t;

#define HCDP_LOG_TABLE \
    /* X   ,  100*LogX */ \
    { 1    ,      0  }, \
    { 2    ,      30 }, \
    { 3    ,      47 }, \
    { 4    ,      60 }, \
    { 5    ,      69 }, \
    { 6    ,      77 }, \
    { 7    ,      84 }, \
    { 8    ,      90 }, \
    { 9    ,      95 }, \
    { 10   ,     100 }, \
    { 11   ,     104 }, \
    { 12   ,     107 }, \
    { 13   ,     111 }, \
    { 14   ,     114 }, \
    { 15   ,     117 }, \
    { 16   ,     120 }, \
    { 17   ,     123 }, \
    { 18   ,     125 }, \
    { 19   ,     127 }, \
    { 20   ,     130 }, \
    { 21   ,     132 }, \
    { 22   ,     134 }, \
    { 23   ,     136 }, \
    { 24   ,     138 }, \
    { 25   ,     139 }, \
    { 26   ,     141 }, \
    { 27   ,     143 }, \
    { 28   ,     144 }, \
    { 29   ,     146 }, \
    { 30   ,     147 }, \
    { 31   ,     149 }, \
    { 32   ,     150 }, \
    { 33   ,     151 }, \
    { 34   ,     153 }, \
    { 35   ,     154 }, \
    { 36   ,     155 }, \
    { 37   ,     156 }, \
    { 38   ,     157 }, \
    { 39   ,     159 }, \
    { 41   ,     161 }, \
    { 42   ,     162 }, \
    { 43   ,     163 }, \
    { 45   ,     165 }, \
    { 46   ,     166 }, \
    { 48   ,     168 }, \
    { 49   ,     169 }, \
    { 51   ,     170 }, \
    { 52   ,     171 }, \
    { 54   ,     173 }, \
    { 56   ,     174 }, \
    { 58   ,     176 }, \
    { 60   ,     177 }, \
    { 62   ,     179 }, \
    { 63   ,     179 }, \
    { 66   ,     181 }, \
    { 68   ,     183 }, \
    { 70   ,     184 }, \
    { 72   ,     185 }, \
    { 74   ,     186 }, \
    { 77   ,     188 }, \
    { 79   ,     189 }, \
    { 82   ,     191 }, \
    { 84   ,     192 }, \
    { 87   ,     193 }, \
    { 90   ,     195 }, \
    { 93   ,     196 }, \
    { 96   ,     198 }, \
    { 99   ,     199 }, \
    { 102  ,     200 }, \
    { 105  ,     202 }, \
    { 109  ,     203 }, \
    { 112  ,     204 }, \
    { 116  ,     206 }, \
    { 120  ,     207 }, \
    { 123  ,     208 }, \
    { 127  ,     210 }, \
    { 132  ,     212 }, \
    { 136  ,     213 }, \
    { 140  ,     214 }, \
    { 145  ,     216 }, \
    { 149  ,     217 }, \
    { 154  ,     218 }, \
    { 159  ,     220 }, \
    { 164  ,     221 }, \
    { 169  ,     222 }, \
    { 175  ,     224 }, \
    { 180  ,     225 }, \
    { 186  ,     226 }, \
    { 192  ,     228 }, \
    { 198  ,     229 }, \
    { 205  ,     231 }, \
    { 211  ,     232 }, \
    { 218  ,     233 }, \
    { 225  ,     235 }, \
    { 232  ,     236 }, \
    { 240  ,     238 }, \
    { 247  ,     239 }, \
    { 255  ,     240 }, \
    { 264  ,     242 }, \
    { 272  ,     243 }, \
    { 281  ,     244 }, \
    { 290  ,     246 }, \
    { 299  ,     247 }, \
    { 309  ,     248 }, \
    { 318  ,     250 }, \
    { 329  ,     251 }, \
    { 339  ,     253 }, \
    { 350  ,     254 }, \
    { 361  ,     255 }, \
    { 373  ,     257 }, \
    { 385  ,     258 }, \
    { 397  ,     259 }, \
    { 410  ,     261 }, \
    { 423  ,     262 }, \
    { 437  ,     264 }, \
    { 451  ,     265 }, \
    { 465  ,     266 }, \
    { 480  ,     268 }, \
    { 495  ,     269 }, \
    { 511  ,     270 }, \
    { 528  ,     272 }, \
    { 544  ,     273 }, \
    { 562  ,     274 }, \
    { 580  ,     276 }, \
    { 598  ,     277 }, \
    { 618  ,     279 }, \
    { 637  ,     280 }, \
    { 658  ,     281 }, \
    { 679  ,     283 }, \
    { 701  ,     284 }, \
    { 723  ,     285 }, \
    { 746  ,     287 }, \
    { 770  ,     288 }, \
    { 795  ,     290 }, \
    { 820  ,     291 }, \
    { 846  ,     292 }, \
    { 874  ,     294 }, \
    { 901  ,     295 }, \
    { 930  ,     296 }, \
    { 960  ,     298 }, \
    { 991  ,     299 }, \
    { 1023 ,     300 }, \
    { 1055 ,     302 }, \
    { 1089 ,     303 }, \
    { 1124 ,     305 }, \
    { 1160 ,     306 }, \
    { 1197 ,     307 }, \
    { 1235 ,     309 }, \
    { 1275 ,     310 }, \
    { 1316 ,     311 }, \
    { 1358 ,     313 }, \
    { 1401 ,     314 }, \
    { 1446 ,     316 }, \
    { 1493 ,     317 }, \
    { 1540 ,     318 }, \
    { 1590 ,     320 }, \
    { 1641 ,     321 }, \
    { 1693 ,     322 }, \
    { 1747 ,     324 }, \
    { 1803 ,     325 }, \
    { 1861 ,     326 }, \
    { 1920 ,     328 }, \
    { 1982 ,     329 }, \
    { 2045 ,     331 }, \
    { 2111 ,     332 }, \
    { 2178 ,     333 }, \
    { 2248 ,     335 }, \
    { 2320 ,     336 }, \
    { 2394 ,     337 }, \
    { 2471 ,     339 }, \
    { 2550 ,     340 }, \
    { 2632 ,     342 }, \
    { 2716 ,     343 }, \
    { 2803 ,     344 }, \
    { 2892 ,     346 }, \
    { 2985 ,     347 }, \
    { 3081 ,     348 }, \
    { 3179 ,     350 }, \
    { 3281 ,     351 }, \
    { 3386 ,     352 }, \
    { 3494 ,     354 }, \
    { 3606 ,     355 }, \
    { 3722 ,     357 }, \
    { 3841 ,     358 }, \
    { 3964 ,     359 }, \
    { 4090 ,     361 }, \
    { 4221 ,     362 }, \
    { 4356 ,     363 }, \
    { 4496 ,     365 }, \
    { 4640 ,     366 }, \
    { 4788 ,     368 }, \
    { 4941 ,     369 }, \
    { 5100 ,     370 }, \
    { 5263 ,     372 }, \
    { 5431 ,     373 }, \
    { 5605 ,     374 }, \
    { 5784 ,     376 }, \
    { 5970 ,     377 }, \
    { 6161 ,     378 }, \
    { 6358 ,     380 }, \
    { 6561 ,     381 }, \
    { 6771 ,     383 }, \
    { 6988 ,     384 }, \
    { 7212 ,     385 }, \
    { 7442 ,     387 }, \
    { 7680 ,     388 }, \
    { 7926 ,     389 }, \
    { 8180 ,     391 }, \
    { 8442 ,     392 }, \
    { 8712 ,     394 }, \
    { 8991 ,     395 }, \
    { 9278 ,     396 }, \
    { 9575 ,     398 }, \
    { 9882 ,     399 }, \
    { 10198,     400 }, \
    { 10524,     402 }, \
    { 10861,     403 }, \
    { 11209,     404 }, \
    { 11567,     406 }, \
    { 11938,     407 }, \
    { 12320,     409 }, \
    { 12714,     410 }, \
    { 13121,     411 }, \
    { 13541,     413 }, \
    { 13974,     414 }, \
    { 14421,     415 }, \
    { 14883,     417 }, \
    { 15359,     418 }, \
    { 15850,     420 }, \
    { 16357,     421 }, \
    { 16881,     422 }, \
    { 17421,     424 }, \
    { 17979,     425 }, \
    { 18554,     426 }, \
    { 19148,     428 }, \
    { 19760,     429 }, \
    { 20393,     430 }, \
    { 21045,     432 }, \
    { 21719,     433 }, \
    { 22414,     435 }, \
    { 23131,     436 }, \
    { 23871,     437 }, \
    { 24635,     439 }, \
    { 25424,     440 }, \
    { 26237,     441 }, \
    { 27077,     443 }, \
    { 27943,     444 }, \
    { 28837,     445 }, \
    { 29760,     447 }, \
    { 30713,     448 }, \
    { 31695,     450 }, \
    { 32710,     451 }, \
    { 33756,     452 }, \
    { 34837,     454 }, \
    { 35951,     455 }, \
    { 37102,     456 }, \
    { 38289,     458 }, \
    { 39514,     459 }, \
    { 40779,     461 }, \
    { 42084,     462 }, \
    { 43431,     463 }, \
    { 44820,     465 }, \
    { 46255,     466 }, \
    { 47735,     467 }, \
    { 49262,     469 }, \
    { 50839,     470 }, \
    { 52466,     471 }, \
    { 54145,     473 }, \
    { 55877,     474 }, \
    { 57665,     476 }, \
    { 59511,     477 }, \
    { 61415,     478 }, \
    { 63380,     480 }, \
    { 65408,     481 }, \
    { 67501,     482 }, \
    { 69662,     484 }, \
    { 71891,     485 }, \
    { 74191,     487 }, \
    { 76565,     488 }, \
    { 79015,     489 }, \
    { 81544,     491 }, \
    { 84153,     492 }, \
    { 86846,     493 }, \
    { 89625,     495 }, \
    { 92493,     496 }, \
    { 95453,     497 }, \
    { 98508,     499 }

typedef struct _HCDP_Channel_t
{
    UInt32                  uIF;
    UInt32                  uSR;
    tmFrontEndFECMode_t     eFECMode;
    tmFrontEndModulation_t  eMod;
    tmFrontEndSpecInv_t     eSI;
    HCDP_ExtendSymbolRateMode_t  eExtendedSRModeCurrent;
    HCDP_ExtendSymbolRateMode_t  eExtendedSRModeNew;
} HCDP_Channel_t;

typedef struct _HCDP_Config_t
{
    /* Clocks */
    UInt32                  uSamplingClock;
    UInt32                  uDClock;
    UInt32                  uXtalMultiplier;
    
    /* AGC */
    UInt16                  uIFAGCThreshold;
    UInt16                  uRFAGCThreshold;

    /* Equalizer */
    UInt16                  uEqFfLeakageAlpha_BeforeLock;
    UInt16                  uEqFfLeakageAlpha_AfterLock;
}HCDP_Config_t;

typedef enum _HCDP_PNBERWindow
{
    HCDP_PNBERWindow_1E5 = 0,   /*1e5 bits*/
    HCDP_PNBERWindow_1E6,       /*1e6 bits*/
    HCDP_PNBERWindow_1E7,       /*1e7 bits*/
    HCDP_PNBERWindow_1E8,       /*1e8 bits*/
    HCDP_PNBERWindow_1E9,       /*1e9 bits*/
    HCDP_PNBERWindow_1E10,      /*1e10 bits*/
    HCDP_PNBERWindow_1E11,      /*1e11 bits*/
    HCDP_PNBERWindow_1E12,      /*1e12 bits*/
    HCDP_PNBERWindow_1E13,      /*1e13 bits*/
    HCDP_PNBERWindow_Invalid    /*1e5 bits*/
} HCDP_PNBERWindow_t;

typedef struct _HCDPObject_t 
{
    tmUnitSelect_t                      tUnit;
    tmUnitSelect_t                      tUnitW;
    HCDP_HwCdpSample_t                  eHwCdpSample;
    ptmbslFrontEndMutexHandle           pMutex;
    Bool                                init;
    Bool                                bReset;
    tmbslFrontEndIoFunc_t               sIo;
    tmbslFrontEndTimeFunc_t             sTime;
    tmbslFrontEndDebugFunc_t            sDebug;
    tmbslFrontEndMutexFunc_t            sMutex;

    /* Device specific part: */ 
    HCDP_Config_t                       sConfig;
    HCDP_Channel_t                      sCurrentChannel;

    /* Measurements */
    UInt32                              uStoredBitErrs;
    UInt32                              uStoredUncorrectable;
    UInt32                              uStoredCorrected;
    UInt32                              uStoredNonerrors;
    tmFrontEndFracNb32_t                sPreviousBER;
    UInt32                              uBaseUncors;
    UInt32                              uPreviousUncors;
    HCDP_BERWindow_t                    eBERWindow;

    /* Internal functions variables */
    UInt16                              usBerWindowCount;
    UInt16                              usPBWindow;
    HCDP_PNBERWindow_t                  eWindow;

    UInt32                              uPNReadyOld;
    UInt32                              usConDScale;
    UInt32                              usConDMask;
    tmFrontEndFracNb32_t                sI[64];
    tmFrontEndFracNb32_t                sQ[64];
    Bool                                bSetAfterLock;
    UInt32                              Marker;
} HCDPObject_t, *pHCDPObject_t, **ppHCDPObject_t;

/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

extern tmErrorCode_t
iHCDP_RegWrite(pHCDPObject_t pObj, HCDPRegisterIndex_t eRegIndex, UInt16 uValue);

extern tmErrorCode_t
iHCDP_RegRead(pHCDPObject_t pObj, HCDPRegisterIndex_t eRegIndex, UInt16 *puValue);

extern tmErrorCode_t
iHCDP_SetRFAGCThreshold(pHCDPObject_t pObj, UInt16 uRFAGCThrLow, UInt16 uRFAGCThrHigh);

extern tmErrorCode_t
iHCDP_Wait(pHCDPObject_t pObj, UInt32 uTime);

#ifdef _TVFE_IMPLEMENT_MUTEX
extern tmErrorCode_t iHCDP_MutexAcquire(pHCDPObject_t pObj, UInt32 timeOut);
extern tmErrorCode_t iHCDP_MutexRelease(pHCDPObject_t pObj);
#endif

#ifdef __cplusplus
}
#endif  /*__cplusplus*/

#endif /*_TMBSL_HCDP_LOCAL_H*/
