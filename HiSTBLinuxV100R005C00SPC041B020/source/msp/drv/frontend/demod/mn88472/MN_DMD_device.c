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
#include "MN88472.h"
#include "hi_drv_tuner.h"

/* **************************************************** */
/*	Constant*/
/* **************************************************** */
#define	DMD_DVBT_LOCKED_STATUS	10
#define	DMD_DVBT2_LOCKED_STATUS	13
#define	DMD_DVBC_LOCKED_STATUS	8

#define DMD_L1_BANK_SIZE 11
#define DMD_L1_REG_SIZE   8

/* '11/08/01 : OKAMOTO	Correct Error. */
#define DMD_ERROR_STATUS 0xFFFFFFFF


/* **************************************************** */
/*	Local Functions */
/* **************************************************** */
DMD_u32_t	DMD_RegRev(void);
DMD_u32_t	DMD_PSEQRev(DMD_PARAMETER_t *param);
DMD_u32_t	DMD_AGC(DMD_PARAMETER_t *param);
DMD_u32_t	DMD_System(DMD_PARAMETER_t *param);
DMD_u32_t	DMD_Status(DMD_PARAMETER_t *param);			//SSEQ Status
DMD_u32_t	DMD_BER( DMD_PARAMETER_t *param, DMD_u32_t *err , DMD_u32_t *sum);
DMD_u32_t	DMD_BER_dvbt2( DMD_PARAMETER_t *param, DMD_u32_t* err , DMD_u32_t* sum  , DMD_u32_t common);
DMD_u32_t	DMD_PER( DMD_PARAMETER_t *param, DMD_u32_t *err , DMD_u32_t *sum);
DMD_u32_t	DMD_CNR( DMD_PARAMETER_t *param, DMD_u32_t *cnr_i,DMD_u32_t *cnr_d);

/*! Information */
DMD_ERROR_t DMD_information_dvbt( DMD_PARAMETER_t *param , DMD_u32_t id , DMD_u32_t allflag);
DMD_ERROR_t DMD_information_dvbt2( DMD_PARAMETER_t *param , DMD_u32_t id, DMD_u32_t allflag);
DMD_ERROR_t DMD_information_dvbc( DMD_PARAMETER_t *param , DMD_u32_t id);
DMD_u32_t	DMD_get_l1( DMD_PARAMETER_t *param, DMD_u8_t l1info[11][8] , DMD_u32_t	bitw , DMD_u32_t bank , DMD_u32_t adr , DMD_u32_t pos , DMD_u32_t allflag);
DMD_ERROR_t DMD_get_l1all( DMD_PARAMETER_t *param, DMD_u8_t l1info[11][8]);
/* Scan ( for Channel Search )*/
DMD_ERROR_t DMD_scan_dvbt2( DMD_PARAMETER_t *param );
DMD_ERROR_t DMD_scan_dvbt( DMD_PARAMETER_t *param );
DMD_ERROR_t DMD_scan_dvbc( DMD_PARAMETER_t *param );

/*Global Variable*/
DMD_PARAMETER_t g_stMN88472Demod[TUNER_NUM];

/* **************************************************** */
/*	Functions*/
/* **************************************************** */
/* **************************************************** */
/*! System Support information */
/* **************************************************** */

DMD_ERROR_t	DMD_system_support( DMD_SYSTEM_t sys )
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
/*! Load Auto Control Sequence */
/* **************************************************** */
DMD_ERROR_t DMD_device_load_pseq( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t ret = DMD_E_OK;
	DMD_u32_t     i2cnum;
	DMD_u32_t    i2cAddr;
	DMD_u8_t	data;
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);
       
	i2cnum = pstTunerOps->enI2cChannel;
	i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T_;
       
	/* Load PSEQ Program */
	ret  = DMD_I2C_Write    ( i2cnum, i2cAddr, DMD_PSEQCTRL , 0x20 );
	ret |= DMD_I2C_Write    ( i2cnum, i2cAddr, DMD_PSEQSET  , 0x03 );
	ret |= DMD_I2C_WriteRead( i2cnum, i2cAddr, DMD_PSEQPRG  , MN88472_REG_AUTOCTRL , MN88472_REG_AUTOCTRL_SIZE , 0, 0 );
	ret |= DMD_I2C_Write    ( i2cnum, i2cAddr, DMD_PSEQSET  , 0x00 );

	/* Check Parity bit */
	ret |= DMD_I2C_Read( i2cnum, i2cAddr , DMD_PSEQFLG , &data);
	if( data & 0x10 ){
		DMD_DBG_TRACE( "ERROR: PSEQ Parity" );
		ret |= DMD_E_ERROR;

	}
	
	return ret;
}

/* **************************************************** */
/*! Check Auto Control Runs or not */
// PSEQSET bit0 -> 0 : PSEQ running; 1 : PSEQ hold. 
// Troy.wang added, 20130412 
/* **************************************************** */
DMD_ERROR_t DMD_device_pseq_status(DMD_PARAMETER_t* param, DMD_u8_t* pseq_status )
{
	DMD_ERROR_t ret;
	DMD_u8_t	data;
	DMD_u32_t     i2cnum;
    DMD_u32_t    i2cAddr;
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);
       
	i2cnum = pstTunerOps->enI2cChannel;
	i2cAddr = pstTunerOps->u32DemodAddress;
	ret = DMD_I2C_Read( i2cnum, i2cAddr+ DMD_BANK_T_, DMD_PSEQSET  , &data );
	*pseq_status = data&0x01;
    return ret;	
}

/* **************************************************** */
/*! Device Open */
/* **************************************************** */
DMD_ERROR_t DMD_device_open( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t ret = DMD_E_OK;
	ret = DMD_set_power_mode(param,DMD_PWR_MODE_NORMAL);
	return ret;
}
/* **************************************************** */
/*! Device Open */
/* **************************************************** */
DMD_ERROR_t DMD_device_close( DMD_PARAMETER_t* param )
{
	 return DMD_E_OK;
}
/* **************************************************** */
/*! Device Open */
/* **************************************************** */
DMD_ERROR_t DMD_device_term( DMD_PARAMETER_t* param )
{
	 return DMD_E_OK;
}

