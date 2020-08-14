/******************************************************************************

  Copyright (C), 2016, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : avl6381.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2016/09/21
  Description   :
  History       :
  1.Date        : 2016/09/21
    Author      : 
    Modification: Created file

  ATTENTION		: The Code Version of AVL6381 only support DTMB Mode !!!
 *******************************************************************************/
 
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/semaphore.h>

#include "drv_tuner_ioctl.h"
#include "drv_demod.h"
#include "hi_drv_mem.h"

#include "AVL6381.h"
#include "mxl608.h"

#define MAX_TUNER (3)

#define AVL6381_CHECK_INIT_SEM \
    if (!s_bSemInited) \
    { \
        HI_INIT_MUTEX(&(s_stSemaphore)); \
        s_bSemInited = HI_TRUE; \
    }

#define AVL6381_QUEUE_LOCK do \
    { \
        HI_S32 lockRet; \
        if (1) \
        { \
            lockRet = down_interruptible(&(s_stSemaphore)); \
        } \
    } while (0)

#define AVL6381_QUEUE_UNLOCK up(&(s_stSemaphore))

/* Hardware configuration */
typedef struct
{
    HI_U8  u8ChannelId;      /*index*/
    HI_U8  u8DemodAddress;   /* I2C slave 0c/0d */
    HI_U32 u32ResetGpioNo;
    HI_U32 enDemodReferenceClk; /*Crystal frequency,depending on the hardware board*/
    enum AVL6381_MPEG_Mode enTSOutPutMode;
    enum AVL6381_MpegClockPolarity enTSClockPolarity;
    enum AVL6381_MpegFormat enTSFormat;
    enum AVL6381_MpegSerialPin enSerDataPin;

    HI_U16 u16TunerAddress;                     /* Tuner address, depending on the tuner */
    HI_U16 u16TunerMaxLPF_100Khz;               /* LPF, depending on the tuner */
    HI_U16 u16TunerI2CClock_kHz;                /* Tuner I2C clock */
    enum AVL6381_RfagcPola enTunerRFAGC;        /* AGC, depending on the tuner*/
    enum AVL6381_SpectrumPolarity enTunerIQSpectrumMode; /*IQ, depending on the hardware board*/
} AVL6381_DEMOD_TUNER_CONFIG_S;


/* Parameters */
typedef struct AVL6381_PARA_S
{
    HI_U32  u32TunerPort;
    HI_BOOL bInited;                        /* Init flag */
    
    struct AVL6381_Chip stAVLChip;
    struct AVL6381_Tuner stTuner;           /* AVL tuner struct */
    AVL6381_DEMOD_TUNER_CONFIG_S stConfig;  /* Demod and tuner config, depending on the hardware */

    struct AVL6381_PARA_S* pstNext;
} AVL6381_DEMOD_PARA_S;

static AVL6381_DEMOD_PARA_S* s_pstParaHead = HI_NULL; 	//used for pstParam queue operation
static struct semaphore s_stSemaphore; 					//used for pst queue operation
static struct semaphore AVL6381_Sem;   					//used for AVL6381 operation
static HI_BOOL s_bSemInited = HI_FALSE;					//used for define if the chip is inited

AVL6381_uchar gAVL6381freezeflag = 0;
AVL6381_CFG   gsAVL6381_Config;

/*-------------------------------find a node --------------------------------*/
/* Malloc and insert a node */
static AVL6381_DEMOD_PARA_S* AVL6381_queue_insert(HI_U32 u32TunerPort)
{
    AVL6381_DEMOD_PARA_S* pstNew;
    AVL6381_DEMOD_PARA_S* pstTail = s_pstParaHead;

   AVL6381_CHECK_INIT_SEM;

    pstNew = HI_VMALLOC(HI_ID_TUNER, sizeof(AVL6381_DEMOD_PARA_S));
    if (HI_NULL == pstNew)
    {
        HI_INFO_TUNER("AVL6211 vmalloc fail.\n");
        return HI_NULL;
    }

    memset(pstNew, 0, sizeof(AVL6381_DEMOD_PARA_S));
    pstNew->u32TunerPort = u32TunerPort;
    pstNew->bInited = HI_FALSE;
    pstNew->pstNext = HI_NULL;

    AVL6381_QUEUE_LOCK;
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
    AVL6381_QUEUE_UNLOCK;

    return pstNew;
}

