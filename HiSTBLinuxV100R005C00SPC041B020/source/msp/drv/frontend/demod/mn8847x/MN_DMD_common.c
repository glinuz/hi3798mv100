/* **************************************************** */
/*!
   @file	MN_DMD_driver.c
   @brief	Panasonic Demodulator Driver
   @author	R.Mori
   @date	2011/6/30
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#include "MN_DMD_common.h"
#include "MN_I2C.h"
#include "MN8847x.h" 
#include "MN_DMD_device.h"


/* **************************************************** */
/* I2C useful function */
/* **************************************************** */

DMD_ERROR_t	MN8847X_DMD_I2C_MaskWrite(  DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t mask , DMD_u8_t  data )
{
	DMD_u8_t	rd;
	DMD_ERROR_t ret;
	ret =MN8847X_DMD_I2C_Read(  slvadr , adr , &rd );
	rd |= mask & data;
	rd &= (mask^0xff) | data;
	ret |= MN8847X_DMD_I2C_Write(  slvadr , adr ,rd );

	return ret;
}

/* **************************************************** */
/* Calculate BER by integer only */
/* **************************************************** */
#define DMD_BER_UNIT	(10000000)	//DMD_ber_int = 1 means 10^-7
#define DMD_BER_LIMIT	(215)		//2^31 / DMD_BER_UNIT
DMD_u32_t	MN8847X_DMD_ber_int( DMD_u32_t	err , DMD_u32_t sum )
{
	DMD_u32_t	ret = DMD_BER_UNIT;
	DMD_u32_t	mul;
	if( sum == 0 )
	{
		ret = DMD_BER_UNIT;
	}
	else if( err >= sum )
	{
		ret = DMD_BER_UNIT;
	}
	else
	{	
		//To get integer value , multiply BER_UNIT to err.
		//(avoid overflow integer limit(2^32))
		for(mul=1;mul<DMD_BER_UNIT;mul*=10)
		{
			if( err < (DMD_BER_LIMIT*mul) )
			{
				ret = err * (DMD_BER_UNIT/mul) / sum;
				ret *= mul;
				break;
			}
		}
	}

	return ret;

}
/* **************************************************** */
/* Get RF Frequency in kHz unit */
/* **************************************************** */
DMD_u32_t	MN8847X_DMD_get_freq_kHz(DMD_PARAMETER_t* param )
{
	DMD_u32_t	ret = 0;
	switch( param->funit ){
		case DMD_E_MHZ:
			ret = param->freq * 1000;
			break;
		case DMD_E_KHZ:
			ret = param->freq ;
			break;
		case DMD_E_HZ:
			ret = param->freq / 1000;
			break;
	}

	return ret;

}
/* **************************************************** */
/* Calculate SQI (Nordig) */
/* **************************************************** */
#define CNR_MULT	100
//---------------------------------------------
//Nordig required CNR table
//CNR_P1 = CNR_p1 * 100

static DMD_u32_t DMD_DVBT_CNR_P1[3][5] =
{	{ 510 ,	 690 ,  790 , 890 , 970 } ,
	{1080 ,	1310 , 1460 ,1560 , 1600} ,
	{1650 , 1870 , 2020 ,2160 , 2250 }
};
static DMD_u32_t DMD_DVBT2_CNR_P1[4][6] =
{	{ 350 ,	 470 ,  560 , 660 , 720 , 770 } ,
	{ 870 ,	1010 , 1140 ,1250 , 1330, 1380} ,
	{1300 , 1480 , 1620 ,1770 , 1870, 1940} ,
	{1700 , 1940 , 2080 ,2290 , 2430, 2510}
};

//20130418,troy.wang added, DVBC SQI calculation, as it's has no SPEC to obey,
//and the coding method(conventional + RS)is same as DVB-T, so the arithmetic is same as DVB-T. 
static DMD_u32_t DMD_DVBC_CNR_P1[5] =
{	
	2600, 3300, 2900, 2300, 2000
};

