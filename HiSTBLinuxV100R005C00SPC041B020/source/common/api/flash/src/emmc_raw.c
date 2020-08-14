/***********************************************************
*                      Copyright    , 2009-2014, Hisilicon Tech. Co., Ltd.
*                                   ALL RIGHTS RESERVED
* FileName:  emmc_raw.c
* Description: emmc flash read and write module
*
* History:
* Version   Date           Author            DefectNum      Description
* main\1    2011-09-20     lidongxiang           NULL       Create this file.
************************************************************/
#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "hi_flash.h"
#include "nand.h"
#include "emmc_raw.h"
#include "hi_debug.h"

EMMC_FLASH_S g_stEmmcFlash;

#define EMMC_RAW_AREA_START 0//EMMC_SECTOR_SIZE

extern int cmdline_parts_init(char *bootargs);
extern HI_S32 get_part_info(HI_U8 partindex, HI_U64 *start, HI_U64 *size);

HI_S32 find_part_from_devname(char *media_name, char *bootargs, 
			      char *devname, HI_U64 *start, HI_U64 *size)
{
	HI_U8 partnum = 0;
	char *tmp;

	if (!(tmp = strstr(bootargs, "blkdevparts=")))
		return HI_FAILURE;
	tmp += strlen("blkdevparts=");

	if (!strstr(bootargs, media_name))
		return HI_FAILURE;

	if (!(tmp = strstr(devname, "mmcblk0p")))
		return HI_FAILURE;
	tmp += strlen("mmcblk0p");
	partnum = (HI_U8)strtol(tmp, NULL, 10);

	if (get_part_info(partnum, start, size))
		return HI_FAILURE;

	return HI_SUCCESS;
}

HI_S32 emmc_raw_init(char *bootargs)
{
    HI_U8  aucBuf[512];
    HI_S32  dev_fd;
    HI_S32  tmp_fd = -1;
    HI_S32 ret = -1;

    if (!bootargs) {
        HI_PRINT("Invalid parameter, bootargs NULL\n");
        return HI_FAILURE;
    }

    ret = cmdline_parts_init(bootargs);
    if (ret < 0)
    {
#ifdef EMMC_RAW_DBG
        HI_PRINT("cmdline parts init failed, ret=%d, bootargs:%s\n", ret, bootargs);
#endif
        return HI_FAILURE;
    }

#if defined (ANDROID)
    if ((dev_fd = open("/dev/block/mmcblk0", O_RDWR)) == -1)
#else
    if ((dev_fd = open("/dev/mmcblk0", O_RDWR)) == -1)
#endif
    {
        HI_PRINT("open mmcblk0 failed, errno=%d\n", errno);
        return HI_FAILURE;
    }

    g_stEmmcFlash.u32EraseSize = EMMC_SECTOR_SIZE;

    memset(aucBuf, 0, sizeof(aucBuf));
    if(((ssize_t)sizeof(aucBuf) - 1) != read(dev_fd, aucBuf, sizeof(aucBuf) - 1))
    {
        HI_PRINT("Failed to read dev, errno=%d\n", errno);
        close(dev_fd);
        return HI_FAILURE;
    }

    close(dev_fd);

    /* Raw area start from 512, after MBR.. */
    g_stEmmcFlash.u64RawAreaStart = EMMC_RAW_AREA_START;
	tmp_fd = open("/sys/block/mmcblk0/size", O_RDONLY);
	if (tmp_fd < 0)
	{
		HI_PRINT("Fail to open the size of mmcblk0\n");
		return HI_FAILURE;
	}
	
	memset(aucBuf, 0, sizeof(aucBuf));
	if (0 > read(tmp_fd, aucBuf, sizeof(aucBuf) - 1))
	{
        HI_PRINT("Failed to read the size of mmcblk0\n");
        close(tmp_fd);
        return HI_FAILURE;
    }
	close(tmp_fd);
	
	g_stEmmcFlash.u64RawAreaSize  = (HI_U64)strtoull((char *)aucBuf, NULL, 10) 
									* EMMC_SECTOR_SIZE - EMMC_RAW_AREA_START;
	
    return HI_SUCCESS;
}

static HI_S32 emmc_flash_probe(void)
{
    int dev;
#if defined (ANDROID)
    if ((dev = open("/dev/block/mmcblk0", O_RDWR | O_SYNC)) == -1)
#else
    if ((dev = open("/dev/mmcblk0", O_RDWR | O_SYNC)) == -1)
#endif
    {
        HI_PRINT("Failed to open device '/dev/mmcblk0'.");
        return HI_FAILURE;
    }

    return (dev);
}

