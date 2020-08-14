/** \file vp_debug.c
 * vp_debug.c
 *
 *  This file contains the implementation of VP_DEBUG MACRO.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7350 $
 * $LastChangedDate: 2010-06-15 16:36:44 -0500 (Tue, 15 Jun 2010) $
 */

#include "vp_api_cfg.h"
#include "vp_api.h"
#include "vp_debug.h"

#ifdef VP_CC_VCP2_SERIES
#include "hbi_common.h"
#endif

uint32 vpDebugSelectMask = VP_OPTION_DEFAULT_DEBUG_SELECT;

/**
 * VpRegisterDump()
 *  This function is used to dump the content of all device registers.
 *
 * Preconditions:
 *  Device context should be created.
 *
 * Postconditions:
 *  Debug output of "all" registers.
 */
VpStatusType
VpRegisterDump(
    VpDevCtxType *pDevCtx)
{
    VpStatusType status;
    VP_API_ENTER(VpDevCtxType, pDevCtx, "RegisterDump");

    /* Basic argument checking */
    if (pDevCtx == VP_NULL) {
        status = VP_STATUS_INVALID_ARG;
    } else {
        status = VP_CALL_DEV_FUNC(RegisterDump, (pDevCtx));
    }

    VP_API_EXIT(VpDevCtxType, pDevCtx, "RegisterDump", status);
    return status;
} /* VpRegisterDump() */

/**
 * VpObjectDump()
 *  This function is used to dump the contents of a Device Context and Device
 * Object (if pDevCtx != VP_NULL) and/or the contents of a Line Context and Line
 * Object (if pLineCtx != VP_NULL).  Either pDevCtx or pLineCtx must be VP_NULL,
 * but not both.
 *
 * Preconditions:
 *  VP_CC_DEBUG_SELECT must include VP_DBG_ERROR for this function to be
 * compiled in.
 *
 * Postconditions:
 *  Debug output of "all" registers.
 */
VpStatusType
VpObjectDump(
    VpLineCtxType *pLineCtx,
    VpDevCtxType *pDevCtx)
{
    VpStatusType status = VP_STATUS_FUNC_NOT_SUPPORTED;

#if (VP_CC_DEBUG_SELECT & (VP_DBG_API_FUNC | VP_DBG_INFO))
    bool singleLine = (pLineCtx != VP_NULL);
#endif

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (singleLine) {
        VP_API_ENTER(VpLineCtxType, pLineCtx, "ObjectDump");
    } else {
        VP_API_ENTER(VpDevCtxType, pDevCtx, "ObjectDump");
    }
#endif

    /* Basic argument checking */
    if (
        ((pDevCtx == VP_NULL) && (pLineCtx == VP_NULL)) ||
        ((pDevCtx != VP_NULL) && (pLineCtx != VP_NULL))
    ) {
        status = VP_STATUS_INVALID_ARG;
    } else {

#if (VP_CC_DEBUG_SELECT & VP_DBG_INFO)
        VpSysDebugPrintf("\npDevCtx = %lu\n", (unsigned long)pDevCtx);
        VpSysDebugPrintf("pLineCtx = %lu\n", (unsigned long)pLineCtx);

        /* Contexts are device-independent, so code for displaying them is
           included here. */
        if (singleLine) {
            VpSysDebugPrintf("\nDumping Line Context:\n");
            VpSysDebugPrintf("\tpLineCtx->pDevCtx = %lu\n", (unsigned long)(pLineCtx->pDevCtx));
            VpSysDebugPrintf("\tpLineCtx->pLineObj = %lu\n", (unsigned long)(pLineCtx->pLineObj));
            pDevCtx = pLineCtx->pDevCtx;
            status = VP_CALL_DEV_FUNC(ObjectDump, (pLineCtx, VP_NULL));
        } else {
            int line;
            VpSysDebugPrintf("\nDumping Device Context:\n");
            VpSysDebugPrintf("\tpDevCtx->deviceType = %d\n", (int)(pDevCtx->deviceType));
            VpSysDebugPrintf("\tpDevCtx->pDevObj = %lu\n", (unsigned long)(pDevCtx->pDevObj));
            VpSysDebugPrintf("\tpDevCtx->pLineCtx[%d] = {", VP_MAX_LINES_PER_DEVICE);
            for (line = 0; line < VP_MAX_LINES_PER_DEVICE; line++) {
                VpSysDebugPrintf(" %lu", (unsigned long)(pDevCtx->pLineCtx[line]));
            }
            VpSysDebugPrintf(" }\n");
            status = VP_CALL_DEV_FUNC(ObjectDump, (VP_NULL, pDevCtx));
        }
#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_INFO) */

    }

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
    if (singleLine) {
        VP_API_EXIT(VpLineCtxType, pLineCtx, "ObjectDump", status);
    } else {
        VP_API_EXIT(VpDevCtxType, pDevCtx, "ObjectDump", status);
    }
#endif

    return status;
} /* VpObjectDump() */

#ifdef VP_DEBUG

