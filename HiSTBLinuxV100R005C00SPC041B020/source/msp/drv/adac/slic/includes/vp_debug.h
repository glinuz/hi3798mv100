/** \file vp_debug.h
 * vp_debug.h
 *
 * This file contains the configuration and compile time settings for
 * the VoicePath API debug statements.
 *
 * Copyright (c) 2010, Zarlink Semiconductor, Inc.
 *
 * $Revision: 7343 $
 * $LastChangedDate: 2010-06-14 18:16:12 -0500 (Mon, 14 Jun 2010) $
 *
 * As of svn 5835, no part of the VP-API-II uses the old VP_DOUT method.
 */

#ifndef VP_DEBUG_H
#define VP_DEBUG_H

#include "vp_api_cfg.h"
#include "vp_api_types.h"
#include "sys_service.h"

/*
 * These are not documented in the API-II and should be used for internal
 * purposes only. Reserved range (0x00010000 to 0x80000000)
 */
#define DBG_EVENT_VERBOSE   0x00010000 /* Verbose event debug (792) */
#define DBG_BOOT_WRITE      0x00020000 /* Displays all boot blocks */

/* Define VP_DEBUG_COLOR to display ANSI X3.64 color codes in debug output. */
#define VP_DEBUG_COLOR
#include "vp_debug_colors.h"

/* Colors for debug messages (if VP_DEBUG_COLOR is defined): */
#define VP_DBG_ERROR_COLOR          bright_red
#define VP_DBG_WARNING_COLOR        dark_yellow
#define VP_DBG_INFO_COLOR           dark_blue
#define VP_DBG_API_FUNC_COLOR       bright_cyan
#define VP_DBG_API_FUNC_INT_COLOR   dark_cyan
#define VP_DBG_HAL_COLOR            bright_white
#define VP_DBG_SSL_COLOR            dark_green
#define VP_DBG_EVENT_COLOR          bright_blue
#define VP_DBG_CALIBRATION_COLOR    bright_white
#define VP_DBG_LINE_STATE_COLOR     dark_yellow
#define VP_DBG_HOOK_COLOR           bright_cyan
#define VP_DBG_TEST_COLOR           bright_blue
#define VP_DBG_TEST_FUNC_COLOR      bright_yellow
#define VP_DBG_FXO_FUNC_COLOR       dark_green
#define VP_DBG_SEQUENCER_COLOR      dark_blue
#define VP_DBG_CID_COLOR            bright_white
#define VP_DBG_TEST_PCM_COLOR       bright_cyan

/* Define VP_DEBUG_LOC to include the location (file and line) in each debug message. */
#undef VP_DEBUG_LOC

#ifdef VP_DEBUG_LOC
#define VP_SHOW_LOC() VpSysDebugPrintf(" (" __FILE__ ":%d)", __LINE__)
#else
#define VP_SHOW_LOC()
#endif

/* Basic debug output macro: */
#define VP_DOUT_(msgType, msgColor, objType, pObj, printf_args) \
    if (VpDebugEnabled_ ## objType (VP_DBG_ ## msgType, color_fg(msgColor) #msgType, pObj)) { \
        VpSysDebugPrintf printf_args; \
        VP_SHOW_LOC(); \
        VpSysDebugPrintf(color_reset "\n"); \
    }