/* 
Purpose  : DVB-T SQI meat Nordig test spec.
Cause    : DVB-T CNR value read from register is not accurate, transfer it
Position : After DMD_CNR(), before SQI calculation
Formula :
 - 64QAM : C/N value(converted) = C/N value(measured)^2 * 0.01156 + C/N value(measured)*  1.069 - 9.72(ori : -10.32)
 - 16QAM : C/N value(converted) = C/N value(measured)^2 * 0.0005 + C/N value(measured) * 1.2468 - 6.8348
 - QPSK  : C/N value(converted) = C/N value(measured)^2 * 0.0001 + C/N value(measured) * 1.1380 - 3.0955
input : 
     1. CNR value(x100) after DMD_CNR()
     2. DVB-T constellation 
return :
    CNR value(x100) after formula
date : 20140508, troy.wangyx
*/
DMD_u32_t MN8847X_DMD_DVBT_MAKE_CNR_ACCURATE(DMD_PARAMETER_t* param, DMD_DVBT_CONST_t dvbt_const, DMD_DVBT_CR_t dvbt_cr)
{
	DMD_u32_t	cnr_bef = 0;
	DMD_u32_t	cnr_aft = 0;

	cnr_bef = param->info[DMD_E_INFO_CNR_INT] * CNR_MULT + param->info[DMD_E_INFO_CNR_DEC];
	cnr_aft = cnr_bef;

	#ifdef DETAIL_DIG_DEBUG_ON 
	printk(" --- DVB-T transfer ori = %d   --- ",cnr_bef);
	#endif
	
	MN8847X_DMD_get_info(param,DMD_E_INFO_LOCK);
	if ((param->system == DMD_E_DVBT)&&(param->info[DMD_E_INFO_LOCK] == DMD_E_LOCKED))
	{			
		switch( dvbt_const ){
			case DMD_E_DVBT_CONST_64QAM:
				if ((dvbt_cr == DMD_E_DVBT_CR_3_4)&&(cnr_bef<=2084))
					cnr_aft = ((cnr_bef*cnr_bef)/100*1156/100 + cnr_bef*1069 - 1072000)/1000;//troy.wangyx, 20140523, modify for SQI show in range
				else if((dvbt_cr == DMD_E_DVBT_CR_2_3)&&(cnr_bef<=2031))
					cnr_aft = ((cnr_bef*cnr_bef)/100*1156/100 + cnr_bef*1069 - 1072000)/1000;//troy.wangyx, 20140523, modify for SQI show in range
				else
					cnr_aft = ((cnr_bef*cnr_bef)/100*1156/100 + cnr_bef*1069 - 972000)/1000;
				break;
			case DMD_E_DVBT_CONST_16QAM:
				cnr_aft = ((cnr_bef*cnr_bef)/100*5 + cnr_bef*12468 - 6834800)/10000;
				break;
			case DMD_E_DVBT_CONST_QPSK:
				cnr_aft = ((cnr_bef*cnr_bef)/100*1 + cnr_bef*11380 - 3095500)/10000;
				break;
			default:
				break;
		}
	}

	return cnr_aft;

}
DMD_u32_t	MN8847X_DMD_calc_SQI( DMD_PARAMETER_t* param)
{
	DMD_u32_t	cnr=0;
	DMD_s32_t	cnr_rel=0;
	DMD_u32_t	beri=0;	
	DMD_u32_t	mod=0,cr=0;
	DMD_u32_t	sqi=0,ber_sqi=0;
	//DMD_u8_t   multipath_interface_level=0;

	cnr = param->info[DMD_E_INFO_CNR_INT] * CNR_MULT + param->info[DMD_E_INFO_CNR_DEC];

	if(  param->info[DMD_E_INFO_BERRNUM] != 0 )
	{
			beri = param->info[DMD_E_INFO_BITNUM] /  param->info[DMD_E_INFO_BERRNUM];
	}
	else
	{
		beri = 100000000;
	}

	//Interference detect,troy.wang added, 20130628, DVB-T2, for OdB echo test, cnr < nordig spec, SQI = 0
	//DMD_device_read_strong_echo(param,&multipath_interface_level);
	//multipath_interface_level = multipath_interface_level & (0x03); //bit[1:0]
	/*20140415,troy.wangyx masked, change a new way to display SQI, to prevent SQI = 0, while PER = 0.
        - Considering of field environment. First priority is to follow CNR read from DMD
	    - SQI show small value when cnr_rel lower than limiation but PER = 0
	*/

	if( param->system == DMD_E_DVBT )
	{
			DMD_DVBT_HIERARCHY_t hierarchy_exist;
			DMD_DVBT_HIER_SEL_t hierarchy_selection;

			mod = param->info[DMD_E_INFO_DVBT_CONSTELLATION];

			// Check HP stream exist or not					
			hierarchy_exist = param->info[DMD_E_INFO_DVBT_HIERARCHY];

			/* 0 : DMD_E_DVBT_HIER_SEL_LP(default),	1: DMD_E_DVBT_HIER_SEL_HP 
			[ Attention !]Selection should be executed at topper layer: 
			DMD_SET_DVBT_HP_LP_MODE(DMD_PARAMETER_t* param, DMD_u32_t val) // val -> 1 :  HP ; 0 : LP 
			*/
			hierarchy_selection = param->info[DMD_E_INFO_DVBT_HIERARCHY_SELECT];

			if (hierarchy_exist != DMD_E_DVBT_HIERARCHY_NO)
			{
					if (hierarchy_selection == DMD_E_DVBT_HIER_SEL_HP)//Hierarchy, Alpha 1,2,4 & Customer chooses to decode HP data stream
					{						
						cr = param->info[DMD_E_INFO_DVBT_HP_CODERATE];
						mod = DMD_E_DVBT_CONST_QPSK; //HP only supports QPSK
					}	
					else //Non-hierarchy OR Customer choose LP when HP&LP both transmitted. 
					{
						cr = param->info[DMD_E_INFO_DVBT_LP_CODERATE];
					}
			}
			else //no hierarchy 
			{   
				//troy, 20130131, tested, found that if not open HIERARCHY, DVB-T code rate gets from HP area. 
				 cr = param->info[DMD_E_INFO_DVBT_HP_CODERATE];
			}		
		
		if( mod >= 3 || cr >= 5 )
			{
			return 0;	
			}
		else
			{
			    //20140508, make CNR accurate 
				cnr = MN8847X_DMD_DVBT_MAKE_CNR_ACCURATE(param,(DMD_DVBT_CONST_t)mod,(DMD_DVBT_CR_t)cr);//troy.wangyx, 20140523, modify for SQI show in range
				cnr_rel = cnr - DMD_DVBT_CNR_P1[mod][cr];

				//20140415,troy.wangyx, change a new way to display SQI, to prevent SQI = 0, while PER = 0.
				if((cnr_rel < -700)\
				&&(param->info[DMD_E_INFO_DVBT_PERRNUM] == 0)&&(param->info[DMD_E_INFO_DVBT_PACKETNUM] > 0))//cnr decrease sharply but video played well
				{
					#ifdef DETAIL_DIG_DEBUG_ON 
					printk(" --- DVB-T SQI NOISE too strong, set CNR to fixed value  --- \n");
					#endif
				   cnr_rel = -400; // 4dB lower than C/Nnordig, SQI depends on BER
				}			
			}

			
	}
	else if( param->system == DMD_E_DVBT2 )
	{
		mod = param->info[DMD_E_INFO_DVBT2_DAT_PLP_MOD];
		cr  = param->info[DMD_E_INFO_DVBT2_DAT_PLP_COD];

#if (DEMOD_TYPE == DMD_TYPE_MN88473)//20140416, temp solution for SQI show way for T2-Lite, currently, no Nordig standard.
		if (param->t2_work_profile_mode == DMD_E_DVBT2_WORK_MODE_LITE)
			{
				cnr_rel = 100;

				if (param->info[DMD_E_INFO_DVBT2_PERRNUM] > 0)
					{
						cnr_rel = -200;
					}
			}
		else if (param->t2_work_profile_mode == DMD_E_DVBT2_WORK_MODE_BASE)
#endif
			{
				if( mod >= 4 || cr >= 6 )
					{
					return 0;	
					}
				else
					{
						cnr_rel = cnr - DMD_DVBT2_CNR_P1[mod][cr];

						//20140415,troy.wangyx change a new way to display SQI, to prevent SQI = 0, while PER = 0.
						if((cnr_rel < -300)\
						&&(param->info[DMD_E_INFO_DVBT2_PERRNUM] == 0)&&(param->info[DMD_E_INFO_DVBT2_PACKETNUM] > 0))//cnr decrease sharply but video played well
						{
							#ifdef DETAIL_DIG_DEBUG_ON 
							printk(" --- DVB-T2 SQI NOISE too strong, set CNR to fixed value  --- \n");
							#endif
						   cnr_rel = -108; // 1dB lower than C/Nnordig, SQI depends on BER //show 30%
						}
					}		
			}
	}
	else if( param->system == DMD_E_DVBC ) //20130418,troy.wang added,DVBC SQI calculation 
	{
		mod = param->info[DMD_E_INFO_DVBC_CONSTELLATION];
		if( mod > 4 )	return 0;	
			cnr_rel = cnr - DMD_DVBC_CNR_P1[mod];
	}
	else
	{
		return 0;
	}

#ifdef DETAIL_DIG_DEBUG_ON 
		if( param->system == DMD_E_DVBT )
		{
			printk(" --- DVB-T SQI   --- ");
			printk(" cnr = %d,  cnr_rel = %d, bit error =  %d / %d \n ", cnr,cnr_rel,param->info[DMD_E_INFO_BERRNUM],param->info[DMD_E_INFO_BITNUM]);
			printk(" constallation_t = %d, coderate_t= %d \n",mod, cr);
		}
		else if ( param->system == DMD_E_DVBT2 )
		{
			printk(" --- DVB-T2 SQI --- ");
			printk(" cnr = %d, cnr_rel = %d, bit error =  %d / %d \n ", cnr,cnr_rel, param->info[DMD_E_INFO_BERRNUM],param->info[DMD_E_INFO_BITNUM]);
			printk(" constallation_t2 = %d, coderate_t2 = %d \n",mod, cr);
		}		
#endif // #ifdef COMMON_DEBUG_ON 
		
	if( param->system == DMD_E_DVBT ){
		if( cnr_rel < -700 )
		{
			sqi = 0;
		}
		else 
		{
			if( beri < 1000 )		//BER>10-3
			{
				ber_sqi = 0;
			} 
			else if( beri < 10000000 )	//BER>10-7
			{
				ber_sqi = 20 * MN8847X_log10_easy( beri ) - 40000;
				ber_sqi /= 1000;
			}
			else
			{
				ber_sqi = 100;
			}

			if( cnr_rel <= 300 )
			{
				sqi = ( ( (cnr_rel - 300) /10) + 100 ) * ber_sqi;
			}
			else
			{
				sqi = ber_sqi * CNR_MULT;
			}
		}
	}
	else if( param->system == DMD_E_DVBT2 )
	{
		if( cnr_rel < -300 ){		//CNR_rel < -3[dB]
			sqi = 0;
		}
		else if( cnr_rel <= 300 )	//-3[dB] <= CNR_rel <= +3[dB]
		{
			if( beri < 10000 )	//BER>10e-4
			{
				ber_sqi = 0;
			}
			else if( beri < 10000000 )	// 10e-4 > BER > 10e-7
			{
				ber_sqi = CNR_MULT*(100/15);
			}
			else	//BER < 10e-7
			{
				ber_sqi = CNR_MULT*(100/6);
			}

			sqi = ((cnr_rel + (3*CNR_MULT) ) * ber_sqi) / CNR_MULT;
		}
		else			//CNR>+3[dB]
		{
			sqi = 100*CNR_MULT; //this is correct ! 20140417 confirmed.
		}
	}
	else if( param->system == DMD_E_DVBC )//20130418,troy.wang added, DVBC SQI calculation
	{
		if( cnr_rel < -700 )
		{
			sqi = 0;
		}
		else 
		{
			if( beri < 1000 )		//BER>10-3
			{
				ber_sqi = 0;
			} 
			else if( beri < 10000000 )	//BER>10-7
			{
				ber_sqi = 20 * MN8847X_log10_easy( beri ) - 40000;
				ber_sqi /= 1000;
			}
			else
			{
				ber_sqi = 100;
			}

			if( cnr_rel <= 300 )
			{
				sqi = ( ( (cnr_rel - 300) /10) + 100 ) * ber_sqi;
			}
			else
			{
				sqi = ber_sqi * CNR_MULT;
			}
		}
	}
	else
	{
		return 0;
	}
	 
#ifdef DETAIL_DIG_DEBUG_ON 
	printk(" ---  SQI_done  :  cnr_rel = %d, beri = %d, ber_sqi = %d, sqi = %d --- ",cnr_rel, beri, ber_sqi, sqi/100 ); //20130129
#endif 
	return sqi / CNR_MULT;
}


