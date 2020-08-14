/*
  Copyright (C) 2006-2010 NXP B.V., All Rights Reserved.
  This source code and any compilation or derivative thereof is the proprietary
  information of NXP B.V. and is confidential in nature. Under no circumstances
  is this software to be  exposed to or placed under an Open Source License of
  any type without the expressed written permission of NXP B.V.
 *
 * \file          tmbslFrontEndTypes.h
 *
 *                %version: CFR_FEAP#4 %
 *
 * \date          %modify_time%
 *
 * \brief         Describe briefly the purpose of this file.
 *
 * REFERENCE DOCUMENTS :
 *
 *
 * \section info Change Information
 *
*/

#ifndef _FRONTEND_TYPES_H
#define _FRONTEND_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/* Types and defines:                                                         */
/*============================================================================*/

    /* Status of the carrier phase lock loop */
    typedef enum _tmbslFrontEndState_t
    {
        /** status Unknown */
        tmbslFrontEndStateUnknown = 0,
        /** Channel locked*/
        tmbslFrontEndStateLocked,
        /** Channel not locked */
        tmbslFrontEndStateNotLocked,
        /** Channel lock in process */
        tmbslFrontEndStateSearching,
        tmbslFrontEndStateMax
    } tmbslFrontEndState_t, *ptmbslFrontEndState_t;


    /* GPIO Config */
    typedef enum _tmbslFrontEndGpioConfig_t
    {
        tmbslFrontEndGpioConfUnknown = 0,
        tmbslFrontEndGpioConfInput,
        tmbslFrontEndGpioConfOutputPushPull,
        tmbslFrontEndGpioConfOutputOpenDrain,
        tmbslFrontEndGpioConfTriState,
        tmbslFrontEndGpioConfMax
    } tmbslFrontEndGpioConfig_t, *ptmbslFrontEndGpioConfig_t;

    /* GPIO Polarity */
    typedef enum _tmbslFrontEndGpioPolarity_t
    {
        tmbslFrontEndGpioPolUnknown = 0,
        tmbslFrontEndGpioPolNotInverted,
        tmbslFrontEndGpioPolInverted,
        tmbslFrontEndGpioPolMax
    } tmbslFrontEndGpioPolarity_t, *ptmbslFrontEndGpioPolarity_t;

    /* IT Selection */
    typedef enum _tmbslFrontEndITSel_t
    {
        tmbslFrontEndITSelUnknown = 0,
        tmbslFrontEndITSelFELGoesUp,
        tmbslFrontEndITSelFELGoesDown,
        tmbslFrontEndITSelDVBSynchroFlag,
        tmbslFrontEndITSelVBERRefreshed,
        tmbslFrontEndITSelBERRefreshed,
        tmbslFrontEndITSelUncor,
        tmbslFrontEndGpioITSelMax
    } tmbslFrontEndITSel_t, *ptmbslFrontEndITSel_t;

    /* I2C switch */
    typedef enum _tmbslFrontEndI2CSwitchState_t
    {
        tmbslFrontEndI2CSwitchStateUnknown = 0,
        tmbslFrontEndI2CSwitchStateOpen,
        tmbslFrontEndI2CSwitchStateClosed,
        tmbslFrontEndI2CSwitchStateReset,
        tmbslFrontEndI2CSwitchStateMax
    } tmbslFrontEndI2CSwitchState_t, *ptmbslFrontEndI2CSwitchState_t;

    /* DVBT2 PLP */
    typedef struct _tmbslFrontEndDVBT2PLP_t
    {
        UInt32 uId;
        UInt32 uGroupId;
        tmFrontEndDVBT2PLPType_t eType;
        tmFrontEndDVBT2PLPPayloadType_t ePayloadType;
        tmFrontEndCodeRate_t eCodeRate;
        tmFrontEndModulation_t eModulation;
        tmFrontEndRotationState eRotation;
        tmFrontEndDVBT2FECType_t eFECType;
    } tmbslFrontEndDVBT2PLP_t;

    typedef enum _tmbslFrontEndTVStandard_t
    {
        tmbslFrontEndTVStandardNone,
        tmbslFrontEndTVStandardMN,
        tmbslFrontEndTVStandardBG,
        tmbslFrontEndTVStandardI,
        tmbslFrontEndTVStandardDKL,
        tmbslFrontEndTVStandardLp,
        tmbslFrontEndTVStandardMax
    } tmbslFrontEndTVStandard_t;

    typedef enum _tmbslFrontEndStandardMode_t {
        tmbslFrontEndStandardModeUnknown = 0,  /* Unknown standard */
        tmbslFrontEndStandardQAM6MHz,          /* Digital TV QAM 6MHz */
        tmbslFrontEndStandardQAM8MHz,          /* Digital TV QAM 8MHz */
        tmbslFrontEndStandardATSC6MHz,         /* Digital TV ATSC 6MHz */
        tmbslFrontEndStandardISDBT6MHz,        /* Digital TV ISDBT 6MHz */
        tmbslFrontEndStandardDVBT1_7MHz,       /* Digital TV DVB-T/T2 6MHz */
        tmbslFrontEndStandardDVBT6MHz,         /* Digital TV DVB-T/T2 6MHz */
        tmbslFrontEndStandardDVBT7MHz,         /* Digital TV DVB-T/T2 7MHz */
        tmbslFrontEndStandardDVBT8MHz,         /* Digital TV DVB-T/T2 8MHz */
        tmbslFrontEndStandardDVBT10MHz,        /* Digital TV DVB-T/T2 10MHz */     
        tmbslFrontEndStandardDMBT8MHz,         /* Digital TV DMB-T 8MHz */
        tmbslFrontEndStandardDTMB8MHz,         /* Digital TV DTMB 8MHz */
        tmbslFrontEndStandardFMRadio,          /* Analog FM Radio */
        tmbslFrontEndStandardANLGMN,           /* Analog TV M/N */
        tmbslFrontEndStandardANLGB,            /* Analog TV B */
        tmbslFrontEndStandardANLGGH,           /* Analog TV G/H */
        tmbslFrontEndStandardANLGI,            /* Analog TV I */
        tmbslFrontEndStandardANLGDK,           /* Analog TV D/K */
        tmbslFrontEndStandardANLGL,            /* Analog TV L */
        tmbslFrontEndStandardANLGLL,           /* Analog TV L' */
        tmbslFrontEndStandardScanning,         /* Analog Preset Blind Scanning */
        tmbslFrontEndStandardScanXpress,       /* ScanXpress */
		tmbslFrontEndStandardDAB,              /* DAB */
		tmbslFrontEndStandardHDRADIO,          /* HDRADIO */
		tmbslFrontEndStandardGPS,              /* GPS */
		tmbslFrontEndStandardLNA,              /* LNA only */
		tmbslFrontEndStandardStandardMode_Max
    } tmbslFrontEndStandardMode_t, *ptmbslFrontEndStandardMode_t;

	/* Bus Access */
    typedef enum _tmbslFrontEndBusAccess_t
    {
        /* RW Bus access */
        Bus_RW = 0x00,
        /* No Read Bus access */
        Bus_NoRead = 0x01,
        /* No Write Bus access */
        Bus_NoWrite = 0x02,
        /* No Bus access. Update RegMap only */
        Bus_None = Bus_NoRead|Bus_NoWrite,
        Bus_Max
    } tmbslFrontEndBusAccess_t;
    
    typedef tmErrorCode_t   (*GetMasterContributionPointer)(UInt32 uRF, Int32* pDeltaGain);

	/******************************************************************************/
	/** \brief "Function pointers to hardware access services"
	 *
	 ******************************************************************************/
	 
