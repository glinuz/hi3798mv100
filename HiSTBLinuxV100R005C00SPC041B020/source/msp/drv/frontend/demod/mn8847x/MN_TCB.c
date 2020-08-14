/* **************************************************** */
/*!
   @file	MN_DMD_device.c
   @brief	Device dependence functions for MN88472
   @author	R.Mori
   @date	2011/7/6
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#include "MN_DMD_driver.h"
#include "MN_DMD_common.h"
#include "MN_DMD_device.h"
#include "MN_I2C.h"
#include "MN_TCB.h"
#include "MN8847x.h" 

#if (TUNER_TYPE == NXP18275)
#include "tmNxTypes.h"
/* **************************************************** */
/* Tuner slave address */
/* **************************************************** */
#if (TUNER_I2C_MODE == TUNER_CONTROL_BY_SOC) 
     #define NXP18275_SLAVE_ADDRESS 0x60 //for SoC to control Tuner directly 
#else 
   #if (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_REPEAT_MODE)
    #define NXP18275_SLAVE_ADDRESS 0xC0//(0x60<<1), for Demod to control tuner, slave address treated as normal data -- Repeator mode 
   #else
    #define NXP18275_SLAVE_ADDRESS 0x60//for Demod bypass mode
   #endif
#endif
// Tuner be controlled by Demod , if by BE/Host, pls. use standard I2C communication function.
tmErrorCode_t UserWrittenWrite(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 WriteLen, UInt8* pData)
{
	DMD_ERROR_t	ret = DMD_E_OK;

#if(TUNER_I2C_MODE == TUNER_CONTROL_BY_SOC)
	MN8847X_DMD_I2C_WriteRead(NXP18275_SLAVE_ADDRESS ,*pAddr , pData , WriteLen , 0, 0 );
#elif (TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)
	MN8847X_DMD_TCB_WriteRead(NXP18275_SLAVE_ADDRESS ,*pAddr , pData , WriteLen , 0, 0 );	
#endif //#if(TUNER_I2C_MODE == TUNER_CONTROL_BY_SOC)
	
	return ret;
}

// Tuner be controlled by Demod , if by BE/Host, pls. use standard I2C communication function.
tmErrorCode_t UserWrittenRead(tmUnitSelect_t tUnit, UInt32 AddrSize, UInt8* pAddr, UInt32 ReadLen, UInt8* pData)
{    
	DMD_ERROR_t	ret = DMD_E_OK;

#if(TUNER_I2C_MODE == TUNER_CONTROL_BY_SOC)
#ifdef PC_CONTROL_FE 
#ifdef PANA_I2C_CONTROL
	MN8847X_DMD_I2C_WriteRead(NXP18275_SLAVE_ADDRESS, *pAddr, 0, 0, pData, 1);	
#endif //#ifdef PANA_I2C_CONTROL
#else /*=============== Controlled by customer's SoC ================*/
   MN8847X_DMD_I2C_Read(NXP18275_SLAVE_ADDRESS, *pAddr,pData);
#endif//#ifdef PC_CONTROL_FE 
#elif (TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)
	#if (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_REPEAT_MODE)
	MN8847X_DMD_TCB_WriteRead(NXP18275_SLAVE_ADDRESS , *pAddr, 0, 0, 0, 0); 
	MN8847X_DMD_TCB_WriteRead(NXP18275_SLAVE_ADDRESS , 0, 0, 0, pData, ReadLen); 
	#elif (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_BYPASS_MODE)
	MN8847X_DMD_TCB_WriteRead(NXP18275_SLAVE_ADDRESS , *pAddr, 0, 0, pData, ReadLen); 
	#endif
#endif //#if(TUNER_I2C_MODE == TUNER_CONTROL_BY_SOC)

	return ret;
}

tmErrorCode_t UserWrittenWait(tmUnitSelect_t FrontEndUnit, UInt32 tms)
{	    
     return MN8847X_DMD_wait(tms); 
}

tmErrorCode_t UserWrittenPrint(UInt32 level, const char* format, ...)
{
    return DMD_E_OK; //To be added. 
}
#endif //#if (TUNER_TYPE == NXP18275)

#if (TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)
/* **************************************************** */
/*! Write 1byte to Tuner via Demodulator */
/* **************************************************** */

DMD_ERROR_t MN8847X_DMD_TCB_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t data )
{
	return MN8847X_DMD_TCB_WriteRead( slvadr , adr , &data , 1 , 0, 0 );
}

/* **************************************************** */
/*! Read 1byte from Tuner via Demodulator */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_TCB_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t *data )
{
	return MN8847X_DMD_TCB_WriteRead( slvadr , adr ,  0, 0 , data , 1 );
}

