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


#include "MN_DMD_common.h"
#include "MN_DMD_driver.h"
#include "MN_DMD_device.h"
#include "MN_I2C.h"
#include "MN8847x.h" 
#include "MN_Tuner.h"

/* **************************************************** */
/*	Constant
 * **************************************************** */
#define	DMD_DVBT_LOCKED_STATUS	10
#define	DMD_DVBT2_LOCKED_STATUS	13
#define	DMD_DVBC_LOCKED_STATUS	8

#define DMD_L1_BANK_SIZE 11
#define DMD_L1_REG_SIZE   8

/* '11/08/01 : OKAMOTO	Correct Error. */
#define DMD_ERROR_STATUS 0xFFFFFFFF


/* **************************************************** */
/*	Local Functions 
 * **************************************************** */
DMD_u32_t	MN8847X_DMD_RegRev(void);
DMD_u32_t	MN8847X_DMD_PSEQRev(void);
DMD_u32_t	MN8847X_DMD_AGC(void);
DMD_u32_t	MN8847X_DMD_System(void);
DMD_u32_t	MN8847X_DMD_Status(DMD_SYSTEM_t sys);			//SSEQ Status
DMD_u32_t	MN8847X_DMD_BER( DMD_SYSTEM_t sys , DMD_u32_t *err , DMD_u32_t *sum);
DMD_u32_t	MN8847X_DMD_BER_dvbt2( DMD_u32_t* err , DMD_u32_t* sum  , DMD_u32_t common );
DMD_u32_t	MN8847X_DMD_PER( DMD_SYSTEM_t sys , DMD_u32_t *err , DMD_u32_t *sum);
DMD_u32_t	MN8847X_DMD_CNR( DMD_SYSTEM_t sys , DMD_u32_t *cnr_i,DMD_u32_t *cnr_d);

/*! Information */
DMD_ERROR_t MN8847X_DMD_information_dvbt( DMD_PARAMETER_t *param , DMD_u32_t id , DMD_u32_t allflag);
DMD_ERROR_t MN8847X_DMD_information_dvbt2( DMD_PARAMETER_t *param , DMD_u32_t id, DMD_u32_t allflag);
DMD_ERROR_t MN8847X_DMD_information_dvbc( DMD_PARAMETER_t *param , DMD_u32_t id);
DMD_u32_t	MN8847X_DMD_get_l1( DMD_u8_t l1info[11][8] , DMD_u32_t	bitw , DMD_u32_t bank , DMD_u32_t adr , DMD_u32_t pos , DMD_u32_t allflag);
DMD_ERROR_t MN8847X_DMD_get_l1all( DMD_u8_t l1info[11][8] );

/* Scan ( for Channel Search )*/
DMD_ERROR_t MN8847X_DMD_scan_dvbt2( DMD_PARAMETER_t *param );
DMD_ERROR_t MN8847X_DMD_scan_dvbt( DMD_PARAMETER_t *param );
DMD_ERROR_t MN8847X_DMD_scan_dvbc( DMD_PARAMETER_t *param );

/* **************************************************** */
/*	Functions
 * **************************************************** */

DMD_ERROR_t	MN8847X_DMD_system_support( DMD_SYSTEM_t sys )
{
	DMD_ERROR_t	ret;

	switch( sys )
	{
	case DMD_E_DVBT:
	case DMD_E_DVBT2:
	case DMD_E_DVBC:
		ret = DMD_E_OK;
		break;
	case DMD_E_ISDBT:
	case DMD_E_ISDBS:
	case DMD_E_DVBC2:
	case DMD_E_ATSC:
	case DMD_E_QAMB_64QAM:
	case DMD_E_QAMB_256QAM:
	case DMD_E_QAMC_64QAM:
	case DMD_E_QAMC_256QAM:
	case DMD_E_NTSC_M_BTSC:
	case DMD_E_PAL_M_BTSC:
	case DMD_E_PAL_N_BTSC:
	case DMD_E_PAL_B_G_NICAM:
	case DMD_E_PAL_I_NiCAM:
	case DMD_E_PAL_D_NiCAM:
	case DMD_E_PAL_B_G_A2:
	case DMD_E_SECAM_L_NiCAM:
	case DMD_E_SECAM_D_K_A2:
	default:
		ret = DMD_E_ERROR;
		break;
	}

	return ret;
}


/* **************************************************** */
/*! Read chip id */  
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_read_chipid( DMD_u8_t*	data )
{
	DMD_ERROR_t ret = DMD_E_OK;   
	ret = MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_CHIPRD, data);
	return ret;
}

/* **************************************************** */
/*! Load Auto Control Sequence */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_load_pseq( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t ret;
	DMD_u8_t	data;
	DMD_u32_t count = 0;
	
	MN8847X_DMD_device_read_chipid(&data);
		
	/* Load PSEQ Program */
	ret  = MN8847X_DMD_I2C_Write(   DMD_BANK_T_, DMD_PSEQCTRL , 0x20 );
	ret |= MN8847X_DMD_I2C_Write(   DMD_BANK_T_, DMD_PSEQSET  , 0x03 );
	
//to prevent potential risk that writing PSEQ to DMD wrongly, because some SoC cannot support large bytes writting at one time.
//If your SoC already matched with MN88472, no need to modify.
#if 1 
	if (data == DMD_TYPE_MN88472)
	{
#if(DEMOD_TYPE == DMD_TYPE_MN88472) 
		for ( count = 0; count < MN88472_REG_AUTOCTRL_SIZE; count++)
		{
		MN8847X_DMD_I2C_Write(  DMD_BANK_T_, DMD_PSEQPRG, MN88472_REG_AUTOCTRL[count] );
		}
#endif
	}
	else if (data == DMD_TYPE_MN88473)
	{
		for ( count = 0; count < MN88473_REG_AUTOCTRL_SIZE; count++)
		{
		MN8847X_DMD_I2C_Write(  DMD_BANK_T_, DMD_PSEQPRG, MN88473_REG_AUTOCTRL[count] );
		}
	}
	else
	{
	    DMD_DBG_TRACE( "ERROR: CHIPID ERROR!! \n" );
	}
#else
		//ret |= MN8847X_DMD_I2C_WriteRead( DMD_BANK_T_, DMD_PSEQPRG  , MN88472_REG_AUTOCTRL , MN88472_REG_AUTOCTRL_SIZE , 0, 0 );
#endif

	ret |= MN8847X_DMD_I2C_Write(   DMD_BANK_T_, DMD_PSEQSET  , 0x00 );

	/* Check Parity bit */
	ret |= MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PSEQFLG , &data);
	if( data & 0x10 )
	{
		DMD_DBG_TRACE( "ERROR: PSEQ Parity" ); //sometimes it has no function to judge the PSEQ write procedure successes or not...|||
		ret |= DMD_E_ERROR;
	}

	return ret;
}

/* **************************************************** */
/*! Check Auto Control Runs or not */
// PSEQSET bit0 -> 0 : PSEQ running; 1 : PSEQ hold. 
// Troy.wang added, 20130412 
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_pseq_status( DMD_u8_t* pseq_status )
{
	DMD_ERROR_t ret;
	DMD_u8_t	data;
	ret = MN8847X_DMD_I2C_Read(   DMD_BANK_T_, DMD_PSEQSET  , &data );
	
	*pseq_status = data&0x01;
    return ret;	
}

/* **************************************************** */
/*! Device Open - wake up*/
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_open( DMD_PARAMETER_t* param )
{

		DMD_ERROR_t ret = DMD_E_OK;
		ret = MN8847X_DMD_set_power_mode(DMD_PWR_MODE_NORMAL);
		return ret;
}
/* **************************************************** */
/*! Device Close - Standby mode : Can reduce power consumption by about 300mW */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_close(  DMD_PARAMETER_t* param )
{
		DMD_ERROR_t ret = DMD_E_OK;
	    ret = MN8847X_DMD_set_power_mode(DMD_PWR_MODE_STANDBY);
		return ret;
}
/* **************************************************** */
/*! Device term */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_term(  DMD_PARAMETER_t* param )
{
	 return DMD_E_OK;
}

/* **************************************************** */
/*!	Get Register Settings for Broadcast system */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_init(  DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret = 0;
	DMD_u8_t rd;
	/* Tranfer common register setting */
	MN8847X_DMD_device_read_chipid(&rd);

	if (rd == DMD_TYPE_MN88472)
	{
#if(DEMOD_TYPE == DMD_TYPE_MN88472)     
	ret = MN8847X_DMD_trans_reg(  MN88472_REG_COMMON );
#endif
	}
    else if(rd == DMD_TYPE_MN88473)
	{
	ret = MN8847X_DMD_trans_reg(  MN88473_REG_COMMON );
	}
	else
	{
		ret = DMD_E_ERROR;
		DMD_DBG_TRACE( "ERROR init: Not Supported Demod type ! \n");
	}
	
	DMD_CHECK_ERROR( ret , "DMD_device_init" );

//test I2C communication
	MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_PLPID,  0x99 );
	MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_PLPID,  &rd );	
#ifdef COMMON_DEBUG_ON
	HI_INFO_TUNER("\n ---- Test Demod I2C !! --- reg[PLPID = 0x%x < 0x99 means I2C OK>\n", rd);
#endif
	MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_PLPID,  0x0 ); //back to default value.
	
    return ret;
}
/* **************************************************** */
/*!	Set Register setting for each broadcast system & bandwidth */
/* **************************************************** */
DMD_ERROR_t	MN8847X_DMD_device_set_system(  DMD_PARAMETER_t* param)
{
	DMD_ERROR_t	ret;
	DMD_u8_t	nosupport,rd;
	ret = DMD_E_OK;
	nosupport = 0;

	MN8847X_DMD_device_read_chipid(&rd);

#ifdef COMMON_DEBUG_ON 
    HI_INFO_TUNER("set_system in ! chipid[%d], system[%d], bw[%d] \n",rd,param->system,param->bw);
#endif
	if (rd == DMD_TYPE_MN88472)
	{
#if(DEMOD_TYPE == DMD_TYPE_MN88472) 
		switch( param->system ){
			case DMD_E_DVBT2:
				switch( param->bw ){
					case DMD_E_BW_8MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT2_8MHZ );
						break;
					case DMD_E_BW_6MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT2_6MHZ );
						break;
					case DMD_E_BW_5MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT2_5MHZ );
						break;
					case DMD_E_BW_1_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT2_1_7MHZ );
						break;
					case DMD_E_BW_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT2_7MHZ );
						break;
					default:
						nosupport = 1;
						break;
				}
				break;
			case DMD_E_DVBT:
				switch( param->bw ){
					case DMD_E_BW_8MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT_8MHZ );
						break;
					case DMD_E_BW_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT_7MHZ );
						break;
					case DMD_E_BW_6MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBT_6MHZ );
						break;
					default:
						nosupport = 1;
						break;	
				}
				break;
			case DMD_E_DVBC:
					ret = MN8847X_DMD_trans_reg(  MN88472_REG_DVBC );
				break;
			default:
					nosupport = 1;
		}
#endif
	}
	else if (rd == DMD_TYPE_MN88473)  
	{
		switch( param->system ){
			case DMD_E_DVBT2:
				switch( param->bw ){
					case DMD_E_BW_8MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT2_8MHZ );
						break;
					case DMD_E_BW_6MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT2_6MHZ );
						break;
					case DMD_E_BW_5MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT2_5MHZ );
						break;
					case DMD_E_BW_1_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT2_1_7MHZ );
						break;
					case DMD_E_BW_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT2_7MHZ );
						break;
					default:
						nosupport = 1;
						break;
				}
				break;
			case DMD_E_DVBT:
				switch( param->bw ){
					case DMD_E_BW_8MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT_8MHZ );
						break;
					case DMD_E_BW_7MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT_7MHZ );
						break;
					case DMD_E_BW_6MHZ:
						ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBT_6MHZ );
						break;
					default:
						nosupport = 1;
						break;	
				}
				break;
			case DMD_E_DVBC:
					ret = MN8847X_DMD_trans_reg(  MN88473_REG_DVBC );
				break;
			default:
					nosupport = 1;
		}
	}
	else
	{
	    DMD_DBG_TRACE( "ERROR : Not Supported Demod type ! \n");
	   ret = DMD_E_ERROR;
	}
	
	if( nosupport )
	{
		DMD_DBG_TRACE( "ERROR : Not Supported System");
	}
	DMD_CHECK_ERROR( ret , "DMD_device_set_system" );
	return ret;
}
/* **************************************************** */
/*! Pretune Process */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_pre_tune( DMD_PARAMETER_t* param )
{
if (param->system == DMD_E_DVBT2)
{
	//clear flags
	param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = 0;
	param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] = DMD_E_DVBT2_SIGNAL_PREAMBLE_COMBI_TYPE_SINGLE;
	param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_ONLY;

#if (DEMOD_TYPE == DMD_TYPE_MN88473) //20140410
	if (param->user_scan_action == DMD_E_SCAN_GET_CHANNEL_INFO)//20140414, only affective when search channel.No need when set channel param
	{
	//set work default mode
	param->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_BASE;
	//param->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_LITE;
	MN8847X_DMD_set_t2_work_profile( param );
	}
#endif

}
	return DMD_E_OK;
}
/* **************************************************** */
/*! Posttune Process */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_post_tune( DMD_PARAMETER_t* param )
{
	MN8847X_DMD_device_reset(  param);
	return DMD_E_OK;
}
/* **************************************************** */
/*! Soft Reset */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_reset( DMD_PARAMETER_t* param )
{
	//DMD_I2C_Write(   DMD_BANK_T2_,DMD_INVSET ,0x80);//for debugging,    

	MN8847X_DMD_I2C_Write(   DMD_BANK_T2_,DMD_RSTSET1,0x9f); //addr : 0xF8; ori value : 0x9f
   	MN8847X_DMD_wait( 10);	 //wait 10ms for Demod itself, troy.wang, 20130226 
	return DMD_E_OK;
}

/* **************************************************** */
/*! scan */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_scan( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_ERROR;
	switch( param->system )
	{
	case DMD_E_DVBT2:
		ret = MN8847X_DMD_scan_dvbt2( param );
		break;
	case DMD_E_DVBT:
		ret = MN8847X_DMD_scan_dvbt( param );
		break;
	case DMD_E_DVBC:
		ret = MN8847X_DMD_scan_dvbc( param );
		break;
	default:
		return DMD_E_ERROR;
	}
	return ret;
}