/* **************************************************** */
/* Local Functions */
/* **************************************************** */
/*! Tranfer Register Array */
DMD_ERROR_t MN8847X_DMD_trans_reg(  DMD_I2C_Register_t* t)
{
	int i=0;
	while(i<DMD_REGISTER_MAX)
	{
		/* '11/08/05 : OKAMOTO Correct slave addr in "DMD_trans_reg". */
		DMD_u8_t slvadr;

 // Pls. be noticed that "I2C address, should make it include R/W bit."
 // For example, if write DMD_BANK_T2_, the slave ID should be (0x1c << 1). 
		switch(t[i].slvadr){
		case 0x1c:
			slvadr = DMD_BANK_T2_;
			break;
		case 0x18:
			slvadr = DMD_BANK_T_;
			break;
		case 0x1a:
			slvadr = DMD_BANK_C_;
			break;
		default:
			return DMD_E_ERROR;
		}
		if( MN8847X_DMD_I2C_Write( slvadr , t[i].adr , t[i].data ) == DMD_E_ERROR ){
			return DMD_E_ERROR;
		}
		i++;
		if(t[i].flag == DMD_E_END_OF_ARRAY ) break;
	}
	return DMD_E_OK;
}

/* **************************************************** */
/* Register  */
/* **************************************************** */
DMD_ERROR_t MN8847X_DMD_send_registers(  DMD_u8_t	*slaveset , DMD_u8_t* regset )
{
	DMD_u32_t	i;
	for(i=0;;)
	{
		if( regset[i] == 0xff ) break;
		MN8847X_DMD_I2C_Write( slaveset[regset[i]] , regset[i+1] , regset[i+2] );
		i=i+3;
	}
	return DMD_E_OK;
}


