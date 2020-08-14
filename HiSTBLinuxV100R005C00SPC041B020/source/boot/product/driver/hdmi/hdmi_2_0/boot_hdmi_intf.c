#include "hi_drv_hdmi.h"
#include "hi_unf_hdmi.h"
#include "hi_drv_disp.h"
#include "drv_hdmi_common.h"
#include "boot_hdmi_intf.h"
#include "hi_drv_pdm.h"
#include "hdmi_emi.h"

#define HDMI_MULTIPLE_2P0(x)         (x*=2)
#define HDMI_MULTIPLE_1P5(x)         (x=(x * 3) >> 1)
#define HDMI_MULTIPLE_1P25(x)        (x=(x * 5) >> 2)
#define HDMI_MULTIPLE_0P5(x)         (x>>=1)
#define HDMI_MAX_HDMI14_TMDS_RATE    340000
#define HDMI_MIN_HDMI14_TMDS_RATE    25000

#define CHECK_POINTER(p) \
do{                      \
    if(HI_NULL == p){\
        HDMI_ERR("Error In %s,Line %d\n",__FUNCTION__,__LINE__);       \
        return HI_FAILURE;\
    } \
}while(0)

#define HDMI_UDEEPCOLO_2_KDEEPCOLOR(udeepcolor, kdeepcolor) \
do{\
    if (HI_UNF_HDMI_DEEP_COLOR_24BIT == udeepcolor)\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_24BIT;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_30BIT == udeepcolor)\
    {\
        kdeepcolor = HDMI_DEEP_COLOR_30BIT;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_36BIT == udeepcolor) \
    {\
        kdeepcolor = HDMI_DEEP_COLOR_36BIT;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_OFF == udeepcolor) \
    {\
        kdeepcolor = HDMI_DEEP_COLOR_OFF;\
    }\
    else if (HI_UNF_HDMI_DEEP_COLOR_BUTT == udeepcolor) \
    {\
        kdeepcolor = HDMI_DEEP_COLOR_BUTT;\
    }\
}while(0)

static HDMI_DEVICE_S s_stHdmiDev[HDMI_DEVICE_ID_BUTT];

typedef struct FMT_TO_TIMING_S
{
    HI_DRV_DISP_FMT_E       enFmt;
    HDMI_VIDEO_TIMING_E     enTiming;
}HDMI_FMT_TO_TIMING_S;