EMMC_CB_S *emmc_raw_open(HI_U64 u64Addr,
                         HI_U64 u64Length)
{
    EMMC_CB_S *pstEmmcCB;
    int fd;

    fd = emmc_flash_probe();
    if( -1 == fd )
    {
        HI_PRINT("no devices available.");
        return NULL;
    }

    /* Reject open, which are not block aligned */
    if ((u64Addr & (g_stEmmcFlash.u32EraseSize - 1))
            || (u64Length & (g_stEmmcFlash.u32EraseSize - 1)))
    {
        HI_PRINT("Attempt to open non block aligned, "
                "eMMC blocksize: 0x%x, address: 0x%08llx, length: 0x%08llx.",
                g_stEmmcFlash.u32EraseSize,
                u64Addr,
                u64Length);
        close(fd);
        return NULL;
    }

    if ((u64Addr > g_stEmmcFlash.u64RawAreaStart + g_stEmmcFlash.u64RawAreaSize)
            || (u64Length > g_stEmmcFlash.u64RawAreaStart + g_stEmmcFlash.u64RawAreaSize)
            || ((u64Addr + u64Length) > g_stEmmcFlash.u64RawAreaStart + g_stEmmcFlash.u64RawAreaSize))
    {
        HI_PRINT("Attempt to open outside the flash area, "
                "eMMC chipsize: 0x%08llx, address: 0x%08llx, length: 0x%08llx\n",
                g_stEmmcFlash.u64RawAreaStart + g_stEmmcFlash.u64RawAreaSize,
                u64Addr,
                u64Length);
        close(fd);
        return NULL;
    }

    if ((pstEmmcCB = (EMMC_CB_S *)malloc(sizeof(EMMC_CB_S))) == NULL)
    {
        HI_PRINT("no many memory.");
        close(fd);
        return NULL;
    }

    pstEmmcCB->u64Address  = u64Addr;
    pstEmmcCB->u64PartSize = u64Length;
    pstEmmcCB->u32EraseSize = g_stEmmcFlash.u32EraseSize;
    pstEmmcCB->fd          = fd;
    pstEmmcCB->enPartType  = EMMC_PART_TYPE_RAW;

    return pstEmmcCB;
}

EMMC_CB_S *emmc_node_open(const HI_U8 *pu8Node)
{
    EMMC_CB_S *pstEmmcCB;
    HI_S32    fd;

    if( NULL == pu8Node )
    {
        return NULL;
    }

    if (-1 == (fd = open((const char*)pu8Node, O_RDWR)))
    {
        HI_PRINT("no devices available.");
        return NULL;
    }

    if( NULL == (pstEmmcCB = (EMMC_CB_S *)malloc(sizeof(EMMC_CB_S))))
    {
        HI_PRINT("No enough space.");
        close(fd);
        return NULL;
    }

    pstEmmcCB->u64Address   = 0;
    pstEmmcCB->u64PartSize  = 0;
    pstEmmcCB->u32EraseSize = g_stEmmcFlash.u32EraseSize;
    pstEmmcCB->fd         = fd;
    pstEmmcCB->enPartType = EMMC_PART_TYPE_LOGIC;

    return pstEmmcCB;
}

HI_S32 emmc_block_read(HI_S32 fd,
                              HI_U64 u64Start,
                              HI_U32 u32Len,
                              void *buff)
{
    HI_S32 s32Ret = HI_FAILURE;
    ssize_t length;
    if( -1 == lseek64(fd, (off64_t)u64Start, SEEK_SET))
    {
        HI_PRINT("Failed to lseek64.");
        return s32Ret;
    }

    length = read(fd, buff, u32Len);
    if (length < 0)
        return s32Ret;

    return (HI_S32)length;
}

HI_S32 emmc_block_write(HI_S32 fd,
                               HI_U64 u64Start,
                               HI_U32 u32Len,
                               const void *buff)
{
    HI_S32 s32Ret;
    if( -1 == lseek64(fd, (off64_t)u64Start, SEEK_SET))
    {
        HI_PRINT("Failed to lseek64.");
        return HI_FAILURE;
    }

    s32Ret = write(fd, buff, u32Len);
    if (s32Ret < 0)
        return HI_FAILURE;

    return s32Ret;
}

