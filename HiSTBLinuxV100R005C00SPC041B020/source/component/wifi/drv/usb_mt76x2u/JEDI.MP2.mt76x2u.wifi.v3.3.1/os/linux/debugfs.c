/*
 ***************************************************************************
 * MediaTec Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2006, MediaTek Technology, Inc.
 *
 * All rights reserved. MediaTek's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    debugfs.c

Abstract:
    debugfs related subroutines

    Revision History:
    Who         When          What
    --------    ----------    ----------------------------------------------
    Conard Chou   03-10-2015    created
*/

#include <linux/debugfs.h>
#include <linux/slab.h>

#include "rt_config.h"

struct dentry *mt_dfs_dir = NULL;


#define DFS_FILE_OPS(name)						\
static const struct file_operations mt_dfs_##name##_fops = {		\
	.read = mt_##name##_read,					\
	.write = mt_##name##_write,					\
	.open = mt_open_generic,					\
};

#define DFS_FILE_READ_OPS(name)						\
static const struct file_operations mt_dfs_##name##_fops = {		\
	.read = mt_##name##_read,					\
	.open = mt_open_generic,					\
};

#define DFS_FILE_WRITE_OPS(name)					\
static const struct file_operations mt_dfs_##name##_fops = {		\
	.write = mt_##name##_write,					\
	.open = mt_open_generic,					\
};



static int mt_open_generic(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}


static ssize_t mt_info_read(struct file *file, char __user *user_buf,
						size_t count, loff_t *ppos)
{
	ssize_t retval;
	char *buf, *p;
	size_t size = 1024;

	buf = kzalloc(size, GFP_KERNEL);
	p = buf;
	if (!p)
		return -ENOMEM;

	p += sprintf(p, "DRI VER-%s\n", STA_DRIVER_VERSION);
	retval = simple_read_from_buffer(user_buf, count, ppos, buf,
					(size_t) p - (size_t)buf);

	kfree(buf);
	return retval;
}


