/***********************************************************************************
*              Copyright 2012 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi3136.c
* Description:
*
* History:
* Version   Date             Author     DefectNum        Description
* main\1    2012-08-08   j207467    NULL                Create this file.
***********************************************************************************/

/*#include <linux/delay.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/time.h>
#include <linux/module.h>
#include <asm/io.h>
#include <linux/kthread.h>*/

#include "hi_type.h"
#include "hi_drv_i2c.h"
#include "hi_debug.h"
/*#include "hi_error_mpi.h"*/
//#include " hi_unf_ecs_type.h"
//#include "drv_proc_ext.h"

//#include "common_proc.h"
//#include "tuner_common_pilot.h"
#include "drv_demod.h"
#include "av2011.h"
#include "sharp_qm1d1c004x.h"
#include "M88TS2022.h"
#include "RDA5815.h"
//#define TUNER_NUM 3

HI3136_CFG_ITEM_S hi3136_cfg[TUNER_NUM];
HI_U32 u32LPF_KHz[TUNER_NUM] = {0};
static HI_S32 hi3136_setlpf(HI_U32 u32TunerPort, HI_U32 u32SymbolRate/*unit BAUD ,neither K, nor M*/);

typedef enum tagHI3136_LOCK_STATUS_E
{
    UNLOCKED = 0,
    AGC_LOCKED,
    CBS_LOCKED,
    TR_LOCKED,
    SYNC_LOCKED,
    CR_LOCKED,
    FEC_LOCKED
} HI3136_LOCK_STATUS_E;

/*HI_U32 hi3136_I2C_Check(u32TunerPort)
{
    return HI_SUCCESS;
}*/

static HI_VOID hi3136_HotReset_CHIP(HI_U32 u32TunerPort)
{
    HI_U8 u8Temp;

    qam_read_byte(u32TunerPort, RSTN_CTRL, &u8Temp);
    u8Temp &= 0xfd;
    qam_write_byte(u32TunerPort, RSTN_CTRL, u8Temp);
    u8Temp |= 0x02;
    qam_write_byte(u32TunerPort, RSTN_CTRL, u8Temp);
}

HI_VOID hi3136_config_i2c_out(HI_U32 u32TunerPort, HI_BOOL bTuner)
{
    if (HI_UNF_DEMOD_DEV_TYPE_3136I == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        //printk("%s, %d\n", __func__, __LINE__);
        return;
    }
    
    if (bTuner)
    {
        (HI_VOID)qam_write_bit(u32TunerPort, TUNER_SEL, 0, 1); /*set I2C_repeater_en 1*/
    }
}

static HI_VOID hi3136_config_IQ_swap(HI_U32 u32TunerPort, HI_BOOL bSwap)  /*TR module S2 IQ swap*/
{
    if (bSwap)
    {
        (HI_VOID)qam_write_bit(u32TunerPort, AGC_CTRL, 3, 1);
    }
    else
    {
        (HI_VOID)qam_write_bit(u32TunerPort, AGC_CTRL, 3, 0);
    }
}

#ifndef BOOT_COMPILE
HI_BOOL hi3136_Set_Ts_Out_Enable(HI_U32 u32TunerPort,HI_BOOL bTS_ON)
{
    //HI_U8 u8Temp;

    if(bTS_ON)
    {
        qam_write_bit(u32TunerPort, TS_CTRL0, 1, 1);
    }
    else
    {
        qam_write_bit(u32TunerPort, TS_CTRL0, 1, 0);
    }

    return HI_SUCCESS;
}
#endif

HI3136_LOCK_STATUS_E hi3136_Read_Lock_Status(HI_U32 u32TunerPort)  //设置状态寄存器，
{	
    HI_U8 u8Temp;
    HI3136_LOCK_STATUS_E lockstatus;

    lockstatus = UNLOCKED;

    qam_read_byte(u32TunerPort, LOCK_FLAG, &u8Temp);
    //printk("%s, %d, LOCK_FLAG = %x\n", __func__, __LINE__, u8Temp);
    if (u8Temp & (1 << 5))
    {
        lockstatus = FEC_LOCKED;
    }
    else if (u8Temp & (1 << 4))
    {
        lockstatus = CR_LOCKED;
    }
    else if (u8Temp & (1 << 3))
    {
        lockstatus = SYNC_LOCKED; 
    }
    else if (u8Temp & (1 << 2))
    {
        lockstatus = TR_LOCKED;
    }
    else if (u8Temp & (1 << 1))
    {
        lockstatus = CBS_LOCKED;
    }
    else if (u8Temp & 1)
    {
        lockstatus = AGC_LOCKED;
    }
    else
    {
        lockstatus = UNLOCKED;
    }

    return lockstatus;
}

#ifndef BOOT_COMPILE
HI_S32 hi3136_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)  
{
    HI_U8 au8RegVal[2] = {0};

    HI_ASSERT(HI_NULL != pu32SignalStrength);	//判断是否是 空指针

    qam_read_byte(u32TunerPort, AGC_CTRL_L, &au8RegVal[1]);
    qam_read_byte(u32TunerPort, AGC_CTRL_H, &au8RegVal[0]); 

    pu32SignalStrength[1] = ((au8RegVal[0] & 0x0f) << 8) + au8RegVal[1];

    return HI_SUCCESS;
}
#endif

#if 0
HI_S32 hi3136_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength)  
{
    HI_U8 au8RegVal[2] = {0};
    HI_U8 u16TimeOut;
    
    HI_ASSERT(HI_NULL != pu32SignalStrength);	//判断是否是 空指针
    
    qam_read_byte(u32TunerPort, AGC_CTRL_L, &au8RegVal[0]);
    qam_read_byte(u32TunerPort, AGC_CTRL_H, &au8RegVal[1]); 
    do
    {
        msleep_interruptible(2);
        qam_read_byte(u32TunerPort, AGC_CTRL_L, &au8RegVal[0]);
        qam_read_byte(u32TunerPort, AGC_CTRL_H, &au8RegVal[1]); 
        u16TimeOut += 2;
    } while((au8RegVal[1]>128)&&u16TimeOut<10);
    
    pu32SignalStrength[0] = ((au8RegVal[1] & 0x0f) << 8) + au8RegVal[0];
    HI_UNF_TUNER_DEV_TYPE_E enTunerDevType;
    HI_UNF_TUNER_I2cChannel_E enI2cChannel;
    HI_S16 level,final_level;
    HI_U8 BW,u8Temp,temp1,temp2,cnr_ok;
    HI_U32 symbolRate,SNR;
    HI_FLOAT bw_ratio,cnr_ratio,temp3,delta;
    switch (enTunerDevType)
    {
    case HI_UNF_TUNER_DEV_TYPE_AV2011:
    default:
    {
    if(pu32SignalStrength[0]<1000)  //>-10dbm
    level=-10;
    else if ((pu32SignalStrength[0]>=1000)&&(pu32SignalStrength[0]<1600))	//-40dbm ~ -10dbm
    level =-40-0.05*(pu32SignalStrength[0]-1600);
    else if ((pu32SignalStrength[0]>=1600)&&(pu32SignalStrength[0]<2200))	//-40dbm ~ -10dbm
    level =-60-0.033*(pu32SignalStrength[0]-2200);
    else if ((pu32SignalStrength[0]>=2200)&&(pu32SignalStrength[0]<3000))	//-80dbm ~ -60dbm
    level =-80-0.025*(pu32SignalStrength[0]-3000);
    else 
    level =-90-0.01*(pu32SignalStrength[0]-4095);  	//<-80dbm
    
    break;
    }
    case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
    {	
    if(pu32SignalStrength[0]<600)	//>-0dbm
    level=0;
    else if ((pu32SignalStrength[0]>=600)&&(pu32SignalStrength[0]<1150))	//-18dbm ~ 0dbm
    level =-18-(HI_FLOAT)(18/500)*(pu32SignalStrength[0]-1150);
    else if ((pu32SignalStrength[0]>=1150)&&(pu32SignalStrength[0]<1300))	//-30dbm ~ -18dbm
    level =-30-(HI_FLOAT)(12/150)*(pu32SignalStrength[0]-1300);
    else if ((pu32SignalStrength[0]>=1300)&&(pu32SignalStrength[0]<1700))	//-40dbm ~ -30dbm
    level =-40-(HI_FLOAT)(10/400)*(pu32SignalStrength[0]-1700);
    else if ((pu32SignalStrength[0]>=1700)&&(pu32SignalStrength[0]<2340))	//-60dbm ~ -40dbm
    level =-60-(HI_FLOAT)(20/640)*(pu32SignalStrength[0]-2340);
    else if ((pu32SignalStrength[0]>=2340)&&(pu32SignalStrength[0]<2480))	//-63dbm ~ -60dbm
    level =-63-(HI_FLOAT)(3/140)*(pu32SignalStrength[0]-2480);
    else if ((pu32SignalStrength[0]>=2480)&&(pu32SignalStrength[0]<2650))	//-66dbm ~ -63dbm
    level =-66-(HI_FLOAT)(3/170)*(pu32SignalStrength[0]-2650);
    else if ((pu32SignalStrength[0]>=2650)&&(pu32SignalStrength[0]<3500))	//-85dbm ~ -66dbm
    level =-85-(HI_FLOAT)(19/850)*(pu32SignalStrength[0]-3500);
    else 		//<-85dbm
    level=-85;
    break;
    }     
    
    }
    switch (enTunerDevType)
    {
    case HI_UNF_TUNER_DEV_TYPE_AV2011:
    {
    av2011_read (u32TunerPort,  enI2cChannel, 0xc7,0x05, 1, &u8Temp);
    BW=(u8Temp*21100-0.5)/(127*1000);
    break;
    }
    case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
    {
    break;
    }
    }
    qam_read_byte(u32TunerPort, FS_L, &temp1);
    qam_read_byte(u32TunerPort, FS_H, &temp2);
    symbolRate=temp1+temp2*256;
    bw_ratio=(HI_FLOAT)(BW/symbolRate);	//浮点运算，保留小数
    if(bw_ratio<1)
    bw_ratio=1;
    qam_read_byte(u32TunerPort, NOISE_POW_CPU_H, &temp2); //先读高，后读低
    qam_read_byte(u32TunerPort, NOISE_POW_CPU_L, &temp1);
    if(temp2>=128)
    cnr_ok=1;
    if(cnr_ok)
    {
    SNR=(temp2&0x3f)*256+temp1;
    cnr_ratio=(HI_FLOAT)(8192/SNR);
    temp3=(HI_FLOAT)((bw_ratio-1)/(cnr_ratio+1)+1);
    delta=10*log10(temp3);
    final_level=(HI_S32)(level-delta);		//dbm
    if(final_level<-90)
    final_level=-90;
    }
    else
    {
    final_level=level;		//dbm
    }
    
    return HI_SUCCESS;
}
#endif

