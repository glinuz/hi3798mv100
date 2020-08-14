/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_ctrl.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/


#include "hdmi_reg_tx.h"
#include "hdmi_reg_audio_path.h"
#include "hdmi_reg_video_path.h"
#include "hdmi_reg_hdcp.h"

#include "hdmi_reg_mcu.h"
#include "hdmi_reg_aon.h"
#include "hdmi_reg_ctrl.h"

#include "hdmi_hal_ncts.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_ctrl.h"
#include "hdmi_hal_intf.h"

#include "hdmi_product_define.h"

#ifndef HDMI_BUILD_IN_BOOT
#include "hdmi_hal_machine.h"
#endif

#define CTRL_SET_BIT(var,bit)   do{var |= 1 << bit;}while(0)
#define CTRL_CLR_BIT(var,bit)   do{var &= ~(1 << bit);}while(0)
#define IS_BIT_SET(var,bit)     ({(var) & (0x1<<bit) ? HI_TRUE : HI_FALSE ;})

#define CTRL_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define CTRL_INIT_CHK(pstCtrlInfo) do{\
	CTRL_NULL_CHK(pstCtrlInfo);\
	if(pstCtrlInfo->bInit != HI_TRUE)\
	{HDMI_WARN("hdmi ctrl module didn't init!\n");\
	return HI_FAILURE;}\
}while(0)

/********************** VIDEO DEFINE *********************************/

#define CTRL_CHANNEL0_Y                 0x0
#define CTRL_CHANNEL0_Y422              0x3
#define CTRL_CHANNEL1_Cb                0x1
#define CTRL_CHANNEL1_Y422              0x4
#define CTRL_CHANNEL2_Cr                0x2
#define CTRL_CHANNEL2_Y422              0x3

#define CTRL_COLORMETRY_OUT_MASK        0xfc
#define CTRL_COLORMETRY_OUT_BIT         (4-4)
#define CTRL_COLORMETRY_IN_MASK         0xcf
#define CTRL_COLORMETRY_IN_BIT          (8-4)
#define CTRL_COLORMETRY_MASK            0x3

#define CTRL_RGB_OUT_BIT                (7-4)
#define CTRL_RGB_IN_BIT                 (11-4)

#define CTRL_QUANTIZAION_OUT_BIT        (6-4)
#define CTRL_QUANTIZAION_IN_BIT         (10-4)

#define CTRL_SYCN_POL_V_BIT             (25-25)
#define CTRL_SYCN_POL_H_BIT             (26-25)
#define CTRL_SYCN_POL_DE_BIT            (28-25)

#define CTRL_BLACK_Y_Cb_Cr      0x000000


typedef struct
{
    HI_BOOL             bVerDownSampBypass;
    HI_BOOL             bVerDownSampEnable;
	HI_BOOL             bHorDownSampEnable;
	HI_BOOL             bDemuxY420Enalbe;
	HI_BOOL             bPxlDivEnable;
	HI_BOOL             bHorFilterEnable;
}HDMI_VIDOE_DOWNSAMP_S;



/********************** AUDIO DEFINE *********************************/

#define CTRL_AUDIO_INVALID_CFG          0xff
#define CTRL_AUDIO_INVALID_RATE         0xffffffff




typedef enum{

    HDMI_AUDIO_FIFO_MAP_SD0 = 0x0,
    HDMI_AUDIO_FIFO_MAP_SD1 = 0x1,
    HDMI_AUDIO_FIFO_MAP_SD2 = 0x2,
    HDMI_AUDIO_FIFO_MAP_SD3 = 0x3,
    HDMI_AUDIO_FIFO_MAP_BUTT,

}HDMI_AUDIO_FIFO_MAP_E;


typedef enum{
    HDMI_AUDIO_I2S_SD_NONE  = 0x0,
    HDMI_AUDIO_I2S_SD_0     = 0x1,
    HDMI_AUDIO_I2S_SD_2     = 0x2,
    HDMI_AUDIO_I2S_SD_3     = 0x4,
    HDMI_AUDIO_I2S_SD_4     = 0x8,
    HDMI_AUDIO_I2S_SD_ALL   = 0xf,
}HDMI_AUDIO_I2S_SD_E;

typedef enum
{
    HDMI_AUDIO_HBRA_MASK_ALL    = 0x0,
    HDMI_AUDIO_HBRA_MASK_NONE   = 0xf,

}CTRL_I2S_HBRA_MASK_E;



typedef enum
{
	HDMI_AUDIO_CLK_ACCURACY_LEVEL2 = 0x0,
	HDMI_AUDIO_CLK_ACCURACY_LEVEL1 ,
	HDMI_AUDIO_CLK_ACCURACY_LEVEL3 ,
	HDMI_AUDIO_CLK_ACCURACY_NOTMATCH
}HDMI_AUDIO_CLK_ACCURACY_E;

typedef struct
{
    //HI_U32                  u32FifoMap;
    HI_BOOL                 bTest;
    CTRL_I2S_HBRA_MASK_E    enHbraMask;
}CTRL_FIFO_CTRL_S;

typedef struct
{
    /* cfg0 */
    HDMI_AUDIO_CLK_ACCURACY_E   enClkAccuracy ;
    HI_BOOL                     bProfessionalApp ;
    HI_BOOL                     bCompress;
    HI_U32                      u32SampRate;
    //HI_U32      u32AudioRelative;

    /* cfg1 */
    //HI_U32      u32AudioRelative1;
    HI_U32                      u32OrgRate;
    HI_U32                      u32OutBitLength;

    HDMI_AUDIO_FIFO_MAP_E       enFifoMap;
}CTRL_I2S_STATUS_S;

typedef struct
{
    HI_BOOL     bChannelLRSwap;
    HI_U32      u32InBitLength;
    HI_BOOL     bVbitCompress;
    HI_BOOL     bDataDirMsbFirst;
    HI_BOOL     bJustifyWsRight;
    HI_BOOL     bPolarityWsMsbLeft;
    HI_BOOL     bSckShiftOne;
    HI_BOOL     bHbraEnable;

}CTRL_I2S_CTRL_S;

typedef struct
{
    HI_U32      u32BitValue;
    HI_U32      u32BitReg;
}HDMI_AUDIO_SAMP_BIT_S;


typedef struct
{
    HI_U32      u32RateValue;
    HI_U32      u32RateReg;
    HI_U32      u32RateOrgReg;
}HDMI_I2S_SAMP_RATE_S;

typedef struct
{
    HI_U32      u32RateValue;
    HI_U32      u32RateReg;
}HDMI_SPDIF_SAMP_RATE_S;


static HDMI_AUDIO_SAMP_BIT_S s_au32SampBit[] =
{
    /* bit,     reg    */
    { 16,   0x2 },
    { 17,   0xc },
    { 18,   0x4 },
    { 19,   0x8 },
    { 20,   0xa },
    { 21,   0xd },
    { 22,   0x5 },
    { 23,   0x9 },
    { 24,   0xb },
};



static  HDMI_I2S_SAMP_RATE_S s_astI2sSmapRate[] =
{
    /* sample rate value,          Reg      orgReg,  */
    {HDMI_SAMPLE_RATE_8K  ,  0x01,     0x6   },
    {HDMI_SAMPLE_RATE_11K ,  0x01,     0xa   },
    {HDMI_SAMPLE_RATE_12K ,  0x01,     0x2   },
    {HDMI_SAMPLE_RATE_16K ,  0x01,     0x8   },
    {HDMI_SAMPLE_RATE_22K ,  0x04,     0xb   },
    {HDMI_SAMPLE_RATE_24K ,  0x06,     0x9   },
    {HDMI_SAMPLE_RATE_32K ,  0x04,     0xc   },
    {HDMI_SAMPLE_RATE_44K ,  0x00,     0xf   },
    {HDMI_SAMPLE_RATE_48K ,  0x02,     0xd   },
    {HDMI_SAMPLE_RATE_88K ,  0x08,     0x7   },
    {HDMI_SAMPLE_RATE_96K ,  0x0a,     0x5   },
    {HDMI_SAMPLE_RATE_176K,  0x0c,     0x3   },
    {HDMI_SAMPLE_RATE_192K,  0x0e,     0x1   },
    {HDMI_SAMPLE_RATE_768K,  0x09,     0x1   },
};


static HDMI_SPDIF_SAMP_RATE_S s_astSpdifSmapRate[] = {
    /* rate value,  reg */
     {   22050,  0x04   },
     {   44100,  0x00   },
     {   88200,  0x08   },
     {  176400,  0x0c   },
     {  352800,  0x0d   },
     {  705600,  0x2d   },
     { 1411200,  0x1d   },
     {   24000,  0x06   },
     {   48000,  0x02   }, /* default */
     {   96000,  0x0a   },
     {  192000,  0x0e   },
     {  384000,  0x05   },
     {  768000,  0x09   },
     { 1536000,  0x15   },
     {   32000,  0x03   },
     {   64000,  0x0b   },
     {  128000,  0x2b   },
     {  256000,  0x1b   },
     {  512000,  0x3b   },
     { 1024000,  0x35   },
 };


static HDMI_CTRL_INFO_S    s_stCtrlInfo[HDMI_DEVICE_ID_BUTT];

static HDMI_CTRL_INFO_S *CtrlInfoPtrGet(HDMI_DEVICE_ID_E enHdmi)
{
    if (enHdmi < HDMI_DEVICE_ID_BUTT)
    {
        return &s_stCtrlInfo[enHdmi];
    }
    return HI_NULL;
}

/*
 *   retval:  a valid reg cfg / CTRL_AUDIO_INVALID_CFG
*/
static HI_U32  CtrlI2SRateRegGet(HI_U32 u32RateValue)
{
    HI_U32                  i = 0;
    HDMI_I2S_SAMP_RATE_S    *pstI2sRate = HI_NULL;

    for (pstI2sRate = &s_astI2sSmapRate[0];
        pstI2sRate && (i < HDMI_ARRAY_SIZE(s_astI2sSmapRate));
        pstI2sRate++,i++)
    {
        if (pstI2sRate->u32RateValue == u32RateValue)
        {
            return  pstI2sRate->u32RateReg;
        }
    }

    HDMI_ERR("ctrl can't find aduio°°rate= %u,force 48k\n",u32RateValue);

    return 0x02; /*16bit*/
}

/*
 *   retval:  a valid reg cfg / CTRL_AUDIO_INVALID_CFG
*/
static HI_U32  CtrlI2SRateOrgRegGet(HI_U32 u32RateValue)
{
    HI_U32                  i = 0;
    HDMI_I2S_SAMP_RATE_S    *pstI2sRate = HI_NULL;

    for (pstI2sRate = &s_astI2sSmapRate[0];
        pstI2sRate && (i < HDMI_ARRAY_SIZE(s_astI2sSmapRate));
        pstI2sRate++,i++)
    {
        if (pstI2sRate->u32RateValue == u32RateValue)
        {
            return  pstI2sRate->u32RateOrgReg;
        }
    }

    HDMI_ERR("ctrl can't find audio rate= %u,force 48k\n",u32RateValue);

    return 0x0d;/*48k*/
}

/*
 *   retval:  a valid sample rate / CTRL_AUDIO_INVALID_RATE
*/
static HI_U32 CtrlI2SRateValueGet(HI_VOID)
{
    HI_U32                  i = 0;
    HI_U32                  u32RateReg = 0;
    HI_U32                  u32RateOrgReg = 0;
    HDMI_I2S_SAMP_RATE_S    *pstI2sRate = HI_NULL;

    u32RateReg = HDMI_AUD_CHST_CFG0_chst_byte3_fsGet();
    u32RateOrgReg = HDMI_AUD_CHST_CFG1_chst_byte4_org_fsGet();

    for (pstI2sRate = &s_astI2sSmapRate[0];
        pstI2sRate && (i < HDMI_ARRAY_SIZE(s_astI2sSmapRate));
        pstI2sRate++,i++)
    {
        if ((pstI2sRate->u32RateReg  == u32RateReg)
            && (pstI2sRate->u32RateOrgReg  == u32RateOrgReg))
        {
            return  pstI2sRate->u32RateValue;
        }
    }

    return CTRL_AUDIO_INVALID_RATE;
}


/*
 *   retval:  a valid reg cfg / CTRL_AUDIO_INVALID_CFG
*/
static HI_U32  CtrlAudioBitRegGet(HI_U32 u32BitValue)
{
    HI_U32                  i = 0;
    HDMI_AUDIO_SAMP_BIT_S   *pstAudioBit = HI_NULL;

    for (pstAudioBit = &s_au32SampBit[0];
        pstAudioBit && (i < HDMI_ARRAY_SIZE(s_au32SampBit));
        pstAudioBit++,i++)
    {
        if (pstAudioBit->u32BitValue == u32BitValue)
        {
            return  pstAudioBit->u32BitReg;
        }
    }

    HDMI_ERR("ctrl can't find audio bit= %u,force 16\n",u32BitValue);
    return 0x02;/*16bit*/
}

/*
 *   retval:  a valid sample rate / CTRL_AUDIO_INVALID_RATE
*/
static HI_U32 CtrlAudioBitValueGet(HI_BOOL bSpdif)
{
    HI_U32                  i = 0;
    HI_U32                  u32BitReg = 0;
    HDMI_AUDIO_SAMP_BIT_S   *pstAudioBit = HI_NULL;

    if (bSpdif)
    {
        u32BitReg = HDMI_TX_AUDIO_STATE_aud_lengthGet();
    }
    else
    {
        u32BitReg = HDMI_AUD_CHST_CFG1_chst_byte4_lengthGet();
    }

    for (pstAudioBit = &s_au32SampBit[0];
        pstAudioBit && (i < HDMI_ARRAY_SIZE(s_au32SampBit));
        pstAudioBit++,i++)
    {
        if (pstAudioBit->u32BitReg  == u32BitReg)
        {
            return  pstAudioBit->u32BitValue;
        }
    }

    return CTRL_AUDIO_INVALID_RATE;
}

#if 0

/*
 *   retval:  a valid reg cfg / CTRL_AUDIO_INVALID_CFG
*/
static HI_U32   CtrlSpdifRateRegGet(HI_U32 u32SpdifRate)
{
    HI_U32                  i = 0;
    HDMI_SPDIF_SAMP_RATE_S *pstSpdif = HI_NULL;

    for (pstSpdif = &s_astSpdifSmapRate[0];
        pstSpdif && (i < HDMI_ARRAY_SIZE(s_astSpdifSmapRate));
        pstSpdif++,i++)
    {
        if (pstSpdif->u32RateValue == u32SpdifRate)
        {
            return  pstSpdif->u32RateReg;
        }
    }

    return 0x02;/*48k*/
}
#endif

/*
 *   retval:  a valid sample rate / CTRL_AUDIO_INVALID_RATE
*/
static HI_U32   CtrlSpdifRateValueGet(HI_VOID)
{
    HI_U32                  i = 0;
    HI_U32                  u32SpdifCfg = 0;
    HDMI_SPDIF_SAMP_RATE_S  *pstSpdif = HI_NULL;

    u32SpdifCfg = HDMI_TX_AUDIO_STATE_aud_spdif_fsGet();

    for (pstSpdif = &s_astSpdifSmapRate[0];
        pstSpdif && (i < HDMI_ARRAY_SIZE(s_astSpdifSmapRate));
        pstSpdif++,i++)
    {
        if (pstSpdif->u32RateReg  == u32SpdifCfg)
        {
            return  pstSpdif->u32RateValue;
        }
    }

    return CTRL_AUDIO_INVALID_RATE;
}

