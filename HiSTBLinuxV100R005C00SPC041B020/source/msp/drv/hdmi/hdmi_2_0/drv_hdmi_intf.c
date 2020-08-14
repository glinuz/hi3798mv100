#include <linux/device.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm/delay.h>
#include <asm/thread_info.h>
#include <linux/poll.h>
//#include <mach/hardware.h>
#include <linux/interrupt.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/kthread.h>
#include <linux/timer.h>
#include <linux/delay.h>
#include "hdmi_hal.h"
#include "hdmi_platform.h"
#include "drv_hdmi_edid.h"
#include "drv_hdmi_event.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_platform.h"
#include "drv_hdmi_intf.h"
#include "drv_hdmi_ioctl.h"
#include "drv_hdmi_proc.h"
#include "drv_hdmi_debug.h"
#include "drv_hdmi_compatibility.h"
#include "drv_hdmi_timming.h"
#ifdef HDMI_ANDROID_SUPPORT
#include <linux/switch.h>

static HI_BOOL            s_bIsSwitchOn = HI_FALSE;
static struct switch_dev  s_stSwitchDev =
{
    .name = "hdmi",
};
#endif

//static struct semaphore s_HdmiMutex;
HI_HDMI_DECLARE_MUTEX(s_HdmiMutex);

#define HDMI_MULTIPLE_2P0(x)         (x *= 2)
#define HDMI_MULTIPLE_1P5(x)         (x =(x * 3) >> 1)
#define HDMI_MULTIPLE_1P25(x)        (x =(x * 5) >> 2)
#define HDMI_MULTIPLE_0P5(x)         (x =(x) >> 1)

#define CHECK_POINTER(p) \
do{                      \
    if(HI_NULL == p){\
        HDMI_ERR("The pointer is null\n");       \
        return HI_FAILURE;\
    } \
}while(0)

#define CHECK_POINTER_NO_RET(p) \
do{                      \
    if(HI_NULL == p){\
        HDMI_ERR("The pointer is null\n");       \
        return; \
    } \
}while(0)

#define CHECK_HDMI_OPEN(state) \
do \
{   \
    if (!(state & HDMI_RUN_STATE_OPEN)) \
    {   \
        HDMI_WARN("The hdmi device not open\n");\
        return HI_FAILURE;\
    } \
}while(0)

#define BITDEPTH_2_DEEPCOLOR(BitDepth, DeepColor) \
do \
{   \
    if (HDMI_VIDEO_BITDEPTH_8 == BitDepth)\
    {\
        DeepColor = HDMI_DEEP_COLOR_24BIT;\
    }\
    else if (HDMI_VIDEO_BITDEPTH_10 == BitDepth)\
    {\
        DeepColor = HDMI_DEEP_COLOR_30BIT;\
    }\
    else if (HDMI_VIDEO_BITDEPTH_12 == BitDepth)\
    {\
        DeepColor = HDMI_DEEP_COLOR_36BIT;\
    }\
    else if (HDMI_VIDEO_BITDEPTH_16 == BitDepth)\
    {\
        DeepColor = HDMI_DEEP_COLOR_48BIT;\
    }\
    else\
    {\
        DeepColor = HDMI_DEEP_COLOR_BUTT;\
    }\
}while(0)

#define DEEPCOLOR_2_BITDEPTH(BitDepth, DeepColor) \
do \
{   \
    if (HDMI_DEEP_COLOR_24BIT == DeepColor)\
    {\
        BitDepth = HDMI_VIDEO_BITDEPTH_8;\
    }\
    else if (HDMI_DEEP_COLOR_30BIT == DeepColor)\
    {\
        BitDepth = HDMI_VIDEO_BITDEPTH_10;\
    }\
    else if (HDMI_DEEP_COLOR_36BIT == DeepColor)\
    {\
        BitDepth = HDMI_VIDEO_BITDEPTH_12;\
    }\
    else if (HDMI_DEEP_COLOR_48BIT == DeepColor)\
    {\
        BitDepth = HDMI_VIDEO_BITDEPTH_16;\
    }\
    else\
    {\
        BitDepth = HDMI_VIDEO_BITDEPTH_BUTT;\
    }\
}while(0)

#define HDRCOLORIMETRY_2_COLORIMETRY(HdrColorimetry, Colorimetry) \
do \
{   \
    if (HDMI_HDR_COLORIMETRY_NONE == HdrColorimetry)\
    {\
        Colorimetry = HDMI_COLORIMETRY__NONE;\
    }\
    else if (HDMI_HDR_COLORIMETRY_ITU_601 == HdrColorimetry)\
    {\
        Colorimetry = HDMI_COLORIMETRY__ITU_601;\
    }\
    else if (HDMI_HDR_COLORIMETRY_ITU_709 == HdrColorimetry)\
    {\
        Colorimetry = HDMI_COLORIMETRY__ITU_709;\
    }\
    else if (HDMI_HDR_COLORIMETRY_EXTENDED == HdrColorimetry)\
    {\
        Colorimetry = HDMI_COLORIMETRY__EXTENDED;\
    }\
}while(0)

extern HI_S32 HI_DRV_HDMI_Init(HI_VOID);
extern HI_VOID HI_DRV_HDMI_Deinit(HI_VOID);
extern HI_S32 HI_DRV_HDMI_KernelEventCallback(HI_VOID* data, HDMI_EVENT_E enEvent);

static atomic_t  s_HdmiCount;
static HI_S32 HdmiRelease(HDMI_DEVICE_S* pstHdmiDev);
static HI_S32 HdmiFileOpen(struct inode *inode, struct file *filp);
static HI_S32 HdmiFileClose(struct inode *inode, struct file *filp);
static long  HdmiFileIoctl(struct file *file,unsigned int cmd, unsigned long arg);
static HI_VOID HdmiSetAndroidState(HDMI_SWITCH_STATE enState);
static HI_S32 HdmiVideoPathSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr);
static HI_S32 HdmiEventCallback(HI_VOID* pData , HDMI_EVENT_E enEvent);
static HI_BOOL VOAttrIsChanged(HDMI_VO_ATTR_S* pstHwVOAttr, HDMI_VO_ATTR_S* pstSwVOAttr);
static HI_BOOL APPAttrIsChanged(HDMI_APP_ATTR_S* pstHwAPPAttr, HDMI_APP_ATTR_S* pstSwAPPAttr);
HI_VOID DRV_HDMI_BlackDataSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable);

#ifdef HDMI_HDR_SUPPORT
static HI_VOID HdmiZeroDrmIfTimerSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_TIMER_CONFIG_S* pstTimer);
static HI_VOID HdmiZeroDrmIfStop(HDMI_DEVICE_S* pstHdmiDev);
static HI_VOID HdmiHdrModeChangeTimerSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_TIMER_CONFIG_S* pstTimer);
static HI_VOID HdmiZeroDrmIfStop(HDMI_DEVICE_S* pstHdmiDev);
static HI_S32 HdmiHdrModeChangeTimeout(HDMI_DEVICE_S* pstHdmiDev);
#endif

DECLARE_HDMI_NODE("HDMI", HdmiFileOpen, HdmiFileClose, HI_NULL, HdmiFileIoctl, HI_NULL, HI_NULL);

static HDMI_DEVICE_S s_stHdmiCtrl[HDMI_DEVICE_ID_BUTT];

HDMI_DEVICE_S* GetHdmiDevice(HDMI_DEVICE_ID_E enHdmiID)
{
    if (enHdmiID < HDMI_DEVICE_ID_BUTT)
    {
        s_stHdmiCtrl[enHdmiID].u32HdmiDevId = enHdmiID;
        return &s_stHdmiCtrl[enHdmiID];
    }
    return HI_NULL;
}

static HI_VOID HdmiVoCapabilityIntersection(DRV_HDMI_VIDEO_CAPABILITY_S *pstDrvVideoCap, HDMI_SINK_CAPABILITY_S *pstSinkCap)
{
    CHECK_POINTER_NO_RET(pstDrvVideoCap);
    CHECK_POINTER_NO_RET(pstSinkCap);

	memcpy(&(pstDrvVideoCap->stVideoCap.stColorMetry), &(pstSinkCap->stColorMetry), sizeof(HDMI_EDID_COLORIMETRY_S));

#ifdef HDMI_HDR_SUPPORT
    memcpy(&(pstDrvVideoCap->stVideoCap.stHdrCaps), &(pstSinkCap->stHdrCap), sizeof(HDMI_EDID_HDR_CAP_S));
    memcpy(&(pstDrvVideoCap->stVideoCap.stDolbyCaps), &(pstSinkCap->stDolbyCap), sizeof(HDMI_EDID_DOLBY_CAP_S));

    if(   (pstSinkCap->stHdrCap.stEotf.bEotfSdr == HI_TRUE)
       || (pstSinkCap->stHdrCap.stEotf.bEotfHdr == HI_TRUE)
       || (pstSinkCap->stHdrCap.stEotf.bEotfSmpteSt2084 == HI_TRUE))
    {
        pstDrvVideoCap->stVideoCap.bHdrSupport = HI_TRUE;
    }
    else
    {
        pstDrvVideoCap->stVideoCap.bHdrSupport = HI_FALSE;
    }

    pstDrvVideoCap->stVideoCap.bDolbySupport = (pstSinkCap->stDolbyCap.u32DolbyOui == DOLBY_IEEE_OUI) ? HI_TRUE : HI_FALSE;
#endif
}

static HI_S32 HdmiCapabilityIntersection (HDMI_SINK_CAPABILITY_S *psDestCap, HDMI_TX_CAPABILITY_S *pstTxCap, HI_BOOL bAuthMode)
{
    CHECK_POINTER(psDestCap);
    CHECK_POINTER(pstTxCap);

    psDestCap->bSupportHdmi     &= pstTxCap->bTxHdmi_14;
    psDestCap->bSupportHdmi_2_0 &= pstTxCap->bTxHdmi_20;
    psDestCap->bSupportSCDC     &= pstTxCap->bTxScdc;

    psDestCap->stColorSpace.bRGB444   &= pstTxCap->bTxRGB444;
    psDestCap->stColorSpace.bYCbCr444 &= pstTxCap->bTxYCBCR444;
    psDestCap->stColorSpace.bYCbCr422 &= pstTxCap->bTxYCBCR422;
    psDestCap->stColorSpace.bYCbCr420 = (psDestCap->stColorSpace.bYCbCr420 & pstTxCap->bTxYCBCR420) | bAuthMode;

    psDestCap->stDeepColor.bDeepColor30Bit &= pstTxCap->bTxDeepClr10Bit;
    psDestCap->stDeepColor.bDeepColor36Bit &= pstTxCap->bTxDeepClr12Bit;
    psDestCap->stDeepColor.bDeepColor48Bit &= pstTxCap->bTxDeepClr16Bit;

    psDestCap->stDeepColorY420.bDeepColor30Bit = (psDestCap->stDeepColorY420.bDeepColor30Bit & (pstTxCap->bTxYCBCR420 & pstTxCap->bTxDeepClr10Bit)) | bAuthMode;
    psDestCap->stDeepColorY420.bDeepColor36Bit = (psDestCap->stDeepColorY420.bDeepColor36Bit & (pstTxCap->bTxYCBCR420 & pstTxCap->bTxDeepClr12Bit)) | bAuthMode;
    psDestCap->stDeepColorY420.bDeepColor48Bit = (psDestCap->stDeepColorY420.bDeepColor48Bit & (pstTxCap->bTxYCBCR420 & pstTxCap->bTxDeepClr16Bit)) | bAuthMode;

    psDestCap->u32MaxTMDSClock = psDestCap->u32MaxTMDSClock < pstTxCap->u32TxMaxTmdsClk ?
                                           psDestCap->u32MaxTMDSClock : pstTxCap->u32TxMaxTmdsClk;

    psDestCap->stHdcpSupport.bHdcp14Support &= pstTxCap->bTxHdcp_14;
    psDestCap->stHdcpSupport.bHdcp22Support &= pstTxCap->bTxHdcp_22;

    return HI_SUCCESS;
}

HI_VOID HdmiPhyOutputEnable(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bPhyOutputEnable)
{
    HI_BOOL     bPhyOutputStatus = HI_FALSE;
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstHdmiDev->pstHdmiHal);

    if(bPhyOutputEnable)
    {
        pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableGet(pstHdmiDev->pstHdmiHal, &bPhyOutputStatus);
        if(HI_FALSE == bPhyOutputStatus)
        {
            pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
            pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_TRUE);
        }
    }
    else
    {
        pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    }

    return;
}
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
/* nothing */
#else
#ifdef HDMI_SCDC_SUPPORT
static HI_S32 HdmiScrambleEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HDMI_SCDC_CONFIG_S      stScdcConfig;
    HDMI_SCDC_STATUS_S      stScdcStatus;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    memset(&stScdcStatus, 0, sizeof(HDMI_SCDC_STATUS_S));
    pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcStatusGet(pstHdmiDev->pstHdmiHal, &stScdcStatus);
    //pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoMuteSet(pstHdmiDev->pstHdmiHal,HI_TRUE);

    if ((stScdcStatus.bSourceScrambleOn &&
        stScdcStatus.bSinkScrambleOn &&
        stScdcStatus.u8TmdsBitClkRatio == 40 && bEnable) ||
        (!stScdcStatus.bSourceScrambleOn &&
        !stScdcStatus.bSinkScrambleOn &&
        stScdcStatus.u8TmdsBitClkRatio == 10 && !bEnable))
    {
        //pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoMuteSet(pstHdmiDev->pstHdmiHal,HI_FALSE);
        HDMI_INFO("Scramble status is not change, so no need to config scramble\n");
        return HI_SUCCESS;
    }

    HDMI_INFO("SourceScrambleOn:%d, SinkScrambleOn:%d, TmdsBitClkRatio:%d\n",stScdcStatus.bSourceScrambleOn,stScdcStatus.bSinkScrambleOn,stScdcStatus.u8TmdsBitClkRatio);
    memset(&stScdcConfig, 0, sizeof(HDMI_SCDC_CONFIG_S));
    stScdcConfig.bScdcEnable = bEnable;

    if (bEnable)
    {
        stScdcConfig.u32MaxTmdsCharacterRate = 600 * 1000000;
    }
    else
    {
        stScdcConfig.u32MaxTmdsCharacterRate = 300 * 1000000;
    }
#if 0
    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
        stScdcConfig.bScdcPresent            = HI_TRUE;
        stScdcConfig.bRRCapable              = HI_FALSE;
        stScdcConfig.bLTE340McscScramble     = HI_FALSE;
        stScdcConfig.u32MaxTmdsCharacterRate = 600 * 1000000;
    }
    else
    {
        stScdcConfig.bScdcPresent            = pstSinkCap->bSupportSCDC;
        stScdcConfig.bRRCapable              = pstSinkCap->bSupportRRCapable;
        stScdcConfig.bLTE340McscScramble     = pstSinkCap->bSupportLte340McscScrameble;
        stScdcConfig.u32MaxTmdsCharacterRate = pstSinkCap->u32MaxTMDSClock * 1000000;
    }
#else
    stScdcConfig.bScdcPresent            = HI_TRUE;
    stScdcConfig.bRRCapable              = HI_FALSE;
    stScdcConfig.bLTE340McscScramble     = HI_FALSE;

#endif
    HdmiPhyOutputEnable(pstHdmiDev, HI_FALSE);

    HDMI_INFO("ScdcEnable:%d\n",stScdcConfig.bScdcEnable);
    HDMI_INFO("Scdc Config:ScdcPresent(%d),RRCapable(%d),LTE340McscScramble(%d),MaxTmdsCharacterRate(%d)\n",stScdcConfig.bScdcPresent,stScdcConfig.bRRCapable,stScdcConfig.bLTE340McscScramble,stScdcConfig.u32MaxTmdsCharacterRate);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoMuteSet(pstHdmiDev->pstHdmiHal,HI_TRUE);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcConfig(pstHdmiDev->pstHdmiHal, &stScdcConfig);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoMuteSet(pstHdmiDev->pstHdmiHal, HI_FALSE);

    return HI_SUCCESS;
}
#endif
#endif
static HI_BOOL HdmiYCbCr420FmtCheck(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_U32                  i = 0;
    HI_BOOL                 bSearchOut = HI_FALSE;
    HDMI_VIDEO_CODE_E       enVic = 0;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap = HI_NULL;
    HDMI_VO_ATTR_S          *pstVoAttr = HI_NULL;

    CHECK_POINTER(pstHdmiDev);

    /* Auth mode donot do CbCr420FmtCheck and return HI_TRUE*/
    if (HI_TRUE == pstHdmiDev->stAttr.stAppAttr.bAuthMode)
    {
        HDMI_INFO("bAuthMode: %d \n", pstHdmiDev->stAttr.stAppAttr.bAuthMode);
        return HI_TRUE;
    }

    pstVoAttr  = &pstHdmiDev->stAttr.stVOAttr;

    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
    }

    enVic = DRV_HDMI_VicSearch(pstVoAttr->enVideoTiming,pstVoAttr->enPictureAspect, HI_FALSE);

    if (enVic != 0)
    {
        for (i = 0;i< pstSinkCap->u32SupportY420VICNum && i<HDMI_ARRAY_SIZE(pstSinkCap->au32SupportY420Format);i++)
        {
            if (enVic == pstSinkCap->au32SupportY420Format[i])
            {
                bSearchOut = HI_TRUE;
                break;
            }
        }
        for (i = 0;(!bSearchOut) && i<pstSinkCap->u32OnlySupportY420VICNum && i<HDMI_ARRAY_SIZE(pstSinkCap->au32OnlySupportY420Format);i++)
        {
            if (enVic == pstSinkCap->au32OnlySupportY420Format[i])
            {
                bSearchOut = HI_TRUE;
                break;
            }
        }
    }

    HDMI_INFO("Is Y420 support enVic=%d :%s\n",enVic,bSearchOut ? "YES" : "NO");

    return bSearchOut;

}

static HI_S32 HdmiColorSpaceCheck(HDMI_DEVICE_S* pstHdmiDev,HDMI_APP_ATTR_S *pstUserApp)
{
    HI_BOOL                 bSupportClrSpace = HI_FALSE;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap = HI_NULL;
    HDMI_TX_CAPABILITY_S    stTxCap = {0};

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstUserApp);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);
    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
    }

    switch (pstUserApp->enOutColorSpace)
    {
        case HDMI_COLORSPACE_RGB:
            bSupportClrSpace = pstSinkCap->stColorSpace.bRGB444 && stTxCap.bTxRGB444;
            if (!bSupportClrSpace)
            {
                HDMI_ERR("sink or source not support RGB!\n");
            }
            break;

        case HDMI_COLORSPACE_YCbCr422:
            bSupportClrSpace = pstSinkCap->stColorSpace.bYCbCr422 && stTxCap.bTxYCBCR422;
            break;

        case HDMI_COLORSPACE_YCbCr444:
            bSupportClrSpace = pstSinkCap->stColorSpace.bYCbCr444 && stTxCap.bTxYCBCR444;
            break;
        case HDMI_COLORSPACE_YCbCr420:
            bSupportClrSpace = stTxCap.bTxYCBCR420 && HdmiYCbCr420FmtCheck(pstHdmiDev);
            break;

        default:
            HDMI_ERR("un-know ColorSpace=%d!\n", pstUserApp->enOutColorSpace);
            return HI_FAILURE;
            break;
    }

    if (!bSupportClrSpace)
    {
        if (pstUserApp->bEnableClrSpaceAdapt)
        {
            HDMI_WARN("ColorSpace check fail,clr(%d) adapt to RGB!\n", pstUserApp->enOutColorSpace);
            pstUserApp->enOutColorSpace = HDMI_COLORSPACE_RGB;
        }
        else
        {
            HDMI_ERR("ColorSpace check fail,adapt clrspace fail!\n");
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;

}

HI_VOID CheckDeepColor10bit(HDMI_SINK_CAPABILITY_S *pstSinkCap, HDMI_APP_ATTR_S *pstUserApp,
                            HDMI_TX_CAPABILITY_S *pstTxCap, HI_U32 *pu32FmtPixClk, HI_BOOL *pbSupportDeepClr)
{
    HI_U32 u32PixClk = *pu32FmtPixClk;

    switch (pstUserApp->enOutColorSpace)
    {
        case HDMI_COLORSPACE_RGB:
            HDMI_MULTIPLE_1P25(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor30Bit && pstTxCap->bTxDeepClr10Bit) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr444:
            HDMI_MULTIPLE_1P25(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor30Bit && pstTxCap->bTxDeepClr10Bit
                            && pstSinkCap->stDeepColor.bDeepColorY444) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr420:
            HDMI_MULTIPLE_1P25(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColorY420.bDeepColor30Bit && pstTxCap->bTxDeepClr10Bit) ? HI_TRUE : HI_FALSE;
            break;
        default:
            /* Y422 , ignore deepclr */
            *pbSupportDeepClr = HI_TRUE;
            break;
    }

    *pu32FmtPixClk = u32PixClk;

    return;
}

HI_VOID CheckDeepColor12bit(HDMI_SINK_CAPABILITY_S *pstSinkCap, HDMI_APP_ATTR_S *pstUserApp,
                            HDMI_TX_CAPABILITY_S *pstTxCap, HI_U32 *pu32FmtPixClk, HI_BOOL *pbSupportDeepClr)
{
    HI_U32 u32PixClk = 0;

    CHECK_POINTER_NO_RET(pstSinkCap);
    CHECK_POINTER_NO_RET(pstUserApp);
    CHECK_POINTER_NO_RET(pstTxCap);
    CHECK_POINTER_NO_RET(pu32FmtPixClk);
    CHECK_POINTER_NO_RET(pbSupportDeepClr);

    u32PixClk = *pu32FmtPixClk;

    switch (pstUserApp->enOutColorSpace)
    {
        case HDMI_COLORSPACE_RGB:
            HDMI_MULTIPLE_1P5(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor36Bit && pstTxCap->bTxDeepClr12Bit) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr444:
            HDMI_MULTIPLE_1P5(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor36Bit && pstTxCap->bTxDeepClr12Bit
                            && pstSinkCap->stDeepColor.bDeepColorY444) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr420:
            HDMI_MULTIPLE_1P5(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColorY420.bDeepColor36Bit && pstTxCap->bTxDeepClr12Bit) ? HI_TRUE : HI_FALSE;
            break;
        default:
            /* Y422 ,ignore deepclr */
            *pbSupportDeepClr = HI_TRUE;
            break;
    }

    *pu32FmtPixClk = u32PixClk;

    return;
}

HI_VOID CheckDeepColor16bit(HDMI_SINK_CAPABILITY_S *pstSinkCap, HDMI_APP_ATTR_S *pstUserApp,
                            HDMI_TX_CAPABILITY_S *pstTxCap, HI_U32 *pu32FmtPixClk, HI_BOOL *pbSupportDeepClr)
{
    HI_U32 u32PixClk = 0;

    CHECK_POINTER_NO_RET(pstSinkCap);
    CHECK_POINTER_NO_RET(pstUserApp);
    CHECK_POINTER_NO_RET(pstTxCap);
    CHECK_POINTER_NO_RET(pu32FmtPixClk);
    CHECK_POINTER_NO_RET(pbSupportDeepClr);

    u32PixClk = *pu32FmtPixClk;

    switch (pstUserApp->enOutColorSpace)
    {
        case HDMI_COLORSPACE_RGB:
            HDMI_MULTIPLE_2P0(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor48Bit&& pstTxCap->bTxDeepClr16Bit) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr444:
            HDMI_MULTIPLE_2P0(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColor.bDeepColor48Bit && pstTxCap->bTxDeepClr16Bit
                            && pstSinkCap->stDeepColor.bDeepColorY444) ? HI_TRUE : HI_FALSE;
            break;
        case HDMI_COLORSPACE_YCbCr420:
            HDMI_MULTIPLE_2P0(u32PixClk);
            *pbSupportDeepClr = (pstSinkCap->stDeepColorY420.bDeepColor48Bit && pstTxCap->bTxDeepClr16Bit) ? HI_TRUE : HI_FALSE;
            break;
        default:
            /* Y422 ,ignore deepclr */
            *pbSupportDeepClr = HI_TRUE;
            break;
    }

    *pu32FmtPixClk = u32PixClk;

    return;
}

static HI_S32 HdmiDeepColorCheck(HDMI_DEVICE_S* pstHdmiDev, HDMI_APP_ATTR_S *pstUserApp, HI_U32 u32MaxTmdsClk)
{
    HI_U32                  u32FmtPixClk        = 0;
    HI_BOOL                 bSupportDeepClr     = HI_FALSE;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap         = HI_NULL;
    HDMI_VO_ATTR_S          *pstVoAttr          = HI_NULL;
    HDMI_TX_CAPABILITY_S    stTxCap             = {0};
    HI_BOOL                 bAdaptToY422        = HI_FALSE;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstUserApp);

    pstVoAttr  = &pstHdmiDev->stAttr.stVOAttr;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);
    if (HDMI_EDID_DATA_INVALID == DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
    {
        HDMI_WARN("Get sink capability fail\n");
    }

    u32FmtPixClk = pstVoAttr->u32ClkFs;

    if(HDMI_COLORSPACE_YCbCr420 == pstUserApp->enOutColorSpace)
    {
        HDMI_MULTIPLE_0P5(u32FmtPixClk);
    }

    switch(pstUserApp->enDeepColorMode)
    {
        case HDMI_DEEP_COLOR_30BIT:
                CheckDeepColor10bit(pstSinkCap, pstUserApp, &stTxCap, &u32FmtPixClk, &bSupportDeepClr);
                break;

        case HDMI_DEEP_COLOR_36BIT:
                CheckDeepColor12bit(pstSinkCap, pstUserApp, &stTxCap, &u32FmtPixClk, &bSupportDeepClr);
                break;

        case HDMI_DEEP_COLOR_48BIT:
                CheckDeepColor16bit(pstSinkCap, pstUserApp, &stTxCap, &u32FmtPixClk, &bSupportDeepClr);
                break;

        case HDMI_DEEP_COLOR_24BIT:
        case HDMI_DEEP_COLOR_OFF:
        default:
#ifndef HDMI_SUPPORT_LOGIC_HISIV100
                pstUserApp->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
#endif
                bSupportDeepClr = HI_TRUE;
                break;
    }

    if (bSupportDeepClr && (u32FmtPixClk < u32MaxTmdsClk))
    {
        pstVoAttr->u32HdmiAdaptPixClk = u32FmtPixClk;
        return HI_SUCCESS;
    }
    else
    {
        pstVoAttr->u32HdmiAdaptPixClk = pstVoAttr->u32ClkFs;
        if(HDMI_COLORSPACE_YCbCr420 == pstUserApp->enOutColorSpace)
        {
            HDMI_MULTIPLE_0P5(pstVoAttr->u32HdmiAdaptPixClk);
        }

        bAdaptToY422 = (  (pstUserApp->enOutColorSpace == HDMI_COLORSPACE_YCbCr444)
                        && (pstUserApp->bEnableClrSpaceAdapt)
                        && (pstUserApp->enDeepColorMode != HDMI_DEEP_COLOR_24BIT)
                        && (pstUserApp->enDeepColorMode != HDMI_DEEP_COLOR_OFF)
                        && (pstUserApp->enDeepColorMode != HDMI_DEEP_COLOR_48BIT)
                        && (pstSinkCap->stColorSpace.bYCbCr422 && stTxCap.bTxYCBCR422)
                       );

        /* Y444 12/10bit */
        if (bAdaptToY422)
        {
            HDMI_WARN("DeepClr check fail,Y444 adapt to Y422!\n");
            pstUserApp->enOutColorSpace = HDMI_COLORSPACE_YCbCr422;
        }
        /* Y444 16bit/ RGB / Y420 */
        else if (pstUserApp->bEnableDeepClrAdapt)
        {
            HDMI_WARN("DeepClr check fail,deepclr(%d) adapt to 8bit!\n",pstUserApp->enDeepColorMode);
            pstUserApp->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
        }
        else
        {
            HDMI_ERR("DeepClr check fail, adapt deepclr fail!\n");
            return HI_FAILURE;
        }

        return HI_SUCCESS;
    }
}