#ifdef TMFL_NEWSPI

	typedef struct _tmbslFrontEndIoFunc_t
	{
		/** Read hardware function */
		tmErrorCode_t   (*Read)(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData);
		/** Write hardware register, 8bit aligned function */
		tmErrorCode_t   (*Write)(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData);
		/** SPI Read hardware function */
		tmErrorCode_t   (*SpiRead)(tmUnitSelect_t tUnit, UInt8 CS, UInt32 ReadBitsLen, UInt8* pData);
		/** SPI Write hardware register */
		tmErrorCode_t   (*SpiWrite)(tmUnitSelect_t tUnit, UInt8	CS, UInt32 WriteBitsLen, UInt8* pData);
		/** SPI Write followed by Read hardware register */
		tmErrorCode_t   (*SpiWriteRead)(tmUnitSelect_t tUnit, UInt8	CS, UInt32 WriteBitsLen, UInt8* pDataWrite, UInt32 ReadBitsLen, UInt8* pDataRead);
	} tmbslFrontEndIoFunc_t, *ptmbslFrontEndIoFunc_t;

#else

	typedef struct _tmbslFrontEndIoFunc_t
    {
        /** Read hardware function */
        tmErrorCode_t   (*Read)(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData);
        /** Write hardware register, 8bit aligned function */
        tmErrorCode_t   (*Write)(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData);
    } tmbslFrontEndIoFunc_t, *ptmbslFrontEndIoFunc_t;

