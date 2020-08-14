
/******************************************************************************
  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.
******************************************************************************
File Name     : drv_display.h
Version       : Initial Draft
Author        : Hisilicon multimedia software group
Created       : 2012/12/30
Last Modified :
Description   :
Function List :
History       :
******************************************************************************/
#include "drv_display.h"
#include "drv_disp_com.h"
#include "drv_disp_priv.h"
#include "drv_disp_hal.h"
#include "drv_disp_da.h"
#ifdef __DISP_PLATFORM_BOOT__
#include "hi_drv_pq.h"
#endif
#ifdef __DISP_PLATFORM_BOOT__
#include "hi_drv_hdmi.h"
#endif

#ifndef __DISP_PLATFORM_BOOT__
#include "drv_pq_ext.h"
#include "drv_mcvn.h"
#endif
#ifdef HI_DISP_BUILD_FULL
#include "drv_disp_isr.h"
#include "drv_disp_cast.h"
#include "hi_drv_module.h"
#include "drv_hdmi_ext.h"
#include "hi_drv_sys.h"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/******************************************************************************
    global object
******************************************************************************/
static volatile HI_S32 s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_CLOSE;
static DISP_DEV_S s_stDisplayDevice;

/* OPTM ISR handle */
HI_U32 g_DispIrqHandle = 0;

#ifdef __DISP_PLATFORM_BOOT__

#else
static HDMI_EXPORT_FUNC_S* s_pstHDMIFunc;
#endif


/******************************************************************************
    local function and macro
******************************************************************************/
#define DEF_DRV_DISP_INTER_FUNCTION_AND_MACRO_START_HERE

#define DispCheckDeviceState()    \
    {                                \
        if (DISP_DEVICE_STATE_OPEN != s_s32DisplayGlobalFlag)  \
        {                            \
            DISP_ERROR("DISP ERROR! DISP is not inited in %s,status:%d!\n", __FUNCTION__,s_s32DisplayGlobalFlag); \
            return HI_ERR_DISP_NO_INIT;  \
        }                             \
    }

