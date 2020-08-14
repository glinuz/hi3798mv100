/******************************************************************************

          Copyright (C), 2001-2015, HiSilicon Technologies Co., Ltd.
 ******************************************************************************
 File Name       : hiota_parse.h
 Version         : Initial draft
 Author          : Device Chipset STB Development Dept
 Created Date    : 2015-07-29
 Last Modified by: l00163273
 Description     :
 Function List   :
 Change History  :
 Version  Date        Author     DefectNum         Description
 main\1   2015-07-29  l00163273  N/A               Create this file.
 ******************************************************************************/

#ifndef __LOADERDB_DBINFO_H__
#define __LOADERDB_DBINFO_H__

/******************************************************************************/
/*  Header file references                                                    */
/*  Quote order                                                               */
/*  1. Global header file                                                     */
/*  2. Public header files of other modules                                   */
/*  3. Header files within this module                                        */
/*  4. Private files within this module (.inc)                                */
/******************************************************************************/

#include "hi_type.h"
#include "hi_loader_info_v2.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

/*************************** Macro Definition *********************************/
/** \addtogroup      LOADER_DBINFO */
/** @{ */  /** <!-- [LOADER_DBINFO] */

#define LDDB_D_LEN_MACADDR          (12)                    /**< Max length of Mac address */
#define LDDB_D_LEN_PARTNAME         (32)                    /**< Max length of partiton name */

#define SAT_PARAMETER(pstParam)     (pstParam)->unParam.stOTAParam.unParam.stSat
#define SAT_SATLNBCONFIG(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stSat.stLNBConfig
#define SAT_SWITCH16PORT(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stSat.stSwitch16Port
#define SAT_SWITCH04PORT(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stSat.stSwitch4Port
#define SAT_USALSPARAM(pstParam)    (pstParam)->unParam.stOTAParam.unParam.stSat.stUSALSParam
#define SAT_POSITION(pstParam)      (pstParam)->unParam.stOTAParam.unParam.stSat.stPosition

#define SAT_CONNECTPARAM(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stSat.stConnectParam
#define CAB_CONNECTPARAM(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stCab.stConnectParam
#define TER_CONNECTPARAM(pstParam)  (pstParam)->unParam.stOTAParam.unParam.stTer.stConnectParam


