/*******************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: drv_aiao_alsa_func.c
 * Description: aiao alsa interface func
 *
 * History:
 * Version   Date         Author     DefectNum    Description
 * main\1    
 ********************************************************************************/
#include "hal_aiao_common.h"

#include "drv_ao_hw_func.h"
#include "hi_drv_ao.h"
#include "drv_ao_ioctl.h"
#include <linux/kernel.h>
#include <asm/io.h>
#include "drv_ao_op.h"
#include "hal_aiao.h"

//#define DSP0TOA9ADDR 0xf8a200c8
//volatile HI_U32 *pdsp0intaddr;
/************************ interface with aiao *******************************/
//DRV Interface

HI_S32 Alsa_AO_OpenDev(struct file  *file,void *pAlsaAttr,void *p);
extern HI_S32 Alsa_AO_CloseDev(struct file  *file,HI_UNF_SND_E snd_idx);
//SND Interface 
extern HI_S32 AOGetSndDefOpenAttr(HI_UNF_SND_ATTR_S *pstSndAttr);
extern HI_S32 AOSetProcStatistics(AIAO_IsrFunc *pFunc);
extern HI_S32 AOGetProcStatistics(AIAO_IsrFunc **pFunc);
extern HI_S32 AOGetEnport(HI_UNF_SND_E enSound,AIAO_PORT_ID_E *enPort,SND_OUTPUT_TYPE_E enOutType);
extern HI_S32 AOGetHandel(HI_UNF_SND_E enSound,HI_VOID** ppSndOp,SND_OUTPUT_TYPE_E enOutType) ;
extern HI_S32 AlsaHwSndOpStop(HI_VOID* pSndOp, HI_VOID *pstParams);
extern HI_S32 AlsaHwSndOpStart(HI_VOID* pSndOp, HI_VOID *pstParams);

int ao_dma_start(HI_UNF_SND_E snd_idx, void * p)
{
    HI_VOID* pSndOp;
    int ret;
    
    ret = AOGetHandel(snd_idx, &pSndOp, SND_OUTPUT_TYPE_I2S);
    if(ret != HI_SUCCESS )
    {
       return HI_FAILURE;
    }
    
    return AlsaHwSndOpStart(pSndOp, HI_NULL);
}

int ao_dma_stop(HI_UNF_SND_E snd_idx, void * p)
{
    int ret;
    HI_VOID* pSndOp;
    
    ret = AOGetHandel(snd_idx,&pSndOp,SND_OUTPUT_TYPE_I2S);
    if(ret != HI_SUCCESS )
    {
       return HI_FAILURE;
    }
    
    return AlsaHwSndOpStop(pSndOp, HI_NULL);
}

int ao_dma_gethwparam(void *param)
{
    HI_UNF_SND_ATTR_S *pstSndAttr = (HI_UNF_SND_ATTR_S *)param;
    pstSndAttr->u32PortNum = 0;

    pstSndAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_I2S0;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.bMaster = HI_TRUE;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enI2sMode = HI_UNF_I2S_STD_MODE;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enMclkSel = HI_UNF_I2S_MCLK_256_FS;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enBclkSel = HI_UNF_I2S_BCLK_4_DIV;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enChannel = HI_UNF_I2S_CHNUM_2;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enBitDepth = HI_UNF_I2S_BIT_DEPTH_16;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.bPcmSampleRiseEdge = HI_TRUE;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stI2sAttr.stAttr.enPcmDelayCycle = HI_UNF_I2S_PCM_1_DELAY;
    pstSndAttr->u32PortNum++;

    return HI_TRUE;
}

#if 0
int ao_dma_getopendefparam(void *p)
{
    return AOGetSndDefOpenAttr((HI_UNF_SND_ATTR_S *)p);
}
#endif

int hi_ao_alsa_update_writeptr(HI_UNF_SND_E snd_idx,HI_U32 u32WritePos)
{
    int ret;
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
     
     ret = AOGetEnport(snd_idx, &enPort, SND_OUTPUT_TYPE_I2S);  
     if(ret != HI_SUCCESS )
     {
        return HI_FAILURE;
     }

    return HAL_AIAO_P_ALSA_UpdateWptr(enPort, HI_NULL, u32WritePos);
}

void hi_ao_alsa_query_readpos(HI_UNF_SND_E snd_idx,HI_U32 *pos)
{
    int ret;
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;;
     
    ret = AOGetEnport(snd_idx,&enPort,SND_OUTPUT_TYPE_I2S); 
    
   *pos = HAL_AIAO_P_ALSA_QueryReadPos(enPort);
}

int ao_dma_flushbuf(HI_UNF_SND_E snd_idx)
{
     int ret;
     AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;; 
     
     ret = AOGetEnport(snd_idx,&enPort,SND_OUTPUT_TYPE_I2S);  
     if(ret != HI_SUCCESS )
     {
        return HI_FAILURE;
     }
     
     return HAL_AIAO_P_ALSA_FLASH(enPort);   
}

int hi_ao_alsa_get_proc_func(AIAO_IsrFunc **pFunc)
{
    return  AOGetProcStatistics(pFunc);
}

int hi_ao_alsa_set_proc_func(AIAO_IsrFunc *pFunc)
{
    return  AOSetProcStatistics(pFunc);
}

int  ao_dma_open(void *p, void *pAlsaAttr, struct file *file)
{
   int ret;

   ret = Alsa_AO_OpenDev(file,pAlsaAttr, p);
   return ret;
}

int  ao_dma_close(struct file *file,HI_UNF_SND_E snd_idx)
{
   int ret;

   ret = Alsa_AO_CloseDev(file,snd_idx);
   return ret;

}
