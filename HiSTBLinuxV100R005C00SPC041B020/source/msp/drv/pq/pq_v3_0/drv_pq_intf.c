/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : drv_pq_intf.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/04/01
  Author      : p00203646
******************************************************************************/
#include "drv_pq.h"
#include "hi_drv_dev.h"
#include "drv_pq_define.h"



#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

DEFINE_SEMAPHORE(g_stPQSemaphore);

extern HI_S32  DRV_PQ_Suspend(PM_BASEDEV_S* pdev, pm_message_t state);
extern HI_S32  DRV_PQ_Resume(PM_BASEDEV_S* pdev);

static HI_S32 PQ_Open(struct inode* node, struct file* filp)
{
    return HI_SUCCESS;
}

static HI_S32 PQ_Close(struct inode* node, struct file* filp)
{
    return HI_SUCCESS;
}

static HI_S32 PQIoctl(struct inode* inode, struct file* filp, unsigned int cmd, HI_VOID* arg)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = down_interruptible(&g_stPQSemaphore);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_PQ("Acquire PQ mutex failed!\n");
        return s32Ret;
    }

    switch (cmd)
    {
        case HIIOC_PQ_S_REGISTER:
        {
            HI_PQ_REGISTER_S* pstReg;

            pstReg = (HI_PQ_REGISTER_S*)arg;
            s32Ret = DRV_PQ_SetReg(pstReg);

            break;
        }
        case HIIOC_PQ_G_REGISTER:
        {
            HI_PQ_REGISTER_S* pstReg;

            pstReg = (HI_PQ_REGISTER_S*)arg;
            s32Ret = DRV_PQ_GetReg(pstReg);

            break;
        }

        case HIIOC_PQ_S_ACM_LUMA:
        {
            HI_PQ_ACM_LUT_S* pstAttr;
            pstAttr = (HI_PQ_ACM_LUT_S*)arg;

            s32Ret = DRV_PQ_SetAcmLuma(pstAttr);
            break;
        }
        case HIIOC_PQ_G_ACM_LUMA:
        {
            COLOR_LUT_S* pstAttr;
            pstAttr = (COLOR_LUT_S*)arg;

            s32Ret = PQ_HAL_GetACMLumaTbl(pstAttr);
            break;
        }
        case HIIOC_PQ_S_ACM_HUE:
        {
            HI_PQ_ACM_LUT_S* pstAttr;
            pstAttr = (HI_PQ_ACM_LUT_S*)arg;

            s32Ret = DRV_PQ_SetAcmHue(pstAttr);
            break;
        }
        case HIIOC_PQ_G_ACM_HUE:
        {
            COLOR_LUT_S* pstAttr;
            pstAttr = (COLOR_LUT_S*)arg;

            s32Ret = PQ_HAL_GetACMHueTbl(pstAttr);
            break;
        }
        case HIIOC_PQ_S_ACM_SAT:
        {
            HI_PQ_ACM_LUT_S* pstAttr;
            pstAttr = (HI_PQ_ACM_LUT_S*)arg;

            s32Ret = DRV_PQ_SetAcmSat(pstAttr);
            break;
        }
        case HIIOC_PQ_G_ACM_SAT:
        {
            COLOR_LUT_S* pstAttr;
            pstAttr = (COLOR_LUT_S*)arg;

            s32Ret = PQ_HAL_GetACMSatTbl(pstAttr);
            break;
        }

        case HIIOC_PQ_S_DCI:
        {
            DCI_WGT_S* pstAttr;
            pstAttr = (DCI_WGT_S*)arg;
            s32Ret = DRV_PQ_SetDCIWgtLut(pstAttr);

            break;
        }
        case HIIOC_PQ_G_DCI:
        {
            DCI_WGT_S* pstAttr;

            pstAttr = (DCI_WGT_S*)arg;
            s32Ret = PQ_HAL_GetDCIWgtLut(pstAttr);

            break;
        }

        case HIIOC_PQ_G_DCI_HIST:
        {
            DCI_HISTGRAM_S* pstAttr;

            pstAttr = (DCI_HISTGRAM_S*)arg;
            s32Ret = PQ_HAL_GetDCIHistgram(pstAttr);

            break;
        }
        case HIIOC_PQ_S_DCI_LEVEL:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetDCILevelGain(u32Level);

            break;
        }
        case HIIOC_PQ_G_DCI_LEVEL:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*) arg;
            s32Ret = PQ_MNG_GetDCILevelGain(pu32Level);

            break;
        }

        case HIIOC_PQ_S_SD_BRIGHTNESS:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSDBrightness(u32Level);

            break;
        }
        case HIIOC_PQ_G_SD_BRIGHTNESS:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSDBrightness(pu32Level);

            break;
        }

        case HIIOC_PQ_S_SD_CONTRAST:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSDContrast(u32Level);

            break;
        }
        case HIIOC_PQ_G_SD_CONTRAST:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSDContrast(pu32Level);

            break;
        }

        case HIIOC_PQ_S_SD_SATURATION:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSDSaturation(u32Level);

            break;
        }
        case HIIOC_PQ_G_SD_SATURATION:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSDSaturation(pu32Level);

            break;
        }
        case HIIOC_PQ_S_HDR_SATURATION:
        {
            break;
        }
        case HIIOC_PQ_G_HDR_SATURATION:
        {
            break;
        }

        case HIIOC_PQ_S_SD_HUE:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSDHue(u32Level);

            break;
        }
        case HIIOC_PQ_G_SD_HUE:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSDHue(pu32Level);

            break;
        }

        case HIIOC_PQ_S_HD_BRIGHTNESS:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetHDBrightness(u32Level);

            break;
        }
        case HIIOC_PQ_G_HD_BRIGHTNESS:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetHDBrightness(pu32Level);

            break;
        }

        case HIIOC_PQ_S_HD_CONTRAST:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetHDContrast(u32Level);

            break;
        }
        case HIIOC_PQ_G_HD_CONTRAST:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetHDContrast(pu32Level);

            break;
        }

        case HIIOC_PQ_S_HD_SATURATION:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetHDSaturation(u32Level);

            break;
        }
        case HIIOC_PQ_G_HD_SATURATION:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetHDSaturation(pu32Level);

            break;
        }

        case HIIOC_PQ_S_HD_HUE:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetHDHue(u32Level);

            break;
        }
        case HIIOC_PQ_G_HD_HUE:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetHDHue(pu32Level);

            break;
        }

        case HIIOC_PQ_S_TNR:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetTnrLevel(u32Level);

            break;
        }

        case HIIOC_PQ_G_TNR:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetTnrLevel(pu32Level);

            break;
        }
		
		case HIIOC_PQ_S_SNR:
        {

            break;
        }

        case HIIOC_PQ_G_SNR:
        {

            break;
        }

        case HIIOC_PQ_S_SHARPNESS:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSharpness(u32Level);

            break;
        }
        case HIIOC_PQ_G_SHARPNESS:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSharpness(pu32Level);

            break;
        }

        case HIIOC_PQ_S_DB:
        {
            break;
        }
        case HIIOC_PQ_G_DB:
        {
            break;
        }

        case HIIOC_PQ_S_DR:
        {
            break;
        }
        case HIIOC_PQ_G_DR:
        {
            break;
        }

        case HIIOC_PQ_S_COLORGAIN:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetColorEhance(u32Level);

            break;
        }
        case HIIOC_PQ_G_COLORGAIN:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetColorEhance(pu32Level);

            break;
        }

        case HIIOC_PQ_S_FLESHTONE:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetFleshToneLevel(u32Level);

            break;
        }
        case HIIOC_PQ_G_FLESHTONE:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetFleshToneLevel(pu32Level);

            break;
        }

        case HIIOC_PQ_S_SIXBASECOLOR:
        {
            HI_PQ_SIX_BASE_S* pstAttr;

            pstAttr = (HI_PQ_SIX_BASE_S*)arg;
            s32Ret = DRV_PQ_SetSixBaseColorLevel(pstAttr);

            break;
        }

        case HIIOC_PQ_G_SIXBASECOLOR:
        {
            SIX_BASE_COLOR_OFFSET_S* pstAttr;
            pstAttr = (SIX_BASE_COLOR_OFFSET_S*)arg;

            s32Ret = PQ_MNG_GetSixBaseColorLevel(pstAttr);

            break;
        }

        case HIIOC_PQ_S_COLOR_ENHANCE_MODE:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetColorEnhanceMode(u32Level);

            break;
        }

        case HIIOC_PQ_G_COLOR_ENHANCE_MODE:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = PQ_MNG_GetColorEnhanceMode(pu32Level);

            break;
        }

        case HIIOC_PQ_S_MODULE:
        {
            HI_PQ_MODULE_S* pstAttr;
            pstAttr = (HI_PQ_MODULE_S*)arg;

            s32Ret = DRV_PQ_SetPQModule(pstAttr->enModule, pstAttr->u32OnOff);

            break;
        }
        case HIIOC_PQ_G_MODULE:
        {
            HI_PQ_MODULE_S* pstAttr;
            pstAttr = (HI_PQ_MODULE_S*)arg;

            s32Ret = DRV_PQ_GetPQModule(pstAttr->enModule, &(pstAttr->u32OnOff));
            break;
        }

        case HIIOC_PQ_S_DEMO:
        {
            HI_PQ_DEMO_S* pstAttr;
            pstAttr = (HI_PQ_DEMO_S*)arg;

            s32Ret = DRV_PQ_SetDemoMode(pstAttr->enModule, pstAttr->bOnOff);

            break;
        }


        case HIIOC_PQ_S_SR_DEMO:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret = DRV_PQ_SetSRMode(u32Level);

            break;
        }

        case HIIOC_PQ_G_SR_DEMO:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetSRMode(pu32Level);

            break;
        }

        case HIIOC_PQ_G_BIN_ADDR:
        {
            HI_U32* pu32Addr;

            pu32Addr = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetBinPhyAddr(pu32Addr);

            break;
        }

        case HIIOC_PQ_S_GRAPH_SD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_SetSDPictureSetting(pstAttr);

            break;
        }
        case HIIOC_PQ_G_GRAPH_SD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_GetSDPictureSetting(pstAttr);

            break;
        }

        case HIIOC_PQ_S_GRAPH_HD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_SetHDPictureSetting(pstAttr);

            break;
        }
        case HIIOC_PQ_G_GRAPH_HD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_GetHDPictureSetting(pstAttr);

            break;
        }
        case HIIOC_PQ_S_VIDEO_SD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_SetSDVideoSetting(pstAttr);

            break;
        }
        case HIIOC_PQ_G_VIDEO_SD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_GetSDVideoSetting(pstAttr);

            break;
        }

        case HIIOC_PQ_S_VIDEO_HD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_SetHDVideoSetting(pstAttr);

            break;
        }
        case HIIOC_PQ_G_VIDEO_HD_PARAM:
        {
            HI_PQ_PICTURE_SETTING_S* pstAttr;
            pstAttr = (HI_PQ_PICTURE_SETTING_S*)arg;

            s32Ret = DRV_PQ_GetHDVideoSetting(pstAttr);

            break;
        }