#define DispCheckCastHandleValid(handle)    \
    {                                \
        if ( ((handle >> 16) != HI_ID_DISP)      \
             || (((handle & 0xffff) != HI_DRV_DISPLAY_0)    \
                 &&((handle & 0xffff) != HI_DRV_DISPLAY_1)    \
                 && ((handle & 0xffff) != HI_DRV_DISPLAY_2)) )  \
        {                                                  \
            DISP_ERROR("DISP ERROR! bad cast handle in  %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_INVALID_PARA;  \
        }                             \
    }


#define DispCheckNullPointer(ptr) \
    {                                \
        if (!ptr)                    \
        {                            \
            DISP_ERROR("DISP ERROR! Input null pointer in %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_NULL_PTR;  \
        }                             \
    }

#if 0

#define DispCheckID(id)    \
    {                                \
        if ( (id > HI_DRV_DISPLAY_BUTT) || (id > HI_DRV_DISPLAY_BUTT))  \
        {                            \
            DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_INVALID_PARA;  \
        }                             \
    }
#endif

#define DispGetPointerByID(id, ptr)    \
    {                                      \
        if (id >= HI_DRV_DISPLAY_BUTT)     \
        {                                     \
            DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
            return HI_ERR_DISP_INVALID_PARA;  \
        }                                     \
        ptr = &s_stDisplayDevice.stDisp[id - HI_DRV_DISPLAY_0]; \
    }

#define DispGetPointerByIDNoReturn(id, ptr)    \
    {                                      \
        if (id >= HI_DRV_DISPLAY_BUTT)     \
        {                                     \
            DISP_ERROR("DISP ERROR! Invalid display in %s!\n", __FUNCTION__); \
        }                                     \
        ptr = &s_stDisplayDevice.stDisp[id - HI_DRV_DISPLAY_0]; \
    }


#define DispShouldBeOpened(id) \
    {                              \
        if(!DISP_IsOpened(id))     \
        {                          \
            DISP_WARN("DISP ERROR! Display is not opened!\n"); \
            return HI_ERR_DISP_NOT_OPEN;  \
        }                          \
    }

HI_VOID DispSetHardwareState(HI_VOID)
{
    s_stDisplayDevice.bHwReseted = HI_TRUE;
    return;
}

HI_VOID DispClearHardwareState(HI_VOID)
{
    s_stDisplayDevice.bHwReseted = HI_FALSE;
    return;
}


HI_VOID DispResetHardware(HI_VOID)
{
    if (!s_stDisplayDevice.bHwReseted)
    {
        s_stDisplayDevice.stIntfOpt.PF_ResetHardware();
        s_stDisplayDevice.bHwReseted = HI_TRUE;

        DISP_WARN("========DispResetHardware=======\n");
    }
}
HI_VOID DispCloseClkResetModule(HI_VOID)
{
    s_stDisplayDevice.stIntfOpt.PF_CloseClkResetModule();
}


#ifndef __DISP_PLATFORM_BOOT__
HI_S32 DispGetHdmiFunction(void)
{
    HI_S32 nRet = HI_SUCCESS;

    s_pstHDMIFunc = HI_NULL;
    nRet = HI_DRV_MODULE_GetFunction(HI_ID_HDMI, (HI_VOID**)&s_pstHDMIFunc);

    if ((nRet != HI_SUCCESS) || (s_pstHDMIFunc == HI_NULL))
    {
        DISP_ERROR("DISP_get HDMI func failed!");
        return HI_FAILURE;
    }
    return nRet;
}
#endif

HI_S32 DispSearchCastHandle(HI_VOID** cast_ptr, HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;

    if (DISP_IsOpened(enDisp))
    {
        DispGetPointerByID(enDisp, pstDisp);

        if (pstDisp->Cast_ptr != 0)
        {
            *cast_ptr = pstDisp->Cast_ptr;
            return HI_SUCCESS;
        }
    }
    else
    {
        DISP_ERROR("Disp: %d not opened!\n", enDisp);
        return HI_ERR_DISP_NOT_OPEN;
    }

    return HI_ERR_DISP_NULL_PTR;
}

HI_S32 DispCheckMaster(HI_DRV_DISPLAY_E enDisp, HI_BOOL bDetach)
{
    if (enDisp != HI_DRV_DISPLAY_0)
    {
        DISP_ERROR("DISP ERROR! Not support display %d as master!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (bDetach)
    {
        // if detach, check  'eMaster is enDisp'
        if (s_stDisplayDevice.stAttchDisp.eMaster != enDisp)
        {
            DISP_ERROR("DISP ERROR! Display %d is not master!\n", (HI_S32)enDisp);
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        // if attach, check  'eMaster is exist'; only support one master diaplay.
        if (s_stDisplayDevice.stAttchDisp.eMaster != HI_DRV_DISPLAY_BUTT)
        {
            DISP_ERROR("DISP ERROR! Display master has existed!\n");
            return HI_ERR_DISP_INVALID_OPT;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DispCheckSlace(HI_DRV_DISPLAY_E enDisp, HI_BOOL bDetach)
{
    if (enDisp != HI_DRV_DISPLAY_1)
    {
        DISP_ERROR("DISP ERROR! Not support display %d as slave!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (bDetach)
    {
        // if detach, check  'eSlave is enDisp'
        if (s_stDisplayDevice.stAttchDisp.eSlave != enDisp)
        {
            DISP_ERROR("DISP ERROR! Display %d is not slave!\n", (HI_S32)enDisp);
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        // if attach, check  'eSlave is exist'; only support one slave diaplay.
        if (s_stDisplayDevice.stAttchDisp.eSlave != HI_DRV_DISPLAY_BUTT)
        {
            DISP_ERROR("DISP ERROR! Display slave has existed!\n");
            return HI_ERR_DISP_INVALID_OPT;
        }
    }

    return HI_SUCCESS;
}


HI_BOOL DispFmtIsStandDefinition(HI_DRV_DISP_FMT_E enEncFmt)
{
    if ((enEncFmt >= HI_DRV_DISP_FMT_PAL) && (enEncFmt <= HI_DRV_DISP_FMT_1440x480i_60) )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_DRV_DISP_FMT_E DispTransferFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEncFmt)
{
    if (HI_DRV_DISPLAY_1 == enDisp)
    {
        if (   (HI_DRV_DISP_FMT_PAL <= enEncFmt)
               && (HI_DRV_DISP_FMT_PAL_Nc >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x576i_50;
        }

        if (   (HI_DRV_DISP_FMT_SECAM_SIN <= enEncFmt)
               && (HI_DRV_DISP_FMT_SECAM_H >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x576i_50;
        }

        if (   (HI_DRV_DISP_FMT_PAL_M <= enEncFmt)
               && (HI_DRV_DISP_FMT_NTSC_443 >= enEncFmt) )
        {
            return HI_DRV_DISP_FMT_1440x480i_60;
        }

        return enEncFmt;
    }
    else
    {
        return enEncFmt;
    }
}

HI_S32 DispSetMasterAndSlace(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    s_stDisplayDevice.stAttchDisp.eMaster = enMaster;
    s_stDisplayDevice.stAttchDisp.eSlave  = enSlave;

    return HI_SUCCESS;
}

HI_S32 DispCheckReadyForOpen(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;
    DispGetPointerByID(enDisp, pstDisp);

    if (HI_DRV_DISP_FMT_BUTT == pstDisp->stSetting.enFormat)
    {
        DISP_WARN("WARNING! Fmt is not set!\n");
        return HI_ERR_DISP_NOT_SUPPORT_FMT;
    }

    return HI_SUCCESS;
}

/*==========================================================================
    interface control
*/
HI_VOID DispCleanIntf(DISP_INTF_S* pstIntf)
{
    DISP_MEMSET(pstIntf, 0, sizeof(DISP_INTF_S));

    pstIntf->bOpen = HI_FALSE;
    pstIntf->bLinkVenc = HI_FALSE;
    pstIntf->eVencId = DISP_VENC_MAX;

    /*
        for(i=0; i<DISP_VENC_SIGNAL_MAX_NUMBER; i++)
        {
            pstIntf->eSignal[i] = HI_DRV_DISP_VDAC_NONE;
        }
    */
    pstIntf->stIf.eID = HI_DRV_DISP_INTF_ID_MAX;
    pstIntf->stIf.u8VDAC_Y_G  = HI_DISP_VDAC_INVALID_ID;
    pstIntf->stIf.u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
    pstIntf->stIf.u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;
    pstIntf->stIf.bDacSync = HI_TRUE;

    return;
}

HI_VOID DispCleanAllIntf(DISP_S* pstDisp)
{
    HI_S32 i;

    for (i = 0; i < (HI_S32)HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        DispCleanIntf(&pstDisp->stSetting.stIntf[i]);
    }

    return;
}


/*if dac is busy ,release the  interface ,in order to used for new config is valid!*/
HI_S32 DispPrepareVDAC( HI_U32 u32DacId)
{
    HI_U32 i, j;
    DISP_S* pstDisp;
    DISP_INTF_S* pstIt;

    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_ReleaseIntf2);

    for ( i = HI_DRV_DISPLAY_0; i < HI_DRV_DISPLAY_2 ; i++)
    {
        DispGetPointerByID((HI_DRV_DISPLAY_E) i, pstDisp);
        //DISP_INFO("check  dac *********(%d)\n",u32DacId);

        for ( j = HI_DRV_DISP_INTF_YPBPR0; j <= HI_DRV_DISP_INTF_VGA0 ; j++)
        {
            pstIt = &pstDisp->stSetting.stIntf[j];
            //DISP_INFO("*disp%d*[%d]***(%d)(%d)(%d)***(%d)\n",i,j,pstIt->stIf.u8VDAC_Y_G,pstIt->stIf.u8VDAC_Pb_B,pstIt->stIf.u8VDAC_Pr_R,pstDisp->stSetting.stIntf[i].bOpen);

            if ( (pstIt->stIf.u8VDAC_Y_G == u32DacId)
                 || (pstIt->stIf.u8VDAC_Pb_B == u32DacId)
                 || (pstIt->stIf.u8VDAC_Pr_R == u32DacId)
               )
            {
                // s1 release vdac
                //DISP_INFO("del **disp%d*[%d]***(%d)(%d)(%d)***(%d)\n", i, j, pstIt->stIf.u8VDAC_Y_G, pstIt->stIf.u8VDAC_Pb_B, pstIt->stIf.u8VDAC_Pr_R, pstDisp->stSetting.stIntf[j].bOpen);
                pfOpt->PF_ReleaseIntf2(i, pstIt);
                /* clean */

                DispCleanIntf(pstIt);
                continue;
            }
        }
    }

    return HI_SUCCESS;
}
#ifdef __DISP_PLATFORM_BOOT__
HI_S32 DispPrepareHDMI( HI_DRV_DISP_INTF_ID_E enHDMIId)
{
    return HI_SUCCESS;
}
#else
HI_S32 DispPrepareHDMI( HI_DRV_DISP_INTF_ID_E enHDMIId)
{
    HI_U32 i;
    DISP_S* pstDisp;
    DISP_INTF_S* pstIt;

    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_ReleaseIntf2);

    for ( i = HI_DRV_DISPLAY_0; i < HI_DRV_DISPLAY_2 ; i++)
    {
        DispGetPointerByID((HI_DRV_DISPLAY_E) i, pstDisp);
        pstIt = &pstDisp->stSetting.stIntf[enHDMIId];
		
        if ((!DispGetHdmiFunction())) 
		{
            if (s_pstHDMIFunc->pfnHdmiDetach && s_pstHDMIFunc->pfnHdmiAttach)
            {
               s_pstHDMIFunc->pfnHdmiDetach(HI_UNF_HDMI_ID_0);
			#if defined (HI_HDMI_SUPPORT_2_0)
               s_pstHDMIFunc->pfnHdmiClose(HI_UNF_HDMI_ID_0);
			#endif
            }
			pfOpt->PF_ReleaseIntf2(i, pstIt);
			DispCleanIntf(pstIt);
        } 
        continue;
    }

    return HI_SUCCESS;
}

#endif

#ifdef __DISP_PLATFORM_BOOT__
HI_S32 DispPrepareLCD_BT1120( HI_DRV_DISP_INTF_ID_E enIntfId)
{
    return HI_SUCCESS;
}
#else
HI_S32 DispPrepareLCD_BT1120( HI_DRV_DISP_INTF_ID_E enIntfId)
{
    HI_U32 i, j;
    DISP_S* pstDisp;
    DISP_INTF_S* pstIt;

    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_ReleaseIntf2);

    /*Because SOC  GPIO limit ,at same time LCD or BT1120 only support by DHD1 ,*/
    for ( i = HI_DRV_DISPLAY_1; i < HI_DRV_DISPLAY_2 ; i++)
    {
        DispGetPointerByID((HI_DRV_DISPLAY_E) i, pstDisp);
        for ( j = HI_DRV_DISP_INTF_BT1120_0; j <= HI_DRV_DISP_INTF_LCD2 ; j++)
        {
            pstIt = &pstDisp->stSetting.stIntf[j];
            pfOpt->PF_ReleaseIntf2(i, pstIt);
            DispCleanIntf(pstIt);
            continue;
        }
    }
    return HI_SUCCESS;
}

#endif

#define DEF_DRV_DISP_INTERFACE_CONTROL_START_HERE

HI_S32 DispCheckIntfValid(HI_DRV_DISP_INTF_S* pstIntf)
{
    if (pstIntf->eID >= HI_DRV_DISP_INTF_ID_MAX)
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (  (pstIntf->eID == HI_DRV_DISP_INTF_YPBPR0)
          || (pstIntf->eID == HI_DRV_DISP_INTF_VGA0)
          || (pstIntf->eID == HI_DRV_DISP_INTF_RGB0)
       )
    {
        if (  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
              || (pstIntf->u8VDAC_Pb_B >= HI_DISP_VDAC_MAX_NUMBER)
              || (pstIntf->u8VDAC_Pr_R >= HI_DISP_VDAC_MAX_NUMBER)
           )
        {
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    if (pstIntf->eID == HI_DRV_DISP_INTF_SVIDEO0)
    {
        if (  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
              || (pstIntf->u8VDAC_Pb_B >= HI_DISP_VDAC_MAX_NUMBER)
              || (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
           )
        {
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    if (pstIntf->eID == HI_DRV_DISP_INTF_CVBS0)
    {
        if (  (pstIntf->u8VDAC_Y_G  >= HI_DISP_VDAC_MAX_NUMBER)
              || (pstIntf->u8VDAC_Pb_B != HI_DISP_VDAC_INVALID_ID)
              || (pstIntf->u8VDAC_Pr_R != HI_DISP_VDAC_INVALID_ID)
           )
        {
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    return HI_SUCCESS;
}

HI_S32 DispPrepareInft(HI_DRV_DISP_INTF_S* pstIntf)
{
    switch (pstIntf->eID)
    {
        case HI_DRV_DISP_INTF_YPBPR0:
        case HI_DRV_DISP_INTF_VGA0:
        case HI_DRV_DISP_INTF_RGB0:
            DispPrepareVDAC(pstIntf->u8VDAC_Y_G);
            DispPrepareVDAC(pstIntf->u8VDAC_Pb_B);
            DispPrepareVDAC(pstIntf->u8VDAC_Pr_R);
            break;

        case HI_DRV_DISP_INTF_SVIDEO0:
            DispPrepareVDAC(pstIntf->u8VDAC_Y_G);
            DispPrepareVDAC(pstIntf->u8VDAC_Pb_B);
            break;

        case HI_DRV_DISP_INTF_CVBS0:
            DispPrepareVDAC(pstIntf->u8VDAC_Y_G);
            break;

        case HI_DRV_DISP_INTF_HDMI0:
        case HI_DRV_DISP_INTF_HDMI1:
        case HI_DRV_DISP_INTF_HDMI2:
            DispPrepareHDMI(pstIntf->eID);
            break;

        case HI_DRV_DISP_INTF_BT1120_0:
        case HI_DRV_DISP_INTF_BT1120_1:
        case HI_DRV_DISP_INTF_BT1120_2:
        case HI_DRV_DISP_INTF_LCD0:
        case HI_DRV_DISP_INTF_LCD1:
        case HI_DRV_DISP_INTF_LCD2:
            DispPrepareLCD_BT1120(pstIntf->eID);
            break;

        default:
            break;
    }

    return HI_SUCCESS;
}



HI_BOOL DispCheckIntfExist(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S* pstIntf)
{
    DISP_S* pstDisp;
    HI_DRV_DISP_INTF_S* pstIntf2;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    pstIntf2 = &pstDisp->stSetting.stIntf[pstIntf->eID].stIf;

    if (pstIntf->eID >= HI_DRV_DISP_INTF_ID_MAX)
        return HI_FALSE;

    if ((pstIntf->eID >= HI_DRV_DISP_INTF_BT1120_0) && (pstIntf->eID <= HI_DRV_DISP_INTF_LCD2))
    {
            if (  (HI_TRUE == pstDisp->stSetting.stIntf[pstIntf->eID].bOpen) && (pstIntf->eID == pstIntf2->eID))
                return HI_TRUE;
            else
                return HI_FALSE;
    }

    if (  (HI_TRUE == pstDisp->stSetting.stIntf[pstIntf->eID].bOpen)
          && (pstIntf->eID         == pstIntf2->eID)
          && (pstIntf->u8VDAC_Y_G  == pstIntf2->u8VDAC_Y_G)
          && (pstIntf->u8VDAC_Pb_B == pstIntf2->u8VDAC_Pb_B)
          && (pstIntf->u8VDAC_Pr_R == pstIntf2->u8VDAC_Pr_R)
       )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

HI_BOOL DispCheckIntfExistByType(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S* pstIntf)
{
    DISP_S* pstDisp;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    if (HI_TRUE == pstDisp->stSetting.stIntf[pstIntf->eID].bOpen)
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

DISP_INTF_S* DispGetIntfPtr(DISP_S* pstDisp, HI_DRV_DISP_INTF_ID_E eID)
{
    return &pstDisp->stSetting.stIntf[eID];
}

HI_S32 DispAddIntf(DISP_S* pstDisp, HI_DRV_DISP_INTF_S* pstIntf)
{
    DISP_INTF_S* pstIt = &pstDisp->stSetting.stIntf[pstIntf->eID];
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    HI_DRV_DISP_INTF_S stBackup;
    HI_BOOL bBkFlag = HI_FALSE;
    HI_S32 nRet = HI_SUCCESS;
    DispCheckNullPointer(pfOpt);

    //DISP_INFO("DispAddIntf ***0**add (%d)----%d (%d)(%d)(%d)\n",pstDisp->enDisp,pstIntf->eID,pstIntf->u8VDAC_Y_G,pstIntf->u8VDAC_Pb_B,pstIntf->u8VDAC_Pr_R);
    /* if intf exist, release firstly */
    if (pstIt->bOpen)
    {
        bBkFlag = HI_TRUE;

        stBackup = pstIt->stIf;

        // s1 release vdac
        DispCheckNullPointer(pfOpt->PF_ReleaseIntf2);
        pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, pstIt);
    }

    /* clean */
    DispCleanIntf(pstIt);

    DISP_PRINT("DispAddIntf  pstIntf->u8VDAC_Y_G = %d\n", pstIntf->u8VDAC_Y_G);

    pstIt->stIf = *pstIntf;
        DispCheckNullPointer(pfOpt->PF_AcquireIntf2);
    nRet = pfOpt->PF_AcquireIntf2(pstDisp->enDisp, pstIt);

    if (nRet)
    {
        DISP_ERROR("DISP %d acquire  (%d) failed\n", pstDisp->enDisp, pstIt->stIf.eID);
        goto __SET_BACKUP__;
    }
#ifdef __DISP_PLATFORM_BOOT__
#else
    if ((pstIt->stIf.eID >= HI_DRV_DISP_INTF_HDMI0 ) && (pstIt->stIf.eID <= HI_DRV_DISP_INTF_HDMI2 ) && (!DispGetHdmiFunction()))
    {
        if (s_pstHDMIFunc->pfnHdmiDetach && s_pstHDMIFunc->pfnHdmiAttach)
        {
        #if defined (HI_HDMI_SUPPORT_2_0)
		 
         HDMI_VIDEO_ATTR_S  stVideoAttr;

		 if (HI_SUCCESS == DISPGetDispPara(pstDisp->enDisp,&stVideoAttr))
		 {
			s_pstHDMIFunc->pfnHdmiOpen(HI_UNF_HDMI_ID_0);
			s_pstHDMIFunc->pfnHdmiAttach(HI_UNF_HDMI_ID_0,&stVideoAttr);
		 }
		 #else
            s_pstHDMIFunc->pfnHdmiAttach(HI_UNF_HDMI_ID_0, pstDisp->stSetting.enFormat, pstDisp->stSetting.eDispMode);
		#endif
		}
    }
#endif
    pstIt->bOpen = HI_TRUE;

    return HI_SUCCESS;

__SET_BACKUP__:

    if (bBkFlag == HI_TRUE)
    {
        DispAddIntf(pstDisp, &stBackup);
    }

    return nRet;
}

#define  DISPLAY_DEFAULT_VIRT_SCREEN_WIDTH   1280
#define  DISPLAY_DEFAULT_VIRT_SCREEN_HEIGHT  720

/*==========================================================================
    video encoding
*/
HI_S32 DispProduceDisplayInfo(DISP_S* pstDisp, HI_DISP_DISPLAY_INFO_S* pstInfo)
{
    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_DRV_DISP_FMT_E eFmt;
    HI_S32 u32TcFreq;
#ifndef __DISP_PLATFORM_BOOT__
    HI_PQ_PICTURE_SETTING_S stPictureSetting;
#endif

    if (!pstDisp)
    {
        DISP_ERROR("Found null pointer in %s\n", __FUNCTION__);
        return HI_ERR_DISP_NULL_PTR;
    }

    eFmt = pstDisp->stSetting.enFormat;
    if (eFmt < HI_DRV_DISP_FMT_CUSTOM)
    {
        (HI_VOID)DISP_HAL_GetEncFmtPara(eFmt, &stFmt);

        pstInfo->bIsMaster = pstDisp->bIsMaster;
        pstInfo->bIsSlave  = pstDisp->bIsSlave;
        //DISP_INFO("id=%d, bm=%d, bs=%d\n", pstDisp->enDisp, pstInfo->bIsMaster,  pstInfo->bIsSlave);
        pstInfo->enAttachedDisp = pstDisp->enAttachedDisp;


        pstInfo->eDispMode = pstDisp->stSetting.eDispMode;
        pstInfo->bRightEyeFirst = pstDisp->stSetting.bRightEyeFirst;
        pstInfo->bInterlace = stFmt.bInterlace;

        pstInfo->stVirtaulScreen = pstDisp->stSetting.stVirtaulScreen;


        if ((pstInfo->stVirtaulScreen.s32Width == 0)
            || (pstInfo->stVirtaulScreen.s32Height == 0))
        {
            pstInfo->stVirtaulScreen.s32Width = DISPLAY_DEFAULT_VIRT_SCREEN_WIDTH;
            pstInfo->stVirtaulScreen.s32Height = DISPLAY_DEFAULT_VIRT_SCREEN_HEIGHT;
        }

        pstInfo->stOffsetInfo    = pstDisp->stSetting.stOffsetInfo;
        pstInfo->stFmtResolution = stFmt.stRefRect;
        pstInfo->stPixelFmtResolution = stFmt.stOrgRect;


        pstDisp->stSetting.stColor.enInCS = stFmt.enColorSpace;
        pstInfo->eColorSpace = stFmt.enColorSpace;
#if defined (HI_HDMI_SUPPORT_2_0)
		pstDisp->stSetting.stColor.enOutCS= pstDisp->stSetting.stColor.enInCS;
#endif		
       // DISP_INFO(">>>>>>>>>>>>>>>> 002 cs =%d\n",  pstInfo->eColorSpace);

        if (!pstDisp->stSetting.bCustomRatio)
        {
            pstInfo->stAR = stFmt.stAR;
        }
        else
        {
            pstInfo->stAR.u32ARh = pstDisp->stSetting.u32CustomRatioHeight;
            pstInfo->stAR.u32ARw = pstDisp->stSetting.u32CustomRatioWidth;
        }

        pstInfo->u32RefreshRate = stFmt.u32RefreshRate;

#ifndef __DISP_PLATFORM_BOOT__

        if (pstDisp->enDisp == HI_DRV_DISPLAY_1)
        {
            DRV_PQ_GetHDVideoSetting(&stPictureSetting);
        }
        else
        {
            DRV_PQ_GetSDVideoSetting(&stPictureSetting);
        }

        pstInfo->u32Bright = stPictureSetting.u16Brightness;
        pstInfo->u32Contrst = stPictureSetting.u16Contrast;
        pstInfo->u32Hue = stPictureSetting.u16Hue;
        pstInfo->u32Satur = stPictureSetting.u16Saturation;

#endif

    }
    else if (eFmt == HI_DRV_DISP_FMT_CUSTOM)
    {
        pstInfo->bIsMaster = pstDisp->bIsMaster;
        pstInfo->bIsSlave  = pstDisp->bIsSlave;
        pstInfo->enAttachedDisp = pstDisp->enAttachedDisp;


        pstInfo->eDispMode = pstDisp->stSetting.eDispMode;
        pstInfo->bRightEyeFirst = pstDisp->stSetting.bRightEyeFirst;

        pstInfo->bInterlace = pstDisp->stSetting.stCustomTimg.bInterlace;


        pstInfo->stVirtaulScreen = pstDisp->stSetting.stVirtaulScreen;

        if ((pstInfo->stVirtaulScreen.s32Width == 0)
            || (pstInfo->stVirtaulScreen.s32Height == 0))
        {
            pstInfo->stVirtaulScreen.s32Width = DISPLAY_DEFAULT_VIRT_SCREEN_WIDTH;
            pstInfo->stVirtaulScreen.s32Height = DISPLAY_DEFAULT_VIRT_SCREEN_HEIGHT;
        }

        pstInfo->stOffsetInfo    = pstDisp->stSetting.stOffsetInfo;

        pstInfo->stPixelFmtResolution.s32X = 0;
        pstInfo->stPixelFmtResolution.s32Y = 0;
        pstInfo->stPixelFmtResolution.s32Width = pstDisp->stSetting.stCustomTimg.u32HACT;
        pstInfo->stPixelFmtResolution.s32Height = pstDisp->stSetting.stCustomTimg.u32VACT;

        pstInfo->stFmtResolution.s32X = 0;
        pstInfo->stFmtResolution.s32Y = 0;

        pstInfo->stFmtResolution.s32Width = pstDisp->stSetting.stCustomTimg.u32HACT;
        pstInfo->stFmtResolution.s32Height = pstDisp->stSetting.stCustomTimg.u32VACT;

        /**/
        /*set  DHDx  in Color Space*/
        pstDisp->stSetting.stColor.enInCS = HI_DRV_CS_BT709_RGB_FULL;
        pstInfo->eColorSpace = HI_DRV_CS_BT709_RGB_FULL;
#if defined (HI_HDMI_SUPPORT_2_0)
		pstDisp->stSetting.stColor.enOutCS= pstDisp->stSetting.stColor.enInCS;
#endif	
        pstInfo->stAR.u32ARh = pstDisp->stSetting.stCustomTimg.u32AspectRatioH;
        pstInfo->stAR.u32ARw = pstDisp->stSetting.stCustomTimg.u32AspectRatioW;

        /*set  Rate*/
        /*tc set freq is *1000  but VDP module need *100*/
        u32TcFreq = pstDisp->stSetting.stCustomTimg.u32VertFreq;
        pstInfo->u32RefreshRate = u32TcFreq/10;

#ifndef __DISP_PLATFORM_BOOT__
        if (pstDisp->enDisp == HI_DRV_DISPLAY_1)
        {
            DRV_PQ_GetHDVideoSetting(&stPictureSetting);
        }
        else
        {
            DRV_PQ_GetSDVideoSetting(&stPictureSetting);
        }


        pstInfo->u32Bright = stPictureSetting.u16Brightness;
        pstInfo->u32Contrst = stPictureSetting.u16Contrast;
        pstInfo->u32Hue = stPictureSetting.u16Hue;
        pstInfo->u32Satur = stPictureSetting.u16Saturation;

#endif
    }
    else
    {
        DISP_WARN("Invalid display encoding format now\n");
        return HI_ERR_DISP_NOT_SUPPORT_FMT;
    }

    return HI_SUCCESS;
}

HI_VOID DispInitCSC(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S* pstColor)
{
    // s1 CSC

    DISP_MEMSET(pstColor, 0, sizeof(HI_DRV_DISP_COLOR_SETTING_S));

    pstColor->enInCS  = HI_DRV_CS_DEFAULT;
    pstColor->enOutCS = HI_DRV_CS_DEFAULT;

    pstColor->pReserve = HI_NULL;
    pstColor->u32Reserve = 0;
    return;
}


HI_VOID DispGetTestInitParam(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_SETTING_S* pstSetting)
{
    if (enDisp == HI_DRV_DISPLAY_0)
    {
        pstSetting->bIsMaster = HI_TRUE;
        pstSetting->enAttachedDisp = HI_DRV_DISPLAY_1;

        pstSetting->enFormat = HI_DRV_DISP_FMT_1080i_50;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].eID = HI_DRV_DISP_INTF_YPBPR0;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Y_G  = 1;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Pb_B = 2;
        pstSetting->stIntf[HI_DRV_DISP_INTF_YPBPR0].u8VDAC_Pr_R = 0;
    }

    if (enDisp == HI_DRV_DISPLAY_1)
    {
        pstSetting->bIsSlave = HI_TRUE;
        pstSetting->enAttachedDisp = HI_DRV_DISPLAY_0;

        pstSetting->enFormat = HI_DRV_DISP_FMT_PAL;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].eID = HI_DRV_DISP_INTF_CVBS0;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Y_G  = 3;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[HI_DRV_DISP_INTF_CVBS0].u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;
    }

    return;
}

extern HI_S32 DispGetInitParam(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INIT_PARAM_S* pstSetting);

HI_S32 DispGetInitParamPriv(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_SETTING_S* pstSetting)
{
    HI_DRV_DISP_INTF_ID_E enIf;
    HI_DRV_DISP_INIT_PARAM_S stInitParam;

    DISP_MEMSET(pstSetting, 0, sizeof(HI_DRV_DISP_SETTING_S));

    if (enDisp > HI_DRV_DISPLAY_1)
    {
        return HI_ERR_DISP_INVALID_PARA;
    }

    // s1 get init param form PDM

    // s2 check para, if(ok){return;} else {get default param;}
    pstSetting->u32BootVersion = 0xfffffffful;


    pstSetting->bGetPDMParam = HI_FALSE;

    pstSetting->bIsMaster = HI_FALSE;
    pstSetting->bIsSlave  = HI_FALSE;
    pstSetting->enAttachedDisp = HI_DRV_DISPLAY_BUTT;

    /* output format */
    pstSetting->eDispMode = DISP_STEREO_NONE;

    pstSetting->enFormat  = HI_DRV_DISP_FMT_BUTT;
    pstSetting->bIsMaster = HI_FALSE;
    pstSetting->bIsSlave  = HI_FALSE;
    pstSetting->enAttachedDisp = HI_DRV_DISPLAY_BUTT;


    /* about color */
    DispInitCSC(enDisp, &pstSetting->stColor);

    /* background color */
    pstSetting->stBgColor.u8Red   = DISP_DEFAULT_COLOR_RED;
    pstSetting->stBgColor.u8Green = DISP_DEFAULT_COLOR_GREEN;
    pstSetting->stBgColor.u8Blue  = DISP_DEFAULT_COLOR_BLUE;

    /*zorder */
    pstSetting->enLayer[0] = HI_DRV_DISP_LAYER_GFX;
    pstSetting->enLayer[1] = HI_DRV_DISP_LAYER_VIDEO;

    /* interface setting */
    for (enIf = HI_DRV_DISP_INTF_YPBPR0; enIf < HI_DRV_DISP_INTF_ID_MAX; enIf++)
    {
        pstSetting->stIntf[enIf].eID = HI_DRV_DISP_INTF_ID_MAX;
        pstSetting->stIntf[enIf].u8VDAC_Y_G  = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[enIf].u8VDAC_Pb_B = HI_DISP_VDAC_INVALID_ID;
        pstSetting->stIntf[enIf].u8VDAC_Pr_R = HI_DISP_VDAC_INVALID_ID;
    }

    pstSetting->u32LayerNumber = 0;
    //HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */


    pstSetting->bCustomRatio = HI_FALSE;
    pstSetting->u32CustomRatioWidth  = 0;
    pstSetting->u32CustomRatioHeight = 0;

    pstSetting->stVirtaulScreen.s32X      = 0;
    pstSetting->stVirtaulScreen.s32Y      = 0;
    pstSetting->stVirtaulScreen.s32Width  = 1280;
    pstSetting->stVirtaulScreen.s32Height = 720;

    pstSetting->stOffsetInfo.u32Left     = 0;
    pstSetting->stOffsetInfo.u32Right    = 0;
    pstSetting->stOffsetInfo.u32Top      = 0;
    pstSetting->stOffsetInfo.u32Bottom   = 0;

    //pstSetting->u32Reseve;
    //pstSetting->pRevData;
    //DispGetTestInitParam(enDisp, pstSetting);
    //DispGetInitParam(enDisp, pstSetting);
    if ( DispGetInitParam(enDisp, &stInitParam) == HI_SUCCESS)
    {
        pstSetting->bGetPDMParam = HI_TRUE;
        pstSetting->u32BootVersion = stInitParam.u32Version;


        pstSetting->bIsMaster = stInitParam.bIsMaster;
        pstSetting->bIsSlave  = stInitParam.bIsSlave;
        pstSetting->enAttachedDisp = stInitParam.enAttachedDisp;


        pstSetting->enFormat = DispTransferFormat(enDisp, stInitParam.enFormat);
        pstSetting->stCustomTimg  = stInitParam.stDispTiming;

        pstSetting->stBgColor = stInitParam.stBgColor;

        pstSetting->bCustomRatio = stInitParam.bCustomRatio;
        pstSetting->u32CustomRatioWidth  = stInitParam.u32CustomRatioWidth;
        pstSetting->u32CustomRatioHeight = stInitParam.u32CustomRatioHeight;

        pstSetting->stVirtaulScreen.s32X      = 0;
        pstSetting->stVirtaulScreen.s32Y      = 0;
        pstSetting->stVirtaulScreen.s32Width  = stInitParam.u32VirtScreenWidth;
        pstSetting->stVirtaulScreen.s32Height = stInitParam.u32VirtScreenHeight;

        pstSetting->stOffsetInfo              = stInitParam.stOffsetInfo;

        for (enIf = HI_DRV_DISP_INTF_YPBPR0; enIf < HI_DRV_DISP_INTF_ID_MAX; enIf++)
        {
            if (stInitParam.stIntf[enIf].eID != HI_DRV_DISP_INTF_ID_MAX)
            {
                pstSetting->stIntf[enIf] = stInitParam.stIntf[enIf];
                DISP_PRINT(">>>>>>>>>> intf %d id=%d\n", enIf,
                           stInitParam.stIntf[enIf].eID);
            }
        }

        //stInitParam.stDispTiming;
    }

    return HI_SUCCESS;
}

HI_VOID DispParserInitParam(DISP_S* pstDisp, HI_DRV_DISP_SETTING_S* pstSetting)
{
#ifdef HI_DISP_BUILD_FULL
    DISP_INTF_OPERATION_S* pstIntfOpt = DISP_HAL_GetOperationPtr();
#endif

    DISP_SETTING_S* pstS = &pstDisp->stSetting;

    //DISP_ASSERT(pstIntfOpt);
    pstS->u32Version = DISP_DRVIER_VERSION;
    pstS->u32BootVersion = pstSetting->u32BootVersion;
    //pstS->bSelfStart = pstSetting->bSelfStart;
    pstS->bGetPDMParam = pstSetting->bGetPDMParam;

    pstS->eDispMode = pstSetting->eDispMode;
    pstS->bRightEyeFirst = HI_FALSE;
    pstS->enFormat  = pstSetting->enFormat;
    //pstS->bFmtChanged = HI_FALSE;  // TODO

    pstS->stCustomTimg = pstSetting->stCustomTimg;

    /* about color */
    pstS->stColor = pstSetting->stColor;

    /* background color */
    pstS->stBgColor = pstSetting->stBgColor;

    /* interface setting */
    DispCleanAllIntf(pstDisp);

    pstS->u32LayerNumber = 0;
    //HI_DRV_DISP_LAYER_E enLayer[HI_DRV_DISP_LAYER_BUTT]; /* Z-order is from bottom to top */

    pstS->bCustomRatio = pstSetting->bCustomRatio;

    if (pstS->bCustomRatio)
    {
        pstS->u32CustomRatioWidth = pstSetting->u32CustomRatioWidth;
        pstS->u32CustomRatioHeight = pstSetting->u32CustomRatioHeight;
    }

    pstS->u32Reseve = 0;
    pstS->pRevData  = HI_NULL;
    pstS->stVirtaulScreen = pstSetting->stVirtaulScreen;
    pstS->stOffsetInfo    = pstSetting->stOffsetInfo;

    /* for attach display */
    pstDisp->bIsMaster = pstSetting->bIsMaster;
    pstDisp->bIsSlave  = pstSetting->bIsSlave;
    pstDisp->enAttachedDisp = pstSetting->enAttachedDisp;

#if 0
    DISP_PRINT("FOLLOW INFO: DISP %d, M=%d,S=%d,ATT=%d......\n",
               pstDisp->enDisp,
               pstDisp->bIsMaster,
               pstDisp->bIsSlave,
               pstDisp->enAttachedDisp);
#endif

#ifdef HI_DISP_BUILD_FULL

    if (pstS->bGetPDMParam)
    {
        HI_BOOL bOutput;

        // todo
        if (( !pstIntfOpt) || (!pstIntfOpt->PF_GetChnEnable) )
        {
            DISP_ERROR(" %s has null ptr!\n", __FUNCTION__);
            return ;
        }
        pstIntfOpt->PF_GetChnEnable(pstDisp->enDisp, &bOutput);

        if (bOutput)
        {
            pstDisp->bEnable = HI_TRUE;
            DISP_PRINT("DISP %d is working......\n", pstDisp->enDisp);

            // VDP is working, not to reset.
            DispSetHardwareState();
        }
    }

#endif

    return;
}

extern HI_U32 s_u32VdpBaseAddr;

HI_VOID DispInitDisplay(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;
	HI_U32 i = 0;
    HI_DRV_DISP_SETTING_S stDefSetting;
	DISP_INTF_OPERATION_S* pstIntfOpt = HI_NULL ;


    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    DISP_MEMSET(pstDisp, 0, sizeof(DISP_S));

    // s1 set id
    pstDisp->enDisp = enDisp;

    // s2 get base parameters
    if (DispGetInitParamPriv(enDisp, &stDefSetting))
    {
        pstDisp->bBaseExist  = HI_FALSE;
        DISP_PRINT("DispGetInitParam  failed\n");
        return;
    }
	
    pstDisp->bBaseExist  = HI_TRUE;
    //component operation
    pstDisp->pstIntfOpt = &s_stDisplayDevice.stIntfOpt;

    /*we delete the pq,hue,contrast not initialied.*/
    DispParserInitParam(pstDisp, &stDefSetting);

    pstDisp->eState  = DISP_PRIV_STATE_DISABLE;
    pstDisp->bOpen = HI_FALSE;;

    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
	pstDisp->hCast = HI_NULL;

	for (i = 0; i < HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        if (stDefSetting.stIntf[i].eID < HI_DRV_DISP_INTF_ID_MAX)
        {
            //todo
            DispAddIntf(pstDisp, &stDefSetting.stIntf[i]);
			
			pstIntfOpt = DISP_HAL_GetOperationPtr();
            if ((!pstIntfOpt) ||(!pstIntfOpt->PF_GetChnEnable ))
            {
                DISP_ERROR(" %s has null ptr!\n", __FUNCTION__);
                return ;
            }
            pstIntfOpt->PF_InitDacDetect(&stDefSetting.stIntf[i]);
        }
    }
 

    return;
}


HI_VOID DispDeInitDisplay(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;
    HI_S32 t;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

#ifdef HI_DISP_BUILD_FULL

    if (pstDisp->hCast)
    {
        DISP_SetCastEnable(pstDisp->hCast, HI_FALSE);
        DISP_DestroyCast(pstDisp->hCast);
    }

#endif

    for (t = 0; t < HI_DRV_DISP_INTF_ID_MAX; t++)
    {
        if (pstDisp->stSetting.stIntf[t].bOpen)
        {
            DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
            if (pfOpt)
            {
                if ( pfOpt->PF_ReleaseIntf2)
                    pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[t]);
            }
            // s3 set intf
            DispCleanIntf(&pstDisp->stSetting.stIntf[t]);
        }
    }

    return;
}

HI_S32 DispSetFormat(HI_DRV_DISPLAY_E eDisp, HI_DRV_DISP_FMT_E eFmt,HI_DRV_DISP_STEREO_E enStereo)
{
    DISP_S* pstDisp;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    HI_U32 u;
    HI_S32 nRet;

    DispGetPointerByID(eDisp, pstDisp);
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_ResetIntfFmt2);

    // s1 set channel
    if (HI_DRV_DISP_FMT_CUSTOM == eFmt)
        nRet = pstDisp->pstIntfOpt->PF_SetChnTiming(pstDisp->enDisp, &pstDisp->stSetting.stCustomTimg);
    else
        nRet = pstDisp->pstIntfOpt->PF_SetChnFmt(pstDisp->enDisp, eFmt, enStereo);

    if (HI_SUCCESS != nRet)
    {
        DISP_ERROR("set disp%d  fmt %d err !(%d)\n",eDisp,eFmt,nRet);
        return nRet;
    }

    // s2 set interface  if necessarily
    for (u = 0; u < HI_DRV_DISP_INTF_ID_MAX; u++)
    {
        if (pstDisp->stSetting.stIntf[u].bOpen)
        {
            pfOpt->PF_ResetIntfFmt2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[u], eFmt,&pstDisp->stSetting.stCustomTimg);
        }
    }

    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);

    return HI_SUCCESS;
}


HI_S32 DispSetColor(DISP_S* pstDisp)
{
    DISP_HAL_COLOR_S stColor;
    HI_DRV_DISP_COLOR_SETTING_S* pstC = &pstDisp->stSetting.stColor;

    stColor.enInputCS  = pstC->enInCS;
    stColor.enOutputCS = HI_DRV_CS_BT709_RGB_FULL;

    (HI_VOID)pstDisp->pstIntfOpt->PF_SetChnColor(pstDisp->enDisp, &stColor);

    pstDisp->pstIntfOpt->PF_SetChnBgColor(pstDisp->enDisp,
                                          pstDisp->stSetting.stColor.enInCS,
                                          &pstDisp->stSetting.stBgColor);


    return HI_SUCCESS;
}

HI_S32 DispSetIntfEnable(DISP_S* pstDisp, HI_BOOL bEnable)
{
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    HI_U32 u;

    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_SetIntfEnable2);
    // s1 set interface if necessarily
    for (u = 0; u < HI_DRV_DISP_INTF_ID_MAX; u++)
    {
        if (pstDisp->stSetting.stIntf[u].bOpen)
        {
            //DispSetIntfLink(pstDisp, u);
            pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[u], bEnable);
        }
    }

    return HI_SUCCESS;
}

HI_S32 DispSetEnable(DISP_S* pstDisp, HI_BOOL bEnable)
{
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    HI_U32 u;
    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_SetIntfEnable2);
    DispCheckNullPointer(pfOpt->PF_SetChnEnable);

    // s1 set interface if necessarily
    for (u = 0; u < HI_DRV_DISP_INTF_ID_MAX; u++)
    {
        if (pstDisp->stSetting.stIntf[u].bOpen)
        {
            //DispSetIntfLink(pstDisp, u);
            pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[u], bEnable);
        }
    }

    // s2 set channel
    (HI_VOID)pfOpt->PF_SetChnEnable(pstDisp->enDisp, bEnable);

    return HI_SUCCESS;
}

/******************************************************************************
    display function
*****************************************************************************/
#define DEF_DRV_DISP_API_FUNCTION_START_HERE

HI_S32 DISP_GetInitFlag(HI_BOOL* pbInited)
{
    DispCheckNullPointer(pbInited);

    *pbInited = (s_s32DisplayGlobalFlag == DISP_DEVICE_STATE_OPEN) ? HI_TRUE : HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 DISP_GetVersion(HI_DRV_DISP_VERSION_S* pstVersion)
{
    DispCheckNullPointer(pstVersion);

    // check whether display is inited.
    DispCheckDeviceState();

    // return version
    DISP_HAL_GetVersion(pstVersion);

    return HI_SUCCESS;
}

HI_BOOL DISP_IsOpened(HI_DRV_DISPLAY_E enDisp)
{
    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 return display OPEN state
    return s_stDisplayDevice.stDisp[enDisp - HI_DRV_DISPLAY_0].bOpen;
}

/*check the channel is master or not?*/
HI_BOOL DISP_IsFollowed(HI_DRV_DISPLAY_E enDisp)
{
    DispCheckDeviceState();
    DispCheckID(enDisp);
	return s_stDisplayDevice.stDisp[enDisp - HI_DRV_DISPLAY_0].bIsMaster;
}

/*check the channel is master or not?*/
HI_BOOL DISP_Check_IsogenyMode(HI_VOID)
{
	return  (DISP_IsFollowed(HI_DRV_DISPLAY_0) || DISP_IsFollowed(HI_DRV_DISPLAY_1));
}


HI_S32 DISP_Init(HI_VOID)
{
    HI_DRV_DISPLAY_E eDisp;
    HI_DRV_DISP_VERSION_S stDispVersion;
    HI_S32 nRet;
	
    memset(&stDispVersion,0x0,sizeof(HI_DRV_DISP_VERSION_S));
    if (s_s32DisplayGlobalFlag != DISP_DEVICE_STATE_CLOSE)
    {
        DISP_INFO("DISPLAY has been inited");
        return HI_SUCCESS;
    }

    DISP_MEMSET(&s_stDisplayDevice, 0, sizeof(DISP_DEV_S));

    // s1 get interface operation
    nRet = DISP_HAL_Init(DISP_BASE_ADDRESS);
    if (nRet)
    {
        DISP_ERROR("DISP_HAL_Init failed!");
        goto __ERR_EXIT__;
    }

    nRet = DISP_HAL_GetOperation(&s_stDisplayDevice.stIntfOpt);
    nRet = DISP_HAL_GetVersion(&stDispVersion);

    // s1.1 init alg
    nRet = DISP_DA_Init(&stDispVersion);
    if (nRet)
    {
        DISP_ERROR("DISP_DA_Init failed!");
        goto __ERR_EXIT__;
    }

    // s2 inited display
    for (eDisp = HI_DRV_DISPLAY_0; eDisp < HI_DRV_DISPLAY_2; eDisp++)
    {
        DispInitDisplay(eDisp);
    }

#ifdef HI_DISP_BUILD_FULL
    nRet = DISP_ISR_Init();
    if (request_irq(DISP_INT_NUMBER, (irq_handler_t)DISP_ISR_Main, IRQF_SHARED, "hi_vdp_irq", &g_DispIrqHandle) != 0)
    {
        DISP_ERROR("DISP registe IRQ failed!\n");
        nRet = HI_ERR_DISP_CREATE_ERR;
        goto __ERR_EXIT__;
    }

#endif

    DispResetHardware();

    s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_OPEN;

    for (eDisp = HI_DRV_DISPLAY_0; eDisp < HI_DRV_DISPLAY_2; eDisp++)
    {
        DISP_S* pstDisp = HI_NULL;
        DispGetPointerByID(eDisp, pstDisp);

#ifdef __DISP_PLATFORM_BOOT__
        DISP_Open(eDisp);

        if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen)
        {
            HI_DRV_DISP_FMT_E enEncFmt = HI_DRV_DISP_FMT_BUTT;
            HI_DRV_DISP_STEREO_MODE_E enStereo;
            DISP_GetFormat(eDisp, &enStereo, &enEncFmt);

            HI_DRV_HDMI_Init();
#if defined (HI_HDMI_SUPPORT_2_0)
            HI_DRV_HDMI_ATTR_S stAttr;
            HI_DRV_DISP_TIMING_S  stTiming;

            
            if (HI_SUCCESS != HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_0))
            {
                DISP_PRINT("HI_UNF_HDMI_Open Err \n");
            }

            stAttr.bEnableHdmi  = HI_TRUE;

            if ((HI_DRV_CS_BT601_RGB_LIMITED == pstDisp->stSetting.stColor.enOutCS)
                || (HI_DRV_CS_BT601_RGB_FULL == pstDisp->stSetting.stColor.enOutCS)
                || (HI_DRV_CS_BT709_RGB_LIMITED == pstDisp->stSetting.stColor.enOutCS)
                || (HI_DRV_CS_BT709_RGB_FULL == pstDisp->stSetting.stColor.enOutCS)
                )
            {
                stAttr.enVidInMode  = HI_UNF_HDMI_VIDEO_MODE_RGB444;
                stAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_RGB444;
            }
            else
            {
                stAttr.enVidInMode  = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
                stAttr.enVidOutMode = HI_UNF_HDMI_VIDEO_MODE_YCBCR444;
            }
           
            stAttr.u32DispFmt   = enEncFmt;

            DispGetFmtTiming(enEncFmt,&stTiming);
            if ((HI_DRV_DISP_FMT_1080P_24_FP == enEncFmt )
			||(HI_DRV_DISP_FMT_720P_60_FP == enEncFmt )
			||(HI_DRV_DISP_FMT_720P_50_FP == enEncFmt ))
			{
				stAttr.u32ClkFs = stTiming.u32PixFreq*2;
			}
			else
			{
				stAttr.u32ClkFs = stTiming.u32PixFreq;
			}
            
            HI_DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_0, &stAttr);
            HI_DRV_HDMI_Start(HI_UNF_HDMI_ID_0);

            
            
#else
            if (HI_SUCCESS != HI_DRV_HDMI_Open(enEncFmt))
            {
                DISP_PRINT("HI_UNF_HDMI_Open Err \n");
            }
			HI_DRV_HDMI_Start();
#endif
        }

#else
       /*DTS2013110506251  ,
             * base para is exist,but logo is not exist,should call hdmi init and open too.
             */
        if ( pstDisp->bEnable ||  pstDisp->bBaseExist )
        {
            DISP_Open(eDisp);

#ifndef HI_MCE_SUPPORT

#if defined (HI_HDMI_SUPPORT_2_0)
			/*do nothing */					
#else	

            if ((pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen) && (!DispGetHdmiFunction()))
            {
 
                    if (s_pstHDMIFunc->pfnHdmiInit && s_pstHDMIFunc->pfnHdmiOpen)
                    {
                        DISP_PRINT(">>>>>>>>>>>>>>>>>>>>>>>>>>>> disp init hdmi\n");
                        s_pstHDMIFunc->pfnHdmiInit();
                        s_pstHDMIFunc->pfnHdmiOpen(HI_UNF_HDMI_ID_0);
                    }
            }
#endif			
#endif

        }

#endif
    }
	
    return HI_SUCCESS;
	
__ERR_EXIT__:
    /*ua deinit is not neccessary yet.*/

    DISP_HAL_DeInit();

    return nRet;
}

HI_S32 DISP_DeInit(HI_VOID)
{
    HI_DRV_DISPLAY_E eDisp;

    DISP_PRINT("DISP_DeInit 001\n");

    if (DISP_DEVICE_STATE_CLOSE == s_s32DisplayGlobalFlag)
    {
        DISP_INFO("DISPLAY has NOT inited");
        return HI_SUCCESS;
    }

    DISP_PRINT("DISP_DeInit 002\n");

    // s2 inited display
    for (eDisp = HI_DRV_DISPLAY_0; eDisp < HI_DRV_DISPLAY_2; eDisp++)
    {
        DISP_Close(eDisp);
        DispDeInitDisplay(eDisp);
    }

    DISP_PRINT("DISP_DeInit 003\n");
    //DISP_MSLEEP(40);
    //s_stDisplayDevice.stIntfOpt.PF_ResetHardware();

#ifdef HI_DISP_BUILD_FULL

    free_irq(DISP_INT_NUMBER, &g_DispIrqHandle);
    DISP_ISR_DeInit();
    DISP_PRINT("DISP_DeInit 004\n");

#endif

    DISP_DA_DeInit();


    DISP_HAL_DeInit();

    DISP_MEMSET(&s_stDisplayDevice, 0, sizeof(DISP_DEV_S));

    DISP_PRINT("DISP_DeInit 004\n");


    s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_CLOSE;

    return HI_SUCCESS;
}

/*here is just for fastbootup, because, android fastbootup, make image and  bootup,
  they use the same code, but runs in a different code branch.*/
volatile HI_U32 *pDispUoot2KernelFlag = HI_NULL;
HI_U32 Disp_GetFastbootupFlag(HI_VOID)
{
	pDispUoot2KernelFlag = (volatile HI_U32 *) DISP_IOADDRESS(DISP_USED_COMMON_REGISTER);
	return *pDispUoot2KernelFlag;
}

HI_U32 Disp_SetFastbootupFlag(HI_U32 u32Value)
{
	pDispUoot2KernelFlag = (volatile HI_U32 *) DISP_IOADDRESS(DISP_USED_COMMON_REGISTER);
	*pDispUoot2KernelFlag = u32Value;
    return HI_SUCCESS;
}

HI_S32 DISP_Suspend(HI_VOID)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S* pstDisp;
    HI_DRV_DISPLAY_E enD;
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    //DISP_INTF_OPERATION_S *pfOpt = DISP_HAL_GetOperationPtr();

    DispCheckDeviceState();
    DispCheckNullPointer(pfHal);
    DispCheckNullPointer(pfHal->PF_VDP_RegSave);

    if (DISP_DEVICE_STATE_OPEN == s_s32DisplayGlobalFlag)
    {
        for (enD = HI_DRV_DISPLAY_0; enD < HI_DRV_DISPLAY_BUTT; enD++)
        {
            DispGetPointerByIDNoReturn(enD, pstDisp);

            pstDisp->bStateBackup = pstDisp->bEnable;

            if (pstDisp->bEnable == HI_TRUE)
            {
                DISP_SetEnable(enD, HI_FALSE);
            }
        }

        DISP_ISR_Suspend();

        /*save VDP reg*/
        if (HI_SUCCESS != pfHal->PF_VDP_RegSave())
        {
            DISP_ERROR("Display save registers for suspend failed!\n");
             return HI_ERR_DISP_MALLOC_MAP_ERR;
        }
        /*dts:DTS2013080709083, we should reset the module and close all the clk.*/
        DispCloseClkResetModule();

        DispClearHardwareState();

        s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_SUSPEND;
    }

    /*clear the flag passed from uboot to kernel, which indicates a android fastbootup mode.
	 * since the reg is stored for disp for ever, so even in non-fastbootup mode ,we can operate the reg eigther.*/
    Disp_SetFastbootupFlag(0);

#endif

    return HI_SUCCESS;
}

HI_S32 DISP_Resume(HI_VOID)
{
#ifndef __DISP_PLATFORM_BOOT__

    DISP_S *pstDisp = HI_NULL;
    DISP_S *pstDisp_bak = HI_NULL;
    HI_DRV_DISPLAY_E enD;
    HI_VDP_PQ_INFO_S stTimingInfo;

    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    DispCheckNullPointer(pfHal);
    DispCheckNullPointer(pfHal->PF_VDP_RegReStore);

    pstDisp_bak =  (DISP_S *)DISP_MALLOC(sizeof(DISP_S));
    if (!pstDisp_bak)
        return HI_ERR_DISP_MALLOC_MAP_ERR;

    if (DISP_DEVICE_STATE_SUSPEND == s_s32DisplayGlobalFlag)
    {
        if (Disp_GetFastbootupFlag() ==  DISP_FASTBOOTUP_FLAG)
        {
            // s2 inited display
            for (enD = HI_DRV_DISPLAY_0; enD < HI_DRV_DISPLAY_2; enD++)
            {
                DispGetPointerByIDNoReturn(enD, pstDisp);

                *pstDisp_bak = *pstDisp;
				/*init the new value,just for new setting.*/
                DispInitDisplay(enD);

                pstDisp_bak->bDispSettingChange = HI_TRUE;
                pstDisp_bak->stDispInfo = pstDisp->stDispInfo;

                pstDisp_bak->stSetting.eDispMode =  pstDisp->stSetting.eDispMode;
                pstDisp_bak->stSetting.enFormat =  pstDisp->stSetting.enFormat;
                pstDisp_bak->stSetting.stCustomTimg =  pstDisp->stSetting.stCustomTimg;
                pstDisp_bak->stSetting.stColor =  pstDisp->stSetting.stColor;
                pstDisp_bak->stSetting.stVirtaulScreen =  pstDisp->stSetting.stVirtaulScreen;
                pstDisp_bak->stSetting.stOffsetInfo =  pstDisp->stSetting.stOffsetInfo;

				/*we get the old value except the new setting.*/
                *pstDisp = *pstDisp_bak;

                /*restore VDP reg excep  dhdx*/
                if (HI_SUCCESS != pfHal->PF_VDP_RegReStore())
                {
                    DISP_ERROR("Display restore registers for resume failed!\n");
                    DISP_FREE(pstDisp_bak);
                    return HI_ERR_DISP_MALLOC_MAP_ERR;
                }


            }
        }
        else
        {
            DispResetHardware();

            /*restore VDP reg*/
            if (HI_SUCCESS != pfHal->PF_VDP_RegReStore())
            {
                DISP_FREE(pstDisp_bak);
                DISP_ERROR("Display restore registers for resume failed!\n");
                return HI_ERR_DISP_MALLOC_MAP_ERR;
            }
        }
		
        s_s32DisplayGlobalFlag = DISP_DEVICE_STATE_OPEN;

        for (enD = HI_DRV_DISPLAY_0; enD < HI_DRV_DISPLAY_BUTT; enD++)
        {
            DispGetPointerByIDNoReturn(enD, pstDisp);

            if (pstDisp->bStateBackup == HI_TRUE)
            {
                DISP_SetEnable(enD, HI_TRUE);
            }
			
			if (enD == HI_DRV_DISPLAY_0)
            {
                /*load pq configuration to register.*/
                stTimingInfo.u32Width =  720;
                stTimingInfo.u32Height = 480;
                stTimingInfo.u32OutWidth  = 720;
                stTimingInfo.u32OutHeight  = 480;
            }
            else
            {
                /*load pq configuration to register.*/
                stTimingInfo.u32Width =  1280;
                stTimingInfo.u32Height = 720;
                stTimingInfo.u32OutWidth  = 1280;
                stTimingInfo.u32OutHeight  = 720;
            }

            stTimingInfo.bSRState  = HI_FALSE;
            stTimingInfo.bPartUpdate = HI_FALSE;
            stTimingInfo.bIsogenyMode = DISP_Check_IsogenyMode();

            DRV_PQ_UpdateVdpPQ((HI_PQ_DISPLAY_E)enD, &stTimingInfo, (S_VDP_REGS_TYPE *)s_u32VdpBaseAddr);
        }

        /*resume should be after enable.*/
        mdelay(60);
        //if (Disp_GetFastbootupFlag() !=  DISP_FASTBOOTUP_FLAG)
        DISP_ISR_Resume();
    }

    DISP_FREE(pstDisp_bak);
#endif

    return HI_SUCCESS;
}


HI_S32 DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    DISP_S* pstM, *pstS;
    HI_RECT_S stvirscreen;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_TestChnAttach);
    if ( !pfOpt->PF_TestChnAttach(enMaster, enSlave) )
    {
        DISP_ERROR("Display NOT support %d attach to %d!\n", enSlave, enMaster);
        return HI_ERR_DISP_INVALID_OPT;
    }

    DispGetPointerByID(enMaster, pstM);
    DispGetPointerByID(enSlave, pstS);

    if (pstM->bIsMaster && (pstM->enAttachedDisp == enSlave))
    {
        DISP_INFO("Display has been ATTACHED!\n");
        return HI_SUCCESS;
    }

    if (pstM->bIsMaster || pstM->bIsSlave || pstS->bIsMaster || pstS->bIsSlave)
    {
        DISP_ERROR("Display has been opened!\n");
        return HI_ERR_DISP_INVALID_OPT;
    }

    pstM->bIsMaster = HI_TRUE;
    pstM->bIsSlave  = HI_FALSE;
    pstM->enAttachedDisp = enSlave;

    pstS->bIsMaster = HI_FALSE;
    pstS->bIsSlave  = HI_TRUE;
    pstS->enAttachedDisp = enMaster;
    memset(&stvirscreen,0x0,sizeof(HI_RECT_S));

    DISP_GetVirtScreen(enMaster, &stvirscreen);
    DISP_SetVirtScreen(enMaster, stvirscreen);
    return HI_SUCCESS;
}

HI_S32 DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    DISP_S* pstM, *pstS;

    // s1 check input parameters
    DispCheckDeviceState();

    // s2 set detach
    DispGetPointerByID(enMaster, pstM);
    DispGetPointerByID(enSlave, pstS);

    if (!pstM->bIsMaster || (pstM->enAttachedDisp != enSlave))
    {
        DISP_INFO("Display has NOT been ATTACHED!\n");
        return HI_SUCCESS;
    }

    pstM->bIsMaster = HI_FALSE;
    pstM->enAttachedDisp = HI_DRV_DISPLAY_BUTT;
    pstM->bDispSettingChange = HI_TRUE;


    pstS->bIsSlave = HI_FALSE;
    pstS->enAttachedDisp = HI_DRV_DISPLAY_BUTT;
    pstS->bDispSettingChange = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 DISP_SetDACDetEn(HI_BOOL bDACDetEn)
{
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    if (HI_NULL == pfHal)
    {
        DISP_ERROR("get null ptr!\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    pfHal->PF_SetDACDetEn(bDACDetEn);

    return HI_SUCCESS;
}
HI_S32 DISP_SetAllDacEn(HI_BOOL bDacEn)
{
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    if (HI_NULL == pfHal)
    {
        DISP_ERROR("get null ptr!\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    pfHal->PF_SetAllDACEn(bDacEn);

    return HI_SUCCESS;
}
HI_S32 DISP_GetDACAttr(HI_DRV_VDAC_ATTR_S *pDACAttr)
{
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    if (HI_NULL == pfHal)
    {
        DISP_ERROR("get null ptr!\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    pfHal->PF_GetDACAttr(pDACAttr);
    return HI_SUCCESS;
}

#ifdef HI_DISP_BUILD_FULL
#define  DISPLAY0_BUS_UNDERFLOW_INT 0x00000080UL
#define  DISPLAY1_BUS_UNDERFLOW_INT 0x00000008UL

HI_VOID DISP_CB_PreProcess(HI_VOID* hHandle, const HI_DRV_DISP_CALLBACK_INFO_S* pstInfo)
{
    DISP_S* pstDisp = (DISP_S*)hHandle;
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();
    HI_U32 uIntState;

#if defined(DAC_TYPE_HISI_VOLTAGE)
	/*Only HISI_VOLTAGE   Support  DAC Detect*/
    HI_U8 u8DacReg;
#endif

    if ((!pfHal) || (!pfHal->PF_GetUnmaskedIntState) || (!pfHal->PF_CleanIntState) )
    {
        DISP_ERROR("has null ptr!\n");
        return ;
    }

    pfHal->PF_GetUnmaskedIntState(&uIntState);

    if ((pstDisp->enDisp == HI_DRV_DISPLAY_0) && (uIntState & DISPLAY0_BUS_UNDERFLOW_INT) )
    {
        pstDisp->u32Underflow++;
        pfHal->PF_CleanIntState(DISPLAY0_BUS_UNDERFLOW_INT);
    }

    if ((pstDisp->enDisp == HI_DRV_DISPLAY_1) && (uIntState & DISPLAY1_BUS_UNDERFLOW_INT) )
    {
        pstDisp->u32Underflow++;
        pfHal->PF_CleanIntState(DISPLAY1_BUS_UNDERFLOW_INT);
    }

#if defined(DAC_TYPE_HISI_VOLTAGE)
	/*Only HISI_VOLTAGE   Support  DAC Detect*/
    u8DacReg = (uIntState & VDAC_STATE_INT)>>12;
    pfHal->PF_DACIsr(u8DacReg);// shielding function, subsequent to continue to use this functio
#endif

    pstDisp->stDispInfo = pstInfo->stDispInfo;
    DispProduceDisplayInfo(pstDisp, &pstDisp->stDispInfo);
    DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);

    switch (pstDisp->eState)
    {
        case DISP_PRIV_STATE_DISABLE:
            if (pstDisp->bEnable)
            {
                DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_OPEN);
                DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                pstDisp->eState = DISP_PRIV_STATE_WILL_ENABLE;
                DISP_PRINT("DISP_CB_PreProcess001 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
            }

            break;

        case DISP_PRIV_STATE_WILL_ENABLE:
            DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_VT_INT);
            DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
            pstDisp->eState = DISP_PRIV_STATE_ENABLE;

            DISP_PRINT("DISP_CB_PreProcess002 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);

            break;

        case DISP_PRIV_STATE_ENABLE:
            if (!pstDisp->bEnable)
            {
                DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_PREPARE_CLOSE);
                pstDisp->eState = DISP_PRIV_STATE_WILL_DISABLE;
                DISP_PRINT("DISP_CB_PreProcess003 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
            }
            else
            {
                if (pstDisp->bDispSettingChange)
                {
                    DISP_PRINT("DISP_CB_PreProcess0031 id=%d,en=%d\n", pstDisp->enDisp, pstDisp->bEnable);
                    DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_DISPLAY_SETTING_CHANGE);
                    pstDisp->bDispSettingChange = HI_FALSE;
                }
                else
                {
                    DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_VT_INT);
                    //DISP_ISR_SetDispInfo(pstDisp->enDisp, &pstDisp->stDispInfo);
                }
            }

            break;

        case DISP_PRIV_STATE_WILL_DISABLE:
            DISP_ISR_SetEvent(pstDisp->enDisp, HI_DRV_DISP_C_EVET_NONE);
            pstDisp->eState = DISP_PRIV_STATE_DISABLE;

            DISP_PRINT("DISP_CB_PreProcess004 id=%d, en=%d\n", pstDisp->enDisp,  pstDisp->bEnable);

            break;

        default :
            break;

    }

    //    DISP_INFO("event = %d, ", pstDisp->eEventType);
    return;
}
#endif

HI_S32 DISP_Open(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;
    HI_S32 nRet = HI_SUCCESS;	
#ifndef __DISP_PLATFORM_BOOT__
	HI_VDP_PQ_INFO_S stTimingInfo;
#endif

#ifdef HI_DISP_BUILD_FULL
    HI_DRV_DISP_CALLBACK_S stCB;
#endif

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 check whether display opened
    if (DISP_IsOpened(enDisp))
    {
        DISP_INFO("Display has been opened!\n");
        return HI_SUCCESS;
    }
	
    if(HI_SUCCESS != DispCheckReadyForOpen(enDisp))
    {
        DISP_INFO("format has not  been set!\n");
        return HI_ERR_DISP_NOT_SUPPORT_FMT;
    }

    // s2.1 get display channel
    DispGetPointerByID(enDisp, pstDisp);
    if (( !pstDisp) || (!pstDisp->pstIntfOpt) ||  (!pstDisp->pstIntfOpt->PF_TestChnSupport))
    {
        DISP_ERROR(" %s has null ptr!\n", __FUNCTION__);
        return HI_ERR_DISP_NULL_PTR;
    }

    if (!pstDisp->pstIntfOpt->PF_TestChnSupport(enDisp))
    {
        DISP_ERROR("DISP ERROR! This version does not support display %d\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }


    // s3 check whether necessory attributes are configed
#ifdef HI_DISP_BUILD_FULL
    // s3.2 open display channel isr
    DISP_ISR_OpenChn(enDisp);

    // set display info for first time
    DISP_ISR_SetDispInfo(enDisp, &pstDisp->stDispInfo);

    // register display callback
    stCB.hDst = (HI_VOID*)pstDisp;
    stCB.pfDISP_Callback = DISP_CB_PreProcess;
    nRet = DISP_ISR_RegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
    DISP_ASSERT(!nRet);
#endif

    DISP_SetLayerZorder(enDisp, HI_DRV_DISP_LAYER_GFX, HI_DRV_DISP_ZORDER_MOVETOP);

#ifndef __DISP_PLATFORM_BOOT__

	DRV_CGMS_Init(enDisp,pstDisp->stSetting.enFormat,&(pstDisp->stCgmsInfo));
    DRV_MCVN_Init(enDisp,pstDisp->stSetting.enFormat);

    if (enDisp == HI_DRV_DISPLAY_0)
    {
        /*update the pq param.*/	
        stTimingInfo.u32Width =  720;
        stTimingInfo.u32Height = 480;
        stTimingInfo.u32OutWidth  = 720;
        stTimingInfo.u32OutHeight  = 480;
    }
    else
    {
        /*update the pq param.*/	
        stTimingInfo.u32Width =  1280;
        stTimingInfo.u32Height = 720;
        stTimingInfo.u32OutWidth  = 1280;
        stTimingInfo.u32OutHeight  = 720;
    }

    stTimingInfo.bSRState  = HI_FALSE;
    stTimingInfo.bPartUpdate = HI_FALSE;
    stTimingInfo.bIsogenyMode = DISP_Check_IsogenyMode();	
    DRV_PQ_UpdateVdpPQ((HI_PQ_DISPLAY_E)enDisp, &stTimingInfo, (S_VDP_REGS_TYPE *)s_u32VdpBaseAddr);

#endif

    DISP_SetEnable(enDisp, HI_TRUE);
#if defined (CHIP_HIFONEB02)
#ifndef __DISP_PLATFORM_BOOT__

        if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen && !DispGetHdmiFunction())
        {
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
            #if defined (HI_HDMI_SUPPORT_2_0)
				HDMI_VIDEO_ATTR_S  stVideoAttr;
				if (HI_SUCCESS == DISPGetDispPara(enDisp,&stVideoAttr))
				{
					s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, &stVideoAttr);
				}
			#else
                s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, enEncFmt, enStereo);
			#endif
            }
        }

#endif
#endif
    pstDisp->bOpen = HI_TRUE;
	
    return nRet;
}

HI_VOID DispReleaseIntf(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;
    HI_DRV_DISP_INTF_ID_E i;

    DispGetPointerByIDNoReturn(enDisp, pstDisp);

    for (i = 0; i < (HI_S32)HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        if (pstDisp->stSetting.stIntf[i].bOpen)
        {
            DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

            // s1 release vdac
            if (pfOpt && pfOpt->PF_ReleaseIntf2)
            pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, &pstDisp->stSetting.stIntf[i]);

            // s3 set intf
            DispCleanIntf(&pstDisp->stSetting.stIntf[i]);
        }
    }

    return;
}

HI_S32 DISP_Close(HI_DRV_DISPLAY_E enDisp)
{
    DISP_S* pstDisp;

#ifdef HI_DISP_BUILD_FULL
    HI_DRV_DISP_CALLBACK_S stCB;
    HI_S32 nRet;
#endif

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 check whether display opened
    if (!DISP_IsOpened(enDisp))
    {
        DISP_INFO("Display is not opened!\n");
        return HI_SUCCESS;
    }

    // s4 set close state
    DispGetPointerByID(enDisp, pstDisp);

    // s3 Product ask for that display must be enabled at the same time.
    DISP_SetEnable(enDisp, HI_FALSE);
    
#ifndef __DISP_PLATFORM_BOOT__
    DRV_MCVN_DeInit(enDisp);
	DRV_CGMS_DeInit(&(pstDisp->stCgmsInfo));
#endif

#ifdef HI_DISP_BUILD_FULL
    stCB.hDst = (HI_VOID*)pstDisp;
    stCB.pfDISP_Callback = DISP_CB_PreProcess;
    nRet = DISP_ISR_UnRegCallback(enDisp, HI_DRV_DISP_C_INTPOS_0_PERCENT, &stCB);
    DISP_ASSERT(!nRet);

    // s4.2 delete isr
    DISP_ISR_CloseChn(enDisp);
#endif

    pstDisp->bOpen = HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 DispGetVactTime(DISP_S* pstDisp)
{
    HI_S32 vtime;

    if (pstDisp->stDispInfo.u32RefreshRate)
    {
        vtime = (1000 * 100) / pstDisp->stDispInfo.u32RefreshRate;
    }
    else
    {
        vtime = 50;
    }

    if (vtime > 50)
    {
        vtime = 50;
    }
    else if (vtime < 20)
    {
        vtime = 20;
    }

    return vtime;
}


HI_S32 DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    DISP_S* pstDisp;
    DISP_S* pstDispS;
    HI_S32 nRet, u;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);

    // s2 check state
    if (bEnable == pstDisp->bEnable)
    {
        DISP_PRINT(" DISP Set enable return!\n");
        return HI_SUCCESS;
    }

    //DISP_INFO("Set enable 0002  DISP=%d, IS M= %d\n", enDisp, pstDisp->bIsMaster);

    if (pstDisp->bIsSlave)
    {
        return DISP_SetEnable(pstDisp->enAttachedDisp, bEnable);
    }

    if (pstDisp->bIsMaster)
    {
        DispGetPointerByID(pstDisp->enAttachedDisp, pstDispS);
        DispCheckNullPointer(pfOpt);
        DispCheckNullPointer(pfOpt->PF_SetMSChnEnable);

        // s3 if Enable, set all config
        if (bEnable)
        {
            nRet = DispCheckReadyForOpen(enDisp);
            if (nRet)
            {
                DISP_ERROR("not  Ready to Open!\n");
                return nRet;
            }

            nRet = DispCheckReadyForOpen(pstDisp->enAttachedDisp);

            if (nRet)
            {
                DISP_ERROR("not  Ready to Open!\n");
                return nRet;
            }

            // s1 set FMT
            nRet = DispSetFormat(enDisp, pstDisp->stSetting.enFormat, pstDisp->stSetting.eDispMode);

            if (nRet)
            {
                DISP_ERROR("Set format failed\n");
                return nRet;
            }

            // s2 set CSC
            nRet = DispSetColor(pstDisp);

            if (nRet)
            {
                DISP_ERROR("Set color failed\n");
                return nRet;
            }


            // s1 set FMT
            nRet = DispSetFormat(pstDisp->enAttachedDisp, pstDispS->stSetting.enFormat,pstDispS->stSetting.eDispMode);

            if (nRet)
            {
                DISP_ERROR("Set format failed\n");
                return nRet;
            }

            // s2 set CSC
            nRet = DispSetColor(pstDispS);

            if (nRet)
            {
                DISP_ERROR("Set color failed\n");
                return nRet;
            }

            // s3 set enable
            pstDisp->eState = DISP_PRIV_STATE_DISABLE;
            DispSetIntfEnable(pstDisp, bEnable);

            pstDispS->eState = DISP_PRIV_STATE_DISABLE;
            DispSetIntfEnable(pstDispS, bEnable);

            pfOpt->PF_SetMSChnEnable(pstDisp->enDisp, pstDisp->enAttachedDisp, 0, bEnable);

#ifndef __DISP_PLATFORM_BOOT__
            HI_DRV_SYS_GetTimeStampMs(&pstDisp->u32StartTime);
#endif

            pstDisp->bEnable = bEnable;
            pstDispS->bEnable = bEnable;
        }
        else
        {
            HI_S32 vtime;

            // addtional 2ms delay for safe
            vtime = DispGetVactTime(pstDisp) + 2;

            // s1 set state and wait ISR Process
            pstDisp->bEnable = bEnable;
            pstDispS->bEnable = bEnable;

            DISP_MSLEEP(2 * vtime);

            u = 0;

            while (pstDisp->eState != DISP_PRIV_STATE_DISABLE)
            {
                DISP_MSLEEP(vtime);
                u++;

                if (u > DISP_SET_TIMEOUT_THRESHOLD)
                {
                    DISP_WARN("Set enable timeout\n");
                    break;
                }
            }

            // s2 set disable
            //DispSetEnable(pstDisp, bEnable);
            //DispSetEnable(pstDispS, bEnable);
            DispSetIntfEnable(pstDisp, bEnable);
            DispSetIntfEnable(pstDispS, bEnable);

            pfOpt->PF_SetMSChnEnable(pstDisp->enDisp, pstDisp->enAttachedDisp, vtime, bEnable);

#ifndef __DISP_PLATFORM_BOOT__
            HI_DRV_SYS_GetTimeStampMs(&pstDisp->u32StartTime);
#endif
            //DISP_INFO(">>>>>>>>>>>> close disp%d =0x%x\n", pstDisp->enDisp,pstDisp->u32StartTime);
        }
    }
    else
    {
        // s3 if Enable, set all config
        if (bEnable)
        {
            nRet = DispCheckReadyForOpen(enDisp);

            if (nRet)
            {
                return nRet;
            }

            // s1 set FMT
            nRet = DispSetFormat(enDisp, pstDisp->stSetting.enFormat, pstDisp->stSetting.eDispMode);

            if (nRet)
            {
                DISP_ERROR("Set format failed\n");
                return nRet;
            }

            // s2 set CSC
            nRet = DispSetColor(pstDisp);

            if (nRet)
            {
                DISP_ERROR("Set color failed\n");
                return nRet;
            }

            // s3 set enable
            pstDisp->eState = DISP_PRIV_STATE_DISABLE;
            DispSetEnable(pstDisp, bEnable);

#ifndef __DISP_PLATFORM_BOOT__
            HI_DRV_SYS_GetTimeStampMs(&pstDisp->u32StartTime);
#endif
            pstDisp->bEnable = bEnable;

            //DISP_INFO("disp%d =0x%x, ", pstDisp->enDisp,pstDisp->u32StartTime);
        }
        else
        {
            HI_S32 vtime;

            // addtional 2ms delay for safe
            vtime = DispGetVactTime(pstDisp) + 2;

            // s1 set state and wait ISR Process
            pstDisp->bEnable = bEnable;

            DISP_MSLEEP(2 * vtime);

            u = 0;

            while (pstDisp->eState != DISP_PRIV_STATE_DISABLE)
            {
                DISP_MSLEEP(vtime);
                u++;

                if (u > DISP_SET_TIMEOUT_THRESHOLD)
                {
                    DISP_WARN("Set enable timeout\n");
                    break;
                }
            }

            // s2 set disable
            DispSetEnable(pstDisp, bEnable);

            // s3 wait vdp diable really
            DISP_MSLEEP(vtime);
        }
    }

    return HI_SUCCESS;
}

HI_DRV_DISPLAY_E  DISPGetIntfChannel(HI_DRV_DISP_INTF_ID_E enIntfID)
{
    HI_DRV_DISPLAY_E enDisp = HI_DRV_DISPLAY_BUTT;
    HI_U32 i = 0;
    DISP_S* pstDisp = HI_NULL;

    for ( i = (HI_U32) HI_DRV_DISPLAY_0 ;i <=  (HI_U32) HI_DRV_DISPLAY_1 ; i++)
    {
        DispGetPointerByID((HI_DRV_DISPLAY_E) i, pstDisp);
        if (pstDisp->stSetting.stIntf[enIntfID].bOpen)
        {
            enDisp  = (HI_DRV_DISPLAY_E)i;
            break;
        }
    }
    return enDisp;
}


HI_S32 DispGetFmtTiming(HI_DRV_DISP_FMT_E eFmt,HI_DRV_DISP_TIMING_S *pstTiming)
{	
    HI_S32 nRet = HI_FAILURE;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    DispCheckNullPointer(pfOpt);

    // s1 set channel
    if (HI_DRV_DISP_FMT_CUSTOM >= eFmt)
	{
        nRet = pfOpt->PF_GetFmtTiming( eFmt, pstTiming);
	}

    if (HI_SUCCESS == nRet)
    {
        return HI_SUCCESS;
    }

	DISP_WARN("Get fmt %d err !(%d)\n",eFmt,nRet);
	return  nRet;
}
#ifndef __DISP_PLATFORM_BOOT__
HI_S32 DispSetHDMI420(HI_DRV_DISPLAY_E enDisp,HI_BOOL bEnable)
{

    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    DispCheckNullPointer(pfOpt);
	DispCheckNullPointer(pfOpt->PF_SetHDMI420);

	return  pfOpt->PF_SetHDMI420(enDisp,bEnable);
}

HI_S32 Disp_SetHDMI420(HI_DRV_DISPLAY_E enDisp,HI_BOOL bEnable)
{
	DispCheckDeviceState();
	DispCheckID(enDisp);

	return  DispSetHDMI420(enDisp ,bEnable);
}

#if defined (HI_HDMI_SUPPORT_2_0)

HI_S32  DISPGetDispPara(HI_DRV_DISPLAY_E enDisp ,HDMI_VIDEO_ATTR_S  *pstVideoAttr)
{
	DISP_S* pstDisp;
	HI_S32 nRet = HI_FAILURE;
	HI_DRV_DISP_TIMING_S  stTiming;
	DispCheckID(enDisp);
	DispGetPointerByID(enDisp, pstDisp);
	
	pstVideoAttr->enVideoFmt = pstDisp->stSetting.enFormat;
	nRet = DispGetFmtTiming(pstVideoAttr->enVideoFmt,&stTiming);
	if ( HI_SUCCESS != nRet )
	{
		
		pstVideoAttr->u32ClkFs = 0;
		pstVideoAttr->enHvSyncPol = DRV_HDMI_HV_SYNC_POL_HPVP;
		pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_BUTT;
		pstVideoAttr->u32ColorSpace = HI_DRV_CS_BT709_YUV_LIMITED;
		pstVideoAttr->u32DispRatioWidth = 16;
		pstVideoAttr->u32DispRatioHeight = 9;
		pstVideoAttr->u32PixelRepeat = 1;
		pstVideoAttr->enInBitDepth = HI_UNF_HDMI_DEEP_COLOR_30BIT;
		pstVideoAttr->enVideoFmt = HI_DRV_DISP_FMT_BUTT;
		
		DISP_WARN("DISPGetDispPara err nRet=%d !\n",nRet);
		return HI_SUCCESS;
	}
	else
	{
		if (stTiming.bIHS)
		{
			if (stTiming.bIVS)
				pstVideoAttr->enHvSyncPol = DRV_HDMI_HV_SYNC_POL_HNVN;
			else
				pstVideoAttr->enHvSyncPol = DRV_HDMI_HV_SYNC_POL_HNVP;
		}
		else
		{
			if (stTiming.bIVS)
				pstVideoAttr->enHvSyncPol = DRV_HDMI_HV_SYNC_POL_HPVN;
			else
				pstVideoAttr->enHvSyncPol = DRV_HDMI_HV_SYNC_POL_HPVP;
		}
		if ((HI_DRV_DISP_FMT_1080P_24_FP == pstVideoAttr->enVideoFmt )
			||(HI_DRV_DISP_FMT_720P_60_FP == pstVideoAttr->enVideoFmt )
			||(HI_DRV_DISP_FMT_720P_50_FP == pstVideoAttr->enVideoFmt )
			)
		{
			pstVideoAttr->u32ClkFs = stTiming.u32PixFreq*2;
		}
		else
		{
			pstVideoAttr->u32ClkFs = stTiming.u32PixFreq;
		}
	}
	
	/*need to do  start--->*/
	pstVideoAttr->u32ColorSpace = pstDisp->stSetting.stColor.enOutCS;
	pstVideoAttr->u32DispRatioWidth = pstDisp->stSetting.u32CustomRatioWidth;
	pstVideoAttr->u32DispRatioHeight = pstDisp->stSetting.u32CustomRatioHeight;
	pstVideoAttr->enInBitDepth = HI_UNF_HDMI_DEEP_COLOR_30BIT;

	if ((pstDisp->stSetting.enFormat >= HI_DRV_DISP_FMT_PAL  ) 
		&& (pstDisp->stSetting.enFormat <= HI_DRV_DISP_FMT_1440x480i_60 )
		)
	{
		pstVideoAttr->u32PixelRepeat = 2;
	}
	else
	{
		pstVideoAttr->u32PixelRepeat = 1;
	}

	switch(pstDisp->stSetting.eDispMode)
    {
		case DISP_STEREO_FPK:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_FRAME_PACKETING;
			break;
			
		case DISP_STEREO_SBS_HALF:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_SIDE_BY_SIDE_HALF;
			break;
			
		case DISP_STEREO_TAB:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_TOP_AND_BOTTOM;
			break;
		case DISP_STEREO_FIELD_ALTE:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_FIELD_ALTERNATIVE;
			break;
		case DISP_STEREO_LINE_ALTE:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_LINE_ALTERNATIVE;
			break;
		case DISP_STEREO_SBS_FULL:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_SIDE_BY_SIDE_FULL;
			break;
		case DISP_STEREO_L_DEPT:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_L_DEPTH;
			break;
		case DISP_STEREO_L_DEPT_G_DEPT:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_L_DEPTH_GRAPHICS_GRAPHICS_DEPTH;
			break;
		case DISP_STEREO_NONE:
			
		default:
			pstVideoAttr->enStereoMode = HI_UNF_EDID_3D_BUTT;
			break;
	}

	return HI_SUCCESS;
}

#endif
#endif

HI_S32 DISP_ExternSetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S* pstDisp;
    DISP_S* pstDispS;

    HI_S32 nRet;
    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);
    DispGetPointerByID(pstDisp->enAttachedDisp, pstDispS);

    if (bEnable == pstDisp->bEnable)
    {
        DISP_PRINT(" DISP Set enable return!\n");
        return HI_SUCCESS;
    }

    if (bEnable)
    {

        if ((HI_DRV_DISPLAY_0 == DISPGetIntfChannel(HI_DRV_DISP_INTF_CVBS0)))
        {
            //DISP_INFO("CVBS=====YPbPr======Delay=======500ms\n");
            DISP_MSLEEP(400);
        }
    }
    else
    {
        if ((enDisp == DISPGetIntfChannel(HI_DRV_DISP_INTF_HDMI0)) && !DispGetHdmiFunction())
         {
             if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
             {
				#if defined (HI_HDMI_SUPPORT_2_0)
				s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0);
				#else
                 //DISP_INFO("%s, %d-------hdmi-----disp%d--------close\n",__FUNCTION__, __LINE__,pstDisp->enDisp);
                 s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0, pstDisp->stSetting.enFormat);
				#endif
             }
         }
    }

    nRet = DISP_SetEnable(enDisp,bEnable);
    if (HI_SUCCESS != nRet)
    {
        DISP_ERROR("Display enable is failed\n");
        return nRet;
    }

     if (bEnable)
     {
        if ((enDisp == DISPGetIntfChannel(HI_DRV_DISP_INTF_HDMI0)) && !DispGetHdmiFunction())
        {
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
			#if defined (HI_HDMI_SUPPORT_2_0)
				HDMI_VIDEO_ATTR_S  stVideoAttr;
				if (HI_SUCCESS == DISPGetDispPara(enDisp,&stVideoAttr))
				{
					s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, &stVideoAttr);
				}
			#else
                 //DISP_INFO("%s, %d-------hdmi-----disp%d--------open\n",__FUNCTION__, __LINE__,pstDisp->enDisp);
                s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, pstDisp->stSetting.enFormat, pstDisp->stSetting.eDispMode);
			#endif
			}
        }
     }