static HI_S32 AdaptTo420(HDMI_VO_ATTR_S *pstVoAttr, HDMI_APP_ATTR_S *pstUserApp, HI_U32 u32MaxTmdsClk)
{
    CHECK_POINTER(pstVoAttr);
    CHECK_POINTER(pstUserApp);

    if (pstVoAttr->u32ClkFs > u32MaxTmdsClk)
    {
        HDMI_WARN("PixClk(%u) > MaxTmdsClk(%u)\n",pstVoAttr->u32ClkFs ,u32MaxTmdsClk);
		// add pstUserApp->enOutColorSpace to avoid set yuv420 failed after the timing set success
        if (pstUserApp->enOutColorSpace != HDMI_COLORSPACE_YCbCr420)
        {
            if (pstUserApp->bEnableClrSpaceAdapt)
            {
                HDMI_WARN("Clr(%d) adapt to Y420 or user clr is Y420!\n",pstUserApp->enOutColorSpace);
                pstUserApp->enOutColorSpace = HDMI_COLORSPACE_YCbCr420;
            }
            else
            {
                HDMI_ERR("adapt Y420 fail!\n");
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

HI_VOID EdidInvalidOrAuthMode(HDMI_DEVICE_S* pstHdmiDev, HDMI_APP_ATTR_S *pstUserApp)
{
    HDMI_VO_ATTR_S          *pstVoAttr = HI_NULL;

    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstUserApp);

    pstVoAttr  = &pstHdmiDev->stAttr.stVOAttr;

    pstVoAttr->u32HdmiAdaptPixClk = (HDMI_COLORSPACE_YCbCr420 == pstUserApp->enOutColorSpace) ?
                                        (pstVoAttr->u32ClkFs >> 1) : pstVoAttr->u32ClkFs;

    if (HDMI_COLORSPACE_YCbCr422 != pstUserApp->enOutColorSpace)
        {
            switch(pstUserApp->enDeepColorMode)
            {
                case HDMI_DEEP_COLOR_48BIT:
                    HDMI_MULTIPLE_2P0(pstVoAttr->u32HdmiAdaptPixClk);
                    break;
                case HDMI_DEEP_COLOR_36BIT:
                    HDMI_MULTIPLE_1P5(pstVoAttr->u32HdmiAdaptPixClk);
                    break;
                case HDMI_DEEP_COLOR_30BIT:
                    HDMI_MULTIPLE_1P25(pstVoAttr->u32HdmiAdaptPixClk);
                    break;
                default:
                    break;
            }
        }
        else
        {
            HDMI_INFO("Y422 foce deepcolor 8bit");
            pstUserApp->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
            if(   pstHdmiDev->stAttr.stVOAttr.enVideoTiming == HDMI_VIDEO_TIMING_1440X480I_60000
               || pstHdmiDev->stAttr.stVOAttr.enVideoTiming == HDMI_VIDEO_TIMING_1440X576I_50000)
            {
                HDMI_WARN("Y422 is not support at pal and ntsc, force adapt to rgb!\n");
                pstUserApp->enOutColorSpace = HDMI_COLORSPACE_RGB;
            }
        }

    return;
}

static HI_S32 HdmiColorAndBitStrategy(HDMI_DEVICE_S* pstHdmiDev, HDMI_APP_ATTR_S *pstUserApp)
{
    HI_S32                  s32Ret          = HI_SUCCESS;
    HI_U32                  u32MaxTmdsClk   = 0;
    HDMI_VO_ATTR_S          *pstVoAttr      = HI_NULL;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap     = HI_NULL;
    HDMI_TX_CAPABILITY_S    stTxCap         = {0};
    HDMI_EDID_DATA_E        eRet            = HDMI_EDID_DATA_BUTT;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstUserApp);

    pstVoAttr  = &pstHdmiDev->stAttr.stVOAttr;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);
    eRet = DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap);
    if (HDMI_EDID_DATA_INVALID == eRet || (HI_TRUE == pstHdmiDev->stAttr.stAppAttr.bAuthMode))
    {
        EdidInvalidOrAuthMode(pstHdmiDev, pstUserApp);
        /* Get EdidCapability fail means cannot do strategy, according to Appatrr */
        HDMI_WARN("Get sink capability fail or bAuthMode=%d,adapt u32HdmiAdaptPixClk=%u\n", pstHdmiDev->stAttr.stAppAttr.bAuthMode, pstVoAttr->u32HdmiAdaptPixClk);
        return HI_SUCCESS;
    }
    else
    {
        /* get MaxTmdsClk,in KHz. */
        if(pstSinkCap->u32MaxTMDSClock == 0)
        {
            /* But when the Sink's MaxTMDSClock is 0MHz, app can not set attr success.
               Then set u32MaxTmdsClk = 300M default. */
            u32MaxTmdsClk = 300;
        }
        else
        {
            u32MaxTmdsClk = pstSinkCap->u32MaxTMDSClock < stTxCap.u32TxMaxTmdsClk ? pstSinkCap->u32MaxTMDSClock : stTxCap.u32TxMaxTmdsClk;
        }

        u32MaxTmdsClk *= 1000;
    }

    /*  whether adapt to YCbCr420  */
    s32Ret = AdaptTo420(pstVoAttr, pstUserApp, u32MaxTmdsClk);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* DVI mode ,must set RGB & DeepColorOff */
    if( !pstUserApp->bEnableHdmi )
    {
        HDMI_INFO("DVI mode, force DeepColor %d -> OFF, ColorSpace %d -> RGB!\n", pstUserApp->enDeepColorMode, pstUserApp->enOutColorSpace);
        pstUserApp->enOutColorSpace = HDMI_COLORSPACE_RGB;
        pstUserApp->enDeepColorMode = HDMI_DEEP_COLOR_OFF;
    }

    /* ColorSpace check & adapt */
    s32Ret = HdmiColorSpaceCheck(pstHdmiDev, pstUserApp);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* DeepColor check & adapt */
    s32Ret = HdmiDeepColorCheck(pstHdmiDev, pstUserApp, u32MaxTmdsClk);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    /* Y422 default 12bit output, DeepColor force adapt to off(24bit). */
    if (pstUserApp->enOutColorSpace == HDMI_COLORSPACE_YCbCr422)
    {
        if(   pstHdmiDev->stAttr.stVOAttr.enVideoTiming == HDMI_VIDEO_TIMING_1440X480I_60000
           || pstHdmiDev->stAttr.stVOAttr.enVideoTiming == HDMI_VIDEO_TIMING_1440X576I_50000)
        {
            HDMI_WARN("Y422 is not support at pal and ntsc!\n");
            if(HI_TRUE != pstUserApp->bEnableClrSpaceAdapt)
            {
                return HI_FAILURE;
            }
            else
            {
                HDMI_WARN("force change colorspace to RGB!\n");
                pstUserApp->enOutColorSpace = HDMI_COLORSPACE_RGB;
            }
        }
    	HDMI_WARN("when Y422, DeepColor force adapt to OFF!\n");
        pstUserApp->enDeepColorMode = HDMI_DEEP_COLOR_OFF;
    }

    HDMI_INFO("bClrSpaceAdapt=%d,bDeepClrAdapt=%d,adptclrspace=%d,adptbit=%d,adptclk=%u,maxClk=%u\n",
                                                pstUserApp->bEnableClrSpaceAdapt,
                                                pstUserApp->bEnableDeepClrAdapt,
                                                pstUserApp->enOutColorSpace,
                                                pstUserApp->enDeepColorMode,
                                                pstVoAttr->u32HdmiAdaptPixClk,
                                                u32MaxTmdsClk);

    return HI_SUCCESS;
}

#ifdef HDMI_CEC_SUPPORT
static HI_S32 HdmiCecNetWorkSet(HDMI_DEVICE_S* pstHdmiDev)
{

    HI_S32                  s32Ret = HI_FAILURE;
    HDMI_EDID_DATA_E        enEdidRet;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap = HI_NULL;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    enEdidRet = DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap);
    if ((enEdidRet != HDMI_EDID_DATA_INVALID) && (pstSinkCap->stCECAddr.bPhyAddrValid))
    {
        pstHdmiDev->stCecInfo.au8PhysicalAddr[0] = pstSinkCap->stCECAddr.u8PhyAddrA;
        pstHdmiDev->stCecInfo.au8PhysicalAddr[1] = pstSinkCap->stCECAddr.u8PhyAddrB;
        pstHdmiDev->stCecInfo.au8PhysicalAddr[2] = pstSinkCap->stCECAddr.u8PhyAddrC;
        pstHdmiDev->stCecInfo.au8PhysicalAddr[3] = pstSinkCap->stCECAddr.u8PhyAddrD;
    }
    else
    {
        HDMI_WARN("EDID's cec phy address fail\n");
    }

    if( DRV_HDMI_CompatCecValidCheck(pstHdmiDev) )
    {
        s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_CecNetPing(pstHdmiDev->pstHdmiHal, pstHdmiDev->stCecInfo.au8PhysicalAddr);
    }
    return s32Ret;
}
#endif


#if defined HDMI_SUPPORT_LOGIC_HISIV100
static HI_S32 HdmiModeStrategy(HDMI_DEVICE_S* pstHdmiDev)
{

    HDMI_SCDC_STATUS_S      stScdcStatus;
    HDMI_SCDC_STATUS_S      stScdcGet;
    HDMI_EDID_DATA_E        enEdidRet;
    HDMI_TX_CAPABILITY_S    stTxCap;
    HDMI_SINK_CAPABILITY_S  *pstSinkCap;
    HDMI_APP_ATTR_S         *pstAppAttr = HI_NULL;
    HDMI_VO_ATTR_S          *pstVoAttr = HI_NULL;
    HDMI_TMDS_MODE_E         enTmdsMode = HDMI_TMDS_MODE_HDMI_1_4;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstAppAttr      = &pstHdmiDev->stAttr.stAppAttr;
    pstVoAttr       = &pstHdmiDev->stAttr.stVOAttr;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);
    enEdidRet = DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap);
    if (enEdidRet == HDMI_EDID_DATA_INVALID)
    {
        HDMI_WARN("Invalid EdidCapability!\n");
    }

    if (!pstAppAttr->bEnableHdmi)//DVI
    {
        enTmdsMode                      = HDMI_TMDS_MODE_DVI;

        stScdcStatus.bSinkScrambleOn    = HI_FALSE;
        stScdcStatus.bSourceScrambleOn  = HI_FALSE;
        stScdcStatus.u8TmdsBitClkRatio  = 10;
        if ((pstVoAttr->u32HdmiAdaptPixClk > HDMI_EDID_MAX_HDMI14_TMDS_RATE)&&(!pstAppAttr->bEnableClrSpaceAdapt))
        {
            HDMI_ERR("TmdsClk=%u can't support in DVI mode",pstVoAttr->u32HdmiAdaptPixClk);
            return HI_FAILURE;
        }
    }
    else if(pstVoAttr->u32HdmiAdaptPixClk >= HDMI_EDID_MAX_HDMI14_TMDS_RATE
            && stTxCap.bTxHdmi_20
            && pstSinkCap->bSupportSCDC)
    {
        enTmdsMode                      = HDMI_TMDS_MODE_HDMI_2_0;

        stScdcStatus.bSinkScrambleOn    = HI_TRUE;
        stScdcStatus.bSourceScrambleOn  = HI_TRUE;
        stScdcStatus.u8TmdsBitClkRatio  = 40;
        if(!pstAppAttr->bEnableHdmi)
        {
            pstAppAttr->bEnableHdmi = HI_TRUE;
            HDMI_WARN("TmdsClk=%u can't support in DVI,adapt to Hdmi2.0\n",pstVoAttr->u32HdmiAdaptPixClk);
        }

    }
    else if(stTxCap.bTxHdmi_20 && pstSinkCap->bSupportSCDC && pstSinkCap->bSupportLte340McscScrameble)
    {
        enTmdsMode                      = HDMI_TMDS_MODE_HDMI_2_0;

        stScdcStatus.bSinkScrambleOn    = HI_TRUE;
        stScdcStatus.bSourceScrambleOn  = HI_TRUE;
        stScdcStatus.u8TmdsBitClkRatio  = 10;
    }
    else
    {
        enTmdsMode                      = HDMI_TMDS_MODE_HDMI_1_4;

        stScdcStatus.bSinkScrambleOn    = HI_FALSE;
        stScdcStatus.bSourceScrambleOn  = HI_FALSE;
        stScdcStatus.u8TmdsBitClkRatio  = 10;
    }

    HDMI_INFO("Tmds Mode %u->%u.\n",pstHdmiDev->enTmdsMode,enTmdsMode);
    pstHdmiDev->enTmdsMode = enTmdsMode;
    pstHdmiDev->pstHdmiHal->HAL_HDMI_TmdsModeSet(pstHdmiDev->pstHdmiHal, pstHdmiDev->enTmdsMode);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcStatusGet(pstHdmiDev->pstHdmiHal, &stScdcGet);

    if (pstSinkCap->bSupportSCDC
        && (stScdcGet.bSinkScrambleOn != stScdcStatus.bSinkScrambleOn
        || stScdcGet.bSourceScrambleOn != stScdcStatus.bSourceScrambleOn
        || stScdcGet.u8TmdsBitClkRatio != stScdcStatus.u8TmdsBitClkRatio))
    {
        stScdcStatus.u32ScrambleInterval    = 20;
        stScdcStatus.u32ScrambleTimeout     = 200;
        stScdcStatus.bSinkReadQuest         = HI_FALSE;

        pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcStatusSet(pstHdmiDev->pstHdmiHal, &stScdcStatus);
    }


    return HI_SUCCESS;
}

#else
static HI_S32 HdmiModeStrategy(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_APP_ATTR_S         *pstAppAttr = HI_NULL;
    HDMI_VO_ATTR_S          *pstVoAttr = HI_NULL;
    HDMI_TMDS_MODE_E         enTmdsMode = HDMI_TMDS_MODE_HDMI_1_4;

    CHECK_POINTER(pstHdmiDev);

    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;
    pstVoAttr = &pstHdmiDev->stAttr.stVOAttr;

    if (!pstAppAttr->bEnableHdmi)//DVI
    {
        enTmdsMode = HDMI_TMDS_MODE_DVI;
        if (pstVoAttr->u32HdmiAdaptPixClk > HDMI_EDID_MAX_HDMI14_TMDS_RATE)
        {
            if (pstAppAttr->bEnableClrSpaceAdapt)
            {
                pstAppAttr->bEnableHdmi = HI_TRUE;
                enTmdsMode = HDMI_TMDS_MODE_HDMI_2_0;
                HDMI_WARN("TmdsClk=%u can't support in DVI,adapt to Hdmi2.0\n",pstVoAttr->u32HdmiAdaptPixClk);
            }
            else
            {
                HDMI_ERR("TmdsClk=%u can't support in DVI mode",pstVoAttr->u32HdmiAdaptPixClk);
                return HI_FAILURE;
            }
        }
    }
    else//HDMI
    {
        enTmdsMode = pstVoAttr->u32HdmiAdaptPixClk < HDMI_EDID_MAX_HDMI14_TMDS_RATE ?
                                    HDMI_TMDS_MODE_HDMI_1_4 : HDMI_TMDS_MODE_HDMI_2_0;
    }

    HDMI_INFO("Tmds Mode %u->%u.\n",pstHdmiDev->enTmdsMode,enTmdsMode);
    pstHdmiDev->enTmdsMode = enTmdsMode;
    pstHdmiDev->pstHdmiHal->HAL_HDMI_TmdsModeSet(pstHdmiDev->pstHdmiHal, pstHdmiDev->enTmdsMode);

#ifdef HDMI_SCDC_SUPPORT

    if(HI_TRUE == pstHdmiDev->stEdidInfo.stCapability.bSupportSCDC || HI_TRUE == pstHdmiDev->stAttr.stAppAttr.bAuthMode)
    {
        if (pstHdmiDev->enTmdsMode == HDMI_TMDS_MODE_HDMI_2_0)
        {
            HdmiScrambleEnableSet(pstHdmiDev, HI_TRUE);
        }
        else
        {
            HdmiScrambleEnableSet(pstHdmiDev, HI_FALSE);
        }
    }
    else
    {
        HDMI_INFO("Can't support SCDC \n");
    }
#endif

    return HI_SUCCESS;
}
#endif

#ifdef HDMI_HDCP_SUPPORT

static HI_S32 HdcpSet(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_BOOL bHotPlug = HI_FALSE;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_HotPlugStatusGet(pstHdmiDev->pstHdmiHal, &bHotPlug);
    HDMI_INFO("Hdcp schedule, Hotplug:%d, HdcpIsOn:%d, HdcpWasOn:%d\n", bHotPlug,pstHdmiDev->stHdcpInfo.bHdcpIsOn, pstHdmiDev->stHdcpInfo.bHdcpWasOn);

    HDMI_INFO("bHotPlug:%d, %d-%d\n", bHotPlug, pstHdmiDev->stHdcpInfo.bHdcpIsOn, pstHdmiDev->stHdcpInfo.bHdcpWasOn);

    if (bHotPlug)// hot plug in
    {
        if (pstHdmiDev->stHdcpInfo.bHdcpIsOn && !pstHdmiDev->stHdcpInfo.bHdcpWasOn)
        {
            HDMI_INFO("Start hdcp\n");
            msleep(50);
            pstHdmiDev->stHdcpInfo.bHdcpWasOn = HI_TRUE;
            pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpEnableSet(pstHdmiDev->pstHdmiHal, HI_TRUE);
            HDMI_INFO("Start hdcp\n");
        }
        else if (!pstHdmiDev->stHdcpInfo.bHdcpIsOn && pstHdmiDev->stHdcpInfo.bHdcpWasOn)
        {
            HDMI_INFO("Stop hdcp\n");
            msleep(50);
            pstHdmiDev->stHdcpInfo.bHdcpWasOn   = HI_FALSE;
            pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
            HDMI_INFO("Stop hdcp\n");
        }
    }
    else// hot plug out
    {
        if (pstHdmiDev->stHdcpInfo.bHdcpWasOn)
        {
            HDMI_INFO(">>>>>>>>>>>>>\n");
            pstHdmiDev->stHdcpInfo.bHdcpWasOn = HI_FALSE;
            pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
            HDMI_WARN("Hotplug is out,so stop hdcp\n");
        }
        else if (pstHdmiDev->stHdcpInfo.bHdcpIsOn)
        {
            HDMI_WARN("Hotplug is out,so do not start hdcp\n");
            return HI_FAILURE;
        }
    }

    if(!pstHdmiDev->stHdcpInfo.bHdcpIsOn)
    {
        pstHdmiDev->stHdcpInfo.bHdcp2Prep   = HI_FALSE;
        pstHdmiDev->stHdcpInfo.bHdcpAuthSuc = HI_FALSE;
    }

    HDMI_INFO("OUT...");

    return HI_SUCCESS;
}

static HI_S32 HdcpStrategy(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_S32  s32Ret = HI_FAILURE;
    HDMI_APP_ATTR_S* pstAppAttr;
    HDMI_SINK_CAPABILITY_S  *pstSinkCaps = HI_NULL;
    HI_BOOL                 bHdcp22Mode = HI_FALSE;
    HI_BOOL                 bHdcp14Mode = HI_FALSE;
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S stParam;
#endif
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstAppAttr = &pstHdmiDev->stAttr.stAppAttr;
    pstSinkCaps = &(pstHdmiDev->stEdidInfo.stCapability);

    /* HDCP mode select*/
    HDMI_INFO("bHdcpIsOn: %d\n", pstHdmiDev->stHdcpInfo.bHdcpIsOn);
    if (pstHdmiDev->stHdcpInfo.bHdcpIsOn)
    {
        HDMI_TX_CAPABILITY_S     stTxCap;
        if (HI_TRUE != pstHdmiDev->stHdcpInfo.bHdcpWasOn)
        {
            DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
        }

        pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);

        pstHdmiDev->stHdcpInfo.eHdcp22Support = pstSinkCaps->stHdcpSupport.bHdcp22Support ? HDMI_HDCP_SINK_SUPPORT : HDMI_HDCP_SINK_NOSUPPORT;
        pstHdmiDev->stHdcpInfo.eHdcp14Support = pstSinkCaps->stHdcpSupport.bHdcp14Support ? HDMI_HDCP_SINK_SUPPORT : HDMI_HDCP_SINK_NOSUPPORT;


        pstSinkCaps->stHdcpSupport.bHdcp14Support = (HI_TRUE == pstHdmiDev->stAttr.stAppAttr.bAuthMode)? \
                                                     HI_TRUE : pstSinkCaps->stHdcpSupport.bHdcp14Support;

        bHdcp22Mode = (   (HI_TRUE == stTxCap.bTxHdcp_22)
                       && (HI_TRUE == pstSinkCaps->stHdcpSupport.bHdcp22Support)
                       && ((HDMI_HDCP_MODE_2_2 == pstHdmiDev->stHdcpInfo.enHdcpMode || HDMI_HDCP_MODE_AUTO == pstHdmiDev->stHdcpInfo.enHdcpMode)));

        bHdcp14Mode = (   (HI_TRUE == stTxCap.bTxHdcp_14)
                       && (HI_TRUE == pstSinkCaps->stHdcpSupport.bHdcp14Support)
                       && ((HDMI_HDCP_MODE_1_4 == pstHdmiDev->stHdcpInfo.enHdcpMode || HDMI_HDCP_MODE_AUTO == pstHdmiDev->stHdcpInfo.enHdcpMode)));

        if (HI_TRUE == bHdcp22Mode)
        {
            /* start hdcp2.2 auth */
            pstHdmiDev->stHdcpInfo.enHdcpMode = HDMI_HDCP_MODE_2_2;

            HDMI_INFO("Sink hdcp2.2 support(%d)\n", pstHdmiDev->stHdcpInfo.eHdcp22Support);

#ifndef HDMI_TEE_SUPPORT
            s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpMcuCodeLoad(pstHdmiDev->pstHdmiHal, pstHdmiDev->stHdcpInfo.enHdcpMode);
            if(HI_SUCCESS != s32Ret)
            {
                HDMI_ERR("8051 code load fail. \n");
                return HI_FAILURE;
            }
            pstHdmiDev->stHdcpInfo.bHdcp2Prep = HI_TRUE;
#else
            if(!pstHdmiDev->stHdcpInfo.bHdcpWasOn)
            {
                HDMI_MEMSET(&stParam, 0, sizeof(HDMI_TEE_PARAM_S));
                stParam.enTeeCmd = HI_HDMI_HDCP22_INIT;
                s32Ret = DRV_HDMI_TeeCmdSend(pstHdmiDev->u32HdmiDevId, &stParam);
                if(s32Ret != HI_SUCCESS)
                {
                    HDMI_ERR("8051 code check fail. \n");
                    return HI_FAILURE;
                }
                pstHdmiDev->stHdcpInfo.bHdcp2Prep = HI_TRUE;
            }
#endif
        }
        else if (HI_TRUE == bHdcp14Mode)
        {
            pstHdmiDev->stHdcpInfo.enHdcpMode = HDMI_HDCP_MODE_1_4;
            if (!pstHdmiDev->stHdcpInfo.bHdcp1Prep)
            {
                HDMI_ERR("Do not load hdcp1.4 key\n");
                return HI_FAILURE;
            }
        }
        else
        {
            HDMI_WARN("Do not support HDCP\n");
            pstHdmiDev->stHdcpInfo.bHdcpIsOn = HI_FALSE;
            return HI_FAILURE;
        }

        s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpModeSet(pstHdmiDev->pstHdmiHal, pstHdmiDev->stHdcpInfo.enHdcpMode);
        if (s32Ret != HI_SUCCESS)
        {
            HDMI_ERR("Set Hdcp mode fail\n");
            return HI_FAILURE;
        }
    }

    s32Ret = HdcpSet(pstHdmiDev);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif


HI_S32 DRV_HDMI_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