#ifndef BOOT_COMPILE
HI_S32 hi3136_get_snr(HI_U32 u32TunerPort, HI_U32* pu32SNR)  
{
    HI_U8 au8RegVal[2] = {0};
    HI_U32 snr = 0;

    HI_ASSERT(HI_NULL != pu32SNR);	//判断是否是 空指针

    qam_read_byte(u32TunerPort, NOISE_POW_CPU_H, &au8RegVal[0]); 
    qam_read_byte(u32TunerPort, NOISE_POW_CPU_L, &au8RegVal[1]);
    snr = ((au8RegVal[0] & 0x7f) << 8) + au8RegVal[1];	   

    *pu32SNR = snr;

    return HI_SUCCESS;	
}

HI_S32 hi3136_get_ber(HI_U32 u32TunerPort, HI_U32 *pu32ber)  
{
    HI_U8   u8Temp, temp, temp1, temp2;
    HI_U32 	ErrorRate = 0;
    HI_U32 	a, b;
    HI_U32 	total_num, temp3;

    if(pu32ber == HI_NULL)
    {
        return HI_FAILURE;
    }
    if (hi3136_Read_Lock_Status(u32TunerPort) >= SYNC_LOCKED)
    {
        qam_read_byte(u32TunerPort, SEAR_RESULT, &u8Temp);			
        temp = (u8Temp & 0x01);		//DVBS OR DIRECT TV
        qam_read_byte(u32TunerPort, LOCK_FLAG, &u8Temp);
        if(u8Temp & (1 << 3))
        {
            temp = 2;     //sync_ok 指示S2 signal
        }

        switch(temp)
        {
        case 0: 
            {	//DVBS

                qam_read_byte(u32TunerPort, BER_CTRL, &u8Temp);						
                temp1 = (u8Temp & 0x70) >> 4;
                switch(temp1)
                {
                case 0 : 
                    a = 0x10;   
                    break;
                case 1 :  
                    a = 0x40;       
                    break;
                case 2 :   
                    a = 0x100;      
                    break;
                case 3 :   
                    a = 0x400;      
                    break;
                case 4 :   
                    a = 0x1000;     
                    break;
                case 5 :  
                    a = 0x4000;     
                    break;
                case 6 :   
                    a = 0x10000; 
                    break;
                default:    
                    a = 0x40000; 
                }
                total_num = a * 204 * 8;
                qam_read_byte(u32TunerPort, FEC_BER_L, &temp2);
                qam_read_byte(u32TunerPort, FEC_BER_H, &temp1);	

                temp3 = (temp1 << 8) + temp2;
                ErrorRate = temp3 / total_num * 1000000000;
            }
            break;
        case 1: 
            {	//DIRECT TV
                qam_read_byte(u32TunerPort, BER_CTRL, &u8Temp);						
                temp1 = (u8Temp & 0x70) >> 4;
                switch(temp1)
                {
                case 0 :   
                    a = 0x10;       
                    break;
                case 1 :    
                    a = 0x40;       
                    break;
                case 2 :    
                    a = 0x100;      
                    break;
                case 3 :    
                    a = 0x400;      
                    break;
                case 4 :    
                    a = 0x1000;     
                    break;
                case 5 :    
                    a = 0x4000;     
                    break;
                case 6 :    
                    a = 0x10000; 
                    break;
                default:    
                    a = 0x40000; 
                }
                total_num = a * 146 * 8;
                qam_read_byte(u32TunerPort, FEC_BER_L, &temp2);
                qam_read_byte(u32TunerPort, FEC_BER_H, &temp1);	
                temp3 = (temp1 << 8) + temp2;
                ErrorRate = temp3 / total_num * 1000000000;
            }
            break;
        case 2:
            {	//DVBS2
                qam_read_byte(u32TunerPort, CR_PLS_CODE, &u8Temp);
                temp = ((u8Temp & 0x7c) >> 2);  //内码
                temp1 = ((u8Temp & 0x02) >> 1); //长短帧
                if(temp1 == 0)  	/*长 帧*/	
                {	
                    switch(temp)
                    {
                    case 1 :                          		
                        b = 16200; 
                        break;
                    case 2 : 
                        b = 21600;
                        break;
                    case 3 :  
                        b = 25920; 
                        break;
                    case 4 :  
                        b = 32400; 
                        break;
                    case 5 : 
                    case 12: 
                        b = 38880;
                        break;
                    case 6 :
                    case 13:
                    case 18:      
                        b = 43200; 
                        break;
                    case 7 : 
                    case 14:
                    case 19: 
                    case 24: 
                        b = 48600; 
                        break;
                    case 8 : 
                    case 20:
                    case 25:        
                        b = 51840; 
                        break;
                    case 9 :
                    case 15:
                    case 21: 
                    case 26:
                        b = 54000; 
                        break;
                    case 10:
                    case 16:
                    case 22: 
                    case 27: 
                        b = 57600; 
                        break;
                    case 11:
                    case 17:
                    case 23: 
                    case 28: 
                        b = 58320;
                        break;
                    default: 
                        b = 583200; 
                    }		
                }
                else  /*短帧*/
                {
                    switch (temp)
                    {
                    case 1 : 
                        b = 3240; 
                        break;
                    case 2 :         
                        b = 5400; 
                        break;
                    case 3 :         
                        b = 6480; 
                        break;
                    case 4 :     
                        b = 7200; 
                        break;
                    case 5 :
                    case 12:    
                        b = 9720; 
                        break;
                    case 6 :
                    case 13:
                    case 18:     
                        b = 10800; 
                        break;
                    case 7 :
                    case 14:
                    case 19:
                    case 24:   
                        b = 11880; 
                        break;
                    case 8 :
                    case 20:
                    case 25:  
                        b = 12600; 
                        break;
                    case 9 :
                    case 15:
                    case 21:
                    case 26:    
                        b = 13320; 
                        break;
                    case 10:
                    case 16:
                    case 22:
                    case 27:    
                        b = 14400; 
                        break;
                    default:
                        b = 14400; 
                    }
                }

                qam_read_byte(u32TunerPort, BER_CTRL, &u8Temp);						
                temp1 = (u8Temp & 0x70) >> 4;
                switch(temp1)
                {
                case 0 :    
                    a = 0x4;        
                    break;
                case 1 :    
                    a = 0x10;      
                    break;
                case 2 :    
                    a = 0x80;      
                    break;
                case 3 :    
                    a = 0x100;   
                    break;
                case 4 :    
                    a = 0x800;    
                    break;
                case 5 :    
                    a = 0x1000;  
                    break;
                case 6 :    
                    a = 0x8000;  
                    break;
                default:    
                    a = 0xFFFF;  
                    break;
                }
                total_num = a * b;	
                qam_read_byte(u32TunerPort, FEC_BER_L, &temp2);
		        qam_read_byte(u32TunerPort, FEC_BER_H, &temp1);
                temp3 = (temp1 << 8) + temp2;
                ErrorRate = temp3 / total_num * 1000000000;
                break;
            }
        }

        *pu32ber=ErrorRate;
    }

    return HI_SUCCESS;			//Return ErrorRate??  
}
#endif

static HI_BOOL hi3136_Set_BS_Mode(HI_U32 u32TunerPort, HI_BOOL bs)   //blind sacn mode
{
    HI_U8 temp1,temp2;
    HI_U8  u8Temp = 0;

    qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp);

    temp1 = u8Temp | 0x80;//defalult blind scan mode
    temp2 = u8Temp & 0x7f;

    if(bs == 1)
    {	
        qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, temp1);
    }
    else if(bs == 0)
    {
        qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, temp2);
    }

    return HI_SUCCESS;
}

/* 0 Power off, 1 power on */
HI_S32 hi3136_lnbctrl_power(HI_U32 u32TunerPort, HI_U8 u8Power)
{
    HI_U8 u8Temp ;

    if (u8Power)
    {
        qam_read_byte(u32TunerPort, DSEC_EN, &u8Temp);
        u8Temp |= 0x80;
        qam_write_byte(u32TunerPort, DSEC_EN, u8Temp);
    }
    else
    {
        qam_read_byte(u32TunerPort, DSEC_EN, &u8Temp);
        u8Temp &= 0x7f;
        qam_write_byte(u32TunerPort, DSEC_EN, u8Temp);
    }

    return HI_SUCCESS;
}