/******************************* private interface  ***********************************/

static HI_VOID CtrlAvmuteSet(HI_BOOL bEnable,HI_U32 u32RepCnt)
{

	if(bEnable)
    {

        /* set GCP set mute & clr mute  */
        HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(HI_TRUE);
        HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(HI_FALSE);

        /* set GCP avmute funtion enable,sent loop  */
		HDMI_CEA_CP_CFG_cea_cp_enSet(HI_FALSE);
		HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(6);
		HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(HI_TRUE);
		HDMI_CEA_CP_CFG_cea_cp_enSet(HI_TRUE);

    }
    else
    {
        /* set GCP set mute & clr mute  */
        HDMI_CP_PKT_AVMUTE_cp_set_avmuteSet(HI_FALSE);
        HDMI_CP_PKT_AVMUTE_cp_clr_avmuteSet(HI_TRUE);

        /* set GCP avmute funtion enable,sent 6 times  */
		HDMI_CEA_CP_CFG_cea_cp_enSet(HI_FALSE);
		HDMI_CEA_CP_CFG_cea_cp_rpt_cntSet(6);
		HDMI_CEA_CP_CFG_cea_cp_rpt_enSet(HI_TRUE);
		HDMI_CEA_CP_CFG_cea_cp_enSet(HI_TRUE);
    }

    return ;
}


static HI_S32 CtrlAvmuteGet(HI_BOOL *pbEnable)
{

    CTRL_NULL_CHK(pbEnable);

    *pbEnable = HI_FALSE;

    if(HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet()
    || HDMI_CEA_CP_CFG_cea_cp_rpt_enGet())
    {
        if (HDMI_CP_PKT_AVMUTE_cp_set_avmuteGet())
        {
            *pbEnable = HI_TRUE;
        }
    }

    return HI_SUCCESS;
}

static HI_VOID CtrlAudioMuteSet(HI_BOOL bEnable)
{
	HDMI_TX_AUDIO_CTRL_aud_mute_enSet(bEnable);
	//HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlSet(bEnable);

    return ;
}

static HI_BOOL CtrlAudioMuteGet(HI_VOID)
{

    if(HDMI_TX_AUDIO_CTRL_aud_mute_enGet())
    //|| HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlGet())
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }

}


static HI_VOID CtrlAudioI2sEnalbeSet(HI_BOOL bEnable)
{
    HDMI_AUDIO_I2S_SD_E enAudioI2sEnable = HDMI_AUDIO_I2S_SD_NONE;
    enAudioI2sEnable = bEnable ? HDMI_AUDIO_I2S_SD_ALL : HDMI_AUDIO_I2S_SD_NONE;
	HDMI_TX_AUDIO_CTRL_aud_i2s_enSet(enAudioI2sEnable);

    return;
}

static HI_BOOL CtrlAudioI2sEnalbeGet(HI_VOID)
{
	if (HDMI_TX_AUDIO_CTRL_aud_i2s_enGet())
	{
        return  HI_TRUE;
	}
    else
    {
        return  HI_FALSE;
    }
}

static HI_VOID CtrlAudioSpdifEnalbeSet(HI_BOOL bSpdifEnable)
{
	HDMI_TX_AUDIO_CTRL_aud_spdif_enSet(bSpdifEnable);
    return ;
}

static HI_BOOL CtrlAudioSpdifEnalbeGet(HI_VOID)
{
	if (HDMI_TX_AUDIO_CTRL_aud_spdif_enGet())
        //&& HDMI_TX_AUDIO_STATE_aud_inavailableGet())
	{
        return  HI_TRUE;
	}
    else
    {
        return  HI_FALSE;
    }
}


static HI_VOID CtrlAudioPathEnableSet(HI_BOOL bAudioEnable)
{
	HDMI_TX_AUDIO_CTRL_aud_in_enSet(bAudioEnable);
	return ;
}


static HI_BOOL CtrlAudioPathEnableGet(HI_VOID)
{
    if (HDMI_TX_AUDIO_CTRL_aud_in_enGet())
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlAudioPathNvalueSet(HI_U32 u32AudioNvalue)
{
	HDMI_ACR_N_VAL_SW_acr_n_val_swSet(u32AudioNvalue);
    return ;
}

static HI_U32 CtrlAudioPathNvalueGet(HI_VOID)
{
    HI_U32 u32NValue = 0;

    u32NValue = HDMI_ACR_N_VAL_SW_acr_n_val_swGet();

	return u32NValue;

}


static HI_VOID CtrlAudioPathCtsValueSet(HI_BOOL bSwCts, HI_U32 u32CtsSwVal)
{
/*
    if (bSwCts)
    {
        HDMI_ACR_CTS_VAL_SW_acr_cts_val_swSet(u32CtsSwVal);
    }
*/
    HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selSet(bSwCts);

    return ;
}

static HI_U32 CtrlAudioPathCtsValueGet(HI_VOID)
{
    HI_U32 u32CtsValue = 0;

    msleep(1); //wait reg valid
    /* sw */
    if (HDMI_AUD_ACR_CTRL_acr_cts_hw_sw_selGet())
    {
        u32CtsValue = HDMI_ACR_CTS_VAL_SW_acr_cts_val_swGet();
    }
    /* hw */
    else
    {
        u32CtsValue = HDMI_ACR_CTS_VAL_HW_acr_cts_val_hwGet();
    }

    return u32CtsValue;
}


static HI_VOID CtrlAudioPathLayoutSet(HI_BOOL bMutiLayout)
{
    HDMI_TX_AUDIO_CTRL_aud_layoutSet(bMutiLayout);
    return ;
}

static HI_U32 CtrlAudioPathLayoutGet(HI_VOID)
{
    HI_U32 u32Layout = 0;

    u32Layout = HDMI_TX_AUDIO_CTRL_aud_layoutGet();

    return u32Layout;
}

static HI_VOID CtrlAudioHbraEnableSet(HI_BOOL bHbraEnalbe)
{
    HDMI_AUD_I2S_CTRL_i2s_hbra_onSet(bHbraEnalbe);
    return ;
}

static HI_BOOL CtrlAudioHbraEnableGet(HI_VOID)
{
    if(HDMI_AUD_I2S_CTRL_i2s_hbra_onGet())
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_S32 CtrlAudioI2sCtrlSet(CTRL_I2S_CTRL_S *pstI2sCtrl)
{
    CTRL_NULL_CHK(pstI2sCtrl);

    //HDMI_AUD_I2S_CTRL_i2s_hbra_onSet(pstI2sCtrl->bHbraEnable);
    HDMI_AUD_I2S_CTRL_i2s_1st_shiftSet(pstI2sCtrl->bSckShiftOne);
    HDMI_AUD_I2S_CTRL_i2s_ws_polaritySet(pstI2sCtrl->bPolarityWsMsbLeft);
    HDMI_AUD_I2S_CTRL_i2s_justifySet(pstI2sCtrl->bJustifyWsRight);
    HDMI_AUD_I2S_CTRL_i2s_data_dirSet(pstI2sCtrl->bDataDirMsbFirst);
    HDMI_AUD_I2S_CTRL_i2s_vbitSet(pstI2sCtrl->bVbitCompress);
    HDMI_AUD_I2S_CTRL_i2s_lengthSet(pstI2sCtrl->u32InBitLength);
    HDMI_AUD_I2S_CTRL_i2s_ch_swapSet(pstI2sCtrl->bChannelLRSwap);

    return HI_SUCCESS;
}

static HI_S32 CtrlAudioI2sStatusSet(CTRL_I2S_STATUS_S *pstI2sStatus)
{
    CTRL_NULL_CHK(pstI2sStatus);

    /* byte0_a Config (0: consumer applications,1: professional applications)  */
    HDMI_AUD_CHST_CFG0_chst_byte0_aSet(pstI2sStatus->bProfessionalApp);
    /* byte0_b Config (0: PCM,1:compress) 	 */
    HDMI_AUD_CHST_CFG0_chst_byte0_bSet(pstI2sStatus->bCompress);
    HDMI_AUD_CHST_CFG0_chst_byte3_fsSet(pstI2sStatus->u32SampRate);
    HDMI_AUD_CHST_CFG0_chst_byte3_clock_accuracySet(pstI2sStatus->enClkAccuracy);

    HDMI_AUD_CHST_CFG1_chst_byte4_org_fsSet(pstI2sStatus->u32OrgRate);
    HDMI_AUD_CHST_CFG1_chst_byte4_lengthSet(pstI2sStatus->u32OutBitLength);

    return HI_SUCCESS;
}

static HI_S32 CtrlAudioFifoCtrlSet(CTRL_FIFO_CTRL_S *pstFifoCtrl)
{
    CTRL_NULL_CHK(pstFifoCtrl);

    //HDMI_TX_AUDIO_CTRL_aud_fifo0_mapSet(pstFifoCtrl->u32FifoMap);
    HDMI_AUD_FIFO_CTRL_aud_fifo_testSet(pstFifoCtrl->bTest);
    HDMI_AUD_FIFO_CTRL_aud_fifo_hbr_maskSet(pstFifoCtrl->enHbraMask);
    return HI_SUCCESS;
}

static HI_VOID CtrlAudioReset(HI_VOID)
{
    HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(HI_TRUE);
    HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(HI_TRUE);

    HDMI_TX_PWD_RST_CTRL_tx_afifo_srst_reqSet(HI_FALSE);
    HDMI_TX_PWD_RST_CTRL_tx_acr_srst_reqSet(HI_FALSE);
    HDMI_TX_PWD_RST_CTRL_tx_aud_srst_reqSet(HI_FALSE);

    return ;
}


static HI_VOID CtrlVideoPathDeepClrSet(HI_BOOL bEnalbeGcp,HDMI_DEEP_COLOR_E enDeepColorBit)
{
    HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeSet(enDeepColorBit);
    HDMI_AVMIXER_CONFIG_dc_pkt_enSet(bEnalbeGcp);
    return ;
}

static HI_S32 CtrlVideoPathDeepClrGet(HI_BOOL *pbEnalbeGcp,HDMI_DEEP_COLOR_E *penDeepColorBit)
{
    CTRL_NULL_CHK(pbEnalbeGcp);
    CTRL_NULL_CHK(penDeepColorBit);

    *penDeepColorBit = HDMI_TX_PACK_FIFO_CTRL_tmds_pack_modeGet();
    if (HDMI_AVMIXER_CONFIG_dc_pkt_enGet())
    {
        *pbEnalbeGcp = HI_TRUE;
    }
    else
    {
        *pbEnalbeGcp = HI_FALSE;
    }
    return HI_SUCCESS;
}

#if 0
static HI_VOID CtrlVideoPathPolaritySet(HDMI_HVSYNC_POLARITY_E enInSyncPol,HDMI_HVSYNC_POLARITY_E enOutSyncPol)
{
    HI_U32      u32SyncPolCfg = 0;
    HI_BOOL     bHSyncPolIn = HI_FALSE;
    HI_BOOL     bVSyncPolIn = HI_FALSE;
    HI_BOOL     bHSyncPolOut = HI_FALSE;
    HI_BOOL     bVSyncPolOut = HI_FALSE;

    switch(enInSyncPol)
    {
        case HDMI_HV_SYNC_POL_HPVP:
            bHSyncPolIn = HI_TRUE;
            bVSyncPolIn = HI_TRUE;
            break;

        case HDMI_HV_SYNC_POL_HPVN:
            bHSyncPolIn = HI_TRUE;
            bVSyncPolIn = HI_FALSE;
            break;

        case HDMI_HV_SYNC_POL_HNVP:
            bHSyncPolIn = HI_FALSE;
            bVSyncPolIn = HI_TRUE;
            break;

        case HDMI_HV_SYNC_POL_HNVN:
            bHSyncPolIn = HI_FALSE;
            bVSyncPolIn = HI_FALSE;
            break;
        default:
            HDMI_WARN("ctrl un-known polarity !\n");
            break;
    }

    switch(enOutSyncPol)
    {
        case HDMI_HV_SYNC_POL_HPVP:
            bHSyncPolOut= HI_TRUE;
            bVSyncPolOut= HI_TRUE;
            break;

        case HDMI_HV_SYNC_POL_HPVN:
            bHSyncPolOut= HI_TRUE;
            bVSyncPolOut= HI_FALSE;
            break;

        case HDMI_HV_SYNC_POL_HNVP:
            bHSyncPolOut= HI_FALSE;
            bVSyncPolOut= HI_TRUE;
            break;

        case HDMI_HV_SYNC_POL_HNVN:
            bHSyncPolOut= HI_FALSE;
            bVSyncPolOut= HI_FALSE;
            break;
        default:
            HDMI_WARN("ctrl un-known polarity !\n");
            break;
    }

    if (bVSyncPolOut == bVSyncPolIn)
    {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_V_SYCN_POL_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_V_SYCN_POL_BIT);
    }

    if (bHSyncPolOut == bHSyncPolIn)
    {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_H_SYCN_POL_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_H_SYCN_POL_BIT);
    }

    HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(u32SyncPolCfg);
    HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(HI_FALSE);

    return ;
}


static HI_S32 CtrlVideoPathPolarityGet(HDMI_HVSYNC_POLARITY_E *penInSyncPol,HDMI_HVSYNC_POLARITY_E *penOutSyncPol)
{
    HI_U32      u32SyncPolCfg = 0;
    HI_BOOL     bHSyncPolIn = HI_FALSE;
    HI_BOOL     bVSyncPolIn = HI_FALSE;
    HI_BOOL     bHSyncPolOut = HI_FALSE;
    HI_BOOL     bVSyncPolOut = HI_FALSE;

    CTRL_NULL_CHK(penInSyncPol);
    CTRL_NULL_CHK(penOutSyncPol);

    u32SyncPolCfg = HDMI_VIDEO_DMUX_CTRL_reg_inver_syncGet();
    bHSyncPolIn = HDMI_FDET_STATUS_hsync_polarityGet();
    bVSyncPolIn = HDMI_FDET_STATUS_vsync_polarityGet();
    bHSyncPolOut = IS_BIT_SET(u32SyncPolCfg,CTRL_H_SYCN_POL_BIT) ? (!bHSyncPolIn) : bHSyncPolIn;
    bVSyncPolOut = IS_BIT_SET(u32SyncPolCfg,CTRL_V_SYCN_POL_BIT) ? (!bVSyncPolIn) : bVSyncPolIn;

    /* in */
    if ( (bHSyncPolIn) && (bVSyncPolIn) )
    {
        *penInSyncPol = HDMI_HV_SYNC_POL_HPVP;
    }
    else if ( (bHSyncPolIn) && (!bVSyncPolIn) )
    {
        *penInSyncPol = HDMI_HV_SYNC_POL_HPVN;
    }
    else if ( (bHSyncPolIn) && (!bVSyncPolIn) )
    {
        *penInSyncPol = HDMI_HV_SYNC_POL_HNVP;
    }
    else if ( (!bHSyncPolIn) && (!bVSyncPolIn) )
    {
        *penInSyncPol = HDMI_HV_SYNC_POL_HNVN;
    }
    else
    {
        /* do nothing */
    }

    /* out */
    if ( (bHSyncPolOut) && (bVSyncPolOut) )
    {
        *penOutSyncPol = HDMI_HV_SYNC_POL_HPVP;
    }
    else if ( (bHSyncPolOut) && (!bVSyncPolOut) )
    {
        *penOutSyncPol = HDMI_HV_SYNC_POL_HPVN;
    }
    else if ( (bHSyncPolOut) && (!bVSyncPolOut) )
    {
        *penOutSyncPol = HDMI_HV_SYNC_POL_HNVP;
    }
    else if ( (!bHSyncPolOut) && (!bVSyncPolOut) )
    {
        *penOutSyncPol = HDMI_HV_SYNC_POL_HNVN;
    }
    else
    {
        /* do nothing */
    }

    return HI_SUCCESS;

}

