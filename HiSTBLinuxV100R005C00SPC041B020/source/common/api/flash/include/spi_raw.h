#ifndef SPI_RAWH
#define SPI_RAWH
/******************************************************************************/

#include "nand.h"

int spi_raw_init(void);

int spi_raw_destroy(void);

int spi_raw_dump_partition(void);

int spi_raw_read(int fd, unsigned long long *startaddr, unsigned char *buffer,
    unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int read_oob, int skip_badblock);

int spi_raw_write(int fd, unsigned long long *startaddr, unsigned char *buffer,
    unsigned long length, unsigned long long openaddr, unsigned long long limit_leng, int write_oob);

long long spi_raw_erase(int fd, unsigned long long startaddr,
    unsigned long long length, unsigned long long openaddr, unsigned long long limit_leng);

unsigned long long spi_raw_get_start_addr(const char *dev_name, unsigned long blocksize, int *value_valid);

void spi_raw_get_info(unsigned long long *totalsize, unsigned int *pagesize, unsigned int *blocksize,
    unsigned int *oobsize, unsigned int *blockshift);

/******************************************************************************/
#endif /* SPI_RAWH */