HI_S32 hi3136_set_lnb_out(HI_U32 u32TunerPort, TUNER_LNB_OUT_E enOut)
{
    switch (enOut)
    {
    case TUNER_LNB_OUT_0V:
    case TUNER_LNB_OUT_BUTT:
    default:    
        return HI_FAILURE;
        
    case TUNER_LNB_OUT_13V:
    case TUNER_LNB_OUT_14V:
        qam_write_bit(u32TunerPort, TX_CTRL, 0, 0);
        break;
        
    case TUNER_LNB_OUT_18V:
    case TUNER_LNB_OUT_19V:
        qam_write_bit(u32TunerPort, TX_CTRL, 0, 1);
        break;
    }

    return HI_SUCCESS;
}

HI_S32 hi3136_send_continuous_22K(HI_U32 u32TunerPort,HI_BOOL b22k_on) // 设置22k,
{	
    HI_U8	u8Temp;
    HI_U32	diseqc_Ratio;

    //diseqc_Ratio = (HI3136_SYSCLK_SAMPLE / 22) & 0xffff;
    diseqc_Ratio = (hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz / 22) & 0xffff;

    qam_write_byte(u32TunerPort, DSEC_RATIO_L, (HI_U8)(diseqc_Ratio & 0xff));
    qam_write_byte(u32TunerPort, DSEC_RATIO_H, (HI_U8)((diseqc_Ratio & 0xff00) >> 8));

    qam_read_byte(u32TunerPort, TX_CTRL, &u8Temp);
    u8Temp &= 0xff;
    if(b22k_on)
    {
        u8Temp &= 0xf1;
        u8Temp |= (1 << 1);
    }
    else
    {
        u8Temp &= ~(1 << 1);
    }
    
    msleep_interruptible(10);		//必要时可延长,延时多长，请确认
    qam_write_byte(u32TunerPort, TX_CTRL, u8Temp);

    qam_read_byte(u32TunerPort, TX_CTRL, &u8Temp);
    return HI_SUCCESS;
}

static HI_S32 hi3136_set_symrate(HI_U32 u32TunerPort, HI_U32 u32SymRate)  /*u32SymRate unit hz*/
{
    /*HI_U32 u32Temp;
    HI_U8  u8Temp = 0;	*/
    HI_S32 Center_shift_Value = 0;	

    if(u32SymRate == 0)    
        return HI_FAILURE;

    u32SymRate = (u32SymRate / 1000) & 0xffff;
    qam_write_byte(u32TunerPort, FS_L, (HI_U8)(u32SymRate & 0xff));
    qam_write_byte(u32TunerPort, FS_H, (HI_U8)((u32SymRate & 0xff00) >> 8));
    /*针对低符号率进行频率偏移*/
   	if(u32SymRate <= 5000)
    {
        Center_shift_Value = 5000;
    }

    if(hi3136_cfg[u32TunerPort].enIQSpectrum)				
    {
        Center_shift_Value = -Center_shift_Value;
    }

    Center_shift_Value &= 0xffff;
    qam_write_byte(u32TunerPort, CENT_FREQ_L, (HI_U8)(Center_shift_Value&0xff));  
    qam_write_byte(u32TunerPort, CENT_FREQ_H, (HI_U8)(Center_shift_Value>>8)&0xff);
    
	
    return HI_SUCCESS;
}


#ifndef BOOT_COMPILE
HI_S32 hi3136_get_signal_info(HI_U32 u32TunerPort, HI_UNF_TUNER_SIGNALINFO_S *pstInfo)
{
    HI_U8 	temp1, temp2, temp3, u8Temp, u8Temp1;
    HI_S16 rf_center_offset_KHz;
    //HI_S32 offset_cbs;
    HI_BOOL cbs_on = 0;
    HI_U32 u32SymbolRate_Khz = 0;

    pstInfo->enSigType = HI_UNF_TUNER_SIG_TYPE_SAT;
    qam_read_byte(u32TunerPort, TX_CTRL, &u8Temp);

    if (hi3136_Read_Lock_Status(u32TunerPort) == FEC_LOCKED)
    {
        qam_read_byte(u32TunerPort, SEAR_RESULT, &u8Temp);
        temp1 = u8Temp & 0x01;
        qam_read_byte(u32TunerPort, LOCK_FLAG, &u8Temp1);
        if(u8Temp1 & 0x08)
        {
            temp1 = 2;    //sync_ok 指示S2 signal
        }
        switch(temp1)
        {
        case 0:
            {
                pstInfo->unSignalInfo.stSat.enSATType = HI_UNF_TUNER_FE_DVBS;
                pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;

                temp2 = ((u8Temp & 0x0e) >> 1);
                switch(temp2)
                {
                case 0: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                        break;
                    }
                case 1: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    }
                case 2:
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    }
                case 3:
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    }
                case 4: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_7_8;
                        break;
                    }
                default: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT;
                        break;
                    }
                }
                break;
            }

        case 1:
            {
                pstInfo->unSignalInfo.stSat.enSATType = HI_UNF_TUNER_FE_DIRECTV;
                pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;

                temp2 = ((u8Temp & 0x0e) >> 1);
                switch(temp2)
                {
                case 0: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                        break;
                    }
                case 1: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    }
                case 5: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_6_7;
                        break;
                    }
                default: 
                    {
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT;
                        break;
                    }
                }
                break;
            }

        case 2:
            {
                pstInfo->unSignalInfo.stSat.enSATType = HI_UNF_TUNER_FE_DVBS2;

                qam_read_byte(u32TunerPort, CR_PLS_CODE, &u8Temp);	//???
                temp3 = ((u8Temp & 0x7c) >> 2);
                switch (temp3)
                {
                case 1: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_1_4;
                        break;
                    }
                case 2: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_1_3;
                        break;
                    }
                case 3: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_5;
                        break;
                    }
                case 4: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_1_2;
                        break;
                    }
                case 5: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_5;
                        break;
                    }
                case 6:
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    }
                case 7: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    }
                case 8: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
                        break;
                    }
                case 9: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    }
                case 10: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_8_9;
                        break;
                    }
                case 11: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_QPSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_9_10;
                        break;
                    }
                case 12: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_5;
                        break;
                    }
                case 13: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    }
                case 14: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    }
                case 15: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    }
                case 16: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_8_9;
                        break;
                    }
                case 17: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_8PSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_9_10;
                        break;
                    }
                case 18: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_2_3;
                        break;
                    }
                case 19: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    }
                case 20: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
                        break;
                    }
                case 21: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    }
                case 22: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_8_9;
                        break;
                    }
                case 23: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_16APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_9_10;
                        break;
                    }
                case 24: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_3_4;
                        break;
                    }
                case 25: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_4_5;
                        break;
                    }
                case 26: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_5_6;
                        break;
                    }
                case 27: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_8_9;
                        break;
                    }
                case 28: 	
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_32APSK;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FEC_9_10;
                        break;
                    }
                default: 		
                    {
                        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_BUTT;
                        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT;
                        break;
                    }
                }
                break;
            }

        default:
            {
                pstInfo->unSignalInfo.stSat.enSATType = HI_UNF_TUNER_FE_BUTT;
                pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_BUTT;
                pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT;
            }
        }			
    }
    else
    {
        pstInfo->unSignalInfo.stSat.enSATType = HI_UNF_TUNER_FE_BUTT;
        pstInfo->unSignalInfo.stSat.enModType = HI_UNF_MOD_TYPE_BUTT;
        pstInfo->unSignalInfo.stSat.enFECRate = HI_UNF_TUNER_FE_FECRATE_BUTT;
    }

    qam_read_byte(u32TunerPort, CENT_FREQ_L, &temp1);
    qam_read_byte(u32TunerPort, CENT_FREQ_H, &temp2);

#if 0
    if(((temp2 << 8) + temp1) <= 32767)
    {
        rf_center_offset_KHz = (temp2 << 8) + temp1;
    }
    else if(((temp2 << 8) + temp1) > 32767)
    {
        rf_center_offset_KHz = ((temp2 << 8) + temp1) - 65536;    /*有符号数*/
    }
#endif
    rf_center_offset_KHz = (HI_S16)((temp2 << 8) + temp1);

    if (hi3136_cfg[u32TunerPort].enIQSpectrum)
    {
        rf_center_offset_KHz = -rf_center_offset_KHz;
    }

    qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp);
    cbs_on = (HI_BOOL)((u8Temp & 0x80) >> 7);

    if (hi3136_Read_Lock_Status(u32TunerPort) == FEC_LOCKED && cbs_on == 1)  /*作预留调试*/
    {
        qam_read_byte(u32TunerPort, CBS_FS_L, &temp2);
        qam_read_byte(u32TunerPort, CBS_FS_H, &temp1);
        u32SymbolRate_Khz = (temp1 << 8) + temp2;
        //tp_info.u32Freq_info_KHz = (HI_S32)u32Freq_Khz + rf_center_offset_KHz;
        //tp_info.u32SymolRate_info_Kbps = u32SymbolRate_Khz;
    }

    else
    {
        //tp_info.u32Freq_info_KHz = u32Freq_Khz;
        //tp_info.u32SymolRate_info_Kbps = u32SymbolRate_Khz;
    }

    //return tp_info;
    return HI_SUCCESS;
}
#endif