/* Find and return a node */
static AVL6381_DEMOD_PARA_S* AVL6381_queue_get(HI_U32 u32TunerPort)
{
    AVL6381_DEMOD_PARA_S* pstTemp = s_pstParaHead;

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
static HI_VOID AVL6381_queue_remove(HI_U32 u32TunerPort)
{
    AVL6381_DEMOD_PARA_S* pstTemp = s_pstParaHead;
    AVL6381_DEMOD_PARA_S* pstLast = s_pstParaHead;

    AVL6381_CHECK_INIT_SEM;

    AVL6381_QUEUE_LOCK;
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
    AVL6381_QUEUE_UNLOCK;
}

/*---------------------------------avl6381_init----------------------------------*/	
/*---------------------------------avl6381_init----------------------------------*/	
/*---------------------------------avl6381_init----------------------------------*/	
HI_S32 avl6381_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
	HI_S32 s32Ret;
	AVL6381_DEMOD_PARA_S* 	pstParam;
	AVL6381_uint32   		avl6381_chipid = 0;
	AVL6381_uchar 			ucPLLIndex = 0;
	AVL6381_uint32 			uiDelayUnit = 10; /* Time-out window 10*20 ms */
	AVL6381_uint32 			uiCounter = 0;
	AVL6381_uint32 			uiTimeoutCount = 20;
	AVL6381_VerInfo    		pVerInfo_T;
	
	if (u32TunerPort >= MAX_TUNER)
    {
        return HI_FAILURE;
    }
    /* Make a node for device */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = AVL6381_queue_insert(u32TunerPort);
        if (HI_NULL == pstParam)
		{
            return HI_FAILURE;
        }
    }

	HI_INFO_TUNER("::enSigType=%d enI2cChannel=%d enTunerDevType=%d enDemodDevType=%d stCurrPara=%d enTsType=%d\n",
					g_stTunerOps[u32TunerPort].enSigType,
					g_stTunerOps[u32TunerPort].enI2cChannel,
					g_stTunerOps[u32TunerPort].enTunerDevType,
					g_stTunerOps[u32TunerPort].enDemodDevType,
					g_stTunerOps[u32TunerPort].stCurrPara,
					g_stTunerOps[u32TunerPort].enTsType);
	HI_INFO_TUNER("::u32DemodAddress=0x%x u32XtalClk=%d u32TunerAddress=0x%x u32CurrQamMode=%d u8AdcType=%d u8AgcOutputSel=%d u8AdcDataFmt=%d\n",
					g_stTunerOps[u32TunerPort].u32DemodAddress,
					g_stTunerOps[u32TunerPort].u32XtalClk,
					g_stTunerOps[u32TunerPort].u32TunerAddress,
					g_stTunerOps[u32TunerPort].u32CurrQamMode,
					g_stTunerOps[u32TunerPort].u8AdcType,
					g_stTunerOps[u32TunerPort].u8AgcOutputSel,
					g_stTunerOps[u32TunerPort].u8AdcDataFmt);
	
	pstParam->stConfig.u8ChannelId = u32TunerPort;
	pstParam->stConfig.u8DemodAddress = (HI_U8)g_stTunerOps[u32TunerPort].u32DemodAddress;
	pstParam->stConfig.enTSOutPutMode = AVL6381_MPEG_PARALLEL; /* Initial parallel */
	pstParam->stConfig.u16TunerAddress = g_stTunerOps[u32TunerPort].u32TunerAddress;

	/* Configurate I2C Num */
	pstParam->stAVLChip.m_I2CChn = enI2cChannel;
	pstParam->stAVLChip.m_SlaveAddr = pstParam->stConfig.u8DemodAddress;

	/* Setup tuner parameters for tuner initialization. */
	pstParam->stTuner.m_uiSlaveAddress = pstParam->stConfig.u16TunerAddress;
	pstParam->stTuner.m_pParameters = NULL;
	pstParam->stTuner.m_uiSymbolRate_Hz = 45000000;  //MAX_SYMBOLRATE_HZ
	pstParam->stTuner.m_pAVLChip = &pstParam->stAVLChip;

	
	HI_INFO_TUNER("func=%s\n",__func__);
	HI_INFO_TUNER("tunerport=%d iic_channel=%d demod_addr=0x%x tuner_addr=0x%x\n",
					pstParam->stConfig.u8ChannelId,\
					pstParam->stAVLChip.m_I2CChn,\
					pstParam->stAVLChip.m_SlaveAddr,\
					pstParam->stConfig.u16TunerAddress);

	//init Mutex  -->ok
	AVL6381_Mutex_init(&AVL6381_Sem);
	s32Ret = AVL6381_II2C_Initialize();
	
	//read chip ID -->ok
	AVL6381_GetChipID(&(pstParam->stAVLChip), &avl6381_chipid);
	HI_INFO_TUNER("chip ID=0x%x\n", avl6381_chipid);
	if(avl6381_chipid == 0x6){
		HI_INFO_TUNER("Availink AVL6381 Demod!\n");
	}else if(avl6381_chipid == 0xe)	{
		gAVL6381freezeflag = 1;
		HI_INFO_TUNER("Availink AVL6381 freeze Demod!\n");
	}else
		HI_INFO_TUNER("NO Availink AVL6381 Demod!\n");

	
	/*----------------initialize gsAVL6381_Config struct--------------------*/
	if(HI_UNF_TUNER_SIG_TYPE_DTMB == g_stTunerOps[u32TunerPort].enSigType)
		gsAVL6381_Config.ucStartupMode = AVL6381_DEMOD_MODE_DTMB;
	else{
		//the signal format is no correct, exit!!!
		//the driver code only support DTMB Mode Now!!! 20161009
		AVL6381_queue_remove(u32TunerPort);
		return HI_FAILURE;		
	}
	
	gsAVL6381_Config.m_SlaveAddr			= pstParam->stConfig.u8DemodAddress;
	gsAVL6381_Config.m_current_demod_mode	= gsAVL6381_Config.ucStartupMode;
	gsAVL6381_Config.m_pInitialData			= NULL;
	gsAVL6381_Config.m_DemodFrequency_Hz	= 0;	//init 
	gsAVL6381_Config.m_SDRAMFrequency_Hz	= 0;	//init
	gsAVL6381_Config.m_FECFrequency_Hz		= 0;	//init
	gsAVL6381_Config.m_RefFrequency_Hz		= 0;	//init
	gsAVL6381_Config.m_ADCFrequency_Hz		= 0;	//init
	gsAVL6381_Config.m_semRx				= &AVL6381_Sem;
	//struct AVL6381_MPEGConfig  m_MPEG_Info
	gsAVL6381_Config.m_MPEG_Info.enumSerialParallel = AVL6381_MPEG_PARALLEL;
	gsAVL6381_Config.m_MPEG_Info.enumRisingFalling 	= AVL6381_MPEG_FALLING;
	gsAVL6381_Config.m_MPEG_Info.enumConti 			= AVL6381_MPEG_CONTINUOUS_DISABLE;
	//struct AVL6381_InputConfig m_InputSignal_Info
	gsAVL6381_Config.m_InputSignal_Info.usDemodI2CAddr		= gsAVL6381_Config.m_SlaveAddr;	
    gsAVL6381_Config.m_InputSignal_Info.ueRefConfig 		= AVL6381_Xtal_30M;
	gsAVL6381_Config.m_InputSignal_Info.ueDTMBInputPath 	= ADC_IF_I;
	gsAVL6381_Config.m_InputSignal_Info.ueDVBCInputPath 	= ADC_IF_I;
    gsAVL6381_Config.m_InputSignal_Info.uiDTMBIFFreqHz 		= 5000000;
	gsAVL6381_Config.m_InputSignal_Info.uiDVBCIFFreqHz 		= 5000000;
    gsAVL6381_Config.m_InputSignal_Info.uiDTMBSymbolRateHz 	= 7560000;
	gsAVL6381_Config.m_InputSignal_Info.uiDVBCSymbolRateHz 	= 6875000;  
	//struct AVL6381_ErrorStat_Info m_ErrorStat_Info
	gsAVL6381_Config.m_ErrorStat_Info.m_ErrorStat_Mode 		=	ERROR_STAT_AUTO;
	gsAVL6381_Config.m_ErrorStat_Info.m_AutoErrorStat_Type	=	ERROR_STAT_TIME;
	gsAVL6381_Config.m_ErrorStat_Info.m_BER_Test_Pattern	=	AVL6381_TEST_LFSR_23;
	gsAVL6381_Config.m_ErrorStat_Info.m_BER_FB_Inversion	=	AVL6381_LFSR_FB_INVERTED;
	gsAVL6381_Config.m_ErrorStat_Info.m_LFSR_Sync			=	0;
	gsAVL6381_Config.m_ErrorStat_Info.m_Time_Threshold_Ms	=	0;
	gsAVL6381_Config.m_ErrorStat_Info.m_Number_Threshold_Byte	=	0;	
	/*----------------------end initialize gsAVL6381_Config struct-----------------------------------*/

    ucPLLIndex = gsAVL6381_Config.m_InputSignal_Info.ueRefConfig * 2; //Para->sInputConfig.ueRefConfig=0
	HI_INFO_TUNER("ucPLLIndex = %d\n",ucPLLIndex);
	s32Ret = IBase_Initialize_6381(&(gAVL6381PLLConfig[ucPLLIndex]),&(pstParam->stAVLChip));
    tuner_mdelay(uiDelayUnit);
    for(uiCounter = 0; uiCounter < uiTimeoutCount; uiCounter++)
    {
        s32Ret = CheckChipReady_6381(&(pstParam->stAVLChip));
        if(s32Ret == AVL6381_EC_OK)
            break;
        tuner_mdelay(uiDelayUnit);
    }

	/* Receiver Init */
    s32Ret = IRx_Initialize_6381(&(pstParam->stAVLChip));

   	s32Ret = DTMB_SetSymbolRate_6381(gsAVL6381_Config.m_InputSignal_Info.uiDTMBSymbolRateHz,&(pstParam->stAVLChip));
	HI_INFO_TUNER("gsAVL6381_Config.m_InputSignal_Info.uiDTMBSymbolRateHz = %d\n",gsAVL6381_Config.m_InputSignal_Info.uiDTMBSymbolRateHz);
    
    s32Ret = SetMpegMode_6381(&(pstParam->stAVLChip));
    s32Ret = SetMpegSerialPin_6381(geAVL6381SerialPin,       &(pstParam->stAVLChip));
    s32Ret = SetMpegSerialOrder_6381(geAVL6381SerialOrder,   &(pstParam->stAVLChip));
    s32Ret = SetMpegSerialSyncPulse_6381(geAVL6381SerialSync,&(pstParam->stAVLChip));
    s32Ret = SetMpegErrorBit_6381(geAVL6381ErrorBit,   &(pstParam->stAVLChip));
    s32Ret = SetMpegErrorPola_6381(geAVL6381ErrorPola, &(pstParam->stAVLChip));
    s32Ret = SetMpegValidPola_6381(geAVL6381ValidPola, &(pstParam->stAVLChip));
    s32Ret = SetMpegPacketLen_6381(geAVL6381PacketLen, &(pstParam->stAVLChip));
    
    if(gsAVL6381_Config.m_MPEG_Info.enumConti == AVL6381_MPEG_CONTINUOUS_ENABLE)
    {
    	HI_INFO_TUNER("m_MPEG_Info.enumConti is continue enable\n");
        if(gsAVL6381_Config.m_MPEG_Info.enumSerialParallel == AVL6381_MPEG_SERIAL)
        {
            s32Ret = DTMB_EnableMpegContinuousMode_6381(guiDTMBSerialMPEGContinuousHz, &(pstParam->stAVLChip));
        }
        else
        {
            s32Ret = DTMB_EnableMpegContinuousMode_6381(guiParallelMPEGContinuousHz, &(pstParam->stAVLChip));
        }
    }else{
		HI_INFO_TUNER("m_MPEG_Info.enumConti is continue disable\n");
	}
    
    s32Ret = EnableMpegOutput_6381(&(pstParam->stAVLChip));

    /* Reset tuner I2C module */
    s32Ret = TunerI2C_Initialize_6381(&(pstParam->stAVLChip));

    /* Set AGC */
    s32Ret = SetAGCPola_6381(geAVL6381AGCPola,&(pstParam->stAVLChip));

    s32Ret = EnableAGC_6381(&(pstParam->stAVLChip));

    /* Error statistics Setting */
    s32Ret = InitErrorStat_6381(&(pstParam->stAVLChip));

	s32Ret = AVL6381_II2C_Write32(&(pstParam->stAVLChip), rc_AVL6381_DVBC_ddc__clks_per_2x_symbol_dvbc_iaddr, 10);


	s32Ret = AVL6381_GetVersion(&pVerInfo_T, &(pstParam->stAVLChip));
	if(s32Ret!=AVL6381_EC_OK)
	{
		HI_ERR_TUNER("Failed to get demod version!\n");
		return s32Ret;
	}
	HI_INFO_TUNER("demod_Chip  = %d.%d.%d\n", pVerInfo_T.m_Chip.m_Major, pVerInfo_T.m_Chip.m_Minor, pVerInfo_T.m_Chip.m_Build);
	HI_INFO_TUNER("API_version = %d.%d.%d\n", pVerInfo_T.m_API.m_Major, pVerInfo_T.m_API.m_Minor, pVerInfo_T.m_API.m_Build);
	HI_INFO_TUNER("fw_version  = %d.%d.%d\n", pVerInfo_T.m_Patch.m_Major, pVerInfo_T.m_Patch.m_Minor, pVerInfo_T.m_Patch.m_Build);

	//Start init Mxl608
	AVL6381_I2CBypassOn(&(pstParam->stAVLChip));
    if( HI_UNF_TUNER_DEV_TYPE_MXL608 == g_stTunerOps[u32TunerPort].enTunerDevType)
    {    
        s32Ret = mxl608_init_tuner(u32TunerPort);
        if(HI_FAILURE == s32Ret)
        {    
            HI_ERR_TUNER("mxl608 init tuner error.\n");
            return HI_FAILURE;
        }    
    }    
    else 
    {    
        HI_INFO_TUNER("tuner type unsupport.\n");
    }
	AVL6381_I2CBypassOff(&(pstParam->stAVLChip));
	//End init Mxl608
	
	pstParam->bInited = HI_TRUE;
    return HI_SUCCESS;
}
/*---------------------------------avl6381_init end----------------------------------*/	
/*---------------------------------avl6381_init end----------------------------------*/	
/*---------------------------------avl6381_init end----------------------------------*/

