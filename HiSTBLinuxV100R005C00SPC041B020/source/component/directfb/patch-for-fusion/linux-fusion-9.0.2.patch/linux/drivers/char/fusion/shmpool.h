/*
   (c) Copyright 2002-2011  The world wide DirectFB Open Source Community (directfb.org)
   (c) Copyright 2002-2004  Convergence (integrated media) GmbH

   All rights reserved.

   Written by Denis Oliver Kropp <dok@directfb.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version
   2 of the License, or (at your option) any later version.
*/

#ifndef __FUSION__SHMPOOL_H__
#define __FUSION__SHMPOOL_H__

#include <asm/bitsperlong.h>

#ifndef FUSION_SHM_PER_WORLD_SPACE
#define FUSION_SHM_PER_WORLD_SPACE (__BITS_PER_LONG == 64)
#endif


#include "fusiondev.h"
#include "types.h"

#if defined(__mips__)
#define FUSION_SHM_BASE_32	0x50010000	/* virtual base address */
#elif defined(CONFIG_CPU_SH4)
#define FUSION_SHM_BASE_32	0x30000000	/* virtual base address */
#else
#define FUSION_SHM_BASE_32	0x20000000	/* virtual base address */
#endif
#define FUSION_SHM_SIZE_32	0x20000000	/* size of virtual address space */

#define FUSION_SHM_BASE_64	0x6f00000000//0x523000000000LL	/* virtual base address */
#define FUSION_SHM_SIZE_64	0x001000000000LL	/* size of virtual address space */

#ifndef FUSION_SHM_BASE_DEFAULT
#define FUSION_SHM_BASE_DEFAULT (__BITS_PER_LONG == 64 ? (FUSION_SHM_BASE_64) : (FUSION_SHM_BASE_32))
#endif

#ifndef FUSION_SHM_SIZE_DEFAULT
#define FUSION_SHM_SIZE_DEFAULT (__BITS_PER_LONG == 64 ? (FUSION_SHM_SIZE_64) : (FUSION_SHM_SIZE_32))
#endif


/* module init/cleanup */

int fusion_shmpool_init(FusionDev * dev);
void fusion_shmpool_deinit(FusionDev * dev);

/* public API */

int fusion_shmpool_new(FusionDev * dev, Fusionee *fusionee, FusionSHMPoolNew * pool);

int fusion_shmpool_attach(FusionDev * dev,
                          FusionSHMPoolAttach * attach, FusionID fusion_id);

int fusion_shmpool_detach(FusionDev * dev, int id, FusionID fusion_id);

int fusion_shmpool_dispatch(FusionDev * dev,
                            FusionSHMPoolDispatch * dispatch,
                            Fusionee * fusionee);

int fusion_shmpool_destroy(FusionDev * dev, int id);

/* internal functions */

void fusion_shmpool_detach_all(FusionDev * dev, FusionID fusion_id);

int fusion_shmpool_fork_all(FusionDev * dev,
                            FusionID fusion_id, FusionID from_id);

#ifdef FUSION_CORE_SHMPOOLS
int fusion_shmpool_map(FusionDev *dev, struct vm_area_struct *vma);
#endif

#endif