static bool
DeviceDebugEnabled(
    uint32 debugSelectMask,
    char *msgTypeStr,
    VpDeviceIdType deviceId)
{
    if (!debugSelectMask)
        return FALSE;

    /* Display the message type and device ID. */
    VpSysDebugPrintf(msgTypeStr);
    VP_PRINT_DEVICE_ID(deviceId);
    VpSysDebugPrintf(": ");
    return TRUE;
}

static bool
LineDebugEnabled(
    uint32 debugSelectMask,
    char *msgTypeStr,
    VpLineIdType lineId)
{
    if (!debugSelectMask)
        return FALSE;

    /* Display the message type and line ID. */
    VpSysDebugPrintf(msgTypeStr);
    VP_PRINT_LINE_ID((lineId));
    VpSysDebugPrintf(": ");
    return TRUE;
}

/* This function is a "fallback" for the case where an object was expected, but
   VP_NULL was passed. */
static bool
NULLDebugEnabled(
    uint32 msgType,
    char *errorMsg,
    char *msgTypeStr)
{
    if (msgType & vpDebugSelectMask) {
        VpSysDebugPrintf(color_fg(red) "%s", errorMsg);
    }
    return VpDebugEnabled_None(msgType, msgTypeStr, VP_NULL);
}

#ifdef VP_CC_VCP2_SERIES
bool
VpDebugEnabled_VpVcp2DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_VpVcp2LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_VCP2_SERIES */

#ifdef VP_CC_890_SERIES
bool
VpDebugEnabled_Vp890DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_Vp890LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_890_SERIES */

#ifdef VP_CC_880_SERIES
bool
VpDebugEnabled_Vp880DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp880DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_Vp880LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp880LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_880_SERIES */

#ifdef VP_CC_580_SERIES
bool
VpDebugEnabled_Vp580DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp580DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pDevObj->debugSelectMask;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

bool
VpDebugEnabled_Vp580LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp580LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pLineObj->debugSelectMask;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_580_SERIES */

#ifdef VP_CC_792_SERIES
static bool
VpDebugEnabled_Vp792DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp792DeviceObjectType *pDevObj)
{
    uint32 debugSelectMask;
    VpDeviceIdType deviceId;

    if (pDevObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pDevObj->options.debugSelect;
    deviceId = pDevObj->deviceId;
    VP_ASSERT(pDevObj != VP_NULL);
    return DeviceDebugEnabled(msgType & debugSelectMask, msgTypeStr, deviceId);
}

static bool
VpDebugEnabled_Vp792LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp792LineObjectType *pLineObj)
{
    uint32 debugSelectMask;
    VpLineIdType lineId;

    if (pLineObj == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineObj == VP_NULL) ", msgTypeStr);
    }

    debugSelectMask = pLineObj->options.debugSelect;
    lineId = pLineObj->lineId;
    VP_ASSERT(pLineObj != VP_NULL);
    return LineDebugEnabled(msgType & debugSelectMask, msgTypeStr, lineId);
}
#endif /* VP_CC_792_SERIES */

bool
VpDebugEnabled_VpDeviceIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpDeviceIdType *pDeviceId)
{
    VP_ASSERT(pDeviceId != VP_NULL);
    return DeviceDebugEnabled(msgType & vpDebugSelectMask, msgTypeStr, *pDeviceId);
}

bool
VpDebugEnabled_VpLineIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineIdType *pLineId)
{
    VP_ASSERT(pLineId != VP_NULL);
    return LineDebugEnabled(msgType & vpDebugSelectMask, msgTypeStr, *pLineId);
}


/*
  * This function returns TRUE if the specified message type is enabled for the specified device.
  * As a side effect, it displays the message type and device ID.
  */
bool
VpDebugEnabled_VpDevCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpDevCtxType *pDevCtx)
{
    if (pDevCtx == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pDevCtx == VP_NULL) ", msgTypeStr);
    }

    switch (pDevCtx->deviceType) {

#ifdef VP_CC_VCP2_SERIES
        case VP_DEV_VCP2_SERIES:
            return VpDebugEnabled_VpVcp2DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            return VpDebugEnabled_Vp890DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
#ifdef VP_CC_880_SERIES
        case VP_DEV_880_SERIES:
            return VpDebugEnabled_Vp880DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
#ifdef VP_CC_792_SERIES
        case VP_DEV_792_SERIES:
            return VpDebugEnabled_Vp792DeviceObjectType(msgType, msgTypeStr, pDevCtx->pDevObj);
#endif
        default:
            /* For other device types (for which we are still using VP_DOUT)
               use the global debug select mask. */
            return VpDebugEnabled_None(msgType, msgTypeStr, (void *)0);
        }
}

/*
  * This function returns TRUE if the specified message type is enabled for the specified line.
  * As a side effect, it displays the message type and line ID.
  */
