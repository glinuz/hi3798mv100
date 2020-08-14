/***********************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: xg_3bl.h
* Description:
*
* History:
* Version   Date             Author     DefectNum        Description
* main\1    2007-10-16   w54542    NULL                Create this file.
***********************************************************************************/
#ifndef __XG_3BL_H__
#define __XG_3BL_H__

#include "hi_type.h"
#include "drv_tuner_ext.h"

#define XG_3BL_STEPS_DEF 62500
#define XG_3BL_SUPPLY_DEF 0

extern HI_S32 xg_3bl_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 u32RF);

#endif