#endif
     return HI_SUCCESS;

}

HI_S32 DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL* pbEnable)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pbEnable);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);

    *pbEnable = pstDisp->bEnable;

    return HI_SUCCESS;
}

#ifndef __DISP_PLATFORM_BOOT__
//#define DISP_DEBUG_TEST_SET_FORMAT_TIME 1
#endif
HI_S32 DISP_ReviseIntfRGB_VGA(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_FMT_E enEncFmt)
{
    DISP_S* pstDisp;
    DISP_INTF_S stDispIntfRGB;
    DISP_INTF_S stDispIntfVGA;

    DispGetPointerByID(enDisp, pstDisp);

    stDispIntfRGB = pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_RGB0];
    stDispIntfVGA = pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_VGA0];

    if  (((enEncFmt >= HI_DRV_DISP_FMT_861D_640X480_60) && (enEncFmt < HI_DRV_DISP_FMT_3840X2160_24))
        ||  (enEncFmt == HI_DRV_DISP_FMT_CUSTOM))
    {
        /*VESA or Custom FMT*/
        if (stDispIntfRGB.bOpen)
        {
            DISP_DelIntf(enDisp, &stDispIntfRGB.stIf);
            stDispIntfRGB.stIf.eID = HI_DRV_DISP_INTF_VGA0;
            DISP_AddIntf(enDisp, &stDispIntfRGB.stIf);
        }
    }
    else
    {
        if (stDispIntfVGA.bOpen)
        {
            DISP_DelIntf(enDisp, &stDispIntfVGA.stIf);
            stDispIntfVGA.stIf.eID = HI_DRV_DISP_INTF_RGB0;
            DISP_AddIntf(enDisp, &stDispIntfVGA.stIf);
        }
    }
    return HI_SUCCESS;
}