DMD_ERROR_t MN8847X_DMD_read_registers(void)
{
       DMD_u32_t        i;
       DMD_u8_t rd;
       #ifdef PC_CONTROL_FE
	   DMD_s8_t	buf[10];
       #endif
       printk("\n DMD_read_registers --- IN ! \n");

       for(i=0;;i++)
       {			   
               MN8847X_DMD_I2C_Read( (DMD_BANK_T2_), i, &rd );
               printk("DVB-T2 : Regaddr[0x%x]  =  0x%x     \n",i,rd);
			   
					#ifdef PC_CONTROL_FE
						if( (i%20) == 19 )
						{
						printk("--More--");
						fgets( buf , 10 , stdin );
						}
					#endif
						
			       if(  i == 0xff ) break;
       }

        for(i=0;;i++)
       {              
               MN8847X_DMD_I2C_Read( (DMD_BANK_T_), i, &rd );
               printk("DVB-T Regaddr[0x%x]  =  0x%x     \n",i,rd);
		
				  	#ifdef PC_CONTROL_FE
						if( (i%20) == 19 )
						{
						printk("--More--");
						fgets( buf , 10 , stdin );
						}
					#endif
						
			    if( i == 0xff ) break;
       }
       printk("\n DMD_read_registers --- OUT ! \n");
       return DMD_E_OK;
}

/* **************************************************** */
/* Log Table */
/* **************************************************** */
static DMD_s32_t	logtbl[] = {
0	,-1000,-699	,-523,-398,
-301,-222 ,-155	,-97 ,-46 ,
0	,41	,79	,114	,146	,
176	,204	,230	,255	,
279	,301	,322	,342	,
362	,380	,398	,415	,
431	,447	,462	,477	,
491	,505	,519	,531	,
544	,556	,568	,580	,
591	,602	,613	,623	,
633	,643	,653	,663	,
672	,681	,690	,699	,
708	,716	,724	,732	,
740	,748	,756	,763	,
771	,778	,785	,792	,
799	,806	,813	,820	,
826	,833	,839	,845	,
851	,857	,863	,869	,
875	,881	,886	,892	,
898	,903	,908	,914	,
919	,924	,929	,934	,
940	,944	,949	,954	,
959	,964	,968	,973	,
978	,982	,987	,991	,
996	,1000};

/* Calculate Log10 */
DMD_u32_t MN8847X_log10_easy( DMD_u32_t cnr )
{
	DMD_u32_t	c;
	DMD_s32_t	ret;
	c = 0;

	while( cnr > 100 ){
		cnr = cnr / 10;
		c++;
	}
	ret = logtbl[cnr] + c*1000 + 1000;

	return ret;
}

/* **************************************************** */
/* return Info value string */
/* **************************************************** */
DMD_text_t	MN8847X_DMD_value_decimal( DMD_u32_t val )
{
	static DMD_s8_t	buf[256];
	//sprintf_s( buf , 256, "DEC %d",  val ); //this function is included in <stdio.h>
	sprintf( buf , "DEC %d",  val ); //if your system didn't support 'sprintf_s', pls. use older one
	return buf;
}
DMD_text_t	MN8847X_DMD_value_hex( DMD_u32_t val )
{
	static DMD_s8_t	buf[256];
	//sprintf_s( buf , 256, "HEX 0x%x",  val );
	sprintf( buf , "DEC %d",  val ); //if your system didn't support 'sprintf_s', pls. use older one
	return buf;
}

DMD_text_t MN8847X_DMD_value_textlist( DMD_textlist_t* list , DMD_u32_t val )
{
	if( val >= list->size )
	{

		return "Not Defined";
	}
	else
	{

		return list->list[val];
	}
}

