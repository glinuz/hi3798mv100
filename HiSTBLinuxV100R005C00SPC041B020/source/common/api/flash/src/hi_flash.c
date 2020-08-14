#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "hi_debug.h"
#include "hi_flash.h"
#include "nand.h"
#include "spi_raw.h"
#include "nand_raw.h"
#include "emmc_raw.h"
#include "hi_debug.h"

#define MAX_HANDLE        MAX_PARTS    /* Flash max handle number */
#define MAX_BOOTARGS_LEN  1024  /* The max length of bootargs */

/* Expand hiflash handle fd, only HI_Flash_OpenByTypeAndName() use it */
#define  SPAN_PART_HANDLE 1000

#define HI_ERR_FLASH(fmt...) \
             HI_ERR_PRINT(HI_ID_FLASH, fmt)

#define HI_INFO_FLASH(fmt...) \
             HI_INFO_PRINT(HI_ID_FLASH, fmt)

/** Flash Infomation */
typedef struct
{
    HI_U64  TotalSize;                  /**< flash total size */
    HI_U64  PartSize;                   /**< flash partition size */
    HI_U32  BlockSize;                  /**< flash block size */
    HI_U32  PageSize;                   /**< flash page size */
    HI_U32  OobSize;                    /**< flash OOB size */
    HI_VOID *fd;                        /**< file handle */
    HI_U64  OpenAddr;                   /**< flash open address */
    HI_U64  OpenLeng;                   /**< flash open length */
    HI_FLASH_TYPE_E FlashType;          /**< flash type */
    FLASH_OPT_S *pFlashopt;             /**< operation callbacks on this flash */
    HI_Flash_PartInfo_S *pPartInfo;     /**< parition descriptions on this flash */
} HI_FLASH_INTER_INFO_S;

typedef enum hi_flash_dev_stat
{
    HI_FLASH_STAT_INSTALL,
    HI_FLASH_STAT_UNINSTALL,
    /*lint -save -e749 */
    HI_FLASH_STAT_BUTT
}HI_FLASH_DEV_STAT;

extern HI_S32 find_flash_part(char *cmdline_string,
                              const char *media_name,  /* hi_sfc, hinand */
                              char *ptn_name,
                              HI_U64 *start,
                              HI_U64 *length);
extern HI_S32 find_part_from_devname(char *media_name, char *bootargs,
                     char *devname, HI_U64 *start, HI_U64 *size);

static HI_FLASH_INTER_INFO_S gFlashInfo[MAX_HANDLE];
static HI_Flash_PartInfo_S gPartInfo[MAX_PARTS];
static HI_BOOL gInitFlag = HI_FALSE;
static HI_U8 g_au8Bootargs[MAX_BOOTARGS_LEN];
static HI_CHAR szFlashStr[HI_FLASH_TYPE_BUTT][16]={
    "hi_sfc:",
    "hinand:",
    "mmcblk0:"};
static HI_CHAR *pszPos[HI_FLASH_TYPE_BUTT];
static HI_FLASH_DEV_STAT g_eDevStat[HI_FLASH_TYPE_BUTT];
static pthread_mutex_t gFlashMutex;

static FLASH_OPT_S g_stFlashOpt[HI_FLASH_TYPE_BUTT];

extern EMMC_FLASH_S g_stEmmcFlash;

#define CHECK_FLASH_INIT(hFlash) \
    do \
    { \
        if (!gInitFlag) \
        { \
            HI_ERR_FLASH("NOT init yet!\n"); \
            return HI_FAILURE; \
        } \
        if (MAX_HANDLE <= hFlash) \
        { \
            return HI_FAILURE; \
        } \
        if ((HI_VOID *)INVALID_FD == gFlashInfo[hFlash].fd) \
        { \
            return HI_FAILURE; \
        } \
    } while (0)

#define CHECK_ADDR_LEN_VALID(Address, Len, LimitLeng) \
    do \
    { \
        if ((Address >= LimitLeng) || ((Address + Len) > LimitLeng)) \
        { \
            HI_ERR_FLASH("startaddr(0x%llX) + length(0x%x) or startaddr should be smaller than partsize(0x%llX)\n", Address, Len, LimitLeng); \
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex); \
            return HI_FAILURE; \
        } \
    } while (0)

static HI_S8* skip_space(HI_S8* line)
{
    HI_S8* p = line;
    while (*p == ' ' || *p == '\t')
    {
        p++;
    }
    return p;
}

static HI_S8* skip_word(HI_S8* line)
{
    HI_S8* p = line;
    while (*p != '\t' && *p != ' ' && *p != '\n' && *p != 0)
    {
        p++;
    }
    return p;
}

static HI_S8* get_word(HI_S8* line, HI_S8* value)
{
    HI_S8* p = line;
    p = skip_space(p);
    while (*p != '\t' && *p != ' ' && *p != '\n' && *p != 0)
    {
        *value++ = *p++;
    }
    *value = 0;
    return p;
}

static HI_S32 get_bootargs(HI_U8 *pu8Bootarags, HI_U16 u16Len)
{
    FILE *pf;

    if( NULL == pu8Bootarags)
    {
        HI_ERR_FLASH("Pointer is null.\n");
        return HI_FAILURE;
    }

    if(NULL == (pf = fopen("/proc/cmdline", "r")))
    {
        HI_ERR_FLASH("Failed to open '/proc/cmdline'.\n");
        return HI_FAILURE;
    }

    if(NULL == fgets((char*)pu8Bootarags, u16Len, pf))
    {
        HI_ERR_FLASH("Failed to fgets string.\n");
        fclose(pf);
        return HI_FAILURE;
    }

    fclose(pf);
    return HI_SUCCESS;
}

static HI_FLASH_TYPE_E get_flashtype_by_bootargs(HI_CHAR *pszPartitionName)
{
    HI_CHAR *pszPartitionPos = HI_NULL;
    HI_CHAR *pszTmpPos = HI_NULL;
    HI_CHAR pszTmpStr[64];
    HI_U32 i;
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;

    if(HI_NULL == pszPartitionName)
    {
        return (HI_FLASH_TYPE_BUTT);
    }
    memset(pszTmpStr, 0, sizeof(pszTmpStr));
    (HI_VOID)snprintf(pszTmpStr, sizeof(pszTmpStr)-1, "(%s)", pszPartitionName);
    pszPartitionPos = strstr((HI_CHAR *)g_au8Bootargs, pszTmpStr);
    if(HI_NULL == pszPartitionPos)
    {
        return (HI_FLASH_TYPE_BUTT);
    }

    for(i=0; i<HI_FLASH_TYPE_BUTT; i++)
    {
        if(HI_NULL == pszPos[i])
        {
            continue;
        }
        /* pszTmpPos is used to be a cursor */
        /*lint -save -e613 */
        if((pszPartitionPos >= pszPos[i])&&(pszPos[i] >= pszTmpPos))
        {
            enFlashType = (HI_FLASH_TYPE_E) i;
            pszTmpPos = pszPos[i];
        }
    }
    return enFlashType;
}

