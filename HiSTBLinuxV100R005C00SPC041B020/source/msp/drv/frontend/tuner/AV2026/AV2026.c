/*************************************************************
** file name:		av2026.c
** brief: 
** COPYRIGHT(C) HDIC
** -----------------------------------------------------------
** module information: 
** 
** -----------------------------------------------------------
** revision history 
** -----------------------------------------------------------
**	date 		version		action				author   
** -----------------------------------------------------------
**	2016-2-26 	1.0			created				   
** 
*************************************************************/

/* include -------------------------------------------------*/


#include <linux/delay.h>
#include "linux/kernel.h"

#include "hi_type.h"
#include "hi_drv_gpioi2c.h"
#include "hi_drv_gpio.h"
#include "hi_debug.h"
#include "hi_error_mpi.h"
#include "hi_drv_proc.h"
#include "hi_drv_sys.h"

#include "hd2501_type.h"
#include "hd2501_drv.h"
#include "AV2026.h"

/* macro & structure ---------------------------------------*/

#ifdef _USE_HD2501_CHIP_

/* global & static variables -------------------------------*/

extern HI_U32 			genI2cChannelHD2501;

/* extern functions declaration ----------------------------*/
/* static functions declaration ----------------------------*/
/* functions define ----------------------------------------*/
/* static functions definition -----------------------------*/

U8  AV2026_Tuner_I2C_write(U8 regAddr, U8* data, U8 length)
{
	HI_S32 ret;

    if (HI_STD_I2C_NUM > genI2cChannelHD2501)
    {
    	ret = HI_DRV_I2C_Write(genI2cChannelHD2501, HDIC_TUNER_DEV_ADDR_AV2026, regAddr, 1, data, length );
    }
    else
    {
        ret = HI_DRV_GPIOI2C_WriteExt(genI2cChannelHD2501, HDIC_TUNER_DEV_ADDR_AV2026, regAddr, 1, data, length );

    }

	if(HI_SUCCESS == ret)
		return HDIC_NO_ERROR;
	else
		return HDIC_ERROR_IIC_ERROR;
}

