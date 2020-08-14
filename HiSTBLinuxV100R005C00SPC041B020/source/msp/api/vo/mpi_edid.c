/******************************************************************************

   Copyright (C), 2001-2012, Hisilicon Tech. Co., Ltd.

******************************************************************************
	File Name     : mpi_edid.c
	Version       : Initial Draft
	Author        : Hisilicon TC group
	Created       : 2012/05/26
	Description   :
	History       :
	1.1Date       : 2012/07/16
	Author	      : l214567
	Modification  : Created file

*******************************************************************************/
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

#include "list.h"
//#include "priv_i2c.h"
#include "hi_unf_common.h"
#include "mpi_priv_edid.h"
#include "hi_unf_hdmi.h"
#include "hi_mpi_edid.h"

HI_VOID HI_MPI_EDID_EdidRelease(EDID_INFO_S* pstEdidInfo)
{
    return ;
}

HI_S32 HI_MPI_EDID_EdidParse(HI_U8* u8Edid, HI_U32 u32EdidLength, EDID_INFO_S* pstEdidInfo)
{
    return HI_ERR_DISP_NOT_SUPPORT;
}
//TC MODIFY END

