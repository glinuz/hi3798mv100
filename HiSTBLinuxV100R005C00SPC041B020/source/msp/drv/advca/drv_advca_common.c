/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_advca_common.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       :
  Last Modified :
  Description   :
  Function List :
  History       :
******************************************************************************/
#include <asm/io.h>
#include "drv_advca_internal.h"
#include "drv_advca_external.h"

#define __REMAP__

static HI_U8 *g_CaVirAddr = 0;


HI_VOID DRV_ADVCA_SystemSetClock()
{
    HI_U8 *pu8VirAddr = HI_NULL;
    OTP_V200_CRG_CTRL_U unCRG;

    unCRG.u32 = 0;

    pu8VirAddr = (HI_U8*)ioremap_nocache(OTP_V200_CRG_ADDR,0x10);
    if(HI_NULL == pu8VirAddr)
    {
        HI_ERR_OTP("ioremap_nocache map error\n");
        return;
    }

    CA_readReg(pu8VirAddr, &unCRG.u32);
	/* Select 200M clock */
    unCRG.bits.ca_kl_srst_req = 0;
    CA_writeReg(pu8VirAddr, unCRG.u32);
    iounmap(pu8VirAddr);
    return;
}

HI_VOID DRV_ADVCA_SystemSetReset()
{
    OTP_V200_CRG_CTRL_U unCRG;
    HI_U8 * pu8VirAddr = HI_NULL;

    pu8VirAddr = (HI_U8*)ioremap_nocache(OTP_V200_CRG_ADDR, 0x10);
    if (HI_NULL == pu8VirAddr)
    {
        HI_ERR_OTP("ioremap_nocache map error\n");
        return;
    }

    /* Reset */
    unCRG.u32 = 0;
    CA_readReg(pu8VirAddr, &unCRG.u32);
    unCRG.bits.ca_kl_srst_req = 1;
    CA_writeReg(pu8VirAddr, unCRG.u32);

    /* Cancel Reset */
    unCRG.u32 = 0;
    CA_readReg(pu8VirAddr, &unCRG.u32);
    unCRG.bits.ca_kl_srst_req = 0;
    CA_writeReg(pu8VirAddr, unCRG.u32);
    iounmap(pu8VirAddr);

    return;
}



HI_VOID DRV_ADVCA_RegisterMap(void)
{
    HI_VOID *ptr;

    ptr = ca_ioremap_nocache(CA_V300_BASE_ADDR, 0x1000);
    if (ptr == NULL)
    {
        HI_ERR_CA("ca_ioremap_nocache err!\n");
        return;
    }

    g_CaVirAddr = ptr;

    return;
}

HI_VOID DRV_ADVCA_RegisterUnMap(void)
{
    if (g_CaVirAddr)
    {
        ca_iounmap((HI_VOID *)g_CaVirAddr);
        g_CaVirAddr = 0;
    }

    return;
}

HI_VOID DRV_ADVCA_WriteReg(HI_U32 addr, HI_U32 val)
{
#ifdef __REMAP__
    CA_writeReg((g_CaVirAddr + (addr - CA_V300_BASE_ADDR)), val);
#endif
	return;
}

HI_U32 DRV_ADVCA_ReadReg(HI_U32 addr)
{
	HI_U32 val;
#ifdef __REMAP__
    CA_readReg((g_CaVirAddr + (addr - CA_V300_BASE_ADDR)), &val);
#endif
	return val;
}

HI_VOID DRV_ADVCA_SysWriteReg(HI_U8 *addr, HI_U32 val)
{
#ifdef __REMAP__
    CA_writeReg(addr, val);
#endif
	return;
}

HI_U32 DRV_ADVCA_SysReadReg(HI_U8 *addr)
{
	HI_U32 val;
#ifdef __REMAP__
    CA_readReg(addr, &val);
#endif
	return val;
}

HI_VOID DRV_ADVCA_Wait(HI_U32 us)
{
	return;
}

HI_S32 DRV_ADVCA_GetRevision(HI_U8 u8Revision[25])
{
    HI_S32 ret = HI_SUCCESS;
    HI_U32 u32VendorId = 0;

    if (HI_NULL == u8Revision)
    {
        HI_ERR_CA("Invalid Parameters.\n");
        return HI_FAILURE;
    }

    ret = DRV_ADVCA_GetVendorId(&u32VendorId);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_CA("CA_GetVendorId failed.\n");
        return HI_FAILURE;
    }

    if (0x01 == u32VendorId)
    {
        ca_snprintf((HI_CHAR *)u8Revision, strlen(NAGRA_REVISION) + 1, "%s", NAGRA_REVISION);
    }
    else
    {
        ca_snprintf((HI_CHAR *)u8Revision, strlen(ADVCA_REVISION) + 1, "%s", ADVCA_REVISION);
    }

    return ret;
}

/*---------------------------------------------END--------------------------------------*/

