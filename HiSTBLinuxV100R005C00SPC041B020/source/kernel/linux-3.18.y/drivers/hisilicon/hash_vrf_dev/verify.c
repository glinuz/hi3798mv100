/******************************************************************************
 *    COPYRIGHT (C) 2013 Hisilicon
 *    All rights reserved.
 * ***
 *    Create by wangjian 2016-04
 *
******************************************************************************/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <linux/blkdev.h>
#include <linux/hdreg.h>
#include <linux/bio.h>
#include <linux/genhd.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <asm/sizes.h>
#include <linux/reboot.h>
#include <linux/suspend.h>
#include <linux/crypto.h>
#include <linux/scatterlist.h>
#include <asm/setup.h>


#define VERBLOCK_SIZE		512
#define HASG_LEN			32
#define HAS_PART			"shatable"
#define VERIFY_BLOCK_NAME   "verify-block"
#define VERIFY_BLOCK_COUNT	1
#define TOUSY
/******************************************************************************/

struct verify_block_device {
	int idx;             /* 索引号 */
	char hhfile_name[64];     /* 保存 hash 值的文件名称 */
	char blkdev_name[64];   /* 被校验的块设备路径名称 */
	char blkname[32];
	char verify_tmp[HASG_LEN];
	char hash_tmp[HASG_LEN];

	struct gendisk *gd;  /* 映射后的块设备 */
	spinlock_t queue_lock;
	struct request_queue *queue;
	struct mutex dev_mutex;

	struct request_queue *rq;
	struct workqueue_struct *wq;
	struct work_struct work;

	struct file *blkdev_file; /* 块设备文件句柄 */
	struct file *blkdev_fp;

	int refcnt;  /* 映射后的块设备打开次数 */
};


static int vblk_dev_major = 0;
static struct verify_block_device verify_block_devices[VERIFY_BLOCK_COUNT];
/******************************************************************************/

#ifndef MODULE
static int part_find_num(char *name)
{
	char * b;
	char * tmp;
	char * blkdevparts;
	char * haspart;
	char * buffer[128];
	int	   num = 0;
	char * has_st ;

	blkdevparts = strstr(boot_command_line, "blkdevparts");
	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,blkdevparts,sizeof(buffer));
	b = strim(buffer);
	while (b) {
		tmp = strsep(&b, ",");
		if (!tmp)
			continue;
		num++;
		if(strstr(tmp,name))
			break;
	}
	return num ;
}
/******************************************************************************/
// vmxfy=system:/hastable/verify0,cache:/hastable/verfy1

static int __init early_verify_paramter(char *p)
{
	int i;
	char * b;
	char * tmp_buf;
	char * buffer[128];
	memset(buffer,0,sizeof(buffer));
	memcpy(buffer,p,sizeof(buffer));
	b = strim(buffer);

	for (i = 0; i < VERIFY_BLOCK_COUNT ; i++) {
		tmp_buf = strsep(&b, ":,");
		if(!tmp_buf)
			continue ;
		sprintf(verify_block_devices[i].blkdev_name,"/dev/block/mmcblk0p%d",part_find_num(tmp_buf));
		sprintf(verify_block_devices[i].blkname,"%s",tmp_buf);
		tmp_buf = strsep(&b, ":,");
		if(!tmp_buf)
			continue ;
		sprintf(verify_block_devices[i].hhfile_name,"%s",tmp_buf);
	}
	return 0;
}
early_param("vmxfy", early_verify_paramter);
#endif
/******************************************************************************/

int init_verify_file(struct verify_block_device *vbdev)
{
	vbdev->blkdev_fp= filp_open(vbdev->hhfile_name, O_RDONLY | O_LARGEFILE, 0600);
	if (IS_ERR(vbdev->blkdev_fp)) {
		pr_err("can't open block device '%s'.\n", vbdev->hhfile_name);
		return -1;
	}
	return 0;
}
/******************************************************************************/

static int vblk_dev_open(struct block_device *bdev, fmode_t mode)
{
	int ret;
	loff_t size;
	struct file *blkdev_file;
	struct block_device *src_bdev;
	struct verify_block_device *vbdev = bdev->bd_disk->private_data;

	init_verify_file(vbdev);
	mutex_lock(&vbdev->dev_mutex);
	if (vbdev->refcnt > 0)
		goto out_done;

	if (mode & FMODE_WRITE) {
		ret = -EPERM;
		goto out_unlock;
	}

	blkdev_file = filp_open(vbdev->blkdev_name, O_RDONLY | O_LARGEFILE, 0600);
	if (IS_ERR(blkdev_file)) {
		pr_err("can't open block device '%s'.\n", vbdev->blkdev_name);
		ret = -ENODEV;
		goto out_unlock;
	}

	if (!(blkdev_file->f_mode & FMODE_READ)) {
		filp_close(blkdev_file, NULL);
		pr_err("block device '%s' is not readable.\n", vbdev->blkdev_name);
		ret = -EPERM;
		goto out_unlock;
	}
	vbdev->blkdev_file = blkdev_file;
	src_bdev = blkdev_file->private_data;
	size = i_size_read(blkdev_file->f_mapping->host) >> 9;
	set_capacity(vbdev->gd, size);

out_done:
	vbdev->refcnt++;
	mutex_unlock(&vbdev->dev_mutex);
	return 0;

out_unlock:
	mutex_unlock(&vbdev->dev_mutex);
	return ret;
}
/******************************************************************************/

