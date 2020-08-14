
/***********************************************************************************
 *              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName: alps.h
 * Description: 
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16   w54542    NULL                Create this file.
 ***********************************************************************************/
#ifndef __MXL603_H__
#define __MXL603_H__

#include "hi_type.h"

extern HI_S32 mxl603_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF);
extern HI_S32 mxl603_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_VOID mxl603_tuner_resume(HI_U32 u32TunerPort);

#endif