static ssize_t mt_debug_read(struct file *file, char __user *user_buf,
						size_t count, loff_t *ppos)
{
	ssize_t retval;
	char *buf;
	size_t size = 1024;
	size_t len = 0;

	buf = kzalloc(size, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;

	len = sprintf(buf, "debug read\n");
	retval = simple_read_from_buffer(user_buf, count, ppos, buf, len);

	kfree(buf);
	return retval;
}

static ssize_t mt_statlog_read(struct file *file, char __user *user_buf,
						size_t count, loff_t *ppos)
{
	ssize_t retval;
	char *buf, *p;
	unsigned long size = 2048;
	PRTMP_ADAPTER pad = (PRTMP_ADAPTER) file->private_data;

	buf = kzalloc(size, GFP_KERNEL);

	p = buf;
	if (!p)
		return -ENOMEM;

	memset(p, 0x00, size);
	RtmpIoctl_rt_private_get_statistics(pad, p, size);
	p = (char *)((size_t) p + strlen(p) + 1);   /* 1: size of '\0' */

	retval = simple_read_from_buffer(user_buf, count, ppos, buf,
					(size_t) p - (size_t)buf);

	kfree(buf);
	return retval;
}



static ssize_t mt_chinfo_read(struct file *file, char __user *user_buf,
						size_t count, loff_t *ppos)
{
	ssize_t retval;
	char *buf, *p;
	unsigned long size = 8192;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) file->private_data;
	/* char reg_domain[5][7] = {"CE", "FCC", "JAP", "JAP_W53", "JAP_W56"}; */
	PCFG80211_CTRL cfg80211_ctrl = NULL;
	struct _CH_FLAGS_BEACON *flags_updated;
	CFG80211_CB *cfg80211_cb = NULL;
	struct wiphy *pWiphy = NULL;
	struct ieee80211_supported_band *band2g = NULL;
	struct ieee80211_supported_band *band5g = NULL;
	UINT32 band_id;
	UINT32 num_ch, ch_id;
	INT sup_band = 0;
	INT start_band = 0;
	CFG80211_BAND band_info;
	int i = 0;

	buf = kzalloc(size, GFP_KERNEL);

	p = buf;
	if (!p)
		return -ENOMEM;

	memset(p, 0x00, size);
	pWiphy = pAd->net_dev->ieee80211_ptr->wiphy;
	/* Init */
	RTMP_DRIVER_80211_BANDINFO_GET(pAd, &band_info);
	if (band_info.RFICType == 0)
		band_info.RFICType = RFIC_24GHZ | RFIC_5GHZ;
	/* 1. Calcute the Channel Number */
	if (band_info.RFICType & RFIC_5GHZ)
		band5g = pWiphy->bands[IEEE80211_BAND_5GHZ];
	band2g = pWiphy->bands[IEEE80211_BAND_2GHZ];

	start_band = (band_info.RFICType & RFIC_24GHZ)?0:1;
	if (band_info.RFICType & RFIC_24GHZ)
		sup_band++;

	if (band_info.RFICType & RFIC_5GHZ)
		sup_band++;

	sprintf(p + strlen(p), "Wiphy ch info\n");
	for (band_id = start_band; band_id < sup_band; band_id++) {
		struct ieee80211_supported_band *band_tmp = NULL;
		struct ieee80211_channel *channels = NULL;

		if (band_id == 0)
			band_tmp = band2g;
		else
			band_tmp = band5g;

		if (!band_tmp)
			goto fail;
		num_ch = band_tmp->n_channels;
		channels = band_tmp->channels;

		if (!channels)
			goto fail;

		for (ch_id = 0; ch_id < num_ch; ch_id++) {
			struct ieee80211_channel *ch = &channels[ch_id];
			UINT32 chidx = 0;
			chidx = ieee80211_frequency_to_channel(ch->center_freq);

			sprintf(p + strlen(p), "channel %d,\t", chidx);
			sprintf(p + strlen(p), "flag %x: ", ch->flags);
			if (ch->flags & CHANNEL_DISABLED) {
				sprintf(p + strlen(p), "%s\n", "DISABLED");
				continue;
			}

			if (ch->flags & CHANNEL_PASSIVE_SCAN)
				sprintf(p + strlen(p), "%s ", "PASSIVE_SCAN");
			if (ch->flags & CHANNEL_RADAR)
				sprintf(p + strlen(p), "%s ", "RADAR");
			sprintf(p + strlen(p), "\n");
		}
	}
	sprintf(p + strlen(p), "\n");

	RTMP_DRIVER_80211_CB_GET(pAd, &cfg80211_cb);
	flags_updated = cfg80211_cb->ch_flags_by_beacon;

	sprintf(p + strlen(p), "Flags by beacon2\n");
	for (i = 0; i < Num_Cfg80211_Chan; i++) {
		if (!&flags_updated[i])
			break;
		sprintf(p + strlen(p), "channel %d\t", flags_updated[i].ch);
		sprintf(p + strlen(p), "flag %x: ", flags_updated[i].flags);
		if (flags_updated[i].flags & CHANNEL_PASSIVE_SCAN)
			sprintf(p + strlen(p), "%s ", "PASSIVE_SCAN");
		if (flags_updated[i].flags & CHANNEL_RADAR)
			sprintf(p + strlen(p), "%s ", "RADAR");
		sprintf(p + strlen(p), "\n");
	}
	sprintf(p + strlen(p), "\n");
	sprintf(p + strlen(p), "DFS channel skip: %s\n"
		, (pAd->CommonCfg.bIEEE80211H_PASSIVE_SCAN)?"NO":"YES");
	for (i = 0; i < pAd->ChannelListNum; i++) {
		sprintf(p + strlen(p), "channel %d,\t", pAd->ChannelList[i].Channel);
		sprintf(p + strlen(p), "DFS:%s\t", (pAd->ChannelList[i].DfsReq)?"YES,":"NO,\t");
		sprintf(p + strlen(p), "flag: ");
		if (pAd->ChannelList[i].Flags & CHANNEL_PASSIVE_SCAN)
			sprintf(p + strlen(p), "%s ", "PASSIVE_SCAN");
		if (pAd->ChannelList[i].Flags & CHANNEL_RADAR)
			sprintf(p + strlen(p), "%s ", "RADAR");
		sprintf(p + strlen(p), "\n");
	}

fail:
	cfg80211_ctrl = &pAd->cfg80211_ctrl;
	if (cfg80211_ctrl->pCfg80211ChanList) {
		sprintf(p + strlen(p), "CFG Scan Channel List@%u: ", cfg80211_ctrl->Cfg80211ChanListLen);
		for (i = 0; i < cfg80211_ctrl->Cfg80211ChanListLen; i++)
			sprintf(p + strlen(p), "%u ", cfg80211_ctrl->pCfg80211ChanList[i]);
	} else
		sprintf(p + strlen(p), "No Scan Channel list");
	sprintf(p + strlen(p), "\n");
	p = (char *)((size_t) p + strlen(p) + 1);   /* 1: size of '\0' */

	retval = simple_read_from_buffer(user_buf, count, ppos, buf,
					(size_t) p - (size_t)buf);
	kfree(buf);
	return retval;
}