#if 0
static HI_VOID Hi3136_change_iterNum(HI_U32 u32TunerPort)
{	
    HI_U8 s2_sync_bit = 0, temp1 = 0, temp2 = 0, temp3 = 0, u8Temp = 0, ldpc_iter_num = 0;
    HI_U32 fs_cur = 0;
    HI_U16 ldpc_iter_lmt = 0;
    HI_U32 fec_clk = 0, fvco = 0, Fcrystal = 0;
    HI_U8 fec_freq_l = 0, fec_freq_m = 0, fec_freq_h = 0, M;

    //printk("%s, %d\n", __func__, __LINE__);
    qam_read_bit(u32TunerPort, LOCK_FLAG, 3, &s2_sync_bit);
    
    if(s2_sync_bit)     //当前是否S2模式判定
    {
        qam_read_byte(u32TunerPort, 0x26, &fec_freq_l);
        qam_read_byte(u32TunerPort, 0x27, &fec_freq_m);
        qam_read_byte(u32TunerPort, 0x28, &fec_freq_h);
        
        fec_clk = (fec_freq_h & 0x03) << 16 + fec_freq_m << 8 + fec_freq_l;	//unit Khz
        
        qam_read_byte(u32TunerPort, FS_L, &temp1);
        qam_read_byte(u32TunerPort, FS_H, &temp2);
        fs_cur = temp1 + temp2<<8;
        
        qam_read_byte(u32TunerPort, CR_PLS_CODE, &u8Temp);	
        temp3 = ((u8Temp & 0x7c) >> 2);
        if(temp3 > 0 && temp3 <= 11)
        {
            M=2;
        }
        else if((temp3 > 11) && (temp3 <= 17))
        {
            M=3;
        }
        else if((temp3 > 17) && (temp3 <= 23))
        {
            M=4;
        }
        else
        {
            M=5;
        }
        
        ldpc_iter_lmt =  (HI_U16)(64800 * fec_clk / (45000 * 3 * 4032)) - 1;  //取整
        if(ldpc_iter_lmt > LDPC_ITER_45M)
        {
            ldpc_iter_lmt = LDPC_ITER_45M;
        }
        
        ldpc_iter_num = (HI_U16) ((45000 / fs_cur) * ldpc_iter_lmt / M * 3);	//四舍五入 								   
        
        if(ldpc_iter_num > LDPC_ITER_MAX)
        {
            ldpc_iter_num = LDPC_ITER_MAX;
        }

        //printk("%s, %d\n", __func__, __LINE__);
        qam_write_byte(u32TunerPort, LDPC_ITER_ADDR, ldpc_iter_num | 0xc0);             
    }
}
#endif

HI_S32 hi3136_lock_TP(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *tp)
{	
    //printk("%s, %d\n", __func__, __LINE__);
    HI_U8 temp, temp1/*, u8Temp*/;
    HI_S16 center_shift_KHz = 0;
    HI_S32 s32FuncRet = HI_SUCCESS;

    hi3136_Set_BS_Mode(u32TunerPort, 0); //关闭盲扫功能

    //hi3136_set_polar(u32TunerPort,tp->enPolar); 
    //hi3136_set_lnb_22k(u32TunerPort,tp->_22k_on);	//是否需要保留?		

    hi3136_set_symrate(u32TunerPort, tp->unSRBW.u32SymbolRate);
    qam_read_byte(u32TunerPort, CENT_FREQ_L, &temp1);
    qam_read_byte(u32TunerPort, CENT_FREQ_H, &temp);

    center_shift_KHz = (HI_S16)((temp << 8) + temp1);
    
    if(hi3136_cfg[u32TunerPort].enIQSpectrum)
    {
        center_shift_KHz = -center_shift_KHz;
    }

    //printk("%s, %d, 0x%x, %d\n", __func__, __LINE__, center_shift_KHz, tp->u32Frequency);

    hi3136_setlpf(u32TunerPort, tp->unSRBW.u32SymbolRate + abs(center_shift_KHz));
    if (g_stTunerOps[u32TunerPort].set_tuner)
    {
        s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, tp->u32Frequency -center_shift_KHz + 500);
        if (HI_SUCCESS != s32FuncRet)
        {
            HI_ERR_TUNER( "set tuner error, s32FuncRet = %d\n", s32FuncRet);
            return s32FuncRet;
        }
    }

    //	center_shift_KHz=temp1+(temp<<8);
    //	if(tp->bSI==1)	center_shift_KHz=0-center_shift_KHz;  	/*具体根据实际硬件配置*/
    //打开IIC转发通道
    //hi3136_config_i2c_out(u32TunerPort, 1);
    //av2011_set_tuner(u32TunerPort, enI2cChannel,(tp.u32Frequency+center_shift_KHz+500)/1000,tp.u32SymbolRate+abs(center_shift_KHz));

    hi3136_HotReset_CHIP(u32TunerPort);
    msleep_interruptible(20);

    return HI_SUCCESS;
}

HI_S32 hi3136_lock_TP_BS(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *tp)
{
    HI_U8 temp, temp1, temp2, u8Temp, u8Temp1, fs_grade;
    HI_U8 /*may_sfu, may_no_sig, fec_ok_flag, signal_type, u8cbs_reliablity,*/ cbs_reliablity_temp[32] = {0};
    HI_U8 j, k, /*n,*/ temp_TPnum = 0, cbs_temp_num, max_cbs_reliablity, u8tempReliablity;
    //HI_U8 tim_wait_tmp;
    HI_U8 fec_ok_bit = 0;
    HI_U16 /*fec_ok_cnt, fec_no_ok_cnt,*/ tim_cnt, u16TimeOut = 0, fec_wait_tmp;
    HI_U32 fs_cur, u32tempFreq, u32tempSymb;
    HI_S32 center_shift_KHz;
    HI_UNF_TUNER_SAT_TPINFO_S tempChannel[32];// = {0};
    HI_S32 s32Freqoffset = 0;
    HI_S32 s32FuncRet = HI_SUCCESS;
    
    hi3136_Set_BS_Mode(u32TunerPort, 1); 
    qam_write_bit(u32TunerPort, CBS_CTRL_RDADDR, 6, 1);
    qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp);
    u8Temp &= 0xe0;
    qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp);

    qam_write_byte(u32TunerPort, LDPC_ITER_ADDR, 0x50);
    
    hi3136_set_symrate(u32TunerPort, tp->unSRBW.u32SymbolRate);
    qam_read_byte(u32TunerPort, CENT_FREQ_L, &temp1);
    qam_read_byte(u32TunerPort, CENT_FREQ_H, &temp);
    center_shift_KHz = (HI_S16)((temp << 8) + temp1);
    
    if(hi3136_cfg[u32TunerPort].enIQSpectrum)
    {
        center_shift_KHz = -center_shift_KHz;
    }
    
    fs_cur = tp->unSRBW.u32SymbolRate;
    
    //fs_grade select
    if(fs_cur < 2000000)   	//unit Hz
    {
        fs_grade = 0;
    }
    else if(fs_cur < 3000000)
    {
        fs_grade = 1;
    }
    else if(fs_cur < 4900000)
    {
        fs_grade = 2;
    }
    else if(fs_cur < 8000000)
    {
        fs_grade = 3;
    }
    else if(fs_cur < 15000000)
    {
        fs_grade = 4;
    }
    else
    {
        fs_grade = 5;
    }
    
    hi3136_setlpf(u32TunerPort, tp->unSRBW.u32SymbolRate + abs(center_shift_KHz));
    if (g_stTunerOps[u32TunerPort].set_tuner)
    {
        s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, tp->u32Frequency - center_shift_KHz + 500);
        if (HI_SUCCESS != s32FuncRet)
        {
            HI_ERR_TUNER( "set tuner error\n");
            return s32FuncRet;
        }
    }
    
    hi3136_HotReset_CHIP(u32TunerPort);
    msleep_interruptible(20);
    //以上ok

    u8Temp = 0;
    u16TimeOut = 0;
    do
    {
        msleep_interruptible(5);
        qam_read_bit(u32TunerPort, CBS_FINISH, 0, &u8Temp);
        if(u8Temp)  //cbs_finish 标志位1表示已完成
        {
            qam_read_byte(u32TunerPort, CBS_R2_NUM, &temp1);
            temp1 &= 0x1f;
            temp_TPnum = temp1;
            break;
        }
        u16TimeOut += 5;
    }while((!u8Temp) && (u16TimeOut < 200) );		//等待读取CBS_FINISH 操作
    
    if(temp_TPnum > 0)
    {			
        for(j = 0; j < temp_TPnum; j++)		//已知Fs Fc情况下，盲扫信息参数
        {
            qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp1);
            //printk("%s, %d, u8Temp1=%x\n", __func__, __LINE__,  u8Temp1);
            u8Temp1 &= 0xe0;
            u8Temp1 |= j;
            //printk("%s, %d, u8Temp1=%x,j=%d\n", __func__, __LINE__,  u8Temp1,j);
            qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp1);
            
            qam_read_byte(u32TunerPort, CBS_FC_L, &temp1);
            qam_read_byte(u32TunerPort, CBS_FC_H, &temp2);
            
            tempChannel[j].u32Freq = temp1 + (temp2<<8);
            if(tempChannel[j].u32Freq > 32767)
            {
                tempChannel[j].u32Freq = tempChannel[j].u32Freq - 65536; 
            }

            qam_read_byte(u32TunerPort, CBS_FS_L, &temp1);
            qam_read_byte(u32TunerPort, CBS_FS_H, &temp2);
            
            tempChannel[j].u32SymbolRate = ((temp2 << 8) + temp1) * 1000;
            
            qam_read_byte(u32TunerPort, CBS_RELIABLITY, &temp1);
            tempChannel[j].cbs_reliablity = temp1;
            cbs_reliablity_temp[j] = ((HI_S32)(tempChannel[j].u32Freq)) / 64 + tempChannel[j].cbs_reliablity;
        }
        
        cbs_temp_num = temp_TPnum;
        max_cbs_reliablity = cbs_reliablity_temp[0];
        k = 0;
        for(j = 1; j < cbs_temp_num; j++)
        {
            if(cbs_reliablity_temp[j] > max_cbs_reliablity)
            {	
                max_cbs_reliablity = tempChannel[j].cbs_reliablity;
                k = j;
            }
        }
        u32tempFreq = tp->u32Frequency;
        u32tempSymb = tempChannel[k].u32SymbolRate;
        u8tempReliablity = tempChannel[k].cbs_reliablity;
        s32Freqoffset = (HI_S32)(tempChannel[k].u32Freq);

        if(cbs_temp_num == 1)
        {           
            fec_wait_tmp = (320 - fs_grade * 2) * 2 + 100;	//延时较大fs_grade意义不大????
            //printk("%s, %d,fec_wait_tmp=%d,fs_grade=%d\n", __func__, __LINE__,  fec_wait_tmp,fs_grade);	

            tim_cnt = 0;
            do
            {
                msleep_interruptible(5);
                qam_read_bit(u32TunerPort, LOCK_FLAG, 5, &fec_ok_bit);
                if(fec_ok_bit)  //cbs_finish 标志位1表示已完成
                {
                    //Hi3136_change_iterNum(u32TunerPort);
                    return HI_SUCCESS;
                }
                tim_cnt += 5;
            } while((!fec_ok_bit) && (tim_cnt< fec_wait_tmp));	
            //printk("%s, %d,fec_ok_bit=%d,tim_cnt=%d\n", __func__, __LINE__,  fec_ok_bit,tim_cnt);			
        }
        else
        {
            qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, 0x40);	//关闭盲扫，Ram地址为0
            qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp1);
            //printk("%s, %d, u8Temp1=%x\n", __func__, __LINE__,  u8Temp1);

            hi3136_setlpf(u32TunerPort, (u32tempSymb + abs(center_shift_KHz)));
            s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, (u32tempFreq + s32Freqoffset - center_shift_KHz + 500));
            if (HI_SUCCESS != s32FuncRet)
            {
                HI_ERR_TUNER( "set tuner error\n");
                return s32FuncRet;
            }

            hi3136_set_symrate(u32TunerPort, u32tempSymb);
            //printk("%s, %d,u32tempSymb=%d\n", __func__, __LINE__,  u32tempSymb);
            hi3136_HotReset_CHIP(u32TunerPort);
            
            fec_wait_tmp = (320 - fs_grade * 2) * 2 + 400;
            //printk("%s, %d,fec_wait_tmp=%d,fs_grade=%d\n", __func__, __LINE__,  fec_wait_tmp,fs_grade);

            tim_cnt = 0;
            do
            {
                msleep_interruptible(5);
                qam_read_bit(u32TunerPort, LOCK_FLAG, 5, &fec_ok_bit);
                if(fec_ok_bit)  //cbs_finish 标志位1表示已完成
                {
                    //printk("%s, %d,  %d\n", __func__, __LINE__, fec_ok_bit);
                    //Hi3136_change_iterNum(u32TunerPort);
                    return HI_SUCCESS;
                }
                tim_cnt += 5;
            } while((!fec_ok_bit) && (tim_cnt < fec_wait_tmp));	
            //printk("%s, %d,fec_ok_bit=%d,tim_cnt=%d\n", __func__, __LINE__,  fec_ok_bit,tim_cnt);
        }
    }
    
    return HI_SUCCESS;
}

