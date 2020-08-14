/*******************************************************************************
 *
 * FILE NAME          : MxL608_TunerSpurTable.h
 * 
 * AUTHOR             : Dong Liu
 *  
 * DATE CREATED       : 12/21/2012
 *
 * DESCRIPTION        : This file contains spur table definition for MxL608.
 *
 *******************************************************************************
 *                Copyright (c) 2012, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL608_TUNER_SPUR_TABLE_H__
#define __MXL608_TUNER_SPUR_TABLE_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"

/******************************************************************************
    Macros
******************************************************************************/
#define   MXL608_MAX_SPUR_REG_NUM    2 
/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
typedef struct
{
  UINT8 SpurRegNum;  
  UINT8 SpurRegAddr[MXL608_MAX_SPUR_REG_NUM];
} MXL608_CHAN_DEPENDENT_SPUR_REGISTER_T, *PMXL608_CHAN_DEPENDENT_SPUR_REGISTER_T;

typedef struct
{
  UINT32 centerFreqHz;
  UINT8  reg_0xEA;
  UINT8  reg_0xEB;
  UINT8  xtalFreq;
} MXL608_CHAN_DEPENDENT_FREQ_TABLE_T, *PMXL608_CHAN_DEPENDENT_FREQ_TABLE_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/
extern MXL608_CHAN_DEPENDENT_SPUR_REGISTER_T MxL608_SPUR_REGISTER;
extern MXL608_CHAN_DEPENDENT_FREQ_TABLE_T MXL608_DIG_CABLE_FREQ_LUT[];
extern MXL608_CHAN_DEPENDENT_FREQ_TABLE_T MXL608_DIG_TERR_FREQ_LUT[];

/******************************************************************************
    Prototypes
******************************************************************************/

#endif /* __MXL608_TUNER_SPUR_TABLE_H__*/




