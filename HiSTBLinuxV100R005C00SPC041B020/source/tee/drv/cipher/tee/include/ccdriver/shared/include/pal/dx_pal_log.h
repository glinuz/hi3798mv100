/***************************************************************
*  Copyright 2014 (c) Discretix Technologies Ltd.              *
*  This software is protected by copyright, international      *
*  treaties and various patents. Any copy, reproduction or     *
*  otherwise use of this software must be authorized in a      *
*  license agreement and include this Copyright Notice and any *
*  other notices specified in the license agreement.           *
*  Any redistribution in binary form must be authorized in the *
*  license agreement and include this Copyright Notice and     *
*  any other notices specified in the license agreement and/or *
*  in materials provided with the binary distribution.         *
****************************************************************/

#ifndef _DX_PAL_LOG_H_
#define _DX_PAL_LOG_H_

#include "dx_pal_types.h"
#include "dx_pal_log_plat.h"

/* PAL log levels (to be used in DX_PAL_logLevel) */
#define DX_PAL_LOG_LEVEL_NULL      -1 /* Disable logging */
#define DX_PAL_LOG_LEVEL_ERR       0
#define DX_PAL_LOG_LEVEL_WARN      1
#define DX_PAL_LOG_LEVEL_INFO      2
#define DX_PAL_LOG_LEVEL_DEBUG     3
#define DX_PAL_LOG_LEVEL_TRACE     4
#define DX_PAL_LOG_LEVEL_DATA      5

#ifndef DX_PAL_LOG_CUR_COMPONENT
/* Setting default component mask in case caller did not define */
/* (a mask that is always on for every log mask value but full masking) */
#define DX_PAL_LOG_CUR_COMPONENT 0xFFFFFFFF
#endif
#ifndef DX_PAL_LOG_CUR_COMPONENT_NAME
#define DX_PAL_LOG_CUR_COMPONENT_NAME "Dx"
#endif

/* Select compile time log level (default if not explicitly specified by caller) */
#ifndef DX_PAL_MAX_LOG_LEVEL /* Can be overriden by external definition of this constant */
#ifdef DEBUG
#define DX_PAL_MAX_LOG_LEVEL  DX_PAL_LOG_LEVEL_ERR /*DX_PAL_LOG_LEVEL_DEBUG*/
#else /* Disable logging */
#define DX_PAL_MAX_LOG_LEVEL DX_PAL_LOG_LEVEL_NULL
#endif
#endif /*DX_PAL_MAX_LOG_LEVEL*/
/* Evaluate DX_PAL_MAX_LOG_LEVEL in case provided by caller */
#define __DX_PAL_LOG_LEVEL_EVAL(level) level
#define _DX_PAL_MAX_LOG_LEVEL __DX_PAL_LOG_LEVEL_EVAL(DX_PAL_MAX_LOG_LEVEL)


#ifdef ARM_DSM
#define DX_PAL_LogInit() do {} while (0)
#define DX_PAL_LogLevelSet(setLevel) do {} while (0)
#define DX_PAL_LogMaskSet(setMask) do {} while (0)
#else
#if _DX_PAL_MAX_LOG_LEVEL > DX_PAL_LOG_LEVEL_NULL
void DX_PAL_LogInit(void);
void DX_PAL_LogLevelSet(int setLevel);
void DX_PAL_LogMaskSet(uint32_t setMask);
extern int DX_PAL_logLevel;
extern uint32_t DX_PAL_logMask;
#else /* No log */
static inline void DX_PAL_LogInit(void) {}
static inline void DX_PAL_LogLevelSet(int setLevel) {}
static inline void DX_PAL_LogMaskSet(uint32_t setMask) {}
#endif
#endif

/* Filter logging based on logMask and dispatch to platform specific logging mechanism */
#define _DX_PAL_LOG(level, format, ...)  \
	if (DX_PAL_logMask & DX_PAL_LOG_CUR_COMPONENT) \
		__DX_PAL_LOG_PLAT(DX_PAL_LOG_LEVEL_ ## level, "%s:%s: " format, DX_PAL_LOG_CUR_COMPONENT_NAME, __func__, ##__VA_ARGS__)

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_ERR)
#define DX_PAL_LOG_ERR(format, ...) \
	_DX_PAL_LOG(ERR, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_ERR(format, arg...) do {} while (0)
#endif

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_WARN)
#define DX_PAL_LOG_WARN(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_WARN) \
		_DX_PAL_LOG(WARN, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_WARN(format, arg...) do {} while (0)
#endif

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_INFO)
#define DX_PAL_LOG_INFO(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_INFO) \
		_DX_PAL_LOG(INFO, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_INFO(format, arg...) do {} while (0)
#endif

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_DEBUG)
#define DX_PAL_LOG_DEBUG(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_DEBUG) \
		_DX_PAL_LOG(DEBUG, format, ##__VA_ARGS__)

#define DX_PAL_LOG_DUMP_BUF(msg, buf, size)		\
	do {						\
	int i;						\
	uint8_t	*pData = (uint8_t*)buf;			\
							\
	PRINTF("%s (%d):\n", msg, size);		\
	for (i = 0; i < size; i++) {			\
		PRINTF("0x%02X ", pData[i]);		\
		if ((i & 0xF) == 0xF) {			\
			PRINTF("\n");			\
		}					\
	}						\
	PRINTF("\n");					\
	} while (0)
#else
#define DX_PAL_LOG_DEBUG(format, arg...) do {} while (0)
#define DX_PAL_LOG_DUMP_BUF(msg, buf, size)	do {} while (0)
#endif

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_TRACE)
#define DX_PAL_LOG_TRACE(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_TRACE) \
		_DX_PAL_LOG(TRACE, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_TRACE(format, arg...) do {} while (0)
#endif

#if (_DX_PAL_MAX_LOG_LEVEL >= DX_PAL_LOG_LEVEL_TRACE)
#define DX_PAL_LOG_DATA(format, ...) \
	if (DX_PAL_logLevel >= DX_PAL_LOG_LEVEL_TRACE) \
		_DX_PAL_LOG(DATA, format, ##__VA_ARGS__)
#else
#define DX_PAL_LOG_DATA(format, arg...) do {} while (0)
#endif

#endif /*_DX_PAL_LOG_H_*/
