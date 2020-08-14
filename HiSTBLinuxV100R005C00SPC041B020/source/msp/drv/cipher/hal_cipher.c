/******************************************************************************

  Copyright (C), 2011-2014, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hal_cipher.c
  Version       : Initial Draft
  Author        : Hisilicon hisecurity team
  Created       : 
  Last Modified :
  Description   : 
  Function List :
  History       :
******************************************************************************/

#include <linux/jiffies.h>
#include <asm/barrier.h>    /* mb() */
#include "hi_type.h"
#include "hi_debug.h"
#include "hi_common.h"
#include "hi_error_mpi.h"
#include "drv_cipher_ioctl.h"
#include "drv_cipher_reg.h"
#include "hal_cipher.h"
#include "drv_advca_ext.h"
#include "hi_drv_mmz.h"
#include "hi_drv_cipher.h"
#include "hi_drv_reg.h"
#include "hi_reg_common.h"
#include "hi_drv_module.h"

/* Set the defualt timeout value for hash calculating (5000 ms)*/
#define HASH_MAX_DURATION (5000)

extern HI_VOID HI_DRV_SYS_GetChipVersion(HI_CHIP_TYPE_E *penChipType, HI_CHIP_VERSION_E *penChipVersion);

HI_S32 HAL_Cipher_HashEnable(HI_VOID);
HI_S32 HAL_Cipher_HashDisable(HI_VOID);
HI_VOID HAL_RSA_Disable(HI_VOID);

/***************************** Macro Definition ******************************/
/*process of bit*/
#define HAL_SET_BIT(src, bit)        ((src) |= (1<<bit))
#define HAL_CLEAR_BIT(src,bit)       ((src) &= ~(1<<bit))
#ifdef CIPHER_MHASH_SUPPORT
#define MAX_HASH_USER  32
#else
#define MAX_HASH_USER  1
#endif
static HI_U32 s_bIsHashUser = 0;

#if 0
#ifndef HI_REG_READ32
#define HI_REG_READ32(addr,result)  ((result) = *(volatile unsigned int *)(addr))
#endif
#ifndef HI_REG_WRITE32
#define HI_REG_WRITE32(addr,result)  (*(volatile unsigned int *)(addr) = (result))
#endif
#endif

#define RSA_DATA_CLR            (7<<4)
#define RSA_DATA_CLR_KEY        (1<<4)
#define RSA_DATA_CLR_INPUT      (2<<4)
#define RSA_DATA_CLR_OUTPUT     (4<<4)
#define RSA_MOD_SEL		        (3 << 0)
#define RSA_MOD_SEL_OPT	        (0 << 0)
#define RSA_MOD_SEL_KEY_UPDATA	(1 << 0)
#define RSA_MOD_SEL_RAM_CLAER	(2 << 0)
#define RSA_MOD_SEL_CRC16	    (3 << 0)
#define RSA_BUSY		        (1 << 0)
#define RSA_START		        (1 << 0)

#define RSA_RTY_CNT             50000

/*************************** Structure Definition ****************************/
typedef enum
{
    HASH_READY,
    REC_READY,
    DMA_READY,
}HASH_WAIT_TYPE;

/******************************* API declaration *****************************/
HI_VOID HAL_CIPHER_ReadReg(HI_U32* addr, HI_U32 *pu32Val)
{
    HI_REG_READ32(addr, *pu32Val);
    return;
}

HI_VOID HAL_CIPHER_WriteReg(HI_U32* addr, HI_U32 u32Val)
{
    HI_REG_WRITE32(addr, u32Val);
    return;
}

inline HI_S32 HASH_WaitReady( HASH_WAIT_TYPE enType)
{
    CIPHER_SHA_STATUS_U unCipherSHAstatus;
    HI_SIZE_T ulStartTime = 0;
    HI_SIZE_T ulLastTime = 0;
    HI_SIZE_T ulDuraTime = 0;

    /* wait for hash_rdy */
    ulStartTime = jiffies;
    while(1)
    {
        unCipherSHAstatus.u32 = 0;
        (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAstatus.u32);
        if(HASH_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.hash_rdy)
            {
                break;
            }
        }
        else if (REC_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.rec_rdy)
            {
                break;
            }
        }
        else if (DMA_READY == enType)
        {
            if(1 == unCipherSHAstatus.bits.dma_rdy)
            {
                break;
            }
        }
        else
        {
            HI_ERR_CIPHER("Error! Invalid wait type!\n");
            return HI_FAILURE;
        }

        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= HASH_MAX_DURATION )
        { 
            HI_ERR_CIPHER("Error! Hash time out!\n");
            return HI_FAILURE;
        }

		msleep(1);
    }

    return HI_SUCCESS;
}

/* check if hash module is idle or not */
static HI_S32 HAL_CIPHER_WaitHashIdle(HI_VOID)
{
#ifndef CIPHER_MHASH_SUPPORT
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    HI_SIZE_T ulStartTime = 0;
    HI_SIZE_T ulLastTime = 0;
    HI_SIZE_T ulDuraTime = 0;
    
__HASH_WAIT__:

    ulStartTime = jiffies;
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    while(0 != (unCipherSHACtrl.bits.usedbyarm | unCipherSHACtrl.bits.usedbyc51))
    {
        ulLastTime = jiffies;
        ulDuraTime = jiffies_to_msecs(ulLastTime - ulStartTime);
        if (ulDuraTime >= HASH_MAX_DURATION)
        { 
            HI_ERR_CIPHER("Error! Hash module is busy now!\n");
            return HI_FAILURE;
        }
        else
        {
            msleep(1);
            unCipherSHACtrl.u32 = 0;
            (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
            continue;
        }
    }

    /* set bit 6 */
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.usedbyarm = 0x1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);

    /* check if set bit 6 valid or not */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.u32 = (unCipherSHACtrl.u32 >> 6) & 0x3;
    switch(unCipherSHACtrl.u32)
    {
        case 0x1:
        {
            return HI_SUCCESS;
        }
        case 0x3:
        {
            /* clear bit 6*/
            (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
            unCipherSHACtrl.bits.usedbyarm = 0;
            (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);
            goto __HASH_WAIT__;
        }
        case 0x0:
        case 0x2:
        {
            return HI_FAILURE;
        }
        default:
        {
            return HI_FAILURE;
        }
    }
#endif

    return HI_SUCCESS;
}

static HI_VOID HAL_CIPHER_MarkHashIdle(HI_VOID)
{
#ifndef CIPHER_MHASH_SUPPORT
    CIPHER_SHA_CTRL_U unCipherSHACtrl;

    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.usedbyarm = 0x0;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);
#endif
    return;
}

