#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>

#include <assert.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include "hi_unf_common.h"
#include "hi_unf_mce.h"
#include "hi_unf_pdm.h"


HI_VOID MCE_PrintBaseParam(HI_UNF_PDM_DISP_PARAM_S stParam)
{
    printf("================= BASE PARAM START ====================\n");
    printf("enFormat: %d\n", stParam.enFormat);
    printf("u32Brightness: %d\n", stParam.u32Brightness);
    printf("u32Contrast: %d\n", stParam.u32Contrast);
    printf("u32Saturation: %d\n", stParam.u32Saturation);
    printf("bGammaEnable: %d\n", stParam.bGammaEnable);
    printf("enPixelFormat: %d\n", stParam.enPixelFormat);
    printf("================== BASE PARAM END =====================\n");
    
    return;
}

HI_VOID MCE_PrintLogoParam(HI_UNF_MCE_LOGO_PARAM_S stParam)
{
    printf("================= LOGO PARAM START ====================\n");
    printf("u32CheckFlag: %d\n", stParam.bLogoEnable);
    printf("u32LogoLen: %d\n", stParam.u32LogoLen);
    printf("================== LOGO PARAM END =====================\n");
    
    return;
}


HI_S32 MCE_UpdateBase(HI_VOID)
{
    HI_S32                          Ret;
    HI_UNF_PDM_DISP_PARAM_S         stDispParam;

    Ret = HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_GetBaseParam, Ret = %#x\n", Ret);
        return Ret;
    }

    MCE_PrintBaseParam(stDispParam);

    stDispParam.enFormat = HI_UNF_ENC_FMT_720P_50;

    Ret = HI_UNF_PDM_UpdateBaseParam(HI_UNF_PDM_BASEPARAM_DISP1, &stDispParam);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_UpdateBaseParam, Ret = %#x\n", Ret);
        return Ret;
    }

    return Ret;
}


HI_S32 MCE_UpdateLogo(HI_CHAR *pFilePath)
{
    HI_S32                          Ret;
    HI_UNF_MCE_LOGO_PARAM_S         stLogoParam;
    FILE                            *fpSrc = HI_NULL;
    HI_U8                           *pTmpBuf = HI_NULL;
    HI_U32                          ReadSize; 
    HI_U32                          MaxSize = 2*1024*1024;

    Ret = HI_UNF_PDM_GetLogoParam(&stLogoParam);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_UpdateBaseParam, Ret = %#x\n", Ret);
        return Ret;
    }

    MCE_PrintLogoParam(stLogoParam);

    fpSrc = fopen(pFilePath, "r");
    if(HI_NULL == fpSrc)
    {
        printf("ERR: fopen\n");
        return Ret;        
    }

    pTmpBuf = malloc(MaxSize);
    if(HI_NULL == pTmpBuf)
    {
        printf("ERR: malloc !\n");
        return HI_FAILURE;        
    }

    memset(pTmpBuf, 0x00, MaxSize);

    ReadSize = fread(pTmpBuf, sizeof(HI_CHAR), MaxSize, fpSrc);

    Ret = HI_UNF_PDM_UpdateLogoContent(pTmpBuf, ReadSize);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_UpdateLogoContent, Ret = %#x\n", Ret);
        free(pTmpBuf);
        return Ret;
    }

    stLogoParam.bLogoEnable = HI_TRUE;
    stLogoParam.u32LogoLen = ReadSize;
    
    Ret = HI_UNF_PDM_UpdateLogoParam(&stLogoParam);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_UpdateLogoParam, Ret = %#x\n", Ret);
        free(pTmpBuf);
        return Ret;
    }
        
    free(pTmpBuf);
	pTmpBuf = HI_NULL;

    return Ret;
}

