
#include <linux/delay.h>
#include "drv_tuner_ioctl.h"
#include "drv_demod.h"
#include "M88TS2022.h"

#if defined(DEMOD_DEV_TYPE_HI3136)
#define TUNER_UNDEF      0
#define TUNER_M88TS2020  1			//define the tuner Version
#define TUNER_M88TS2022  2

#define TUNER_I2C_ADDR		0xc0	 /* M88TS2022 i2c address */
#define TUNER_XTAL_FREQ		27000	 /* Crystal Frequency of M88TS2022 used, unit: kHz */

#define TUNER_RFBYPASS_ON    	0	/* If you need loop through function, please set it to 1 , else set it to 0 */
#define TUNER_CKOUT_ON       	0	/* If you need clock out from tuner clkout pin to demodulator use, please set it to 1 , else set it 0*/
#define TUNER_CKOUT_DIV      	0x00	/* The clock out division,if it is 0x00 means it use the hardware default setting */
#define TUNER_CKOUT_XTAL        0       /* If you need clock out from tuner xtalout pin to demodulator use, please set it to 1 , else set it 0*/

#define DEMOD_TYPE_ALI          0       // define the demodulator type, if it is Ali chip,please set it to 1, else set it to 0

static HI_U8 gvTunerVersion = TUNER_M88TS2022;  

HI_VOID	ReadTunerRegister(HI_U32 u32TunerPort, HI_U8 RegAddress, HI_U8 *RegData);

HI_VOID	WriteTunerRegister(HI_U32 u32TunerPort, HI_U8 RegAddress, HI_U8 RegData)
{
    //TODO: write tuner register
    
    HI_U8 enI2cChannel = 0;
    TUNER_I2C_DATA_S stI2cData = {0};
    HI_U8 au8Data[2] = {0};
    HI_U8 u8DevAddr = 0;

    u8DevAddr = g_stTunerOps[u32TunerPort].u32TunerAddress;

    au8Data[0] = RegAddress;
    au8Data[1] = RegData;

    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    stI2cData.pu8ReceiveBuf = 0;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = au8Data;
    stI2cData.u32SendLength = 2;


    hi3136_config_i2c_out(u32TunerPort, 1);

    tuner_i2c_send_data(enI2cChannel, u8DevAddr, &stI2cData);
}

HI_VOID	ReadTunerRegister(HI_U32 u32TunerPort, HI_U8 RegAddress, HI_U8 *RegData)
{
    //TODO: read  tuner register
    HI_U8 enI2cChannel = 0;
    HI_U8 u8DevAddr = 0;

    u8DevAddr = g_stTunerOps[u32TunerPort].u32TunerAddress;
    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;

    hi3136_config_i2c_out(u32TunerPort, 1);

    if (4 >= enI2cChannel)
    {
        HI_DRV_I2C_Read(enI2cChannel, u8DevAddr, RegAddress, 1, RegData, 1);
    }
}

HI_VOID	WaitTime(HI_U16 time)
{         //Wait time , unit : ms
    
    //TODO: wait time
    tuner_mdelay(time);
}


/* Function to Check the tuner version: 0: Error  1: M88TS2020  2: M88TS2022*/ 

HI_U8	CheckTunerVersion(HI_U32 u32TunerPort)
{
    
    HI_U8 buf;
    
    // Wake Up the tuner
    
    ReadTunerRegister(u32TunerPort,0x00,&buf);
    buf &= 0x03;
    
    if(buf == 0x00)
    {
        WriteTunerRegister(u32TunerPort,0x00,0x01);
        WaitTime(2);
    }
    WriteTunerRegister(u32TunerPort,0x00,0x03);    
    WaitTime(2);
    
    //Check the tuner version
    
    ReadTunerRegister(u32TunerPort,0x00,&buf);
    
    if((buf == 0x01) || (buf == 0x41) || (buf == 0x81))
    {
        return TUNER_M88TS2020;	
    }
    else if((buf == 0xc3)|| (buf == 0x83))
    {
        return TUNER_M88TS2022;	
    }
    else
    {
        return TUNER_UNDEF;	
    }
}

