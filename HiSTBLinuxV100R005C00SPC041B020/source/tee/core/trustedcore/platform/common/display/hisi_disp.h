/* Copyright (c) 2014-2015, Hisilicon Tech. Co., Ltd. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 */
#ifndef HISI_DISP_H
#define HISI_DISP_H
#include <sre_typedef.h>
#include <boot.h>
#include <debug.h>
#include <sre_sys.h>
#include <sre_mem.h>
#include <sre_hwi.h>
#include <secure_gic_init.h>
#include "stdint.h"
#include "init_ext.h"
#include "sre_securemmu.h"
#include "sre_ticktimer_functions.h"
#include "libhwsecurec/securec.h"

#include "hisi_dss.h"

struct hisi_panel_info {
	uint32_t type;
	uint32_t xres;
	uint32_t yres;
	uint32_t width; //mm
	uint32_t height; //mm
	uint32_t bpp;
	uint32_t fps;
	uint32_t orientation;
};

struct hisifb_data_type {
	uint32_t index;

	uint32_t dss_base;
	uint32_t peri_crg_base;
	uint32_t sctrl_base;
	uint32_t pctrl_base;
	uint32_t mmbuf_crg_base;
	uint32_t noc_dss_base;
	uint32_t mipi_dsi0_base;
	uint32_t mipi_dsi1_base;
	uint32_t pmc_base;

	uint32_t dpe_sec_irq;
	uint32_t sec_rch_idx;
	uint32_t sec_mctl_idx;
	uint32_t mode_cfg;
	uint32_t xres;
	uint32_t yres;
	uint32_t frame_count;

	int secure_status;              //   0:unsecure; 1:secure
	int vactive_start_flag;         //   wait until vactive_flag becomes 1
	int frame_end_flag;
	int disp_lock;
	int first_frame;
	int disp_debug_dump;
	int alpha_enable;

	dss_layer_t layer;
	dss_layer_t layer_prev;

	/* function entry chain */
	int (*fb_cfg_sec)(struct hisifb_data_type *hisifd, int sec_value);
	int (*wait_vactive_flag)(struct hisifb_data_type *hisifd);
	int (*wait_release_flag)(struct hisifb_data_type *hisifd);
	int (*get_disp_info)(struct hisifb_data_type *hisifd, struct hisi_panel_info *pinfo);
	int (*pan_display_sec)(struct hisifb_data_type *hisifd, dss_layer_t *layer);
	void (*set_reg)(uint32_t addr, uint32_t val, unsigned char bw, unsigned char bs);
	int (*panel_power_on)(struct hisifb_data_type *hisifd);
};

extern int hisi_fb_cfg_sec(int value);
extern int hisi_pan_display_sec(dss_layer_t *layer);
extern int hisi_wait_vactive_flag(void);
extern int hisi_wait_release_flag(void);
extern int hisi_get_disp_info(struct hisi_panel_info *pinfo);
int hisi_fb_alpha_set(int value);
#endif
