#include "hi_type.h"
#include "hi_unf_i2c.h"
#include "drv_tuner_ioctl.h"

#define DMD_BANK_1st 0x1c

HI_S32 tda6651_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32PuRF)
{
    HI_U8  szBuffer[7] = {0}; 
    HI_U8 tBuf[10] = {0};
    HI_U32 dwTunerFreq = 0;
    HI_U32 u32TunerI2cAddr = 0;
    HI_U32 u32DemodI2cAddr = 0;
    TUNER_I2C_DATA_S stI2cDataStr = {0};
    HI_U32 FcompkHzx100 = 16667;
    HI_UNF_DEMOD_DEV_TYPE_E	  enDemodDevType = HI_UNF_DEMOD_DEV_TYPE_BUTT;

    enDemodDevType = g_stTunerOps[u32TunerPort].enDemodDevType;
    u32TunerI2cAddr = g_stTunerOps[u32TunerPort].u32TunerAddress;

    dwTunerFreq = u32PuRF;
    szBuffer[2] = 0xca;
    if(dwTunerFreq > 444000 )
    {
    	szBuffer[3] = 0x4c;
    }
    else if (dwTunerFreq > 159000 )
    {        
    	szBuffer[3] = 0x4a;
    }
    else
    {
	szBuffer[3] = 0x49;
    }

    /**The problem of adjacent interference of coolech*//**CNcomment酷乐橙邻频干扰指标问题*/
    szBuffer[3] &= 0x07;
    switch( g_stTunerOps[u32TunerPort].enSigType )
    {
        case HI_UNF_TUNER_SIG_TYPE_DVB_T2:
            switch( g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth )
            {
                case 8000:
                    szBuffer[3] |= 0x18;
                    break;
                case 6000:
                case 5000:
                    szBuffer[3] |= 0x10;
                    break;
                case 7000:
                    szBuffer[3] |= 0x0;
                    break;
                default:
                    return HI_FAILURE;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_DVB_T:
            switch( g_stTunerOps[u32TunerPort].stCurrPara.unSRBW.u32BandWidth )
            {
                case 8000:
                    szBuffer[3] |= 0x18;
                    break;
                case 7000:
                    szBuffer[3] |= 0x0;
                    break;
                case 6000:
                    szBuffer[3] |= 0x10;
                    break;
                default:
                    return HI_FAILURE;
            }
            break;
        case HI_UNF_TUNER_SIG_TYPE_CAB:
            szBuffer[3] |= 0x18;
            break;
        default:
            return HI_FAILURE;
    }
    
    dwTunerFreq += 36167;
	
    szBuffer[3] &= 0x1f;
    if(((dwTunerFreq>=80000)&&(dwTunerFreq<92000))||((dwTunerFreq>=196000)&&(dwTunerFreq<224000)))
    	szBuffer[3] |= 1<<5;
    else if(((dwTunerFreq>=92000)&&(dwTunerFreq<144000))||((dwTunerFreq>=224000)&&(dwTunerFreq<296000)))
    	szBuffer[3] |= 2<<5;
    else if(((dwTunerFreq>=144000)&&(dwTunerFreq<156000))||((dwTunerFreq>=296000)&&(dwTunerFreq<380000))||((dwTunerFreq>=484000)&&(dwTunerFreq<604000)))
    	szBuffer[3] |= 3<<5;
    else if(((dwTunerFreq>=156000)&&(dwTunerFreq<176000))||((dwTunerFreq>=380000)&&(dwTunerFreq<404000))||((dwTunerFreq>=604000)&&(dwTunerFreq<676000)))
    	szBuffer[3] |= 4<<5;
    else if(((dwTunerFreq>=176000)&&(dwTunerFreq<184000))||((dwTunerFreq>=404000)&&(dwTunerFreq<448000))||((dwTunerFreq>=676000)&&(dwTunerFreq<752000)))
    	szBuffer[3] |= 5<<5;
    else if(((dwTunerFreq>=184000)&&(dwTunerFreq<196000))||((dwTunerFreq>=448000)&&(dwTunerFreq<472000))||((dwTunerFreq>=752000)&&(dwTunerFreq<774000)))
    	szBuffer[3] |= 6<<5;
    else //if(((dwTunerFreq>=472000)&&(dwTunerFreq<484000))||((dwTunerFreq>=868000)&&(dwTunerFreq<904000)))
    	szBuffer[3] |= 7<<5;

    /* convert frequency to divider ratio*/
    dwTunerFreq = (dwTunerFreq * 1000) /FcompkHzx100;
    dwTunerFreq +=5; /*round off the result*/
    dwTunerFreq /=10;    

    szBuffer[0] = (unsigned char) ((dwTunerFreq >> 8) & 0xff);
    szBuffer[1] = (unsigned char) (dwTunerFreq  & 0xff);

    szBuffer[4] = 0x80;
    szBuffer[5] = szBuffer[3];
    switch(enDemodDevType)
    {
        case HI_UNF_DEMOD_DEV_TYPE_MN88472:
            u32DemodI2cAddr = (DMD_BANK_1st + g_stTunerOps[u32TunerPort].u32DemodAddress) << 1;
            tBuf[1] = 0xf7;
            tBuf[2] = u32TunerI2cAddr;
            tBuf[3] = szBuffer[0];
            tBuf[4] = szBuffer[1];
            tBuf[5] = szBuffer[2];
            tBuf[6] = szBuffer[3];
            tBuf[7] = szBuffer[4];
            tBuf[8] = szBuffer[5];

            stI2cDataStr.pu8ReceiveBuf = 0;
            stI2cDataStr.u32ReceiveLength = 0;
            stI2cDataStr.pu8SendBuf = tBuf + 1;
            stI2cDataStr.u32SendLength = 8;
            if(!tuner_i2c_send_data(enI2cChannel, u32DemodI2cAddr, &stI2cDataStr))
            {
                HI_INFO_TUNER("tda6651_set_tuner OK!\r\n");
                return HI_SUCCESS;
            }
            else
            {
                HI_INFO_TUNER("tda6651_set_tuner failed!\r\n");
                return HI_FAILURE;
            }
            
        default:
            return HI_FAILURE;
    }
    
}

