#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "drv_demod.h"
    
#include "hal_hiFoneCab100.h"
#include "hiF_J83B100.h"
#include "hi_reg_common.h"
#include "tda18250b.h"

#ifdef __cplusplus
 #if __cplusplus
    extern "C" {
 #endif
#endif /* __cplusplus */

static HI_BOOL s_bHiDVBCInited  = HI_FALSE;
//HI_UNF_MODULATION_TYPE_E	enDVBCModType = HI_UNF_MOD_TYPE_QAM_64;

HI_VOID HiFJ83B100_HotReset(HI_U32 u32TunerPort)
{
    U_QAMCT_RESET        qamct_reset;
	
	qam_read_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H, &(qamct_reset.u8));
    qamct_reset.bits.hot_reset = 0x0;
    qam_write_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H, qamct_reset.u8);
    tuner_mdelay(1);
    qam_read_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H , &(qamct_reset.u8));
    qamct_reset.bits.hot_reset = 0x1;
    qam_write_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H, qamct_reset.u8);

	return;
}

HI_S32 HiFJ83B100_Init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    HI_U32 fdem = 0;
    HI_U32 fadc = 0;
    HI_U32 freq_if = 0;
    HI_U8  fl,fm,fh;
	HI_S32 s32Ret = HI_FAILURE;
    
    U_HiDTMB_MOD_CTRL    HiDTMB_MOD_CTRL  ;
    U_QAMC_FDEMO_H       qamc_fdemo_h     ;
    U_QAMC_FADC_H        qamc_fadc_h      ;
    U_ADC_AGC_CTRL       adc_agc_ctrl     ;
	U_QAMC_SF_RMV_CTRL	 sf_rmv_ctrl;
	U_QAMC_TR_CR_CTRL	 tr_cr_ctrl;

    HiDTV100_HAL_CLK_ENABLE(HI_TRUE);
	HiDTV100_HAL_AD_CFG(HI_FALSE);
	HiDTV100_HAL_AD_RESET();
	HiDTV100_HAL_QAMC_RESET();
	 
    /*PERICTRL config*/
    s32Ret = HiDTV100_HAL_AD_CFG(HI_TRUE);
    if(HI_FAILURE == s32Ret)
    {
        HI_ERR_TUNER("HiDTV100_PERI_CFG is nok! Fail to initial qamadc!\n");
        return s32Ret;
    }
	tuner_mdelay(200);

    fdem    = 86400;//对应的符号率:6875 fdem/11
    fadc    = 43200;//86.4/2;
    freq_if = 5000;
    HI_INFO_TUNER("-->IN--freq_if  = %d\n",freq_if);

    //mod 工作模式
    qam_read_byte(u32TunerPort,HIFONE_J83B_CHN_SEL, &(HiDTMB_MOD_CTRL.u8));

	HiDTMB_MOD_CTRL.bits.mode = 0x3; //j83b

    qam_write_byte(u32TunerPort,HIFONE_J83B_CHN_SEL, HiDTMB_MOD_CTRL.u8);

    //dem clk 
    fl = (fdem & 0xFF);
    fm = (fdem>>8) & 0xFF;
    fh = (fdem>>16) & 0x03;
    
    qam_read_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H, &(qamc_fdemo_h.u8));
    qamc_fdemo_h.bits.fdemo_h = fh;
    qam_write_byte(u32TunerPort,HIFONE_J83B_DEMO_FREQ_L, fl);
    qam_write_byte(u32TunerPort,HIFONE_J83B_DEMO_FREQ_M, fm);
    qam_write_byte(u32TunerPort,HIFONE_J83B_DEMO_FEC_FREQ_H, qamc_fdemo_h.u8);

    //adc clk
    fl = (fadc & 0xFF);
    fm = (fadc>>8) & 0xFF;
    fh = (fadc>>16) & 0x03;
    qam_read_byte( u32TunerPort,HIFONE_J83B_TR_LOOP_CTRL, &(qamc_fadc_h.u8));
    qamc_fadc_h.bits.fadc_h = fh;
    qam_write_byte( u32TunerPort,HIFONE_J83B_FREQ_ADC_L, fl);
    qam_write_byte( u32TunerPort,HIFONE_J83B_FREQ_ADC_M, fm);
    qam_write_byte( u32TunerPort,HIFONE_J83B_TR_LOOP_CTRL, qamc_fadc_h.u8);
    
    //IF clk  
    fl = (freq_if & 0xFF);
    fh = (freq_if>>8) & 0xFF;
    qam_write_byte( u32TunerPort,HIFONE_J83B_FREQ_DEV_L, fl);
    qam_write_byte( u32TunerPort,HIFONE_J83B_FREQ_DEV_H, fh);
    
    
    //ADC/AGC ctrl
    qam_read_byte( u32TunerPort,HIFONE_J83B_AGC_CTRL, &(adc_agc_ctrl.u8));
    adc_agc_ctrl.bits.c_adc_twos  = 0x1; //补码
    adc_agc_ctrl.bits.c_iq_swap   = 0x0; //I
    qam_write_byte( u32TunerPort,HIFONE_J83B_AGC_CTRL, adc_agc_ctrl.u8);
    
    //qam_write_byte( DVBC_J83B_AGC_GOAL, 0x18);
    /*if(HI_UNF_TUNER_DEV_TYPE_TDA18275 == enDTV100TunerType
		|| HI_UNF_TUNER_DEV_TYPE_SI2151 == enDTV100TunerType
		||HI_UNF_TUNER_DEV_TYPE_TDA18275A == enDTV100TunerType)
	{*/
		qam_write_byte(u32TunerPort,HIFONE_J83B_AGC_GOAL, 0x10);
		//qam_write_byte(DTMB_AGC_SPEED_BOUND, 0x6c);
	/*}
	else
	{
		qam_write_byte(u32TunerPort,DVBC_J83B_AGC_GOAL, 0x0d);
		//qam_write_byte(DTMB_AGC_SPEED_BOUND, 0x67);
	}*/
	qam_read_byte( u32TunerPort,HIFONE_J83B_SF_RMV_CTRL, &(sf_rmv_ctrl.u8));
	sf_rmv_ctrl.bits.sf_rmv = 1;
	qam_write_byte( u32TunerPort,HIFONE_J83B_SF_RMV_CTRL, sf_rmv_ctrl.u8);

	qam_read_byte( u32TunerPort,HIFONE_J83B_TR_CR_CTRL, &(tr_cr_ctrl.u8));
	tr_cr_ctrl.bits.scan_fs = 0;
	qam_write_byte( u32TunerPort,HIFONE_J83B_TR_CR_CTRL, tr_cr_ctrl.u8);
    s_bHiDVBCInited = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_Connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_U8  fl,fh;
    HI_U32 u32SymbolRate = 0;
    U_QAMC_MODU_MODE qamc_modu_mode;

	HiFone_J83B_CHECKPOINTER(pstChannel);

	g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort,g_stTunerOps[u32TunerPort].enI2cChannel,pstChannel->u32Frequency);
	HiDTV100_HAL_AD_RESET();
	
	//after set tuner,delay 20ms befor setting demod
	tuner_mdelay(20);
	
    u32SymbolRate = pstChannel->unSRBW.u32SymbolRate / 1000; //kHz
    
    /*调制模式*/
    
    qam_read_byte( u32TunerPort,HIFONE_J83B_EQU_CTRL, &(qamc_modu_mode.u8));
    switch(pstChannel->enQamType)
    {
        case QAM_TYPE_16:
        {
            qamc_modu_mode.bits.modu_mode = 0x4;
            break;
        }
        case QAM_TYPE_32:
        {
            qamc_modu_mode.bits.modu_mode = 0x5;
            break;
            
        }
        case QAM_TYPE_64:
        {
            qamc_modu_mode.bits.modu_mode = 0x6;
            break;
            
        }
        case QAM_TYPE_128:
        {
            qamc_modu_mode.bits.modu_mode = 0x7;
            break;
            
        }
        case QAM_TYPE_256:
        {
            qamc_modu_mode.bits.modu_mode = 0x8;
            break;
            
        }
        default:
        {
            qamc_modu_mode.bits.modu_mode = 0x6;
            break;
        }
    }
    qam_write_byte( u32TunerPort,HIFONE_J83B_EQU_CTRL, qamc_modu_mode.u8);

    //符号率
    fl = (u32SymbolRate & 0xFF);
    fh = (u32SymbolRate>>8) & 0xFF;
    qam_write_byte( u32TunerPort,HIFONE_J83B_FS_L, fl);
    qam_write_byte( u32TunerPort,HIFONE_J83B_FS_H, fh);

    //热复位 hidtmb_reset
    HiFJ83B100_HotReset(u32TunerPort);
    
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_DisConnect(HI_VOID)
{
    HiDTV100_HAL_AD_CFG(HI_FALSE);
    HiDTV100_HAL_CLK_ENABLE(HI_FALSE);
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_GetStatus(HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    HI_U8 u8RegData;

    qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8RegData);
    if((u8RegData & 0xbf) == 0xbf)
    {
        HI_INFO_TUNER("--IN-->HiDTV100_DVBC_GetStatus \n");
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
	else
	{
		*penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
	}

	
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_Standby(HI_U32 u32Standby)
{
    return HI_SUCCESS;
}

HI_VOID HiFJ83B100_GetRegisters(HI_VOID *p)
{   
	HiFone_J83B_CHECKPOINTER(p);

	return;
}


HI_S32 HiFJ83B100_GetSignalInfo(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
	HiFone_J83B_CHECKPOINTER(pstInfo);

	pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_J83B;
	
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_GetBer(HI_U32 u32TunerPort, HI_U32 *pu32ber)
{
    //HI_U32 u32BerThres = 4; /* modify by chenxu 2008-01-15 AI7D02092 */
    HI_U8  u8Temp	 = 0;
    HI_U8  u8Val     = 0;
    //HI_U32 u8RegValue1 = 0;
    //HI_U32 u8RegValue2 = 0;
    HI_U32 u32RegValue3 = 0;

	HiFone_J83B_CHECKPOINTER(pu32ber);
	
	qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8Temp);
	if(0x0 == (u8Temp & 0x30))
	{
	    HI_WARN_TUNER("--IN-->HiDTV100_DVBC_GetRs fec_no_ok! \n");
		return HI_SUCCESS;
	}
 
	/* rs package,calculate RS error  package*/

	/*qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Temp);	
	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Val);
	u8RegValue1 = ( u8Temp << 8 ) | u8Val;

	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Temp);	
	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Val);
	u8RegValue2 = ( u8Temp << 8 ) | u8Val;*/
	
	qam_read_byte( u32TunerPort,HIFONE_J83B_ERR_PKT_L, &u8Val);
	qam_read_byte( u32TunerPort,HIFONE_J83B_ERR_PKT_H, &u8Temp);//先低后高读寄存器
    u8Temp = ( u8Temp & 0x1F );
	u32RegValue3 = ( u8Temp << 8 ) | u8Val;
	pu32ber[0] = u32RegValue3;//u8RegValue1;
	pu32ber[1] = 0;//u8RegValue2;
	pu32ber[2] = 0;//u32RegValue3;
	
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_GetSnr(HI_U32 u32TunerPort, HI_U32* pu32SNR)
{
    HI_U8  u8RegData;
    HI_U8  au8RegData[2] = { 0 };
    HI_U32 u32Snr = 0;
    
    HiFone_J83B_CHECKPOINTER(pu32SNR);

	qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8RegData);
	if(0x0 == (u8RegData & 0x4))
	{
	    HI_WARN_TUNER("--IN-->HiDTV100_DVBC_GetSnr equ_no_ok! \n");
		return HI_SUCCESS;
	}
    
    qam_read_byte( u32TunerPort,HIFONE_J83B_NOISE_POW_L, &au8RegData[0]);
    qam_read_byte( u32TunerPort,HIFONE_J83B_NOISE_POW_H, &au8RegData[1]);  //先低后高读寄存器
    u32Snr = au8RegData[0] + au8RegData[1]*256;

    *pu32SNR = u32Snr;

    return HI_SUCCESS;
}


HI_S32 HiFJ83B100_GetSignalStrength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)
{
    HI_U8  u8RegData;
    HI_U8  au8RegData[2] = { 0 };
    HI_U32 u32TmpSigStrength = 0;
    
    HiFone_J83B_CHECKPOINTER(pu32SignalStrength);

	qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8RegData);
	if(0x0 == (u8RegData & 0x1))
	{
	    HI_WARN_TUNER("--IN-->HiDTV100_DVBC_GetSignalStrength agc_no_ok! \n");
		return HI_SUCCESS;
	}
    
    qam_read_byte( u32TunerPort,HIFONE_J83B_AGC_CTRL_L, &au8RegData[0]);
    qam_read_byte( u32TunerPort,HIFONE_J83B_AGC_CTRL_H, &au8RegData[1]);  //先低后高读寄存器
    u32TmpSigStrength = au8RegData[0] + (au8RegData[1] & 0x0f)*256;
	if(u32TmpSigStrength <= 419)
	{
		pu32SignalStrength[1] = 99;
	}
	else if(u32TmpSigStrength >= 2498)
	{
		pu32SignalStrength[1] = 10;
	}
	else
	{
		pu32SignalStrength[1] = 117 - u32TmpSigStrength * 89 / 2079;
	}
        
    HI_INFO_TUNER("-->IN--HiDTV100_DVBC_GetSignalStrength u32TmpSigStrength = %d,%d\n",u32TmpSigStrength,pu32SignalStrength[1]);
   
    return HI_SUCCESS;

}