static HI_S32 HdmiAttrConstruct(HDMI_DEVICE_S * pstHdmiDev, HDMI_ATTR_S *pstAttr)
{
    HDMI_HARDWARE_STATUS_S   stHwStatus;
    HDMI_AO_ATTR_S*          pstAudioAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_APP_ATTR_S*         pstAppAttr;
    HI_BOOL                  bIsDolby = HI_FALSE;
    HI_BOOL                  bInClrIsRgb = HI_FALSE;
    HI_BOOL                  bAspectIs256 = HI_FALSE;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstAttr);

    pstAppAttr   = &pstAttr->stAppAttr;
    pstAudioAttr = &pstAttr->stAOAttr;
    pstVideoAttr = &pstAttr->stVOAttr;

    memset(&stHwStatus, 0, sizeof(HDMI_HARDWARE_STATUS_S));
    pstHdmiDev->pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiDev->pstHdmiHal, &stHwStatus);
    /* user attribute construct*/
    pstHdmiDev->enTmdsMode = stHwStatus.stCommonStatus.enTmdsMode;
    switch (pstHdmiDev->enTmdsMode)
    {
        case HDMI_TMDS_MODE_HDMI_1_4:
        case HDMI_TMDS_MODE_HDMI_2_0:
        {
            pstAppAttr->bEnableHdmi  = HI_TRUE;
            pstAppAttr->bEnableVideo = HI_TRUE;
            break;
        }
        case HDMI_TMDS_MODE_DVI:
        {
            pstAppAttr->bEnableHdmi  = HI_FALSE;
            pstAppAttr->bEnableVideo = HI_TRUE;
            break;
        }
        default:
        {
            pstAppAttr->bEnableHdmi  = HI_FALSE;
            pstAppAttr->bEnableVideo = HI_FALSE;
            break;
        }
    }

    pstAppAttr->bEnableAudio     = (stHwStatus.stAudioStatus.bAudioEnable &&
                                   stHwStatus.stInfoFrameStatus.bAUDIOEnable);
    /* out colorspace is yuv444 when the hdr mode is dolby. */
    bIsDolby = (   (HDMI_HDR_USERMODE_DOLBY == pstHdmiDev->stAttr.stHdrAttr.enUserHdrMode)
                && (   (!stHwStatus.stVideoStatus.bRGB2YCbCr)
                    && (!stHwStatus.stVideoStatus.bYCbCr2RGB)
                    && (!stHwStatus.stVideoStatus.bYCbCr444_422)
                    && (!stHwStatus.stVideoStatus.bYCbCr422_420)
                    && (!stHwStatus.stVideoStatus.bYCbCr422_444)
                    && (!stHwStatus.stVideoStatus.bYCbCr420_422)));

    pstAppAttr->enOutColorSpace = (HI_TRUE == bIsDolby) ? HDMI_COLORSPACE_YCbCr444 : stHwStatus.stVideoStatus.enOutColorSpace;

    BITDEPTH_2_DEEPCOLOR(stHwStatus.stVideoStatus.enOutBitDepth, pstAppAttr->enDeepColorMode);

	pstAppAttr->bEnableAviInfoFrame  = stHwStatus.stInfoFrameStatus.bAVIEnable;
	pstAppAttr->bEnableAudInfoFrame  = stHwStatus.stInfoFrameStatus.bAUDIOEnable;
    pstAppAttr->bxvYCCMode          = stHwStatus.stInfoFrameStatus.bGBDEnable;
    /* video attribute construct*/
    pstVideoAttr->enInColorSpace = pstAppAttr->enOutColorSpace;
    pstVideoAttr->bVSyncPol      = stHwStatus.stVideoStatus.bVSyncPol;
    pstVideoAttr->bHSyncPol      = stHwStatus.stVideoStatus.bHSyncPol;
    pstVideoAttr->bDEPol         = stHwStatus.stVideoStatus.bDEPol;

    bInClrIsRgb = (   (stHwStatus.stVideoStatus.bRGB2YCbCr)
                   || (   (!stHwStatus.stVideoStatus.bYCbCr2RGB)
                       && (HDMI_COLORSPACE_RGB == stHwStatus.stVideoStatus.enOutColorSpace)));
    pstVideoAttr->enInColorSpace = (HI_TRUE == bInClrIsRgb)? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCbCr444;

    if (stHwStatus.stInfoFrameStatus.bAVIEnable)
    {
        HDMI_VIDEO_CODE_E  enVideoCode  = stHwStatus.stInfoFrameStatus.u8AVI[7];

        /* when the timing is 4096*2160, the aspect ratio in AVI Infoframe is 0
          (but the real aspect ratio is 256:135<0x4>, the enVideoCode is 0) */
        bAspectIs256 = (   ((enVideoCode == 0) && (stHwStatus.stInfoFrameStatus.u8VSIF[8] == 4))
                        || ((enVideoCode >= HDMI_4096X2160P25_256_135) && (enVideoCode <= HDMI_4096X2160P60_256_135)));

        pstVideoAttr->enPictureAspect = (HI_TRUE == bAspectIs256)? HDMI_PICTURE_ASPECT_256_135 : ((stHwStatus.stInfoFrameStatus.u8AVI[5] >> 4) & 0x3);

        pstVideoAttr->enActiveAspect    = stHwStatus.stInfoFrameStatus.u8AVI[5] & 0xf;
        pstVideoAttr->enColorimetry     = (stHwStatus.stInfoFrameStatus.u8AVI[5] >> 6) & 0x3;
        pstVideoAttr->enRGBQuantization = (stHwStatus.stInfoFrameStatus.u8AVI[6] >> 2) & 0x3;
        pstVideoAttr->enYCCQuantization = (stHwStatus.stInfoFrameStatus.u8AVI[8] >> 6) & 0x3;
        pstVideoAttr->u32PixelRepeat    = (stHwStatus.stInfoFrameStatus.u8AVI[8] & 0xf) + 1;
		pstVideoAttr->enExtendedColorimetry = (stHwStatus.stInfoFrameStatus.u8AVI[6] >> 4) & 0x07;
        pstVideoAttr->enVideoTiming     =  DRV_HDMI_VideoTimingLookup(enVideoCode, pstVideoAttr->enPictureAspect);
        HDMI_INFO("the enVideoTiming is:%d, enVideoCode:%d, enPictureAspect:%d\n", pstVideoAttr->enVideoTiming, enVideoCode, pstVideoAttr->enPictureAspect);
        if ((!stHwStatus.stInfoFrameStatus.bVSIFEnable) && (!enVideoCode))
        {
            pstVideoAttr->enVideoTiming = HDMI_VIDEO_TIMING_UNKNOWN;
        }
    }
    pstVideoAttr->enStereoMode = HDMI_3D_BUTT;
    if (stHwStatus.stInfoFrameStatus.bVSIFEnable)
    {
        enum hdmi_video_format format = (stHwStatus.stInfoFrameStatus.u8VSIF[7] >> 5) & 0x3;
        if (format == HDMI_VIDEO_FORMAT_4K)
        {
            HDMI_VSIF_VIC_E enVideoCode = stHwStatus.stInfoFrameStatus.u8VSIF[8];
            pstVideoAttr->enVideoTiming = DRV_HDMI_VsifVideoTimingLookup(enVideoCode, pstVideoAttr->enPictureAspect);
        }
        else if (format == HDMI_VIDEO_FORMAT_3D)
        {
            pstVideoAttr->enStereoMode    = (stHwStatus.stInfoFrameStatus.u8VSIF[8] >> 4) & 0xf;
        }
    }

    /* audio attrbute construct*/
    pstAudioAttr->bDownSample   = stHwStatus.stAudioStatus.bDownSample;
    pstAudioAttr->enChanels     = stHwStatus.stAudioStatus.enLayout;
    pstAudioAttr->enSampleDepth = stHwStatus.stAudioStatus.enSampleDepth;
    pstAudioAttr->enSampleFs    = stHwStatus.stAudioStatus.enSampleFs;
    pstAudioAttr->enSoundIntf   = stHwStatus.stAudioStatus.enSoundIntf;
    if (stHwStatus.stInfoFrameStatus.bAUDIOEnable)
    {
        pstAudioAttr->enAudioCode = (stHwStatus.stInfoFrameStatus.u8AUDIO[4] >> 4) & 0xf;
    }

    return HI_SUCCESS;
}

#ifdef HDMI_TEE_SUPPORT
static HI_S32 HdmiTeeHdcpCapSet(HDMI_DEVICE_S *pstHdmiDev)
{
    HDMI_TEE_PARAM_S         stParam;
    HI_S32                   s32Ret = HI_SUCCESS;
    HDMI_TX_CAPABILITY_S     stTxCap;
    HDMI_HDCP_CAPABILITY_S   *pstHdcpCaps = HI_NULL;
    CHECK_POINTER(pstHdmiDev);

	HDMI_MEMSET(&stParam, 0, sizeof(HDMI_TEE_PARAM_S));
	HDMI_MEMSET(&stTxCap, 0, sizeof(HDMI_TX_CAPABILITY_S));

    pstHdcpCaps = &(pstHdmiDev->stEdidInfo.stCapability.stHdcpSupport);
    if(HI_TRUE == pstHdmiDev->stTeeInfo.bTeeOpen)
    {
        stParam.enTeeCmd = HI_HDMI_HDCP_CAPS;

        pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);

        stParam.seHdcpCaps.bHdcp14Support = (pstHdcpCaps->bHdcp14Support & stTxCap.bTxHdcp_14);
        stParam.seHdcpCaps.bHdcp22Support = (pstHdcpCaps->bHdcp22Support & stTxCap.bTxHdcp_22);

        s32Ret = DRV_HDMI_TeeCmdSend(pstHdmiDev->u32HdmiDevId, &stParam);
        if(s32Ret != HI_SUCCESS)
        {
            HDMI_ERR("send HDCP caps to tee error!\n");
        }
    }
    return s32Ret;
}
#endif
/*
static HDMI_TRANSITION_STATE_E HdmiTransitionStateGet(HDMI_DEVICE_S * pstHdmiDev)
{
    CHECK_POINTER(pstHdmiDev);
    return pstHdmiDev->enTransitionState;
}
*/

HI_S32 HpdEvent(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_S32 s32Ret           = HI_SUCCESS;
    HI_U32 u32TimeOutCnt    = 5;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    /* update edid from sink*/
    if(HI_FALSE == pstHdmiDev->bHpdDetected)
    {
        DRV_HDMI_EdidReset(&pstHdmiDev->stEdidInfo);
        do
        {
            /*  1.for some TV ,EDID need to read more times ;
                2.for some repeater, EDID is readable after HPD assert 250ms. */

            s32Ret = DRV_HDMI_EdidUpdate(&pstHdmiDev->stEdidInfo, HDMI_EDID_UPDATE_SINK);
#ifdef HDMI_HDCP_SUPPORT
            pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpSinkCapabilityGet(pstHdmiDev->pstHdmiHal, &(pstHdmiDev->stEdidInfo.stCapability.stHdcpSupport));
#endif
            if (s32Ret == HI_SUCCESS)
            {
                pstHdmiDev->bHpdDetected = HI_TRUE;
                break;
            }
                msleep(50);
        }while(u32TimeOutCnt--);

        if (s32Ret != HI_SUCCESS)
        {
            HDMI_WARN("Update EDID fail,u32TimeOutCnt=%d\n", u32TimeOutCnt);
        }

#ifdef HDMI_TEE_SUPPORT
        s32Ret = HdmiTeeHdcpCapSet(pstHdmiDev);
#endif
    }
#ifdef HDMI_CEC_SUPPORT
    HdmiCecNetWorkSet(pstHdmiDev);
#endif

    return s32Ret;
}

static HI_VOID EventNotify(HDMI_DEVICE_S* pstHdmiDev, HDMI_EVENT_E enEvent)
{
    CHECK_POINTER_NO_RET(pstHdmiDev);

    if (enEvent <= HDMI_EVENT_HDCP_USERSETTING)
    {
        if (!pstHdmiDev->pKCallback)/* Notify to user */
        {
            /* Don't notify to user when the auth mode is enable and the event is RSEN,
               to avoid HDCP2.2 1B-08 CTS test failed on SL8800. */
            if(   (HI_TRUE == pstHdmiDev->stAttr.stAppAttr.bAuthMode)
               && ((HDMI_EVENT_RSEN_CONNECT == enEvent) || (HDMI_EVENT_RSEN_DISCONNECT == enEvent)))
            {
                HDMI_WARN("Auth mode, RSEN event not notify to user.\n");
            }
            else
            {
                HDMI_INFO("Notify event(0x%x) to user\n", enEvent);
                DRV_HDMI_EventPoolWrite(&pstHdmiDev->stEventInfo, enEvent);
            }
        }
        else/* Notify to kernel */
        {
            HDMI_INFO("Notify event(0x%x) to kernel\n", enEvent);
			HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
			if(pstHdmiDev->pKCallback)
			{
				pstHdmiDev->pKCallback((HI_VOID*)&(pstHdmiDev->u32HdmiDevId), enEvent);
			}
			HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
        }
    }
    return;
}

static HI_S32 HdmiEventCallback(HI_VOID* pData , HDMI_EVENT_E enEvent)
{
    HI_S32          s32Ret          = HI_SUCCESS;
    HDMI_DEVICE_S* pstHdmiDev       = (HDMI_DEVICE_S*) pData;
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S    stParam;
    HI_BOOL             bTeeEventHdcp1xSucc = HI_FALSE;

    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd    = HI_HDMI_HDCP14_EVENT;
    stParam.pvData      = &bTeeEventHdcp1xSucc;
    stParam.u32Size     = sizeof(bTeeEventHdcp1xSucc);
#endif

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    switch (enEvent)
    {
        case HDMI_EVENT_HOTPLUG:
            {
                HpdEvent(pstHdmiDev);
                HdmiSetAndroidState(STATE_HOTPLUGIN);
                break;
            }
        case HDMI_EVENT_HOTUNPLUG:
            {
                pstHdmiDev->bHpdDetected = HI_FALSE;
#ifdef HDMI_HDCP_SUPPORT
                pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpEnableSet(pstHdmiDev->pstHdmiHal,HI_FALSE);
#endif
                HdmiSetAndroidState(STATE_HOTPLUGOUT);
                break;
            }
        case HDMI_EVENT_HDCP_FAIL:
            {
                DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
                pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
                pstHdmiDev->stHdcpInfo.bHdcpAuthSuc = HI_FALSE;

#ifdef HDMI_TEE_SUPPORT
                if(HDMI_HDCP_MODE_1_4 == pstHdmiDev->stHdcpInfo.enHdcpMode)
                {
                    bTeeEventHdcp1xSucc = HI_FALSE;
                    DRV_HDMI_TeeCmdSend(pstHdmiDev->u32HdmiDevId, &stParam);
                }
#endif
                break;
            }
        case HDMI_EVENT_RSEN_CONNECT:
            {
                 break;
            }
        case HDMI_EVENT_RSEN_DISCONNECT:
            {
                 break;
            }
        case HDMI_EVENT_HDCP_OFF:
            {
                DRV_HDMI_BlackDataSet(pstHdmiDev, HI_FALSE);
                DRV_HDMI_AudioOutputEnableSet(pstHdmiDev, HI_FALSE);
                pstHdmiDev->stHdcpInfo.bHdcpAuthSuc = HI_FALSE;
                break;
            }
        case HDMI_EVENT_HDCP_SUCCESS:
            {
                DRV_HDMI_BlackDataSet(pstHdmiDev, HI_FALSE);
                pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet(pstHdmiDev->pstHdmiHal, HI_TRUE);
                pstHdmiDev->stHdcpInfo.bHdcpAuthSuc = HI_TRUE;

#ifdef HDMI_TEE_SUPPORT
                if(HDMI_HDCP_MODE_1_4 == pstHdmiDev->stHdcpInfo.enHdcpMode)
                {
                    bTeeEventHdcp1xSucc = HI_TRUE;
                    DRV_HDMI_TeeCmdSend(pstHdmiDev->u32HdmiDevId, &stParam);
                }
#endif
                break;
            }
        case HDMI_EVENT_SCRAMBLE_FAIL:
            break;
        case HDMI_EVENT_SCRAMBLE_SUCCESS:
            break;
#ifdef HDMI_HDR_SUPPORT
        case HDMI_EVENT_ZERO_DRMIF_TIMEOUT:
            {
                HdmiZeroDrmIfStop(pstHdmiDev);
                break;
            }
        case HDMI_EVENT_SWITCH_TO_HDRMODE_TIMEOUT:
            {
                HdmiHdrModeChangeTimeout(pstHdmiDev);
                break;
            }
#endif
        default :
            break;
    }

    EventNotify(pstHdmiDev, enEvent);

    return s32Ret;
}

static HI_S32 HdmiKThreadTimer(void* data)
{
    HDMI_DEVICE_S* pstHdmiDev = (HDMI_DEVICE_S*)data;
    CHECK_POINTER(pstHdmiDev);

    while ( 1 )
    {
        if (kthread_should_stop())
        {
            break;
        }

        if (pstHdmiDev->stThreadInfo.kThreadTimerSate == HDMI_THREAD_STATE_STOP)
        {
            msleep(90);
            continue;
        }

        if (pstHdmiDev->pstHdmiHal)
        {
            if (pstHdmiDev->pstHdmiHal->HAL_HDMI_SequencerHandlerProcess)
            {
                pstHdmiDev->pstHdmiHal->HAL_HDMI_SequencerHandlerProcess((struct hiHDMI_HAL_S *)pstHdmiDev);
            }
        }
        else
        {
            HDMI_ERR("HDMI_KThreadTimer's input param is NULL\n");
        }
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
        msleep(10);
#endif
#if defined (HDMI_SUPPORT_LOGIC_SILICON)
        msleep(90);
#endif
    }

    return HI_SUCCESS;
}


HDMI_VIDEO_TIMING_E DispFmt2HdmiTiming(HDMI_DEVICE_S* pstHdmiDev, HI_U32 u32DispFmt)
{
    if (pstHdmiDev->pstHdmiHal && pstHdmiDev->pstHdmiHal->HAL_HDMI_DispFmt2HdmiTiming)
    {
        return pstHdmiDev->pstHdmiHal->HAL_HDMI_DispFmt2HdmiTiming(pstHdmiDev->pstHdmiHal, u32DispFmt);
    }

    return HDMI_VIDEO_TIMING_1280X720P_50000;
}

#if 1
static HI_VOID HdmiVOAttrInit(HDMI_VO_ATTR_S *pstVideoAttr)
{
    HI_BOOL bColorimetryIs601 = HI_FALSE;
    CHECK_POINTER_NO_RET(pstVideoAttr);


    bColorimetryIs601 = ( (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_720X480P_60000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_720X576P_50000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X240P_60000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X480I_60000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X480I_60000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576I_50000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576P_50000)
                       || (pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_1440X576I_60000));

    if(pstVideoAttr->enVideoTiming  == HDMI_VIDEO_TIMING_640X480P_60000)
    {
        pstVideoAttr->enColorimetry   = HDMI_COLORIMETRY__ITU_709;
        pstVideoAttr->enPictureAspect = HDMI_PICTURE_ASPECT_4_3;
    }
    else if (HI_TRUE == bColorimetryIs601)
    {
        pstVideoAttr->enColorimetry   = HDMI_COLORIMETRY__ITU_601;
        pstVideoAttr->enPictureAspect = HDMI_PICTURE_ASPECT_4_3;

	}
    else if(   pstVideoAttr->enVideoTiming <= HDMI_VIDEO_TIMING_4096X2160P_60000
            && pstVideoAttr->enVideoTiming >= HDMI_VIDEO_TIMING_4096X2160P_24000)
    {
        pstVideoAttr->enColorimetry   = HDMI_COLORIMETRY__ITU_709;
        pstVideoAttr->enPictureAspect = HDMI_PICTURE_ASPECT_256_135;
    }
    else
    {
        pstVideoAttr->enColorimetry   = HDMI_COLORIMETRY__ITU_709;
        pstVideoAttr->enPictureAspect = HDMI_PICTURE_ASPECT_16_9;
    }

    pstVideoAttr->u32PixelRepeat = 1;
    if (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X480I_60000 ||
        pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X576I_50000)
    {
        pstVideoAttr->u32PixelRepeat = 2;
    }

    if (pstVideoAttr->enVideoTiming <= HDMI_VIDEO_TIMING_640X480P_60000)
    {
		// cts1.4 TestID 7-24 required RGBQuantization is default or limited when the timing is 640x480p60
        pstVideoAttr->enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
    }
    else
    {
        pstVideoAttr->enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
    }

#if 1
    if (pstVideoAttr->enVideoTiming != HDMI_VIDEO_TIMING_UNKNOWN &&
        pstVideoAttr->enVideoTiming != HDMI_VIDEO_TIMING_640X480P_60000 )
    {
        pstVideoAttr->enInColorSpace = HDMI_COLORSPACE_YCbCr444;
    }
    else
    {
        pstVideoAttr->enInColorSpace = HDMI_COLORSPACE_RGB;
    }
#else
    //pstVideoAttr->enInColorSpace = HDMI_COLORSPACE_YCbCr444;
#endif

    pstVideoAttr->enStereoMode    = HDMI_3D_BUTT;
    pstVideoAttr->enInBitDepth    = HDMI_VIDEO_BITDEPTH_10;
    pstVideoAttr->enActiveAspect  = HDMI_ACTIVE_ASPECT_PICTURE;
}
#endif

static HI_S32 HdmiDeviceInit(HDMI_DEVICE_S * pstHdmiDev)
{
    //HI_U32                   u32DispFmt = 0;
    HDMI_HAL_BASE_PARAM_S    stBaseParam;
    HDMI_AO_ATTR_S*          pstAudioAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_APP_ATTR_S*         pstAppAttr;
    HDMI_HDR_ATTR_S*         pstHdrAttr;
    CHECK_POINTER(pstHdmiDev);

    HDMI_INIT_MUTEX(pstHdmiDev->stMutexThread);

    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;
    pstAudioAttr = &pstHdmiDev->stAttr.stAOAttr;
    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstHdrAttr   = &pstHdmiDev->stAttr.stHdrAttr;

    DRV_HDMI_DefaultActionSet(pstHdmiDev, HDMI_DEFAULT_ACTION_HDMI);
#ifdef HDMI_EMI_ENABLE
    pstHdmiDev->bEmiEnable          = HI_TRUE;
#else
    pstHdmiDev->bEmiEnable          = HI_FALSE;
#endif
    /* connon para init */
    pstHdmiDev->stDebugContext.u32StopDelay = 0;
    /* user attribute init*/
    pstAppAttr->bEnableHdmi         = HI_TRUE;
    pstAppAttr->bEnableVideo        = HI_TRUE;
    pstAppAttr->bEnableAudio        = HI_TRUE;
    pstAppAttr->bEnableAudInfoFrame = HI_TRUE;
    pstAppAttr->bEnableAviInfoFrame = HI_TRUE;
    pstAppAttr->enDeepColorMode     = HDMI_DEEP_COLOR_24BIT;
    pstAppAttr->enOutColorSpace     = HDMI_COLORSPACE_YCbCr444;
    pstAppAttr->bEnableClrSpaceAdapt= HI_TRUE;
    pstAppAttr->bEnableDeepClrAdapt = HI_TRUE;
#ifdef HDMI_HDR_SUPPORT
    pstHdrAttr->enUserHdrMode       = HDMI_HDR_USERMODE_SDR;
    pstAppAttr->bEnableDRMInfoFrame = HI_TRUE;

    pstHdmiDev->stHdrInfo.enHdrDebugMode                = HDMI_HDR_DEBUG_MODE_NONE;
    pstHdmiDev->stHdrInfo.stHdrModeChnTimer.bTimerStart = HI_FALSE;
    pstHdmiDev->stHdrInfo.stHdrModeChnTimer.enTimerType = HDMI_TIMER_SDR_TO_HDR10;
    pstHdmiDev->stHdrInfo.stHdrModeChnTimer.u32Time     = HDRMODE_CHANGE_TIME;
    pstHdmiDev->stHdrInfo.stZeroDrmIfTimer.bTimerStart  = HI_FALSE;
    pstHdmiDev->stHdrInfo.stZeroDrmIfTimer.enTimerType  = HDMI_TIMER_ZERO_DRMIF;
    pstHdmiDev->stHdrInfo.stZeroDrmIfTimer.u32Time      = ZERO_DRMIF_SEND_TIME;
#endif

#ifdef HDMI_TEE_SUPPORT
    pstHdmiDev->stTeeInfo.bTeeOpen = HI_FALSE;
#endif

    /* video attribute init*/
    pstVideoAttr->enVideoTiming = HDMI_VIDEO_TIMING_1280X720P_50000;
    pstVideoAttr->u32ClkFs      = 74250;
    pstVideoAttr->bHSyncPol     = HI_FALSE;
    pstVideoAttr->bVSyncPol     = HI_FALSE;
    pstVideoAttr->bDEPol        = HI_FALSE;
    HDMI_MEMSET(&(pstHdmiDev->stIntfStatus), 0, sizeof(HDMI_INTF_STATUS_S));

    if (pstHdmiDev->pstHdmiHal && pstHdmiDev->pstHdmiHal->HAL_HDMI_DispFmtGet)
    {
        pstHdmiDev->pstHdmiHal->HAL_HDMI_DispFmtGet(pstHdmiDev->pstHdmiHal, &stBaseParam);
        pstVideoAttr->enVideoTiming = DispFmt2HdmiTiming(pstHdmiDev, stBaseParam.u32DispFmt);
        pstVideoAttr->u32ClkFs      = stBaseParam.u32PixClk;
        pstAppAttr->enDeepColorMode = stBaseParam.enDeepColor;
        pstAppAttr->enOutColorSpace = stBaseParam.enColorSpace;
        HDMI_INFO("BaseParam: DispFmt=%u,PixClk=%u,enDeepColorMode=%u,enVidOutMode=%u\n",
                                stBaseParam.u32DispFmt,
                                stBaseParam.u32PixClk,
                                stBaseParam.enDeepColor,
                                stBaseParam.enColorSpace);
    }

    HdmiVOAttrInit(pstVideoAttr);

    /* audio attribute init*/
    pstAudioAttr->enSoundIntf   = HDMI_AUDIO_INTERFACE__I2S;
    pstAudioAttr->enSampleFs    = HDMI_SAMPLE_RATE_48K;
    pstAudioAttr->enSampleDepth = HDMI_AUDIO_BIT_DEPTH_16;
    pstAudioAttr->enChanels     = HDMI_AUDIO_FORMAT_2CH;

    HDMI_INFO("VideoTiming:%d\n",pstVideoAttr->enVideoTiming );
    //DRV_HDMI_EventInit(&pstHdmiDev->stEventInfo);
    return HI_SUCCESS;
}