#endif

    /******************************************************************************/
    /** \brief "Function pointers to Time services"
    *
    ******************************************************************************/
    typedef struct _tmbslFrontEndTimeFunc_t
    {
        /** Return current time value in ms */
        tmErrorCode_t   (*Get)(UInt32 *ptms); 
        /**  Wait t ms without blocking scheduler; warning this function 
        don't schedule others frontend instance */
        tmErrorCode_t   (*Wait)(tmUnitSelect_t tUnit, UInt32 tms);
    } tmbslFrontEndTimeFunc_t, *ptmbslFrontEndTimeFunc_t;


    /******************************************************************************/
    /** \brief "Function pointers to Debug services "
    *
    ******************************************************************************/
    typedef struct _tmbslFrontEndDebugFunc_t
    {
        /** Print a debug message */
        tmErrorCode_t   (*Print)(UInt32 level, const char* format, ...);
    } tmbslFrontEndDebugFunc_t, *ptmbslFrontEndDebugFunc_t;

    /* Mutex types */
    typedef void *ptmbslFrontEndMutexHandle;
#define TMBSL_FRONTEND_MUTEX_TIMEOUT_INFINITE       (0xFFFFFFFF)

    /******************************************************************************/
    /** \brief "Function pointers to Mutex services "
    *
    ******************************************************************************/
#ifdef _TVFE_SW_ARCH_V4

    typedef struct _tmbslFrontEndMutexFunc_t
    {
        /* Open a mutex object */
        tmErrorCode_t (*Open)(ptmbslFrontEndMutexHandle *ppMutexHandle);
        /* Close a mutex object */
        tmErrorCode_t (*Close)(ptmbslFrontEndMutexHandle pMutexHandle);
        /* Acquire a mutex object */
        tmErrorCode_t (*Acquire)(ptmbslFrontEndMutexHandle pMutexHandle, UInt32 timeOut);
        /* Release a mutex object */
        tmErrorCode_t (*Release)(ptmbslFrontEndMutexHandle pMutexHandle);
    } tmbslFrontEndMutexFunc_t, *ptmbslFrontEndMutexFunc_t;

#else

    /* Deprecated Mutex function pointer structure: */
    typedef struct _tmbslFrontEndMutexFunc_t
    {
        /* Open a mutex object */
        tmErrorCode_t (*Init)(ptmbslFrontEndMutexHandle *ppMutexHandle);
        /* Close a mutex object */
        tmErrorCode_t (*DeInit)(ptmbslFrontEndMutexHandle pMutexHandle);
        /* Acquire a mutex object */
        tmErrorCode_t (*Acquire)(ptmbslFrontEndMutexHandle pMutexHandle, UInt32 timeOut);
        /* Release a mutex object */
        tmErrorCode_t (*Release)(ptmbslFrontEndMutexHandle pMutexHandle);
    } tmbslFrontEndMutexFunc_t, *ptmbslFrontEndMutexFunc_t;