HI_S32 HAL_Cipher_SetInBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    /* register0~15 bit is valid, others bits reserved */
    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%#x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetSrcLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 *regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_SRC_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *addr = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetDestLstRaddr(HI_U32 chnId, HI_U32 *addr)
{
    HI_U32 *regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == addr)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_DEST_LST_RADDR(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *addr = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr  = 0;
    HI_U32 regValue = 0;

    if (CIPHER_PKGx1_CHAN == chnId || HI_NULL == pNum)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_NUM(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" HAL_Cipher_SetInBufCnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IBUF_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetInBufEmpty(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetInBufEmpty(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_IEMPTY_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    
    *pNum = regValue;
    
    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetOutBufNum(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER("chn=%d cnt=%u\n", chnId, num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufNum(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_NUM(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufCnt(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER("SetOutBufCnt=%u, chnId=%u\n", num,chnId);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufCnt(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OBUF_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" HAL_Cipher_GetOutBufCnt=%u\n", regValue);
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetOutBufFull(HI_U32 chnId, HI_U32 num)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);

    if (num > 0xffff)
    {
        HI_ERR_CIPHER("value err:%x, set to 0xffff\n", num);
        num = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, num);
    
    HI_INFO_CIPHER(" cnt=%u\n", num);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetOutBufFull(HI_U32 chnId, HI_U32 *pNum)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    if ( (CIPHER_PKGx1_CHAN == chnId) || (HI_NULL == pNum) )
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHANn_OFULL_CNT(chnId);
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    *pNum = regValue;

    HI_INFO_CIPHER(" cnt=%u\n", regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_WaitIdle(HI_VOID)
{
    HI_S32 i = 0;
    HI_U32 *pu32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    /* channel 0 configuration register [31-2]:reserved, [1]:ch0_busy, [0]:ch0_start 
         * [1]:channel 0 status signal, [0]:channel 0 encrypt/decrypt start signal
         */

    pu32RegAddr = CIPHER_REG_CHAN0_CFG;
    for (i = 0; i < CIPHER_WAIT_IDEL_TIMES; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &u32RegValue);
        if (0x0 == ((u32RegValue >> 1) & 0x01))
        {
            return HI_SUCCESS;
        }
        else
        {
            //udelay(1);
        }
    }

    return HI_FAILURE;
}
/*
just only check for channel 0
 */
HI_BOOL HAL_Cipher_IsIdle(HI_U32 chn)
{
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chn);

    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_REG_CHAN0_CFG, &u32RegValue);
    if (0x0 == ((u32RegValue >> 1) & 0x01))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HAL_Cipher_SetDataSinglePkg(HI_DRV_CIPHER_DATA_INFO_S * info)
{
    HI_U32 *regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DIN(0);
    
    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, info->u32DataPkg[i]);
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_ReadDataSinglePkg(HI_U32 *pData)
{
    HI_U32 *regAddr = 0;
    HI_U32 i = 0;

    regAddr = CIPHER_REG_CHAN0_CIPHER_DOUT(0);

    /***/
    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + i, pData+i);
    }
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_StartSinglePkg(HI_U32 chnId)
{
    HI_U32 *pu32RegAddr = 0;
    HI_U32 u32RegValue = 0;

    HI_ASSERT(CIPHER_PKGx1_CHAN == chnId);

    pu32RegAddr = CIPHER_REG_CHAN0_CFG;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &u32RegValue);
    
    u32RegValue |= 0x1;
    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, u32RegValue); /* start work */
    
    return HI_SUCCESS;
}


HI_S32 HAL_Cipher_SetBufAddr(HI_U32 chnId, CIPHER_BUF_TYPE_E bufType, HI_U32 addr)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_BUF_TYPE_IN == bufType)
    {
        regAddr = CIPHER_REG_CHANn_SRC_LST_SADDR(chnId);
    }
    else if (CIPHER_BUF_TYPE_OUT == bufType)
    {
        regAddr = CIPHER_REG_CHANn_DEST_LST_SADDR(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetBufAddr type err:%x.\n", bufType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }


    HI_INFO_CIPHER("Set chn%d '%s' BufAddr to:%x.\n",chnId,
        (CIPHER_BUF_TYPE_IN == bufType)?"In":"Out",  addr);

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, addr);

    return HI_SUCCESS;
}



HI_VOID HAL_Cipher_Reset(HI_VOID)
{

    //(HI_VOID)HAL_CIPHER_WriteReg(CIPHER_SOFT_RESET_ADDR, 1);
    return;
}

HI_S32 HAL_Cipher_GetOutIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;


    regAddr = CIPHER_REG_CHAN_CIPHER_IVOUT(chnId);

    /***/
    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_ReadReg(regAddr + i, &pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetInIV(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN != chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    regAddr = CIPHER_REG_CHAN0_CIPHER_IVIN(0);

    /***/
    for (i = 0; i < 4; i++)
    {
        (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, pCtrl->u32IV[i]);
    }

    return HI_SUCCESS;
}

extern ADVCA_EXPORT_FUNC_S  *s_pAdvcaFunc;

HI_S32 HAL_Cipher_SetKey(HI_U32 chnId, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 i = 0;
    HI_U32 *regAddr = 0;
    HI_S32 ret;
    DRV_ADVCA_EXTFUNC_PARAM_S stADVCAFuncParam = {0};

    regAddr = CIPHER_REG_CIPHER_KEY(chnId);

    if(NULL == pCtrl)
    {
        HI_ERR_CIPHER("Error, null pointer!\n");
        return HI_ERR_CA_INVALID_PARA;
    }

    if (HI_FALSE == pCtrl->bKeyByCA)
    {
        for (i = 0; i < 8; i++)
        {
            (HI_VOID)HAL_CIPHER_WriteReg(regAddr + i, pCtrl->u32Key[i]);
        }
    }
    else
    {
        ret = HI_DRV_MODULE_GetFunction(HI_ID_CA, (HI_VOID**)&s_pAdvcaFunc);
        if ( (HI_SUCCESS != ret) || (NULL == s_pAdvcaFunc) )
        {
              HI_ERR_CIPHER("Get advca function failed.\n");
            return -1;
        }
        if (s_pAdvcaFunc->pfnAdvcaCrypto)
        {
            memset(&stADVCAFuncParam, 0, sizeof(stADVCAFuncParam));
            stADVCAFuncParam.enCAType = pCtrl->enCaType;
            stADVCAFuncParam.AddrID = chnId;
            /* Ignore evenOrOdd value here */
            stADVCAFuncParam.EvenOrOdd = 0;
            stADVCAFuncParam.pu8Data = (HI_U8 *)pCtrl->u32Key;
            stADVCAFuncParam.bIsDeCrypt = HI_TRUE;
            stADVCAFuncParam.enTarget = HI_UNF_ADVCA_CA_TARGET_MULTICIPHER;
            return (s_pAdvcaFunc->pfnAdvcaCrypto)(stADVCAFuncParam); 
        }
        else
        {
            HI_ERR_CIPHER("Error, Advca Crypto function is null!\n");
            return HI_FAILURE;
        }
    }

    HI_INFO_CIPHER("SetKey: chn%u,Key:%#x, %#x, %#x, %#x.\n", chnId, pCtrl->u32Key[0], pCtrl->u32Key[1], pCtrl->u32Key[2], pCtrl->u32Key[3]);
    
    return HI_SUCCESS;
}

static HI_S32 HAL_CIPHER_IsCABusy(HI_VOID)
{
    HI_U32 cnt = 0;
    HI_U32 u32CAState = 0;

    while (cnt < 50)
    {
        u32CAState = 0;
        (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_REG_CA_STATE, &u32CAState);
        if( ((u32CAState >> 31) & 0x1) == 0)
        {
            break;
        }
        mdelay(10);
        cnt++;
    }

    if (cnt >= 50)
    {
        HI_ERR_CIPHER("Error Time out! \n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_CIPHER_LoadSTBRootKey(HI_U32 u32ChID)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_CA_STB_KEY_CTRL_U unSTBKeyCtrl;
    CIPHER_CA_CONFIG_STATE_U unConfigState;

    unConfigState.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_REG_CA_CONFIG_STATE, &unConfigState.u32);
    if(unConfigState.bits.st_vld != 1)
    {
        HI_ERR_CIPHER("Error: ca unStatus.bits.st_vld != 1\n");
        return HI_FAILURE;
    }

    ret = HAL_CIPHER_IsCABusy();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("CA time out!\n");
        return HI_FAILURE;
    }

    unSTBKeyCtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_REG_STB_KEY_CTRL, &unSTBKeyCtrl.u32);
    unSTBKeyCtrl.bits.key_addr = u32ChID;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_STB_KEY_CTRL, unSTBKeyCtrl.u32);

    ret = HAL_CIPHER_IsCABusy();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("CA time out!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


/*
=========channel n control register==========
[31:22]             weight [in 64bytes, just only for multi-packet channel encrypt or decrypt, otherwise reserved.]
[21:17]             reserved
[16:14]     RW    key_adder [current key sequence number]
[13]          RW    key_sel [key select control, 0-CPU keys, 1-keys from key Ladder]
[12:11]             reserved
[10:9]      RW      key_length[key length control
                                            (1).AES, 00-128 bits key, 01-192bits 10-256bits, 11-128bits
                                            (2).DES, 00-3 keys, 01-3keys, 10-3keys, 11-2keys]
[8]                     reserved
[7:6]       RW      width[bits width control
                                 (1).for DES/3DES, 00-64bits, 01-8bits, 10-1bit, 11-64bits
                                 (2).for AES, 00-128bits, 01-8bits, 10-1bit, 11-128bits]
[5:4]       RW      alg_sel[algorithm type, 00-DES, 01-3DES, 10-AES, 11-DES]
[3:1]       RW      mode[mode control, 
                                  (1).for AES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, 100-CTR, others-ECB
                                  (2).for DES, 000-ECB, 001-CBC, 010-CFB, 011-OFB, others-ECB]
[0]         RW      decrypt[encrypt or decrypt control, 0 stands for encrypt, 1 stands for decrypt]
*/
HI_S32 HAL_Cipher_Config(HI_U32 chnId, HI_BOOL bDecrypt, HI_BOOL bIVChange, HI_UNF_CIPHER_CTRL_S* pCtrl)
{
    HI_U32 keyId = 0;
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    HI_BOOL bKeyByCA = pCtrl->bKeyByCA;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        /* channel 0, single packet encrypt or decrypt channel */
        regAddr = CIPHER_REG_CHAN0_CIPHER_CTRL;
    }
    else
    {
        regAddr = CIPHER_REG_CHANn_CIPHER_CTRL(chnId);
    }

    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (HI_FALSE == bDecrypt)/* encrypt */
    {
        regValue &= ~(1 << 0);
    }
    else /* decrypt */
    {
        regValue |= 1;
    }

    /* set mode */
    regValue &= ~(0x07 << 1);/* clear bit1~bit3 */
    regValue |= ((pCtrl->enWorkMode & 0x7) << 1);

    /* set algorithm bits */
    regValue &= ~(0x03 << 4); /* clear algorithm bits*/
    regValue |= ((pCtrl->enAlg & 0x3) << 4);

    /* set bits width */
    regValue &= ~(0x03 << 6);
    regValue |= ((pCtrl->enBitWidth & 0x3) << 6);

    regValue &= ~(0x01 << 8);
    regValue |= ((bIVChange & 0x1) << 8);
    if (bIVChange) ///?
    {
        HAL_Cipher_SetInIV(chnId, pCtrl);
    }

    regValue &= ~(0x03 << 9);
    regValue |= ((pCtrl->enKeyLen & 0x3) << 9);

    regValue &= ~(0x01 << 13);
    regValue |= ((bKeyByCA & 0x1) << 13);

//    if (HI_FALSE == bKeyByCA) /* By User */
//    {
        keyId = chnId;/**/

        //HAL_Cipher_SetKey(chnId, pCtrl->u32Key);

        regValue &= ~(0x07 << 14);
        regValue |= ((keyId & 0x7) << 14);
//    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetAGEThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_IAGE_CNT(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_OAGE_CNT(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetAGEThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetAGEThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetIntThreshold(HI_U32 chnId, CIPHER_INT_TYPE_E intType, HI_U32 value)
{
    HI_U32 *regAddr = 0;

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (CIPHER_INT_TYPE_IN_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_ICNTCFG(chnId);
    }
    else if (CIPHER_INT_TYPE_OUT_BUF == intType)
    {
        regAddr = CIPHER_REG_CHANn_INT_OCNTCFG(chnId);
    }
    else
    {
        HI_ERR_CIPHER("SetIntThreshold type err:%x.\n", intType);
        return HI_ERR_CIPHER_INVALID_PARA;
    }

    if (value > 0xffff)
    {
        HI_ERR_CIPHER("SetIntThreshold value err:%x, set to 0xffff\n", value);
        value = 0xffff;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, value);

    return HI_SUCCESS;
}

/*
interrupt enable
[31]-----cipher module unitary interrupt enable
[30:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/
HI_S32 HAL_Cipher_EnableInt(HI_U32 chnId, int intType)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    regValue |= (1 << 31); /* sum switch int_en */
#if defined(CHIP_TYPE_hi3719mv100) \
    || defined(CHIP_TYPE_hi3718mv100) \
    || defined(CHIP_TYPE_hi3798mv100) \
    || defined (CHIP_TYPE_hi3798mv200) \
    || defined (CHIP_TYPE_hi3798mv200_a)  \
    || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100) \
    || defined (CHIP_TYPE_hi3798cv200_a) \
    || defined (CHIP_TYPE_hi3798cv200_b) \
    || defined (CHIP_TYPE_hi3716mv410) \
    || defined (CHIP_TYPE_hi3716mv420) \
	|| defined (CHIP_TYPE_hi3798cv200)
    regValue |= (1 << 30); /* sec_int_en */
#endif

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue |= (1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue |= (1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue |= (1 << (0 + chnId));
        }
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_EnableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_DisableInt(HI_U32 chnId, int intType)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (CIPHER_PKGx1_CHAN == chnId)
    {
        regValue &= ~(1 << 8);
    }
    else
    {
        if (CIPHER_INT_TYPE_OUT_BUF == (CIPHER_INT_TYPE_OUT_BUF & intType))
        {
            regValue &= ~(1 << (8 + chnId));
        }

        /* NOT else if */
        if (CIPHER_INT_TYPE_IN_BUF == (CIPHER_INT_TYPE_IN_BUF & intType))
        {
            regValue &= ~(1 << (0 + chnId));
        }
    }

    if (0 == (regValue & 0x7fffffff))
    {
        regValue &= ~(1 << 31); /* regValue = 0; sum switch int_en */
    }

    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);

    HI_INFO_CIPHER("HAL_Cipher_DisableInt: Set INT_EN:%#x\n", regValue);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_DisableAllInt(HI_VOID)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;
    HI_U32 i;

    regAddr = CIPHER_REG_INT_EN;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);
    for(i=CIPHER_PKGxN_CHAN_MIN; i<=CIPHER_PKGxN_CHAN_MAX; i++)
	{
	    regValue &= ~(0x01 << i);
    }
    regValue &= 0x3FFFFFFF;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

