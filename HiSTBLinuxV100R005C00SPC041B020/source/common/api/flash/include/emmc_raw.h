/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*    Create by lidongxiang. 2011-09-21
*
******************************************************************************/
#ifndef __EMMC_RAW_H__
#define __EMMC_RAW_H__

#include "hi_type.h"
#include <linux/types.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define EMMC_EXT_PART_ID       5
#define EMMC_SECTOR_TAIL      (0xAA55)
#define EMMC_SECTOR_SIZE      (512)
#define MMC_BLOCK_MAJOR			179
#define MMC_ERASE_CMD _IOW(MMC_BLOCK_MAJOR, 1, struct mmc_erase_cmd)


typedef enum eMMCPartType_E
{
    EMMC_PART_TYPE_RAW,
    EMMC_PART_TYPE_LOGIC,

    EMMC_PART_TYPE_BUTT
}EMMC_PART_TYPE_E;

typedef struct emmc_flast_s
{
    /* None ext area start address.
     * Absolutely offset from emmc flash start.
     */
    HI_U64 u64RawAreaStart;

    /* None ext area size. In Byte */
    HI_U64 u64RawAreaSize;

    /* Block size. Default is 512B */
    HI_U32 u32EraseSize;
}EMMC_FLASH_S;

typedef struct emmc_cb_s
{
    HI_S32 fd;
    HI_U64 u64Address;
    HI_U64 u64PartSize;
    HI_U32 u32EraseSize;
    EMMC_PART_TYPE_E enPartType;

} EMMC_CB_S;

struct mmc_erase_cmd {
	unsigned int from; /* first sector to erase */
	unsigned int nr;   /* number of sectors to erase */

	/* erase command argument (SD supports only %MMC_ERASE_ARG) */
	unsigned int arg;
};


///////////////////////////////////////////////////
HI_S32 emmc_raw_init(char *bootargs);
EMMC_CB_S *emmc_raw_open(HI_U64 u64Addr,
                         HI_U64 u64Length);
EMMC_CB_S *emmc_node_open(const HI_U8 *pu8Node);

HI_S32 emmc_block_read(HI_S32 fd,
                       HI_U64 u32Start,
                       HI_U32 u32Len,
                       void *buff);

HI_S32 emmc_block_write(HI_S32 fd,
                        HI_U64 u32Start,
                        HI_U32 u32Len,
                        const void *buff);

HI_S32 emmc_raw_read(const EMMC_CB_S *pstEmmcCB,
                     HI_U64    u64Offset,
                     HI_U32    u32Length,
                     HI_U8     *buf);

HI_S32 emmc_raw_write(const EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,
                      HI_U32    u32Length,
                      const HI_U8     *buf);

HI_S32 emmc_raw_close(EMMC_CB_S *pstEmmcCB);
#if defined (HI_EMC_ERASE_SUPPORT)
HI_S32 emc_raw_erase(EMMC_CB_S *pstEmmcCB,
                      HI_U64    u64Offset,
                      HI_U64    u64Length);
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif

