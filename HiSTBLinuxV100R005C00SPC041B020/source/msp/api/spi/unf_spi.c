/***********************************************************************************
 *              Copyright 2008 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:         unf_spi.c
 * Description:      supply the api for userspace application
 * History:          NULL
 * Version           Date           Author     DefectNum    Description
 * 1.1               2014-05-05   h001279   NULL        first draft
 ***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <malloc.h>
#include <string.h>
#include <semaphore.h>

#include "hi_drv_struct.h"
#include "hi_drv_spi.h"
#include "hi_osal.h"
#include "hi_unf_spi.h"
#include "drv_spi_ioctl.h"
#include "hi_error_mpi.h"

static pthread_mutex_t g_SpiMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_SPI_LOCK()   (void)pthread_mutex_lock(&g_SpiMutex);
#define HI_SPI_UNLOCK() (void)pthread_mutex_unlock(&g_SpiMutex);

#define CHECK_SPI_OPEN() \
    do {\
        HI_SPI_LOCK(); \
        if (g_SpiDevFd < 0)\
        {\
            HI_ERR_SPI("SPI is not open.\n"); \
            HI_SPI_UNLOCK(); \
            return HI_ERR_SPI_NOT_INIT; \
        } \
        HI_SPI_UNLOCK(); \
    } while (0)


#define CHECK_SPI_NULL_PTR(ptr) \
    do {\
        if (NULL == (ptr))\
        {\
            HI_ERR_SPI("PTR('%s') is NULL.\n", # ptr); \
            return HI_ERR_SPI_NULL_PTR; \
        } \
    } while (0)

static HI_S32 g_SpiDevFd = -1;


//#define SPI_USE_INTERRUPT

static HI_S32 SPI_CheckAttr(const HI_UNF_SPI_ATTR_S *pstAttr)
{
    CHECK_SPI_NULL_PTR(pstAttr);

    if (pstAttr->enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", pstAttr->enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }

    if (pstAttr->enFrf >= HI_UNF_SPI_FRF_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", pstAttr->enFrf);
        return HI_ERR_SPI_INVALID_PARA;
    }

    if ((pstAttr->enBigend != HI_UNF_SPI_BIGEND_LITTLE)&&(pstAttr->enBigend != HI_UNF_SPI_BIGEND_BIG))
    {
        HI_ERR_SPI("invalid para enBigend %d\n", pstAttr->enBigend);
        return HI_ERR_SPI_INVALID_PARA;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_SPI_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 HI_UNF_SPI_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

/** Open spi bus device. */
/** CNcomment:打开时指定 spi 设备*/
HI_S32 HI_UNF_SPI_Open(HI_UNF_SPI_DEV_E enDev)
{
    HI_S32 Ret = 0;
    HI_SPI_LOCK();

    if (g_SpiDevFd > 0)
    {
        HI_SPI_UNLOCK();
        return HI_SUCCESS;
    }

    g_SpiDevFd = open("/dev/" UMAP_DEVNAME_SPI, O_RDWR, 0);
    if (g_SpiDevFd < 0)
    {
        HI_FATAL_SPI("open spi err\n");
        HI_SPI_UNLOCK();
        return HI_ERR_SPI_OPEN_ERR;
    }
	
    Ret = ioctl(g_SpiDevFd, CMD_SPI_OPEN, enDev);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SPI("open spi err.0x%x\n", Ret);
        HI_SPI_UNLOCK();
        return HI_ERR_SPI_OPEN_ERR;
    }

    HI_SPI_UNLOCK();
    return Ret;
}


/** Close spi bus device*/
/** CNcomment:关闭时指定 spi 设备 */
HI_S32 HI_UNF_SPI_Close(HI_UNF_SPI_DEV_E enDev)
{
    HI_S32 Ret = HI_SUCCESS;

    HI_SPI_LOCK();

    if (g_SpiDevFd < 0)
    {
        HI_SPI_UNLOCK();
        return HI_SUCCESS;
    }

    Ret = ioctl(g_SpiDevFd, CMD_SPI_CLOSE,enDev);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SPI("Close SPI FAILED.\n");
        HI_SPI_UNLOCK();
        return HI_ERR_SPI_CLOSE_ERR;
    }

    Ret = close(g_SpiDevFd);
    if (HI_SUCCESS != Ret)
    {
        HI_FATAL_SPI("Close SPI err.\n");
        HI_SPI_UNLOCK();
        return HI_ERR_SPI_CLOSE_ERR;
    }

    g_SpiDevFd = -1;

    HI_SPI_UNLOCK();

    return HI_SUCCESS;
}