extern HI_DRV_DISP_FMT_E DISP_Disp0FmtRevise_Attach(HI_DRV_DISP_FMT_E U);



HI_S32 DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enEncFmt)
{
    DISP_S* pstDisp;
    HI_DRV_DISP_FMT_E enEncFmt2;
#ifndef __DISP_PLATFORM_BOOT__
    HI_DRV_DISP_MACROVISION_E enMode;
#endif

#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
    struct timeval tv;
    HI_U32 t2, t1, t0;
#endif

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    enEncFmt2 = DispTransferFormat(enDisp, enEncFmt);

    //DISP_INFO("DISP_SetFormat  disp(%d) :mode (%d) fmt(%d)\n",enDisp,enStereo,enEncFmt);
    if (enEncFmt2 > HI_DRV_DISP_FMT_CUSTOM)
    {
        DISP_ERROR("Display fmt is invalid\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

#ifndef HI_DISP_DOUBLE_HD_SUPPORT
    if (((HI_DRV_DISPLAY_0 == enDisp) && (pstDisp->bIsSlave))
        && ((enEncFmt2 > HI_DRV_DISP_FMT_SECAM_H) || (enEncFmt2 < HI_DRV_DISP_FMT_PAL))
       )
    {
        DISP_ERROR("attach mode  disp0 not support fmt(%d)\n", enEncFmt2);
        return HI_ERR_DISP_INVALID_PARA;
    }
#endif

    if ( (enEncFmt2 == pstDisp->stSetting.enFormat)
         && (enStereo == (HI_DRV_DISP_STEREO_MODE_E)pstDisp->stSetting.eDispMode)
         && (enEncFmt2 < HI_DRV_DISP_FMT_CUSTOM))
    {
        DISP_PRINT(" DISP Set same format and return!\n");
        return HI_SUCCESS;
    }

    if (!pstDisp->pstIntfOpt->PF_TestChnEncFmt(pstDisp->enDisp, enEncFmt2))
    {
        DISP_ERROR("Display %d does not support fmt %d\n", (HI_S32)enDisp, (HI_S32)enEncFmt2);
        return HI_ERR_DISP_INVALID_PARA;
    }
	
#ifndef HI_VO_DISP_ISOLATED_FMT_SUPPORT
    // If hd work at follow-mode, set display0 sd format
    if (   (enDisp == HI_DRV_DISPLAY_1) && pstDisp->bIsMaster
           && DispFmtIsStandDefinition(enEncFmt))
    {
        DISP_SetFormat(pstDisp->enAttachedDisp, HI_DRV_DISP_STEREO_NONE, enEncFmt);
    }
#endif

    pstDisp->stSetting.eDispMode = enStereo;
    pstDisp->stSetting.enFormat = enEncFmt2;
#ifndef __DISP_PLATFORM_BOOT__
    (HI_VOID)DRV_MCVN_GetMacrovision(enDisp, &enMode);
    (HI_VOID)DRV_MCVN_SetMacrovision(enDisp, 
                                enMode,
                                enEncFmt2);

	{
		DISP_CGMS_STATE_S stState;
		HI_DRV_DISP_CGMSA_CFG_S stCgmsCfg;

		DRV_CGMS_Get(&(pstDisp->stCgmsInfo),&stState);

		stCgmsCfg.bEnable = stState.bEnable; 
		stCgmsCfg.enType = stState.enType;
		stCgmsCfg.enMode = stState.enMode; 
		
		DRV_CGMS_Set(&(pstDisp->stCgmsInfo),pstDisp->stSetting.enFormat,stCgmsCfg); 
	}
	
#endif
    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    if (pstDisp->bEnable)
    {
#ifndef __DISP_PLATFORM_BOOT__

        //if ((enDisp == HI_DRV_DISPLAY_1) && (s_pstHDMIFunc))

    if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen && !DispGetHdmiFunction())
        {
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
				#if defined (HI_HDMI_SUPPORT_2_0)
				s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0);
				#else
                s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0, enEncFmt);
				#endif
            }
        }
		