HI_S32 permission_check(HI_FLASH_TYPE_E enFlashType,
                                     HI_U64 u64StartAddress,
                                     HI_U64 u64Len)
{
    HI_U64 u64EndAddress = u64StartAddress + u64Len -1;
    HI_U32 i;

    for(i=0; i< MAX_PARTS; i++)
    {
        if(gPartInfo[i].FlashType != enFlashType)
            continue;
        if(gPartInfo[i].perm != ACCESS_NONE)
            continue;
        if((gPartInfo[i].StartAddr >= u64StartAddress)&&(gPartInfo[i].StartAddr <= u64EndAddress))
        {
            HI_INFO_FLASH("%s(%s) is not permitted to be opened.\n", gPartInfo[i].DevName, gPartInfo[i].PartName);
            return HI_FAILURE;
        }
    }
    return HI_SUCCESS;
}

static HI_S32 Flash_Init(HI_VOID)
{
    HI_CHAR line[512];
    FILE* fp = HI_NULL;
    HI_U32 i = 0;
    HI_U64 u64StartAddr[HI_FLASH_TYPE_BUTT];
    HI_S32 ret = HI_SUCCESS;

    for(i=0; i< MAX_PARTS; i++)
    {
        gPartInfo[i].StartAddr= 0;
        gPartInfo[i].PartSize= 0;
        gPartInfo[i].BlockSize = 0;
        gPartInfo[i].FlashType = HI_FLASH_TYPE_BUTT;
        gPartInfo[i].perm = ACCESS_BUTT;
        memset(gPartInfo[i].DevName, '\0', FLASH_NAME_LEN);
        memset(gPartInfo[i].PartName, '\0', FLASH_NAME_LEN);
    }

    for(i=0; i< MAX_HANDLE; i++)
    {
	(HI_VOID)pthread_mutex_lock(&gFlashMutex);
        gFlashInfo[i].fd = (HI_VOID *)INVALID_FD;
        gFlashInfo[i].OpenAddr = 0;
        gFlashInfo[i].OpenLeng= 0;
        gFlashInfo[i].pPartInfo = NULL;
        gFlashInfo[i].FlashType = HI_FLASH_TYPE_BUTT;
	(HI_VOID)pthread_mutex_unlock(&gFlashMutex);
    }

    if( (HI_FLASH_STAT_INSTALL == g_eDevStat[HI_FLASH_TYPE_SPI_0])
            || (HI_FLASH_STAT_INSTALL == g_eDevStat[HI_FLASH_TYPE_NAND_0]))
    {

        fp = fopen("/proc/mtd", "r");
        if (fp)
        {
            if (NULL == fgets(line, sizeof(line), fp)) { //skip first line
                fclose(fp);
                return HI_FAILURE;
            }

            HI_INFO_FLASH(" DevName\t  PartSize\tBlockSize   PartName    Startaddr\n");
            for(i=0; i<HI_FLASH_TYPE_BUTT; i++)
            {
                u64StartAddr[i] = 0;
            }
            for(i=0; fgets(line, sizeof(line), fp) != 0; i++)
            {
                HI_S8   argv[4][32];
                HI_S8   *p;
                int fd = -1;

                p = (HI_S8*)&line[0];

                p = skip_space(p);
                p = skip_word(p);
                p = get_word(p, argv[1]);
                p = get_word(p, argv[2]);
                p = get_word(p, argv[3]);
                p = p; /* for no TQE warning */

                if (i >= MAX_PARTS)
                {
                    HI_PRINT("Detected there has more than %d partitions.\n" \
                              "You should encrease MAX_PARTS in order to use left partitions!\n", MAX_PARTS);
                    break;
                }

                gPartInfo[i].PartSize = (HI_U64)(HI_S64)strtoull((const char*)argv[1], (char**)NULL, 16);
                gPartInfo[i].BlockSize = (HI_U64)(HI_S64)strtol((const char*)argv[2], (char**)NULL,16); //erase size
                memset(gPartInfo[i].PartName, 0, sizeof(gPartInfo[i].PartName));
                strncpy(gPartInfo[i].PartName, (char*)(argv[3]+1), (strlen((char*)argv[3])-2));
                gPartInfo[i].FlashType = get_flashtype_by_bootargs(gPartInfo[i].PartName);
                gPartInfo[i].StartAddr = u64StartAddr[gPartInfo[i].FlashType];
                u64StartAddr[gPartInfo[i].FlashType] += gPartInfo[i].PartSize;
#if defined (ANDROID)
                snprintf(gPartInfo[i].DevName, sizeof(gPartInfo[i].DevName), DEV_MTDBASE"%d", i);
#else
                snprintf(gPartInfo[i].DevName, sizeof(gPartInfo[i].DevName), "/dev/mtd%d", i);
#endif
                if ((fd = open(gPartInfo[i].DevName, O_RDWR)) == -1)
                {
                    if ((fd = open(gPartInfo[i].DevName, O_RDONLY)) == -1)
                    {
                        HI_ERR_FLASH("Can't open \"%s\"\n", gPartInfo[i].DevName);
                        //fclose(fp);
                        gPartInfo[i].perm = ACCESS_NONE;
                        continue;
                    }
                    HI_INFO_FLASH("access %s readonly!\n", gPartInfo[i].DevName);
                    gPartInfo[i].perm = ACCESS_RD;
                } else {
                    HI_INFO_FLASH("access %s read and write, i:%d, fd:%x!\n", gPartInfo[i].DevName, i, fd);
                    gPartInfo[i].perm = ACCESS_RDWR;
                }
                close(fd);
            }
        }
        else
        {
            HI_ERR_FLASH("open /proc/mtd file failure!\n");
            return HI_FAILURE;
        }

        g_stFlashOpt[HI_FLASH_TYPE_SPI_0].raw_erase = spi_raw_erase;
        g_stFlashOpt[HI_FLASH_TYPE_SPI_0].raw_read  = spi_raw_read;
        g_stFlashOpt[HI_FLASH_TYPE_SPI_0].raw_write = spi_raw_write;
        g_stFlashOpt[HI_FLASH_TYPE_NAND_0].raw_erase = nand_raw_erase;
        g_stFlashOpt[HI_FLASH_TYPE_NAND_0].raw_read  = nand_raw_read;
        g_stFlashOpt[HI_FLASH_TYPE_NAND_0].raw_write = nand_raw_write;

        if (fclose(fp))
        {
            return HI_FAILURE;
        }
    }

    return ret;
}

