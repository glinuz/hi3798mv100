/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv2bO4a86Ra1cP5w/R608AW9MyvBFsCSDo8GzQMC4Jhtu/zShyENpjFzntvB+
qu9VLVfiymwG7fnplCA6Fl/SkNk8QCfaDWEYQFuh1otobaWBGw+QwNBpSIu3GlY73mvsy+2E
KyET5RtZivvqGs/qTHlFhta7FysPmmPf6kbIUYnweu8H+9XOrW6Mp/Tdvf9fTw==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/* ./edmac.h
 *
 * History:
 *
 */


#ifndef __DSP_ELF_H__
#define __DSP_ELF_H__

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/ioport.h>
#include <linux/string.h>
#include <linux/dma-mapping.h>
#include <linux/kthread.h>
#include "hi_type.h"

#define ELF_PRINT PRIN_NULL

HI_S32 CopyELFSection(HI_UCHAR *pElfAddr);
HI_S32 CheckELFPaser(HI_UCHAR* pElfAddr);
HI_S32 ResetELFSection(HI_U32 u32BaseAddr, HI_U32 u32Size);

#endif