/* '11/08/05 : OKAMOTO Implement "Through Mode". */
DMD_ERROR_t MN8847X_DMD_TCB_WriteAnyLength(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* data , DMD_u32_t wlen)
{
	return MN8847X_DMD_TCB_WriteRead( slvadr , adr , data , wlen , 0, 0 );
}

/* '11/08/05 : OKAMOTO Implement "Through Mode". */
DMD_ERROR_t MN8847X_DMD_TCB_ReadAnyLength(DMD_u8_t	slvadr , DMD_u8_t *data  , DMD_u8_t rlen)
{
	return MN8847X_DMD_TCB_WriteRead( slvadr , 0 ,  0, 0 , data , rlen );
}


/* ******************************************************/
/*   [ --- WARNING--- ] 
This file indicates "Tuner BUS Controll by Demodulator"  
If you use SoC to control Tuner directly, 
pls. use functionsin MN_I2C.c, as same as communicating with Demodulator 
*/
/* ******************************************************/
//------ Tuner I2C read protocal-------
// [Note] Take Mxl603 for example; normally, no need 0xFB.
// 8 bit Register Read Protocol:
//Step1
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |MASTER|S|SADDR|W| |0xFB| |RegAddr    | |P|
  // +------+-+-----+-+-+----+-+----------+-+-+
  // |SLAVE |         |A|    |A|          |A| |
  // +------+-+-----+-+-+----+-+----------+-+-+ 
//Step2
  // +------+-+-----+-+-+-----+--+-+
  // |MASTER|S|SADDR|R| |     |MN|P|
  // +------+-+-----+-+-+-----+--+-+
  // |SLAVE |         |A|Data |  | |
  // +------+---------+-+-----+--+-+
  // SADDR(I2c slave address), S(Start condition), 
  // A(Slave Ack), MN(Master NACK),  P(Stop condition)

/* **************************************************** */
/*! Write&Read any length from/to Tuner via Demodulator : Repeat mode */
/* **************************************************** */
#if (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_REPEAT_MODE)
DMD_ERROR_t MN8847X_DMD_TCB_WriteRead(DMD_u8_t tuner_slvadr , DMD_u8_t tuner_reg_adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	DMD_u8_t	d[DMD_TCB_DATA_MAX];
	DMD_u32_t i;
	DMD_ERROR_t	ret = DMD_E_OK;
	
/* Set TCB Through Mode */ //Troy.wangyx masked, 20120801, once is enough; already done after initialization.(fixed in register array)
//	ret  = DMD_I2C_MaskWrite( DMD_BANK_T2_ , DMD_TCBSET , 0x7f , 0x53 ); //should be 0xD3 : Don't trigger an automatic demod reset when performing a write to the tuner in bridge mode
//	ret |= DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBADR , 0x00 );

	if( (wlen == 0 && rlen == 0) || (wlen != 0) ) //I2C consecutive write; wlen = 0, means tuner_read "step1", as noted above. 
	{
		d[0] = tuner_slvadr & 0xFE; // bit0 : 0 -> I2C write cmd
		d[1] = tuner_reg_adr; 
		for(i=0;i<wlen;i++)	d[i+2] = wdata[i];
				
		ret |= MN8847X_DMD_I2C_WriteRead(DMD_BANK_T2_ , DMD_TCBCOM , d , wlen + 2 , rdata , rlen ); //beware the address to write is FIXed, not Increased.
	}
	else //I2C consecutive read, this part means tuner_read "step2", as noted above. 
	{	
		d[0] = tuner_slvadr | 1;
		ret |= MN8847X_DMD_I2C_WriteRead(DMD_BANK_T2_ , DMD_TCBCOM , d , 1 , rdata , rlen );//beware the address to read is FIXed, not Increased.
	}


	return ret;
}	
#elif (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_BYPASS_MODE)
/* **************************************************** */
/*! Write&Read any length from/to Tuner : Bypass mode, control Tuner by SoC directly  */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_TCB_WriteRead(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen)
{
	DMD_ERROR_t ret = DMD_E_OK;

	//------ Set to bypass Mode
	//ret |= DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBSET , 0xe3 );
	ret |= MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBCOM , 0x1 );
	
	//Example. Customer can communicate Tuner directly.
	//####################################################################
	MN8847X_DMD_I2C_WriteRead(slvadr , adr, wdata, wlen, rdata, rlen);
	//####################################################################

	//------ bypass mode off
	ret |= MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBCOM , 0x0 );// need off everytime, after off, SoC can communicate with Demod 
	
	return ret;
}	
#else
#error  "pls.define TCB_I2C_mode "
#endif

#endif //#if (TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)