/* **************************************************** */
/*! Set Information  */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_set_info(  DMD_PARAMETER_t* param , DMD_u32_t id , DMD_u32_t val )
{
	DMD_ERROR_t	ret =DMD_E_ERROR;
	DMD_u8_t	rd;
	switch( param->system )
	{
		case DMD_E_DVBT:
			switch( id )
			{
			case DMD_E_INFO_DVBT_HIERARCHY_SELECT:
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_RSDSET_T , &rd );
				if( val == 1 )
				{
					rd |= 0x10;
				}
				else
				{
					rd &= 0xef;
				}
				MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_RSDSET_T , rd );
				param->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT]	= (rd >> 4) & 0x1;
				ret = DMD_E_OK;
				break;
			case DMD_E_INFO_DVBT_MODE:
				ret =DMD_E_OK;
				if( val == DMD_E_DVBT_MODE_NOT_DEFINED ){
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDSET_T  ,  0xba );
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDASET_T ,  0x13 );
				}
				else
				{
					MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0xf0 , 0xf0 );
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDASET_T    , 0x0  );
					switch( val )
					{
					case DMD_E_DVBT_MODE_2K:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x0c , 0x00 );
						break;
					case DMD_E_DVBT_MODE_8K:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x0c , 0x08 );
						break;
					default:
						ret = DMD_E_ERROR;
						break;
					}

				}
				break;
			case DMD_E_INFO_DVBT_GI:
				ret =DMD_E_OK;
				if( val == DMD_E_DVBT_GI_NOT_DEFINED ){
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDSET_T  ,  0xba );
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDASET_T ,  0x13 );
				}
				else
				{
					MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0xf0 , 0xf0 );
					MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_MDASET_T    , 0x0  );
					switch( val )
					{
					case DMD_E_DVBT_GI_1_32:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x03 , 0x00 );
						break;
					case DMD_E_DVBT_GI_1_16:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x03 , 0x01 );
						break;
					case DMD_E_DVBT_GI_1_8:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x03 , 0x02 );
						break;
					case DMD_E_DVBT_GI_1_4:
						MN8847X_DMD_I2C_MaskWrite(   DMD_BANK_T_  , DMD_MDSET_T , 0x03 , 0x03 );
						break;
					default:
						ret = DMD_E_ERROR;
						break;
					}

				}
     		} //case DVB-T end 
			break;	/* '12/02/22 : OKAMOTO	Correct error. */
		case DMD_E_DVBT2:
			switch( id )
			{
				case	DMD_E_INFO_DVBT2_SELECTED_PLP	:
					rd = (DMD_u8_t) val;
					MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_PLPID , rd );
					ret = DMD_E_OK;
					break;

			}
			break;	
		
		default:
			ret = DMD_E_ERROR;
			break;
	}

	return ret;
}

/* **************************************************** */
/*! Get Information  */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_get_info( DMD_PARAMETER_t* param , DMD_u32_t id )
{
	DMD_u32_t i;
	switch( param->system ){
		case DMD_E_DVBT:
			if( id == DMD_E_INFO_DVBT_ALL ){
				param->info[DMD_E_INFO_DVBT_ALL] = DMD_E_INFO_DVBT_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBT_ALL] ; i++ )
					MN8847X_DMD_information_dvbt( param , i , 1 );
			}
			else
					MN8847X_DMD_information_dvbt( param , id , 0 );
			break;
		case DMD_E_DVBT2:
			if( id == DMD_E_INFO_DVBT2_ALL ){
				param->info[DMD_E_INFO_DVBT2_ALL] = DMD_E_INFO_DVBT2_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBT2_ALL] ; i++ )
					MN8847X_DMD_information_dvbt2( param , i , 1);
			}
			else if( id == DMD_E_INFO_DVBT2_L1_ALL )
			{
				MN8847X_DMD_information_dvbt2( param , id , 0);
				for(i=DMD_E_INFO_DVBT2_L1_ALL+1;i<DMD_E_INFO_DVBT2_END_OF_INFORMATION;i++)
				{
					MN8847X_DMD_information_dvbt2( param , i , 1);
				}
			}
			else
					MN8847X_DMD_information_dvbt2( param , id , 0);
			break;
		case DMD_E_DVBC:
			if( id == DMD_E_INFO_DVBC_ALL ){
				param->info[DMD_E_INFO_DVBC_ALL] = DMD_E_INFO_DVBC_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBC_ALL] ; i++ )
					MN8847X_DMD_information_dvbc( param , i );
			}
			else
					MN8847X_DMD_information_dvbc( param , id );
			break;
		default:
			DMD_DBG_TRACE( "Unsupported Broadcast system" );
			return DMD_E_ERROR;
	}
	return DMD_E_OK;
}
	