#ifdef HI_VO_DISP_ISOLATED_FMT_SUPPORT
		if ( (enDisp == HI_DRV_DISPLAY_1) && pstDisp->bIsMaster)
        {
            HI_DRV_DISP_FMT_E  enEncFmtTmp = 0;
            enEncFmtTmp = DISP_Disp0FmtRevise_Attach(enEncFmt);
            DISP_SetFormat(pstDisp->enAttachedDisp, HI_DRV_DISP_STEREO_NONE, enEncFmtTmp);
        }
#endif

#endif

#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
        HI_DRV_SYS_GetTimeStampMs(&t0);
#endif

        (HI_VOID)DISP_SetEnable(enDisp, HI_FALSE);
        /*
        if interface is RGB mode ;
        when fmt switch to VESA or Custom Fmt :
            1:change to VGA interface!
            2:HDMI Data tpye use RGB ;TV FMT use YUV!
        */



#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
        HI_DRV_SYS_GetTimeStampMs(&t1);
#endif

        if (enDisp == HI_DRV_DISPLAY_0)
        {
            // DTS2013060905670 : if time between setdisable and set enable is
            // less than 160ms, the screen on TV linked in CVBS flicker.
            // Increase time interval, flicker disappear.
            // set diable use time 60ms
            //DISP_MSLEEP(400);
            DISP_MSLEEP(500);
        }

        (HI_VOID)DISP_SetEnable(enDisp, HI_TRUE);