/* Function to Initialize the M88TS2022 */
HI_S32  M88TS2022_init_tuner(HI_U32 u32TunerPort)
{ 
    HI_U8 buf;
    
    gvTunerVersion = CheckTunerVersion(u32TunerPort);   	// check the tuner version and power on the tuner
    
    if(gvTunerVersion == TUNER_UNDEF)
    {
        return HI_FAILURE;
    }
   
    if(gvTunerVersion == TUNER_M88TS2020)
    {       // For M88TS2020   
        
        if(TUNER_RFBYPASS_ON)
        {
            WriteTunerRegister(u32TunerPort,0x62,0xfd);
        }
        else
        {
            WriteTunerRegister(u32TunerPort,0x62,0xbd);
        }
        WaitTime(2);	
        
        if(TUNER_CKOUT_ON)
        {
            if(TUNER_CKOUT_DIV)
            {
                buf = TUNER_CKOUT_DIV ;
                buf &= 0x1f;
                WriteTunerRegister(u32TunerPort,0x05,buf);
                WaitTime(2);	
            }
            WriteTunerRegister(u32TunerPort,0x42,0x73);
            WaitTime(2);
        }
        else
        {
            WriteTunerRegister(u32TunerPort,0x42,0x63);
        }
        
        WriteTunerRegister(u32TunerPort,0x07,0x02);
        WriteTunerRegister(u32TunerPort,0x08,0x01);
    }
   
    if(gvTunerVersion == TUNER_M88TS2022)
    {       //For M88TS2022
        
        if(TUNER_RFBYPASS_ON)
        {
            WriteTunerRegister(u32TunerPort,0x62,0xec);
        }
        else
        {
            WriteTunerRegister(u32TunerPort,0x62,0x6c);
        }	
        WaitTime(2);
        
        if(TUNER_CKOUT_XTAL)
        {
            WriteTunerRegister(u32TunerPort,0x42,0x6c);
            WaitTime(2);   
        }
        else if(TUNER_CKOUT_ON)
        {
            if(TUNER_CKOUT_DIV)
            {
                buf = TUNER_CKOUT_DIV ;
                buf &= 0x1f;
                WriteTunerRegister(u32TunerPort,0x05,buf);
                WaitTime(2);	
            }
            WriteTunerRegister(u32TunerPort,0x42,0x70); 
            WaitTime(2);          	
        }
        else
        {
            WriteTunerRegister(u32TunerPort,0x42,0x60);
        } 
        
        WriteTunerRegister(u32TunerPort,0x7d, 0x9d);    
        WriteTunerRegister(u32TunerPort,0x7c, 0x9a);
        WriteTunerRegister(u32TunerPort,0x7a, 0x76);
        WriteTunerRegister(u32TunerPort,0x3b, 0x01);  
        WriteTunerRegister(u32TunerPort,0x63, 0x88);	 	
        WriteTunerRegister(u32TunerPort,0x61, 0x85);
        WriteTunerRegister(u32TunerPort,0x22, 0x30);
        WriteTunerRegister(u32TunerPort,0x30, 0x40);
        WriteTunerRegister(u32TunerPort,0x20, 0x23);
        WriteTunerRegister(u32TunerPort,0x24, 0x02);		
        WriteTunerRegister(u32TunerPort,0x12, 0xa0);  
        
        if(DEMOD_TYPE_ALI)
        {   
            WriteTunerRegister(u32TunerPort,0x07, 0x33);
            WriteTunerRegister(u32TunerPort,0x24, 0x01);	
        }	
    }

    return HI_SUCCESS;
}

/* Function to Set the M88TS2022*/ 
/*fPLL:    Frequency         			unit: MHz	from 950 to 2150*/
/*fSym:    SymbolRate         			unit: KS/s  from 1000 to 45000*/
/*lpfOffset: Set the low pass filter offset when the demodulator set the PLL offset at low symbolrate  unit: KHz*/
/*gainHold:  The flag of AGC gain hold, the tuner gain is hold when gainHold == 1 , default please set gainHold = 0*/
/*return:	 Frequency offset of PLL 	 	unit: KHz*/

