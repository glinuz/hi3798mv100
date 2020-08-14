/*******************************************************************************
 *
 * FILE NAME          : MxL68x_PhySpurTable.h
 * 
 * AUTHOR             : Dong Liu
 *  
 * DATE CREATED       : 11/16/2011
 *
 * DESCRIPTION        : This file contains spur table definition.
 *
 *******************************************************************************
 *                Copyright (c) 2011, MaxLinear, Inc.
 ******************************************************************************/

#ifndef __MXL68x_PHY_SPUR_TABLE_H__
#define __MXL68x_PHY_SPUR_TABLE_H__

/******************************************************************************
    Include Header Files
    (No absolute paths - paths handled by make file)
******************************************************************************/
#include "MaxLinearDataTypes.h"

/******************************************************************************
    Macros
******************************************************************************/
#define   MAX_SPUR_REG_NUM    4 
/******************************************************************************
    User-Defined Types (Typedefs)
******************************************************************************/
typedef struct
{
  UINT8 SpurRegNum;  
  UINT8 SpurRegAddr[MAX_SPUR_REG_NUM];
} CHAN_DEP_SPUR_REG_T, *PCHAN_DEP_SPUR_REG_T;

typedef struct
{
  UINT32 centerFreqHz;
  UINT8  rfLutSwp1Reg;
  UINT8  rfLutDivInBypReg;
  UINT8  refLutBypReg;
  UINT8  refIntModReg;
} MXL68x_CHAN_DEP_FREQ_TABLE_T, *PMXL68x_CHAN_DEP_FREQ_TABLE_T;

/******************************************************************************
    Global Variable Declarations
******************************************************************************/

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_XTAL_16MHZ_LIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_XTAL_24MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_HRC_16MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_HRC_24MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_IRC_16MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_IRC_24MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_MN_LUT[];

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_7MHZ_LUT_XTAL_16MHZ_LIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_7MHZ_LUT_XTAL_24MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_8MHZ_LUT_XTAL_16MHZ_LIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_8MHZ_LUT_XTAL_24MHZ[];

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_D_LUT_XTAL_16MHZ_LIF[]; 
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_D_LUT_XTAL_24MHZ[]; 

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_I_LUT_XTAL_16MHZ_LIF[]; 
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_I_LUT_XTAL_24MHZ[]; 

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_SECAM_L_LUT_XTAL_16MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_SECAM_L_LUT_XTAL_24MHZ[];

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_D_LUT_XTAL_16MHZ_HIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_I_LUT_XTAL_16MHZ_HIF[]; 
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_7MHZ_LUT_XTAL_16MHZ_HIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_PAL_BG_8MHZ_LUT_XTAL_16MHZ_HIF[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_NTSC_LUT_XTAL_16MHZ_HIF[];

extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_DIG_CABLE_LUT_BW_6MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_DIG_CABLE_LUT_BW_8MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_DIG_TERR_LUT_BW_6MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_DIG_TERR_LUT_BW_7MHZ[];
extern MXL68x_CHAN_DEP_FREQ_TABLE_T MXL68x_DIG_TERR_LUT_BW_8MHZ[];

/******************************************************************************
    Prototypes
******************************************************************************/

#endif /* __MXL68x_PHY_SPUR_TABLE_H__*/




