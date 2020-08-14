/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : isl9492.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/25
  Description   :
  History       :
  1.Date        : 2012/02/25
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For LNB control chipset ISL9492.
 */

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include <linux/semaphore.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>

#include "drv_lnbctrl.h"
#include "isl9492.h"
#include "drv_i2c_ext.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

/*SR1*/
#define SR1_INDEX 0x00
#define SR1_ADDR_OFFSET (0x00 << 5)
#define SR1_OTF (0x1 << 4)
#define SR1_CABF (0x1 << 3)
#define SR1_OUVF (0x1 << 2)
#define SR1_OLF (0x1 << 1)
#define SR1_BCF (0x1 << 0)

/*SR2*/
#define SR2_INDEX 0x01
#define SR2_ADDR_OFFSET (0x01 << 5)
#define SR2_ENT (0x1 << 4)
#define SR2_MSEL (0x1 << 3)
#define SR2_TTH (0x1 << 2)

/*SR3*/
#define SR3_INDEX 0x02
#define SR3_ADDR_OFFSET (0x02 << 5)
#define SR3_VSPEN (0x1 << 3)
#define SR3_DCL (0x1 << 2)
#define SR3_ISELH (0x1 << 1)
#define SR3_ISELL (0x1 << 0)

/*SR4*/
#define SR4_INDEX 0x03
#define SR4_ADDR_OFFSET (0x03 << 5)
#define SR4_EN (0x1 << 4)
#define SR4_DLIN (0x1 << 3)
#define SR4_X (0x1 << 2)
#define SR4_VTOP (0x1 << 1)
#define SR4_VBOT (0x1 << 0)

#define ISL9492_I2C_WRITE_TIMES (3)


/*-------------------- STATIC STRUCTURE DECLARATIONS -----------------------------------*/

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

/*------------------------- STATIC DECLARATIONS ----------------------------------------*/

static HI_S32 isl9492_write_reg(LNBCTRL_DEV_PARAM_S* pstParam, HI_U8 u8RegValue);

/*------------------------------------ CODE --------------------------------------------*/

static HI_S32 isl9492_write_reg(LNBCTRL_DEV_PARAM_S* pstParam, HI_U8 u8RegValue)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i = 0;

    for (i = 0; i < ISL9492_I2C_WRITE_TIMES; i++)
    {
        s32Ret = HI_DRV_I2C_Write(pstParam->u32I2CNum, pstParam->u8DevAddr, 0, 0, &u8RegValue, 1);
        if (HI_SUCCESS == s32Ret)
        {
            HI_INFO_TUNER("isl9492 write i2c 0x%02x ok.\n", u8RegValue);
            return HI_SUCCESS;
        }
    }

    HI_INFO_TUNER("isl9492 write i2c fail, %x\n", s32Ret);
    return HI_FAILURE;
}

HI_S32 isl9492_init(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType)
{
    LNBCTRL_DEV_PARAM_S* pstParam;
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8RegValue = 0;

    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = lnbctrl_queue_insert(u32TunerPort, u32I2CNum, u8DevAddr, enDemodDevType);
        if (HI_NULL == pstParam)
        {
            return HI_FAILURE;
        }
    }

    if (!pstParam->bInited)
    {
        /**< 570mA */
        u8RegValue  = SR3_ADDR_OFFSET;
        u8RegValue |= (SR3_VSPEN);
        u8RegValue &= (~SR3_DCL);
        u8RegValue |= (SR3_ISELH);
        u8RegValue &= (~SR3_ISELL); //u8RegValue |= (SR3_ISELL);
        s32Ret = isl9492_write_reg(pstParam, u8RegValue);

        /*0V*/
        u8RegValue = SR4_ADDR_OFFSET;
        s32Ret |= isl9492_write_reg(pstParam, u8RegValue);

        /*Ext 22K*/
        u8RegValue  = SR2_ADDR_OFFSET;
        u8RegValue &= ~(SR2_ENT);
        u8RegValue |= (SR2_MSEL);
        u8RegValue |= (SR2_TTH);
        s32Ret |= isl9492_write_reg(pstParam, u8RegValue);

        if (HI_SUCCESS != s32Ret)
        {
            HI_INFO_TUNER("isl9492 %d init fail\n", u32TunerPort);
            return HI_FAILURE;
        }

        pstParam->enLNBOut = TUNER_LNB_OUT_0V;
        pstParam->bInited = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 isl9492_deInit(HI_U32 u32TunerPort)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    isl9492_set_lnb_out(u32TunerPort, TUNER_LNB_OUT_0V);
    
    if (pstParam->bInited)
    {
        pstParam->bInited = HI_FALSE;
    }
    lnbctrl_queue_remove(u32TunerPort);

    return HI_SUCCESS;
}

