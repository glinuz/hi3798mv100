/*
  Copyright (C) 2006-2009 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslTDA18250local.h
 *
 *                3
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *                TDA18250_Driver_User_Guide.pdf
 *
 * Detailed description may be added here.
 *
 * \section info Change Information
 *
*/

#ifndef _TMBSL_TDA18250_LOCAL_H 
#define _TMBSL_TDA18250_LOCAL_H


#ifdef __cplusplus
extern "C"
{
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

#define TMBSL_TDA18250_COMPONENT_NAME_STR   "TDA18250"

#define TDA18250_MUTEX_TIMEOUT              (5000)

#define POBJ_SRVFUNC_SIO                    pObj->sRWFunc
#define POBJ_SRVFUNC_STIME                  pObj->sTime

#define P_DBGPRINTVALID                     ((pObj != Null) && (pObj->sDebug.Print != Null))
#define P_DBGPRINTEx                        pObj->sDebug.Print

#define TMBSL_TDA18250_IR_FREQ1 123000000
#define TMBSL_TDA18250_IR_FREQ2 380000000
#define TMBSL_TDA18250_IR_FREQ3 780000000

#define TMBSL_TDA18250_DEFAULT_LO 896250000

#define tmTDA18250_IND_RFAGC_Top_Max 6

#define TDA18250_SIGNAL_CALIB_WAIT    5   /* ms */
#define TDA18250_SIGNAL_CALIB_TIMEOUT 2   /* nb of occurences */
#define TDA18250_IMAGE_CALIB_WAIT     5   /* ms */
#define TDA18250_IMAGE_CALIB_TIMEOUT  8   /* nb of occurences */

/* Switch transition */
typedef enum _tmTDA18250_SwitchSmooth_t
{
    tmTDA18250_SW_SMOOTH_OFF = 0,
    tmTDA18250_SW_SMOOTH_ON = 1,
    tmTDA18250_SW_SMOOTH_Max = 2
} tmTDA18250_SwitchSmooth_t, *ptmTDA18250_SwitchSmooth_t;


/* AGC1 type for Power Level contribution */
typedef enum _tmTDA18250_AGC1_Type_t
{
    tmTDA18250_AGC1_MasterMulti = 0,
    tmTDA18250_AGC1_MasterSingle = 1,
    tmTDA18250_AGC1_Slave = 2,
    tmTDA18250_AGC1_Max = 3
} tmTDA18250_AGC1_Type_t, *ptmTDA18250_AGC1_Type_t;


/* TDA18250 Blocks Path configuration regarding channel Analog/Digital path */
typedef struct _tmTDA18250BlocksPath_t
{
    tmddTDA18250_PD_t  pdAGC2a;
    tmddTDA18250_PD_t  pdAGC2b;
    tmddTDA18250_PD_t  PD_RFAGC_Det;
    tmddTDA18250_PD_t  pdAGC2_DETECT2;
    tmddTDA18250_PD_t  pdAGC2_DETECT3;
    tmddTDA18250_PD_t  pdAGC2_DETECT4;
    tmddTDA18250_PD_t  PD_RFAGC_Ifout;
    UInt8              BUF1b_DCloopIH;
    tmddTDA18250_PD_t  PDBUF1b;
} tmTDA18250BlocksPath_t, *ptmTDA18250BlocksPath_t;


typedef struct _tmTDA18250RFAGC_Top_t
{
    UInt32  RfMax;
    UInt8   uRFAGC_Top;
} tmTDA18250RFAGC_Top_t, *ptmTDA18250RFAGC_Top_t, **pptmTDA18250RFAGC_Top_t;


/* TDA18250 specific device type: */
typedef enum _tmTDA18250DeviceType_t {
    tmTDA18250_DeviceTypeMasterMulti = 0,                   /* TDA18250 Master with slave(s) Device */
    tmTDA18250_DeviceTypeMasterMultiWoXTout = 1,            /* TDA18250 Master with slave(s) Device Without external clock */
    tmTDA18250_DeviceTypeMasterSingleWithLTWithXTout = 2,   /* TDA18250 Master Single Device With Loop through and external clock */
    tmTDA18250_DeviceTypeMasterSingleWithLTWoXTout = 3,     /* TDA18250 Master Single Device Without Loop through, with external clock */
    tmTDA18250_DeviceTypeMasterSingleWoLTWithXTout = 4,     /* TDA18250 Master Single Device Without Loop through, with external clock */
    tmTDA18250_DeviceTypeMasterSingleWoLTWoXTout = 5,       /* TDA18250 Master Single Device Without Loop through, without external clock */
    tmTDA18250_DeviceTypeSlave = 6,                         /* Slave Device */
    tmTDA18250_DeviceTypeSlaveWithXTout = 7,                /* Slave Device With external clock */
    tmTDA18250_DeviceTypeSlaveWithLTWoXTout = 8,            /* Slave Device With Loop through, without external clock */
    tmTDA18250_DeviceTypeMax = 9
} tmTDA18250DeviceType_t, *ptmTDA18250DeviceType_t;

/* TDA18250 Step */
typedef enum _tmTDA18250_Step_t
{
    tmTDA18250_INIT = 0,
    tmTDA18250_RESTORE = 1,
    tmTDA18250_Step_Max = 2
} tmTDA18250_Step_t, *ptmTDA18250_Step_t;


typedef enum _tmTDA18250_AGC2Step3dB
{
    tmTDA18250_AGC2Step3dB_OFF = 0,                               
    tmTDA18250_AGC2Step3dB_ON = 1,              
    tmTDA18250_AGC2Step3dB_Max = 2                 
} tmTDA18250_AGC2Step3dB, *ptmTDA18250_AGC2Step3dB;


typedef enum _tmTDA18250LP_Fc_t
{
    tmTDA18250_LPF_6MHz = 0,    /* 6MHz LPFc */
    tmTDA18250_LPF_7MHz = 1,    /* 7MHz LPFc */
    tmTDA18250_LPF_8MHz = 2,    /* 8MHz LPFc */
    tmTDA18250_LPF_9MHz = 3,    /* 9MHz LPFc */
    tmTDA18250_LPF_Max = 4
} tmTDA18250LP_Fc_t, *ptmTDA18250LP_Fc_t;

/* TDA18250 specific Standard parameters: */
typedef struct _tmTDA18250StdParameters_t
{
    tmTDA18250ChannelType_t         ChannelType;
    UInt32                          IF;
    tmddTDA18250_SW_RFcal_modeOFF_t RFcal_modeOFF;
    tmTDA18250LP_Fc_t               LP_Fc;
    tmddTDA18250_PD_t               PD_hpf;
    UInt8                           IF_hpf;
    UInt8                           notch_select;
    UInt8                           RC_LPFC_Offset;
    UInt8                           RC_LPFC_Sign;
} tmTDA18250StdParameters_t, *ptmTDA18250StdParameters_t;

typedef union _uBandx3F_t
{            
    UInt8 DTA_Filter;                 /* address 0x3F */
    struct
    {
        UInt8 Unused                         :1;
        UInt8 BP_Ind1                        :3;
        UInt8 Cap_HP_Left                    :2;
        UInt8 Cap_HP_Right                   :2;
    }bF;
}uBandx3F_t;

typedef union            
{            
    UInt8 AGC1bis;                    /* address 0x40 */
    struct
    {
        UInt8 Agc1bis_gain                   :3;
        UInt8 pd_AGC1bis                     :1;
        UInt8 Unused_0x40                    :4;
    }bF;
}uBandx40;


typedef union _uBandx44_t
{            
    UInt8 RFSW_NOTCH;                 /* address 0x44 */
    struct
    {
        UInt8 Cap_HP_Mid                     :2;
        UInt8 Cap_L10n                       :2;
        UInt8 Cap_L6n                        :2;
        UInt8 BP_Ind2                        :2;
    }bF;
}uBandx44_t;

typedef struct _tmTDA18250BandP_t
{
    UInt32                  RfMax;          /* LoMax-IF (Hz) */
    tmTDA18250ChannelType_t ChannelType;
    tmTDA18250Switch_t      Switch;
    tmddTDA18250_State_t    SwitchState;
    UInt8                   Ind_Div2;
    
    UInt8                   BP_Ind1;
    UInt8                   Cap_HP_Left;
    UInt8                   Cap_HP_Right;
    
    UInt8                   Cap_HP_Mid;
    UInt8                   Cap_L10n;
    UInt8                   Cap_L6n;
    UInt8                   BP_Ind2;
    
    UInt8                   DTA_notMOCA;
    tmddTDA18250_PD_t       pd_AGC1bis;
    UInt8                   AGC1bis_gain;
    UInt8                   BP_Filter;
} tmTDA18250BandP_t, *ptmTDA18250BandP_t;


typedef struct _tmTDA18250BandParameters_t
{
    tmTDA18250Filter_t        Filter;
    UInt32                    RfMax;          /* LoMax-IF (Hz) */
    tmTDA18250ChannelType_t   ChannelType;
    tmTDA18250Switch_t        Switch;
    tmddTDA18250SwitchState_t SwitchState;
    UInt8                     Ind_Div2;
    UInt8                     Cap_HP_Left;
    UInt8                     Cap_HP_Mid;
    UInt8                     Cap_HP_Right;
    UInt8                     BP_Ind1;
    UInt8                     BP_Ind2;
    UInt8                     Cap_L6n;
    UInt8                     Cap_L10n;
    UInt8                     DTA_notMOCA;
    tmddTDA18250_PD_t         pd_AGC1bis;
    UInt8                     AGC1bis_gain;
    UInt8                     BP_Filter;
    UInt8                     Stob_Gain;
    UInt8                     Stob_DC;
    UInt8                     PSM_Stob;
} tmTDA18250BandParameters_t, *ptmTDA18250BandParameters_t;


/* Power Level */
typedef enum _tmTDA18250Lvl_RSSI_t
{
    tmTDA18250_RSSI_X0 = 0,
    tmTDA18250_RSSI_X1 = 1,
    tmTDA18250_RSSI_Max = 2
} tmTDA18250Lvl_RSSI_t, *ptmTDA18250Lvl_RSSI_t;


typedef enum _tmTDA18250Lvl_RfFilter_t
{
    tmTDA18250_RfFilter_X0 = 0,
    tmTDA18250_RfFilter_X1 = 1,
    tmTDA18250_RfFilter_X2 = 2,
    tmTDA18250_RfFilter_RfMax = 3,
    tmTDA18250_RfFilter_X_Max = 4
} tmTDA18250Lvl_RfFilter_t, *ptmTDA18250Lvl_RfFilter_t;

typedef enum _tmTDA18250Lvl_AGC1_t
{
    tmTDA18250_AGC1_X0 = 0,
    tmTDA18250_AGC1_X1 = 1,
    tmTDA18250_AGC1_X2 = 2,
    tmTDA18250_AGC1_Code = 3,
    tmTDA18250_AGC1_X_Max = 4
} tmTDA18250Lvl_AGC1_t, *ptmTDA18250Lvl_AGC1_t;


typedef enum _tmTDA18250Lvl_AGC2b_t
{
    tmTDA18250_AGC2b_X0 = 0,
    tmTDA18250_AGC2b_X1 = 1,
    tmTDA18250_AGC2b_X2 = 2,
    tmTDA18250_AGC2b_Code = 3,
    tmTDA18250_AGC2b_X_Max = 4
} tmTDA18250Lvl_AGC2b_t, *ptmTDA18250Lvl_AGC2b_t;


typedef enum _tmTDA18250Lvl_Offset_t
{
    tmTDA18250_Offset_X0 = 0,
    tmTDA18250_Offset_X1 = 1,
    tmTDA18250_Offset_X_Max = 2
} tmTDA18250Lvl_Offset_t, *ptmTDA18250Lvl_Offset_t;

typedef enum _tmTDA18250Lvl_Xdiv10_t
{
    tmTDA18250_XDIV10_X0 = 0,
    tmTDA18250_XDIV10_X1 = 1,
    tmTDA18250_XDIV10_X2 = 2,
    tmTDA18250_XDIV10_SUM = 3,
    tmTDA18250_XDIV10_X_Max = 4
} tmTDA18250Lvl_Xdiv10_t, *ptmTDA18250Lvl_Xdiv10_t;


typedef enum _tmTDA18250Lvl_IndexDiv10_t
{
    tmTDA18250_DIV10_RF = 0,
    tmTDA18250_XDIV10_AGC2 = 1,
    tmTDA18250_XDIV10_AGC1 = 2,
    tmTDA18250_XDIV10_AGC1Multi = 3,
    tmTDA18250_XDIV10_OffsetSlave = 4,
    tmTDA18250_XDIV10_Index_Max = 5
} tmTDA18250Lvl_IndexDiv10_t, *ptmTDA18250Lvl_IndexDiv10_t;


/* Group Delay Signe */
typedef enum _tmTDA18250_GdSigne_t
{
    tmTDA18250_GD_SIGNE_PLUS = 0,
    tmTDA18250_GD_SIGNE_MINUS = 1,
    tmTDA18250_GD_SIGNE_Max = 2
} tmTDA18250_GdSigne_t, *ptmTDA18250_GdSigne_t;


typedef enum _tmTDA18250_RCcal_t
{
    tmTDA18250_GD_RCcal = 0,
    tmTDA18250_GD_Signe = 1,
    tmTDA18250_GD_Offset = 2,
    tmTDA18250_GD_Max = 3
} tmTDA18250_RCcal_t, *ptmTDA18250_RCcal_t;


typedef struct _tmTDA18250Object_t
{
    tmUnitSelect_t                tUnit;
    tmUnitSelect_t                tUnitW;
    ptmbslFrontEndMutexHandle     pMutex;
    Bool                          init;
    tmbslFrontEndIoFunc_t         sRWFunc;
    tmbslFrontEndTimeFunc_t       sTime;
    tmbslFrontEndDebugFunc_t      sDebug;
    tmbslFrontEndMutexFunc_t      sMutex;
    UInt32                        uRF;
    UInt32                        uFreqCut;
    Bool                          ResetCalledPreviously;
    tmTDA18250PowerTransitionMode_t ePowerTransitionMode;
    /* Device specific part: */
    tmTDA18250DeviceType_t        deviceType;
    tmTDA18250PowerState_t        curPowerState;
    tmTDA18250StandardMode_t      StandardMode;
    Bool                          clock_mode_auto;
    tmddTDA18250_AGC1_Gain_t      AGC1Mode;
    tmddTDA18250_TimeCst_t        Switch_TimeCst;
    tmddTDA18250_TimeCst_t        AGC1_TimeCst;
    tmddTDA18250_TimeCst_t        LT_TimeCst;
    tmddTDA18250_TimeCst_t        MTO_TimeCst;
    tmddTDA18250IF_Level_t        uIF_Level;
    UInt8                         AGC1_Gud[tmTDA18250_AGC1_Gud_Max];
    UInt8                         AGC1_Aup[tmTDA18250_AGC1_Aup_Max];
    UInt8                         AGC1_Ado[tmTDA18250_AGC1_Ado_Max];
    UInt8                         AGC2_GdownDetector[tmTDA18250_BP_Filter_Max][tmTDA18250_AGC2_Detector_Max];
    tmTDA18250StdParameters_t     Std_Array[tmTDA18250_StandardMode_Max];
    tmTDA18250BandIndex_t         BandIndex;
    tmTDA18250BandParameters_t    BandParam[tmTDA18250_IND_Band_Max];
    tmTDA18250RFAGC_Top_t         RFAGC_Top[tmTDA18250_IND_RFAGC_Top_Max];
} tmTDA18250Object_t, *ptmTDA18250Object_t, **pptmTDA18250Object_t;


/*============================================================================*/
/* Internal Prototypes:                                                       */
/*============================================================================*/

extern tmErrorCode_t
TDA18250Reset
(
    tmUnitSelect_t  tUnit     /* I: Unit number */
);

extern tmErrorCode_t
TDA18250IRCalibration
(
    tmUnitSelect_t tUnit        /* I: Unit number */
);

extern tmErrorCode_t
TDA18250IRCAL_InitRestore
(
    tmUnitSelect_t     tUnit,       /* I: Unit number */
    tmTDA18250_Step_t  uIRInit      /* I: IRCAL Init  */
);

extern tmErrorCode_t
TDA18250IRCAL_Init
(
    tmUnitSelect_t       tUnit,       /* I: Unit number */
    tmddTDA18250_State_t uIRInit      /* I: IRCAL Init  */
);

extern tmErrorCode_t
TDA18250SignalCalibration
(
    tmUnitSelect_t tUnit,       /* I: Unit number */
    UInt8          uIR_Freq,    /* I: Freq step   */
    UInt32         uLO          /* I: LO          */
);

extern tmErrorCode_t
TDA18250ImageCalibration
(
    tmUnitSelect_t tUnit     /* I: Unit number */
);

extern tmErrorCode_t
TDA18250ResetAGC
(
    tmUnitSelect_t   tUnit      /* I: Unit number */
);

extern tmErrorCode_t
TDA18250WriteAGC1_TOP
(
    tmUnitSelect_t   tUnit      /* I: Unit number */
);

extern tmErrorCode_t
TDA18250WriteAGC1_UpDownGain
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uAudAdd,  /* I: Up/Down address */
    UInt8            uAudGain  /* I: Up/Down gain    */
);
extern tmErrorCode_t
TDA18250WriteAGC2_TOP
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uBPIndex  /* I: BP filter index */
);