/* **************************************************** */
/*!	Get Register Settings for Broadcast system */
/* **************************************************** */
DMD_ERROR_t DMD_device_init( DMD_PARAMETER_t* param )
{
    DMD_ERROR_t	ret;
    DMD_u32_t     i2cnum;
    DMD_u32_t     i2caddr;
	DMD_u8_t	  rd;
    TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);

    i2cnum = pstTunerOps->enI2cChannel;
    i2caddr = pstTunerOps->u32DemodAddress;
    /* Tranfer common register setting */
    ret = DMD_trans_reg( i2cnum, i2caddr, MN88472_REG_COMMON );
    if(DMD_E_ERROR == ret)
    {
        return DMD_E_ERROR;
    }

    DMD_CHECK_ERROR( ret , "DMD_device_init" );

	//test I2C communication 
	DMD_I2C_Write(i2cnum, DMD_BANK_T2_ +i2caddr, DMD_CHIPRD  ,  0x99 );
	DMD_I2C_Read( i2cnum,DMD_BANK_T2_ +i2caddr, DMD_CHIPRD  ,  &rd );
	HI_INFO_TUNER("\n ---- Test Demod I2C !! --- reg[0xff] = 0x%x < 0x99 means I2C OK>\n", rd);
	
    return ret;
}
/* **************************************************** */
/*!	Set Register setting for each broadcast system & bandwidth */
/* **************************************************** */
DMD_ERROR_t	DMD_device_set_system( DMD_PARAMETER_t* param){
        DMD_ERROR_t	ret;
        DMD_u32_t     i2cnum;
        DMD_u32_t    i2cAddr;
        HI_UNF_TUNER_DEV_TYPE_E enTunerType;
        //float MN88472_IF;
        TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);       
        DMD_u8_t	nosupport;
        ret = DMD_E_OK;
        nosupport = 0;
        i2cnum = pstTunerOps->enI2cChannel;
        i2cAddr = pstTunerOps->u32DemodAddress;
        enTunerType = pstTunerOps->enTunerDevType;
        switch( param->system ){
            case DMD_E_DVBT2:
			switch( param->bw ){
				case DMD_E_BW_8MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT2_8MHZ );
					break;
				case DMD_E_BW_6MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT2_6MHZ );
					break;
				case DMD_E_BW_5MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT2_5MHZ );
					break;
				case DMD_E_BW_1_7MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT2_1_7MHZ );
					break;
				case DMD_E_BW_7MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT2_7MHZ );
					break;
				default:
					nosupport = 1;
					break;
			}
			break;
            case DMD_E_DVBT:
			switch( param->bw ){
				case DMD_E_BW_8MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT_8MHZ );
					break;
				case DMD_E_BW_7MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT_7MHZ );
					break;
				case DMD_E_BW_6MHZ:
					ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBT_6MHZ );
					break;
				default:
					nosupport = 1;
					break;	
			}
			break;
		case DMD_E_DVBC:
			ret = DMD_trans_reg( i2cnum, i2cAddr, MN88472_REG_DVBC );
			break;
		default:
				nosupport = 1;
	}

    /*The default IF is 5M, but the IF of TDA6651 is 36.167M, so we should set it again.*/
    if (HI_UNF_TUNER_DEV_TYPE_TDA6651 == enTunerType)
    {
        DMD_I2C_Write(i2cnum, 0x1c + i2cAddr, DMD_NCOFREQU, 0xc1) ;
        DMD_I2C_Write(i2cnum, 0x1c + i2cAddr, DMD_NCOFREQM, 0x8f) ;
        DMD_I2C_Write(i2cnum, 0x1c + i2cAddr, DMD_NCOFREQL, 0x9c) ;
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
DMD_ERROR_t DMD_device_pre_tune( DMD_PARAMETER_t* param )
{
	return DMD_E_OK;
}
/* **************************************************** */
/*! Pretune Process */
/* **************************************************** */
DMD_ERROR_t DMD_device_post_tune( DMD_PARAMETER_t* param )
{
	DMD_device_reset(param);
	return DMD_E_OK;
}
/* **************************************************** */
/*! Soft Reset */
/* **************************************************** */
DMD_ERROR_t DMD_device_reset( DMD_PARAMETER_t* param )
{     
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;

	DMD_I2C_Write(i2cnum, DMD_BANK_T2_ + i2cAddr,DMD_WAFCSET3,0x13);//default 0x3 ->[warning] only for MN88472
	DMD_I2C_Write(i2cnum, DMD_BANK_T2_ + i2cAddr,DMD_RSTSET1,0x9f);
   	tuner_mdelay( 10);	 //wait 10ms for Demod itself, troy.wang, 20130226 
	return DMD_E_OK;
}

/* **************************************************** */
/*! scan */
/* **************************************************** */
DMD_ERROR_t DMD_device_scan( DMD_PARAMETER_t* param )
{
	DMD_ERROR_t	ret;
	ret = DMD_E_ERROR;
	switch( param->system )
	{
	case DMD_E_DVBT2:
		ret = DMD_scan_dvbt2( param );
		break;
	case DMD_E_DVBT:
		ret = DMD_scan_dvbt( param );
		break;
	case DMD_E_DVBC:
		ret = DMD_scan_dvbc( param );
		break;
        default:
            break;
	}
	return ret;
}

/* **************************************************** */
/*! Set Information  */
/* **************************************************** */
DMD_ERROR_t DMD_device_set_info( DMD_PARAMETER_t* param , DMD_u32_t id , DMD_u32_t val )
{
	DMD_ERROR_t	ret =DMD_E_ERROR;
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;
	DMD_u8_t	rd;
	
	switch( param->system )
	{
		case DMD_E_DVBT:
			switch( id )
			{
			case DMD_E_INFO_DVBT_HIERARCHY_SELECT:
				DMD_I2C_Read( i2cnum, DMD_BANK_T_+i2cAddr , DMD_RSDSET_T , &rd );
				if( val == 1 )
				{
					rd |= 0x10;
				}
				else
				{
					rd &= 0xef;
				}
				DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_RSDSET_T , rd );
				param->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT]	= (rd >> 4) & 0x1;
				ret = DMD_E_OK;
				break;
			case DMD_E_INFO_DVBT_MODE:
				ret =DMD_E_OK;
				if( val == DMD_E_DVBT_MODE_NOT_DEFINED ){
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T  ,  0xba );
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDASET_T ,  0x13 );
				}
				else
				{
					DMD_I2C_MaskWrite( i2cnum,DMD_BANK_T_+ i2cAddr , DMD_MDSET_T , 0xf0 , 0xf0 );
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDASET_T    , 0x0  );
					switch( val )
					{
					case DMD_E_DVBT_MODE_2K:
						DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x0c , 0x00 );
						break;
					case DMD_E_DVBT_MODE_8K:
						DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x0c , 0x08 );
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
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T  ,  0xba );
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDASET_T ,  0x13 );
				}
				else
				{
					DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0xf0 , 0xf0 );
					DMD_I2C_Write( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDASET_T    , 0x0  );
					switch( val )
					{
					case DMD_E_DVBT_GI_1_32:
						DMD_I2C_MaskWrite( i2cnum,DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x03 , 0x00 );
						break;
					case DMD_E_DVBT_GI_1_16:
						DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x03 , 0x01 );
						break;
					case DMD_E_DVBT_GI_1_8:
						DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x03 , 0x02 );
						break;
					case DMD_E_DVBT_GI_1_4:
						DMD_I2C_MaskWrite( i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDSET_T , 0x03 , 0x03 );
						break;
					default:
						ret = DMD_E_ERROR;
						break;
					}

				}
				break;
					

			}
		case DMD_E_DVBT2:
			switch( id )
			{
				case	DMD_E_INFO_DVBT2_SELECTED_PLP	:
					rd = (DMD_u8_t) val;
					DMD_I2C_Write( i2cnum, DMD_BANK_T2_ + i2cAddr, DMD_PLPID , rd );
					ret = DMD_E_OK;
					break;

			}
            default:
                    break;
	}

	return ret;
}

