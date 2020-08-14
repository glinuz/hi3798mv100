/*
  * Copyright (c) 2016 MediaTek Inc.  All rights reserved.
  *
  * This software is available to you under a choice of one of two
  * licenses.  You may choose to be licensed under the terms of the GNU
  * General Public License (GPL) Version 2, available from the file
  * COPYING in the main directory of this source tree, or the
  * BSD license below:
  *
  *     Redistribution and use in source and binary forms, with or
  *     without modification, are permitted provided that the following
  *     conditions are met:
  *
  *      - Redistributions of source code must retain the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer.
  *
  *      - Redistributions in binary form must reproduce the above
  *        copyright notice, this list of conditions and the following
  *        disclaimer in the documentation and/or other materials
  *        provided with the distribution.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
  * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
  * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
  * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  * SOFTWARE.
  */



#ifndef __META_AGENT_H__
#define __META_AGENT_H__

#include "rt_ate.h"
extern CH_FREQ_MAP CH_HZ_ID_MAP[];
extern int CH_HZ_ID_MAP_NUM;
/*
 *	struct for command formats
 */
typedef struct _META_CMD_HDR {
	UINT32 oid;
	UINT32 len_in;		/* For set */
	UINT32 len_out;		/* For query */
	UCHAR data[2048];
} __packed META_CMD_HDR;

typedef struct _PARAM_MTK_WIFI_TEST {
	UINT32 idx;
	UINT32 data;
} __packed PARAM_MTK_WIFI_TEST;

typedef struct _PARAM_CUSTOM_EFUSE_RW {
	UINT32 offset;
	UINT32 data;
} __packed PARAM_CUSTOM_EFUSE_RW;

typedef struct _PARAM_CUSTOM_EEPROM_RW {
	UINT8 method;
	UINT8 idx;
	UINT8 reserved;
	UINT16 data;
} PARAM_CUSTOM_EEPROM_RW, PARAM_CUSTOM_NVRAM_RW;

typedef struct _PARAM_CUSTOM_MCR_RW {
	UINT32 offset;
	UINT32 data;
} __packed PARAM_CUSTOM_MCR_RW;

typedef INT32(*META_CMD_HANDLER) (int ioctl_cmd, struct _RTMP_ADAPTER *pAd,
				  RTMP_IOCTL_INPUT_STRUCT * wrq, META_CMD_HDR * cmd_hdr);
#ifdef RTMP_SDIO_SUPPORT
INT Set_Sdio_Bist_Write(struct _RTMP_ADAPTER *pAd, RTMP_STRING *arg);
INT Set_Sdio_Bist_Read(struct _RTMP_ADAPTER *pAd, RTMP_STRING *arg);
#endif /* endif */
/*
 *	define for command oid
 */
#define MT_META_WIFI_TEST_TABLE_VER 0x01180000

#define OID_CUSTOM_IF_VER				0xFFA0C000
#define OID_CUSTOM_MCR_RW				0xFFA0C801
#define OID_CUSTOM_EEPROM_RW			0xFFA0C803
#define OID_CUSTOM_EFUSE_RW				0xFFA0C805
#define	OID_CUSTOM_TEST_MODE			0xFFA0C901
#define OID_CUSTOM_ABORT_TEST_MODE		0xFFA0C906
#define OID_CUSTOM_MTK_WIFI_TEST		0xFFA0C911
/* NVRAM */
#define OID_CUSTOM_MTK_NVRAM_RW         0xFFA0C941
#define OID_CUSTOM_CFG_SRC_TYPE         0xFFA0C942
#define OID_CUSTOM_EEPROM_TYPE          0xFFA0C943

#define MT_META_WIFI_TEST_CMD_MASK		0x0000001F	/* 32 cmd for 1 set, 0x1F=31 */
#define MT_META_WIFI_TEST_SET_MASK		0x0EFFFFFF	/* 32 bits width, rest 27 bits */

#define SLOT_TIME_SHORT 9
#define PARAM_EEPROM_READ_METHOD_READ           1
#define PARAM_EEPROM_READ_METHOD_GETSIZE        0

/*
 * MACRO
 */
#define FUNC_IDX_GET_IDX(__idx, __func_idx) (__func_idx = __idx & 0x000000FF)
#define FUNC_IDX_GET_SUBFIELD(__idx, __sub) (__sub = ((__idx & 0xff000000)>>24)&0x000000ff)

/* TODO: Need to put into include/rt_os.h when merge the file */
#define MTPRIV_IOCTL_META_SET (SIOCIWFIRSTPRIV + 0x08)
#define MTPRIV_IOCTL_META_QUERY (SIOCIWFIRSTPRIV + 0x09)
#endif /* __META_AGENT_H__ */
