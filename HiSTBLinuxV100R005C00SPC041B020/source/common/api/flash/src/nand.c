#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "hi_flash.h"
#include "nand.h"

/*****************************************************************************/
/* change length to string */
char *int_to_size(unsigned long long size)
{
    int ix;
    static char buffer[20];
    const char *fmt[] = {"%u", "%uK", "%uM", "%uG", "%uT", "%uT"};

    for (ix = 0; (ix < 5) && !(size & 0x3FF) && size; ix++)
    {
        size = (size >> 10);
    }
    snprintf(buffer, sizeof(buffer), fmt[ix], (unsigned int)size);

    return buffer;
}
/*****************************************************************************/
/*
 * Modified for CONNAX CA.
 * When /dev path has no excute permission, stat call will fail.
 * Try parse /proc/mtd to get max partition number.
 */
int get_max_partition(void)
{
    FILE *fd;
#define MAX_PROC_LINE_SZ    1024
    char bf[MAX_PROC_LINE_SZ];
    int nr = 0;

    fd = fopen(PROC_MTD_FILE, "r");
    if (fd == NULL) {
        DBG_OUT("Fail to open %s!\n",PROC_MTD_FILE);
        return 0;
    }
    /* skip first prompt line */
    if (NULL == fgets(bf, MAX_PROC_LINE_SZ, fd)) {
        fclose(fd);
        return -1;
    }

    while (fgets(bf, MAX_PROC_LINE_SZ, fd))
            nr ++;

    fclose(fd);
    DBG_OUT("max partition nr %d\n", nr);
    //HI_PRINT("max partition nr %d\n", nr);
    /* keep the way before do */
    return nr - 1;
}
/*****************************************************************************/

int offshift(unsigned long n)
{
    int shift = -1;
    while (n)
    {
        n = n >> 1;
        shift++;
    }
    return shift;
}
/*****************************************************************************/

static char* skip_space(char* line)
{
    char* p = line;
    while (*p == ' ' || *p == '\t')
    {
        p++;
    }
    return p;
}

static char* get_word(char* line, char* value)
{
    char* p = line;
    p = skip_space(p);
    while (*p != '\t' && *p != ' '  && *p != '\n' && *p != 0)
    {
        *value++ = *p++;
    }
    *value = 0;
    return p;
}

static int get_bootargs(char *pu8Bootarags, unsigned short u16Len)
{
    FILE *pf;

    if( NULL == pu8Bootarags)
    {
        DBG_OUT("Pointer is null.\n");
        return -1;
    }

    if(NULL == (pf = fopen("/proc/cmdline", "r")))
    {
        DBG_OUT("Failed to open '/proc/cmdline'.\n");
        return -1;
    }

    if(NULL == fgets((char*)pu8Bootarags, u16Len, pf))
    {
        DBG_OUT("Failed to fgets string.\n");
        fclose(pf);
        return -1;
    }

    fclose(pf);
    return 0;
}

static HI_FLASH_TYPE_E get_flashtype_by_bootargs(const char *pszBootargs, char *pszPartitionName)
{
    char *pszPartitionPos = NULL;
    char *pszTmpPos = NULL;
    char pszTmpStr[64];
    int i;
    HI_FLASH_TYPE_E enFlashType = HI_FLASH_TYPE_BUTT;
    char szTypeStr[HI_FLASH_TYPE_BUTT][16]={
        "hi_sfc:",
        "hinand:",
        "mmcblk0:"};
    char *pszType[HI_FLASH_TYPE_BUTT];

    if(NULL == pszPartitionName)
    {
        return (HI_FLASH_TYPE_BUTT);
    }

    for(i=0; i<HI_FLASH_TYPE_BUTT; i++)
    {
        pszType[i] = strstr((char *)pszBootargs, szTypeStr[i]);
    }

    memset(pszTmpStr, 0, sizeof(pszTmpStr));
    (HI_VOID)snprintf(pszTmpStr, sizeof(pszTmpStr)-1, "(%s)", pszPartitionName);
    pszPartitionPos = strstr((HI_CHAR *)pszBootargs, pszTmpStr);
    if(NULL == pszPartitionPos)
    {
        return (HI_FLASH_TYPE_BUTT);
    }

    for(i=0; i<HI_FLASH_TYPE_BUTT; i++)
    {
        if(NULL == pszType[i])
        {
            continue;
        }
        /* pszTmpPos is used to be a cursor */
        /*lint -save -e613 */
        if((pszPartitionPos >= pszType[i])&&(pszType[i] >= pszTmpPos))
        {
            enFlashType = (HI_FLASH_TYPE_E) i;
            pszTmpPos = pszType[i];
        }
    }
    return enFlashType;
}