#endif

static HI_S32 CtrlVideoPathPolaritySet(HDMI_SYNC_POL_S *pstOutSyncPol)
{
    HI_U32  u32SyncPolCfg = 0;

    CTRL_NULL_CHK(pstOutSyncPol);

    if (pstOutSyncPol->bHPolInver)
    {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_H_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_H_BIT);
    }

    if (pstOutSyncPol->bVPolInver)
    {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_V_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_V_BIT);
    }

    if (pstOutSyncPol->bDeInver)
    {
        CTRL_SET_BIT(u32SyncPolCfg,CTRL_SYCN_POL_DE_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32SyncPolCfg,CTRL_SYCN_POL_DE_BIT);
    }

    HDMI_VIDEO_DMUX_CTRL_reg_inver_syncSet(u32SyncPolCfg);
    HDMI_VIDEO_DMUX_CTRL_reg_syncmask_enSet(HI_FALSE);

    return HI_SUCCESS;

}


static HI_S32 CtrlVideoPathPolarityGet(HDMI_SYNC_POL_S *pstOutSyncPol)
{
    HI_U32  u32SyncPolCfg = 0;

    CTRL_NULL_CHK(pstOutSyncPol);

    u32SyncPolCfg = HDMI_VIDEO_DMUX_CTRL_reg_inver_syncGet();
    pstOutSyncPol->bHPolInver = IS_BIT_SET(u32SyncPolCfg,CTRL_SYCN_POL_H_BIT) ? HI_TRUE : HI_FALSE;
    pstOutSyncPol->bVPolInver = IS_BIT_SET(u32SyncPolCfg,CTRL_SYCN_POL_V_BIT) ? HI_TRUE : HI_FALSE;
    pstOutSyncPol->bDeInver = IS_BIT_SET(u32SyncPolCfg,CTRL_SYCN_POL_DE_BIT) ? HI_TRUE : HI_FALSE;

    return HI_SUCCESS;
}

static HI_VOID CtrlVideoPathDitherSet(HI_BOOL bEnable,HDMI_VEDIO_DITHER_E enDitherMode)
{
	HDMI_DITHER_CONFIG_dither_rnd_bypSet(bEnable);
	HDMI_DITHER_CONFIG_dither_modeSet(enDitherMode);
    return ;
}

static HI_S32 CtrlVideoPathDitherGet(HI_BOOL *pbEnable,HDMI_VEDIO_DITHER_E *penDitherMode)
{
    CTRL_NULL_CHK(pbEnable);
    CTRL_NULL_CHK(penDitherMode);

	if(HDMI_DITHER_CONFIG_dither_rnd_bypGet())
	{
        *pbEnable = HI_TRUE;
	}
	else
	{
        *pbEnable = HI_FALSE;
	}

	*penDitherMode = HDMI_DITHER_CONFIG_dither_modeGet();

    return HI_SUCCESS;
}



static HI_VOID CtrlVideoColorRgbSet(HI_BOOL bRgbIn,HI_BOOL bRgbOut)
{
    HI_U32  u32RegValue  = 0;

    u32RegValue = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    if (bRgbIn)
    {
        CTRL_SET_BIT(u32RegValue,CTRL_RGB_IN_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32RegValue,CTRL_RGB_IN_BIT);
    }

    if (bRgbOut)
    {
        CTRL_SET_BIT(u32RegValue,CTRL_RGB_OUT_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32RegValue,CTRL_RGB_OUT_BIT);
    }
    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32RegValue);

    return ;
}


static HI_S32 CtrlVideoColorRgbGet(HI_BOOL *pbRgbIn,HI_BOOL *pbRgbOut)
{
    HI_U32  u32RegValue  = 0;

    CTRL_NULL_CHK(pbRgbIn);
    CTRL_NULL_CHK(pbRgbOut);

    u32RegValue = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    *pbRgbIn    = IS_BIT_SET(u32RegValue,CTRL_RGB_IN_BIT);
    *pbRgbOut   = IS_BIT_SET(u32RegValue,CTRL_RGB_OUT_BIT);

    return HI_SUCCESS;
}

static HI_VOID CtrlVideoColorCscSet(HI_BOOL bCscEnable)
{
    HDMI_MULTI_CSC_CTRL_reg_csc_enSet(bCscEnable);
    return ;
}