static HI_S32 CreateTask(HDMI_DEVICE_S* pstHdmiDev, HDMI_HAL_INIT_S *pstHalCfg)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHalCfg);

    if (pstHdmiDev->stThreadInfo.kThreadTimer == NULL)
    {
        /* open Hal module*/
        pstHalCfg->pfnEventCallBack   = HdmiEventCallback;
        pstHalCfg->pvEventData        = pstHdmiDev;
        pstHalCfg->u32HdmiDevId       = pstHdmiDev->u32HdmiDevId;
        s32Ret = HAL_HDMI_Open(pstHalCfg, &pstHdmiDev->pstHdmiHal);
        if (!pstHdmiDev->pstHdmiHal || s32Ret != HI_SUCCESS)
        {
             HDMI_ERR("HAL_HDMI_Open fail\n");
             return HI_FAILURE;
        }
        pstHdmiDev->pu32VirtualBaseAddr = pstHdmiDev->pstHdmiHal->HAL_HDMI_BaseAddrGet(pstHdmiDev->pstHdmiHal);
        if(pstHdmiDev->pu32VirtualBaseAddr == HI_NULL)
        {
            HDMI_ERR("pstHdmiDev->pu32VirtualBaseAddr map failed \n");
            return HI_FAILURE;
        }

        pstHdmiDev->pstHdmiHal->stHalCtx.hHdmiDev = pstHdmiDev;
        pstHdmiDev->pstHdmiHal->stHalCtx.u32HdmiID = pstHdmiDev->u32HdmiDevId;

        HDMI_INFO("Create HDMI Timer Thread \n");
        pstHdmiDev->stThreadInfo.kThreadTimer = kthread_run(HdmiKThreadTimer, pstHdmiDev, "HI_HDMI_kThread");
        if (HI_NULL == pstHdmiDev->stThreadInfo.kThreadTimer)
        {
            HDMI_ERR("Create HDMI Timer Thread fail\n");
            return HI_FAILURE;
        }
        HdmiDeviceInit(pstHdmiDev);
        pstHdmiDev->enRunState = HDMI_RUN_STATE_OPEN;
        pstHdmiDev->stHdcpInfo.eHdcp22Support = HDMI_HDCP_SINK_UNKNOWN;
        pstHdmiDev->stHdcpInfo.eHdcp14Support = HDMI_HDCP_SINK_UNKNOWN;
    }

    return s32Ret;
}


HI_S32 DRV_HDMI_Open(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bUser)
{
    HI_S32 s32Ret = HI_FAILURE;
    HI_BOOL bHdmiOn  = HI_FALSE;
    HI_BOOL bHotPLug = HI_FALSE;
    HDMI_HAL_INIT_S stHalCfg = {0};
    HI_U32 u32EventPoolId = 0;

    HDMI_INFO(">>> DRV_HDMI_Open in...\n");
    CHECK_POINTER(pstHdmiDev);

    HDMI_INFO("bUser: %d\n", bUser);

    /* create hdmi task ,every hdmi device only create once*/
    s32Ret = CreateTask(pstHdmiDev, &stHalCfg);
    if(HI_SUCCESS != s32Ret)
    {
        return HI_FAILURE;
    }

    DRV_HDMI_EventInit(&pstHdmiDev->stEventInfo);

    if (bUser)
    {
        pstHdmiDev->u32UserCnt++;
        /* Must Create Event Queue first */
        s32Ret = DRV_HDMI_EventPoolMalloc(&pstHdmiDev->stEventInfo, &u32EventPoolId);
        if (s32Ret != HI_SUCCESS)
        {
            if (s32Ret == HDMI_EVENT_ID_EXIST)
            {
                pstHdmiDev->u32UserCnt--;
            }
            else
            {
                HDMI_ERR("DRV_HDMI_EventPoolMalloc fail\n");
                return HI_FAILURE;
           }
        }
        HDMI_INFO("Create Event Queue for process:%d \n", get_current()->tgid);
    }
    else
    {
        pstHdmiDev->u32KernelCnt++;
    }

    if (pstHdmiDev->u32UserCnt > 0)
    {
    	HDMI_MUTEX_UNLOCK(s_HdmiMutex);
		HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
        pstHdmiDev->pKCallback = HI_NULL;
		HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
		HDMI_MUTEX_LOCK(s_HdmiMutex);
    }
    else if (pstHdmiDev->u32KernelCnt > 0)
    {
		HDMI_MUTEX_UNLOCK(s_HdmiMutex);
		HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
		pstHdmiDev->pKCallback = HI_DRV_HDMI_KernelEventCallback;
		HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
		HDMI_MUTEX_LOCK(s_HdmiMutex);
    }

    HDMI_INFO("u32UserCnt: %d, u32KernelCnt: %d\n", pstHdmiDev->u32UserCnt, pstHdmiDev->u32KernelCnt);

#ifdef HDMI_TEE_SUPPORT
    DRV_HDMI_TeeOpen(pstHdmiDev->u32HdmiDevId);
#endif

    /*20150513, l232354 STB boot have done it. But BVT have not done it in boot.
	  BVT work in boot need equa SBT work in boot later.
	*/
#if defined (CONFIG_HDMI_STB_SDK)
    pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableGet(pstHdmiDev->pstHdmiHal, &bHdmiOn);
#endif

#ifndef HDMI_FPGA_SUPPORT
    if (!bHdmiOn)
    {
         pstHdmiDev->enTransitionState = HDMI_TRANSITION_NONE;
         pstHdmiDev->pstHdmiHal->HAL_HDMI_HardwareInit(pstHdmiDev->pstHdmiHal);
    }
    else
#endif

    {
        if (bUser)
        {
            pstHdmiDev->enTransitionState = (pstHdmiDev->u32KernelCnt > 0)? HDMI_TRANSITION_MCE_APP : HDMI_TRANSITION_BOOT_APP;
        }
        else
        {
            pstHdmiDev->enTransitionState = HDMI_TRANSITION_BOOT_MCE;
        }

        pstHdmiDev->pstHdmiHal->HAL_HDMI_HotPlugStatusGet(pstHdmiDev->pstHdmiHal, &bHotPLug);
        if (bHotPLug)
        {
            HdmiEventCallback(pstHdmiDev, HDMI_EVENT_HOTPLUG);
        }
        else
        {
            HdmiEventCallback(pstHdmiDev, HDMI_EVENT_HOTUNPLUG);
        }

    }


    /* only debug,delete it in future*/
    //pstHdmiDev->pstHdmiHal->HAL_HDMI_HardwareInit(pstHdmiDev->pstHdmiHal);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_IrqEnableSet(pstHdmiDev->pstHdmiHal, HDMI_INT_TYPE_HPD_RSEN, HI_TRUE);
    HDMI_INFO("TransitionState: %d\n", pstHdmiDev->enTransitionState);

    HDMI_INFO("<<< DRV_HDMI_Open out...\n");

    return HI_SUCCESS;
}

static HI_S32 HdmiRelease(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_INFO(">>> HdmiRelease in...\n");
    CHECK_POINTER(pstHdmiDev);

    if (pstHdmiDev->enRunState & HDMI_RUN_STATE_START)
    {
        DRV_HDMI_Stop(pstHdmiDev);
    }

    if (pstHdmiDev->u32UserCnt == 0)
    {
        DRV_HDMI_EventDeInit(&pstHdmiDev->stEventInfo);

        if (pstHdmiDev->u32KernelCnt == 0)
        {
            if (pstHdmiDev->stThreadInfo.kThreadTimer)
            {
                HDMI_INFO("Stop hdmi kernel thread timer\n");
                kthread_stop(pstHdmiDev->stThreadInfo.kThreadTimer);
                pstHdmiDev->stThreadInfo.kThreadTimer = HI_NULL;
                DRV_HDMI_ThreadStateSet(pstHdmiDev, HDMI_THREAD_STATE_STOP);
            }

            if (pstHdmiDev->pstHdmiHal)
            {
                HAL_HDMI_Close(pstHdmiDev->pstHdmiHal);
            }
            memset(pstHdmiDev, 0, sizeof(HDMI_DEVICE_S));
        }
        else
        {
        //#if defined(CONFIG_HDMI_STB_SDK)
         	HDMI_MUTEX_UNLOCK(s_HdmiMutex);
        	HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
            pstHdmiDev->pKCallback = HI_DRV_HDMI_KernelEventCallback;
			HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
            HDMI_MUTEX_LOCK(s_HdmiMutex);
        //#endif
        }
    }
    HDMI_INFO("<<< HdmiRelease out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_Close(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bUser)
{
    HI_S32 s32Ret = HI_FAILURE;

    HDMI_INFO(">>> DRV_HDMI_Close in...\n");
    CHECK_POINTER(pstHdmiDev);
    CHECK_HDMI_OPEN(pstHdmiDev->enRunState);

    HDMI_INFO("bUser: %d\n", bUser);
    HDMI_INFO("u32UserCnt: %d, u32KernelCnt: %d\n", pstHdmiDev->u32UserCnt, pstHdmiDev->u32KernelCnt);

    if (bUser && pstHdmiDev->u32UserCnt > 0)
    {
        HDMI_INFO("Delete Event Queue for process:%d \n",get_current()->tgid);
        s32Ret = DRV_HDMI_EventPoolFree(&pstHdmiDev->stEventInfo, get_current()->tgid);
        if (s32Ret != HI_SUCCESS)
        {
            HDMI_ERR("DRV_HDMI_EventQueueAdd fail\n");
            return HI_FAILURE;
        }
        pstHdmiDev->u32UserCnt--;
        if (pstHdmiDev->u32UserCnt == 0 && pstHdmiDev->u32KernelCnt > 0)
        {
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
        	HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
            pstHdmiDev->pKCallback = HI_DRV_HDMI_KernelEventCallback;
			HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
            HDMI_MUTEX_LOCK(s_HdmiMutex);
        }
		else
		{
			 HDMI_MUTEX_UNLOCK(s_HdmiMutex);
			 HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
			 pstHdmiDev->pKCallback = HI_NULL;
			 HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
             HDMI_MUTEX_LOCK(s_HdmiMutex);

		}
    }
    else if (!bUser && pstHdmiDev->u32KernelCnt > 0)
    {
        pstHdmiDev->u32KernelCnt--;
    }

    if (pstHdmiDev->u32UserCnt == 0 && pstHdmiDev->u32KernelCnt == 0)
    {
#ifdef HDMI_TEE_SUPPORT
        DRV_HDMI_TeeClose(pstHdmiDev->u32HdmiDevId);
#endif
        HdmiRelease(pstHdmiDev);
        pstHdmiDev->enRunState = HDMI_RUN_STATE_CLOSE;
    }

    HDMI_INFO("<<< DRV_HDMI_Close out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_Start(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_APP_ATTR_S* pstAppAttr;
    HI_BOOL bAudioEnable = HI_FALSE;

    HDMI_INFO(">>> DRV_HDMI_Start in...\n");
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_HDMI_OPEN(pstHdmiDev->enRunState);

    pstAppAttr = &pstHdmiDev->stAttr.stAppAttr;
    bAudioEnable = (pstHdmiDev->stAttr.stAppAttr.bEnableAudio) && (pstAppAttr->bEnableHdmi);

	/* 20160415, Fix SAMSUNG TV(UA55JU6400JXXZ) doesn't show at 10bit/12bit.
       This TV clear mute must after phy enable when the deepcolor is 10bit and 12bit. */
    pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    HdmiPhyOutputEnable(pstHdmiDev, HI_TRUE);

    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_FALSE);
    DRV_HDMI_AudioPathEnable(pstHdmiDev, bAudioEnable);

    pstHdmiDev->stHdcpInfo.bHdcpIsOn = pstHdmiDev->stAttr.stAppAttr.bHDCPEnable;

#ifdef HDMI_HDCP_SUPPORT
    if (HI_SUCCESS != HdcpStrategy(pstHdmiDev))
    {
        DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
		DRV_HDMI_AudioOutputEnableSet(pstHdmiDev, HI_FALSE);
        HdmiEventCallback(pstHdmiDev, HDMI_EVENT_HDCP_FAIL);
    }
#endif
    /* stop black frame here if hdcp is disable, otherwise stop it when the hdcp auth success. */
    if(pstHdmiDev->stAttr.stAppAttr.bHDCPEnable == HI_FALSE)
    {
        DRV_HDMI_BlackDataSet(pstHdmiDev, HI_FALSE);
    }

    pstHdmiDev->enRunState &= ~HDMI_RUN_STATE_STOP;
    pstHdmiDev->enRunState |= HDMI_RUN_STATE_START;

    HDMI_INFO("<<< DRV_HDMI_Start out...\n");

    return HI_SUCCESS;
}

static HI_VOID HdmiDebugDelay(HDMI_DEVICE_S* pstHdmiDev, HI_U8 *u8Info)
{
    CHECK_POINTER_NO_RET(u8Info);
	CHECK_POINTER_NO_RET(pstHdmiDev);

    msleep(pstHdmiDev->stDebugContext.u32StopDelay);
    if(pstHdmiDev->stDebugContext.u32StopDelay > 0)
    {
        HDMI_INFO("%s, delay %dms...\n", u8Info, pstHdmiDev->stDebugContext.u32StopDelay);
    }
}

HI_S32 DRV_HDMI_Stop(HDMI_DEVICE_S* pstHdmiDev)
{
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S stParam;
#endif

    HDMI_INFO(">>> DRV_HDMI_Stop in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    HdmiDebugDelay(pstHdmiDev, "DRV_HDMI_Stop debug start");

    if (!(pstHdmiDev->enRunState & HDMI_RUN_STATE_START) ||
        (pstHdmiDev->enRunState & HDMI_RUN_STATE_STOP))
    {
        return HI_SUCCESS;
    }

    HdmiDebugDelay(pstHdmiDev, "DRV_HDMI_Stop audio path disable");
    DRV_HDMI_AudioPathEnable(pstHdmiDev, HI_FALSE);
    HdmiDebugDelay(pstHdmiDev, "DRV_HDMI_Stop balck data enable");
    DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
    HdmiDebugDelay(pstHdmiDev, "DRV_HDMI_Stop avmute enable");
    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_TRUE);

#ifdef HDMI_HDCP_SUPPORT
    pstHdmiDev->stHdcpInfo.bHdcpIsOn = HI_FALSE;
    HdcpStrategy(pstHdmiDev);
    if(HI_TRUE != pstHdmiDev->stAttr.stAppAttr.bAuthMode)
    {
        msleep(200);
    }
#endif
#ifdef HDMI_TEE_SUPPORT
    HDMI_MEMSET(&stParam, 0, sizeof(HDMI_TEE_PARAM_S));
    stParam.enTeeCmd = HI_HDMI_STOP;
    DRV_HDMI_TeeCmdSend(pstHdmiDev->u32HdmiDevId, &stParam);
#endif

    HdmiDebugDelay(pstHdmiDev, "DRV_HDMI_Stop phy disable");
    HdmiPhyOutputEnable(pstHdmiDev, HI_FALSE);

    pstHdmiDev->enRunState &= ~HDMI_RUN_STATE_START;
    pstHdmiDev->enRunState |= HDMI_RUN_STATE_STOP;

    HDMI_INFO("<<< DRV_HDMI_Stop out...\n");

    return HI_SUCCESS;
}


HI_VOID DRV_HDMI_AVMuteSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bAvMute)
{
    HDMI_INFO(">>> DRV_HDMI_AVMuteSet in...\n");
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstHdmiDev->pstHdmiHal);

    HDMI_INFO("bAvMute: %d\n", bAvMute);
    if(HI_TRUE != pstHdmiDev->stAttr.stAppAttr.bAuthMode)
    {
        /* fix compatibility problem*/
        if (!bAvMute)
        {
            HI_U32 u32Delay = 0;
            FormatDelayGet(pstHdmiDev, &u32Delay);
            msleep(u32Delay);
        }
    }

    pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, bAvMute);

    if(HI_TRUE != pstHdmiDev->stAttr.stAppAttr.bAuthMode)
    {
        /* fix compatibility problem*/
        if (bAvMute)
        {
            HI_U32 u32Delay = 0;
            AvMuteDelayGet(pstHdmiDev, &u32Delay);
            msleep(u32Delay);
        }
    }

    HDMI_INFO("<<< DRV_HDMI_AVMuteSet out...\n");

    return;
}

HI_VOID DRV_HDMI_BlackDataSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HDMI_BLACK_FRAME_INFO_S stBlackInfo;
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstHdmiDev->pstHdmiHal);

    stBlackInfo.bBlacEnable = bEnable;
    stBlackInfo.inBitDepth = pstHdmiDev->stAttr.stVOAttr.enInBitDepth;
    stBlackInfo.inColorSpace = pstHdmiDev->stAttr.stVOAttr.enInColorSpace;
    stBlackInfo.inQuantization = (stBlackInfo.inColorSpace == HDMI_COLORSPACE_RGB) ? \
        pstHdmiDev->stAttr.stVOAttr.enRGBQuantization : pstHdmiDev->stAttr.stVOAttr.enYCCQuantization;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_BlackDataSet(pstHdmiDev->pstHdmiHal, &stBlackInfo);

    return;
}


HI_S32 DRV_HDMI_EmiStatusGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_EMI_STATUS_S *pstEmiStatus)
{
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstEmiStatus);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_EmiStatusGet(pstHdmiDev->pstHdmiHal, pstEmiStatus);
    return HI_SUCCESS;
}

HI_S32 IsEmiParamChange(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstHwVOAttr, HDMI_APP_ATTR_S* pstHwAPPAttr)
{
    HDMI_VO_ATTR_S          *pstSwVOAttr;
    HDMI_APP_ATTR_S         *pstSwAPPAttr;
    CHECK_POINTER(pstHwVOAttr);
    CHECK_POINTER(pstHwAPPAttr);
    CHECK_POINTER(pstHdmiDev);

    pstSwVOAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstSwAPPAttr = &pstHdmiDev->stAttr.stAppAttr;

    CHECK_POINTER(pstSwVOAttr);
    CHECK_POINTER(pstSwAPPAttr);

    if (pstHwVOAttr->enVideoTiming != pstSwVOAttr->enVideoTiming)
    {
        return HI_TRUE;
    }
    if (pstHwAPPAttr->enDeepColorMode != pstSwAPPAttr->enDeepColorMode)
    {
        return HI_TRUE;
    }
    if (pstHwAPPAttr->enOutColorSpace != pstSwAPPAttr->enOutColorSpace)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 DRV_HDMI_EmiSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstHwVOAttr, HDMI_APP_ATTR_S* pstHwAPPAttr)
{
    HDMI_EMI_CONFIG_S       stEmiConfig;
    HDMI_VO_ATTR_S          *pstSwVOAttr;
    HDMI_APP_ATTR_S         *pstSwAPPAttr;
    HDMI_EMI_STATUS_S       stEmiStatus;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstHwVOAttr);
    CHECK_POINTER(pstHwAPPAttr);

    pstSwVOAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstSwAPPAttr = &pstHdmiDev->stAttr.stAppAttr;

    CHECK_POINTER(pstSwVOAttr);
    CHECK_POINTER(pstSwAPPAttr);

    HDMI_INFO(">>> DRV_HDMI_EmiSet in...\n");

    HDMI_MEMSET(&stEmiStatus, 0, sizeof(HDMI_EMI_STATUS_S));
    DRV_HDMI_EmiStatusGet(pstHdmiDev, &stEmiStatus);

    if(!IsEmiParamChange(pstHdmiDev, pstHwVOAttr, pstHwAPPAttr)
       && pstHdmiDev->bEmiEnable == stEmiStatus.bSWEmiEnable)
    {
        HDMI_INFO("Emi params are not change.\n");
        return HI_SUCCESS;
    }

	stEmiConfig.bVesaFmt = (pstSwVOAttr->enVideoTiming == HDMI_VIDEO_TIMING_UNKNOWN) ? HI_TRUE : HI_FALSE;
    if(pstSwVOAttr->enVideoTiming != HDMI_VIDEO_TIMING_UNKNOWN)
    {
		stEmiConfig.bEmiEnable = pstHdmiDev->bEmiEnable;
    }
    else
    {
		/* if the timing is vesa, disable emi. */
		stEmiConfig.bEmiEnable = HI_FALSE;
    }
    stEmiConfig.bDebugEnable    = stEmiStatus.bDebugEnable;
    stEmiConfig.u32TmdsClk      = pstSwVOAttr->u32HdmiAdaptPixClk;
    stEmiConfig.enDeepColor     = pstSwAPPAttr->enDeepColorMode;
    stEmiConfig.enOutColorSpace = pstSwAPPAttr->enOutColorSpace;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_EmiSet(pstHdmiDev->pstHdmiHal, &stEmiConfig);

    HDMI_INFO("<<< DRV_HDMI_EmiSet out...\n");
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_DeepColorSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_DEEP_COLOR_E enDeepColor)
{
    HI_S32 s32Ret = HI_FAILURE;
    HDMI_ATTR_S stAttr ;

    HDMI_INFO(">>> DRV_HDMI_DeepColorSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    HDMI_INFO("enDeepColor: %d\n", enDeepColor);

    s32Ret = DRV_HDMI_AttrGet(pstHdmiDev, &stAttr);

#ifdef HDMI_HDR_SUPPORT
// if the current hdmi mode is HDR or Dolby, DeepColorSet is invalid.
    if(stAttr.stHdrAttr.enHdrMode == HDMI_HDR_MODE_DOLBY_NORMAL ||
       stAttr.stHdrAttr.enHdrMode == HDMI_HDR_MODE_DOLBY_TUNNELING)
    {
        HDMI_WARN("don't change deepcolor when dolby mode is: %d \n", stAttr.stHdrAttr.enHdrMode);
        return HI_FAILURE;
    }
#endif

    stAttr.stAppAttr.enDeepColorMode = enDeepColor;
    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_TRUE);
    s32Ret = DRV_HDMI_AttrSet(pstHdmiDev, &stAttr);
    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_FALSE);

    HDMI_INFO("<<< DRV_HDMI_DeepColorSet out...\n");

    return s32Ret;
}

