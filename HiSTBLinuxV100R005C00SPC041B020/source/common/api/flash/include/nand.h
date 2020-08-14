#ifndef NANDH
#define NANDH
/******************************************************************************/
#include <stdint.h>

#define MTD_ABSENT      0
#define MTD_RAM         1
#define MTD_ROM         2
#define MTD_SPIFLASH        3
#define MTD_NANDFLASH       4       /* SLC NAND */
#define MTD_DATAFLASH       6
#define MTD_UBIVOLUME       7
#define MTD_MLCNANDFLASH    8       /* MLC NAND (including TLC) */

#define MAX_PARTS         32    /* Flash max partition number*/
#define MAX_MTD_PARTITION   (MAX_PARTS)

#if defined (ANDROID)
#define DEV_MTDBASE         "/dev/mtd/mtd"
#else
#define DEV_MTDBASE         "/dev/mtd"
#endif

#define PROC_MTD_FILE       "/proc/mtd"

#ifndef __kernel_loff_t
    typedef long long   __kernel_loff_t;
#endif

/*****************************************************************************/

struct mtd_info_user {
    uint8_t type;
    uint32_t flags;
    uint32_t size;
    uint32_t erasesize;
    uint32_t writesize;
    uint32_t oobsize;
    uint32_t ecctype;
    uint32_t eccsize;
};
#define MEMGETINFO      _IOR('M', 1, struct mtd_info_user)

struct erase_info_user64 {
    uint64_t start;
    uint64_t length;
};
#define MEMERASE64      _IOW('M', 20, struct erase_info_user64)


struct mtd_oob_buf {
    uint32_t start;
    uint32_t length;
    unsigned char *ptr;
};
#define MEMREADOOB      _IOWR('M', 4, struct mtd_oob_buf)

/**
 * struct mtd_write_req - data structure for requesting a write operation
 *
 * @start:      start address
 * @len:        length of data buffer
 * @ooblen:     length of OOB buffer
 * @usr_data:   user-provided data buffer
 * @usr_oob:    user-provided OOB buffer
 * @mode:       MTD mode (see "MTD operation modes")
 * @padding:    reserved, must be set to 0
 *
 * This structure supports ioctl(MEMWRITE) operations, allowing data and/or OOB
 * writes in various modes. To write to OOB-only, set @usr_data == NULL, and to
 * write data-only, set @usr_oob == NULL. However, setting both @usr_data and
 * @usr_oob to NULL is not allowed.
 */
struct mtd_write_req {
        uint64_t start;
        uint64_t len;
        uint64_t ooblen;
        uint64_t usr_data;
        uint64_t usr_oob;
        uint8_t mode;
        uint8_t padding[7];
};

#define MEMWRITE            _IOWR('M', 24, struct mtd_write_req)

#define MEMGETBADBLOCK      _IOW('M', 11, __kernel_loff_t)

#define MEMSETBADBLOCK      _IOW('M', 12, __kernel_loff_t)

#define MEMFORCEERASEBLOCK  _IOW('M', 128, __kernel_loff_t)

/*****************************************************************************/

struct mtd_partition
{
    uint64_t start;
    uint64_t end;
    int readonly;
    enum ACCESS_PERM perm;
#if defined (ANDROID)
    char mtddev[25];
#else
    char mtddev[12];
#endif
    int fd;
};

struct nand_raw_ctrl
{
    int num_partition;
    uint64_t size;

    uint32_t pagesize;
    uint32_t blocksize;
    uint32_t oobsize;
    uint32_t oobused;

    uint32_t pageshift;
    uint32_t blockshift;

    uint32_t pagemask;
    uint32_t blockmask;

    struct mtd_partition partition[1];
};

#if 0
#  define DBG_OUT(fmt, arg...) \
    printf("  %s[%d]: " fmt, __FUNCTION__, __LINE__, ##arg)
#else
#  define DBG_OUT(fmt, arg...)
#endif

char *int_to_size(unsigned long long size);

int get_max_partition(void);

int offshift(unsigned long n);

int flash_partition_info_init(void);

HI_Flash_PartInfo_S * get_flash_partition_info(HI_FLASH_TYPE_E FlashType, const char * devname);

unsigned long long get_flash_total_size(HI_FLASH_TYPE_E FlashType);

/******************************************************************************/
#endif /* NANDH */