DMD_text_t MN8847X_DMD_info_value_common( DMD_u32_t id , DMD_u32_t val )
{
	switch( id )
	{
		case	DMD_E_INFO_ALL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_REGREV	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_PSEQREV	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_SYSTEM	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_SYSTEM	,val);
		case	DMD_E_INFO_LOCK	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_LOCK	,val);
		case	DMD_E_INFO_AGC	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_BERRNUM	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_BITNUM	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_CNR_INT	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_CNR_DEC	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_PERRNUM	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_PACKETNUM	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ERRORFREE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
		default: break;

	}
	return "Not Defined\n";
}
#ifndef DMD_DISABLE_ISDB
DMD_text_t MN8847X_DMD_info_value_isdbt( DMD_u32_t id , DMD_u32_t val )
{

	switch( id )
	{
		case	DMD_E_INFO_ISDBT_STATUS		:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_STATUS	,val);		
		case	DMD_E_INFO_ISDBT_BERRNUM_A	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_BITNUM_A	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_BERRNUM_B	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_BITNUM_B	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_BERRNUM_C	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_BITNUM_C	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_ERRORFREE_A	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
		case	DMD_E_INFO_ISDBT_ERRORFREE_B	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
		case	DMD_E_INFO_ISDBT_ERRORFREE_C	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
		case	DMD_E_INFO_ISDBT_MODE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_MODE	,val);
		case	DMD_E_INFO_ISDBT_GI	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_GI	,val);
		case	DMD_E_INFO_ISDBT_SYS_TMCC	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_SYSTEM	,val);
		case	DMD_E_INFO_ISDBT_COUNTDOWN	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_EMGFLG	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_YESNO	,val);		
		case	DMD_E_INFO_ISDBT_PART	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_YESNO	,val);
		case	DMD_E_INFO_ISDBT_MAPA	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_MAP	,val);
		case	DMD_E_INFO_ISDBT_CRA	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_CR	,val);
		case	DMD_E_INFO_ISDBT_INTA	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_INT	,val);
		case	DMD_E_INFO_ISDBT_SEGA	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_MAPB	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_MAP	,val);
		case	DMD_E_INFO_ISDBT_CRB	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_CR	,val);
		case	DMD_E_INFO_ISDBT_INTB	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_INT	,val);
		case	DMD_E_INFO_ISDBT_SEGB	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_MAPC	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_MAP	,val);
		case	DMD_E_INFO_ISDBT_CRC	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_CR	,val);
		case	DMD_E_INFO_ISDBT_INTC	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBT_INT	,val);
		case	DMD_E_INFO_ISDBT_SEGC	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_ISDBT_PHCOR	:	return	MN8847X_DMD_value_decimal(val);		

	}
	return "Not Defined";
}

DMD_text_t MN8847X_DMD_info_value_isdbs( DMD_u32_t id , DMD_u32_t val )
{

	switch( id )
	{
	case	DMD_E_INFO_ISDBS_BERRNUM_1	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_BITNUM_1	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_BERRNUM_2	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_BITNUM_2	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_BERRNUM_3	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_BITNUM_3	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_ERRORFREE_0	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
	case	DMD_E_INFO_ISDBS_ERRORFREE_1	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
	case	DMD_E_INFO_ISDBS_ERRORFREE_2	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
	case	DMD_E_INFO_ISDBS_ERRORFREE_3	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
	case	DMD_E_INFO_ISDBS_ERRORFREE_T	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ERRORFREE	,val);
	case	DMD_E_INFO_ISDBS_TSNO	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_TSID	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_MOD	:	return	MN8847X_DMD_value_hex(val);
	case	DMD_E_INFO_ISDBS_EMGSW	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_YESNO	,val);
	case	DMD_E_INFO_ISDBS_UPLINK	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_EXON	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_YESNO	,val);
	case	DMD_E_INFO_ISDBS_CHANGE	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_MOD0	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBS_MOD	,val);
	case	DMD_E_INFO_ISDBS_SLOT0	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_MOD1	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBS_MOD	,val);
	case	DMD_E_INFO_ISDBS_SLOT1	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_MOD2	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBS_MOD	,val);
	case	DMD_E_INFO_ISDBS_SLOT2	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_MOD3	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_ISDBS_MOD	,val);
	case	DMD_E_INFO_ISDBS_SLOT3	:	return	MN8847X_DMD_value_decimal(val);		
	case	DMD_E_INFO_ISDBS_TSID0	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID1	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID2	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID3	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID4	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID5	:	return	MN8847X_DMD_value_hex(val);		
	case	DMD_E_INFO_ISDBS_TSID6	:	return	MN8847X_DMD_value_hex(val);
	case	DMD_E_INFO_ISDBS_TSID7	:	return	MN8847X_DMD_value_hex(val);
	default:
			if( id <= DMD_E_INFO_ISDBS_TSID7 )
				return MN8847X_DMD_value_hex(val);
			else if( id <= DMD_E_INFO_ISDBS_TSNO48 )
				return	MN8847X_DMD_value_decimal(val);
	
	}
	return "Not Defined";
}
#endif

DMD_text_t MN8847X_DMD_info_value_dvbc( DMD_u32_t id , DMD_u32_t val )
{

	switch( id )
	{
	case	DMD_E_INFO_STATUS	:	return	MN8847X_DMD_value_textlist( &MN8847X_DMD_TEXTLIST_DVBC_STATUS , val );		
	}
	return "Not Defined";
}
DMD_text_t MN8847X_DMD_info_value_vq( DMD_u32_t id , DMD_u32_t val )
{

	switch( id )
	{
	case	DMD_E_INFO_STATUS	:	return	MN8847X_DMD_value_decimal( val );		
	}
	return "Not Defined";
}


DMD_text_t MN8847X_DMD_info_value_dvbt( DMD_u32_t id , DMD_u32_t val )
{
	switch( id )
	{
		case	DMD_E_INFO_DVBT_STATUS		:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_STATUS	,val);		
		case	DMD_E_INFO_DVBT_SQI		:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT_HIERARCHY_SELECT	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_HIERARCHY_SEL	,val);
		case	DMD_E_INFO_DVBT_TPS_ALL	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_TPS_ALL	,val);
		case	DMD_E_INFO_DVBT_MODE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_MODE	,val);
		case	DMD_E_INFO_DVBT_GI	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_GI	,val);
		case	DMD_E_INFO_DVBT_LENGTH_INDICATOR	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT_CONSTELLATION	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_CONST	,val);
		case	DMD_E_INFO_DVBT_HIERARCHY	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_HIERARCHY	,val);
		case	DMD_E_INFO_DVBT_HP_CODERATE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_CR	,val);
		case	DMD_E_INFO_DVBT_LP_CODERATE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT_CR	,val);
		case	DMD_E_INFO_DVBT_CELLID	:	return	MN8847X_DMD_value_hex(val);		
	}

	return "Not Defined";
}