static HI_BOOL CtrlVideoColorCscGet(HI_VOID)
{
    if (HDMI_MULTI_CSC_CTRL_reg_csc_enGet())
    {
        return  HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlVideoColorYCbCr422Set(HI_BOOL bYCbCr422)
{
    if (bYCbCr422)
    {
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(CTRL_CHANNEL0_Y422);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(CTRL_CHANNEL1_Y422);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(CTRL_CHANNEL2_Y422);
    }
    else
    {
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selSet(CTRL_CHANNEL0_Y);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selSet(CTRL_CHANNEL1_Cb);
        HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selSet(CTRL_CHANNEL2_Cr);
    }

    return ;
}

static HI_BOOL CtrlVideoColorYCbCr422Get(HI_VOID)
{
    if ((CTRL_CHANNEL0_Y422 == HDMI_VIDEO_DMUX_CTRL_reg_vmux_y_selGet())
        &&(CTRL_CHANNEL1_Y422 == HDMI_VIDEO_DMUX_CTRL_reg_vmux_cb_selGet())
        &&(CTRL_CHANNEL2_Y422 == HDMI_VIDEO_DMUX_CTRL_reg_vmux_cr_selGet()))
    {
       return HI_TRUE;
    }
    else
    {
        return HI_FALSE;

    }

}

static HI_VOID CtrlVideoColorYCbCr420Set(HI_BOOL bYCbCr420)
{
    HDMI_DATA_ALIGN_CTRL_reg_demux_420_enSet(bYCbCr420);
    HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enSet(bYCbCr420);

    return ;
}

static HI_BOOL CtrlVideoColorYCbCr420Get(HI_VOID)
{
    if ((HDMI_DATA_ALIGN_CTRL_reg_demux_420_enGet())
        &&(HDMI_DATA_ALIGN_CTRL_reg_pxl_div_enGet()))
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlVideoColorDwsmVertSet(HI_BOOL bDwsmVert)
{
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypSet(!bDwsmVert);
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enSet(bDwsmVert);

    return ;
}

static HI_BOOL CtrlVideoColorDwsmVertGet(HI_VOID)
{
    if (HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_enGet()
        && (!HDMI_VIDEO_DWSM_CTRL_reg_dwsm_vert_bypGet()) )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlVideoColorDwsmHoriSet(HI_BOOL bDwsmHori)
{
    HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enSet(bDwsmHori);
    HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enSet(bDwsmHori);

    return ;
}

static HI_BOOL CtrlVideoColorDwsmHoriGet(HI_VOID)
{
    if (HDMI_VIDEO_DWSM_CTRL_reg_hori_filter_enGet()
        && HDMI_VIDEO_DWSM_CTRL_reg_dwsm_hori_enGet() )
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlVideoPathColorSpaceSet(HDMI_COLORSPACE_E enInColorSpace,HDMI_COLORSPACE_E enOutColorSpace)
{
    HI_BOOL     bInRgb          = HI_FALSE;
    HI_BOOL     bOutRgb         = HI_FALSE;
    HI_BOOL     bCscEnalbe      = HI_FALSE;
    HI_BOOL     bY422Enalbe     = HI_FALSE;
    HI_BOOL     bY420Enalbe     = HI_FALSE;
    HI_BOOL     bDwsmHoriEnalbe = HI_FALSE;
    HI_BOOL     bDwsmVertEnalbe = HI_FALSE;

    if (enInColorSpace == HDMI_COLORSPACE_YCbCr444 )
    {
    	switch(enOutColorSpace)
    	{
			case HDMI_COLORSPACE_YCbCr422:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_TRUE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;

    		case HDMI_COLORSPACE_YCbCr420:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_TRUE;
                bDwsmVertEnalbe = HI_TRUE;
    			break;

    		case HDMI_COLORSPACE_RGB:
    		    bInRgb          = HI_FALSE;
                bOutRgb         = HI_TRUE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;

		    case HDMI_COLORSPACE_YCbCr444:
		    default:
                bInRgb          = HI_FALSE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;

	    }
    }
	else if (enInColorSpace == HDMI_COLORSPACE_RGB )
    {

        switch(enOutColorSpace)
    	{
    		case HDMI_COLORSPACE_YCbCr444:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;

    		case HDMI_COLORSPACE_YCbCr422:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_TRUE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;

    		case HDMI_COLORSPACE_YCbCr420:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_FALSE;
                bCscEnalbe      = HI_TRUE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_TRUE;
                bY420Enalbe     = HI_TRUE;
                bDwsmVertEnalbe = HI_TRUE;
    			break;

    		case HDMI_COLORSPACE_RGB:
    		default:
                bInRgb          = HI_TRUE;
                bOutRgb         = HI_TRUE;
                bCscEnalbe      = HI_FALSE;
                bY422Enalbe     = HI_FALSE;
                bDwsmHoriEnalbe = HI_FALSE;
                bY420Enalbe     = HI_FALSE;
                bDwsmVertEnalbe = HI_FALSE;
    			break;


        }
	}
	/* bypass */
	else if (enInColorSpace == HDMI_COLORSPACE_YCbCr422)
	{
        bInRgb          = HI_FALSE;
        bOutRgb         = HI_FALSE;
        bCscEnalbe      = HI_FALSE;
        bY422Enalbe     = HI_FALSE;
        bDwsmHoriEnalbe = HI_FALSE;
        bY420Enalbe     = HI_FALSE;
        bDwsmVertEnalbe = HI_FALSE;
	}
	else
	{
        HDMI_WARN("Ctrl un-expected enInColorSpace=%u\n",enInColorSpace);
	}

	HDMI_INFO(  "Ctrl csc set:\n"
	            "bInRgb=%d,bOutRgb=%d,bCscEnalbe=%d \n"
	            "bY422Enalbe=%d,bDwsmHoriEnalbe=%d \n"
	            "bY420Enalbe=%d,bDwsmVertEnalbe=%d \n",
            	bInRgb,bOutRgb,bCscEnalbe,
            	bY422Enalbe,bDwsmHoriEnalbe,
            	bY420Enalbe,bDwsmVertEnalbe);

    CtrlVideoColorRgbSet(bInRgb,bOutRgb);
    CtrlVideoColorYCbCr422Set(bY422Enalbe);
    CtrlVideoColorDwsmHoriSet(bDwsmHoriEnalbe);
    CtrlVideoColorYCbCr420Set(bY420Enalbe);
    CtrlVideoColorDwsmVertSet(bDwsmVertEnalbe);
    CtrlVideoColorCscSet(bCscEnalbe);
	return ;
}


static HI_S32 CtrlVideoPathColorSpaceGet(HDMI_COLORSPACE_E *penInColorSpace,HDMI_COLORSPACE_E *penOutColorSpace)
{
    HI_BOOL     bInRgb          = HI_FALSE;
    HI_BOOL     bOutRgb         = HI_FALSE;

    CTRL_NULL_CHK(penInColorSpace);
    CTRL_NULL_CHK(penOutColorSpace);

    CtrlVideoColorRgbGet(&bInRgb,&bOutRgb);
    /* in */
    *penInColorSpace = bInRgb ? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCbCr444;

    /* out */
    *penOutColorSpace = bOutRgb ? HDMI_COLORSPACE_RGB : HDMI_COLORSPACE_YCbCr444;
    if (CtrlVideoColorYCbCr422Get() && CtrlVideoColorDwsmHoriGet())
    {
        *penOutColorSpace = HDMI_COLORSPACE_YCbCr422;
    }

    if (CtrlVideoColorYCbCr420Get() && CtrlVideoColorDwsmVertGet())
    {
        *penOutColorSpace = HDMI_COLORSPACE_YCbCr420;
    }

    return HI_SUCCESS;

}


static HI_VOID CtrlVideoPathColormetrySet(HDMI_COLORMETRY_E enInColormetry,HDMI_COLORMETRY_E enOutColormetry)
{
    HI_U32 u32CscMode = 0;

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    enOutColormetry &= CTRL_COLORMETRY_MASK;
    u32CscMode &= CTRL_COLORMETRY_OUT_MASK;
    u32CscMode |= enOutColormetry << CTRL_COLORMETRY_OUT_BIT;

    enInColormetry &= CTRL_COLORMETRY_MASK;
    u32CscMode &= CTRL_COLORMETRY_IN_MASK;
    u32CscMode |= enInColormetry << CTRL_COLORMETRY_IN_BIT;

    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32CscMode);

    return ;
}

static HI_S32 CtrlVideoPathColormetryGet(HDMI_COLORMETRY_E *penInColormetry,HDMI_COLORMETRY_E *penOutColormetry)
{
    HI_U32 u32CscMode = 0;

    CTRL_NULL_CHK(penInColormetry);
    CTRL_NULL_CHK(penOutColormetry);

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    *penOutColormetry = (u32CscMode & CTRL_COLORMETRY_OUT_MASK) >> CTRL_COLORMETRY_OUT_BIT;
    *penInColormetry = (u32CscMode & CTRL_COLORMETRY_OUT_MASK) >> CTRL_COLORMETRY_IN_BIT;

    return HI_SUCCESS;
}



static HI_VOID CtrlVideoPathQuantizationSet(HDMI_QUANTIZATION_RANGE enInQuantization,HDMI_QUANTIZATION_RANGE enOutQuantization)
{
    HI_U32 u32CscMode = 0;

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    if (HDMI_QUANTIZATION_RANGE_LIMITED != enInQuantization)
    {
        CTRL_SET_BIT(u32CscMode,CTRL_QUANTIZAION_IN_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32CscMode,CTRL_QUANTIZAION_IN_BIT);
    }


    if (HDMI_QUANTIZATION_RANGE_LIMITED != enOutQuantization)
    {
        CTRL_SET_BIT(u32CscMode,CTRL_QUANTIZAION_OUT_BIT);
    }
    else
    {
        CTRL_CLR_BIT(u32CscMode,CTRL_QUANTIZAION_OUT_BIT);
    }

    HDMI_MULTI_CSC_CTRL_reg_csc_modeSet(u32CscMode);
    HDMI_MULTI_CSC_CTRL_reg_csc_saturate_enSet(HI_TRUE);

    return ;
}


static HI_S32 CtrlVideoPathQuantizationGet(HDMI_QUANTIZATION_RANGE *penInQuantization,HDMI_QUANTIZATION_RANGE *penOutQuantization)
{
    HI_U32 u32CscMode = 0;

    CTRL_NULL_CHK(penInQuantization);
    CTRL_NULL_CHK(penOutQuantization);

    u32CscMode = HDMI_MULTI_CSC_CTRL_reg_csc_modeGet();

    *penInQuantization  = IS_BIT_SET(u32CscMode,CTRL_QUANTIZAION_IN_BIT) ? HDMI_QUANTIZATION_RANGE_FULL : HDMI_QUANTIZATION_RANGE_LIMITED ;
    *penOutQuantization = IS_BIT_SET(u32CscMode,CTRL_QUANTIZAION_OUT_BIT) ? HDMI_QUANTIZATION_RANGE_FULL : HDMI_QUANTIZATION_RANGE_LIMITED ;

    return HI_SUCCESS;
}


static HI_S32 CtrlTimmingDecectGet(CTRL_TIMMING_DETECT_S *pstTimming)
{
    CTRL_NULL_CHK(pstTimming);

    pstTimming->bSyncSwEnable = HI_FALSE;
    HDMI_FORMAT_DET_CONFIG_sync_polarity_forceSet(pstTimming->bSyncSwEnable);

    pstTimming->bVsyncPolarity  = HDMI_FDET_STATUS_vsync_polarityGet() ? HI_TRUE : HI_FALSE;
    pstTimming->bHsyncPolarity  = HDMI_FDET_STATUS_hsync_polarityGet() ? HI_TRUE : HI_FALSE;
    pstTimming->bProgressive    = HDMI_FDET_STATUS_interlacedGet() ? HI_FALSE : HI_TRUE; // NOTE: register table invert

    pstTimming->u32HsyncTotal   = HDMI_FDET_HORI_RES_hsync_total_cntGet();
    pstTimming->u32HactiveCnt   = HDMI_FDET_HORI_RES_hsync_active_cntGet();

    pstTimming->u32VsyncTotal   = HDMI_FDET_VERT_RES_vsync_total_cntGet();
    pstTimming->u32VactiveCnt   = HDMI_FDET_VERT_RES_vsync_active_cntGet();

    /* clear format detect status */
    HDMI_FORMAT_DET_CONFIG_fdt_status_clearSet(HI_FALSE);
    HDMI_FORMAT_DET_CONFIG_fdt_status_clearSet(HI_TRUE);

    return HI_SUCCESS;

}

//#define CTRL_BLACK_DATA_RGB         0x4010040
//#define CTRL_BLACK_DATA_YUV         0x20010200

#define CTRL_BLACK_DATA_YUV_CR      (0x80<<2)
#define CTRL_BLACK_DATA_YUV_Y       (0x10<<2)
#define CTRL_BLACK_DATA_YUV_CB      (0x80<<2)

#define CTRL_BLACK_DATA_RGB_R       (0x10<<2)
#define CTRL_BLACK_DATA_RGB_G       (0x10<<2)
#define CTRL_BLACK_DATA_RGB_B       (0x10<<2)

static HI_VOID CtrlVideoMuteSet(HI_BOOL bEnable)
{

	//HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet(bEnable);
    /* black color set ,  olny valid when hdcp1x_vmute_ctrl is set.*/
    //HDMI_VIDEO_BLANK_CFG_video_blankSet(CTRL_BLACK_Y_Cb_Cr);
//#if defined (HIFONEV300)
    HDMI_COLORSPACE_E       enInColorSpace;
    HDMI_COLORSPACE_E       enOutColorSpace;
    HI_U32                  u32DataValue = 0;
    CtrlVideoPathColorSpaceGet(&enInColorSpace,&enOutColorSpace);

    u32DataValue = (enInColorSpace==HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_R : CTRL_BLACK_DATA_YUV_CR;
    HDMI_SOLID_PATTERN_CONFIG_solid_pattern_crSet(u32DataValue);
    u32DataValue = (enInColorSpace==HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_G : CTRL_BLACK_DATA_YUV_Y;
    HDMI_SOLID_PATTERN_CONFIG_solid_pattern_ySet(u32DataValue);
    u32DataValue = (enInColorSpace==HDMI_COLORSPACE_RGB) ? CTRL_BLACK_DATA_RGB_B : CTRL_BLACK_DATA_YUV_CB;
    HDMI_SOLID_PATTERN_CONFIG_solid_pattern_cbSet(u32DataValue);

    HDMI_PATTERN_GEN_CTRL_tpg_enableSet(bEnable);
    HDMI_PATTERN_GEN_CTRL_solid_pattern_enSet(bEnable);
//#else
    //HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet(bEnable);
//#endif
    return ;
}

static HI_S32 CtrlVideoMuteGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);
//#if defined (HIFONEV300)
    if(HDMI_PATTERN_GEN_CTRL_tpg_enableGet()
    && HDMI_PATTERN_GEN_CTRL_solid_pattern_enGet())
//#else
//    if(HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlGet())
//#endif
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }
    return HI_SUCCESS;
}


static HI_S32 CtrlTmdsModeSet(HDMI_TMDS_MODE_E enTmdsMode)
{
    switch(enTmdsMode)
    {
        case HDMI_TMDS_MODE_DVI:
            HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_FALSE);

            break;
        case HDMI_TMDS_MODE_HDMI_1_4:
			HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_TRUE);
			HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(HI_FALSE);
			//TMDS Clock 1/10
			//pHwObj->enClockDiv	=   HDMI_CLOCK_DIV_1 ;
			//HDMI_HDMI_ENC_CTRL_enc_ck_div_selSet(pHwObj->enClockDiv);
			//Scramble  ≈‰÷√0
			//HDMI_HDMI_ENC_CTRL_enc_scr_onSet(HI_FALSE);
            break;
        case HDMI_TMDS_MODE_HDMI_2_0:
			HDMI_AVMIXER_CONFIG_hdmi_modeSet(HI_TRUE);
			HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(HI_TRUE);
			HDMI_HDMI_ENC_CTRL_enc_bypassSet(HI_FALSE);
			//TMDS Clock 1/40
			//pHwObj->enClockDiv	=   HDMI_CLOCK_DIV_4 ;
			//HDMI_HDMI_ENC_CTRL_enc_ck_div_selSet(pHwObj->enClockDiv);
			//Scramble  ≈‰÷√1
			//HDMI_HDMI_ENC_CTRL_enc_scr_onSet(HI_TRUE);
            break;
        default:
            HDMI_WARN("un-known tmds mode:%u\n",enTmdsMode);
            return HI_FAILURE;
            //break;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlTmdsModeGet(HDMI_TMDS_MODE_E *penTmdsMode)
{
    HI_BOOL bHdmiMode       = HI_FALSE;
    HI_BOOL bHdmi2xEnable   = HI_FALSE;
    CTRL_NULL_CHK(penTmdsMode);

    bHdmiMode       = HDMI_AVMIXER_CONFIG_hdmi_modeGet();
    bHdmi2xEnable   = HDMI_HDMI_ENC_CTRL_enc_hdmi2_onGet();
    //bScrambleOpen = HDMI_HDMI_ENC_CTRL_enc_scr_onGet();
	//u8TmdsClockDiv =HDMI_HDMI_ENC_CTRL_enc_ck_div_selGet();

    if (!bHdmiMode)
    {
        *penTmdsMode = HDMI_TMDS_MODE_DVI;
    }
    else if(bHdmiMode && (!bHdmi2xEnable))
    {
        *penTmdsMode = HDMI_TMDS_MODE_HDMI_1_4;
    }
    else if(bHdmiMode && bHdmi2xEnable)
    {
        *penTmdsMode = HDMI_TMDS_MODE_HDMI_2_0;
    }
    else
    {
        HDMI_WARN("un-config tmds mode!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlVendorInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enSet(bEnable);
    HDMI_CEA_VSIF_CFG_cea_vsif_enSet(bEnable);

    return HI_SUCCESS;
}

static HI_S32 CtrlVendorInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    //if ( HDMI_CEA_VSIF_CFG_cea_vsif_enGet())
    if (HDMI_CEA_VSIF_CFG_cea_vsif_rpt_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;

}

static HI_S32 CtrlVendorInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    /* HB config */
    HDMI_VSIF_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    /* pkt0 config */
    HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    /* pkt1 config */
    HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    /* pkt2 config */
    HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    /* pkt3 config */
    HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlVendorInfoframeDataGet(HI_U8 *pu8IfData)
{
    U_VSIF_PKT_HEADER o_vsif_pkt_header;
    U_VSIF_SUB_PKT0_L o_vsif_sub_pkt0_l;
    U_VSIF_SUB_PKT0_H o_vsif_sub_pkt0_h;
    U_VSIF_SUB_PKT1_L o_vsif_sub_pkt1_l;
    U_VSIF_SUB_PKT1_H o_vsif_sub_pkt1_h;
    U_VSIF_SUB_PKT2_L o_vsif_sub_pkt2_l;
    U_VSIF_SUB_PKT2_H o_vsif_sub_pkt2_h;
    U_VSIF_SUB_PKT3_L o_vsif_sub_pkt3_l;
    U_VSIF_SUB_PKT3_H o_vsif_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_VSIF_PKT_HEADER_hbGet(&o_vsif_pkt_header);
    HDMI_VSIF_SUB_PKT0_L_vsif_pkt0_pbGet(&o_vsif_sub_pkt0_l);
    HDMI_VSIF_SUB_PKT0_H_vsif_pkt0_pbGet(&o_vsif_sub_pkt0_h);
    HDMI_VSIF_SUB_PKT1_L_vsif_pkt1_pbGet(&o_vsif_sub_pkt1_l);
    HDMI_VSIF_SUB_PKT1_H_vsif_pkt1_pbGet(&o_vsif_sub_pkt1_h);
    HDMI_VSIF_SUB_PKT2_L_vsif_pkt2_pbGet(&o_vsif_sub_pkt2_l);
    HDMI_VSIF_SUB_PKT2_H_vsif_pkt2_pbGet(&o_vsif_sub_pkt2_h);
    HDMI_VSIF_SUB_PKT3_L_vsif_pkt3_pbGet(&o_vsif_sub_pkt3_l);
    HDMI_VSIF_SUB_PKT3_H_vsif_pkt3_pbGet(&o_vsif_sub_pkt3_h);

    pu8IfData[0] = o_vsif_pkt_header.bits.vsif_pkt_hb0;
    pu8IfData[1] = o_vsif_pkt_header.bits.vsif_pkt_hb1;
    pu8IfData[2] = o_vsif_pkt_header.bits.vsif_pkt_hb2;

    pu8IfData[3] = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb0;
    pu8IfData[4] = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb1;
    pu8IfData[5] = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb2;
    pu8IfData[6] = o_vsif_sub_pkt0_l.bits.vsif_sub_pkt0_pb3;
    pu8IfData[7] = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb4;
    pu8IfData[8] = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb5;
    pu8IfData[9] = o_vsif_sub_pkt0_h.bits.vsif_sub_pkt0_pb6;

    pu8IfData[10] = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb0;
    pu8IfData[11] = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb1;
    pu8IfData[12] = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb2;
    pu8IfData[13] = o_vsif_sub_pkt1_l.bits.vsif_sub_pkt1_pb3;
    pu8IfData[14] = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb4;
    pu8IfData[15] = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb5;
    pu8IfData[16] = o_vsif_sub_pkt1_h.bits.vsif_sub_pkt1_pb6;

    pu8IfData[17] = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb0;
    pu8IfData[18] = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb1;
    pu8IfData[19] = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb2;
    pu8IfData[20] = o_vsif_sub_pkt2_l.bits.vsif_sub_pkt2_pb3;
    pu8IfData[21] = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb4;
    pu8IfData[22] = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb5;
    pu8IfData[23] = o_vsif_sub_pkt2_h.bits.vsif_sub_pkt2_pb6;

    pu8IfData[24] = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb0;
    pu8IfData[25] = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb1;
    pu8IfData[26] = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb2;
    pu8IfData[27] = o_vsif_sub_pkt3_l.bits.vsif_sub_pkt3_pb3;
    pu8IfData[28] = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb4;
    pu8IfData[29] = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb5;
    pu8IfData[30] = o_vsif_sub_pkt3_h.bits.vsif_sub_pkt3_pb6;

    return HI_SUCCESS;

}

static HI_S32 CtrlAviInfoFrameEnSet(HI_BOOL bEnable)
{
    /* Repeat Enable */
    HDMI_CEA_AVI_CFG_cea_avi_rpt_enSet(bEnable);
    /* InfoFrame Enable */
    HDMI_CEA_AVI_CFG_cea_avi_enSet(bEnable);

    return HI_SUCCESS;
}

static HI_S32 CtrlAviInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_AVI_CFG_cea_avi_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlAviInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    /* HB config */
    HDMI_AVI_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlAviInfoframeDataGet(HI_U8 *pu8IfData)
{
    U_AVI_PKT_HEADER o_avi_pkt_header;
    U_AVI_SUB_PKT0_L o_avi_sub_pkt0_l;
    U_AVI_SUB_PKT0_H o_avi_sub_pkt0_h;
    U_AVI_SUB_PKT1_L o_avi_sub_pkt1_l;
    U_AVI_SUB_PKT1_H o_avi_sub_pkt1_h;
    U_AVI_SUB_PKT2_L o_avi_sub_pkt2_l;
    U_AVI_SUB_PKT2_H o_avi_sub_pkt2_h;
    U_AVI_SUB_PKT3_L o_avi_sub_pkt3_l;
    U_AVI_SUB_PKT3_H o_avi_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_AVI_PKT_HEADER_hbGet(&o_avi_pkt_header);
    HDMI_AVI_SUB_PKT0_L_avi_pkt0_pbGet(&o_avi_sub_pkt0_l);
    HDMI_AVI_SUB_PKT0_H_avi_pkt0_pbGet(&o_avi_sub_pkt0_h);
    HDMI_AVI_SUB_PKT1_L_avi_pkt1_pbGet(&o_avi_sub_pkt1_l);
    HDMI_AVI_SUB_PKT1_H_avi_pkt1_pbGet(&o_avi_sub_pkt1_h);
    HDMI_AVI_SUB_PKT2_L_avi_pkt2_pbGet(&o_avi_sub_pkt2_l);
    HDMI_AVI_SUB_PKT2_H_avi_pkt2_pbGet(&o_avi_sub_pkt2_h);
    HDMI_AVI_SUB_PKT3_L_avi_pkt3_pbGet(&o_avi_sub_pkt3_l);
    HDMI_AVI_SUB_PKT3_H_avi_pkt3_pbGet(&o_avi_sub_pkt3_h);

    pu8IfData[0] = o_avi_pkt_header.bits.avi_pkt_hb0;
    pu8IfData[1] = o_avi_pkt_header.bits.avi_pkt_hb1;
    pu8IfData[2] = o_avi_pkt_header.bits.avi_pkt_hb2;

    pu8IfData[3] = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb0;
    pu8IfData[4] = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb1;
    pu8IfData[5] = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb2;
    pu8IfData[6] = o_avi_sub_pkt0_l.bits.avi_sub_pkt0_pb3;
    pu8IfData[7] = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb4;
    pu8IfData[8] = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb5;
    pu8IfData[9] = o_avi_sub_pkt0_h.bits.avi_sub_pkt0_pb6;

    pu8IfData[10] = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb0;
    pu8IfData[11] = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb1;
    pu8IfData[12] = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb2;
    pu8IfData[13] = o_avi_sub_pkt1_l.bits.avi_sub_pkt1_pb3;
    pu8IfData[14] = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb4;
    pu8IfData[15] = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb5;
    pu8IfData[16] = o_avi_sub_pkt1_h.bits.avi_sub_pkt1_pb6;

    pu8IfData[17] = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb0;
    pu8IfData[18] = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb1;
    pu8IfData[19] = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb2;
    pu8IfData[20] = o_avi_sub_pkt2_l.bits.avi_sub_pkt2_pb3;
    pu8IfData[21] = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb4;
    pu8IfData[22] = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb5;
    pu8IfData[23] = o_avi_sub_pkt2_h.bits.avi_sub_pkt2_pb6;

    pu8IfData[24] = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb0;
    pu8IfData[25] = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb1;
    pu8IfData[26] = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb2;
    pu8IfData[27] = o_avi_sub_pkt3_l.bits.avi_sub_pkt3_pb3;
    pu8IfData[28] = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb4;
    pu8IfData[29] = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb5;
    pu8IfData[30] = o_avi_sub_pkt3_h.bits.avi_sub_pkt3_pb6;

    return HI_SUCCESS;
}

static HI_S32 CtrlSpdInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_SPF_CFG_cea_spf_rpt_enSet(bEnable);
    HDMI_CEA_SPF_CFG_cea_spf_enSet(bEnable);
    return HI_SUCCESS;
}

static HI_S32 CtrlSpdInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_SPF_CFG_cea_spf_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlSpdInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_SPIF_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlSpdInfoframeDataGet(HI_U8 *pu8IfData)
{
    U_SPIF_PKT_HEADER o_spif_pkt_header;
    U_SPIF_SUB_PKT0_L o_spif_sub_pkt0_l;
    U_SPIF_SUB_PKT0_H o_spif_sub_pkt0_h;
    U_SPIF_SUB_PKT1_L o_spif_sub_pkt1_l;
    U_SPIF_SUB_PKT1_H o_spif_sub_pkt1_h;
    U_SPIF_SUB_PKT2_L o_spif_sub_pkt2_l;
    U_SPIF_SUB_PKT2_H o_spif_sub_pkt2_h;
    U_SPIF_SUB_PKT3_L o_spif_sub_pkt3_l;
    U_SPIF_SUB_PKT3_H o_spif_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_SPIF_PKT_HEADER_hbGet(&o_spif_pkt_header);
    HDMI_SPIF_SUB_PKT0_L_spif_pkt0_pbGet(&o_spif_sub_pkt0_l);
    HDMI_SPIF_SUB_PKT0_H_spif_pkt0_pbGet(&o_spif_sub_pkt0_h);
    HDMI_SPIF_SUB_PKT1_L_spif_pkt1_pbGet(&o_spif_sub_pkt1_l);
    HDMI_SPIF_SUB_PKT1_H_spif_pkt1_pbGet(&o_spif_sub_pkt1_h);
    HDMI_SPIF_SUB_PKT2_L_spif_pkt2_pbGet(&o_spif_sub_pkt2_l);
    HDMI_SPIF_SUB_PKT2_H_spif_pkt2_pbGet(&o_spif_sub_pkt2_h);
    HDMI_SPIF_SUB_PKT3_L_spif_pkt3_pbGet(&o_spif_sub_pkt3_l);
    HDMI_SPIF_SUB_PKT3_H_spif_pkt3_pbGet(&o_spif_sub_pkt3_h);

    pu8IfData[0] = o_spif_pkt_header.bits.spd_pkt_hb0;
    pu8IfData[1] = o_spif_pkt_header.bits.spd_pkt_hb1;
    pu8IfData[2] = o_spif_pkt_header.bits.spd_pkt_hb2;

    pu8IfData[3] = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb0;
    pu8IfData[4] = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb1;
    pu8IfData[5] = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb2;
    pu8IfData[6] = o_spif_sub_pkt0_l.bits.spd_sub_pkt0_pb3;
    pu8IfData[7] = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb4;
    pu8IfData[8] = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb5;
    pu8IfData[9] = o_spif_sub_pkt0_h.bits.spd_sub_pkt0_pb6;

    pu8IfData[10] = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb0;
    pu8IfData[11] = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb1;
    pu8IfData[12] = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb2;
    pu8IfData[13] = o_spif_sub_pkt1_l.bits.spd_sub_pkt1_pb3;
    pu8IfData[14] = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb4;
    pu8IfData[15] = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb5;
    pu8IfData[16] = o_spif_sub_pkt1_h.bits.spd_sub_pkt1_pb6;

    pu8IfData[17] = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb0;
    pu8IfData[18] = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb1;
    pu8IfData[19] = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb2;
    pu8IfData[20] = o_spif_sub_pkt2_l.bits.spd_sub_pkt2_pb3;
    pu8IfData[21] = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb4;
    pu8IfData[22] = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb5;
    pu8IfData[23] = o_spif_sub_pkt2_h.bits.spd_sub_pkt2_pb6;

    pu8IfData[24] = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb0;
    pu8IfData[25] = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb1;
    pu8IfData[26] = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb2;
    pu8IfData[27] = o_spif_sub_pkt3_l.bits.spd_sub_pkt3_pb3;
    pu8IfData[28] = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb4;
    pu8IfData[29] = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb5;
    pu8IfData[30] = o_spif_sub_pkt3_h.bits.spd_sub_pkt3_pb6;

    return HI_SUCCESS;
}


static HI_S32 CtrlAudioInfoFrameEnSet(HI_BOOL bEnalbe)
{
    /* Repeat Enable */
    HDMI_CEA_AUD_CFG_cea_aud_rpt_enSet(bEnalbe);
    /* InfoFrame Enable */
    HDMI_CEA_AUD_CFG_cea_aud_enSet(bEnalbe);
    return HI_SUCCESS;
}

static HI_S32 CtrlAudioInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_AUD_CFG_cea_aud_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlAudioInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_AUDIO_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    /* pkt0 config */
    HDMI_AUDIO_SUB_PKT0_L_audio_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_AUDIO_SUB_PKT0_H_audio_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    /* pkt1 config */
    HDMI_AUDIO_SUB_PKT1_L_audio_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_AUDIO_SUB_PKT1_H_audio_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    /* pkt2 config */
    HDMI_AUDIO_SUB_PKT2_L_audio_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_AUDIO_SUB_PKT2_H_audio_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    /* pkt3 config */
    HDMI_AUDIO_SUB_PKT3_L_audio_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_AUDIO_SUB_PKT3_H_audio_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlAudioInfoframeDataGet(HI_U8 *pu8IfData)
{
    U_AIF_PKT_HEADER o_aif_pkt_header;
    U_AIF_SUB_PKT0_L o_aif_sub_pkt0_l;
    U_AIF_SUB_PKT0_H o_aif_sub_pkt0_h;
    U_AIF_SUB_PKT1_L o_aif_sub_pkt1_l;
    U_AIF_SUB_PKT1_H o_aif_sub_pkt1_h;
    U_AIF_SUB_PKT2_L o_aif_sub_pkt2_l;
    U_AIF_SUB_PKT2_H o_aif_sub_pkt2_h;
    U_AIF_SUB_PKT3_L o_aif_sub_pkt3_l;
    U_AIF_SUB_PKT3_H o_aif_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_AIF_PKT_HEADER_hbGet(&o_aif_pkt_header);
    HDMI_AIF_SUB_PKT0_L_aif_pkt0_pbGet(&o_aif_sub_pkt0_l);
    HDMI_AIF_SUB_PKT0_H_aif_pkt0_pbGet(&o_aif_sub_pkt0_h);
    HDMI_AIF_SUB_PKT1_L_aif_pkt1_pbGet(&o_aif_sub_pkt1_l);
    HDMI_AIF_SUB_PKT1_H_aif_pkt1_pbGet(&o_aif_sub_pkt1_h);
    HDMI_AIF_SUB_PKT2_L_aif_pkt2_pbGet(&o_aif_sub_pkt2_l);
    HDMI_AIF_SUB_PKT2_H_aif_pkt2_pbGet(&o_aif_sub_pkt2_h);
    HDMI_AIF_SUB_PKT3_L_aif_pkt3_pbGet(&o_aif_sub_pkt3_l);
    HDMI_AIF_SUB_PKT3_H_aif_pkt3_pbGet(&o_aif_sub_pkt3_h);

    pu8IfData[0] = o_aif_pkt_header.bits.aif_pkt_hb0;
    pu8IfData[1] = o_aif_pkt_header.bits.aif_pkt_hb1;
    pu8IfData[2] = o_aif_pkt_header.bits.aif_pkt_hb2;

    pu8IfData[3] = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb0;
    pu8IfData[4] = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb1;
    pu8IfData[5] = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb2;
    pu8IfData[6] = o_aif_sub_pkt0_l.bits.aif_sub_pkt0_pb3;
    pu8IfData[7] = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb4;
    pu8IfData[8] = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb5;
    pu8IfData[9] = o_aif_sub_pkt0_h.bits.aif_sub_pkt0_pb6;

    pu8IfData[10] = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb0;
    pu8IfData[11] = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb1;
    pu8IfData[12] = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb2;
    pu8IfData[13] = o_aif_sub_pkt1_l.bits.aif_sub_pkt1_pb3;
    pu8IfData[14] = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb4;
    pu8IfData[15] = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb5;
    pu8IfData[16] = o_aif_sub_pkt1_h.bits.aif_sub_pkt1_pb6;

    pu8IfData[17] = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb0;
    pu8IfData[18] = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb1;
    pu8IfData[19] = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb2;
    pu8IfData[20] = o_aif_sub_pkt2_l.bits.aif_sub_pkt2_pb3;
    pu8IfData[21] = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb4;
    pu8IfData[22] = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb5;
    pu8IfData[23] = o_aif_sub_pkt2_h.bits.aif_sub_pkt2_pb6;

    pu8IfData[24] = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb0;
    pu8IfData[25] = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb1;
    pu8IfData[26] = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb2;
    pu8IfData[27] = o_aif_sub_pkt3_l.bits.aif_sub_pkt3_pb3;
    pu8IfData[28] = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb4;
    pu8IfData[29] = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb5;
    pu8IfData[30] = o_aif_sub_pkt3_h.bits.aif_sub_pkt3_pb6;

    return HI_SUCCESS;
}

static HI_S32 CtrlMpegInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_MPEG_CFG_cea_mpeg_rpt_enSet(bEnable);
    HDMI_CEA_MPEG_CFG_cea_mpeg_enSet(bEnable);
    return HI_SUCCESS;
}

static HI_S32 CtrlMpegInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_MPEG_CFG_cea_mpeg_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlMpegInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    /* HB config */
    HDMI_MPEG_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    /* pkt config */
    HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlMpegInfoframeDataGet(HI_U8 *pu8IfData)
{
    U_MPEG_PKT_HEADER o_mpeg_pkt_header;
    U_MPEG_SUB_PKT0_L o_mpeg_sub_pkt0_l;
    U_MPEG_SUB_PKT0_H o_mpeg_sub_pkt0_h;
    U_MPEG_SUB_PKT1_L o_mpeg_sub_pkt1_l;
    U_MPEG_SUB_PKT1_H o_mpeg_sub_pkt1_h;
    U_MPEG_SUB_PKT2_L o_mpeg_sub_pkt2_l;
    U_MPEG_SUB_PKT2_H o_mpeg_sub_pkt2_h;
    U_MPEG_SUB_PKT3_L o_mpeg_sub_pkt3_l;
    U_MPEG_SUB_PKT3_H o_mpeg_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_MPEG_PKT_HEADER_hbGet(&o_mpeg_pkt_header);
    HDMI_MPEG_SUB_PKT0_L_mpeg_pkt0_pbGet(&o_mpeg_sub_pkt0_l);
    HDMI_MPEG_SUB_PKT0_H_mpeg_pkt0_pbGet(&o_mpeg_sub_pkt0_h);
    HDMI_MPEG_SUB_PKT1_L_mpeg_pkt1_pbGet(&o_mpeg_sub_pkt1_l);
    HDMI_MPEG_SUB_PKT1_H_mpeg_pkt1_pbGet(&o_mpeg_sub_pkt1_h);
    HDMI_MPEG_SUB_PKT2_L_mpeg_pkt2_pbGet(&o_mpeg_sub_pkt2_l);
    HDMI_MPEG_SUB_PKT2_H_mpeg_pkt2_pbGet(&o_mpeg_sub_pkt2_h);
    HDMI_MPEG_SUB_PKT3_L_mpeg_pkt3_pbGet(&o_mpeg_sub_pkt3_l);
    HDMI_MPEG_SUB_PKT3_H_mpeg_pkt3_pbGet(&o_mpeg_sub_pkt3_h);

    pu8IfData[0] = o_mpeg_pkt_header.bits.mpeg_pkt_hb0;
    pu8IfData[1] = o_mpeg_pkt_header.bits.mpeg_pkt_hb1;
    pu8IfData[2] = o_mpeg_pkt_header.bits.mpeg_pkt_hb2;

    pu8IfData[3] = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb0;
    pu8IfData[4] = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb1;
    pu8IfData[5] = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb2;
    pu8IfData[6] = o_mpeg_sub_pkt0_l.bits.mpeg_sub_pkt0_pb3;
    pu8IfData[7] = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb4;
    pu8IfData[8] = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb5;
    pu8IfData[9] = o_mpeg_sub_pkt0_h.bits.mpeg_sub_pkt0_pb6;

    pu8IfData[10] = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb0;
    pu8IfData[11] = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb1;
    pu8IfData[12] = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb2;
    pu8IfData[13] = o_mpeg_sub_pkt1_l.bits.mpeg_sub_pkt1_pb3;
    pu8IfData[14] = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb4;
    pu8IfData[15] = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb5;
    pu8IfData[16] = o_mpeg_sub_pkt1_h.bits.mpeg_sub_pkt1_pb6;

    pu8IfData[17] = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb0;
    pu8IfData[18] = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb1;
    pu8IfData[19] = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb2;
    pu8IfData[20] = o_mpeg_sub_pkt2_l.bits.mpeg_sub_pkt2_pb3;
    pu8IfData[21] = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb4;
    pu8IfData[22] = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb5;
    pu8IfData[23] = o_mpeg_sub_pkt2_h.bits.mpeg_sub_pkt2_pb6;

    pu8IfData[24] = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb0;
    pu8IfData[25] = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb1;
    pu8IfData[26] = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb2;
    pu8IfData[27] = o_mpeg_sub_pkt3_l.bits.mpeg_sub_pkt3_pb3;
    pu8IfData[28] = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb4;
    pu8IfData[29] = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb5;
    pu8IfData[30] = o_mpeg_sub_pkt3_h.bits.mpeg_sub_pkt3_pb6;

    return HI_SUCCESS;
}

static HI_S32 CtrlGbdInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_GAMUT_CFG_cea_gamut_rpt_enSet(bEnable);
    HDMI_CEA_GAMUT_CFG_cea_gamut_enSet(bEnable);
    return HI_SUCCESS;
}

static HI_S32 CtrlGbdInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_GAMUT_CFG_cea_gamut_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlGbdInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }
    HDMI_GAMUT_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    /* pkt config */
    HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlGbdInfoframeDataGet(HI_U8 *pu8IfData)
{


    U_GAMUT_PKT_HEADER o_gamut_pkt_header;
    U_GAMUT_SUB_PKT0_L o_gamut_sub_pkt0_l;
    U_GAMUT_SUB_PKT0_H o_gamut_sub_pkt0_h;
    U_GAMUT_SUB_PKT1_L o_gamut_sub_pkt1_l;
    U_GAMUT_SUB_PKT1_H o_gamut_sub_pkt1_h;
    U_GAMUT_SUB_PKT2_L o_gamut_sub_pkt2_l;
    U_GAMUT_SUB_PKT2_H o_gamut_sub_pkt2_h;
    U_GAMUT_SUB_PKT3_L o_gamut_sub_pkt3_l;
    U_GAMUT_SUB_PKT3_H o_gamut_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_GAMUT_PKT_HEADER_hbGet(&o_gamut_pkt_header);
    HDMI_GAMUT_SUB_PKT0_L_gamut_pkt0_pbGet(&o_gamut_sub_pkt0_l);
    HDMI_GAMUT_SUB_PKT0_H_gamut_pkt0_pbGet(&o_gamut_sub_pkt0_h);
    HDMI_GAMUT_SUB_PKT1_L_gamut_pkt1_pbGet(&o_gamut_sub_pkt1_l);
    HDMI_GAMUT_SUB_PKT1_H_gamut_pkt1_pbGet(&o_gamut_sub_pkt1_h);
    HDMI_GAMUT_SUB_PKT2_L_gamut_pkt2_pbGet(&o_gamut_sub_pkt2_l);
    HDMI_GAMUT_SUB_PKT2_H_gamut_pkt2_pbGet(&o_gamut_sub_pkt2_h);
    HDMI_GAMUT_SUB_PKT3_L_gamut_pkt3_pbGet(&o_gamut_sub_pkt3_l);
    HDMI_GAMUT_SUB_PKT3_H_gamut_pkt3_pbGet(&o_gamut_sub_pkt3_h);

    pu8IfData[0] = o_gamut_pkt_header.bits.gamut_pkt_hb0;
    pu8IfData[1] = o_gamut_pkt_header.bits.gamut_pkt_hb1;
    pu8IfData[2] = o_gamut_pkt_header.bits.gamut_pkt_hb2;

    pu8IfData[3] = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb0;
    pu8IfData[4] = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb1;
    pu8IfData[5] = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb2;
    pu8IfData[6] = o_gamut_sub_pkt0_l.bits.gamut_sub_pkt0_pb3;
    pu8IfData[7] = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb4;
    pu8IfData[8] = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb5;
    pu8IfData[9] = o_gamut_sub_pkt0_h.bits.gamut_sub_pkt0_pb6;

    pu8IfData[10] = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb0;
    pu8IfData[11] = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb1;
    pu8IfData[12] = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb2;
    pu8IfData[13] = o_gamut_sub_pkt1_l.bits.gamut_sub_pkt1_pb3;
    pu8IfData[14] = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb4;
    pu8IfData[15] = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb5;
    pu8IfData[16] = o_gamut_sub_pkt1_h.bits.gamut_sub_pkt1_pb6;

    pu8IfData[17] = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb0;
    pu8IfData[18] = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb1;
    pu8IfData[19] = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb2;
    pu8IfData[20] = o_gamut_sub_pkt2_l.bits.gamut_sub_pkt2_pb3;
    pu8IfData[21] = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb4;
    pu8IfData[22] = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb5;
    pu8IfData[23] = o_gamut_sub_pkt2_h.bits.gamut_sub_pkt2_pb6;

    pu8IfData[24] = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb0;
    pu8IfData[25] = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb1;
    pu8IfData[26] = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb2;
    pu8IfData[27] = o_gamut_sub_pkt3_l.bits.gamut_sub_pkt3_pb3;
    pu8IfData[28] = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb4;
    pu8IfData[29] = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb5;
    pu8IfData[30] = o_gamut_sub_pkt3_h.bits.gamut_sub_pkt3_pb6;

    return HI_SUCCESS;
}

//GEN(1)
static HI_S32 CtrlDrmInfoFrameEnSet(HI_BOOL bEnable)
{
    HDMI_CEA_GEN_CFG_cea_gen_rpt_enSet(bEnable);
    HDMI_CEA_GEN_CFG_cea_gen_enSet(bEnable);
    return HI_SUCCESS;
}

static HI_S32 CtrlDrmInfoFrameEnGet(HI_BOOL *pbEnable)
{
    CTRL_NULL_CHK(pbEnable);

    if (HDMI_CEA_GEN_CFG_cea_gen_enGet())
    {
        *pbEnable = HI_TRUE;
    }
    else
    {
        *pbEnable = HI_FALSE;
    }

    return HI_SUCCESS;
}

static HI_S32 CtrlDrmInfoframeDataSet(HI_U8 *pu8IfData)
{
    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_GEN_PKT_HEADER_hbSet(pu8IfData[0],pu8IfData[1],pu8IfData[2]);
    HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbSet(pu8IfData[3],pu8IfData[4],pu8IfData[5],pu8IfData[6]);
    HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbSet(pu8IfData[7],pu8IfData[8],pu8IfData[9]);
    HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbSet(pu8IfData[10],pu8IfData[11],pu8IfData[12],pu8IfData[13]);
    HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbSet(pu8IfData[14],pu8IfData[15],pu8IfData[16]);
    HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbSet(pu8IfData[17],pu8IfData[18],pu8IfData[19],pu8IfData[20]);
    HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbSet(pu8IfData[21],pu8IfData[22],pu8IfData[23]);
    HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbSet(pu8IfData[24],pu8IfData[25],pu8IfData[26],pu8IfData[27]);
    HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbSet(pu8IfData[28],pu8IfData[29],pu8IfData[30]);

    return HI_SUCCESS;
}

static HI_S32 CtrlDrmInfoframeDataGet(HI_U8 *pu8IfData)
{


    U_GEN_PKT_HEADER o_gen_pkt_header;
    U_GEN_SUB_PKT0_L o_gen_sub_pkt0_l;
    U_GEN_SUB_PKT0_H o_gen_sub_pkt0_h;
    U_GEN_SUB_PKT1_L o_gen_sub_pkt1_l;
    U_GEN_SUB_PKT1_H o_gen_sub_pkt1_h;
    U_GEN_SUB_PKT2_L o_gen_sub_pkt2_l;
    U_GEN_SUB_PKT2_H o_gen_sub_pkt2_h;
    U_GEN_SUB_PKT3_L o_gen_sub_pkt3_l;
    U_GEN_SUB_PKT3_H o_gen_sub_pkt3_h;

    HI_U8 *pu8Tmp = HI_NULL;

    for (pu8Tmp = pu8IfData; pu8Tmp < pu8IfData+30; pu8Tmp++)
    {
        CTRL_NULL_CHK(pu8Tmp);
    }

    HDMI_GEN_PKT_HEADER_hbGet(&o_gen_pkt_header);
    HDMI_GEN_SUB_PKT0_L_gen_pkt0_pbGet(&o_gen_sub_pkt0_l);
    HDMI_GEN_SUB_PKT0_H_gen_pkt0_pbGet(&o_gen_sub_pkt0_h);
    HDMI_GEN_SUB_PKT1_L_gen_pkt1_pbGet(&o_gen_sub_pkt1_l);
    HDMI_GEN_SUB_PKT1_H_gen_pkt1_pbGet(&o_gen_sub_pkt1_h);
    HDMI_GEN_SUB_PKT2_L_gen_pkt2_pbGet(&o_gen_sub_pkt2_l);
    HDMI_GEN_SUB_PKT2_H_gen_pkt2_pbGet(&o_gen_sub_pkt2_h);
    HDMI_GEN_SUB_PKT3_L_gen_pkt3_pbGet(&o_gen_sub_pkt3_l);
    HDMI_GEN_SUB_PKT3_H_gen_pkt3_pbGet(&o_gen_sub_pkt3_h);

    pu8IfData[0] = o_gen_pkt_header.bits.gen_pkt_hb0;
    pu8IfData[1] = o_gen_pkt_header.bits.gen_pkt_hb1;
    pu8IfData[2] = o_gen_pkt_header.bits.gen_pkt_hb2;

    pu8IfData[3] = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb0;
    pu8IfData[4] = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb1;
    pu8IfData[5] = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb2;
    pu8IfData[6] = o_gen_sub_pkt0_l.bits.gen_sub_pkt0_pb3;
    pu8IfData[7] = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb4;
    pu8IfData[8] = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb5;
    pu8IfData[9] = o_gen_sub_pkt0_h.bits.gen_sub_pkt0_pb6;

    pu8IfData[10] = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb0;
    pu8IfData[11] = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb1;
    pu8IfData[12] = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb2;
    pu8IfData[13] = o_gen_sub_pkt1_l.bits.gen_sub_pkt1_pb3;
    pu8IfData[14] = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb4;
    pu8IfData[15] = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb5;
    pu8IfData[16] = o_gen_sub_pkt1_h.bits.gen_sub_pkt1_pb6;

    pu8IfData[17] = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb0;
    pu8IfData[18] = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb1;
    pu8IfData[19] = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb2;
    pu8IfData[20] = o_gen_sub_pkt2_l.bits.gen_sub_pkt2_pb3;
    pu8IfData[21] = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb4;
    pu8IfData[22] = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb5;
    pu8IfData[23] = o_gen_sub_pkt2_h.bits.gen_sub_pkt2_pb6;

    pu8IfData[24] = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb0;
    pu8IfData[25] = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb1;
    pu8IfData[26] = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb2;
    pu8IfData[27] = o_gen_sub_pkt3_l.bits.gen_sub_pkt3_pb3;
    pu8IfData[28] = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb4;
    pu8IfData[29] = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb5;
    pu8IfData[30] = o_gen_sub_pkt3_h.bits.gen_sub_pkt3_pb6;

    return HI_SUCCESS;
}






static  HI_BOOL CtrlHpdGet(HI_VOID)
{
    HI_BOOL bHpd;
    HI_BOOL bHotplugPol;

    bHotplugPol = HDMI_HOTPLUG_ST_CFG_hpd_polarity_ctlGet();
    bHpd        = HDMI_TX_AON_STATE_hotplug_stateGet();
    return bHotplugPol ? (!bHpd) : bHpd;
}


static HI_BOOL CtrlRsenGet(HI_VOID)
{
    if(HDMI_TX_AON_STATE_phy_rx_senseGet())
    {
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID CtrlHpdIntrEnalbe(HI_BOOL bEnalbe)
{
	HDMI_TX_AON_INTR_STATE_aon_intr_stat0Set(HI_TRUE);
    HDMI_TX_AON_INTR_MASK_aon_intr_mask0Set(bEnalbe);
    return ;
}

#ifndef HDMI_BUILD_IN_BOOT
#ifdef HDMI_HDR_SUPPORT
static HI_VOID CtrlHdrTimerInvoke(HDMI_CTRL_INFO_S *pstCtrlInfo)
{
    HI_U32              i = 0;
    HI_U64              u64TimeOut = 0;
    CTRL_HDR_TIMER_S    *pstHdrTimer = HI_NULL;

    if (!pstCtrlInfo)
    {
        return ;
    }

    for (i = 0; i<HDMI_TIMER_TYPE_BUTT ;i++)
    {
        pstHdrTimer = &pstCtrlInfo->astHdrTimer[i];
        if (pstHdrTimer->bEnable)
        {
            u64TimeOut = HAL_HDMI_MachMsGet() - pstHdrTimer->u64StartTime;
            if(u64TimeOut >= pstHdrTimer->u64TimeLength)
            {
                pstHdrTimer->bEnable = HI_FALSE;
                HDMI_INFO("ctrl timer %d disable!\n",pstHdrTimer->enTimerType);
                switch(pstHdrTimer->enTimerType)
                {
                    case HDMI_TIMER_ZERO_DRMIF:
                        pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_ZERO_DRMIF_TIMEOUT);
                        break;

                    case HDMI_TIMER_SDR_TO_HDR10:
                        pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_SWITCH_TO_HDRMODE_TIMEOUT);
                        break;
                    default :
                        break;
                }
            }
        }
    }

    return ;
}
#endif

HI_VOID CtrlMachPpdCallBack(HI_VOID *pvData)
{
    HDMI_CTRL_INFO_S  *pstCtrlInfo = (HDMI_CTRL_INFO_S  *)pvData;

    if ((!pvData )
        || (!pstCtrlInfo->pfnEventCallBack )
        || (!pstCtrlInfo->pvEventData ) )
    {
        HDMI_WARN("null pointer!\n");
        return ;
    }

    if (HDMI_TX_AON_INTR_STATE_aon_intr_stat0Get())
    {
        /* clr intr */
    	HDMI_TX_AON_INTR_STATE_aon_intr_stat0Set(HI_TRUE);
    	/* event */
    	if (CtrlHpdGet())
    	{
            pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_HOTPLUG);
    	}
    	else
    	{
            pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_HOTUNPLUG);
    	}
    }

    if (HDMI_TX_AON_INTR_STATE_aon_intr_stat1Get())
    {
        /* clr intr */
    	HDMI_TX_AON_INTR_STATE_aon_intr_stat1Set(HI_TRUE);
    	/* event */
    	if (CtrlRsenGet())
    	{
            pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_RSEN_CONNECT);
    	}
    	else
    	{
            pstCtrlInfo->pfnEventCallBack(pstCtrlInfo->pvEventData,HDMI_EVENT_RSEN_DISCONNECT);
    	}
    }

