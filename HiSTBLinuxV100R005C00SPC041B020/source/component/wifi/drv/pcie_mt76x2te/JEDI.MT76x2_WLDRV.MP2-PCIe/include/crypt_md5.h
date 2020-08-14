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


/****************************************************************************
    Module Name:
    MD5

    Abstract:
    RFC1321: The MD5 Message-Digest Algorithm

    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2008/11/24      Create md5
***************************************************************************/

#ifndef __CRYPT_MD5_H__
#define __CRYPT_MD5_H__


/* Algorithm options */
#define MD5_SUPPORT

#ifdef MD5_SUPPORT
#define MD5_BLOCK_SIZE    64	/* 512 bits = 64 bytes */
#define MD5_DIGEST_SIZE   16	/* 128 bits = 16 bytes */
typedef struct {
	UINT32 HashValue[4];
	UINT64 MessageLen;
	UINT8 Block[MD5_BLOCK_SIZE];
	UINT BlockLen;
} MD5_CTX_STRUC, *PMD5_CTX_STRUC;

VOID RT_MD5_Init(IN MD5_CTX_STRUC * pMD5_CTX);
VOID RT_MD5_Hash(IN MD5_CTX_STRUC * pMD5_CTX);
VOID RT_MD5_Append(IN MD5_CTX_STRUC * pMD5_CTX, IN const UINT8 Message[], IN UINT MessageLen);
VOID RT_MD5_End(IN MD5_CTX_STRUC * pMD5_CTX, OUT UINT8 DigestMessage[]);
VOID RT_MD5(IN const UINT8 Message[], IN UINT MessageLen, OUT UINT8 DigestMessage[]);
#endif /* MD5_SUPPORT */


#endif /* __CRYPT_MD5_H__ */