bool
VpDebugEnabled_VpLineCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineCtxType *pLineCtx)
{
    if (pLineCtx == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineCtx == VP_NULL) ", msgTypeStr);
    }

    if (pLineCtx->pDevCtx == VP_NULL) {
        return NULLDebugEnabled(msgType, "(pLineCtx->pDevCtx == VP_NULL) ", msgTypeStr);
    }

    switch (pLineCtx->pDevCtx->deviceType) {

#ifdef VP_CC_VCP2_SERIES
        case VP_DEV_VCP2_SERIES:
            return VpDebugEnabled_VpVcp2LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
#ifdef VP_CC_890_SERIES
        case VP_DEV_890_SERIES:
            return VpDebugEnabled_Vp890LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
#ifdef VP_CC_880_SERIES
        case VP_DEV_880_SERIES:
            return VpDebugEnabled_Vp880LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
#ifdef VP_CC_580_SERIES
        case VP_DEV_580_SERIES:
            return VpDebugEnabled_Vp580LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
#ifdef VP_CC_792_SERIES
        case VP_DEV_792_SERIES:
            return VpDebugEnabled_Vp792LineObjectType(msgType, msgTypeStr, pLineCtx->pLineObj);
#endif
        default:
            /* For other device types (for which we are still using VP_DOUT)
               use the global debug select mask. */
            return VpDebugEnabled_None(msgType, msgTypeStr, (void *)0);
    }
}

bool
VpDebugEnabled_None(
    uint32 msgType,
    char *msgTypeStr,
    void *nothing)
{
    if (!(msgType & vpDebugSelectMask))
        return FALSE;

    VpSysDebugPrintf(msgTypeStr);
    VpSysDebugPrintf(": ");
    return TRUE;
}

#if (VP_CC_DEBUG_SELECT & (VP_DBG_API_FUNC | VP_DBG_EVENT | VP_DBG_SSL))
static const char *
VpGetEnumString(
    const char *strTable[],
    int numStrings,
    int value)
{
    static char buf[7] = "0x0000"; /* not reentrant */
    static const char hex[16] = "0123456789ABCD";

    if ((value < 0) || (value >= numStrings) || (strTable[value] == VP_NULL)) {

        /* Can't find a string for the requested value. */
        buf[5] = hex[value & 0xF];
        value >>= 4;
        buf[4] = hex[value & 0xF];
        value >>= 4;
        buf[3] = hex[value & 0xF];
        value >>= 4;
        buf[2] = hex[value & 0xF];
        return buf;
    }

    /* Found the requested string in the table. */
    return strTable[value];
}
#endif /* (VP_CC_DEBUG_SELECT & (VP_DBG_API_FUNC | VP_DBG_EVENT | VP_DBG_SSL)) */

#if (VP_CC_DEBUG_SELECT & (VP_DBG_API_FUNC | VP_DBG_EVENT))
const char *
VpGetString_VpStatusType(
    VpStatusType status)
{
    static const char *strTable[VP_STATUS_NUM_TYPES] = {
        "SUCCESS",
        "FAILURE",
        "FUNC_NOT_SUPPORTED",
        "INVALID_ARG",
        "MAILBOX_BUSY",
        "ERR_VTD_CODE",
        "OPTION_NOT_SUPPORTED",
        "ERR_VERIFY",
        "DEVICE_BUSY",
        "MAILBOX_EMPTY",
        "ERR_MAILBOX_DATA",
        "ERR_HBI",
        "ERR_IMAGE",
        "IN_CRTCL_SECTN",
        "DEV_NOT_INITIALIZED",
        "ERR_PROFILE",
        "16",
        "CUSTOM_TERM_NOT_CFG",
        "DEDICATED_PINS",
        "INVALID_LINE",
        "LINE_NOT_CONFIG",
        "INPUT_PARAM_OOR"
    };

    /* Make sure we update the above table when we update VpStatusType: */
    VP_ASSERT(VP_STATUS_NUM_TYPES == 22);

    return VpGetEnumString(strTable, VP_STATUS_NUM_TYPES, (int)status);
}
#endif /* (VP_CC_DEBUG_SELECT & (VP_DBG_API_FUNC | VP_DBG_EVENT)) */

#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
const char *
VpGetString_VpOptionIdType(
    VpOptionIdType optionId)
{
    static const char *strTable[VP_NUM_OPTION_IDS] = {
        "VP_DEVICE_OPTION_ID_PULSE", /* only differs by prefix */
        "CRITICAL_FLT",
        "ZERO_CROSS",
        "RAMP2STBY",
        "PULSE_MODE",
        "TIMESLOT",
        "CODEC",
        "PCM_HWY",
        "LOOPBACK",
        "LINE_STATE",
        "EVENT_MASK",
        VP_NULL,
        "RING_CNTRL",
        VP_NULL,
        "DTMF_MODE",
        "DEVICE_IO",
        VP_NULL,
        "PCM_TXRX_CNTRL",
        "PULSE2",
        "LINE_IO_CFG",
        "DEV_IO_CFG",
        "DTMF_SPEC",
        "PARK_MODE",
        "DCFEED_SLOPE",
        "SWITCHER_CTRL",
        "HOOK_DETECT_MODE",
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        VP_NULL,
        "VP_OPTION_ID_PULSE",        /* only differs by prefix */
        "DEBUG_SELECT"
        "ABS_GAIN",
        "PCM_SIG_CTL",
        "LINESTATE_CTL_MODE"
    };

    /* Make sure we update the above table when we update VpOptionIdType: */
    VP_ASSERT(VP_NUM_OPTION_IDS == 0x29);

    return VpGetEnumString(strTable, VP_NUM_OPTION_IDS, (int)optionId);
}
#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC) */

