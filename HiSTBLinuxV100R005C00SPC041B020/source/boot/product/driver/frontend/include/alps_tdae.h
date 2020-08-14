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
#ifndef __ALPS_TDAE_H__
#define __ALPS_TDAE_H__

#include "hi_type.h"

#define ALPS_RF_MIN 45000
#define ALPS_RF_MAX 870000  //kHz

#define ALPS_STEPS_DEF    62500 //Hz
#define ALPS_IF_DEF 36125   //kHz

extern HI_S32 alps_tdae_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF);


#endif