extern tmErrorCode_t
TDA18250WriteAGC2_UpDownGain
(
    tmUnitSelect_t   tUnit,    /* I: Unit number     */
    UInt8            uGudAdd,  /* I: Up/Down address */
    UInt8            uGudGain  /* I: Up/Down gain    */
);

extern tmErrorCode_t
TDA18250CalAV
(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

extern tmErrorCode_t
TDA18250SetSwitchOpen
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t
TDA18250SetSwitchClose
(
    tmUnitSelect_t            tUnit,         /* I: Unit number */
    tmTDA18250BandIndex_t     uNewBandIndex  /* I: Band Index  */
);

extern tmErrorCode_t
TDA18250SwitchDirectAccess
(
    tmUnitSelect_t            tUnit,       /* I: Unit number  */
    tmTDA18250Switch_t        Switch,      /* I: Switch Name  */
    tmddTDA18250SwitchState_t SwitchState  /* I: Switch State */
);

extern tmErrorCode_t
TDA18250SwitchSmoothAccess
(
    tmUnitSelect_t            tUnit,       /* I: Unit number  */
    tmTDA18250Switch_t        Switch,      /* I: Switch Name  */
    tmddTDA18250SwitchState_t SwitchState  /* I: Switch State */
);

extern tmErrorCode_t
TDA18250SetMTOOpen
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           MTOindex    /* MTO number     */
);