#if (VP_CC_DEBUG_SELECT & VP_DBG_EVENT)
void VpDisplayEvent(
    VpEventType *pEvent)
{
    typedef struct {
        VpEventCategoryType category;
        uint16 id;
        char *nameStr;
        bool hasEventData;
        bool hasParmHandle;
    } VpEventInfo;

    static const VpEventInfo eventInfoTable[] = {
        { VP_EVCAT_FAULT,      VP_DEV_EVID_BAT_FLT,            "BAT_FLT",            TRUE,  FALSE },
        { VP_EVCAT_FAULT,      VP_DEV_EVID_CLK_FLT,            "CLK_FLT",            TRUE,  FALSE },
        { VP_EVCAT_FAULT,     VP_LINE_EVID_THERM_FLT,          "THERM_FLT",          TRUE,  FALSE },
        { VP_EVCAT_FAULT,     VP_LINE_EVID_DC_FLT,             "DC_FLT",             TRUE,  FALSE },
        { VP_EVCAT_FAULT,     VP_LINE_EVID_AC_FLT,             "AC_FLT",             TRUE,  FALSE },
        { VP_EVCAT_FAULT,     VP_LINE_EVID_SYNC_FLT,           "SYNC_FLT",           FALSE, FALSE },
        { VP_EVCAT_FAULT,     VP_LINE_EVID_RES_LEAK_FLT,       "RES_LEAK_FLT",       TRUE,  TRUE  },
        { VP_EVCAT_FAULT,      VP_DEV_EVID_WDT_FLT,            "WDT_FLT",            FALSE, FALSE },
        { VP_EVCAT_FAULT,      VP_DEV_EVID_EVQ_OFL_FLT,        "EVQ_OFL_FLT",        FALSE, FALSE },
        { VP_EVCAT_FAULT,      VP_DEV_EVID_SYSTEM_FLT,         "SYSTEM_FLT",         TRUE,  FALSE },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_HOOK_OFF,           "HOOK_OFF",           FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_HOOK_ON,            "HOOK_ON",            FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_GKEY_DET,           "GKEY_DET",           FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_GKEY_REL,           "GKEY_REL",           FALSE, FALSE },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_FLASH,              "FLASH",              FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_STARTPULSE,         "STARTPULSE",         FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_DTMF_DIG,           "DTMF_DIG",           TRUE,  FALSE },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_PULSE_DIG,          "PULSE_DIG",          TRUE,  TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_MTONE,              "MTONE",              FALSE, FALSE },
        { VP_EVCAT_SIGNALING,  VP_DEV_EVID_TS_ROLLOVER,        "TS_ROLLOVER",        FALSE, FALSE },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_US_TONE_DETECT,     "US_TONE_DETECT",     TRUE,  TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_DS_TONE_DETECT,     "DS_TONE_DETECT",     TRUE,  TRUE  },
        { VP_EVCAT_SIGNALING,  VP_DEV_EVID_SEQUENCER,          "SEQUENCER",          TRUE,  TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_BREAK_MAX,          "BREAK_MAX",          TRUE,  TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_EXTD_FLASH,         "EXTD_FLASH",         FALSE, TRUE  },
        { VP_EVCAT_SIGNALING, VP_LINE_EVID_HOOK_PREQUAL,       "HOOK_PREQUAL",       TRUE,  TRUE  },
        { VP_EVCAT_RESPONSE,   VP_DEV_EVID_BOOT_CMP,           "BOOT_CMP",           TRUE,  FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_LLCMD_TX_CMP,       "LLCMD_TX_CMP",       FALSE, FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_LLCMD_RX_CMP,       "LLCMD_RX_CMP",       FALSE, TRUE  },
        { VP_EVCAT_RESPONSE,   VP_DEV_EVID_DNSTR_MBOX,         "DNSTR_MBOX",         FALSE, FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_RD_OPTION,          "RD_OPTION",          TRUE,  TRUE  },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_RD_LOOP,            "RD_LOOP",            FALSE, TRUE  },
        { VP_EVCAT_RESPONSE,       VP_EVID_CAL_CMP,            "CAL_CMP",            FALSE, FALSE },
        { VP_EVCAT_RESPONSE,       VP_EVID_CAL_BUSY,           "CAL_BUSY",           FALSE, FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_GAIN_CMP,           "GAIN_CMP",           FALSE, TRUE  },
        { VP_EVCAT_RESPONSE,   VP_DEV_EVID_DEV_INIT_CMP,       "DEV_INIT_CMP",       TRUE,  FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_LINE_INIT_CMP,      "LINE_INIT_CMP",      FALSE, FALSE },
        { VP_EVCAT_RESPONSE,   VP_DEV_EVID_IO_ACCESS_CMP,      "IO_ACCESS_CMP",      TRUE,  FALSE },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_LINE_IO_RD_CMP,     "LINE_IO_RD_CMP",     FALSE, TRUE  },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_LINE_IO_WR_CMP,     "LINE_IO_WR_CMP",     FALSE, TRUE  },
        { VP_EVCAT_RESPONSE,  VP_LINE_EVID_SLAC_INIT_CMP,      "SLAC_INIT_CMP",      TRUE,  FALSE },
        { VP_EVCAT_TEST,      VP_LINE_EVID_TEST_CMP,           "TEST_CMP",           FALSE, TRUE  },
        { VP_EVCAT_TEST,      VP_LINE_EVID_DTONE_DET,          "DTONE_DET",          TRUE,  FALSE },
        { VP_EVCAT_TEST,      VP_LINE_EVID_DTONE_LOSS,         "DTONE_LOSS",         TRUE,  FALSE },
        { VP_EVCAT_TEST,       VP_DEV_EVID_STEST_CMP,          "STEST_CMP",          TRUE,  FALSE },
        { VP_EVCAT_TEST,       VP_DEV_EVID_CHKSUM,             "CHKSUM",             FALSE, TRUE  },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_MTR_CMP,            "MTR_CMP",            FALSE, FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_MTR_ABORT,          "MTR_ABORT",          TRUE,  FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_CID_DATA,           "CID_DATA",           TRUE,  FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_RING_CAD,           "RING_CAD",           TRUE,  FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_SIGNAL_CMP,         "SIGNAL_CMP",         TRUE,  TRUE  },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_MTR_CAD,            "MTR_CAD",            TRUE,  FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_TONE_CAD,           "TONE_CAD",           FALSE, FALSE },
        { VP_EVCAT_PROCESS,   VP_LINE_EVID_MTR_ROLLOVER,       "MTR_ROLLOVER",       TRUE,  TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_RING_ON,            "RING_ON",            FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_LIU,                "LIU",                FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_LNIU,               "LNIU",               FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_FEED_DIS,           "FEED_DIS",           FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_FEED_EN,            "FEED_EN",            FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_DISCONNECT,         "DISCONNECT",         FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_RECONNECT,          "RECONNECT",          FALSE, TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_POLREV,             "POLREV",             TRUE,  TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_POH,                "POH",                TRUE,  TRUE  },
        { VP_EVCAT_FXO,       VP_LINE_EVID_PNOH,               "PNOH",               TRUE,  TRUE  },
        { (VpEventCategoryType)0, 0, "", TRUE, TRUE }
    };
    const VpEventInfo *pEventInfo = eventInfoTable;
    char eventDataStr[16] = "";
    char parmHandleStr[16] = "";
    char eventNameStr[32] = "";
    bool lineSpecific = (pEvent->pLineCtx != VP_NULL);

    if (pEvent->status != VP_STATUS_SUCCESS) {
        VP_DOUT_(EVENT, VP_DBG_EVENT_COLOR, VpDevCtxType, pEvent->pDevCtx, ("status = %s", VpGetString_VpStatusType(pEvent->status)));
        return;
    }

    /* Find the event in the table. */
    while (
        (pEventInfo->id != 0) &&
        ((pEventInfo->category != pEvent->eventCategory) || (pEventInfo->id != pEvent->eventId))
    ) {
        pEventInfo++;
    }

    /* Build display strings. */
    if (pEventInfo->id == 0) {
        sprintf(eventNameStr, "cat=0x%4.4X id=0x%4.4X", pEvent->eventCategory, pEvent->eventId);
    } else {
        sprintf(eventNameStr, "id=%s", pEventInfo->nameStr);
    }
    if (pEventInfo->hasEventData) {
        sprintf(eventDataStr, " data=0x%4.4X", pEvent->eventData);
    }
    if (pEventInfo->hasParmHandle) {
        sprintf(parmHandleStr, " parm=0x%4.4X", pEvent->parmHandle);
    }

