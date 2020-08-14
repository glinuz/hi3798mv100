/* Copyright (C), 1988-2012, Hisilicon Co,.Ltd
 * File name: cmd_bootss.h
 * Author:     Version: 0.0.1    Date: 2012.09.05
 * Description: upgrade module header file.
 *
 * History:
 * Date          Who          Case                Description
 * 2014.01.29    l00288709    DTS2013072205801    fastboot feature add
 **************************************************************************************************/

#include <common.h>
#include <mmc.h>
unsigned int hibernate_qbboot_addr;

#define SS_DBG
#ifdef SS_DBG
#define ss_dbg(x...) printf(x)
#else
#define ss_dbg(x...)
#endif

struct sscomp_block {
	unsigned int org_size;
	unsigned int comp_size;
};

union sscomp_header {
	struct {
#define COMP_TYPE_LZO 2
#define COMP_TYPE_LZMA 3
#define COMP_TYPE_BZIP2 4
		int comp_type;

		int meta_blocks;
		int meta_pages;
		int data_blocks;
		int data_pages;
		unsigned int decomp_block_size;
		int image_ofs;

		int crc32;
#define SSCOMP_MAGIC_4 "SSCOMP#4"
		char magic[8];
		unsigned char sha1sum[20];

#define COMP_BLK_LEGACY 0
#define COMP_BLK_FIXED 1
#define COMP_BLK_DYNAMIC 2
		int block_type;

		unsigned long ctl_func;
		unsigned long ctl_data;
	} info;
	struct {
		unsigned char c[128];
#define MAX_COMP_BLOCKS ((4096-128)/sizeof(struct sscomp_block))
		struct sscomp_block blocks[MAX_COMP_BLOCKS];
	} b;
	unsigned char c[4096];
};

struct qb_param{
	unsigned long qbdata_addr;
	unsigned long qb_text_base;
	unsigned int  qb_reserve_size;
};

#define PAGE_SIZE 4096
typedef void *(*qb_entry)(struct qb_param param);
extern char qb_cmd_start[];
extern char qb_cmd_end[];
/***************************************************************************************************/

/*qb.bin is in boot*/
/*static void * get_qb_entry(){
	char *src_ptr = NULL;
	char *dst_ptr;
	unsigned int length = 0;

	src_ptr = qb_cmd_start;
	dst_ptr = (char *)(0x40C00000);
	length  = qb_cmd_end - src_ptr;

	printf("src_ptr = %x, dst_ptr = %x, length = %d\n"
		, *src_ptr, *dst_ptr, length);
	if (!src_ptr || !length) {
		printf("qb_bootss is unsupport.\n");
		return NULL;
	}
	memcpy(dst_ptr, src_ptr, length);
	return (void *) dst_ptr;
}*/

int find_hibernate_part(uint64_t *part_offset, char *part_name)
{
	char *s = getenv("bootargs");
	if (!s)
		return -1;
	char *var = strstr(s, "blkdevparts=");
	if (!var)
		return -1;
	uint64_t size;

	int result = find_flash_part(var, "mmcblk0", part_name, part_offset, &size);

	return result;
}

#define HIBERNATE_TAG_SIZE 128
u64 part_ofs;
u64 qbdata_ofs;
int dev_read_page(struct mmc *mymmc, unsigned long page_i, int page_nr, void *dst)
{

	int dev = simple_strtoul("0", NULL, 10);
	u32 qbdata_flag = simple_strtoul(&part_ofs,NULL,16);
	u32 mypart_ofs = ((u64)part_ofs)/512;
	void *addr = dst;
	u32 cnt = page_nr*8;
	u32 n;
	u32 blk = mypart_ofs + 8*page_i;

	mymmc = find_mmc_device(mymmc);

	if (!mymmc)
		return 1;

	ss_dbg("\nMMC read: dev # %d, block # %d, count %d ... ",
		dev, blk, cnt);

	mmc_init(mymmc);

	n = mymmc->block_dev.block_read(dev, blk, cnt, addr);

	/* flush cache after read */
	flush_cache((ulong)addr, cnt * 512); /* FIXME */

	printf("%d blocks read: %s\n",
		n, (n == cnt) ? "OK" : "ERROR");

	if (n == cnt)
		return 1;
	else
		return 0;

}

