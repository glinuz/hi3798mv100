/******************************************************************************
*    Copyright (c) 2009-2011 by Hisi.
*    All rights reserved.
* ***
*
******************************************************************************/

#ifndef NAND_LOGIFH
#define NAND_LOGIFH
/*****************************************************************************/

typedef struct nand_logic_t {
	unsigned long long address;   /* NAND paratition start address */
	unsigned long long length;    /* NAND paratition length */
	unsigned long long erasesize; /* NAND block size */
	void *nand;
	void *bbt_cache;
} nand_logic_t;

/*****************************************************************************/

nand_logic_t *nand_logic_open(unsigned long long address,
	unsigned long long length);

void nand_logic_close(nand_logic_t *nand_logic);

long long nand_logic_erase(nand_logic_t *nand_logic, unsigned long long offset,
	unsigned long long length);

int nand_logic_write(nand_logic_t *nand_logic, unsigned long long offset,
	unsigned int length, unsigned char *buf, int withoob);

int nand_logic_read(nand_logic_t *nand_logic, unsigned long long offset,
	unsigned int length, unsigned char *buf, int withoob);

/******************************************************************************/
#endif /* NAND_LOGIFH */

