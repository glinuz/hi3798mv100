
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
#ifndef __TMX7070X_H__
#define __TMX7070X_H__

#include "hi_type.h"

#define TMX7070X_RF_MIN 51000
#define TMX7070X_RF_MAX 864000  //kHz

#define TMX7070X_STEPS_DEF    62500 //Hz
#define TMX7070X_IF_DEF 36125   //kHz

extern HI_S32 tmx7070x_set_tuner(HI_U32 u32TunerPort, HI_U8 enI2cChannel, HI_U32 puRF);

#endif

