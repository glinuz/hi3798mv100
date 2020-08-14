/*******************************************************************************
 *             Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: init_keyled.c
 * Description:  init keyled display
 *
 * History:
 * Version   Date              Author    DefectNum       Description
 *
 *******************************************************************************/

#include "exports.h"
#include "hi_keyled_ct1642.h"



HI_VOID KEYLED_Ct1642Init(HI_VOID)
{
	HI_U32 tmpData=0X0;
	HI_S32 Ret;
	
	HI_KEYLED_Open();
	HI_KEYLED_Enable();

	Ret = HI_KEYLED_Display(tmpData);
	
	HI_KEYLED_Close();
}




