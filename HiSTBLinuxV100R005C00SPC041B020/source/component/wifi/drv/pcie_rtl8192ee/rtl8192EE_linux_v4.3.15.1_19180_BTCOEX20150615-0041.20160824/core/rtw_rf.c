/******************************************************************************
 *
 * Copyright(c) 2007 - 2011 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/
#define _RTW_RF_C_

#include <drv_types.h>
#include <hal_data.h>

u8 center_ch_5g_all[CENTER_CH_5G_ALL_NUM] = {
		36, 38, 40, 42, 44, 46, 48,			/* Band 1 */
		52, 54, 56, 58, 60, 62, 64,			/* Band 2 */
		100, 102, 104, 106, 108, 110, 112,	/* Band 3 */
		116, 118, 120, 122, 124, 126, 128,	/* Band 3 */
		132, 134, 136, 138, 140, 142, 144,	/* Band 3 */
		149, 151, 153, 155, 157, 159, 161,	/* Band 4 */
		165, 167, 169, 171, 173, 175, 177};	/* Band 4 */

u8 center_ch_5g_20m[CENTER_CH_5G_20M_NUM] = {
	36, 40, 44, 48,
	52, 56, 60, 64,
	100, 104, 108, 112, 116, 120, 124, 128, 132, 136, 140, 144,
	149, 153, 157, 161, 165, 169, 173, 177
};

u8 center_ch_5g_40m[CENTER_CH_5G_40M_NUM] = {38, 46, 54, 62, 102, 110, 118, 126, 134, 142, 151, 159, 167, 175};

u8 center_ch_5g_80m[CENTER_CH_5G_80M_NUM] = {42, 58, 106, 122, 138, 155, 171};

struct ch_freq {
	u32 channel;
	u32 frequency;
};

struct ch_freq ch_freq_map[] = {
	{1, 2412},{2, 2417},{3, 2422},{4, 2427},{5, 2432},
	{6, 2437},{7, 2442},{8, 2447},{9, 2452},{10, 2457},
	{11, 2462},{12, 2467},{13, 2472},{14, 2484},
	/*  UNII */
	{36, 5180},{40, 5200},{44, 5220},{48, 5240},{52, 5260},
	{56, 5280},{60, 5300},{64, 5320},{149, 5745},{153, 5765},
	{157, 5785},{161, 5805},{165, 5825},{167, 5835},{169, 5845},
	{171, 5855},{173, 5865},
	/* HiperLAN2 */
	{100, 5500},{104, 5520},{108, 5540},{112, 5560},{116, 5580},
	{120, 5600},{124, 5620},{128, 5640},{132, 5660},{136, 5680},
	{140, 5700},
	/* Japan MMAC */
	{34, 5170},{38, 5190},{42, 5210},{46, 5230},
	/*  Japan */
	{184, 4920},{188, 4940},{192, 4960},{196, 4980},
	{208, 5040},/* Japan, means J08 */
	{212, 5060},/* Japan, means J12 */
	{216, 5080},/* Japan, means J16 */
};

int ch_freq_map_num = (sizeof(ch_freq_map) / sizeof(struct ch_freq));

u32 rtw_ch2freq(u32 channel)
{
	u8	i;
	u32	freq = 0;

	for (i = 0; i < ch_freq_map_num; i++)
	{
		if (channel == ch_freq_map[i].channel)
		{
			freq = ch_freq_map[i].frequency;
				break;
		}
	}
	if (i == ch_freq_map_num)
		freq = 2412;

	return freq;
}

u32 rtw_freq2ch(u32 freq)
{
	u8	i;
	u32	ch = 0;

	for (i = 0; i < ch_freq_map_num; i++)
	{
		if (freq == ch_freq_map[i].frequency)
		{
			ch = ch_freq_map[i].channel;
				break;
		}
	}
	if (i == ch_freq_map_num)
		ch = 1;

	return ch;
}

static const char * const _ch_width_str[] = {
	"20MHz",
	"40MHz",
	"80MHz",
	"160MHz",
	"80_80MHz",
	"CHANNEL_WIDTH_MAX",
};

const char *ch_width_str(u8 bw)
{
	bw = (bw >= CHANNEL_WIDTH_MAX) ? CHANNEL_WIDTH_MAX : bw;
	return _ch_width_str[bw];
}

const char rf_path_char(u8 path)
{
	if (path >= RF_PATH_MAX)
		return 'X';
	return 'A' + path;
}

