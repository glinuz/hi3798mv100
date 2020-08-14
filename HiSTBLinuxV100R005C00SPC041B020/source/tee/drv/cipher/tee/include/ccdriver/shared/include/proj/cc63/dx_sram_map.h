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

/*!
 * \file dx_sram_map.h
 * sram mapping definition.
 */
#ifndef _DX_SRAM_MAP_H_
#define _DX_SRAM_MAP_H_

#define DX_SRAM_PKA_BASE_ADDRESS                                0x0
#define DX_SRAM_PKA_MAX_SIZE                                    0x1000    /* first 4KB in SRAM reserved for PKA operations */

#define DX_SRAM_RND_HW_DMA_ADDRESS                              0x1800
#define DX_SRAM_RND_MAX_SIZE                                    0x800    /* addresses 6K-8K in SRAM reserved for RND operations */

#define DX_SRAM_MLLI_BASE_ADDR                                  0x2000
#define DX_SRAM_MLLI_MAX_SIZE                                   0x800    /* addresses 8K-10K in SRAM reserved for MLLI tables */

#define DX_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_ADDR               0x3C00
#define DX_SRAM_DRIVER_ADAPTOR_CONTEXT_CACHE_LAST_WORD_ADDR     0x3FFC
#define DX_SRAM_DRIVER_ADAPTOR_CONTEXT_MAX_SIZE                 0x400    /* last 1K, addresses 15K-16K, in SRAM reserved for driver adaptor context */

#endif /*_DX_SRAM_MAP_H_*/