/* **************************************************** */
/*! Get Information  */
/* **************************************************** */
DMD_ERROR_t DMD_device_get_info( DMD_PARAMETER_t* param , DMD_u32_t id )
{
	DMD_u32_t i;
	switch( param->system ){
		case DMD_E_DVBT:
			if( id == DMD_E_INFO_DVBT_ALL ){
				param->info[DMD_E_INFO_DVBT_ALL] = DMD_E_INFO_DVBT_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBT_ALL] ; i++ )
					DMD_information_dvbt( param , i , 1 );
			}
			else
					DMD_information_dvbt( param , id , 0 );
			break;
		case DMD_E_DVBT2:
			if( id == DMD_E_INFO_DVBT2_ALL ){
				param->info[DMD_E_INFO_DVBT2_ALL] = DMD_E_INFO_DVBT2_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBT2_ALL] ; i++ )
					DMD_information_dvbt2( param , i , 1);
			}
			else if( id == DMD_E_INFO_DVBT2_L1_ALL )
			{
				DMD_information_dvbt2( param , id , 0);
				for(i=DMD_E_INFO_DVBT2_L1_ALL+1;i<DMD_E_INFO_DVBT2_END_OF_INFORMATION;i++)
				{
					DMD_information_dvbt2( param , i , 1);
				}
			}
			else
					DMD_information_dvbt2( param , id , 0);
			break;
		case DMD_E_DVBC:
			if( id == DMD_E_INFO_DVBC_ALL ){
				param->info[DMD_E_INFO_DVBC_ALL] = DMD_E_INFO_DVBC_END_OF_INFORMATION;
				for( i=1 ; i < param->info[DMD_E_INFO_DVBC_ALL] ; i++ )
					DMD_information_dvbc( param , i );
			}
			else
					DMD_information_dvbc( param , id );
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
DMD_ERROR_t DMD_information_dvbt( DMD_PARAMETER_t *param , DMD_u32_t id , DMD_u32_t allflag)
{
	DMD_u32_t	err,sum;
	DMD_u32_t	cnr_i,cnr_d;
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T_;
	DMD_u8_t	rd;

	switch( id ){
		case	DMD_E_INFO_DVBT_ALL	:
			param->info[DMD_E_INFO_DVBT_ALL] = DMD_E_INFO_DVBT_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBT_REGREV	:
			param->info[DMD_E_INFO_DVBT_REGREV] = DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBT_PSEQRV	:
			param->info[DMD_E_INFO_DVBT_PSEQRV] = DMD_PSEQRev(param);
			break;
		case	DMD_E_INFO_DVBT_SYSTEM	:
			param->info[DMD_E_INFO_DVBT_SYSTEM] = DMD_System(param);
			break;
		case	DMD_E_INFO_DVBT_STATUS	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_LOCK	:
			param->info[DMD_E_INFO_DVBT_STATUS] = DMD_Status( param);
			if( param->info[DMD_E_INFO_DVBT_STATUS] >= DMD_DVBT_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBT_LOCK] = DMD_E_LOCKED;
			}
			else if( param->info[DMD_E_INFO_DVBT_LOCK] == DMD_E_LOCKED )
			{
				param->info[DMD_E_INFO_DVBT_LOCK] = DMD_E_ERROR;
			}
			break;
		case	DMD_E_INFO_DVBT_AGC	:
			param->info[DMD_E_INFO_DVBT_AGC] = DMD_AGC(param);
			break;
		case	DMD_E_INFO_DVBT_BERRNUM	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_BITNUM	:
			DMD_BER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT_BITNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBT_CNR_INT	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_CNR_DEC	:
			DMD_CNR(param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBT_PERRNUM	:
			if( allflag == 1 ) break;
		case	DMD_E_INFO_DVBT_PACKETNUM	:
			DMD_PER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT_PACKETNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBT_ERRORFREE	:
			DMD_I2C_Read( i2cnum,  i2cAddr, DMD_ERRFLG_T , &rd );
			param->info[DMD_E_INFO_DVBT_ERRORFREE] = (rd & 0x1)?0:1;
			break;
		case	DMD_E_INFO_DVBT_SQI	:
			//Get CNR
			DMD_CNR(param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT_CNR_DEC]  = cnr_d;
			//Get BER
			DMD_BER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT_BITNUM]   = sum;
			DMD_PER( param,&err,&sum);
			param->info[DMD_E_INFO_DVBT_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT_PACKETNUM]   = sum;
			//Get constelation mode 
				DMD_I2C_Read( i2cnum,   i2cAddr, DMD_TMCCD2_T , &rd );
				param->info[DMD_E_INFO_DVBT_CONSTELLATION]	= (rd >> 3 ) & 0x3; 
			   param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7; //troy, 20130131, DVB-T NON-HIERARCHY
			//Get code rate			
				DMD_I2C_Read(  i2cnum, i2cAddr, DMD_TMCCD3_T , &rd );
				param->info[DMD_E_INFO_DVBT_HP_CODERATE]	= (rd >> 3 ) & 0x7;
				param->info[DMD_E_INFO_DVBT_LP_CODERATE]	= rd & 0x7;
			//Calc SQI
			param->info[DMD_E_INFO_DVBT_SQI] = DMD_calc_SQI( param );
			break;
		case	DMD_E_INFO_DVBT_HIERARCHY_SELECT	:
			DMD_I2C_Read( i2cnum, i2cAddr, DMD_RSDSET_T , &rd );
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
			if( DMD_Status(param) < DMD_DVBT_LOCKED_STATUS ){
				//Sync is not established , TPS information is not available
				param->info[DMD_E_INFO_DVBT_TPS_ALL]	=	0;
			}
			else
			{
				param->info[DMD_E_INFO_DVBT_TPS_ALL]	=	1;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_TMCCD4_T , &rd );
				param->info[DMD_E_INFO_DVBT_MODE]	= rd & 0x3;
				param->info[DMD_E_INFO_DVBT_GI]	= (rd >> 2 ) & 0x3;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_TMCCD1_T , &rd );
				param->info[DMD_E_INFO_DVBT_LENGTH_INDICATOR]	= rd & 0x3f;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_TMCCD2_T , &rd );
				param->info[DMD_E_INFO_DVBT_CONSTELLATION]	= (rd >> 3 ) & 0x3; 
				param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_TMCCD3_T , &rd );
				param->info[DMD_E_INFO_DVBT_HP_CODERATE]	= (rd >> 3 ) & 0x7;
				param->info[DMD_E_INFO_DVBT_LP_CODERATE]	= rd & 0x7;
				//param->info[DMD_E_INFO_DVBT_HIERARCHY]	= rd & 0x7;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_CELLIDU_T , &rd );
				param->info[DMD_E_INFO_DVBT_CELLID]	= rd * 0x100;
				DMD_I2C_Read( i2cnum, i2cAddr , DMD_CELLIDL_T , &rd );
				param->info[DMD_E_INFO_DVBT_CELLID]	+= rd ;

			}
			break;

		default:
			return DMD_E_ERROR;
	}


	return DMD_E_OK;
}
/*! DVBT2 Information */
DMD_ERROR_t DMD_information_dvbt2( DMD_PARAMETER_t *param , DMD_u32_t id, DMD_u32_t allflag)
{
	DMD_u32_t	err,sum;
	DMD_u32_t	cnr_i,cnr_d;
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	DMD_u8_t	rd;
	static	DMD_u8_t	l1info[11][8];			//L1 Information Register
	switch( id ){
		case	DMD_E_INFO_DVBT2_ALL	:
			param->info[DMD_E_INFO_DVBT2_ALL] = DMD_E_INFO_DVBT2_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBT2_REGREV	:
			param->info[DMD_E_INFO_DVBT2_REGREV] = DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBT2_PSEQRV	:
			param->info[DMD_E_INFO_DVBT2_PSEQRV] = DMD_PSEQRev(param);
			break;
		case	DMD_E_INFO_DVBT2_SYSTEM	:
			param->info[DMD_E_INFO_DVBT2_SYSTEM] = DMD_System(param);
			break;
		case	DMD_E_INFO_DVBT2_LOCK	:
			param->info[DMD_E_INFO_DVBT2_STATUS] = DMD_Status(param);
			if( param->info[DMD_E_INFO_DVBT2_STATUS] >= DMD_DVBT2_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBT2_LOCK] = DMD_E_LOCKED;
			} else if( param->info[DMD_E_INFO_DVBT2_LOCK] == DMD_E_LOCKED )
			{
				param->info[DMD_E_INFO_DVBT2_LOCK] = DMD_E_LOCK_ERROR;
			}
			break;
		case	DMD_E_INFO_DVBT2_AGC	:
			param->info[DMD_E_INFO_DVBT2_AGC] = DMD_AGC(param);
			break;
		case	DMD_E_INFO_DVBT2_BERRNUM	:
		case	DMD_E_INFO_DVBT2_BITNUM	:
			DMD_BER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT2_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT2_BITNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBT2_CNR_INT	:
		case	DMD_E_INFO_DVBT2_CNR_DEC	:
			DMD_CNR(param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT2_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT2_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBT2_PERRNUM	:
		case	DMD_E_INFO_DVBT2_PACKETNUM	:
			DMD_PER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT2_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT2_PACKETNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBT2_ERRORFREE	:
				DMD_I2C_Read( i2cnum,DMD_BANK_T2_+ i2cAddr , DMD_ERRFLG , &rd );
				param->info[DMD_E_INFO_DVBT2_ERRORFREE] = (rd & 0x1)?0:1;
				break;
		case	DMD_E_INFO_DVBT2_SQI	:
			//Get CNR
			DMD_CNR(param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBT2_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBT2_CNR_DEC]  = cnr_d;
			//Get BER
			DMD_BER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBT2_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT2_BITNUM]   = sum;
			DMD_PER( param,&err,&sum);
			param->info[DMD_E_INFO_DVBT2_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBT2_PACKETNUM]   = sum;
			//Get constelation mode 
			param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD]= DMD_get_l1(  param, l1info, 3 , 3 , 1 , 7, allflag);
			//Get code rate
			param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= DMD_get_l1(  param, l1info, 3 , 3 , 1 , 4, allflag); 
			//Calc SQI
			param->info[DMD_E_INFO_DVBT2_SQI] = DMD_calc_SQI( param );
			break;
		case	DMD_E_INFO_DVBT2_MODE	:
				DMD_I2C_Read( i2cnum, DMD_BANK_T2_+i2cAddr , DMD_SSEQRD1 , &rd );
				param->info[DMD_E_INFO_DVBT2_MODE] = rd & 0x7;
				break;
		case	DMD_E_INFO_DVBT2_GI	:	
		  param->info[DMD_E_INFO_DVBT2_GI]= DMD_get_l1( param, l1info, 3 , 0 , 2 , 6 , 0); break;
			break;
		case	DMD_E_INFO_DVBT2_BERRNUM_C	:
		case	DMD_E_INFO_DVBT2_BITNUM_C	:
			DMD_BER_dvbt2(param,&err,&sum,1);
			param->info[DMD_E_INFO_DVBT2_BERRNUM_C]   = err;
			param->info[DMD_E_INFO_DVBT2_BITNUM_C]   = sum;
			break;
		case	DMD_E_INFO_DVBT2_SELECTED_PLP	:
				DMD_I2C_Read( i2cnum, DMD_BANK_T2_+i2cAddr , DMD_PLPID , &rd );
				param->info[DMD_E_INFO_DVBT2_SELECTED_PLP] = rd ;
				break;
		case DMD_E_INFO_DVBT2_L1_ALL:
			DMD_get_l1all(param, l1info );
			break;
		case DMD_E_INFO_DVBT2_TYPE :			  param->info[DMD_E_INFO_DVBT2_TYPE]= DMD_get_l1( param,l1info, 8 , 0 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_BW_EXT :			  param->info[DMD_E_INFO_DVBT2_BW_EXT]= DMD_get_l1( param,l1info, 1 , 0 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_S1 :			  param->info[DMD_E_INFO_DVBT2_S1]= DMD_get_l1( param, l1info, 3 , 0 , 1 , 6 , allflag); break;
		case DMD_E_INFO_DVBT2_S2 :			  param->info[DMD_E_INFO_DVBT2_S2]= DMD_get_l1( param, l1info, 4 , 0 , 1 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_PAPR :			  param->info[DMD_E_INFO_DVBT2_PAPR]= DMD_get_l1( param, l1info, 4 , 0 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_MOD :			  param->info[DMD_E_INFO_DVBT2_L1_MOD]= DMD_get_l1( param, l1info, 4 , 0 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_COD :			  param->info[DMD_E_INFO_DVBT2_L1_COD]= DMD_get_l1( param, l1info, 2 , 0 , 3 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_FEC_TYPE :		  param->info[DMD_E_INFO_DVBT2_L1_FEC_TYPE]= DMD_get_l1( param, l1info, 2 , 0 , 3 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_SIZE :		  param->info[DMD_E_INFO_DVBT2_L1_POST_SIZE]= DMD_get_l1( param, l1info, 18 , 1 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_INFO_SIZE :	  param->info[DMD_E_INFO_DVBT2_L1_POST_INFO_SIZE]= DMD_get_l1( param, l1info, 18 , 1 , 3 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_PILOT_PATTERN :		  param->info[DMD_E_INFO_DVBT2_PILOT_PATTERN]= DMD_get_l1( param, l1info, 4 , 0 , 4 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_TX_ID_AVAILABILITY :	  param->info[DMD_E_INFO_DVBT2_TX_ID_AVAILABILITY]= DMD_get_l1( param, l1info, 8 , 10 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_CELL_ID :			  param->info[DMD_E_INFO_DVBT2_CELL_ID]= DMD_get_l1( param, l1info, 16 , 10 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NETWORK_ID :		  param->info[DMD_E_INFO_DVBT2_NETWORK_ID]= DMD_get_l1( param, l1info, 16 , 10 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_T2_SYSTEM_ID :		  param->info[DMD_E_INFO_DVBT2_T2_SYSTEM_ID]= DMD_get_l1( param, l1info, 16 , 10 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_T2_FRAMES :		  param->info[DMD_E_INFO_DVBT2_NUM_T2_FRAMES]= DMD_get_l1( param, l1info, 8 , 0 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_DATA_SYMBOLS :	  param->info[DMD_E_INFO_DVBT2_NUM_DATA_SYMBOLS]= DMD_get_l1( param, l1info, 12 , 0 , 6 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_REGEN_FLAG :		  param->info[DMD_E_INFO_DVBT2_REGEN_FLAG]= DMD_get_l1( param, l1info, 3 , 0 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_POST_EXTENSION :	  param->info[DMD_E_INFO_DVBT2_L1_POST_EXTENSION]= DMD_get_l1( param, l1info, 1 , 0 , 4 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_RF :			  param->info[DMD_E_INFO_DVBT2_NUM_RF]= DMD_get_l1( param, l1info, 3 , 10 , 7 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_CURRENT_RF_IDX :		  param->info[DMD_E_INFO_DVBT2_CURRENT_RF_IDX]= DMD_get_l1( param, l1info, 3 , 10 , 7 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_SUB_SLICES_PER_FRAME :	  param->info[DMD_E_INFO_DVBT2_SUB_SLICES_PER_FRAME]= DMD_get_l1( param, l1info, 15 , 2 , 1 , 6 , allflag); break;
		case DMD_E_INFO_DVBT2_SUB_SLICE_INTERVAL :	  param->info[DMD_E_INFO_DVBT2_SUB_SLICE_INTERVAL]= DMD_get_l1( param, l1info, 22 , 2 , 3 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_PLP :			  param->info[DMD_E_INFO_DVBT2_NUM_PLP]= DMD_get_l1( param, l1info, 8 , 2 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_NUM_AUX :			  param->info[DMD_E_INFO_DVBT2_NUM_AUX]= DMD_get_l1( param, l1info, 4 , 9 , 5 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_PLP_MODE :		  param->info[DMD_E_INFO_DVBT2_PLP_MODE]= DMD_get_l1( param, l1info, 2 , 3 , 2 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_TYPE :		  param->info[DMD_E_INFO_DVBT2_FEF_TYPE]= DMD_get_l1( param, l1info, 4 , 9 , 0 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_LENGTH :		  param->info[DMD_E_INFO_DVBT2_FEF_LENGTH]= DMD_get_l1( param, l1info, 22 , 9 , 1 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_FEF_INTERVAL :		  param->info[DMD_E_INFO_DVBT2_FEF_INTERVAL]= DMD_get_l1( param, l1info, 8 , 9 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_ID :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_ID]= DMD_get_l1( param, l1info, 8 , 7 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TYPE :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TYPE]= DMD_get_l1( param, l1info, 3 , 3 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_PAYLOAD_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_PAYLOAD_TYPE]= DMD_get_l1( param, l1info, 5 , 3 , 0 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_GROUP_ID :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_GROUP_ID]= DMD_get_l1( param, l1info, 8 , 7 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_COD :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= DMD_get_l1( param, l1info, 3 , 3 , 1 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_MOD :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD]= DMD_get_l1( param, l1info, 3 , 3 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_ROTATION :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_ROTATION]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE]= DMD_get_l1( param, l1info, 2 , 3 , 1 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS_MAX :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS_MAX]= DMD_get_l1( param, l1info, 10 , 3 , 4 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_FRAME_INTEVAL :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_FRAME_INTEVAL]= DMD_get_l1( param, l1info, 8 , 3 , 6 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_LENGTH :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_LENGTH]= DMD_get_l1( param, l1info, 8 , 3 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_TYPE :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_TYPE]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_FF_FLAG :		  param->info[DMD_E_INFO_DVBT2_DAT_FF_FLAG]= DMD_get_l1(param, l1info, 1 , 7 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_ID :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_ID]= DMD_get_l1(param, l1info, 8 , 7 , 4 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TYPE :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_TYPE]= DMD_get_l1( param, l1info, 3 , 5 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_PAYLOAD_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_PAYLOAD_TYPE]= DMD_get_l1( param, l1info, 5 , 5 , 0 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_GROUP_ID :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_GROUP_ID]= DMD_get_l1( param, l1info, 8 , 7 , 5 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_COD :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_COD]= DMD_get_l1( param, l1info, 3 , 5 , 1 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_MOD :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_MOD]= DMD_get_l1( param, l1info, 3 , 5 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_ROTATION :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_ROTATION]= DMD_get_l1( param, l1info, 1 , 5 , 2 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_FEC_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_FEC_TYPE]= DMD_get_l1( param, l1info, 2 , 5 , 1 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS_MAX :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS_MAX]= DMD_get_l1( param, l1info, 10 , 5 , 4 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_FRAME_INTEVAL :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_FRAME_INTEVAL]= DMD_get_l1( param, l1info, 8 , 5 , 6 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_LENGTH :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_LENGTH]= DMD_get_l1( param, l1info, 8 , 5 , 3 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_TYPE :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_TYPE]= DMD_get_l1( param, l1info, 1 , 5 , 2 , 0 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_FF_FLAG :		  param->info[DMD_E_INFO_DVBT2_COM_FF_FLAG]= DMD_get_l1( param, l1info, 1 , 7 , 6 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_FRAME_IDX :		  param->info[DMD_E_INFO_DVBT2_FRAME_IDX]= DMD_get_l1( param, l1info, 8 , 8 , 0 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_TYPE_2_START :		  param->info[DMD_E_INFO_DVBT2_TYPE_2_START]= DMD_get_l1( param, l1info, 22 , 8 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_L1_CHANGE_COUNTER :	  param->info[DMD_E_INFO_DVBT2_L1_CHANGE_COUNTER]= DMD_get_l1( param, l1info, 8 , 8 , 1 , 7 , allflag); break;
		case DMD_E_INFO_DVBT2_START_RF_IDX :		  param->info[DMD_E_INFO_DVBT2_START_RF_IDX]= DMD_get_l1( param, l1info, 8 , 3 , 4 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_FIRST_RF_IDX :	  param->info[DMD_E_INFO_DVBT2_DAT_FIRST_RF_IDX]= DMD_get_l1( param, l1info, 3 , 7 , 2 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_START :		  param->info[DMD_E_INFO_DVBT2_DAT_PLP_START]= DMD_get_l1( param, l1info, 22 , 4 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS :	  param->info[DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS]= DMD_get_l1( param, l1info, 10 , 4 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_FIRST_RF_IDX :	  param->info[DMD_E_INFO_DVBT2_COM_FIRST_RF_IDX]= DMD_get_l1(param, l1info, 3 , 7 , 6 , 2 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_START :		  param->info[DMD_E_INFO_DVBT2_COM_PLP_START]= DMD_get_l1( param, l1info, 22 , 6 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS :	  param->info[DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS]= DMD_get_l1( param, l1info, 10 , 6 , 0 , 1 , allflag); break;
		case DMD_E_INFO_DVBT2_STATIC_FLAG :		  param->info[DMD_E_INFO_DVBT2_STATIC_FLAG]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 5 , allflag); break;
		case DMD_E_INFO_DVBT2_STATIC_PADDING_FLAG :	  param->info[DMD_E_INFO_DVBT2_STATIC_PADDING_FLAG]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 4 , allflag); break;
		case DMD_E_INFO_DVBT2_IN_BAND_A_FLAG :		  param->info[DMD_E_INFO_DVBT2_IN_BAND_A_FLAG]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 3 , allflag); break;
		case DMD_E_INFO_DVBT2_IN_BAND_B_FLAG :		  param->info[DMD_E_INFO_DVBT2_IN_BAND_B_FLAG]= DMD_get_l1( param, l1info, 1 , 3 , 2 , 2 , allflag); break;

	
	}
	return DMD_E_OK;
}
/*
Author  : Troy.wang
Date    : 20130621
Purpose : Detailed signal's modulation parameter detect(especially for unknow field test signal)
*/
void DMD_signal_information_printout_all( DMD_PARAMETER_t *param)
{
		DMD_u8_t	i;
		TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
	   
		DMD_get_info(param,DMD_E_INFO_LOCK);
		if ( param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED )
		{
			HI_INFO_TUNER(" CLK bias :  \n");  
			DMD_Carrier_Frequency_bias( param);  // check IF bias. 
			DMD_XTAL_Frequency_bias( param);    // check XTAL bias. 

			DMD_I2C_Read( i2cnum,  DMD_BANK_T2_+i2cAddr , DMD_DETFLG, &i );
			HI_INFO_TUNER("\n  Interference detect : DET FLG = 0x%x \n",i);  // to check interference 

			tuner_mdelay(2000);//wait for TS output 
			
			HI_INFO_TUNER(" BEGIN : Modulation parameters \n");  
			DMD_get_info(param,DMD_E_INFO_ALL);

			for(i = 0; i < param->info[DMD_E_INFO_ALL];i++)
			{
				HI_INFO_TUNER("No.%d : %30s : ", i, DMD_info_title(param->system, i));
				HI_INFO_TUNER( "[Text]%30s : [Value]%d \n", DMD_info_value(param->system, i, param->info[i]), param->info[i]);
			}				
			HI_INFO_TUNER(" END : Modulation parameters \n");  
		}
		else
		{
			HI_INFO_TUNER(" Signal unlocked, read out value invalid !! ");
		}
}
					
/*! DVBC Information */
DMD_ERROR_t DMD_information_dvbc( DMD_PARAMETER_t *param , DMD_u32_t id)
{
	DMD_u32_t	err,sum;
	DMD_u32_t	cnr_i,cnr_d,symbol_rate;
	DMD_u8_t	rd;

	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	switch( id ){
		case	DMD_E_INFO_DVBC_ALL	:
			param->info[DMD_E_INFO_DVBC_ALL] = DMD_E_INFO_DVBC_END_OF_INFORMATION;
			break;
		case	DMD_E_INFO_DVBC_REGREV	:
			param->info[DMD_E_INFO_DVBC_REGREV] = DMD_RegRev();
			break;
		case	DMD_E_INFO_DVBC_PSEQRV	:
			param->info[DMD_E_INFO_DVBC_PSEQRV] = DMD_PSEQRev(param);
			break;
		case	DMD_E_INFO_DVBC_SYSTEM	:
			param->info[DMD_E_INFO_DVBC_SYSTEM] = DMD_System(param);
			break;
		case	DMD_E_INFO_DVBC_LOCK	:
			param->info[DMD_E_INFO_DVBC_STATUS] = DMD_Status(param);
			if( param->info[DMD_E_INFO_DVBC_STATUS] >= DMD_DVBC_LOCKED_STATUS ){
				param->info[DMD_E_INFO_DVBC_LOCK] = DMD_E_LOCKED;
			}
			break;
		case	DMD_E_INFO_DVBC_AGC	:
			param->info[DMD_E_INFO_DVBC_AGC] = DMD_AGC(param);
			break;
		case	DMD_E_INFO_DVBC_BERRNUM	:
		case	DMD_E_INFO_DVBC_BITNUM	:
			DMD_BER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBC_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_BITNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBC_CNR_INT	:
		case	DMD_E_INFO_DVBC_CNR_DEC	:
			DMD_CNR(param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBC_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBC_CNR_DEC]  = cnr_d;
			break;
		case	DMD_E_INFO_DVBC_PERRNUM	:
		case	DMD_E_INFO_DVBC_PACKETNUM	:
			DMD_PER(param,&err,&sum);
			param->info[DMD_E_INFO_DVBC_PERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_PACKETNUM]   = sum;
			break;
		case	DMD_E_INFO_DVBC_ERRORFREE	:
			DMD_I2C_Read( i2cnum, i2cAddr+ DMD_BANK_T_ , DMD_ERRFLG_T , &rd );
			param->info[DMD_E_INFO_DVBT_ERRORFREE] = (rd & 0x1)?0:1;
			break;
	/* ---20130418,troy.wang added, DVBC SQI calculation--- BEGIN */	
		case	DMD_E_INFO_DVBC_SQI	:
			//Get CNR
			DMD_CNR( param,&cnr_i,&cnr_d);
			param->info[DMD_E_INFO_DVBC_CNR_INT]  = cnr_i;
			param->info[DMD_E_INFO_DVBC_CNR_DEC]  = cnr_d;
			//Get BER
			DMD_BER( param,&err,&sum);
			param->info[DMD_E_INFO_DVBC_BERRNUM]  = err;
			param->info[DMD_E_INFO_DVBC_BITNUM]   = sum;
			//Get constelation mode 
			DMD_I2C_Read(  i2cnum, DMD_BANK_C_+i2cAddr ,  DMD_SSEQMON1_C, &rd );
			param->info[DMD_E_INFO_DVBC_CONSTELLATION]= (rd&0x07);
			//Get code rate - DVBC no code rate
	         //param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD]= DMD_get_l1(   l1info, 3 , 3 , 1 , 4, allflag); 
			//Calc SQI
			param->info[DMD_E_INFO_DVBC_SQI] = DMD_calc_SQI( param );
			break;
	/* ---20130418,troy.wang added, DVBC SQI calculation--- END */
	
    //troy added, 20130507, some customer want to show DVB-C constellation and symbol rate
	   case DMD_E_INFO_DVBC_CONSTELLATION:
			DMD_I2C_Read(  i2cnum, DMD_BANK_C_+i2cAddr ,  DMD_SSEQMON1_C, &rd );
			param->info[DMD_E_INFO_DVBC_CONSTELLATION]= (rd&0x07); //pls. refer to structrue "DMD_DVBC_CONST_t"
			break;
	   case DMD_E_INFO_DVBC_SYMBOL_RATE:
			DMD_DVBC_GET_SYMBOL_RATE(param,&symbol_rate);
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
DMD_u32_t	DMD_RegRev(void)
{
	return DMD_RegSet_Rev;
}
//! Infomation : Auto control Version
DMD_u32_t	DMD_PSEQRev(DMD_PARAMETER_t *param)
{
    TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
    DMD_u8_t	rd;	
    DMD_I2C_Read( i2cnum, DMD_BANK_T_+i2cAddr , DMD_PSEQOP1_T , &rd );
    return (DMD_u32_t) rd;
}
//! Infomation : AGC Loop filter Level
DMD_u32_t	DMD_AGC(DMD_PARAMETER_t *param)
{
    TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
    DMD_u32_t	ret;
    DMD_u8_t	rd;
	DMD_I2C_Write(  i2cnum, DMD_BANK_T2_+i2cAddr , DMD_AGCRDSET, 0x00); 
    
    DMD_I2C_Read( i2cnum, DMD_BANK_T2_+i2cAddr , DMD_AGCRDU , &rd );
    ret = rd * 4;
    DMD_I2C_Read( i2cnum, DMD_BANK_T2_+i2cAddr , DMD_AGCRDL , &rd );
    ret += rd >> 6;
    
    return ret;
}
DMD_s32_t DMD_Carrier_Frequency_bias( DMD_PARAMETER_t* param)
{ 
	DMD_u8_t  CarrierErrU = 0,CarrierErrM = 0, CarrierErrL = 0, CarrierDVBCFlag = 0;
	DMD_u32_t CarrierErrRd = 0; //unsigned
	DMD_s32_t CarrierErrKHz = 0; //signed. 
	DMD_u8_t  notsupport = 0,fNegativeCarrierErr = 0;
	
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	/* Get estimated carrier error */	
	switch( param->system )
	 {
		case DMD_E_DVBT2:
		     {
			   // AGC monitor AGCRD selection : Carrier frequency error
			    DMD_I2C_Write(  i2cnum , DMD_BANK_T2_+i2cAddr , DMD_AGCRDSET, 0x06 );
			   //AGCRD read [15:0]
			 	DMD_I2C_Read(  i2cnum , DMD_BANK_T2_ +i2cAddr, DMD_AGCRDU , &CarrierErrU);
				DMD_I2C_Read(  i2cnum , DMD_BANK_T2_ +i2cAddr, DMD_AGCRDL , &CarrierErrL);
				HI_INFO_TUNER("dvbt2 carrier, 0x%x%x \n",CarrierErrU,CarrierErrL);

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
		      DMD_I2C_Write( i2cnum ,  DMD_BANK_T2_+i2cAddr , DMD_AGCRDSET, 0x00 );
			} //case DMD_E_DVBT2 ends. 
			break;			
			
		case DMD_E_DVBT:
				{		
				DMD_u8_t dvbt_fft_mode;
				DMD_u32_t dvbt_ffts;
			   // Synchronization monitor SYN2RD_T selection : Carrier frequency error
				DMD_I2C_Write(  i2cnum , DMD_BANK_T_+i2cAddr , DMD_SYN2RDSET_T, 0x00 );
				//SYN2RDU read [10:0]
			 	DMD_I2C_Read(  i2cnum , DMD_BANK_T_+i2cAddr , DMD_SYN2RDU_T, &CarrierErrU);
				DMD_I2C_Read( i2cnum ,  DMD_BANK_T_ +i2cAddr, DMD_SYN2RDL_T, &CarrierErrL);

				CarrierErrRd = ( ((DMD_u32_t)CarrierErrU << 8) & 0xFF00) + CarrierErrL;

				/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
				if (0x0400 & CarrierErrRd) //valid 11 bits.
				{
				CarrierErrRd = ((~CarrierErrRd) & 0x000007FF) + 0x1;
				fNegativeCarrierErr = 1;
				}
				/*Carrier frequency error = (fS*10^6) /ffts * SYN2RD[10:0] [Hz] 
				                                   = SYN2RD[10:0]*(fS*10^3) / ffts [Khz]
				where				
				ffts : FFT size (8K: 8192, 2K: 2048)
				fS : FFT sampling frequency
				8 MHz bandwidth : 64/7 MHz
				7 MHz bandwidth : 8 MHz
				6 MHz bandwidth : 48/7 MHz
				  */
				DMD_I2C_Read(  i2cnum , DMD_BANK_T_ +i2cAddr, DMD_MDRD_T, &dvbt_fft_mode );
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

			DMD_I2C_Read(  i2cnum , DMD_BANK_C_ +i2cAddr, DMD_AFCMON4_C, &CarrierErrL);
			DMD_I2C_Read(  i2cnum , DMD_BANK_C_ +i2cAddr, DMD_AFCMON3_C, &CarrierErrM);
				CarrierErrRd = (((DMD_u32_t)CarrierErrM << 8)& 0xFF00) + CarrierErrL;
			DMD_I2C_Read(  i2cnum , DMD_BANK_C_ +i2cAddr, DMD_AFCMON2_C, &CarrierErrU);
			    CarrierErrRd = ( ((DMD_u32_t)CarrierErrU << 16) & 0xFF0000) + CarrierErrRd;

			/* Signed Two's Compliment, so if the MSB is 1, set invert and add 1 */
			DMD_I2C_Read(  i2cnum , DMD_BANK_C_ +i2cAddr, DMD_AFCMON1_C, &CarrierDVBCFlag);
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
DMD_s32_t DMD_XTAL_Frequency_bias( DMD_PARAMETER_t* param)
{ 
	DMD_u8_t  ClockErrU = 0, ClockErrL = 0;
	DMD_u32_t ClockErr = 0;
	DMD_s32_t ClockErrPPM = 0;
	DMD_u8_t  fNegativeClockErr = 0;
	DMD_u8_t nosupport = 0;

	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	/* Get estimated clock error */	
	switch( param->system ){
		case DMD_E_DVBT2:
			//Synchronization monitor SYN1RD selection : Clock frequency error
			DMD_I2C_Write(  i2cnum , DMD_BANK_T2_+i2cAddr , DMD_SYN1RDSET, 0x01 );

			//Calculate clock frequency error from SYN1RD : read [15:0]
			DMD_I2C_Read( i2cnum ,  DMD_BANK_T2_ +i2cAddr, DMD_SYN1RDU , &ClockErrU );
			DMD_I2C_Read(  i2cnum,  DMD_BANK_T2_ +i2cAddr, DMD_SYN1RDL, &ClockErrL );
			break;			
		case DMD_E_DVBT:
			//Synchronization monitor SYN1RD _T selection : Clock frequency error
			DMD_I2C_Write(  i2cnum , DMD_BANK_T_+i2cAddr, DMD_SYN1RDSET_T, 0x01 );

			//Calculate clock frequency error from SYN1RD_T : read [15:0]
			DMD_I2C_Read( i2cnum ,  DMD_BANK_T_ +i2cAddr, DMD_SYN1RDU_T , &ClockErrU );
			DMD_I2C_Read(  i2cnum,  DMD_BANK_T_ +i2cAddr, DMD_SYN1RDL_T , &ClockErrL );
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
/*! Information : System detect (from register setting) */
DMD_u32_t	DMD_System(DMD_PARAMETER_t *param)
{
    DMD_u32_t	ret;
    TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;
    DMD_u8_t	rd;
       

	DMD_I2C_Read( i2cnum, DMD_BANK_T2_ + i2cAddr, DMD_DTVSET , &rd );

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
DMD_u32_t	DMD_Status( DMD_PARAMETER_t *param)
{
	DMD_u8_t	rd;
	DMD_u32_t	ret;
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;
    

	ret = 0;
	switch( param->system ){
		case DMD_E_DVBT:
			DMD_I2C_Read( i2cnum, DMD_BANK_T_ + i2cAddr, DMD_SSEQRD_T , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
		case DMD_E_DVBT2:
			DMD_I2C_Read( i2cnum, DMD_BANK_T2_ + i2cAddr, DMD_SSEQFLG , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
		case DMD_E_DVBC:
			DMD_I2C_Read( i2cnum, DMD_BANK_C_ + i2cAddr, DMD_SSEQMON2_C , &rd );
			ret = (DMD_u32_t) (rd & 0xf);
			break;
              default:
                    break;
	};

	return ret;
}
//! Information : ERROR Num
DMD_u32_t	DMD_BER( DMD_PARAMETER_t *param, DMD_u32_t* err , DMD_u32_t* sum )
{

       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T_;
	DMD_u8_t	rd;

	switch( param->system )
	{
		case DMD_E_DVBT:
		case DMD_E_DVBC:
			//SET BERSET1[5] = 0
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERSET1_T , &rd );
			rd &= 0xDF;	//1101_1111
			DMD_I2C_Write( i2cnum, i2cAddr , DMD_BERSET1_T , rd );
			//SET BERRDSET[3:0] = 0101
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERRDSET_T, &rd );
			rd = (rd & 0xF0 ) | 0x5;
			DMD_I2C_Write( i2cnum, i2cAddr , DMD_BERRDSET_T, rd );
			//Read ERROR
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERRDU_T , &rd );
			*err = rd * 0x10000;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERRDM_T , &rd );
			*err += rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERRDL_T , &rd );
			*err += rd ;
			//Read BERLEN
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERLENRDU_T , &rd );
			*sum = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_BERLENRDL_T , &rd );
			*sum += rd;
			*sum = *sum * 203 * 8;
			break;
		case DMD_E_DVBT2:
			DMD_BER_dvbt2(param, err , sum , 0 );	//Data PLP
			break;
              default:
                     break;
	}

    return 0;
}

//! Information : ERROR Num
DMD_u32_t	DMD_BER_dvbt2( DMD_PARAMETER_t *param,DMD_u32_t* err , DMD_u32_t* sum  , DMD_u32_t common)
{
    TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T2_;
    DMD_u8_t	rd;


    //DATA PLP 
    DMD_I2C_Read ( i2cnum,i2cAddr , DMD_BERSET , &rd );

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
    DMD_I2C_Write( i2cnum,i2cAddr , DMD_BERSET , rd );
    //Read ERROR
    DMD_I2C_Read( i2cnum,i2cAddr , DMD_BERRDU , &rd );
    *err = rd * 0x10000;
    DMD_I2C_Read( i2cnum,i2cAddr , DMD_BERRDM , &rd );
    *err += rd * 0x100;
    DMD_I2C_Read( i2cnum,i2cAddr , DMD_BERRDL , &rd );
    *err += rd ;
    //Read BERLEN
    DMD_I2C_Read( i2cnum,i2cAddr , DMD_BERLEN , &rd );
    *sum = (1 << (rd & 0xf) );
    if( common == 0 )
    {
        DMD_I2C_Write( i2cnum, i2cAddr , DMD_TPDSET2 , 0x3 );
    }
    else
    {
        DMD_I2C_Read ( i2cnum, i2cAddr , DMD_TPD2    , &rd );
    }
    if( (rd & 0x1 ) == 1 )
    {
    //FEC TYPE = 1
        switch( (rd>>2) & 0x7 )
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
        switch( (rd>>2) & 0x7 )
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
DMD_u32_t	DMD_CNR( DMD_PARAMETER_t *param, DMD_u32_t* cnr_i , DMD_u32_t* cnr_d )
{
	DMD_s32_t	cnr;
	DMD_s32_t	sig,noise;
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;
	DMD_u8_t	rd;

	switch( param->system)
	{
		case DMD_E_DVBT:
                     i2cAddr += DMD_BANK_T_;
			DMD_I2C_Read( i2cnum, i2cAddr, DMD_CNRDU_T , &rd );
			cnr = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNRDL_T , &rd );
			cnr += rd;
			if( cnr != 0 )
			{
				cnr = 65536 / cnr;
				cnr = log10_easy( cnr ) + 200;
				if( cnr < 0 ) cnr = 0;
			}
			else
				cnr = 0;

			*cnr_i = (DMD_u32_t ) cnr / 100;
			*cnr_d = (DMD_u32_t ) cnr % 100;
			break;
		case DMD_E_DVBT2:
                     i2cAddr += DMD_BANK_T2_;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNRDU , &rd );
			cnr = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNRDL , &rd );
			cnr += rd;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNFLG , &rd );
			if( cnr != 0 )
			{
				if( rd & 0x4 )
				{
					//MISO
					cnr = 16384 / cnr;
					cnr = log10_easy( cnr ) - 600;
					if( cnr < 0 ) cnr = 0;
					*cnr_i = (DMD_u32_t ) cnr / 100;
					*cnr_d = (DMD_u32_t ) cnr % 100;
				}
				else
				{
					//SISO
					cnr = 65536 / cnr;
					cnr = log10_easy( cnr ) + 200;
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
                     i2cAddr += DMD_BANK_C_;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNMON1_C , &rd );
			sig = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNMON2_C , &rd );
			sig += rd;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNMON3_C , &rd );
			noise = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_CNMON4_C , &rd );
			noise += rd;

			if( noise != 0 )
				cnr = log10_easy(sig * 8  / noise);
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
DMD_u32_t	DMD_PER( DMD_PARAMETER_t *param, DMD_u32_t* err , DMD_u32_t* sum)
{
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T_;
	DMD_u8_t	rd;

	switch( param->system)
	{
		case DMD_E_DVBT:
		case DMD_E_DVBC:
		case DMD_E_DVBT2:
			//Read ERROR
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_PERRDU , &rd );
			*err = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_PERRDL , &rd );
			*err += rd ;
			//Read BERLEN
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_PERLENRDU , &rd );
			*sum = rd * 0x100;
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_PERLENRDL , &rd );
			*sum += rd;
			break;
            default:
                    break;
	}
	return 0;
}

// fSYM = fADCK / RATEMON_C * (2^18) 
DMD_ERROR_t DMD_DVBC_GET_SYMBOL_RATE(  DMD_PARAMETER_t *param,DMD_u32_t *dvbc_get_symbolrate_Khz )
{
	DMD_ERROR_t ret = DMD_E_ERROR;  
	DMD_u8_t rd;
	DMD_u32_t fadc;
	
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
		
	if ( param->system == DMD_E_DVBC)
    {
		//Read FAD_C 
		DMD_I2C_Read(  i2cnum, DMD_BANK_C_ +i2cAddr, DMD_RATERDU_C , &rd );
		fadc = rd * 0x10000;
		DMD_I2C_Read(  i2cnum, DMD_BANK_C_ +i2cAddr, DMD_RATERDM_C , &rd );
		fadc += rd * 0x100;
		DMD_I2C_Read(  i2cnum, DMD_BANK_C_ +i2cAddr, DMD_RATERDL_C , &rd );
		fadc += rd ;

		 //*dvbc_get_symbolrate = (Xtal*10^6)/fadc*(2^18) -> unit : Hz
		 //  = (xtal*10^3)*(2^16)/fadc*(2^2)   -> unit Khz, for better accuracy, for most 32bits. 
     *dvbc_get_symbolrate_Khz = (DMD_ADCK_FREQ*65536)/fadc*4;//troy, 20130507, treat XTAL seperately 
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
DMD_ERROR_t DMD_scan_dvbt2( DMD_PARAMETER_t *param )
{
	DMD_u32_t	st;
	DMD_u8_t	rd;
	DMD_u32_t	now;
	DMD_ERROR_t ret = DMD_E_ERROR;
	DMD_u32_t	timeout = 1800;

	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T2_;

	DMD_timer(&st);

	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;	
        
	do
	{
		//P1 Give up
		DMD_I2C_Read( i2cnum, i2cAddr  ,DMD_SSEQFLG , &rd );
        
        if( rd & 0x40 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSIGNAL;	
			break;
		}
		//Search Error
		/*if( rd & 0x20 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSYNC;	
			break;
		}*/
		//Sync judgement
		if( (rd & 0xf) >= 13 )
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}
		DMD_wait(1);			//wait 1ms
		DMD_timer( &now );
	}
	while( now - st < timeout );	//timeout 1.8
     
	return ret;
}
DMD_ERROR_t DMD_scan_dvbt( DMD_PARAMETER_t *param )
{
	DMD_u32_t	st;
	DMD_u32_t	now;
	DMD_u32_t	timeout = 1200;
	DMD_u8_t	rd;
	DMD_ERROR_t ret = DMD_E_ERROR;

	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T_;

	DMD_timer(&st);
	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;	

	//set mode&gi search
	DMD_set_info( param , DMD_E_INFO_DVBT_MODE , DMD_E_DVBT_MODE_NOT_DEFINED );
	DMD_set_info( param , DMD_E_INFO_DVBT_GI   , DMD_E_DVBT_GI_NOT_DEFINED );

	do
	{
		DMD_I2C_Read( i2cnum, i2cAddr, DMD_SSEQRD_T , &rd );

		//OFDM ERROR
		if( rd & 0x80 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSIGNAL;	
			break;
		}
		//Mode Search
		if( rd & 0x20 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSYNC;	
			break;
		}
		//Sync detection
		if( (rd & 0xf) >= 9 )
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}
		DMD_wait(1);			//wait 1ms
		DMD_timer( &now );
	}
	while( now - st < timeout );	//timeout
	return ret;
}
DMD_ERROR_t DMD_scan_dvbc( DMD_PARAMETER_t *param )
{
	DMD_u32_t	st;
	DMD_u32_t	now;
	DMD_u32_t	timeout = 1800;
	DMD_ERROR_t ret = DMD_E_ERROR;
	DMD_u8_t	rd;

	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_C_;

	DMD_timer(&st);
	param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_ERROR;	

	do
	{
		DMD_I2C_Read( i2cnum, i2cAddr, DMD_STSMON_C , &rd );

		//QAM ERR
		if( rd & 0x40 )
		{
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCK_NOSYNC;	
			break;
		}
		DMD_I2C_Read( i2cnum, i2cAddr, DMD_DMDSTSMON1_C , &rd );
		//VQLOCK
		if( rd & 0x1 )
		{
			ret = DMD_E_OK;
			param->info[DMD_E_INFO_LOCK] = DMD_E_LOCKED;	
			break;
		}
		DMD_wait(1);			//wait 1ms
		DMD_timer( &now );
	}
	while( now - st < timeout );	//timeout

	return ret;
}

DMD_u32_t	DMD_get_l1( DMD_PARAMETER_t *param, DMD_u8_t l1info[11][8] , DMD_u32_t	bitw , DMD_u32_t bank , DMD_u32_t adr , DMD_u32_t pos , DMD_u32_t allflag)
{
	DMD_s32_t	flag = bitw;
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T2_;
	DMD_u32_t	ret = 0;
    	DMD_u8_t	data;


	for(;;)
	{
		if( allflag == 1 )
		{
			data = l1info[bank][adr];

		}
		else
		{
			DMD_I2C_Write( i2cnum, i2cAddr , DMD_TPDSET1 , 0 );
			DMD_I2C_Write( i2cnum, i2cAddr , DMD_TPDSET2 , (DMD_u8_t) bank );
			DMD_I2C_Read( i2cnum, i2cAddr , (DMD_u8_t)(DMD_TPD1 + adr) , &data );
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

DMD_ERROR_t	DMD_get_l1all( DMD_PARAMETER_t *param, DMD_u8_t l1info[DMD_L1_BANK_SIZE][DMD_L1_REG_SIZE])
{
       TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress + DMD_BANK_T2_;
	DMD_u8_t i,j;
	DMD_u8_t	data;

	for( i=0; i < DMD_L1_BANK_SIZE ; i++ )
	{
		DMD_I2C_Write( i2cnum, i2cAddr , DMD_TPDSET1 ,  0 );
		DMD_I2C_Write( i2cnum, i2cAddr , DMD_TPDSET2 ,  i );
		for( j=0; j< DMD_L1_REG_SIZE; j ++ )
		{
			DMD_I2C_Read( i2cnum, i2cAddr , DMD_TPD1 + j , &data );
			l1info[i][j] = data;
		}
	}

	return DMD_E_OK;
}

DMD_ERROR_t	DMD_set_error_flag_output(DMD_PARAMETER_t *param, DMD_u8_t bErrorFlagOutputEnable )
{
       DMD_u8_t data;
	   TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
       DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
       DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress;
	   
	if(bErrorFlagOutputEnable){		
		/* 1st,Adr:0x09(TSSET2) bit[2:0]=1 ("001") */
		if( DMD_I2C_Read(  i2cnum, i2cAddr+DMD_BANK_T2_ , DMD_TSSET2 , &data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		data &= 0xF8;
		data |= 0x1; //'01'
		if( DMD_I2C_Write(  i2cnum, i2cAddr+DMD_BANK_T2_ , DMD_TSSET2 , data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		/* 1st,Adr:0xD9(FLGSET) bit[6:5]=0  ("00") */
		/*
		if( DMD_I2C_Read(param,  DMD_BANK_T2_ , DMD_FLGSET , &data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		data &= 0x9F;
		if( DMD_I2C_Write(param,  DMD_BANK_T2_ , DMD_FLGSET , data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		*/
	}else{
		//Default setting. INT signal ouput 
		/* 1st,Adr:0x09(TSSET2) bit[2:0]=1 ("000") */
		if( DMD_I2C_Read( i2cnum, i2cAddr+ DMD_BANK_T2_ , DMD_TSSET2 , &data ) == DMD_E_ERROR )
		{
			return DMD_E_ERROR;
		}
		data &= 0xF8; //'00'
		if( DMD_I2C_Write( i2cnum, i2cAddr+ DMD_BANK_T2_ , DMD_TSSET2 , data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
	}
	return DMD_E_OK;
}


/* '11/08/29 : OKAMOTO	Select TS output. */
DMD_ERROR_t DMD_set_ts_output( DMD_PARAMETER_t *param,DMD_TSOUT ts_out )
{
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
	switch(ts_out)
       {
       
	case DMD_E_TSOUT_PARALLEL_FIXED_CLOCK:
		//TS parallel (Fixed clock mode)		TSSET1:0x00	FIFOSET:0xE1
		if( DMD_I2C_Write( i2cnum,DMD_BANK_T2_ +i2cAddr ,DMD_TSSET1 , 0x00 ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		if( DMD_I2C_Write(  i2cnum,DMD_BANK_T_+i2cAddr, DMD_FIFOSET , 0xE1 ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		break;
		
	case DMD_E_TSOUT_PARALLEL_VARIABLE_CLOCK:
		//TS parallel (Variable clock mode)		TSSET1:0x00	FIFOSET:0xE3
		if( DMD_I2C_Write(  i2cnum,DMD_BANK_T2_ +i2cAddr ,DMD_TSSET1 , 0x00 ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		if( DMD_I2C_Write( i2cnum,DMD_BANK_T_+i2cAddr, DMD_FIFOSET , 0xE3 ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		break;
	case DMD_E_TSOUT_SERIAL_VARIABLE_CLOCK:
		//TS serial(Variable clock mode)		TSSET1:0x1D	FIFOSET:0xE3
		if( DMD_I2C_Write( i2cnum,DMD_BANK_T2_ +i2cAddr,DMD_TSSET1 , 0x1D ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		if( DMD_I2C_Write( i2cnum,DMD_BANK_T_+i2cAddr, DMD_FIFOSET , 0xE3 ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		break;
	default:
		return DMD_E_ERROR;
	}
	return DMD_E_OK;
}

DMD_ERROR_t DMD_set_power_mode( DMD_PARAMETER_t *param,DMD_PWR_MODE_t running_mode ) 
{
    DMD_ERROR_t  ret_val = DMD_E_OK ;
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	switch (running_mode)
	{
		case DMD_PWR_MODE_NORMAL: //wake up fucntion; after waken up no need to do init again, just re-tune, re-scan 
			ret_val = DMD_I2C_Write(  i2cnum,DMD_BANK_T2_+i2cAddr, DMD_PWDSET, 0x00 );
			break;

		case DMD_PWR_MODE_STANDBY:
			ret_val = DMD_I2C_Write( i2cnum, DMD_BANK_T2_ +i2cAddr , DMD_PWDSET, 0x3E );
			break;

		case DMD_PWR_MODE_SLEEP:
			ret_val = DMD_I2C_Write( i2cnum, DMD_BANK_T2_+i2cAddr, DMD_PWDSET, 0x3F );
			break;

		default:
			HI_INFO_TUNER("---Power mode selected error !--");
			ret_val = DMD_E_ERROR;
			break;
	}

    return (ret_val);
}

/*
Purpose     : Increase compatibility of locking non-standard signal, which has high ratio of Dummy cell -- Against DVB-T2 SPEC
              This is sometimes caused by customer who set Modulation parameter by themselves.
Side effect : No   
Creator     : Troy.wang, 20130626
*/
static void DMD_lock_high_ratio_dummy_cell_signal(DMD_PARAMETER_t *param)
{
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
	
	DMD_I2C_Write(i2cnum,i2cAddr+DMD_BANK_T2_,DMD_WAFCSET3,0x13); //default 0x03
}


/*
Purpose      : Container, for future use
Where to put : DMD_set_system()
Creator      : Troy.wang, 20130626
*/
void DMD_increase_compatibility_with_nonstandard_signal(DMD_PARAMETER_t *param,DMD_PARAMETER_t *param__UNUSED)
{
    DMD_lock_high_ratio_dummy_cell_signal(param);
}

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
DMD_ERROR_t DMD_device_dvbt_64qam_2k_long_delay_echoes_improve( DMD_PARAMETER_t *param )
{
	TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
	
    DMD_u8_t cur_dvbt_fft_mode , rd_dvbt_CLPRNK2_T;
 
	// only for Si2158 
	if (( param->system == DMD_E_DVBT)&&(param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED ))
	{		
		// detect FFT size
		DMD_I2C_Read(  i2cnum, DMD_BANK_T_+i2cAddr , DMD_MDRD_T, &cur_dvbt_fft_mode );
		//FFT_S_T: bit[3:2] -> 0 : 2K mode; 2 : 8K mode
		cur_dvbt_fft_mode = (cur_dvbt_fft_mode & 0x0C)>>2; 

		//CLPRNK2_T setting check
		DMD_I2C_Read( i2cnum,  DMD_BANK_T_+i2cAddr , DMD_CLPRNK2_T, &rd_dvbt_CLPRNK2_T ); 

    HI_INFO_TUNER(" --- Pana DVB-T --- FFT mode[%x], CLPRNK2set[%x] \n ", cur_dvbt_fft_mode,rd_dvbt_CLPRNK2_T);
			    
		//FFT 2K detected and correponding CLPRNK2_T value not set yet 
		if ( (cur_dvbt_fft_mode == 0) && ( rd_dvbt_CLPRNK2_T != 0x74) )
		{
		  DMD_I2C_Write(  i2cnum, DMD_BANK_T_ +i2cAddr, DMD_CLPRNK2_T,  0x74 );
		}
		else if ((cur_dvbt_fft_mode != 0)&& ( rd_dvbt_CLPRNK2_T != 0x54))// FFT 8K status 
		{
		  DMD_I2C_Write( i2cnum,  DMD_BANK_T_ +i2cAddr, DMD_CLPRNK2_T,  0x54 );//for other test items' performance 
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
DMD_ERROR_t DMD_device_dvbc_multipath_tolerance_improve( DMD_PARAMETER_t *param  )
{
		DMD_ERROR_t ret;
		TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
	
		ret = DMD_I2C_Write(  i2cnum, DMD_BANK_C_+i2cAddr , DMD_QCAFCADD2L_C,  0x65 ); //default, 0x95;  margin more than 1.0 dB ; only for Si2158 + MN88472 
		return ret;
}

DMD_ERROR_t DMD_device_read_P1_flag( DMD_PARAMETER_t *param , DMD_u8_t* p1flg_rd )
{ 
		DMD_ERROR_t  ret_val = DMD_E_OK ;
		TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    	DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    	DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

		//Get FEF status. S2 = xxx1, mixed frame, means T2 super frame including FEF part.  
		ret_val = DMD_I2C_Read(i2cnum, DMD_BANK_T2_+i2cAddr , DMD_P1FLG, p1flg_rd );  

	    return ret_val;
}

/*
Multipath interference detection
DETFLG[1:0],Larger values indicate larger interference levels.
INPUT  : DTV system (DVB-C in cable, no multipath)
OUTPUT : Multipath interface level
Note   : Valid only when signal locked
Creator : troy.wang, 20130628
*/
DMD_ERROR_t DMD_device_read_strong_echo(DMD_PARAMETER_t* param,DMD_u8_t* Multipath_inter_level)
{ 
   DMD_ERROR_t  ret_val = DMD_E_OK ;
   TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
   DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
   DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
		
	if( param->system == DMD_E_DVBT )
	{
	ret_val = DMD_I2C_Read(i2cnum, DMD_BANK_T_+i2cAddr, DMD_DETFLG_T, Multipath_inter_level );  
	}
	else if( param->system == DMD_E_DVBT2 )
	{
	ret_val |= DMD_I2C_Read( i2cnum,DMD_BANK_T2_+i2cAddr, DMD_DETFLG, Multipath_inter_level );  
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
DMD_ERROR_t DMD_device_dvbt2_ignore_ISSY( DMD_PARAMETER_t *param  )
{
   DMD_ERROR_t ret = DMD_E_OK;
   TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;

	DMD_I2C_Write( i2cnum,  DMD_BANK_C_+i2cAddr , DMD_FECSET3 , 0x04 ); //defaut 0x05, temperary, ISSY issue test 
	ret = DMD_I2C_Write(  i2cnum, DMD_BANK_T2_ +i2cAddr, DMD_TSFSET2 , 0xBC );//to ignore ISSY field
			  
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
DMD_ERROR_t DMD_device_dvbt2_ISSY_validity_check( DMD_PARAMETER_t *param )
{
   DMD_ERROR_t ret = DMD_E_OK;
   TUNER_OPS_S* pstTunerOps = (TUNER_OPS_S*)(param->userdata);      
    DMD_u32_t     i2cnum = pstTunerOps->enI2cChannel;
    DMD_u32_t     i2cAddr = pstTunerOps->u32DemodAddress ;
   
	   if (param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED)
	   {  
			DMD_u8_t	rd_tmp, rd_real;
			DMD_I2C_Write(  i2cnum, DMD_BANK_C_ +i2cAddr, DMD_FECSET3 , 0x04 ); //defaut 0x05, temperary, ISSY issue test 
			tuner_mdelay( 10);		  

			DMD_I2C_Read(  i2cnum, DMD_BANK_T2_+i2cAddr , DMD_TSFRDU , &rd_tmp );
			DMD_I2C_Read(  i2cnum, DMD_BANK_T2_ +i2cAddr, DMD_TSFRDM , &rd_real ); //need read from top to bottom, but only need check TSFRDM status .
			ret = DMD_I2C_Read( i2cnum,  DMD_BANK_T2_+i2cAddr , DMD_TSFRDL , &rd_tmp ); 
			HI_INFO_TUNER("ISSY status : TSFRDM = 0x%x \n",rd_real);  
       }
	return ret;  
}

/*
  We can use Network ID to distiguish different Service 
*/