#ifdef HDMI_HDR_SUPPORT
     CtrlHdrTimerInvoke(pstCtrlInfo);
#endif

    return ;
}



#endif

/******************************* public interface  ***********************************/
HI_S32 HAL_HDMI_CtrlInit(HDMI_DEVICE_ID_E enHdmi,HDMI_HAL_INIT_S *pstHalInit)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MACH_CTRL_S    stMachCtrl = {0};
#endif

    CTRL_NULL_CHK(pstCtrlInfo);
    //CTRL_NULL_CHK(pstHalInit);

    if (!pstCtrlInfo->bInit)
    {
        s32Ret |= REG_HDMI_CRG_reg_Init();
        s32Ret |= HDMI_TX_S_tx_aon_reg_REGS_TYPE_Init();
        s32Ret |= HDMI_TX_S_audio_path_reg_REGS_TYPE_Init();
        s32Ret |= HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_Init();
        s32Ret |= HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_Init();
        s32Ret |= HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_Init();
        s32Ret |= HDMI_TX_S_video_path_reg_REGS_TYPE_Init();

        if (pstHalInit)
        {
            pstCtrlInfo->pfnEventCallBack   = pstHalInit->pfnEventCallBack;
            pstCtrlInfo->pvEventData        = pstHalInit->pvEventData;
        }
        pstCtrlInfo->bInit              = HI_TRUE;
#ifndef HDMI_BUILD_IN_BOOT
        stMachCtrl.pCbData      = pstCtrlInfo;
        stMachCtrl.pfnCallBack  = CtrlMachPpdCallBack;
        stMachCtrl.u64Interval  = 10;
        stMachCtrl.pu8Name      = "HPD";
        s32Ret |= HAL_HDMI_MachRegister(&stMachCtrl,&pstCtrlInfo->u32MachId);
        s32Ret |= HAL_HDMI_MachStart(pstCtrlInfo->u32MachId);
#endif
        CtrlHpdIntrEnalbe(HI_TRUE);

    }
    return s32Ret;


}