#ifdef DISP_DEBUG_TEST_SET_FORMAT_TIME
        HI_DRV_SYS_GetTimeStampMs(&t2);
        DISP_FATAL("disable use time=%d, enable use time=%d ms\n", t1 - t0, t2 - t1);
#endif

#ifndef __DISP_PLATFORM_BOOT__

        if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen && !DispGetHdmiFunction())
        {
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
            #if defined (HI_HDMI_SUPPORT_2_0)
				HDMI_VIDEO_ATTR_S  stVideoAttr;
				if (HI_SUCCESS == DISPGetDispPara(enDisp,&stVideoAttr))
				{
					s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, &stVideoAttr);
				}
			#else
                s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, enEncFmt, enStereo);
			#endif
            }
        }

#endif
    }

    return HI_SUCCESS;
}

HI_S32 DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E* penStereo, HI_DRV_DISP_FMT_E* penEncFmt)
{
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penStereo);
    DispCheckNullPointer(penEncFmt);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 if display is not enabled, set format and return
    *penStereo = pstDisp->stSetting.eDispMode;
    *penEncFmt = pstDisp->stSetting.enFormat;

    return HI_SUCCESS;
}

HI_S32 DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    pstDisp->bDispSettingChange = HI_FALSE;
    pstDisp->stSetting.bRightEyeFirst = bEnable;
    pstDisp->bDispSettingChange = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 DISP_SetVirtScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S virtscreen)
{
    DISP_S *pstDisp = HI_NULL, *pstDisp_attach = HI_NULL;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);

    #if 0
    if (pstDisp->bOpen)
    {
         if ((pstDisp->stSetting.stVirtaulScreen.s32Width
             == (virtscreen.s32Width & 0xFFFFFFFEul))
             && (pstDisp->stSetting.stVirtaulScreen.s32Height
             == (virtscreen.s32Height & 0xFFFFFFFCul)))
         {

         }
         else
         {
            DISP_ERROR("Disp %d is opened ,can't set VirtScreen.\n",enDisp);
            return HI_ERR_DISP_NOT_SUPPORT;
         }
    }
    #endif
    if ((virtscreen.s32Height < HI_DRV_DISP_VIRTSCREEN_MIN)
        || (virtscreen.s32Height > HI_DRV_DISP_VIRTSCREEN_MAX)
        || (virtscreen.s32Width < HI_DRV_DISP_VIRTSCREEN_MIN)
        || (virtscreen.s32Width > HI_DRV_DISP_VIRTSCREEN_MAX))
    {
        DISP_ERROR("screen window too small ,must be within [480,3840].\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (pstDisp->bIsMaster)
    {
        pstDisp->bDispSettingChange = HI_FALSE;
        pstDisp->stSetting.stVirtaulScreen.s32Width = virtscreen.s32Width & 0xFFFFFFFEul;
        pstDisp->stSetting.stVirtaulScreen.s32Height = virtscreen.s32Height & 0xFFFFFFFCul;
        pstDisp->bDispSettingChange = HI_TRUE;

        DispGetPointerByID(pstDisp->enAttachedDisp, pstDisp_attach);
        if(pstDisp_attach->bIsSlave)
        {
            pstDisp_attach->bDispSettingChange = HI_FALSE;
            pstDisp_attach->stSetting.stVirtaulScreen.s32Width = virtscreen.s32Width & 0xFFFFFFFEul;
            pstDisp_attach->stSetting.stVirtaulScreen.s32Height = virtscreen.s32Height & 0xFFFFFFFCul;
            pstDisp_attach->bDispSettingChange = HI_TRUE;
        }
    }
    else if (pstDisp->bIsSlave)
    {
        /*non same-source, vscreen can be set.*/
        if (pstDisp->enAttachedDisp >= HI_DRV_DISPLAY_BUTT)
        {
            pstDisp->bDispSettingChange = HI_FALSE;
            pstDisp->stSetting.stVirtaulScreen.s32Width = virtscreen.s32Width & 0xFFFFFFFEul;
            pstDisp->stSetting.stVirtaulScreen.s32Height = virtscreen.s32Height & 0xFFFFFFFCul;
            pstDisp->bDispSettingChange = HI_TRUE;
        }
        else
        {
            DispGetPointerByID(pstDisp->enAttachedDisp, pstDisp_attach);
            if (!pstDisp_attach->bIsMaster)
            {
                pstDisp->bDispSettingChange = HI_FALSE;
                pstDisp->stSetting.stVirtaulScreen.s32Width = virtscreen.s32Width & 0xFFFFFFFEul;
                pstDisp->stSetting.stVirtaulScreen.s32Height = virtscreen.s32Height & 0xFFFFFFFCul;
                pstDisp->bDispSettingChange = HI_TRUE;
            }
            else
            {
                return HI_ERR_DISP_NOT_SUPPORT;
            }
        }
    } else {
        pstDisp->bDispSettingChange = HI_FALSE;
        pstDisp->stSetting.stVirtaulScreen.s32Width = virtscreen.s32Width & 0xFFFFFFFEul;
        pstDisp->stSetting.stVirtaulScreen.s32Height = virtscreen.s32Height & 0xFFFFFFFCul;
        pstDisp->bDispSettingChange = HI_TRUE;
    }

    return HI_SUCCESS;
}

HI_S32 DISP_GetVirtScreen(HI_DRV_DISPLAY_E enDisp, HI_RECT_S* virtscreen)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);

    *virtscreen = pstDisp->stSetting.stVirtaulScreen;

    return HI_SUCCESS;
}

#define HI_DRV_DISP_OFFSET_MAX 200
#define HI_DRV_DISP_OFFSET_HORIZONTAL_ALIGN 0xFFFFFFFEul
#define HI_DRV_DISP_OFFSET_VERTICAL_ALIGN   0xFFFFFFFCul

HI_VOID DispHD2SDOffset(HI_DRV_DISP_OFFSET_S *pstHDOffsetInfo, HI_DRV_DISP_OFFSET_S *pstSDOffsetInfo,HI_RECT_S stHDFmtRect,HI_RECT_S stSDFmtRect)
{		
	/*do correspoding tranlation according the hd and sd fmt.*/
	if ( 0 == pstHDOffsetInfo->u32Left)
	{
		pstSDOffsetInfo->u32Left = 0;
	}
	else
	{
		pstSDOffsetInfo->u32Left = (stSDFmtRect.s32Width * pstHDOffsetInfo->u32Left)/stHDFmtRect.s32Width;
	}

	if ( 0 == pstHDOffsetInfo->u32Top)
	{
		pstSDOffsetInfo->u32Right = 0;
	}
	else
	{
		pstSDOffsetInfo->u32Right = (stSDFmtRect.s32Width * pstHDOffsetInfo->u32Right)/stHDFmtRect.s32Width;
	}
	
	if ( 0 == pstHDOffsetInfo->u32Top)
	{
		pstSDOffsetInfo->u32Top = 0;
	}
	else
	{
		pstSDOffsetInfo->u32Top = (stSDFmtRect.s32Height * pstHDOffsetInfo->u32Top)/stHDFmtRect.s32Height;
	}

	if ( 0 == pstHDOffsetInfo->u32Bottom)
	{
		pstSDOffsetInfo->u32Bottom = 0;
	}
	else
	{
		pstSDOffsetInfo->u32Bottom = (stSDFmtRect.s32Height * pstHDOffsetInfo->u32Bottom)/stHDFmtRect.s32Height;
	}

	
    pstSDOffsetInfo->u32Left   = pstSDOffsetInfo->u32Left   & HI_DRV_DISP_OFFSET_HORIZONTAL_ALIGN;
    pstSDOffsetInfo->u32Right  = pstSDOffsetInfo->u32Right  & HI_DRV_DISP_OFFSET_HORIZONTAL_ALIGN;
    pstSDOffsetInfo->u32Top    = pstSDOffsetInfo->u32Top    & HI_DRV_DISP_OFFSET_VERTICAL_ALIGN;
    pstSDOffsetInfo->u32Bottom = pstSDOffsetInfo->u32Bottom & HI_DRV_DISP_OFFSET_VERTICAL_ALIGN;

}
HI_S32 DISP_SetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S *pstScreenOffset)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstScreenOffset);

    if (  (pstScreenOffset->u32Bottom > HI_DRV_DISP_OFFSET_MAX)
        ||(pstScreenOffset->u32Left   > HI_DRV_DISP_OFFSET_MAX)
        ||(pstScreenOffset->u32Right  > HI_DRV_DISP_OFFSET_MAX)
        ||(pstScreenOffset->u32Top    > HI_DRV_DISP_OFFSET_MAX))
    {
        DISP_ERROR("screen offset must less then 200: %d,%d,%d,%d.\n",
                   pstScreenOffset->u32Left, pstScreenOffset->u32Right,
                   pstScreenOffset->u32Top, pstScreenOffset->u32Bottom);
        return HI_ERR_DISP_INVALID_PARA;
    }

    /*
        if ((screenoffset.u32Bottom & 0x3)
            ||(screenoffset.u32Left & 0x3)
            ||(screenoffset.u32Right & 0x3)
            ||(screenoffset.u32Top   & 0x3))
        {
            DISP_ERROR("screen offset not aligned:%d,%d,%d,%d.\n", screenoffset.u32Bottom,
                                                               screenoffset.u32Left,
                                                               screenoffset.u32Right,
                                                               screenoffset.u32Top);
            return HI_ERR_DISP_INVALID_PARA;
        }
    */
    pstDisp->bDispSettingChange = HI_FALSE;
    pstDisp->stSetting.stOffsetInfo.u32Left   = pstScreenOffset->u32Left   & HI_DRV_DISP_OFFSET_HORIZONTAL_ALIGN;
    pstDisp->stSetting.stOffsetInfo.u32Right  = pstScreenOffset->u32Right  & HI_DRV_DISP_OFFSET_HORIZONTAL_ALIGN;
    pstDisp->stSetting.stOffsetInfo.u32Top    = pstScreenOffset->u32Top    & HI_DRV_DISP_OFFSET_VERTICAL_ALIGN;
    pstDisp->stSetting.stOffsetInfo.u32Bottom = pstScreenOffset->u32Bottom & HI_DRV_DISP_OFFSET_VERTICAL_ALIGN;
    pstDisp->bDispSettingChange = HI_TRUE;

	 if (pstDisp->bIsMaster)
    {
		DISP_S* pSlivestDisp;
		DISP_HAL_ENCFMT_PARAM_S  stHDFmtPara;
		DISP_HAL_ENCFMT_PARAM_S  stSDFmtPara;
		DispGetPointerByID(HI_DRV_DISPLAY_0, pSlivestDisp);
		(HI_VOID)DISP_HAL_GetEncFmtPara(pstDisp->stSetting.enFormat, &stHDFmtPara);
		(HI_VOID)DISP_HAL_GetEncFmtPara(pSlivestDisp->stSetting.enFormat, &stSDFmtPara);
		pSlivestDisp->bDispSettingChange = HI_FALSE;
		DispHD2SDOffset(&pstDisp->stSetting.stOffsetInfo,&pSlivestDisp->stSetting.stOffsetInfo,stHDFmtPara.stRefRect,stSDFmtPara.stRefRect);
       	pSlivestDisp->bDispSettingChange = HI_TRUE;
    }
    return HI_SUCCESS;
}

HI_S32 DISP_GetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S* pstScreenOffset)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstScreenOffset);

    *pstScreenOffset = pstDisp->stSetting.stOffsetInfo;
    return HI_SUCCESS;
}

//set aspect ratio
HI_S32 DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Ratio_h, HI_U32 u32Ratio_v)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    pstDisp->bDispSettingChange = HI_FALSE;

    if (u32Ratio_h && u32Ratio_v)
    {
        pstDisp->stSetting.bCustomRatio = HI_TRUE;
        pstDisp->stSetting.u32CustomRatioWidth  = u32Ratio_h;
        pstDisp->stSetting.u32CustomRatioHeight = u32Ratio_v;
    }
    else
    {
        pstDisp->stSetting.bCustomRatio = HI_FALSE;
    }

    pstDisp->bDispSettingChange = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32* pu32Ratio_h, HI_U32* pu32Ratio_v)
{
    DISP_S* pstDisp;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pu32Ratio_h);
    DispCheckNullPointer(pu32Ratio_v);

    // s3 check state
    DispGetPointerByID(enDisp, pstDisp);

    // s4 if display is enabled, disable it and enable it,
    //    and new format will work.
    if (pstDisp->stSetting.bCustomRatio)
    {
        *pu32Ratio_h = pstDisp->stSetting.u32CustomRatioWidth;
        *pu32Ratio_v = pstDisp->stSetting.u32CustomRatioHeight;
    }
    else
    {
        *pu32Ratio_h = 0;
        *pu32Ratio_v = 0;
    }

    return HI_SUCCESS;
}