#define LDDB_UPDPARAM_CONVERT(pstDest, pstSrc) \
    { \
        (pstDest)->enUpgradeType = (pstSrc)->enUpgradeType;    \
        (pstDest)->enUpgradeMode = (pstSrc)->enUpgradeMode;    \
        (pstDest)->u32SoftwareVersion = (pstSrc)->u32SoftwareVersion;    \
        if (HI_LOADER_UPGRADE_TYPE_USB == (pstSrc)->enUpgradeType)  \
        {   \
            LDDB_MemCpy(&(pstDest)->unParam.stUSBParam, \
                        &(pstSrc)->unParam.stUSBParam, \
                        sizeof(HI_LOADER_PARAM_USB_S));  \
        } \
        else if (HI_LOADER_UPGRADE_TYPE_FS == (pstSrc)->enUpgradeType)   \
        {   \
            LDDB_MemCpy(&(pstDest)->unParam.stFSParam,  \
                        &(pstSrc)->unParam.stFSParam,  \
                        sizeof(HI_LOADER_PARAM_FS_S));  \
        } \
        else if (HI_LOADER_UPGRADE_TYPE_IP == (pstSrc)->enUpgradeType)   \
        {   \
            LDDB_MemCpy(&(pstDest)->unParam.stIPParam,  \
                        &(pstSrc)->unParam.stIPParam,  \
                        sizeof(HI_LOADER_PARAM_IP_S));  \
        } \
        else \
        { \
            (pstDest)->unParam.stOTAParam.enSigType  = (pstSrc)->unParam.stOTAParam.enSigType; \
            (pstDest)->unParam.stOTAParam.u32TunerID = (pstSrc)->unParam.stOTAParam.u32TunerID; \
            (pstDest)->unParam.stOTAParam.u32Pid     = (pstSrc)->unParam.stOTAParam.u32Pid; \
            \
            if ((HI_UNF_TUNER_SIG_TYPE_CAB & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
            { \
                CAB_CONNECTPARAM(pstDest).bReverse      = CAB_CONNECTPARAM(pstSrc).bReverse; \
                CAB_CONNECTPARAM(pstDest).u32Freq       = CAB_CONNECTPARAM(pstSrc).u32Freq; \
                CAB_CONNECTPARAM(pstDest).u32SymbolRate = CAB_CONNECTPARAM(pstSrc).u32SymbolRate; \
                CAB_CONNECTPARAM(pstDest).enModType     = CAB_CONNECTPARAM(pstSrc).enModType; \
            } \
            else if ((HI_UNF_TUNER_SIG_TYPE_SAT & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
            { \
                SAT_PARAMETER(pstDest).enLNBPower   = SAT_PARAMETER(pstSrc).enLNBPower; \
                SAT_PARAMETER(pstDest).enSwitch22K  = SAT_PARAMETER(pstSrc).enSwitch22K; \
                SAT_PARAMETER(pstDest).enMotorType  = SAT_PARAMETER(pstSrc).enMotorType; \
                \
                SAT_CONNECTPARAM(pstDest).u32Freq          = SAT_CONNECTPARAM(pstSrc).u32Freq; \
                SAT_CONNECTPARAM(pstDest).u32SymbolRate    = SAT_CONNECTPARAM(pstSrc).u32SymbolRate; \
                SAT_CONNECTPARAM(pstDest).enPolar          = SAT_CONNECTPARAM(pstSrc).enPolar; \
                SAT_CONNECTPARAM(pstDest).u32ScrambleValue = SAT_CONNECTPARAM(pstSrc).u32ScrambleValue; \
                \
                SAT_SWITCH16PORT(pstDest).enLevel          = SAT_SWITCH16PORT(pstSrc).enLevel; \
                SAT_SWITCH16PORT(pstDest).enPort           = SAT_SWITCH16PORT(pstSrc).enPort; \
                \
                SAT_SWITCH04PORT(pstDest).enLevel          = SAT_SWITCH04PORT(pstSrc).enLevel; \
                SAT_SWITCH04PORT(pstDest).enPort           = SAT_SWITCH04PORT(pstSrc).enPort; \
                SAT_SWITCH04PORT(pstDest).enPolar          = SAT_SWITCH04PORT(pstSrc).enPolar; \
                SAT_SWITCH04PORT(pstDest).enLNB22K         = SAT_SWITCH04PORT(pstSrc).enLNB22K; \
                \
                SAT_SATLNBCONFIG(pstDest).enLNBType        = SAT_SATLNBCONFIG(pstSrc).enLNBType; \
                SAT_SATLNBCONFIG(pstDest).u32LowLO         = SAT_SATLNBCONFIG(pstSrc).u32LowLO; \
                SAT_SATLNBCONFIG(pstDest).u32HighLO        = SAT_SATLNBCONFIG(pstSrc).u32HighLO; \
                SAT_SATLNBCONFIG(pstDest).enLNBBand        = SAT_SATLNBCONFIG(pstSrc).enLNBBand; \
                SAT_SATLNBCONFIG(pstDest).u8UNIC_SCRNO     = SAT_SATLNBCONFIG(pstSrc).u8UNIC_SCRNO; \
                SAT_SATLNBCONFIG(pstDest).u32UNICIFFreqMHz = SAT_SATLNBCONFIG(pstSrc).u32UNICIFFreqMHz; \
                SAT_SATLNBCONFIG(pstDest).enSatPosn        = SAT_SATLNBCONFIG(pstSrc).enSatPosn; \
                \
                SAT_USALSPARAM(pstDest).u16LocalLongitude  = SAT_USALSPARAM(pstSrc).u16LocalLongitude; \
                SAT_USALSPARAM(pstDest).u16LocalLatitude   = SAT_USALSPARAM(pstSrc).u16LocalLatitude; \
                SAT_USALSPARAM(pstDest).u16SatLongitude    = SAT_USALSPARAM(pstSrc).u16SatLongitude; \
                SAT_USALSPARAM(pstDest).u16Angular         = SAT_USALSPARAM(pstSrc).u16Angular; \
                \
                SAT_POSITION(pstDest).enLevel              = SAT_POSITION(pstSrc).enLevel; \
                SAT_POSITION(pstDest).u32Pos               = SAT_POSITION(pstSrc).u32Pos; \
            } \
            else if (((HI_UNF_TUNER_SIG_TYPE_DVB_T & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
                     || ((HI_UNF_TUNER_SIG_TYPE_DVB_T2 & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
                     || ((HI_UNF_TUNER_SIG_TYPE_ISDB_T & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
                     || ((HI_UNF_TUNER_SIG_TYPE_ATSC_T & (pstSrc)->unParam.stOTAParam.enSigType) != 0) \
                     || ((HI_UNF_TUNER_SIG_TYPE_DTMB & (pstSrc)->unParam.stOTAParam.enSigType) != 0)) \
            { \
                TER_CONNECTPARAM(pstDest).u32Freq          = TER_CONNECTPARAM(pstSrc).u32Freq; \
                TER_CONNECTPARAM(pstDest).u32BandWidth     = TER_CONNECTPARAM(pstSrc).u32BandWidth; \
                TER_CONNECTPARAM(pstDest).enModType        = TER_CONNECTPARAM(pstSrc).enModType; \
                TER_CONNECTPARAM(pstDest).bReverse         = TER_CONNECTPARAM(pstSrc).bReverse; \
                TER_CONNECTPARAM(pstDest).enChannelMode    = TER_CONNECTPARAM(pstSrc).enChannelMode; \
                TER_CONNECTPARAM(pstDest).enDVBTPrio       = TER_CONNECTPARAM(pstSrc).enDVBTPrio; \
            } \
        }\
    }

/** @} */  /*! <!-- Macro Definition End */

/*************************** Structure Definition *****************************/
/** \addtogroup      LOADER_DBINFO */
/** @{ */  /** <!-- [LOADER_DBINFO] */

typedef HI_VOID* LDDB_HANDLE;

/****** Enumeration definition ************/

typedef enum tagLDDB_HARDEN_TYPE_E
{
    LDDB_HARDEN_TYPE_COMMON = 0,
    LDDB_HARDEN_TYPE_CIPHER,
    LDDB_HARDEN_TYPE_BUTT

} LDDB_HARDEN_TYPE_E;

typedef struct tagDDB_DISEQC_SWITCH4PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E         enLevel;        /**< DiSEqC device level */
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E   enPort;         /**< DiSEqC switch port */
    HI_UNF_TUNER_FE_POLARIZATION_E      enPolar;        /**< Polarization type */
    HI_UNF_TUNER_FE_LNB_22K_E           enLNB22K;       /**< 22K status */

} LDDB_DISEQC_SWITCH4PORT_S;

typedef struct tagDDB_DISEQC_SWITCH16PORT_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E         enLevel;        /**< DiSEqC device level */
    HI_UNF_TUNER_DISEQC_SWITCH_PORT_E   enPort;         /**< DiSEqC switch port */

} LDDB_DISEQC_SWITCH16PORT_S;

typedef struct tagDDB_DISEQC_USALS_PARAM_S
{
    HI_U16 u16LocalLongitude;                           /**< local longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude)*/
    HI_U16 u16LocalLatitude;                            /**< local latitude, is 10*latitude, in param N:0-900, S:900-1800(1800-latitude)*/
    HI_U16 u16SatLongitude;                             /**< sat longitude, is 10*longitude, in param, E:0-1800, W:1800-3600(3600-longtitude)*/
    HI_U16 u16Angular;                                  /**< calculate result, out param*/

} DDB_DISEQC_USALS_PARAM_S;

typedef struct tagDDB_DISEQC_POSITION_S
{
    HI_UNF_TUNER_DISEQC_LEVEL_E     enLevel;            /**<DiSEqC device level */
    HI_U32                          u32Pos;             /**<Index of position, 0-255 */

} DDB_DISEQC_POSITION_S;

typedef struct tagLDDB_CONNECT_PARAM_CAB_S
{
    HI_U32                          u32Freq;            /**< Frequency, in kHz */
    HI_U32                          u32SymbolRate;      /**< Symbol rate, in bit/s */
    HI_UNF_MODULATION_TYPE_E        enModType;          /**< QAM mode */
    HI_BOOL                         bReverse;           /**< Spectrum reverse mode */

} LDDB_CONNECT_PARAM_CAB_S;

typedef struct tagLDDB_CONNECT_PARAM_TER_S
{
    HI_U32                          u32Freq;            /**< Frequency in KHz*/
    HI_U32                          u32BandWidth;       /**< Bandwidth in KHz*/
    HI_UNF_MODULATION_TYPE_E        enModType;          /**< Modulation type*/
    HI_BOOL                         bReverse;           /**< Spectrum reverse mode*/
    HI_UNF_TUNER_TER_MODE_E         enChannelMode;      /**< dvb-t2 */
    HI_UNF_TUNER_TS_PRIORITY_E      enDVBTPrio;         /**< dvb-t */

} LDDB_CONNECT_PARAM_TER_S;

typedef struct tagLDDB_CONNECT_PARAM_SAT_S
{
    HI_U32                          u32Freq;            /**< Downlink frequency, in kHz */
    HI_U32                          u32SymbolRate;      /**< Symbol rate, in Symb/s */
    HI_UNF_TUNER_FE_POLARIZATION_E  enPolar;            /**< Polarization type */
    HI_U32                          u32ScrambleValue;   /**< Scramble Value */

} LDDB_CONNECT_PARAM_SAT_S;


typedef struct tagLDDB_LNB_CONFIG_S
{
    HI_UNF_TUNER_FE_LNB_TYPE_E      enLNBType;          /**< LNB type*/
    HI_U32                          u32LowLO;           /**< Low Local Oscillator Frequency, MHz */
    HI_U32                          u32HighLO;          /**< High Local Oscillator Frequency, MHz*/
    HI_UNF_TUNER_FE_LNB_BAND_E      enLNBBand;          /**< LNB band, C or Ku */
    HI_U8                           u8UNIC_SCRNO;       /**< SCR number, 0-7 */
    HI_U32                          u32UNICIFFreqMHz;   /**< SCR IF frequency, MHz */
    HI_U32                          enSatPosn;          /**< unicable multi-switch port ::HI_UNF_TUNER_SATPOSITION_E*/

} LDDB_LNB_CONFIG_S;


/** Cab upgrade parameter Struct */
typedef struct tagLDDB_PARAM_CAB_S
{
    LDDB_CONNECT_PARAM_CAB_S        stConnectParam;     /**< Cable Connect Parameters */

} LDDB_PARAM_CAB_S;

/** Terestrial upgrade parameter Struct */
typedef struct tagLDDB_PARAM_TER_S
{
    LDDB_CONNECT_PARAM_TER_S        stConnectParam;     /**< Terestrial Connect Parameters */

} LDDB_PARAM_TER_S;

/** Satellite upgrade parameter Struct */
typedef struct tagLDDB_PARAM_SAT_S
{
    LDDB_CONNECT_PARAM_SAT_S        stConnectParam;     /**< Satellite Connect Parameters*/
    HI_UNF_TUNER_FE_LNB_POWER_E     enLNBPower;         /**< LNB Power */
    LDDB_LNB_CONFIG_S               stLNBConfig;        /**< LNB configurating parameters */
    HI_UNF_TUNER_SWITCH_22K_E       enSwitch22K;        /**< 22KHz switch */
    LDDB_DISEQC_SWITCH4PORT_S       stSwitch4Port;      /**< DiSEqC 1.0 Switch Parameters */
    LDDB_DISEQC_SWITCH16PORT_S      stSwitch16Port;     /**< DiSEqC 1.1 Switch Parameters */
    HI_LOADER_MOTORTYPE_E           enMotorType;        /**< Motor type */
    DDB_DISEQC_USALS_PARAM_S        stUSALSParam;
    DDB_DISEQC_POSITION_S           stPosition;
} LDDB_PARAM_SAT_S;

typedef struct tagLDDB_PARAM_OTA_S
{
    HI_UNF_TUNER_SIG_TYPE_E         enSigType;          /**< Signal type*/
    HI_U32                          u32TunerID;         /**< Tuner identification */
    HI_U32                          u32Pid;             /**< TS PID. The parameter is vital and set it with caution..*/

    union
    {
        LDDB_PARAM_CAB_S            stCab;              /**< Cable transmission signal */
        LDDB_PARAM_SAT_S            stSat;              /**< Satellite transmission signal */
        LDDB_PARAM_TER_S            stTer;              /**< Terestrial transmission signal */

    } unParam;

} LDDB_PARAM_OTA_S;

/*Define upgrade tag parameter.*/
typedef struct tagLDDB_PARAM_TAG_S
{
    HI_BOOL bNeedUpgrade;                               /**< Whether need upgrade.*/
    HI_BOOL bForceUpgrade;                              /**< Whether force upgrade.*/
    HI_BOOL bUsbBurn;                                   /**< Whether burn empty chipset from usb disk.*/

} LDDB_PARAM_TAG_S;

typedef struct tagLDDB_PARAM_UPGRADE_S
{
    HI_LOADER_UPGRADE_TYPE_E    enUpgradeType;          /**< Upgrade type */
    HI_LOADER_UPGRADE_MODE_E    enUpgradeMode;          /**< Upgrade Mode */
    HI_U32                      u32SoftwareVersion;     /**< The software version of the image to upgrade */

    union
    {
        LDDB_PARAM_OTA_S        stOTAParam;             /**< OTA upgrade parameters */
        HI_LOADER_PARAM_USB_S   stUSBParam;             /**< USB upgrade parameters */
        HI_LOADER_PARAM_IP_S    stIPParam;              /**< IP upgrade parameters */
        HI_LOADER_PARAM_FS_S    stFSParam;              /**< FS upgrade parameters */

    } unParam;


} LDDB_PARAM_UPGRADE_S;

typedef struct tagLDDB_PARAM_PRIVATE_S
{
    HI_U32  u32FailedCnt;                       /**< Number of upgrade failures.*/
    HI_BOOL bTagDestroy;                        /**< Whether some images have been destroyed.*/

} LDDB_PARAM_PRIVATE_S;

typedef struct tagLDDB_LOADER_INFO_S
{
    LDDB_PARAM_TAG_S            stTAGParamData;
    LDDB_PARAM_UPGRADE_S        stUPDParamData;
    LDDB_PARAM_PRIVATE_S        stPrivateParam;

} LDDB_LOADER_INFO_S;

typedef struct tagLDDB_DEVICE_INFO_S
{
    HI_U8   au8MacAddress[LDDB_D_LEN_MACADDR];  /**< Mac Address */
    HI_U32  u32OUI;                             /**< Vendor ID */
    HI_U32  u32AreaCode;                        /**< Region code */
    HI_U32  u32HardwareVersion;                 /**< Hardware version */
    HI_U32  u32SerialNum;                       /**< Hardware serial number */

} LDDB_DEVICE_INFO_S;

typedef struct tagLDDB_PARTITION_INFO_S
{
    HI_U8   au8Name[LDDB_D_LEN_PARTNAME];       /**< Partition Identification */
    HI_U32  u32Version;                         /**< Partition image Version.*/

} LDDB_PARTITION_INFO_S;

/** Software information structure */
typedef struct hiLDDB_SOFTWARE_INFO_S
{
    HI_U32  u32UpgradeIdentification;           /**< Upgrade Identification */
    HI_U32  u32SoftwareVersion;                 /**< Main Software Version */
    HI_U32  u32PartitionCount;                  /**< Partition Count */
    LDDB_PARTITION_INFO_S*  pstPartitionInfo;   /**< Partition Description */

} LDDB_SOFTWARE_INFO_S;


/****** Union definition ******************/

/****** Global variable declaration *******/
/** @} */  /** <!-- ==== Structure Definition End ====*/

/******************************* API Declaration ******************************/
/** \addtogroup      LOADER_DBINFO */
/** @{ */  /** <!-- [LOADER_DBINFO] */

HI_S32 LDDB_FlashOpen(HI_CHAR* pcPartitionName, LDDB_HANDLE* phHandle, HI_U32* pu32Size);

HI_S32 LDDB_FlashClose(LDDB_HANDLE hFlashHandle);

HI_S32 LDDB_FlashRead(LDDB_HANDLE hFlashHandle, HI_U8* pu8Buffer, HI_U32 u32Size, HI_U32* pu32RetSize);

HI_S32 LDDB_FlashWrite(LDDB_HANDLE hFlashHandle, HI_U8* pu8Buffer, HI_U32 u32Size);

HI_S32 LDDB_FlashDataSign
(
    LDDB_HARDEN_TYPE_E  enVerifyType,
    HI_U8*              pu8DataAddr,
    HI_U32              u32DataSize,
    HI_U8*              pu8FlashData,
    HI_U32              u32FlashSize
);

HI_S32 LDDB_FlashDataVerify
(
    LDDB_HARDEN_TYPE_E  enVerifyType,
    HI_U8*              pu8FlashData,
    HI_U32              u32FlashSize,
    HI_U8**             ppu8DataAddr,
    HI_U32*             pu32DataSize
);

HI_S32 LDDB_PartitionOpen(HI_CHAR* pcPartitionName, LDDB_HANDLE* phPartitionHandle, HI_U32* pu32PartitionSize);

HI_S32 LDDB_PartitionClose(LDDB_HANDLE hPartitionHandle);

HI_S32 LDDB_PartitionGetSize(LDDB_HANDLE hPartitionHandle, HI_U32* pu32Size);

HI_S32 LDDB_PartitionDataRead(LDDB_HANDLE hPartitionHandle, HI_U8* pu8Buffer, HI_U32 u32Size, HI_U32* pu32RetSize);

HI_S32 LDDB_PartitionDataWrite(LDDB_HANDLE hPartitionHandle, HI_U8* pu8Data, HI_U32 u32Size);

HI_S32 LDDB_GetLoaderInfo(LDDB_LOADER_INFO_S* pstLoaderInfo);

HI_S32 LDDB_SetLoaderInfo(LDDB_LOADER_INFO_S* pstLoaderInfo);

HI_S32 LDDB_GetSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo);

HI_S32 LDDB_SetSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo);

HI_S32 LDDB_FreeSoftwareInfo(LDDB_SOFTWARE_INFO_S* pstSoftwareInfo);

HI_S32 LDDB_GetDeviceInfo(LDDB_DEVICE_INFO_S* pstDeviceInfo);


/** @} */  /** <!-- ==== API Declaration End ==== */

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /*__LOADERDB_DBINFO_H__*/

