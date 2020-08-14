
#include <hi_debug.h>
#include "hi_type.h"
#include "drv_i2c_ext.h"

#include "drv_demod.h"


#define CD1616_TUNER_PARA_NUM 0x04
#define CD1616_RFAGC_TOP_DEF 0x02

#define PLL_STEP 62500
#define CD1616_IF_DVB_DEF 36150000


/* cd1616 i2c write function */
static HI_S32 cd1616_write(HI_U32 u32TunerPort,HI_U8 enI2cChannel, HI_U8* pu8Data, HI_U32 u32Num)
{
    HI_U8 i;
    HI_S32 s32Ret;
    TUNER_I2C_DATA_S stI2cDataStr;
    HI_U8 au8SendData[6];
    HI_U8 u8NBytes = u32Num;

    for (i = 0; i < u8NBytes; i++)
    {
        au8SendData[i] = *pu8Data;
        pu8Data++;
    }

    stI2cDataStr.pu8ReceiveBuf = 0;
    stI2cDataStr.u32ReceiveLength = 0;
    stI2cDataStr.pu8SendBuf = au8SendData;
    stI2cDataStr.u32SendLength = 4;
    s32Ret = tuner_i2c_send_data(enI2cChannel, g_stTunerOps[u32TunerPort].u32TunerAddress, &stI2cDataStr);

    return s32Ret;
}

HI_S32 cd1616_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    HI_U32 u32FreqPll; 
	HI_U32 u32RfProg;
    HI_U8  au8TunerPara[6];

    /* calculate N0-N14*/
    u32PuRF *= 1000;
    u32PuRF += CD1616_IF_DVB_DEF;
    u32FreqPll  = u32PuRF;

    if ( HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType )
    {
	    if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)	
	    {       
	        u32FreqPll += PLL_STEP*2; 
	    }   
	    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode) 
	    {       
	        u32FreqPll += PLL_STEP*1/2;   
	    }   
	    else    
	    {       
	        u32FreqPll += PLL_STEP*1/2;   
	    }   
    }
    
    u32FreqPll /= PLL_STEP;   /* real frequency programmed*/  
    u32RfProg  = u32FreqPll * PLL_STEP;


    /* DB1 DB2 */
    au8TunerPara[0] = (HI_U8)(u32FreqPll >> 8);
    au8TunerPara[1] = (HI_U8)(u32FreqPll);

    /* CB1 */
    au8TunerPara[2] = 0xc6;

    /* CB2 */
    if (u32RfProg <= (157000000 + CD1616_IF_DVB_DEF)) /* low band*/
    {
        au8TunerPara[3] = 0x01;
    }
    else if (u32RfProg <= (442000000 + CD1616_IF_DVB_DEF)) /* mid band*/
    {
        au8TunerPara[3] = 0x02;
    }
    else  /* high band */
    {
        au8TunerPara[3] = 0x04;
    }

    au8TunerPara[3] |= (CD1616_RFAGC_TOP_DEF << 3);

	qam_config_i2c_out( u32TunerPort, 1 );	
	
    cd1616_write(u32TunerPort, enI2cChannel, au8TunerPara, CD1616_TUNER_PARA_NUM);

    return HI_SUCCESS;
}


