#ifndef NAND_RAWH
#define NAND_RAWH
/******************************************************************************/

#include "nand.h"

#define  HI_FLASH_END_DUETO_BADBLOCK -10
#define  HINFC610_OOBSIZE_FOR_YAFFS  32
/**
 * MTD operation modes
 *
 * @MTD_OPS_PLACE_OOB:  OOB data are placed at the given offset (default)
 * @MTD_OPS_AUTO_OOB:   OOB data are automatically placed at the free areas
 *                      which are defined by the internal ecclayout
 * @MTD_OPS_RAW:        data are transferred as-is, with no error correction;
 *                      this mode implies %MTD_OPS_PLACE_OOB
 *
 * These modes can be passed to ioctl(MEMWRITE) and are also used internally.
 * See notes on "MTD file modes" for discussion on %MTD_OPS_RAW vs.
 * %MTD_FILE_MODE_RAW.
 */
enum {
        MTD_OPS_PLACE_OOB = 0,
        MTD_OPS_AUTO_OOB = 1,
        MTD_OPS_RAW = 2,
};

int nand_raw_init(void);

int nand_raw_read(int fd, unsigned long long *startaddr, unsigned char *buffer,
    unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int read_oob, int skip_badblock);

int nand_raw_write(int fd, unsigned long long *startaddr, unsigned char *buffer,
    unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int write_oob);

long long nand_raw_erase(int fd, unsigned long long startaddr,
    unsigned long long length, unsigned long long openaddr, unsigned long long limit_leng);

int nand_raw_force_erase(unsigned long long offset);

int nand_mark_badblock(unsigned long long offset, unsigned long long length);

int nand_show_badblock(unsigned long long offset, unsigned long long length);

int nand_raw_info(struct mtd_info_user *mtdinfo);

int nand_raw_dump_partition(void);

int nand_raw_destroy(void);

unsigned long long nand_raw_get_start_addr(const char *dev_name, unsigned long blocksize, int *value_valid);

void nand_raw_get_info(unsigned long long *totalsize, unsigned int *pagesize, unsigned int *blocksize,
    unsigned int *oobsize, unsigned int *blockshift);

int  nand_raw_get_physical_index(unsigned long long startaddr, int *blockindex, int blocksize);

/******************************************************************************/
#endif /* NAND_RAWH */

