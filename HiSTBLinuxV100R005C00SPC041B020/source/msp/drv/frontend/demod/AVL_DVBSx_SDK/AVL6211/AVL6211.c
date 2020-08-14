/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : avl6211.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2012/02/25
  Description   :
  History       :
  1.Date        : 2011/11/25
    Author      : l00185424
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For AVL6211 DVB-S/S2 Demod.
 */

/*----------------------------- INCLUDE FILES ------------------------------------------*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/semaphore.h>

#include "IBSP.h"
#include "IBase.h"
#include "IRx.h"
#include "ITuner.h"
#include "II2C.h"
#include "IDiseqc.h"
#include "IBlindScan.h"
#include "avl_dvbsx.h"

#include "av2011.h"
#include "av2018.h"

#include "sharp_qm1d1c004x.h"
#include "isl9492.h"

#include "drv_tuner_ioctl.h"
#include "drv_demod.h"
#include "hi_drv_mem.h"

//#include "hi_gpio.h"
#include "hi_drv_gpio.h"
#include "hi_kernel_adapt.h"
/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/

/*The DiSEqC bus speed in the unit of kHz. Normally, it should be 22kHz. */
#define DISEQC_TONE_FREQ (22)
#define MAX_TUNER (3)

#define MIN_SYMBOLRATE_KHZ (2000)  /* The minimum symbol rate, 2M */
#define MAX_SYMBOLRATE_KHZ (45000) /* The minimum symbol rate, 45M */
#define MAX_SYMBOLRATE_HZ (45000000)

#define CHECK_INIT_SEM \
    if (!s_bSemInited) \
    { \
        HI_INIT_MUTEX(&(s_stSemaphore)); \
        s_bSemInited = HI_TRUE; \
    }

#define QUEUE_LOCK do \
    { \
        HI_S32 lockRet; \
        if (1) \
        { \
            lockRet = down_interruptible(&(s_stSemaphore)); \
        } \
    } while (0)
    
#define QUEUE_UNLOCK up(&(s_stSemaphore))


/*-------------------- STATIC STRUCTURE DECLARATIONS -----------------------------------*/

/* The reference clock */
enum AVL_Demod_ReferenceClock_Select_t
{
    AVL_ReferenceClock_4_MHz   = 4000,
    AVL_ReferenceClock_4_5_MHz = 4500,
    AVL_ReferenceClock_10_MHz  = 10000,
    AVL_ReferenceClock_16_MHz  = 16000,
    AVL_ReferenceClock_27_MHz  = 27000
};

/* Hardware configuration */
typedef struct
{
    HI_U8  u8ChannelId;     /*index*/
    HI_U8  u8DemodAddress;   /* I2C slave 0c/0d */
    //HI_U32 u32ResetGpioNo;
    enum AVL_Demod_ReferenceClock_Select_t enDemodReferenceClk; /*Crystal frequency,depending on the hardware board*/
    enum AVL_DVBSx_MpegMode enTSOutPutMode;
    enum AVL_DVBSx_MpegClockPolarity enTSClockPolarity;
    enum AVL_DVBSx_MpegFormat enTSFormat;
    enum AVL_DVBSx_MpegSerialPin enSerDataPin;
    /*The Tuner slave address.
     *It is the write address of the tuner device.
     *i.e. it is 8 bits address and the last bit is the R/W flag which is zero.
     *Although this data structure support 10 bits I2C address.
     *The AVL chip not supports it. */
    HI_U16 u16TunerAddress;                     /* Tuner address, depending on the tuner */

    HI_U16 u16TunerMaxLPF_100Khz;               /* LPF, depending on the tuner */
    HI_U16 u16TunerI2CClock_kHz;                /* Tuner I2C clock */
    enum AVL_DVBSx_RfagcPola enTunerRFAGC;      /* AGC, depending on the tuner*/
    enum AVL_DVBSx_SpectrumPolarity enTunerIQSpectrumMode; /*IQ, depending on the hardware board*/
    enum AVL_DVBSx_Diseqc_WaveFormMode enWaveMode;  /* If use DiSEqC controller like 8125, need configure to AVL_DVBSx_DWM_Envelope */
    AVL_DVBSx_ErrorCode (*m_pInitializeFunc)(struct AVL_Tuner *);
    AVL_DVBSx_ErrorCode (*m_pLockFunc)(struct AVL_Tuner *);
    AVL_DVBSx_ErrorCode (*m_pGetLockStatusFunc)(struct AVL_Tuner *);
} DEMOD_TUNER_CONFIG_S;

/* Parameters */
typedef struct AVL6211_PARA_S
{
    HI_U32  u32TunerPort;
    HI_BOOL bInited;                        /* Init flag */
    
    struct AVL_DVBSx_Chip stAVLChip;
    struct AVL_Tuner stTuner;               /* AVL tuner struct */
    DEMOD_TUNER_CONFIG_S stConfig;          /* Demod and tuner config, depending on the hardware */

    /* For blind scan */
    HI_UNF_TUNER_SAT_TPINFO_S astSat[MAX_TP_NUM];
    HI_U8 u8TotalTP;
    HI_U8 u8Reserve;
    HI_U16 u16Reserve;

    struct AVL6211_PARA_S* pstNext;
} AVL6211_DEMOD_PARA_S;

/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

/* Defined in AVL6211_patch.dat.c */
extern AVL_uchar ucPatchData[];

/*------------------------- STATIC DECLARATIONS ----------------------------------------*/

//static HI_BOOL s_bReseted = HI_FALSE;
static HI_BOOL s_bSemInited = HI_FALSE;
static struct semaphore s_stSemaphore;
static AVL6211_DEMOD_PARA_S* s_pstParaHead = HI_NULL;

static AVL6211_DEMOD_PARA_S* queue_insert(HI_U32 u32TunerPort);
static AVL6211_DEMOD_PARA_S* queue_get(HI_U32 u32TunerPort);
static HI_VOID               queue_remove(HI_U32 u32TunerPort);

/*------------------------------------ CODE --------------------------------------------*/