/*
interrupt status register
[31:16]--reserved
[15] channel 7 output queue data interrupt enable
[14] channel 6 output queue data interrupt enable
[... ] channel ... output queue data interrupt enable
[9]   channel 1 output queue data interrupt enable
[8]   channel 0 data dispose finished interrupt enble
[7] channel 7 input queue data interrupt enable
[6] channel 6 input queue data interrupt enable
...
[1] channel 1 input queue data interrupt enable
[0] reserved
*/

HI_VOID HAL_Cipher_GetIntState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_STATUS;
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetIntEnState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_EN;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetIntEnState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_GetRawIntState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

    regAddr = CIPHER_REG_INT_RAW;
    
    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

    HI_INFO_CIPHER("HAL_Cipher_GetRawIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_ClrIntState(HI_U32 intStatus)
{
    HI_U32 *regAddr;
    HI_U32 regValue;

    regAddr = CIPHER_REG_INT_RAW;
    regValue = intStatus;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

HI_VOID HAL_Cipher_EnableAllSecChn(HI_VOID)
{
	HI_U32 *regAddr = CIPHER_REG_SEC_CHN_CFG;
	HI_U32 regValue = 0;
    HI_U32 i;

    for(i=CIPHER_PKGxN_CHAN_MIN; i<=CIPHER_PKGxN_CHAN_MAX; i++)
	{
	    regValue |= 0x01 << i;
    }
#ifndef HI_TEE_SUPPORT
    regValue |= 0x01;
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
	return;
}

#ifdef HI_SMMU_SUPPORT
HI_S32 HAL_Cipher_SetMmuBypass(HI_U32 chnId)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

	if(chnId == CIPHER_PKGx1_CHAN)
	{
		return HI_SUCCESS;
	}
	
    /*clear rd bypass flag*/
    regAddr = CIPHER_REG_CHANn_RD_DAT_ADDR_SMMU_BYPASS;
    HAL_CIPHER_ReadReg(regAddr, &regValue);
    regValue &= ~(0x01 << (chnId - 1)); 
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);

    /*clear wr bypass flag*/
    regAddr = CIPHER_REG_CHANn_WR_DAT_ADDR_SMMU_BYPASS;
    HAL_CIPHER_ReadReg(regAddr, &regValue);
    regValue &= ~(0x01 << (chnId - 1)); 
    HAL_CIPHER_WriteReg(regAddr, regValue);

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_GetMmuIntState(HI_U32 *pState)
{
    HI_U32 *regAddr = 0;
    HI_U32 regValue = 0;

#ifdef HI_TEE_SUPPORT
    regAddr = CIPHER_MMU_INTSTAT_NS;
#else
    regAddr = CIPHER_MMU_INTSTAT_S;
#endif

    (HI_VOID)HAL_CIPHER_ReadReg(regAddr, &regValue);

    if (pState)
    {
        *pState = regValue;
    }

   HI_INFO_CIPHER("HAL_Cipher_GetMMUIntState=%#x\n", regValue);
}

HI_VOID HAL_Cipher_ClrMmuIntState(HI_U32 intStatus)
{
    HI_U32 *regAddr;
    HI_U32 regValue;

#ifdef HI_TEE_SUPPORT
    regAddr = CIPHER_MMU_INTCLR_NS;
#else
    regAddr = CIPHER_MMU_INTCLR_S;
#endif
    regValue = intStatus;
    (HI_VOID)HAL_CIPHER_WriteReg(regAddr, regValue);
}

HI_VOID HAL_Cipher_EnableMmu(HI_VOID)
{
    HI_U32 Value;
    HI_U32 *pvirt;

    pvirt = ioremap_nocache(CIPHER_MMU_CRG_REG_BASE_ADDR_PHY, 16);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n", CIPHER_MMU_CRG_REG_BASE_ADDR_PHY);
        return ;
    }
    HAL_CIPHER_ReadReg(pvirt, &Value);       
    HAL_SET_BIT(Value, 4); /* reset */ 
    HAL_SET_BIT(Value, 0); /* set the bit 0, clock opened */    
    HAL_CIPHER_WriteReg(pvirt,Value);    
    msleep(50);
    
    /* clock select and cancel reset 0x30100*/    
    HAL_CLEAR_BIT(Value, 4); /* cancel reset */    
    HAL_SET_BIT(Value, 0);   /* set the bit 0, clock opened */        
    HAL_CIPHER_WriteReg(pvirt,Value);    

    iounmap((volatile void __iomem *)pvirt);

    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_MMU_GLOBAL_CTR_ADDR, &Value);
    Value &= ~0x01; //Global bypass disable
    Value |=  0x08; //Init enable
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_GLOBAL_CTR_ADDR, Value);
	return;
}