/** Set attribute on a spi bus device, input param enDev and stAttr */
/** CNcomment:设置属性，输入handle、attr */
HI_S32 HI_UNF_SPI_SetAttr(HI_UNF_SPI_DEV_E enDev, HI_UNF_SPI_ATTR_S *pstAttr)
{
    HI_S32 Ret = 0;
    SPI_BLEND_S setBigEnd;
    HI_UNF_SPI_ATTR_S tmpAttr;
    SPI_FFORM_S SPI_FFORM;

    if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }

    Ret = SPI_CheckAttr(pstAttr);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SPI("invalid para \n");
        return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_OPEN();

    tmpAttr=*pstAttr;
    SPI_FFORM.devId=enDev;
	SPI_FFORM.mode=tmpAttr.enFrf;
    SPI_FFORM.spo=tmpAttr.unExtAttr.stMoto.enSpo;
    SPI_FFORM.sph=tmpAttr.unExtAttr.stMoto.enSph;
    SPI_FFORM.dss=tmpAttr.u32Dss;
	SPI_FFORM.cscfg=tmpAttr.csCfg;
    setBigEnd.devId=enDev;
    setBigEnd.setbend=tmpAttr.enBigend;

    /*Frame And Dss*/
    Ret = ioctl(g_SpiDevFd, CMD_SPI_SET_ATTR, &SPI_FFORM);

    /*Big-Little End*/
    Ret |= ioctl(g_SpiDevFd, CMD_SPI_SET_BLEND, &setBigEnd);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SPI("SET ATTR FAILED.\n");
        return HI_FAILURE;
    }

    return Ret;

}

/** Get attribute on a spi bus device, input param enDev, output stAttr */
/** CNcomment:获取属性，输入handle，输出attr */
HI_S32 HI_UNF_SPI_GetAttr(HI_UNF_SPI_DEV_E enDev, HI_UNF_SPI_ATTR_S *pstAttr)
{
    HI_S32 Ret = 0;
    SPI_FFORM_S SPI_FFORM;
    SPI_BLEND_S getBigEnd;

    if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_NULL_PTR(pstAttr);
    CHECK_SPI_OPEN();

    SPI_FFORM.devId=enDev;
    getBigEnd.devId=enDev;
    Ret = ioctl(g_SpiDevFd, CMD_SPI_GET_ATTR, &SPI_FFORM);

    pstAttr->enDev = enDev;
    pstAttr->enFrf = SPI_FFORM.mode;
    pstAttr->unExtAttr.stMoto.enSpo = SPI_FFORM.spo;
    pstAttr->unExtAttr.stMoto.enSph = SPI_FFORM.sph;
    pstAttr->u32Dss = SPI_FFORM.dss;
	pstAttr->csCfg = SPI_FFORM.cscfg;
    Ret |= ioctl(g_SpiDevFd, CMD_SPI_GET_BLEND, &getBigEnd);
    pstAttr->enBigend= getBigEnd.setbend;
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_SPI("GET ATTR FAILED.\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

/**
 * Read data from a spi device. When read data in maybe send some command or data out first.
 * The data sent out is stored in pu8Send，data size is set by u32ReadCnt.
 * The data read out is stored in pu8Read, how many data want to be read out is set by u32ReadCnt.
 */
/** CNcomment:数据接收 */

HI_S32 HI_UNF_SPI_ReadExt(HI_UNF_SPI_DEV_E enDev, HI_U8 *pu8Send, HI_U32 u32SendCnt, HI_U8 *pu8Read, HI_U32 u32ReadCnt)
{
	HI_S32 ret;
	SPI_DATAEX_S DATA;
	
	if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
		return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_NULL_PTR(pu8Send);
	CHECK_SPI_NULL_PTR(pu8Read);
	CHECK_SPI_OPEN();
	
	DATA.devId=enDev;
	DATA.sData=pu8Send;
	DATA.sDataCnt=u32SendCnt;
	DATA.rData=pu8Read;
	DATA.rDataCnt=u32ReadCnt;

	ret = ioctl(g_SpiDevFd, CMD_SPI_READEX, &DATA);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SPI("read err.\n");
        return ret;
    }
	return HI_SUCCESS;
	
}


/**
 * Read data from a spi device. When read data in maybe send some command or data out first.
 * The data sent out is stored in pu8Send，data size is set by u32ReadCnt.
 * The data read out is stored in pu8Read, how many data want to be read out is set by u32ReadCnt.
 */
/** CNcomment:数据接收 */
HI_S32 HI_UNF_SPI_Read(HI_UNF_SPI_DEV_E enDev, HI_U8 *pu8Read, HI_U32 u32ReadCnt)
{
    HI_S32 ret;
    SPI_DATA_S SDATA;

    if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_NULL_PTR(pu8Read);
    CHECK_SPI_OPEN();

    SDATA.devId = enDev;
    SDATA.sData = pu8Read;
    SDATA.sDataCnt = u32ReadCnt;
    ret = ioctl(g_SpiDevFd, CMD_SPI_READ, &SDATA);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SPI("read err.\n");
        return ret;
    }
    return HI_SUCCESS;

}