#ifndef BOOT_COMPILE
HI_U32 hi3136_BS_one_Window(HI_U32 u32TunerPort, HI_U32 Fcenter/*MHz*/, HI_U32 Lpf_BW_window/*kHz*/, HI_UNF_TUNER_SAT_TPINFO_S *pstChannel)
{
    //printk("%s, %d, Fcenter = %d\n", __func__, __LINE__, Fcenter);
    HI_U32 	TP_Num = 0;	
    //HI_U32 	agc_temp;
    HI_U8 	i;
    HI_U8	temp, temp1, u8Temp, u8Temp1, u8Temp2, cbs_reliablity_temp;	
    HI_S32	Fc_offset_Bs; 
    HI_S32 s32FuncRet = HI_SUCCESS;
    //HI_S32 center_shift_KHz;
    HI_U32	Fs_Bs;
    HI_U16 u16TimeOut = 0;

    hi3136_Set_BS_Mode(u32TunerPort, 1);
	qam_read_byte(u32TunerPort,CBS_CTRL_RDADDR, &u8Temp);
	u8Temp &= 0xa0;
	qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp);

    hi3136_set_symrate(u32TunerPort, Lpf_BW_window * 1000);

    qam_write_byte(u32TunerPort, CBS_FC_L, 0x00);
    qam_write_byte(u32TunerPort, CBS_FC_H, 0x00);	/*BB_CENT 偏移置0*/

    hi3136_setlpf(u32TunerPort, Lpf_BW_window * 1000);
    if (g_stTunerOps[u32TunerPort].set_tuner)
    {
        s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, Fcenter * 1000);
        if (HI_SUCCESS != s32FuncRet)
        {
            HI_ERR_TUNER( "set tuner error\n");
            return s32FuncRet;
        }
    }
    //av2011_set_tuner(u32TunerPort, enI2cChannel,Fcenter,Lpf_BW_window);  //symbolRate unit :KHz
    msleep_interruptible(20);
    hi3136_HotReset_CHIP(u32TunerPort);
    
    do
    {
        msleep_interruptible(5);
        qam_read_bit(u32TunerPort, CBS_FINISH, 0, &u8Temp);
        if(u8Temp)  //cbs_finish 标志位1表示已完成
        {
            qam_read_byte(u32TunerPort, CBS_R2_NUM, &u8Temp1);
            TP_Num = u8Temp1 & 0x1f;
            break;
        }
        u16TimeOut += 5;
    } while((!u8Temp) && (u16TimeOut < 200));		//等待读取CBS_FINISH 操作

    if(TP_Num != 0)
    {
        for(i = 0; i < TP_Num; i++)
        {
            qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp2);
            u8Temp2 &= 0xe0;
            u8Temp2 |= i;

            qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp2);
            qam_read_byte(u32TunerPort, CBS_FC_H, &temp);
            qam_read_byte(u32TunerPort, CBS_FC_L, &temp1);
            Fc_offset_Bs = (temp1 | (temp << 8));
            
            if(Fc_offset_Bs > 32767)
            {
                Fc_offset_Bs = Fc_offset_Bs - 65536; 
            }
            if(hi3136_cfg[u32TunerPort].enIQSpectrum)				
            {
                Fc_offset_Bs = -Fc_offset_Bs;
            }

            pstChannel->u32Freq = (HI_S32)(Fcenter * 1000) + Fc_offset_Bs;//Fcenter 频率 是否需要强制为有符号数unit:KHZ

            qam_read_byte(u32TunerPort, CBS_FS_L, &temp1);
            qam_read_byte(u32TunerPort, CBS_FS_H, &temp);
            Fs_Bs = (temp << 8) + temp1;
            pstChannel->u32SymbolRate = Fs_Bs * 1000;//符号率 unit  KHZ

            //pstChannel->agc_h8 = agc_temp;	//agc高8bit控制字目前将同一频点搜索的TP作相等处理

            qam_read_byte(u32TunerPort, CBS_RELIABLITY, &cbs_reliablity_temp);
            pstChannel->cbs_reliablity = cbs_reliablity_temp;	//cbs 结果可靠度，越大越可靠
            //printk("%s, %d, Fcenter = %d\n", __func__, __LINE__, Fcenter);

            //printk("%s, %d, Fc_offset_Bs = %d\n", __func__, __LINE__, Fc_offset_Bs);

            pstChannel++;
        }
    }
    else
    {
        return 0;
    }

    return TP_Num;
}

HI_S32 hi3136_set_funcmode(HI_U32 u32TunerPort, TUNER_FUNC_MODE_E enFuncMode)
{
    return HI_SUCCESS;
}

HI_S32 hi3136_blindscan_init(HI_U32 u32TunerPort, TUNER_BLINDSCAN_INITPARA_S *pstPara)
{
    HI_ASSERT(HI_NULL != pstPara);

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
HI_S32 hi3136_blindscan_action(HI_U32 u32TunerPort, TUNER_BLINDSCAN_PARA_S *pstPara)
{
    //HI_U32 TP_Num = 0;
    //HI_U32 Fcenter;	//MHZ
    //HI_U32 Bs_window_num = 0;
    //HI_U32 TP_Num_total = 0;
    //HI_U32 i;
    HI_U8 u8Temp;
    //HI_U32 Start_Freq, End_Freq;
    //HI3136_TP_INFO_S stTPInfo[32];

    pstPara->u16Count = hi3136_BS_one_Window(u32TunerPort, pstPara->u32CentreFreq, HI3136_BS_WINDOW_STEP_KHZ, pstPara->unResult.astSat);

    qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp);
    u8Temp &= 0xe0;
    qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp);//将cbs_rd_addr 清零

    return HI_SUCCESS;
}
#endif

