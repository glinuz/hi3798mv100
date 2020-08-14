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

#include "drv_ao_hdmi_func.h"
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
extern HI_S32 Alsa_AO_OpenDev(struct file  *file,void *pAlsaAttr,void *p);
extern HI_S32 Alsa_AO_CloseDev(struct file  *file,HI_UNF_SND_E snd_idx);
extern HI_S32 AOGetSndDefOpenAttr(HI_UNF_SND_ATTR_S *pstSndAttr);
extern HI_S32 AOSetProcStatistics(AIAO_IsrFunc *pFunc);
extern HI_S32 AOGetProcStatistics(AIAO_IsrFunc **pFunc);
extern HI_S32 AOGetEnport(HI_UNF_SND_E enSound,AIAO_PORT_ID_E *enPort,SND_OUTPUT_TYPE_E enOutType);
extern HI_S32 AOGetHandel(HI_UNF_SND_E enSound,HI_HANDLE *hSndOp,SND_OUTPUT_TYPE_E enOutType);
extern HI_S32 AlsaHwSndOpStop(HI_HANDLE hSndOp, HI_VOID *pstParams);
extern HI_S32 AlsaHwSndOpStart(HI_HANDLE hSndOp, HI_VOID *pstParams);
extern HI_S32 HI_DRV_AO_SND_SetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S stGain);
extern HI_S32 HI_DRV_AO_SND_GetVolume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S *pstGain);

int ao_dma_hdmi_start(HI_UNF_SND_E snd_idx, void * p)
{
    HI_HANDLE hSndOp;
    int ret;
    
    ret = AOGetHandel(snd_idx,&hSndOp,SND_OUTPUT_TYPE_HDMI);
    if(ret != HI_SUCCESS )
    {
       return HI_FAILURE;
    }
    
    return AlsaHwSndOpStart(hSndOp, HI_NULL);
}

int ao_dma_hdmi_stop(HI_UNF_SND_E snd_idx, void * p)
{
    int ret;
    HI_HANDLE hSndOp;
    
    ret = AOGetHandel(snd_idx,&hSndOp,SND_OUTPUT_TYPE_HDMI);
    if(ret != HI_SUCCESS )
    {
       return HI_FAILURE;
    }
    
    return AlsaHwSndOpStop(hSndOp, HI_NULL);
}

int ao_dma_hdmi_gethwparam(void *param)
{
    HI_UNF_SND_ATTR_S *pstSndAttr = (HI_UNF_SND_ATTR_S *)param;
    pstSndAttr->u32PortNum = 0;

    pstSndAttr->enSampleRate = HI_UNF_SAMPLE_RATE_48K;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].enOutPort = HI_UNF_SND_OUTPUTPORT_HDMI0;
    pstSndAttr->stOutport[pstSndAttr->u32PortNum].unAttr.stHDMIAttr.pPara = HI_NULL;
    pstSndAttr->u32PortNum++;
    return HI_TRUE;

}

int ao_dma_hdmi_getopendefparam(void *p)
{
    return AOGetSndDefOpenAttr((HI_UNF_SND_ATTR_S *)p);
}

int hi_ao_hdmi_alsa_update_writeptr(HI_UNF_SND_E snd_idx,HI_U32 *pu32WritePos)
{
    int ret;
    AIAO_PORT_ID_E enPort;
     
     ret = AOGetEnport(snd_idx,&enPort,SND_OUTPUT_TYPE_HDMI);  
     if(ret != HI_SUCCESS )
     {
        return HI_FAILURE;
     }

    return HAL_AIAO_P_ALSA_UpdateWptr(enPort,NULL, *pu32WritePos);
}

void hi_ao_hdmi_alsa_query_readpos(HI_UNF_SND_E snd_idx,HI_U32 *pos)
{
    int ret;
    AIAO_PORT_ID_E enPort = AIAO_PORT_BUTT;
     
    ret = AOGetEnport(snd_idx,&enPort,SND_OUTPUT_TYPE_HDMI); 
    
   *pos = HAL_AIAO_P_ALSA_QueryReadPos(enPort);
}

int ao_dma_hdmi_flushbuf(HI_UNF_SND_E snd_idx)
{
     int ret;
     AIAO_PORT_ID_E enPort; 
     
     ret = AOGetEnport(snd_idx,&enPort,SND_OUTPUT_TYPE_HDMI);  
     if(ret != HI_SUCCESS )
     {
        return HI_FAILURE;
     }
     return HAL_AIAO_P_ALSA_FLASH(enPort);   
}

int hi_ao_hdmi_alsa_get_proc_func(AIAO_IsrFunc **pFunc)
{
    return  AOGetProcStatistics(pFunc);
}

int hi_ao_hdmi_alsa_set_proc_func(AIAO_IsrFunc *pFunc)
{
    return  AOSetProcStatistics(pFunc);
}

int  ao_dma_hdmi_open(void *p, void *pAlsaAttr, struct file *file)
{
   int ret;
   
   ret = Alsa_AO_OpenDev(file,pAlsaAttr,p);
   return ret;
}

int  ao_dma_hdmi_close(struct file *file,HI_UNF_SND_E snd_idx)
{
   int ret;

   ret = Alsa_AO_CloseDev(file,snd_idx);
   return ret;
}

int ao_hdmi_set_volume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S stGain)
{
	return HI_DRV_AO_SND_SetVolume(enSound, enOutPort, stGain);
}

int ao_hdmi_get_volume(HI_UNF_SND_E enSound, HI_UNF_SND_OUTPUTPORT_E enOutPort, HI_UNF_SND_GAIN_ATTR_S * stGain)
{
	return HI_DRV_AO_SND_GetVolume(enSound, enOutPort, stGain);
}