HI_S32 HAL_HDMI_CtrlDeinit(HDMI_DEVICE_ID_E enHdmi)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
#ifndef HDMI_BUILD_IN_BOOT
    CtrlHpdIntrEnalbe(HI_FALSE);
    s32Ret |= HAL_HDMI_MachStop(pstCtrlInfo->u32MachId);
    s32Ret |= HAL_HDMI_MachUnRegister(pstCtrlInfo->u32MachId);
#endif

    s32Ret |= HDMI_TX_S_tx_aon_reg_REGS_TYPE_DeInit();
    s32Ret |= HDMI_TX_S_audio_path_reg_REGS_TYPE_DeInit();
    s32Ret |= HDMI_TX_S_tx_ctrl_reg_REGS_TYPE_DeInit();
    s32Ret |= HDMI_TX_S_mcu_cpu_reg_REGS_TYPE_DeInit();
    s32Ret |= HDMI_TX_S_tx_hdmi_reg_REGS_TYPE_DeInit();
    s32Ret |= HDMI_TX_S_video_path_reg_REGS_TYPE_DeInit();
    //s32Ret |= REG_HDMI_CRG_reg_DeInit();
    pstCtrlInfo->bInit = HI_FALSE;

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlReset(HDMI_DEVICE_ID_E enHdmi)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CtrlHpdIntrEnalbe(HI_FALSE);
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_MachStop(pstCtrlInfo->u32MachId);
#endif
    return HI_SUCCESS;
}

