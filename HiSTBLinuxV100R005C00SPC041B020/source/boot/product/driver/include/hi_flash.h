#ifndef __HI_ADP_FLASH__H__
#define __HI_ADP_FLASH__H__
#include "hi_type.h"

#define MAX_PARTS  32           /**<Max Num of Flash Partition*/
#define MAX_HANDLE 32           /**<Max Num of Flash Handle*/
#define FLASH_NAME_LEN 32       /**<Max Length of Flash Name*/

/** */
typedef enum hiHI_FLASH_TYPE_E
{
	HI_FLASH_TYPE_SPI_0,
	HI_FLASH_TYPE_NAND_0,
	HI_FLASH_TYPE_EMMC_0,
	HI_FLASH_TYPE_SD,

	HI_FLASH_TYPE_BUTT
} HI_FLASH_TYPE_E;

#define  INVALID_FD -1
#define  SPAN_PART_HANDLE 1000

#define  HI_FLASH_RW_FLAG_RAW           0x0   /* read/write without oob, such as write kernel/uboot/ubi/cramfs.. */
#define  HI_FLASH_RW_FLAG_WITH_OOB      0x1   /* read/write nand with oob, such as write yaffs2 filesystem image */
#define  HI_FLASH_RW_FLAG_ERASE_FIRST   0x2   /* erase flash before write */

/* sometimes, the partition has bad blocks, limit the length in partsize, if the address access the limit, return this value */
#define  HI_FLASH_END_DUETO_BADBLOCK    -10

typedef struct hiFlash_PartInfo_S
{
    HI_U64  StartAddr;                  /**<Flash Partition start Address*/
    HI_U64  PartSize;
    HI_U32  BlockSize;
    HI_FLASH_TYPE_E FlashType;
    HI_CHAR DevName[FLASH_NAME_LEN];
    HI_CHAR PartName[FLASH_NAME_LEN];
} HI_Flash_PartInfo_S;

typedef struct tagFLASH_OPT_S
{
    int (*raw_read)(int fd, unsigned long long *startaddr, unsigned char *buffer,
        unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int read_oob, int skip_badblock);
    int (*raw_write)(int fd, unsigned long long *startaddr, unsigned char *buffer,
        unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int write_oob);
    int (*raw_erase)(int fd, unsigned long long startaddr,
        unsigned long long length, unsigned long long openaddr, unsigned long long limit_leng);
} FLASH_OPT_S;

/** Flash Infomation */
typedef struct hiFlash_InterInfo_S
{
    HI_U64  TotalSize;                  /**<Flash Partition Total Size*/
    HI_U64  PartSize;                   /**<Flash Partition Size*/
    HI_U32  BlockSize;                  /**<One Block Size*/
    HI_U32  PageSize;                   /**<One Page Size*/
    HI_U32  OobSize;                    /**<One Oob Size*/
    HI_S32  fd;                         /**<file Handle*/
    HI_U64  OpenAddr;
    HI_U64  OpenLeng;
    HI_FLASH_TYPE_E FlashType;
    FLASH_OPT_S *pFlashopt;
    HI_Flash_PartInfo_S *pPartInfo;
} HI_Flash_InterInfo_S;


#if 0
#define HI_ERR_FLASH(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_ERROR, HI_DEBUG_ID_USR, fmt)

#define HI_INFO_FLASH(fmt...) \
            HI_TRACE(HI_LOG_LEVEL_INFO, HI_DEBUG_ID_USR, fmt)
#else
#define HI_ERR_FLASH(fmt...)
#define HI_INFO_FLASH(fmt...)
#endif


/**
\brief: Open FLASH Partition
\attention \n
\param[in] enFlashType      flash type(when the flash type is HI_FLASH_TYPE_BUTT,the flash name must not NULL)
\param[in] pPartitionName   flash name
\param[in] u64Address       Open Address(this address is valid when pPartitionName=NULL)
\param[in] u64Len           Open Length(this address is valid when pPartitionName=NULL)
\retval    fd               Success
\retval    INVALID_FD       Fail
\see \n
No
*/
HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len);

/**
\brief: Open FLASH Partition
\attention \n
\param[in] pPartitionName   flash name
\retval    fd               Success
\retval    INVALID_FD       Fail
\see \n
No
*/
HI_HANDLE HI_Flash_OpenByName(HI_CHAR *pPartitionName);