#define PRINTF_VPEVENT \
    ("%s%s%s%s", eventNameStr, eventDataStr, parmHandleStr, (pEvent->hasResults ? " +results" : ""))

    if (lineSpecific) {
        VP_DOUT_(EVENT, VP_DBG_EVENT_COLOR, VpLineCtxType, pEvent->pLineCtx, PRINTF_VPEVENT);
    } else {
        VP_DOUT_(EVENT, VP_DBG_EVENT_COLOR, VpDevCtxType, pEvent->pDevCtx, PRINTF_VPEVENT);
    }
}

void VpDisplayResults(
    VpEventType *pEvent,
    void *pResult)
{
    char outbuf[800];
    char *pOut = outbuf + sprintf(outbuf, "results={");
    bool lineSpecific = (pEvent->pLineCtx != VP_NULL);
    VpEventCategoryType cat = pEvent->eventCategory;
    uint16 id = pEvent->eventId;

    if (!pEvent->hasResults) {
        return;
    }

    if (
        ((cat == VP_EVCAT_RESPONSE) && (id == VP_DEV_EVID_BOOT_CMP)) ||
        ((cat == VP_EVCAT_TEST) && (id == VP_DEV_EVID_CHKSUM))
    ) {
        VpChkSumType *pChkSum = (VpChkSumType *)pResult;
        pOut += sprintf(pOut, "loadChecksum=0x%8.8X ", (unsigned)pChkSum->loadChecksum);
        pOut += sprintf(pOut, "vInfo={");
        pOut += sprintf(pOut, "vtdRevCode=0x%4.4X ",   pChkSum->vInfo.vtdRevCode);
        pOut += sprintf(pOut, "swProductId=0x%2.2X ",  pChkSum->vInfo.swProductId);
        pOut += sprintf(pOut, "swVerMajor=0x%2.2X ",   pChkSum->vInfo.swVerMajor);
        pOut += sprintf(pOut, "swVerMinor=0x%2.2X}",   pChkSum->vInfo.swVerMinor);
    } else if ((cat == VP_EVCAT_TEST) && (id == VP_LINE_EVID_TEST_CMP)) {
        /* To do */
        pOut += sprintf(pOut, "(test result not displayed)");
    } else if (cat == VP_EVCAT_RESPONSE) {
        switch (pEvent->eventId) {
            case VP_LINE_EVID_LLCMD_RX_CMP: {
                uint8 *pData = (uint8 *)pResult;
                uint16 bytes = pEvent->eventData;
                while (bytes--) {
                    pOut += sprintf(pOut, " %2.2X", *(pData++));
                }
                break;
            }
            case VP_LINE_EVID_RD_OPTION:
                /* To do */
                pOut += sprintf(pOut, "(option value not displayed)");
                break;
            case VP_LINE_EVID_RD_LOOP: {
                VpLoopCondResultsType *pLoopCond = (VpLoopCondResultsType *)pResult;
                pOut += sprintf(pOut, "rloop=%d ",       pLoopCond->rloop);
                pOut += sprintf(pOut, "ilg=%d ",         pLoopCond->ilg);
                pOut += sprintf(pOut, "imt=%d ",         pLoopCond->imt);
                pOut += sprintf(pOut, "vbat1=%d ",       pLoopCond->vbat1);
                pOut += sprintf(pOut, "vsab=%d ",        pLoopCond->vsab);
                pOut += sprintf(pOut, "vbat2=%d ",       pLoopCond->vbat2);
                pOut += sprintf(pOut, "mspl=%d ",        pLoopCond->mspl);
                pOut += sprintf(pOut, "vbat3=%d ",       pLoopCond->vbat3);
                pOut += sprintf(pOut, "selectedBat=%d ", pLoopCond->selectedBat);
                pOut += sprintf(pOut, "dcFeedReg=%d",    pLoopCond->dcFeedReg);
                break;
            }
            case VP_LINE_EVID_GAIN_CMP: {
                VpRelGainResultsType *pRelGain = (VpRelGainResultsType *)pResult;
                pOut += sprintf(pOut, "gResult=%d ",    pRelGain->gResult);
                pOut += sprintf(pOut, "gxValue=%4.4X ", pRelGain->gxValue);
                pOut += sprintf(pOut, "grValue=%4.4X",  pRelGain->grValue);
                break;
            }
            case VP_DEV_EVID_IO_ACCESS_CMP:
                /* We can't distinguish between VpDeviceIoAccessDataType and
                   VpDeviceIoAccessExtType here. */
                pOut += sprintf(pOut, "(cannot display result)");
                break;
            case VP_LINE_EVID_LINE_IO_RD_CMP: {
                VpLineIoAccessType *pLineIoAccess = (VpLineIoAccessType *)pResult;
                pOut += sprintf(pOut, "direction=%d ", pLineIoAccess->direction);
                pOut += sprintf(pOut, "ioBits={");
                pOut += sprintf(pOut, "mask=%2.2X ", pLineIoAccess->ioBits.mask);
                pOut += sprintf(pOut, "data=%2.2X}", pLineIoAccess->ioBits.data);
                break;
            }
            default:
                VP_ERROR(VpDevCtxType, pEvent->pDevCtx, ("Unknown event results type"));
                return;
        }
    }

    sprintf(pOut, "}");
    if (lineSpecific) {
        VP_DOUT_(EVENT, VP_DBG_EVENT_COLOR, VpLineCtxType, pEvent->pLineCtx, ("%s", outbuf));
    } else {
        VP_DOUT_(EVENT, VP_DBG_EVENT_COLOR, VpDevCtxType, pEvent->pDevCtx, ("%s", outbuf));
    }
}