HI_VOID HAL_Cipher_SetMmuAddr(HI_U32 u32ptaddr, HI_U32 u32err_rdaddr, HI_U32 u32err_wraddr)
{
#ifdef HI_TEE_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_CB_TTBR, u32ptaddr);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_ERR_RDADDR_NS, u32err_rdaddr);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_ERR_WRADDR_NS, u32err_wraddr);
#else
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_SCB_TTBR, u32ptaddr);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_ERR_RDADDR_S, u32err_rdaddr);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_MMU_ERR_WRADDR_S, u32err_wraddr);
#endif
	return;
}

#endif

HI_VOID HAL_Cipher_SetHDMITxRxOtpSelReg(HI_VOID)
{
    HI_U32 *pu32Addr = 0;
    HI_U32 u32Value = 0;

    pu32Addr = PERI_HDMI_RX_TX_OTP_SEL_REG_ADDR;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32Addr,&u32Value);

    u32Value &= 0xFFFFFFFE;
    (HI_VOID)HAL_CIPHER_WriteReg(pu32Addr, u32Value);

    return;
}

HI_VOID HAL_Cipher_SetHdcpModeEn(HI_DRV_CIPHER_HDCP_KEY_MODE_E enMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U stHDCPModeCtrl;

    memset((HI_VOID *)&stHDCPModeCtrl, 0, sizeof(stHDCPModeCtrl.u32));

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &stHDCPModeCtrl.u32);
    
    if ( CIPHER_HDCP_MODE_NO_HDCP_KEY == enMode)
    {
        stHDCPModeCtrl.bits.hdcp_mode_en = 0;
    }
    else
    {
        stHDCPModeCtrl.bits.hdcp_mode_en = 1;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, stHDCPModeCtrl.u32);
    
    return;
}

