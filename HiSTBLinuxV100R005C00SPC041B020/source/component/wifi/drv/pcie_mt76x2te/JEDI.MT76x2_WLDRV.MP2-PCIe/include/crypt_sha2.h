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
    SHA2

    Abstract:
    FIPS 180-2: Secure Hash Standard (SHS)

    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2008/11/24      Create SHA1
    Eddy        2008/07/23      Create SHA256
***************************************************************************/

#ifndef __CRYPT_SHA2_H__
#define __CRYPT_SHA2_H__


/* Algorithm options */
#define SHA1_SUPPORT
#define SHA256_SUPPORT

#ifdef SHA1_SUPPORT
#define SHA1_BLOCK_SIZE    64	/* 512 bits = 64 bytes */
#define SHA1_DIGEST_SIZE   20	/* 160 bits = 20 bytes */
typedef struct _SHA1_CTX_STRUC {
	UINT32 HashValue[5];	/* 5 = (SHA1_DIGEST_SIZE / 32) */
	UINT64 MessageLen;	/* total size */
	UINT8 Block[SHA1_BLOCK_SIZE];
	UINT BlockLen;
} SHA1_CTX_STRUC, *PSHA1_CTX_STRUC;

VOID RT_SHA1_Init(IN SHA1_CTX_STRUC * pSHA_CTX);
VOID RT_SHA1_Hash(IN SHA1_CTX_STRUC * pSHA_CTX);
VOID RT_SHA1_Append(IN SHA1_CTX_STRUC * pSHA_CTX, IN const UINT8 Message[], IN UINT MessageLen);
VOID RT_SHA1_End(IN SHA1_CTX_STRUC * pSHA_CTX, OUT UINT8 DigestMessage[]);
VOID RT_SHA1(IN const UINT8 Message[], IN UINT MessageLen, OUT UINT8 DigestMessage[]);
#endif /* SHA1_SUPPORT */

#ifdef SHA256_SUPPORT
#define SHA256_BLOCK_SIZE   64	/* 512 bits = 64 bytes */
#define SHA256_DIGEST_SIZE  32	/* 256 bits = 32 bytes */
typedef struct _SHA256_CTX_STRUC {
	UINT32 HashValue[8];	/* 8 = (SHA256_DIGEST_SIZE / 32) */
	UINT64 MessageLen;	/* total size */
	UINT8 Block[SHA256_BLOCK_SIZE];
	UINT BlockLen;
} SHA256_CTX_STRUC, *PSHA256_CTX_STRUC;

VOID RT_SHA256_Init(IN SHA256_CTX_STRUC * pSHA_CTX);
VOID RT_SHA256_Hash(IN SHA256_CTX_STRUC * pSHA_CTX);
VOID RT_SHA256_Append(IN SHA256_CTX_STRUC * pSHA_CTX, IN const UINT8 Message[], IN UINT MessageLen);
VOID RT_SHA256_End(IN SHA256_CTX_STRUC * pSHA_CTX, OUT UINT8 DigestMessage[]);
VOID RT_SHA256(IN const UINT8 Message[], IN UINT MessageLen, OUT UINT8 DigestMessage[]);
#endif /* SHA256_SUPPORT */


#endif /* __CRYPT_SHA2_H__ */