#endif

    /******************************************************************************/
    /** \brief "This structure contain all the bsl driver external dependencies"
    *
    *  \sa    "all bsl 'init' functions"
    *
    ******************************************************************************/
    typedef struct _tmbslFrontEndDependency_t
    {
        /** Hardware access to FrontEnd */
        tmbslFrontEndIoFunc_t       sIo;
        /** Time services (wait, getTime, ...) */
        tmbslFrontEndTimeFunc_t     sTime;
        /** Debug services (Print, Store, ...) */
        tmbslFrontEndDebugFunc_t    sDebug;
        /** Mutex services */
        tmbslFrontEndMutexFunc_t    sMutex;
        /** Device Parameters data size */
        UInt32                      dwAdditionalDataSize;
        /** Device Parameters data pointer */
        void*                       pAdditionalData;
    } tmbslFrontEndDependency_t, *ptmbslFrontEndDependency_t;

    /* Hardware access Functions macros */
#define FUNC_SIO(_FUNC)                         _FUNC.sIo
#define P_FUNC_SIO(_PFUNC)                      _PFUNC->sIo
#define FUNC_SIO_READ(_FUNC)                    FUNC_SIO(_FUNC).Read
#define P_FUNC_SIO_READ(_PFUNC)                 P_FUNC_SIO(_PFUNC).Read
#define FUNC_SIO_WRITE(_FUNC)                   FUNC_SIO(_FUNC).Write
#define P_FUNC_SIO_WRITE(_PFUNC)                P_FUNC_SIO(_PFUNC).Write
#define FUNC_SIO_READ_VALID(_FUNC)              (FUNC_SIO_READ(_FUNC) != Null)
#define P_FUNC_SIO_READ_VALID(_PFUNC)           (P_FUNC_SIO_READ(_PFUNC) != Null)
#define FUNC_SIO_WRITE_VALID(_FUNC)             (FUNC_SIO_WRITE(_FUNC) != Null)
#define P_FUNC_SIO_WRITE_VALID(_PFUNC)          (P_FUNC_SIO_WRITE(_PFUNC) != Null)

    /* Time Functions macros */
#define FUNC_STIME(_FUNC)                       _FUNC.sTime
#define P_FUNC_STIME(_PFUNC)                    _PFUNC->sTime
#define FUNC_STIME_GET(_FUNC)                   FUNC_STIME(_FUNC).Get
#define P_FUNC_STIME_GET(_PFUNC)                P_FUNC_STIME(_PFUNC).Get
#define FUNC_STIME_WAIT(_FUNC)                  FUNC_STIME(_FUNC).Wait
#define P_FUNC_STIME_WAIT(_PFUNC)               P_FUNC_STIME(_PFUNC).Wait
#define FUNC_STIME_GET_VALID(_FUNC)             (FUNC_STIME_GET(_FUNC) != Null)
#define P_FUNC_STIME_GET_VALID(_PFUNC)          (P_FUNC_STIME_GET(_PFUNC) != Null)
#define FUNC_STIME_WAIT_VALID(_FUNC)            (FUNC_STIME_WAIT(_FUNC) != Null)
#define P_FUNC_STIME_WAIT_VALID(_PFUNC)         (P_FUNC_STIME_WAIT(_PFUNC) != Null)

    /* Debug Functions macros */