#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_EVENT) */

#if ((VP_CC_DEBUG_SELECT & VP_DBG_HAL) && (defined(VP_CC_VCP2_SERIES) || defined(VP_CC_VCP_SERIES) || defined(VP_CC_792_SERIES)))
void
VpDisplayHbiAccess(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    unsigned bufLen,
    uint16 *buf)
{
    VP_HAL(VpDeviceIdType, &deviceId, ("%u-word opaque HBI block {", bufLen));

    while (bufLen > 0) {
        uint16 cmd = *buf++;
        unsigned dataLength = VpDisplayHbiCmd(deviceType, deviceId, cmd);
        bufLen--;
        if (dataLength > 0) {
            if (dataLength > bufLen) {
                VP_ERROR(VpDeviceIdType, &deviceId, ("Not enough data for HAL Access!"));
                dataLength = bufLen;
            }
            VpDisplayHbiData(deviceId, dataLength, buf);
            buf += dataLength;
            bufLen -= dataLength;
        }
    }

    VP_HAL(VpDeviceIdType, &deviceId, ("} end of block"));
}

#define HBI_WORDS_PER_LINE 8
#define HBI_WORD_FILLER "---- "
#define HBI_COMMAND_LINE_FILLER " = "

unsigned
VpDisplayHbiCmd(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    uint16 cmd)
{
    char output[128];
    unsigned dataLength = 0;

    switch (deviceType) {

#if (defined(VP_CC_VCP2_SERIES) || defined(VP_CC_VCP_SERIES))
        case VP_DEV_VCP_SERIES:
        case VP_DEV_VCP2_SERIES:
            if (cmd == 0xFFFF) {
                sprintf(output, "%4.4X%s%s", (int)cmd, HBI_COMMAND_LINE_FILLER, "No Op");
            } else if ((cmd & 0xFF00) == 0xFE00) {
                sprintf(output, "%4.4X%s%s 0x%2.2X", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Select Page", (int)(cmd & 0xFF));
            } else if ((cmd & 0xFF00) == (0xFD00)) {
                sprintf(output, "%4.4X%s%s 0x%2.2X", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Config Interface", (int)(cmd & 0xFF));
            } else if ((cmd & 0xFE00) == (0xFA00)) {
                dataLength = (cmd & 0xFF) + 1;
                sprintf(output, "%4.4X%s%s (%u word%s)", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Continue Paged Access", dataLength, (dataLength > 1 ? "s" : ""));
            } else if ((cmd & 0xFE00) == (0xF800)) {
                dataLength = (cmd & 0xFF) + 1;
                sprintf(output, "%4.4X%s%s (%u word%s)", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Start Paged Access", dataLength, (dataLength > 1 ? "s" : ""));
            } else {
                unsigned offset = (cmd & 0x7F00) >> 8;
                bool direct = ((cmd & 0x8000) != 0);
                bool write = ((cmd & 0x0080) != 0);
                dataLength = (cmd & 0x007F) + 1;
                sprintf(output, "%4.4X%s%s Offset %s (%u word%s at offset 0x%2.2X)", (int)cmd,
                    HBI_COMMAND_LINE_FILLER, (direct ? "Direct" : "Paged"),
                    (write ? "Write" : "Read"), dataLength, (dataLength > 1 ? "s" : ""),
                    offset);
            }
            break;
#endif /* (defined(VP_CC_VCP2_SERIES) || defined(VP_CC_VCP_SERIES)) */

#ifdef VP_CC_792_SERIES
        case VP_DEV_792_SERIES:
            if (cmd == 0xFFFF) {
                sprintf(output, "%4.4X%s         %s", (int)cmd, HBI_COMMAND_LINE_FILLER, "No Op");
            } else if (cmd == 0xF701) {
                dataLength = 2;
                sprintf(output, "%4.4X%s         %s", (int)cmd, HBI_COMMAND_LINE_FILLER, "Read Shared Interrupt Regs");
            } else if ((cmd & 0xFF00) == (0xFD00)) {
                sprintf(output, "%4.4X%s         %s 0x%2.2X", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Config Interface", (int)(cmd & 0xFF));
            } else if ((cmd & 0xFF00) == 0xFE00) {
                if ((cmd & 0x00FF) == 0x00FF) {
                    sprintf(output, "%4.4X%s         %s", (int)cmd, HBI_COMMAND_LINE_FILLER,
                        "Code Load Page Broadcast");
                } else if ((cmd & 0x008F) == 0x0080) {
                    unsigned slacId = (cmd & 0x0070) >> 4;
                    sprintf(output, "%4.4X%s(slac %u) %s", (int)cmd, HBI_COMMAND_LINE_FILLER, slacId,
                        "Select Code Load Page");
                } else if ((cmd & 0x0080) == 0x0000) {
                    unsigned slacId = (cmd & 0x0070) >> 4;
                    unsigned page = cmd & 0x000F;
                    sprintf(output, "%4.4X%s(slac %u) %s %u", (int)cmd, HBI_COMMAND_LINE_FILLER, slacId,
                        "Select Page", page);
                } else {
                    sprintf(output, "%4.4X%s%s", (int)cmd, HBI_COMMAND_LINE_FILLER,
                        "INVALID COMMAND WORD!");
                }
            } else if ((cmd & 0xFF00) == (0xFD00)) {
                sprintf(output, "%4.4X%s         %s 0x%2.2X", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Config Interface", (int)(cmd & 0xFF));
            } else if ((cmd & 0xFF00) == (0xFC00)) {
                unsigned offset = (cmd & 0x00F0) >> 4;
                dataLength = (cmd & 0x000F) + 1;
                sprintf(output, "%4.4X%s         %s (%u word%s at offset 0x%2.2X)", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Direct Page Broadcast", dataLength, (dataLength > 1 ? "s" : ""), offset);
            } else if ((cmd & 0xFE00) == (0xFA00)) {
                dataLength = (cmd & 0xFF) + 1;
                sprintf(output, "%4.4X%s         %s (%u word%s)", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Continue Mailbox Access", dataLength, (dataLength > 1 ? "s" : ""));
            } else if ((cmd & 0xFE00) == (0xF800)) {
                dataLength = (cmd & 0xFF) + 1;
                sprintf(output, "%4.4X%s         %s (%u word%s)", (int)cmd, HBI_COMMAND_LINE_FILLER,
                    "Start Mailbox Access", dataLength, (dataLength > 1 ? "s" : ""));
            } else if (cmd & 0x8000) {
                unsigned offset = (cmd & 0x7F00) >> 8;
                bool write = (cmd & 0x0080);
                unsigned slacId = (cmd & 0x0070) >> 4;
                dataLength = (cmd & 0x000F) + 1;
                sprintf(output, "%4.4X%s(slac %u) %s %s (%u word%s at offset 0x%2.2X)",
                    (int)cmd, HBI_COMMAND_LINE_FILLER, slacId,
                    "Direct Page", (write ? "Write" : "Read"), dataLength,
                    (dataLength > 1 ? "s" : ""), offset);
            } else {
                unsigned offset = (cmd & 0x7F00) >> 8;
                bool write = ((cmd & 0x0080) != 0);
                dataLength = (cmd & 0x007F) + 1;
                sprintf(output, "%4.4X%s         Paged %s (%u word%s at offset 0x%2.2X)", (int)cmd,
                    HBI_COMMAND_LINE_FILLER, (write ? "Write" : "Read"), dataLength,
                    (dataLength > 1 ? "s" : ""), offset);
            }
            break;
#endif /* VP_CC_792_SERIES */

        default:
            sprintf(output, "%4.4X (cmd)", (int)cmd);
    }

    VP_HAL(VpDeviceIdType, &deviceId, ("%s", output));
    return dataLength;
}

void
VpDisplayHbiData(
    VpDeviceIdType deviceId,
    unsigned bufLen,
    uint16 *buf)
{
    char output[128] = "       ";
    char *pOut = &output[7];
    unsigned disp = 0;

    if (buf == VP_NULL) {
        VP_HAL(VpDeviceIdType, &deviceId, ("       (%u words of zeroes)", bufLen));
        return;
    }

    while (bufLen--) {
        pOut += sprintf(pOut, "%4.4X ", *buf++);
        disp++;
        if ((bufLen == 0) || ((disp % HBI_WORDS_PER_LINE) == 0)) {
            VP_HAL(VpDeviceIdType, &deviceId, ("%s", output));
            pOut = &output[7];
        }
    }
}

bool
VpHalHbiReadWrapper(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint16p data)
{
    bool status = VpHalHbiRead(deviceId, cmd, numwords, data);
    VpDisplayHbiCmd(deviceType, deviceId, cmd);
    VpDisplayHbiData(deviceId, numwords + 1, data);
    return status;
}

#endif /* ((VP_CC_DEBUG_SELECT & VP_DBG_HAL) && (defined(VP_CC_VCP2_SERIES) || defined(VP_CC_VCP_SERIES))) */

#if (VP_CC_DEBUG_SELECT & VP_DBG_SSL)

const char *
VpGetString_VpCriticalSecType(
    VpCriticalSecType criticalSecType)
{
    static const char *strTable[VP_NUM_CRITICAL_SEC_TYPES] = {
        "VP_MPI_CRITICAL_SEC",
        "VP_HBI_CRITICAL_SEC",
        "VP_CODE_CRITICAL_SEC"
    };

    VP_ASSERT(VP_NUM_CRITICAL_SEC_TYPES == VP_CODE_CRITICAL_SEC + 1);

    return VpGetEnumString(strTable, VP_NUM_CRITICAL_SEC_TYPES, (int)criticalSecType);
}

uint8
VpSysEnterCriticalWrapper(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
    uint8 depth = VpSysEnterCritical(deviceId, criticalSecType);
    VP_SSL(VpDeviceIdType, &(deviceId), ("VpSysEnterCritical(%s) = %u", VpGetString_VpCriticalSecType(criticalSecType), (unsigned)depth));
    return depth;
}

uint8
VpSysExitCriticalWrapper(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType)
{
    uint8 depth = VpSysExitCritical(deviceId, criticalSecType);
    VP_SSL(VpDeviceIdType, &(deviceId), ("VpSysExitCritical(%s) = %u", VpGetString_VpCriticalSecType(criticalSecType), (unsigned)depth));
    return depth;
}

#endif /* (VP_CC_DEBUG_SELECT & VP_DBG_SSL) */

#endif /* VP_DEBUG */