HI_S32 isl9492_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    /* If standby, power off */
    if (1 == u32Standby)
    {
        return isl9492_set_lnb_out(u32TunerPort, TUNER_LNB_OUT_0V);
    }
    
    return HI_SUCCESS;
}

HI_S32 isl9492_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut)
{
    LNBCTRL_DEV_PARAM_S* pstParam;
    HI_S32 s32Ret = HI_FAILURE;
    HI_U8 u8RegValue = SR4_ADDR_OFFSET;

    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    /* No need set. */
    if (pstParam->enLNBOut == enOut)
    {
        return HI_SUCCESS;
    }

    if (pstParam->bInited)
    {
        switch (enOut)
        {
            /**< 0V: b 011 0xxxx */
        case TUNER_LNB_OUT_0V:
            u8RegValue &= (~SR4_EN);
            break;

            /**< 13.4V: b 011 11x00 */
        case TUNER_LNB_OUT_13V:
            u8RegValue |= SR4_EN;
            u8RegValue |= SR4_DLIN;
            u8RegValue &= (~SR4_VTOP);
            u8RegValue &= (~SR4_VBOT);
            break;

            /**< 14V: b 011 11x01 */
        case TUNER_LNB_OUT_14V:
            u8RegValue |= SR4_EN;
            u8RegValue |= SR4_DLIN;
            u8RegValue &= (~SR4_VTOP);
            u8RegValue |= SR4_VBOT;
            break;

            /**< 18.7V: b 011 11x10 */
        case TUNER_LNB_OUT_18V:
            u8RegValue |= SR4_EN;
            u8RegValue |= SR4_DLIN;
            u8RegValue |= SR4_VTOP;
            u8RegValue &= (~SR4_VBOT);
            break;

            /**< 20V: b 011 11x11 */
        case TUNER_LNB_OUT_19V:
            u8RegValue |= SR4_EN;
            u8RegValue |= SR4_DLIN;
            u8RegValue |= SR4_VTOP;
            u8RegValue |= SR4_VBOT;
            break;
        default:
            return HI_FAILURE;
            break;
        }

        s32Ret = isl9492_write_reg(pstParam, u8RegValue);
    }
    else
    {
        HI_INFO_TUNER("isl9492 %d set lnb out %dV fail.\n", u32TunerPort, enOut);
    }

    if (HI_SUCCESS == s32Ret)
    {
        pstParam->enLNBOut = enOut;
    }
    return s32Ret;
}

/* Use extend 22K */
HI_S32 isl9492_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K)
{
    HI_INFO_TUNER("isl9492 only support extend 22K now.\n");
    return HI_FAILURE;
}

/* Use extend 22K */
HI_S32 isl9492_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone)
{
    HI_INFO_TUNER("isl9492 only support extend 22K now.\n");
    return HI_FAILURE;
}

HI_S32 isl9492_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg)
{
    HI_INFO_TUNER("isl9492 only support extend 22K now.\n");
    return HI_FAILURE;
}

HI_S32 isl9492_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg)
{
    HI_INFO_TUNER("isl9492 only support extend 22K now.\n");
    return HI_FAILURE;
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif/*  __cpluscplus */
#endif /*  __cpluscplus */

/** @}*/