HI_S32 DRV_HDMI_DeepColorGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_DEEP_COLOR_E* enDeepColor)
{
    HDMI_APP_ATTR_S* pstAppAttr = HI_NULL;

    HDMI_INFO(">>> DRV_HDMI_DeepColorGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(enDeepColor);

    pstAppAttr =  &pstHdmiDev->stAttr.stAppAttr;
    *enDeepColor = pstAppAttr->enDeepColorMode;

    HDMI_INFO("enDeepColor: %d\n", *enDeepColor);
    HDMI_INFO("<<< DRV_HDMI_DeepColorGet out...\n");

    return HI_SUCCESS;
}

static HI_S32 CheckAudioAttr(HDMI_AO_ATTR_S *pstAOAttr)
{
    CHECK_POINTER(pstAOAttr);

    if (HDMI_AUDIO_INTERFACE__I2S != pstAOAttr->enSoundIntf &&
        HDMI_AUDIO_INTERFACE__SPDIF != pstAOAttr->enSoundIntf &&
        HDMI_AUDIO_INTERFACE__HBRA != pstAOAttr->enSoundIntf)
    {
        HDMI_ERR("The Audio interface(%d) is invalid\n",pstAOAttr->enSoundIntf);
        return HI_FAILURE;
    }

    if (pstAOAttr->enChanels < HDMI_AUDIO_FORMAT_2CH ||
        pstAOAttr->enChanels > HDMI_AUDIO_FORMAT_8CH)
    {
        HDMI_ERR("The Audio chanel number(%d) is invalid\n",pstAOAttr->enChanels);
        return HI_FAILURE;
    }

    switch (pstAOAttr->enSampleFs)
    {
        case HDMI_SAMPLE_RATE_32K:
        case HDMI_SAMPLE_RATE_44K:
        case HDMI_SAMPLE_RATE_48K:
        case HDMI_SAMPLE_RATE_88K:
        case HDMI_SAMPLE_RATE_96K:
        case HDMI_SAMPLE_RATE_176K:
        case HDMI_SAMPLE_RATE_192K:
        case HDMI_SAMPLE_RATE_768K:
            break;
        default:
            HDMI_ERR("The input Audio Frequency(%d) is invalid\n",pstAOAttr->enSampleFs);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_AOAttrSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_AO_ATTR_S *pstAOAttr)
{
    HDMI_APP_ATTR_S*         pstAppAttr;
    HDMI_AO_ATTR_S*          pstAudioAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_ATTR_S              stHwAttr;
    HDMI_AUDIO_CONFIG_S      stAudioCfg;

    HDMI_INFO(">>> DRV_HDMI_AOAttrSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAOAttr);

    if (HI_SUCCESS != CheckAudioAttr(pstAOAttr))
    {
        HDMI_ERR("HDMI_CheckAudioAttr fail\n");
        return HI_FAILURE;
    }
    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;
    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstAudioAttr = &pstHdmiDev->stAttr.stAOAttr;
    memcpy(pstAudioAttr, pstAOAttr, sizeof(HDMI_AO_ATTR_S));

    memset(&stHwAttr, 0, sizeof(HDMI_ATTR_S));
    HdmiAttrConstruct(pstHdmiDev, &stHwAttr);

    HDMI_INFO("bDownSample: %d\n", pstAudioAttr->bDownSample);
    HDMI_INFO("enSampleFs: %d\n", pstAudioAttr->enSampleFs);
    HDMI_INFO("enChanels: %d\n", pstAudioAttr->enChanels);
    HDMI_INFO("enSoundIntf: %d\n", pstAudioAttr->enSoundIntf);
    HDMI_INFO("enSampleDepth: %d\n", pstAudioAttr->enSampleDepth);
    HDMI_INFO("enAudioCode: %d\n", pstAudioAttr->enAudioCode);

    stAudioCfg.bDownSample    = pstAOAttr->bDownSample;
    stAudioCfg.enLayout       = pstAOAttr->enChanels;
    stAudioCfg.enSampleDepth  = pstAOAttr->enSampleDepth;
    stAudioCfg.enSampleFs     = pstAOAttr->enSampleFs;
    stAudioCfg.enSoundIntf    = pstAOAttr->enSoundIntf;
	stAudioCfg.bEnableAudio   = (pstHdmiDev->stAttr.stAppAttr.bEnableAudio) && (pstAppAttr->bEnableHdmi);
    stAudioCfg.u32TmdsClk     = pstVideoAttr->u32HdmiAdaptPixClk;
    stAudioCfg.u32PixelRepeat = pstVideoAttr->u32PixelRepeat;

	pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathSet(pstHdmiDev->pstHdmiHal, &stAudioCfg);
    DRV_HDMI_AudioInfoFrameSend(&pstHdmiDev->stInfoFrame, (pstAppAttr->bEnableAudio && pstAppAttr->bEnableAudInfoFrame));
    pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet(pstHdmiDev->pstHdmiHal, stAudioCfg.bEnableAudio);

    HDMI_INFO("<<< DRV_HDMI_AOAttrSet out...\n");

    return HI_SUCCESS;
}

HI_VOID DRV_HDMI_AudioPathEnable(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER_NO_RET(pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathEnableSet(pstHdmiDev->pstHdmiHal, bEnable);

    return;
}

HI_S32 DRV_HDMI_AOAttrGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_AO_ATTR_S* pstAOAttr)
{
    HDMI_INFO(">>> DRV_HDMI_AOAttrGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAOAttr);

    memcpy(pstAOAttr, &pstHdmiDev->stAttr.stAOAttr, sizeof(HDMI_AO_ATTR_S));

    HDMI_INFO("<<< DRV_HDMI_AOAttrGet out...\n");

    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_AudioOutputEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HI_BOOL bMute = bEnable ? HI_FALSE : HI_TRUE;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioMuteSet(pstHdmiDev->pstHdmiHal, bMute);

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_ThreadStateSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_THREAD_STATE_E enState)
{
    HDMI_INFO(">>> DRV_HDMI_ThreadStateSet in...\n");

    CHECK_POINTER(pstHdmiDev);

    HDMI_INFO("enState: %d\n", enState);

    pstHdmiDev->stThreadInfo.kThreadTimerSate = enState;

    HDMI_INFO("<<< DRV_HDMI_ThreadStateSet out...\n");

    return HI_SUCCESS;
}

static HI_S32 CheckVideoAttr(HDMI_VO_ATTR_S* pstVOAttr)
{
    CHECK_POINTER(pstVOAttr);

    if (pstVOAttr->enVideoTiming >= HDMI_VIDEO_TIMING_BUTT)
    {
        HDMI_ERR("Hdmi video timing(%d) is wrong\n", pstVOAttr->enVideoTiming);
        return HI_FAILURE;
    }

    if (pstVOAttr->enInColorSpace >= HDMI_COLORSPACE_BUTT)
    {
        HDMI_ERR("Hdmi video InColorSpace(%d) is wrong\n", pstVOAttr->enInColorSpace);
        return HI_FAILURE;
    }

    if (pstVOAttr->enColorimetry != HDMI_COLORIMETRY__ITU_601
        && pstVOAttr->enColorimetry != HDMI_COLORIMETRY__ITU_709
        && pstVOAttr->enColorimetry != HDMI_COLORIMETRY__EXTENDED)
    {
        HDMI_ERR("Hdmi video Colorimetry(%d) is wrong\n", pstVOAttr->enColorimetry);
        return HI_FAILURE;
    }

    if (pstVOAttr->enRGBQuantization > HDMI_QUANTIZATION_RANGE_FULL)
    {
        HDMI_ERR("Hdmi video RGBQuantization(%d) is wrong\n", pstVOAttr->enRGBQuantization);
        return HI_FAILURE;
    }

    if (pstVOAttr->enPictureAspect != HDMI_PICTURE_ASPECT_4_3
        && pstVOAttr->enPictureAspect != HDMI_PICTURE_ASPECT_16_9
        && pstVOAttr->enPictureAspect != HDMI_PICTURE_ASPECT_64_27
        && pstVOAttr->enPictureAspect != HDMI_PICTURE_ASPECT_256_135)
    {
        HDMI_ERR("Hdmi video PictureAspect(%d) is wrong\n", pstVOAttr->enPictureAspect);
        return HI_FAILURE;
    }

    if (pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_8 &&
        pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_10 &&
        pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_12)
    {
        HDMI_ERR("Hdmi video InBitDepth(%d) is wrong\n", pstVOAttr->enInBitDepth);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID HdmiAttrToVidcfg(HDMI_VO_ATTR_S* pstVOAttr, HDMI_APP_ATTR_S* pstAppAttr, HDMI_VIDEO_CONFIG_S* pstVideoCfg)
{
    CHECK_POINTER_NO_RET(pstVOAttr);
    CHECK_POINTER_NO_RET(pstAppAttr);
    CHECK_POINTER_NO_RET(pstVideoCfg);

    pstVideoCfg->u32PixelClk     = (pstAppAttr->enOutColorSpace == HDMI_COLORSPACE_YCbCr420)  ?
                                    (pstVOAttr->u32ClkFs >> 1) : pstVOAttr->u32ClkFs;
    pstVideoCfg->u32TmdsClk      = pstVOAttr->u32HdmiAdaptPixClk;
    pstVideoCfg->enInBitDepth    = pstVOAttr->enInBitDepth;
    pstVideoCfg->enQuantization  = pstVOAttr->enRGBQuantization;
    pstVideoCfg->enInColorSpace  = pstVOAttr->enInColorSpace;
    pstVideoCfg->enDeepColor     = pstAppAttr->enDeepColorMode;
    pstVideoCfg->enOutColorSpace = pstAppAttr->enOutColorSpace;
    pstVideoCfg->bVSyncPol       = pstVOAttr->bVSyncPol;
    pstVideoCfg->bHSyncPol       = pstVOAttr->bHSyncPol;
    pstVideoCfg->bDEPol          = pstVOAttr->bDEPol;
    if(HDMI_COLORSPACE_RGB == pstVOAttr->enInColorSpace)
    {
        pstVideoCfg->eOutCscQuantization = pstVOAttr->enRGBQuantization;
    }
    else
    {
        switch(pstVOAttr->enYCCQuantization)
        {
            case HDMI_YCC_QUANTIZATION_RANGE_FULL:
                pstVideoCfg->eOutCscQuantization = HDMI_QUANTIZATION_RANGE_FULL;
                break;
            default:
                pstVideoCfg->eOutCscQuantization = HDMI_QUANTIZATION_RANGE_LIMITED;
                break;
        }
    }

    switch (pstVOAttr->enColorimetry)
    {
        case HDMI_COLORIMETRY__ITU_601:
            pstVideoCfg->enConvStd = HDMI_CONV_STD_BT_601;
            break;
        case HDMI_COLORIMETRY__ITU_709:
            pstVideoCfg->enConvStd = HDMI_CONV_STD_BT_709;
            break;
        case HDMI_COLORIMETRY__EXTENDED:
            if (pstVOAttr->enExtendedColorimetry == HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous)
            {
                pstVideoCfg->enConvStd = HDMI_CONV_STD_BT_2020_non_const_luminous;
            }
            else if (pstVOAttr->enExtendedColorimetry == HDMI_EXTENDED_COLORIMETRY_2020_const_luminous)
            {
                pstVideoCfg->enConvStd = HDMI_CONV_STD_BT_2020_const_luminous;
            }
            break;
        default:
            pstVideoCfg->enConvStd = HDMI_CONV_STD_BT_709;
            break;
    }
    return;
}

static HI_S32 HdmiCscSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr)
{
    HDMI_VIDEO_CONFIG_S stVideoCfg = {0};
    HDMI_APP_ATTR_S*    pstAppAttr = HI_NULL;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstVOAttr);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstAppAttr = &pstHdmiDev->stAttr.stAppAttr;

    HdmiAttrToVidcfg(pstVOAttr, pstAppAttr, &stVideoCfg);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_CscParamSet(pstHdmiDev->pstHdmiHal, &stVideoCfg);

    return HI_SUCCESS;
}

static HI_S32 HdmiVideoPathSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr)
{
    HDMI_VIDEO_CONFIG_S stVideoCfg = {0};
    HDMI_APP_ATTR_S*    pstAppAttr = HI_NULL;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstVOAttr);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstAppAttr 				= &pstHdmiDev->stAttr.stAppAttr;
    stVideoCfg.bEmiEnable   = pstHdmiDev->bEmiEnable;

    HdmiAttrToVidcfg(pstVOAttr, pstAppAttr, &stVideoCfg);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoPathSet(pstHdmiDev->pstHdmiHal, &stVideoCfg);
    HdmiCscSet(pstHdmiDev, pstVOAttr);

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_VideoOutputEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HI_BOOL bMute = bEnable ? HI_FALSE : HI_TRUE;

    HDMI_INFO(">>> DRV_HDMI_VideoOutputEnableSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    HDMI_INFO("bEnable: %d\n", bEnable);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoMuteSet(pstHdmiDev->pstHdmiHal, bMute);

    HDMI_INFO("<<< DRV_HDMI_VideoOutputEnableSet out...\n");

    return HI_SUCCESS;
}

static HI_BOOL APPAttrIsChanged(HDMI_APP_ATTR_S* pstHwAPPAttr, HDMI_APP_ATTR_S* pstSwAPPAttr)
{
    if (pstHwAPPAttr == HI_NULL || pstSwAPPAttr == HI_NULL)
    {
        HDMI_ERR("The input pointer is null\n");
        return HI_FALSE;
    }

    if (pstHwAPPAttr->bEnableHdmi != pstSwAPPAttr->bEnableHdmi)
    {
        HDMI_INFO("tmds mode change\n");
        return HI_TRUE;
    }
#if 0
    if (pstSwAPPAttr->bYUV420Enable)
    {
        if (pstHwAPPAttr->enOutColorSpace != HDMI_COLORSPACE_YCbCr420)
        {
            HDMI_INFO("out colorSpace change, out colorSpace switch to YUV420\n");
            return HI_TRUE;
        }

    }
    else
#endif
    if (pstHwAPPAttr->enOutColorSpace != pstSwAPPAttr->enOutColorSpace)
    {
        HDMI_INFO("out colorSpace change,old(%d)->new(%d)\n",pstHwAPPAttr->enOutColorSpace, pstSwAPPAttr->enOutColorSpace);
        return HI_TRUE;
    }

    if (pstHwAPPAttr->enDeepColorMode != pstSwAPPAttr->enDeepColorMode)
    {
        HDMI_INFO("deepcolor mode change,old(%d)->new(%d)\n",pstHwAPPAttr->enDeepColorMode, pstSwAPPAttr->enDeepColorMode);
        return HI_TRUE;
    }

    if (pstHwAPPAttr->bxvYCCMode != pstSwAPPAttr->bxvYCCMode)
    {
        HDMI_INFO("bxvYCCMode mode change,old(%d)->new(%d)\n",pstHwAPPAttr->bxvYCCMode, pstSwAPPAttr->bxvYCCMode);
        return HI_TRUE;
    }

    return HI_FALSE;
}

static HI_BOOL VOAttrIsChanged(HDMI_VO_ATTR_S* pstHwVOAttr, HDMI_VO_ATTR_S* pstSwVOAttr)
{
    if (pstHwVOAttr == HI_NULL || pstSwVOAttr == HI_NULL)
    {
        HDMI_ERR("The input pointer is null\n");
        return HI_FALSE;
    }

    if (pstHwVOAttr->enVideoTiming != pstSwVOAttr->enVideoTiming)
    {
        HDMI_INFO("video timing change,old(%d)->new(%d)\n",pstHwVOAttr->enVideoTiming,pstSwVOAttr->enVideoTiming);
        return HI_TRUE;
    }
    else
    {
        /* l00232354: if timing is change, the sync param need according to user config, or not,
           should not change the sync param.
        */
        pstSwVOAttr->bVSyncPol = pstHwVOAttr->bVSyncPol;
        pstSwVOAttr->bHSyncPol = pstHwVOAttr->bHSyncPol;
        pstSwVOAttr->bDEPol = pstHwVOAttr->bDEPol;
    }

    if (pstHwVOAttr->enInColorSpace != pstSwVOAttr->enInColorSpace)
    {
        HDMI_INFO("input colorspace change,old(%d)->new(%d)\n",pstHwVOAttr->enInColorSpace,pstSwVOAttr->enInColorSpace);
        return HI_TRUE;
    }

    if (pstHwVOAttr->enStereoMode != pstSwVOAttr->enStereoMode)
    {
        HDMI_INFO("3d mode change,old(%d)->new(%d)\n",pstHwVOAttr->enStereoMode, pstSwVOAttr->enStereoMode);
        return HI_TRUE;
    }

    if (pstHwVOAttr->u32PixelRepeat != pstSwVOAttr->u32PixelRepeat)
    {
        HDMI_INFO("pixel repeation change,old(%d)->new(%d)\n",pstHwVOAttr->u32PixelRepeat, pstSwVOAttr->u32PixelRepeat);
        return HI_TRUE;
    }

    if (pstHwVOAttr->enColorimetry != pstSwVOAttr->enColorimetry)
    {
        HDMI_INFO("colorimetry change,old(%d)->new(%d)\n",pstHwVOAttr->enColorimetry, pstSwVOAttr->enColorimetry);
        return HI_TRUE;
    }
    if(pstHwVOAttr->enExtendedColorimetry != pstSwVOAttr->enExtendedColorimetry)
    {
        HDMI_INFO("ExtendedColorimetry change,old(%d)->new(%d)\n",pstHwVOAttr->enExtendedColorimetry, pstSwVOAttr->enExtendedColorimetry);
        return HI_TRUE;
    }
    if (pstHwVOAttr->enRGBQuantization != pstSwVOAttr->enRGBQuantization)
    {
        HDMI_INFO("RGB quantization change,old(%d)->new(%d)\n",pstHwVOAttr->enRGBQuantization, pstSwVOAttr->enRGBQuantization);
        return HI_TRUE;
    }

    if (pstHwVOAttr->enYCCQuantization != pstSwVOAttr->enYCCQuantization)
    {
        HDMI_INFO("YCC quantization change,old(%d)->new(%d)\n",pstHwVOAttr->enYCCQuantization, pstSwVOAttr->enYCCQuantization);
        return HI_TRUE;
    }

    if (pstHwVOAttr->enPictureAspect != pstSwVOAttr->enPictureAspect)
    {
        HDMI_INFO("picture aspect change,old(%d)->new(%d)\n",pstHwVOAttr->enPictureAspect, pstSwVOAttr->enPictureAspect);
        return HI_TRUE;
    }

    return HI_FALSE;
}


static HI_S32 CheckAppAttr(HDMI_APP_ATTR_S* pstAppAttr)
{
    HI_S32  s32Ret = HI_SUCCESS;

    CHECK_POINTER(pstAppAttr);

    if (pstAppAttr->enOutColorSpace != HDMI_COLORSPACE_RGB
        && pstAppAttr->enOutColorSpace != HDMI_COLORSPACE_YCbCr422
        && pstAppAttr->enOutColorSpace != HDMI_COLORSPACE_YCbCr444
        && pstAppAttr->enOutColorSpace != HDMI_COLORSPACE_YCbCr420)
    {
        HDMI_ERR("Hdmi OutColorSpace=%d is wrong\n", pstAppAttr->enOutColorSpace);
        s32Ret |=  HI_FAILURE;
    }

    if (pstAppAttr->enDeepColorMode != HDMI_DEEP_COLOR_24BIT
        && pstAppAttr->enDeepColorMode != HDMI_DEEP_COLOR_30BIT
        && pstAppAttr->enDeepColorMode != HDMI_DEEP_COLOR_36BIT
        && pstAppAttr->enDeepColorMode != HDMI_DEEP_COLOR_48BIT
        && pstAppAttr->enDeepColorMode != HDMI_DEEP_COLOR_OFF)
    {
        HDMI_ERR("Hdmi DeepClrMode=%d is wrong\n", pstAppAttr->enDeepColorMode);
        s32Ret |=  HI_FAILURE;
    }

	if(   pstAppAttr->enHDCPMode != HDMI_HDCP_MODE_1_4
       && pstAppAttr->enHDCPMode != HDMI_HDCP_MODE_2_2
       && pstAppAttr->enHDCPMode != HDMI_HDCP_MODE_AUTO)
    {
        HDMI_ERR("Hdmi HdcpMode=%d is wrong\n", pstAppAttr->enHDCPMode);
        s32Ret |=  HI_FAILURE;
    }

    return s32Ret;

}

#ifdef HDMI_HDR_SUPPORT

static HI_VOID HdrModeCheck(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S *pstVOAttr, HDMI_APP_ATTR_S *pstAppAttr)
{
    HDMI_HDR_ATTR_S         *pstHdrAttr = HI_NULL;

    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstVOAttr);
    CHECK_POINTER_NO_RET(pstAppAttr);

    pstHdrAttr = &(pstHdmiDev->stAttr.stHdrAttr);

    switch(pstHdrAttr->enUserHdrMode)
    {
        case HDMI_HDR_USERMODE_DOLBY:
            pstVOAttr->enYCCQuantization            = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            pstAppAttr->enOutColorSpace             = HDMI_COLORSPACE_YCbCr444;
            pstVOAttr->u32HdmiAdaptPixClk           = pstVOAttr->u32ClkFs;
            pstAppAttr->enDeepColorMode             = HDMI_DEEP_COLOR_24BIT;
            pstVOAttr->enRGBQuantization            = HDMI_QUANTIZATION_RANGE_DEFAULT;
            break;
        case HDMI_HDR_USERMODE_HDR10:
            if(HDMI_HDR_MODE_CEA_861_3 == pstHdrAttr->enHdrMode)
            {
                if(pstHdrAttr->enColorimetry > HDMI_HDR_COLORIMETRY_EXTENDED)
                {
                    pstVOAttr->enColorimetry = HDMI_COLORIMETRY__EXTENDED;
                    pstVOAttr->enExtendedColorimetry = pstHdrAttr->enColorimetry - HDMI_HDR_EXTENDED_COLORIMETRY_XV_YCC_601;
                }
                else
                {
                    HDRCOLORIMETRY_2_COLORIMETRY(pstHdrAttr->enColorimetry, pstVOAttr->enColorimetry);
                    pstVOAttr->enExtendedColorimetry = HDMI_EXTENDED_COLORIMETRY_BUTT;
                }
                pstAppAttr->bxvYCCMode = HI_FALSE;
            }
            break;
        default:
            break;
    }
    HDMI_INFO("user hdr mode:%d, acture hdr mode:%d \n", pstHdrAttr->enUserHdrMode, pstHdrAttr->enHdrMode);
    return ;
}
#endif

HI_S32 DRV_HDMI_VOAttrSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr)
{
    HDMI_APP_ATTR_S*         pstAppAttr;
    HDMI_ATTR_S              stHwAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_APP_ATTR_S          stTmpApp;
    //HDMI_TMDS_MODE_E         enTmdsMode;
    static HI_U32           u32DispFmt = 0xffffffff;
    HI_BOOL                 bEnableClrSpaceAdaptBack = HI_TRUE;
    HI_BOOL                 bEnableDeepClrAdaptBack  = HI_TRUE;

    HDMI_INFO(">>> DRV_HDMI_VOAttrSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstVOAttr);

    if (HI_SUCCESS != CheckVideoAttr(pstVOAttr))
    {
        HDMI_ERR("HDMI_CheckVideoAttr fail\n");
        return HI_FAILURE;
    }

    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;
    memcpy(pstVideoAttr, pstVOAttr, sizeof(HDMI_VO_ATTR_S));

    HDMI_INFO("u32ClkFs: %d\n", pstVideoAttr->u32ClkFs);
    HDMI_INFO("u32PixelRepeat: %d\n", pstVideoAttr->u32PixelRepeat);
    HDMI_INFO("enVideoTiming: %d\n", pstVideoAttr->enVideoTiming);
    HDMI_INFO("enStereoMode: %d\n", pstVideoAttr->enStereoMode);
    HDMI_INFO("enInColorSpace: %d\n", pstVideoAttr->enInColorSpace);
    HDMI_INFO("enColorimetry: %d\n", pstVideoAttr->enColorimetry);
    HDMI_INFO("enExtendedColorimetry: %d\n", pstVideoAttr->enExtendedColorimetry);
    HDMI_INFO("enRGBQuantization: %d\n", pstVideoAttr->enRGBQuantization);
    HDMI_INFO("enYCCQuantization: %d\n", pstVideoAttr->enYCCQuantization);
    HDMI_INFO("enPictureAspect: %d\n", pstVideoAttr->enPictureAspect);
    HDMI_INFO("enActiveAspect: %d\n", pstVideoAttr->enActiveAspect);
    HDMI_INFO("enPictureScaling: %d\n", pstVideoAttr->enPictureScaling);
    HDMI_INFO("enInBitDepth: %d\n", pstVideoAttr->enInBitDepth);
    HDMI_INFO("u32DispFmt: %d\n", pstVideoAttr->u32DispFmt);
    HDMI_INFO("u32ClkFs: %d\n", pstVideoAttr->u32ClkFs);
    HDMI_INFO("bVSyncPol: %d\n", pstVideoAttr->bVSyncPol);
    HDMI_INFO("bHSyncPol: %d\n", pstVideoAttr->bHSyncPol);
    HDMI_INFO("bDEPol: %d\n", pstVideoAttr->bDEPol);
    HDMI_INFO("enUserHdrMode: %d\n", pstHdmiDev->stAttr.stHdrAttr.enUserHdrMode);

    pstAppAttr =  &pstHdmiDev->stAttr.stAppAttr;

    bEnableClrSpaceAdaptBack = pstAppAttr->bEnableClrSpaceAdapt;
    bEnableDeepClrAdaptBack  = pstAppAttr->bEnableDeepClrAdapt;
    memcpy(&stTmpApp,pstAppAttr,sizeof(stTmpApp));
    /* when format is changed, force to enable  HdmiColorAndBitStrategy */
    if (u32DispFmt != pstVideoAttr->u32DispFmt)
    {
        stTmpApp.bEnableClrSpaceAdapt   = HI_TRUE;
        stTmpApp.bEnableDeepClrAdapt    = HI_TRUE;
        u32DispFmt                      = pstVideoAttr->u32DispFmt;
    }
    if (HI_SUCCESS != HdmiColorAndBitStrategy(pstHdmiDev,&stTmpApp))
    {
        HDMI_ERR(" HdmiColorAndBitStrategy fail\n");
        return HI_FAILURE;
    }
    memcpy(pstAppAttr,&stTmpApp,sizeof(stTmpApp));
    pstAppAttr->bEnableClrSpaceAdapt = bEnableClrSpaceAdaptBack;
    pstAppAttr->bEnableDeepClrAdapt  = bEnableDeepClrAdaptBack;

#ifdef HDMI_HDR_SUPPORT
    HdrModeCheck(pstHdmiDev, pstVideoAttr, pstAppAttr);
#endif

    if (HI_SUCCESS != HdmiModeStrategy(pstHdmiDev))
    {
        HDMI_ERR(" HdmiModeStrategy fail\n");
        return HI_FAILURE;
    }

    memset(&stHwAttr, 0, sizeof(HDMI_ATTR_S));
    HdmiAttrConstruct(pstHdmiDev, &stHwAttr);
	DRV_HDMI_EmiSet(pstHdmiDev, &stHwAttr.stVOAttr, &stHwAttr.stAppAttr);
    if (!VOAttrIsChanged(&stHwAttr.stVOAttr, pstVideoAttr) &&
        !APPAttrIsChanged(&stHwAttr.stAppAttr, pstAppAttr))
    {
        HDMI_INFO("The video and app attr is not changed\n");
        return HI_SUCCESS;
    }

    if (pstAppAttr->bEnableVideo)
    {
        HdmiVideoPathSet(pstHdmiDev, pstVideoAttr);
    }

    DRV_HDMI_GBDPackSend(&pstHdmiDev->stInfoFrame,(pstAppAttr->bEnableHdmi && pstAppAttr->bxvYCCMode));
    DRV_HDMI_AVIInfoFrameSend(&pstHdmiDev->stInfoFrame,(pstAppAttr->bEnableHdmi && pstAppAttr->bEnableAviInfoFrame));//pstAppAttr->bEnableAviInfoFrame);
    DRV_HDMI_VendorInfoFrameSend(&pstHdmiDev->stInfoFrame, pstAppAttr->bEnableHdmi);

#ifdef HDMI_HDR_SUPPORT
    /* the drm infoframe is stop because software was reset in videopath, so reenable it if the mode is HDR10. */
    if(pstHdmiDev->stAttr.stHdrAttr.enHdrMode == HDMI_HDR_MODE_CEA_861_3)
    {
        DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_FALSE);
        DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_TRUE);
    }
#endif

    HDMI_INFO("<<< DRV_HDMI_VOAttrSet out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_VOAttrGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr)
{
    HDMI_INFO(">>> DRV_HDMI_VOAttrGet in...\n");

	CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstVOAttr);

    memcpy(pstVOAttr, &pstHdmiDev->stAttr.stVOAttr, sizeof(HDMI_VO_ATTR_S));

    HDMI_INFO("<<< DRV_HDMI_VOAttrGet out...\n");

    return HI_SUCCESS;
}

#ifdef HDMI_HDR_SUPPORT

static HI_BOOL HdrAttrIsChange(DRV_HDMI_HDR_S *pstAppHdrAttr, HDMI_HDR_ATTR_S *pstHdrAttr)
{
    if(HI_NULL == pstAppHdrAttr || HI_NULL == pstHdrAttr)
    {
        HDMI_ERR("input param is null!\n");
        return HI_FALSE;
    }

    if(pstAppHdrAttr->stHdrAttr.enHdrMode != pstHdrAttr->enHdrMode)
    {
        HDMI_INFO("HdrMode change, old(%d)->new(%d) \n", pstHdrAttr->enHdrMode, pstAppHdrAttr->stHdrAttr.enHdrMode);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.enEotfType != pstHdrAttr->enEotfType)
    {
        HDMI_INFO("EotfType change, old(%d)->new(%d) \n", pstHdrAttr->enEotfType, pstAppHdrAttr->stHdrAttr.enEotfType);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.enMetadataId != pstHdrAttr->enMetadataId)
    {
        HDMI_INFO("MetadataId change, old(%d)->new(%d) \n", pstHdrAttr->enMetadataId, pstAppHdrAttr->stHdrAttr.enMetadataId);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.enColorimetry != pstHdrAttr->enColorimetry)
    {
        HDMI_INFO("Colorimetry change, old(%d)->new(%d) \n", pstHdrAttr->enColorimetry, pstAppHdrAttr->stHdrAttr.enColorimetry);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries0_X != pstHdrAttr->unDescriptor.stType1.u16Primaries0_X)
    {
        HDMI_INFO("Primaries0_X change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries0_X, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries0_X);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries0_Y != pstHdrAttr->unDescriptor.stType1.u16Primaries0_Y)
    {
        HDMI_INFO("Primaries0_Y change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries0_Y, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries0_Y);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries1_X != pstHdrAttr->unDescriptor.stType1.u16Primaries1_X)
    {
        HDMI_INFO("Primaries1_X change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries1_X, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries1_X);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries1_Y != pstHdrAttr->unDescriptor.stType1.u16Primaries1_Y)
    {
        HDMI_INFO("Primaries1_Y change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries1_Y, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries1_Y);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries2_X != pstHdrAttr->unDescriptor.stType1.u16Primaries2_X)
    {
        HDMI_INFO("Primaries2_X change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries2_X, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries2_X);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries2_Y != pstHdrAttr->unDescriptor.stType1.u16Primaries2_Y)
    {
        HDMI_INFO("Primaries2_Y change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16Primaries2_Y, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16Primaries2_Y);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MaxLuminance != pstHdrAttr->unDescriptor.stType1.u16MaxLuminance)
    {
        HDMI_INFO("MaxLuminance change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16MaxLuminance, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MaxLuminance);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MinLuminance != pstHdrAttr->unDescriptor.stType1.u16MinLuminance)
    {
        HDMI_INFO("MinLuminance change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16MinLuminance, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MinLuminance);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MaxLightLevel != pstHdrAttr->unDescriptor.stType1.u16MaxLightLevel)
    {
        HDMI_INFO("MaxLightLevel change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16MaxLightLevel, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16MaxLightLevel);
        return HI_TRUE;
    }
    if(pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16AverageLightLevel != pstHdrAttr->unDescriptor.stType1.u16AverageLightLevel)
    {
        HDMI_INFO("AverageLightLevel change, old(%d)->new(%d) \n", pstHdrAttr->unDescriptor.stType1.u16AverageLightLevel, pstAppHdrAttr->stHdrAttr.unDescriptor.stType1.u16AverageLightLevel);
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_S32 HdrAttrCheck(DRV_HDMI_HDR_S *pstAppHdrAttr)
{
    CHECK_POINTER(pstAppHdrAttr);

    if (pstAppHdrAttr->stHdrAttr.enHdrMode >= HDMI_HDR_MODE_BUTT)
    {
        HDMI_ERR("Hdmi hdr enHdrMode(%d) is wrong\n", pstAppHdrAttr->stHdrAttr.enHdrMode);
        return HI_FAILURE;
    }
    if (pstAppHdrAttr->stHdrAttr.enEotfType >= HDMI_EOTF_BUTT)
    {
        HDMI_ERR("Hdmi hdr enEotfType(%d) is wrong\n", pstAppHdrAttr->stHdrAttr.enEotfType);
        return HI_FAILURE;
    }
    if (pstAppHdrAttr->stHdrAttr.enMetadataId >= HDMI_HDR_METADATA_ID_BUTT)
    {
        HDMI_ERR("Hdmi hdr enMetadataId(%d) is wrong\n", pstAppHdrAttr->stHdrAttr.enMetadataId);
        return HI_FAILURE;
    }
    if (pstAppHdrAttr->stHdrAttr.enColorimetry > HDMI_HDR_EXTENDED_COLORIMETRY_2020_non_const_luminous)
    {
        HDMI_ERR("Hdmi hdr enColorimetry(%d) is wrong\n", pstAppHdrAttr->stHdrAttr.enColorimetry);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_VOID HdmiZeroDrmIfStop(HDMI_DEVICE_S* pstHdmiDev)
{
    CHECK_POINTER_NO_RET(pstHdmiDev);

    if(HDMI_HDR_MODE_CEA_861_3 != pstHdmiDev->stAttr.stHdrAttr.enHdrMode)
    {
        DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_FALSE);
        HDMI_INFO("Zero DRM Infoframe send timeout.\n");
    }

    return;
}

static HI_S32 HdmiHdrModeChangeTimeout(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_S32              s32Ret = HI_SUCCESS;

    CHECK_POINTER(pstHdmiDev);

    if(HDMI_HDR_DEBUG_MODE_OE == pstHdmiDev->stHdrInfo.enHdrDebugMode)
    {
        HdmiPhyOutputEnable(pstHdmiDev, HI_TRUE);
    }

    pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, HI_FALSE);

    if(HDMI_HDR_DEBUG_MODE_OE == pstHdmiDev->stHdrInfo.enHdrDebugMode)
    {
        DRV_HDMI_BlackDataSet(pstHdmiDev, HI_FALSE);
        s32Ret |= DRV_HDMI_AudioOutputEnableSet(pstHdmiDev, HI_TRUE);
    }

    HDMI_INFO("HDR mode change timeout, debug mode: %d.\n", pstHdmiDev->stHdrInfo.enHdrDebugMode);
    return s32Ret;
}

static HI_VOID HdmiZeroDrmIfTimerSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_TIMER_CONFIG_S* pstTimer)
{
    HDMI_ATTR_S                     *pstAttr;
    HDMI_TIMER_CONFIG_S              stTimerCfg;
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstTimer);

    pstAttr = &pstHdmiDev->stAttr;
    CHECK_POINTER_NO_RET(pstAttr);

    stTimerCfg.enTimerType = HDMI_TIMER_ZERO_DRMIF;
    stTimerCfg.bTimerStart = HI_FALSE;
    stTimerCfg.u32Time     = ZERO_DRMIF_SEND_TIME;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_HdrTimerSet(pstHdmiDev->pstHdmiHal, &stTimerCfg);

    if(HI_FALSE == pstTimer->bTimerStart)
    {
        return;
    }

    HDMI_MEMSET(&(pstAttr->stHdrAttr.unDescriptor), 0, sizeof(HDMI_META_DESCRIPTOR_UN));
    DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_TRUE);

    pstTimer->enTimerType = HDMI_TIMER_ZERO_DRMIF;
    pstHdmiDev->pstHdmiHal->HAL_HDMI_HdrTimerSet(pstHdmiDev->pstHdmiHal, pstTimer);

    return;
}

static HI_VOID HdmiHdrModeChangeTimerSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_TIMER_CONFIG_S* pstTimer)
{
    HDMI_TIMER_CONFIG_S              stTimerCfg;

    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstTimer);

    stTimerCfg.enTimerType = HDMI_TIMER_SDR_TO_HDR10;
    stTimerCfg.u32Time     = HDRMODE_CHANGE_TIME;
    stTimerCfg.bTimerStart = HI_FALSE;

    /* stop timer first. */
    pstHdmiDev->pstHdmiHal->HAL_HDMI_HdrTimerSet(pstHdmiDev->pstHdmiHal, &stTimerCfg);
    if(HI_FALSE == pstTimer->bTimerStart)
    {
        return;
    }

    pstTimer->enTimerType = HDMI_TIMER_SDR_TO_HDR10;
    pstHdmiDev->pstHdmiHal->HAL_HDMI_HdrTimerSet(pstHdmiDev->pstHdmiHal, pstTimer);

    return;
}

static HI_VOID HdmiTimerSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_TIMER_CONFIG_S* pstTimer)
{
    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstTimer);

    switch(pstTimer->enTimerType)
    {
        case HDMI_TIMER_ZERO_DRMIF:
            HdmiZeroDrmIfTimerSet(pstHdmiDev, pstTimer);
            break;
        case HDMI_TIMER_SDR_TO_HDR10:
            HdmiHdrModeChangeTimerSet(pstHdmiDev, pstTimer);
            break;
        default:
            break;
    }

    return;
}

static HI_VOID HdrColorimetryConv(HDMI_HDR_COLORIMETRY enColorimetry, HDMI_VO_ATTR_S* pstVOAttr)
{
    CHECK_POINTER_NO_RET(pstVOAttr);

    if(HDMI_HDR_COLORIMETRY_EXTENDED <= enColorimetry)
    {
        pstVOAttr->enColorimetry = HDMI_COLORIMETRY__EXTENDED;
        pstVOAttr->enExtendedColorimetry = enColorimetry - HDMI_HDR_EXTENDED_COLORIMETRY_XV_YCC_601;
    }
    else
    {
        switch(enColorimetry)
        {
            case HDMI_HDR_COLORIMETRY_NONE:
                pstVOAttr->enColorimetry = HDMI_COLORIMETRY__NONE;
                break;
            case HDMI_HDR_COLORIMETRY_ITU_601:
                pstVOAttr->enColorimetry = HDMI_COLORIMETRY__ITU_601;
                break;
            case HDMI_HDR_COLORIMETRY_ITU_709:
                pstVOAttr->enColorimetry = HDMI_COLORIMETRY__ITU_709;
                break;
            default:
                break;
        }
        pstVOAttr->enExtendedColorimetry = HDMI_EXTENDED_COLORIMETRY_BUTT;
    }
    return;
}

HI_VOID DisableHdr(HDMI_DEVICE_S *pstHdmiDev, DRV_HDMI_HDR_S *pstAppHdrAttr,
                   HDMI_COLORIMETRY_E enSdrColorimetryBack, HDMI_EXTENDED_COLORIMETRY enSdrExtColorimetryBack)
{
    HDMI_HDR_ATTR_S         *pstHdrAttr      = HI_NULL;
    HDMI_ATTR_S             *pstAttr         = HI_NULL;
    HDMI_APP_ATTR_S         *pstAppAttr      = HI_NULL;
    HDMI_HDR_INFO_S         *pstHdrInfo      = HI_NULL;

    CHECK_POINTER_NO_RET(pstHdmiDev);
    CHECK_POINTER_NO_RET(pstAppHdrAttr);

    pstAttr    = &(pstHdmiDev->stAttr);
    pstHdrAttr = &(pstAttr->stHdrAttr);
    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);
    pstHdrInfo = &(pstHdmiDev->stHdrInfo);

    switch(pstHdrAttr->enHdrMode)
    {
        case HDMI_HDR_MODE_DISABLE:
            HDMI_INFO("Hdr mode is disable, do not change anything! \n");
            return ;
        case HDMI_HDR_MODE_CEA_861_3:
            pstAttr->stVOAttr.enColorimetry         = enSdrColorimetryBack;
            pstAttr->stVOAttr.enExtendedColorimetry = enSdrExtColorimetryBack;

            HDMI_MEMCPY(&pstAttr->stHdrAttr, &(pstAppHdrAttr->stHdrAttr), sizeof(HDMI_HDR_ATTR_S));

            if(HDMI_HDR_DEBUG_MODE_NONE != pstHdrInfo->enHdrDebugMode)
            {
                /* To solve the problem of some TV can not switch smoothly from HDR10 to non-HDR10. */
                if(HDMI_HDR_DEBUG_MODE_OE == pstHdrInfo->enHdrDebugMode)
                {
                    DRV_HDMI_AudioOutputEnableSet(pstHdmiDev, HI_FALSE);
                    DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
                }

                pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, HI_TRUE);

                if(HDMI_HDR_DEBUG_MODE_OE == pstHdrInfo->enHdrDebugMode)
                {
                    HdmiPhyOutputEnable(pstHdmiDev, HI_FALSE);
                }

                pstHdrInfo->stHdrModeChnTimer.bTimerStart = HI_TRUE;
                HdmiTimerSet(pstHdmiDev, &(pstHdrInfo->stHdrModeChnTimer));
                DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_FALSE);
            }
            else
            {
                /* start timer, send zero DRMIF and stop after 2 seconds. */
                pstHdrInfo->stZeroDrmIfTimer.bTimerStart = HI_TRUE;
                HdmiTimerSet(pstHdmiDev, &(pstHdrInfo->stZeroDrmIfTimer));
            }

            HDMI_INFO("resume enColorimetry:%d, enExtendedColorimetry:%d\n", enSdrColorimetryBack, enSdrExtColorimetryBack);
            break;
        case HDMI_HDR_MODE_CEA_861_3_AUTHEN:
        case HDMI_HDR_MODE_DOLBY_NORMAL:
        case HDMI_HDR_MODE_DOLBY_TUNNELING:
            if(HDMI_HDR_MODE_DOLBY_NORMAL != pstHdrAttr->enHdrMode)
            {
                pstAppAttr->enOutColorSpace = HDMI_COLORSPACE_YCbCr444;
            }
            pstAttr->stVOAttr.enRGBQuantization = HDMI_QUANTIZATION_RANGE_DEFAULT;
            pstAttr->stVOAttr.enYCCQuantization = HDMI_YCC_QUANTIZATION_RANGE_LIMITED;
            break;
        default:
            break;
    }

    return;
}

HI_S32 ChangeToHdr(HDMI_DEVICE_S *pstHdmiDev, DRV_HDMI_HDR_S *pstAppHdrAttr, HDMI_COLORIMETRY_E *penSdrColorimetryBack,
                   HDMI_EXTENDED_COLORIMETRY *penSdrExtColorimetryBack, HDMI_HDR_MODE_E enHdrModeBack)
{
    HI_S32                   s32Ret          = HI_SUCCESS;
    HDMI_HDR_ATTR_S         *pstHdrAttr      = HI_NULL;
    HDMI_ATTR_S             *pstAttr         = HI_NULL;
    HDMI_APP_ATTR_S         *pstAppAttr      = HI_NULL;
    HDMI_HDR_INFO_S         *pstHdrInfo      = HI_NULL;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAppHdrAttr);
    CHECK_POINTER(penSdrColorimetryBack);
    CHECK_POINTER(penSdrExtColorimetryBack);

    pstAttr    = &(pstHdmiDev->stAttr);
    pstHdrAttr = &(pstAttr->stHdrAttr);
    pstAppAttr = &(pstHdmiDev->stAttr.stAppAttr);
    pstHdrInfo = &(pstHdmiDev->stHdrInfo);


    pstHdrInfo->stZeroDrmIfTimer.bTimerStart = HI_FALSE;
    HdmiZeroDrmIfTimerSet(pstHdmiDev, &(pstHdrInfo->stZeroDrmIfTimer));

    if(pstHdrAttr->enUserHdrMode != HDMI_HDR_USERMODE_HDR10)
    {
        HDMI_ERR("usermode is not HDR10.\n");
        return HI_FAILURE;
    }
    /* backup colorimetry of SDR */
    if(HDMI_HDR_MODE_DISABLE == enHdrModeBack)
    {
        HDMI_INFO("back enColorimetry:%d, enExtendedColorimetry:%d\n", pstAttr->stVOAttr.enColorimetry, pstAttr->stVOAttr.enExtendedColorimetry);
        *penSdrColorimetryBack = pstAttr->stVOAttr.enColorimetry;
        *penSdrExtColorimetryBack = pstAttr->stVOAttr.enExtendedColorimetry;
    }

    pstAppAttr->bEnableDRMInfoFrame = HI_TRUE;
    HdrColorimetryConv(pstAppHdrAttr->stHdrAttr.enColorimetry, &(pstAttr->stVOAttr));

    if(HDMI_HDR_DEBUG_MODE_NONE != pstHdrInfo->enHdrDebugMode)
    {
        /* To solve the problem of some TV can not switch smoothly from non-HDR10 to HDR10. */
        if(HDMI_HDR_DEBUG_MODE_OE == pstHdrInfo->enHdrDebugMode)
        {
            s32Ret |= DRV_HDMI_AudioOutputEnableSet(pstHdmiDev, HI_FALSE);
            DRV_HDMI_BlackDataSet(pstHdmiDev, HI_TRUE);
        }

        pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, HI_TRUE);

        if(HDMI_HDR_DEBUG_MODE_OE == pstHdrInfo->enHdrDebugMode)
        {
            HdmiPhyOutputEnable(pstHdmiDev, HI_FALSE);
        }

        pstHdrInfo->stHdrModeChnTimer.bTimerStart = HI_TRUE;
        HdmiTimerSet(pstHdmiDev, &(pstHdrInfo->stHdrModeChnTimer));
    }

    return s32Ret;
}

/* stAppHdrAttr: user attr */
HI_S32 DRV_HDMI_HdrAttrSet(HDMI_DEVICE_S* pstHdmiDev, DRV_HDMI_HDR_S *pstAppHdrAttr)
{
    HDMI_HDR_ATTR_S         *pstHdrAttr      = HI_NULL;
    HDMI_ATTR_S             *pstAttr         = HI_NULL;
    HDMI_APP_ATTR_S         *pstAppAttr      = HI_NULL;
    HDMI_AVI_INFOFRAME_S    *pstAVIInfoframe = HI_NULL;
    HI_S32                   s32Ret          = HI_SUCCESS;
    static HDMI_HDR_MODE_E   enHdrModeBack   = HDMI_HDR_MODE_DISABLE;
    HDMI_HDR_INFO_S         *pstHdrInfo      = HI_NULL;
    static HDMI_COLORIMETRY_E          enSdrColorimetryBack;
    static HDMI_EXTENDED_COLORIMETRY   enSdrExtColorimetryBack;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAppHdrAttr);

    pstAttr = &pstHdmiDev->stAttr;
    CHECK_POINTER(pstAttr);

    pstHdrAttr = &pstAttr->stHdrAttr;
    CHECK_POINTER(pstHdrAttr);

    pstAppAttr = &pstHdmiDev->stAttr.stAppAttr;
    CHECK_POINTER(pstAppAttr);

    pstHdrInfo = &(pstHdmiDev->stHdrInfo);
	CHECK_POINTER(pstHdrInfo);

    if(HI_SUCCESS != HdrAttrCheck(pstAppHdrAttr))
    {
        HDMI_INFO("AppHdrAttr check failed! \n");
        return HI_FAILURE;
    }

    if(!HdrAttrIsChange(pstAppHdrAttr, pstHdrAttr))
    {
        return HI_SUCCESS;
    }

    HDMI_INFO("DRV_HDMI_HdrSet in...\n");

    pstAVIInfoframe = &pstHdmiDev->stInfoFrame.stAVIInfoFrame;
    /* Change mode to SDR */
	if(pstAppHdrAttr->stHdrAttr.enHdrMode == HDMI_HDR_MODE_DISABLE)
	{
            DisableHdr(pstHdmiDev, pstAppHdrAttr, enSdrColorimetryBack, enSdrExtColorimetryBack);
		HDMI_MEMCPY(&pstAttr->stHdrAttr, &(pstAppHdrAttr->stHdrAttr), sizeof(HDMI_HDR_ATTR_S));

        goto SEND_INFOFRAME;
	}
    /* Change mode to HDR */
    HDMI_MEMCPY(&pstAttr->stHdrAttr, &(pstAppHdrAttr->stHdrAttr), sizeof(HDMI_HDR_ATTR_S));
    switch(pstAppHdrAttr->stHdrAttr.enHdrMode)
    {
        case HDMI_HDR_MODE_CEA_861_3:
            ChangeToHdr(pstHdmiDev, pstAppHdrAttr, &enSdrColorimetryBack, &enSdrExtColorimetryBack, enHdrModeBack);
            HDMI_INFO("HDR10 mode, colorimetry(%d), extended_colorimetry(%d)\n", pstAttr->stVOAttr.enColorimetry, pstAttr->stVOAttr.enExtendedColorimetry);
            break;
        case HDMI_HDR_MODE_CEA_861_3_AUTHEN:
            /* HDR10 authen mode, HDMI must bypass. */
            if(pstHdrAttr->enUserHdrMode != HDMI_HDR_USERMODE_DOLBY)
            {
                HDMI_ERR("usermode is not dolby.\n");
                return HI_FAILURE;
            }

            pstAppAttr->bEnableDRMInfoFrame = HI_TRUE;
            HDMI_INFO("Hdr authen mode is enable! \n");
            break;
        case HDMI_HDR_MODE_DOLBY_NORMAL:
        case HDMI_HDR_MODE_DOLBY_TUNNELING:
            if(pstHdrAttr->enUserHdrMode != HDMI_HDR_USERMODE_DOLBY)
            {
                HDMI_ERR("usermode is not dolby.\n");
                return HI_FAILURE;
            }

            pstAppAttr->bEnableDRMInfoFrame = HI_FALSE;
            HDMI_INFO("Dolby mode(%d) is enable! \n", pstAppHdrAttr->stHdrAttr.enHdrMode);
            break;
        default:
            return HI_SUCCESS;
    }

    s32Ret |= DRV_HDMI_DRMInfoFrameSend(&pstHdmiDev->stInfoFrame, pstAppAttr->bEnableHdmi && pstAppAttr->bEnableDRMInfoFrame);
    if(s32Ret != HI_SUCCESS)
    {
        HDMI_ERR("DrmInfoframe send error!\n");
        return HI_FAILURE;
    }

SEND_INFOFRAME:

    s32Ret |= DRV_HDMI_AVIInfoFrameSend(&pstHdmiDev->stInfoFrame,(pstAppAttr->bEnableHdmi && pstAppAttr->bEnableAviInfoFrame));
    if(s32Ret != HI_SUCCESS)
    {
        HDMI_ERR("AviInfoframe send error!\n");
        return HI_FAILURE;
    }

    s32Ret |= DRV_HDMI_VendorInfoFrameSend(&pstHdmiDev->stInfoFrame, pstAppAttr->bEnableHdmi);

    s32Ret |= HdmiCscSet(pstHdmiDev, &pstAttr->stVOAttr);
    enHdrModeBack = pstAppHdrAttr->stHdrAttr.enHdrMode;

    HDMI_INFO("DRV_HDMI_HdrSet out...\n");

    return s32Ret;
}
#endif

HI_S32 DRV_HDMI_AttrGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_ATTR_S *pstAttr)
{
    HDMI_INFO(">>> DRV_HDMI_AttrGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAttr);

    HDMI_MEMCPY(pstAttr, &pstHdmiDev->stAttr, sizeof(HDMI_ATTR_S));
    HDMI_INFO("<<< DRV_HDMI_AttrGet out...\n");

    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_AttrSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_ATTR_S *pstAttr)
{
    HDMI_AO_ATTR_S*          pstAudioAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_APP_ATTR_S*         pstAppAttr;

    HDMI_INFO(">>> DRV_HDMI_AttrSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstAttr);

    HDMI_INFO("bEnableHdmi: %d\n", pstAttr->stAppAttr.bEnableHdmi);
    HDMI_INFO("bEnableVideo: %d\n", pstAttr->stAppAttr.bEnableVideo);
    HDMI_INFO("bEnableAudio: %d\n", pstAttr->stAppAttr.bEnableAudio);
    HDMI_INFO("enOutColorSpace: %d\n", pstAttr->stAppAttr.enOutColorSpace);
    HDMI_INFO("enDeepColorMode: %d\n", pstAttr->stAppAttr.enDeepColorMode);
    HDMI_INFO("bxvYCCMode: %d\n", pstAttr->stAppAttr.bxvYCCMode);
    HDMI_INFO("bEnableAviInfoFrame: %d\n", pstAttr->stAppAttr.bEnableAviInfoFrame);
    HDMI_INFO("bEnableSpdInfoFrame: %d\n", pstAttr->stAppAttr.bEnableSpdInfoFrame);
    HDMI_INFO("bEnableMpegInfoFrame: %d\n", pstAttr->stAppAttr.bEnableMpegInfoFrame);
    HDMI_INFO("bEnableAudInfoFrame: %d\n", pstAttr->stAppAttr.bEnableAudInfoFrame);
    HDMI_INFO("bDebugFlag: %d\n", pstAttr->stAppAttr.bDebugFlag);
    HDMI_INFO("bHDCPEnable: %d\n", pstAttr->stAppAttr.bHDCPEnable);
	HDMI_INFO("enHDCPMode: %d\n", pstAttr->stAppAttr.enHDCPMode);
    HDMI_INFO("bEnableClrSpaceAdapt: %d\n", pstAttr->stAppAttr.bEnableClrSpaceAdapt);
    HDMI_INFO("bEnableDeepClrAdapt: %d\n", pstAttr->stAppAttr.bEnableDeepClrAdapt);
    HDMI_INFO("bAuthMode: %d\n", pstAttr->stAppAttr.bAuthMode);

    if (HI_SUCCESS != CheckAppAttr(&pstAttr->stAppAttr))
    {
        HDMI_ERR("Check hdmi attr fail\n");
        return HI_FAILURE;
    }

    if (HI_SUCCESS != HdmiColorAndBitStrategy(pstHdmiDev, &pstAttr->stAppAttr))
    {
        HDMI_ERR(" HdmiColorAndBitStrategy fail\n");
        return HI_FAILURE;
    }

    pstAudioAttr = &pstHdmiDev->stAttr.stAOAttr;
    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;
    HDMI_MEMCPY(pstAppAttr, &pstAttr->stAppAttr, sizeof(HDMI_APP_ATTR_S));

#ifdef HDMI_HDCP_SUPPORT
    pstHdmiDev->stHdcpInfo.bHdcpIsOn = pstAppAttr->bHDCPEnable;
	pstHdmiDev->stHdcpInfo.enHdcpMode = pstAppAttr->enHDCPMode;
#endif

    if (HI_SUCCESS != DRV_HDMI_AOAttrSet(pstHdmiDev, &pstAttr->stAOAttr))
    {
        HDMI_ERR("DRV_HDMI_AOAttrSet fail\n");
        return HI_FAILURE;
    }

    if (HI_SUCCESS != DRV_HDMI_VOAttrSet(pstHdmiDev, &pstAttr->stVOAttr))
    {
        HDMI_ERR("DRV_HDMI_VOAttrSet fail\n");
        return HI_FAILURE;
    }

    HDMI_INFO(">>> DRV_HDMI_AttrSet out...\n");

    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_InfoFrameGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_INFOFRAME_ID_E enInfoFrameId, HDMI_INFOFRAME_U *puInfoFrame)
{
    HDMI_INFO(">>> DRV_HDMI_InfoFrameGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(puInfoFrame);

    switch (enInfoFrameId)
    {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            HDMI_MEMCPY(&puInfoFrame->stVendorInfoFrame, &pstHdmiDev->stInfoFrame.stVendorInfoFrame, sizeof(HDMI_VENDOR_INFOFRAME_S));
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            HDMI_MEMCPY(&puInfoFrame->stAVIInfoFrame, &pstHdmiDev->stInfoFrame.stAVIInfoFrame, sizeof(HDMI_AVI_INFOFRAME_S));
            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            HDMI_MEMCPY(&puInfoFrame->stAudioInfoFrame, &pstHdmiDev->stInfoFrame.stAudioInfoFrame, sizeof(HDMI_AUDIO_INFOFRAME_S));
            break;
        default:
            return HI_FAILURE;
    }

    HDMI_INFO("<<< DRV_HDMI_InfoFrameGet out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_InfoFrameSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_INFOFRAME_ID_E enInfoFrameId, HDMI_INFOFRAME_U *puInfoFrame)
{
    HDMI_VO_ATTR_S *pVoAttr;
    HI_S32  s32Ret = 0;

    HDMI_INFO(">>> DRV_HDMI_InfoFrameSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(puInfoFrame);

    pVoAttr = &pstHdmiDev->stAttr.stVOAttr;
    CHECK_POINTER(pVoAttr);

    switch (enInfoFrameId)
    {
        case HDMI_INFOFRAME_TYPE_VENDOR:
            {
                HDMI_VENDOR_INFOFRAME_S *pSrcVendInfo  = &puInfoFrame->stVendorInfoFrame;
                HDMI_VENDOR_INFOFRAME_S *pDestVendInfo = &pstHdmiDev->stInfoFrame.stVendorInfoFrame;
                HDMI_MEMCPY(pDestVendInfo, pSrcVendInfo, sizeof(HDMI_VENDOR_INFOFRAME_S));
            }
            break;
        case HDMI_INFOFRAME_TYPE_AVI:
            {
                HDMI_AVI_INFOFRAME_S *pSrcAviInfo = &puInfoFrame->stAVIInfoFrame;
                HDMI_AVI_INFOFRAME_S *pDestAviInfo = &pstHdmiDev->stInfoFrame.stAVIInfoFrame;
                HDMI_MEMCPY(pDestAviInfo, pSrcAviInfo, sizeof(HDMI_AVI_INFOFRAME_S));

                HDMI_INFO("type: %d\n", pDestAviInfo->type);
                HDMI_INFO("version: %d\n", pDestAviInfo->version);
                HDMI_INFO("length: %d\n", pDestAviInfo->length);
                HDMI_INFO("colorspace: %d\n", pDestAviInfo->colorspace);
                HDMI_INFO("active_info_valid: %d\n", pDestAviInfo->active_info_valid);
                HDMI_INFO("horizontal_bar_valid: %d\n", pDestAviInfo->horizontal_bar_valid);
                HDMI_INFO("vertical_bar_valid: %d\n", pDestAviInfo->vertical_bar_valid);
                HDMI_INFO("scan_mode: %d\n", pDestAviInfo->scan_mode);
                HDMI_INFO("colorimetry: %d\n", pDestAviInfo->colorimetry);
                HDMI_INFO("picture_aspect: %d\n", pDestAviInfo->picture_aspect);
                HDMI_INFO("active_aspect: %d\n", pDestAviInfo->active_aspect);
                HDMI_INFO("itc: %d\n", pDestAviInfo->itc);
                HDMI_INFO("extended_colorimetry: %d\n", pDestAviInfo->extended_colorimetry);
                HDMI_INFO("quantization_range: %d\n", pDestAviInfo->quantization_range);
                HDMI_INFO("nups: %d\n", pDestAviInfo->nups);
                HDMI_INFO("video_timing: %d\n", pDestAviInfo->video_timing);
                HDMI_INFO("disp_fmt: %d\n", pDestAviInfo->disp_fmt);
                HDMI_INFO("ycc_quantization_range: %d\n", pDestAviInfo->ycc_quantization_range);
                HDMI_INFO("content_type: %d\n", pDestAviInfo->content_type);
                HDMI_INFO("pixel_repeat: %d\n", pDestAviInfo->pixel_repeat);
                HDMI_INFO("top_bar: %d\n", pDestAviInfo->top_bar);
                HDMI_INFO("bottom_bar: %d\n", pDestAviInfo->bottom_bar);
                HDMI_INFO("left_bar: %d\n", pDestAviInfo->left_bar);
                HDMI_INFO("right_bar: %d\n", pDestAviInfo->right_bar);

                // q00352704 @2015.08.21. set aspect and colorimetry by attr
                pVoAttr->enPictureAspect 		= pDestAviInfo->picture_aspect;
                pVoAttr->enColorimetry 			= pDestAviInfo->colorimetry;
                pVoAttr->enExtendedColorimetry 	= pDestAviInfo->extended_colorimetry;
                pVoAttr->enRGBQuantization      = pDestAviInfo->quantization_range;
                pVoAttr->enYCCQuantization      = pDestAviInfo->ycc_quantization_range;

                s32Ret = DRV_HDMI_VOAttrSet(pstHdmiDev, pVoAttr);
            }

            break;
        case HDMI_INFOFRAME_TYPE_AUDIO:
            {
                HDMI_AUDIO_INFOFRAME_S *pSrcAudioInfo  = &puInfoFrame->stAudioInfoFrame;
                HDMI_AUDIO_INFOFRAME_S *pDestAudioInfo = &pstHdmiDev->stInfoFrame.stAudioInfoFrame;
                HDMI_MEMCPY(pDestAudioInfo, pSrcAudioInfo, sizeof(HDMI_AUDIO_INFOFRAME_S));

                HDMI_INFO("type: %d\n", pDestAudioInfo->type);
                HDMI_INFO("version: %d\n", pDestAudioInfo->version);
                HDMI_INFO("length: %d\n", pDestAudioInfo->length);
                HDMI_INFO("channels: %d\n", pDestAudioInfo->channels);
                HDMI_INFO("coding_type: %d\n", pDestAudioInfo->coding_type);
                HDMI_INFO("sample_size: %d\n", pDestAudioInfo->sample_size);
                HDMI_INFO("sample_frequency: %d\n", pDestAudioInfo->sample_frequency);
                HDMI_INFO("coding_type_ext: %d\n", pDestAudioInfo->coding_type_ext);
                HDMI_INFO("channel_allocation: %d\n", pDestAudioInfo->channel_allocation);
                HDMI_INFO("lfe_playback_level: %d\n", pDestAudioInfo->lfe_playback_level);
                HDMI_INFO("level_shift_value: %d\n", pDestAudioInfo->level_shift_value);
                HDMI_INFO("downmix_inhibit: %d\n", pDestAudioInfo->downmix_inhibit);
                HDMI_INFO("level_shift_value: %d\n", pDestAudioInfo->level_shift_value);
                HDMI_INFO("downmix_inhibit: %d\n", pDestAudioInfo->downmix_inhibit);
            }
           break;
        default:
            return HI_FAILURE;
    }

    if(enInfoFrameId != HDMI_INFOFRAME_TYPE_AVI)
	{
        DRV_HDMI_InfoFrameSend(&pstHdmiDev->stInfoFrame, enInfoFrameId, puInfoFrame);
	}

    HDMI_INFO("<<< DRV_HDMI_InfoFrameSet out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_StatusGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_STATUS_S *pHdmiStatus)
{
    HDMI_HARDWARE_STATUS_S stHwStatus;

    HDMI_INFO(">>> DRV_HDMI_StatusGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pHdmiStatus);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_HardwareStatusGet(pstHdmiDev->pstHdmiHal, &stHwStatus);
    /* Some TV sometimes has no hotPlug but has rsen*/
    pHdmiStatus->bConnected     = (stHwStatus.stCommonStatus.bHotPlug || stHwStatus.stCommonStatus.bRsen);
    pHdmiStatus->bSinkPowerOn   = stHwStatus.stCommonStatus.bRsen;
    pHdmiStatus->bAuthed        = pstHdmiDev->stHdcpInfo.bHdcpAuthSuc;
    pHdmiStatus->u8Bksv[0]      = stHwStatus.stHdcpStatus.u8BKSVs[0];
    pHdmiStatus->u8Bksv[1]      = stHwStatus.stHdcpStatus.u8BKSVs[1];
    pHdmiStatus->u8Bksv[2]      = stHwStatus.stHdcpStatus.u8BKSVs[2];
    pHdmiStatus->u8Bksv[3]      = stHwStatus.stHdcpStatus.u8BKSVs[3];
    pHdmiStatus->u8Bksv[4]      = stHwStatus.stHdcpStatus.u8BKSVs[4];

    switch(pstHdmiDev->stHdcpInfo.enHdcpMode)
    {
        case HDMI_HDCP_MODE_1_4:
            pHdmiStatus->enHdcpVersion = HDMI_HDCP_VERSION_1_4;
            break;
        case HDMI_HDCP_MODE_2_2:
            pHdmiStatus->enHdcpVersion = HDMI_HDCP_VERSION_2_2;
            break;
        default:
            pHdmiStatus->enHdcpVersion = HDMI_HDCP_VERSION_NONE;
            break;
    }
    HDMI_INFO("<<< DRV_HDMI_StatusGet out...\n");

    return HI_SUCCESS;
}

#ifdef HDMI_CEC_SUPPORT
HI_S32 DRV_HDMI_CecCmdSend(HDMI_DEVICE_S* pstHdmiDev,  HDMI_CEC_CMD_S  *pstCecCmd)
{
    HI_S32 s32Ret = HI_SUCCESS;

    HDMI_INFO(">>> DRV_HDMI_CecCmdSend in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    if(!pstHdmiDev->stEdidInfo.stCapability.stCECAddr.bPhyAddrValid)
    {
        HDMI_WARN("Sink not support CEC,Suggest don't sent.\n");
    }

    s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_CecMsgSend(pstHdmiDev->pstHdmiHal, pstCecCmd);

    HDMI_INFO("<<< DRV_HDMI_CecCmdSend out...\n");

    return s32Ret;
}

HI_S32 DRV_HDMI_CecCmdReceive(HDMI_DEVICE_S* pstHdmiDev, HDMI_CEC_CMD_S  *pstCecCmd)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HDMI_INFO(">>> DRV_HDMI_CecCmdReceive in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstCecCmd);

    s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_CecMsgReceive(pstHdmiDev->pstHdmiHal, pstCecCmd);

    HDMI_INFO("<<< DRV_HDMI_CecCmdReceive out...\n");
    return s32Ret;
}

HI_S32 DRV_HDMI_CecStatusGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_CEC_INFO_S  *pstCecStatus)
{
    HI_S32  s32Ret = HI_SUCCESS;

    HDMI_INFO(">>> DRV_HDMI_CecStatusGet in...\n");
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    CHECK_POINTER(pstCecStatus);

    s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_CecStatusGet(pstHdmiDev->pstHdmiHal, &pstHdmiDev->stCecInfo);
    HDMI_MEMCPY(pstCecStatus, &pstHdmiDev->stCecInfo, sizeof(HDMI_CEC_INFO_S));

    HDMI_INFO("<<< DRV_HDMI_CecStatusGet out...\n");

    return s32Ret;
}

HI_S32 DRV_HDMI_CecEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{

    HI_S32                  s32Ret = HI_SUCCESS;
    HDMI_INFO(">>> in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    if(!pstHdmiDev->stEdidInfo.stCapability.stCECAddr.bPhyAddrValid)
    {
        HDMI_WARN("Sink not support CEC,Suggest don't eanble.\n");
    }
    pstHdmiDev->stCecInfo.bCecEnable = bEnable;
    s32Ret = pstHdmiDev->pstHdmiHal->HAL_HDMI_CecEnableSet(pstHdmiDev->pstHdmiHal, bEnable);
    //HDMI_INFO("DRV Cec bEnable = %d \n", pstHdmiDev->stCecInfo.bCecEnable);

    HDMI_INFO(">>> out...\n");

    return s32Ret;
}

#endif


#ifdef HDMI_HDCP_SUPPORT
HI_S32 DRV_HDMI_KeyLoad(HDMI_DEVICE_S* pstHdmiDev, HDMI_LOAD_KEY_S *pstLoadKey)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_DRV_CIPHER_FLASH_ENCRYPT_HDCPKEY_S stFlashEncrytedHdcpKey;
    CIPHER_EXPORT_FUNC_S *g_stCipherExtFunc = HI_NULL;

    HDMI_INFO(">>> DRV_HDMI_KeyLoad in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstLoadKey);

    s32Ret = HI_DRV_MODULE_GetFunction(HI_ID_CIPHER, (HI_VOID**)&g_stCipherExtFunc);
    if((NULL == g_stCipherExtFunc) || (s32Ret != HI_SUCCESS))
    {
        HDMI_ERR("Get cipher functions failed!\n");
        return HI_FAILURE;
    }

    if((HI_NULL != g_stCipherExtFunc)
     &&(HI_NULL != g_stCipherExtFunc->pfnCipherLoadHdcpKey))
    {
        /* load hdcp key */
        memcpy(stFlashEncrytedHdcpKey.u8Key, pstLoadKey->pu8InputEncryptedKeyAddr, pstLoadKey->u32KeyLength);
#if 1
        stFlashEncrytedHdcpKey.u32KeyLen = pstLoadKey->u32KeyLength;
        stFlashEncrytedHdcpKey.enHDCPKeyType = HI_DRV_HDCPKEY_TX0;
        stFlashEncrytedHdcpKey.enHDCPVersion = HI_DRV_HDCP_VERIOSN_1x;
#endif
        s32Ret = (g_stCipherExtFunc->pfnCipherLoadHdcpKey)(&stFlashEncrytedHdcpKey);
        if( HI_SUCCESS != s32Ret)
        {
            HDMI_ERR("Load hdcp key error!\n");
            return HI_FAILURE;
        }
        HDMI_INFO("Load hdcp key successful!\n");
    }
    pstHdmiDev->stHdcpInfo.bHdcp1Prep = HI_TRUE;
    //HdcpStrategy(pstHdmiDev);

    HDMI_INFO("<<< DRV_HDMI_KeyLoad out...\n");

    return s32Ret;

}

#endif


HI_S32 DRV_HDMI_PlayStatusGet(HDMI_DEVICE_S* pstHdmiDev, HI_U32 *pu32PlayStatus)
{
    //HDMI_INFO(">>> DRV_HDMI_PlayStatusGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pu32PlayStatus);

    if (pstHdmiDev->enRunState & HDMI_RUN_STATE_START)
    {
        *pu32PlayStatus = 1;
    }
    else
    {
        *pu32PlayStatus = 0;
    }

    //HDMI_INFO("<<< DRV_HDMI_PlayStatusGet out...\n");

    return HI_SUCCESS;
}

HDMI_DEFAULT_ACTION_E DRV_HDMI_DefaultActionGet(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_DEFAULT_ACTION_E eDefaultAction;
    HDMI_INFO(">>> DRV_HDMI_DefaultActionGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    eDefaultAction = pstHdmiDev->stAttr.stAppAttr.enHdmiAction;

    HDMI_INFO("<<< DRV_HDMI_DefaultActionGet out...\n");

    return eDefaultAction;
}

HI_S32 DRV_HDMI_DefaultActionSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_DEFAULT_ACTION_E enAction)
{
    HDMI_INFO(">>> DRV_HDMI_DefaultActionSet in...\n");

    CHECK_POINTER(pstHdmiDev);
    pstHdmiDev->stAttr.stAppAttr.enHdmiAction = enAction;

    HDMI_INFO("<<< DRV_HDMI_DefaultActionSet out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_DelayGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_DELAY_S *pHdmiDelay)
{
    HDMI_INFO(">>> DRV_HDMI_DelayGet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pHdmiDelay);

    memcpy(pHdmiDelay, &pstHdmiDev->stDelay, sizeof(HDMI_DELAY_S));

    HDMI_INFO("<<< DRV_HDMI_DelayGet out...\n");

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_DelaySet(HDMI_DEVICE_S* pstHdmiDev, HDMI_DELAY_S *pHdmiDelay)
{
    HDMI_INFO(">>> DRV_HDMI_DelaySet in...\n");

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pHdmiDelay);

    memcpy(&pstHdmiDev->stDelay, pHdmiDelay, sizeof(HDMI_DELAY_S));

    HDMI_INFO("u32MuteDelay: %d\n", pHdmiDelay->u32MuteDelay);
    HDMI_INFO("u32FmtDelay: %d\n", pHdmiDelay->u32FmtDelay);
    HDMI_INFO("bForceFmtDelay: %d\n", pHdmiDelay->bForceFmtDelay);
    HDMI_INFO("bForceMuteDelay: %d\n", pHdmiDelay->bForceMuteDelay);

    HDMI_INFO("<<< DRV_HDMI_DelaySet out...\n");

    return HI_SUCCESS;
}

static HI_VOID HdmiSetAndroidState(HDMI_SWITCH_STATE enState)
{
#ifdef HDMI_ANDROID_SUPPORT
    if(s_bIsSwitchOn == HI_TRUE)
    {
        switch_set_state(&s_stSwitchDev, enState);
    }
#endif
}


static HI_S32 HdmiFileOpen(struct inode *inode, struct file *filp)
{
#ifndef HDMI_CHIP_HI3536
	HDMI_INFO(">>> HdmiFileOpen in...\n");
    atomic_inc_return(&s_HdmiCount);
    HDMI_INFO("s_HdmiCount:%d\n", s_HdmiCount);
    HDMI_INFO("<<< HdmiFileOpen out...\n");
#endif
    return HI_SUCCESS;
}

static HI_S32 HdmiFileClose(struct inode *inode, struct file *filp)
{
#ifndef HDMI_CHIP_HI3536
    HI_U32          u32HdmiId = 0;
    HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;

    HDMI_INFO(">>> HdmiFileClose in...\n");

    if (atomic_dec_and_test(&s_HdmiCount))
    {
        for(; u32HdmiId < HDMI_DEVICE_ID_BUTT; u32HdmiId++)
        {
            pstHdmiDev = GetHdmiDevice(u32HdmiId);
            if (pstHdmiDev != HI_NULL)
            {
                pstHdmiDev->u32UserCnt = 0;
                if (pstHdmiDev->enRunState & HDMI_RUN_STATE_OPEN)
                {
                    HdmiRelease(pstHdmiDev);
                }
                else
                {
                    HDMI_INFO("enRunState:%d of u32HdmiId[%d] is not open!\n", pstHdmiDev->enRunState, u32HdmiId);
                }
            }
            else
            {
                HDMI_ERR("pstHdmiDev[%d] is null!\n", u32HdmiId);
            }
        }
    }
    else
    {
        HDMI_INFO("s_HdmiCount=%d\n", s_HdmiCount);
        pstHdmiDev = GetHdmiDevice(u32HdmiId);
        if (pstHdmiDev != HI_NULL)
        {
            HDMI_INFO("u32UserCnt=%d\n", pstHdmiDev->u32UserCnt);
            DRV_HDMI_Close(pstHdmiDev,HI_TRUE);
        }
    }

    HDMI_INFO("<<< HdmiFileClose out...\n");
#endif
    return HI_SUCCESS;
}

#ifdef CONFIG_COMPAT
HI_S32 DRV_HDMI_CompatCmdProcess(unsigned int cmd, HI_VOID *arg, HI_BOOL bUser)
{
    HI_S32       s32Ret = HI_FAILURE;
    HI_VOID __user *argp = (HI_VOID __user *)arg;
    switch (cmd)
    {
#ifdef HDMI_HDCP_SUPPORT
        case CMD_HDMI_COMPAT_LOAD_KEY:
        {
            DRV_HDMI_LOAD_KEY_S stLoadKey;
            DRV_HDMI_COMPAT_LOAD_KEY_S *pstCompatLoadKey = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstCompatLoadKey = (DRV_HDMI_COMPAT_LOAD_KEY_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstCompatLoadKey->enHdmiID);

            stLoadKey.enHdmiID = pstCompatLoadKey->enHdmiID;
            stLoadKey.stKey.pu8InputEncryptedKeyAddr = (HI_U8 *)compat_ptr(pstCompatLoadKey->stKey.u32InputEncryptedKeyAddr);
            stLoadKey.stKey.u32KeyLength = pstCompatLoadKey->stKey.u32KeyLength;

            s32Ret = DRV_HDMI_KeyLoad(pstHdmiDev, &(stLoadKey.stKey));
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
#endif
        default:
            s32Ret = DRV_HDMI_CmdProcess(cmd, argp, bUser);
            break;
    }
    return s32Ret;
}
#endif

HI_S32 DRV_HDMI_CmdProcess(unsigned int cmd, HI_VOID *arg, HI_BOOL bUser)
{
    HI_S32       s32Ret = HI_FAILURE;
    HI_VOID __user *argp = (HI_VOID __user *)arg;

    switch (cmd)
    {
        case CMD_HDMI_INIT:
        {
            HDMI_MUTEX_LOCK(s_HdmiMutex);
            s32Ret = DRV_HDMI_Init();
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_DEINIT:
        {
            HDMI_MUTEX_LOCK(s_HdmiMutex);
            s32Ret = DRV_HDMI_DeInit();
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_OPEN:
        {
            HDMI_OPEN_S* pstOpen = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            pstOpen = (HDMI_OPEN_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstOpen->enHdmiID);

            DRV_HDMI_DefaultActionSet(pstHdmiDev, pstOpen->enDefaultMode);
            HDMI_INFO("CMD_HDMI_OPEN bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_Open(pstHdmiDev, bUser);

            break;
        }

        case CMD_HDMI_CLOSE:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);

            HDMI_INFO("CMD_HDMI_CLOSE bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_Close(pstHdmiDev, bUser);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_START:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);

            HDMI_INFO("CMD_HDMI_START bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_Start(pstHdmiDev);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_STOP:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);

            HDMI_INFO("CMD_HDMI_STOP bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_Stop(pstHdmiDev);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_GET_AUDIO_CAPABILITY:
        {
            HI_U32                      i = 0;
            HI_U32                      u32AudFmt = 0;
            HDMI_EDID_DATA_E            enEdidRet = HDMI_EDID_DATA_INVALID;
            HDMI_SINK_CAPABILITY_S      *pstSinkCap = HI_NULL;
            DRV_HDMI_AUDIO_CAPABILITY_S *pstDrvAudioCap = HI_NULL;
            HDMI_DEVICE_S               *pstHdmiDev = HI_NULL;

            pstDrvAudioCap = (DRV_HDMI_AUDIO_CAPABILITY_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvAudioCap->enHdmiID);
            if(HI_NULL == pstHdmiDev)
            {
                HDMI_ERR("pstHdmiDev is null.\n");
                return HI_FAILURE;
            }

            enEdidRet = DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap);

            HDMI_MEMSET(pstDrvAudioCap,0,sizeof(DRV_HDMI_AUDIO_CAPABILITY_S));

            if (enEdidRet != HDMI_EDID_DATA_INVALID)
            {
                for (i = 0 ;i < pstSinkCap->u32AudioInfoNum ; i++)
                {
                    u32AudFmt = pstSinkCap->stAudioInfo[i].enAudFmtCode;

                    if (u32AudFmt < HDMI_ARRAY_SIZE(pstDrvAudioCap->stAudioCap.bAudioFmtSupported))
                    {
                        pstDrvAudioCap->stAudioCap.bAudioFmtSupported[u32AudFmt] = HI_TRUE;
                    }

                    if ( HDMI_AUDIO_CODING_TYPE_PCM == u32AudFmt )
                    {
                        HDMI_MEMCPY(pstDrvAudioCap->stAudioCap.u32AudioSampleRateSupported,
                                    pstSinkCap->stAudioInfo[i].enSupportSampleRate,
                                    sizeof(pstDrvAudioCap->stAudioCap.u32AudioSampleRateSupported) );
                        pstDrvAudioCap->stAudioCap.u32MaxPcmChannels = pstSinkCap->stAudioInfo[i].u8AudChannel;
                    }
                }
                s32Ret = HI_SUCCESS;
            }
            else
            {
                HDMI_WARN("No HPD,Get Auido capabity fail\n");
                s32Ret = HI_FAILURE;
            }

            break;
        }
        case CMD_HDMI_GET_SINK_CAPABILITY:
        {
            HDMI_TX_CAPABILITY_S       stTxCap = {0};
            HDMI_SINK_CAPABILITY_S     *pstSinkCap = HI_NULL;
            DRV_HDMI_SINK_CAPABILITY_S *pstDrvSinkCap = HI_NULL;
            HDMI_DEVICE_S              *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvSinkCap = (DRV_HDMI_SINK_CAPABILITY_S*)argp;
            if(HI_NULL == pstDrvSinkCap)
            {
                HDMI_ERR("pstDrvSinkCap is null.\n");
				HDMI_MUTEX_UNLOCK(s_HdmiMutex);
                return HI_FAILURE;
            }

            pstHdmiDev = GetHdmiDevice(pstDrvSinkCap->enHdmiID);
            if(HI_NULL == pstHdmiDev)
            {
                HDMI_ERR("pstHdmiDev is null.\n");
				HDMI_MUTEX_UNLOCK(s_HdmiMutex);
                return HI_FAILURE;
            }
            if(HI_NULL == pstHdmiDev->pstHdmiHal)
            {
                HDMI_ERR("pstHdmiDev->pstHdmiHal is null.\n");
				HDMI_MUTEX_UNLOCK(s_HdmiMutex);
                return HI_FAILURE;
            }

            pstHdmiDev->pstHdmiHal->HAL_HDMI_TxCapabilityGet(pstHdmiDev->pstHdmiHal, &stTxCap);
            if (HDMI_EDID_DATA_INVALID != DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap))
            {
                HDMI_MEMCPY(&pstDrvSinkCap->stCap, pstSinkCap, sizeof(HDMI_SINK_CAPABILITY_S));
                s32Ret = HdmiCapabilityIntersection(&pstDrvSinkCap->stCap,&stTxCap, pstHdmiDev->stAttr.stAppAttr.bAuthMode);
            }
            else
            {
                HDMI_WARN("No HPD,Get sink capabity fail\n");
                s32Ret = HI_FAILURE;
            }
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_POLL_EVENT:
        {
            DRV_HDMI_EVENT_S *pstEvent = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev   = HI_NULL;

            pstEvent = (DRV_HDMI_EVENT_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstEvent->enHdmiID);
            if(HI_NULL != pstHdmiDev)
            {
                s32Ret = DRV_HDMI_EventPoolRead(&pstHdmiDev->stEventInfo, get_current()->tgid, &pstEvent->enEvent);
            }
            break;
        }

        case CMD_HDMI_GET_ATTR:
        {
            HDMI_ATTR_S      stHDMIAttr;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_APP_ATTR_S *pstDrvAppAttr = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvAppAttr = (DRV_HDMI_APP_ATTR_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvAppAttr->enHdmiID);

            memset((void*)&stHDMIAttr, 0, sizeof(HDMI_ATTR_S));
            s32Ret = DRV_HDMI_AttrGet(pstHdmiDev, &stHDMIAttr);
            HDMI_INFO("CMD_HDMI_GET_ATTR bUser=%d\n",bUser);
            memcpy(&pstDrvAppAttr->stAPPAttr,&stHDMIAttr.stAppAttr,sizeof(HDMI_APP_ATTR_S));
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_SET_ATTR:
        {
            HDMI_ATTR_S      stHDMIAttr;
            HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
            DRV_HDMI_APP_ATTR_S *pstDrvAppAttr = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvAppAttr = (DRV_HDMI_APP_ATTR_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvAppAttr->enHdmiID);

            s32Ret = DRV_HDMI_AttrGet(pstHdmiDev, &stHDMIAttr);
            memcpy(&stHDMIAttr.stAppAttr, &pstDrvAppAttr->stAPPAttr, sizeof(HDMI_APP_ATTR_S));
            HDMI_INFO("CMD_HDMI_SET_ATTR bUser=%d\n",bUser);
            s32Ret |= DRV_HDMI_AttrSet(pstHdmiDev, &stHDMIAttr);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);

            break;
        }

        case CMD_HDMI_GET_INFOFRAME:
        {
            HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
            DRV_HDMI_INFOFRAME_S *pInfoframe = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pInfoframe = (DRV_HDMI_INFOFRAME_S*)argp;
            pstHdmiDev = GetHdmiDevice(pInfoframe->enHdmiID);

            s32Ret = DRV_HDMI_InfoFrameGet(pstHdmiDev, pInfoframe->enInfoFrameId, &(pInfoframe->uInfoFrame));

            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_SET_INFOFRAME:
        {
            HDMI_DEVICE_S   *pstHdmiDev = HI_NULL;
            DRV_HDMI_INFOFRAME_S *pInfoframe = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pInfoframe = (DRV_HDMI_INFOFRAME_S*)argp;
            pstHdmiDev = GetHdmiDevice(pInfoframe->enHdmiID);

            s32Ret = DRV_HDMI_InfoFrameSet(pstHdmiDev, pInfoframe->enInfoFrameId, &(pInfoframe->uInfoFrame));

            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        case CMD_HDMI_SET_AVMUTE:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_AVMUTE_S* pstAvmute = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstAvmute = (DRV_HDMI_AVMUTE_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstAvmute->enHdmiID);

            DRV_HDMI_AVMuteSet(pstHdmiDev, pstAvmute->bAvMute);
			DRV_HDMI_AudioPathEnable(pstHdmiDev, !(pstAvmute->bAvMute));
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);

			s32Ret = HI_SUCCESS;
            break;
        }
		case CMD_HDMI_GET_VO_ATTR:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_VO_ATTR_S *pstDrvVOAttr = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvVOAttr = (DRV_HDMI_VO_ATTR_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvVOAttr->enHdmiID);

            HDMI_INFO("CMD_HDMI_GET_VO_ATTR bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_VOAttrGet(pstHdmiDev, &pstDrvVOAttr->stVOAttr);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_SET_VO_ATTR:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_VO_ATTR_S *pstDrvVOAttr = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);

            pstDrvVOAttr = (DRV_HDMI_VO_ATTR_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvVOAttr->enHdmiID);

            HDMI_INFO("CMD_HDMI_SET_VO_ATTR bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_VOAttrSet(pstHdmiDev, &pstDrvVOAttr->stVOAttr);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);

            break;
        }
#if 0
        case CMD_HDMI_PREVTIMING:
        {
            HDMI_VO_ATTR_S *pstPreVideoTiming;

            pstPreVideoTiming = (HDMI_VO_ATTR_S*)argp;
            s32Ret = DRV_HDMI_PreFormat(pstHdmiDev, pstPreVideoTiming);
            break;
        }
#endif
        case CMD_HDMI_GET_DEEPCOLOR:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_DEEP_COLOR_S *pstDrvDeepColor = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvDeepColor = (DRV_HDMI_DEEP_COLOR_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvDeepColor->enHdmiID);

            s32Ret = DRV_HDMI_DeepColorGet(pstHdmiDev, &pstDrvDeepColor->enDeepColorMode);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_SET_DEEPCOLOR:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            DRV_HDMI_DEEP_COLOR_S *pstDrvDeepColor = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvDeepColor = (DRV_HDMI_DEEP_COLOR_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvDeepColor->enHdmiID);

            s32Ret = DRV_HDMI_DeepColorSet(pstHdmiDev, pstDrvDeepColor->enDeepColorMode);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_SET_XVYCC:
        {
			s32Ret = HI_SUCCESS;
            break;
        }

#if defined(HDMI_CEC_SUPPORT)
        case CMD_HDMI_SET_CEC:
        {
            DRV_HDMI_CEC_CMD_S *pstCecCmd = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstCecCmd = (DRV_HDMI_CEC_CMD_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstCecCmd->enHdmiID);

            s32Ret = DRV_HDMI_CecCmdSend(pstHdmiDev, &pstCecCmd->CECCmd);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_GET_CEC:
        {
            DRV_HDMI_CEC_CMD_S *pstCecCmd = (DRV_HDMI_CEC_CMD_S*)argp;

            HDMI_DEVICE_S *pstHdmiDev = GetHdmiDevice(pstCecCmd->enHdmiID);

            /* may sleep in this function,so release the lock first*/
            s32Ret = DRV_HDMI_CecCmdReceive(pstHdmiDev, &pstCecCmd->CECCmd);

            break;
        }
        case CMD_HDMI_GET_CECSTATUS:
        {
            DRV_HDMI_CEC_INFO_S *pstCecStatus = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstCecStatus = (DRV_HDMI_CEC_INFO_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstCecStatus->enHdmiID);

            s32Ret = DRV_HDMI_CecStatusGet(pstHdmiDev, &pstCecStatus->stCecInfo);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_CEC_ENABLE:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);

            s32Ret = DRV_HDMI_CecEnableSet(pstHdmiDev,HI_TRUE);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_CEC_DISABLE:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);

            s32Ret = DRV_HDMI_CecEnableSet(pstHdmiDev,HI_FALSE);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
#endif
        case CMD_HDMI_UPDATE_EDID:
        {
            DRV_HDMI_EDID_RAWDATA_S *pstDrvEdidRaw = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvEdidRaw = (DRV_HDMI_EDID_RAWDATA_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvEdidRaw->enHdmiID);
            if(HI_NULL != pstHdmiDev)
            {
                /* to avoid read EDID fail when the HDCP is enable, don't read EDID by I2C. */
                if((HI_TRUE == pstHdmiDev->stHdcpInfo.bHdcpWasOn) || (HI_TRUE == pstHdmiDev->stHdcpInfo.bHdcpIsOn))
                {
                    HDMI_WARN("Don't read EDID by I2C when the HDCP is enable!\n");
                    s32Ret = HI_SUCCESS;
                }
                else
                {
                    s32Ret = DRV_HDMI_EdidUpdate(&pstHdmiDev->stEdidInfo, HDMI_EDID_UPDATE_SINK);
#ifdef HDMI_HDCP_SUPPORT
                    pstHdmiDev->pstHdmiHal->HAL_HDMI_HdcpSinkCapabilityGet(pstHdmiDev->pstHdmiHal, &(pstHdmiDev->stEdidInfo.stCapability.stHdcpSupport));
#endif
                }

                if (s32Ret == HI_SUCCESS)
                {
                    pstDrvEdidRaw->stEdidRaw.u32EdidLen = DRV_HDMI_EdidRawGet(&pstHdmiDev->stEdidInfo, pstDrvEdidRaw->stEdidRaw.u8Edid, HDMI_EDID_SIZE);
                    pstDrvEdidRaw->stEdidRaw.u8EdidValid = 1;
                }
            }
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_GET_HDMI_PLAYSTATUS:
        {
            DRV_HDMI_PLAYSTATUS_S* pstPlayStatus = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            pstPlayStatus = (DRV_HDMI_PLAYSTATUS_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstPlayStatus->enHdmiID);

            s32Ret = DRV_HDMI_PlayStatusGet(pstHdmiDev, &pstPlayStatus->u32Status);
            break;
        }
#if defined(HDMI_HDCP_SUPPORT)
		case CMD_HDMI_LOAD_KEY:
		{
            DRV_HDMI_LOAD_KEY_S *pstLoadKey = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            HDMI_MUTEX_LOCK(s_HdmiMutex);
			pstLoadKey = (DRV_HDMI_LOAD_KEY_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstLoadKey->enHdmiID);

            s32Ret = DRV_HDMI_KeyLoad(pstHdmiDev, &pstLoadKey->stKey);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
			break;
		}
#endif
        case CMD_HDMI_GET_AO_ATTR:
        {
            DRV_HDMI_AO_ATTR_S *pstDrvAOAttr = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvAOAttr = (DRV_HDMI_AO_ATTR_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvAOAttr->enHdmiID);

            HDMI_INFO("CMD_HDMI_GET_AO_ATTR bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_AOAttrGet(pstHdmiDev, &pstDrvAOAttr->stAOAttr);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_SET_AO_ATTR:
        {
            DRV_HDMI_AO_ATTR_S *pstDrvAOAttr = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvAOAttr = (DRV_HDMI_AO_ATTR_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvAOAttr->enHdmiID);

            HDMI_INFO("CMD_HDMI_SET_AO_ATTR bUser=%d\n",bUser);
            s32Ret = DRV_HDMI_AOAttrSet(pstHdmiDev, &pstDrvAOAttr->stAOAttr);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_GET_STATUS:
        {
            DRV_HDMI_STATUS_S *pstStatus = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstStatus = (DRV_HDMI_STATUS_S *)argp;
            pstHdmiDev = GetHdmiDevice(pstStatus->enHdmiID);

            s32Ret = DRV_HDMI_StatusGet(pstHdmiDev, &pstStatus->stHdmiStatus);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
#if defined (HDMI_DEFUALT_CALLBACK_SUPPORT)
        case CMD_HDMI_REGISTER_CALLBACK:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);
			if (pstHdmiDev)
			{
                pstHdmiDev->u32UserCallBackCnt++;
				HDMI_MUTEX_UNLOCK(s_HdmiMutex);
				HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
            	pstHdmiDev->pKCallback = HI_NULL;
				HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
				HDMI_MUTEX_LOCK(s_HdmiMutex);
				s32Ret= HI_SUCCESS;
			}
			else
			{
				s32Ret= HI_FAILURE;
			}
			HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

		case CMD_HDMI_UNREGISTER_CALLBACK:
        {
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;
            HDMI_MUTEX_LOCK(s_HdmiMutex);

            pstHdmiDev = GetHdmiDevice(*(HDMI_DEVICE_ID_E*)argp);
			if (pstHdmiDev)
			{
                /* BVT:  maybe call CMD_HDMI_UNREGISTER_CALLBACK only,
                   need to register pKCallback */
                if (pstHdmiDev->u32UserCallBackCnt > 0)
                {
                    pstHdmiDev->u32UserCallBackCnt--;
                }

                if (0 == pstHdmiDev->u32UserCallBackCnt)
                {
                    HDMI_MUTEX_UNLOCK(s_HdmiMutex);
                    HDMI_MUTEX_LOCK(pstHdmiDev->stMutexThread);
                    pstHdmiDev->pKCallback = HI_DRV_HDMI_KernelEventCallback;
                    HDMI_MUTEX_UNLOCK(pstHdmiDev->stMutexThread);
                    HDMI_MUTEX_LOCK(s_HdmiMutex);
                    s32Ret= HI_SUCCESS;
                }
			}
			else
			{
				s32Ret= HI_FAILURE;
			}
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
#endif
        case CMD_HDMI_GET_DELAY:
        {
            DRV_HDMI_DELAY_S *pstDelay = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDelay = (DRV_HDMI_DELAY_S *)arg;
            pstHdmiDev = GetHdmiDevice(pstDelay->enHdmiID);

            s32Ret= DRV_HDMI_DelayGet(pstHdmiDev,&pstDelay->stDelay);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
        case CMD_HDMI_SET_DELAY:
        {
            DRV_HDMI_DELAY_S *pstDelay = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDelay = (DRV_HDMI_DELAY_S *)arg;
            pstHdmiDev = GetHdmiDevice(pstDelay->enHdmiID);

            s32Ret= DRV_HDMI_DelaySet(pstHdmiDev,&pstDelay->stDelay);
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }
#ifdef HDMI_HDR_SUPPORT
        case CMD_HDMI_SET_HDR_ATTR:
        {
            DRV_HDMI_HDR_S  *pstHdrAttr = (DRV_HDMI_HDR_S*) arg;                // from intf_k.c
            HDMI_DEVICE_S   *pstHdmiDev = GetHdmiDevice(pstHdrAttr->enHdmiID);  // find HdmiDevice by HdmiID
            s32Ret = DRV_HDMI_HdrAttrSet(pstHdmiDev, pstHdrAttr);
            break;
        }
#endif
        case CMD_HDMI_GET_VIDEO_CAPABILITY:
        {
            HDMI_EDID_DATA_E             enEdidRet = HDMI_EDID_DATA_INVALID;
            HDMI_SINK_CAPABILITY_S      *pstSinkCap = HI_NULL;
            DRV_HDMI_VIDEO_CAPABILITY_S *pstDrvVideoCap = HI_NULL;
            HDMI_DEVICE_S *pstHdmiDev = HI_NULL;

            HDMI_MUTEX_LOCK(s_HdmiMutex);
            pstDrvVideoCap = (DRV_HDMI_VIDEO_CAPABILITY_S*)argp;
            pstHdmiDev = GetHdmiDevice(pstDrvVideoCap->enHdmiID);
            if(HI_NULL != pstHdmiDev)
            {
                enEdidRet = DRV_HDMI_EdidCapabilityGet(&pstHdmiDev->stEdidInfo, &pstSinkCap);

                HDMI_MEMSET(pstDrvVideoCap, 0, sizeof(DRV_HDMI_VIDEO_CAPABILITY_S));

                if (enEdidRet != HDMI_EDID_DATA_INVALID)
                {
                    HdmiVoCapabilityIntersection(pstDrvVideoCap, pstSinkCap);
                    s32Ret = HI_SUCCESS;
                }
                else
                {
                    HDMI_WARN("Get video capabity fail\n");
                    s32Ret = HI_FAILURE;
                }
            }
            HDMI_MUTEX_UNLOCK(s_HdmiMutex);
            break;
        }

        default:
        {
            HDMI_ERR("unkonw cmd:0x%x\n", cmd);
            return -ENOIOCTLCMD;
        }
    }

    if (s32Ret != HI_SUCCESS)
    {
        HDMI_WARN("s32Ret != HI_SUCCESS!\n");
    }

    return s32Ret;
}

#if defined(CONFIG_HDMI_STB_SDK)
#ifndef CONFIG_COMPAT
static HI_S32 HdmiIoctl(struct inode *inode, struct file *file,
                           unsigned int cmd, HI_VOID *arg)
{
    HI_S32   s32Ret = HI_FAILURE;

    HDMI_INFO(">>> HdmiIoctl in...\n");

    s32Ret = DRV_HDMI_CmdProcess(cmd, arg, HI_TRUE);

    HDMI_INFO("<<< HdmiIoctl out...\n");

    return s32Ret;
}
#else
static HI_S32 HdmiCompactIoctl(struct inode *inode, struct file *file,
                           unsigned int cmd, HI_VOID *arg)
{
    HI_S32   s32Ret = HI_FAILURE;

    HDMI_INFO(">>> HdmiIoctl in...\n");

    s32Ret = DRV_HDMI_CompatCmdProcess(cmd, arg, HI_TRUE);

    HDMI_INFO("<<< HdmiIoctl out...\n");

    return s32Ret;
}
#endif

#elif defined(CONFIG_HDMI_BVT_SDK)
static HI_S32 HdmiIoctl(struct file *file,
                           unsigned int cmd,long unsigned int arg)
{
    HI_S32   s32Ret = HI_FAILURE;

    HDMI_INFO(">>> HdmiIoctl in...\n");

    s32Ret = DRV_HDMI_CmdProcess(cmd, (HI_VOID*)arg, HI_TRUE);

    HDMI_INFO("<<< HdmiIoctl out...\n");

    return s32Ret;
}
#endif

#ifdef HDMI_CHIP_HI3536
HI_S32 Hdmi_Init(HI_VOID* pArgs)
{
    atomic_inc_return(&s_HdmiCount);
    HDMI_INFO("s_HdmiCount:%d\n", s_HdmiCount);

    return 0;
}

HI_VOID Hdmi_Exit(HI_VOID)
{
    HI_U32 u32HdmiId = 0;
    HDMI_DEVICE_S* pstHdmiDev;

    if (atomic_dec_and_test(&s_HdmiCount))
    {
        for(u32HdmiId = 0; u32HdmiId < HDMI_DEVICE_ID_BUTT; u32HdmiId++)
        {
            pstHdmiDev = GetHdmiDevice(u32HdmiId);
            if (pstHdmiDev != HI_NULL)
            {
                pstHdmiDev->u32UserCnt = 0;
                if (pstHdmiDev->enRunState & HDMI_RUN_STATE_OPEN)
                {
                    HdmiRelease(pstHdmiDev);
                }
                else
                {
                    HDMI_INFO("enRunState:%d of u32HdmiId[%d] is not open!\n", pstHdmiDev->enRunState, u32HdmiId);
                }
            }
            else
            {
                HDMI_ERR("pstHdmiDev[%d] is null!\n", u32HdmiId);
            }
        }
    }
    else
    {
        HDMI_INFO("s_HdmiCount:%d\n", s_HdmiCount);
    }

    return;
}

static HI_VOID Hdmi_Notify(MOD_NOTICE_ID_E enNotice)
{
    return ;
}

static HI_VOID Hdmi_QueryState(MOD_STATE_E *pstState)
{
    return ;
}

static HI_U32 Hdmi_GetVerMagic(HI_VOID)
{
	return VERSION_MAGIC;
}

static UMAP_MODULE_S s_stModule =
{
    .pstOwner = THIS_MODULE,
    .enModId = HI_ID_HDMI,
    .aModName = "hdmi",

    .pfnInit = Hdmi_Init,
    .pfnExit = Hdmi_Exit,
    .pfnQueryState = Hdmi_QueryState,
    .pfnNotify = Hdmi_Notify,
    .pfnVerChecker = Hdmi_GetVerMagic,

    .pstExportFuncs = HI_NULL,
    .pData = HI_NULL,
};
#endif

static long  HdmiFileIoctl(struct file *file,unsigned int cmd, unsigned long arg)
{
#ifndef CONFIG_COMPAT
    return (long) HI_HDMI_UserCopy(file, cmd, arg, HdmiIoctl);
#else
    return (long) HI_HDMI_UserCopy(file, cmd, arg, HdmiCompactIoctl);
#endif
}

#if (1 == HI_PROC_SUPPORT)
HDMI_PROC_ITEM_S s_pHdmiProcItem =
{
	.fnRead 	= DRV_HDMI_ReadProc,
#ifdef HDMI_CHIP_HI3536
	.fnWrite	= HI_NULL,
#else
    .fnWrite    = DRV_HDMI_DebugSourceWrite,
#endif
	.fnIoctl	= HI_NULL,
};

HDMI_PROC_ITEM_S s_pVOProcItem =
{
	.fnRead 	= DRV_HDMI_VOReadProc,
	.fnWrite	= HI_NULL,
	.fnIoctl	= HI_NULL,
};

HDMI_PROC_ITEM_S s_pAOProcItem =
{
	.fnRead 	= DRV_HDMI_AOReadProc,
	.fnWrite	= HI_NULL,
	.fnIoctl	= HI_NULL,
};

HDMI_PROC_ITEM_S s_pSinkProcItem =
{
	.fnRead 	= DRV_HDMI_SinkReadProc,
	.fnWrite	= HI_NULL,
	.fnIoctl	= HI_NULL,
};

HI_VOID DRV_HDMI_PROC_Register(HI_U32 u32HdmiId)
{
    HI_CHAR procName[16];

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d", "hdmi", u32HdmiId);
    HI_HDMI_PROC_AddModule(procName, &s_pHdmiProcItem, HI_NULL);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_vo");
    HI_HDMI_PROC_AddModule(procName, &s_pVOProcItem, HI_NULL);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_ao");
    HI_HDMI_PROC_AddModule(procName, &s_pAOProcItem, HI_NULL);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_sink");
    HI_HDMI_PROC_AddModule(procName, &s_pSinkProcItem, HI_NULL);

    return;
}

HI_VOID DRV_HDMI_PROC_UnRegister(HI_U32 u32HdmiId)
{
    HI_CHAR procName[16];

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d", "hdmi", u32HdmiId);
    HI_HDMI_PROC_RemoveModule(procName);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_vo");
    HI_HDMI_PROC_RemoveModule(procName);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_ao");
    HI_HDMI_PROC_RemoveModule(procName);

    memset(procName, 0, sizeof(procName));
    snprintf(procName, sizeof(procName), "%s%d%s", "hdmi", u32HdmiId, "_sink");
    HI_HDMI_PROC_RemoveModule(procName);

    return;
}
#endif


HI_S32 HDMI_DRV_ModInit(HI_VOID)
{
    HI_S32 s32Ret = HI_FAILURE;
#if (1 == HI_PROC_SUPPORT)
    HI_U32 u32HdmiId = 0;
#endif

#ifdef HDMI_DEBUG
    DRV_HDMI_LogInit();
#endif
	//sema_init(&s_HdmiMutex, 1);

    /* clear*/
    //memset(&s_stHdmiCtrl[0], 0, sizeof(HDMI_DEVICE_S)*HDMI_DEVICE_ID_BUTT);
    atomic_set(&s_HdmiCount, 0);

#if defined(CONFIG_HDMI_BVT_SDK)
    if (CMPI_RegisterModule(&s_stModule))
    {
        HDMI_WARN("CMPI_RegisterModule hdmi fail \n");
        return HI_FAILURE;
    }
#endif

    /* register hdmi device*/
    snprintf(s_stHdmiDev.devfs_name, sizeof(s_stHdmiDev.devfs_name), "%s", DEVNAME_HDMI);
    s32Ret = HI_HDMI_DEV_Register(s_stHdmiDev);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_WARN("s_stHdmiDev register fail!\n");
    }
#if (1 == HI_PROC_SUPPORT)
    /* register hdmi proc*/
    for(; u32HdmiId < HDMI_DEVICE_ID_BUTT; u32HdmiId++)
    {
        DRV_HDMI_PROC_Register(u32HdmiId);
    }
#endif
#ifndef HI_MCE_SUPPORT
    HI_DRV_HDMI_Init();
#endif

#ifdef HDMI_ANDROID_SUPPORT
    //android only
    if (switch_dev_register(&s_stSwitchDev))
    {
        s_bIsSwitchOn = HI_FALSE;
    }
    else
    {
        s_bIsSwitchOn = HI_TRUE;
        HdmiSetAndroidState(STATE_PLUG_UNKNOWN);
    }
#endif
#if (defined(MODULE) && defined(CONFIG_HDMI_STB_SDK))
    HI_PRINT("Load hi_hdmi.ko success.  \t(%s)\n", VERSION_STRING);
#endif
    return HI_SUCCESS;
}

HI_VOID HDMI_DRV_ModExit(HI_VOID)
{
    HI_U32 u32HdmiId = 0;
    HDMI_DEVICE_S *pstHdmiDev;

#ifdef HDMI_ANDROID_SUPPORT
    //android only
    if(s_bIsSwitchOn == HI_TRUE)
    {
        switch_dev_unregister(&s_stSwitchDev);
    }
#endif


#ifndef HI_MCE_SUPPORT
    HI_DRV_HDMI_Deinit();
#endif


    for(; u32HdmiId < HDMI_DEVICE_ID_BUTT; u32HdmiId++)
    {
        pstHdmiDev = GetHdmiDevice(u32HdmiId);
        if (pstHdmiDev != HI_NULL)
        {
            if (pstHdmiDev->enRunState & HDMI_RUN_STATE_OPEN)
            {
                pstHdmiDev->u32UserCnt = pstHdmiDev->u32KernelCnt = 0;
                HdmiRelease(pstHdmiDev);
                memset(pstHdmiDev, 0, sizeof(HDMI_DEVICE_S));
            }
        }
#if (1 == HI_PROC_SUPPORT)
        DRV_HDMI_PROC_UnRegister(u32HdmiId);
#endif
    }

    HI_HDMI_DEV_UnRegister(s_stHdmiDev);

#if defined(CONFIG_HDMI_BVT_SDK)
    CMPI_UnRegisterModule(HI_ID_HDMI);
#endif

#ifdef HDMI_DEBUG
    DRV_HDMI_LogDeInit();
#endif
}

#ifdef MODULE
module_init(HDMI_DRV_ModInit);
module_exit(HDMI_DRV_ModExit);
#endif
MODULE_LICENSE("GPL");