DMD_text_t MN8847X_DMD_info_value_dvbt2( DMD_u32_t id , DMD_u32_t val )
{
	switch( id )
	{
		case	DMD_E_INFO_DVBT2_STATUS		:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_STATUS	,val);		
		case	DMD_E_INFO_DVBT2_SQI		:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_MODE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_MODE	,val);
		case	DMD_E_INFO_DVBT2_GI	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_GI	,val);
		case	DMD_E_INFO_DVBT2_BERRNUM_C	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_BITNUM_C	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_SELECTED_PLP	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_L1_ALL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_TYPE	,val);
		case	DMD_E_INFO_DVBT2_BW_EXT	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_S1	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_S2	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_PAPR	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PAPR	,val);
		case	DMD_E_INFO_DVBT2_L1_MOD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_L1_MOD	,val);
		case	DMD_E_INFO_DVBT2_L1_COD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_COD	,val);
		case	DMD_E_INFO_DVBT2_L1_FEC_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_FEC_TYPE	,val);
		case	DMD_E_INFO_DVBT2_L1_POST_SIZE	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_L1_POST_INFO_SIZE	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_PILOT_PATTERN	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PILOT_PATTERN	,val);
		case	DMD_E_INFO_DVBT2_TX_ID_AVAILABILITY	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_CELL_ID	:	return	MN8847X_DMD_value_hex(val);		
		case	DMD_E_INFO_DVBT2_NETWORK_ID	:	return	MN8847X_DMD_value_hex(val);		
		case	DMD_E_INFO_DVBT2_T2_SYSTEM_ID	:	return	MN8847X_DMD_value_hex(val);		
		case	DMD_E_INFO_DVBT2_NUM_T2_FRAMES	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_NUM_DATA_SYMBOLS	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_REGEN_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_L1_POST_EXTENSION	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_NUM_RF	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_CURRENT_RF_IDX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_SUB_SLICES_PER_FRAME	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_SUB_SLICE_INTERVAL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_NUM_PLP	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_NUM_AUX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_PLP_MODE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PLP_MODE	,val);
		case	DMD_E_INFO_DVBT2_FEF_TYPE	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_FEF_LENGTH	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_FEF_INTERVAL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_ID	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PLP_TYPE	,val);
		case	DMD_E_INFO_DVBT2_DAT_PLP_PAYLOAD_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PAYLOAD_TYPE	,val);
		case	DMD_E_INFO_DVBT2_DAT_PLP_GROUP_ID	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_COD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_COD	,val);
		case	DMD_E_INFO_DVBT2_DAT_PLP_MOD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PLP_MOD	,val);
		case	DMD_E_INFO_DVBT2_DAT_PLP_ROTATION	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_FEC_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_FEC_TYPE	,val);
		case	DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS_MAX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_FRAME_INTEVAL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_LENGTH	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_TIME_IL_TYPE	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_FF_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_ID	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PLP_TYPE	,val);
		case	DMD_E_INFO_DVBT2_COM_PLP_PAYLOAD_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PAYLOAD_TYPE	,val);
		case	DMD_E_INFO_DVBT2_COM_PLP_GROUP_ID	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_COD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_COD	,val);
		case	DMD_E_INFO_DVBT2_COM_PLP_MOD	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_PLP_MOD	,val);
		case	DMD_E_INFO_DVBT2_COM_PLP_ROTATION	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_FEC_TYPE	:	return	MN8847X_DMD_value_textlist(	&MN8847X_DMD_TEXTLIST_DVBT2_FEC_TYPE	,val);
		case	DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS_MAX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_FRAME_INTEVAL	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_LENGTH	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_TIME_IL_TYPE	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_FF_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_FRAME_IDX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_TYPE_2_START	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_L1_CHANGE_COUNTER	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_START_RF_IDX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_FIRST_RF_IDX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_START	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_DAT_PLP_NUM_BLOCKS	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_FIRST_RF_IDX	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_START	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_COM_PLP_NUM_BLOCKS	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_STATIC_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_STATIC_PADDING_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_IN_BAND_A_FLAG	:	return	MN8847X_DMD_value_decimal(val);		
		case	DMD_E_INFO_DVBT2_IN_BAND_B_FLAG	:	return	MN8847X_DMD_value_decimal(val);	
#if (DEMOD_TYPE == DMD_TYPE_MN88473)
		case   DMD_E_INFO_DVBT2_L1_POST_SCRAMBLE_ENABLED_FLG :	return	MN8847X_DMD_value_decimal(val);	//20131219, add for indicator of L1_POST_SCRAMBLE
#endif
		default:
			return "Not Defined";

	}

}