#define FUNC_SDEBUG(_FUNC)                      _FUNC.sDebug
#define P_FUNC_SDEBUG(_PFUNC)                   _PFUNC->sDebug
#define FUNC_SDEBUG_PRINT(_FUNC)                FUNC_SDEBUG(_FUNC).Print
#define P_FUNC_SDEBUG_PRINT(_PFUNC)             P_FUNC_SDEBUG(_PFUNC).Print
#define FUNC_SDEBUG_PRINT_VALID(_FUNC)          (FUNC_SDEBUG_PRINT(_FUNC) != Null)
#define P_FUNC_SDEBUG_PRINT_VALID(_PFUNC)       (P_FUNC_SDEBUG_PRINT(_PFUNC) != Null)

    /* Mutex Functions macros */
#define FUNC_SMUTEX(_FUNC)                      _FUNC.sMutex
#define P_FUNC_SMUTEX(_PFUNC)                   _PFUNC->sMutex

#ifdef _TVFE_SW_ARCH_V4
#define FUNC_SMUTEX_OPEN(_FUNC)                 FUNC_SMUTEX(_FUNC).Open
#define P_FUNC_SMUTEX_OPEN(_PFUNC)              P_FUNC_SMUTEX(_PFUNC).Open
#define FUNC_SMUTEX_CLOSE(_FUNC)                FUNC_SMUTEX(_FUNC).Close
#define P_FUNC_SMUTEX_CLOSE(_PFUNC)             P_FUNC_SMUTEX(_PFUNC).Close
#else
#define FUNC_SMUTEX_OPEN(_FUNC)                 FUNC_SMUTEX(_FUNC).Init
#define P_FUNC_SMUTEX_OPEN(_PFUNC)              P_FUNC_SMUTEX(_PFUNC).Init
#define FUNC_SMUTEX_CLOSE(_FUNC)                FUNC_SMUTEX(_FUNC).DeInit
#define P_FUNC_SMUTEX_CLOSE(_PFUNC)             P_FUNC_SMUTEX(_PFUNC).DeInit
#endif

#define FUNC_SMUTEX_ACQUIRE(_FUNC)              FUNC_SMUTEX(_FUNC).Acquire
#define P_FUNC_SMUTEX_ACQUIRE(_PFUNC)           P_FUNC_SMUTEX(_PFUNC).Acquire
#define FUNC_SMUTEX_RELEASE(_FUNC)              FUNC_SMUTEX(_FUNC).Release
#define P_FUNC_SMUTEX_RELEASE(_PFUNC)           P_FUNC_SMUTEX(_PFUNC).Release
#define FUNC_SMUTEX_OPEN_VALID(_FUNC)           (FUNC_SMUTEX_OPEN(_FUNC) != Null)
#define P_FUNC_SMUTEX_OPEN_VALID(_PFUNC)        (P_FUNC_SMUTEX_OPEN(_PFUNC) != Null)
#define FUNC_SMUTEX_CLOSE_VALID(_FUNC)          (FUNC_SMUTEX_CLOSE(_FUNC) != Null)
#define P_FUNC_SMUTEX_CLOSE_VALID(_PFUNC)       (P_FUNC_SMUTEX_CLOSE(_PFUNC) != Null)
#define FUNC_SMUTEX_ACQUIRE_VALID(_FUNC)        (FUNC_SMUTEX_ACQUIRE(_FUNC) != Null)
#define P_FUNC_SMUTEX_ACQUIRE_VALID(_PFUNC)     (P_FUNC_SMUTEX_ACQUIRE(_PFUNC) != Null)
#define FUNC_SMUTEX_RELEASE_VALID(_FUNC)        (FUNC_SMUTEX_RELEASE(_FUNC) != Null)
#define P_FUNC_SMUTEX_RELEASE_VALID(_PFUNC)     (P_FUNC_SMUTEX_RELEASE(_PFUNC) != Null)

    /*============================================================================*/

    typedef struct tmSWSettingsVersion
    {
        UInt32  customerNr;     /* SW Settings customer number */
        UInt32  projectNr;      /* SW Settings project number */
        UInt32  majorVersionNr; /* SW Settings major version number */
        UInt32  minorVersionNr; /* SW Settings minor version number */

    }   tmSWSettingsVersion_t, *ptmSWSettingsVersion_t;

    /******************************************************************************/
    /** \brief "These macros map to trace functions "
    * P_DBGPRINTEx macro should be defined in each component
    ******************************************************************************/