static HI_S32 All_FLash_Init()
{
    HI_U8 u8Loop;

    if (!gInitFlag)
    {
        (HI_VOID)pthread_mutex_init(&gFlashMutex, NULL);

        if(HI_SUCCESS != get_bootargs(g_au8Bootargs, (sizeof(g_au8Bootargs) -1)))
        {
            HI_ERR_FLASH("Failed to get bootargs. \n");
            return HI_FAILURE;
        }

        for( u8Loop = 0; u8Loop < HI_FLASH_TYPE_BUTT; u8Loop++)
        {
            g_eDevStat[u8Loop] = HI_FLASH_STAT_UNINSTALL;
        }

        for(u8Loop=0; u8Loop<HI_FLASH_TYPE_BUTT; u8Loop++)
        {
            pszPos[u8Loop] = strstr((HI_CHAR *)g_au8Bootargs, szFlashStr[u8Loop]);
        }

        if( HI_SUCCESS == spi_raw_init())
        {
            g_eDevStat[HI_FLASH_TYPE_SPI_0] = HI_FLASH_STAT_INSTALL;
        }
        /*else if(strstr((char*)g_au8Bootargs, "hi_sfc:"))*/
        else if(HI_NULL != pszPos[HI_FLASH_TYPE_SPI_0])
        {
            HI_ERR_FLASH("spi init fail! \n");
            return HI_FAILURE;
        }

        if( HI_SUCCESS == nand_raw_init())
        {
            g_eDevStat[HI_FLASH_TYPE_NAND_0] = HI_FLASH_STAT_INSTALL;
        }
        /*else if(strstr((char*)g_au8Bootargs, "hinand:"))*/
        else if(HI_NULL != pszPos[HI_FLASH_TYPE_NAND_0])
        {
            (HI_VOID)spi_raw_destroy();
            HI_ERR_FLASH("nand init fail! \n");
            return HI_FAILURE;
        }

        if( HI_SUCCESS == emmc_raw_init((char *)g_au8Bootargs))
        {
            g_eDevStat[HI_FLASH_TYPE_EMMC_0] = HI_FLASH_STAT_INSTALL;
        }
        else if(HI_NULL != pszPos[HI_FLASH_TYPE_EMMC_0])
        {
            (HI_VOID)spi_raw_destroy();
            (HI_VOID)nand_raw_destroy();
            HI_ERR_FLASH("emmc init fail! \n");
            return HI_FAILURE;
        }

        if (Flash_Init())
        {
            (HI_VOID)spi_raw_destroy();
            (HI_VOID)nand_raw_destroy();
            HI_ERR_FLASH("Flash init fail! \n");
            return HI_FAILURE;
        }
    }
    gInitFlag = HI_TRUE;

    return HI_SUCCESS;
}

HI_S32 HI_Flash_UnusedHandle()
{
    HI_S8 u8Loop;

    for(u8Loop = 0; u8Loop < MAX_HANDLE; u8Loop++)
    {
        if((HI_VOID *)INVALID_FD == gFlashInfo[u8Loop].fd)
        {
            break;
        }
    }

    return u8Loop;
}