HI_S32 emmc_raw_read(const EMMC_CB_S *pstEmmcCB,
                     HI_U64    u64Offset,    /* should be alignment with emmc block size */
                     HI_U32    u32Length,    /* should be alignment with emmc block size */
                     HI_U8     *buf)
{
    HI_S32 S32Ret;
    HI_U64 u64Start;

    if( NULL == pstEmmcCB || NULL == buf)
    {
        HI_PRINT("Pointer is null.");
        return HI_FAILURE;
    }

    /* Reject read, which are not block aligned */
    if( EMMC_PART_TYPE_RAW == pstEmmcCB->enPartType )
    {
        if ((u64Offset > pstEmmcCB->u64PartSize)
                || (u32Length > pstEmmcCB->u64PartSize)
                || ((u64Offset + u32Length) > pstEmmcCB->u64PartSize))
        {
            HI_PRINT("Attempt to write outside the flash handle area, "
                    "eMMC part size: 0x%08llx, offset: 0x%08llx, "
                    "length: 0x%08x.",
                    pstEmmcCB->u64PartSize,
                    u64Offset,
                    u32Length);

            return HI_FAILURE;
        }
    }

    u64Start = pstEmmcCB->u64Address + u64Offset;
    S32Ret = emmc_block_read(pstEmmcCB->fd, u64Start, u32Length, buf);
    return S32Ret;
}

HI_S32 emmc_raw_write(const EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,    /* should be alignment with emmc block size */
                      HI_U32    u32Length,    /* should be alignment with emmc block size */
                      const HI_U8     *buf)
{
    HI_S32 S32Ret;
    HI_U64 u64Start;

    if( NULL == pstEmmcCB || NULL == buf)
    {
        HI_PRINT("Pointer is null.");
        return HI_FAILURE;
    }

    if( EMMC_PART_TYPE_RAW == pstEmmcCB->enPartType )
    {
        if ((u64Offset > pstEmmcCB->u64PartSize)
                || (u32Length > pstEmmcCB->u64PartSize)
                || ((u64Offset + u32Length) > pstEmmcCB->u64PartSize))
        {
            HI_PRINT("Attempt to write outside the flash handle area, "
                    "eMMC part size: 0x%08llx, offset: 0x%08llx, "
                    "length: 0x%08x\n",
                    pstEmmcCB->u64PartSize,
                    u64Offset,
                    u32Length);

            return HI_FAILURE;
        }
    }

    u64Start = pstEmmcCB->u64Address + u64Offset;
    S32Ret = emmc_block_write(pstEmmcCB->fd, u64Start, u32Length, buf);
    return S32Ret;
}

HI_S32 emmc_raw_close(EMMC_CB_S *pstEmmcCB)
{
    if( NULL == pstEmmcCB )
    {
        HI_PRINT("Pointer is null.");
        return HI_FAILURE;
    }

    close(pstEmmcCB->fd);
    free(pstEmmcCB);
    pstEmmcCB = NULL;

    return HI_SUCCESS;
}

#if defined (HI_EMC_ERASE_SUPPORT)
HI_S32 emc_raw_erase(EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,
                      HI_U64    u64Length)
{
    HI_S32 S32Ret;
    HI_U64 u64Start;
    struct mmc_erase_cmd cmd;
    
    if( NULL == pstEmmcCB )
    {
        HI_PRINT("Pointer is null.");
        return HI_FAILURE;
    }
    if( EMMC_PART_TYPE_RAW == pstEmmcCB->enPartType )
    {
        if ((u64Offset > pstEmmcCB->u64PartSize)
                || (u64Length > pstEmmcCB->u64PartSize)
                || ((u64Offset + u64Length) > pstEmmcCB->u64PartSize))
        {
            HI_PRINT("Attempt to write outside the flash handle area, "
                    "eMMC part size: 0x%08llx, offset: 0x%08llx, "
                    "length: 0x%08llx\n",
                    pstEmmcCB->u64PartSize,
                    u64Offset,
                    u64Length);

            return HI_FAILURE;
        }
    }

    u64Start = pstEmmcCB->u64Address + u64Offset;
    //HI_PRINT("[%s][%d][%s] u64Offset=0x%08llx, u32Length=0x%08llx, u64Start=0x%08llx, fd=%d \n", __FILE__, __LINE__, __FUNCTION__, u64Offset, u64Length, u64Start, pstEmmcCB->fd);
	memset(&cmd, 0, sizeof(cmd));

	cmd.from = (unsigned int)(u64Start >> 9);
	cmd.nr   = (unsigned int)(u64Length >> 9);
	cmd.arg = 0x01;
   // HI_PRINT("from=0x%x, nr=0x%x\n", cmd.from, cmd.nr);
	S32Ret = ioctl(pstEmmcCB->fd, MMC_ERASE_CMD, &cmd);
	if (S32Ret)
		HI_PRINT("ioctl: ret=%d\n", S32Ret);

    return S32Ret;
}
#endif