/** Data transimit out through a spi bus device.
 * Data sent out is stored in pu8Send, u32SendCnt set how many data want to be sent out.
 */
/** CNcomment:数据发送 */
HI_S32 HI_UNF_SPI_Write(HI_UNF_SPI_DEV_E enDev, HI_U8 *pu8Send, HI_U32 u32SendCnt)
{
    HI_S32 ret;
    SPI_DATA_S SDATA;

    if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_NULL_PTR(pu8Send);
    CHECK_SPI_OPEN();

    SDATA.devId=enDev;
    SDATA.sData=pu8Send;
    SDATA.sDataCnt=u32SendCnt;

    ret=ioctl(g_SpiDevFd, CMD_SPI_WRITE, &SDATA);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SPI("write err.\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 HI_UNF_SPI_SetLoop(HI_UNF_SPI_DEV_E enDev, HI_U8 bSet)
{
    HI_S32 ret;
    SPI_LOOP_S SDATA;

    if (enDev >= HI_UNF_SPI_DEV_BUTT) {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
        return HI_ERR_SPI_INVALID_PARA;
    }
    CHECK_SPI_OPEN();

    SDATA.devId=enDev;
    SDATA.setLoop=bSet;

    ret=ioctl(g_SpiDevFd, CMD_SPI_SET_LOOP, &SDATA);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SPI("setloop err:%d\n", ret);
        return HI_FAILURE;
    }
    return HI_SUCCESS;    
}

HI_S32 HI_UNF_SPI_RW_Loop(HI_UNF_SPI_DEV_E enDev, HI_U8 *pu8Send, HI_U32 u32SendCnt, HI_U8 *pu8Read, HI_U32 u32ReadCnt)
{
    HI_S32 ret;
	SPI_DATAEX_S DATA;
	
	if (enDev >= HI_UNF_SPI_DEV_BUTT)
    {
        HI_ERR_SPI("invalid para enDev %d\n", enDev);
		return HI_ERR_SPI_INVALID_PARA;
    }

    CHECK_SPI_NULL_PTR(pu8Send);
	CHECK_SPI_NULL_PTR(pu8Read);
	CHECK_SPI_OPEN();
	
	DATA.devId=enDev;
	DATA.sData=pu8Send;
	DATA.sDataCnt=u32SendCnt;
	DATA.rData=pu8Read;
	DATA.rDataCnt=u32ReadCnt;

	ret = ioctl(g_SpiDevFd, CMD_SPI_RW_LOOP, &DATA);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_SPI("read err.\n");
        return ret;
    }
	return HI_SUCCESS;
}

