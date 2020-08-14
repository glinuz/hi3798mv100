/******************************************************************************

  Copyright (C), 2015, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : pq_mng_comm.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2015/09/07
  Description   :

******************************************************************************/
#include <linux/fs.h>

#include "hi_osal.h"
#include "hi_drv_pq.h"
#include "drv_pq_comm.h"
#include "pq_hal_comm.h"
#include "hi_drv_mem.h"

DEFINE_SEMAPHORE(g_PqMutex);

#define DRV_PQ_LOCK()      \
    do{         \
        if(down_interruptible(&g_PqMutex))   \
        {       \
            HI_ERR_PQ("DRV PQ lock error!\n");  \
        }       \
    }while(0)

#define DRV_PQ_UNLOCK()      \
    do{         \
        up(&g_PqMutex);    \
    }while(0)

#define MIN_LEN(x, y) ( (x) > (y) ? (y) : (x) )


static PQ_SOURCE_MODE_E sg_enSourceMode = SOURCE_MODE_NO;
static PQ_OUTPUT_MODE_E sg_enOutputMode = OUTPUT_MODE_NO;
static PQ_ALG_REG_S*   psg_astAlgRegModule[HI_PQ_MODULE_BUTT] = { NULL };
static HI_U8 sg_au8MoudleCtrl[HI_PQ_MODULE_BUTT][SOURCE_MODE_ALL];

HI_S32 PQ_COMM_SetPqStatus(PQ_STATUS_S* pstPqStatus)
{
    PQ_CHECK_NULL_PTR(pstPqStatus);

    DRV_PQ_LOCK();
    memcpy(&g_stPqStatus, pstPqStatus, sizeof(PQ_STATUS_S));
    DRV_PQ_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 PQ_COMM_GetPqStatus(PQ_STATUS_S* pstPqStatus)
{
    PQ_CHECK_NULL_PTR(pstPqStatus);

    memcpy(pstPqStatus, &g_stPqStatus, sizeof(PQ_STATUS_S));

    return HI_SUCCESS;
}


HI_S32 PQ_COMM_CheckChipName(HI_CHAR* pchChipName)
{
    HI_S32 s32Ret = HI_FAILURE;
    PQ_CHECK_NULL_PTR(pchChipName);

#if defined(CHIP_TYPE_hi3798cv200)
    s32Ret = HI_OSAL_Strncmp(pchChipName , "HI_CHIP_TYPE_HI3798C", strlen("HI_CHIP_TYPE_HI3798C"));
#elif defined(CHIP_TYPE_hi3798mv200)||defined(CHIP_TYPE_hi3798mv200_a)
    s32Ret = HI_OSAL_Strncmp(pchChipName , "HI_CHIP_TYPE_HI3798M", strlen("HI_CHIP_TYPE_HI3798M"));
#else
    HI_ERR_PQ("unknown Chip Type \r\n");
    return HI_FAILURE;
#endif

    if (0 != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 PQ_COMM_AlgRegister( HI_PQ_MODULE_E       enModule,
                            PQ_REG_TYPE_E                enRegType,
                            PQ_BIN_ADAPT_TYPE_E          enAdaptType,
                            const HI_U8*                 pu8AlgName,
                            HI_VOID*                     pData,
                            const stPQAlgFuncs*          pFuncs)
{
    PQ_ALG_REG_S* pstAlgReg;
    HI_U32   u32MinLen = 0;

    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return HI_FAILURE;
    }

    if (HI_NULL == psg_astAlgRegModule[enModule])
    {
        pstAlgReg = (PQ_ALG_REG_S*)HI_KMALLOC(HI_ID_PQ, sizeof(PQ_ALG_REG_S), GFP_KERNEL);
        if (HI_NULL == pstAlgReg)
        {
            HI_ERR_PQ("ID: %d, register failure!", (HI_U32)enRegType);
            return HI_FAILURE;
        }

        psg_astAlgRegModule[enModule] = pstAlgReg;
    }
    else
    {
        pstAlgReg = psg_astAlgRegModule[enModule];
    }

    pstAlgReg->u32AlgID      = (HI_U32)enModule;
    pstAlgReg->u32TypeID     = (HI_U32)enRegType;
    pstAlgReg->u32AdapeType  = (HI_U32)enAdaptType;
    pstAlgReg->Data          = pData;
    pstAlgReg->pFun          = pFuncs;

    u32MinLen = MIN_LEN( sizeof(pstAlgReg->u8AlgName) - 1, strlen(pu8AlgName));
    memset(pstAlgReg->u8AlgName, 0, sizeof(pstAlgReg->u8AlgName));
    memcpy(pstAlgReg->u8AlgName, pu8AlgName, u32MinLen);

    return HI_SUCCESS;
}

HI_S32 PQ_COMM_AlgUnRegister( HI_PQ_MODULE_E      enModule)
{
    PQ_ALG_REG_S* pstAlgReg;

    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return HI_FAILURE;
    }

    pstAlgReg = psg_astAlgRegModule[enModule];

    pstAlgReg->u32AlgID      =  HI_NULL;
    pstAlgReg->u32TypeID     =  HI_NULL;
    pstAlgReg->u32AdapeType  =  HI_NULL;
    pstAlgReg->Data          =  HI_NULL;
    pstAlgReg->pFun          =  HI_NULL;

    memset(pstAlgReg->u8AlgName, 0, sizeof(pstAlgReg->u8AlgName));

    HI_KFREE(HI_ID_PQ, pstAlgReg);

    psg_astAlgRegModule[enModule] = HI_NULL;
    return HI_SUCCESS;
}

PQ_ALG_REG_S* PQ_COMM_GetAlg(HI_PQ_MODULE_E    enModule )
{
    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return HI_NULL;
    }

    return psg_astAlgRegModule[enModule];
}