HI_S32 HiFJ83B100_SetTsType(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    return HI_SUCCESS;
}

HI_S32 HiFJ83B100_GetFreqSymbOffset(HI_U32 u32TunerPort, HI_U32 * pu32Freq, HI_U32 * pu32Symb )	
{
    HI_U8  u8RegData = 0;
    HI_U32 u32Freq = 0;
    HI_S32 s32FreqOffset = 0;
    HI_U8  au8RegData[2] = { 0 };
    HI_S32 s32RealSymb = 0;

	HiFone_J83B_CHECKPOINTER(pu32Freq);
	HiFone_J83B_CHECKPOINTER(pu32Symb);
	
	qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8RegData);
	if(0x0 == (u8RegData & 0x30))
	{
	    HI_WARN_TUNER("--IN-->HiDTV100_DVBC_GetFreqSymbOffset fec_no_ok! \n");
		return HI_SUCCESS;
	}
    
    /* calc freq */
    qam_read_byte( u32TunerPort,HIFONE_J83B_FREQ_DEV_L, &au8RegData[0]);
    qam_read_byte( u32TunerPort,HIFONE_J83B_FREQ_DEV_H, &au8RegData[1]);	//先低后高读寄存器
    u32Freq = au8RegData[0] + au8RegData[1]*256;
    if(u32Freq >= 32768)
    {
        s32FreqOffset = u32Freq - 65536;
    }
    else
    {
        s32FreqOffset = u32Freq;
    }
    
    pu32Symb[0] = s32FreqOffset;
    HI_INFO_TUNER("s32FreqOffset = %d\n",s32FreqOffset);
    
    /* calc symbolrate */
    qam_read_byte( u32TunerPort,HIFONE_J83B_FS_L, &au8RegData[0]);
    qam_read_byte( u32TunerPort,HIFONE_J83B_FS_H, &au8RegData[1]);	
    
    s32RealSymb = au8RegData[0] + au8RegData[1]*256;
   
    pu32Symb[1] = s32RealSymb;
    HI_INFO_TUNER("s32RealSymb = %d\n",s32RealSymb);
  
    return HI_SUCCESS;
}

    
HI_S32 HiFJ83B100_GetRs(HI_U32 u32TunerPort,HI_U32 * pu32Rs)
{
    //HI_U32 u32BerThres = 4; /* modify by chenxu 2008-01-15 AI7D02092 */
    HI_U8  u8Temp	 = 0;
    HI_U8  u8Val     = 0;
    //HI_U32 u8RegValue1 = 0;
    //HI_U32 u8RegValue2 = 0;
    HI_U32 u32RegValue3 = 0;

	HiFone_J83B_CHECKPOINTER(pu32Rs);
	
	qam_read_byte( u32TunerPort,HIFONE_J83B_LOCK_FLAG, &u8Temp);
	if(0x0 == (u8Temp & 0x30))
	{
	    HI_WARN_TUNER("--IN-->HiDTV100_DVBC_GetRs fec_no_ok! \n");
		return HI_SUCCESS;
	}
 
	/* rs package,calculate RS error  package*/

	/*qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Temp);	
	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Val);
	u8RegValue1 = ( u8Temp << 8 ) | u8Val;

	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Temp);	
	qam_read_byte(u32TunerPort, DVBC_J83B_FS_H, &u8Val);
	u8RegValue2 = ( u8Temp << 8 ) | u8Val;*/
	
	qam_read_byte( u32TunerPort,HIFONE_J83B_ERR_PKT_L, &u8Val);
	qam_read_byte( u32TunerPort,HIFONE_J83B_ERR_PKT_H, &u8Temp);//先低后高读寄存器
    u8Temp = ( u8Temp & 0x1F );
	u32RegValue3 = ( u8Temp << 8 ) | u8Val;
	pu32Rs[0] = 0;//u8RegValue1;
	pu32Rs[1] = 0;//u8RegValue2;
	pu32Rs[2] = u32RegValue3;
	
	return HI_SUCCESS;
  
}

