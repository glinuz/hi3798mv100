

#include <common.h>
#include <command.h>
#include <mmc.h>
#include <asm/io.h>

extern u32 emmc_ds_rp;
extern u32 emmc_ds_fp;

u32 mmc_wr_pattern[12] = {
	0x00ff00ff,
	0xff00ff00,
	0x5a5a5a5a,
	0xa5a5a5a5,
	0x01010101,
	0x02020202,
	0x04040404,
	0x08080808,
	0x10101010,
	0x20202020,
	0x40404040,
	0x80808080
};

typedef struct{
	struct mmc *mmc;
	int dev;
	void *addr_in;
	void *addr_out;
	u32 cnt;
	int blk;
}mmc_wr_val_t;

int memcmp_pr(const void * cs,const void * ct,size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;
	printf("mem compare: src 0x%p, dst 0x%p\n", cs, ct);
	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0) {
			printf("differ at src 0x%p[0x%X], dst 0x%p[0x%X]\n", su1, *su1, su2, *su2);
			break;
		}
	return res;
}
static int mmc_check_wr(mmc_wr_val_t* val, u32 pattern, int cmp)
{
	unsigned long long start_ticks, end_ticks;
	unsigned long long size = 0;
	mmc_wr_val_t *p = val;
	u32 pat = pattern;
	u32 n;
	int i;

	size = p->mmc->write_bl_len * (p->cnt);

	dcache_enable(0);
	for (i = 0; i < size/sizeof(pat); i++) {
		*((u32 *)(p->addr_in) + i) = pat;
	}
	dcache_disable();
	/*
	printf("IN: i=%d, *0x%08X=0x%08X, *0x%08X=0x%08X, *0x%08X=0x%08X, *0x%08X=0x%08X\n",
		i, (u32)(p->addr_in + 0), *((u32 *)(p->addr_in) + 0),
		(u32)(p->addr_in) + 4, *((u32 *)(p->addr_in) + 1),
		(u32)(p->addr_in + (i - 2) * 4), *((u32 *)(p->addr_in) + i - 2),
		(u32)(p->addr_in + (i - 1)*4), *((u32 *)(p->addr_in) + i - 1));
	*/
	//memset(p->addr_in, pat, size);

	printf("\nMMC write: dev # %d, block # 0x%x, count 0x%x, pattern 0x%X\n",p->dev, p->blk,
		p->cnt, pat);
	start_ticks = get_ticks();
	n = p->mmc->block_dev.block_write(p->dev, p->blk, p->cnt, p->addr_in);
	end_ticks = get_ticks();

	if (n != p->cnt) {
		printf("0x%x blocks written: %s\n", n, "ERROR");
		return -1;
	}

	printf("0x%x blocks writen: %s, write speed: %llu KB/s\n", n, "OK",
		((size >> 10) * CONFIG_SYS_HZ)/(end_ticks - start_ticks));

	printf("MMC read: dev # %d, block # 0x%x, count 0x%x, pattern 0x%X\n",
		p->dev, p->blk,	p->cnt, pat);
	start_ticks = get_ticks();
	n = p->mmc->block_dev.block_read(p->dev, p->blk, p->cnt, p->addr_out);
	end_ticks = get_ticks();
	/* flush cache after read */
	flush_cache((ulong)(p->addr_out), size); /* FIXME */
	if (n != p->cnt) {
	    printf("0x%x blocks read: %s\n", n, "ERROR");
	    return -1;
	}

	dcache_enable(0);
	for (i = 0; i < size/sizeof(pat); i++) {
		*((u32 *)(p->addr_out) + i) = pat;
	}
	dcache_disable();

	/*
	printf("OUT: i=%d, *0x%08X=0x%08X, *0x%08X=0x%08X, *0x%08X=0x%08X, *0x%08X=0x%08X\n",
		i, (u32)(p->addr_out + 0), *((u32 *)(p->addr_out) + 0),
		(u32)(p->addr_out) + 4, *((u32 *)(p->addr_out) + 1),
		(u32)(p->addr_out + (i - 2) * 4), *((u32 *)(p->addr_out) + i - 2),
		(u32)(p->addr_out + (i - 1)*4), *((u32 *)(p->addr_out) + i - 1));
	*/

	printf("0x%x blocks read: %s, read speed: %llu KB/s\n", n, "OK",
		((size >> 10) * CONFIG_SYS_HZ)/(end_ticks - start_ticks));


	if (cmp) {
		dcache_enable(0);
		n = memcmp_pr(p->addr_in, p->addr_out, size);
		dcache_disable();
		if (n) {
			printf("cmp check fail\n");
			return -1;
		}
		printf("cmp check ok\n");

	}


	return 0;
}

int mmc_test_wr(mmc_wr_val_t* val)
{
	int ret = 0;
	int j;

	printf("MMC R/W test start\n");
	for(j = 0; j < sizeof(mmc_wr_pattern)/sizeof(mmc_wr_pattern[0]); j++) {
		ret = mmc_check_wr(val, mmc_wr_pattern[j], 1);
		if (ret) {
			 printf("fail!\n");
			 hang();
			 return ret;
		}
	}
	printf("done!\n");
	return ret;
}

#ifdef CONFIG_ARCH_HI3798CV2X
int mmc_check_hs400_ds(mmc_wr_val_t* emmc_wr_p)
{
	int i;
	u32 dll_backup = 0;

	printf("MMC check HS400 DS start\n");

#define EMMC_DLL_DLLSSEL 0x90101

	dll_backup = readl(REG_BASE_CRG + REG_PERI_CRG164);
	writel(EMMC_DLL_DLLSSEL, REG_BASE_CRG + REG_PERI_CRG164);
	printf("HS400 DS phase window[%d, %d]\n", emmc_ds_rp, emmc_ds_fp);
	for (i = emmc_ds_rp; i <= emmc_ds_fp; i++) {
		unsigned int val = readl(REG_BASE_CRG + REG_PERI_CRG164);
		val &= ~(0xFF<<8);
		val |= i<<8;
		writel(val, REG_BASE_CRG + REG_PERI_CRG164);
		printf("REG 0x%X = 0x%X\n", REG_BASE_CRG + REG_PERI_CRG164 , readl(REG_BASE_CRG + REG_PERI_CRG164));
		if (mmc_check_wr(emmc_wr_p, 0x5a5a5a5a, 0)) {
			printf("fail!\n");
			hang();
			return -1;
		}
	}

	printf("done!\n");
	writel(dll_backup, REG_BASE_CRG + REG_PERI_CRG164);
	return 0;
}
#endif

int mmc_test(struct mmc *mmc, int dev, void *addr_in, void *addr_out, int blk, u32 cnt)
{
	mmc_wr_val_t mmc_wr_val;
	int ret = -1;

	mmc_wr_val.mmc = mmc;
	mmc_wr_val.dev = dev;
	mmc_wr_val.addr_in = addr_in;
	mmc_wr_val.addr_out = addr_out;
	mmc_wr_val.cnt = cnt;
	mmc_wr_val.blk = blk;

	ret = mmc_test_wr(&mmc_wr_val);
	if (ret)
		goto error;
#ifdef CONFIG_ARCH_HI3798CV2X
	if (mmc->card_caps & MMC_MODE_HS400) {
		ret = mmc_check_hs400_ds(&mmc_wr_val);
		if (ret)
			goto error;
	}
#endif
	return 0;
error:
	return -1;
}