static HDMI_FMT_TO_TIMING_S g_enFmtToTiming[] =
{
	{HI_DRV_DISP_FMT_1080P_60,          HDMI_VIDEO_TIMING_1920X1080P_60000 },
	{HI_DRV_DISP_FMT_1080P_59_94,       HDMI_VIDEO_TIMING_1920X1080P_60000 },
	{HI_DRV_DISP_FMT_1080P_50,          HDMI_VIDEO_TIMING_1920X1080P_50000 },
	{HI_DRV_DISP_FMT_1080P_30,          HDMI_VIDEO_TIMING_1920X1080P_30000 },
	{HI_DRV_DISP_FMT_1080P_29_97,       HDMI_VIDEO_TIMING_1920X1080P_30000 },
	{HI_DRV_DISP_FMT_1080P_25,          HDMI_VIDEO_TIMING_1920X1080P_25000 },
	{HI_DRV_DISP_FMT_1080P_24,          HDMI_VIDEO_TIMING_1920X1080P_24000 },
	{HI_DRV_DISP_FMT_1080P_23_976,      HDMI_VIDEO_TIMING_1920X1080P_24000 },
	{HI_DRV_DISP_FMT_1080P_24_FP,       HDMI_VIDEO_TIMING_1920X1080P_24000 },
	{HI_DRV_DISP_FMT_1080i_60,          HDMI_VIDEO_TIMING_1920X1080I_60000 },
	{HI_DRV_DISP_FMT_1080i_59_94,       HDMI_VIDEO_TIMING_1920X1080I_60000 },
	{HI_DRV_DISP_FMT_1080i_50,          HDMI_VIDEO_TIMING_1920X1080I_50000 },
	{HI_DRV_DISP_FMT_720P_60,           HDMI_VIDEO_TIMING_1280X720P_60000  },
	{HI_DRV_DISP_FMT_720P_59_94,        HDMI_VIDEO_TIMING_1280X720P_60000  },
	{HI_DRV_DISP_FMT_720P_60_FP,        HDMI_VIDEO_TIMING_1280X720P_60000  },
	{HI_DRV_DISP_FMT_720P_50,           HDMI_VIDEO_TIMING_1280X720P_50000  },
	{HI_DRV_DISP_FMT_720P_50_FP,        HDMI_VIDEO_TIMING_1280X720P_50000  },
	{HI_DRV_DISP_FMT_576P_50,           HDMI_VIDEO_TIMING_720X576P_50000   },
	{HI_DRV_DISP_FMT_480P_60,           HDMI_VIDEO_TIMING_720X480P_60000   },
	{HI_DRV_DISP_FMT_3840X2160_24,      HDMI_VIDEO_TIMING_3840X2160P_24000 },
	{HI_DRV_DISP_FMT_3840X2160_23_976,  HDMI_VIDEO_TIMING_3840X2160P_24000 },
	{HI_DRV_DISP_FMT_3840X2160_25,      HDMI_VIDEO_TIMING_3840X2160P_25000 },
	{HI_DRV_DISP_FMT_3840X2160_30,      HDMI_VIDEO_TIMING_3840X2160P_30000 },
	{HI_DRV_DISP_FMT_3840X2160_29_97,   HDMI_VIDEO_TIMING_3840X2160P_30000 },
	{HI_DRV_DISP_FMT_4096X2160_24,      HDMI_VIDEO_TIMING_4096X2160P_24000 },
	{HI_DRV_DISP_FMT_4096X2160_25,      HDMI_VIDEO_TIMING_4096X2160P_25000 },
	{HI_DRV_DISP_FMT_4096X2160_30,      HDMI_VIDEO_TIMING_4096X2160P_30000 },
	{HI_DRV_DISP_FMT_3840X2160_50,      HDMI_VIDEO_TIMING_3840X2160P_50000 },
	{HI_DRV_DISP_FMT_3840X2160_60,      HDMI_VIDEO_TIMING_3840X2160P_60000 },
	{HI_DRV_DISP_FMT_4096X2160_50,      HDMI_VIDEO_TIMING_4096X2160P_50000 },
	{HI_DRV_DISP_FMT_4096X2160_60,      HDMI_VIDEO_TIMING_4096X2160P_60000 },
	{HI_DRV_DISP_FMT_1440x576i_50,      HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL,               HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_B,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_B1,            HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_D,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_D1,            HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_G,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_H,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_K,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_I,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_M,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_N,             HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_Nc,            HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_PAL_60,            HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_SIN,         HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_COS,         HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_L,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_B,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_G,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_D,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_K,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_SECAM_H,           HDMI_VIDEO_TIMING_1440X576I_50000  },
	{HI_DRV_DISP_FMT_1440x480i_60,      HDMI_VIDEO_TIMING_1440X480I_60000  },
	{HI_DRV_DISP_FMT_NTSC,              HDMI_VIDEO_TIMING_1440X480I_60000  },
	{HI_DRV_DISP_FMT_NTSC_J,            HDMI_VIDEO_TIMING_1440X480I_60000  },
	{HI_DRV_DISP_FMT_NTSC_443,          HDMI_VIDEO_TIMING_1440X480I_60000  },
	{HI_DRV_DISP_FMT_861D_640X480_60,   HDMI_VIDEO_TIMING_640X480P_60000   },
	{HI_DRV_DISP_FMT_CUSTOM,            HDMI_VIDEO_TIMING_UNKNOWN          },
	{HI_DRV_DISP_FMT_BUTT,              HDMI_VIDEO_TIMING_UNKNOWN          },

};

HDMI_VIDEO_TIMING_E DispFmt2HdmiTiming(HI_U32 u32DispFmt)
{
    HI_U32 i = 0;
    HDMI_VIDEO_TIMING_E enVideoTimingMode = HDMI_VIDEO_TIMING_UNKNOWN;

    for(i = 0; i < (sizeof(g_enFmtToTiming) / sizeof(g_enFmtToTiming[0])); i++)
    {
        if(u32DispFmt == g_enFmtToTiming[i].enFmt)
        {
            enVideoTimingMode = g_enFmtToTiming[i].enTiming;
        }
    }

    return enVideoTimingMode;
}