HI_S32 HAL_Cipher_GetHdcpModeEn(HI_DRV_CIPHER_HDCP_KEY_MODE_E *penMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U stHDCPModeCtrl;

    if ( NULL == penMode )
    {
        HI_ERR_CIPHER("Invald param, null pointer!\n");
        return HI_FAILURE;
    }

    memset((HI_VOID *)&stHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &stHDCPModeCtrl.u32);
    
    if ( 0 == stHDCPModeCtrl.bits.hdcp_mode_en)
    {
        *penMode = CIPHER_HDCP_MODE_NO_HDCP_KEY;
    }
    else
    {
        *penMode = CIPHER_HDCP_MODE_HDCP_KEY;
    }

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_SetHdcpKeyTxRead(HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E enMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);

    if ( CIPHER_HDCP_KEY_RAM_MODE_READ == enMode)
    {
        unHDCPModeCtrl.bits.tx_read = 0x1;      //hdmi read mode
    }
    else
    {
        unHDCPModeCtrl.bits.tx_read = 0x0;      //cpu write mode
    }    

    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, unHDCPModeCtrl.u32);
    
    return;
}

HI_S32 HAL_Cipher_GetHdcpKeyRamMode(HI_DRV_CIPHER_HDCP_KEY_RAM_MODE_E *penMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    if ( NULL == penMode )
    {
        return HI_FAILURE;
    }

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));
    
    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);
    
    if ( 0 == unHDCPModeCtrl.bits.tx_read )
    {
        *penMode = CIPHER_HDCP_KEY_RAM_MODE_WRITE;      //cpu write mode
    }
    else
    {
        *penMode = CIPHER_HDCP_KEY_RAM_MODE_READ;       //hdmi read mode
    }    
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_SetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E enHdcpKeySelectMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);       
    
    if ( CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY == enHdcpKeySelectMode )
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x00;
    }
    else if ( CIPHER_HDCP_KEY_TYPE_HISI_DEFINED == enHdcpKeySelectMode )
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x01;
    }
    else if ( CIPHER_HDCP_KEY_TYPE_HOST_ROOT_KEY == enHdcpKeySelectMode)
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x2;
    }
    else
    {
        unHDCPModeCtrl.bits.hdcp_rootkey_sel = 0x3;
        (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, unHDCPModeCtrl.u32);

        HI_ERR_CIPHER("Unexpected hdcp key type selected!\n");
        return HI_FAILURE;
    }
    
    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, unHDCPModeCtrl.u32);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetHdcpKeySelectMode(HI_DRV_CIPHER_HDCP_ROOT_KEY_TYPE_E *penHdcpKeySelectMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;
    
    if ( NULL == penHdcpKeySelectMode )
    {
        HI_ERR_CIPHER("Invalid param, NULL pointer!\n");
        return HI_FAILURE;
    }

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));
    
    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);

    if ( 0x00 == unHDCPModeCtrl.bits.hdcp_rootkey_sel )
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_OTP_ROOT_KEY;
    }
    else if ( 0x01 == unHDCPModeCtrl.bits.hdcp_rootkey_sel)
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_HISI_DEFINED;
    }
    else if (  0x02 == unHDCPModeCtrl.bits.hdcp_rootkey_sel )
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_HOST_ROOT_KEY;
    }
    else
    {
        *penHdcpKeySelectMode = CIPHER_HDCP_KEY_TYPE_BUTT;
    }
    
    return HI_SUCCESS;
}

HI_VOID HAL_CIPHER_SetRxRead(HI_DRV_CIPHER_HDCP_KEY_RX_READ_E enRxReadMode)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);
    unHDCPModeCtrl.bits.rx_read = enRxReadMode;
    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, unHDCPModeCtrl.u32);

    return;
}

HI_S32 HAL_Cipher_SetRxSelect(HI_DRV_CIPHER_HDCP_KEY_RX_SELECT_E enRxSelect)
{
    HI_U32 *pu32RegAddr = 0;
    CIPHER_HDCP_MODE_CTRL_U unHDCPModeCtrl;

    memset((HI_VOID *)&unHDCPModeCtrl, 0, sizeof(CIPHER_HDCP_MODE_CTRL_U));

    pu32RegAddr = CIPHER_REG_HDCP_MODE_CTRL;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);

    if ( CIPHER_HDCP_KEY_SELECT_RX == enRxSelect)
    {
        unHDCPModeCtrl.bits.rx_sel = 0x1;      //select rx ram
    }
    else
    {
        unHDCPModeCtrl.bits.rx_sel = 0x0;      //select tx ram
    }

    (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr, unHDCPModeCtrl.u32);

#if 1
    /* check set correctly or not */
    unHDCPModeCtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr, &unHDCPModeCtrl.u32);
    if ( CIPHER_HDCP_KEY_SELECT_RX == enRxSelect)
    {
        if(unHDCPModeCtrl.bits.rx_sel != 0x1)
        {
            HI_ERR_CIPHER("Set rx select error!\n");
            return HI_FAILURE;
        }
    }
    else
    {
        if(unHDCPModeCtrl.bits.rx_sel != 0x0)
        {
            HI_ERR_CIPHER("Set tx select error!\n");
            return HI_FAILURE;
        }
    }
#endif
    
    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_ClearHdcpCtrlReg(HI_VOID)
{
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_HDCP_MODE_CTRL, 0);
    return;
}

#ifdef CIPHER_MHASH_SUPPORT
static HI_U32 HAL_Hash_Small2Large(HI_U32 u32SamllVal)
{
    HI_U32 u32LargeVal = 0;

    u32LargeVal  = (u32SamllVal >> 24) & 0xFF;
    u32LargeVal |= ((u32SamllVal >> 16) & 0xFF) << 8;
    u32LargeVal |= ((u32SamllVal >> 8) & 0xFF) << 16;
    u32LargeVal |= ((u32SamllVal) & 0xFF) << 24;

    return u32LargeVal;
}
#endif

HI_VOID HAL_Cipher_FreeHash(HI_VOID)
{
    HAL_CIPHER_MarkHashIdle();
    HAL_Cipher_HashDisable();

    if (s_bIsHashUser > 0)
    {
        s_bIsHashUser--;
    }
    else
    {
        s_bIsHashUser = 0;
    }
}