extern tmErrorCode_t
TDA18250SetMTOClose
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8           MTOindex    /* I: MTO number  */
);

extern tmErrorCode_t
TDA18250MTODirectAccess
(
    tmUnitSelect_t              tUnit,      /* I: Unit number */
    UInt8                       MTOindex,   /* I: MTO number  */
    tmddTDA18250SwitchState_t   MTOState    /* I: MTO State   */
);

extern tmErrorCode_t
TDA18250MTOSmoothAccess
(
    tmUnitSelect_t            tUnit,        /* I: Unit number */
    UInt8                     MTOindex,     /* I: MTO number  */
    tmddTDA18250SwitchState_t MTOState      /* I: MTO State   */
);

extern tmErrorCode_t
TDA18250SetDigitalPath
(
    tmUnitSelect_t        tUnit,        /* I: Unit number */
    tmTDA18250BandIndex_t NewBandIndex  /* I: Band Index  */
);

extern tmErrorCode_t
TDA18250RF2BandIndex
(
    tmUnitSelect_t        tUnit,       /* I: Unit number */
    UInt32                uRF,         /* I: Switch Type */
    tmTDA18250BandIndex_t *pBandIndex  /* O: Band Index  */
);

extern tmErrorCode_t
TDA18250WriteAGC2ToxdB
(
    tmUnitSelect_t          tUnit,        /* I: Unit number */
    UInt8                   AGC2_Gain     /* I: Gain        */
);