static  HI_Flash_PartInfo_S gPartInfo[MAX_MTD_PARTITION];

int flash_partition_info_init(void)
{
    int ret = -1;
    int i = 0;
    char bootargs[512];
    FILE* fp = HI_NULL;
    static int init_flag = -1;
    HI_Flash_PartInfo_S *pstPartInfo = NULL;

    if(init_flag >=0)
        return 0;

    for(i=0; i< MAX_MTD_PARTITION; i++)
    {
        pstPartInfo = &gPartInfo[i];
        pstPartInfo->StartAddr= 0;
        pstPartInfo->PartSize= 0;
        pstPartInfo->BlockSize = 0;
        pstPartInfo->FlashType = HI_FLASH_TYPE_BUTT;
        pstPartInfo->perm = ACCESS_BUTT;
        memset(pstPartInfo->DevName, '\0', FLASH_NAME_LEN);
        memset(pstPartInfo->PartName, '\0', FLASH_NAME_LEN);
    }

    ret = get_bootargs(bootargs, sizeof(bootargs)-1);
    if(ret != 0)
        return ret;

    fp = fopen("/proc/mtd", "r");
    if (fp)
    {
        unsigned long long u64StartAddr[HI_FLASH_TYPE_BUTT];
        char line[512];
        if (NULL == fgets(line, sizeof(line), fp)) {//skip first line
            fclose(fp);
            return -1;
        }

//        DBG_OUT(" DevName\t  PartSize\tBlockSize   PartName    Startaddr\n");
        for(i=0; i<HI_FLASH_TYPE_BUTT; i++)
        {
            u64StartAddr[i] = 0;
        }

        i=0;
        while(i<MAX_MTD_PARTITION)
        {
            char   argv[4][32];
            char   *p;

            if(fgets(line, sizeof(line), fp) == NULL)
            {
                break;
            }

            p = &line[0];
            p = get_word(p, argv[0]);
            p = get_word(p, argv[1]);
            p = get_word(p, argv[2]);
            p = get_word(p, argv[3]);
            p = p; /* for no TQE warning */

            pstPartInfo = &gPartInfo[i];
            pstPartInfo->PartSize = (HI_U64)strtoull(argv[1],NULL, 16);  //partion size
            pstPartInfo->BlockSize = (HI_U64)(HI_U32)strtol(argv[2], NULL,16); //erase size
            strncpy(pstPartInfo->DevName, argv[0], (strlen(argv[0])-1));
            strncpy(pstPartInfo->PartName, (argv[3]+1), (strlen(argv[3])-2));
            pstPartInfo->FlashType = get_flashtype_by_bootargs(bootargs , pstPartInfo->PartName);
            pstPartInfo->StartAddr = u64StartAddr[pstPartInfo->FlashType];
            u64StartAddr[pstPartInfo->FlashType] += pstPartInfo->PartSize;

            i++;
        }

        fclose(fp);
    }
    else
    {
        return -1;
    }

    init_flag = 0;
    return 0;
}

HI_Flash_PartInfo_S * get_flash_partition_info(HI_FLASH_TYPE_E FlashType, const char * devname)
{
    int i;
    HI_Flash_PartInfo_S *pstPartInfo = NULL;

    if(NULL == devname)
        return NULL;

    for(i=0; i< MAX_MTD_PARTITION; i++)
    {
        pstPartInfo = &gPartInfo[i];
        if(pstPartInfo->FlashType != FlashType)
            continue;
        if(strncmp(pstPartInfo->DevName, devname,
        strlen(pstPartInfo->DevName) > strlen(devname) ?
            strlen(pstPartInfo->DevName) : strlen(devname)) == 0)
            return (pstPartInfo);
    }
    return NULL;
}

unsigned long long get_flash_total_size(HI_FLASH_TYPE_E FlashType)
{
    int i;
    unsigned long long totalsize=0;
    HI_Flash_PartInfo_S *pstPartInfo = NULL;

    for(i=0; i< MAX_MTD_PARTITION; i++)
    {
        pstPartInfo = &gPartInfo[i];
        if(pstPartInfo->FlashType != FlashType)
            continue;
        totalsize += pstPartInfo->PartSize;
    }
    return totalsize;
}