HI_S32 avl6381_set_ter_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_TER_ATTR_S *pstTerTunerAttr)
{
#if 0
	AVL6381_DEMOD_PARA_S* pstParam;
   // AVL6381_ErrorCode r = AVL6381_EC_OK;
    HI_UNF_TUNER_TER_ATTR_S* pstAttr;


	HI_INFO_TUNER("func= %s  \n",__func__);

    HI_TUNER_CHECKPOINTER( pstTerTunerAttr);

    if (u32TunerPort >= MAX_TUNER)
    {
        return HI_FAILURE;
    }

    /* Make a node for device */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        pstParam = AVL6381_queue_insert(u32TunerPort);
        if (HI_NULL == pstParam)
        {
            return HI_FAILURE;
        }
    }

    if (pstParam->stConfig.u8ChannelId != u32TunerPort)
    {
        return HI_FAILURE;
    }

    pstAttr = (HI_UNF_TUNER_TER_ATTR_S*)pstTerTunerAttr;

	//pstParam->stConfig.enDemodReferenceClk = pstAttr->u32DemodClk;

	HI_INFO_TUNER("u32DemodClk=%d u32ResetGpioNo=%d u16TunerMaxLPF=%d u16TunerI2CClk=%d enRFAGC=%d enIQSpectrum=%d enTSClkPolar=%d enTSFormat=%d enTSSerialPIN=%d enTSSyncHead=%d\n",
				   pstAttr->u32DemodClk,
				   pstAttr->u32ResetGpioNo,
				   pstAttr->u16TunerMaxLPF,
				   pstAttr->u16TunerI2CClk,
				   pstAttr->enRFAGC,
				   pstAttr->enIQSpectrum,
				   pstAttr->enTSClkPolar,
				   pstAttr->enTSFormat,
				   pstAttr->enTSSerialPIN,
				   pstAttr->enTSSyncHead);