static HI_S32 hi3136_setlpf(HI_U32 u32TunerPort, HI_U32 u32SymbolRate/*unit BAUD ,neither K, nor M*/)
{
    u32LPF_KHz[u32TunerPort] = (u32SymbolRate * 75 / 10000000 + 40) * 100;

    if (u32LPF_KHz[u32TunerPort] >= hi3136_cfg[u32TunerPort].u16TunerLPF_100Khz * 100)
    {
        u32LPF_KHz[u32TunerPort] = hi3136_cfg[u32TunerPort].u16TunerLPF_100Khz * 100;
    }
    return HI_SUCCESS;
}

HI_S32 hi3136_init(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_UNF_TUNER_DEV_TYPE_E enTunerDevType)
{
    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3136 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        qam_write_byte(u32TunerPort, ADC_CTRL1, 0x7a);//adc power down
        qam_write_byte(u32TunerPort, ADC_CTRL1, 0x4a);//adc power on
    }

    //将TS_CLK生成固定的时钟
    qam_write_byte(u32TunerPort, TS_CTRL4, 0x8);
    qam_write_byte(u32TunerPort, TS_CTRL0, 0x12);

    /* Init tuner fuctions */
    switch (enTunerDevType)
    {
#if defined(TUNER_DEV_TYPE_AV2011)    
    case HI_UNF_TUNER_DEV_TYPE_AV2011:
        av2011_init_tuner(u32TunerPort);
        break;
#endif

#if defined(TUNER_DEV_TYPE_SHARP7903)
    case HI_UNF_TUNER_DEV_TYPE_SHARP7903:
        sharp7903_init_tuner(u32TunerPort);
        break;
#endif

#if defined(TUNER_DEV_TYPE_M88TS2022)
    case HI_UNF_TUNER_DEV_TYPE_M88TS2022:
        M88TS2022_init_tuner(u32TunerPort);
        break;
#endif

#if defined(TUNER_DEV_TYPE_RDA5815)
    case HI_UNF_TUNER_DEV_TYPE_RDA5815:
        RDA5815_init_tuner(u32TunerPort);
        break;
#endif

    default:
        HI_ERR_TUNER("Tuner type not supported!\n");
        return HI_FAILURE;
    }

    qam_write_byte(u32TunerPort, 0x22, 0x1e);      //reset waiting time 
    qam_write_byte(u32TunerPort, 0x8c, 0x85 | (0<<5) | (1<<3));      //fec not search direcTV, 
    qam_write_byte(u32TunerPort, 0x8d, 0x5e);      //fec not search direcTV,     
    qam_write_byte(u32TunerPort, 0xa1, 0x05);

    return HI_SUCCESS;
}

HI_S32 hi3136_set_sat_attr(HI_U32 u32TunerPort, HI_UNF_TUNER_SAT_ATTR_S *pstSatTunerAttr)
{
    HI_UNF_TUNER_SAT_ATTR_S* pstAttr;

    HI_ASSERT(HI_NULL != pstSatTunerAttr);

    if (u32TunerPort >= TUNER_NUM)
    {
        return HI_FAILURE;
    }

    pstAttr = (HI_UNF_TUNER_SAT_ATTR_S*)pstSatTunerAttr;

    hi3136_cfg[u32TunerPort].u16TunerLPF_100Khz = pstAttr->u16TunerMaxLPF * 10;
    hi3136_cfg[u32TunerPort].enIQSpectrum = pstAttr->enIQSpectrum;
    
    //修改agc_inverse
    switch (pstAttr->enRFAGC)
    {
    case HI_UNF_TUNER_RFAGC_INVERT:
    default:
        qam_write_bit(u32TunerPort, AGC_CTRL, 1, 1);
        break;
    case HI_UNF_TUNER_RFAGC_NORMAL:
        qam_write_bit(u32TunerPort, AGC_CTRL, 1, 0);
        break;
    }

    if(HI_UNF_DEMOD_DEV_TYPE_3136 == g_stTunerOps[u32TunerPort].enDemodDevType)
    {
        switch (pstAttr->u32DemodClk)
        {
        case 13500:
            //printk("%s, %d\n", __func__, __LINE__);
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 1);
            //set PLL1
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL3, 0x37);
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 0);
            //set PLL2
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL8, 0x52);
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 0);
            msleep_interruptible(5);
            qam_write_byte(u32TunerPort, PLL_CTRL5, 0x4);
            qam_write_byte(u32TunerPort, PLL_CTRL10, 0xe);
    
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 0);
    
            qam_write_byte(u32TunerPort, CLK_DEMO_L, 0x98);
            qam_write_byte(u32TunerPort, CLK_DEMO_M, 0xe5);
            qam_write_byte(u32TunerPort, CLK_DEMO_H, 0x1);
            hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = 124312;
    
            qam_write_byte(u32TunerPort, CLK_FEC_L, 0x65);
            qam_write_byte(u32TunerPort, CLK_FEC_M, 0xd8);
            qam_write_byte(u32TunerPort, CLK_FEC_H, 0x2);
            break;
        case 16000:
            //printk("%s, %d\n", __func__, __LINE__);
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 1);
            //set PLL1
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL3, 0x2e);
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 0);
            //set PLL2
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL8, 0x45);
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 0);
            msleep_interruptible(5);
            qam_write_byte(u32TunerPort, PLL_CTRL5, 0x4);
            qam_write_byte(u32TunerPort, PLL_CTRL10, 0xe);
    
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 0);
    
            qam_write_byte(u32TunerPort, CLK_DEMO_L, 0xc5);
            qam_write_byte(u32TunerPort, CLK_DEMO_M, 0xe1);
            qam_write_byte(u32TunerPort, CLK_DEMO_H, 0x1);
            hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = 123333;
    
            qam_write_byte(u32TunerPort, CLK_FEC_L, 0xdd);
            qam_write_byte(u32TunerPort, CLK_FEC_M, 0xd7);
            qam_write_byte(u32TunerPort, CLK_FEC_H, 0x2);
            break;
        case 24000:
	        //printk("%s, %d\n", __func__, __LINE__);
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 1);
            //set PLL1
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL3, 0x1f);
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 0);
            //set PLL2
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL8, 0x2e);
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 0);
            msleep_interruptible(5);
            qam_write_byte(u32TunerPort, PLL_CTRL5, 0x4);
            qam_write_byte(u32TunerPort, PLL_CTRL10, 0xe);
    
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 0);
    
            qam_write_byte(u32TunerPort, CLK_DEMO_L, 0x48);
            qam_write_byte(u32TunerPort, CLK_DEMO_M, 0xe8);
            qam_write_byte(u32TunerPort, CLK_DEMO_H, 0x1);
            hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = 125000;
    
            qam_write_byte(u32TunerPort, CLK_FEC_L, 0x6c);
            qam_write_byte(u32TunerPort, CLK_FEC_M, 0xdc);
            qam_write_byte(u32TunerPort, CLK_FEC_H, 0x2);
            break;
        case 27000:
            //printk("%s, %d\n", __func__, __LINE__);
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 1);
            //set PLL1
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL3, 0x1b);
            qam_write_bit(u32TunerPort, PLL_CTRL1, 7, 0);
            //set PLL2
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 1);
            qam_write_byte(u32TunerPort, PLL_CTRL8, 0x28);
            qam_write_bit(u32TunerPort, PLL_CTRL5, 7, 0);
            msleep_interruptible(5);
            qam_write_byte(u32TunerPort, PLL_CTRL5, 0x4);
            qam_write_byte(u32TunerPort, PLL_CTRL10, 0xe);
    
            qam_write_bit(u32TunerPort, ADC_CTRL2, 4, 0);
    
            qam_write_byte(u32TunerPort, CLK_DEMO_L, 0x1);
            qam_write_byte(u32TunerPort, CLK_DEMO_M, 0xdf);
            qam_write_byte(u32TunerPort, CLK_DEMO_H, 0x1);
            hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = 122625;
    
            qam_write_byte(u32TunerPort, CLK_FEC_L, 0x81);
            qam_write_byte(u32TunerPort, CLK_FEC_M, 0xce);
            qam_write_byte(u32TunerPort, CLK_FEC_H, 0x2);
            break;
        }
    }
    else
    {
        hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz = 125000;
    }

    return HI_SUCCESS;
}

HI_S32 hi3136_connect(HI_U32 u32TunerPort, TUNER_ACC_QAM_PARAMS_S *pstChannel)
{
    HI_S32 s32FuncRet = HI_SUCCESS;
    //HI_U32 u32SymbolRate;
    //HI_U8 u8Temp;

    HI_ASSERT(HI_NULL != pstChannel);

    /*step 1:config tuner register*/

    /*if(pstChannel->use_cbs==1)
    {
    s32FuncRet=hi3136_lock_TP_BS(u32TunerPort, pstChannel);	
    }
    else
    {
    s32FuncRet=hi3136_lock_TP(u32TunerPort, pstChannel);
    }
    */

    s32FuncRet = hi3136_lock_TP_BS(u32TunerPort, pstChannel); //改成开邋BS
    if (HI_SUCCESS != s32FuncRet)
    {
        HI_ERR_TUNER( "set tuner error\n");
        return s32FuncRet;
    }
    
    /*CBS关闭信道锁定情况下，利用CBS打开进行锁TP*/	
    /*qam_read_byte(u32TunerPort, LOCK_FLAG, &u8Temp);
    if( !(u8Temp & (1 << 5)) )  
    {
        s32FuncRet = hi3136_lock_TP_BS(u32TunerPort, pstChannel);	
    }*/

    /*step 2:config hi3136 register */

    /*set for different usr request about symbolrate, ts type, demo in or out,  inversion or not*/
    // hi3136_set_symrate(u32TunerPort, pstChannel->u32SymbolRate); /*配置符号率,在lock_tp里已有设定符号率*/	
    hi3136_config_IQ_swap(u32TunerPort, hi3136_cfg[u32TunerPort].enIQSpectrum); 	/*配置IQ*/
    /*配置AD*/
    /*step 3:reset chip*/

    hi3136_set_ts_type(u32TunerPort, g_stTunerOps[u32TunerPort].enTsType);
    
    return HI_SUCCESS;
}