/* Malloc and insert a node */
static AVL6211_DEMOD_PARA_S* queue_insert(HI_U32 u32TunerPort)
{
    AVL6211_DEMOD_PARA_S* pstNew;
    AVL6211_DEMOD_PARA_S* pstTail = s_pstParaHead;

    CHECK_INIT_SEM;
    
    pstNew = HI_VMALLOC(HI_ID_TUNER, sizeof(AVL6211_DEMOD_PARA_S));
    if (HI_NULL == pstNew)
    {
        HI_INFO_TUNER("AVL6211 vmalloc fail.\n");
        return HI_NULL;
    }

    memset(pstNew, 0, sizeof(AVL6211_DEMOD_PARA_S));
    pstNew->u32TunerPort = u32TunerPort;
    pstNew->bInited = HI_FALSE;
    pstNew->pstNext = HI_NULL;

    QUEUE_LOCK;
    if (HI_NULL == pstTail)
    {
        s_pstParaHead = pstNew;
    }
    else
    {
        while (HI_NULL != pstTail->pstNext)
        {
            pstTail = pstTail->pstNext;
        }

        pstTail->pstNext = pstNew;
    }
    QUEUE_UNLOCK;

    return pstNew;
}

/* Find and return a node */
static AVL6211_DEMOD_PARA_S* queue_get(HI_U32 u32TunerPort)
{
    AVL6211_DEMOD_PARA_S* pstTemp = s_pstParaHead;

    while (HI_NULL != pstTemp)
    {
        if (u32TunerPort == pstTemp->u32TunerPort)
        {
            break;
        }

        pstTemp = pstTemp->pstNext;
    }

    return pstTemp;
}

/* Remove a node */
static HI_VOID queue_remove(HI_U32 u32TunerPort)
{
    AVL6211_DEMOD_PARA_S* pstTemp = s_pstParaHead;
    AVL6211_DEMOD_PARA_S* pstLast = s_pstParaHead;

    CHECK_INIT_SEM;

    QUEUE_LOCK;
    while (HI_NULL != pstTemp)
    {
        if (u32TunerPort == pstTemp->u32TunerPort)
        {
            if (pstTemp == s_pstParaHead)
            {
                s_pstParaHead = pstTemp->pstNext;
            }

            pstLast->pstNext = pstTemp->pstNext;
            HI_VFREE(HI_ID_TUNER, pstTemp);
            break;
        }

        pstLast = pstTemp;
        pstTemp = pstTemp->pstNext;
    }
    QUEUE_UNLOCK;
}

HI_S32 ConvertError(AVL_DVBSx_ErrorCode r)
{
    switch (r)
    {
    case AVL_DVBSx_EC_OK:
        return HI_SUCCESS;

    case AVL_DVBSx_EC_GeneralFail:
    case AVL_DVBSx_EC_I2CFail:
    case AVL_DVBSx_EC_TimeOut:
    case AVL_DVBSx_EC_Running:
    case AVL_DVBSx_EC_InSleepMode:
    case AVL_DVBSx_EC_MemoryRunout:
    case AVL_DVBSx_EC_BSP_ERROR1:
    case AVL_DVBSx_EC_BSP_ERROR2:
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

static AVL_DVBSx_ErrorCode CPUHalt(struct AVL_DVBSx_Chip * pAVLChip)
{
    AVL_DVBSx_ErrorCode r;
    AVL_uint16 i = 0;

    r = AVL_DVBSx_IBase_SendRxOP(OP_RX_HALT, pAVLChip );

    if (AVL_DVBSx_EC_OK == r)
    {
        while (i++ < 20)
        {
            r = AVL_DVBSx_IBase_GetRxOPStatus(pAVLChip);
            if (AVL_DVBSx_EC_OK == r)
            {
                break;
            }
            else
            {
                //AVL_DVBSx_IBSP_Delay(10);
                tuner_mdelay(10);
            }
        }
    }

    return ConvertError(r);
}

static AVL_DVBSx_ErrorCode SetLPF(HI_U32 u32TunerPort, struct AVL_Tuner * pTuner)
{
    AVL6211_DEMOD_PARA_S* pstParam;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL_uint16 FunctionalMode = AVL_DVBSx_FunctMode_Demod;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    r = AVL_DVBSx_II2C_Read16(pTuner->m_pAVLChip, rc_functional_mode_addr, &FunctionalMode);
    FunctionalMode &= 0x0001;

    if (FunctionalMode == AVL_DVBSx_FunctMode_Demod)
    {
        pTuner->m_uiLPF_100kHz = pTuner->m_uiSymbolRate_Hz * 75 / 10000000 + 40;
        if (pTuner->m_uiLPF_100kHz > pstParam->stConfig.u16TunerMaxLPF_100Khz)
        {
            pTuner->m_uiLPF_100kHz = pstParam->stConfig.u16TunerMaxLPF_100Khz;
        }
    }

    if (FunctionalMode == AVL_DVBSx_FunctMode_BlindScan)
    {
        pTuner->m_uiLPF_100kHz = pstParam->stConfig.u16TunerMaxLPF_100Khz;
    }

    return ConvertError(r);
}

HI_S32 avl6211_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    if (u32TunerPort >= MAX_TUNER)
    {
        return HI_FAILURE;
    }

    /* Make a node for device */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = queue_insert(u32TunerPort);
        if (HI_NULL == pstParam)
        {
            return HI_FAILURE;
        }
    }

    pstParam->stConfig.u8ChannelId = u32TunerPort;
    pstParam->stConfig.u8DemodAddress = (HI_U8)g_stTunerOps[u32TunerPort].u32DemodAddress;
    pstParam->stConfig.enTSOutPutMode = AVL_DVBSx_MPM_Parallel; /* Initial parallel */
    pstParam->stConfig.u16TunerAddress = g_stTunerOps[u32TunerPort].u32TunerAddress;  

    /* Init tuner fuctions */
    switch (enTunerDevType)
    {
#if defined(TUNER_DEV_TYPE_AV2011)    
    case HI_UNF_TUNER_DEV_TYPE_AV2011:
    default:
        pstParam->stConfig.m_pInitializeFunc = AVL_DVBSx_ExtAV2011_Initialize;
        pstParam->stConfig.m_pLockFunc = AVL_DVBSx_ExtAV2011_Lock;
        pstParam->stConfig.m_pGetLockStatusFunc = AVL_DVBSx_ExtAV2011_GetLockStatus;
        break;
#endif
#if defined(TUNER_DEV_TYPE_AV2018)    
    case HI_UNF_TUNER_DEV_TYPE_AV2018:
        pstParam->stConfig.m_pInitializeFunc = AVL_DVBSx_ExtAV2018_Initialize;
        pstParam->stConfig.m_pLockFunc = AVL_DVBSx_ExtAV2018_Lock;
        pstParam->stConfig.m_pGetLockStatusFunc = AVL_DVBSx_ExtAV2018_GetLockStatus;
        break;
#endif


#if defined(TUNER_DEV_TYPE_SHARP7903)
    case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
        pstParam->stConfig.m_pInitializeFunc = ExtSharpQM1D1C004x_Initialize;
        pstParam->stConfig.m_pLockFunc = ExtSharpQM1D1C004x_Lock;
        pstParam->stConfig.m_pGetLockStatusFunc = ExtSharpQM1D1C004x_GetLockStatus;
        break;
#endif
    }

    /* Configurate I2C Num */
    pstParam->stAVLChip.m_I2CChn = enI2cChannel;

    /* Setup tuner parameters for tuner initialization. */
    pstParam->stTuner.m_uiSlaveAddress = pstParam->stConfig.u16TunerAddress;
    pstParam->stTuner.m_pParameters = 0;
    pstParam->stTuner.m_uiSymbolRate_Hz = MAX_SYMBOLRATE_HZ;
    pstParam->stTuner.m_pAVLChip = &pstParam->stAVLChip;
    pstParam->stTuner.m_pInitializeFunc = pstParam->stConfig.m_pInitializeFunc;
    pstParam->stTuner.m_pLockFunc = pstParam->stConfig.m_pLockFunc;
    pstParam->stTuner.m_pGetLockStatusFunc = pstParam->stConfig.m_pGetLockStatusFunc;

    return HI_SUCCESS;
}

