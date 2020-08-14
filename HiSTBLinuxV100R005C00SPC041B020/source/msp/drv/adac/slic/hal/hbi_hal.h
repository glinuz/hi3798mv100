/* hbi_hal.h
 *
 * This file defines the interface between the VoicePath API and the Hardware
 * Abstraction Layer (HAL).  The types and functions declared in this file are
 * platform-dependent. Porting the VoicePath API to a new host processor
 * consists of modifying the typedefs in this file, setting the HBI_PINCONFIG
 * define below, and modifying the function implementations in vp_hal.c.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */
#ifndef HBI_HAL_H
#define HBI_HAL_H
#include "vp_api_types.h"
/* Define the EXTERN macro, if necessary */
#ifndef EXTERN
  #ifdef __cplusplus
    #define EXTERN extern "C"
  #else
    #define EXTERN extern
  #endif /* __cplusplus */
#endif /* EXTERN */
/******************************************************************************
 *                          PLATFORM-SPECIFIC OPTIONS                         *
 ******************************************************************************/
/* HBI configuration register */
#define HBI_CFG_ENDLITTLE       0    /* 1=byte swapping for data words only */
#define HBI_CFG_PWAIT_POLHI     0    /* 0=PWAIT pin active low, 1=PWAIT pin active high */
#define HBI_CFG_PWAIT_EN        0    /* 0=PWAIT pin disabled, 1=PWAIT pin enabled */
#define HBI_CFG_PWAIT_MODEOSD   0    /* 0=PWAIT pin TTL, 1=PWAIT pin open source/drain */
#define HBI_CFG_INT_MODETTL     1    /* 0=INT pin open drain, 1=INT pin TTL */
#define HBI_CFG_PINEA           0    /* 0=IO[2:0], 1=EA[10:8] */
#define HBI_PINCONFIG   (HBI_CFG_ENDLITTLE | (HBI_CFG_PWAIT_POLHI << 1) | (HBI_CFG_PWAIT_EN << 2) | (HBI_CFG_PWAIT_MODEOSD << 3) | (HBI_CFG_INT_MODETTL << 4) | (HBI_CFG_PINEA << 5))
#define HBI_CMD_CONFIGURE_INT	0xFD00
#define DEFAULT_DEVICE_ID	0
/******************************************************************************
 *                         PLATFORM-SPECIFIC FUNCTIONS                        *
 ******************************************************************************/
/* See vp_hal.c for descriptions. */
EXTERN bool VpHalHbiInit(VpDeviceIdType deviceId);
EXTERN bool VpHalHbiCmd(VpDeviceIdType deviceId, uint16 cmd);
EXTERN bool VpHalHbiWrite(VpDeviceIdType deviceId, uint16 cmd, uint8 numwords, uint16p data);
EXTERN bool VpHalHbiRead(VpDeviceIdType deviceId, uint16 cmd, uint8 numwords, uint16p data);
EXTERN bool VpHalHbiBootWr(VpDeviceIdType deviceId, uint8 numwords, VpImagePtrType data);
#endif /* HBI_HAL_H */