HI_HANDLE HI_Flash_OpenByTypeAndAddr(HI_FLASH_TYPE_E enFlashType,
                                     HI_U64 u64Address,
                                     HI_U64 u64Len)
{
    HI_U64  TotalSize = 0;
    HI_U32  PageSize = 0;
    HI_U32  BlockSize = 0;
    HI_U32  OobSize = 0;
    HI_U32  BlockShift = 0;
    EMMC_CB_S *pstEmmcCB;
    HI_U32  hFlash = 0;

    if (All_FLash_Init())
    {
        return (HI_HANDLE)INVALID_FD;
    }

    if ((HI_FLASH_TYPE_BUTT <= enFlashType))
    {
        HI_ERR_FLASH("FlashType error! \n");
        return (HI_HANDLE)INVALID_FD;
    }

    if( (HI_FLASH_STAT_INSTALL != g_eDevStat[enFlashType]))
    {
        HI_ERR_FLASH("No config flash[type:%d].", enFlashType);
        return (HI_HANDLE)INVALID_FD;
    }

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if( HI_FLASH_TYPE_EMMC_0 == enFlashType )
    {
        pstEmmcCB = emmc_raw_open(u64Address, u64Len);
        if( NULL == pstEmmcCB )
        {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return (HI_HANDLE)INVALID_FD;
        }

        if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
        {
            HI_ERR_FLASH("flash array full! \n");
            (HI_VOID)emmc_raw_close(pstEmmcCB);
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return (HI_HANDLE)INVALID_FD;
        }

        gFlashInfo[hFlash].fd = pstEmmcCB;
        gFlashInfo[hFlash].FlashType = HI_FLASH_TYPE_EMMC_0;
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return  (HI_HANDLE)hFlash;
    }
    else if (HI_FLASH_TYPE_SPI_0 == enFlashType)
    {
        spi_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
    }
    else
    {
        nand_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
    }

    if (0 == BlockSize)
    {
        HI_ERR_FLASH("BlockSize shouldn't equal 0!\n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)INVALID_FD;
    }

    if ((int)(u64Address % BlockSize) || (int)(u64Len % BlockSize))
    {
        HI_ERR_FLASH("Open Address(%#llx) and Len(%#llx) should be align with BlockSize(0x%lX)!\n",
                     u64Address, u64Len, BlockSize);
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)INVALID_FD;
    }

    if ((u64Address >= TotalSize) || (u64Address + u64Len) > TotalSize)
    {
        HI_ERR_FLASH("Open Address(%#llx) and Len(%#llx) should be smaller than TotalSize(0x%lX)!\n",
                     u64Address, u64Len, TotalSize);
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)INVALID_FD;
    }

    if(HI_SUCCESS != permission_check(enFlashType, u64Address, u64Len))
    {
        HI_INFO_FLASH("not permission to be opened.\n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)INVALID_FD;
    }

    if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
    {
        HI_ERR_FLASH("flash array full! \n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)INVALID_FD;
    }

    gFlashInfo[hFlash].fd = (long *)(long)(SPAN_PART_HANDLE + hFlash);
    gFlashInfo[hFlash].OpenAddr = u64Address;
    gFlashInfo[hFlash].OpenLeng= u64Len;
    gFlashInfo[hFlash].pPartInfo = NULL;
    gFlashInfo[hFlash].FlashType = enFlashType;
    gFlashInfo[hFlash].PageSize = PageSize;
    gFlashInfo[hFlash].OobSize = OobSize;
    gFlashInfo[hFlash].BlockSize = BlockSize;

    if (HI_FLASH_TYPE_SPI_0 == enFlashType)
    {
        gFlashInfo[hFlash].pFlashopt = (FLASH_OPT_S *)&g_stFlashOpt[HI_FLASH_TYPE_SPI_0];
    }
    if (HI_FLASH_TYPE_NAND_0 == enFlashType)
    {
        gFlashInfo[hFlash].pFlashopt = (FLASH_OPT_S *)&g_stFlashOpt[HI_FLASH_TYPE_NAND_0];
    }
    HI_INFO_FLASH("fd= %ld, OpenAddress= %#llx, OpenLen= %#llx \n", (long)gFlashInfo[hFlash].fd, u64Address, u64Len);
    HI_INFO_FLASH("end.\n");
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return (HI_HANDLE)hFlash;
}

HI_HANDLE HI_Flash_OpenByTypeAndName(HI_FLASH_TYPE_E enFlashType,
                                     HI_CHAR *pPartitionName)
{
    HI_S64  fd = (HI_S64)INVALID_FD;
    HI_S32  i, j;
    HI_CHAR DevName[FLASH_NAME_LEN] = {0};
    HI_U32  hPart = 0;
    HI_U32  hFlash = 0;
    HI_U64 u64Address = 0;
    HI_U64 u64Len = 0;
    char *ptr;
    char media_name[20] = {0};
    EMMC_CB_S *pstEmmcCB;
    HI_U64  TotalSize = 0;
    HI_U32  PageSize = 0;
    HI_U32  BlockSize = 0;
    HI_U32  OobSize = 0;
    HI_U32  BlockShift = 0;

    if (HI_FLASH_TYPE_BUTT == enFlashType)
    {
        HI_ERR_FLASH("FlashType error(FlashType=%d)! \n", enFlashType);
        return (HI_HANDLE)INVALID_FD;
    }

    if (!pPartitionName)
    {
        HI_ERR_FLASH("pPartitionName is null! \n");
        return (HI_HANDLE)INVALID_FD;
    }


    if (All_FLash_Init())
    {
        return (HI_HANDLE)INVALID_FD;
    }

    if( (HI_FLASH_STAT_INSTALL != g_eDevStat[enFlashType]))
    {
        HI_ERR_FLASH("No config flash[type:%d].", enFlashType);
        return (HI_HANDLE)INVALID_FD;
    }

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if( HI_FLASH_TYPE_EMMC_0 == enFlashType)
    {
#if defined (ANDROID)
        if( 0 == strncmp(pPartitionName, "/dev/block/mmcblk0p", strlen("/dev/block/mmcblk0p")))
#else
        if( 0 == strncmp(pPartitionName, "/dev/mmcblk0p", strlen("/dev/mmcblk0p")))
#endif
        {
            pstEmmcCB = emmc_node_open((HI_U8*)pPartitionName);
            if( NULL == pstEmmcCB )
            {
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)INVALID_FD;
            }

            if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
            {
                HI_ERR_FLASH("flash array full! \n");
                (HI_VOID)emmc_raw_close(pstEmmcCB);
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)INVALID_FD;
            }

            strncpy(media_name, "mmcblk0", sizeof(media_name) - 1);
            media_name[sizeof(media_name) - 1] = '\0';
            if (find_part_from_devname(media_name, (char *)g_au8Bootargs,
                                       pPartitionName, &u64Address, &u64Len))
            {
                HI_ERR_FLASH("Cannot find partiton from %s\n", pPartitionName);
        (HI_VOID)emmc_raw_close(pstEmmcCB);
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)INVALID_FD;
            }

            pstEmmcCB->u64PartSize = u64Len;
            gFlashInfo[hFlash].fd = pstEmmcCB;
            gFlashInfo[hFlash].FlashType = HI_FLASH_TYPE_EMMC_0;
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return  (HI_HANDLE)hFlash;
        }

        ptr = strstr((char*)g_au8Bootargs, "mmcblk0:");
        if(NULL != ptr)
        {
            memset(media_name, 0 , sizeof(media_name));
            strncpy(media_name, "mmcblk0", sizeof(media_name) - 1);
            media_name[sizeof(media_name) - 1] = '\0';
            if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
            {
                HI_ERR_FLASH("Cannot find partition: %s\n", pPartitionName);
            }
            else if(u64Len == (HI_U64)(-1))
            {
                HI_ERR_FLASH("Can not contain char '-'\n");
            }
            else
            {
                pstEmmcCB = emmc_raw_open(u64Address, u64Len);
                if( NULL == pstEmmcCB )
                {
                    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                    return (HI_HANDLE)INVALID_FD;
                }

                if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
                {
                    HI_ERR_FLASH("flash array full! \n");
                    (HI_VOID)emmc_raw_close(pstEmmcCB);
                    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                    return (HI_HANDLE)INVALID_FD;
                }

                gFlashInfo[hFlash].fd = pstEmmcCB;
                gFlashInfo[hFlash].FlashType = HI_FLASH_TYPE_EMMC_0;
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)hFlash;

            }
        }

        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return ((HI_HANDLE)INVALID_FD);
    }

    if(strstr((char*)g_au8Bootargs, "hinand:")
       || strstr((char*)g_au8Bootargs, "hi_sfc:"))
    {
        for(i=0; i<MAX_PARTS; i++)
        {
            if(!strncmp(gPartInfo[i].DevName, pPartitionName, strlen(pPartitionName) + 1) && (gPartInfo[i].FlashType == enFlashType)) //eg: "/dev/mtd* "
            {
                break;
            }
            if(!strncmp(gPartInfo[i].PartName, pPartitionName, strlen(pPartitionName) + 1) && (gPartInfo[i].FlashType == enFlashType))
            {
                break;
            }
        }

        for(j=MAX_PARTS - 1; j>= 0; j--)
        {
            if(!strncmp(gPartInfo[j].DevName, pPartitionName, strlen(pPartitionName) + 1) && (gPartInfo[j].FlashType == enFlashType)) //eg: "/dev/mtd* "
            {
                break;
            }
            if(!strncmp(gPartInfo[j].PartName, pPartitionName, strlen(pPartitionName) + 1) && (gPartInfo[j].FlashType == enFlashType))
            {
                break;
            }
        }
        /* add i < 0 test in if branch to avoid pclint warning:
         * Warning 676: Possibly negative subscript (-1) in operator '['
         */
        if ((MAX_PARTS == i) || (i != j) || i < 0)
        {
            HI_ERR_FLASH("can not find a right flash part(i=%d, j=%d)!\n", i, j);
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return (HI_HANDLE)INVALID_FD;
        }

        memset(DevName, 0, FLASH_NAME_LEN);
        strncpy(DevName, gPartInfo[i].DevName, sizeof(DevName));
        DevName[sizeof(DevName) - 1] = '\0';

        hPart = (HI_U32)i;

        if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
        {
            HI_ERR_FLASH("flash array full! \n");
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return (HI_HANDLE)INVALID_FD;
        }

        for(i=0; i<MAX_HANDLE; i++) // if the partition open, return index of array(gFlashInfo)
        {
            if((HI_NULL != gFlashInfo[i].pPartInfo) && (!strncmp(gFlashInfo[i].pPartInfo->DevName, DevName, strlen(DevName) + 1)))
            {
                if ((HI_VOID *)INVALID_FD != gFlashInfo[i].fd)
                {
                    HI_INFO_FLASH("fd = %p, DevName =\"%s\"(%s)\n", gFlashInfo[i].fd, gFlashInfo[i].pPartInfo->DevName, gFlashInfo[i].pPartInfo->PartName);
                    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                    return (HI_HANDLE)i;
                }
            }
        }

        if(gPartInfo[hPart].perm == ACCESS_RDWR)
        {
            fd = open(DevName, O_RDWR);
        }
        else if(gPartInfo[hPart].perm == ACCESS_RD)
        {
            fd = open(DevName, O_RDONLY);
        }
        else  if(gPartInfo[hPart].perm == ACCESS_WR)
        {
            fd = open(DevName, O_WRONLY);
        }
        else
        {
            HI_ERR_FLASH("Device \"%s\"(%s) can not be opened \n", gPartInfo[hPart].DevName, gPartInfo[hPart].PartName);
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return ((HI_HANDLE)INVALID_FD);
        }

        if((fd < 0) || (fd >= SPAN_PART_HANDLE))
        {
            HI_ERR_FLASH("Open %s flash partition failure(fd = %lld)!\n", DevName, (HI_S64)fd);
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            if (fd >= 0)
            {
                close (fd);
            }
            return (HI_HANDLE)INVALID_FD;
        }

        if (HI_FLASH_TYPE_SPI_0 == enFlashType)
        {
            spi_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
        }
        else
        {
            nand_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
        }

        gFlashInfo[hFlash].fd = (long *)(long)fd;
        //gFlashInfo[hFlash].OpenAddr = 0;
        gFlashInfo[hFlash].OpenLeng= 0;
        gFlashInfo[hFlash].pPartInfo = &gPartInfo[hPart];
        gFlashInfo[hFlash].OpenAddr = gFlashInfo[hFlash].pPartInfo->StartAddr;
        gFlashInfo[hFlash].FlashType = enFlashType;
        gFlashInfo[hFlash].PageSize = PageSize;
        gFlashInfo[hFlash].OobSize = OobSize;
        gFlashInfo[hFlash].BlockSize = BlockSize;

        if (HI_FLASH_TYPE_SPI_0 == enFlashType)
        {
            gFlashInfo[hFlash].pFlashopt = (FLASH_OPT_S *)&g_stFlashOpt[HI_FLASH_TYPE_SPI_0];
        }
        if (HI_FLASH_TYPE_NAND_0 == enFlashType)
        {
            gFlashInfo[hFlash].pFlashopt = (FLASH_OPT_S *)&g_stFlashOpt[HI_FLASH_TYPE_NAND_0];
        }
        HI_INFO_FLASH("fd = %p, DevName =\"%s\"(%s)\n", gFlashInfo[hFlash].fd, gFlashInfo[hFlash].pPartInfo->DevName, gFlashInfo[hFlash].pPartInfo->PartName);
        HI_INFO_FLASH("end.\n");

        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return (HI_HANDLE)hFlash;
    }

    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
    return ((HI_HANDLE)INVALID_FD);
}