HI_S32 avl6211_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
    struct AVL_DVBSx_Diseqc_Para sDiseqcPara;
    struct AVL_DVBSx_MpegInfo sMpegMode;
    struct AVL_DVBSx_PllConf * pPLLConf = HI_NULL;
    AVL6211_DEMOD_PARA_S* pstParam;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    HI_UNF_TUNER_SAT_ATTR_S* pstAttr;

    HI_TUNER_CHECKPOINTER( pstSatTunerAttr);

    if (u32TunerPort >= MAX_TUNER)
    {
        return HI_FAILURE;
    }

    /* Make a node for device */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = queue_insert(u32TunerPort);
        if (HI_NULL == pstParam)
        {
            return HI_FAILURE;
        }
    }

    if (pstParam->stConfig.u8ChannelId != u32TunerPort)
    {
        return HI_FAILURE;
    }

    pstAttr = (HI_UNF_TUNER_SAT_ATTR_S*)pstSatTunerAttr;

    pstParam->stConfig.enDemodReferenceClk = pstAttr->u32DemodClk;
    switch(pstAttr->enTSClkPolar)
    {
        case HI_UNF_TUNER_TSCLK_POLAR_FALLING:
        default:
            pstParam->stConfig.enTSClockPolarity = AVL_DVBSx_MPCP_Falling;
            break;
        case HI_UNF_TUNER_TSCLK_POLAR_RISING:
            pstParam->stConfig.enTSClockPolarity = AVL_DVBSx_MPCP_Rising;
            break;
    }
    switch(pstAttr->enTSFormat)
    {
        case HI_UNF_TUNER_TS_FORMAT_TS:
        default:
            pstParam->stConfig.enTSFormat = AVL_DVBSx_MPF_TS;
            break;
        case HI_UNF_TUNER_TS_FORMAT_TSP:
            pstParam->stConfig.enTSFormat = AVL_DVBSx_MPF_TSP;
            break;
    }
    switch(pstAttr->enTSSerialPIN)
    {
        case HI_UNF_TUNER_TS_SERIAL_PIN_0:
        default:
            pstParam->stConfig.enSerDataPin = AVL_DVBSx_MPSP_DATA0;
            break;
        case HI_UNF_TUNER_TS_SERIAL_PIN_7:
            pstParam->stConfig.enSerDataPin = AVL_DVBSx_MPSP_DATA7;
            break;
    }
    pstParam->stConfig.u16TunerMaxLPF_100Khz = pstAttr->u16TunerMaxLPF * 10;
    pstParam->stConfig.u16TunerI2CClock_kHz = pstAttr->u16TunerI2CClk;
    switch (pstAttr->enRFAGC)
    {
    case HI_UNF_TUNER_RFAGC_INVERT:
    default:
        pstParam->stConfig.enTunerRFAGC = AVL_DVBSx_RA_Invert;
        break;
    case HI_UNF_TUNER_RFAGC_NORMAL:
        pstParam->stConfig.enTunerRFAGC = AVL_DVBSx_RA_Normal;
        break;
    }
    switch(pstAttr->enIQSpectrum)
    {
        case HI_UNF_TUNER_IQSPECTRUM_NORMAL:
        default:
            pstParam->stConfig.enTunerIQSpectrumMode = AVL_DVBSx_Spectrum_Normal;
            break;
        case HI_UNF_TUNER_IQSPECTRUM_INVERT:
            pstParam->stConfig.enTunerIQSpectrumMode = AVL_DVBSx_Spectrum_Invert;
            break;
    }
    switch(pstAttr->enDiSEqCWave)
    {
        case HI_UNF_TUNER_DISEQCWAVE_NORMAL:
        default:
            pstParam->stConfig.enWaveMode = AVL_DVBSx_DWM_Normal;
            break;
        case HI_UNF_TUNER_DISEQCWAVE_ENVELOPE:
            pstParam->stConfig.enWaveMode = AVL_DVBSx_DWM_Envelope;
            break;
    }
    
    /* This function should be called after bsp initialized to initialize the chip object. */
    r = Init_AVL_DVBSx_ChipObject(&pstParam->stAVLChip, pstParam->stConfig.u8DemodAddress);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Chip Object Initialization failed !\n");
        return ConvertError(r);
    }

    /* Clock */
    switch (pstParam->stConfig.enDemodReferenceClk)
    {
    case AVL_ReferenceClock_4_MHz:
        pPLLConf = (struct AVL_DVBSx_PllConf *)&(pll_conf[0]);
        break;

    case AVL_ReferenceClock_4_5_MHz:
        pPLLConf = (struct AVL_DVBSx_PllConf *)&(pll_conf[1]);
        break;

    case AVL_ReferenceClock_10_MHz:
        pPLLConf = (struct AVL_DVBSx_PllConf *)&(pll_conf[2]);
        break;

    case AVL_ReferenceClock_16_MHz:
        pPLLConf = (struct AVL_DVBSx_PllConf *)&(pll_conf[3]);
        break;

    case AVL_ReferenceClock_27_MHz:
        pPLLConf = (struct AVL_DVBSx_PllConf *)&(pll_conf[4]);
        break;
	default:
		HI_ERR_TUNER("reference clock error.\n");
		return HI_FAILURE;
    }

    AVL_DVBSx_IBSP_Delay(100);

    /* 
     * This function should be called after chip object initialized to initialize the IBase,
     * using reference clock as 10M. Make sure you pickup the right pll_conf since it may be modified in BSP.
     */
    r = AVL_DVBSx_IBase_Initialize(pPLLConf, ucPatchData, &pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("IBase Initialization failed! r = %x\n", r);
        return ConvertError(r);
    }

    /* Wait 100 ms to assure that the AVL_DVBSx chip boots up.This function should be implemented by customer.*/
    AVL_DVBSx_IBSP_Delay(100);

    /* This function should be called to verify the AVL_DVBSx chip has completed its initialization procedure. */
    r = AVL_DVBSx_IBase_GetStatus(&pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Booted failed! r=%x\n", r, r);
        return ConvertError(r);
    }

    /* This function should be called after IBase initialized to initialize the demod. */
    r = AVL_DVBSx_IRx_Initialize(&pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Demod Initialization failed !\n");
        return ConvertError(r);
    }

    /* modify rx_config.aagc_ref */
    r |= AVL_DVBSx_II2C_Write16(&pstParam->stAVLChip, rc_aagc_ref_addr, 48);
    r |= AVL_DVBSx_IRx_DriveRFAGC(&pstParam->stAVLChip);

    /* This function should be called after demod initialized to set RF AGC polar.*/
    /* 
     * User does not need to setup this for Sharp tuner since it is the default value. 
     * But for other tuners, user may need to do it here.
     */
    r |= AVL_DVBSx_IRx_SetRFAGCPola(pstParam->stConfig.enTunerRFAGC, &pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Set RF AGC Polar failed !\n");
        return ConvertError(r);
    }

    /* This function should be called after demod initialized to set spectrum polar.*/
    r = AVL_DVBSx_IBase_SetSpectrumPolarity(pstParam->stConfig.enTunerIQSpectrumMode, &pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Set Spectrum Polar failed !\n");
        return ConvertError(r);
    }

    /* Setup MPEG mode parameters.*/
    sMpegMode.m_MpegFormat = pstParam->stConfig.enTSFormat;
    sMpegMode.m_MpegMode = pstParam->stConfig.enTSOutPutMode;
    sMpegMode.m_MpegClockPolarity = pstParam->stConfig.enTSClockPolarity;

    /*
     * This function should be called after demod initialized to set MPEG mode.
     * (These parameters will be valid after call lock channel function)
     */
    r = AVL_DVBSx_IRx_SetMpegMode(&sMpegMode, &pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Set MPEG output mode failed !\n");
        return ConvertError(r);
    }

    /* If TS output in serial mode, need config serial PIN  */
    if (AVL_DVBSx_MPM_Serial == pstParam->stConfig.enTSOutPutMode)
    {
        r = AVL_DVBSx_IRx_SetMpegSerialPin(&pstParam->stAVLChip, pstParam->stConfig.enSerDataPin);
        if (AVL_DVBSx_EC_OK != r)
        {
            HI_INFO_TUNER("Set MPEG serial PIN mode failed !\n");
            return ConvertError(r);
        }
    }

    /* Enable the MPEG output (this function call has no effect for the AVL_DVBSxLG and AVL_DVBSxLGa devices)*/
    r = AVL_DVBSx_IRx_DriveMpegOutput(&pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Drive MPEG output failed !\n");
        return ConvertError(r);
    }

    /* Setup tuner parameters for tuner initialization. */
    pstParam->stTuner.m_uiSlaveAddress = pstParam->stConfig.u16TunerAddress;
    pstParam->stTuner.m_uiI2CBusClock_kHz = pstParam->stConfig.u16TunerI2CClock_kHz;
    pstParam->stTuner.m_uiLPF_100kHz = pstParam->stConfig.u16TunerMaxLPF_100Khz;
    pstParam->stTuner.m_pParameters = 0;
    pstParam->stTuner.m_uiSymbolRate_Hz = MAX_SYMBOLRATE_HZ;
    pstParam->stTuner.m_pAVLChip = &pstParam->stAVLChip;
    pstParam->stTuner.m_pInitializeFunc = pstParam->stConfig.m_pInitializeFunc;
    pstParam->stTuner.m_pLockFunc = pstParam->stConfig.m_pLockFunc;
    pstParam->stTuner.m_pGetLockStatusFunc = pstParam->stConfig.m_pGetLockStatusFunc;

    /* This function should be called after IBase initialized to initialize the tuner. */
    r = pstParam->stTuner.m_pInitializeFunc(&pstParam->stTuner);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Tuner Initialization failed !\n");
        return ConvertError(r);
    }

    /* Setup Diseqc parameters for Diseqc initialization.*/
    sDiseqcPara.m_RxTimeout  = AVL_DVBSx_DRT_150ms;
    sDiseqcPara.m_RxWaveForm = pstParam->stConfig.enWaveMode;
    sDiseqcPara.m_ToneFrequency_kHz = DISEQC_TONE_FREQ;
    sDiseqcPara.m_TXGap = AVL_DVBSx_DTXG_15ms;
    sDiseqcPara.m_TxWaveForm = pstParam->stConfig.enWaveMode;

    /*
     * The DiSEqC should be initialized if AVL_DVBSx need to supply power to LNB. 
     * This function should be called after IBase initialized to initialize the DiSEqC.
     */
    r = AVL_DVBSx_IDiseqc_Initialize(&sDiseqcPara, &pstParam->stAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("DiSEqC Initialization failed !\n");
        return ConvertError(r);
    }

    pstParam->bInited = HI_TRUE;
    HI_INFO_TUNER("AVL6211 Init %d success!\n", u32TunerPort);
    return HI_SUCCESS;
}

