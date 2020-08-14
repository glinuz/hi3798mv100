
//#include <hi_debug.h>
#include "hi_type.h"
//#include "hi_i2c.h"
#include "hi_unf_frontend.h"
#include "hi_drv_tuner.h"
#include "tdcc.h"
#include "drv_demod.h"
#include <common.h>

#define TUNER_PARA_NUM 0x06
#define CD1616_RFAGC_TOP_DEF    0x02

#define BANDWIDTH_DEF    8000000
#define ATC_DEF          0
#define PLL_STEP         62500
#define IF_DVB_DEF       36150000

HI_S32 tdcc_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    HI_U32 u32FreqPll;
	HI_U32 u32RfProg;
    HI_U8  au8TunerPara[6];
    HI_S32 s32Ret;
    TUNER_I2C_DATA_S  stI2cDataStr; 

    /* calculate N0-N14*/
    /*uFreqPll = puRF*1000 + 36125000;*/
    u32FreqPll = u32PuRF * 1000 + 36125000 + 200000; /*hyp*/

    if ( HI_UNF_DEMOD_DEV_TYPE_3130E == g_stTunerOps[u32TunerPort].enDemodDevType )
    {
	    if(4 == g_stTunerOps[u32TunerPort].u32CurrQamMode)  
	    {       
	        u32FreqPll += PLL_STEP * 2; 
	    }   
	    else if(2 == g_stTunerOps[u32TunerPort].u32CurrQamMode) 
	    {       
	        u32FreqPll += PLL_STEP * 1 / 2;   
	    }   
	    else    
	    {       
	        u32FreqPll += PLL_STEP * 1 / 2;   
	    }   
    }

    u32FreqPll /= PLL_STEP;   /* real frequency programmed*/  
    u32RfProg  = u32FreqPll * PLL_STEP;

    //au8TunerPara[0] = g_stTunerOps[u32TunerPort].u32TunerAddress;
    au8TunerPara[0] = 0xc2;

    /* DB1 DB2 */
    au8TunerPara[1] = (HI_U8)(u32FreqPll >> 8);
    au8TunerPara[2] = (HI_U8)(u32FreqPll);    

    /* CB1 */
    au8TunerPara[3] = 0x93;

    /* BB */
    au8TunerPara[4] = 0x00;
    if(u32PuRF <= 148000)
    {
        au8TunerPara[4] += 0x1;
    }
    else if(u32PuRF <= 430000)
    {
        au8TunerPara[4] += 0x2;
    }
    else
    {
        au8TunerPara[4] += 0x8;
    }
        
    /* CB2 */
    au8TunerPara[5] = 0xc2;

	qam_config_i2c_out( u32TunerPort, 1 );	
	
    stI2cDataStr.pu8ReceiveBuf = 0;
    stI2cDataStr.u32ReceiveLength = 0;
    stI2cDataStr.pu8SendBuf = &au8TunerPara[1];
    stI2cDataStr.u32SendLength = 5; 
    s32Ret =  tuner_i2c_send_data(enI2cChannel, au8TunerPara[0], &stI2cDataStr);
    if(HI_SUCCESS != s32Ret)
    {
        TUNER_DEBUG( "%s error\n",__FUNCTION__ );
        return s32Ret;
    }
            
    return HI_SUCCESS; 
}


