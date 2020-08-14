/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : aud_dec_alg_type.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/5/16
  Last Modified :
  Description   : Hisi Audio(HA) decoder driver(backend interface)
  Function List :
  History       :
  1.Date        : 2008/05/14
    Author      : Z40717
    Modification: Created file

******************************************************************************/

#ifndef __AUD_OSAL_H__
#define __AUD_OSAL_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "hi_type.h"



#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>      //msleep
#include <asm/uaccess.h>      //copy_from/to_user
#include <linux/pci.h>        //IO_ADDRESS
#include <linux/fs.h>
#include <linux/file.h>

#include "hi_module.h"
#include "hi_drv_mem.h"


#if 0
static HI_VOID *AUDOS_MEM_MALLOC(HI_U32 u32Size)
{
	/* windows platform */
	return (HI_KMALLOC(HI_ID_ADEC, (size_t)u32Size, GFP_KERNEL));
	/* board platform */
	
	
}

static HI_VOID AUDOS_MEM_FREE(HI_VOID *pvMem)
{
	/* windows platform */
	HI_KFREE(HI_ID_ADEC, pvMem);
	/* board platform */
	
}

static HI_VOID *AUDOS_MEM_SET(HI_VOID *pvMem, HI_CHAR ch, HI_U32 u32Size)
{
	/* windows platform */
    return ( memset(pvMem, ch, u32Size) );
	
	/* board platform */
}

static HI_VOID *AUDOS_MEM_CPY(HI_VOID *pvDest, HI_VOID *pvSource, HI_U32 u32Size)
{
	
	/* windows platform */
    return (memcpy(pvDest, pvSource, u32Size));
	
	/* board platform */
	
}

static HI_VOID *AUDOS_MEM_MOV(HI_VOID *pvDest, const HI_VOID *pvSource, HI_U32 u32Size)
{
	/* windows platform */
    return (memmove(pvDest, pvSource, u32Size));
	
	/* board platform */
}


#else

#define AUDOS_MEM_MALLOC(u32Size) \
            (HI_VOID *)(HI_KMALLOC(HI_ID_ADEC, (size_t)u32Size, GFP_KERNEL))

#define AUDOS_MEM_FREE(pvMem) \
            HI_KFREE(HI_ID_ADEC, (HI_VOID *)pvMem)

#define AUDOS_MEM_SET(pvMem, ch, u32Size) \
            (memset((HI_VOID *)pvMem, ch, u32Size))

#define AUDOS_MEM_CPY(pvDest, pvSource, u32Size) \
            (memcpy((HI_VOID *)pvDest, (HI_VOID *)pvSource, u32Size))            

#define AUDOS_MEM_MOV(pvDest, pvSource, u32Size) \
            (memmove((HI_VOID *)pvDest, (const HI_VOID *)pvSource, u32Size))

#define AUDOS_MEM_CMP(pvDest, pvSource, u32Size) \
            (memcmp((HI_VOID *)pvDest, (const HI_VOID *)pvSource, u32Size))            
#endif




#ifdef __cplusplus
}
#endif

#endif  //__AUD_OSAL_H__
