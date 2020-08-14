/*
 ***************************************************************************
 * MediaTek Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2004, MediaTek Technology, Inc.
 *
 * All rights reserved. MediaTek's source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek Tech. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek Technology, Inc. is obtained.
 ***************************************************************************

    Module Name:
    debugfs.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
    Conard    03-16-2015
*/



#ifndef __DEBUGFS_H__
#define __DEBUGFS_H__
void mt_debugfs_init(void);
void mt_dev_debugfs_init(PRTMP_ADAPTER);
void mt_dev_debugfs_remove(PRTMP_ADAPTER);
void mt_debugfs_remove(void);
#endif