HI_S32 HAL_Cipher_AllocHash(HI_VOID)
{
    HI_S32 ret = HI_SUCCESS;
    
    if (s_bIsHashUser >= MAX_HASH_USER)
    {
        HI_ERR_CIPHER("Error! Hash is Busy!\n");
        return HI_FAILURE;
    }

    HAL_Cipher_HashSoftReset();

    ret = HAL_CIPHER_WaitHashIdle();
    if(HI_SUCCESS != ret)
    {
        HI_ERR_CIPHER("Time out!Hash is busy now!\n");
        return HI_FAILURE;
    }

    if (s_bIsHashUser > 0)
    {
        s_bIsHashUser++;
    }
    else
    {
        s_bIsHashUser = 1;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashInit(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_CTRL_U unCipherSHACtrl;
    CIPHER_SHA_START_U unCipherSHAStart;
#ifdef CIPHER_MHASH_SUPPORT
    HI_U32 i;
    HI_U32 *pu32RegAddr;
    HI_U32 u32ShaVal;
#endif
    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error! Null pointer input!\n");
        return HI_FAILURE;
    }

    ret = HAL_Cipher_AllocHash();
    if(HI_SUCCESS != ret)
    {
        return HI_FAILURE;
    }

    /* wait for hash_rdy */
    ret = HASH_WaitReady(HASH_READY);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

#if 0
    /* set hmac-sha key */
    if( ((HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType) || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        && (HI_CIPHER_HMAC_KEY_FROM_CPU == pCipherHashData->enHMACKeyFrom) )
    {
        for( i = 0; i < CIPHER_HMAC_KEY_LEN; i = i + 4)
        {
            u32WriteData = (pCipherHashData->pu8HMACKey[3+i] << 24) |
                           (pCipherHashData->pu8HMACKey[2+i] << 16) |
                           (pCipherHashData->pu8HMACKey[1+i] << 8)  |
                           (pCipherHashData->pu8HMACKey[0+i]);
            (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_MCU_KEY0 + i, u32WriteData);
        }
    }
#endif

    /* write total len low and high */
#ifdef CIPHER_MHASH_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, pCipherHashData->u32DataLen);
#else
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_LOW_ADDR, 0xFFFFFFFF);
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_TOTALLEN_HIGH_ADDR, 0);

    /* config sha_ctrl : read by dma first, and by cpu in the hash final function */
    unCipherSHACtrl.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_CTRL_ADDR, &unCipherSHACtrl.u32);
    unCipherSHACtrl.bits.read_ctrl = 0;
    if( HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType )
    {
		/* unCipherSHACtrl.bits.hardkey_hmac_flag = 1; */
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x0;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else if( HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType )
    {
        unCipherSHACtrl.bits.hardkey_hmac_flag = 0;
        unCipherSHACtrl.bits.sha_sel= 0x1;
		/* unCipherSHACtrl.bits.hardkey_sel = pCipherHashData->enHMACKeyFrom; */
    }
    else
    {
        HI_ERR_CIPHER("Invalid hash type input!\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    unCipherSHACtrl.bits.small_end_en = 1;
#ifdef CIPHER_MHASH_SUPPORT
    unCipherSHACtrl.bits.sha_init_update_en = 1;
    pu32RegAddr = CIPHER_HASH_REG_INIT1_UPDATE;
    for(i=0; i<8; i++)
    {
        u32ShaVal = HAL_Hash_Small2Large(pCipherHashData->u32ShaVal[i]);
        (HI_VOID)HAL_CIPHER_WriteReg(pu32RegAddr+i, u32ShaVal);
    //     HI_PRINT ("IV[%d] 0x%x\n", i, u32ShaVal);
    }
#endif
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_CTRL_ADDR, unCipherSHACtrl.u32);    
    
    /* config sha_start */
    unCipherSHAStart.u32 = 0;
    unCipherSHAStart.bits.sha_start = 1;
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_START_ADDR, unCipherSHAStart.u32);
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashUpdate(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    ret= HASH_WaitReady(REC_READY);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_START_ADDR, pCipherHashData->u32DataPhy);
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_HASH_REG_DMA_LEN, pCipherHashData->u32DataLen);

    ret  = HASH_WaitReady(REC_READY);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_CalcHashFinal(CIPHER_HASH_DATA_S *pCipherHashData)
{
    HI_S32 ret = HI_SUCCESS;
    CIPHER_SHA_STATUS_U unCipherSHAStatus;
    HI_U32 *pu32RegAddr;
    HI_U32 i = 0;

    if( NULL == pCipherHashData )
    {
        HI_ERR_CIPHER("Error, Null pointer input!\n");
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    /* wait for rec_ready instead of hash_ready */
    ret= HASH_WaitReady(REC_READY);
    if(HI_SUCCESS != ret)
    {
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    /* read digest */
    unCipherSHAStatus.u32 = 0;
    (HI_VOID)HAL_CIPHER_ReadReg(CIPHER_HASH_REG_STATUS_ADDR, &unCipherSHAStatus.u32);

    pu32RegAddr = CIPHER_HASH_REG_SHA_OUT1;
    if( (0x00 == unCipherSHAStatus.bits.error_state) && (0x00 == unCipherSHAStatus.bits.len_err))
    {
        if( (HI_UNF_CIPHER_HASH_TYPE_SHA1 == pCipherHashData->enShaType)
         || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA1 == pCipherHashData->enShaType))
        {
            
            for(i = 0; i < 5; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr+i, &pCipherHashData->u32ShaVal[i]);
                  //HI_PRINT ("HASH[%d] 0x%x\n", i, pCipherHashData->u32ShaVal[i]);
            }
        }
        else if( (HI_UNF_CIPHER_HASH_TYPE_SHA256 == pCipherHashData->enShaType )
              || (HI_UNF_CIPHER_HASH_TYPE_HMAC_SHA256 == pCipherHashData->enShaType))
        {
            for(i = 0; i < 8; i++)
            {
                 (HI_VOID)HAL_CIPHER_ReadReg(pu32RegAddr+i, &pCipherHashData->u32ShaVal[i]);
                 //HI_PRINT ("HASH[%d] 0x%x\n", i, pCipherHashData->u32ShaVal[i]);
    		}
        }
        else
        {
            HI_ERR_CIPHER("Invalid hash type : %d!\n", pCipherHashData->enShaType);
            (HI_VOID)HAL_Cipher_FreeHash();
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CIPHER("Error! SHA Status Reg: error_state = %d!\n", unCipherSHAStatus.bits.error_state);
        (HI_VOID)HAL_Cipher_FreeHash();
        return HI_FAILURE;
    }

    HAL_Cipher_FreeHash();
    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_GetRandomNumber(CIPHER_RNG_S *pstRNG)
{
#if defined (CHIP_TYPE_hi3798cv200) || defined (CHIP_TYPE_hi3798mv200) || defined (CHIP_TYPE_hi3798mv200_a)    \

    HI_U32 u32RngStat = 0;
    HI_U32 u32TimeOut = 0;

	if(NULL == pstRNG)
	{
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_FAILURE;
	}
    
#ifndef HI_TEE_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(HISEC_COM_TRNG_CTRL, 0x80aa);
#endif

    if(0 == pstRNG->u32TimeOutUs)
	{
		/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	    (HI_VOID)HAL_CIPHER_ReadReg(HISEC_COM_TRNG_DATA_ST, &u32RngStat);
	    if(((u32RngStat >> 8) & 0x3F) <= 0)
	    {
	        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
	    }
	}
	else
	{
	    while(u32TimeOut ++ < pstRNG->u32TimeOutUs)
	    {
	    	/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	        (HI_VOID)HAL_CIPHER_ReadReg(HISEC_COM_TRNG_DATA_ST, &u32RngStat);
	        if(((u32RngStat >> 8) & 0x3F) > 0)
	        {
	            break;
	        }
	    }
        if (u32TimeOut >= pstRNG->u32TimeOutUs)
        {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
	}

    (HI_VOID)HAL_CIPHER_ReadReg(HISEC_COM_TRNG_FIFO_DATA, &pstRNG->u32RNG);
    
#ifndef HI_TEE_SUPPORT
    (HI_VOID)HAL_CIPHER_WriteReg(HISEC_COM_TRNG_DATA_ST, 0x00);
#endif

#else
    HI_U32 u32RngStat = 0;
    HI_U32 u32RngCtrl = 0;
    HI_U32 u32TimeOut = 0;

	if(NULL == pstRNG)
	{
        HI_ERR_CIPHER("Invalid params!\n");
        return HI_FAILURE;
	}
    
	/* select rng source: 2 */
    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_BASE_ADDR, &u32RngCtrl);
    if ((0x01 == (u32RngCtrl & 0x03)) || (0x0 == (u32RngCtrl & 0x03))) /* 0x01 and 0x00 is not used */
    {
        u32RngCtrl &= 0xfffffffc;
        u32RngCtrl |= 0x2;		/* select rng source 0x2, but 0x03 is ok too */
        (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);
		//msleep(2);
    }

    /* config post_process_depth */
	(HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_BASE_ADDR, &u32RngCtrl);
    if (0 == (u32RngCtrl & 0x00009000))
    {
        u32RngCtrl |= 0x00009000;
        (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);
    }

    /* config post_process_enable andd drop_enable */
	(HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_BASE_ADDR, &u32RngCtrl);
    if (0 == (u32RngCtrl & 0x000000a0))
    {
        u32RngCtrl |= 0x000000a0;
        (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);
    }

	if(0 == pstRNG->u32TimeOutUs)
	{
		/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_STAT_ADDR, &u32RngStat);
	    if((u32RngStat & 0x7) <= 0)
	    {
	        return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
	    }
	}
	else
	{
	    while(u32TimeOut ++ < pstRNG->u32TimeOutUs)
	    {
	    	/* low 3bit(RNG_data_count[2:0]), indicate how many RNGs in the fifo is available now */
	        (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_STAT_ADDR, &u32RngStat);
	        if((u32RngStat & 0x7) > 0)
	        {
	            break;
	        }

			msleep(1);
	    }
        if (u32TimeOut >= pstRNG->u32TimeOutUs)
        {
            return HI_ERR_CIPHER_NO_AVAILABLE_RNG;
        }
	}

    (HI_VOID)HAL_CIPHER_ReadReg(REG_RNG_NUMBER_ADDR, &pstRNG->u32RNG);

    u32RngCtrl &= ~0x03;
    (HI_VOID)HAL_CIPHER_WriteReg(REG_RNG_BASE_ADDR, u32RngCtrl);
#endif
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_HashDisable(HI_VOID)
{
#ifndef CHIP_TYPE_hi3796mv200
    U_PERI_CRG49 unShaCrg;

/* reset request */
    unShaCrg.u32 = g_pstRegCrg->PERI_CRG49.u32;
#if defined (CHIP_TYPE_hi3798cv200) || defined (CHIP_TYPE_hi3798mv200) || defined (CHIP_TYPE_hi3798mv200_a)
	unShaCrg.bits.sha1_srst_req = 1;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);
	unShaCrg.bits.sha1_cken = 0;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
#else
    unShaCrg.bits.sha_srst_req = 1;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);
    unShaCrg.bits.sha_cken = 0;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
#endif
#endif
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_HashEnable(HI_VOID)
{
#ifndef CHIP_TYPE_hi3796mv200
    U_PERI_CRG49 unShaCrg;

/* cancel reset */
    unShaCrg.u32 = g_pstRegCrg->PERI_CRG49.u32;
#if defined (CHIP_TYPE_hi3798cv200) || defined (CHIP_TYPE_hi3798mv200) || defined (CHIP_TYPE_hi3798mv200_a)
    unShaCrg.bits.sha1_cken = 1;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);
    unShaCrg.bits.sha1_srst_req = 0;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);    