HI_S32 DISPCheckCustomTiming(HI_DRV_DISP_TIMING_S* pstTiming)
{
    if ((pstTiming->bIDV != HI_TRUE)
        && (pstTiming->bIDV != HI_FALSE)
       )
    {
        DISP_ERROR("para pstTiming->IDV is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstTiming->bIHS != HI_TRUE)
        && (pstTiming->bIHS != HI_FALSE)
       )
    {
        DISP_ERROR("para pstTiming->IHS is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstTiming->bIVS != HI_TRUE)
        && (pstTiming->bIVS != HI_FALSE)
       )
    {
        DISP_ERROR("para pstTiming->IVS is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstTiming->bClkReversal != HI_TRUE)
        && (pstTiming->bClkReversal != HI_FALSE)
       )
    {
        DISP_ERROR("para pstTiming->ClockReversal is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (pstTiming->eDataFmt >= HI_DRV_DISP_INTF_DATA_FMT_BUTT)
    {
        DISP_ERROR("para pstTiming->ItfFormat is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstTiming->bDitherEnable != HI_TRUE)
        && (pstTiming->bDitherEnable != HI_FALSE)
       )
    {
        DISP_ERROR("para pstTiming->DitherEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    //DISP_INFO("custom  ------------H,W(%d,%d)",pstTiming->u32AspectRatioH,pstTiming->u32AspectRatioW);
    if ((( pstTiming->u32AspectRatioW ) && ( pstTiming->u32AspectRatioH ))
        &&( (pstTiming->u32AspectRatioW >= (pstTiming->u32AspectRatioH * 16))
         || ( pstTiming->u32AspectRatioH >= (pstTiming->u32AspectRatioW * 16)))
       )
    {

        DISP_ERROR(" DISP Set Custom Timing AspectRatio err!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ( ( !pstTiming->u32ClkPara0 ) ||  ( !pstTiming->u32ClkPara1 ))
    {
        if (pstTiming->u32PixFreq )
        {
            if ((20000 > pstTiming->u32PixFreq) || (600000 < pstTiming->u32PixFreq))
            {
                DISP_ERROR("u32PixClk (%d)out of rang  (20000~600000)!!\n", pstTiming->u32PixFreq);
                return HI_ERR_DISP_INVALID_PARA;
            }
        }
        else
        {
             if ((pstTiming->u32VertFreq <= 20000) || ( pstTiming->u32VertFreq >= 120000))
            {
                DISP_ERROR(" para pstTiming->u32VertFreq err!\n");
                return HI_ERR_DISP_INVALID_PARA;
            }
        }
    }

    return HI_SUCCESS;

}

HI_S32 DISPCheckCustomTimingIsSet(HI_DRV_DISP_TIMING_S* pstTiming, HI_DRV_DISP_TIMING_S* pstSetting)
{
    if ( (pstSetting->u32HBB != pstTiming->u32HBB)
        || (pstSetting->u32HACT != pstTiming->u32HACT)
        || (pstSetting->u32HFB != pstTiming->u32HFB)
        || (pstSetting->u32HPW != pstTiming->u32HPW)

        || (pstSetting->u32VACT != pstTiming->u32VACT)
        || (pstSetting->u32VBB != pstTiming->u32VBB)
        || (pstSetting->u32VFB  != pstTiming->u32VFB)
        || (pstSetting->u32VPW != pstTiming->u32VPW)

        || (pstSetting->bIDV != pstTiming->bIDV)
        || (pstSetting->bIHS != pstTiming->bIHS)
        || (pstSetting->bIVS != pstTiming->bIVS)
        || (pstSetting->bInterlace != pstTiming->bInterlace)
        || (pstSetting->u32VertFreq != pstTiming->u32VertFreq)
        )
    {

        return HI_ERR_DISP_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 DISP_SetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S* pstTiming)
{
    DISP_S* pstDisp;
    HI_S32 nRet;
    HI_DRV_DISP_STEREO_MODE_E enStereo ;
    HI_DRV_DISP_FMT_E enEncFmt;

    DispCheckDeviceState();

    DispCheckID(enDisp);
    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstTiming);

    nRet = DISPCheckCustomTiming(pstTiming);
    if (HI_SUCCESS != nRet)
    {
        DISP_ERROR("Check Custom Timing Para err! (%d)\n", nRet);
        return nRet;
    }


    if (HI_DRV_DISP_FMT_CUSTOM == pstDisp->stSetting.enFormat )
    {
        nRet = DISPCheckCustomTimingIsSet(&pstDisp->stSetting.stCustomTimg,pstTiming);
        if (HI_SUCCESS == nRet)
        {
            DISP_ERROR("set same custom timing!\n");
            return nRet;
        }
    }

    enStereo = HI_DRV_DISP_STEREO_NONE;
    enEncFmt = HI_DRV_DISP_FMT_CUSTOM;

    if ((! pstTiming->u32AspectRatioW) || (! pstTiming->u32AspectRatioH ))
    {
        pstTiming->u32AspectRatioW = pstTiming->u32HACT;
        pstTiming->u32AspectRatioH = pstTiming->u32VACT;
    }

    pstDisp->stSetting.eDispMode = enStereo;
    pstDisp->stSetting.enFormat = enEncFmt;
    pstDisp->stSetting.stCustomTimg = *pstTiming;

#ifndef __DISP_PLATFORM_BOOT__
    if (pstDisp->bEnable)
    {
         // s4 if display is enabled, disable it and enable it,
        //    and new format will work.
        if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen && !DispGetHdmiFunction())
        {
            if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
            {
				#if defined (HI_HDMI_SUPPORT_2_0)
				s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0);
				#else
                s_pstHDMIFunc->pfnHdmiPreFormat(HI_UNF_HDMI_ID_0, enEncFmt);
				#endif
            }
        }
    }
#endif

    nRet = DISP_SetEnable(enDisp, HI_FALSE);
    if (HI_SUCCESS != nRet )
    {
        DISP_ERROR("set custom timing disenable disp%d err \n",enDisp);
       goto TIMING_EXIT0;
    }

    nRet = DISP_SetEnable(enDisp, HI_TRUE);
     if (HI_SUCCESS != nRet )
    {
        DISP_ERROR("set custom timing enable disp%d err \n",enDisp);
        goto TIMING_EXIT0;
    }

#ifndef __DISP_PLATFORM_BOOT__
    if (pstDisp->stSetting.stIntf[HI_DRV_DISP_INTF_HDMI0].bOpen && !DispGetHdmiFunction())
    {
        if (s_pstHDMIFunc->pfnHdmiPreFormat && s_pstHDMIFunc->pfnHdmiSetFormat)
        {
			#if defined (HI_HDMI_SUPPORT_2_0)
			HDMI_VIDEO_ATTR_S  stVideoAttr;
			if (HI_SUCCESS == DISPGetDispPara(enDisp,&stVideoAttr))
			{
				s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, &stVideoAttr);
			}
			#else
            s_pstHDMIFunc->pfnHdmiSetFormat(HI_UNF_HDMI_ID_0, enEncFmt, enStereo);
			#endif
        }
    }
#endif
    return nRet;

TIMING_EXIT0:
    pstDisp->stSetting.enFormat = HI_DRV_DISP_FMT_BUTT;
    return nRet;
}
HI_S32 DISP_GetCustomTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S* pstTiming)
{
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstTiming);

    *pstTiming = pstDisp->stSetting.stCustomTimg;

    return HI_SUCCESS;
}

HI_S32 DISP_SetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S* pstBGColor)
{
    DISP_S* pstDisp;
    HI_S32 Ret = HI_SUCCESS;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstBGColor);

    //todo check color setting
    /*
        DISP_PRINT("xxxxxxDISP_SetBGColor R=%d,G=%d,B=%d\n",
                     pstBGColor->u8Red,
                     pstBGColor->u8Green, pstBGColor->u8Blue);
    */
    pstDisp->stSetting.stBgColor = *pstBGColor;

    if (pstDisp->bEnable)
    {
        Ret = DispSetColor(pstDisp);
        if (Ret != HI_SUCCESS)
        { goto __SET_BGCOLOR__; }
    }

__SET_BGCOLOR__:
    if (pstDisp->bIsMaster)
    {
        //DISP_PRINT("DISP_SetColor, attech  disp ID = %d\n", pstDisp->enAttachedDisp);
        Ret = DISP_SetBGColor(pstDisp->enAttachedDisp, pstBGColor);
        if (Ret != HI_SUCCESS)
        { goto __SET_BGCOLOR_EXIT_; }
    }

__SET_BGCOLOR_EXIT_:
    return Ret;
}
HI_S32 DISP_GetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S* pstBGColor)
{
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstBGColor);

    //todo check color setting

    *pstBGColor = pstDisp->stSetting.stBgColor;

    return HI_SUCCESS;
}

HI_S32 DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_DRV_DISP_LAYER_E enUpLayer;
    DISP_S *pstDisp = HI_NULL;
    DispGetPointerByID(enDisp, pstDisp);

    if ((HI_DRV_DISP_ZORDER_MOVETOP == enZFlag) || (HI_DRV_DISP_ZORDER_MOVEUP == enZFlag))
    {
        /*if  move top!*/
        enUpLayer = (enLayer == HI_DRV_DISP_LAYER_VIDEO) ? HI_DRV_DISP_LAYER_VIDEO : HI_DRV_DISP_LAYER_GFX;
    }
    else
    {
         /*if  move bottom!*/
         enUpLayer = (enLayer == HI_DRV_DISP_LAYER_VIDEO)?HI_DRV_DISP_LAYER_GFX:HI_DRV_DISP_LAYER_VIDEO;
    }

    if (enUpLayer == HI_DRV_DISP_LAYER_VIDEO)
    {
        pstDisp->stSetting.enLayer[0] = HI_DRV_DISP_LAYER_GFX;
        pstDisp->stSetting.enLayer[1] = HI_DRV_DISP_LAYER_VIDEO;
    }
    else
    {
        pstDisp->stSetting.enLayer[0] = HI_DRV_DISP_LAYER_VIDEO;
        pstDisp->stSetting.enLayer[1] = HI_DRV_DISP_LAYER_GFX;
    }

    pstDisp->pstIntfOpt->PF_CBM_MovTOP(enDisp,enUpLayer);

    /*if attach mode ,set Attached disp too!*/
    if (pstDisp->bIsMaster)
        DISP_SetLayerZorder(pstDisp->enAttachedDisp,enLayer,enZFlag);

    return HI_SUCCESS;
}

HI_S32 DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder)
{
    DISP_S *pstDisp = HI_NULL;
    HI_U32 index,i;
    DispGetPointerByID(enDisp, pstDisp);

    index = 0;
    for (i =0; i < HI_DRV_DISP_LAYER_BUTT ; i++)
    {
        if (enLayer == pstDisp->stSetting.enLayer[i])
        {
            index = i;
            break;
        }
    }

    *pu32Zorder = index;

    return HI_SUCCESS;
}

HI_S32 DISP_TestMacrovisionSupport(HI_DRV_DISPLAY_E enDisp, HI_BOOL* pbSupport)
{

    return HI_SUCCESS;
}

#ifdef HI_DISP_BUILD_FULL
//snapshot
HI_S32 DISP_AcquireSnapshot(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S* pstSnapShotFrame, HI_VOID **snapshotHandleOut)
{
    DISP_CAST_S* pstCast;
    HI_S32 Ret;
    HI_S32 i = 0;
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckID(enDisp);

    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstSnapShotFrame);
    if (!pstDisp->bOpen || !pstDisp->bEnable)
    {
        DISP_ERROR("Disp not open, cannot snapshot!\n");
        return HI_ERR_DISP_NOT_OPEN;
    }

    if (!pstDisp->pstIntfOpt->PF_TestChnSupportCast(pstDisp->enDisp))
    {
        DISP_ERROR("Disp %d not support cast!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }

    /* reget pstCast for user may call cast_destroy. */
    pstCast = (DISP_CAST_S*)pstDisp->Cast_ptr;

    if (pstCast)
    {
        /* notify cast to shedule wbc. */
        pstCast->bScheduleWbc = HI_TRUE;;
        pstCast->u32Ref++;
    retry:

        if (pstCast->bEnable)
        {
            msleep(40);

            if (!pstCast->bScheduleWbcStatus && ++i < 3)
            {
                goto retry;
            }

            if (!pstCast->bScheduleWbcStatus)
            {
                Ret = HI_ERR_DISP_TIMEOUT;
                goto out;
            }
        }
    }

    Ret = DISP_Acquire_Snapshot(enDisp, snapshotHandleOut, pstSnapShotFrame);

out:
    pstCast = (DISP_CAST_S*)pstDisp->Cast_ptr;
    if (pstCast)
    {
        pstCast->bScheduleWbc = HI_FALSE;
        pstCast->bScheduleWbcStatus = HI_FALSE;
        /* In order to decrease pstCast->u32Ref. Maybe do some destroy actually */
        DISP_DestroyCast(pstDisp->hCast);
    }

    return Ret;
}
HI_S32 DISP_ReleaseSnapshot(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S* pstSnapShotFrame, HI_VOID* snapshotHandle)
{
    HI_S32 Ret;
    DISP_S* pstDisp;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispGetPointerByID(enDisp, pstDisp);
    DispCheckNullPointer(pstSnapShotFrame);
    if (!pstDisp->bOpen || !pstDisp->bEnable)
    {
        DISP_ERROR("Disp not open, cannot snapshot!\n");
        return HI_ERR_DISP_NOT_OPEN;
    }

    if (!pstDisp->pstIntfOpt->PF_TestChnSupportCast(pstDisp->enDisp))
    {
        DISP_ERROR("Disp %d not support cast!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }

    Ret = DISP_Release_Snapshot(enDisp, snapshotHandle, pstSnapShotFrame);
    return Ret;
}


HI_S32 DISP_DestroySnapshot(HI_VOID* hSnapshot)
{
    HI_S32 nRet;

    DispCheckDeviceState();

    if (!hSnapshot)
        return HI_ERR_DISP_NULL_PTR;

    nRet = DISP_SnapshotDestroy(hSnapshot);

    return nRet;
}


//miracast
HI_S32 DISP_CreateCast(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S* pstCfg, HI_HANDLE* phCast)
{
    DISP_S* pstDisp;
    HI_S32 nRet;
    DISP_CAST_S* cast_ptr = HI_NULL;
    HI_DISP_DISPLAY_INFO_S pstInfo;

    /* check input parameters*/
    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCfg);
    DispCheckNullPointer(phCast);

    /*check open status.*/
    DispShouldBeOpened(enDisp);


    /*check cast support .*/
    DispGetPointerByID(enDisp, pstDisp);

    if (!pstDisp->pstIntfOpt->PF_TestChnSupportCast(pstDisp->enDisp))
    {
        DISP_ERROR("Disp %d not support cast!\n", (HI_S32)enDisp);
        return HI_ERR_DISP_INVALID_OPT;
    }

    (HI_VOID) DISP_GetDisplayInfo(enDisp,  &pstInfo);

    nRet = DISP_CastCreate(enDisp, &pstInfo, pstCfg, &cast_ptr);

    if (!nRet)
    {
        /*hcast is a user handle containing of mod id info and display channel.
         *for there is 1:1 relationship bettween display channel and cast handle.*/
        pstDisp->hCast      = (HI_ID_DISP << 16) | enDisp;
        *phCast              = pstDisp->hCast;

        /*this is a cast instance, we can get it from DISP_S struct definition.*/
        pstDisp->Cast_ptr   = cast_ptr;
    }

    DISP_WARN("DISP_CreateCast  pstDisp->hCast = 0x%x, cast_ptr:0x%x\n", (HI_U32)pstDisp->hCast, pstDisp->Cast_ptr);

    return nRet;
}

HI_S32 DISP_DestroyCast(HI_HANDLE hCast)
{
    HI_DRV_DISPLAY_E enDisp;
    DISP_S* pstDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;
    DISP_CAST_S* pstCast;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    // s2 TODO: search display
    enDisp  =  hCast & 0xff;
    nRet = DispSearchCastHandle(&cast_ptr, enDisp);

    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    // s3 check whether display opened
    DispShouldBeOpened(enDisp);
    // s4 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    pstCast = (DISP_CAST_S*)cast_ptr;

    if (--pstCast->u32Ref > 0)
    {
        return HI_SUCCESS;
    }

    // s5 destroy cast
    nRet = DISP_CastDestroy(cast_ptr);

    pstDisp->hCast      = HI_NULL;
    pstDisp->Cast_ptr   = HI_NULL;

    return nRet;
}

HI_S32 DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);
    DISP_WARN("DISP_SetCastEnable  hCast = 0x%x\n", (HI_U32)hCast);

    // s2 TODO: search display
    enDisp  =  (HI_DRV_DISPLAY_E)(hCast & 0xff);
    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_CastSetEnable(cast_ptr, bEnable);
    return nRet;
}


HI_S32 DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL* pbEnable)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);
    DispCheckNullPointer(pbEnable);

    // s2 TODO: search display
    enDisp  = (HI_DRV_DISPLAY_E)(hCast & 0xff);
    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_CastGetEnable(cast_ptr, pbEnable);
    return nRet;
}

HI_S32 DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S* pstCastFrame)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    // s2 TODO: search display
    enDisp  = (HI_DRV_DISPLAY_E)( hCast & 0xff);
    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_CastAcquireFrame(cast_ptr, pstCastFrame);
    return nRet;
}

HI_S32 DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S* pstCastFrame)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);
    DispCheckNullPointer(pstCastFrame);

    // s2 TODO: search display
    enDisp  = (HI_DRV_DISPLAY_E)(hCast & 0xff);
    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_CastReleaseFrame(cast_ptr, pstCastFrame);
    return nRet;
}

HI_S32 DISP_External_Attach(HI_HANDLE hCast, HI_HANDLE hsink)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    // s2 TODO: search display
    enDisp  =  (HI_DRV_DISPLAY_E)(hCast & 0xff);
    // s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_Cast_AttachSink(cast_ptr, hsink);
    return nRet;
}

HI_S32 DISP_External_DeAttach(HI_HANDLE hCast, HI_HANDLE hsink)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    // s1 check input parameters
    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    // s2 TODO: search display
    enDisp  =  (HI_DRV_DISPLAY_E)(hCast & 0xff);
    //s3 check whether display opened
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_Cast_DeAttachSink(cast_ptr, hsink);
    return nRet;
}

/*currently, this function only called by venc to change cast resolution. */
HI_S32 DRV_DISP_SetCastAttr(HI_HANDLE hCast, HI_DRV_DISP_Cast_Attr_S *castAttr)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    enDisp  = (HI_DRV_DISPLAY_E)(hCast & 0xff);
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_Cast_SetAttr(cast_ptr, castAttr);
    return nRet;
}

HI_S32 DRV_DISP_GetCastAttr(HI_HANDLE hCast, HI_DRV_DISP_Cast_Attr_S *castAttr)
{
    HI_DRV_DISPLAY_E enDisp;
    HI_S32 nRet;
    HI_VOID* cast_ptr = 0;

    DispCheckDeviceState();
    DispCheckCastHandleValid(hCast);

    enDisp  =  (HI_DRV_DISPLAY_E)(hCast & 0xff);
    DispShouldBeOpened(enDisp);

    nRet = DispSearchCastHandle(&cast_ptr, enDisp);
    if (nRet)
    {
        DISP_ERROR("DISP cast not exist!\n");
        return HI_ERR_DISP_NOT_EXIST;
    }

    nRet = DISP_Cast_GetAttr(cast_ptr, castAttr);
    return nRet;
}
HI_S32 DispGetCastHandle(HI_DRV_DISPLAY_E enDisp, HI_HANDLE *phCast, HI_VOID **phCast_ptr)
{
    DISP_S *pstDisp = HI_NULL;
    DispGetPointerByID(enDisp, pstDisp);

    *phCast = pstDisp->hCast;
    *phCast_ptr = pstDisp->Cast_ptr;
    return  HI_SUCCESS;
}
#endif

HI_S32 DISP_SetMacrovisionCustomer(HI_DRV_DISPLAY_E enDisp, HI_VOID* pData)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S* pstDisp;
    
    DispCheckDeviceState();

    DispCheckID(enDisp);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);
    
    if (enMode >= HI_DRV_DISP_MACROVISION_BUTT)
    {
        HI_ERR_DISP("DISP invalid parameters!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    return DRV_MCVN_SetMacrovision(enDisp,enMode,pstDisp->stSetting.enFormat);
#else
    return HI_SUCCESS;
#endif
}

HI_S32 DISP_GetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E* penMode)
{
#ifndef __DISP_PLATFORM_BOOT__
    return DRV_MCVN_GetMacrovision(enDisp,penMode);
#else
    return HI_SUCCESS;
#endif
}

//cgms-a
HI_S32 DISP_SetCGMS(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S* pstCfg)
{
#ifndef __DISP_PLATFORM_BOOT__
    DISP_S* pstDisp;
	HI_S32 s32Ret;

    DispCheckDeviceState();

    DispCheckNullPointer(pstCfg);

    DispGetPointerByID(enDisp, pstDisp);

	s32Ret = DRV_CGMS_Set(&(pstDisp->stCgmsInfo),pstDisp->stSetting.enFormat,*pstCfg);

    return s32Ret;
#else
    return HI_SUCCESS;
#endif
}


//vbi
HI_S32 DISP_CreateVBIChannel(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_VBI_CFG_S* pstCfg, HI_HANDLE* phVbi)
{

    return HI_SUCCESS;
}

HI_S32 DISP_DestroyVBIChannel(HI_HANDLE hVbi)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SendVbiData(HI_HANDLE hVbi, HI_DRV_DISP_VBI_DATA_S* pstVbiData)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetWss(HI_HANDLE hVbi, HI_DRV_DISP_WSS_DATA_S* pstWssData)
{

    return HI_SUCCESS;
}


//may be deleted
HI_S32 DISP_SetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S* pstCfg)
{

    return HI_SUCCESS;
}

HI_S32 DISP_GetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S* pstCfg)
{

    return HI_SUCCESS;
}

HI_S32 DISP_SetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S* pstSetting)
{

    return HI_SUCCESS;
}

HI_S32 DISP_GetSetting(HI_DRV_DISPLAY_E enDisp, DISP_SETTING_S* pstSetting)
{

    return HI_SUCCESS;
}


HI_S32 DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S* pstIntf)
{
    DISP_S* pstDisp;
    HI_S32 nRet;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(pstIntf);
    //DISP_INFO("add****DISP_AddIntf**************** (%d)(%d)(%d)\n",pstIntf->eID,pstIntf->u8VDAC_Pb_B,pstIntf->u8VDAC_Pr_R);
    nRet = DispCheckIntfValid(pstIntf);

    if (nRet)
    {
        DISP_ERROR("Invalid intf parameters in %s!\n", __FUNCTION__);
        return HI_ERR_DISP_INVALID_PARA;
    }

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);



    // s3.2 check whether eIntf exists
    if (DispCheckIntfExist(enDisp, pstIntf))
    {
        return HI_SUCCESS;
    }

    DispPrepareInft(pstIntf);

    // s3.1 check whether eIntf is supported
    nRet = DispAddIntf(pstDisp, pstIntf);

    if ( pstDisp->bEnable && !nRet)
    {
        //DISP_INFO(">>>>> xb1\n");
        DispCheckNullPointer(pfOpt);
        DispCheckNullPointer(pfOpt->PF_ResetIntfFmt2);
        DispCheckNullPointer(pfOpt->PF_SetIntfEnable2);
        pfOpt->PF_ResetIntfFmt2(pstDisp->enDisp,
                                DispGetIntfPtr(pstDisp, pstIntf->eID),
                                pstDisp->stSetting.enFormat, &pstDisp->stSetting.stCustomTimg);

        pfOpt->PF_SetIntfEnable2(pstDisp->enDisp,
                                 DispGetIntfPtr(pstDisp, pstIntf->eID),
                                 HI_TRUE);
    }

    return nRet;
}