HI_U32 PQ_COMM_GetAlgTypeID(HI_PQ_MODULE_E    enModule )
{
    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return REG_TYPE_BUTT;
    }

    if (HI_NULL == psg_astAlgRegModule[enModule])
    {
        return REG_TYPE_BUTT;
    }

    return psg_astAlgRegModule[enModule]->u32TypeID;
}

HI_U32 PQ_COMM_GetAlgAdapeType(HI_PQ_MODULE_E    enModule )
{
    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return PQ_BIN_ADAPT_BUTT;
    }

    if (HI_NULL == psg_astAlgRegModule[enModule])
    {
        return PQ_BIN_ADAPT_BUTT;
    }

    return psg_astAlgRegModule[enModule]->u32AdapeType;
}

HI_U8* PQ_COMM_GetAlgName(HI_PQ_MODULE_E    enModule )
{
    if (HI_PQ_MODULE_BUTT <= enModule)
    {
        return HI_NULL;
    }

    if (HI_NULL == psg_astAlgRegModule[enModule])
    {
        return HI_NULL;
    }

    return psg_astAlgRegModule[enModule]->u8AlgName;
}

HI_U8 PQ_COMM_GetMoudleCtrl(HI_PQ_MODULE_E  enModule, PQ_SOURCE_MODE_E enSrcMode)
{
    if ((enModule >= HI_PQ_MODULE_BUTT)
        || (enSrcMode >= SOURCE_MODE_ALL))
    {
        return HI_FALSE;
    }

    return sg_au8MoudleCtrl[enModule][enSrcMode];
}

HI_VOID PQ_COMM_SetMoudleCtrl(HI_PQ_MODULE_E  enModule, PQ_SOURCE_MODE_E enSrcMode, HI_U8 u8OnOff)
{
    if ((enModule >= HI_PQ_MODULE_BUTT)
        || (enSrcMode >= SOURCE_MODE_ALL))
    {
        return;
    }

    sg_au8MoudleCtrl[enModule][enSrcMode] = u8OnOff;
    return;
}

PQ_SOURCE_MODE_E PQ_COMM_CheckSourceMode(HI_U32 u32Width)
{
    if (720 >= u32Width) /* SD Source */
    {
        return SOURCE_MODE_SD;
    }
    else if ((1280 >= u32Width) && (720 < u32Width)) /* HD Source */
    {
        return SOURCE_MODE_HD;
    }
    else if ((1920 >= u32Width) && (1280 < u32Width)) /* FHD Source */
    {
        return SOURCE_MODE_HD;
    }
    else if ((4096 >= u32Width) && (1920 < u32Width)) /* UHD Source */
    {
        return SOURCE_MODE_UHD;
    }

    return SOURCE_MODE_NO;
}

PQ_OUTPUT_MODE_E PQ_COMM_CheckOutputMode(HI_S32 s32Width)
{
    if ((720 >= s32Width) && (0 < s32Width)) /* SD Source */
    {
        return OUTPUT_MODE_SD;
    }
    else if ((1280 >= s32Width) && (720 < s32Width)) /* HD Source */
    {
        return OUTPUT_MODE_HD;
    }
    else if ((1920 >= s32Width) && (1280 < s32Width)) /* FHD Source */
    {
        return OUTPUT_MODE_HD;
    }
    else if ((4096 >= s32Width) && (1920 < s32Width)) /* UHD Source */
    {
        return OUTPUT_MODE_UHD;
    }

    return OUTPUT_MODE_NO;

}

PQ_SOURCE_MODE_E PQ_COMM_GetSourceMode(HI_VOID)
{
    return sg_enSourceMode;
}

HI_VOID PQ_COMM_SetSourceMode(PQ_SOURCE_MODE_E enMode)
{
    sg_enSourceMode = enMode;
    return;
}

PQ_OUTPUT_MODE_E PQ_COMM_GetOutputMode(HI_VOID)
{
    return sg_enOutputMode;
}

HI_VOID PQ_COMM_SetOutputMode(PQ_OUTPUT_MODE_E enMode)
{
    sg_enOutputMode = enMode;
    return;
}