HI_S32 hi3136_get_status (HI_U32 u32TunerPort, HI_UNF_TUNER_LOCK_STATUS_E  *penTunerStatus)
{
    //HI_U8  u8Status = 0;
    //HI_S32 s32Ret = HI_SUCCESS;
    //HI_S32 i = 0;

    HI_ASSERT(HI_NULL != penTunerStatus);

    if (hi3136_Read_Lock_Status(u32TunerPort) == FEC_LOCKED)
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_LOCKED;
    }
    else
    {
        *penTunerStatus = HI_UNF_TUNER_SIGNAL_DROPPED;
    }

    return HI_SUCCESS;
}

HI_S32 hi3136_set_ts_type(HI_U32 u32TunerPort, HI_UNF_TUNER_OUPUT_MODE_E enTsType)
{
    HI_U8 u8Temp;
    
    /*ts type control*/
    switch (enTsType)
    {
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_A:
    case HI_UNF_TUNER_OUTPUT_MODE_PARALLEL_MODE_B:
        {
            qam_read_byte(u32TunerPort, TS_PARALL_CTRL, &u8Temp);
            u8Temp = u8Temp & 0xe0;
            u8Temp = u8Temp | (1 << 5) | 0x0a;
            qam_write_byte(u32TunerPort, TS_PARALL_CTRL, u8Temp);/*config ts_out_10bit*/
            //qam_write_byte(u32TunerPort, TS_OUT98_SEL, 0x98);/*config ts_out_9&8bit*/
            //qam_write_byte(u32TunerPort, TS_OUT76_SEL, 0x76 );/*config ts_out_7&6bit*/
            //qam_write_byte(u32TunerPort, TS_OUT54_SEL, 0x54 );/*config ts_out_5&4bit*/
            //qam_write_byte(u32TunerPort, TS_OUT32_SEL, 0x32 );/*config ts_out_3&2bit*/
            //qam_write_byte(u32TunerPort, TS_OUT10_SEL, 0x10 );/*config ts_out_1&0bit*/
            //qam_write_bit(u32TunerPort,  TS_CTRL0, 1, 1);
            break;
        }
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL:
        {
            qam_write_bit(u32TunerPort, TS_PARALL_CTRL, 5, 0);/*SERIAL MODE*/ 
            //qam_write_byte(u32TunerPort,TS_OUT98_SEL, 0x0f);/*config ts_out_9&8bit*/
            //qam_write_byte(u32TunerPort,TS_OUT76_SEL,0x76 );/*config ts_out_7&6bit   在串行配置具体可调整*/
            //qam_write_bit(u32TunerPort, TS_CTRL0, 1, 1);
            break;
        }
    case HI_UNF_TUNER_OUTPUT_MODE_SERIAL_2BIT:
        {
            qam_read_byte(u32TunerPort, TS_PARALL_CTRL, &u8Temp);
            u8Temp = u8Temp | (1 << 5) | (1 << 4);
            qam_write_byte(u32TunerPort, TS_PARALL_CTRL, u8Temp);
            //qam_write_byte(u32TunerPort,TS_OUT98_SEL, 0x01);/*config ts_out_9&8bit*/
            //qam_write_byte(u32TunerPort,TS_OUT76_SEL,0x76 );/*config ts_out_7&6bit   在串行配置具体可调整*/
            //qam_write_bit(u32TunerPort, TS_CTRL0, 1, 1);
            break;
        }
    default:
        break;
    }

    g_stTunerOps[u32TunerPort].enTsType = enTsType;

    return HI_SUCCESS;
}

HI_S32 hi3136_DiSEqC_send_msg(HI_U32 u32TunerPort, HI_UNF_TUNER_DISEQC_SENDMSG_S *pstSendMsg)
{
    HI_U8 i = 0;
    HI_U8 u8Temp = 0;
    HI_U8	  diseqc_mode = 0;
    HI_U32	  diseqc_Ratio = 0;

    qam_read_byte(u32TunerPort, TX_CTRL, &diseqc_mode);
    //diseqc_Ratio = (HI3136_SYSCLK_SAMPLE / 22) & 0xffff;
    diseqc_Ratio = (hi3136_cfg[u32TunerPort].u32CLK_DEMO_kHz / 22) & 0xffff;
    //printk("diseqc_mode = 0x%x, diseqc_Ratio = 0x%x\n", diseqc_mode, diseqc_Ratio);
    qam_write_byte(u32TunerPort, DSEC_RATIO_L, (HI_U8)(diseqc_Ratio & 0xff));
    qam_write_byte(u32TunerPort, DSEC_RATIO_H, (HI_U8)((diseqc_Ratio & 0xff00) >> 8)); //设置22K 时钟


    u8Temp = diseqc_mode & 0x01;   /*diseqc 处于空闲状态，即使diseqc module 复位，屏蔽也可用*/
    qam_write_byte(u32TunerPort, TX_CTRL, u8Temp);


    for (i = 0; i < pstSendMsg->u8Length; i++)
    {
        //printk("i = %d, pstSendMsg->au8Msg[i] = 0x%x\n", i, pstSendMsg->au8Msg[i]);
        qam_write_byte(u32TunerPort, DSEC_DATA, pstSendMsg->au8Msg[i]);
        qam_write_byte(u32TunerPort, DSEC_ADDR, i);
    }
    
    u8Temp =  (diseqc_mode & 0x01) | (1 << 3) | (((pstSendMsg->u8Length - 1) & 7) << 4);
    //printk("u8Temp = 0x%x\n", u8Temp);
    qam_write_byte(u32TunerPort,TX_CTRL,u8Temp); /*发送num+1个字节,并发送完整diseqc信息*/

    msleep_interruptible(60);	//至少延时30ms

    return HI_SUCCESS;
}