HI_S32 avl6211_deInit(HI_U32 u32TunerPort)
{
    if (u32TunerPort >= MAX_TUNER)
    {
        return HI_FAILURE;
    }

    queue_remove(u32TunerPort);
    return HI_SUCCESS;
}

HI_S32 avl6211_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel)
{
    struct AVL_DVBSx_Channel Channel;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    //HI_U16 u16LockStatus = 0;
    //HI_U16 u16Counter;
    //HI_U32 u32Timeth;
    struct AVL_DVBSx_Chip * pAVLChip = HI_NULL;
    struct AVL_Tuner * pTuner = HI_NULL;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( pstChannel);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &pstParam->stAVLChip;
    pTuner = &pstParam->stTuner;

    /* Halt CPU to improve the tuner locking speed. It's the optional operation before locking tuner. */
    r = CPUHalt(pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("CPU halt failed !\n");
        return ConvertError(r);
    }

    pTuner->m_uiFrequency_100kHz = pstChannel->u32Frequency / 100;

    /* Change the value defined by macro and go back here when we want to lock a new channel. */
    pTuner->m_uiSymbolRate_Hz = pstChannel->unSRBW.u32SymbolRate; /* symbol rate of the channel to be locked. */

    /* This function should be called before locking the tuner to adjust the tuner LPF based on channel symbol rate. */
    r = SetLPF(u32TunerPort, pTuner);

    /* This function should be called after initialization to lock the tuner. */
    r |= pTuner->m_pLockFunc(pTuner);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Tuner lock failed !r = %x\n", r);
        return ConvertError(r);
    }

    /* Wait a while for tuner to lock in certain frequency. */
    //AVL_DVBSx_IBSP_Delay(50);
    tuner_mdelay(50);

    /* This function should be called to check the lock status of the tuner. */
    r = pTuner->m_pGetLockStatusFunc(pTuner);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Tuner test failed !r = %x\n", r);
        return ConvertError(r);
    }

    Channel.m_uiFrequency_kHz = pTuner->m_uiFrequency_100kHz * 100;     
    Channel.m_uiSymbolRate_Hz = pstChannel->unSRBW.u32SymbolRate;
    /* Normal IQ */
    Channel.m_Flags  = (CI_FLAG_IQ_NO_SWAPPED) << CI_FLAG_IQ_BIT;
    /* Enable automatic IQ swap detection */
    Channel.m_Flags |= (CI_FLAG_IQ_AUTO_BIT_AUTO) << CI_FLAG_IQ_AUTO_BIT;
    /* Enable automatic standard detection */
    Channel.m_Flags |= (CI_FLAG_DVBS2_UNDEF) << CI_FLAG_DVBS2_BIT;

    /* This function should be called after tuner locked to lock the channel. */
    r = AVL_DVBSx_IRx_LockChannel(&Channel, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Lock channel failed !r = %d\n", r);
        return ConvertError(r);
    }

    HI_INFO_TUNER("Lock channel succeed !\n");