static void vblk_dev_release(struct gendisk *gd, fmode_t mode)
{
	struct verify_block_device *vbdev = gd->private_data;

	mutex_lock(&vbdev->dev_mutex);
	vbdev->refcnt--;
	if (vbdev->refcnt == 0) {
		filp_close(vbdev->blkdev_file, NULL);
	}
	mutex_unlock(&vbdev->dev_mutex);
}
/******************************************************************************/

static int vblk_dev_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	geo->heads = 1;
	geo->cylinders = 1;
	geo->sectors = get_capacity(bdev->bd_disk);
	geo->start = 0;
	return 0;
}
/******************************************************************************/

static const struct block_device_operations vblk_dev_ops = {
	.owner = THIS_MODULE,
	.open = vblk_dev_open,
	.release = vblk_dev_release,
	.getgeo = vblk_dev_getgeo,
};
/******************************************************************************/

static void vblk_dev_request(struct request_queue *rq)
{
	struct verify_block_device *vbdev;
	struct request *req;

	vbdev = rq->queuedata;

	if (!vbdev)
		while ((req = blk_fetch_request(rq)) != NULL)
			__blk_end_request_all(req, -ENODEV);
	else
		queue_work(vbdev->wq, &vbdev->work);
}
/******************************************************************************/

static char string_to_num(char a)
{
	return (a>0x60)?(a-0x57):(a-0x30);
}
/******************************************************************************/

static char compose_num(char a,char b)
{
	a = string_to_num(a) ;
	b = string_to_num(b) ;
	return ((0xf0 & (a<<4))| (0x0f & b));
}
/******************************************************************************/

bool verify_key_right(struct verify_block_device *vbdev  ,unsigned char *hash ,u64 off)
{
	int i;
	kernel_read(vbdev->blkdev_fp, (off<<5), vbdev->verify_tmp, HASG_LEN);
	if(memcmp(vbdev->verify_tmp,hash,HASG_LEN)) {
		pr_err("tmp=  ");
		for(i=0;i<HASG_LEN;i++)
			pr_err("%.2x",vbdev->verify_tmp[i]);
		pr_err("\nhash= ");
		for(i=0;i<HASG_LEN;i++)
			pr_err("%.2x",hash[i]);
		pr_err("\n");
		return false ;
	}
	else
		return true ;
}
/******************************************************************************/

static int sha256_string(char *buf,unsigned char *hash)
{
	int ret;
	int readlen;
	struct crypto_hash *tfm;
	struct hash_desc desc;
	struct scatterlist sg;
	tfm = crypto_alloc_hash("sha256", 0, CRYPTO_ALG_ASYNC);
	if (IS_ERR(tfm)) {
		pr_err("Failed to load transform for sha256 \n");
		return -1;
	}
	desc.tfm = tfm;
	desc.flags = 0;
	ret = crypto_hash_init(&desc);
	if (ret) {
		pr_err("hash init fail.\n");
		return -1;
	}
	sg_init_one(&sg, buf, VERBLOCK_SIZE);
	crypto_hash_digest(&desc, &sg, VERBLOCK_SIZE, hash);
	crypto_free_hash(tfm);

	return 0;
}
/******************************************************************************/

static int vblk_read(struct verify_block_device *vbdev, char *buffer,
		     sector_t sec, int len)
{
	int i;
	u64 pos = sec << 9;
	kernel_read(vbdev->blkdev_file, pos, buffer, len);
	if(len == 0)
		return 0;
#if 1
	for(i=0;i<(len>>9);i++) {
		sha256_string(buffer,vbdev->hash_tmp);
		buffer += VERBLOCK_SIZE;
		if(false == verify_key_right(vbdev,vbdev->hash_tmp,(sec + i))) {
			pr_err("verify key error , system reboot ... !\n");
			kernel_restart(NULL);
			return -1;
		}
	}
#endif
	return 0;
}
/******************************************************************************/

static int do_verify_block_request(struct verify_block_device *vbdev,
				   struct request *req)
{
	int len, ret;
	sector_t sec;

	if (req->cmd_type != REQ_TYPE_FS)
		return -EIO;

