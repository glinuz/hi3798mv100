/*******************************************************************************
* All rights reserved, Copyright (C) huawei LIMITED 2012
*------------------------------------------------------------------------------
* File Name   : tee_nv_info.h
* Description :
* Platform    :
* Author      : qiqingchao
* Version     : V1.0
* Date        : 2013.5.21
* Notes       :
*
*------------------------------------------------------------------------------
* Modifications:
*   Date        Author          Modifications
*******************************************************************************/
/*******************************************************************************
 * This source code has been made available to you by HUAWEI on an
 * AS-IS basis. Anyone receiving this source code is licensed under HUAWEI
 * copyrights to use it in any way he or she deems fit, including copying it,
 * modifying it, compiling it, and redistributing it either with or without
 * modifications. Any person who transfers this source code or any derivative
 * work must include the HUAWEI copyright notice and this paragraph in
 * the transferred software.
*******************************************************************************/
#ifndef _TEE_NV_INFO_H_
#define _TEE_NV_INFO_H_

#define     SB_AES_KEY_LEN                      128
#define     WIDEVINE_NV_WVLOCK_NAME            "WVLOCK"
#define     WIDEVINE_NV_WVLOCK_NUM             208
#define     WIDEVINE_NV_WVLOCK_SIZE            68

typedef enum SECURE_LOCKSTATE_TAG {
	SECURE_LOCKED = 0,
	SECURE_UNLOCKED = 1,
	SECURE_RELOCKED = 2,
} SECURE_LOCKSTATE;

typedef struct SecureBoot_LockInfo_Tag {
	unsigned int user_password[4];      //user pw, used as key, should compare to user pw
	unsigned int unlock_flag[4];       //unlock flag
	unsigned int relock_flag[4];       //relock flag
	unsigned int encrypted_flag[4];    //flag which is encrypted with user pw
	unsigned int lockstate;            //temporary lock state
} SecureBoot_LockInfo;

int handle_nv(SecureBoot_LockInfo *info);

#endif