#if 0
    /* Channel lock time increase while symbol rate decrease.Give the max waiting time for different symbolrates. */
    if (pTuner->m_uiSymbolRate_Hz < 5000000)
    {
        /* The max waiting time is 5000ms,considering the IQ swapped status the time should be doubled. */
        //u32Timeth = 5000 * 2;
        return ConvertError(r);
    }
    else if (pTuner->m_uiSymbolRate_Hz < 10000000)
    {
        /* The max waiting time is 600ms,considering the IQ swapped status the time should be doubled. */
        u32Timeth = 600 * 2;
    }
    else
    {
        /* The max waiting time is 250ms,considering the IQ swapped status the time should be doubled. */
        u32Timeth = 250 * 2;
    }

    u16Counter = u32Timeth / 10;
    do
    {
        /* Wait 10ms for demod to lock the channel. */
        AVL_DVBSx_IBSP_Delay(10); 
        /* This function should be called to check the lock status of the demod. */
        r = AVL_DVBSx_IRx_GetLockStatus(&u16LockStatus, pAVLChip);
        if ((AVL_DVBSx_EC_OK == r) && (1 == u16LockStatus))
        {
            break;
        }
    } while (--u16Counter);

    if (0 == u16Counter)
    {
        HI_ERR_TUNER("Time out...\nChannel unlock !\n");
        return HI_FAILURE;
    }
#endif
    return ConvertError(r);
}

HI_S32 avl6211_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U16 u16LockStatus  = 0;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( penTunerStatus);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (pstParam->bInited)
    {
        /* If locked, u16LockStatus will be 1 */
        r = AVL_DVBSx_IRx_GetLockStatus(&u16LockStatus, &pstParam->stAVLChip);
        if (AVL_DVBSx_EC_OK != r)
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            return HI_FAILURE;
        }
        else
        {
            *penTunerStatus = (HI_UNF_TUNER_LOCK_STATUS_E)u16LockStatus;
            return HI_SUCCESS;
        }
    }
    else
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        return HI_SUCCESS;
    }
}

HI_S32 avl6211_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    HI_U32 u32BER = 0;
    HI_U32 u32Integer;
    HI_U32 u32Fraction;
    HI_U32 u32Exponent;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( pu32ber);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    /* estimated BER : 0 */
    if (AVL_DVBSx_EC_OK != AVL_DVBSx_IRx_GetBER(&u32BER, &pstParam->stAVLChip))
    {
        HI_INFO_TUNER("Tuner %d get ber fail.\n", u32TunerPort);
        return HI_FAILURE;
    }

    /* The return value is scaled by 1e-9, convert it to format XX.YYYE-Z */
    u32Integer  = u32BER;
    u32Fraction = 0;
    u32Exponent = 9;

    pu32ber[0] = u32Integer;
    pu32ber[1] = u32Fraction;
    pu32ber[2] = u32Exponent;

    return HI_SUCCESS;
}

HI_S32 avl6211_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( pu32SNR);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (AVL_DVBSx_EC_OK != AVL_DVBSx_IRx_GetSNR(pu32SNR, &pstParam->stAVLChip))
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 avl6211_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    HI_U16 u16Strength = 0;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( pu32SignalStrength);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (AVL_DVBSx_EC_OK != AVL_DVBSx_IRx_GetSignalLevel(&u16Strength, &pstParam->stAVLChip))
    {
        return HI_FAILURE;
    }

    pu32SignalStrength[1] = u16Strength;
    return HI_SUCCESS;
}

HI_S32 avl6211_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    HI_S16 s16FreqOffset;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_TUNER_CHECKPOINTER( pu32Freq);
    HI_TUNER_CHECKPOINTER( pu32Symb);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (AVL_DVBSx_EC_OK != AVL_DVBSx_IRx_GetRFOffset(&s16FreqOffset, &pstParam->stAVLChip))
    {
        return HI_FAILURE;
    }

    /* Convert 100kHz to kHz, s16 to u32 */
    *pu32Freq = (HI_U32)s16FreqOffset * 100;

    /* Don't support get symbol rate offset now. */
    *pu32Symb = 0;

    return HI_SUCCESS;
}