HI_HANDLE HI_Flash_OpenByName(HI_CHAR *pPartitionName)
{
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;
    HI_S32 i, j;
    HI_U32 hFlash;
    HI_U64 u64Address = 0;
    HI_U64 u64Len = 0;
    char *ptr;
    char media_name[20];
    EMMC_CB_S *pstEmmcCB;

    if (!pPartitionName)
    {
        HI_ERR_FLASH("pPartitionName is null! \n");
        return (HI_HANDLE)INVALID_FD;
    }

    if (All_FLash_Init())
    {
        return (HI_HANDLE)INVALID_FD;
    }

    enFlashType = get_flashtype_by_bootargs(pPartitionName);
    if (HI_FLASH_TYPE_BUTT == enFlashType)
    {
        HI_ERR_FLASH("Invalid partition name: %s\n", pPartitionName);
        return (HI_HANDLE)INVALID_FD;
    }

    if((HI_FLASH_TYPE_EMMC_0 == enFlashType)
        && (NULL != (ptr = strstr((char*)g_au8Bootargs, "mmcblk0:"))))
    {
        memset(media_name, 0 , sizeof(media_name));
        strncpy(media_name, "mmcblk0", sizeof(media_name) - 1);
        media_name[sizeof(media_name) - 1] = '\0';
        if (find_flash_part(ptr, media_name, pPartitionName, &u64Address, &u64Len) == 0)
        {
            HI_ERR_FLASH("Cannot find partition: %s\n", pPartitionName);
            return ((HI_HANDLE)INVALID_FD);
        }
        else if(u64Len == (HI_U64)(-1))
        {
            HI_ERR_FLASH("Can not contain char '-'\n");
            return ((HI_HANDLE)INVALID_FD);
        }
        else
        {
            (HI_VOID)pthread_mutex_lock(&gFlashMutex); pstEmmcCB = emmc_raw_open(u64Address, u64Len);
            if( NULL == pstEmmcCB )
            {
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)INVALID_FD;
            }

            if(MAX_HANDLE == (hFlash = (HI_U32)HI_Flash_UnusedHandle()))
            {
                HI_ERR_FLASH("flash array full! \n");
                (HI_VOID)emmc_raw_close(pstEmmcCB);
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return (HI_HANDLE)INVALID_FD;
            }

            gFlashInfo[hFlash].fd = pstEmmcCB;
            gFlashInfo[hFlash].FlashType = HI_FLASH_TYPE_EMMC_0;
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return (HI_HANDLE)hFlash;
        }
    }

    if(strstr((char*)g_au8Bootargs, "hinand:")
       || strstr((char*)g_au8Bootargs, "hi_sfc:"))
    {
        for(i=0; i<MAX_PARTS; i++)
        {
            if(!strncmp(gPartInfo[i].DevName, pPartitionName, strlen(pPartitionName) + 1)) //eg: "/dev/mtd* "
            {
                break;
            }
            if(!strncmp(gPartInfo[i].PartName, pPartitionName, strlen(pPartitionName) + 1))
            {
                break;
            }
        }

        for(j=MAX_PARTS - 1; j>= 0; j--)
        {
            if(!strncmp(gPartInfo[j].DevName, pPartitionName, strlen(pPartitionName) + 1)) //eg: "/dev/mtd* "
            {
                break;
            }
            if(!strncmp(gPartInfo[j].PartName, pPartitionName, strlen(pPartitionName) + 1))
            {
                break;
            }
        }
        /* add i < 0 test in if branch to avoid pclint warning:
         * Warning 676: Possibly negative subscript (-1) in operator '['
         */
        if ((MAX_PARTS == i) || (i != j || i < 0))
        {
            HI_ERR_FLASH("can not find a right flash part(i=%d, j=%d)!\n", i, j);
            return (HI_HANDLE)INVALID_FD;
        }

        enFlashType = gPartInfo[i].FlashType;
        hFlash = HI_Flash_OpenByTypeAndName(enFlashType, pPartitionName);
        return hFlash;
    }

    return ((HI_HANDLE)INVALID_FD);
}