static ssize_t mt_chinfo_write(struct file *file, const char __user *user_buf,
				size_t count, loff_t *ppos)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) file->private_data;
	char buf[16] = {0};
	PSTRING op = NULL;
	PSTRING val = NULL;
	int err;
	long lval = 0;

	if (count > 16)
		DBGPRINT(RT_DEBUG_ERROR, ("%s:invalid param\n", __func__));

	if (copy_from_user(buf, user_buf, min(count, sizeof(buf))))
		return -EFAULT;

	buf[strlen(buf)-1] = '\0';
	op = rstrtok(buf, " ");
	val = rstrtok(NULL, " ");

	if (!op || !val)
		goto err0;

	err = os_strtol(val, 10, &lval);

	if (err)
		goto err0;

	pAd->CommonCfg.bIEEE80211H_PASSIVE_SCAN = (lval)?TRUE:FALSE;
err0:
	return count;
}

/* Add debugfs file operation */
DFS_FILE_READ_OPS(info);
DFS_FILE_READ_OPS(debug);
DFS_FILE_READ_OPS(statlog);


DFS_FILE_OPS(chinfo);
/*
 * create debugfs root dir
 */
void mt_debugfs_init(void)
{
	char *dfs_dir_name = "mtwifi";

	if (!mt_dfs_dir)
		mt_dfs_dir = debugfs_create_dir(dfs_dir_name, NULL);


	if (!mt_dfs_dir) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create %s dir fail!\n", __func__, dfs_dir_name));
		return;
	}
}

/*
 * create debugfs directory & file for each device
 */
void mt_dev_debugfs_init(PRTMP_ADAPTER pad)
{
	POS_COOKIE pobj = (POS_COOKIE) pad->OS_Cookie;

	DBGPRINT(RT_DEBUG_INFO, ("device debugfs init start!\n"));

	/* create device dir */
	pobj->debugfs_dev = debugfs_create_dir(pad->net_dev->name, mt_dfs_dir);

	if (!pobj->debugfs_dev) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create %s dir fail!\n",
					__func__, pad->net_dev->name));
		return;
	}
	/* Add debugfs file */
	if (!debugfs_create_file("info", 0644, pobj->debugfs_dev, pad, &mt_dfs_info_fops)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create info file fail!\n", __func__));
		goto LABEL_CREATE_FAIL;
	}

	if (!debugfs_create_file("debug", 0644, pobj->debugfs_dev, pad, &mt_dfs_debug_fops)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create debug file fail!\n", __func__));
		goto LABEL_CREATE_FAIL;
	}

	if (!debugfs_create_file("statlog", 0644, pobj->debugfs_dev, pad, &mt_dfs_statlog_fops)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create statistics file fail!\n", __func__));
		goto LABEL_CREATE_FAIL;
	}

	if (!debugfs_create_file("chinfo", 0644, pobj->debugfs_dev, pad, &mt_dfs_chinfo_fops)) {
		DBGPRINT(RT_DEBUG_ERROR, ("%s: create channel info file fail!\n", __func__));
		goto LABEL_CREATE_FAIL;
	}

	DBGPRINT(RT_DEBUG_INFO, ("debugfs init finish!\n"));
	return;

LABEL_CREATE_FAIL:
	debugfs_remove_recursive(pobj->debugfs_dev);
	pobj->debugfs_dev = NULL;
}

/*
 * remove debugfs directory and files for each device
 */
void mt_dev_debugfs_remove(PRTMP_ADAPTER pad)
{
	POS_COOKIE pobj = (POS_COOKIE) pad->OS_Cookie;

	if (pobj->debugfs_dev != NULL) {
		debugfs_remove_recursive(pobj->debugfs_dev);
		pobj->debugfs_dev = NULL;
		DBGPRINT(RT_DEBUG_INFO, ("debugfs remove finsih!\n"));
	}
}

/*
 * remove debugfs root dir
 */
void mt_debugfs_remove(void)
{
	if (mt_dfs_dir != NULL) {
		debugfs_remove(mt_dfs_dir);
		mt_dfs_dir = NULL;
	}
}