extern tmErrorCode_t
TDA18250WriteAGC2To0dB
(
    tmUnitSelect_t          tUnit,        /* I: Unit number        */
    tmTDA18250_AGC2Step3dB  eAGC2Step3dB  /* I: Switch Smooth used */
);

extern tmErrorCode_t
TDA18250WriteRF_Filters
(
    tmUnitSelect_t         tUnit,         /* I: Unit number */
    tmTDA18250BandIndex_t  uNewBandIndex  /* I: Band Index  */
);

extern tmErrorCode_t
TDA18250WriteAnalogRF_Filters
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t
TDA18250SetStandardMode
(
    tmUnitSelect_t tUnit    /* I: Unit number */
);

extern tmErrorCode_t
TDA18250SetRFAGC_Top
(
    tmUnitSelect_t  tUnit   /* I: Unit number  */
);

extern tmErrorCode_t
TDA18250SetBlocksPath
(
    tmUnitSelect_t          tUnit,       /* I: Unit number  */
    tmTDA18250ChannelType_t ChannelType  /* I: Channel type */
);

extern tmErrorCode_t
TDA18250GetRSSI
(
    tmUnitSelect_t  tUnit,      /* I: Unit number */
    UInt8*          pRSSI       /* O: RSSI value  */
);

Int32
TDA18250CalcPow
(
    Int32 uX,     /* I: value  */
    Int32 uY      /* I: value  */
);