#endif

	return HI_SUCCESS;
}
HI_S32 avl6381_deInit(HI_U32 u32TunerPort)
{
    HI_INFO_TUNER("func= %s  \n",__func__);
    if (u32TunerPort >= MAX_TUNER)
    {    
        return HI_FAILURE;
    }    

    AVL6381_queue_remove(u32TunerPort);
    return HI_SUCCESS;
}

HI_S32 avl6381_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S * pstChannel)
{
	AVL6381_ErrorCode r = AVL6381_EC_OK;
	struct AVL6381_Chip * pAVLChip = HI_NULL;
	struct AVL6381_Tuner * pTuner = HI_NULL;
	AVL6381_DEMOD_PARA_S* pstParam;
#if 0
	AVL6381_uint16 times_out_cnt = 0;
	AVL6381_uint32 uiNoSignal = 0;
	AVL6381_uint32 demod_lock_status = 0;
#endif

	HI_INFO_TUNER("func= %s, freq:%d kHz bw:%d KHz\n",__func__,pstChannel->u32Frequency,pstChannel->unSRBW.u32BandWidth);

    HI_ASSERT(HI_NULL != pstChannel);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {    
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }    
    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
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

	r = AVL6381_Halt(pAVLChip);
    if (AVL6381_EC_OK != r)
    {
        HI_ERR_TUNER("CPU halt failed !\n");
        return r;
    }



	HI_INFO_TUNER("func=%s port=%d chn=%d freq=%d\n", __func__,u32TunerPort,pAVLChip->m_I2CChn,pstChannel->u32Frequency);

	//Start set Mxl608
	AVL6381_I2CBypassOn(pAVLChip);

	r = mxl608_set_tuner(u32TunerPort,pAVLChip->m_I2CChn,pstChannel->u32Frequency);
	
	AVL6381_I2CBypassOff(pAVLChip);
	//End set Mxl608
	
    if(HI_FAILURE == r)
    {    
        HI_ERR_TUNER("mxl608 set tuner error.\n");
        return HI_FAILURE;
    }
	tuner_mdelay(50);

  	r = AVL6381_AutoLock(pAVLChip);
	

    if (AVL6381_EC_OK != r)
    {
        HI_ERR_TUNER("func=%s Lock channel failed !r = %d\n",__func__, r);
        return r;
    }

#if 0
	//There is no need to check lock status, the app will use get_status func to get the lock status
    while(times_out_cnt < 30)//this time-out window can be customized, 300*10 = 3000ms for this time
    {
        r = AVL6381_NoSignalDetection(&uiNoSignal,pAVLChip);
        if(uiNoSignal == 1)
        {
            HI_INFO_TUNER("[AVL6381_GetLockStatus] there's no signal!\n");
            break;//no signal, no need to wait any more
        }
        
        //get the lock status
        r = AVL6381_GetLockStatus(&demod_lock_status,pAVLChip);
        if(demod_lock_status == 1)
        {
            HI_INFO_TUNER("[AVL6381_GetLockStatus] channel locked!\n");
            break;//locked, no need to wait any more
        }else{
        	HI_INFO_TUNER("[AVL6381_GetLockStatus] channel NOT locked!\n");
        }
		
        tuner_mdelay(100);
        times_out_cnt++;
    }

    if (AVL6381_EC_OK != r)
    {
        HI_INFO_TUNER("Lock channel fuction failed !r = %d\n", r);
        return r;
    }
#endif

	return r;
}