	if (blk_rq_pos(req) + blk_rq_cur_sectors(req) >
	    get_capacity(req->rq_disk))
		return -EIO;

	if (rq_data_dir(req) != READ)
		return -ENOSYS;

	sec = blk_rq_pos(req);
	len = blk_rq_cur_bytes(req);

	mutex_lock(&vbdev->dev_mutex);
	ret = vblk_read(vbdev, bio_data(req->bio), sec, len);
	mutex_unlock(&vbdev->dev_mutex);

	return ret;
}
/******************************************************************************/

static void vblk_dev_do_work(struct work_struct *work)
{
	struct verify_block_device *vbdev =
		container_of(work, struct verify_block_device, work);
	struct request_queue *rq = vbdev->rq;
	struct request *req;
	int res;

	spin_lock_irq(rq->queue_lock);

	req = blk_fetch_request(rq);
	while (req) {
		spin_unlock_irq(rq->queue_lock);
		res = do_verify_block_request(vbdev, req);
		spin_lock_irq(rq->queue_lock);
		if (!__blk_end_request_cur(req, res))
			req = blk_fetch_request(rq);
	}

	spin_unlock_irq(rq->queue_lock);
}
/******************************************************************************/

int vblk_dev_create(struct verify_block_device *vbdev, int idx)
{
	int ret;
	struct gendisk *gd;

	mutex_init(&vbdev->dev_mutex);

	gd = alloc_disk(1);
	if (!gd) {
		pr_err("alloc_disk failed");
		return -ENODEV;
	}

	gd->fops = &vblk_dev_ops;
	gd->major = vblk_dev_major;
	gd->first_minor = idx;
	gd->private_data = vbdev;

	set_capacity(gd, VERBLOCK_SIZE);

	sprintf(gd->disk_name, "verify_%s",vbdev->blkname);
	vbdev->gd = gd;

	spin_lock_init(&vbdev->queue_lock);
	vbdev->rq = blk_init_queue(vblk_dev_request, &vbdev->queue_lock);
	if (!vbdev->rq) {
		pr_err("blk_init_queue failed.\n");
		ret = -ENODEV;
		goto out_put_disk;
	}

	vbdev->rq->queuedata = vbdev;
	vbdev->gd->queue = vbdev->rq;

	vbdev->wq = alloc_workqueue("%s", 0, 0, gd->disk_name);
	if (!vbdev->wq) {
		pr_err("alloc_workqueue failed.\n");
		ret = -ENOMEM;
		goto out_free_queue;
	}
	INIT_WORK(&vbdev->work, vblk_dev_do_work);

	add_disk(vbdev->gd);

	pr_info("created verify block %s from %s\n",
		gd->disk_name, vbdev->blkdev_name);

	return 0;

out_free_queue:
	blk_cleanup_queue(vbdev->rq);
out_put_disk:
	put_disk(vbdev->gd);

	vbdev->gd = NULL;

	return ret;
}
/******************************************************************************/

static void verify_block_remove_all(void)
{
	int i;

	for (i = 0; i < VERIFY_BLOCK_COUNT; i++) {
		struct verify_block_device *vbdev = &verify_block_devices[i];

		if (!vbdev->blkdev_name)
			break;

		destroy_workqueue(vbdev->wq);

		del_gendisk(vbdev->gd);
		blk_cleanup_queue(vbdev->rq);

		pr_info("%s released\n", vbdev->gd->disk_name);

		put_disk(vbdev->gd);
		vfree(vbdev->blkdev_name);
		filp_close(vbdev->blkdev_fp,NULL);
	}
}
/******************************************************************************/

static int __init vblk_device_init(void)
{
	int i, ret;

	vblk_dev_major = register_blkdev(0, VERIFY_BLOCK_NAME);
	if (vblk_dev_major < 0)
		return -ENODEV;

	for (i = 0; i < VERIFY_BLOCK_COUNT; i++) {
		struct verify_block_device *vbdev = &verify_block_devices[i];

		if (0 == strlen(vbdev->blkdev_name))
			break;

		ret = vblk_dev_create(vbdev, i);
		if (ret) {
			pr_err("can't create '%s' verify block, err=%d\n",
			       vbdev->blkdev_name, ret);
			goto err_uncreate;
		}
	}

	return 0;

err_uncreate:
	unregister_blkdev(vblk_dev_major, VERIFY_BLOCK_NAME);
	verify_block_remove_all();

	return ret;
}
/******************************************************************************/

static void __exit vblk_device_exit(void)
{
	verify_block_remove_all();
	unregister_blkdev(vblk_dev_major, VERIFY_BLOCK_NAME);
}
/******************************************************************************/

module_init(vblk_device_init);
module_exit(vblk_device_exit);
MODULE_LICENSE("GPL");