/* **************************************************** */
/* MN88472 Local functions */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_information_dvbt(  DMD_PARAMETER_t *param , DMD_u32_t id , DMD_u32_t allflag)
{
	DMD_u32_t	biterr,bitnum,packerr,packnum;
	DMD_u32_t	cnr_i,cnr_d;
	DMD_u8_t	rd;

    //HI_INFO_TUNER("--->[%s,%d] \n",__func__,__LINE__);
	switch( id ){
		case	DMD_E_INFO_DVBT_ALL	:
			param->info[DMD_E_INFO_DVBT_ALL] = DMD_E_INFO_DVBT_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBT_REGREV	:
			param->info[DMD_E_INFO_DVBT_REGREV] = MN8847X_DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBT_PSEQRV	:
			param->info[DMD_E_INFO_DVBT_PSEQRV] = MN8847X_DMD_PSEQRev( );
			break;
		case	DMD_E_INFO_DVBT_SYSTEM	:
			param->info[DMD_E_INFO_DVBT_SYSTEM] = MN8847X_DMD_System( );
			break;
		case	DMD_E_INFO_DVBT_STATUS	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_LOCK	:
			param->info[DMD_E_INFO_DVBT_STATUS] = MN8847X_DMD_Status( param->system);
			if( param->info[DMD_E_INFO_DVBT_STATUS] >= DMD_DVBT_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBT_LOCK] = DMD_E_LOCKED;
			}
			else if( param->info[DMD_E_INFO_DVBT_LOCK] == DMD_E_LOCKED )
			{
				param->info[DMD_E_INFO_DVBT_LOCK] = DMD_E_ERROR;
			}
			break;
		case	DMD_E_INFO_DVBT_AGC	:
			param->info[DMD_E_INFO_DVBT_AGC] = MN8847X_DMD_AGC( );
			break;
		case	DMD_E_INFO_DVBT_BERRNUM	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_BITNUM	:
			MN8847X_DMD_BER( param->system,&biterr,&bitnum);
			param->info[DMD_E_INFO_DVBT_BERRNUM]  = biterr;
			param->info[DMD_E_INFO_DVBT_BITNUM]   = bitnum;
			break;
		case	DMD_E_INFO_DVBT_CNR_INT	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_CNR_DEC	:
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBT_PERRNUM	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_PACKETNUM	:
			MN8847X_DMD_PER( param->system,&packerr,&packnum);
			param->info[DMD_E_INFO_DVBT_PERRNUM]  = packerr;
			param->info[DMD_E_INFO_DVBT_PACKETNUM]   = packnum;
			break;
		case	DMD_E_INFO_DVBT_ERRORFREE	:
			MN8847X_DMD_I2C_Read(  DMD_BANK_T_ , DMD_ERRFLG_T , &rd );
		 	param->info[DMD_E_INFO_DVBT_ERRORFREE] = (rd & 0x1)?0:1;
			break;
		case	DMD_E_INFO_DVBT_SQI	:
			//Get CNR
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT_CNR_DEC]  = cnr_d;
			//Get BER
			MN8847X_DMD_BER( param->system,&biterr,&bitnum);
			param->info[DMD_E_INFO_DVBT_BERRNUM]  = biterr;
			param->info[DMD_E_INFO_DVBT_BITNUM]   = bitnum;
			//Get PER, troy.wang added, 20130628
			MN8847X_DMD_PER( param->system,&packerr,&packnum);
			param->info[DMD_E_INFO_DVBT_PERRNUM]  = packerr;
			param->info[DMD_E_INFO_DVBT_PACKETNUM]   = packnum;
			//Get constelation mode 
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD2_T , &rd );
				param->info[DMD_E_INFO_DVBT_CONSTELLATION]	= (rd >> 3 ) & 0x3; 
			   param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7; //troy, 20130131, DVB-T NON-HIERARCHY
			//Get code rate			
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD3_T , &rd );
				param->info[DMD_E_INFO_DVBT_HP_CODERATE]	= (rd >> 3 ) & 0x7;
				param->info[DMD_E_INFO_DVBT_LP_CODERATE]	= rd & 0x7;
			//Calc SQI
			param->info[DMD_E_INFO_DVBT_SQI] = MN8847X_DMD_calc_SQI( param );
			break;
		case	DMD_E_INFO_DVBT_HIERARCHY_SELECT	:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_RSDSET_T , &rd );
			param->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT]	= (rd >> 4) & 0x1;
			break;
		case	DMD_E_INFO_DVBT_MODE	:
		case	DMD_E_INFO_DVBT_GI	:
		case	DMD_E_INFO_DVBT_LENGTH_INDICATOR	:
		case	DMD_E_INFO_DVBT_CONSTELLATION	:
		case	DMD_E_INFO_DVBT_HIERARCHY	:
		case	DMD_E_INFO_DVBT_HP_CODERATE	:
		case	DMD_E_INFO_DVBT_LP_CODERATE	:
		case	DMD_E_INFO_DVBT_CELLID	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_TPS_ALL	:
			//Get TPS Information
			if( MN8847X_DMD_Status( param->system) < DMD_DVBT_LOCKED_STATUS ){
				//Sync is not established , TPS information is not available
				param->info[DMD_E_INFO_DVBT_TPS_ALL]	=	0;
			}
			else
			{
				param->info[DMD_E_INFO_DVBT_TPS_ALL]	=	1;
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD4_T , &rd );
				param->info[DMD_E_INFO_DVBT_MODE]	= rd & 0x3;
				param->info[DMD_E_INFO_DVBT_GI]	= (rd >> 2 ) & 0x3;
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD1_T , &rd );
				param->info[DMD_E_INFO_DVBT_LENGTH_INDICATOR]	= rd & 0x3f;
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD2_T , &rd );
				param->info[DMD_E_INFO_DVBT_CONSTELLATION]	= (rd >> 3 ) & 0x3; 
				param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7;
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_TMCCD3_T , &rd );
				param->info[DMD_E_INFO_DVBT_HP_CODERATE]	= (rd >> 3 ) & 0x7;
				param->info[DMD_E_INFO_DVBT_LP_CODERATE]	= rd & 0x7;
				//param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7; //troy.wangyx masked, 20121207, it's redundant and it's wrong to get HP info. here . 
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_CELLIDU_T , &rd );
				param->info[DMD_E_INFO_DVBT_CELLID]	= rd * 0x100;
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_CELLIDL_T , &rd );
				param->info[DMD_E_INFO_DVBT_CELLID]	+= rd ;

			}
			break;

		default:
			return DMD_E_ERROR;
	}


	return DMD_E_OK;
}
/*! DVBT2 Information */
DMD_ERROR_t MN8847X_DMD_information_dvbt2(  DMD_PARAMETER_t *param , DMD_u32_t id, DMD_u32_t allflag)
{
	DMD_u32_t	biterr,bitnum,packerr,packnum;
	DMD_u32_t	cnr_i,cnr_d;
	DMD_u8_t	rd;
	static	DMD_u8_t	l1info[11][8];			//L1 Information Register

    //HI_INFO_TUNER("--->[%s,%d] \n",__func__,__LINE__);
	switch( id ){
		case	DMD_E_INFO_DVBT2_ALL	:
			param->info[DMD_E_INFO_DVBT2_ALL] = DMD_E_INFO_DVBT2_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBT2_REGREV	:
			param->info[DMD_E_INFO_DVBT2_REGREV] = MN8847X_DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBT2_PSEQRV	:
			param->info[DMD_E_INFO_DVBT2_PSEQRV] = MN8847X_DMD_PSEQRev( );
			break;
		case	DMD_E_INFO_DVBT2_SYSTEM	:
			param->info[DMD_E_INFO_DVBT2_SYSTEM] = MN8847X_DMD_System( );
			break;
		case	DMD_E_INFO_DVBT2_LOCK	:
			param->info[DMD_E_INFO_DVBT2_STATUS] = MN8847X_DMD_Status( param->system);
			if( param->info[DMD_E_INFO_DVBT2_STATUS] >= DMD_DVBT2_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBT2_LOCK] = DMD_E_LOCKED;
			} else if( param->info[DMD_E_INFO_DVBT2_LOCK] == DMD_E_LOCKED )
			{
				param->info[DMD_E_INFO_DVBT2_LOCK] = DMD_E_LOCK_ERROR;
			}
			break;
		case	DMD_E_INFO_DVBT2_AGC	:
			param->info[DMD_E_INFO_DVBT2_AGC] = MN8847X_DMD_AGC( );
			break;
		case	DMD_E_INFO_DVBT2_BERRNUM	:
		case	DMD_E_INFO_DVBT2_BITNUM	:
			MN8847X_DMD_BER( param->system,&biterr,&bitnum);
			param->info[DMD_E_INFO_DVBT2_BERRNUM]  = biterr;
			param->info[DMD_E_INFO_DVBT2_BITNUM]   = bitnum;
			break;
		case	DMD_E_INFO_DVBT2_CNR_INT	:
		case	DMD_E_INFO_DVBT2_CNR_DEC	:
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT2_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT2_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBT2_PERRNUM	:
		case	DMD_E_INFO_DVBT2_PACKETNUM	:
			MN8847X_DMD_PER( param->system,&packerr,&packnum);
			param->info[DMD_E_INFO_DVBT2_PERRNUM]  = packerr;
			param->info[DMD_E_INFO_DVBT2_PACKETNUM]   = packnum;
			break;
		case	DMD_E_INFO_DVBT2_ERRORFREE	:
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_ERRFLG , &rd );
				param->info[DMD_E_INFO_DVBT2_ERRORFREE] = (rd & 0x1)?0:1;
				break;
		case	DMD_E_INFO_DVBT2_SQI	:
			//Get CNR
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT2_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT2_CNR_DEC]  = cnr_d;
			//Get BER
			MN8847X_DMD_BER( param->system,&biterr,&bitnum);
			param->info[DMD_E_INFO_DVBT2_BERRNUM]  = biterr;
			param->info[DMD_E_INFO_DVBT2_BITNUM]   = bitnum;
			//Get PER, troy.wang added, 20130628
			MN8847X_DMD_PER( param->system,&packerr,&packnum);
			param->info[DMD_E_INFO_DVBT2_PERRNUM]  = packerr;
			param->info[DMD_E_INFO_DVBT2_PACKETNUM]   = packnum;
			//Get constelation mode 
			param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD]= MN8847X_DMD_get_l1(   l1info, 3 , 3 , 1 , 7, allflag);
			//Get code rate
			param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= MN8847X_DMD_get_l1(   l1info, 3 , 3 , 1 , 4, allflag); 
			//Calc SQI
			param->info[DMD_E_INFO_DVBT2_SQI] = MN8847X_DMD_calc_SQI( param );
			break; 
		case	DMD_E_INFO_DVBT2_MODE	:
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_SSEQRD1 , &rd );
				param->info[DMD_E_INFO_DVBT2_MODE] = rd & 0x7;
				break;
		case	DMD_E_INFO_DVBT2_GI	:	
		  		param->info[DMD_E_INFO_DVBT2_GI]= MN8847X_DMD_get_l1(   l1info, 3 , 0 , 2 , 6 , 0); 
				break;
		case	DMD_E_INFO_DVBT2_BERRNUM_C	:
		case	DMD_E_INFO_DVBT2_BITNUM_C	:
			MN8847X_DMD_BER_dvbt2( &biterr,&bitnum,1);
			param->info[DMD_E_INFO_DVBT2_BERRNUM_C]   = biterr;
			param->info[DMD_E_INFO_DVBT2_BITNUM_C]   = bitnum;
			break;
		case	DMD_E_INFO_DVBT2_SELECTED_PLP	:
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_PLPID , &rd );
				param->info[DMD_E_INFO_DVBT2_SELECTED_PLP] = rd ;
				break;
		case DMD_E_INFO_DVBT2_L1_ALL:
			MN8847X_DMD_get_l1all( l1info );
			break;
		case DMD_E_INFO_DVBT2_TYPE :			  param->info[DMD_E_INFO_DVBT2_TYPE]= MN8847X_DMD_get_l1(   l1info, 8 , 0 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_BW_EXT :			  param->info[DMD_E_INFO_DVBT2_BW_EXT]= MN8847X_DMD_get_l1(   l1info, 1 , 0 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_S1 :			  param->info[DMD_E_INFO_DVBT2_S1]= MN8847X_DMD_get_l1(   l1info, 3 , 0 , 1 , 6 , allflag); break;
		case DMD_E_INFO_DVBT2_S2 :			  param->info[DMD_E_INFO_DVBT2_S2]= MN8847X_DMD_get_l1(   l1info, 4 , 0 , 1 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_PAPR :			  param->info[DMD_E_INFO_DVBT2_PAPR]= MN8847X_DMD_get_l1(   l1info, 4 , 0 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_MOD :			  param->info[DMD_E_INFO_DVBT2_L1_MOD]= MN8847X_DMD_get_l1(   l1info, 4 , 0 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_COD :			  param->info[DMD_E_INFO_DVBT2_L1_COD]= MN8847X_DMD_get_l1(   l1info, 2 , 0 , 3 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_FEC_TYPE :		  param->info[DMD_E_INFO_DVBT2_L1_FEC_TYPE]= MN8847X_DMD_get_l1(   l1info, 2 , 0 , 3 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_SIZE :		  param->info[DMD_E_INFO_DVBT2_L1_POST_SIZE]= MN8847X_DMD_get_l1(   l1info, 18 , 1 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_INFO_SIZE :	  param->info[DMD_E_INFO_DVBT2_L1_POST_INFO_SIZE]= MN8847X_DMD_get_l1(   l1info, 18 , 1 , 3 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_PILOT_PATTERN :		  param->info[DMD_E_INFO_DVBT2_PILOT_PATTERN]= MN8847X_DMD_get_l1(   l1info, 4 , 0 , 4 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_TX_ID_AVAILABILITY :	  param->info[DMD_E_INFO_DVBT2_TX_ID_AVAILABILITY]= MN8847X_DMD_get_l1(   l1info, 8 , 10 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_CELL_ID :			  param->info[DMD_E_INFO_DVBT2_CELL_ID]= MN8847X_DMD_get_l1(   l1info, 16 , 10 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NETWORK_ID :		  param->info[DMD_E_INFO_DVBT2_NETWORK_ID]= MN8847X_DMD_get_l1(   l1info, 16 , 10 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_T2_SYSTEM_ID :		  param->info[DMD_E_INFO_DVBT2_T2_SYSTEM_ID]= MN8847X_DMD_get_l1(   l1info, 16 , 10 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_T2_FRAMES :		  param->info[DMD_E_INFO_DVBT2_NUM_T2_FRAMES]= MN8847X_DMD_get_l1(   l1info, 8 , 0 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_DATA_SYMBOLS :	  param->info[DMD_E_INFO_DVBT2_NUM_DATA_SYMBOLS]= MN8847X_DMD_get_l1(   l1info, 12 , 0 , 6 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_REGEN_FLAG :		  param->info[DMD_E_INFO_DVBT2_REGEN_FLAG]= MN8847X_DMD_get_l1(   l1info, 3 , 0 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_EXTENSION :	  param->info[DMD_E_INFO_DVBT2_L1_POST_EXTENSION]= MN8847X_DMD_get_l1(   l1info, 1 , 0 , 4 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_RF :			  param->info[DMD_E_INFO_DVBT2_NUM_RF]= MN8847X_DMD_get_l1(   l1info, 3 , 10 , 7 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_CURRENT_RF_IDX :		  param->info[DMD_E_INFO_DVBT2_CURRENT_RF_IDX]= MN8847X_DMD_get_l1(   l1info, 3 , 10 , 7 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_SUB_SLICES_PER_FRAME :	  param->info[DMD_E_INFO_DVBT2_SUB_SLICES_PER_FRAME]= MN8847X_DMD_get_l1(   l1info, 15 , 2 , 1 , 6 , allflag); break;
		case DMD_E_INFO_DVBT2_SUB_SLICE_INTERVAL :	  param->info[DMD_E_INFO_DVBT2_SUB_SLICE_INTERVAL]= MN8847X_DMD_get_l1(   l1info, 22 , 2 , 3 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_PLP :			  param->info[DMD_E_INFO_DVBT2_NUM_PLP]= MN8847X_DMD_get_l1(   l1info, 8 , 2 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_AUX :			  param->info[DMD_E_INFO_DVBT2_NUM_AUX]= MN8847X_DMD_get_l1(   l1info, 4 , 9 , 5 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_PLP_MODE :		  param->info[DMD_E_INFO_DVBT2_PLP_MODE]= MN8847X_DMD_get_l1(   l1info, 2 , 3 , 2 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_TYPE :		  param->info[DMD_E_INFO_DVBT2_FEF_TYPE]= MN8847X_DMD_get_l1(   l1info, 4 , 9 , 0 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_LENGTH :		  param->info[DMD_E_INFO_DVBT2_FEF_LENGTH]= MN8847X_DMD_get_l1(   l1info, 22 , 9 , 1 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_INTERVAL :		  param->info[DMD_E_INFO_DVBT2_FEF_INTERVAL]= MN8847X_DMD_get_l1(   l1info, 8 , 9 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_ID :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID]= MN8847X_DMD_get_l1(   l1info, 8 , 7 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TYPE :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE]= MN8847X_DMD_get_l1(   l1info, 3 , 3 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_PAYLOAD_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_PAYLOAD_TYPE]= MN8847X_DMD_get_l1(   l1info, 5 , 3 , 0 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_GROUP_ID :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_GROUP_ID]= MN8847X_DMD_get_l1(   l1info, 8 , 7 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_COD :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= MN8847X_DMD_get_l1(   l1info, 3 , 3 , 1 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_MOD :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD]= MN8847X_DMD_get_l1(   l1info, 3 , 3 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_ROTATION :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_ROTATION]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE]= MN8847X_DMD_get_l1(   l1info, 2 , 3 , 1 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS_MAX :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS_MAX]= MN8847X_DMD_get_l1(   l1info, 10 , 3 , 4 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_FRAME_INTEVAL :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_FRAME_INTEVAL]= MN8847X_DMD_get_l1(   l1info, 8 , 3 , 6 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_LENGTH :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_LENGTH]= MN8847X_DMD_get_l1(   l1info, 8 , 3 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_TYPE]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_FF_FLAG :		  param->info[DMD_E_INFO_DVBT2_DAT_FF_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 7 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_ID :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_ID]= MN8847X_DMD_get_l1(   l1info, 8 , 7 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TYPE :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_TYPE]= MN8847X_DMD_get_l1(   l1info, 3 , 5 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_PAYLOAD_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_PAYLOAD_TYPE]= MN8847X_DMD_get_l1(   l1info, 5 , 5 , 0 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_GROUP_ID :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_GROUP_ID]= MN8847X_DMD_get_l1(   l1info, 8 , 7 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_COD :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_COD]= MN8847X_DMD_get_l1(   l1info, 3 , 5 , 1 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_MOD :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_MOD]= MN8847X_DMD_get_l1(   l1info, 3 , 5 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_ROTATION :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_ROTATION]= MN8847X_DMD_get_l1(   l1info, 1 , 5 , 2 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_FEC_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_FEC_TYPE]= MN8847X_DMD_get_l1(   l1info, 2 , 5 , 1 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS_MAX :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS_MAX]= MN8847X_DMD_get_l1(   l1info, 10 , 5 , 4 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_FRAME_INTEVAL :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_FRAME_INTEVAL]= MN8847X_DMD_get_l1(   l1info, 8 , 5 , 6 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_LENGTH :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_LENGTH]= MN8847X_DMD_get_l1(   l1info, 8 , 5 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_TYPE]= MN8847X_DMD_get_l1(   l1info, 1 , 5 , 2 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_FF_FLAG :		  param->info[DMD_E_INFO_DVBT2_COM_FF_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 7 , 6 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_FRAME_IDX :		  param->info[DMD_E_INFO_DVBT2_FRAME_IDX]= MN8847X_DMD_get_l1(   l1info, 8 , 8 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_TYPE_2_START :		  param->info[DMD_E_INFO_DVBT2_TYPE_2_START]= MN8847X_DMD_get_l1(   l1info, 22 , 8 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_CHANGE_COUNTER :	  param->info[DMD_E_INFO_DVBT2_L1_CHANGE_COUNTER]= MN8847X_DMD_get_l1(   l1info, 8 , 8 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_START_RF_IDX :		  param->info[DMD_E_INFO_DVBT2_START_RF_IDX]= MN8847X_DMD_get_l1(   l1info, 8 , 3 , 4 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_FIRST_RF_IDX :	  param->info[DMD_E_INFO_DVBT2_DAT_FIRST_RF_IDX]= MN8847X_DMD_get_l1(   l1info, 3 , 7 , 2 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_START :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_START]= MN8847X_DMD_get_l1(   l1info, 22 , 4 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS]= MN8847X_DMD_get_l1(   l1info, 10 , 4 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_FIRST_RF_IDX :	  param->info[DMD_E_INFO_DVBT2_COM_FIRST_RF_IDX]= MN8847X_DMD_get_l1(   l1info, 3 , 7 , 6 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_START :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_START]= MN8847X_DMD_get_l1(   l1info, 22 , 6 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS]= MN8847X_DMD_get_l1(   l1info, 10 , 6 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_STATIC_FLAG :		  param->info[DMD_E_INFO_DVBT2_STATIC_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_STATIC_PADDING_FLAG :	  param->info[DMD_E_INFO_DVBT2_STATIC_PADDING_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_IN_BAND_A_FLAG :		  param->info[DMD_E_INFO_DVBT2_IN_BAND_A_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_IN_BAND_B_FLAG :		  param->info[DMD_E_INFO_DVBT2_IN_BAND_B_FLAG]= MN8847X_DMD_get_l1(   l1info, 1 , 3 , 2 , 2 , allflag); break;
#if (DEMOD_TYPE == DMD_TYPE_MN88473)
		case DMD_E_INFO_DVBT2_L1_POST_SCRAMBLE_ENABLED_FLG:
			MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_L1DECRD1 , &rd );
			param->info[DMD_E_INFO_DVBT2_L1_POST_SCRAMBLE_ENABLED_FLG] = (rd & 0x2)>>1;//20131219, add for indicator of L1_POST_SCRAMBLE
			break;
#endif
		case DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE : // 20140410. to indicate T2 frame type found in Super frame | record types here.
				MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_P1FLG, &rd );
				#ifdef DETAIL_DIG_DEBUG_ON
				HI_INFO_TUNER("get info : p1flg = 0x%x, s1 area: %d \n",rd,((rd&0x7f)>>4));
				#endif
				if (rd&0x01) //different type 
				{
					//check preamble types all same or different
					if (param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] != DMD_E_DVBT2_SIGNAL_PREAMBLE_COMBI_TYPE_MIXED)
					{
					param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] = DMD_E_DVBT2_SIGNAL_PREAMBLE_COMBI_TYPE_MIXED;
					//first time clear flag
					param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = 0;
					}
					
					switch((rd&0x7f)>>4)//S1 field
					{
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_SISO: 
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MISO: 
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] |= DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MASK ;
						break;
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_NON_T2:
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] |= DMD_E_DVBT2_SIGNAL_PREAMBLE_NON_T2_MASK ;
						break;
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_SISO: 				
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MISO: 
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] |= DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MASK ;
						break;
					default:
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] |= DMD_E_DVBT2_SIGNAL_PREAMBLE_RESERVED_MASK ;		
						break;
					}				

				}
				else if(param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] != DMD_E_DVBT2_SIGNAL_PREAMBLE_COMBI_TYPE_MIXED)
				{//All same type; add this condition to prevent internal reset of MN88473.
					switch((rd&0x7f)>>4)//S1 field
					{
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_SISO: 
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MISO: 
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MASK ;
						break;
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_NON_T2:
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_NON_T2_MASK ;
						break;
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_SISO: 				
					case DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MISO: 
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MASK ;
						break;
					default:
						param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_RESERVED_MASK ;		
						break;
					}
				}
				
				break;
      default:
			return DMD_E_ERROR;

	}
	return DMD_E_OK;
}

/*
Author  : Troy.wang
Date    : 20130621
Purpose : Detailed signal's modulation parameter detect(especially for unknow field test signal)
*/
void MN8847X_DMD_signal_information_printout_all( DMD_PARAMETER_t *param)
{
		DMD_u8_t	i;
		MN8847X_DMD_get_info(param,DMD_E_INFO_LOCK);
		if ( param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED )
		{
			HI_INFO_TUNER(" CLK bias :  \n");  
			MN8847X_DMD_Carrier_Frequency_bias( param);  // check IF bias. 
			HI_INFO_TUNER("[Advice] IF bias should be less than 100Khz, otherwise, pls. make sure tuner output 5Mhz IF \n");
			MN8847X_DMD_XTAL_Frequency_bias( param);    // check XTAL bias. 
			HI_INFO_TUNER("[Advice] XTAL bias should be less than 50ppm, otherwise,demod hard to lock signal. pls. check : \n");
			HI_INFO_TUNER("1.(Hareware)if DEMOD has independent XTAL,try to change capacity aroud it\n");
			HI_INFO_TUNER("2.(software)if DEMOD get CLK from Tuner,try to change capacity by adjust param <xtalCfg.xtalCap>_Mxl603\n");
			
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_DETFLG, &i );
			HI_INFO_TUNER("Interference detect : DET FLG = 0x%x \n",i);  // to check interference 

			MN8847X_DMD_wait(2000);//wait for TS output 
			MN8847X_DMD_get_info( param , DMD_E_INFO_CNR_INT ); 
			MN8847X_DMD_get_info( param, DMD_E_INFO_PERRNUM);
			HI_INFO_TUNER(" CNR = %d.%d\n",param->info[DMD_E_INFO_CNR_INT],param->info[DMD_E_INFO_CNR_DEC]); 
			HI_INFO_TUNER(" PER: ERRORNUM = %d,PACKNUM = %d\n",param->info[DMD_E_INFO_PERRNUM],param->info[DMD_E_INFO_PACKETNUM]);
			HI_INFO_TUNER("[Advice]if PER : ERRORNUM = 0 and PACKNUM > 0, it means the TS output from Demod has no problem! If no video, pls. check : \n");
			HI_INFO_TUNER("1. Demod TS output mode and Soc receive mode matches or not\n");
			HI_INFO_TUNER("2. Check SoC's TS receiving code\n");

			HI_INFO_TUNER("---------------BEGIN : Modulation parameters------------------- \n");  
			MN8847X_DMD_get_info(param,DMD_E_INFO_ALL);

			for(i = 0; i < param->info[DMD_E_INFO_ALL];i++)
			{
				HI_INFO_TUNER("No.%d : %30s : ", i, MN8847X_DMD_info_title(param->system, i));
				HI_INFO_TUNER( "[Text]%30s : [Value]%d \n", MN8847X_DMD_info_value(param->system, i, param->info[i]), param->info[i]);
			}				
			HI_INFO_TUNER("----------------END : Modulation parameters------------------------\n");  
		}
		else
		{
			HI_INFO_TUNER(" Signal unlocked, read out value invalid !! ");
			HI_INFO_TUNER("[Advice]pls. follow the steps to check : ");
			HI_INFO_TUNER("1. pls. make sure the signal itself no problem, eg. use MP set to test \n ");
			HI_INFO_TUNER("2. pls. check the RF level is too low or not, by func. <Tuner_check_RF_input_power>\n ");
			HI_INFO_TUNER("3. if you are using Maxlinear tuner, pls. check param <singleSupply_3_3V> with its power supply design \n ");
			HI_INFO_TUNER("4. pls. check if you have set correct registers same as XTAL+IF, in func. <DMD_device_set_system>\n ");
			HI_INFO_TUNER("5. pls. set Tuner self-AGC Mxl603_(agcCfg.agcType = MXL603_AGC_SELF)\n if no problem, pls. check circuit of External AGC\n ");
			HI_INFO_TUNER("6. pls. try to receive DVB-T or change FFT size to lower one, eg. 2K, 8K, until signal locked\n ");
			HI_INFO_TUNER("When signal locked, check debug of <CLK bias :>, normally it's caused by XTAL bias more than 50ppm!\n ");
			HI_INFO_TUNER("7. After confirmed 1~5 steps, still NG :  Contact us ! \n ");
		}
}
					
/*! DVBC Information */
DMD_ERROR_t MN8847X_DMD_information_dvbc( DMD_PARAMETER_t *param , DMD_u32_t id)
{
	DMD_u32_t	err,sum;
	DMD_u32_t	cnr_i,cnr_d,symbol_rate;
	DMD_u8_t	rd;

	switch( id ){
		case	DMD_E_INFO_DVBC_ALL	:
			param->info[DMD_E_INFO_DVBC_ALL] = DMD_E_INFO_DVBC_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBC_REGREV	:
			param->info[DMD_E_INFO_DVBC_REGREV] = MN8847X_DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBC_PSEQRV	:
			param->info[DMD_E_INFO_DVBC_PSEQRV] = MN8847X_DMD_PSEQRev( );
			break;
		case	DMD_E_INFO_DVBC_SYSTEM	:
			param->info[DMD_E_INFO_DVBC_SYSTEM] = MN8847X_DMD_System( );
			break;
		case	DMD_E_INFO_DVBC_LOCK	:
			param->info[DMD_E_INFO_DVBC_STATUS] = MN8847X_DMD_Status( param->system);
			if( param->info[DMD_E_INFO_DVBC_STATUS] >= DMD_DVBC_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBC_LOCK] = DMD_E_LOCKED;
			}
			break;
		case	DMD_E_INFO_DVBC_AGC	:
			param->info[DMD_E_INFO_DVBC_AGC] = MN8847X_DMD_AGC( );
			break;
		case	DMD_E_INFO_DVBC_BERRNUM	:
		case	DMD_E_INFO_DVBC_BITNUM	:
			MN8847X_DMD_BER( param->system,&err,&sum);
			param->info[DMD_E_INFO_DVBC_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_BITNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBC_CNR_INT	:
		case	DMD_E_INFO_DVBC_CNR_DEC	:
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBC_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBC_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBC_PERRNUM	:
		case	DMD_E_INFO_DVBC_PACKETNUM	:
			MN8847X_DMD_PER( param->system,&err,&sum);
			param->info[DMD_E_INFO_DVBC_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_PACKETNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBC_ERRORFREE	:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_ERRFLG_T , &rd );
			param->info[DMD_E_INFO_DVBT_ERRORFREE] = (rd & 0x1)?0:1;
			break;
	/* ---20130418,troy.wang added, DVBC SQI calculation--- BEGIN */	
		case	DMD_E_INFO_DVBC_SQI	:
			//Get CNR
			MN8847X_DMD_CNR( param->system,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBC_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBC_CNR_DEC]  = cnr_d;
			//Get BER
			MN8847X_DMD_BER( param->system,&err,&sum);
			param->info[DMD_E_INFO_DVBC_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_BITNUM]   = sum;
			//Get constelation mode 
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_, DMD_SSEQMON1_C, &rd );
			param->info[DMD_E_INFO_DVBC_CONSTELLATION]= (rd&0x07);
			//Get code rate - DVBC no code rate
	         //param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= DMD_get_l1(   l1info, 3 , 3 , 1 , 4, allflag); 
			//Calc SQI
			param->info[DMD_E_INFO_DVBC_SQI] = MN8847X_DMD_calc_SQI( param );
			break;
	/* ---20130418,troy.wang added, DVBC SQI calculation--- END */
	
    //troy added, 20130507, some customer want to show DVB-C constellation and symbol rate
	   case DMD_E_INFO_DVBC_CONSTELLATION:
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_, DMD_SSEQMON1_C, &rd );
			param->info[DMD_E_INFO_DVBC_CONSTELLATION]= (rd&0x07); //pls. refer to structrue "DMD_DVBC_CONST_t"
			break;
	   case DMD_E_INFO_DVBC_SYMBOL_RATE:
			MN8847X_DMD_DVBC_GET_SYMBOL_RATE(param,&symbol_rate);
			param->info[DMD_E_INFO_DVBC_SYMBOL_RATE]= symbol_rate; //unit : Khz
			break;

		default:
			return DMD_E_ERROR;

	}
	return DMD_E_OK;

}

/* **************************************************** */
/* Informations */
/* **************************************************** */
//! Infomation : Register set Version
DMD_u32_t	MN8847X_DMD_RegRev()
{
	return MN8847X_DMD_RegSet_Rev;
}
//! Infomation : Auto control Version
DMD_u32_t	MN8847X_DMD_PSEQRev( )
{
	DMD_u8_t	rd;	
	MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PSEQOP1_T , &rd );
	return (DMD_u32_t) rd;
}
//! Infomation : AGC Loop filter Level
//Bigger value means bigger RF power level
#define AGC_MIN 230  //-100dBm
#define AGC_MAX 896  //0dBm
#define AGC_RANGE (AGC_MAX - AGC_MIN)
DMD_u32_t	MN8847X_DMD_AGC(void)
{
	DMD_u8_t	rd;
	DMD_u32_t	ret;

	// Set to default -> AGC monitor AGCRD selection : AGC loop filter value (input signal level)
	MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_AGCRDSET, 0x00); 

	MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_AGCRDU , &rd );
	ret = rd * 4;
	MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_AGCRDL , &rd );
	ret += rd >> 6;

/* [Simple SSI indicator]
agc = ret;
if(agc > AGC_MAX)
	*data = 100;
else if(agc < AGC_MIN)
	*data = 0;
else
	*data = (agc - AGC_MIN)*100/AGC_RANGE;
*/

	HI_INFO_TUNER("---->[%s,%d] agc = %d\n",__func__,__LINE__,ret);

	return ret;

}

//Troy.wang added, 20120629, feedback IF center frequency bias for Tuner to retune, which is used for field test
//Return : kHz
DMD_s32_t MN8847X_DMD_Carrier_Frequency_bias( DMD_PARAMETER_t* param)
{ 
	DMD_u8_t  CarrierErrU = 0,CarrierErrM = 0, CarrierErrL = 0, CarrierDVBCFlag = 0;
	DMD_u32_t CarrierErrRd = 0; //unsigned
	DMD_s32_t CarrierErrKHz = 0; //signed. 
	DMD_u8_t  notsupport = 0,fNegativeCarrierErr = 0;

	/* Get estimated carrier error */	
	switch( param->system )
	 {
		case DMD_E_DVBT2:
		     {
				//AGC monitor AGCRD selection : Carrier frequency error
				MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_AGCRDSET, 0x06 );
				//AGCRD read [15:0]
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_AGCRDU , &CarrierErrU);
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_AGCRDL , &CarrierErrL);
				
				#ifdef COMMON_DEBUG_ON
				HI_INFO_TUNER("dvbt2 carrier, 0x%x%x \n",CarrierErrU,CarrierErrL);
				#endif

				CarrierErrRd = ( ((DMD_u32_t)CarrierErrU << 8) & 0xFF00) + CarrierErrL;

				/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
				if (0x8000 & CarrierErrRd) 
				{
				CarrierErrRd = ((~CarrierErrRd) & 0x0000FFFF) + 0x1;
				fNegativeCarrierErr = 1;
				}
				/*
				Carrer frequency error can be calculated by the following equation:
				Carrier frequency error = (fS*10^6) / 2^17 * AGCRRD[15:0] [Hz] 
				                                   = AGCRRD[15:0] *(fS*10^3) / 2^17 [KHz]
				//1MHz = 10^6 Hz, 2^17 = 131072
				where
				fS : FFT sampling frequency
				8 MHz bandwidth : 64/7 MHz
				7 MHz bandwidth : 8 MHz
				6 MHz bandwidth : 48/7 MHz
				5 MHz bandwidth : 40/7 MHz
				1.7 MHz bandwidth : 131/71 MHz
				*/  				
				switch( param->bw ) // Integer ,For better accuracy; Uint : KHz
				{
					case DMD_E_BW_8MHZ:
						CarrierErrKHz = 64*CarrierErrRd/7*1000/131072; 
						break;
						case DMD_E_BW_7MHZ:
						CarrierErrKHz = 8*CarrierErrRd*1000/131072; 
						break;
					case DMD_E_BW_6MHZ:
						CarrierErrKHz = 48*CarrierErrRd/7*1000/131072; 
						break;
					case DMD_E_BW_5MHZ:
						CarrierErrKHz = 40*CarrierErrRd/7*1000/131072; 
						break;
					case DMD_E_BW_1_7MHZ:
						CarrierErrKHz = 131*CarrierErrRd/71*1000/131072;
						break;
					default:
						notsupport = 1; //Hz
						break;
					}

					// Set to default -> AGC monitor AGCRD selection : AGC loop filter value (input signal level)
					MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_AGCRDSET, 0x00 );
			} //case DMD_E_DVBT2 ends. 
			break;			
			
		case DMD_E_DVBT:
				{		
				DMD_u8_t dvbt_fft_mode;
				DMD_u32_t dvbt_ffts;
			   // Synchronization monitor SYN2RD_T selection : Carrier frequency error
				MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_SYN2RDSET_T, 0x00 );
				//SYN2RDU read [10:0]
			 	MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_SYN2RDU_T, &CarrierErrU);
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_SYN2RDL_T, &CarrierErrL);

				CarrierErrRd = ( ((DMD_u32_t)CarrierErrU << 8) & 0xFF00) + CarrierErrL;

				/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
				if (0x0400 & CarrierErrRd) //valid 11 bits.
				{
				CarrierErrRd = ((~CarrierErrRd) & 0x000007FF) + 0x1;
				fNegativeCarrierErr = 1;
				}
				/*
				Carrier frequency error = (fS*10^6) /ffts * SYN2RD[10:0] [Hz] 
				                                   = SYN2RD[10:0]*(fS*10^3) / ffts [Khz]
				where				
				ffts : FFT size (8K: 8192, 2K: 2048)
				fS : FFT sampling frequency
				8 MHz bandwidth : 64/7 MHz
				7 MHz bandwidth : 8 MHz
				6 MHz bandwidth : 48/7 MHz
				  */
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_MDRD_T, &dvbt_fft_mode );
				dvbt_fft_mode = (dvbt_fft_mode&0x0C)>>2; //FFT_S_T: bit[2:3] ; 0xC0, troy.wang modified, previous position was wrong... 20120920  

				if ( dvbt_fft_mode == 0 ) // 2K mode
					{
					    dvbt_ffts = 2048;
					}
				else if ( dvbt_fft_mode == 2 )//8K mode
					{
					    dvbt_ffts = 8192;
					} 
				else
					{
						notsupport = 1;
						break;
					}
					
				switch( param->bw ) //Integer , For better accuracy ; Unit : KHz 
				{
					case DMD_E_BW_8MHZ:
						CarrierErrKHz =  CarrierErrRd*1000*64/7/dvbt_ffts; 
						break;
			      	case DMD_E_BW_7MHZ:
						CarrierErrKHz = CarrierErrRd*1000*8/dvbt_ffts;
						break;
					case DMD_E_BW_6MHZ:
						CarrierErrKHz = CarrierErrRd*1000*48/7/dvbt_ffts;
						break;
					default:
						notsupport = 1;
						break;
				 }
			} //case DMD_E_DVBT: ends.
			break;
			
		case DMD_E_DVBC:

			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_AFCMON4_C, &CarrierErrL);
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_AFCMON3_C, &CarrierErrM);
				CarrierErrRd = (((DMD_u32_t)CarrierErrM << 8)& 0xFF00) + CarrierErrL;
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_AFCMON2_C, &CarrierErrU);
				CarrierErrRd = ( ((DMD_u32_t)CarrierErrU << 16) & 0xFF0000) + CarrierErrRd;

			/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_AFCMON1_C, &CarrierDVBCFlag);
			if (0x01 & CarrierDVBCFlag) 
			{
				CarrierErrRd = ((~CarrierErrRd) & 0x00FFFFFF) + 0x1;
				fNegativeCarrierErr = 1;
			}

			/*
                Calculate carrier frequency fERR error from AFCMON_C and
				sampling frequency fADCK is described by the following equation:
				fERR = fADCK / 2^27 * AFCMON_C[23:0] = fADCK / 2^18 * AFCMON_C / 2^9 Hz
			*/
			CarrierErrKHz = ((DMD_ADCK_FREQ*1000)/262144*CarrierErrRd/512)/1000;		
			
			break;
		default:
			notsupport = 1;
			break;
	}

			if(notsupport == 1)
			{
				CarrierErrKHz = 0;
				DMD_DBG_TRACE( "ERROR : Not Supported mode");
			}
			else
			{                          				 	
				if (1 == fNegativeCarrierErr) 
				CarrierErrKHz = CarrierErrKHz * (-1);

				HI_INFO_TUNER(" Carrier frequency bias is [%d Khz] \n", CarrierErrKHz);
			}
 
	return CarrierErrKHz;
}

//Troy.wang added, 20120629, feedback Clock frequency bias, , which is used for signal lock issue.
//unit Hz .
DMD_s32_t MN8847X_DMD_XTAL_Frequency_bias( DMD_PARAMETER_t* param)
{ 
	DMD_u8_t  ClockErrU = 0, ClockErrL = 0;
	DMD_u32_t ClockErr = 0;
	DMD_s32_t ClockErrPPM = 0;
	DMD_u8_t  fNegativeClockErr = 0;
	DMD_u8_t nosupport = 0;

	/* Get estimated clock error */	
	switch( param->system ){
		case DMD_E_DVBT2:
				//Synchronization monitor SYN1RD selection : Clock frequency error
				MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_SYN1RDSET, 0x01 );

				//Calculate clock frequency error from SYN1RD : read [15:0]
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_SYN1RDU , &ClockErrU );
				MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_SYN1RDL, &ClockErrL );				 
			break;			
		case DMD_E_DVBT:
				//Synchronization monitor SYN1RD _T selection : Clock frequency error
				MN8847X_DMD_I2C_Write(   DMD_BANK_T_, DMD_SYN1RDSET_T, 0x01 );

				//Calculate clock frequency error from SYN1RD_T : read [15:0]
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_SYN1RDU_T , &ClockErrU );
				MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_SYN1RDL_T , &ClockErrL );
			break;
			
		//DVB-C demodulator monitor doesn't have relative registers.
		case DMD_E_DVBC:
		default:
			nosupport = 1;
			break;
	}

	if( nosupport )
	{
		ClockErrPPM = 0;
		DMD_DBG_TRACE( "ERROR : Not Supported System");
	}
	else
	{
		ClockErr = ( ((DMD_u32_t)ClockErrU << 8) & 0xFF00) + ClockErrL;

		/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
		if (0x8000 & ClockErr) 
		{
			ClockErr = ((~ClockErr) & 0x0000FFFF) + 0x1;
			fNegativeClockErr = 1;
		}

	switch( param->system ){
			case DMD_E_DVBT2:
				// Equation : SYN1RD/2^24 * 10^6 [ppm]	, for better accuracy :	
				// ClockErrPPM = (ClockErr) / 2^24 * 2^20 - (ClockErr)/ 2^24 * 48576	
				ClockErrPPM = (ClockErr/16) - (ClockErr*48576/16777216);		 
				break;			
			case DMD_E_DVBT:
				// Equation : SYN1RD/2^27 * 10^6 [ppm]	, for better accuracy :	
				// ClockErrPPM = (ClockErr) / 2^27 * 2^20 - (ClockErr)/ 2^27 * 48576	
				ClockErrPPM = (ClockErr/128) - (ClockErr*48576/134217728);
				break;
			case DMD_E_DVBC:
			default:
	        break;
		}

		if (1 == fNegativeClockErr) 
		ClockErrPPM = ClockErrPPM * (-1);
	}

 	HI_INFO_TUNER(" XTAL frequency bias is [%d ppm] \n", ClockErrPPM);
	return ClockErrPPM;
}


//troy.wang, 20131015, customer want to show constellation map 
/*
Input :
 action - 0: stop operation; 1 : start operation
 mon_type - different type of cell to monitor
output : 
  None
*/
DMD_ERROR_t MN8847X_DMD_Constellation_monitor_pre_setting( DMD_PARAMETER_t* param, DMD_u8_t action, DMD_DVBT2_CON_MON_t dvbt2_mon_type, DMD_DVBT_CON_MON_t dvbt_mon_type)
{
	DMD_ERROR_t ret = DMD_E_OK;
	DMD_u8_t ctmod = 0, temprd = 0;
	
	switch( param->system ){
		case DMD_E_DVBT2:
		if ( 1 == action )
			{
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ , DMD_CTSET,  &temprd );
			//select type -> 0 : data plp ;  1 : common plp;  2 : L1pre;   3 : L1post
			ctmod = (0x03&dvbt2_mon_type)|0x04; 
			ctmod = ctmod | (temprd&0xf8); //not change bit 3~7 unknown. 
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET,  ctmod );  
			}
		else
			{
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET, 0x00 ); //stop monitoring
			}

			break;
				
		case DMD_E_DVBT:
		if ( 1 == action )
			{
			//start monitoring, only need do once before continous read. 
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CNSET_T , 0x00 );  //original 0x10.

			//select type -> 0 : CTA(Pre-deinterleave) ;  1 : CTB(Post-deinterleave)
			ctmod = dvbt_mon_type ? (0x60) : (0x40) ;
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET1_T , ctmod ); //original 0x00; contellation monitor start
			}
		else
			{
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CNSET_T , 0x10 ); //stop monitoring
			}
			break;
				
		case DMD_E_DVBC:
			//default OPEN. 
			break;
			
		default:
			HI_INFO_TUNER("CONST MON unsupport system !\n");
			ret = DMD_E_ERROR;
	}