/* **************************************************** */
/* Text List */
/* **************************************************** */
DMD_textlist_t	MN8847X_DMD_TEXTLIST_SYSTEM			=	
{	22	,
{		"ISDBT",	
		"ISDBS",	
		"DVBT",	
		"DVBT2",	
		"DVBC",	
		"DVBC2",	
		"ATSC",
		"QAMB_64QAM",	
		"QAMB_256QAM",	
		"QAMC_64QAM",	
		"QAMC_256QAM",
		"ISDBT_Brazil",	
		"---",	
		"NTSC_M_BTSC",	
		"PAL_M_BTSC",	
		"PAL_N_BTSC",	
		"PAL_B_G_NICAM",
		"PAL_I_NiCAM",	
		"PAL_D_NiCAM",	
		"PAL_B_G_A2",	
		"SECAM_L_NiCAM",	
		"SECAM_L1_NiCAM",	
		"SECAM_D_K_A2",
		"SECAM_BG_NICAM"},
};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_LOCK			=	{	4	,{	"LOCKED",	"NOSIGNAL",	"NOSYNC",	"ERROR(TimeOut)"}};								
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_STATUS	=	{	12	,{	"0_RESET",	"1_TUNING",	"2_AGC",	"3_Mode_Search",	"4_Clock_Carrier_Sync",	"5_",	"6_",	"7_",	"8_Freq_Sync",	"9_Frame_Detect",	"10_Sync",	"11_Sync_Protect"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ERRORFREE		=	{	3	,{	"ERROR",	"ERROR FREE" , "Not Available"}};										
DMD_textlist_t	MN8847X_DMD_TEXTLIST_HIERARCHY_SEL	=	{	2	,{	"LP(No Hier)",	"HP"}};										
DMD_textlist_t	MN8847X_DMD_TEXTLIST_TPS_ALL		=	{	2	,{	"Fail",	"Success"}};										
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_MODE		=	{	3	,{	"2k",	"8k",	"4k"}};									
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_GI		=	{	4	,{	"1/32",	"1/16",	"1/8",	"1/4"}};								
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_CONST		=	{	3	,{	"QPSK",	"16QAM",	"64QAM"}};									
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_HIERARCHY	=	{	4	,{	"No Hier",	"alpha=1",	"alpha=2",	"alpha=4"}};								
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT_CR		=	{	5	,{	"1/2",	"2/3",	"3/4",	"5/6",	"7/8"}};							
DMD_textlist_t	MN8847X_DMD_TEXTLIST_BW				=	{	6	,{	"NOT_SPECIFIED",	"6MHZ",	"7MHZ",	"8MHZ",	"5MHZ",	"1_7MHZ"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_YESNO			=	{	2	,{	"No","Yes"}};

DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_STATUS	=	{	14	,{	"0_RESET",	"1_TUNING",	"2_AGC",	"3_P1_SEARCH",	"4_GI_SEARCH",	"5_Clock_Carrier_Sync",	"6_",	"7_",	"8_",	"9_",	"10_Freq_Sync",	"11_L1_PRE_Lock",	"12_L1_POST_Lock",	"13_Sync"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_MODE		=	{	6	,{	"1k",	"2k",	"4k",	"8k",	"16k",	"32k"}};		
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_GI		=	{	7	,{	"1/32",	"1/16",	"1/8",	"1/4",	"1/128",	"19/128",	"19/256"}};	
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_TYPE		=	{	3	,{	"TS Only",	"GS Only",	"TS & GS"}};					
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PAPR		=	{	4	,{	"No PAPR",	"ACE-PAPR",	"TR-PAPR",	"ACE&PAPR"}};				
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_L1_MOD		=	{	4	,{	"BPSK",	"QPSK",	"16QAM",	"64QAM"}};				
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_COD		=	{	8	,{	"1/2",	"3/5",	"2/3",	"3/4",	"4/5(reserved)",	"5/6(reserved)",	"1/3",	"2/5"}};//add T2-lite CR in the "()", 20131219	
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_FEC_TYPE		=	{	2	,{	"16k LDPC",	"64k LDPC"}};						
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PILOT_PATTERN		=	{	8	,{	"PP1",	"PP2",	"PP3",	"PP4",	"PP5",	"PP6",	"PP7",	"PP8"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_MODE		=	{	3	,{	"Not Specified",	"Normal Mode",	"HE Mode"}};	
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_TYPE		=	{	3	,{	"Common",	"Data Type1",	"Data Type2"}};	
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PAYLOAD_TYPE		=	{	4	,{	"GFPS",	"GCS",	"GSE",	"TS"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBT2_PLP_MOD		=	{	4	,{	"QPSK",	"16QAM",	"64QAM",	"256QAM"}};

#ifndef DMD_DISABLE_ISDB
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_MODE		=	{	3	,{	"Mode1",	"Mode2",	"Mode3"}};					
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_GI		=	{	4	,{	"1/32",	"1/16",	"1/8",	"1/4"}};				
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_SYSTEM		=	{	2	,{	"ISDBT",	"ISDBT(SB)"}};						
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_MAP		=	{	8	,{	"DQPSK",	"QPSK",	"16QAM",	"64QAM",	"-",	"-",	"-",	"-"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_CR		=	{	8	,{	"1/2",	"2/3",	"3/4",	"5/6",	"7/8",	"-",	"-",	"-"}};
DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBT_INT		=	{	8	,{	"I=1,2,4",	"I=2,4,8",	"I=4,8,16",	"I=8,16,32",	"-",	"-",	"-",	"-"}};


DMD_textlist_t	MN8847X_DMD_TEXTLIST_ISDBS_MOD	=	{	8	,{	"Not Defined",	"BPSK 1/2",	"QPSK 1/2",	"QPSK 2/3",	"QPSK 3/4",	"QPSK 5/6",	"QPSK 7/8",	"TC8PSK 2/3"}};
#endif

DMD_textlist_t	MN8847X_DMD_TEXTLIST_DVBC_STATUS	=	{	11	,{	"0_RESET",	"1_TUNING",	"2_AGC",	"3_Rate Search",	"4_",	"5_",	"6_Mode Search",	"7_",	"8_Frame_Sync",	"9_Sync",	"10_Sync"}};

/* **************************************************** */
/* Information Strings */
/* **************************************************** */
DMD_text_t	MN8847X_DMD_INFO_TITLE_COMMON[] = 
{
"ALL",
"Reg. Ver.",
"PSEQ Ver.",
"SYSTEM",
"LOCK",
"AGC",
"Bit Error",
"Total Bit",
"CNR(int)",
"CNR(dec)",
"Packet Error",
"Total Packet",
"Status",
"Error Free",
};
DMD_text_t	MN8847X_DMD_INFO_TITLE_DVBT[] = 
{
"SQI",
"Selected Hier.",
"TPS Obtain",
"Mode",
"GI",
"LENGTH_INDICATOR",
"CONSTELLATION",
"HIERARCHY",
"HP_CODERATE",
"LP_CODERATE",
"CELLID"
};
DMD_text_t	MN8847X_DMD_INFO_TITLE_DVBC[] = 
{
"Not Defined"
};

#ifndef DMD_DISABLE_ISDB
DMD_text_t	MN8847X_DMD_INFO_TITLE_ISDBS[] = 
{
"BERRNUM_1",
"BITNUM_1",
"BERRNUM_2",
"BITNUM_2",
"BERRNUM_3",
"BITNUM_3",
"ERRORFREE_0",
"ERRORFREE_1",
"ERRORFREE_2",
"ERRORFREE_3",
"ERRORFREE_T",
"TSNO",
"TSID",
"Modulation(selected)",
"Emergency Switch",
"UPLINK",
"EXON",
"CHANGE",
"Modulation(Layer0)",
"Slot Num. (Layer0)",
"Modulation(Layer1)",
"Slot Num. (Layer1)",
"Modulation(Layer2)",
"Slot Num. (Layer2)",
"Modulation(Layer3)",
"Slot Num. (Layer3)",
"TSID0",
"TSID1",
"TSID2",
"TSID3",
"TSID4",
"TSID5",
"TSID6",
"TSID7",
"TSNO01",
"TSNO02",
"TSNO03",
"TSNO04",
"TSNO05",
"TSNO06",
"TSNO07",
"TSNO08",
"TSNO09",
"TSNO10",
"TSNO11",
"TSNO12",
"TSNO13",
"TSNO14",
"TSNO15",
"TSNO16",
"TSNO17",
"TSNO18",
"TSNO19",
"TSNO20",
"TSNO21",
"TSNO22",
"TSNO23",
"TSNO24",
"TSNO25",
"TSNO26",
"TSNO27",
"TSNO28",
"TSNO29",
"TSNO30",
"TSNO31",
"TSNO32",
"TSNO33",
"TSNO34",
"TSNO35",
"TSNO36",
"TSNO37",
"TSNO38",
"TSNO39",
"TSNO40",
"TSNO41",
"TSNO42",
"TSNO43",
"TSNO44",
"TSNO45",
"TSNO46",
"TSNO47",
"TSNO48"};

DMD_text_t	MN8847X_DMD_INFO_TITLE_ISDBT[] = 
{
"BERRNUM_A",
"BITNUM_A",
"BERRNUM_B",
"BITNUM_B",
"BERRNUM_C",
"BITNUM_C",
"ERRORFREE_A",
"ERRORFREE_B",
"ERRORFREE_C",
"MODE",
"GI",
"SYSTEM",
"COUNTDOWN",
"EMGFLG",
"Part Reception",
"Modulation(A)",
"CodeRate(A)",
"InterleaveLen.(A)",
"Segment Num.(A)",
"Modulation(B)",
"CodeRate(B)",
"InterleaveLen.(B)",
"Segment Num.(B)",
"Modulation(C)",
"CodeRate(C)",
"InterleaveLen.(C)",
"Segment Num.(C)",
"PhaseShift",
};
#endif
DMD_text_t	MN8847X_DMD_INFO_TITLE_DVBT2[] = 
{
"SQI",
"MODE",
"GI",
"BERRNUM_C",
"BITNUM_C",
"SELECTED_PLP",
"L1_ALL",
"TYPE",
"BW_EXT",
"S1",
"S2",
"PAPR",
"L1_MOD",
"L1_COD",
"L1_FEC_TYPE",
"L1_POST_SIZE",
"L1_POST_INFO_SIZE",
"PILOT_PATTERN",
"TX_ID_AVAILABILITY",
"CELL_ID",
"NETWORK_ID",
"T2_SYSTEM_ID",
"NUM_T2_FRAMES",
"NUM_DATA_SYMBOLS",
"REGEN_FLAG",
"L1_POST_EXTENSION",
"NUM_RF",
"CURRENT_RF_IDX",
"SUB_SLICES_PER_FRAME",
"SUB_SLICE_INTERVAL",
"NUM_PLP",
"NUM_AUX",
"PLP_MODE",
"FEF_TYPE",
"FEF_LENGTH",
"FEF_INTERVAL",
"DAT_PLP_ID",
"DAT_PLP_TYPE",
"DAT_PLP_PAYLOAD_TYPE",
"DAT_PLP_GROUP_ID",
"DAT_PLP_COD",
"DAT_PLP_MOD",
"DAT_PLP_ROTATION",
"DAT_PLP_FEC_TYPE",
"DAT_PLP_NUM_BLOCKS_MAX",
"DAT_PLP_FRAME_INTEVAL",
"DAT_PLP_TIME_IL_LENGTH",
"DAT_PLP_TIME_IL_TYPE",
"DAT_FF_FLAG",
"COM_PLP_ID",
"COM_PLP_TYPE",
"COM_PLP_PAYLOAD_TYPE",
"COM_PLP_GROUP_ID",
"COM_PLP_COD",
"COM_PLP_MOD",
"COM_PLP_ROTATION",
"COM_PLP_FEC_TYPE",
"COM_PLP_NUM_BLOCKS_MAX",
"COM_PLP_FRAME_INTEVAL",
"COM_PLP_TIME_IL_LENGTH",
"COM_PLP_TIME_IL_TYPE",
"COM_FF_FLAG",
"FRAME_IDX",
"TYPE_2_START",
"L1_CHANGE_COUNTER",
"START_RF_IDX",
"DAT_FIRST_RF_IDX",
"DAT_PLP_START",
"DAT_PLP_NUM_BLOCKS",
"COM_FIRST_RF_IDX",
"COM_PLP_START",
"COM_PLP_NUM_BLOCKS",
"STATIC_FLAG",
"STATIC_PADDING_FLAG",
"IN_BAND_A_FLAG",
"IN_BAND_B_FLAG",
"L1_POST_SCRAMBLE_EN", //20131219, add for indicator of L1_POST_SCRAMBLE
"SIGNAL_CONTENT_BASE_LITE" // 20140410
};