static HI_VOID HdmiVOAttrInit(HDMI_VO_ATTR_S *pstVideoAttr)
{
    HI_BOOL bColorimetryIs601 = HI_FALSE;

    //pstVideoAttr->enVideoTiming = DispFmt2HdmiTiming(u32DispFmt);

    bColorimetryIs601 = ( (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_720X480P_60000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_720X576P_50000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X240P_60000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X288P_50000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X480I_60000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X576I_50000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X576P_50000)
                       || (pstVideoAttr->enVideoTiming == HDMI_VIDEO_TIMING_1440X576I_60000));

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

    HDMI_INFO("enVideoTiming:%d, enColorimetry:%d, enPictureAspect:%d\n", pstVideoAttr->enVideoTiming, pstVideoAttr->enColorimetry, pstVideoAttr->enPictureAspect);

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

    if (pstVideoAttr->enVideoTiming != HDMI_VIDEO_TIMING_UNKNOWN &&
        pstVideoAttr->enVideoTiming != HDMI_VIDEO_TIMING_640X480P_60000)
    {
        pstVideoAttr->enInColorSpace = HDMI_COLORSPACE_YCbCr444;
    }
    else
    {
        pstVideoAttr->enInColorSpace = HDMI_COLORSPACE_RGB;
    }

    pstVideoAttr->enStereoMode    = HDMI_3D_BUTT;
    pstVideoAttr->enInBitDepth    = HDMI_VIDEO_BITDEPTH_10;
    pstVideoAttr->enActiveAspect  = HDMI_ACTIVE_ASPECT_PICTURE;
}


static HI_VOID DispFmt2VOAttr(HI_U32 u32DispFmt, HDMI_VO_ATTR_S *pstVideoAttr)
{
    pstVideoAttr->enVideoTiming = DispFmt2HdmiTiming(u32DispFmt);
    HdmiVOAttrInit(pstVideoAttr);
}

static HI_S32 HdmiDeviceInit(HDMI_DEVICE_S * pstHdmiDev)
{
    HDMI_AO_ATTR_S*          pstAudioAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
    HDMI_APP_ATTR_S*         pstAppAttr;
    CHECK_POINTER(pstHdmiDev);

    pstAudioAttr = &pstHdmiDev->stAttr.stAOAttr;
    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;
    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;

    pstHdmiDev->enTmdsMode           = HDMI_TMDS_MODE_HDMI_1_4;
    /* application attribute init */
    pstAppAttr->bEnableHdmi          = HI_TRUE;
    pstAppAttr->bEnableClrSpaceAdapt = HI_TRUE;
    pstAppAttr->bEnableDeepClrAdapt  = HI_TRUE;

    /* video attribute init*/
    pstVideoAttr->enVideoTiming = HDMI_VIDEO_TIMING_1280X720P_50000;
    HdmiVOAttrInit(pstVideoAttr);

    /* audio attribute init*/
    pstAudioAttr->enSoundIntf   = HDMI_AUDIO_INTERFACE__I2S;
    pstAudioAttr->enSampleFs    = HDMI_SAMPLE_RATE_48K;
    pstAudioAttr->enSampleDepth = HDMI_AUDIO_BIT_DEPTH_16;
    pstAudioAttr->enChanels     = HDMI_AUDIO_FORMAT_2CH;

    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_Open(HDMI_DEVICE_S* pstHdmiDev)
{
    HI_S32 s32Ret = HI_FAILURE;
    CHECK_POINTER(pstHdmiDev);

    /* open hdmi hal module*/
    s32Ret = HAL_HDMI_Open(HI_NULL, &pstHdmiDev->pstHdmiHal);
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_ERR("HAL_HDMI_Open fail\n");
        return HI_FAILURE;
    }

    HdmiDeviceInit(pstHdmiDev);
    pstHdmiDev->pstHdmiHal->stHalCtx.hHdmiDev = pstHdmiDev;
    pstHdmiDev->pstHdmiHal->stHalCtx.u32HdmiID = pstHdmiDev->u32HdmiDevId;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_HardwareInit(pstHdmiDev->pstHdmiHal);
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_AVMuteSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bAvMute)
{
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_AvMuteSet(pstHdmiDev->pstHdmiHal, bAvMute);
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_Start(HDMI_DEVICE_S* pstHdmiDev)
{
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyPowerEnableSet(pstHdmiDev->pstHdmiHal, HI_TRUE);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_TmdsModeSet(pstHdmiDev->pstHdmiHal, pstHdmiDev->enTmdsMode);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_TRUE);
    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_FALSE);

    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_Stop(HDMI_DEVICE_S* pstHdmiDev)
{
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);
    DRV_HDMI_AVMuteSet(pstHdmiDev, HI_TRUE);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyPowerEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_Close(HDMI_DEVICE_S* pstHdmiDev)
{
    CHECK_POINTER(pstHdmiDev);

    DRV_HDMI_Stop(pstHdmiDev);
    HAL_HDMI_Close(pstHdmiDev->pstHdmiHal);
    return HI_SUCCESS;
}



