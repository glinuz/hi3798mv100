/*****************Copyright (c) 2010 Hisilicon Tech Co., Ltd.******************
    project: wish
    subprj:  device driver
    module:  audio codec
    file:    slac_private.h
    abstract:Control driver of FXS interface slac chip head file
*******************************************************************************/

#ifndef _INC_SLAC_PRIVATE
#define _INC_SLAC_PRIVATE
//#define SLIC_DEBUG

#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
//#include <linux/config.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/mm.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 36))
#include <linux/smp_lock.h>
#endif
#include <linux/init.h>
#include <asm/uaccess.h>
//#include <asm/hardware.h>
#include <asm/io.h>
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3, 18, 0))
#include <asm/system.h>
#endif
#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/poll.h>
//#include <asm/hardware.h>
#include <asm/bitops.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <linux/moduleparam.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <asm/delay.h>
//#include <linux/devfs_fs_kernel.h>
//#include "vp_api_types.h"
//#include "vp_api_common.h"

#include "hi_drv_struct.h"

#define DEBUG_LEVEL 1                   /*Print Switch*/
/*****************************************************************************************
    Macro definition function: print
    Usage: 1. DPRINTK(0, format(maybe string description), variance or value)
           2. DPRINTK(0, string description)
*****************************************************************************************/
#define DEV_NAME "slac"          /*Chip name, used for printing*/
/********************************** typedef ***********************************/

#define HI_REG(reg) *((volatile unsigned int *)(reg))

/*******************************GPIO macro definition*************************************/
/*GPIO base address definition*/
#define GPIO_BASE(x)    ((x == 5)?(IO_ADDRESS(0x101E4000)) : ((IO_ADDRESS(0x101E6000)) + x*0x1000))
/*GPIO direction set*/
#define _GPIO_SET_DIR_(DIR,GROUP,BIT) do    {    \
    unsigned char result;    \
    result=(*((volatile unsigned char *)(GPIO_BASE(GROUP)+0x400)));    \
    result&=~(1 << BIT);\
    result|=(DIR << BIT);        \
    *((volatile unsigned char *)(GPIO_BASE(GROUP)+0x400)) = result;        \
}while(0)
/*read some bit of GPIO value*/
#define _GPIO_RDBIT_(GROUP,BIT) ((*((volatile unsigned char *)(GPIO_BASE(GROUP)+(0x1<<(BIT+2)))) >> BIT)&1)
/*write some bit of GPIO value*/
#define _GPIO_WRBIT_(VALUE,GROUP,BIT) do {        \
    unsigned char result;        \
    result=(*((volatile unsigned char *)(GPIO_BASE(GROUP)+(0x1<<(BIT+2)))));    \
    result&=~(1<<BIT);    \
    result|=(VALUE<<BIT);    \
    *((volatile unsigned char *)(GPIO_BASE(GROUP)+(0x1<<(BIT+2))))=result;    \
}while(0)

#define VP890_USE_SPI

#ifndef VP890_USE_SPI
/******************************** SPI GPIO and bit**************************/
#define SDO_GPIO_GNUM               1
#define SDO_GPIO_PNUM               1

#define SDI_GPIO_GNUM               1
#define SDI_GPIO_PNUM               2

#define SCK_GPIO_GNUM               1
#define SCK_GPIO_PNUM               0

#define SCS_GPIO_GNUM               1
#define SCS_GPIO_PNUM               3

#define SLIC_SDO_OUT() _GPIO_SET_DIR_(1,SDO_GPIO_GNUM,SDO_GPIO_PNUM)
#define SLIC_SDO_SET() _GPIO_WRBIT_(1,SDO_GPIO_GNUM,SDO_GPIO_PNUM)
#define SLIC_SDO_CLR() _GPIO_WRBIT_(0,SDO_GPIO_GNUM,SDO_GPIO_PNUM)

#define SLIC_SCS_OUT() _GPIO_SET_DIR_(1,SCS_GPIO_GNUM,SCS_GPIO_PNUM)
#define SLIC_SCS_SET() _GPIO_WRBIT_(1,SCS_GPIO_GNUM,SCS_GPIO_PNUM)
#define SLIC_SCS_CLR() _GPIO_WRBIT_(0,SCS_GPIO_GNUM,SCS_GPIO_PNUM)

#define SLIC_SCK_OUT() _GPIO_SET_DIR_(1,SCK_GPIO_GNUM,SCK_GPIO_PNUM)
#define SLIC_SCK_SET() _GPIO_WRBIT_(1,SCK_GPIO_GNUM,SCK_GPIO_PNUM)
#define SLIC_SCK_CLR() _GPIO_WRBIT_(0,SCK_GPIO_GNUM,SCK_GPIO_PNUM)

#define SLIC_SDI_IN() _GPIO_SET_DIR_(0,SDI_GPIO_GNUM,SDI_GPIO_PNUM)
#define SLIC_SDI_READ() _GPIO_RDBIT_(SDI_GPIO_GNUM,SDI_GPIO_PNUM)
#endif

#ifdef VP890_USE_SPI
#if 0
extern void hi_ssp_reset(int set);
extern void hi_ssp_clock(int enable);
extern void hi_ssp_enable(void);
extern void hi_ssp_disable(void);
extern int hi_ssp_set_frameform(unsigned char framemode,unsigned char spo,unsigned char sph,unsigned char datawidth);
extern int hi_ssp_set_serialclock(unsigned char scr,unsigned char cpsdvsr);
extern void hi_ssp_set_bigend(bool bBigEnd);
extern void hi_ssp_set_inturrupt(unsigned char regvalue);
extern void hi_ssp_dmac_disable(void);
extern void hi_ssp_interrupt_clear(void);
extern void hi_ssp_writedata(unsigned short sdata);
extern int hi_ssp_readdata(void);
#else

#define   SPIDEVNUM  0	//use SPI 0
extern HI_S32 HI_DRV_SPI_Open(HI_U8 devId);
extern  HI_S32 HI_DRV_SPI_Close(HI_U8 devId);
extern HI_S32 HI_DRV_SPI_SetFrom(HI_U8 devId,HI_U8 framemode,HI_U8 spo,HI_U8 sph,HI_U8 datawidth);
extern HI_S32 HI_DRV_SPI_WriteQuery(HI_U8 devId,HI_U8 *Send, HI_U32 SendCnt);
extern HI_S32 HI_DRV_SPI_ReadQuery(HI_U8 devId, HI_U8 *Read, HI_U32 ReadCnt);
extern HI_VOID HI_DRV_SPI_SetBlend(HI_U8 devId,HI_U8 bBigEnd);
extern HI_S32 HI_DRV_SPI_SetCfg(HI_BOOL bCfg);
extern HI_VOID HI_DRV_SPI_SetCs_Gpio(HI_U8 devId);
extern HI_VOID HI_DRV_SPI_SetCs_Level(HI_U8 devId,HI_U32 Level);

#endif


#endif
#endif