HI_HANDLE HI_Flash_Open(HI_FLASH_TYPE_E enFlashType, HI_CHAR *pPartitionName, HI_U64 u64Address, HI_U64 u64Len)
{
    HI_U32  hFlash = 0;

    //HI_INFO_FLASH("FlashType=%d, PartitionName=%s, Address=0x%llx, Len=0x%llx\n", enFlashType, pPartitionName, u64Address, u64Len);

    if( NULL == pPartitionName )
    {
        hFlash = HI_Flash_OpenByTypeAndAddr(enFlashType, u64Address, u64Len);
    }
    else
    {
        if ( HI_FLASH_TYPE_BUTT == enFlashType )
        {
            hFlash = HI_Flash_OpenByName(pPartitionName);
        }
        else
        {
            hFlash = HI_Flash_OpenByTypeAndName(enFlashType, pPartitionName);
        }
    }

    return hFlash;
}


HI_S32 HI_Flash_Close(HI_HANDLE hFlash)
{
    HI_Flash_PartInfo_S *pPartInfo = NULL;
    HI_U32 i;
    HI_S32 ret;
    EMMC_CB_S *pstEmmcCB;

    /* avoid pclint warning */
    pPartInfo = pPartInfo;

    CHECK_FLASH_INIT(hFlash);

    if( HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType )
    {
	(HI_VOID)pthread_mutex_lock(&gFlashMutex);
        pstEmmcCB = ((EMMC_CB_S *)(gFlashInfo[hFlash].fd));
        (HI_VOID)emmc_raw_close(pstEmmcCB);
        gFlashInfo[hFlash].fd = (HI_S64 *)(HI_S64)INVALID_FD;
        gFlashInfo[hFlash].OpenAddr = 0;
        gFlashInfo[hFlash].OpenLeng= 0;
        gFlashInfo[hFlash].pPartInfo = NULL;
        gFlashInfo[hFlash].FlashType = HI_FLASH_TYPE_BUTT;

	(HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return HI_SUCCESS;
    }

    pPartInfo = gFlashInfo[hFlash].pPartInfo;

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);
    HI_INFO_FLASH("fd = %p\n", gFlashInfo[hFlash].fd);

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        gFlashInfo[hFlash].fd = (HI_S64 *)(HI_S64)INVALID_FD;
        gFlashInfo[hFlash].OpenAddr = 0;
        gFlashInfo[hFlash].OpenLeng= 0;
        gFlashInfo[hFlash].pPartInfo = NULL;
    }
    else
    {
        ret = close((long)gFlashInfo[hFlash].fd);
        if(0 != ret)
        {
            HI_ERR_FLASH("Close %s flash partition failure %d!\n", gFlashInfo[hFlash].pPartInfo->DevName, ret);
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
        gFlashInfo[hFlash].fd = (HI_S64 *)(HI_S64)INVALID_FD;
        gFlashInfo[hFlash].OpenAddr = 0;
        gFlashInfo[hFlash].OpenLeng= 0;
        gFlashInfo[hFlash].pPartInfo = NULL;
    }

    for(i=0; i< MAX_HANDLE; i++)
    {
        if ((HI_VOID *)INVALID_FD != gFlashInfo[i].fd)
        {
            break;
        }
    }

    if (MAX_HANDLE == i)
    {
        (HI_VOID)spi_raw_destroy();
        (HI_VOID)nand_raw_destroy();
        gInitFlag = HI_FALSE;
    }

    HI_INFO_FLASH("end.\n");
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return HI_SUCCESS;
}

static HI_S32 Compensate_Nand_Address(HI_U64 u64Address, HI_U64 *StartAddr)
{
    HI_U64 TotalSize = 0;
    HI_U32 PageSize = 0;
    HI_U32 BlockSize = 0;
    HI_U32 OobSize = 0;
    HI_U32 BlockShift = 0;
    int idx = 0;
    HI_S32 ret;

    nand_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
    if (0 == BlockSize)
    {
        HI_ERR_FLASH("BlockSize shouldn't equal 0!\n");
        return HI_FAILURE;
    }
    idx = (int)(u64Address >> BlockShift);
    ret = nand_raw_get_physical_index(*StartAddr, &idx, (HI_S32)BlockSize);
    if (0 != ret)
    {
        HI_ERR_FLASH("logical addr change to physical addr error!\n");
        return ret;
    }
    *StartAddr += (unsigned long long)((HI_U32)idx << BlockShift);
    *StartAddr += (u64Address % BlockSize);

    return HI_SUCCESS;
}