#if 0
static HI_S32 CheckAudioAttr(HDMI_AO_ATTR_S *pstAOAttr)
{
    if (HDMI_AUDIO_INTERFACE__I2S != pstAOAttr->enSoundIntf
       && HDMI_AUDIO_INTERFACE__SPDIF != pstAOAttr->enSoundIntf
       && HDMI_AUDIO_INTERFACE__HBRA != pstAOAttr->enSoundIntf)
    {
        HDMI_COM_ERR("The Audio interface(%d) is invalid\n",pstAOAttr->enSoundIntf);
        return HI_FAILURE;
    }

    if (pstAOAttr->enChanels < HDMI_AUDIO_FORMAT_2CH || pstAOAttr->enChanels > HDMI_AUDIO_FORMAT_8CH)
    {
        HDMI_COM_ERR("The Audio chanel number(%d) is invalid\n",pstAOAttr->enChanels);
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
            HDMI_COM_ERR("The input Audio Frequency(%d) is invalid\n",pstAOAttr->enSampleFs);
            return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 HdmiAudioPathSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_AO_ATTR_S *pstAOAttr)
{
    HDMI_AUDIO_CONFIG_S      stAudioCfg;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAOAttr);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    stAudioCfg.bDownSample   = pstAOAttr->bDownSample;
    stAudioCfg.enLayout      = pstAOAttr->enChanels;
    stAudioCfg.enSampleDepth = pstAOAttr->enSampleDepth;
    stAudioCfg.enSampleFs    = pstAOAttr->enSampleFs;
    stAudioCfg.enSoundIntf   = pstAOAttr->enSoundIntf;
    pstHdmiDev->pstHdmiHal->HAL_HDMI_AudioPathSet(pstHdmiDev->pstHdmiHal, &stAudioCfg);

    return HI_SUCCESS;

}



HI_S32 DRV_HDMI_AOAttrSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_AO_ATTR_S *pstAOAttr)
{
    HDMI_AO_ATTR_S*          pstAudioAttr;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAOAttr);

    if (HI_SUCCESS != CheckAudioAttr(pstAOAttr))
    {
        HDMI_COM_ERR("HDMI_CheckAudioAttr fail\n");
        return HI_FAILURE;
    }

    pstAudioAttr = &pstHdmiDev->stAttr.stAOAttr;
#if 0
    if (!memcmp(pstAOAttr, pstAudioAttr, sizeof(HDMI_AO_ATTR_S)))
    {
        HDMI_COM_INFO("The Audio Attr is not change\n");
        return HI_SUCCESS;
    }
#endif
    HdmiAudioPathSet(pstHdmiDev, pstAOAttr);
    memcpy(pstAudioAttr, pstAOAttr, sizeof(HDMI_AO_ATTR_S));

    HDMI_AudioInfoFrameSet(&pstHdmiDev->stInfoFrame);

    return HI_SUCCESS;
}

#endif


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

    if (/*pstVOAttr->enColorimetry <= HDMI_COLORIMETRY__NONE || */
        pstVOAttr->enColorimetry > HDMI_COLORIMETRY__EXTENDED)
    {
        HDMI_ERR("Hdmi video Colorimetry(%d) is wrong\n", pstVOAttr->enColorimetry);
        return HI_FAILURE;
    }

    if (pstVOAttr->enRGBQuantization != HDMI_QUANTIZATION_RANGE_DEFAULT
        && pstVOAttr->enRGBQuantization != HDMI_QUANTIZATION_RANGE_LIMITED
        && pstVOAttr->enRGBQuantization != HDMI_QUANTIZATION_RANGE_FULL)
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

    if (pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_8
        && pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_10
        && pstVOAttr->enInBitDepth != HDMI_VIDEO_BITDEPTH_12)
    {
        HDMI_ERR("Hdmi video InBitDepth(%d) is wrong\n", pstVOAttr->enInBitDepth);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}