HI_S32 avl6381_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U32 u32LockStatus  = 0; 
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_DEMOD_PARA_S* pstParam;

    HI_ASSERT(HI_NULL != penTunerStatus);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {    
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }    

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {    
        return HI_FAILURE;
    }

	if(pstParam->bInited)
    {
        /* If locked, u16LockStatus will be 1 */
        r = DTMB_GetLockStatus_6381(&u32LockStatus, &(pstParam->stAVLChip));
        if (AVL6381_EC_OK != r)
        {
            *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
            return HI_FAILURE;
        }
		else
		{
        	HI_INFO_TUNER("func= %s  tuner lockstatus=%d \n",__func__,u32LockStatus);
            *penTunerStatus = (HI_UNF_TUNER_LOCK_STATUS_E)u32LockStatus;
            return HI_SUCCESS;
        }
    }
	else
	{
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
        return HI_SUCCESS;
    }
	return HI_SUCCESS;
}
HI_S32 avl6381_get_ber(HI_U32 u32TunerPort, HI_U32* pu32ber)
{
    HI_U32 u32BER = 0;
    HI_U32 u32Integer;
    HI_U32 u32Fraction;
    HI_U32 u32Exponent;
    AVL6381_DEMOD_PARA_S* pstParam;
	AVL6381_ErrorCode r = AVL6381_EC_OK;

	HI_INFO_TUNER("func= %s  \n",__func__);
    HI_TUNER_CHECKPOINTER( pu32ber);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }

    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

	r = AVL6381_GetBER(&u32BER, &(pstParam->stAVLChip));
	if(r != AVL6381_EC_OK){
		HI_INFO_TUNER("AVL6381_GetBER failed\n");
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
HI_S32 avl6381_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_DEMOD_PARA_S* pstParam;

	HI_INFO_TUNER("func= %s  \n",__func__);
    HI_TUNER_CHECKPOINTER(pu32SNR);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }
    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

	r = AVL6381_DTMB_GetSNR(pu32SNR,&(pstParam->stAVLChip));	
	if(r != AVL6381_EC_OK){
		HI_INFO_TUNER("AVL6381_DTMB_GetSNR failed\n");
		return HI_FAILURE;
	}
	
	*pu32SNR = (*pu32SNR)/100;
	
	return HI_SUCCESS;
}
HI_S32 avl6381_get_signal_strength(HI_U32 u32TunerPort, HI_U32* pu32SignalStrength)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_DEMOD_PARA_S* pstParam;
	AVL6381_uint32  SignalStrength_temp = 0;

	HI_INFO_TUNER("func= %s  \n",__func__);
    HI_TUNER_CHECKPOINTER(pu32SignalStrength);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_ERR_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }
    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

	r = AVL6381_DTMB_GetSignalLevel(&SignalStrength_temp,&(pstParam->stAVLChip));	
	if(r != AVL6381_EC_OK){
		HI_ERR_TUNER("AVL6381_DTMB_GetSNR failed\n");
		return HI_FAILURE;
	}
	pu32SignalStrength[1] = SignalStrength_temp*100/65535;
	return HI_SUCCESS;
}

