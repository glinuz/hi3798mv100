/*******************************************************************
* (c) Copyright 2011-2012 Discretix Technologies Ltd.              *
* This software is protected by copyright, international           *
* treaties and patents.                                            *
* Use of this Software as part of or with the Discretix CryptoCell *
* or Packet Engine products is governed by the products'           *
* commercial end user license agreement ("EULA").                  *
* It is possible that copies of this Software might be distributed *
* under some type of GNU General Public License ("GPL").           *
* Notwithstanding any such distribution under the terms of GPL,    *
* GPL does not govern the use of this Software as part of or with  *
* the Discretix CryptoCell or Packet Engine products, for which a  *
* EULA is required.                                                *
* If the product's EULA allows any copy or reproduction of this    *
* Software, then such copy or reproduction must include this       *
* Copyright Notice as well as any other notices provided           *
* thereunder.                                                      *
********************************************************************/


#ifndef _DX_CC_DEFS_H_
#define _DX_CC_DEFS_H_

#define DX_INT32_MAX 0x7FFFFFFFL

typedef enum {
	DX_USER_KEY = 0,
	DX_ROOT_KEY = 1,
	DX_PROVISIONING_KEY = 2,
	DX_XOR_HDCP_KEY = 3,
	DX_APPLET_KEY = 4,
	DX_SESSION_KEY = 5,
	DX_END_OF_KEYS = DX_INT32_MAX,
} DX_CRYPTO_KEY_TYPE_t;


#endif
