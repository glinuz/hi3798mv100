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
    RC4

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ------------------------------------------
    Eddy        2009/05/13      ARC4
***************************************************************************/

#include "crypt_arc4.h"

/*
========================================================================
Routine Description:
    ARC4 initialize the key block

Arguments:
    pARC4_CTX        Pointer to ARC4 CONTEXT
    Key              Cipher key, it may be 16, 24, or 32 bytes (128, 192, or 256 bits)
    KeyLength        The length of cipher key in bytes

========================================================================
*/
VOID ARC4_INIT(IN ARC4_CTX_STRUC * pARC4_CTX, IN PUCHAR pKey, IN UINT KeyLength)
{
	UINT BlockIndex = 0, SWAPIndex = 0, KeyIndex = 0;
	UINT8 TempValue = 0;

	/*Initialize the block value */
	pARC4_CTX->BlockIndex1 = 0;
	pARC4_CTX->BlockIndex2 = 0;
	for (BlockIndex = 0; BlockIndex < ARC4_KEY_BLOCK_SIZE; BlockIndex++)
		pARC4_CTX->KeyBlock[BlockIndex] = (UINT8) BlockIndex;

	/*Key schedule */
	for (BlockIndex = 0; BlockIndex < ARC4_KEY_BLOCK_SIZE; BlockIndex++) {
		TempValue = pARC4_CTX->KeyBlock[BlockIndex];
		KeyIndex = BlockIndex % KeyLength;
		SWAPIndex = (SWAPIndex + TempValue + pKey[KeyIndex]) & 0xff;
		pARC4_CTX->KeyBlock[BlockIndex] = pARC4_CTX->KeyBlock[SWAPIndex];
		pARC4_CTX->KeyBlock[SWAPIndex] = TempValue;
	}			/* End of for */

}				/* End of ARC4_INIT */

/*
========================================================================
Routine Description:
    ARC4 encryption/decryption

Arguments:
    pARC4_CTX       Pointer to ARC4 CONTEXT
    InputText       Input text
    InputTextLength The length of input text in bytes

Return Value:
    OutputBlock       Return output text
 ========================================================================
*/
VOID ARC4_Compute(IN ARC4_CTX_STRUC * pARC4_CTX,
		  IN UINT8 InputBlock[], IN UINT InputBlockSize, OUT UINT8 OutputBlock[])
{
	UINT InputIndex = 0;
	UINT8 TempValue = 0;

	for (InputIndex = 0; InputIndex < InputBlockSize; InputIndex++) {
		pARC4_CTX->BlockIndex1 = (pARC4_CTX->BlockIndex1 + 1) & 0xff;
		TempValue = pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex1];
		pARC4_CTX->BlockIndex2 = (pARC4_CTX->BlockIndex2 + TempValue) & 0xff;

		pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex1] =
		    pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex2];
		pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex2] = TempValue;

		TempValue = (TempValue + pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex1]) & 0xff;
		OutputBlock[InputIndex] = InputBlock[InputIndex] ^ pARC4_CTX->KeyBlock[TempValue];

	}			/* End of for */
}				/* End of ARC4_Compute */

/*
========================================================================
Routine Description:
    Discard the key length

Arguments:
    pARC4_CTX   Pointer to ARC4 CONTEXT
    Length      Discard the key length

========================================================================
*/
VOID ARC4_Discard_KeyLength(IN ARC4_CTX_STRUC * pARC4_CTX, IN UINT Length)
{
	UINT Index = 0;
	UINT8 TempValue = 0;

	for (Index = 0; Index < Length; Index++) {
		pARC4_CTX->BlockIndex1 = (pARC4_CTX->BlockIndex1 + 1) & 0xff;
		TempValue = pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex1];
		pARC4_CTX->BlockIndex2 = (pARC4_CTX->BlockIndex2 + TempValue) & 0xff;

		pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex1] =
		    pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex2];
		pARC4_CTX->KeyBlock[pARC4_CTX->BlockIndex2] = TempValue;
	}			/* End of for */

}				/* End of ARC4_Discard_KeyLength */