HI_S32 DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S* pstIntf)
{
    DISP_S* pstDisp;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    DISP_INTF_S* pstIf;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(pstIntf);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!DispCheckIntfExistByType(enDisp, pstIntf))
    {
        return HI_ERR_DISP_NOT_EXIST;
    }

    pstIf = DispGetIntfPtr(pstDisp, pstIntf->eID);

    DispCheckNullPointer(pfOpt);
    DispCheckNullPointer(pfOpt->PF_SetIntfEnable2);
    DispCheckNullPointer(pfOpt->PF_ReleaseIntf2);
    // s4 set intf disable
    pfOpt->PF_SetIntfEnable2(pstDisp->enDisp, pstIf, HI_FALSE);

    // s5 release intf
    pfOpt->PF_ReleaseIntf2(pstDisp->enDisp, pstIf);

    DispCleanIntf(pstIf);

    if (HI_DRV_DISP_INTF_HDMI0 == pstIntf->eID)
    {
        DispPrepareHDMI(pstIntf->eID);
    }

    if  (( pstIntf->eID >= HI_DRV_DISP_INTF_BT1120_0 ) && (pstIntf->eID <= HI_DRV_DISP_INTF_LCD2 ))
    {
        DispPrepareLCD_BT1120(pstIntf->eID);
    }

    return HI_SUCCESS;
}


HI_S32 DISP_GetSlave(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E* penSlave)
{
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penSlave);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!pstDisp->bIsMaster)
    {
        return HI_ERR_DISP_INVALID_OPT;
    }

    *penSlave = pstDisp->enAttachedDisp;

    return HI_SUCCESS;
}

HI_S32 DISP_GetMaster(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISPLAY_E* penMaster)
{
    DISP_S* pstDisp;

    // s1 check input parameters
    DispCheckDeviceState();

    DispCheckID(enDisp);
    DispCheckNullPointer(penMaster);

    // s2 get pointer
    DispGetPointerByID(enDisp, pstDisp);

    // s3 check if eIntf exists
    if (!pstDisp->bIsSlave)
    {
        return HI_ERR_DISP_INVALID_OPT;
    }

    *penMaster = pstDisp->enAttachedDisp;

    return HI_SUCCESS;
}

HI_S32 DISP_GetDisplaySetting(HI_DRV_DISPLAY_E enDisp,
            HI_DRV_DISP_FMT_E *penFormat,
            HI_DRV_DISP_STEREO_E *peDispMode)
{
    DISP_S* pstDisp;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();

    DispCheckDeviceState();
    // s1 check input parameters
    DispCheckID(enDisp);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);

    if ( (pstDisp->stSetting.enFormat < HI_DRV_DISP_FMT_BUTT) && pfOpt )
    {
        *penFormat = pstDisp->stSetting.enFormat;
        *peDispMode = pstDisp->stSetting.eDispMode;
    }
    else
    {
        DISP_ERROR("Display %d info not available!\n", enDisp);
        return HI_ERR_DISP_NO_INIT;
    }

    return HI_SUCCESS;
}


HI_S32 DISP_GetDisplayInfo(HI_DRV_DISPLAY_E enDisp, HI_DISP_DISPLAY_INFO_S* pstInfo)
{
    DISP_S* pstDisp;
    DISP_INTF_OPERATION_S* pfOpt = DISP_HAL_GetOperationPtr();
    HI_BOOL bBtm;
    HI_U32 vcnt;

    //DISP_HAL_ENCFMT_PARAM_S stFmt;
    //HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstInfo);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    // s3 get ENABLE state and return
    DispGetPointerByID(enDisp, pstDisp);

    if ( (pstDisp->stSetting.enFormat < HI_DRV_DISP_FMT_BUTT) && pfOpt )
    {
        *pstInfo = pstDisp->stDispInfo;
        DispCheckNullPointer(pfOpt->FP_GetChnBottomFlag);

        pfOpt->FP_GetChnBottomFlag(enDisp, &bBtm, &vcnt);

        pstInfo->bIsBottomField = bBtm;
        pstInfo->u32Vline = vcnt;
    }
    else
    {
        DISP_ERROR("Display %d info not available!\n", enDisp);
        return HI_ERR_DISP_NO_INIT;
    }

    return HI_SUCCESS;
}


#ifdef HI_DISP_BUILD_FULL
HI_S32 DISP_RegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                        HI_DRV_DISP_CALLBACK_S* pstCB)
{
    //    DISP_S *pstDisp;
    //    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCB);

    // s2 check whether display opened
    DispShouldBeOpened(enDisp);

    if (!pstCB->pfDISP_Callback)
    {
        DISP_ERROR("Callback function is null!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    nRet = DISP_ISR_RegCallback(enDisp, eType, pstCB);

    return nRet;
}

HI_S32 DISP_UnRegCallback(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CALLBACK_TYPE_E eType,
                          HI_DRV_DISP_CALLBACK_S* pstCB)
{
    //    DISP_S *pstDisp;
    //    DISP_HAL_ENCFMT_PARAM_S stFmt;
    HI_S32 nRet;

    DispCheckDeviceState();

    // s1 check input parameters
    DispCheckID(enDisp);
    DispCheckNullPointer(pstCB);

    // s2 check whether display opened
    //DispShouldBeOpened(enDisp);

    if (!pstCB->pfDISP_Callback)
    {
        DISP_ERROR("Callback function is null!\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    nRet = DISP_ISR_UnRegCallback(enDisp, eType, pstCB);

    return nRet;
}

HI_VOID DISP_GetCastInfor(DISP_CAST_S *pstCast_info, DISP_Cast_PROC_INFO_S *pstCastProc)
{
    HI_U32 i = 0;

    if (pstCast_info)
    {
        pstCastProc->u32CastEmptyBufferNum = 0;
        pstCastProc->u32CastFullBufferNum = 0;
        pstCastProc->u32CastWriteBufferNum = 0;
        pstCastProc->u32CastUsedBufferNum  = 0;

        for (i = 0; i < pstCast_info->stBP.u32BufNum; i++)
        {
            switch (pstCast_info->stBP.pstBufQueue[i].enState)
            {
                case BUF_STATE_EMPTY:
                    pstCastProc->u32CastEmptyBufferNum ++;
                    break;
                case BUF_STATE_FULL:
                    pstCastProc->u32CastFullBufferNum ++;
                    break;
                case BUF_STATE_WRITING:
                    pstCastProc->u32CastWriteBufferNum ++;
                    break;
                case BUF_STATE_READING:
                case BUF_STATE_DONE:
                    pstCastProc->u32CastUsedBufferNum ++;
                    break;
                default:
                    break;
            }
        }

        pstCastProc->bEnable = pstCast_info->bEnable;
        pstCastProc->bLowDelay = pstCast_info->bLowDelay;
        pstCastProc->bUserAllocate = pstCast_info->stConfig.bUserAlloc;
        pstCastProc->u32OutResolutionWidth  = pstCast_info->stAttr.stOut.s32Width;
        pstCastProc->u32OutResolutionHeight = pstCast_info->stAttr.stOut.s32Height;
        pstCastProc->u32CastOutFrameRate = pstCast_info->stDispInfo.u32RefreshRate;
        pstCastProc->u32TotalBufNum = pstCast_info->stBP.u32BufNum;
        pstCastProc->u32BufSize  = pstCast_info->stBP.u32BufSize;
        pstCastProc->u32BufStride = pstCast_info->stBP.u32BufStride;

        pstCastProc->u32CastAcquireTryCnt = pstCast_info->u32CastAcquireTryCnt;
        pstCastProc->u32CastAcquireOkCnt = pstCast_info->u32CastAcquireOkCnt;
        pstCastProc->u32CastReleaseTryCnt = pstCast_info->u32CastReleaseTryCnt;
        pstCastProc->u32CastReleaseOkCnt = pstCast_info->u32CastReleaseOkCnt;
        pstCastProc->u32CastIntrCnt = pstCast_info->u32CastIntrCnt;
        pstCastProc->bAttached = pstCast_info->bAttached;

        for (i = 0; i < pstCastProc->u32TotalBufNum; i++)
        {
            pstCastProc->enState[i] = (HI_U32)pstCast_info->stBP.pstBufQueue[i].enState;
            pstCastProc->u32FrameIndex[i] = pstCast_info->stBP.pstBufQueue[i].stFrame.u32FrameIndex;
        }
    }

    return;

}


HI_S32 DISP_GetProcInto(HI_DRV_DISPLAY_E enDisp, DISP_PROC_INFO_S* pstInfo)
{
    DISP_S* pstDisp;
    HI_S32 i;

    DispCheckDeviceState();
    DispCheckID(enDisp);
    DispCheckNullPointer(pstInfo);
    DispShouldBeOpened(enDisp);
    DispGetPointerByID(enDisp, pstDisp);

    /*the channel such as display0,or 1 enable or not.*/
    pstInfo->bEnable         = pstDisp->bEnable;
    pstInfo->eFmt            = pstDisp->stSetting.enFormat;
    pstInfo->eDispMode       = pstDisp->stSetting.eDispMode;
    pstInfo->bRightEyeFirst  = pstDisp->stSetting.bRightEyeFirst;
    pstInfo->stVirtaulScreen = pstDisp->stSetting.stVirtaulScreen;
    pstInfo->stOffsetInfo    = pstDisp->stSetting.stOffsetInfo;

    /*get the aspect setting.*/
    pstInfo->bCustAspectRatio = pstDisp->stSetting.bCustomRatio;
    pstInfo->u32AR_w       = pstDisp->stDispInfo.stAR.u32ARw;
    pstInfo->u32AR_h       = pstDisp->stDispInfo.stAR.u32ARh;

    /*get the csc space transfer.*/
    pstInfo->eDispColorSpace = pstDisp->stDispInfo.eColorSpace;

    /*get the display csc setting.*/
    pstInfo->u32Bright = pstDisp->stDispInfo.u32Bright;
    pstInfo->u32Hue = pstDisp->stDispInfo.u32Hue;
    pstInfo->u32Satur = pstDisp->stDispInfo.u32Satur;
    pstInfo->u32Contrst = pstDisp->stDispInfo.u32Contrst;

    /*get the background setting.*/
    pstInfo->stBgColor = pstDisp->stSetting.stBgColor;

    /*get the zorder, which one is on the top or bottom.*/
    for (i = 0; i < HI_DRV_DISP_LAYER_BUTT; i++)
    {
        pstInfo->enLayer[i] = pstDisp->stSetting.enLayer[i];
    }

    /*get the master or slave role, and attached layer.*/
    pstInfo->bMaster = pstDisp->bIsMaster;
    pstInfo->bSlave  = pstDisp->bIsSlave;
    pstInfo->enAttachedDisp = pstDisp->enAttachedDisp;

    /*get the unflow times.*/
    pstInfo->u32Underflow = pstDisp->u32Underflow;

    /*FIXME: i don't know what does it means.*/
    pstInfo->u32StartTime = pstDisp->u32StartTime;
    pstInfo->stTiming   = pstDisp->stSetting.stCustomTimg;
    pstInfo->stColorSetting  = pstDisp->stSetting.stColor;

    /*get the cast information.*/
    pstInfo->pstCastInfor = pstDisp->Cast_ptr;

    DISP_GetCastInfor((DISP_CAST_S*)pstDisp->Cast_ptr, &pstInfo->stCastInfor);



    pstInfo->u32IntfNumber = 0;
    for (i = 0; i < HI_DRV_DISP_INTF_ID_MAX; i++)
    {
        if (pstDisp->stSetting.stIntf[i].bOpen)
        {
            pstInfo->stIntf[pstInfo->u32IntfNumber] = pstDisp->stSetting.stIntf[i].stIf;
            pstInfo->u32Link[pstInfo->u32IntfNumber] = pstDisp->stSetting.stIntf[i].eVencId;
            pstInfo->u32IntfNumber++;
        }
    }

    return HI_SUCCESS;
}

#if ((!defined(__DISP_PLATFORM_BOOT__)) && defined(DAC_TYPE_SYNOPSYS))

HI_S32 DISP_SetGammaCtrl(HI_DRV_DISPLAY_E eDisp, GAMMA_MODE_E enGammaMode, HI_BOOL bEnable)
{
    HI_S32 s32Ret;
    DISP_S* pstDisp;
    GAMMA_CS_E enGammCsMode  ;
    // s1 check input parameters
    // DispCheckDeviceState();
    DispCheckID(eDisp);

    if (GAMMA_PQ_MODE_BUTT <= enGammaMode)
    {
        DISP_PRINT("DISP_SetGammaCtrl, Gamma mode = %d error\r\n", enGammaMode);
        return HI_ERR_DISP_INVALID_PARA;
    }
    // s2 get pointer
    DispGetPointerByID(eDisp, pstDisp);

    //s3 get disp color space mode
    if (HI_DRV_DISP_FMT_861D_640X480_60 > pstDisp->stSetting.enFormat)
    {
        enGammCsMode = GAMMA_MODE_YUV;
    }
    else
    {
        enGammCsMode = GAMMA_MODE_RGB;
    }

    //check para
    s32Ret = pstDisp->pstIntfOpt->PF_SetGammaCtrl(eDisp, enGammCsMode, enGammaMode, bEnable);

    return s32Ret;
}
HI_S32 DISP_UpdateGamma(GAMMA_CS_E  enGammaCsMode, PQ_GAMMA_RGB_MODE_S* pstPqGammaModeData)
{
    HI_S32 s32Ret;
    HI_DRV_DISPLAY_E eDisp;
    DISP_S* pstDisp;

    eDisp = HI_DRV_DISPLAY_0;
    // s1 check input parameters
    //DispCheckDeviceState();
    DispCheckID(eDisp);

    if (GAMMA_MODE_YUV < enGammaCsMode)
    {
        DISP_PRINT("DISP_UpdateGamma, cs mode = %d error\r\n", enGammaCsMode);
        return HI_ERR_DISP_INVALID_PARA;
    }

    // s2 get pointer
    DispGetPointerByID(eDisp, pstDisp);
    DispCheckNullPointer(pstPqGammaModeData);

    s32Ret =  pstDisp->pstIntfOpt->PF_UpdateGamma(enGammaCsMode, pstPqGammaModeData);

    return s32Ret;
}
#endif

#endif

#ifndef __DISP_PLATFORM_BOOT__
/* ****************************************************
 *  O5 data
 * *****************************************************/
HI_U32      g_O5_u32DacPower = 0;
HI_U32      g_O5_u32DacType = 0;
HI_U32      g_O5_u32Signal = 0;
HI_U8       g_O5_u8MacvTable[18];
HI_U32     g_O5_u8MacvFlag = 0;
/**************************************************************/
HI_S32  DRV_DISP_O5_EnableDacPower(HI_BOOL bEnable)
{
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();

    if (!pfHal)
    {
        DISP_ERROR("--Get NULL PTR!\n");
        return HI_FAILURE;
    }

    pfHal->PF_SetAllDACEn(bEnable);
    g_O5_u32DacPower = (bEnable) ? 0xff : 0;

    return HI_SUCCESS;
}
HI_S32  DRV_DISP_O5_GetDacPower(HI_BOOL *bEnable)
{
    *bEnable = (g_O5_u32DacPower) ? HI_TRUE : HI_FALSE;
    return HI_SUCCESS;
}
HI_U32* DRV_DISP_O5_GetSignal(HI_VOID)
{
    return &g_O5_u32Signal;
}
HI_S32  DRV_DISP_O5_SetSignal(HI_U32 u32DacType, HI_U32 flag)
{
    /* bit define see opentv/src/devmgr/include/pi/pividenc.h*/
    #if 0
    /* cvbs output  */
    if (flag ==  0x01)
    {
        Disp_SetDacPower(DISP_DAC_MODE_CVBS, u32DacType);
    }
    /* RGB output  */
    if (flag == 0x02)
    {
        Disp_SetDacPower(DISP_DAC_MODE_R, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_G, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_B, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_R, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_G, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_B, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_Y, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_PB, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_HD_PR, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_Y, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_PB, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_PR, u32DacType);
    }
    /* svideo output */
    if (flag == 0x04)
    {
        Disp_SetDacPower(DISP_DAC_MODE_SVIDEO_Y, u32DacType);
        Disp_SetDacPower(DISP_DAC_MODE_SVIDEO_C, u32DacType);
    }
    #endif
    DISP_INTF_OPERATION_S* pfHal = DISP_HAL_GetOperationPtr();

    if (!pfHal)
    {
        DISP_ERROR("--get null ptr!\n");
        return HI_FAILURE;
    }

    pfHal->PF_SetAllDACEn(HI_TRUE);

    return HI_SUCCESS;
}

HI_S32  DRV_DISP_O5_SetMacv(HI_BOOL bEnable)
{
    HI_U32     u32macv[5];
    HI_S32      ret;

    g_O5_u8MacvFlag = bEnable;

    /*TODO: convert user macvision table to 5 bytes registers  */
    if (bEnable)
        memcpy(u32macv, g_O5_u8MacvTable, 5);
    else
        memset(u32macv, 0x0, 5);

    ret = DISP_SetMacrovision(HI_DRV_DISPLAY_0, HI_DRV_DISP_MACROVISION_CUSTOMER);

    if (!ret)
        DISP_SetMacrovisionCustomer(HI_DRV_DISPLAY_0, u32macv);

    return ret;

}

HI_U32  DRV_DISP_O5_GetMacv(HI_VOID)
{
    return g_O5_u8MacvFlag;
}

HI_S32  DRV_DISP_O5_SetMacvCps(HI_CHAR *ptable)
{
    memcpy(g_O5_u8MacvTable, ptable, 18);
    DRV_DISP_O5_SetMacv(HI_TRUE);
    return HI_SUCCESS;
}

HI_S32  DRV_DISP_O5_GetCGMS(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S *pstcgms)
{
    return HI_SUCCESS;
}

HI_S32  DRV_DISP_O5_GetWSS(HI_DRV_DISP_WSS_DATA_S *pstWssData)
{
    return HI_SUCCESS;
}

HI_S32  DRV_DISP_O5_SetCGMS(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CGMSA_CFG_S* pstCgmsCfg)
{
    (HI_VOID)DISP_SetCGMS(enDisp, pstCgmsCfg);

    return HI_SUCCESS;
}
HI_S32  DRV_DISP_O5_SetWss(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_WSS_DATA_S *pstWssData)
{
    (HI_VOID)DISP_SetWss(enDisp, pstWssData);

    return HI_SUCCESS;
}

EXPORT_SYMBOL(DRV_DISP_O5_EnableDacPower);
EXPORT_SYMBOL(DRV_DISP_O5_GetDacPower);
EXPORT_SYMBOL(DRV_DISP_O5_GetSignal);
EXPORT_SYMBOL(DRV_DISP_O5_SetSignal);
EXPORT_SYMBOL(DRV_DISP_O5_SetMacv);
EXPORT_SYMBOL(DRV_DISP_O5_GetMacv);
EXPORT_SYMBOL(DRV_DISP_O5_SetMacvCps);
EXPORT_SYMBOL(DRV_DISP_O5_GetCGMS);
EXPORT_SYMBOL(DRV_DISP_O5_GetWSS);
EXPORT_SYMBOL(DRV_DISP_O5_SetCGMS);
EXPORT_SYMBOL(DRV_DISP_O5_SetWss);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


