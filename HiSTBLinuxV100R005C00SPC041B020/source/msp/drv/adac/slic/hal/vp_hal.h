/* vp_hal.h
 *
 * This file defines the interface between the VoicePath API and the Hardware
 * Abstraction Layer (HAL).  The types and functions declared in this file are
 * platform-dependent.  The functions are defined in ve_hal.c.  Porting the
 * VoicePath API to a new host processor consists of modifying the typedefs
 * in this file, setting the HBI_PINCONFIG define below, and modifying the
 * function implementations in vp_hal.c.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */
#ifndef _VP_HAL_H
#define _VP_HAL_H
#include "vp_api_types.h"
#include "hbi_hal.h"
#include "mpi_hal.h"
#include "sys_service.h"
#endif /* _VP_HAL_H */

