/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*    Create by HiHi. 2011-07-04
*
******************************************************************************/

#ifndef EMMC_LOGIFH
#define EMMC_LOGIFH
/******************************************************************************/

typedef struct emmc_logic_t {
	unsigned long long address;
	unsigned long long length;
	unsigned long long blocksize;
	unsigned long long erase_grp_size;
	void *mmc;

} emmc_logic_t;

emmc_logic_t *emmc_logic_open(unsigned long long address,
			      unsigned long long length);

void emmc_logic_close(emmc_logic_t *emmc_logic);

int emmc_logic_write(emmc_logic_t *emmc_logic, unsigned long long offset,
		     unsigned int length, unsigned char *buf);

int emmc_logic_read(emmc_logic_t *emmc_logic, unsigned long long offset,
		    unsigned int length, unsigned char *buf);

/******************************************************************************/
#endif /* EMMC_LOGIFH */