#define STACK_SIZE 0x4000
#define QBDATA_PAGE_NUM 1024
/*read qb.bin from emmc*/
static void * get_qb_entry(){
	char *src_ptr = NULL;
	char *dst_ptr;
	unsigned int length = 0;

	u64 qbboot_ofs;

	int result = find_hibernate_part(&qbboot_ofs, "qbboot");

	if (qbboot_ofs < 1) {
		printf("find qbboot faild!!!\n\n");
		return NULL;
	}

	void *mydst = hibernate_qbboot_addr + STACK_SIZE;
	part_ofs = qbboot_ofs;

	printf("qbdata's offset = %llx\n\n",qbboot_ofs);

	struct mmc *mydev;
	int n = dev_read_page(mydev, 0, QBDATA_PAGE_NUM, mydst);
	if (n < 1) {
		printf("\nread qbboot faild!!! no qb\n");
		return NULL;
	}

	return (void *) mydst;
}

int before_jump(void){
	qb_entry entry;

	entry = (qb_entry)get_qb_entry();
	if(!entry)
		return -1;
	printf("bootss is Starting...\n\n");
	printf ("## qb's entry: 0x%08X\n\n",(unsigned int)entry);

	struct qb_param param;


	param.qb_text_base = hibernate_qbboot_addr;
	param.qb_reserve_size = CONFIG_HIBERNATE_QBBOOT_SIZE;

	printf("qbdata's offset = %llx\n",part_ofs);
	if(part_ofs > 0){
		param.qbdata_addr = qbdata_ofs;
		entry(param);
	}
	return 1;//no up here
}

static inline void print_comp_info(const union sscomp_header *sscomp, int v)
{
	ss_dbg("Compress Header:\n");
	ss_dbg("decomp block size:   %u\n", sscomp->info.decomp_block_size);
	ss_dbg("   resume addr:     0x%8lx\n", sscomp->info.ctl_func);
	ss_dbg("   meta pages:     %d\n", sscomp->info.meta_pages);
	ss_dbg("   data pages:     %d\n", sscomp->info.data_pages);

	ss_dbg("%d meta-blocks\n", sscomp->info.meta_blocks);
	ss_dbg("%d data-blocks\n", sscomp->info.data_blocks);
	ss_dbg("%s block type\n", sscomp->info.block_type ? "fix" : "flex");

	const struct sscomp_block *block = &sscomp->b.blocks[0];
	int total_blocks = sscomp->info.meta_blocks + sscomp->info.data_blocks;
	int total_org_size = 0;
	int total_comp_size = 0;
	int i;

	for (i = 0; i < total_blocks; i++) {
		unsigned int aligned_comp_size;
		aligned_comp_size =
			(block->comp_size + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
		total_org_size += block->org_size;
		total_comp_size += aligned_comp_size;
		ss_dbg("    block[%2d]: %9d:%d(%d)\n", i,
			block->org_size, block->comp_size, aligned_comp_size);
		block++;
	}
	ss_dbg("     original: %9d bytes\n", total_org_size);
	ss_dbg("   compressed: %9d bytes\n", total_comp_size);
}


int checkout_qbboot(void){
	int ret;
	char tag_buf[HIBERNATE_TAG_SIZE];

	hibernate_qbboot_addr = (unsigned int)get_qbboot_reserve_mem();
	if (!hibernate_qbboot_addr) {
		ret = 0;
		printf("qbboot_reserve_mem is no init");
		return ret;
	}


	if (hibernate_qbboot_addr%0x100000) {
		printf("Error!!Hibernate qbboot address(0x%X) NOT aligned with 1M\n", hibernate_qbboot_addr);
		return 0;
	}

	u64 offset;
	int result = find_hibernate_part(&offset, "qbdata");
	if (!result) {
		printf("Quick booting is closed!\n");
		return 0;
	}

	memset(tag_buf, 0, sizeof(tag_buf));
	snprintf(tag_buf, sizeof(tag_buf), "hibernate_qbboot_addr=%x", hibernate_qbboot_addr);
	set_param_data("qbboot", tag_buf, sizeof(tag_buf));

	printf("\nHibernate Qbboot Address: %x\n", hibernate_qbboot_addr);

	union sscomp_header *sscomp;

	void *mydst = hibernate_qbboot_addr;
	struct mmc *mmc = find_mmc_device(0);

	part_ofs = offset;
	qbdata_ofs = offset;

	struct mmc *mydev;
	int n = dev_read_page(mydev, 0, 1, mydst);
	if (n < 1) {
		printf("\nread sscomp faild!!! no qb\n");
		return 0;
	}

	sscomp = (union sscomp_header *)mydst;
	//print_comp_info(sscomp, 1);
	int meta_blocks = sscomp->info.meta_blocks;
	if(meta_blocks < 1)
		return 0;

	before_jump();

	return 1;

}

int do_bootss(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]){

	//checkout_qbboot();
	//before_jump();
}
U_BOOT_CMD(
	bootss, CONFIG_SYS_MAXARGS, 1,
	do_bootss,
	"boot from snapshot image", "[1 [1 [1 [1[1 [1]]]]] - number of arguments meaning verbosity"
	);

/**************************************************************************************************/