HI_S32 M88TS2022_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/)  //freq :unit MHZ, symbol:unit  Khz
{
    
    HI_U8      buf, capCode, div4, changePLL, K, lpf_mxdiv, divMax, divMin, RFgain = 0;
    HI_U32     gdiv28;
    HI_U32     N, lpf_gm, f3dB, f_REF, divN, lpf_coeff; 
    HI_S32      freqOffset;
    HI_U32 fPLL = 0, fSym = 0;

    fPLL = u32PuRF / 1000;
    fSym = g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32SymbolRate / 1000;
    if(gvTunerVersion == TUNER_UNDEF)
    {
        return HI_FAILURE;
    }

    //Set the PLL
    
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        WriteTunerRegister(u32TunerPort,0x10,0x00); 
    }
    if(gvTunerVersion == TUNER_M88TS2022)
    { 
        WriteTunerRegister(u32TunerPort,0x10, 0x0b);  
        WriteTunerRegister(u32TunerPort,0x11, 0x40);
    }
    
    div4 = 0;
    changePLL = 0;
    K = 0;
    divN = 0;
    N = 0;
    f_REF = 2;  
   
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        
        K = (TUNER_XTAL_FREQ / 1000 + 1)/2 - 8;  
        
        if (fPLL < 1146 )
        {
            
            WriteTunerRegister(u32TunerPort,0x10, 0x11); 					  
            div4 = 1;
            divN = fPLL * (K + 8) * 4000  / TUNER_XTAL_FREQ ;
        }
        else
        {
            
            WriteTunerRegister(u32TunerPort,0x10, 0x01);
            divN = fPLL * (K + 8) * 2000  / TUNER_XTAL_FREQ ;
        }
        
        divN =divN + divN % 2;
        
        N = divN - 1024;
        
        buf = (N >> 8) & 0x0f;  
        WriteTunerRegister(u32TunerPort,0x01, buf);  
        
        buf = N & 0xff;           
        WriteTunerRegister(u32TunerPort,0x02, buf); 
        
        buf = K;
        WriteTunerRegister(u32TunerPort,0x03,buf);
        
    }
   
    if(gvTunerVersion == TUNER_M88TS2022)
    {
        
        if(f_REF == 1)
            K = TUNER_XTAL_FREQ / 1000  - 8;
        else
            K = (TUNER_XTAL_FREQ / 1000 + 1) / 2 - 8;
        
        if (fPLL < 1103 )
        { 
            WriteTunerRegister(u32TunerPort,0x10, 0x1b);
            div4 = 1; 
            divN = fPLL * (K+8) * 4000 / TUNER_XTAL_FREQ; 
        }
        else
        {
            divN = fPLL * (K+8) * 2000 / TUNER_XTAL_FREQ; 
        }
        
        divN = divN + divN % 2;	
        
        if(divN < 4095)
        {
            N = divN - 1024;
        }
        else if(divN < 6143)
        {
            N = divN + 1024;
        }
        else
        {
            N = divN + 3072;
        }
        
        buf = (N >> 8) & 0x3f;  
        WriteTunerRegister(u32TunerPort,0x01, buf);  
        
        buf = N & 0xff;           
        WriteTunerRegister(u32TunerPort,0x02, buf); 
        
        buf = K;
        WriteTunerRegister(u32TunerPort,0x03,buf);
    
    }
    
    WriteTunerRegister(u32TunerPort,0x51, 0x0f);   
    WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
    WriteTunerRegister(u32TunerPort,0x50, 0x10);       
    WriteTunerRegister(u32TunerPort,0x50, 0x00);		
    WaitTime(5); 
    
    ReadTunerRegister(u32TunerPort,0x15,&buf);
    if((buf&0x40) != 0x40)
    {
        WriteTunerRegister(u32TunerPort,0x51, 0x0f);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x10);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00);	
        WaitTime(5);  
    }
  
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        
        ReadTunerRegister(u32TunerPort,0x66,&buf);
        changePLL = (((buf & 0x80) >> 7) != div4); 
        
        if(changePLL)
        {
            
            WriteTunerRegister(u32TunerPort,0x10, 0x11); 
            div4 = 1;
            
            divN = fPLL * (K + 8) * 4000 / TUNER_XTAL_FREQ;
            divN = divN + divN % 2;	       
            N = divN - 1024;
            
            buf = (N >>8) & 0x0f;  
            WriteTunerRegister(u32TunerPort,0x01, buf);  
            
            buf = N & 0xff;           
            WriteTunerRegister(u32TunerPort,0x02, buf); 
            
            WriteTunerRegister(u32TunerPort,0x51, 0x0f);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x10);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00); 
            WaitTime(5);  
        }
    }

    if(gvTunerVersion == TUNER_M88TS2022)
    {
        
        ReadTunerRegister(u32TunerPort,0x14, &buf); 
        buf &= 0x7f;
        if(buf < 64)
        {                   
            ReadTunerRegister(u32TunerPort,0x10, &buf); 
            buf |= 0x80;                          
            WriteTunerRegister(u32TunerPort,0x10, buf);
            WriteTunerRegister(u32TunerPort,0x11, 0x6F);
            
            WriteTunerRegister(u32TunerPort,0x51, 0x0f);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x10);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00); 			
            WaitTime(5); 
            
            ReadTunerRegister(u32TunerPort,0x15,&buf);
            if((buf&0x40) != 0x40)
            {
                WriteTunerRegister(u32TunerPort,0x51, 0x0f);   
                WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
                WriteTunerRegister(u32TunerPort,0x50, 0x10);       
                WriteTunerRegister(u32TunerPort,0x50, 0x00);	
                WaitTime(5); 
            }
        }
        
        ReadTunerRegister(u32TunerPort,0x14, &buf); 
        buf &= 0x1f;		
        if(buf > 19)
        {
            ReadTunerRegister(u32TunerPort,0x10, &buf); 
            buf &= 0xfd;                         
            WriteTunerRegister(u32TunerPort,0x10, buf);
        }	
    }
    
    freqOffset  = (HI_S32)(divN  * TUNER_XTAL_FREQ  / (K + 8)/(div4 + 1) / 2  - fPLL * 1000);
    
    // set the RF gain
    
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        WriteTunerRegister(u32TunerPort,0x60,0x79);
    }
    
    WriteTunerRegister(u32TunerPort,0x51, 0x17);   
    WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
    WriteTunerRegister(u32TunerPort,0x50, 0x08);       
    WriteTunerRegister(u32TunerPort,0x50, 0x00); 	
    WaitTime(5); 
    
    ReadTunerRegister(u32TunerPort,0x3c,&buf);
    if(buf == 0x00)
    {
        WriteTunerRegister(u32TunerPort,0x51, 0x17);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x08);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00); 
        WaitTime(5); 
    }
    
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        
        ReadTunerRegister(u32TunerPort,0x3d,&buf);
        RFgain = buf & 0x0f;
        
        if(RFgain < 15)
        {
            
            if(RFgain < 4)  	RFgain = 0;
            else    			RFgain = RFgain - 3 ;
            
            buf = ((RFgain << 3) | 0x01) & 0x79;
            WriteTunerRegister(u32TunerPort,0x60, buf);
            
            WriteTunerRegister(u32TunerPort,0x51, 0x17);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x08);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00);	
            WaitTime(5); 
        }
    }
    
    // set the LPF
    
    if(gvTunerVersion == TUNER_M88TS2022)
    {
        
        WriteTunerRegister(u32TunerPort,0x25, 0x00);
        WriteTunerRegister(u32TunerPort,0x27, 0x70);	
        WriteTunerRegister(u32TunerPort,0x41, 0x09);
        WriteTunerRegister(u32TunerPort,0x08, 0x0b);
    }
    
    f3dB = fSym * 135 / 200 + 2000;     
        
    if(f3dB < 7000 )	f3dB = 7000;
    if(f3dB > 40000) 	f3dB = 40000;
    
    gdiv28 = ((TUNER_XTAL_FREQ / 1000) * 1694 + 500) / 1000; 
    
    buf = gdiv28;
    WriteTunerRegister(u32TunerPort,0x04, buf); 
    
    WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
    WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
    WriteTunerRegister(u32TunerPort,0x50, 0x04);       
    WriteTunerRegister(u32TunerPort,0x50, 0x00);  	
    WaitTime(2); 
    
    ReadTunerRegister(u32TunerPort,0x26,&buf);
    if(buf == 0x00)
    {
        WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x04);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00);
        WaitTime(2);
        
        ReadTunerRegister(u32TunerPort,0x26,&buf); 
    }
    
    capCode = buf & 0x3f;
    
    if(gvTunerVersion == TUNER_M88TS2022)
    {
        
        WriteTunerRegister(u32TunerPort,0x41, 0x0d);
        
        WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x04);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00); 		 
        WaitTime(2);  
        
        ReadTunerRegister(u32TunerPort,0x26,&buf);
        if(buf == 0x00)
        {
            WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x04);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00); 
            WaitTime(2);  
            
            ReadTunerRegister(u32TunerPort,0x26,&buf);
        }
        
        buf &= 0x3f;
        capCode = (capCode + buf) / 2;
    }
    
    gdiv28 = gdiv28 * 207 / (capCode * 2 + 151);   
    
    divMax = gdiv28 * 135 / 100 ;                           
    divMin = gdiv28 * 78 / 100 ;
    
    if(divMax > 63)	divMax=63;
    
    if(gvTunerVersion == TUNER_M88TS2020)
    {
        lpf_coeff = 2766;
    }
    
    if(gvTunerVersion == TUNER_M88TS2022)
    {
        lpf_coeff = 3200;
    }
    
    lpf_gm = (f3dB * gdiv28 * 2 / lpf_coeff / (TUNER_XTAL_FREQ / 1000)  + 1) / 2 ;
    
    if (lpf_gm > 23)  lpf_gm = 23;
    if (lpf_gm < 1)   lpf_gm = 1;
    
    lpf_mxdiv = (lpf_gm * (TUNER_XTAL_FREQ / 1000) * lpf_coeff * 2 / f3dB + 1) / 2;
    
    if (lpf_mxdiv < divMin)
    {
        lpf_gm++;
        lpf_mxdiv = (lpf_gm  * (TUNER_XTAL_FREQ / 1000) * lpf_coeff * 2  / f3dB + 1) / 2;
    }
    
    if (lpf_mxdiv > divMax)
    {
        lpf_mxdiv = divMax;
    }
    
    buf = lpf_mxdiv;
    WriteTunerRegister(u32TunerPort,0x04, buf); 
    
    buf = lpf_gm;
    WriteTunerRegister(u32TunerPort,0x06, buf);   
    
    WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
    WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
    WriteTunerRegister(u32TunerPort,0x50, 0x04);       
    WriteTunerRegister(u32TunerPort,0x50, 0x00); 	
    WaitTime(2);  
    
    ReadTunerRegister(u32TunerPort,0x26,&buf);
    if(buf == 0x00)
    {
        WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x04);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00);  		
        WaitTime(2);  
        
        ReadTunerRegister(u32TunerPort,0x26,&buf);
    }
    
    if(gvTunerVersion == TUNER_M88TS2022)
    {
        
        capCode = buf & 0x3f;
        
        WriteTunerRegister(u32TunerPort,0x41,0x09);
        
        WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x04);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00);		
        WaitTime(2); 
        
        ReadTunerRegister(u32TunerPort,0x26,&buf);
        if(buf == 0x00)
        {
            WriteTunerRegister(u32TunerPort,0x51, 0x1b);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x04);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00); 
            WaitTime(2);  
            ReadTunerRegister(u32TunerPort,0x26,&buf);
        }
        
        buf &= 0x3f;
        capCode = (capCode + buf) / 2;  
        
        buf = capCode | 0x80;
        WriteTunerRegister(u32TunerPort,0x25, buf);
        WriteTunerRegister(u32TunerPort,0x27, 0x30);	
        
        WriteTunerRegister(u32TunerPort,0x08, 0x09);		
    }
    
    // Set the BB gain
    // default should set gainHold = 0;
    // except when the AGC of demodulator is hold, for example application at blind scan use Haier demodulator
    
    //if(gainHold == 0)
    {
        
        WriteTunerRegister(u32TunerPort,0x51, 0x1e);   
        WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
        WriteTunerRegister(u32TunerPort,0x50, 0x01);       
        WriteTunerRegister(u32TunerPort,0x50, 0x00); 
        WaitTime(20); 	
        
        ReadTunerRegister(u32TunerPort,0x21,&buf);
        if(buf == 0x00)
        {
            WriteTunerRegister(u32TunerPort,0x51, 0x1e);   
            WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
            WriteTunerRegister(u32TunerPort,0x50, 0x01);       
            WriteTunerRegister(u32TunerPort,0x50, 0x00); 
            WaitTime(20);  
        }
        
        if(gvTunerVersion == TUNER_M88TS2020)
        {
            
            if(RFgain == 15)
            {
                
                WaitTime(20); 
                ReadTunerRegister(u32TunerPort,0x21,&buf);
                buf &= 0x0f; 
                
                if(buf < 3)
                {
                    
                    WriteTunerRegister(u32TunerPort,0x60, 0x61);
                    
                    WriteTunerRegister(u32TunerPort,0x51, 0x17);   
                    WriteTunerRegister(u32TunerPort,0x51, 0x1f);   
                    WriteTunerRegister(u32TunerPort,0x50, 0x08);       
                    WriteTunerRegister(u32TunerPort,0x50, 0x00);	
                    WaitTime(20);  
                }
            }
        }
        
        //User should delay 100ms here to wait the Tuner gain stable before checking the chip lock status
        //If there have delay time at the function of setting demodulator, you can take out it to reduce the lock time;
        
        WaitTime(60); 
    }
    
    return  HI_SUCCESS;	// return the frequency offset : KHz

}
#endif