HI_S32 avl6211_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    struct AVL_DVBSx_SignalInfo SignalInfo;
    struct AVL_DVBSx_Chip * pAVLChip = HI_NULL;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;

    HI_UNF_TUNER_FE_FECTYPE_E aenFECType[] =
    {
        HI_UNF_TUNER_FE_DVBS,  HI_UNF_TUNER_FE_DVBS,  HI_UNF_TUNER_FE_DVBS,
        HI_UNF_TUNER_FE_DVBS,  HI_UNF_TUNER_FE_DVBS,  HI_UNF_TUNER_FE_DVBS,
        HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2,
        HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2,
        HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2,
        HI_UNF_TUNER_FE_DVBS2, HI_UNF_TUNER_FE_DVBS2
    };
    HI_UNF_TUNER_FE_FECRATE_E aenFECRate[] =
    {
        HI_UNF_TUNER_FE_FEC_1_2, HI_UNF_TUNER_FE_FEC_2_3, HI_UNF_TUNER_FE_FEC_3_4,
        HI_UNF_TUNER_FE_FEC_5_6, HI_UNF_TUNER_FE_FEC_6_7, HI_UNF_TUNER_FE_FEC_7_8,
        HI_UNF_TUNER_FE_FEC_1_4, HI_UNF_TUNER_FE_FEC_1_3, HI_UNF_TUNER_FE_FEC_2_5,
        HI_UNF_TUNER_FE_FEC_1_2, HI_UNF_TUNER_FE_FEC_3_5, HI_UNF_TUNER_FE_FEC_2_3,
        HI_UNF_TUNER_FE_FEC_3_4, HI_UNF_TUNER_FE_FEC_4_5, HI_UNF_TUNER_FE_FEC_5_6,
        HI_UNF_TUNER_FE_FEC_8_9, HI_UNF_TUNER_FE_FEC_9_10
    };

    HI_TUNER_CHECKPOINTER( pstInfo);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &pstParam->stAVLChip;

    r = AVL_DVBSx_IRx_GetSignalInfo(&SignalInfo, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Get SignalInfo Fail !\n");
        return HI_FAILURE;
    }

    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_SAT;
    switch (SignalInfo.m_modulation)
    {
    case AVL_DVBSx_MM_QPSK:
        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
        break;
    case AVL_DVBSx_MM_8PSK:
        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
        break;
    case AVL_DVBSx_MM_16APSK:
        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
        break;
    case AVL_DVBSx_MM_32APSK:
        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
        break;
    default:
        return HI_FAILURE;
    }

    /* Convert SignalInfo.m_coderate to UNF FEC Type */
    pstInfo->unSignalInfo.stSat.enSATType = aenFECType[SignalInfo.m_coderate];

    /* Convert SignalInfo.m_coderate to UNF FEC Rate */
    pstInfo->unSignalInfo.stSat.enFECRate = aenFECRate[SignalInfo.m_coderate];
    return HI_SUCCESS;
}

HI_S32 avl6211_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        HI_INFO_TUNER("Tuner %d can't be found in queue.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (pstParam->stConfig.u8ChannelId != u32TunerPort)
    {
        return HI_FAILURE;
    }

    switch (enTsType)
    {
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
        pstParam->stConfig.enTSOutPutMode = AVL_DVBSx_MPM_Serial;
        break;

    case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
    default:
        pstParam->stConfig.enTSOutPutMode = AVL_DVBSx_MPM_Parallel;
        break;
    }

    return HI_SUCCESS;
}

HI_S32 ChannelFilter(HI_UNF_TUNER_SAT_TPINFO_S *pstTPValidList, HI_U8 *pu8ValidNum,
                          struct AVL_DVBSx_Channel *pstTPTempList,
                          HI_U8 u8TempNum)
{
    struct AVL_DVBSx_Channel *pTemp;
    HI_UNF_TUNER_SAT_TPINFO_S *pValid;
    HI_U32 u32SymbolRateHz;
    HI_U32 u32SROffset;
    HI_BOOL bValid;
    HI_U8 i, j;
    HI_U8 u8Num;

    HI_TUNER_CHECKPOINTER( pstTPValidList);
    HI_TUNER_CHECKPOINTER( pu8ValidNum);

    u8Num = *pu8ValidNum;

    for (i = 0; i < u8TempNum; i++)
    {
        pTemp  = &pstTPTempList[i];
        bValid = HI_TRUE;
        for (j = 0; j < *pu8ValidNum; j++)
        {
            pValid = &pstTPValidList[j];
            if (abs(pValid->u32Freq - pTemp->m_uiFrequency_kHz)
                < min(pValid->u32SymbolRate, pTemp->m_uiSymbolRate_Hz) / 2000)
            {
                bValid = HI_FALSE;
                break;
            }
        }

        if (bValid)
        {
            pstTPValidList[u8Num].u32Freq = pTemp->m_uiFrequency_kHz;
            pstTPValidList[u8Num].u32SymbolRate = pTemp->m_uiSymbolRate_Hz;

            u32SymbolRateHz = pstTPValidList[u8Num].u32SymbolRate;

            /* Adjust symbol rate offset */
            u32SROffset = ((u32SymbolRateHz % 10000) > 5000) ? 
                                (10000 - (u32SymbolRateHz % 10000)) : (u32SymbolRateHz % 10000);
            if (((u32SymbolRateHz > 10000000) && (u32SROffset < 3500)) 
                || ((u32SymbolRateHz > 5000000) && (u32SROffset < 2000)))
            {
                u32SymbolRateHz = (u32SymbolRateHz % 10000 < 5000) ? 
                                (u32SymbolRateHz - u32SROffset) : (u32SymbolRateHz + u32SROffset);
            }

            u32SROffset = ((u32SymbolRateHz % 1000) > 500) ? 
                                (1000 - (u32SymbolRateHz % 1000)) : (u32SymbolRateHz % 1000);
            if ((u32SymbolRateHz < 5000000) && (u32SROffset < 500))
            {
                u32SymbolRateHz = (u32SymbolRateHz % 1000 < 500) ? 
                                (u32SymbolRateHz - u32SROffset) : (u32SymbolRateHz + u32SROffset);
            }

            pstTPValidList[u8Num].u32SymbolRate = 1000 * (u32SymbolRateHz / 1000);

            u8Num++;
        }
    }

    *pu8ValidNum = u8Num;
    return HI_SUCCESS;
}

HI_S32 avl6211_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara)
{
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;
    struct AVL_DVBSx_Chip *pAVLChip = HI_NULL;

    HI_TUNER_CHECKPOINTER( pstPara);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }
    pAVLChip = &(pstParam->stAVLChip);

    r |= AVL_DVBSx_IBase_SetSpectrumPolarity(pstParam->stConfig.enTunerIQSpectrumMode, pAVLChip);
    r |= AVL_DVBSx_IBlindScan_Reset(pAVLChip);
    
    pstParam->u8TotalTP = 0;
    memset(pstParam->astSat, 0, sizeof(pstParam->astSat));

    return HI_SUCCESS;
}

