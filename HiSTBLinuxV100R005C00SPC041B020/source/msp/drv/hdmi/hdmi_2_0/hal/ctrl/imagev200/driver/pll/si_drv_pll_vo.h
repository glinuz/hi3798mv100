/*************************************************************************
* si_drv_pll_vo.h
*************************************************************************/
#ifndef __SI_DRV_PLL_VO_H__
#define __SI_DRV_PLL_VO_H__

#include "si_datatypes.h"

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/
typedef struct
{
	uint8_t  pllNr;
	uint16_t pllNf;
	uint8_t  pllOd;
	uint8_t  pllPd;

	uint32_t cpif;
} SiiDrvPllVoPllPar_t;

typedef struct
{
	uint32_t             inpClkFrq;
	uint32_t             outClkFrq;

	SiiDrvPllVoPllPar_t  pllPar0;
	SiiDrvPllVoPllPar_t  pllPar1;
} SiiDrvPllVoCapRec_t;

void SiiDrvPllDcmSet(bool_t pll, uint8_t div, uint8_t clkfb, uint8_t clk0);

#endif /* __SI_DRV_PLL_VO_H__ */
