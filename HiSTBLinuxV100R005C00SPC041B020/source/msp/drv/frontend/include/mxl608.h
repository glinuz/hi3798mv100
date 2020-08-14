
/***********************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
 * FileName: mxl608.h
 * Description: 
 *
 * History:
 * Version   Date             Author     DefectNum        Description
 * main\1    2007-10-16                 NULL                Create this file.
 ***********************************************************************************/
#ifndef __MXL608_H__
#define __MXL608_H__

#include "hi_type.h"

extern HI_S32 mxl608_init_tuner(HI_U32 u32TunerPort);
extern HI_S32 mxl608_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF);
extern HI_S32 mxl608_get_signal_strength(HI_U32 u32TunerPort, HI_U32 *pu32SignalStrength);
extern HI_VOID mxl608_tuner_resume(HI_U32 u32TunerPort);

#endif

