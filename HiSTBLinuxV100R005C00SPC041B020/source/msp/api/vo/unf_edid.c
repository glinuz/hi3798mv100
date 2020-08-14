/******************************************************************************

   Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.

******************************************************************************
	File Name     : hi_unf_edid.c
	Version       : Initial Draft
	Author        : Hisilicon TC group
	Created       : 2013/10/08
	Description   :
	History       :
	1.Date        : 2013/10/08
	Author	      : w00217574
	Modification  : Created file

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "hi_mpi_edid.h"


HI_UNF_ENC_FMT_E EDID_GetVideoFormat(EDID_VIDEO_TIMING_S *pstVideoTiming,HI_BOOL *pbNativeFormat)
{
    return HI_ERR_DISP_NOT_SUPPORT;
}

HI_S32 HI_UNF_EDID_EdidParse(HI_U8 *pu8Edid, HI_U32 u32EdidLength,HI_UNF_EDID_INFO_S *pstEdidInfo)
{
	return HI_ERR_DISP_NOT_SUPPORT;
	
   
}

HI_VOID HI_UNF_EDID_EdidRelease(HI_UNF_EDID_INFO_S *pEdidInfo)
{
     
}