#ifndef BOOT_COMPILE
HI_S32 hi3136_tp_verify(HI_U32 u32TunerPort, TUNER_TP_VERIFY_PARAMS_S * pstChannel)
{
	HI_U8 i, j, k, u8Temp, u8Temp1, temp1, temp2, tim_ok_bit, s2_sync_bit, fec_ok_bit = 0;
    HI_U32 temp_TPnum = 0, cbs_temp_num = 0;	
	//HI_U8 fec_ok;
    HI_S32 s32FuncRet = HI_SUCCESS;
	HI_U16 u16TimeOut =0 ;
	HI_U8 cbs_reliablity_temp, max_cbs_reliablity;
	HI_UNF_TUNER_SAT_TPINFO_S tempChannel[32];
	HI_U8 tim_wait_tmp, u8tempReliablity;
    HI_U16 fec_wait_tmp = 0, tim_cnt = 0;
	HI_S16 s16Freqoffset;
	HI_U32 u32tempFreq = 0, u32tempSymb = 0;

	hi3136_Set_BS_Mode(u32TunerPort, 1); 
	qam_write_bit(u32TunerPort,CBS_CTRL_RDADDR, 6, 1);
	qam_read_byte(u32TunerPort,CBS_CTRL_RDADDR, &u8Temp);
    //printk("%s, %d, ctrl reg is %x\n", __func__, __LINE__, u8Temp);
	u8Temp &= 0xe0;
	qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp);

    qam_write_byte(u32TunerPort, LDPC_ITER_ADDR, 0x50);

    hi3136_set_symrate(u32TunerPort, *(pstChannel->pu32SymbolRate) * 1000);

    for (i = 0; i < 2; i++)
    {
        hi3136_setlpf(u32TunerPort, *(pstChannel->pu32SymbolRate) * 1000/*75000 / 100*/);
        if (g_stTunerOps[u32TunerPort].set_tuner)
        {
            s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, *(pstChannel->pu32Frequency) + 500);
            if (HI_SUCCESS != s32FuncRet)
            {
                printk( "set tuner error, s32FuncRet = %d\n", s32FuncRet);
                return s32FuncRet;
            }
        }

        u8Temp = pstChannel->CBS_TH;
        qam_write_byte(u32TunerPort, CBS_TH1_COR_NO, u8Temp);
        
        hi3136_HotReset_CHIP(u32TunerPort);
        msleep_interruptible(20);
    
        u8Temp = 0;
        temp_TPnum = 0;
        u16TimeOut = 0;
        do
        {
            msleep_interruptible(5);
            qam_read_bit(u32TunerPort, CBS_FINISH, 0, &u8Temp);
            if(u8Temp)  //cbs_finish 标志位1表示已完成
            {
                msleep_interruptible(5);
                qam_read_byte(u32TunerPort, CBS_R2_NUM, &temp1);
                temp1 &= 0x1f;
                temp_TPnum = temp1;
            }
            u16TimeOut += 5;
            //printk("%s, %d, stop bit is %d, temp_TPnum = %d\n", __func__, __LINE__, u8Temp, temp_TPnum);
        }
        while((!u8Temp) && (u16TimeOut <200));		//等待读取CBS_FINISH 操作
    
        if(temp_TPnum != 0)	//TP>=1 即盲扫成功，并开始已知Fs Fc进行盲扫，读取相应的TP数，找可靠度最高的TP
        {
            for(j = 0; j < temp_TPnum; j++)		//已知Fs Fc情况下，盲扫信息参数
            {
                qam_read_byte(u32TunerPort, CBS_CTRL_RDADDR, &u8Temp1);
                u8Temp1 &= 0xe0;
                u8Temp1 |= j;
                
                qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, u8Temp1);
                
                qam_read_byte(u32TunerPort, CBS_FC_L, &temp1);
                qam_read_byte(u32TunerPort, CBS_FC_H, &temp2);
                
                tempChannel[j].u32Freq = (temp1 | (temp2 << 8));

                if(tempChannel[j].u32Freq > 32767)
                {
                    tempChannel[j].u32Freq = tempChannel[j].u32Freq - 65536; 
                }
                if(hi3136_cfg[u32TunerPort].enIQSpectrum)				
                {
                    tempChannel[j].u32Freq = -tempChannel[j].u32Freq;
                }
                
                qam_read_byte(u32TunerPort, CBS_FS_L, &temp1);
                qam_read_byte(u32TunerPort, CBS_FS_H, &temp2);
                
                tempChannel[j].u32SymbolRate = ((temp2 << 8) + temp1) * 1000;
                
                qam_read_byte(u32TunerPort, CBS_RELIABLITY, &cbs_reliablity_temp);
                tempChannel[j].cbs_reliablity = cbs_reliablity_temp;
            }

            if(temp_TPnum == 1)
            {	
                tim_wait_tmp = (30 - pstChannel->fs_grade * 2);
                tim_cnt = 0;
                
                do
                {
                    qam_read_bit(u32TunerPort, LOCK_FLAG, 2, &tim_ok_bit); 
                    msleep_interruptible(5);
                    tim_cnt += 5;
                }
                while((!tim_ok_bit) && (tim_cnt < tim_wait_tmp));	
    
                if(tim_ok_bit == 1)
                {
                    //printk("%s, %d tim_ok_bit\n", __func__, __LINE__);
                    qam_read_bit(u32TunerPort, LOCK_FLAG, 3, &s2_sync_bit);	
                    fec_wait_tmp = (320 - pstChannel->fs_grade * 2) * 2 + 100;//320 - pstChannel->fs_grade * 2 + s2_sync_bit * 100;
                    //printk("fec_wait_tmp = %d\n", fec_wait_tmp);
                    tim_cnt = 0;
    
                    do
                    {
                        qam_read_bit(u32TunerPort, LOCK_FLAG, 5, &fec_ok_bit);
                        msleep_interruptible(10);	
                        tim_cnt += 10;
                    }while((!fec_ok_bit) && (tim_cnt < fec_wait_tmp));
                    
                    if(fec_ok_bit != 1)
                    {
                        //printk("%s, %d\n", __func__, __LINE__);
                        if(s2_sync_bit == 1)
                        {
                            continue;
                        }
                        else if(pstChannel[i].cbs_reliablity > (pstChannel->CBS_TH * 125 / 100))//???????要不要写进A2寄存器
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            else
            {
                cbs_temp_num = temp_TPnum;
                max_cbs_reliablity = tempChannel[0].cbs_reliablity;
                k = 0;
                for(j = 1; j < cbs_temp_num; j++)	//找可靠度最高的TP
                {
                    if(abs(tempChannel[j].u32Freq) < (tempChannel[j].u32SymbolRate * 25 / 100)) 
                    {	
                        if(tempChannel[j].cbs_reliablity > max_cbs_reliablity)		//选择可靠度最高的TP
                        {	
                            max_cbs_reliablity = tempChannel[j].cbs_reliablity;
                            k = j;
                        }

                        u32tempFreq = *(pstChannel->pu32Frequency);
                        u32tempSymb = tempChannel[k].u32SymbolRate;
                        u8tempReliablity = tempChannel[k].cbs_reliablity;
                        s16Freqoffset = tempChannel[k].u32Freq;
                        u32tempFreq = *(pstChannel->pu32Frequency) + (HI_S32)s16Freqoffset;
                        *(pstChannel->pu32Frequency) = u32tempFreq;
                        //*(pstChannel->pu32SymbolRate) = u32tempSymb;
                    }
                }

                hi3136_setlpf(u32TunerPort, u32tempSymb * 1000/*75000 / 100*/);
                if (g_stTunerOps[u32TunerPort].set_tuner)
                {
                    s32FuncRet = g_stTunerOps[u32TunerPort].set_tuner(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, u32tempFreq + 500);
                    if (HI_SUCCESS != s32FuncRet)
                    {
                        printk( "set tuner error, s32FuncRet = %d\n", s32FuncRet);
                        return s32FuncRet;
                    }
                }

                qam_write_byte(u32TunerPort, CBS_CTRL_RDADDR, 0x00);	//关闭盲扫，Ram地址为0
                
                //s32FuncRet = (*g_stTunerOps[u32TunerPort].set_s2_tuner)(u32TunerPort, g_stTunerOps[u32TunerPort].enI2cChannel, (u32tempFreq+500)/1000,u32tempSymb);
                hi3136_HotReset_CHIP(u32TunerPort);
                msleep_interruptible(20);
                
                tim_wait_tmp = (30 - pstChannel->fs_grade*2);
                tim_cnt = 0;
                do
                {
                    qam_read_bit(u32TunerPort, LOCK_FLAG, 2, &tim_ok_bit);
                    msleep_interruptible(5);
                    tim_cnt += 5;
                }while((!tim_ok_bit) && (tim_cnt < tim_wait_tmp));	
    
                if(tim_ok_bit == 1)
                {	
                    qam_read_bit(u32TunerPort, LOCK_FLAG, 3, &s2_sync_bit);	
                    fec_wait_tmp = 320 - pstChannel->fs_grade*2 + s2_sync_bit*100;
                    //printk("fec_wait_tmp = %d\n", fec_wait_tmp);
                    tim_cnt = 0;
    
                    do
                    {
                        qam_read_bit(u32TunerPort, LOCK_FLAG, 5, &fec_ok_bit);
                        msleep_interruptible(10);	
                        tim_cnt += 10;
                    }while((!fec_ok_bit) && (tim_cnt < fec_wait_tmp));
                    
                    if(fec_ok_bit != 1)
                    {
                        if(s2_sync_bit == 1)
                        {
                            continue;
                        }
                        else if(pstChannel[i].cbs_reliablity > (pstChannel->CBS_TH * 125 / 100))
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
        }
        else
        {
            //printk("%s, %d  no tp------------------------------\n", __func__, __LINE__);
            break;
        }
    }
    
    if(fec_ok_bit)
    {	
        //printk("%s, %d, fec_ok_cnt = %d freq = %d, symb = %d\n", __func__, __LINE__, *(pstChannel->fec_ok_cnt), *(pstChannel->pu32Frequency), *(pstChannel->pu32SymbolRate));
        //Hi3136_change_iterNum(u32TunerPort);
        *(pstChannel->fec_ok_cnt) += 1;

        return HI_SUCCESS;
    }
    else
    {
        //printk("%s, %d, fec_no_ok_cnt = %d\n", __func__, __LINE__, *(pstChannel->fec_no_ok_cnt));
        *(pstChannel->fec_no_ok_cnt) += 1;
        return HI_SUCCESS;
    }
}
#endif

HI_S32 hi3136_set_ts_out(HI_U32 u32TunerPort, HI_UNF_TUNER_TSOUT_SET_S *pstTSOut)
{
    HI_U8 u8Temp = 0, i;
    HI_ASSERT(HI_NULL != pstTSOut);

    /* Check tuner port and init. */
    if (u32TunerPort >= TUNER_NUM)
    {
        HI_INFO_TUNER("Bad u32TunerPort\n");
        return HI_FAILURE;
    }

    for (i = 0; i < MAX_TS_LINE; i++)
    {
        if (HI_UNF_TUNER_OUTPUT_BUTT <= pstTSOut->enTSOutput[i])
        {
            HI_INFO_TUNER("Error pin define!\n");
            return HI_FAILURE;
        }
    }

    qam_read_byte(u32TunerPort, TS_PARALL_CTRL, &u8Temp);
    u8Temp = u8Temp & 0xf0;
    u8Temp = u8Temp | (pstTSOut->enTSOutput[10] & 0xf);
    qam_write_byte(u32TunerPort, TS_PARALL_CTRL, u8Temp);

    qam_write_byte(u32TunerPort, TS_OUT98_SEL, ((pstTSOut->enTSOutput[9]& 0xf)<<4) | pstTSOut->enTSOutput[8]);/*config ts_out_9&8bit*/
    qam_write_byte(u32TunerPort, TS_OUT76_SEL, ((pstTSOut->enTSOutput[7]& 0xf)<<4) | pstTSOut->enTSOutput[6]);/*config ts_out_7&6bit*/
    qam_write_byte(u32TunerPort, TS_OUT54_SEL, ((pstTSOut->enTSOutput[5]& 0xf)<<4) | pstTSOut->enTSOutput[4]);/*config ts_out_5&4bit*/
    qam_write_byte(u32TunerPort, TS_OUT32_SEL, ((pstTSOut->enTSOutput[3]& 0xf)<<4) | pstTSOut->enTSOutput[2]);/*config ts_out_3&2bit*/
    qam_write_byte(u32TunerPort, TS_OUT10_SEL, ((pstTSOut->enTSOutput[1]& 0xf)<<4) | pstTSOut->enTSOutput[0]);/*config ts_out_1&0bit*/

    return HI_SUCCESS;
}