/* Conditionally compile in various types of debug messages: */
#if !defined(VP_DEBUG) && defined(VP_CC_DEBUG_SELECT)
#undef VP_CC_DEBUG_SELECT
#endif
#ifndef VP_CC_DEBUG_SELECT
#define VP_CC_DEBUG_SELECT 0
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_ERROR)
#define VP_ERROR(objType, pObj, printf_args) VP_DOUT_(ERROR, VP_DBG_ERROR_COLOR, objType, pObj, printf_args)
#else
#define VP_ERROR(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_WARNING)
#define VP_WARNING(objType, pObj, printf_args) VP_DOUT_(WARNING, VP_DBG_WARNING_COLOR, objType, pObj, printf_args)
#else
#define VP_WARNING(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_INFO)
#define VP_INFO(objType, pObj, printf_args) VP_DOUT_(INFO, VP_DBG_INFO_COLOR, objType, pObj, printf_args)
#else
#define VP_INFO(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC)
#define VP_API_FUNC(objType, pObj, printf_args) VP_DOUT_(API_FUNC, VP_DBG_API_FUNC_COLOR, objType, pObj, printf_args)
#else
#define VP_API_FUNC(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_API_FUNC_INT)
#define VP_API_FUNC_INT(objType, pObj, printf_args) VP_DOUT_(API_FUNC_INT, VP_DBG_API_FUNC_INT_COLOR, objType, pObj, printf_args)
#else
#define VP_API_FUNC_INT(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_CALIBRATION)
#define VP_CALIBRATION(objType, pObj, printf_args) VP_DOUT_(CALIBRATION, VP_DBG_CALIBRATION_COLOR, objType, pObj, printf_args)
#else
#define VP_CALIBRATION(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_LINE_STATE)
#define VP_LINE_STATE(objType, pObj, printf_args) VP_DOUT_(LINE_STATE, VP_DBG_LINE_STATE_COLOR, objType, pObj, printf_args)
#else
#define VP_LINE_STATE(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_HOOK)
#define VP_HOOK(objType, pObj, printf_args) VP_DOUT_(HOOK, VP_DBG_HOOK_COLOR, objType, pObj, printf_args)
#else
#define VP_HOOK(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_TEST)
#define VP_TEST(objType, pObj, printf_args) VP_DOUT_(TEST, VP_DBG_TEST_COLOR, objType, pObj, printf_args)
#else
#define VP_TEST(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_TEST_FUNC)
#define VP_TEST_FUNC(objType, pObj, printf_args) VP_DOUT_(TEST_FUNC, VP_DBG_TEST_FUNC_COLOR, objType, pObj, printf_args)
#else
#define VP_TEST_FUNC(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_FXO_FUNC)
#define VP_FXO_FUNC(objType, pObj, printf_args) VP_DOUT_(FXO_FUNC, VP_DBG_FXO_FUNC_COLOR, objType, pObj, printf_args)
#else
#define VP_FXO_FUNC(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_SEQUENCER)
#define VP_SEQUENCER(objType, pObj, printf_args) VP_DOUT_(SEQUENCER, VP_DBG_SEQUENCER_COLOR, objType, pObj, printf_args)
#else
#define VP_SEQUENCER(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_CID)
#define VP_CID(objType, pObj, printf_args) VP_DOUT_(CID, VP_DBG_CID_COLOR, objType, pObj, printf_args)
#else
#define VP_CID(objType, pObj, printf_args)
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_TEST_PCM)
#define VP_TEST_PCM(objType, pObj, printf_args) VP_DOUT_(TEST_PCM, VP_DBG_TEST_PCM_COLOR, objType, pObj, printf_args)
#else
#define VP_TEST_PCM(objType, pObj, printf_args)
#endif

#if (VP_CC_DEBUG_SELECT & VP_DBG_SSL)
#define VP_SSL(objType, pObj, printf_args) VP_DOUT_(SSL, VP_DBG_SSL_COLOR, objType, pObj, printf_args)
#define VP_SYS_ENTER_CRITICAL(deviceId, criticalSecType) VpSysEnterCriticalWrapper(deviceId, criticalSecType)
#define VP_SYS_EXIT_CRITICAL(deviceId, criticalSecType) VpSysExitCriticalWrapper(deviceId, criticalSecType)
#define VP_SYS_WAIT(time) \
    (VP_SSL(None, VP_NULL, ("VpSysWait(%u frames = %u ms)", (time), ((time) + 7 / 8))), VpSysWait(time))
#else
#define VP_SSL(objType, pObj, printf_args)
#define VP_SYS_ENTER_CRITICAL(deviceId, criticalSecType) VpSysEnterCritical(deviceId, criticalSecType)
#define VP_SYS_EXIT_CRITICAL(deviceId, criticalSecType) VpSysExitCritical(deviceId, criticalSecType)
#define VP_SYS_WAIT(time) VpSysWait(time)
#endif

#ifndef VP_HAL_DEVICE_TYPE
#define VP_HAL_DEVICE_TYPE VP_DEV_VCP_SERIES
#endif
#if (VP_CC_DEBUG_SELECT & VP_DBG_HAL)
#define VP_HAL(objType, pObj, printf_args) VP_DOUT_(HAL, VP_DBG_HAL_COLOR, objType, pObj, printf_args)
#define VP_HAL_HBI_CMD(deviceId, cmd) \
    (VpDisplayHbiCmd(VP_HAL_DEVICE_TYPE, deviceId, cmd), \
    VpHalHbiCmd(deviceId, cmd))
#define VP_HAL_HBI_WRITE(deviceId, cmd, numwords, data) \
    (VpDisplayHbiCmd(VP_HAL_DEVICE_TYPE, deviceId, cmd), \
    VpDisplayHbiData(deviceId, numwords + 1, data), \
    VpHalHbiWrite(deviceId, cmd, numwords, data))
#define VP_HAL_HBI_READ(deviceId, cmd, numwords, data) \
    VpHalHbiReadWrapper(VP_HAL_DEVICE_TYPE, deviceId, cmd, numwords, data)
#if (VP_CC_DEBUG_SELECT & DBG_BOOT_WRITE)
#define VP_HAL_HBI_BOOT_WR(deviceId, numWords, data) \
    (VpDisplayHbiAccess(VP_HAL_DEVICE_TYPE, deviceId, numWords + 1, (uint16p)(data)), \
    VpHalHbiBootWr(deviceId, numWords, data))