HI_VOID HiFJ83B100_connect_timeout(HI_U32 u32ConnectTimeout)
{
    return;
}

HI_VOID HiFJ83B100_read_tuner_registers(HI_U32 u32TunerPort, struct seq_file * p)
{
	volatile U_PERI_QAM_ADC0 qam_adc0_peri_cfg;
	HI_S32	s32Ret = HI_SUCCESS;
    HI_U8 i;
    HI_U8 u8Data = 0;

	qam_adc0_peri_cfg.u32 = g_pstRegPeri->PERI_QAM_ADC0.u32;
	qam_adc0_peri_cfg.bits.qamadc_i2c_devaddr = 0x48;
	g_pstRegPeri->PERI_QAM_ADC0.u32 = qam_adc0_peri_cfg.u32;
	
    PROC_PRINT(p,"  addr ");
    for(i = 0;i <= 0x0F;i++)
    {
        PROC_PRINT(p,"%2x ",i);
    }
    
    for(i = 0;i < 0xFF;i++)
    {
        //qam_read_byte(u32TunerPort, i, &u8Data);
		s32Ret = HI_DRV_I2C_Read(6,0x90,i,1,&u8Data,1);
		if(HI_SUCCESS != s32Ret)
			break;
		
        if(i % 16 == 0)
        {
        	PROC_PRINT(p,"\n %04x0:",i/16);
        }
        PROC_PRINT(p,"%2x ",u8Data);
    }
    PROC_PRINT(p,"\n");
}

#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */

