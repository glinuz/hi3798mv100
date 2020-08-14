/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mps8125.c
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
 * \brief For LNB control chipset MPS8125.
 */

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include "drv_lnbctrl.h"
#include "drv_demod.h"
#include "mps8125.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

/*-------------------- STATIC STRUCTURE DECLARATIONS -----------------------------------*/

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

/*------------------------- STATIC DECLARATIONS ----------------------------------------*/

/*------------------------------------ CODE --------------------------------------------*/

HI_S32 mps8125_init(HI_U32 u32TunerPort, HI_U32 u32I2CNum, HI_U8 u8DevAddr, HI_UNF_DEMOD_DEV_TYPE_E enDemodDevType)
{
    LNBCTRL_DEV_PARAM_S* pstParam;
    HI_S32 s32Ret = HI_FAILURE;

    /* Make a node for device */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = lnbctrl_queue_insert(u32TunerPort, u32I2CNum, u8DevAddr, enDemodDevType);
        if (HI_NULL == pstParam)
        {
            return HI_FAILURE;
        }
    }

    /*if (!pstParam->bInited)*/
    {
        pstParam->bInited = HI_TRUE;
        
        /* MPS8125 power off when init */
        switch (pstParam->enDemodDevType)
        {
            /* AVL6211: MPS8125's power controlled by demod */
        case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
            s32Ret = avl6211_lnbctrl_power(u32TunerPort, 0);
            if (HI_SUCCESS == s32Ret)
            {
                pstParam->enLNBOut = TUNER_LNB_OUT_0V;
            }
#endif
            return s32Ret;

        case HI_UNF_DEMOD_DEV_TYPE_3136:
        case HI_UNF_DEMOD_DEV_TYPE_3136I:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
            s32Ret = hi3136_lnbctrl_power(u32TunerPort, 0);
            if (HI_SUCCESS == s32Ret)
            {
                pstParam->enLNBOut = TUNER_LNB_OUT_0V;
            }
#endif
            return s32Ret;

        
        case HI_UNF_DEMOD_DEV_TYPE_NONE:
        case HI_UNF_DEMOD_DEV_TYPE_3130I:
        case HI_UNF_DEMOD_DEV_TYPE_3130E:
        case HI_UNF_DEMOD_DEV_TYPE_J83B:
        default:
            HI_ERR_TUNER("mps8125 init fail.\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 mps8125_deInit(HI_U32 u32TunerPort)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    /* MPS8125 power off */
    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's power controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        avl6211_lnbctrl_power(u32TunerPort, 0);
#endif
        break;

    case HI_UNF_DEMOD_DEV_TYPE_3136:
    case HI_UNF_DEMOD_DEV_TYPE_3136I:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
        hi3136_lnbctrl_power(u32TunerPort, 0);
#endif
        break;

    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        break;
    }

    lnbctrl_queue_remove(u32TunerPort);
    return HI_SUCCESS;
}

#if 0
HI_S32 mps8125_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
    LNBCTRL_DEV_PARAM_S* pstParam;
    HI_S32 s32Ret = HI_FAILURE;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    /* If standby, MPS8125 power off */
    if (1 == u32Standby)
    {
        switch (pstParam->enDemodDevType)
        {
            /* AVL6211: MPS8125's power controlled by demod */
        case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
            s32Ret = avl6211_lnbctrl_power(u32TunerPort, 0);
            if (HI_SUCCESS == s32Ret)
            {
                pstParam->enLNBOut = TUNER_LNB_OUT_0V;
            }
#endif
            return s32Ret;

        case HI_UNF_DEMOD_DEV_TYPE_3136:
        case HI_UNF_DEMOD_DEV_TYPE_3136I:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
            s32Ret = hi3136_lnbctrl_power(u32TunerPort, 0);
            if (HI_SUCCESS == s32Ret)
            {
                pstParam->enLNBOut = TUNER_LNB_OUT_0V;
            }
#endif
            return s32Ret;
        
        case HI_UNF_DEMOD_DEV_TYPE_NONE:
        case HI_UNF_DEMOD_DEV_TYPE_3130I:
        case HI_UNF_DEMOD_DEV_TYPE_3130E:
        case HI_UNF_DEMOD_DEV_TYPE_J83B:
        default:
            break;
        }
    }

    return HI_SUCCESS;
}
#endif

HI_S32 mps8125_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut)
{
    LNBCTRL_DEV_PARAM_S* pstParam;
    HI_S32 s32Ret = HI_FAILURE;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    /* MPS8125 power off */
    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's lnb out controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        /* If 0V, stop power supply */
        if (TUNER_LNB_OUT_0V == enOut)
        {
            s32Ret = avl6211_lnbctrl_power(u32TunerPort, 0);
        }
        else
        {
            /* If current 0V, start power supply */
            if (TUNER_LNB_OUT_0V == pstParam->enLNBOut)
            {
                s32Ret = avl6211_lnbctrl_power(u32TunerPort, 1);
            }
            s32Ret = avl6211_set_lnb_out(u32TunerPort, enOut);
            
        }
        /* Save status */
        if (HI_SUCCESS == s32Ret)
        {
            pstParam->enLNBOut = enOut;
        }
#endif
        return s32Ret;

    case HI_UNF_DEMOD_DEV_TYPE_3136:
    case HI_UNF_DEMOD_DEV_TYPE_3136I:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_HI3136)
        /* If 0V, stop power supply */
        if (TUNER_LNB_OUT_0V == enOut)
        {
            s32Ret = hi3136_lnbctrl_power(u32TunerPort, 0);
        }
        else
        {
            /* If current 0V, start power supply */
            if (TUNER_LNB_OUT_0V == pstParam->enLNBOut)
            {
                s32Ret = hi3136_lnbctrl_power(u32TunerPort, 1);
            }
            s32Ret = hi3136_set_lnb_out(u32TunerPort, enOut);
            
        }
        /* Save status */
        if (HI_SUCCESS == s32Ret)
        {
            pstParam->enLNBOut = enOut;
        }
#endif
        return s32Ret;

    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        HI_ERR_TUNER("mps8125 set lnb out fail.\n");
        return HI_FAILURE;
    }
}

#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
HI_S32 mps8125_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's 22K controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        return avl6211_send_continuous_22K(u32TunerPort, u32Continuous22K);
#endif
    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        HI_ERR_TUNER("mps8125 send continuous 22K fail.\n");
        return HI_FAILURE;
    }
}

HI_S32 mps8125_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's 22K controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        return avl6211_send_tone(u32TunerPort, u32Tone);
#endif
    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        HI_ERR_TUNER("mps8125 send tone fail.\n");
        return HI_FAILURE;
    }
}

HI_S32 mps8125_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's 22K controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        return avl6211_DiSEqC_send_msg(u32TunerPort, pstSendMsg);
#endif
    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        HI_ERR_TUNER("mps8125 send tone fail.\n");
        return HI_FAILURE;
    }
}

HI_S32 mps8125_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg)
{
    LNBCTRL_DEV_PARAM_S* pstParam;

    /* Find node */
    pstParam = lnbctrl_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    switch (pstParam->enDemodDevType)
    {
        /* AVL6211: MPS8125's 22K controlled by demod */
    case HI_UNF_DEMOD_DEV_TYPE_AVL6211:
#if defined(HI_BOOT_DEMOD_DEV_TYPE_AVL6211)
        return avl6211_DiSEqC_recv_msg(u32TunerPort, pstRecvMsg);
#endif
    case HI_UNF_DEMOD_DEV_TYPE_NONE:
    case HI_UNF_DEMOD_DEV_TYPE_3130I:
    case HI_UNF_DEMOD_DEV_TYPE_3130E:
    case HI_UNF_DEMOD_DEV_TYPE_J83B:
    default:
        HI_ERR_TUNER("mps8125 send tone fail.\n");
        return HI_FAILURE;
    }
}
#endif

#ifdef __cplusplus
 #if __cplusplus
}
 #endif/*  __cpluscplus */
#endif /*  __cpluscplus */

/** @}*/