return ret;
}

//troy.wang, 20131015, customer want to show constellation map
/*
Input :
 CarrierNo - customer can set a range to check. eg. carrier 0~500 
output : 
  I, Q signal - I,Q is real co-ordinates of data points
*/
DMD_ERROR_t MN8847X_DMD_Constellation_monitoring( DMD_PARAMETER_t* param, DMD_u32_t CarrierNo,DMD_s32_t* p_IMON_RD, DMD_s32_t* p_QMON_RD_p)
{ 
	DMD_u8_t CarrierNoUp, CarrierNoLow, tIMONrd, tQMONrd,temprd;
	DMD_u8_t fNegative_i = 0,fNegative_q = 0;
	DMD_ERROR_t ret = DMD_E_OK;
	DMD_s32_t IMON_RD = 0, QMON_RD = 0;

    //pls. firstly set monitor type and start operation.
		
	switch( param->system ){
		case DMD_E_DVBT2:
			//start monitor [ Need start monitor every time ?! ]
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ , DMD_CTSET,  &temprd );
			temprd |= 0x04;
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET,  temprd );

			//select carrier number 
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTCARU , &temprd );
			CarrierNoUp = (DMD_u8_t)(CarrierNo >> 8 )&(0x7f);  
			CarrierNoUp = CarrierNoUp | (temprd&0x80); //not change bit7 unknown.
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTCARU , CarrierNoUp );	
			CarrierNoLow = (DMD_u8_t)CarrierNo; 	
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTCARL , CarrierNoLow);		 

			MN8847X_DMD_wait(1); //wait for monitoring. 
			
			//   I signal = CTRD[15:8]
			//   Q signal = CTRD[7:0] 
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTRDU , &tIMONrd );
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTRDL , &tQMONrd );
			IMON_RD = tIMONrd;
			QMON_RD = tQMONrd;
			
			if (0x80 & (tIMONrd)) 
				{
				IMON_RD = ((~tIMONrd) & 0x0000007F) + 0x1;
				fNegative_i = 1;
				}
			
			if (0x80 & (tQMONrd)) 
				{
				QMON_RD = ((~tQMONrd) & 0x0000007F) + 0x1;
				fNegative_q = 1;
				}	
		break;			
			
		case DMD_E_DVBT:

			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTSET1_T , &temprd );
			//select carrier number 
			CarrierNoUp = (DMD_u8_t)((CarrierNo >> 8 )&(0x1f));  
			CarrierNoUp = CarrierNoUp | (temprd&0x60); //not change bit CTLD_T and CTSEL_T
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET1_T , CarrierNoUp );	
			CarrierNoLow = (DMD_u8_t)CarrierNo; 	
			MN8847X_DMD_I2C_Write( DMD_BANK_T_ , DMD_CTSET2_T , CarrierNoLow);

			MN8847X_DMD_wait(1); //wait for monitoring. 
            
			// I signal = CTRD_T[15:8]
			// Q signal = CTRD_T[7:0]
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTRDU_T , &tIMONrd );
			MN8847X_DMD_I2C_Read( DMD_BANK_T_ ,DMD_CTRDL_T , &tQMONrd );
			IMON_RD = tIMONrd;
			QMON_RD = tQMONrd;
			
			if (0x80 & (tIMONrd)) 
				{
				IMON_RD = ((~tIMONrd) & 0x0000007F) + 0x1;
				fNegative_i = 1;
				}
			
			if (0x80 & (tQMONrd)) 
				{
				QMON_RD = ((~tQMONrd) & 0x0000007F) + 0x1;
				fNegative_q = 1;
				}
		break;			
			
		case DMD_E_DVBC:
			//No need to selece carrier number 
			
			MN8847X_DMD_I2C_Read( DMD_BANK_C_ ,DMD_IMON1_C, &tIMONrd );
			IMON_RD = (tIMONrd&0x03) << 8;
			MN8847X_DMD_I2C_Read( DMD_BANK_C_ ,DMD_IMON2_C, &tIMONrd );
			IMON_RD = IMON_RD + tIMONrd;

			MN8847X_DMD_I2C_Read( DMD_BANK_C_ ,DMD_QMON1_C, &tQMONrd );
			QMON_RD = (tQMONrd&0x03) << 8;
			MN8847X_DMD_I2C_Read( DMD_BANK_C_ ,DMD_QMON2_C, &tQMONrd );
			QMON_RD = QMON_RD + tQMONrd;

			/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
			if (0x0200 & (IMON_RD)) 
			{
			IMON_RD = ((~IMON_RD) & 0x000001FF) + 0x1;
			fNegative_i = 1;
			}

			if (0x0200 & (QMON_RD)) 
			{
			QMON_RD = ((~QMON_RD) & 0x000001FF) + 0x1;
			fNegative_q = 1;
			}
		break;
			
		default:
			return DMD_E_ERROR;
	}
           
			*p_IMON_RD = IMON_RD;
			*p_QMON_RD_p = QMON_RD;

			if (1 == fNegative_i) 
				*p_IMON_RD = IMON_RD * (-1);

			if (1 == fNegative_q) 
				*p_QMON_RD_p = QMON_RD * (-1);				
	
 	HI_INFO_TUNER(" DTV[%d] carrier[%d] -> I signal = %d, Q signal = %d \n",param->system,CarrierNo,*p_IMON_RD, *p_QMON_RD_p);
	return ret;
}