static HI_S32 HdmiVideoPathSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_VO_ATTR_S* pstVOAttr)
{
    HDMI_VIDEO_CONFIG_S stVideoCfg = {0};
    HDMI_APP_ATTR_S*    pstAppAttr;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstVOAttr);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;

    stVideoCfg.u32TmdsClk      = pstVOAttr->u32HdmiAdaptPixClk;
    stVideoCfg.u32PixelClk     = pstVOAttr->u32ClkFs;
    stVideoCfg.enInBitDepth    = pstVOAttr->enInBitDepth;
    stVideoCfg.enQuantization  = pstVOAttr->enRGBQuantization;
    stVideoCfg.enInColorSpace  = pstVOAttr->enInColorSpace;
    stVideoCfg.enDeepColor     = pstAppAttr->enDeepColorMode;
    stVideoCfg.enOutColorSpace = pstAppAttr->enOutColorSpace;
    stVideoCfg.bVSyncPol       = pstVOAttr->bVSyncPol;
    stVideoCfg.bHSyncPol       = pstVOAttr->bHSyncPol;
    stVideoCfg.bDEPol          = pstVOAttr->bDEPol;
#ifdef HDMI_EMI_ENABLE
    stVideoCfg.bEmiEnable      = HI_TRUE;
#else
    stVideoCfg.bEmiEnable      = HI_FALSE;
#endif

    switch (pstVOAttr->enColorimetry)
    {
        case HDMI_COLORIMETRY__ITU_601:
            stVideoCfg.enConvStd = HDMI_CONV_STD_BT_601;
            break;
        case HDMI_COLORIMETRY__ITU_709:
            stVideoCfg.enConvStd = HDMI_CONV_STD_BT_709;
            break;
        case HDMI_COLORIMETRY__EXTENDED:
            if (pstVOAttr->enExtendedColorimetry == HDMI_EXTENDED_COLORIMETRY_2020_non_const_luminous)
                stVideoCfg.enConvStd = HDMI_CONV_STD_BT_2020_non_const_luminous;
            else if (pstVOAttr->enExtendedColorimetry == HDMI_EXTENDED_COLORIMETRY_2020_const_luminous)
                stVideoCfg.enConvStd = HDMI_CONV_STD_BT_2020_const_luminous;
            break;
        default:
            break;
    }
    /* Video will mute and unmute in this function call*/
    pstHdmiDev->pstHdmiHal->HAL_HDMI_VideoPathSet(pstHdmiDev->pstHdmiHal, &stVideoCfg);
    return HI_SUCCESS;
}

HI_S32 DRV_HDMI_AttrGet(HDMI_DEVICE_S* pstHdmiDev, HDMI_ATTR_S *pstAttr)
{
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAttr);

    memcpy(pstAttr, &pstHdmiDev->stAttr, sizeof(HDMI_ATTR_S));
    return HI_SUCCESS;
}

#ifdef HDMI_SCDC_SUPPORT
#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
static HI_S32 HdmiScrambleEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HDMI_SCDC_STATUS_S      stScdcStatus;
    stScdcStatus.u32ScrambleInterval    = 20;
    stScdcStatus.u32ScrambleTimeout     = 200;
    stScdcStatus.bSinkReadQuest         = HI_FALSE;
    stScdcStatus.bSinkScrambleOn        = bEnable;
    stScdcStatus.bSourceScrambleOn      = bEnable;
    stScdcStatus.u8TmdsBitClkRatio      = bEnable ? 40 : 10;
    if(pstHdmiDev->pstHdmiHal && pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcStatusSet)
        pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcStatusSet(pstHdmiDev->pstHdmiHal, &stScdcStatus);
    return HI_SUCCESS;
}
#else
static HI_S32 HdmiScrambleEnableSet(HDMI_DEVICE_S* pstHdmiDev, HI_BOOL bEnable)
{
    HDMI_SCDC_CONFIG_S       stScdcConfig;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    memset(&stScdcConfig, 0, sizeof(HDMI_SCDC_CONFIG_S));
    stScdcConfig.bScdcEnable             = bEnable;
    stScdcConfig.bScdcPresent            = HI_TRUE;
    stScdcConfig.bRRCapable              = HI_FALSE;
    stScdcConfig.bLTE340McscScramble     = HI_FALSE;

    if (bEnable)
    {
        stScdcConfig.u32MaxTmdsCharacterRate = 600 * 1000000;
        pstHdmiDev->pstHdmiHal->HAL_HDMI_PhyOutputEnableSet(pstHdmiDev->pstHdmiHal, HI_FALSE);
    }
    else
    {
        stScdcConfig.u32MaxTmdsCharacterRate = 300 * 1000000;
    }

    HDMI_INFO("ScdcEnable:%d\n",stScdcConfig.bScdcEnable);
    HDMI_INFO("Scdc Config:ScdcPresent(%d),RRCapable(%d),LTE340McscScramble(%d),MaxTmdsCharacterRate(%d)\n",stScdcConfig.bScdcPresent,stScdcConfig.bRRCapable,stScdcConfig.bLTE340McscScramble,stScdcConfig.u32MaxTmdsCharacterRate);
    pstHdmiDev->pstHdmiHal->HAL_HDMI_ScdcConfig(pstHdmiDev->pstHdmiHal, &stScdcConfig);

    return HI_SUCCESS;
}
#endif //#if defined (HDMI_SUPPORT_LOGIC_HISIV100)
#endif