#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)
        case HIIOC_PQ_S_TNR_Y_PIXMEAN_2_RATIO:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrYMotionPixMeanRatio(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_Y_PIXMEAN_2_RATIO:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrYMotionPixMeanRatio(pstAttr);

            break;
        }

        case HIIOC_PQ_S_TNR_C_PIXMEAN_2_RATIO:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrCMotionPixMeanRatio(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_C_PIXMEAN_2_RATIO:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrCMotionPixMeanRatio(pstAttr);

            break;
        }

        case HIIOC_PQ_S_TNR_Y_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrYMotionMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_Y_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrYMotionMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_S_TNR_C_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrCMotionMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_C_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrCMotionMapping(pstAttr);

            break;
        }


        case HIIOC_PQ_S_TNR_Y_FINAL_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrFnlMotionYMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_Y_FINAL_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrFnlMotionYMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_S_TNR_C_FINAL_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;
            pstAttr = (TNR_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrFnlMotionCMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_C_FINAL_MOTION_MAPPING:
        {
            TNR_MAPPING_S* pstAttr;

            pstAttr = (TNR_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrFnlMotionCMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_S_TNR_FMOTION_MAPPING:
        {
            TNR_FMOTION_MAPPING_S* pstAttr;
            pstAttr = (TNR_FMOTION_MAPPING_S*)arg;

            s32Ret = PQ_MNG_SetTnrFmotionMapping(pstAttr);

            break;
        }

        case HIIOC_PQ_G_TNR_FMOTION_MAPPING:
        {
            TNR_FMOTION_MAPPING_S* pstAttr;

            pstAttr = (TNR_FMOTION_MAPPING_S*)arg;
            s32Ret = PQ_MNG_GetTnrFmotionMapping(pstAttr);

            break;
        }

#endif

        case HIIOC_PQ_S_DEMO_MODE:
        {
            HI_U32 u32Level;

            u32Level = *(HI_U32*)arg;
            s32Ret  = DRV_PQ_SetDemoDispMode(u32Level, HI_PQ_MOUDULE_FLAG_VPSS);
            s32Ret |= DRV_PQ_SetDemoDispMode(u32Level, HI_PQ_MOUDULE_FLAG_VDP);

            break;
        }

        case HIIOC_PQ_G_DEMO_MODE:
        {
            HI_U32* pu32Level;

            pu32Level = (HI_U32*)arg;
            s32Ret = DRV_PQ_GetDemoDispMode(pu32Level);

            break;
        }

        case HIIOC_PQ_S_DEFAULT_PARAM:
        {
            HI_BOOL bDefault;

            bDefault = *(HI_BOOL*)arg;
            s32Ret = DRV_PQ_SetDefaultParam(bDefault);

            break;
        }

        default:
        {
            HI_ERR_PQ("No Such IOCTL Command: %d\n", cmd);
            up(&g_stPQSemaphore);
            return -ENOIOCTLCMD;
        }
    }

    up(&g_stPQSemaphore);

    return s32Ret;
}

long PQ_Ioctl(struct file* filp, unsigned int cmd, unsigned long arg)
{
    return (long)HI_DRV_UserCopy(filp->f_dentry->d_inode, filp, cmd, arg, PQIoctl);
}

HI_S32 PQ_DRV_Suspend(PM_BASEDEV_S* pdev, pm_message_t state)
{
    return 0;
}

HI_S32 PQ_DRV_Resume(PM_BASEDEV_S* pdev)
{
    return 0;
}

static struct file_operations s_stPQOps =
{
    .owner          = THIS_MODULE,
    .open           = PQ_Open,
    .release        = PQ_Close,
    .unlocked_ioctl = PQ_Ioctl,
};

static PM_BASEOPS_S  s_stPQDrvOps =
{
    .probe        = NULL,
    .remove       = NULL,
    .shutdown     = NULL,
    .prepare      = NULL,
    .complete     = NULL,
    .suspend      = DRV_PQ_Suspend,
    .suspend_late = NULL,
    .resume_early = NULL,
    .resume       = DRV_PQ_Resume,
};

static UMAP_DEVICE_S s_stPQDev =
{
    .owner      = THIS_MODULE,
    .minor      = UMAP_MIN_MINOR_PQ,
    .fops       = &s_stPQOps,
    .drvops     = &s_stPQDrvOps,
};

HI_S32 PQ_DRV_ModInit(HI_VOID)
{
    HI_S32 s32Ret;
    DRV_PROC_ITEM_S* pstPqProc = NULL;
    HI_CHAR szPath[128] __attribute__((unused)) = "../bin/";
    
    HI_OSAL_Snprintf(s_stPQDev.devfs_name, HIMEDIA_DEVICE_NAME_MAX_LEN, "%s", UMAP_DEVNAME_PQ);
    s32Ret = HI_DRV_DEV_Register(&s_stPQDev);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_DEV("PQ Device Register Fail!\n");
        return s32Ret;
    }

    pstPqProc = HI_DRV_PROC_AddModule("pq", NULL, NULL);
    if (NULL == pstPqProc)
    {
        HI_ERR_DEV("PQ Proc Register Fail!\n");
        HI_DRV_DEV_UnRegister(&s_stPQDev);

        return s32Ret;
    }
    pstPqProc->read  = DRV_PQ_ProcRead;
    pstPqProc->write = DRV_PQ_ProcWrite;
    pstPqProc->ioctl = NULL;

#ifndef HI_MCE_SUPPORT
    s32Ret = HI_DRV_PQ_Init(szPath);
    if (s32Ret != HI_SUCCESS)
    {
        HI_DRV_DEV_UnRegister(&s_stPQDev);
        HI_DRV_PROC_RemoveModule("pq");
        HI_ERR_DEV("PQ Hal Init Fail!\n");

        return s32Ret;
    }
#endif

#ifdef MODULE
    HI_PRINT("Load hi_pq.ko success.\t\t(%s)\n", VERSION_STRING);
#endif

    return HI_SUCCESS;
}

HI_VOID PQ_DRV_ModuleExit(HI_VOID)
{
#ifndef HI_MCE_SUPPORT
    HI_DRV_PQ_DeInit();
#endif
    HI_DRV_PROC_RemoveModule("pq");
    HI_DRV_DEV_UnRegister(&s_stPQDev);
#ifdef MODULE
    HI_PRINT("remove hi_pq.ko success.     \t(%s)\n", VERSION_STRING);
#endif
}

MODULE_LICENSE("GPL");
#ifdef MODULE
module_init(PQ_DRV_ModInit);
module_exit(PQ_DRV_ModuleExit);
#endif
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

