/******************************************************************************

  Copyright (C), 2012, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : silabs.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2014/03/21
  Description   : 
  History       :
  1.Date        : 2014/03/21
    Author      : 
    Modification: Created file

 *******************************************************************************/

/**
 * \file
 * \brief For SiLabs tuner.
 */
#ifndef __SILABS_H__
#define __SILABS_H__

/*----------------------------- INCLUDE FILES ------------------------------------------*/
#include "hi_type.h"

/*--------------------------- MACRO DECLARATIONS ---------------------------------------*/




/*-------------------- GLOBAL STRUCTURE DECLARATIONS -----------------------------------*/

typedef enum OPERA_MODE {
    SiLabs_READ = 1,
    SiLabs_WRITE = 2
} T_OPERA_MODE;

typedef enum ERROR_OP {
    OK = 1,
    I2C_ERROR = 2,
    LOOP_EXP_ERROR = 3,
    ERROR = 4
} T_ERROR_OP;


/*------------------------- GLOBAL DECLARATIONS ----------------------------------------*/

extern T_ERROR_OP Si2147_Tune_DTV_Initial(HI_U32 u32TunerPort);
extern T_ERROR_OP Si2147_Tune_DTV_Demo(HI_U32 u32TunerPort, HI_U32 u32Frequency,HI_U32 u32BandWidth);

#endif /* __SILABS_H__ */