#define DEBUGLVL_DISABLED       (0)
#define DEBUGLVL_ERROR          (DEBUGLVL_DISABLED+1)
#define DEBUGLVL_TERSE          (DEBUGLVL_ERROR+1)
#define DEBUGLVL_VERBOSE        (DEBUGLVL_TERSE+1)
#define DEBUGLVL_BLAB           (DEBUGLVL_VERBOSE+1)
#define DEBUGLVL_INOUT          (DEBUGLVL_BLAB+1)
#define DEBUGLVL_MAX            (DEBUGLVL_INOUT+1)

#if defined(_TVFE_DEBUG_TRACE) || defined(TMBSLFRONTEND_DEBUG_TRACE)

#ifdef _COMPILER_BEFORE_C99

#define tmDBGPRINTEx(_level, _format, args...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##args); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "==> Error in %s, Line %d\n", __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmDBGPRINTErrEx(_errorCode, _level, _format, args...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##args); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "=> Error 0x%08X in %s (%d)\n", _errorCode, __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmDBGPRINTErrAssertEx(_level, _format, args...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##args); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "=> Error 0x%08X in %s (%d)\n", _tmASSERTExErrorCode, __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmASSERTExTR(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    tmErrorCode_t _tmASSERTExErrorCode = _retVar; \
    tmDBGPRINTErrAssertEx _strings ; \
    return _retVar; \
    } \
    }

#define tmASSERTExT(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    tmErrorCode_t _tmASSERTExErrorCode = _retVar; \
    tmDBGPRINTErrAssertEx _strings ; \
    } \
    }

#else    

#define tmDBGPRINTEx(_level, _format, ...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##__VA_ARGS__); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "==> Error in %s, Line %d\n", __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmDBGPRINTErrEx(_errorCode, _level, _format, ...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##__VA_ARGS__); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "=> Error 0x%08X in %s (%d)\n", _errorCode, __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmDBGPRINTErrAssertEx(_level, _format, ...) \
    { \
    if(P_DBGPRINTVALID) \
    { \
    P_DBGPRINTEx(_level, _format, ##__VA_ARGS__); \
    if(_level == DEBUGLVL_ERROR) \
    { \
    P_DBGPRINTEx(_level, "=> Error 0x%08X in %s (%d)\n", _tmASSERTExErrorCode, __FUNCTION__, __LINE__ ); \
    } \
    } \
    }

#define tmASSERTExTR(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    tmErrorCode_t _tmASSERTExErrorCode = _retVar; \
    tmDBGPRINTErrAssertEx _strings ; \
    return _retVar; \
    } \
    }

#define tmASSERTExT(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    tmErrorCode_t _tmASSERTExErrorCode = _retVar; \
    tmDBGPRINTErrAssertEx _strings ; \
    } \
    }
#endif

#else

#ifdef _COMPILER_BEFORE_C99

#define tmDBGPRINTEx(_level, _format, args...)

#define tmDBGPRINTErrEx(_errorCode, _level, _format, args...)

#define tmDBGPRINTErrAssertEx(_level, _format, args...)

#define tmASSERTExTR(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    return _retVar; \
    } \
    }

#define tmASSERTExT(_retVar, _expr, _strings)

#else 

#define tmDBGPRINTEx(_level, _format, ...)

#define tmDBGPRINTErrEx(_errorCode, _level, _format, ...)

#define tmDBGPRINTErrAssertEx(_level, _format, ...)

#define tmASSERTExTR(_retVar, _expr, _strings) \
    { \
    if((_retVar) != (_expr)) \
    { \
    return _retVar; \
    } \
    }

#define tmASSERTExT(_retVar, _expr, _strings)

#endif

#endif

    /*============================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* _FRONTEND_TYPES_H */