/*! Information : System detect (from register setting) */
DMD_u32_t	MN8847X_DMD_System( )
{
	DMD_u8_t	rd;
	DMD_u32_t	ret;

	MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_DTVSET , &rd );

	rd &= 0x7;
	switch( rd ){
		case 2:
			ret = DMD_E_DVBT;
			break;
		case 3:
			ret = DMD_E_DVBT2;
			break;
		case 4:
			ret = DMD_E_DVBC;
			break;
		default:
			ret =DMD_E_NOT_DEFINED;
			break;

	}
	return ret;
}
/*! Information : Reciever Status ( Detail )  */
DMD_u32_t	MN8847X_DMD_Status( DMD_SYSTEM_t sys ){
	DMD_u8_t	rd;
	DMD_u32_t	ret;

	ret = 0;
	switch( sys ){
		case DMD_E_DVBT:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_SSEQRD_T , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
		case DMD_E_DVBT2:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_SSEQFLG , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
		case DMD_E_DVBC:
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_SSEQMON2_C , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
		/* '11/08/01 : OKAMOTO	Correct Error. */
		default:
			ret = DMD_ERROR_STATUS;
			break;
	};

	return ret;
}
//! Information : ERROR Num
DMD_u32_t	MN8847X_DMD_BER( DMD_SYSTEM_t sys , DMD_u32_t* err , DMD_u32_t* sum ){
	DMD_u8_t	rd;

	switch( sys )
	{
		case DMD_E_DVBT:
		case DMD_E_DVBC:
			//SET BERSET1[5] = 0
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERSET1_T , &rd );
			rd &= 0xDF;	//1101_1111
			MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_BERSET1_T , rd );
			//SET BERRDSET[3:0] = 0101
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERRDSET_T, &rd );
			rd = (rd & 0xF0 ) | 0x5;
			MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_BERRDSET_T, rd );
			//Read ERROR
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERRDU_T , &rd );
			*err = rd * 0x10000;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERRDM_T , &rd );
			*err += rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERRDL_T , &rd );
			*err += rd ;
			//Read BERLEN
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERLENRDU_T , &rd );
			*sum = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_BERLENRDL_T , &rd );
			*sum += rd;
			*sum = *sum * 203 * 8;
			break;

		case DMD_E_DVBT2:
			MN8847X_DMD_BER_dvbt2(   err , sum , 0 );	//Data PLP
			break;

		default:
			break;
	}
	return 0;
}