HI_S32 HI_Flash_Erase(HI_HANDLE hFlash, HI_U64 u64Address, HI_U64 u64Len)
{
    HI_U64 StartAddr = 0;
    HI_U64 LimitLeng = 0;
    HI_S32 ret;

    /* u64Len bigger than 2GB should use HI_Flash_Erase64 instead. */
    if (u64Len >= 0x80000000) {
    HI_ERR_FLASH("u64Len is too big(0x%llx), should use HI_Flash_Erase64() instead.\n");
    return HI_FAILURE;
    }

    CHECK_FLASH_INIT(hFlash);

    if( HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType)
    {
#if defined (HI_EMC_ERASE_SUPPORT)
        //HI_PRINT("HI_Flash_Erase -> emc_raw_erase\nu64Address=0x%08llx, u64Len=0x%08llx\n", u64Address, u64Len);
        return emc_raw_erase((EMMC_CB_S *)gFlashInfo[hFlash].fd, u64Address, u64Len);
#else
        return HI_SUCCESS;
#endif
    }

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        StartAddr = gFlashInfo[hFlash].OpenAddr;
        LimitLeng = gFlashInfo[hFlash].OpenLeng;
    }
    else
    {
        StartAddr = gFlashInfo[hFlash].pPartInfo->StartAddr;
        LimitLeng = gFlashInfo[hFlash].pPartInfo->PartSize;
    }
    CHECK_ADDR_LEN_VALID(u64Address, u64Len, LimitLeng);

    if (HI_FLASH_TYPE_NAND_0 == gFlashInfo[hFlash].FlashType)
    {
        ret = Compensate_Nand_Address(u64Address, &StartAddr);
        if (HI_SUCCESS != ret)
        {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
    }
    else
    {
        StartAddr += u64Address;
    }
    HI_INFO_FLASH("HANDLE=%d, Address=0x%llx, Len=0x%llx\n", hFlash, StartAddr, u64Len);

    if (!gFlashInfo[hFlash].pFlashopt->raw_erase)
    {
        HI_ERR_FLASH("flash service function ptr(raw_erase) is NULL! \n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return HI_FAILURE;
    }

    ret = (HI_S32)gFlashInfo[hFlash].pFlashopt->raw_erase((int)(long)gFlashInfo[hFlash].fd, (unsigned long long)StartAddr, u64Len, gFlashInfo[hFlash].OpenAddr, LimitLeng);
    HI_INFO_FLASH("end.\n");
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return ret;
}

HI_S64 HI_Flash_Erase64(HI_HANDLE hFlash, HI_U64 u64Address, HI_U64 u64Len)
{
    HI_U64 StartAddr = 0;
    HI_U64 LimitLeng = 0;
    HI_S64 ret;

    CHECK_FLASH_INIT(hFlash);

    if( HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType)
    {
        return HI_SUCCESS;
    }

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        StartAddr = gFlashInfo[hFlash].OpenAddr;
        LimitLeng = gFlashInfo[hFlash].OpenLeng;
    }
    else
    {
        StartAddr = gFlashInfo[hFlash].pPartInfo->StartAddr;
        LimitLeng = gFlashInfo[hFlash].pPartInfo->PartSize;
    }
    CHECK_ADDR_LEN_VALID(u64Address, u64Len, LimitLeng);

    if (HI_FLASH_TYPE_NAND_0 == gFlashInfo[hFlash].FlashType)
    {
        ret = (HI_S64)Compensate_Nand_Address(u64Address, &StartAddr);
        if (HI_SUCCESS != ret)
        {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
    }
    else
    {
        StartAddr += u64Address;
    }
    HI_INFO_FLASH("HANDLE=%d, Address=0x%llx, Len=0x%llx\n", hFlash, StartAddr, u64Len);

    if (!gFlashInfo[hFlash].pFlashopt->raw_erase)
    {
        HI_ERR_FLASH("flash service function ptr(raw_erase) is NULL! \n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return HI_FAILURE;
    }

    ret = (HI_S64)gFlashInfo[hFlash].pFlashopt->raw_erase((int)(long)gFlashInfo[hFlash].fd, (unsigned long long)StartAddr, u64Len, gFlashInfo[hFlash].OpenAddr, LimitLeng);
    HI_INFO_FLASH("end.\n");
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return ret;
}

HI_S32 HI_Flash_Read(HI_HANDLE hFlash, HI_U64 u64Address, HI_U8 *pBuf, HI_U32 u32Len, HI_U32 u32Flags)
{
    HI_U64 StartAddr = 0;
    HI_U64 LimitLeng = 0;
    HI_S32 ret, wtihoob;
    EMMC_CB_S *pstEmmcCB;

    if (NULL == pBuf)
    {
        return HI_FAILURE;
    }

    CHECK_FLASH_INIT(hFlash);

    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if( HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType)
    {
        pstEmmcCB = (EMMC_CB_S *)(gFlashInfo[hFlash].fd);
        ret = emmc_raw_read(pstEmmcCB, u64Address, u32Len, pBuf);
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
    /* union return value to HI_FAILURE.*/
    if (ret <0)
        return HI_FAILURE;
    return ret;
    }

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        StartAddr = gFlashInfo[hFlash].OpenAddr;
        LimitLeng = gFlashInfo[hFlash].OpenLeng;
    }
    else
    {
        StartAddr = gFlashInfo[hFlash].pPartInfo->StartAddr;
        LimitLeng = gFlashInfo[hFlash].pPartInfo->PartSize;
    }

    if (HI_FLASH_RW_FLAG_WITH_OOB == (u32Flags & HI_FLASH_RW_FLAG_WITH_OOB))
    {
        HI_U32 u32LenWithoutOOB = (u32Len
                       / (gFlashInfo[hFlash].OobSize
                          + gFlashInfo[hFlash].PageSize))
                      * gFlashInfo[hFlash].PageSize;
        if (u32Len % (gFlashInfo[hFlash].OobSize
                  + gFlashInfo[hFlash].PageSize))
            u32LenWithoutOOB += gFlashInfo[hFlash].PageSize;
        CHECK_ADDR_LEN_VALID(u64Address, u32LenWithoutOOB, LimitLeng);
    }
    else
    {
        CHECK_ADDR_LEN_VALID(u64Address, u32Len, LimitLeng);
    }

    if (HI_FLASH_TYPE_NAND_0 == gFlashInfo[hFlash].FlashType)
    {
        ret = Compensate_Nand_Address(u64Address, &StartAddr);
        if (HI_SUCCESS != ret)
        {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
    }
    else
    {
        StartAddr += u64Address;
    }
    HI_INFO_FLASH("HANDLE=%d, Address=0x%llx, Len=0x%x, Flag=%d\n", hFlash, StartAddr, u32Len, u32Flags);

    if (HI_FLASH_RW_FLAG_WITH_OOB == (u32Flags & HI_FLASH_RW_FLAG_WITH_OOB))
    {
        wtihoob = 1;
    }
    else
    {
        wtihoob = 0;
    }

    if (!gFlashInfo[hFlash].pFlashopt->raw_read)
    {
        HI_ERR_FLASH("flash service function ptr(raw_read) is NULL! \n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return HI_FAILURE;
    }

    ret = gFlashInfo[hFlash].pFlashopt->raw_read((int)(long)gFlashInfo[hFlash].fd, (unsigned long long *)&StartAddr, pBuf, u32Len, gFlashInfo[hFlash].OpenAddr, LimitLeng, wtihoob, 1);
    HI_INFO_FLASH("totalread =0x%x, end.\n", ret);
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return ret;
}

HI_S32 HI_Flash_Write(HI_HANDLE hFlash, HI_U64 u64Address, HI_U8 *pBuf, HI_U32 u32Len, HI_U32 u32Flags)
{
    HI_U64 StartAddr = 0;
    HI_U64 LimitLeng = 0;
    HI_S32 ret, wtihoob, erase;

    HI_U64 TotalSize = 0;
    HI_U32 PageSize = 0;
    HI_U32 BlockSize = 0;
    HI_U32 OobSize = 0;
    HI_U32 BlockShift = 0;

    HI_U32 u32EraseLen = 0;
    HI_U32 BlockSize_new = 0;
    EMMC_CB_S *pstEmmcCB;

    if (NULL == pBuf)
    {
        return HI_FAILURE;
    }

    CHECK_FLASH_INIT(hFlash);
    (HI_VOID)pthread_mutex_lock(&gFlashMutex);

    if( HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType)
    {
        pstEmmcCB = (EMMC_CB_S *)(gFlashInfo[hFlash].fd);
        ret = emmc_raw_write(pstEmmcCB, u64Address, u32Len, pBuf);
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
    if (ret <0)
        return HI_FAILURE;
    return ret;
    }

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        StartAddr = gFlashInfo[hFlash].OpenAddr;
        LimitLeng = gFlashInfo[hFlash].OpenLeng;
    }
    else
    {
        StartAddr = gFlashInfo[hFlash].pPartInfo->StartAddr;
        LimitLeng = gFlashInfo[hFlash].pPartInfo->PartSize;
    }
    if (HI_FLASH_RW_FLAG_WITH_OOB == (u32Flags & HI_FLASH_RW_FLAG_WITH_OOB))
    {
        HI_U32 u32LenWithoutOOB = (u32Len
                       / (gFlashInfo[hFlash].OobSize
                          + gFlashInfo[hFlash].PageSize))
                      * gFlashInfo[hFlash].PageSize;
        if (u32Len % (gFlashInfo[hFlash].OobSize
                  + gFlashInfo[hFlash].PageSize))
            u32LenWithoutOOB += gFlashInfo[hFlash].PageSize;
        CHECK_ADDR_LEN_VALID(u64Address, u32LenWithoutOOB, LimitLeng);
    }
    else
    {
        CHECK_ADDR_LEN_VALID(u64Address, u32Len, LimitLeng);
    }

    if (HI_FLASH_TYPE_NAND_0 == gFlashInfo[hFlash].FlashType)
    {
        ret = Compensate_Nand_Address(u64Address, &StartAddr);
        if (HI_SUCCESS != ret)
        {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
    }
    else
    {
        StartAddr += u64Address;
    }
    HI_INFO_FLASH("HANDLE=%d, Address=0x%llx, Len=0x%x, Flag=%d\n", hFlash, StartAddr, u32Len, u32Flags);

    if (HI_FLASH_TYPE_SPI_0 == gFlashInfo[hFlash].FlashType)
    {
        spi_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
    }
    else if (HI_FLASH_TYPE_NAND_0 == gFlashInfo[hFlash].FlashType)
    {
        nand_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
    }

    if (HI_FLASH_RW_FLAG_ERASE_FIRST == (u32Flags & HI_FLASH_RW_FLAG_ERASE_FIRST))
    {
        erase = 1;
    }
    else
    {
        erase = 0;
    }
    /* avoid pclint div 0 warning */
    if (!PageSize) {
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return -1;
    }

    if (HI_FLASH_RW_FLAG_WITH_OOB == (u32Flags & HI_FLASH_RW_FLAG_WITH_OOB))
    {
        BlockSize_new = BlockSize + OobSize * (BlockSize / PageSize);
        wtihoob = 1;
    }
    else
    {
        BlockSize_new = BlockSize;
        wtihoob = 0;
    }

    if (erase)
    {
        /* avoid pclint div 0 warning */
        if (!BlockSize_new) {
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return -1;
        }

        u32EraseLen = u32Len / BlockSize_new;
        if (u32Len % BlockSize_new)
        {
            u32EraseLen += 1;
        }
        u32EraseLen = u32EraseLen * BlockSize;
        //HI_PRINT("> %s: [%d], u32EraseLen=%#x\n", __FUNCTION__, __LINE__, u32EraseLen);

        if (!gFlashInfo[hFlash].pFlashopt->raw_erase)
        {
            HI_ERR_FLASH("flash service function ptr(raw_erase) is NULL! \n");
            (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
            return HI_FAILURE;
        }
        ret = gFlashInfo[hFlash].pFlashopt->raw_erase((long)gFlashInfo[hFlash].fd, (unsigned long long)StartAddr, (unsigned long long)u32EraseLen, gFlashInfo[hFlash].OpenAddr, LimitLeng);
        if (0 >= ret)
        {
            if (HI_FLASH_END_DUETO_BADBLOCK != ret)
            {
                HI_ERR_FLASH("earse fail!\n");
                (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
                return ret;
            }
        }
    }

    if (!gFlashInfo[hFlash].pFlashopt->raw_write)
    {
        HI_ERR_FLASH("flash service function ptr(raw_write) is NULL! \n");
        (HI_VOID)pthread_mutex_unlock(&gFlashMutex);
        return HI_FAILURE;
    }

    ret = gFlashInfo[hFlash].pFlashopt->raw_write((int)(long)gFlashInfo[hFlash].fd, &StartAddr, pBuf, u32Len, gFlashInfo[hFlash].OpenAddr, LimitLeng, wtihoob);
    HI_INFO_FLASH("totalwrite =0x%x, end.\n", ret);
    (HI_VOID)pthread_mutex_unlock(&gFlashMutex);

    return ret;
}

HI_S32 HI_Flash_GetInfo(HI_HANDLE hFlash, HI_Flash_InterInfo_S *pFlashInfo)
{
    HI_U64 TotalSize = 0;
    HI_U32 PageSize = 0;
    HI_U32 BlockSize = 0;
    HI_U32 OobSize = 0;
    HI_U32 BlockShift = 0;
    EMMC_CB_S *pstEmmcCB;

    if (NULL == pFlashInfo)
    {
        return HI_FAILURE;
    }

    CHECK_FLASH_INIT(hFlash);

    if (HI_FLASH_TYPE_EMMC_0 == gFlashInfo[hFlash].FlashType)
    {
        pstEmmcCB = (EMMC_CB_S *)(gFlashInfo[hFlash].fd);

        memset(pFlashInfo, 0x00, sizeof(*pFlashInfo));
        pFlashInfo->TotalSize = g_stEmmcFlash.u64RawAreaSize;
        pFlashInfo->OobSize = 0;
        pFlashInfo->PartSize = pstEmmcCB->u64PartSize;
        pFlashInfo->BlockSize = (g_stEmmcFlash.u32EraseSize *16);
        pFlashInfo->PageSize = 0;
        pFlashInfo->fd = -1;
        pFlashInfo->FlashType = HI_FLASH_TYPE_EMMC_0;
        pFlashInfo->OpenAddr  = pstEmmcCB->u64Address;
        pFlashInfo->pFlashopt = NULL;

        return HI_SUCCESS;
    }

    switch (gFlashInfo[hFlash].FlashType)
    {
        case HI_FLASH_TYPE_SPI_0:
        {
            spi_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
            break;
        }
        case HI_FLASH_TYPE_NAND_0:
        {
            nand_raw_get_info(&TotalSize, &PageSize, &BlockSize, &OobSize, &BlockShift);
            break;
        }
        default :
        {
            break;
        }
    }

    pFlashInfo->TotalSize = TotalSize;

    if ((HI_VOID *)SPAN_PART_HANDLE <= gFlashInfo[hFlash].fd)
    {
        pFlashInfo->PartSize = gFlashInfo[hFlash].OpenLeng;
        pFlashInfo->pPartInfo = NULL;
    }
    else
    {
        pFlashInfo->PartSize = gFlashInfo[hFlash].pPartInfo->PartSize;
        pFlashInfo->pPartInfo = gFlashInfo[hFlash].pPartInfo;
    }
    pFlashInfo->BlockSize   = BlockSize;
    pFlashInfo->PageSize    = PageSize;
    pFlashInfo->OobSize     = OobSize;
    pFlashInfo->fd          = -1;
    pFlashInfo->FlashType   = gFlashInfo[hFlash].FlashType;
    pFlashInfo->pFlashopt   = NULL;
    pFlashInfo->OpenAddr    = gFlashInfo[hFlash].OpenAddr;
    pFlashInfo->OpenLeng    = gFlashInfo[hFlash].OpenLeng;

    return HI_SUCCESS;
}


