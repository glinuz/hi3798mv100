/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: xg_3bl.c
* Description:
*
* History:
* Version   Date             Author     DefectNum        Description
* main\1    2007-10-16   w54542    NULL                Create this file.
***********************************************************************************/

#include <linux/kernel.h>
#include <asm/uaccess.h>

#include "hi_debug.h"
#include "hi_type.h"
#include "drv_i2c_ext.h"
#include "drv_tuner_ext.h"
#include "drv_demod.h"
#include "xg_3bl.h"


static HI_S32 xg_3bl_write(HI_U32 u32TunerPort, HI_U8 enI2cChannel,
                           HI_U8 u8DevAddr, HI_U8 u8RegAddr, HI_U8 *pu8Buf, HI_U32 u32Num)
{
    HI_U32 i = 0;
    HI_U8 *pu8Tmp   = NULL;
    HI_U8 au8Data[255] = {0};
    TUNER_I2C_DATA_S stI2cData = {0};

	HI_INFO_TUNER("> %s: %d\n", __FUNCTION__, __LINE__);

    if (NULL == pu8Buf)
    {
        HI_ERR_TUNER( "pointer(pu8Buf) is null\n");
        return HI_FAILURE;
    }

    pu8Tmp  = pu8Buf;
    au8Data[0] = u8RegAddr;
    for (i = 0; i < u32Num; i++)
    {
        au8Data[i + 1] = *pu8Tmp;
        pu8Tmp++;
    }

    stI2cData.pu8ReceiveBuf = 0;
    stI2cData.u32ReceiveLength = 0;
    stI2cData.pu8SendBuf = au8Data;
    stI2cData.u32SendLength = u32Num + 1;

    return tuner_i2c_send_data(enI2cChannel, u8DevAddr, &stI2cData);
}

/*************************************************
  Function:            xg_3bl_get_reg_info
  Description:
  Calls:                  NA
  Called By:            xg_3bl_set_tuner
  Table Accessed:   NA
  Table Updated:
  Input:                 dbRFfreq -- Frf ,scope[50,000,000~858,000,000Hz]
                            dbPace    -- Fref,scope[62,500-142,860-166,670-50,000-125,000]
                   nSupply  -- AS voltage,scope[0,1,2,3]
  Output:               cReg       -- xg registers array,6bytes
  Return:                NA
  Others:                Tuner DCT-2A/111
    *************************************************/
static void xg_3bl_get_reg_info(HI_U32 u32TunerPort,
						 HI_U32 dbRFfreq,
                         HI_U32 dbPace,
                         HI_U32 nSupply,
                         HI_U8 *cReg)
{

    /*fstep(Hz)*/
    HI_U32 dbTunerPace[] = {50000, 31250, 0, 62500};

    HI_U32  dbOscFreq = 0,dbPll = 0; /*Fosc = Frf + Fif(36.125)*/
    HI_U32 i, nDiv = 0;
    HI_U32 nPaceIndex = 0; //tuner pace index

    if (NULL == cReg)
    {
        HI_ERR_TUNER( "pointer is null\n");
        return;
    }

    /**********************************************************************
                      d7    d6    d5    d4   d3     d2      d1      d0
    ADDRESS         |  1  |  1  |  0  |  0  |  0  |  MA1  |  MA2  |  R/W  |
    divider byte1   |  0  | n14 | n13 | n12 | n11 |  n10  |  n9   |  n8   |
    divider byte2   |  n7 |  n6 |  n5 |  n4 |  n3 |   n2  |  n1   |  n0   |
    control byte1   |  1  |  1  |  0  |  0  |  0  |   R2  |  R1   |  R0   |
    control byte2   | CP2 | CP1 | CP0 | BS5 | BS4 |  BS3  | BS2   | BS1   |
     ***********************************************************************/

    /*MA1, MA0*/
    *cReg = 0xc0; //address byte
    cReg++;

    dbOscFreq = dbRFfreq + 36000;//Fosc:kHz
    dbPll = dbOscFreq*1000;	//Hz
    
    //if ( HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType )
    if (1)
    {
	    if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
	    {
	        dbPll += dbPace*2;
	    }
	    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode)
	    {
	        dbPll += dbPace*1/2;
	    }
	    else
	    {
	        dbPll += dbPace*1/2;
	    }
    }

    nDiv = dbPll/dbPace;


    *cReg = (nDiv & 0x7f00) >> 8; //divider byte 1
    cReg++;

    *cReg = nDiv & 0x00ff; //divider byte 2
    cReg++;

    //get index of tuner pace
    for (i = 0; i < sizeof(dbTunerPace) / sizeof(HI_U32); i++)
    {
        if (dbTunerPace[i] == dbPace)
        {
            nPaceIndex = i;
            break;
        }
    }

    *cReg = 0x80 | (nPaceIndex << 1); //control byte 1: CP = 0
    cReg++;

    *cReg = 0x00;
}

/*************************************************
  Function:          xg_3bl_set_tuner
  Description:      set xg Tuner
  Calls:                xg_3bl_get_reg_info
  Called By:          CTunerDlg
  Table Accessed:  NA
  Table Updated:   NA
  Input:               dbRFfreq -- rf frequency,scope[51~858MHz]
                          dbPace    -- reference frequency,scope[62.5kHz,142.86kHz, 166.67kHz,50kHz,125kHz]
                          nSupply  --  AS reference voltage index,scope[0,1,2,3]
  Output:             NA
  Return:
  Others:             Tuner DCT-2A/111
*************************************************/
HI_S32 xg_3bl_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32RF)
{
    HI_U8  au8RegValue[5] = {0};
    HI_U32 u32DbPace  = XG_3BL_STEPS_DEF; /*Hz*/
    HI_U32 u32Supply = XG_3BL_SUPPLY_DEF;  
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 i = 0;


	qam_config_i2c_out(u32TunerPort, 1);	

    xg_3bl_get_reg_info( u32TunerPort, u32RF, u32DbPace, u32Supply, au8RegValue);

    au8RegValue[0] = g_stTunerOps[u32TunerPort].u32TunerAddress;
    
    for (i = 0; i < 5; i++)
    {
        HI_INFO_TUNER("xg_3bl_reg[%d]:0x%x\n",i,au8RegValue[i]);
    }
    
    s32Ret = xg_3bl_write(u32TunerPort, enI2cChannel, au8RegValue[0], au8RegValue[1], &au8RegValue[2], 3);
    if (HI_SUCCESS != s32Ret)
    {
        HI_ERR_TUNER( "xg_3bl_set_tuner error\n");
        return s32Ret;
    }

    return HI_SUCCESS;
}


HI_VOID xg_recalculate_signal_strength( HI_U32 * pu32Strength )
{
    
}

