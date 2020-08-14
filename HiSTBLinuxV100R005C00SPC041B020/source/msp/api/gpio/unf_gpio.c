/***********************************************************************************
*              Copyright 2008 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName:         hi_unf_gpio.c
* Description:      supply the api for userspace application
* History:          NULL
* Version           Date           Author     DefectNum    Description
* 1.1               2009-04-14      s00136582   NULL        first draft
*
***********************************************************************************/
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <pthread.h>

#include "hi_unf_gpio.h"
#include "hi_mpi_mem.h"
#include "drv_gpio_ioctl.h"
#include "hi_drv_struct.h"

static HI_S32 g_GpioDrvFd = -1;
static HI_S32 g_GpioOpen = 0;

#define DEVNAME_GPIO "hi_gpio"
static pthread_mutex_t g_GpioMutex = PTHREAD_MUTEX_INITIALIZER;

#define HI_GPIO_LOCK() (void)pthread_mutex_lock(&g_GpioMutex);
#define HI_GPIO_UNLOCK() (void)pthread_mutex_unlock(&g_GpioMutex);


static GPIO_GET_GPIONUM_S g_GpioNum;

HI_S32 HI_UNF_GPIO_Init(HI_VOID)
{
    HI_S32 Ret;

    HI_GPIO_LOCK();

    if (g_GpioDrvFd > 0) {
        HI_GPIO_UNLOCK();
        return HI_SUCCESS;
    }

    g_GpioDrvFd = open("/dev/"UMAP_DEVNAME_GPIO, O_RDWR, 0);
    if (g_GpioDrvFd < 0) {
        HI_ERR_GPIO("open GPIO device failed.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_OPEN_ERR;
    }

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_GET_GPIONUM, &g_GpioNum);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_GPIO("ioctl CMD_GPIO_GET_CHIPTYPE failed.\n");
        close(g_GpioDrvFd);
        g_GpioDrvFd = -1;
        HI_GPIO_UNLOCK();
        return HI_FAILURE;
    }

    g_GpioOpen++;

    HI_GPIO_UNLOCK();

    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_DeInit(HI_VOID)
{
    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0) {
        HI_GPIO_UNLOCK();
        return HI_SUCCESS;
    }

    g_GpioOpen--;

    if (0 == g_GpioOpen) {
        close(g_GpioDrvFd);
        g_GpioDrvFd = -1;
    }

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_SetDirBit(HI_U32 u32GpioNo, HI_BOOL bInput)
{
    GPIO_DIRECTION_S GpioDir;
    HI_S32 Ret;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((bInput != HI_TRUE)
        && (bInput != HI_FALSE)
    )
    {
        HI_ERR_GPIO("para bInput is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();
    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioDir.u32GpioNo = u32GpioNo;
    GpioDir.bInput= bInput;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_SET_DIRECTION, &GpioDir);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETDIRECT;
    }

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_GetDirBit(HI_U32 u32GpioNo, HI_BOOL  *pbInput)
{
    GPIO_DIRECTION_S GpioDir;
    HI_S32 Ret;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (!pbInput)
    {
        HI_ERR_GPIO("para pbInput is null.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioDir.u32GpioNo = u32GpioNo;
    
    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_GET_DIRECTION, &GpioDir);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETDIRECT;
    }
    *pbInput = GpioDir.bInput;

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_WriteBit(HI_U32 u32GpioNo, HI_BOOL bHighVolt)
{
    GPIO_VALUE_S GpioVal;
    HI_S32 Ret;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((bHighVolt != HI_TRUE)
        && (bHighVolt != HI_FALSE)
    ) {
        HI_ERR_GPIO("para bHighVolt is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioVal.u32GpioNo = u32GpioNo;
    GpioVal.bSet = bHighVolt;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_WRITE_BIT, &GpioVal);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETVALUE;
    }

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_ReadBit(HI_U32 u32GpioNo, HI_BOOL *pbHighVolt)
{
    GPIO_VALUE_S GpioVal;
    HI_S32 Ret;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (!pbHighVolt)
    {
        HI_ERR_GPIO("para pbHighVolt is null.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioVal.u32GpioNo = u32GpioNo;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_READ_BIT, &GpioVal);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETVALUE;
    }

    *pbHighVolt = GpioVal.bSet;
    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_SetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  enOutputType)
{
    HI_S32 Ret = HI_SUCCESS;
    GPIO_OUTPUT_TYPE_S stOutputType;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (enOutputType >= HI_UNF_GPIO_OUTPUTTYPE_BUTT)
    {
        HI_ERR_GPIO("para enOutputType is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    stOutputType.u32GpioNo = u32GpioNo;
    stOutputType.enOutputType = enOutputType;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_SET_OUTPUTTYPE, &stOutputType);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETOUTPUTTYPE;
    }

    HI_GPIO_UNLOCK();

    return Ret;
}

HI_S32 HI_UNF_GPIO_GetOutputType(HI_U32 u32GpioNo, HI_UNF_GPIO_OUTPUTTYPE_E  *penOutputType)
{
    HI_S32 Ret = HI_SUCCESS;
    GPIO_OUTPUT_TYPE_S stOutputType;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (penOutputType == HI_NULL)
    {
        HI_ERR_GPIO("para penOutputType is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO is not open.\n");
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    stOutputType.u32GpioNo = u32GpioNo;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_GET_OUTPUTTYPE, &stOutputType);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_GETOUTPUTTYPE;
    }

    *penOutputType = stOutputType.enOutputType;

    HI_GPIO_UNLOCK();

    return Ret;
}

HI_S32 HI_UNF_GPIO_SetIntType(HI_U32 u32GpioNo, HI_UNF_GPIO_INTTYPE_E enIntType)
{
    HI_S32 Ret = HI_SUCCESS;
    GPIO_DATA_S GpioIntType;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if (enIntType >= HI_UNF_GPIO_INTTYPE_BUTT)
    {
        HI_ERR_GPIO("para enIntType is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO dev has not open. fd = %d \n", g_GpioDrvFd);
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioIntType.u32GpioNo = u32GpioNo;
    GpioIntType.enIntType = enIntType;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_SET_INT_TYPE, &GpioIntType);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_GPIO("set gpio interruput type ioctl failed .ret = %x \n", Ret);
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETINT;
    }

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_SetIntEnable(HI_U32 u32GpioNo, HI_BOOL bEnable)
{
    HI_S32 Ret = HI_SUCCESS;
    GPIO_DATA_S GpioIntEnable;

    if (u32GpioNo >= g_GpioNum.u8GpioMaxNum)
    {
        HI_ERR_GPIO("para u32GpioNo is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    if ((bEnable != HI_TRUE) && (bEnable != HI_FALSE))
    {
        HI_ERR_GPIO("para bEnable is invalid.\n");
        return HI_ERR_GPIO_INVALID_PARA;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO dev has not open. fd = %d \n", g_GpioDrvFd);
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioIntEnable.u32GpioNo = u32GpioNo;
    GpioIntEnable.bEnable = bEnable;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_SET_INT_ENABLE, &GpioIntEnable);
    if (HI_SUCCESS != Ret)
    {
        HI_ERR_GPIO("set gpio enabel or disabel ioctl failed .ret = %x \n", Ret);
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_SETENABLE;
    }

    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}

HI_S32 HI_UNF_GPIO_QueryInt(HI_U32 *pu32GpioNo, HI_U32 u32TimeoutMs)
{
    HI_S32 Ret = HI_SUCCESS;
    GPIO_INT_S GpioIntValue;

    if (HI_NULL == pu32GpioNo)
    {
        HI_ERR_GPIO("para pu32GpioNo is NULL pointer.\n");
        return HI_ERR_GPIO_NULL_PTR;
    }

    HI_GPIO_LOCK();

    if (g_GpioDrvFd < 0)
    {
        HI_ERR_GPIO("GPIO dev has not open. fd = %d \n", g_GpioDrvFd);
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_NOT_INIT;
    }

    GpioIntValue.u32TimeoutMs = u32TimeoutMs;

    Ret = ioctl(g_GpioDrvFd, CMD_GPIO_GET_INT, &GpioIntValue);
    if (HI_SUCCESS != Ret)
    {
        HI_GPIO_UNLOCK();
        return HI_ERR_GPIO_FAILED_GETINT;
    }

    *pu32GpioNo = GpioIntValue.u32GpioNo;
    HI_GPIO_UNLOCK();
    return HI_SUCCESS;
}