HI_S32 avl6381_get_signal_quality(HI_U32 u32TunerPort, HI_U32* pu32SignalQuality)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_DEMOD_PARA_S* pstParam;

	HI_INFO_TUNER("func= %s  \n",__func__);
    HI_TUNER_CHECKPOINTER(pu32SignalQuality);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }
    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

	r = AVL6381_GetSQI(pu32SignalQuality,&(pstParam->stAVLChip));
	if(r != AVL6381_EC_OK){
		HI_INFO_TUNER("AVL6381_GetSQI failed\n");
		return HI_FAILURE;
	}
	return HI_SUCCESS;	

}
HI_S32 avl6381_get_freq_symb_offset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb)
{
    AVL6381_ErrorCode r = AVL6381_EC_OK;
    AVL6381_DEMOD_PARA_S* pstParam;
	
    HI_TUNER_CHECKPOINTER(pu32Freq);
	HI_TUNER_CHECKPOINTER(pu32Symb);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
    if (HI_NULL == pstParam)
    {
        return HI_FAILURE;
    }
    if (!pstParam->bInited)
    {
        HI_INFO_TUNER("Tuner %d hadn't been inited.\n", u32TunerPort);
        return HI_FAILURE;
    }

	r = AVL6381_DTMB_Get_CarrierFreq_Offset(pu32Symb, pu32Freq, &(pstParam->stAVLChip));
	if(r != AVL6381_EC_OK){
		HI_INFO_TUNER("func=%s -> AVL6381_DTMB_Get_CarrierFreq_Offset failed\n", __func__);
		return HI_FAILURE;
	}
	HI_INFO_TUNER("func= %s Freq_offset=%d Symbol_rate=%d\n",__func__, *pu32Freq, *pu32Symb);
	
	return HI_SUCCESS;
}
HI_S32 avl6381_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
	AVL6381_ErrorCode r = AVL6381_EC_OK;
	struct AVL6381_Chip * pAVLChip = HI_NULL;
    AVL6381_DEMOD_PARA_S* pstParam;
	AVL6381_DTMB_SignalInfo SignalInfo;
	
    HI_INFO_TUNER("func= %s  \n",__func__);
	HI_TUNER_CHECKPOINTER( pstInfo);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
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

	r = AVL6381_DTMB_GetSignalInfo(&SignalInfo, pAVLChip);
	if(r != AVL6381_EC_OK){
		 HI_ERR_TUNER("func=%s -> DTMB_GetSignalInfo_6381 failed\n", __func__);
		return HI_FAILURE;
	}

	//signal type of the tuner
	pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_DTMB;

	//FEC code rate
	switch (SignalInfo.m_CodeRate)
	{
		case RX_FEC_2_5:
			pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_2_5; 
			break;
		case RX_FEC_3_5:
			pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_3_5; 
			break;
		case RX_FEC_4_5:
			pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FEC_4_5; 
			break;
		default:
			pstInfo->unSignalInfo.stTer.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT; 
			break;
	}

	//modulation mode
	switch (SignalInfo.m_Modulation)
	{
		case RX_DTMB_4QAM:
			pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_DEFAULT; //unsupport now
			break;
		case RX_DTMB_16QAM:
			pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_16;
			break;
		case RX_DTMB_32QAM:
			pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_32;
			break;
		case RX_DTMB_64QAM:
			pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_QAM_64;
			break;
		default:
			pstInfo->unSignalInfo.stTer.enModType = HI_UNF_MOD_TYPE_BUTT;
			break;
	}

	pstInfo->unSignalInfo.stTer.u32Freq 	 = g_stTunerOps[u32TunerPort].stCurrPara.u32Frequency;//khz
	pstInfo->unSignalInfo.stTer.u32BandWidth = g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth;//khz

	HI_INFO_TUNER("func=%s -> freq=%d khz bw=%d khz\n", __func__,pstInfo->unSignalInfo.stTer.u32Freq,pstInfo->unSignalInfo.stTer.u32BandWidth);
	
	return HI_SUCCESS;
}


HI_S32 avl6381_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
	AVL6381_DEMOD_PARA_S * pstParam;

	HI_INFO_TUNER("func= %s  \n",__func__);

    /* Check tuner port and init. */
    if (u32TunerPort >= MAX_TUNER)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }      
    /* Find node */
    pstParam = AVL6381_queue_get(u32TunerPort);
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

	g_stTunerOps[u32TunerPort].enTsType = enTsType;

    switch (enTsType)
    {
	    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
	        pstParam->stConfig.enTSOutPutMode = AVL6381_MPEG_SERIAL;
	        break;

	    case HI_UNF_TUNER_OUTPUT_MODE_DEFAULT:
	    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
	    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
	    default:
	        pstParam->stConfig.enTSOutPutMode = AVL6381_MPEG_PARALLEL;
	        break;
    }
	
	return HI_SUCCESS;
}

HI_S32 avl6381_standby(HI_U32 u32TunerPort, HI_U32 u32Standby)
{
    HI_INFO_TUNER("func= %s  \n",__func__);
	return HI_SUCCESS;
}