//HI_S32 HAL_HDMI_CtrlStatusGet(HDMI_CTRL_INFO_S  *pstCtrlInfo,HDMI_CTRL_STATUS_S *pstCtrlStatus){ return HI_SUCCESS;}

HI_S32 HAL_HDMI_CtrlAvmuteSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    CtrlAvmuteSet(bEnable,5);

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_CtrlAvmuteGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pbEnable);

    return CtrlAvmuteGet(pbEnable);
}

HI_S32 HAL_HDMI_CtrlAudioMuteSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    CtrlAudioMuteSet(bEnable);

    return HI_SUCCESS;
}
HI_S32 HAL_HDMI_CtrlAudioMuteGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pbEnable);

    *pbEnable = CtrlAudioMuteGet();

    return HI_SUCCESS;

}
HI_S32 HAL_HDMI_CtrlVideoMuteSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CtrlVideoMuteSet(bEnable);
    return HI_SUCCESS;
}
HI_S32 HAL_HDMI_CtrlVideoMuteGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    return CtrlVideoMuteGet(pbEnable);
}

HI_S32 HAL_HDMI_CtrlAudioPathEnableSet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    CtrlAudioPathEnableSet(bEnable);

    return HI_SUCCESS;

}

HI_S32 HAL_HDMI_CtrlAudioPathEnableGet(HDMI_DEVICE_ID_E enHdmi,HI_BOOL *pbEnable)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pbEnable);

    *pbEnable = CtrlAudioPathEnableGet();

    return HI_SUCCESS;

}