tmErrorCode_t
TDA18250LvlAgc2Contribution
(
    UInt8       uAGC2b,         /* I: AGC2 gain          */
    UInt32      uRF,            /* I: RF frequency (MHz) */
    Int32*      pPowerLevel     /* I/O: PowerLevel       */
);

tmErrorCode_t
TDA18250LvlAgc1Contribution
(
    tmddTDA18250_AGC1_Gain_t uAGC1,         /* I: AGC1 gain          */
    UInt32                   uRF,           /* I: RF frequency (MHz) */
    Int32*                   pPowerLevel    /* I/O: PowerLevel       */
);

tmErrorCode_t
TDA18250LvlAgc1MasterMultiContribution
(
    tmddTDA18250_AGC1_Gain_t uAGC1,         /* I: AGC1 gain          */
    UInt32                   uRF,           /* I: RF frequency (MHz) */
    Int32*                   pPowerLevel    /* I/O: PowerLevel       */
);

extern tmErrorCode_t
TDA18250GetPower_Level
(
   tmUnitSelect_t  tUnit,        /* I: Unit number                                  */
   UInt32          IF,           /* I: Intermediate Frequency                       */
   UInt8           uRSSIValue,   /* I: RSSI value reading previously                */
   Int32*          pRFEVAL       /* O: Address of the variable to output item value */
);

extern tmErrorCode_t
TDA18250SetAGC1_loop_off
(
    tmUnitSelect_t          tUnit,      /* I: Unit number */
    tmTDA18250_State_t      uState      /* I: State value */
);

tmErrorCode_t
TDA18250SetAGC2_Bypass
(
    tmUnitSelect_t  tUnit   /* I: Unit number */
);

extern tmErrorCode_t TDA18250Wait(ptmTDA18250Object_t pObj, UInt32 Time);

extern tmErrorCode_t TDA18250MutexAcquire(ptmTDA18250Object_t   pObj, UInt32 timeOut);
extern tmErrorCode_t TDA18250MutexRelease(ptmTDA18250Object_t   pObj);


#ifdef __cplusplus
}
#endif

#endif /* _TMBSL_TDA18250_LOCAL_H */