static HI_S32 HdmiModeSchedule(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_APP_ATTR_S*         pstAppAttr;
    HDMI_VO_ATTR_S*          pstVideoAttr;
#ifdef HDMI_SCDC_SUPPORT
    HI_BOOL                  bScrambleEnable = HI_FALSE;
#endif
    pstAppAttr   = &pstHdmiDev->stAttr.stAppAttr;
    pstVideoAttr = &pstHdmiDev->stAttr.stVOAttr;

    pstVideoAttr->u32HdmiAdaptPixClk = pstVideoAttr->u32ClkFs;

    if(pstVideoAttr->u32HdmiAdaptPixClk >= HDMI_MAX_HDMI14_TMDS_RATE)
    {
        HDMI_MULTIPLE_0P5(pstVideoAttr->u32HdmiAdaptPixClk);//Y420
        HDMI_MULTIPLE_0P5(pstVideoAttr->u32ClkFs);
        pstAppAttr->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
        pstAppAttr->enOutColorSpace = HDMI_COLORSPACE_YCbCr420;
        HDMI_INFO("Enable YUV420,force 8bit & pixClk=%u\n",pstVideoAttr->u32HdmiAdaptPixClk);
    }
    else if (pstAppAttr->enOutColorSpace != HDMI_COLORSPACE_YCbCr422)
    {
        switch(pstAppAttr->enDeepColorMode)
        {
            case HDMI_DEEP_COLOR_30BIT:
                    HDMI_MULTIPLE_1P25(pstVideoAttr->u32HdmiAdaptPixClk);
                    break;

            case HDMI_DEEP_COLOR_36BIT:
                    HDMI_MULTIPLE_1P5(pstVideoAttr->u32HdmiAdaptPixClk);
                    break;

            case HDMI_DEEP_COLOR_48BIT:
                    HDMI_MULTIPLE_2P0(pstVideoAttr->u32HdmiAdaptPixClk);
                    break;

            case HDMI_DEEP_COLOR_24BIT:
            case HDMI_DEEP_COLOR_OFF:
            default:
                    pstAppAttr->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
                    break;
        }
    }
    else
    {
        pstAppAttr->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
    }

    if (pstVideoAttr->u32HdmiAdaptPixClk >= HDMI_MAX_HDMI14_TMDS_RATE)
    {
        HDMI_INFO("AdaptPixClk(%d) is bigger than 340M, deepcolor will change to 8bit(%d). \n", pstVideoAttr->u32HdmiAdaptPixClk, pstAppAttr->enDeepColorMode);
        /* q00352704@2016.02.22, when the u32HdmiAdaptPixClk is bigger than 340 at 10bit or 12bit,
		   the fmt will force change yuv422, 8bit, the u32HdmiAdaptPixClk must restore. */
		pstVideoAttr->u32HdmiAdaptPixClk = pstVideoAttr->u32ClkFs;
        pstAppAttr->enDeepColorMode = HDMI_DEEP_COLOR_24BIT;
        /* enOutColorSpace should change to YCbCr422 for 10bit out. */
        pstAppAttr->enOutColorSpace = HDMI_COLORSPACE_YCbCr422;
        pstHdmiDev->enTmdsMode = HDMI_TMDS_MODE_HDMI_1_4;
    }
    else
    {
        if (pstAppAttr->bEnableHdmi == HI_TRUE)
        {
            pstHdmiDev->enTmdsMode = HDMI_TMDS_MODE_HDMI_1_4;
        }
        else
        {
            pstHdmiDev->enTmdsMode = HDMI_TMDS_MODE_DVI;
            pstAppAttr->enOutColorSpace = HDMI_COLORSPACE_RGB;
        }
    }

#ifdef HDMI_SCDC_SUPPORT

    bScrambleEnable = (pstHdmiDev->enTmdsMode == HDMI_TMDS_MODE_HDMI_2_0) ? HI_TRUE : HI_FALSE;
    HdmiScrambleEnableSet(pstHdmiDev, bScrambleEnable);

#endif

    pstHdmiDev->pstHdmiHal->HAL_HDMI_TmdsModeSet(pstHdmiDev->pstHdmiHal, pstHdmiDev->enTmdsMode);
    HDMI_INFO("Tmds mode switch to %d,outSpace=%d\n",pstHdmiDev->enTmdsMode,pstAppAttr->enOutColorSpace);

    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_EmiSet(HDMI_DEVICE_S* pstHdmiDev)
{
    HDMI_EMI_CONFIG_S stEmiConfig;
    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstHdmiDev->pstHdmiHal);

    HDMI_INFO(">>> DRV_HDMI_EmiSet in...\n");
#ifdef HDMI_EMI_ENABLE
    pstHdmiDev->bEmiEnable = HI_TRUE;
#else
    pstHdmiDev->bEmiEnable = HI_FALSE;
#endif
    stEmiConfig.bVesaFmt = (pstHdmiDev->stAttr.stVOAttr.enVideoTiming >= HDMI_VIDEO_TIMING_VESA_DEFINE) ? HI_TRUE : HI_FALSE;
    stEmiConfig.bEmiEnable = pstHdmiDev->bEmiEnable;
    stEmiConfig.bDebugEnable = HI_FALSE;
    stEmiConfig.u32TmdsClk = pstHdmiDev->stAttr.stVOAttr.u32HdmiAdaptPixClk;
    stEmiConfig.enDeepColor = pstHdmiDev->stAttr.stAppAttr.enDeepColorMode;
    stEmiConfig.enOutColorSpace = pstHdmiDev->stAttr.stAppAttr.enOutColorSpace;

    pstHdmiDev->pstHdmiHal->HAL_HDMI_EmiSet(pstHdmiDev->pstHdmiHal, &stEmiConfig);

    HDMI_INFO("<<< DRV_HDMI_EmiSet out...\n");
    return HI_SUCCESS;
}


