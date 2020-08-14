/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_disp.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/12/15
  Description   :
  History       :
  1.Date        : 2009/12/15
    Author      : w58735
    Modification: Created file

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <linux/types.h>

#include "hi_mpi_disp.h"
#include "hi_drv_struct.h"
#include "hi_error_mpi.h"

#include "hi_drv_disp.h"
#include "drv_disp_ioctl.h"

#ifdef ANDROID
#include <utils/Log.h>
#undef LOG_NDEBUG
#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "Hi_Vdp"
#endif

#ifdef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif


static HI_S32            g_DispDevFd    = -1;
static const HI_CHAR     g_DispDevName[] ="/dev/"UMAP_DEVNAME_DISP;
static pthread_mutex_t   g_DispMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_DISP_LOCK()       (void)pthread_mutex_lock(&g_DispMutex);
#define HI_DISP_UNLOCK()     (void)pthread_mutex_unlock(&g_DispMutex);


#define CHECK_DISP_PTR(pointer)\
do{\
    if (!(pointer))\
    {\
        HI_ERR_DISP("para is null ptr.\n");\
        return HI_ERR_DISP_NULL_PTR;\
    }\
}while(0)



HI_S32 HI_MPI_DISP_Init(HI_VOID)
{
    HI_DISP_LOCK();
 
    if (g_DispDevFd > 0)
    {
        HI_DISP_UNLOCK();
        return HI_SUCCESS;
    }

    g_DispDevFd = open(g_DispDevName, O_RDWR|O_NONBLOCK, 0);

    if (g_DispDevFd < 0)
    {
        HI_FATAL_DISP("open DISP err.\n");
        HI_DISP_UNLOCK();
        return HI_ERR_DISP_DEV_OPEN_ERR;
    }

    HI_DISP_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_DeInit(HI_VOID)
{
    HI_S32 Ret;

    HI_DISP_LOCK();

    if (g_DispDevFd < 0)
    {
        HI_DISP_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = close(g_DispDevFd);

    if(HI_SUCCESS != Ret)
    {
        HI_FATAL_DISP("DeInit DISP err.\n");
        HI_DISP_UNLOCK();
        return HI_ERR_DISP_DEV_CLOSE_ERR;
    }

    g_DispDevFd = -1;

    HI_DISP_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_Attach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32         Ret;
    DISP_ATTACH_S  DispAttach;

    if (enMaster >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enMaster is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enSlave >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enSlave is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispAttach.enMaster = enMaster;
    DispAttach.enSlave  = enSlave;

    Ret = ioctl(g_DispDevFd, CMD_DISP_ATTACH, &DispAttach);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_Detach(HI_DRV_DISPLAY_E enMaster, HI_DRV_DISPLAY_E enSlave)
{
    HI_S32          Ret;
    DISP_ATTACH_S   DispAttach;

    if (enMaster >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enMaster is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enSlave >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enSlave is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispAttach.enMaster = enMaster;
    DispAttach.enSlave  = enSlave;

    Ret = ioctl(g_DispDevFd, CMD_DISP_DETACH, &DispAttach);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_Open(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32         Ret;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    Ret = ioctl(g_DispDevFd, CMD_DISP_OPEN, &enDisp);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_Close(HI_DRV_DISPLAY_E enDisp)
{
    HI_S32      Ret;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    Ret = ioctl(g_DispDevFd, CMD_DISP_CLOSE, &enDisp);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
    HI_S32          Ret;
    DISP_ENABLE_S   DispEnable;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((bEnable != HI_TRUE)
      &&(bEnable != HI_FALSE)
       )
    {
        HI_ERR_DISP("para bEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispEnable.enDisp  = enDisp;
    DispEnable.bEnable = bEnable;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_ENABLE, &DispEnable);
    
    return Ret;
}

/*add  for Isogeny set fmt.*/
HI_S32 HI_MPI_DISP_SetIsogenyAttr(const HI_DRV_DISP_ISOGENY_ATTR_S *pstIsogeny,
								  const HI_U32 u32Num)
{
	HI_U32 i = 0;
	HI_S32 Ret = HI_SUCCESS;
	HI_DRV_DISP_ISOGENY_ATTR_S stIsogeny_tmp[HI_DRV_DISPLAY_BUTT];

	if (u32Num > HI_DRV_DISPLAY_BUTT)
	{
		HI_ERR_DISP("Invalid Para u32Num %d!\n",u32Num);
		Ret = HI_ERR_VO_INVALID_PARA;	
        goto __RETURN_POINT;
	}

	memset((HI_VOID*)stIsogeny_tmp, 0, sizeof(HI_DRV_DISP_ISOGENY_ATTR_S)*u32Num);

    for (i = 0; i < u32Num; i++)
    {
		stIsogeny_tmp[i].enDisp = pstIsogeny[i].enDisp;
    }	
	
	Ret = HI_MPI_DISP_GetIsogenyAttr(stIsogeny_tmp, u32Num);
	if (Ret)
	{
		HI_ERR_DISP("Get isogenyattr failed£¡\n");		
        goto __RETURN_POINT;
	}


	if (!memcmp((HI_VOID*)stIsogeny_tmp, (HI_VOID*)pstIsogeny, sizeof(HI_DRV_DISP_ISOGENY_ATTR_S)*u32Num))
	{	
		return HI_SUCCESS;
	}
	
    Ret = HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_1, HI_FALSE);
    if (Ret)
	{
		HI_ERR_DISP("Disable hd channel failed£¡\n");		
        goto __RETURN_POINT;
	}
	
    Ret = HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_0, HI_FALSE);
    if (Ret)
	{
		HI_ERR_DISP("Disable sd channel failed£¡\n");		
        goto __RETURN_POINT;
	}

    for (i = 0; i < u32Num; i++)
    {
        Ret = HI_MPI_DISP_SetFormat(pstIsogeny[i].enDisp, HI_DRV_DISP_STEREO_NONE, pstIsogeny[i].enFormat);
        if (Ret)
		{
			HI_ERR_DISP("endisp %d channel setformat failed£¡\n", pstIsogeny[i].enDisp);
            goto __RETURN_POINT;
		}
    }

    Ret = HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_1, HI_TRUE);
    if (Ret)
	{
		HI_ERR_DISP("enable hd channel failed£¡\n");
        goto __RETURN_POINT;
	}

    Ret = HI_MPI_DISP_SetEnable(HI_DRV_DISPLAY_0, HI_TRUE);
    if (Ret)
	{
		HI_ERR_DISP("enable sd channel failed£¡\n");
        goto __RETURN_POINT;
	}

__RETURN_POINT:
    return Ret;	
}

/*Get Fmt attr.*/
HI_S32 HI_MPI_DISP_GetIsogenyAttr(HI_DRV_DISP_ISOGENY_ATTR_S *pstIsogeny,
								 const HI_U32 u32Num)
{
	HI_U32 i = 0;
	HI_S32 Ret = HI_SUCCESS;
	HI_DRV_DISP_STEREO_MODE_E enStereo;
			
    for (i = 0; i < u32Num; i++)
    {
        Ret = HI_MPI_DISP_GetFormat(pstIsogeny[i].enDisp, &enStereo, &pstIsogeny[i].enFormat);
        if (Ret)
		{
			HI_ERR_DISP("get format failed£¡\n");			
            break;
		}
    }	

    return Ret;	
}



HI_S32 HI_MPI_DISP_GetEnable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable)
{
    HI_S32          Ret;
    DISP_ENABLE_S   DispEnable;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pbEnable)
    {
        HI_ERR_DISP("para pbEnable is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispEnable.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_ENABLE, &DispEnable);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pbEnable = DispEnable.bEnable;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E enStereo, HI_DRV_DISP_FMT_E enFormat)
{
    HI_S32          Ret;
    DISP_FORMAT_S   DispFormat;

    if (enDisp > HI_DRV_DISPLAY_1)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enStereo > HI_DRV_DISP_STEREO_TAB)
    {
        HI_ERR_DISP("para enStereo is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enStereo == HI_DRV_DISP_STEREO_FRAME_PACKING)
    {
        if (  (enFormat < HI_DRV_DISP_FMT_1080P_24_FP)
            ||(enFormat > HI_DRV_DISP_FMT_720P_50_FP)
            )
        {
            HI_ERR_DISP("para enStereo is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else if (enStereo == HI_DRV_DISP_STEREO_SBS_HALF)
    {
        if (  (enFormat != HI_DRV_DISP_FMT_1080i_60)
            &&(enFormat != HI_DRV_DISP_FMT_1080i_50)
            )
        {
            HI_ERR_DISP("para enStereo is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else if (enStereo == HI_DRV_DISP_STEREO_TAB)
    {
        if (  (enFormat != HI_DRV_DISP_FMT_1080P_24)
            &&(enFormat != HI_DRV_DISP_FMT_720P_50)
            &&(enFormat != HI_DRV_DISP_FMT_720P_60)
            )
        {
            HI_ERR_DISP("para enStereo is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }
    else
    {
        if (enFormat > HI_DRV_DISP_FMT_CUSTOM)
        {
            HI_ERR_DISP("para enFormat is invalid.\n");
            return HI_ERR_DISP_INVALID_PARA;
        }
    }

    CHECK_DISP_INIT();

    DispFormat.enDisp   = enDisp;
    DispFormat.enStereo = enStereo;
    DispFormat.enFormat = enFormat;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_FORMAT, &DispFormat);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_GetFormat(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_STEREO_MODE_E *penStereo,HI_DRV_DISP_FMT_E *penFormat)
{

    HI_S32                   Ret;
    DISP_FORMAT_S            DispFormat;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_PTR(penStereo);
    CHECK_DISP_PTR(penFormat);
    CHECK_DISP_INIT();    

    DispFormat.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_FORMAT, &DispFormat);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    
    *penFormat = DispFormat.enFormat;
    *penStereo = DispFormat.enStereo;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetRightEyeFirst(HI_DRV_DISPLAY_E enDisp, HI_BOOL bRFirst)
{
    HI_S32 Ret;
    DISP_R_EYE_FIRST_S stRF;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();    

    stRF.enDisp = enDisp;
    stRF.bREFirst = bRFirst;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_R_E_FIRST, &stRF);

    return Ret;
}

HI_S32 HI_MPI_DISP_SetVirtualScreen(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Width, HI_U32 u32Height)
{
    HI_S32 Ret;
    
    DISP_VIRTSCREEN_S virtscreen;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    CHECK_DISP_INIT();    
    
    virtscreen.enDisp = enDisp;
    virtscreen.stVirtScreen.s32X        = 0;
    virtscreen.stVirtScreen.s32Y        = 0;
    virtscreen.stVirtScreen.s32Height   = u32Height;
    virtscreen.stVirtScreen.s32Width    = u32Width;   
    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_VIRTSCREEN, &virtscreen);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetVirtualScreen(HI_DRV_DISPLAY_E enDisp, HI_U32 *u32Width, HI_U32 *u32Height)
{
    HI_S32 Ret;    
    DISP_VIRTSCREEN_S virtscreen;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((!u32Width) || (!u32Height))
    {
        HI_ERR_DISP("para ptr is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
    
    CHECK_DISP_INIT();    
    
    virtscreen.enDisp = enDisp;    
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_VIRTSCREEN, &virtscreen);

    if (Ret != HI_SUCCESS)
        return Ret;
    
    *u32Width    =   virtscreen.stVirtScreen.s32Width;    
    *u32Height   =   virtscreen.stVirtScreen.s32Height;
    
    return Ret;
}


HI_S32 HI_MPI_DISP_SetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S *pstOffset)
{
    HI_S32 Ret;
    DISP_SCREENOFFSET_S screen_offset;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstOffset)
    {
        HI_ERR_DISP("para ptr is null.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();    
    
    screen_offset.enDisp = enDisp;
    screen_offset.stScreenOffset =  *pstOffset;    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_SCREENOFFSET, &screen_offset);
    
    return Ret;
}


HI_S32 HI_MPI_DISP_GetScreenOffset(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OFFSET_S *pstOffset)
{
    HI_S32 Ret;
    DISP_SCREENOFFSET_S screen_offset;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    if (!pstOffset)
    {
        HI_ERR_DISP("para ptr is null.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();    

    screen_offset.enDisp = enDisp;     
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_SCREENOFFSET, &screen_offset);
    if (Ret != HI_SUCCESS)
        return Ret;

    *pstOffset  =  screen_offset.stScreenOffset;    
    return Ret;
}


HI_S32 HI_MPI_DISP_SetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 u32ARHori, HI_U32 u32ARVert)
{
    HI_S32        Ret;
    DISP_ASPECT_RATIO_S   DispDevRatio;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enVo is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 
    
    CHECK_DISP_INIT();

    DispDevRatio.enDisp = enDisp;
    DispDevRatio.u32ARHori = u32ARHori;
    DispDevRatio.u32ARVert = u32ARVert;
    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_DEV_RATIO, &DispDevRatio);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetAspectRatio(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32ARHori, HI_U32 *pu32ARVert)
{
    HI_S32        Ret;
    DISP_ASPECT_RATIO_S   DispDevRatio;

     if ( !pu32ARHori || !pu32ARVert)
    {
        HI_ERR_DISP("para pstDispAspectRatio is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
     
    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enVo is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 
    CHECK_DISP_INIT();

    DispDevRatio.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_DEV_RATIO, &DispDevRatio);

    if (!Ret)
    {
        *pu32ARHori = DispDevRatio.u32ARHori;
        *pu32ARVert = DispDevRatio.u32ARVert;
    }
    
    return Ret;
}



HI_S32 HI_MPI_DISP_AddIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    HI_S32 Ret;
    DISP_SET_INTF_S DispIntf;

    CHECK_DISP_INIT();

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    DispIntf.enDisp = enDisp;
    memcpy(&DispIntf.stIntf, pstIntf, sizeof(HI_DRV_DISP_INTF_S));

    Ret = ioctl(g_DispDevFd, CMD_DISP_ADD_INTF, &DispIntf);
    return Ret;
}


HI_S32 HI_MPI_DISP_DelIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_INTF_S *pstIntf)
{
    HI_S32 Ret;
    DISP_SET_INTF_S DispIntf;

    CHECK_DISP_INIT();

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    DispIntf.enDisp     = enDisp;
    memcpy(&DispIntf.stIntf, pstIntf, sizeof(HI_DRV_DISP_INTF_S));

    Ret = ioctl(g_DispDevFd, CMD_DISP_DEL_INTF, &DispIntf);
    return Ret;
}


HI_S32 HI_MPI_DISP_SetTiming(HI_DRV_DISPLAY_E enDisp,  HI_DRV_DISP_TIMING_S *pstTiming)
{
    HI_S32          Ret;
    DISP_TIMING_S DispTiming;
    
    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstTiming)
    {
        HI_ERR_DISP("para pstLcdPara is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispTiming.enDisp = enDisp;
    memcpy(&DispTiming.stTimingPara, pstTiming, sizeof(HI_DRV_DISP_TIMING_S));
    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_TIMING, &DispTiming);
   
    return Ret;
}

HI_S32 HI_MPI_DISP_GetTiming(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_TIMING_S *pstTiming)
{
    HI_S32          Ret;
    DISP_TIMING_S DispTiming;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstTiming)
    {
        HI_ERR_DISP("para pstLcdPara is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispTiming.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_TIMING, &DispTiming);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pstTiming,&DispTiming.stTimingPara,sizeof(HI_DRV_DISP_TIMING_S));
    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_DRV_DISP_ZORDER_E enZFlag)
{
    HI_S32           Ret;
    DISP_ZORDER_S    DispZorder;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enLayer >= HI_DRV_DISP_LAYER_BUTT)
    {
        HI_ERR_DISP("para enLayer is invalid.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (enZFlag >= HI_DRV_DISP_ZORDER_BUTT)
    {
        HI_ERR_DISP("para enZFlag is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispZorder.enDisp = enDisp;
    DispZorder.Layer = enLayer;
    DispZorder.ZFlag = enZFlag;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_ZORDER, &DispZorder);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetLayerZorder(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_LAYER_E enLayer, HI_U32 *pu32Zorder)
{
    HI_S32           Ret;
    DISP_ORDER_S     DispOrder;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enLayer >= HI_DRV_DISP_LAYER_BUTT)
    {
        HI_ERR_DISP("para enLayer is invalid.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (!pu32Zorder)
    {
        HI_ERR_DISP("para pu32Zorder is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispOrder.enDisp = enDisp;
    DispOrder.Layer = enLayer;
    
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_ORDER, &DispOrder);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    
    *pu32Zorder = DispOrder.Order;
    return HI_SUCCESS;
}


#define HI_DISP_BGC_MAX_VALUE 255
HI_S32 HI_MPI_DISP_SetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor)
{
    HI_S32          Ret;
    DISP_BGC_S      DispBgc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstBgColor)
    {
        HI_ERR_DISP("para pstBgColor is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispBgc.enDisp = enDisp;
    memcpy(&DispBgc.stBgColor, pstBgColor, sizeof(HI_DRV_DISP_COLOR_S));

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_BGC, &DispBgc);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetBGColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_S *pstBgColor)
{
    HI_S32          Ret;
    DISP_BGC_S      DispBgc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstBgColor)
    {
        HI_ERR_DISP("para pstBgColor is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispBgc.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_BGC, &DispBgc);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    memcpy(pstBgColor, &DispBgc.stBgColor, sizeof(HI_DRV_DISP_COLOR_S));

    return HI_SUCCESS;
}

#define PQ_PARA_MAX 100
#if defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100) || defined(CHIP_TYPE_hi3716cv200) || defined(CHIP_TYPE_hi3716mv400) || defined(CHIP_TYPE_hi3719cv100) || defined(CHIP_TYPE_hi3718cv100) || defined(CHIP_TYPE_hi3719mv100_a)   

HI_S32 HI_MPI_DISP_SetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Brightness)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (u32Brightness > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Brightness is %d invalid.\n", u32Brightness);
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;
    DispCsc.CscValue = u32Brightness;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_BRIGHT, &DispCsc);
   
    return Ret;
}

HI_S32 HI_MPI_DISP_GetBrightness(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Brightness)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pu32Brightness)
    {
        HI_ERR_DISP("para pu32Brightness is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_BRIGHT, &DispCsc);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32Brightness = DispCsc.CscValue;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Contrast)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (u32Contrast > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Contrast is %d invalid.\n", u32Contrast);
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;
    DispCsc.CscValue = u32Contrast;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_CONTRAST, &DispCsc);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetContrast(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Contrast)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pu32Contrast)
    {
        HI_ERR_DISP("para pu32Contrast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_CONTRAST, &DispCsc);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32Contrast = DispCsc.CscValue;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 u32Saturation)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (u32Saturation > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32Saturation is %d invalid.\n", u32Saturation);
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;
    DispCsc.CscValue = u32Saturation;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_SATURATION, &DispCsc);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetSaturation(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32Saturation)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pu32Saturation)
    {
        HI_ERR_DISP("para pu32Saturation is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_SATURATION, &DispCsc);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32Saturation = DispCsc.CscValue;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 u32HuePlus)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (u32HuePlus > PQ_PARA_MAX)
    {
        HI_ERR_DISP("para u32HuePlus is %d invalid.\n", u32HuePlus);
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;
    DispCsc.CscValue = u32HuePlus;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_HUE, &DispCsc);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetHuePlus(HI_DRV_DISPLAY_E enDisp, HI_U32 *pu32HuePlus)
{
    HI_S32          Ret;
    DISP_CSC_S      DispCsc;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pu32HuePlus)
    {
        HI_ERR_DISP("para pu32HuePlus is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispCsc.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_HUE, &DispCsc);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *pu32HuePlus = DispCsc.CscValue;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetAlgCfg(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_ALG_CFG_S *pstAlg)
{
#if 1
	return HI_ERR_DISP_NOT_SUPPORT;
#else
    HI_S32          Ret;
    DISP_ALG_S      DispAlg;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
     if (NULL == pstAlg)
    {
        HI_ERR_DISP("para pstAlg is NULL.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
#if 0
    if ((pstAlg->bAccEnable != HI_TRUE)
      &&(pstAlg->bAccEnable != HI_FALSE)
       )
    {
        HI_ERR_DISP("para bEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if ((pstAlg->bSharpEnable != HI_TRUE)
      &&(pstAlg->bSharpEnable != HI_FALSE)
       )
    {
        HI_ERR_DISP("para bEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
#endif

    CHECK_DISP_INIT();

    DispAlg.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_ALG, &DispAlg);
    
    return Ret;
#endif	
}

HI_S32 HI_MPI_DISP_GetAlgCfg(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_ALG_CFG_S *pstAlg)
{
#if 1
	return HI_ERR_DISP_NOT_SUPPORT;
#else
    HI_S32          Ret;
    DISP_ALG_S      DispAlg;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

     if (NULL == pstAlg)
    {
        HI_ERR_DISP("para pstAlg is NULL.\n");
        return HI_ERR_DISP_NULL_PTR;
    }
     
    CHECK_DISP_INIT();

    DispAlg.enDisp = enDisp;
    
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_ALG, &DispAlg);

//    pstAlg->bAccEnable = DispAlg.stAlg.bAccEnable;
//    pstAlg->bSharpEnable = DispAlg.stAlg.bAccEnable;
    
    return Ret;
#endif
}

#endif
HI_S32 HI_MPI_DISP_CreateVBI(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_VBI_CFG_S *pstCfg, HI_HANDLE *phVbi)
{
    HI_S32 Ret;
    DISP_VBI_CREATE_CHANNEL_S    DispVbiCrtChanl;
    
    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    if (!pstCfg)
    {
        HI_ERR_DISP("para pstCfg is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }  

    if (!phVbi)
    {
        HI_ERR_DISP("para phVbi is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }  
    CHECK_DISP_INIT();
    
    DispVbiCrtChanl.enDisp = enDisp;
 
    memcpy(&DispVbiCrtChanl.stCfg, pstCfg, sizeof(HI_DRV_DISP_VBI_CFG_S));
        
    Ret = ioctl(g_DispDevFd, CMD_DISP_CREATE_VBI_CHANNEL, &DispVbiCrtChanl);
     if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
     
    *phVbi = DispVbiCrtChanl.hVbi;
    return Ret;
}

HI_S32 HI_MPI_DISP_DestroyVBI(HI_HANDLE hVbi)
{
    HI_S32 Ret;
    if ( HI_INVALID_HANDLE == hVbi )
    {
        HI_ERR_DISP("para hVbi is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }   
    CHECK_DISP_INIT();

    Ret = ioctl(g_DispDevFd, CMD_DISP_DESTROY_VBI_CHANNEL, &hVbi);
    return Ret;    
}

HI_S32 HI_MPI_DISP_SendVBIData(HI_HANDLE hVbi, const HI_DRV_DISP_VBI_DATA_S *pstVbiData)
{
    HI_S32          Ret;
    DISP_VBI_S      DispVbi;

    CHECK_DISP_INIT();

    if (hVbi >= (HI_HANDLE)HI_DRV_DISP_VBI_TYPE_BUTT)
    {
        HI_ERR_DISP("para hVbi is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstVbiData)
    {
        HI_ERR_DISP("para pstVbiData is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (!(pstVbiData->pu8DataAddr))
    {
        HI_ERR_DISP("para pstVbiData->pu8DataAddr is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if (!(pstVbiData->u32DataLen))
    {
        return HI_SUCCESS;
    }

    DispVbi.hVbi = hVbi;
    memcpy(&DispVbi.stVbiData, pstVbiData, sizeof(HI_DRV_DISP_VBI_DATA_S));

    Ret = ioctl(g_DispDevFd, CMD_DISP_SEND_VBI, &DispVbi);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_SetWss(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_WSS_DATA_S *pstWssData)
{
    HI_S32          Ret;
    DISP_WSS_S      DispWss;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (!pstWssData)
    {
        HI_ERR_DISP("para pstWssData is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    if ((pstWssData->bEnable != HI_TRUE)
      &&(pstWssData->bEnable != HI_FALSE)
       )
    {
        HI_ERR_DISP("para pstWssData->bEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispWss.enDisp = enDisp;
    memcpy(&DispWss.WssData, pstWssData, sizeof(HI_DRV_DISP_WSS_DATA_S));

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_WSS, &DispWss);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_SetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E enMode)
{
    HI_S32          Ret;
    DISP_MCRVSN_S   DispMcrvsn;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    if (enMode >= HI_DRV_DISP_MACROVISION_BUTT)
    {
        HI_ERR_DISP("para enMode is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }

    CHECK_DISP_INIT();

    DispMcrvsn.enDisp = enDisp;
    DispMcrvsn.eMcrvsn = enMode;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_MCRVSN, &DispMcrvsn);
    
    return Ret;
}

HI_S32 HI_MPI_DISP_GetMacrovision(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_MACROVISION_E *penMode)
{
    HI_S32          Ret;
    DISP_MCRVSN_S   DispMcrvsn;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    if (!penMode)
    {
        HI_ERR_DISP("para penMode is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispMcrvsn.enDisp = enDisp;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_MCRVSN, &DispMcrvsn);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    *penMode = DispMcrvsn.eMcrvsn;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg)
{
    HI_S32          Ret;
    DISP_HDMIINF_S  DispHmdiIntf;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    if (!pstCfg)
    {
        HI_ERR_DISP("para pstCfg is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispHmdiIntf.enDisp = enDisp;
    memcpy(&(DispHmdiIntf.HDMIInf), pstCfg, sizeof(HI_DRV_DISP_HDMI_S));
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_HDMI_INTF, &DispHmdiIntf);
    HI_INFO_DISP("CMD_DISP_SET_HDMI_INTF invoke (%d):0x%x\n", g_DispDevFd, Ret);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_GetHdmiIntf(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_HDMI_S *pstCfg)
{
    HI_S32          Ret;
    DISP_HDMIINF_S  DispHmdiIntf;

    if (enDisp >= HI_DRV_DISPLAY_BUTT)
    {
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    if (!pstCfg)
    {
        HI_ERR_DISP("para pstCfg is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    CHECK_DISP_INIT();

    DispHmdiIntf.enDisp = enDisp;
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_HDMI_INTF, &DispHmdiIntf);
    HI_INFO_DISP("CMD_DISP_GET_HDMI_INTF invoke (%d):0x%x\n", g_DispDevFd, Ret);
    if (Ret != HI_SUCCESS)
    {
        return Ret;
    }
    memcpy(pstCfg, &(DispHmdiIntf.HDMIInf), sizeof(HI_DRV_DISP_HDMI_S));

    return HI_SUCCESS;
}

/* MPI_disp interface for CGMS; 2011-06-02, Huang Minghu */
HI_S32 HI_MPI_DISP_SetCgms(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_CGMSA_CFG_S *pstCgmsCgf)
{
    HI_S32      Ret = 0;
    DISP_CGMS_S DispCgms;
      
    /* DISP validity check */
    if(enDisp >= HI_DRV_DISPLAY_BUTT)
    { 
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 

    /* pointer validity check */
    if (!pstCgmsCgf)
    {
        HI_ERR_DISP("para pstCgmsCgf is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    /* parameters assignment */
    DispCgms.enDisp    = enDisp;
    DispCgms.stCgmsCfg = *pstCgmsCgf;

    /* check validity of pstCgmsCgf->bEnable */
    if (  (pstCgmsCgf->bEnable != HI_TRUE) 
        &&(pstCgmsCgf->bEnable != HI_FALSE)
       )
    {
        HI_ERR_DISP("para pstCgmsCgf->bEnable is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    /* check validity of pstCgmsCgf->enType */
    if (pstCgmsCgf->enType >= HI_DRV_DISP_CGMSA_TYPE_BUTT)
      
    {
        HI_ERR_DISP("para pstCgmsCgf->enType is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    }
    
    /* check validity of pstCgmsCgf->enMode */
    if (pstCgmsCgf->enMode >= HI_DRV_DISP_CGMSA_MODE_BUTT)
       
    {
        HI_ERR_DISP("para pstCgmsCgf->enMode is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 

    /* call SET_CGMS by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_CGMS, &DispCgms);

    return Ret;

}

HI_S32 HI_MPI_DISP_Snapshot_Acquire(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstSnapShotFrame)
{
	DISP_SNAPSHOT_FRAME_S stFrame;
	HI_S32 Ret;
    /* DISP validity check */
    if(enDisp >= HI_DRV_DISPLAY_BUTT)
    { 
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 

    /* check DISP initialization status */
    CHECK_DISP_INIT();
    memset((void*)&stFrame, 0, sizeof(DISP_SNAPSHOT_FRAME_S));
    
	stFrame.enDispLayer = enDisp;
	Ret =  ioctl(g_DispDevFd, CMD_DISP_ACQUIRE_SNAPSHOT, &stFrame);

	if (!Ret)
		memcpy(pstSnapShotFrame, &(stFrame.stFrame), sizeof(HI_DRV_VIDEO_FRAME_S));

	return Ret;
}

HI_S32 HI_MPI_DISP_Snapshot_Release(HI_DRV_DISPLAY_E enDisp, HI_DRV_VIDEO_FRAME_S * pstSnapShotFrame)
{
	DISP_SNAPSHOT_FRAME_S stFrame;

    /* DISP validity check */
    if(enDisp >= HI_DRV_DISPLAY_BUTT)
    { 
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 

    /* check DISP initialization status */
    CHECK_DISP_INIT();

	stFrame.enDispLayer = enDisp;
	stFrame.stFrame = *pstSnapShotFrame;
	
	return ioctl(g_DispDevFd, CMD_DISP_RELEASE_SNAPSHOT, &stFrame);
}

HI_S32 HI_MPI_DISP_CreateCast(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_CAST_CFG_S * pstCfg, HI_HANDLE *phCast)
{
    DISP_CAST_CREATE_S stCast;
    HI_S32      Ret = 0;

    /* DISP validity check */
    if(enDisp >= HI_DRV_DISPLAY_BUTT)
    { 
        HI_ERR_DISP("para enDisp is invalid.\n");
        return HI_ERR_DISP_INVALID_PARA;
    } 

    /* pointer validity check */
    if (!pstCfg || !phCast)
    {
        HI_ERR_DISP("par pstCfg or phCast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.enDisp = enDisp;
    stCast.stCfg  = *pstCfg;

    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_CREATE_CAST, &stCast);
    if (!Ret)
    {
        *phCast = stCast.hCast;
    }

    return Ret;
}
HI_S32 HI_MPI_DISP_DestroyCast(HI_HANDLE hCast)
{
    DISP_CAST_DESTROY_S stCast;
    HI_S32      Ret = 0;

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.hCast = hCast;
    
    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_DESTROY_CAST, &stCast);
    return Ret;
}

HI_S32 HI_MPI_DISP_SetCastEnable(HI_HANDLE hCast, HI_BOOL bEnable)
{
    DISP_CAST_ENABLE_S stCast;
    HI_S32      Ret = 0;

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.hCast = hCast;
    stCast.bEnable = bEnable;
    
    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_CAST_ENABLE, &stCast);

    return Ret;
}

HI_S32 HI_MPI_DISP_GetCastEnable(HI_HANDLE hCast, HI_BOOL *pbEnable)
{
    DISP_CAST_ENABLE_S stCast;
    HI_S32      Ret = 0;

    /* pointer validity check */
    if (!pbEnable)
    {
        HI_ERR_DISP("par pbEnable or phCast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.hCast = hCast;
    
    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_CAST_ENABLE, &stCast);
	if (!Ret)
	{
		*pbEnable = stCast.bEnable;
	}

    return Ret;
}
HI_S32 HI_MPI_DISP_AcquireCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_FRAME_S stCast;
    HI_S32      Ret = 0;

    /* pointer validity check */
    if (!pstCastFrame)
    {
        HI_ERR_DISP("par pstCastFrame or phCast is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.hCast = hCast;
    
    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_ACQUIRE_CAST_FRAME, &stCast);
    if (!Ret)
    {
        *pstCastFrame = stCast.stFrame;
#if 0
        HI_ERR_DISP(" CMD_DISP_ACQUIRE_CAST_FRAME w=%d, h=%d,y=0x%x,c=0x%x\n", 
                    pstCastFrame->u32Width, pstCastFrame->u32Height,
                    pstCastFrame->stBufAddr[0].u32PhyAddr_Y,
                    pstCastFrame->stBufAddr[0].u32PhyAddr_C);
#endif
    }

    return Ret;
}
HI_S32 HI_MPI_DISP_ReleaseCastFrame(HI_HANDLE hCast, HI_DRV_VIDEO_FRAME_S *pstCastFrame)
{
    DISP_CAST_FRAME_S stCast;
    HI_S32      Ret = 0;

    /* pointer validity check */
    if (!pstCastFrame)
    {
        HI_ERR_DISP("par pstCastFrame is null.\n");
        return HI_ERR_DISP_NULL_PTR;
    }

    /* check DISP initialization status */
    CHECK_DISP_INIT();

    stCast.hCast = hCast;
    stCast.stFrame = *pstCastFrame;

    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_RELEASE_CAST_FRAME, &stCast);

    return Ret;
}

HI_S32 HI_MPI_DISP_ExtAttach(HI_HANDLE hCast, HI_HANDLE hSink)
{
    HI_S32      Ret = HI_SUCCESS;
    DISP_EXT_ATTACH_S stAttach;
    
    if (HI_INVALID_HANDLE == hCast)
    {
        HI_ERR_DISP("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    CHECK_DISP_INIT();

    stAttach.enType = EXT_ATTACH_TYPE_SINK;
    stAttach.hCast =  hCast;
    stAttach.hMutual = hSink;

    Ret = ioctl(g_DispDevFd, CMD_DISP_EXT_ATTACH, &stAttach);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_DISP("HI_MPI_DISP_ExtAttach failed\n");
    }
    
    return Ret;
}

HI_S32 HI_MPI_DISP_ExtDeAttach(HI_HANDLE hCast, HI_HANDLE hSink)
{
    HI_S32      Ret = HI_SUCCESS;
    DISP_EXT_ATTACH_S stAttach;
    
    if (HI_INVALID_HANDLE == hCast)
    {
        HI_ERR_DISP("para hWindow is invalid.\n");
        return HI_ERR_VO_INVALID_PARA; 
    }
    CHECK_DISP_INIT();

    stAttach.enType = EXT_ATTACH_TYPE_SINK;
    stAttach.hCast =  hCast;
    stAttach.hMutual = hSink;

    Ret = ioctl(g_DispDevFd, CMD_DISP_EXT_DEATTACH, &stAttach);
    if (Ret != HI_SUCCESS)
    {
        HI_ERR_DISP("HI_MPI_DISP_ExtAttach failed\n");
    }
    
    return Ret;
}

HI_S32 HI_MPI_DISP_SetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    HI_ERR_DISP(" The Func is not support.\n");
    return HI_ERR_DISP_NOT_EXIST;
}

HI_S32 HI_MPI_DISP_GetColor(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_COLOR_SETTING_S *pstCS)
{
    HI_ERR_DISP(" The Func is not support.\n");
    return HI_ERR_DISP_NOT_EXIST;
}

HI_S32 HI_MPI_DISP_Suspend(HI_VOID)
{
    HI_U32 u32Value = 0xaaaaaaaa;
    HI_S32 Ret = 0;


    /* check DISP initialization status */
    CHECK_DISP_INIT();


    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_SUSPEND, &u32Value);

    return Ret;
}

HI_S32 HI_MPI_DISP_Resume(HI_VOID)
{
    HI_U32 u32Value = 0xaaaaaaaa;
    HI_S32 Ret = 0;


    /* check DISP initialization status */
    CHECK_DISP_INIT();


    /* call DISP_SCREEN by kernel */    
    Ret = ioctl(g_DispDevFd, CMD_DISP_RESUME, &u32Value);

    return Ret;
}

HI_S32 HI_MPI_DISP_SetDacOutputEnable(HI_BOOL bEnable)
{
    HI_S32      Ret = 0;
    HI_BOOL     bDacEnable;
    CHECK_DISP_INIT();
    bDacEnable = bEnable;
    Ret = ioctl(g_DispDevFd, CMD_DISP_FORCESET_DAC_ENABLE, &bDacEnable);
    return Ret;
}

HI_S32 HI_MPI_DISP_SetDispOutput(HI_DRV_DISPLAY_E enDisp, const HI_DRV_DISP_OUT_TYPE_E enDispType)
{
    HI_S32              Ret = 0;
    DISP_SET_OUTPUT_S   stDispOutput;
    CHECK_DISP_INIT();

    stDispOutput.enDisp     = enDisp;
    stDispOutput.enDispType = enDispType;

    Ret = ioctl(g_DispDevFd, CMD_DISP_SET_OUTPUT_TYPE, &stDispOutput);
    return Ret;
}

HI_S32 HI_MPI_DISP_GetDispOutput(HI_DRV_DISPLAY_E enDisp, HI_DRV_DISP_OUT_TYPE_E *penDispType)
{
    HI_S32          Ret = HI_FAILURE;
    DISP_GET_OUTPUT_S   stDispOutput;

    stDispOutput.enDisp = enDisp;
    stDispOutput.enDispType = HI_DRV_DISP_TYPE_BUTT;

    Ret = ioctl(g_DispDevFd, CMD_DISP_GET_OUTPUT_TYPE, &stDispOutput);
    if(HI_SUCCESS != Ret)
    {
       return Ret; 
    }
    
    *penDispType = stDispOutput.enDispType;
    return HI_SUCCESS;
}

HI_S32 HI_MPI_DISP_SetBT2020Enable(HI_DRV_DISPLAY_E enDisp, HI_BOOL bEnable)
{
	HI_S32			Ret;
	DISP_BT2020_ENABLE_S	stBT2020Enable;

	if (enDisp >= HI_DRV_DISPLAY_BUTT)
	{
		HI_ERR_DISP("para enDisp is invalid.\n");
		return HI_ERR_DISP_INVALID_PARA;
	}

	CHECK_DISP_INIT();

	stBT2020Enable.enDisp  = enDisp;
	stBT2020Enable.bBT2020Enable = bEnable;

	Ret = ioctl(g_DispDevFd, CMD_DISP_SET_BT2020_ENABLE, &stBT2020Enable);
	
	return Ret;
}

HI_S32 HI_MPI_DISP_GetBT2020Enable(HI_DRV_DISPLAY_E enDisp, HI_BOOL *pbEnable)
{
	HI_S32			Ret;
	DISP_BT2020_ENABLE_S	stBT2020Enable;

	if (enDisp >= HI_DRV_DISPLAY_BUTT)
	{
		HI_ERR_DISP("para enDisp is invalid.\n");
		return HI_ERR_DISP_INVALID_PARA;
	}


	CHECK_DISP_INIT();

	stBT2020Enable.enDisp  = enDisp;

	Ret = ioctl(g_DispDevFd, CMD_DISP_GET_BT2020_ENABLE, &stBT2020Enable);
	if(HI_SUCCESS != Ret)
    {
       return Ret; 
    }
    
    *pbEnable = stBT2020Enable.bBT2020Enable;
	return Ret;
}
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