//! Information : ERROR Num
DMD_u32_t	MN8847X_DMD_BER_dvbt2(  DMD_u32_t* err , DMD_u32_t* sum  , DMD_u32_t common )
{
		DMD_u8_t	rd;

		//DATA PLP 
		MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_BERSET , &rd );

		if( common == 0 )
		{
			rd |= 0x20;		//BERSET[5] = 1 (BER after LDPC)
			rd &= 0xef;		//BERSET[4] = 0 (Data PLP)
		}
		else
		{
			rd |= 0x20;		//BERSET[5] = 1 (BER after LDPC)
			rd |= 0x10;		//BERSET[4] = 1 (Common PLP)
		}
		MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_BERSET , rd );

		
		//Read ERROR
		MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_BERRDU , &rd );
		*err = rd * 0x10000;
		MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_BERRDM , &rd );
		*err += rd * 0x100;
		MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_BERRDL , &rd );
		*err += rd ;
		//Read BERLEN
		MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_BERLEN , &rd );
		*sum = (1 << (rd & 0xf) );
		
		if( common == 0 ) //selected PLP information( data plp )
		{
				 MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET2 , 0x3 );
				 MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TPD2   , &rd );
		}
		else		 // common PLP information 
		{
				 MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET2 , 0x5 );
				 MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TPD2    , &rd ); //troy.wang, 20120226, added for right FEC type read. 
		}
		
		if( (rd & 0x1 ) == 1 )
		{
			//FEC TYPE = 1
			switch( (rd>>2) & 0x7 )//code rate
			{
			case 0:	*sum = (*sum) * 32400; break;
			case 1:	*sum = (*sum) * 38880; break;
			case 2:	*sum = (*sum) * 43200; break;
			case 3:	*sum = (*sum) * 48600; break;
			case 4:	*sum = (*sum) * 51840; break;
			case 5:	*sum = (*sum) * 54000; break;
			}
		}
		else
		{
			//FEC TYPE = 0
			switch( (rd>>2) & 0x7 )//code rate
			{
			case 0:	*sum = (*sum) * 7200 * 4; break;
			case 1:	*sum = (*sum) * 9720 * 4; break;
			case 2:	*sum = (*sum) * 10800* 4; break;
			case 3:	*sum = (*sum) * 11880* 4; break;
			case 4:	*sum = (*sum) * 12600* 4; break;
			case 5:	*sum = (*sum) * 13320* 4; break;
			}
		}
		
		return 0;
}



//! Information : CNR
DMD_u32_t	MN8847X_DMD_CNR( DMD_SYSTEM_t sys , DMD_u32_t* cnr_i , DMD_u32_t* cnr_d ){
	DMD_u8_t	rd;
	DMD_s32_t	cnr;
	DMD_s32_t	sig,noise;
	switch( sys )
	{
		case DMD_E_DVBT:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_CNRDU_T , &rd );
			cnr = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_CNRDL_T , &rd );
			cnr += rd;
			if( cnr != 0 )
			{
				//cnr = 65536 / cnr;
				cnr = MN8847X_log10_easy( 65536 ) - MN8847X_log10_easy( cnr ) + 200; //troy, 20130131, for better accuracy, when Signal is at QPSK, CNR is very large, 65536 / CNRD = integer, not float. 
				if( cnr < 0 ) cnr = 0;
			}
			else
				cnr = 0;

			*cnr_i = (DMD_u32_t ) cnr / 100;
			*cnr_d = (DMD_u32_t ) cnr % 100;
			break;
		case DMD_E_DVBT2:
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_CNRDU , &rd );
			cnr = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_CNRDL , &rd );
			cnr += rd;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_CNFLG , &rd );
			if( cnr != 0 )
			{
				if( rd & 0x4 )
				{
					//MISO
					//cnr = 16384 / cnr;
					cnr =  MN8847X_log10_easy( 16384 ) - MN8847X_log10_easy( cnr ) - 600; //troy, 20130131, for better accuracy, when Signal is at QPSK, CNR is very large, 65536 / CNRD = integer, not float. 
					if( cnr < 0 ) cnr = 0;
					*cnr_i = (DMD_u32_t ) cnr / 100;
					*cnr_d = (DMD_u32_t ) cnr % 100;
				}
				else
				{
					//SISO
					//cnr = 65536 / cnr;
			    	cnr = MN8847X_log10_easy( 65536 ) - MN8847X_log10_easy( cnr ) + 200; //troy, 20130131, for better accuracy, when Signal is at QPSK, CNR is very large, 65536 / CNRD = integer, not float. 
					if( cnr < 0 ) cnr = 0;
					*cnr_i = (DMD_u32_t ) cnr / 100;
					*cnr_d = (DMD_u32_t ) cnr % 100;

				}
			}
			else
			{
				*cnr_i = 0;
				*cnr_d = 0;
			}
			break;
		case DMD_E_DVBC:
			
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_CNMON1_C , &rd );
			sig = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_CNMON2_C , &rd );
			sig += rd;
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_CNMON3_C , &rd );
			noise = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_CNMON4_C , &rd );
			noise += rd;

			if( noise != 0 )
				{
				  //cnr = log10_easy(sig * 8  / noise);
				   cnr = MN8847X_log10_easy(sig * 8) - MN8847X_log10_easy(noise); //troy, 20130131, for better accuracy
				}
			else
				cnr = 0;

			if( cnr < 0 ) cnr = 0;
			*cnr_i = (DMD_u32_t ) cnr / 100;
			*cnr_d = (DMD_u32_t ) cnr % 100;
			break;

   
      default:
			break;
	}
	return 0;
}
//! Information : Packet Error
DMD_u32_t	MN8847X_DMD_PER( DMD_SYSTEM_t sys , DMD_u32_t* err , DMD_u32_t* sum ){
	DMD_u8_t	rd;

	switch( sys )
	{
		case DMD_E_DVBT:
		case DMD_E_DVBC:
		case DMD_E_DVBT2:
			//Read ERROR
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PERRDU , &rd );
			*err = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PERRDL , &rd );
			*err += rd ;
			//Read BERLEN
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PERLENRDU , &rd );
			*sum = rd * 0x100;
			MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_PERLENRDL , &rd );
			*sum += rd;
			break;
		default:
			break;
	}
	return 0;

}

// fSYM = fADCK / RATEMON_C * (2^18) 
DMD_ERROR_t MN8847X_DMD_DVBC_GET_SYMBOL_RATE(  DMD_PARAMETER_t *param,DMD_u32_t *dvbc_get_symbolrate_Khz )
{
	DMD_ERROR_t ret = DMD_E_ERROR;  
	DMD_u8_t rd;
	DMD_u32_t fadc;
		
	if ( param->system == DMD_E_DVBC)
    {
		//Read FAD_C 
		MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_RATERDU_C , &rd );
		fadc = rd * 0x10000;
		MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_RATERDM_C , &rd );
		fadc += rd * 0x100;
		MN8847X_DMD_I2C_Read(   DMD_BANK_C_ , DMD_RATERDL_C , &rd );
		fadc += rd ;

		//*dvbc_get_symbolrate = (Xtal*10^6)/fadc*(2^18) -> unit : Hz
		//  = (xtal*10^3)*(2^16)/fadc*(2^2)   -> unit Khz, for better accuracy, for most 32bits. 
		*dvbc_get_symbolrate_Khz = (DMD_ADCK_FREQ*65536)/fadc*4;//troy, 20130507, treat XTAL seperately 

		#ifdef COMMON_DEBUG_ON
		HI_INFO_TUNER("--- %s symbol rate = %d Khz---\n",__FUNCTION__,*dvbc_get_symbolrate_Khz);
		#endif
		ret = DMD_E_OK;
	}
	else 
	{
		HI_INFO_TUNER(" This function is for DVB-C only !\n");		
		ret = DMD_E_ERROR;  
	}

	return ret;
}

/* **************************************************** */
/* Scan (for Channel search) */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_scan_dvbt2( DMD_PARAMETER_t *param )
{
	DMD_u32_t	time_count_ori = 190;//180+10;
/*troy,wang, 20140217, extended Time out length for signal from Mico, which maybe non-standard signal from Russia
locked this signal for 10000 time, longest time to lock it is 4190ms
purpose : To increase signal's compatibility
side affect : increase scan time only when signal itself too weak
*/
	DMD_u32_t	time_count_offset = 0;

	DMD_u32_t  count = 0;
	DMD_u8_t	rd;
	DMD_ERROR_t	ret = DMD_E_ERROR;	
	DMD_u8_t record_reg_status = 0;

	#if 1//def COMMON_DEBUG_ON
	static DMD_u32_t record_t = 0;//for debug only
	#endif

	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;
	#if (DEMOD_TYPE == DMD_TYPE_MN88473)
	if (param->t2_work_profile_mode == DMD_E_DVBT2_WORK_MODE_LITE)
	{
		time_count_ori = 400; //troy.wangyx, 20140520, for T2-Lite + FEF(Zero) signal, the longest time to lock signal is 3240ms
	}
	#endif
	
   while(count < (time_count_ori + time_count_offset)) 	//timeout most 1.9s + 2.5s
   	{	
		//P1 Give up
		MN8847X_DMD_I2C_Read( DMD_BANK_T2_  ,DMD_SSEQFLG , &rd );

		#ifdef DETAIL_DIG_DEBUG_ON
		HI_INFO_TUNER("DVB-T2 SCAN c=%d	rd =%x \n",count,rd);
		#endif

		// 20140410. to indicate T2 frame type found in Super frame
		if (record_reg_status > DMD_E_DVBT2_SYNC_STATUS_P1_DETECT)//P1 symbol detected OK -> confirmed, no use !
		{
			MN8847X_DMD_get_info(param,DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE);
		}
		
        /* -----BEGIN : increase timeout length to increase compatibility for non-standard signal, toy.wang, 20140217-------*/
        //to record SYNC status, as judgement of signal exist or not.
		if ((rd&0x0f) > record_reg_status )
			{
				record_reg_status = (rd&0x0f);
			}
		
		 //P1 symbol detected = T2 signal existed. Also consider IN-BAND noise         
       if ((count == 180)&&(record_reg_status > DMD_E_DVBT2_SYNC_STATUS_CARRIER_FREQ_DETECT)) //T2_TRANSIT(Step 6) <-> MICO T2MI failed at L1-post status(Step 12)
			{ 
				time_count_offset = 250;
        	}		
	     /* -----END : increase timeout length to increase compatibility for non-standard signal, toy.wang, 20140217-------*/
			
		if ( rd & 0x40 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSIGNAL;	
			break;
		}
		
		//Sync judgement 
		if( (rd & 0xf) >= 13 )
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}

		count++;
		MN8847X_DMD_wait(10);			//wait 10ms			
	}	

	/* 20140411, troy.wangyx added, to set T2-base or T2-lite work profile */
	#if (DEMOD_TYPE == DMD_TYPE_MN88473)
		//need to wait after one SCAN, because for T2-Lite, Maximum FEF length is 1000ms, we need to read P1 flag to get Preamble type
		MN8847X_DMD_T2_FRAME_PROFILE_TYPES_STATS(param);
	#endif

#if 1//def COMMON_DEBUG_ON
	if ((count > 180 )||(count > record_t )||(param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED ))
	{
		record_t = count;	
		HI_INFO_TUNER("DVB-T2 SCAN c=%d	rd =%x, maxtimeout = %d, reg_status[%d], S1 Area[%x]\n ",count,rd, record_t,record_reg_status,param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]);	
	}
#endif
	
	return ret;
}


DMD_ERROR_t MN8847X_DMD_scan_dvbt( DMD_PARAMETER_t *param )
{
	DMD_u32_t  time_count_all = 120;
	DMD_u32_t  count = 0;
	DMD_u8_t	rd;
	DMD_ERROR_t	ret = DMD_E_ERROR;

	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;	

	//set mode&gi search
	MN8847X_DMD_set_info(   param , DMD_E_INFO_DVBT_MODE , DMD_E_DVBT_MODE_NOT_DEFINED );
	MN8847X_DMD_set_info(   param , DMD_E_INFO_DVBT_GI   , DMD_E_DVBT_GI_NOT_DEFINED );
	
   while(count < time_count_all) 	//timeout most 1.7s
	{	
		MN8847X_DMD_I2C_Read(   DMD_BANK_T_  ,DMD_SSEQRD_T , &rd );
		
#ifdef DETAIL_DIG_DEBUG_ON 	
		HI_INFO_TUNER("DVB-T SCAN c=%d	rd =%x	\n	",count,rd );
#endif
		
		//OFDM ERROR
		if ( rd & 0x80 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSIGNAL;	
			break;
		}
		//Mode Search : singal unstable, sync status jump back, for more than 3 times.
		if ( rd & 0x20 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSYNC;	
			break;
		}
		//Sync detection
		if( (rd & 0xf) >= 10 ) //20130701, original : Frame synchronization detection -> Synchronization established 
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}
		MN8847X_DMD_wait( 10);			//wait 10ms
       	count++;
	}
	   
	return ret;
}

DMD_ERROR_t MN8847X_DMD_scan_dvbc(  DMD_PARAMETER_t *param )
{
	DMD_u32_t	st;
	DMD_u32_t	now;
	DMD_u32_t	timeout = 1800;
	DMD_u8_t	rd;
	DMD_ERROR_t	ret = DMD_E_ERROR;

	MN8847X_DMD_timer(&st);
	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;	

	do
	{
		MN8847X_DMD_I2C_Read(   DMD_BANK_C_  ,DMD_STSMON_C , &rd );

		//QAM ERR
		if( rd & 0x40 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSYNC;	
			break;
		}
		
		MN8847X_DMD_I2C_Read(   DMD_BANK_C_  ,DMD_DMDSTSMON1_C , &rd );
		//VQLOCK
		if( rd & 0x1 )
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}
		MN8847X_DMD_wait( 10);	 //wait 10ms
		MN8847X_DMD_timer( &now );
	}
	while( now - st < timeout );	//timeout

	return ret;
}


DMD_u32_t	MN8847X_DMD_get_l1(  DMD_u8_t l1info[11][8] , DMD_u32_t	bitw , DMD_u32_t bank , DMD_u32_t adr , DMD_u32_t pos , DMD_u32_t allflag)
{
	DMD_s32_t	flag = bitw;
	DMD_u8_t	data;
	DMD_u32_t	ret = 0;

	for(;;)
	{
		if( allflag == 1 )
		{
			data = l1info[bank][adr];

		}
		else
		{
			MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET1 , 0 );
			MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET2 , (DMD_u8_t) bank );
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , (DMD_u8_t)(DMD_TPD1 + adr) , &data );
		}

		if( flag <= 8 )
		{
			data = ( data & ((1<<(pos+1))-1) );
			data = ( data >> (pos+1-flag ) ); 
			ret  += data;
		}
		else
		{
			ret += ( data & ((1<<(pos+1))-1) );
		}
		flag -= (pos + 1);
		if( flag <= 0 ) 
		{
			break;
		}
		else
		{
			ret <<= 8;

		}
		adr ++;
		pos = 7;
	}

	return ret;

}

DMD_ERROR_t	MN8847X_DMD_get_l1all(  DMD_u8_t l1info[DMD_L1_BANK_SIZE][DMD_L1_REG_SIZE] )
{
	DMD_u8_t i,j;
	DMD_u8_t	data;

	for( i=0; i < DMD_L1_BANK_SIZE ; i++ )
	{
		MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET1 ,  0 );
		MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TPDSET2 ,  i );
		for( j=0; j< DMD_L1_REG_SIZE; j ++ )
		{
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TPD1 + j , &data );
			l1info[i][j] = data;
		}
	}

	return DMD_E_OK;
}