HI_S32 MCE_UpdateDVB(HI_CHAR *pFilePath)
{
    HI_S32                          Ret;
    HI_UNF_MCE_PLAY_PARAM_S         stPlayParam;

    Ret = HI_UNF_PDM_GetPlayParam(&stPlayParam);
    if (HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_GetPlayParam, Ret = %#x\n", Ret);
        return HI_FAILURE;
    }

    stPlayParam.bPlayEnable = HI_TRUE;
    stPlayParam.enPlayType = HI_UNF_MCE_TYPE_PLAY_DVB;
#if (HI_TUNER_SIGNAL_TYPE == 2)
    stPlayParam.unParam.stDvbParam.enVideoType = HI_UNF_MCE_VDEC_TYPE_MPEG2;
    stPlayParam.unParam.stDvbParam.u32VideoPid = 0x101;
    stPlayParam.unParam.stDvbParam.enAudioType = HI_UNF_MCE_ADEC_TYPE_MP3;
    stPlayParam.unParam.stDvbParam.u32AudioPid = 0x102;
    stPlayParam.unParam.stDvbParam.stConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_SAT;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32Freq = 3750 * 1000;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stSat.u32SymbolRate = 10490 * 1000;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stSat.enPolar = HI_UNF_TUNER_FE_POLARIZATION_H;
    stPlayParam.unParam.stDvbParam.stLnbCfg.enLNBType = HI_UNF_TUNER_FE_LNB_DUAL_FREQUENCY;
    stPlayParam.unParam.stDvbParam.stLnbCfg.enLNBBand = HI_UNF_TUNER_FE_LNB_BAND_C;
    stPlayParam.unParam.stDvbParam.stLnbCfg.u32HighLO = 5750;
    stPlayParam.unParam.stDvbParam.stLnbCfg.u32LowLO = 5150;
    stPlayParam.unParam.stDvbParam.enLNBPower = HI_UNF_TUNER_FE_LNB_POWER_ON;
    stPlayParam.unParam.stDvbParam.enSwitch22K = HI_UNF_TUNER_SWITCH_22K_NONE;
    stPlayParam.unParam.stDvbParam.st4Port.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stPlayParam.unParam.stDvbParam.st4Port.enLNB22K = HI_UNF_TUNER_FE_LNB_22K_ON;
    stPlayParam.unParam.stDvbParam.st4Port.enPolar = HI_UNF_TUNER_FE_POLARIZATION_H;
    stPlayParam.unParam.stDvbParam.st4Port.enPort = HI_UNF_TUNER_DISEQC_SWITCH_NONE;
    stPlayParam.unParam.stDvbParam.st16Port.enLevel = HI_UNF_TUNER_DISEQC_LEVEL_1_X;
    stPlayParam.unParam.stDvbParam.st16Port.enPort = HI_UNF_TUNER_DISEQC_SWITCH_NONE;
#else
	stPlayParam.unParam.stDvbParam.enVideoType = HI_UNF_MCE_VDEC_TYPE_MPEG2;
    stPlayParam.unParam.stDvbParam.u32VideoPid = 512;
    stPlayParam.unParam.stDvbParam.enAudioType = HI_UNF_MCE_ADEC_TYPE_MP3;
    stPlayParam.unParam.stDvbParam.u32AudioPid = 650;
    stPlayParam.unParam.stDvbParam.stConnectPara.enSigType = HI_UNF_TUNER_SIG_TYPE_CAB;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32Freq = 618 * 1000;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stCab.u32SymbolRate = 6875000;
    stPlayParam.unParam.stDvbParam.stConnectPara.unConnectPara.stCab.enModType = HI_UNF_MOD_TYPE_QAM_64;
#endif
    Ret = HI_UNF_PDM_UpdatePlayParam(&stPlayParam);
    if (HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_PDM_UpdatePlayParam, Ret = %#x\n", Ret);
        return HI_FAILURE;
    }
    
    return HI_SUCCESS;
}

static HI_VOID PrintHelp(HI_VOID)
{
    printf("Usage: sample_mce_update image_type file\n");
    printf("       image_type: 1:baseparam, 2:logo, 3:dvb \n");
    printf("       example: sample_mce_update 1 null\n");
    printf("       example: sample_mce_update 2 /mnt/test.jpg\n");
    printf("       example: sample_mce_update 3 null\n");
    return;
}

HI_S32 main(HI_S32 argc,HI_CHAR *argv[])
{
    HI_S32                          Ret;
    HI_CHAR                         *pSrcFile = HI_NULL;
    HI_U32                          UpdateType = 0;

    if(3 != argc)
    {
        PrintHelp();
        return 0;
    }    

    UpdateType = atoi(argv[1]);

    if(UpdateType > 3)
    {
        PrintHelp();
        return 0;    
    }

    Ret = HI_UNF_MCE_Init(HI_NULL);
    if(HI_SUCCESS != Ret)
    {
        printf("ERR: HI_UNF_MCE_Init, Ret = %#x\n", Ret);
        goto ERR0;
    }    

    if (1 == UpdateType)
    {
        Ret = MCE_UpdateBase();
        if(HI_SUCCESS != Ret)
        {
            goto ERR1;
        }
    }
    else if (2 == UpdateType)
    {
        pSrcFile = argv[2];
        Ret = MCE_UpdateLogo(pSrcFile);
        if(HI_SUCCESS != Ret)
        {
            goto ERR1;
        }        
    }
    else if (3 == UpdateType)
    {
        pSrcFile = argv[2];
        Ret = MCE_UpdateDVB(pSrcFile);
        if(HI_SUCCESS != Ret)
        {
            goto ERR1;
        }      
    }
    
ERR1:
    HI_UNF_MCE_DeInit();  
ERR0:    
    return Ret;
}