HI_S32 DRV_HDMI_AttrSet(HDMI_DEVICE_S* pstHdmiDev, HDMI_ATTR_S *pstAttr)
{
    HDMI_VO_ATTR_S*          pstVideoAttr;

    CHECK_POINTER(pstHdmiDev);
    CHECK_POINTER(pstAttr);

    pstVideoAttr = &pstAttr->stVOAttr;
    CheckVideoAttr(pstVideoAttr);
    memcpy(&pstHdmiDev->stAttr, pstAttr, sizeof(HDMI_ATTR_S));

    /* YUV422 is not support when the fmt is PAL or NTSC. */
    if(   HDMI_VIDEO_TIMING_1440X480I_60000 == pstHdmiDev->stAttr.stVOAttr.enVideoTiming
       || HDMI_VIDEO_TIMING_1440X576I_50000 == pstHdmiDev->stAttr.stVOAttr.enVideoTiming)
    {
        if(HDMI_COLORSPACE_YCbCr422 == pstHdmiDev->stAttr.stAppAttr.enOutColorSpace)
        {
            HDMI_INFO("Y422 is not support at pal and ntsc, force adapt to rgb.\n");
            pstHdmiDev->stAttr.stAppAttr.enOutColorSpace = HDMI_COLORSPACE_RGB;
        }
    }

    if(pstHdmiDev->stAttr.stVOAttr.u32ClkFs < HDMI_MIN_HDMI14_TMDS_RATE)
    {
        pstHdmiDev->stAttr.stVOAttr.u32ClkFs *= 2;
    }

    HdmiModeSchedule(pstHdmiDev);

    HdmiVideoPathSet(pstHdmiDev, &pstHdmiDev->stAttr.stVOAttr);
    DRV_HDMI_AVIInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_TRUE);

    DRV_HDMI_VendorInfoFrameSend(&pstHdmiDev->stInfoFrame, HI_TRUE);

    DRV_HDMI_EmiSet(pstHdmiDev);
    return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_Init(HI_VOID)
{
   return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_Open(HI_UNF_HDMI_ID_E enHdmiID)
{
    memset(&s_stHdmiDev[enHdmiID], 0, sizeof(HDMI_DEVICE_S));
    s_stHdmiDev[enHdmiID].u32HdmiDevId = enHdmiID;
    return DRV_HDMI_Open(&s_stHdmiDev[enHdmiID]);
}

HI_S32 HI_DRV_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmiID, HI_DRV_HDMI_ATTR_S *pstAttr)
{
    HI_S32      s32Ret = 0;
    HDMI_ATTR_S stAttr;
    HI_DISP_PARAM_S stDispParam = {0};

    memset(&stAttr, 0, sizeof(HDMI_ATTR_S));
    s32Ret = DRV_HDMI_AttrGet(&s_stHdmiDev[enHdmiID], &stAttr);
    if (s32Ret!=HI_SUCCESS)
    {
        HDMI_INFO("get attr error!\n");
    }
    s32Ret =  HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam);
    if (s32Ret!=HI_SUCCESS)
    {
        HDMI_INFO("HI_DRV_PDM_GetDispParam error!\n");
    }

    DispFmt2VOAttr(pstAttr->u32DispFmt, &stAttr.stVOAttr);
    //DispFmt2VOAttr(stDispParam.enFormat, &stAttr.stVOAttr);
    stAttr.stAppAttr.bEnableHdmi     = pstAttr->bEnableHdmi;
    HDMI_UDEEPCOLO_2_KDEEPCOLOR(stDispParam.enDeepColorMode, stAttr.stAppAttr.enDeepColorMode);
    stAttr.stAppAttr.enOutColorSpace = stDispParam.enVidOutMode;
    stAttr.stVOAttr.enInColorSpace   = pstAttr->enVidInMode;
    stAttr.stVOAttr.u32ClkFs         = pstAttr->u32ClkFs;
    stAttr.stVOAttr.bVSyncPol        = pstAttr->bVSyncPol;
    stAttr.stVOAttr.bHSyncPol        = pstAttr->bHSyncPol;
    stAttr.stVOAttr.bDEPol           = pstAttr->bDEPol;
    // delete in case of warning