DMD_ERROR_t	MN8847X_DMD_set_error_flag_output( DMD_u8_t bErrorFlagOutputEnable )
{
       DMD_u8_t data;
	   
	if(bErrorFlagOutputEnable)
		{		
			/* 1st,Adr:0x09(TSSET2) bit[2:0]=1 ("001") */
			if( MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TSSET2 , &data ) == DMD_E_ERROR ){
				return DMD_E_ERROR;
			}
			data &= 0xF8;
			data |= 0x1; //'01'
			if( MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TSSET2 , data ) == DMD_E_ERROR ){
				return DMD_E_ERROR;
			}
		}
	else
	{	
			//Default setting. INT signal ouput 
			/* 1st,Adr:0x09(TSSET2) bit[2:0]=1 ("000") */
			if( MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TSSET2 , &data ) == DMD_E_ERROR )
			{
				return DMD_E_ERROR;
			}
			data &= 0xF8; //'00'
			if( MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TSSET2 , data ) == DMD_E_ERROR ){
				return DMD_E_ERROR;
			}
	}
	return DMD_E_OK;
}


/*
Purpose : When SoC cannot receive TS from Demod; test the matching of Demod and SoC
Place : inside and at the beginning of "DMD_device_reset"
*/
void MN8847X_DMD_INVERSE_TS_CLK_POLARITY( void )
{
	//CLK inverse -> bit[7]: serial clock; bit[6]: parallel clock 
#if 1
	MN8847X_DMD_I2C_Write( DMD_BANK_T2_,DMD_INVSET ,0x40); //parallel
#else
	DMD_I2C_Write( DMD_BANK_T2_,DMD_INVSET ,0x80); //serial 
#endif
}
/* '11/08/29 : OKAMOTO	Select TS output. */
DMD_ERROR_t MN8847X_DMD_set_ts_output(  DMD_TSOUT ts_out )
{
    DMD_u8_t rd = 0;
	MN8847X_DMD_device_read_chipid(&rd);

	#ifdef COMMON_DEBUG_ON
	    HI_INFO_TUNER("--- set TS output mode --- %d\n",ts_out);
	#endif
		
	if (rd == DMD_TYPE_MN88472)
	{
		switch(ts_out)
	       {	       
		case DMD_E_TSOUT_PARALLEL_FIXED_CLOCK:
			//TS parallel (Fixed clock mode)		TSSET1:0x00	FIFOSET:0xE1
			MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x00 );
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET , 0xE1 );
			break;
			
		case DMD_E_TSOUT_PARALLEL_VARIABLE_CLOCK:
			//TS parallel (Variable clock mode)		TSSET1:0x00	FIFOSET:0xE3
			MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x00 );
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET , 0xE3 );
			break;
			
		case DMD_E_TSOUT_SERIAL_VARIABLE_CLOCK:
			//TS serial(Variable clock mode)		TSSET1:0x1D	FIFOSET:0xE3
			MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x1D );
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET , 0xE3 );
			break;
			
		default:
			return DMD_E_ERROR;
	     }  
    }
	else if (rd == DMD_TYPE_MN88473)  
	{
		switch(ts_out)
			       {	       
				case DMD_E_TSOUT_PARALLEL_FIXED_CLOCK:
					//TS parallel (Fixed clock mode), BCK : 75/8 MHz
					MN8847X_DMD_I2C_Read(  DMD_BANK_T2_ , DMD_TSSET2 , &rd );
					rd |= 0x08;//set bit[3] to be 1
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET2 ,rd );
					
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x00 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_CLKSET1, 0x37 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET , 0x04 );
					break;		
					
				case DMD_E_TSOUT_PARALLEL_VARIABLE_CLOCK:
					//TS parallel (Variable clock mode)		
					MN8847X_DMD_I2C_Read(  DMD_BANK_T2_ , DMD_TSSET2 , &rd );
					rd &= 0xf7;//set bit[3] to be 0
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET2 ,rd );
					
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x00 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_CLKSET1, 0x3 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET, 0x05 );
					break;
					
				case DMD_E_TSOUT_SERIAL_FIXED_CLOCK:
					//TS serial(Fixed clock mode), SCK : 75MHz
					MN8847X_DMD_I2C_Read(  DMD_BANK_T2_ , DMD_TSSET2 , &rd );
					rd |= 0x08;//set bit[3] to be 1
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET2 ,rd );
					
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x1D );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_CLKSET1, 0x37 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET, 0x04 );
					break;
					
				case DMD_E_TSOUT_SERIAL_VARIABLE_CLOCK:
					//TS serial(Variable clock mode)		
					MN8847X_DMD_I2C_Read(  DMD_BANK_T2_ , DMD_TSSET2 , &rd );
					rd &= 0xf7;//set bit[3] to be 0 //20140120, troywang, modifiey BUG to set TS output mode !
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET2 ,rd );
					
					MN8847X_DMD_I2C_Write(  DMD_BANK_T2_ , DMD_TSSET1 , 0x1D );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_CLKSET1, 0x3 );
					MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_FIFOSET, 0x05 );
					break;

				default:
					return DMD_E_ERROR;
		}
	}
	else
	{
	    DMD_DBG_TRACE( "ERROR : Not Supported Demod type ! \n");
	   return DMD_E_ERROR;
	}
	
	return DMD_E_OK;
}

/*
Description : Set DVB-T2 work profile
Input   : 
 param->t2_work_profile_mode 
return : 
Date   : Troy, 20121219
note   : only for MN88473
*/
#if (DEMOD_TYPE == DMD_TYPE_MN88473) 
DMD_ERROR_t MN8847X_DMD_set_t2_work_profile(DMD_PARAMETER_t* param) 
{
    DMD_ERROR_t  ret_val = DMD_E_OK ;

	if (param->system != DMD_E_DVBT2)
	{
	#ifdef COMMON_DEBUG_ON
		HI_INFO_TUNER( " ERROR !! DTV system not DVBT2 \n");  
	#endif
	   return DMD_E_OK;
	}

	switch (param->t2_work_profile_mode)
	{
		case DMD_E_DVBT2_WORK_MODE_BASE: //default
			if ((param->t2_supported_profile_type == DMD_E_DVBT2_SOC_SUPPORT_BASE_ONLY)\
				||(param->t2_supported_profile_type == DMD_E_DVBT2_SOC_SUPPORT_BASE_AND_LITE))//20140414
				{
				#ifdef COMMON_DEBUG_ON
			    HI_INFO_TUNER("--- set T2 base mode OK!---\n");
				#endif
				ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_DTVSET, 0x03 );
				ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_STTOSET1, 0xe4 );//original setting
				}
			else
				{
				 #ifdef COMMON_DEBUG_ON
			    HI_INFO_TUNER("--- didn't support T2 base mode! Forcibly set Lite mode---\n");
				#endif
				ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_DTVSET, 0x0B );
				
				//extends P1 give up timeout to more than 1000ms,T2-lite: FEF max length 1000ms.
				ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_STTOSET1, 0xec );//troy.wangyx, 20140520, for T2-Lite + FEF(Zero) signal
				}
			break;

		case DMD_E_DVBT2_WORK_MODE_LITE:
			if ((param->t2_supported_profile_type == DMD_E_DVBT2_SOC_SUPPORT_LITE_ONLY)\
				||(param->t2_supported_profile_type == DMD_E_DVBT2_SOC_SUPPORT_BASE_AND_LITE))//20140414
				{
				#ifdef COMMON_DEBUG_ON
			    HI_INFO_TUNER("--- set T2 LITE mode OK!---\n");
				#endif
				ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_DTVSET, 0x0B );

				//extends P1 give up timeout to more than 1000ms,T2-lite: FEF max length 1000ms.
				ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_STTOSET1, 0xec );//troy.wangyx, 20140520, for T2-Lite + FEF(Zero) signal
				}
			else
				{
				 #ifdef COMMON_DEBUG_ON
			    HI_INFO_TUNER("--- didn't support LITE base mode! Forcibly set Base mode--- \n");
				#endif
				ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_DTVSET, 0x03 );
				ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_STTOSET1, 0xe4 );//original setting
				}
			break;

		default:
			HI_INFO_TUNER("---ERROR ! Wrong T2 work profile !!--");
			ret_val = DMD_E_ERROR;
			break;
	}

    return (ret_val);
}
#endif

DMD_ERROR_t MN8847X_DMD_set_power_mode(  DMD_PWR_MODE_t running_mode ) 
{
    DMD_ERROR_t  ret_val = DMD_E_OK ;

#ifdef COMMON_DEBUG_ON
	HI_INFO_TUNER("--- mode = %d[0:normal; 1:standby] \n",running_mode);
#endif
	
	switch (running_mode)
	{
		case DMD_PWR_MODE_NORMAL: //wake up fucntion; after waken up no need to do init again, just re-tune, re-scan 
			ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_PWDSET, 0x00 );
			break;

		case DMD_PWR_MODE_STANDBY:
			ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_PWDSET, 0x3E );
			break;

		case DMD_PWR_MODE_SLEEP: //not suggest to set sleep. need to do HW-reset to wake it up
			ret_val = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_PWDSET, 0x3F );
			break;

		default:
			HI_INFO_TUNER("---Power mode selected error !--");
			ret_val = DMD_E_ERROR;
			break;
	}

    return (ret_val);
}

//set I2C control mode, only for MN88473, 20131219
DMD_ERROR_t MN8847X_DMD_set_dmd_control_tuner_i2c_mode(void)
{
DMD_ERROR_t  ret_val = DMD_E_OK ;
#if (TUNER_I2C_MODE == TUNER_CONTROL_BY_DEMOD)
  #if (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_REPEAT_MODE)
  ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBSET, 0xD3 );
  #elif (DEMOD_CONTROL_TUNER_I2C_MODE == TCB_I2C_FORMAT_BYPASS_MODE)
  ret_val = MN8847X_DMD_I2C_Write( DMD_BANK_T2_ , DMD_TCBSET, 0xE3);
  #endif
#endif

return ret_val;
}

/*
Description : Set Input IF range from Tuner, for cooperation with Tuner & Demod. 
Input   : define, do once
return : 
Date   : Troy, 20140303
*/
DMD_ERROR_t MN8847X_DMD_set_adc_input_range(void)
{
	DMD_u8_t rd = 0;
	DMD_u8_t dmd_adc_input_range = 0;
	
#ifdef COMMON_DEBUG_ON
	HI_INFO_TUNER("--- %s = %d[1.0, 1.5, 2vpp] \n",__FUNCTION__,DMD_ADC_INPUT_RANGE);
#endif

	dmd_adc_input_range = DMD_ADC_INPUT_RANGE;
	MN8847X_DMD_device_read_chipid(&rd);
		
	if (rd == DMD_TYPE_MN88472)
	{
		switch(dmd_adc_input_range)
	       {	       
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_0Vpp:
		case DMD_ADC_INPUT_RANGE_SINGLE_1_0Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_ADCSET1, 0x09 );
			break;
			
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_5Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_ADCSET1, 0x0A );
			break;
			
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_2_0Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_T_ , DMD_ADCSET1, 0x0B );
			break;
			
		default:
			return DMD_E_ERROR;
	     }  
    }
	else if (rd == DMD_TYPE_MN88473)  
	{
		switch(dmd_adc_input_range)
		{	       
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_0Vpp:
		case DMD_ADC_INPUT_RANGE_SINGLE_1_0Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_C_ , DMD_ADCSET1, 0x25 );
			break;
			
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_1_5Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_C_ , DMD_ADCSET1, 0x22 );
			break;
			
		case DMD_ADC_INPUT_RANGE_DIFFERENTIAL_2_0Vpp:
			MN8847X_DMD_I2C_Write(  DMD_BANK_C_ , DMD_ADCSET1, 0x20 );
			break;

			default:
			  return DMD_E_ERROR;
		}
	}
	else
	{
	    DMD_DBG_TRACE( "ERROR : Not Supported Demod type ! \n");
	   return DMD_E_ERROR;
	}
	
	return DMD_E_OK;
}

/*
Purpose     : Increase compatibility of locking non-standard signal, which has high ratio of Dummy cell -- Against DVB-T2 SPEC
              This is sometimes caused by customer who set Modulation parameter by themselves.
Side effect : Some parameter comibination(not DTG or Nordig standard),need more than timeout length to lock !!   
Suggest     : Not use this function !! 
Creator     : Troy.wang, 20130626
*/
void DMD_lock_high_ratio_dummy_cell_signal(void)
{
#if (DEMOD_TYPE == DMD_TYPE_MN88472)
	MN8847X_DMD_I2C_Write(DMD_BANK_T2_,DMD_WAFCSET3,0x13); //default 0x03 -> [warning]only for MN88472
#endif
    ;
}

/*
Cause       : Factory DVB-T signal, which is amplified more than twice, from signal generation to different office;
              20% SETs cannot lock this kind of signal because it need more time to stablize IF AGC by demod(external AGC).
              the NG SET receice signal directly from Signal generation, result is OK.
Remedy      : Add AGC timeout for DVB-T      
Side effect : May extend DVB-T scan time by 100ms
Suggest     : Applied! increase compatibility of receiver
Creator     : Troy.wang, 20131211, For GIEC factory issue 
*/
void MN8847X_DMD_DVBT_lock_high_noise_signal(void)
{
	MN8847X_DMD_I2C_Write(DMD_BANK_T_,DMD_SSEQTAU1_T,0x4B); //default 0x1B
}

/*
Issue       : In field, customer use 345Mhz, 7.8Mboud symbol rate, 256QAM.
              7.8M symbol rate is out of DVB-C specification
Remedy      : Increase auto symbol rate searching range to 7.9Mboud     
Side effect : May extend scan time
Suggest     : Applied for certain area
Creator     : Troy.wang, 20140409, For Crenova
*/
void MN8847X_DMD_DVBC_lock_high_symbol_rate(void)
{
#if (DEMOD_TYPE == DMD_TYPE_MN88472) //Unit : Khz
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_BAUDMAXU_C,0x0A); //default 0x0B
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_BAUDMAXL_C,0x3D); //default 0x3C
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_RATE1U_C,0x0A); //default 0x0B
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_RATE1L_C,0x3D); //default 0x3C
#elif (DEMOD_TYPE == DMD_TYPE_MN88473)
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_BAUDMAXU_C,0x0C); //default 0x0D
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_BAUDMAXL_C,0xD2); //default 0xAE
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_RATE1U_C,0x0C); //default 0x0B
	MN8847X_DMD_I2C_Write(DMD_BANK_C_,DMD_RATE1L_C,0xD2); //default 0x3C
#endif
}