HI_S32 HAL_HDMI_CtrlAudioPathSet(HDMI_DEVICE_ID_E enHdmi,HDMI_AUDIO_PATH_S *pstAudioPath)
{

    HI_U32                      u32CTSvalue = 0;
    HI_U32                      u32Nvalue = 0;
    HI_BOOL                     bMutiLayout = HI_FALSE;
    HI_BOOL                     enI2sEnable = HDMI_AUDIO_I2S_SD_NONE;
    HI_BOOL                     bSpdifEnable = HI_FALSE;
    HI_BOOL                     bHbraEnalbe = HI_FALSE;
    CTRL_I2S_STATUS_S           stI2sStatus = {0};
    CTRL_I2S_CTRL_S             stI2sCtrl = {0};
    CTRL_FIFO_CTRL_S            stFifoCtrl = {0};
    HDMI_CTRL_INFO_S            *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);
    HI_U32                      u32OrgRegRateCfg = 0;

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    HDMI_MEMCPY(&pstCtrlInfo->stAudioPath,pstAudioPath,sizeof(HDMI_AUDIO_PATH_S));

    CtrlAudioPathEnableSet(HI_FALSE);
    CtrlAudioReset();

    u32OrgRegRateCfg = pstAudioPath->enSampleRate;

    /* interface type */
    switch(pstAudioPath->enSoundIntf)
    {
        case HDMI_AUDIO_INTERFACE__SPDIF:
            enI2sEnable     = HDMI_AUDIO_I2S_SD_NONE;
            bSpdifEnable    = HI_TRUE;
            break;
        case HDMI_AUDIO_INTERFACE__HBRA:
            bHbraEnalbe         = HI_TRUE;
            u32OrgRegRateCfg    = HDMI_SAMPLE_RATE_768K;
            //no break;
        case HDMI_AUDIO_INTERFACE__I2S:
        default :
            enI2sEnable     = HDMI_AUDIO_I2S_SD_ALL;
            bSpdifEnable    = HI_FALSE;
            break;
    }

    /* i2s */
    if (enI2sEnable)
    {
    	/* I2S layout */
        if (bHbraEnalbe|| pstAudioPath->enChanneNum == HDMI_AUDIO_FORMAT_2CH)
        {
            bMutiLayout = HI_FALSE;
        }
        else if(pstAudioPath->enChanneNum < HDMI_AUDIO_FORMAT_BUTT)
        {
            bMutiLayout = HI_TRUE;
        }
        else
        {
            HDMI_WARN("error audio channel number:%u\n",pstAudioPath->enChanneNum);
            bMutiLayout = HI_FALSE;
        }
        CtrlAudioPathLayoutSet(bMutiLayout);

        /* i2s ctrl */
        stI2sCtrl.bChannelLRSwap      = HI_FALSE;
        stI2sCtrl.u32InBitLength      = CtrlAudioBitRegGet(pstAudioPath->enSampleBit) ;
        stI2sCtrl.bVbitCompress       = HI_FALSE;
        stI2sCtrl.bDataDirMsbFirst    = HI_FALSE;
        stI2sCtrl.bJustifyWsRight     = HI_FALSE;
        stI2sCtrl.bPolarityWsMsbLeft  = HI_FALSE;
        stI2sCtrl.bSckShiftOne        = HI_FALSE;
        stI2sCtrl.bHbraEnable         = bHbraEnalbe;
        CtrlAudioI2sCtrlSet(&stI2sCtrl);

        /* i2s status */
        stI2sStatus.enClkAccuracy     = HDMI_AUDIO_CLK_ACCURACY_LEVEL2;;
        stI2sStatus.bProfessionalApp  = HI_FALSE;
        stI2sStatus.bCompress         = HI_FALSE;
        stI2sStatus.u32SampRate       = CtrlI2SRateRegGet(u32OrgRegRateCfg);
        stI2sStatus.u32OrgRate        = CtrlI2SRateOrgRegGet(pstAudioPath->enSampleRate);
        stI2sStatus.u32OutBitLength   = CtrlAudioBitRegGet(pstAudioPath->enSampleBit);
        CtrlAudioI2sStatusSet(&stI2sStatus);
    }

    stFifoCtrl.bTest        = HI_FALSE;//!pstAudioPath->bEnableAudio;
    stFifoCtrl.enHbraMask   = bHbraEnalbe ? HDMI_AUDIO_HBRA_MASK_ALL : HDMI_AUDIO_HBRA_MASK_NONE;
    CtrlAudioFifoCtrlSet(&stFifoCtrl);

    /* CTS &  N value */
    CtrlAudioPathCtsValueSet(HI_FALSE,0);
    u32Nvalue   = HAL_HDMI_NValueGet(pstAudioPath->enSampleRate,pstAudioPath->u32PixelClk);
    CtrlAudioPathNvalueSet(u32Nvalue);

    u32CTSvalue = HAL_HDMI_CtsValueGet(pstAudioPath->enSampleRate,pstAudioPath->u32PixelClk);
    HDMI_INFO("\n fs=%u,bit=%u,intf=%u,tmds=%u\n"
              "reference N=%u,real N=%u,reference cts=%u,RealCts=%u\n",
                    pstAudioPath->enSampleRate,
                    pstAudioPath->enSampleBit,
                    pstAudioPath->enSoundIntf,
                    pstAudioPath->u32PixelClk,

                    u32Nvalue,
                    CtrlAudioPathNvalueGet(),
                    u32CTSvalue,
                    CtrlAudioPathCtsValueGet());

    /* enalbe */
    CtrlAudioI2sEnalbeSet(enI2sEnable);
    CtrlAudioSpdifEnalbeSet(bSpdifEnable);
    CtrlAudioHbraEnableSet(bHbraEnalbe);/* when spdif ,it must false */
    //CtrlAudioPathEnableSet(pstAudioPath->bEnableAudio);
    //CtrlAudioMuteSet(pstAudioPath->bAudioMute);

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_CtrlAudioPathGet(HDMI_DEVICE_ID_E enHdmi,CTRL_AUDIO_STATUS_S *pstAudioStat)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);
    HDMI_AUDIO_PATH_S   *pstAudioPath = HI_NULL;

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pstAudioStat);
    pstAudioPath = &pstCtrlInfo->stAudioPath;
    CTRL_NULL_CHK(pstAudioPath);

    if (CtrlAudioSpdifEnalbeGet())
    {
        pstAudioStat->enSoundIntf =  HDMI_AUDIO_INTERFACE__SPDIF;
    }
    else if (CtrlAudioHbraEnableGet())
    {
        pstAudioStat->enSoundIntf =  HDMI_AUDIO_INTERFACE__HBRA;
    }
    else if (CtrlAudioI2sEnalbeGet())
    {
        pstAudioStat->enSoundIntf =  HDMI_AUDIO_INTERFACE__I2S;
    }
    else
    {
        pstAudioStat->enSoundIntf =  HDMI_AUDIO_INTERFACE__BUTT;
    }

    pstAudioStat->bEnableAudio = CtrlAudioPathEnableGet();
    pstAudioStat->bAudioMute   = CtrlAudioMuteGet();

    switch(pstAudioStat->enSoundIntf)
    {
        case HDMI_AUDIO_INTERFACE__SPDIF :
            pstAudioStat->enSampleBit  = CtrlAudioBitValueGet(HI_TRUE);
            pstAudioStat->enSampleRate = CtrlSpdifRateValueGet();
            pstAudioStat->enChanneNum  = HDMI_AUDIO_FORMAT_BUTT;
            break;
        case HDMI_AUDIO_INTERFACE__HBRA :
        case HDMI_AUDIO_INTERFACE__I2S  :
        default:
            pstAudioStat->enSampleBit  = CtrlAudioBitValueGet(HI_FALSE);
            pstAudioStat->enSampleRate = CtrlI2SRateValueGet();
            pstAudioStat->enChanneNum  = CtrlAudioPathLayoutGet() ? 8 : 2;
            break;
    }

    pstAudioStat->u32RefCtsvalue = HAL_HDMI_CtsValueGet(pstAudioPath->enSampleRate,pstAudioPath->u32PixelClk);
    pstAudioStat->u32RegCtsvalue = CtrlAudioPathCtsValueGet();
    pstAudioStat->u32RefNvalue   = HAL_HDMI_NValueGet(pstAudioPath->enSampleRate,pstAudioPath->u32PixelClk);
    pstAudioStat->u32RegNvalue   = CtrlAudioPathNvalueGet();

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_CtrlVideoPathSet(HDMI_DEVICE_ID_E enHdmi,HDMI_VIDEO_PATH_S *pstVideoPath)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HI_BOOL             bEnableGcp = HI_FALSE;
    HI_BOOL             bEnalbeDither = HI_FALSE;
    HDMI_VEDIO_DITHER_E enDitherMode = HDMI_VEDIO_DITHER_DISALBE;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    HDMI_MEMCPY(&pstCtrlInfo->stVideoPath,pstVideoPath,sizeof(HDMI_VIDEO_PATH_S));

    /* dither */
    switch(pstVideoPath->enOutDeepColor)
    {
           case  HDMI_DEEP_COLOR_24BIT :

                bEnableGcp      = HI_TRUE;
                switch(pstVideoPath->enInDeepColor)
                {
                    case  HDMI_DEEP_COLOR_30BIT:
                        bEnalbeDither   = HI_TRUE;
                        enDitherMode    = HDMI_VEDIO_DITHER_10_8;
                        break;
                    case  HDMI_DEEP_COLOR_36BIT:
                        bEnalbeDither   = HI_TRUE;
                        enDitherMode    = HDMI_VEDIO_DITHER_12_8;
                        break;
                    case  HDMI_DEEP_COLOR_24BIT:
                    default:
                        bEnalbeDither = HI_FALSE;
                        enDitherMode    = HDMI_VEDIO_DITHER_DISALBE;
                        break;
                }


                break;

           case  HDMI_DEEP_COLOR_30BIT:

                bEnableGcp      = HI_TRUE;
                if(pstVideoPath->enInDeepColor==HDMI_DEEP_COLOR_36BIT)
                {
                    bEnalbeDither   = HI_TRUE;
                    enDitherMode    = HDMI_VEDIO_DITHER_12_10;
                }
                else
                {
                    bEnalbeDither   = HI_FALSE;
                    enDitherMode    = HDMI_VEDIO_DITHER_DISALBE;
                }
                break;

           case  HDMI_DEEP_COLOR_36BIT:
                bEnableGcp      = HI_TRUE;
                bEnalbeDither   = HI_FALSE;
                enDitherMode    = HDMI_VEDIO_DITHER_DISALBE;
                break;

           case  HDMI_DEEP_COLOR_48BIT:
                HDMI_WARN("ctrl  deepcolor un-support 48bit!force disable!\n");
           case  HDMI_DEEP_COLOR_OFF:
           default:
                pstVideoPath->enOutDeepColor   = HDMI_DEEP_COLOR_24BIT;
                bEnableGcp      = HI_FALSE;
                bEnalbeDither   = HI_FALSE;
                enDitherMode    = HDMI_VEDIO_DITHER_DISALBE;
                break;
    }

    CtrlVideoPathDitherSet(bEnalbeDither,enDitherMode);
    //HDMI_ERR("bEnableGcp=%d,deepcolor=%d\n",bEnableGcp,pstVideoPath->enOutDeepColor);
    CtrlVideoPathDeepClrSet(bEnableGcp,pstVideoPath->enOutDeepColor);
    s32Ret |= CtrlVideoPathPolaritySet(&pstVideoPath->stOutHvSyncPol);
    if ((pstVideoPath->enOutColormetry!=HDMI_COLORMETRY_BT2020_CONST)
        &&(pstVideoPath->enOutColormetry!=HDMI_COLORMETRY_BT2020))
    {
        CtrlVideoPathColormetrySet(pstVideoPath->enInColormetry,pstVideoPath->enOutColormetry);
    }
    CtrlVideoPathQuantizationSet(pstVideoPath->enInQuantization,pstVideoPath->enOutQuantization);
    CtrlVideoPathColorSpaceSet(pstVideoPath->enInColorSpace,pstVideoPath->enOutColorSpace);

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlVideoPathGet(HDMI_DEVICE_ID_E enHdmi,CTRL_VIDEO_STATUS_S *pstVideoStat)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HI_BOOL             bEnableGcp = HI_FALSE;
    HI_BOOL             bEnalbeDither = HI_FALSE;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    s32Ret |= CtrlTimmingDecectGet(&pstVideoStat->stTimming);
    s32Ret |= CtrlVideoPathPolarityGet(&pstVideoStat->stOutHvSyncPol);
    s32Ret |= CtrlVideoPathColorSpaceGet(&pstVideoStat->enInColorSpace, &pstVideoStat->enOutColorSpace);
    s32Ret |= CtrlVideoPathDitherGet(&bEnalbeDither,&pstVideoStat->enDitherMode);
    s32Ret |= CtrlVideoPathColormetryGet(&pstVideoStat->enInColormetry,&pstVideoStat->enOutColormetry);
    s32Ret |= CtrlVideoPathQuantizationGet(&pstVideoStat->enInQuantization,&pstVideoStat->enOutQuantization);
    s32Ret |= CtrlVideoPathDeepClrGet(&bEnableGcp,&pstVideoStat->enOutDeepColor);
    pstVideoStat->bCscEnable = CtrlVideoColorCscGet();

    if (!bEnableGcp)
    {
        pstVideoStat->enOutDeepColor = HDMI_DEEP_COLOR_OFF;
    }

    switch(pstVideoStat->enDitherMode)
    {
        case HDMI_VEDIO_DITHER_12_10	:
    	case HDMI_VEDIO_DITHER_12_8	    :
    	    pstVideoStat->enInDeepColor = HDMI_DEEP_COLOR_36BIT;
    	    break;
    	case HDMI_VEDIO_DITHER_10_8	    :
    	    pstVideoStat->enInDeepColor = HDMI_DEEP_COLOR_30BIT;
    	    break;

    	case HDMI_VEDIO_DITHER_DISALBE	:
    	default:
    	    pstVideoStat->enInDeepColor = pstVideoStat->enOutDeepColor;
    	    break;
	}

    pstVideoStat->bY422Enalbe = CtrlVideoColorYCbCr422Get();
    pstVideoStat->bY420Enalbe = CtrlVideoColorYCbCr420Get();

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlTmdsModeSet(HDMI_DEVICE_ID_E enHdmi,HDMI_TMDS_MODE_E enTmdsMode)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);
    s32Ret = CtrlTmdsModeSet(enTmdsMode);
    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlTmdsModeGet(HDMI_DEVICE_ID_E enHdmi,HDMI_TMDS_MODE_E *penTmdsMode)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(penTmdsMode);
    CTRL_INIT_CHK(pstCtrlInfo);
    s32Ret = CtrlTmdsModeGet(penTmdsMode);
    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlInfoFrameEnSet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL bEnable)
{
    HI_S32      s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    switch(enInfoFrameId)
    {
    	case HDMI_INFOFRAME_TYPE_VENDOR  :
    	    s32Ret = CtrlVendorInfoFrameEnSet(bEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_AVI     :
    	    s32Ret = CtrlAviInfoFrameEnSet(bEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_SPD     :
    	    s32Ret = CtrlSpdInfoFrameEnSet(bEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_AUDIO   :
    	    s32Ret = CtrlAudioInfoFrameEnSet(bEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_MPEG    :
    	    s32Ret = CtrlMpegInfoFrameEnSet(bEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_GBD     :
    	    s32Ret = CtrlGbdInfoFrameEnSet(bEnable);
            break;
        case HDMI_INFOFRAME_TYPE_DRM     :
            s32Ret = CtrlDrmInfoFrameEnSet(bEnable);
            break;

        default:
            HDMI_WARN("ctrl un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlInfoFrameEnGet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_BOOL *pbEnable)
{
    HI_S32      s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pbEnable);
    CTRL_INIT_CHK(pstCtrlInfo);

    switch(enInfoFrameId)
    {
    	case HDMI_INFOFRAME_TYPE_VENDOR  :
    	    s32Ret = CtrlVendorInfoFrameEnGet(pbEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_AVI     :
    	    s32Ret = CtrlAviInfoFrameEnGet(pbEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_SPD     :
    	    s32Ret = CtrlSpdInfoFrameEnGet(pbEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_AUDIO   :
    	    s32Ret = CtrlAudioInfoFrameEnGet(pbEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_MPEG    :
    	    s32Ret = CtrlMpegInfoFrameEnGet(pbEnable);
            break;
    	case HDMI_INFOFRAME_TYPE_GBD     :
    	    s32Ret = CtrlGbdInfoFrameEnGet(pbEnable);
            break;
        case HDMI_INFOFRAME_TYPE_DRM     :
            s32Ret = CtrlDrmInfoFrameEnGet(pbEnable);
            break;
        default:
            HDMI_WARN("ctrl un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}


HI_S32 HAL_HDMI_CtrlInfoFrameDataSet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8IfData)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pu8IfData);
    CTRL_NULL_CHK(pstCtrlInfo);

    switch(enInfoFrameId)
    {
    	case HDMI_INFOFRAME_TYPE_VENDOR  :
    	    s32Ret = CtrlVendorInfoframeDataSet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_AVI     :
    	    s32Ret = CtrlAviInfoframeDataSet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_SPD     :
    	    s32Ret = CtrlSpdInfoframeDataSet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_AUDIO   :
    	    s32Ret = CtrlAudioInfoframeDataSet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_MPEG    :
    	    s32Ret = CtrlMpegInfoframeDataSet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_GBD     :
    	    s32Ret = CtrlGbdInfoframeDataSet(pu8IfData);
            break;
        case HDMI_INFOFRAME_TYPE_DRM     :
            s32Ret = CtrlDrmInfoframeDataSet(pu8IfData);
            break;
        default:
            HDMI_WARN("ctrl un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_CtrlInfoFrameDataGet(HDMI_DEVICE_ID_E enHdmi, HDMI_INFOFRAME_ID_E enInfoFrameId, HI_U8 *pu8IfData)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pu8IfData);
    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_INIT_CHK(pstCtrlInfo);

    switch(enInfoFrameId)
    {
    	case HDMI_INFOFRAME_TYPE_VENDOR  :
    	    s32Ret = CtrlVendorInfoframeDataGet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_AVI     :
    	    s32Ret = CtrlAviInfoframeDataGet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_SPD     :
    	    s32Ret = CtrlSpdInfoframeDataGet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_AUDIO   :
    	    s32Ret = CtrlAudioInfoframeDataGet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_MPEG    :
    	    s32Ret = CtrlMpegInfoframeDataGet(pu8IfData);
            break;
    	case HDMI_INFOFRAME_TYPE_GBD     :
    	    s32Ret = CtrlGbdInfoframeDataGet(pu8IfData);
            break;
        case HDMI_INFOFRAME_TYPE_DRM     :
            s32Ret = CtrlDrmInfoframeDataGet(pu8IfData);
            break;
        default:
            HDMI_WARN("ctrl un-support infoframe type:%u!\n",enInfoFrameId);
            s32Ret = HI_FAILURE;
            break;
    }

    return s32Ret;
}

#ifdef HDMI_HDR_SUPPORT

HI_S32 HAL_HDMI_CtrlHdrTimerSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_TIMER_CONFIG_S *pstHdrTimerCfg)
{
    HI_U32              i = 0;
    CTRL_HDR_TIMER_S    *pstHdrTimer = HI_NULL;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstHdrTimerCfg);
    CTRL_INIT_CHK(pstCtrlInfo);

    if(pstHdrTimerCfg->enTimerType >= HDMI_TIMER_TYPE_BUTT)
    {
        HDMI_WARN("invalid timer type=%d,fail\n",pstHdrTimerCfg->enTimerType);
        return HI_FAILURE;
    }

    for (i = 0; i<HDMI_TIMER_TYPE_BUTT ;i++)
    {
        pstHdrTimer = &pstCtrlInfo->astHdrTimer[i];
        if ((pstHdrTimerCfg->bTimerStart) && (!pstHdrTimer->bEnable))
        {
            pstHdrTimer->bEnable        = HI_TRUE ;
            pstHdrTimer->enTimerType    = pstHdrTimerCfg->enTimerType;
            pstHdrTimer->u64StartTime   = HAL_HDMI_MachMsGet();
            pstHdrTimer->u64TimeLength  = pstHdrTimerCfg->u32Time;
            break;
        }
        else if(pstHdrTimerCfg->enTimerType == pstHdrTimer->enTimerType)
        {
            HDMI_MEMSET(pstHdrTimer,0,sizeof(CTRL_HDR_TIMER_S));
            break;
        }
        else
        {
            //nothing to do
        }
    }

    if(i >= HDMI_TIMER_TYPE_BUTT)
    {
        HDMI_WARN("can't find valid timer type=%d,fail\n",pstHdrTimerCfg->enTimerType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

#endif


HI_S32 HAL_HDMI_CtrlHpdRsenGet(HDMI_DEVICE_ID_E enHdmi,HDMI_HPD_RSEN_S *pstHpdRsen)
{
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pstHpdRsen);
    CTRL_INIT_CHK(pstCtrlInfo);

    pstHpdRsen->bRsenOn = CtrlRsenGet();
    pstHpdRsen->bHpdOn  = CtrlHpdGet();

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_CtrlVideoDbgSet(HDMI_DEVICE_ID_E  enHdmi,HDMI_DBG_VIDEO_S *pstDbgVideo)
{
    HI_U32              u32Polarlity = 0x0;
    HI_U32              u32Mask = 0x0;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pstDbgVideo);
    CTRL_INIT_CHK(pstCtrlInfo);

    //u32Polarlity = HDMI_TIMING_GEN_CTRL_reg_sync_polarityGet();
    u32Polarlity |= (pstDbgVideo->bVsyncPolarlity && 0x1) << 1;
    u32Polarlity |= pstDbgVideo->bHsyncPolarlity && 0x1;
    HDMI_TIMING_GEN_CTRL_reg_sync_polaritySet(u32Polarlity);
    HDMI_TIMING_GEN_CTRL_reg_timing_selSet(pstDbgVideo->enTimming);
    HDMI_TIMING_GEN_CTRL_reg_extmodeSet(pstDbgVideo->bSelfDefineTimming);
    HDMI_TIMING_GEN_CTRL_reg_timing_gen_enSet(pstDbgVideo->bTimmingEnable);

    u32Mask |= pstDbgVideo->bCbMask & 0x1;
    u32Mask <<= 1;
    u32Mask |= pstDbgVideo->bYMask & 0x1;
    u32Mask <<= 1;
    u32Mask |= pstDbgVideo->bCrMask & 0x1;

    //HDMI_PATTERN_GEN_CTRL_cbar_pattern_selSet(pstDbgVideo->bMixColor);
    HDMI_PATTERN_GEN_CTRL_cbar_pattern_selSet(pstDbgVideo->bYCbCrSpace);
    HDMI_PATTERN_GEN_CTRL_mask_pattern_enSet(u32Mask);
    HDMI_PATTERN_GEN_CTRL_square_pattern_enSet(pstDbgVideo->bSquarePattern);
    HDMI_PATTERN_GEN_CTRL_colorbar_enSet(pstDbgVideo->bColorBar);
    HDMI_PATTERN_GEN_CTRL_video_formatSet(pstDbgVideo->enPixFormat);
    HDMI_PATTERN_GEN_CTRL_solid_pattern_enSet(pstDbgVideo->bSolidPattern);
    HDMI_PATTERN_GEN_CTRL_tpg_enableSet(pstDbgVideo->bPatternEnable);

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_CtrlVideoDbgGet(HDMI_DEVICE_ID_E  enHdmi,HDMI_DBG_VIDEO_S *pstDbgVideo)
{

    HI_U32              u32Polarlity = 0x0;
    HI_U32              u32Mask = 0x0;
    HDMI_CTRL_INFO_S    *pstCtrlInfo = CtrlInfoPtrGet(enHdmi);

    CTRL_NULL_CHK(pstCtrlInfo);
    CTRL_NULL_CHK(pstDbgVideo);
    CTRL_INIT_CHK(pstCtrlInfo);

    u32Polarlity = HDMI_TIMING_GEN_CTRL_reg_sync_polarityGet();
    pstDbgVideo->bHsyncPolarlity = (u32Polarlity & 0x1) ? HI_TRUE : HI_FALSE;
    pstDbgVideo->bVsyncPolarlity = (u32Polarlity & 0x2) ? HI_TRUE : HI_FALSE;
    pstDbgVideo->enTimming = (HDMI_DBG_TIMMING_E)HDMI_TIMING_GEN_CTRL_reg_timing_selGet();
    pstDbgVideo->bSelfDefineTimming = HDMI_TIMING_GEN_CTRL_reg_extmodeGet() ? HI_TRUE : HI_FALSE ;
    pstDbgVideo->bTimmingEnable = HDMI_TIMING_GEN_CTRL_reg_timing_gen_enGet() ? HI_TRUE : HI_FALSE ;
    //pstDbgVideo->bMixColor = HDMI_PATTERN_GEN_CTRL_cbar_pattern_selGet()? HI_TRUE : HI_FALSE ;
    pstDbgVideo->bYCbCrSpace = HDMI_PATTERN_GEN_CTRL_colorbar_enGet() ? HI_TRUE : HI_FALSE ;
    u32Mask = HDMI_PATTERN_GEN_CTRL_mask_pattern_enGet();
    pstDbgVideo->bCbMask    = u32Mask & 0x1;
    pstDbgVideo->bYMask     = u32Mask & 0x2;
    pstDbgVideo->bCrMask    = u32Mask & 0x4;
    pstDbgVideo->bSquarePattern = HDMI_PATTERN_GEN_CTRL_square_pattern_enGet() ? HI_TRUE : HI_FALSE ;
    pstDbgVideo->bColorBar = HDMI_PATTERN_GEN_CTRL_colorbar_enGet() ? HI_TRUE : HI_FALSE ;
    pstDbgVideo->enPixFormat = (HDMI_DBG_PIX_FMT_E)HDMI_PATTERN_GEN_CTRL_video_formatGet();
    pstDbgVideo->bSolidPattern  = HDMI_PATTERN_GEN_CTRL_solid_pattern_enGet() ? HI_TRUE : HI_FALSE ;
    pstDbgVideo->bPatternEnable = HDMI_PATTERN_GEN_CTRL_tpg_enableGet() ? HI_TRUE : HI_FALSE ;

    return HI_SUCCESS;
}