HI_S32 avl6211_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara)
{
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    struct AVL_DVBSx_Chip *pAVLChip = HI_NULL;
    struct AVL_Tuner *pTuner = HI_NULL;
    struct AVL_DVBSx_BlindScanPara stBSPara;
    struct AVL_DVBSx_BlindScanInfo stBSInfo;
    struct AVL_DVBSx_Channel stChannelListTemp[64];
    HI_U32 u32FreqCenter100KHZ;
    HI_U16 u16FindTPNum = 0;
    HI_U8 u8ChannelNumTemp = 0;
    HI_U8 u8PreTotalNum = 0;
    HI_U8 i = 0, j = 0;
    AVL6211_DEMOD_PARA_S* pstParam = HI_NULL;

    HI_TUNER_CHECKPOINTER( pstPara);

    /* Set 0 */
    r = copy_to_user(&(pstPara->u16Count), &u16FindTPNum, sizeof(pstPara->u16Count));
    if (HI_SUCCESS != r)
    {
        return HI_FAILURE;
    }


    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &(pstParam->stAVLChip);
    pTuner = &(pstParam->stTuner);
    u32FreqCenter100KHZ = pstPara->u32CentreFreq * 10;

    pTuner->m_uiFrequency_100kHz = u32FreqCenter100KHZ;
    stBSPara.m_uiMaxSymRate_kHz = MAX_SYMBOLRATE_KHZ;
    stBSPara.m_uiMinSymRate_kHz = MIN_SYMBOLRATE_KHZ;
    stBSPara.m_uiStartFreq_100kHz = u32FreqCenter100KHZ - 320;
    stBSPara.m_uiStopFreq_100kHz  = u32FreqCenter100KHZ + 320;

    r |= SetLPF(u32TunerPort, pTuner);
    r |= pTuner->m_pLockFunc(pTuner);   /* Lock the tuner. */

    //AVL_DVBSx_IBSP_Delay(100);      /* wait a while for tuner to lock in certain frequency. */
    tuner_mdelay(100);

    r |= pTuner->m_pGetLockStatusFunc(pTuner);    /* Check the lock status of the tuner. */
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Tuner can not lock to %d MHz !!!\n", pTuner->m_uiFrequency_100kHz / 10);
        return ConvertError(r);
    }
    else
    {
        HI_INFO_TUNER("<0>Lock to %4d MHz ", pTuner->m_uiFrequency_100kHz / 10);
    }

    /* This function should be called after tuner lock to perform a blind scan. */
    u16FindTPNum = 0;
    r |= AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_BlindScan);
    r |= AVL_DVBSx_IBlindScan_Scan(&stBSPara, pstParam->stConfig.u16TunerMaxLPF_100Khz, pAVLChip);
    do
    {
        HI_INFO_TUNER("$");
        /* wait 100 milionsecond */
        //AVL_DVBSx_IBSP_Delay(100);
        tuner_mdelay(100);
        /* Query the internal blind scan procedure information. */
        r = AVL_DVBSx_IBlindScan_GetScanStatus(&stBSInfo, pAVLChip);
        if (AVL_DVBSx_EC_OK != r)
        {
            HI_INFO_TUNER("Get scan status failed !\n");
            break;
        }
    } while (100 != stBSInfo.m_uiProgress);   /* 100 indicates the blind scan is finished. */

    if (stBSInfo.m_uiChannelCount > 0)
    {
        r = AVL_DVBSx_IBlindScan_ReadChannelInfo(0, &(stBSInfo.m_uiChannelCount), stChannelListTemp, pAVLChip);
        if (AVL_DVBSx_EC_OK != r)
        {
            HI_INFO_TUNER("Read channel info failed !\n");
            return ConvertError(r); 
        }
    }

    u8ChannelNumTemp = (AVL_uchar)stBSInfo.m_uiChannelCount;
    u8PreTotalNum = pstParam->u8TotalTP;
    ChannelFilter(pstParam->astSat, &pstParam->u8TotalTP, stChannelListTemp, u8ChannelNumTemp);
    i = 0;
    if (pstParam->u8TotalTP > u8PreTotalNum)
    {
        for (j = u8PreTotalNum; j < pstParam->u8TotalTP; j++)
        {
            r = copy_to_user(&(pstPara->unResult.astSat[i].u32Freq), &pstParam->astSat[j].u32Freq,
                             sizeof(pstParam->astSat[j].u32Freq));
            r |= copy_to_user(&(pstPara->unResult.astSat[i].u32SymbolRate), &pstParam->astSat[j].u32SymbolRate,
                             sizeof(pstParam->astSat[j].u32SymbolRate));
            if (AVL_DVBSx_EC_OK != r)
            {
                return HI_FAILURE;
            }
            i++;
        }
    }

    u16FindTPNum = i;

    r = copy_to_user(&(pstPara->u16Count), &u16FindTPNum, sizeof(pstPara->u16Count));
    if (AVL_DVBSx_EC_OK != r)
    {
        return HI_FAILURE;
    }

    /*r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_Demod);*/
    return ConvertError(r);
}

HI_S32 avl6211_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode)
{
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    struct AVL_DVBSx_Chip *pAVLChip = HI_NULL;
    AVL6211_DEMOD_PARA_S* pstParam = HI_NULL;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        HI_INFO_TUNER("Tuner %d can't be found in queue.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &(pstParam->stAVLChip);

    switch (enFuncMode)
    {
    case FunctMode_BlindScan:
        r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_BlindScan);
        break;

    case FunctMode_Demod:
        r = AVL_DVBSx_IBase_SetFunctionalMode(pAVLChip, AVL_DVBSx_FunctMode_Demod);
        break;

    default:
        return HI_FAILURE;
    }

    return ConvertError(r);
}

/* 0 Power off, 1 power on */
HI_S32 avl6211_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    return ConvertError(AVL_DVBSx_IDiseqc_SetLNB1Out(u8Power, &pstParam->stAVLChip));
}

HI_S32 avl6211_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    switch (enOut)
    {
    case TUNER_LNB_OUT_0V:
    case TUNER_LNB_OUT_BUTT:
        return HI_FAILURE;
        
    case TUNER_LNB_OUT_13V:
    case TUNER_LNB_OUT_14V:
        return ConvertError(AVL_DVBSx_IDiseqc_SetLNBOut(1, &pstParam->stAVLChip));
    
    case TUNER_LNB_OUT_18V:
    case TUNER_LNB_OUT_19V:
        return ConvertError(AVL_DVBSx_IDiseqc_SetLNBOut(0, &pstParam->stAVLChip));
    }

    return HI_SUCCESS;
}