/*
Issue       : SQI fluctuates bigger(more than 10%) in field test(use Antenna to receive, not direct to SG)
Cause       : CNR read directly from demod itself fluctuates
Remedy      : increase No. of C/N monitor measurement carriers -> fluctuate in 5%
Side effect : no
Suggest     : Applied! increase accuracy and stability
Creator     : Troy.wang, 20140619, For Mico(Vonemm) Singapore field test issue 
*/
void MN8847X_DMD_DVBT2_CNR_stablize(void)
{
	//default, 0x00, to increase CNR read accuracy and stability, 20140619; 
	MN8847X_DMD_I2C_Write(DMD_BANK_T2_,DMD_CNSET,0x08); 
}
/*
Purpose      : Container, for future use
Where to put : DMD_set_system()
Creator      : Troy.wang, 20130626
*/
void MN8847X_DMD_increase_compatibility_with_nonstandard_signal(DMD_PARAMETER_t *param)
{
/* Part 1 : Increase compatibility for non-standard signal*/
	//DMD_lock_high_ratio_dummy_cell_signal(); //side effect, not use!
	MN8847X_DMD_DVBT_lock_high_noise_signal();
	//DMD_DVBT_lock_LNA_signal_use_self_AGC_TEMP(param);// 20140417, added for Fulan DVB-T
	//DMD_DVBC_lock_high_symbol_rate();	//20140409, For Crenova
	
	
/* Part 2 : Increase stability of demodulator's monitors */
	MN8847X_DMD_DVBT2_CNR_stablize();//20140619, for Mico singapore field test issue.	

}

//to get know curret T2 profile 
DMD_ERROR_t MN8847X_DMD_device_dvbt2_read_P1_flag(  DMD_u8_t* p1flg_rd )
{ 
		DMD_ERROR_t  ret_val = DMD_E_OK ;

		//Get FEF status. S2 = xxx1, mixed frame, means T2 super frame including FEF part.  
		ret_val = MN8847X_DMD_I2C_Read( DMD_BANK_T2_ , DMD_P1FLG, p1flg_rd );  

#ifdef COMMON_DEBUG_ON
		HI_INFO_TUNER("DVB-T2 P1FLG = 0x%x\n",*p1flg_rd);
#endif
		return ret_val;
}

/*
Position:
  Pls. put it after DMD_device_scan_dvbt2()
Description :
  Record T2 Frame profile type in T2 superframe.
Input   : 
 param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE] -> record P1 preamble during DMD_device_scan_dvbt2()
 param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] -> record P1 preamble same type or different type
return : 
 param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] -> if Base+Lite, customer can choose to work either mode
Date   : Troy, 20140410
*/
DMD_ERROR_t MN8847X_DMD_T2_FRAME_PROFILE_TYPES_STATS( DMD_PARAMETER_t* param)
{

if (param->system != DMD_E_DVBT2)
{
	HI_INFO_TUNER("%s only supported by T2 system \n",__FUNCTION__);
	return DMD_E_OK;
}

#ifdef COMMON_DEBUG_ON
	HI_INFO_TUNER("P1FLAG accumulated value = 0x%x \n",param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]);
#endif

    if (param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] != DMD_E_DVBT2_SIGNAL_PREAMBLE_COMBI_TYPE_MIXED)
    	{
			if (param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MASK)
			{
			    //T2-lite preamble detected
				param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_ONLY;
			}
		   else if(param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MASK)
		   	{
		   		param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_ONLY;
				//work as default T2-Base mode
		   	}
		   else //for future use.
		   	{
		   		param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_RESERVED;
				//work as default T2-Base mode
		   	}		   
    	}
	else //if different, how to set work mode(example: T2-base+FEF; T2-lite+FEF; T2-base+T2-lite)
		{

		  if((param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MASK)\
		  	&&(param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MASK))
		  	{
			    //T2-lite + T2-base preamble detected
				param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_MIXED_BASE_LITE_COEXIST;
				//work as default T2-Base mode, it's for HD content
		  	}
		  else if(param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_MASK)
		  	{
		  		//T2-lite preamble detected
				param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_MIXED_LITE_FEF_COEXIST;

		  	}
		  else if(param->info[DMD_E_INFO_DVBT2_SIGNAL_EXISTED_PREAMPLE_TYPE]&DMD_E_DVBT2_SIGNAL_PREAMBLE_BASE_MASK)
		  	{
				param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_MIXED_BASE_FEF_COEXIST;
				//work as default T2-Base mode
		  	}
		  else //for future use
		  	{
				param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] = DMD_E_DVBT2_SIGNAL_PREAMBLE_RESERVED;
				//work as default T2-Base mode
		  	}
		}
#ifdef COMMON_DEBUG_ON
	HI_INFO_TUNER("After %s, mixed status : %d \n",__FUNCTION__,param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE]);
#endif

return DMD_E_OK;
}

/*
Position:
  Pls. put it after DMD_device_scan_dvbt2() and DMD_T2_FRAME_PROFILE_TYPES_STATS()
Description :
 .default work at T2-base mode
 .set to T2-lite mode, if T2-Lite frame existed in T2 super frame
 .if T2-lite preamble detected, and non T2-base existed, set to T2-lite work profile automatically
 .if T2-lite and T2-base preamble coexist, default use Base mode. 
     --- SoC company can develop interface for customer to choose work profile, base or lite. 
Input   : 
  param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_MIXED_STATUS] -> if Base+Lite, customer can choose to work either mode
return : 
  param->t2_work_profile_mode
Date   : Troy, 20140410
*/
#if (DEMOD_TYPE == DMD_TYPE_MN88473) 
DMD_ERROR_t MN8847X_DMD_T2_AUTO_SET_T2LITE_MODE_AFTER_SCAN( DMD_PARAMETER_t* param)
{
	if (param->system != DMD_E_DVBT2)
	{
		HI_INFO_TUNER(" %s, Base lite ONLY supported by T2 !system \n",__FUNCTION__);
		return DMD_E_OK;
	}

	if (param->t2_work_profile_mode == DMD_E_DVBT2_WORK_MODE_LITE)
	{
		HI_INFO_TUNER("Already T2-Lite mode\n");
		return DMD_E_OK;
	}

	if (param->t2_supported_profile_type != DMD_E_DVBT2_SOC_SUPPORT_BASE_AND_LITE)//20140414, if not support both profile, use single profile
	{
		HI_INFO_TUNER("ONLY support single mode! don't do auto search! \n");
		return DMD_E_OK;
	}

	MN8847X_DMD_get_info(param,DMD_E_INFO_LOCK);
	if(param->info[DMD_E_INFO_LOCK] != DMD_E_LOCKED)
	{
		//T2-lite preamble detected
		if ((param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] == DMD_E_DVBT2_SIGNAL_PREAMBLE_LITE_ONLY)\
		||(param->info[DMD_E_INFO_DVBT2_SIGNAL_PREAMBLE_COMBI_MODE] == DMD_E_DVBT2_SIGNAL_PREAMBLE_MIXED_LITE_FEF_COEXIST)\
		)
		{
			//set to LITE mode and re-scan!
			HI_INFO_TUNER("T2-LITE detected, rescan ! \n");
			param->t2_work_profile_mode = DMD_E_DVBT2_WORK_MODE_LITE;
			MN8847X_DMD_set_t2_work_profile( param );
			MN8847X_DMD_device_post_tune(  param );
			MN8847X_DMD_device_scan(  param );
		}
	}

return DMD_E_OK;
}
#endif //#if (DEMOD_TYPE == DMD_TYPE_MN88473)


/*
Multipath interference detection
DETFLG[1:0],Larger values indicate larger interference levels.
INPUT  : DTV system (DVB-C in cable, no multipath)
OUTPUT : Multipath interface level
Note   : Valid only when signal locked
Creator : troy.wang, 20130628
*/
DMD_ERROR_t MN8847X_DMD_device_read_strong_echo(DMD_PARAMETER_t* param,DMD_u8_t* Multipath_inter_level)
{ 
   DMD_ERROR_t  ret_val = DMD_E_OK ;
		
	if( param->system == DMD_E_DVBT )
	{
	ret_val = MN8847X_DMD_I2C_Read( DMD_BANK_T_, DMD_DETFLG_T, Multipath_inter_level );  
	}
	else if( param->system == DMD_E_DVBT2 )
	{
	ret_val |= MN8847X_DMD_I2C_Read( DMD_BANK_T2_, DMD_DETFLG, Multipath_inter_level );  
	}

	return ret_val;
}

/*ISSY test A*/
/*
Purpose : To ignore ISSY field
The following is register setting to make the demod to ignore the ISSY.
  3rd, Adr:0xF6=0x04(default val=0x05)
  1st, Adr:0x7B=0xBC(default val=0x8C)

    Please test the above setting and check whether you can decode normally
or not. If the setting works, then the main issue would be the signal generator.
  *NOTE : This setting is just for monotoring SPLP(no FEF, no DNP),
          because under this situation ISSY function is optional.

Place to use : before DMD_device_reset()
*/
DMD_ERROR_t MN8847X_DMD_device_dvbt2_ignore_ISSY( void )
{
   DMD_ERROR_t ret = DMD_E_OK;

	MN8847X_DMD_I2C_Write(   DMD_BANK_C_ , DMD_FECSET3 , 0x04 ); //defaut 0x05, temperary, ISSY issue test 
	ret = MN8847X_DMD_I2C_Write(   DMD_BANK_T2_ , DMD_TSFSET2 , 0xBC );//to ignore ISSY field
			  
	return ret;  
}

/*ISSY test B*/
/*
Purpose : To check ISSY field enough or not in interleaving frame. 
< ETSI EN 302 755 V1.3.1 >
    Each Interleaving Frame for each PLP shall carry a TTO, a BUFS and at least one ISCR field.

B.  1. set 3rd, Adr:0xF6, bit[0]=0    <- just for monitoring setting !
    2. read 1st, Adr:0xB6, B7, B8
    3. check 1st, Adr:0xB7, bit[2:0]
To show if ISSY on or not .

    if bit[2:0] shows 0x7, ISSY is used. ISSY is consist of three factor, ISCR, TTO, BUFS.
    Above three bit corresponds to these factors as following :
    bit[2] : ISCR
    bit[1] : TTO
    bit[0] : BUFS
    if bit[2:0] shows other value in spite of ISSY on, there is possibility that input stream
    is inappropriate(becahse these three factor shall be carried in each interleaving frame)
 *NOTE : This setting is just for monotoring. this setting can't use for normal setting.
         And above bit[2:0] might show different value in normal setting.
*/
DMD_ERROR_t MN8847X_DMD_device_dvbt2_ISSY_validity_check( DMD_PARAMETER_t *param )
{
   DMD_ERROR_t ret = DMD_E_OK;
   
	   if (param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)
	   {  
			DMD_u8_t	rd_tmp, rd_real;
			MN8847X_DMD_I2C_Write(   DMD_BANK_C_ , DMD_FECSET3 , 0x04 ); //defaut 0x05, temperary, ISSY issue test 
			MN8847X_DMD_wait( 10);		  

			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TSFRDU , &rd_tmp );
			MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TSFRDM , &rd_real ); //need read from top to bottom, but only need check TSFRDM status .
			ret = MN8847X_DMD_I2C_Read(   DMD_BANK_T2_ , DMD_TSFRDL , &rd_tmp ); 
			HI_INFO_TUNER("ISSY status : TSFRDM = 0x%x \n",rd_real);  
       }
	return ret;  
}

#if (DEMOD_TYPE == DMD_TYPE_MN88472)
/* **************************************************** */
/* DVB-T Long delay echo ipmrovement */
/* 
[ Backgroud ]
DVB-T DTG test 10.8.5 Perfermance with long delay echo 
64QAM, CR 2/3, 2K 
< !!! > Si2158 + MN88472 fails  
SPEC : <= 22.2
before modified :  22.2
after modified  :  19.3 

[Principle]
Only affect DVB-T, FFT 2K, long echo delay item. 
The change will make the demodulator do more amendment in dealing echo interface.

[Side-effect]
it will degrade AWGN performance. 

[Notice]
1. since the modification affect another performance,
it needs some control loop :
Pls. add this function after signal locked and decoding begins, once a second

2. Mxl603+MN88472, the test result is OK. 
So, it's better to adjust Si2158 if possible.     
*/
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_dvbt_64qam_2k_long_delay_echoes_improve( DMD_PARAMETER_t *param )
{
    DMD_u8_t cur_dvbt_fft_mode , rd_dvbt_CLPRNK2_T;
 
	// only for Si2158 
	if (( param->system == DMD_E_DVBT)&&(param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED ))
	{		
		// detect FFT size
		MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_MDRD_T, &cur_dvbt_fft_mode );
		//FFT_S_T: bit[3:2] -> 0 : 2K mode; 2 : 8K mode
		cur_dvbt_fft_mode = (cur_dvbt_fft_mode & 0x0C)>>2; 

		//CLPRNK2_T setting check
		MN8847X_DMD_I2C_Read(   DMD_BANK_T_ , DMD_CLPRNK2_T, &rd_dvbt_CLPRNK2_T ); 

    HI_INFO_TUNER(" --- Pana DVB-T --- FFT mode[%x], CLPRNK2set[%x] \n ", cur_dvbt_fft_mode,rd_dvbt_CLPRNK2_T);
			    
		//FFT 2K detected and correponding CLPRNK2_T value not set yet 
		if ( (cur_dvbt_fft_mode == 0) && ( rd_dvbt_CLPRNK2_T != 0x74) )
		{
		  MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_CLPRNK2_T,  0x74 );
		}
		else if ((cur_dvbt_fft_mode != 0)&& ( rd_dvbt_CLPRNK2_T != 0x54))// FFT 8K status 
		{
		  MN8847X_DMD_I2C_Write(   DMD_BANK_T_ , DMD_CLPRNK2_T,  0x54 );//for other test items' performance 
		}  

	} 

   return DMD_E_OK;
}

/* **************************************************** */
/* DVB-C multipath tolerance ipmrovement */
/* 
[ Backgroud ]
DVB-C, 0.5us delay 
64QAM, symbol rate 6.875MS/s, 64QAM 
< !!! > Si2158 + MN88472 fails  
SPEC : <= 10
before modified :  10.3
after modified  :  8.7 

[Principle]
By setting smaller range of carrier frequency, it makes the demod can
receive a critical signal. 
Only affect DVB-C/ 

[Side-effect]
AFC range will be narrowed from 500Khz to 350Khz.

[ How to check AFC range ]
Change RF freq, and check whether demod is locked or not
    For exam : 
    -> RF= 665.7MHz & 666.3 : locked
    -> RF= 665.6MHz & 666.4 : not locked
    Then AFC range is -300 to +300 kHz

[ Where to use ]
in function "DMD_set_system"
*/
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_device_dvbc_multipath_tolerance_improve( void )
{
		DMD_ERROR_t ret;
		ret = MN8847X_DMD_I2C_Write(   DMD_BANK_C_ , DMD_QCAFCADD2L_C,  0x65 ); //default, 0x95;  margin more than 1.0 dB ; only for Si2158 + MN88472 
		return ret;
}

/*
  We can use Network ID to distiguish different Service 
*/
#endif //(DEMOD_TYPE == DMD_TYPE_MN88472)

