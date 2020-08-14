
/*
 Filename:      RDA5815S_3.X_dvbs.c
 Description:   RDA5815S Digital Satellite Tuner IC driver.
version 1.0    The primary version,  created by hongxin wang  2011-2-22
version 2.0    For B version chip ,  created by hongxin wang  2011-5-27
version 2.1    add register 0x1a for improving the sensitivity of 1550MHz
version 2.2    modify register 0x4a and 0x4b in initial both from 0xbb to 0x88 for improving pll stability 2011-06-08
version 2.3    add register 0x3c in initial for enhance the max level 2011-08-11
version 3.1     modify value of register 0x06 from 0x88 to 0xa8 for enlarge the vco level
                     modify value of register 0x4a from 0x88 to 0x48 for decrease presc_1 
                     modify value of register 0x4b from 0x88 to 0x58 for decrease presc_3 
                     modify value of register 0x15 from 0xaa to 0xae for enlarge buf_ibit_1   
                     modify value of register 0x72 from 0x60 to 0x20 and modify value of register 0x73 from 0x76 to 0x72  for change the  pll_sel_vco_freq_th from 1606MHz to 1552MHz
                     All the change is for pll stability and is necessary
                      2011-10-28
 version 3.2   modify value of register 0x57 from 0x74 to 0x64 for reducing ctsdm_gain2 from 10 to 00
                     modify value of register 0x65 from 0x80 to 0x00 for reducing DC offset
                     add register 0x41 in initial  for setting dc_cal_clk to 11
                     Both above change is use to enhance the stability of DC
                      2011-12-27 
  version 3.3   modify value of register 0x8e from 0x9a to 0xe0 
                     modify value of register 0x8f from  0x37 to 0x95 
                     modify value of register 0xb0 from 0x37 to 0x95 
                     modify value of register 0xb1 from 0x37 to 0x95 
                     modify value of register 0xb2 from 0x37 to 0x95 
                     Both above change is use to enlarge the gain 12dB

                     Modify the max bandwidth from 45MHz to 40MHz
                     add register 0x2b in the rda5815Set  for  improving the pll stability
                      2012-1-12            
version 3.4     modify value of register 0x38 from 0x90 to 0x93 for improving the high temperature performance
                      2012-4-17 
                      
*/
#include "hi_type.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "drv_demod.h"
#include "RDA5815.h"

#include <common.h>

HI_VOID RDA5815Sleep(HI_U32 u32Ms)
{
    msleep_interruptible(10 * u32Ms);
}

HI_VOID RDA5815WriteReg(HI_U32 u32TunerPort, HI_U8 u8RegAddress, HI_U8 u8RegData)
{
    HI_U8 enI2cChannel = 0;
    TUNER_I2C_DATA_S stI2cData = {0};
    HI_U8 au8Data[2] = {0};
    HI_U8 u8DevAddr = 0;

    u8DevAddr = g_stTunerOps[u32TunerPort].u32TunerAddress;

    au8Data[0] = u8RegAddress;
    au8Data[1] = u8RegData;

    enI2cChannel = g_stTunerOps[u32TunerPort].enI2cChannel;
    stI2cData.pu8ReceiveBuf = 0;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = au8Data;
    stI2cData.u32SendLength = 2;


    hi3136_config_i2c_out(u32TunerPort, 1);

    tuner_i2c_send_data(enI2cChannel, u8DevAddr, &stI2cData);
}
    