/**
\brief: Open FLASH Partition
\attention \n
\param[in] enFlashType      flash type
\param[in] pPartitionName   flash name
\retval    fd               Success
\retval    INVALID_FD       Fail
\see \n
No
*/
HI_HANDLE HI_Flash_OpenByTypeAndName(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName);

/**
\brief: Open FLASH Partition
\attention \n
\param[in] enFlashType      flash type
\param[in] u64Address       Open Address(this address is valid when pPartitionName=NULL)
\param[in] u64Len           Open Length(this address is valid when pPartitionName=NULL)
\retval    fd               Success
\retval    INVALID_FD       Fail
\see \n
No
*/
HI_HANDLE HI_Flash_OpenByTypeAndAddr(HI_FLASH_TYPE_E enFlashType, HI_U64 u64Address, HI_U64 u64Len);


/**
\brief: Close FLASH Partition
\attention \n
\param[in] hFlash    Flash Handle
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
No
*/
HI_S32 HI_Flash_Close(HI_HANDLE hFlash);

/**
\brief: Erase FLASH Partition
\attention \n
\param[in] hFlash     Flash Handle
\param[in] u64Address Erase Offset,alignment with blocksize by byte
\param[in] u32Len     Data Length,alignment with blocksize by byte
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
No
*/
HI_S32 HI_Flash_Erase(HI_HANDLE hFlash, HI_U64 u64Address, HI_U64 u64Len);

/**
\brief: Read FLASH Partition(only support erase len smaller than 2GB)
\attention \n
\param[in] hFlash       Flash Handle
\param[in] u64Address   Read Offset,alignment with pagesize by byte
\param[in] pBuf         Buffer Address
\param[in] u32Len       Data Length,alignment with pagesize by byte
\param[in] u32Flags     eg: HI_FLASH_RW_FLAG_RAW/HI_FLASH_RW_FLAG_WITH_OOB
\retval ::erase length
\see \n
No
*/
HI_S64 HI_Flash_Erase64(HI_HANDLE hFlash, HI_U64 u64Address, HI_U64 u64Len);

/**
\brief: Read FLASH Partition(support erase len bigger than 2GB)
\attention \n
\param[in] hFlash       Flash Handle
\param[in] u64Address   Read Offset,alignment with pagesize by byte
\param[in] pBuf         Buffer Address
\param[in] u32Len       Data Length,alignment with pagesize by byte
\param[in] u32Flags     eg: HI_FLASH_RW_FLAG_RAW/HI_FLASH_RW_FLAG_WITH_OOB
\retval ::erase length
\see \n
No
*/

HI_S32 HI_Flash_Read(HI_HANDLE hFlash, HI_U64 u64Address, HI_U8 *pBuf,
                     HI_U32 u32Len, HI_U32 u32Flags);

/**
\brief: Write FLASH Partition
\attention \n
1) can not update the current file-system
2) you could call this function only once to write yaffs, with the parameter of u32Flags as
   HI_FLASH_RW_FLAG_WITH_OOB | HI_FLASH_RW_FLAG_ERASE_FIRST.
   but, if you want to write yaffs block by block, you must erase the flash partition(one whole partition) at first,
   the parameter of u32Flags can only be HI_FLASH_RW_FLAG_WITH_OOB.
\param[in] hFlash     Flash Handle
\param[in] u64Address Write Offset,alignment with pagesize by byte
\param[in] pBuf       Buffer Address
\param[in] u32Len     Data Length,alignment with pagesize by byte
\param[in] u32Flags   eg: HI_FLASH_RW_FLAG_RAW/HI_FLASH_RW_FLAG_WITH_OOB/HI_FLASH_RW_FLAG_ERASE_FIRST
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
No
*/
HI_S32 HI_Flash_Write(HI_HANDLE hFlash, HI_U64 u64Address,
                      HI_U8 *pBuf, HI_U32 u32Len, HI_U32 u32Flags);

/**
\brief: Get FLASH Partition Infomation
\attention \n
can only get infomation include: TotalSize,PartSize,BlockSize,PageSize,OobSize,fd
\param[in] hFlash     Flash Handle
\param[in] pInterInfo Flash Partition Infomation
\retval ::HI_SUCCESS
\retval ::HI_FAILURE
\see \n
No
*/
HI_S32 HI_Flash_GetInfo(HI_HANDLE hFlash, HI_Flash_InterInfo_S *pFlashInfo);


#endif // __HI_ADP_FLASH__H__