//    PDM_FMT                          = stDispParam.enFormat;

    return DRV_HDMI_AttrSet(&s_stHdmiDev[enHdmiID], &stAttr);
}

HI_S32 HI_DRV_HDMI_Start(HI_UNF_HDMI_ID_E enHdmiID)
{
    return DRV_HDMI_Start(&s_stHdmiDev[enHdmiID]);
}

HI_S32 HI_DRV_HDMI_Close(HI_UNF_HDMI_ID_E enHdmiID)
{
    return DRV_HDMI_Close(&s_stHdmiDev[enHdmiID]);
}

HI_S32 HI_DRV_HDMI_DeInit(HI_VOID)
{
   return HI_SUCCESS;
}

HI_S32 HI_UNF_HDMI_Init(void)
{
    return HI_DRV_HDMI_Init();
}

HI_S32 HI_UNF_HDMI_DeInit(void)
{
    return HI_DRV_HDMI_DeInit();
}


HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi)
{
    return HI_DRV_HDMI_Open(enHdmi);
}
#if 0
HI_S32 HI_UNF_HDMI_SetFormat(HI_UNF_HDMI_ID_E enHdmi, HI_U32 u32DispFmt)
{
    return HI_DRV_HDMI_SetFormat(enHdmi, u32DispFmt);
}
#endif
HI_S32 HI_UNF_HDMI_SetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    return HI_DRV_HDMI_SetAttr(enHdmi, (HI_DRV_HDMI_ATTR_S*)pstAttr);
}
#if 0
HI_S32 HI_UNF_HDMI_GetAttr(HI_UNF_HDMI_ID_E enHdmi, HI_UNF_HDMI_ATTR_S *pstAttr)
{
    return HI_DRV_HDMI_GetAttr(enHdmi, pstAttr);
}
#endif
HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    return HI_DRV_HDMI_Start(enHdmi);
}


HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    return HI_DRV_HDMI_Close(enHdmi);
}




