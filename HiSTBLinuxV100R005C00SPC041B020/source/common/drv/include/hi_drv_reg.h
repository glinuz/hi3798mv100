/******************************************************************************
Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : hi_drv_reg.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2010/4/15
Last Modified :
Description   : The common register opration define
Function List :
History       :
******************************************************************************/

#ifndef __HI_DRV_REG_H__
#define __HI_DRV_REG_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


/*************** !!! important !!! ***************/
/*
never write reg like this:

    Result = 0x300;
    HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result); <-------wrong!

always write reg like this:

    HI_REG_READ32(SYS_PERI_CRG3_ADDR, Result);  <----read reg first.
    Result = Result | 0x300;                    <----change the bits you want to set.
    HI_REG_WRITE32(SYS_PERI_CRG3_ADDR, Result); <----write the value back to reg.
*/
#ifndef HI_REG_READ8
#define HI_REG_READ8(addr,result)  ((result) = *(volatile unsigned char *)(addr))
#endif

#ifndef HI_REG_READ16
#define HI_REG_READ16(addr,result)  ((result) = *(volatile unsigned short *)(addr))
#endif

#ifndef HI_REG_READ32
#define HI_REG_READ32(addr,result)  ((result) = *(volatile unsigned int *)(addr))
#endif

#ifndef HI_REG_WRITE8
#define HI_REG_WRITE8(addr,result)  (*(volatile unsigned char *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE16
#define HI_REG_WRITE16(addr,result)  (*(volatile unsigned short *)(addr) = (result))
#endif

#ifndef HI_REG_WRITE32
#define HI_REG_WRITE32(addr,result)  (*(volatile unsigned int *)(addr) = (result))
#endif

#ifndef HI_REG_READ
#define HI_REG_READ HI_REG_READ32
#endif

#ifndef HI_REG_WRITE
#define HI_REG_WRITE HI_REG_WRITE32
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __HI_REG_H__ */