HI_S32 RDA5815_init_tuner(HI_U32 u32TunerPort)
{    
    //RDA5815WriteReg(register address,register data); 
    RDA5815Sleep(1);//Wait 1ms. 
    // Chip register soft reset 	
    RDA5815WriteReg(u32TunerPort, 0x04, 0x04);
    RDA5815WriteReg(u32TunerPort, 0x04, 0x05); 
    
    // Initial configuration start
    
    //pll setting 
    RDA5815WriteReg(u32TunerPort, 0x1a, 0x13);   //add by rda 2011.5.28
    RDA5815WriteReg(u32TunerPort, 0x41, 0x53);  //add by rda 2011.12.27
    RDA5815WriteReg(u32TunerPort, 0x38, 0x93); //modify by rda 2012.04.17
    RDA5815WriteReg(u32TunerPort, 0x39, 0x15); 
    RDA5815WriteReg(u32TunerPort, 0x3A, 0x00);
    RDA5815WriteReg(u32TunerPort, 0x3B, 0x00);
    RDA5815WriteReg(u32TunerPort, 0x3C, 0x0c);  //add by rda 2011.8.9
    RDA5815WriteReg(u32TunerPort, 0x0c, 0xE2);
    RDA5815WriteReg(u32TunerPort, 0x2e, 0x6F);
    RDA5815WriteReg(u32TunerPort, 0x72, 0x07);
    RDA5815WriteReg(u32TunerPort, 0x73, 0x20);//modify by rda 2011.10.28
    RDA5815WriteReg(u32TunerPort, 0x74, 0x72);//modify by rda 2011.10.28
    RDA5815WriteReg(u32TunerPort, 0x5b, 0x20);
    RDA5815WriteReg(u32TunerPort, 0x2f, 0x57);
    RDA5815WriteReg(u32TunerPort, 0x0d, 0x70);
    RDA5815WriteReg(u32TunerPort, 0x16, 0x03);
    RDA5815WriteReg(u32TunerPort, 0x18, 0x4B);
    RDA5815WriteReg(u32TunerPort, 0x30, 0xFF);
    RDA5815WriteReg(u32TunerPort, 0x5c, 0xFF);
    RDA5815WriteReg(u32TunerPort, 0x6c, 0xFF);
    RDA5815WriteReg(u32TunerPort, 0x6e, 0xFF);
    RDA5815WriteReg(u32TunerPort, 0x65, 0x00);//modify by rda 2011.12.27
    RDA5815WriteReg(u32TunerPort, 0x70, 0x3F);
    RDA5815WriteReg(u32TunerPort, 0x71, 0x3F);
    RDA5815WriteReg(u32TunerPort, 0x75, 0x06);
    RDA5815WriteReg(u32TunerPort, 0x76, 0x40);
    RDA5815WriteReg(u32TunerPort, 0x77, 0x89);
    RDA5815WriteReg(u32TunerPort, 0x53, 0xA8);
    RDA5815WriteReg(u32TunerPort, 0x46, 0x21);
    RDA5815WriteReg(u32TunerPort, 0x47, 0x84);
    RDA5815WriteReg(u32TunerPort, 0x48, 0x10);
    RDA5815WriteReg(u32TunerPort, 0x49, 0x08);
    RDA5815WriteReg(u32TunerPort, 0x60, 0x80);
    RDA5815WriteReg(u32TunerPort, 0x61, 0x80);
    RDA5815WriteReg(u32TunerPort, 0x6A, 0x08);
    RDA5815WriteReg(u32TunerPort, 0x6B, 0x63);
    RDA5815WriteReg(u32TunerPort, 0x69, 0xF8);
    RDA5815WriteReg(u32TunerPort, 0x57, 0x64);//modify by rda 2011.12.27
    RDA5815WriteReg(u32TunerPort, 0x05, 0x88);
    RDA5815WriteReg(u32TunerPort, 0x06, 0xa8);//modify by rda 2011.10.28
    RDA5815WriteReg(u32TunerPort, 0x15, 0xAE);//modify by rda 2011.10.28
    RDA5815WriteReg(u32TunerPort, 0x4a, 0x48);//modify by rda 2011.10.28
    RDA5815WriteReg(u32TunerPort, 0x4b, 0x58);//modify by rda 2011.10.28
    
    
    
    //agc setting
    
    RDA5815WriteReg(u32TunerPort, 0x4f, 0x40);
    RDA5815WriteReg(u32TunerPort, 0x5b, 0x20);
    
    // for blocker
    RDA5815WriteReg(u32TunerPort, 0x16, 0x10);//stage setting
    RDA5815WriteReg(u32TunerPort, 0x18, 0x20);            
    RDA5815WriteReg(u32TunerPort, 0x30, 0x30);            
    RDA5815WriteReg(u32TunerPort, 0x5c, 0x30);            
    RDA5815WriteReg(u32TunerPort, 0x6c, 0x30);            
    RDA5815WriteReg(u32TunerPort, 0x6e, 0x70);            
    RDA5815WriteReg(u32TunerPort, 0x1b, 0xB2);            
    RDA5815WriteReg(u32TunerPort, 0x1d, 0xB2);            
    RDA5815WriteReg(u32TunerPort, 0x1f, 0xB2);            
    RDA5815WriteReg(u32TunerPort, 0x21, 0xB2);            
    RDA5815WriteReg(u32TunerPort, 0x23, 0xB6);            
    RDA5815WriteReg(u32TunerPort, 0x25, 0xB6);            
    RDA5815WriteReg(u32TunerPort, 0x27, 0xBA);            
    RDA5815WriteReg(u32TunerPort, 0x29, 0xBF);            
    RDA5815WriteReg(u32TunerPort, 0xb3, 0xFF);            
    RDA5815WriteReg(u32TunerPort, 0xb5, 0xFF);            
    
    RDA5815WriteReg(u32TunerPort, 0x17, 0xF0);            
    RDA5815WriteReg(u32TunerPort, 0x19, 0xF0);            
    RDA5815WriteReg(u32TunerPort, 0x31, 0xF0);            
    RDA5815WriteReg(u32TunerPort, 0x5d, 0xF1);            
    RDA5815WriteReg(u32TunerPort, 0x6d, 0xF2);            
    RDA5815WriteReg(u32TunerPort, 0x6f, 0xF2);            
    RDA5815WriteReg(u32TunerPort, 0x1c, 0x31);            
    RDA5815WriteReg(u32TunerPort, 0x1e, 0x72);            
    RDA5815WriteReg(u32TunerPort, 0x20, 0x96);            
    RDA5815WriteReg(u32TunerPort, 0x22, 0xBA);            
    RDA5815WriteReg(u32TunerPort, 0x24, 0xBA);            
    RDA5815WriteReg(u32TunerPort, 0x26, 0xBE);            
    RDA5815WriteReg(u32TunerPort, 0x28, 0xCE);            
    RDA5815WriteReg(u32TunerPort, 0x2a, 0xDE);            
    RDA5815WriteReg(u32TunerPort, 0xb4, 0x0F);            
    RDA5815WriteReg(u32TunerPort, 0xb6, 0x0F);            
    
    RDA5815WriteReg(u32TunerPort, 0xb7, 0x10);	//start    
    RDA5815WriteReg(u32TunerPort, 0xb9, 0x10);	           
    RDA5815WriteReg(u32TunerPort, 0xbb, 0x00);	           
    RDA5815WriteReg(u32TunerPort, 0xbd, 0x00);	           
    RDA5815WriteReg(u32TunerPort, 0xbf, 0x00);	           
    RDA5815WriteReg(u32TunerPort, 0xc1, 0x10);	           
    RDA5815WriteReg(u32TunerPort, 0xc3, 0x10);	           
    RDA5815WriteReg(u32TunerPort, 0xc5, 0x10);	           
    RDA5815WriteReg(u32TunerPort, 0xa3, 0x19);	           
    RDA5815WriteReg(u32TunerPort, 0xa5, 0x2E);	           
    RDA5815WriteReg(u32TunerPort, 0xa7, 0x37);	           
    RDA5815WriteReg(u32TunerPort, 0xa9, 0x47);	           
    RDA5815WriteReg(u32TunerPort, 0xab, 0x47);            
    RDA5815WriteReg(u32TunerPort, 0xad, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xaf, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0xb1, 0x95); //modify by rda 2012.1.12              
    
    
    RDA5815WriteReg(u32TunerPort, 0xb8, 0x47); //end      
    RDA5815WriteReg(u32TunerPort, 0xba, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xbc, 0x37);            
    RDA5815WriteReg(u32TunerPort, 0xbe, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xc0, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xc2, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xc4, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xc6, 0x3F);            
    RDA5815WriteReg(u32TunerPort, 0xa4, 0x47);            
    RDA5815WriteReg(u32TunerPort, 0xa6, 0x57);            
    RDA5815WriteReg(u32TunerPort, 0xa8, 0x5F);            
    RDA5815WriteReg(u32TunerPort, 0xaa, 0x70);            
    RDA5815WriteReg(u32TunerPort, 0xac, 0x70);            
    RDA5815WriteReg(u32TunerPort, 0xae, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0xb0, 0x95); //modify by rda 2012.1.12              
    RDA5815WriteReg(u32TunerPort, 0xb2, 0x95); //modify by rda 2012.1.12              
    
    
    RDA5815WriteReg(u32TunerPort, 0x81, 0x77); //rise     
    RDA5815WriteReg(u32TunerPort, 0x82, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0x83, 0x70);            
    RDA5815WriteReg(u32TunerPort, 0x84, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0x85, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0x86, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0x87, 0x70);            
    RDA5815WriteReg(u32TunerPort, 0x88, 0x47);            
    RDA5815WriteReg(u32TunerPort, 0x89, 0x68);            
    RDA5815WriteReg(u32TunerPort, 0x8a, 0x8E);            
    RDA5815WriteReg(u32TunerPort, 0x8b, 0x8E);            
    RDA5815WriteReg(u32TunerPort, 0x8c, 0x8E);            
    RDA5815WriteReg(u32TunerPort, 0x8d, 0x9C);            
    RDA5815WriteReg(u32TunerPort, 0x8e, 0xe0); //modify by rda 2012.1.12           
    RDA5815WriteReg(u32TunerPort, 0x8f, 0x95); //modify by rda 2012.1.12               
    
    RDA5815WriteReg(u32TunerPort, 0x90, 0x00); //fall     
    RDA5815WriteReg(u32TunerPort, 0x91, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x92, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x93, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x94, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x95, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x96, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x97, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x98, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x99, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x9a, 0x10);            
    RDA5815WriteReg(u32TunerPort, 0x9b, 0x24);            
    RDA5815WriteReg(u32TunerPort, 0x9c, 0x10);            
    RDA5815WriteReg(u32TunerPort, 0x9d, 0x00);            
    RDA5815WriteReg(u32TunerPort, 0x9e, 0x00);            
    
    
    // for SNAR
    /*
    RDA5815WriteReg(0x16,0x10);//stage setting
    RDA5815WriteReg(0x18,0x20);
    RDA5815WriteReg(0x30,0x30);
    RDA5815WriteReg(0x5c,0x30);
    RDA5815WriteReg(0x6c,0x30);
    RDA5815WriteReg(0x6e,0x32);
    RDA5815WriteReg(0x1b,0x32);
    RDA5815WriteReg(0x1d,0x36);
    RDA5815WriteReg(0x1f,0x3A);
    RDA5815WriteReg(0x21,0x3E);
    RDA5815WriteReg(0x23,0x3E);
    RDA5815WriteReg(0x25,0x3F);
    RDA5815WriteReg(0x27,0x7F);
    RDA5815WriteReg(0x29,0xBF);
    RDA5815WriteReg(0xb3,0xFF);
    RDA5815WriteReg(0xb5,0xFF);
    
    RDA5815WriteReg(0x17,0xF0);
    RDA5815WriteReg(0x19,0xF0);
    RDA5815WriteReg(0x31,0xF0);
    RDA5815WriteReg(0x5d,0xF1);
    RDA5815WriteReg(0x6d,0xF2);
    RDA5815WriteReg(0x6f,0xF2);
    RDA5815WriteReg(0x1c,0x3A);
    RDA5815WriteReg(0x1e,0x7A);
    RDA5815WriteReg(0x20,0x9A);
    RDA5815WriteReg(0x22,0xBA);
    RDA5815WriteReg(0x24,0xBE);
    RDA5815WriteReg(0x26,0xBE);
    RDA5815WriteReg(0x28,0xCE);
    RDA5815WriteReg(0x2a,0xDE);
    RDA5815WriteReg(0xb4,0x0F);
    RDA5815WriteReg(0xb6,0x0F);
    
    RDA5815WriteReg(0xb7,0x10);	//start
    RDA5815WriteReg(0xb9,0x10);	
    RDA5815WriteReg(0xbb,0x00);	
    RDA5815WriteReg(0xbd,0x00);	
    RDA5815WriteReg(0xbf,0x00);	
    RDA5815WriteReg(0xc1,0x10);	
    RDA5815WriteReg(0xc3,0x10);	
    RDA5815WriteReg(0xc5,0x10);	
    RDA5815WriteReg(0xa3,0x19);	
    RDA5815WriteReg(0xa5,0x2E);	
    RDA5815WriteReg(0xa7,0x37);	
    RDA5815WriteReg(0xa9,0x47);	
    RDA5815WriteReg(0xab,0x47);  
    RDA5815WriteReg(0xad,0x3F);  
    RDA5815WriteReg(0xaf,0x00);
    RDA5815WriteReg(0xb1,0x37);
    
    
    RDA5815WriteReg(0xb8,0x47); //end
    RDA5815WriteReg(0xba,0x3F);
    RDA5815WriteReg(0xbc,0x37);             
    RDA5815WriteReg(0xbe,0x3F);             
    RDA5815WriteReg(0xc0,0x3F);             
    RDA5815WriteReg(0xc2,0x3F);             
    RDA5815WriteReg(0xc4,0x3F);             
    RDA5815WriteReg(0xc6,0x3F);             
    RDA5815WriteReg(0xa4,0x47);             
    RDA5815WriteReg(0xa6,0x57);             
    RDA5815WriteReg(0xa8,0x5F);             
    RDA5815WriteReg(0xaa,0x70);             
    RDA5815WriteReg(0xac,0x70);             
    RDA5815WriteReg(0xae,0x68);             
    RDA5815WriteReg(0xb0,0x37);             
    RDA5815WriteReg(0xb2,0x37);             
    
    
    RDA5815WriteReg(0x81,0x77); //rise 
    RDA5815WriteReg(0x82,0x68);         
    RDA5815WriteReg(0x83,0x70);         
    RDA5815WriteReg(0x84,0x68);         
    RDA5815WriteReg(0x85,0x70);         
    RDA5815WriteReg(0x86,0x70);         
    RDA5815WriteReg(0x87,0x68);         
    RDA5815WriteReg(0x88,0x68);         
    RDA5815WriteReg(0x89,0x70);         
    RDA5815WriteReg(0x8a,0x7F);         
    RDA5815WriteReg(0x8b,0x8E);         
    RDA5815WriteReg(0x8c,0x8E);         
    RDA5815WriteReg(0x8d,0x8E);         
    RDA5815WriteReg(0x8e,0x9A);         
    RDA5815WriteReg(0x8f,0x37);         
    
    RDA5815WriteReg(0x90,0x00); //fall    
    RDA5815WriteReg(0x91,0x00);           
    RDA5815WriteReg(0x92,0x00);           
    RDA5815WriteReg(0x93,0x00);           
    RDA5815WriteReg(0x94,0x00);           
    RDA5815WriteReg(0x95,0x00);           
    RDA5815WriteReg(0x96,0x00);           
    RDA5815WriteReg(0x97,0x00);           
    RDA5815WriteReg(0x98,0x00);           
    RDA5815WriteReg(0x99,0x00);           
    RDA5815WriteReg(0x9a,0x10);           
    RDA5815WriteReg(0x9b,0x19);           
    RDA5815WriteReg(0x9c,0x10);           
    RDA5815WriteReg(0x9d,0x00);           
    RDA5815WriteReg(0x9e,0x00);           
    */  
    
    RDA5815Sleep(10);//Wait 10ms; 

    return HI_SUCCESS;
    // Initial configuration end
}                      
	                       
	 

	
	
	/*************************************************************************/