#else
    unShaCrg.bits.sha_cken = 1;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);
    unShaCrg.bits.sha_srst_req = 0;
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;
    mdelay(1);    
#endif
    g_pstRegCrg->PERI_CRG49.u32 = unShaCrg.u32;

    (HI_VOID)HAL_CIPHER_MarkHashIdle();

#endif    
    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_HashSoftReset(HI_VOID)
{
    HAL_Cipher_HashDisable();
    HAL_Cipher_HashEnable();

    return HI_SUCCESS;
}

HI_VOID HAL_Cipher_Init(void)
{
#ifndef CHIP_TYPE_hi3796mv200
    U_PERI_CRG48 unCipherCrg;

    unCipherCrg.u32 = g_pstRegCrg->PERI_CRG48.u32;

    unCipherCrg.bits.ca_ci_srst_req = 1;
    g_pstRegCrg->PERI_CRG48.u32 = unCipherCrg.u32;
    mdelay(1);
    unCipherCrg.bits.ca_ci_srst_req = 0;
    g_pstRegCrg->PERI_CRG48.u32 = unCipherCrg.u32;
    
#if defined (CHIP_TYPE_hi3798mv200)
    {
        HI_U32 i;
        HI_U32 u32RegValue;

        for(i=0; i<50000; i++)
        {
           HAL_CIPHER_ReadReg(RST_STA, &u32RegValue); 
           if (u32RegValue & 0x01)
           {
                break;
           }
        }
        if (i >= 50000)
        {
            HI_ERR_CIPHER("Cipher reset failed !!!");
        }
    }
#else
    mdelay(1);
#endif

    /* hash sw reset */
    (HI_VOID)HAL_Cipher_HashSoftReset();	

#if defined(CHIP_TYPE_hi3798cv200_a) || defined (CHIP_TYPE_hi3798cv200_b)
    HAL_Cipher_SetHdcpKeyTxRead(CIPHER_HDCP_KEY_RAM_MODE_READ);
#endif

#endif
    return;
}

HI_VOID HAL_Cipher_DeInit(void)
{
#ifndef CHIP_TYPE_hi3796mv200
    U_PERI_CRG48 unCipherCrg;
    HI_U32 Value;
    HI_U32 *pvirt;

    pvirt = ioremap_nocache(CIPHER_RSA_CRG_REG_BASE_ADDR_PHY, 16);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n", CIPHER_RSA_CRG_REG_BASE_ADDR_PHY);
        return ;
    }
    HAL_CIPHER_ReadReg(pvirt, &Value);       
    HAL_SET_BIT(Value, 4); /* reset */ 
    HAL_CLEAR_BIT(Value, 0); /* set the bit 0, clock closed */    
    HAL_CIPHER_WriteReg(pvirt,Value);    
    iounmap((volatile void __iomem *)pvirt);
    
    unCipherCrg.u32 = g_pstRegCrg->PERI_CRG48.u32;
    unCipherCrg.bits.ca_ci_srst_req = 1;
    g_pstRegCrg->PERI_CRG48.u32 = unCipherCrg.u32;

    HAL_Cipher_HashDisable();
	
#ifdef CONFIG_RSA_HARDWARE_SUPPORT
	HAL_RSA_Disable();
#endif
	
#endif   
    return;
}