HI_S32 avl6211_send_continuous_22K(HI_U32 u32TunerPort, HI_U32 u32Continuous22K)
{
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (0 == u32Continuous22K)
    {
        r = AVL_DVBSx_IDiseqc_StopContinuous(&pstParam->stAVLChip);
    }
    else
    {
        r = AVL_DVBSx_IDiseqc_StartContinuous(&pstParam->stAVLChip);
    }

    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("control 22K failed !r = %x\n", r);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 avl6211_send_tone(HI_U32 u32TunerPort, HI_U32 u32Tone)
{
    struct AVL_DVBSx_Diseqc_TxStatus sTxStatus;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;

    struct AVL_DVBSx_Chip * pAVLChip = HI_NULL;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &pstParam->stAVLChip;

    /* This function can be called after initialization to send out 8 tone_1s. */
    r = AVL_DVBSx_IDiseqc_SendTone(u32Tone, 1, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("Output tone %d Fail!\n", u32Tone);
    }
    else
    {
        do
        {
            /* Get current status of the DiSEqC transmitter data FIFO. */
            r = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, pAVLChip);
        } while (1 != sTxStatus.m_TxDone);          /* Wait until operation finished. */

        if (AVL_DVBSx_EC_OK != r)
        {
            HI_INFO_TUNER("Output tone %d Fail!\n", u32Tone);
        }
    }

    return ConvertError(r);
}

HI_S32 avl6211_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg)
{
    struct AVL_DVBSx_Diseqc_TxStatus sTxStatus;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL6211_DEMOD_PARA_S* pstParam;
    struct AVL_DVBSx_Chip * pAVLChip = HI_NULL;

    HI_TUNER_CHECKPOINTER( pstSendMsg);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &pstParam->stAVLChip;

    /* Send command */
    r = AVL_DVBSx_IDiseqc_SendModulationData(pstSendMsg->au8Msg, pstSendMsg->u8Length, pAVLChip);
    if (AVL_DVBSx_EC_OK != r)
    {
        HI_INFO_TUNER("AVL_DVBSx_IDiseqc_SendModulationData Fail!\n");
    }
    else
    {
        do
        {
            /* Get current status of the DiSEqC transmitter data FIFO. */
            r = AVL_DVBSx_IDiseqc_GetTxStatus(&sTxStatus, pAVLChip);
        } while (1 != sTxStatus.m_TxDone); /* Wait until operation finished. */
        if (AVL_DVBSx_EC_OK == r)
        {
            HI_INFO_TUNER("Output modulation bytes: 0x%x, 0x%x, 0x%x, 0x%x, 0x%x, 0x%x Success !\n",
                pstSendMsg->au8Msg[0], pstSendMsg->au8Msg[1], pstSendMsg->au8Msg[2], 
                pstSendMsg->au8Msg[3], pstSendMsg->au8Msg[4], pstSendMsg->au8Msg[5]);
        }
        else
        {
            HI_INFO_TUNER("Output modulation data Fail!\n");
        }
    }

    return ConvertError(r);
}

HI_S32 avl6211_DiSEqC_recv_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_RECVMSG_S *pstRecvMsg)
{
    struct AVL_DVBSx_Diseqc_RxStatus sRxStatus;
    HI_UNF_TUNER_DISEQC_RECVMSG_S stRecvMsg;
    AVL_DVBSx_ErrorCode r = AVL_DVBSx_EC_OK;
    AVL_uchar i;
    AVL6211_DEMOD_PARA_S* pstParam;
    struct AVL_DVBSx_Chip * pAVLChip = HI_NULL;

    HI_TUNER_CHECKPOINTER( pstRecvMsg);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    pAVLChip = &pstParam->stAVLChip;

    /* Need recv msessage */
    if (HI_NULL != pstRecvMsg)
    {
        memset(&stRecvMsg, 0, sizeof(stRecvMsg));

        i = 0;

        /* Before receiving modulation data, we should send some request data first. */
        /* Read input status. */
        do
        {
            r = AVL_DVBSx_IDiseqc_GetRxStatus(&sRxStatus, pAVLChip);    /* Get current status of the DiSEqC receiver. */
            udelay(1000);
            i++;
        } while ((1 != sRxStatus.m_RxDone) && (i < 150));       /* Wait until operation finished. */

        if (AVL_DVBSx_EC_OK != r)
        {
            stRecvMsg.enStatus = HI_UNF_TUNER_DISEQC_RECV_ERROR;
            HI_INFO_TUNER("Read modulation bytes Fail!\n");
        }
        else
        {
            if (0 != sRxStatus.m_RxFifoCount)      /* Data received. */
            {
                i = sRxStatus.m_RxFifoCount;

                /* This function can be called to read data back from the DiSEqC input FIFO when there are data received. */
                r = AVL_DVBSx_IDiseqc_ReadModulationData(stRecvMsg.au8Msg, &stRecvMsg.u8Length, pAVLChip);
                if (AVL_DVBSx_EC_OK == r)
                {
                    stRecvMsg.enStatus = HI_UNF_TUNER_DISEQC_RECV_OK;
                }
                else
                {
                    stRecvMsg.enStatus = HI_UNF_TUNER_DISEQC_RECV_ERROR;
                    HI_INFO_TUNER("Read modulation data Fail!\n");
                }
            }
            else
            {
                stRecvMsg.enStatus = HI_UNF_TUNER_DISEQC_RECV_TIMEOUT;
                HI_INFO_TUNER("There is no data in input FIFO.\n");
            }
        }

        if (0 != copy_to_user(pstRecvMsg, &stRecvMsg, sizeof(stRecvMsg)))
        {
            return HI_FAILURE;
        }
    }

    return ConvertError(r);
}

HI_S32 avl6211_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_ERR_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (0 == u32Standby)
    {
        return ConvertError(AVL_DVBSx_IBase_Wake(&pstParam->stAVLChip));
    }
    else
    {
        return ConvertError(AVL_DVBSx_IBase_Sleep(&pstParam->stAVLChip));
    }
}

HI_S32 avl6211_disable(HI_U32 u32TunerPort, HI_U32 u32Disable)
{
    AVL6211_DEMOD_PARA_S* pstParam;

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_ERR_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

    if (0 == u32Disable)
    {
        return ConvertError(AVL_DVBSx_IRx_DriveMpegOutput(&pstParam->stAVLChip));
    }
    else
    {
        return ConvertError(AVL_DVBSx_IRx_ReleaseMpegOutput(&pstParam->stAVLChip));
    }
}