//	Function to Set the RDA5815                       
//	fPLL:   Frequency        			unit: MHz  from 250 to 2300 
//	fSym:   SymbolRate       			unit: KS/s from 1000 to 45000 
/************************************************************************/

HI_S32 RDA5815_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF/*kHz*/)  //freq :unit MHZ, symbol:unit  Khz
//INT32 RDA5815Set(UINT32 fPLL, UINT32 fSym )
{			                   
    HI_U8 u8Buffer; 
    HI_U32 u32Temp_value = 0;
    HI_U32 u32BW;
    HI_U8 u8Filter_bw_control_bit;	
    HI_U32 fPLL;
    HI_U32 fSym;
    
    fPLL = u32PuRF / 1000;
    fSym = g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32SymbolRate / 1000;

    RDA5815WriteReg(u32TunerPort, 0x04, 0xc1); //add by rda 2011.8.9,RXON = 0 , change normal working state to idle state
    RDA5815WriteReg(u32TunerPort, 0x2b, 0x95);//clk_interface_27m=0  add by rda 2012.1.12     
    //set frequency start
    u32Temp_value = (HI_U32)fPLL* 77672;//((2<<21) / RDA5815_XTALFREQ);
    
    u8Buffer = ((HI_U8)((u32Temp_value >> 24) & 0xff));
    RDA5815WriteReg(u32TunerPort, 0x07, u8Buffer);
    u8Buffer = ((HI_U8)((u32Temp_value >> 16) & 0xff));	
    RDA5815WriteReg(u32TunerPort, 0x08, u8Buffer);	
    u8Buffer = ((HI_U8)((u32Temp_value >> 8) & 0xff));
    RDA5815WriteReg(u32TunerPort, 0x09, u8Buffer);	
    u8Buffer = ((HI_U8)( u32Temp_value & 0xff));
    RDA5815WriteReg(u32TunerPort, 0x0a, u8Buffer);
    //set frequency end
    
    // set Filter bandwidth start
    u32BW = fSym;
    
    if(u32BW < 4000)
    u32BW = 4000;    // KHz
    else if(u32BW > 45000)
    u32BW = 40000;   // KHz    //modify by rda 2012.1.12   
    
    u8Filter_bw_control_bit = (HI_U8)((u32BW * 135 / 200 + 4000) / 1000);
    
    u8Filter_bw_control_bit &= 0x3f;
    RDA5815WriteReg(u32TunerPort, 0x0b, u8Filter_bw_control_bit);
    // set Filter bandwidth end
    
    RDA5815WriteReg(u32TunerPort, 0x04, 0xc3); //add by rda 2011.8.9,RXON = 0 ,rxon=1,normal working
    RDA5815WriteReg(u32TunerPort, 0x2b, 0x97);//clk_interface_27m=1  add by rda 2012.1.12  
    RDA5815Sleep(5);//Wait 5ms;
    
    return HI_SUCCESS;   
}