HI_S32 HAL_Cipher_CbcMacAuth(CIPHER_CBCMAC_DATA_S *pstParam)
{
    HI_U8 *pu32TmpBuf;
    HI_U32 i;
    HI_U32 u32Value;

    pu32TmpBuf = (HI_U8*)pstParam->u8RefCbcMac;

    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_HL_APP_LEN, pstParam->u32AppLen);

    for (i = 0; i < 4; i++)
    {
        u32Value = (pu32TmpBuf[4 * (3 - i)] << 24)
                                | (pu32TmpBuf[4 * (3 - i) + 1] << 16)
                                | (pu32TmpBuf[4 * (3 - i) + 2] << 8)
                                | (pu32TmpBuf[4 * (3 - i) + 3]);            
        (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_HL_APP_CBC_MAC_REF + i * 4, u32Value);
    }
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_HL_APP_CBC_CTRL, 0x01);

    return HI_SUCCESS;
}

HI_S32 HAL_Cipher_ClearCbcVerifyFlag()
{
    (HI_VOID)HAL_CIPHER_WriteReg(CIPHER_REG_HL_APP_CBC_CTRL, 0x0);

    return HI_SUCCESS;
}

#ifdef CONFIG_RSA_HARDWARE_SUPPORT
HI_VOID HAL_RSA_Reset(HI_VOID)
{
    HI_U32 Value;
    HI_U32 *pvirt;

    pvirt = ioremap_nocache(CIPHER_RSA_CRG_REG_BASE_ADDR_PHY, 16);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n", CIPHER_RSA_CRG_REG_BASE_ADDR_PHY);
        return ;
    }

    HAL_CIPHER_ReadReg(pvirt, &Value); 
    HAL_SET_BIT(Value, 0); /* set the bit 0, clock opened */    
    HAL_CIPHER_WriteReg(pvirt,Value);    
    msleep(10);
    
    HAL_SET_BIT(Value, 4); /* reset */ 
    HAL_CIPHER_WriteReg(pvirt,Value);
    msleep(10);
    
    /* clock select and cancel reset 0x30100*/    
    HAL_CLEAR_BIT(Value, 4); /* cancel reset */    
    HAL_CIPHER_WriteReg(pvirt,Value); 
    msleep(10);

    iounmap((volatile void __iomem *)pvirt);

}

HI_VOID HAL_RSA_Disable(HI_VOID)
{
    HI_U32 Value;
    HI_U32 *pvirt;

    pvirt = ioremap_nocache(CIPHER_RSA_CRG_REG_BASE_ADDR_PHY, 16);
	if (pvirt == 0)
    {
        HI_ERR_CIPHER("ioremap_nocache phy addr err:%x.\n", CIPHER_RSA_CRG_REG_BASE_ADDR_PHY);
        return ;
    }
    
    HAL_CIPHER_ReadReg(pvirt, &Value);
    HAL_SET_BIT(Value, 4); /* reset */ 
    HAL_CIPHER_WriteReg(pvirt,Value);
    msleep(10);
    
    HAL_CLEAR_BIT(Value, 0); /* clean the bit 0, clock closed */    
    HAL_CIPHER_WriteReg(pvirt,Value);    
    msleep(10);

    iounmap((volatile void __iomem *)pvirt); 
}

HI_VOID HAL_RSA_Start(HI_VOID)
{
    HAL_CIPHER_WriteReg(SEC_RSA_START_REG, 0x01);
}

HI_S32 HAL_RSA_WaitFree(HI_VOID)
{
	HI_U32 u32Value;
    HI_U32 u32TryCount = 0;

	do {

        HAL_CIPHER_ReadReg(SEC_RSA_BUSY_REG, &u32Value);
		if ((u32Value & RSA_BUSY) == 0)
		{
		    return HI_SUCCESS;
        }
		u32TryCount++;
        msleep(1);
	} while (u32TryCount < RSA_RTY_CNT);

	return HI_FAILURE;
}

HI_VOID HAL_RSA_ClearRam(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg(SEC_RSA_MOD_REG, &u32Value);
    u32Value &= 0x0c;
    u32Value |= RSA_DATA_CLR_INPUT | RSA_DATA_CLR_OUTPUT | RSA_DATA_CLR_KEY | RSA_MOD_SEL_RAM_CLAER;
    HAL_CIPHER_WriteReg(SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_Crc(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg(SEC_RSA_MOD_REG, &u32Value);
    u32Value &= 0x0C; //don't chang sec_rsa_key_width
    u32Value |= 0x03;
    HAL_CIPHER_WriteReg(SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_GetCrc(HI_U32 *pu32Crc)
{
    HAL_CIPHER_ReadReg(SEC_RSA_CRC16_REG, pu32Crc);
}

HI_VOID HAL_RSA_ConfigMode(CIPHER_RSA_KEY_WIDTH_E enKenWidth)
{
	HI_U32 u32Value;

    u32Value = (enKenWidth << 2) | RSA_MOD_SEL_OPT;
    HAL_CIPHER_WriteReg(SEC_RSA_MOD_REG, u32Value);
}

HI_VOID HAL_RSA_WriteData(CIPHER_RSA_DATA_TYPE_E enDataType, HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Len)
{
	HI_U32 u32Value;
    HI_U32 *u32Reg;
    HI_U8 *pPos;
    HI_U32 i= 0;

    if (enDataType == CIPHER_RSA_DATA_TYPE_CONTEXT)
    {
        u32Reg = SEC_RSA_WDAT_REG;
    }
    else
    {
        u32Reg = SEC_RSA_WSEC_REG;
    }

    pPos = pu8Data;
    for(i=0; i<u32Len; i+=4)
    {
        u32Value  = (HI_U32)pPos[0];
        u32Value |= ((HI_U32)pPos[1]) << 8;
        u32Value |= ((HI_U32)pPos[2]) << 16;
        u32Value |= ((HI_U32)pPos[3]) << 24;
        HAL_CIPHER_WriteReg(u32Reg, u32Value); 
        pPos+=4; 
    }
}

HI_VOID HAL_RSA_ReadData(HI_U8 *pu8Data, HI_U32 u32DataLen, HI_U32 u32Klen)
{
	HI_U32 u32Value;
    HI_U8 *pPos;
    HI_U32 i = 0;

    pPos = pu8Data;
   /* 
    for(i=0; i<(u32Klen - u32DataLen); i+=4)
    {
        HAL_CIPHER_ReadReg(SEC_RSA_RRSLT_REG, &u32Value);  
    }*/
    for(i=0; i<u32Klen; i+=4)
    {
        HAL_CIPHER_ReadReg(SEC_RSA_RRSLT_REG, &u32Value);  
        pPos[0] = (HI_U8)(u32Value & 0xFF);
        pPos[1] = (HI_U8)((u32Value >> 8) & 0xFF);
        pPos[2] = (HI_U8)((u32Value >> 16) & 0xFF);
        pPos[3] = (HI_U8)((u32Value >> 24) & 0xFF);
        pPos+=4; 
    }  
}

HI_U32 HAL_RSA_GetErrorCode(HI_VOID)
{
	HI_U32 u32Value;

    HAL_CIPHER_ReadReg(SEC_RSA_ERROR_REG, &u32Value);

    return u32Value;
}
#endif

