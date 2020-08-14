/** \file mpi_hal.h
 * mpi_hal.h
 *
 * Header file for the VP-API-II c files requiring MPI interface.
 *
 * Copyright (c) 2008, Zarlink Semiconductor, Inc.
 */
#ifndef MPI_UVB_HAL_H
#define MPI_UVB_HAL_H
#include "vp_api_types.h"

EXTERN void
VpMpiCmd(
    VpDeviceIdType deviceId,
    uint8 ecVal,
    uint8 cmd,
    uint8 cmdLen,
    uint8 *dataPtr);
#endif