U8 AV2026_Tuner_control(U32 channel_freq, U32 bb_sym)
{
    // Register initial flag
    static U8 tuner_initial = 0;
    U8 reg[50];
    U8 reg_start;
    U32 fracN;
    U32 BW;
    U32 BF;
    U8 r;
    U8 auto_scan = 0;// Add flag for "’ßÌ¨"
    
    HDIC_Demod_Print("%s:%d,enter\n",__FUNCTION__,__LINE__);

    //when sym is 0 or 45000, means auto-scan channel.
    if (bb_sym == 0 || bb_sym == 45000) //auto-scan mode
    {
        auto_scan = 1;
    }
//	DS_printf("av2020:%d - %d \n\r", channel_freq, bb_sym);

//	DSI_printf(0, " %d  %d \n\r", channel_freq, bb_sym);
    // At Power ON, tuner_initial = 0
    // do the initial if not do yet.
    if (tuner_initial == 0)
    {
        //Tuner Initail registers R0~R41
        reg[0]= (0x38);
        reg[1]= (0x00);
        reg[2]= (0x00);
        reg[3]= (0x50);
        reg[4]= (0x1f);
        reg[5]= (0xa3);
        reg[6]= (0xfd);
        reg[7]= (0x58);
        //reg[8]= (0x46);
        reg[8]=(0x32);
        reg[8]=(0x1e);
        reg[9]= (0x82);
        reg[10]= (0x88);
        reg[11]= (0xb4);
        reg[12]= (0xd6); //RFLP=ON at Power on initial
        reg[13]= (0x40);

        //reg[14]=(char) (0x94);
        //reg[15]=(char) (0x4a);

        reg[14]= (0x5b);
        reg[15]= (0x6a);

        reg[16]= (0x66);
        reg[17]= (0x40);
        reg[18]= (0x80);
        reg[19]= (0x2b);
        reg[20]= (0x6a);
        reg[21]= (0x50);
        reg[22]= (0x91);
        reg[23]= (0x27);
        reg[24]= (0x8f);
        reg[25]= (0xcc);
        reg[26]= (0x21);
        reg[27]= (0x10);
        reg[28]= (0x80);
       // reg[29]= (0x02);
       reg[29]= (0xee);
        reg[30]= (0xf5);
        reg[31]= (0x7f);
        reg[32]= (0x4a);
        reg[33]= (0x9b);
        reg[34]= (0xe0);
        reg[35]= (0xe0);
        reg[36]= (0x36);
        //monsen 20080710. Disble FT-function at Power on initial
        //reg[37]=(char) (0x02);
        reg[37]= (0x00);
        reg[38]= (0xab);
        reg[39]= (0x97);
        reg[40]= (0xc5);
        reg[41]= (0xa8);
        //monsen 20080709. power on initial at first "Tuner_control()" call
        // Sequence 1
        // Send Reg0 ->Reg11
        reg_start = 0;
        r =  AV2026_Tuner_I2C_write(reg_start,reg,12);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Time delay 1ms
        HDIC2501_Wait( 1 );//Wait 1ms;
        // Sequence 2
        // Send Reg13 ->Reg24
        reg_start = 13;
        r = AV2026_Tuner_I2C_write(reg_start,reg+13,12);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Send Reg25 ->Reg35
        reg_start = 25;
        r = AV2026_Tuner_I2C_write(reg_start,reg+25,11);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Send Reg36 ->Reg41
        reg_start = 36;
        r = AV2026_Tuner_I2C_write(reg_start,reg+36,6);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Time delay 1ms
        //GK_Delay_N_ms(1);
        HDIC2501_Wait( 1 );//Wait 1ms;
        // Sequence 3
        // send reg12
        reg_start = 12;
        r = AV2026_Tuner_I2C_write(reg_start,reg+12,1);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        //monsen 20081125, Wait 100 ms
        HDIC2501_Wait( 100 );//Wait 100ms;
        //GK_Delay_N_ms(100);
        //monsen 20081030, Reinitial again
        {
            // Sequence 1
            // Send Reg0 ->Reg11
            reg_start = 0;
            r = AV2026_Tuner_I2C_write(reg_start,reg,12);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            // Time delay 1ms
            HDIC2501_Wait( 1 );//Wait 1ms;
            // Sequence 2
            // Send Reg13 ->Reg24
            reg_start = 13;
            r = AV2026_Tuner_I2C_write(reg_start,reg+13,12);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            // Send Reg25 ->Reg35
            reg_start = 25;
            r = AV2026_Tuner_I2C_write(reg_start,reg+25,11);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            // Send Reg36 ->Reg41
            reg_start = 36;
            r = AV2026_Tuner_I2C_write(reg_start,reg+36,6);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            // Time delay 1ms
            HDIC2501_Wait( 1 );//Wait 1ms;
            // Sequence 3
            // send reg12
            reg_start = 12;
            r = AV2026_Tuner_I2C_write(reg_start,reg+12,1);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
        }
        // Wait 4 ms

        HDIC2501_Wait( 4 );
        // power on initial end
        tuner_initial = 1;
        //***** monsen 20080709. Lock Channel sequence
        //***** After RFAGC is starting tracking
        //***** Make sure the RFAGC do not have sharp jump after sending reg[37]
        // RF Channel Fregency Setting.
        fracN = (channel_freq + AV2026_tuner_crystal/2)/AV2026_tuner_crystal;
        if(fracN > 0xff)
            fracN = 0xff;
        reg[0]= (fracN & 0xff);
        fracN = (channel_freq<<17)/AV2026_tuner_crystal;
        fracN = fracN & 0x1ffff;
        reg[1]= ((fracN>>9)&0xff);
        reg[2]= ((fracN>>1)&0xff);
        // reg[3]_D7 is frac<0>, D6~D0 is 0x50
        reg[3]=(char) (((fracN<<7)&0x80) | 0x50); // default is 0x50
        // Channel Filter Bandwidth Setting.
        //"sym" unit is Hz;
        if(auto_scan)//’ßÌ¨ requested by BB
        {
            reg[5] = 0xA3; //BW=27MHz
        }
        else
        {
            // rolloff is 35%
            BW = bb_sym*135/200;
            // monsen 20080726, BB request low IF when sym < 6.5MHz
            // add 6M when Rs<6.5M,
            if(bb_sym<6500)
            {
                BW = BW + 6000;
            }
            // add 2M for LNB frequency shifting
            BW = BW + 2000;
            // add 8% margin since fc is not very accurate
            BW = BW*108/100;
            // Bandwidth can be tuned from 4M to 40M
            if( BW< 4000)
            {
                BW = 4000;
            }
            if( BW> 40000)
            {
                BW = 40000;
            }
            BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
            reg[5] = BF;
        }
        // Sequence 4
        // Send Reg0 ->Reg4
        r = AV2026_Tuner_I2C_write(0,reg,4);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Sequence 5
        // Send Reg5
        r = AV2026_Tuner_I2C_write(5, reg+5, 1);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Fine-tune Function Control
        //Tuner fine-tune gain function block. bit2.
        //not auto-scan case. enable block function. FT_block=1, FT_EN=1
        if (!auto_scan)
        {
            reg[37] = 0x06;
            r = AV2026_Tuner_I2C_write(37, reg+37, 1);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            //Disable RFLP at Lock Channel sequence after reg[37]
            //RFLP=OFF at Lock Channel sequence
            // RFLP can be Turned OFF, only at Receving mode.
            reg[12] = 0x96;
            r = AV2026_Tuner_I2C_write(12, reg+12, 1);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
        }
    }
    else
    {
        //***** monsen 20080709. Lock Channel sequence
        //***** After RFAGC is starting tracking
        //***** Make sure the RFAGC do not have sharp jump after sending reg[37]
        // RF Channel Fregency Setting.
        fracN = (channel_freq + AV2026_tuner_crystal/2)/AV2026_tuner_crystal;
        if(fracN > 0xff)
            fracN = 0xff;
        reg[0]= (fracN & 0xff);
        fracN = (channel_freq<<17)/AV2026_tuner_crystal;
        fracN = fracN & 0x1ffff;
        reg[1]= ((fracN>>9)&0xff);
        reg[2]= ((fracN>>1)&0xff);
        // reg[3]_D7 is frac<0>, D6~D0 is 0x50
        reg[3]=(char) (((fracN<<7)&0x80) | 0x50); // default is 0x50
        // Channel Filter Bandwidth Setting.
        //"sym" unit is Hz;
        if(auto_scan)//’ßÌ¨ requested by BB
        {
            reg[5] = 0xA3; //BW=27MHz
        }
        else
        {
            // rolloff is 35%
            BW = bb_sym*135/200;
            // monsen 20080726, BB request low IF when sym < 6.5MHz
            // add 6M when Rs<6.5M,
            if(bb_sym<6500)
            {
                BW = BW + 6000;
            }
            // add 2M for LNB frequency shifting
            BW = BW + 2000;
            // add 8% margin since fc is not very accurate
            BW = BW*108/100;
            // Bandwidth can be tuned from 4M to 40M
            if( BW< 4000)
            {
                BW = 4000;
            }
            if( BW> 40000)
            {
                BW = 40000;
            }
            BF = (BW*127 + 21100/2) / (21100); // BW(MHz) * 1.27 / 211KHz
            reg[5] = (U8)BF;
        }
        // Sequence 4
        // Send Reg0 ->Reg4
        r = AV2026_Tuner_I2C_write(0,reg,4);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Sequence 5
        // Send Reg5
        r = AV2026_Tuner_I2C_write(5, reg+5, 1);
        if(r!=HDIC_NO_ERROR)
        {
            return(r);
        }
        // Fine-tune Function Control
        //Tuner fine-tune gain function block. bit2.
        //not auto-scan case. enable block function. FT_block=1, FT_EN=1
        if (!auto_scan)
        {
            reg[37] = 0x06;
            r = AV2026_Tuner_I2C_write(37, reg+37, 1);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
            //Disable RFLP at Lock Channel sequence after reg[37]
            //RFLP=OFF at Lock Channel sequence
            // RFLP can be Turned OFF, only at Receving mode.
            reg[12] = 0x96;
            r = AV2026_Tuner_I2C_write(12, reg+12, 1);
            if(r!=HDIC_NO_ERROR)
            {
                return(r);
            }
        }
    }
    return r;
}

#endif //_USE_HD2501_CHIP_
