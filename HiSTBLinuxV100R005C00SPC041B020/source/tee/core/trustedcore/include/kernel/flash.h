#ifndef _INCLUDE_BOOT_FLASH_H_
#define _INCLUDE_BOOT_FLASH_H_

#include <sre_typedef.h>
/*-----------------------------------------------------------------------
 * NAND or EMMC, choose one
 ------------------------------------------------------------------------*/

/*max partition numbers supported*/
#define MAX_PTN 32

typedef struct ptentry ptentry;

/* flash partitions are defined in terms of blocks
** (flash erase units)
*/
struct ptentry {
	char name[32];
	unsigned start;
	unsigned length;
	unsigned flags;
};
#define EFI_ENTRIES     128
#define EFI_NAMELEN     36
struct efi_entry {
	u8 type_uuid[16];
	u8 uniq_uuid[16];
	u64 first_lba;
	u64 last_lba;
	u64 attr;
	u16 name[EFI_NAMELEN];
};
struct efi_header {
	u8 magic[8];

	u32 version;
	u32 header_sz;

	u32 crc32;
	u32 reserved;

	u64 header_lba;
	u64 backup_lba;
	u64 first_lba;
	u64 last_lba;

	u8 volume_uuid[16];

	u64 entries_lba;

	u32 entries_count;
	u32 entries_size;
	u32 entries_crc32;
} __attribute__((packed));


//Total patition image
struct ptable {
	u8 mbr[512];
	union {
		struct efi_header header;
		u8 block[512];
	};
	struct efi_entry entry[EFI_ENTRIES];
};

/* tools to populate and query the partition table */
struct ptentry *find_ptn(const char *str);
void get_ptn_offset(const char *str, unsigned int *off);
void get_ptn_name(char *str, unsigned int off);

/*boot linux*/
int boot_from_flash(const ptentry *p);
void boot_kernel(const ptentry *ptn);
void boot_recovery();


int emmc_init(void);
int emmc_read(const ptentry *ptn, unsigned offset, void *data, unsigned *p_bytes);
int emmc_write(const ptentry *ptn, const void *data, unsigned bytes);
int emmc_erase(const ptentry *ptn);
int emmc_write_by_block(const ptentry *ptn, unsigned int start_block,
			const void *data, unsigned int bytes);
int emmc_multi_blk_write_DMA(const unsigned char *pbuf, unsigned int blk_num,
			     unsigned int blk_count, unsigned int blk_size);
int emmc_write_zero(ptentry *ptn);
int emmc_erase_all_except_xloader(void);
int emmc_write_char(ptentry *ptn, unsigned char ch);
int emmc_check_char(ptentry *ptn, unsigned char ch);
int emmc_erase_all(void);

/* add for partiton adaptive */
unsigned int emmc_get_blknr(const unsigned char *pbuf, unsigned int dwBlkCount, unsigned int multiBlkSize);

#define flash_init() emmc_init()
#define flash_read(ptn, offset, data, p_bytes) emmc_read(ptn, offset, data, p_bytes)
#define flash_write(ptn, data, bytes) emmc_write(ptn, data, bytes)
#define flash_erase(ptn) emmc_erase(ptn)
#define flash_write_zero(ptn) emmc_write_zero(ptn)
#define flash_erase_all_except_xloader(void) emmc_erase_all_except_xloader(void)

/* add for partiton adaptive */
#define flash_get_blknr(pbuf,dwBlkCount,multiBlkSize)  emmc_get_blknr(pbuf,dwBlkCount,multiBlkSize)

#define FLASH_PAGE_SIZE			2048
#define FLASH_PAGE_BITS			11

#define EMMC_BLOCK_SIZE			512
#define CMD_LINE_SIZE			192

/* sd card */
int sd_init(void);
long file_fat_read(const char *filename, void *buffer, unsigned long maxsize);

#endif