#else
#define VP_HAL_HBI_BOOT_WR(deviceId, numWords, data) \
    VpHalHbiBootWr(deviceId, numWords, data)
#endif
#else
#define VP_HAL_HBI_CMD(deviceId, cmd) \
    VpHalHbiCmd(deviceId, cmd)
#define VP_HAL_HBI_WRITE(deviceId, cmd, numwords, data) \
    VpHalHbiWrite(deviceId, cmd, numwords, data)
#define VP_HAL_HBI_READ(deviceId, cmd, numwords, data) \
    VpHalHbiRead(deviceId, cmd, numwords, data)
#define VP_HAL_HBI_BOOT_WR(deviceId, numWords, data) \
    VpHalHbiBootWr(deviceId, numWords, data)
#endif

/* Basic assert macro: */
#define VP_ASSERT(condition) \
    if (!(condition)) { \
        VP_ERROR(None, VP_NULL, ("Assertion failed: %s\n", #condition)); \
        /* exit(-1); */ \
    }

#define VP_DOUT(msgType, printf_args)

/* Global debug select mask (for messages that are not specific to a particular
   line or device): */
EXTERN uint32 vpDebugSelectMask;

#ifdef VP_CC_VCP2_SERIES
EXTERN bool
VpDebugEnabled_VpVcp2DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_VpVcp2LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    VpVcp2LineObjectType *pLineObj);
#endif

#ifdef VP_CC_890_SERIES
EXTERN bool
VpDebugEnabled_Vp890DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_Vp890LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp890LineObjectType *pLineObj);
#endif

#ifdef VP_CC_880_SERIES
EXTERN bool
VpDebugEnabled_Vp880DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp880DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_Vp880LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp880LineObjectType *pLineObj);
#endif

#ifdef VP_CC_580_SERIES
EXTERN bool
VpDebugEnabled_Vp580DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp580DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_Vp580LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp580LineObjectType *pLineObj);
#endif

#ifdef VP_CC_790_SERIES
EXTERN bool
VpDebugEnabled_Vp790DeviceObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp790DeviceObjectType *pDevObj);

EXTERN bool
VpDebugEnabled_Vp790LineObjectType(
    uint32 msgType,
    char *msgTypeStr,
    Vp790LineObjectType *pLineObj);
#endif

EXTERN bool
VpDebugEnabled_VpDeviceIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpDeviceIdType *pDeviceId);

EXTERN bool
VpDebugEnabled_VpLineIdType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineIdType *pLineId);

EXTERN bool
VpDebugEnabled_VpDevCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpDevCtxType *pDevCtx);

EXTERN bool
VpDebugEnabled_VpLineCtxType(
    uint32 msgType,
    char *msgTypeStr,
    VpLineCtxType *pLineCtx);

EXTERN bool
VpDebugEnabled_None(
    uint32 msgType,
    char *msgTypeStr,
    void *nothing);

EXTERN const char *
VpGetString_VpStatusType(
    VpStatusType status);

EXTERN const char *
    VpGetString_VpOptionIdType(
    VpOptionIdType status);

EXTERN void
VpDisplayEvent(
    VpEventType *pEvent);

EXTERN void
VpDisplayResults(
    VpEventType *pEvent,
    void *pResult);

EXTERN void
VpDisplayHbiAccess(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    unsigned bufLen,
    uint16 *buf);

EXTERN unsigned
VpDisplayHbiCmd(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    uint16 cmd);

EXTERN void
VpDisplayHbiData(
    VpDeviceIdType deviceId,
    unsigned bufLen,
    uint16 *buf);

EXTERN bool
VpHalHbiReadWrapper(
    VpDeviceType deviceType,
    VpDeviceIdType deviceId,
    uint16 cmd,
    uint8 numwords,
    uint16 *data);

#if (VP_CC_DEBUG_SELECT & VP_DBG_SSL)
EXTERN const char *
VpGetString_VpCriticalSecType(
    VpCriticalSecType criticalSecType);

EXTERN uint8
VpSysEnterCriticalWrapper(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType);

EXTERN uint8
VpSysExitCriticalWrapper(
    VpDeviceIdType deviceId,
    VpCriticalSecType criticalSecType);
#endif

/* Derivative debug macros: */
#define VP_API_ENTER(objType, pObj, funcName) \
    VP_API_FUNC(objType, pObj, ("Vp%s():", funcName))
#define VP_API_EXIT(objType, pObj, funcName, status) \
    VP_API_FUNC(objType, pObj, ("Vp%s() = %s", funcName, VpGetString_VpStatusType(status)))
#define VP_API_INT_ENTER(objType, pObj, funcName) \
    VP_API_FUNC_INT(objType, pObj, ("%s():", funcName))
#define VP_API_INT_EXIT(objType, pObj, funcName, status) \
    VP_API_FUNC_INT(objType, pObj, ("%s() = %s", funcName, VpGetString_VpStatusType(status)))

#endif /* VP_DEBUG_H */

