/*************************************************************************
* si_drv_pll_vo.c
*************************************************************************/
//#define SII_DEBUG

#include "si_drv_pll_vo_api.h"
#include "si_drv_pll_vo.h"
#include "si_lib_obj_api.h"
#include "si_lib_seq_api.h"
#include "si_drv_cra_api.h"


/************************************************************************/
/*      Constants Definitions                                           */
/************************************************************************/

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/
typedef struct
{
	SiiLibVideoFrq_t inpClkFrq;
	SiiLibVideoFrq_t outClkFrq;
	uint8_t          voErrors;
} PllVoInst_t;

/************************************************************************/
/*      Local Prototypes                                                */
/************************************************************************/
static void sUpdatePlls( SiiInst_t inst );

/************************************************************************/
/*      Static Variables                                                */
/************************************************************************/
static PllVoInst_t         sPllVoInst;
extern SiiDrvPllVoCapRec_t sSiiDrvPllVoCapTbl[];

/************************************************************************/
/*      Application Interface                                           */
/************************************************************************/
void SiiDrvPllVoCreate( SiiInst_t craInst )
{
	sPllVoInst.inpClkFrq = 27000000UL;
	sPllVoInst.outClkFrq = 74175824UL;
	sPllVoInst.voErrors = SII_DRV_PLL_VO_ERROR__NONE;
	sUpdatePlls(craInst);
}

void SiiDrvPllVoOutputClockFrequencySet(SiiInst_t inst, SiiLibVideoFrq_t clkFrq )
{
	sPllVoInst.outClkFrq = clkFrq;
	sUpdatePlls(inst);
}
#if 0
void SiiDrvPllVoDelete( void )
{
}

void* SiiDrvPllVoModuleId( void )
{
	return &sPllVoInst;
}

SiiDrvPllVoError_t SiiDrvPllVoErrorGet( void )
{
	return sPllVoInst.voErrors;
}


void SiiDrvPllVoInputClockFrequencySet( SiiLibVideoFrq_t clkFrq )
{
	sPllVoInst.inpClkFrq = clkFrq;
	sUpdatePlls();
}

SiiLibVideoFrq_t SiiDrvPllVoInputClockFrequencyGet( void )
{
	return sPllVoInst.inpClkFrq;
}

SiiLibVideoFrq_t SiiDrvPllVoOutputClockFrequencyGet( void )
{
	return sPllVoInst.outClkFrq;
}
#endif
static void sUpdatePlls( SiiInst_t craInst )
{
	SiiDrvPllVoCapRec_t* pTbl = sSiiDrvPllVoCapTbl;


	while( pTbl->inpClkFrq )
	{
		/* Find match in Pll capability table (si_drv_pll_vo_cfg.c) */
		if( (pTbl->inpClkFrq == sPllVoInst.inpClkFrq) && (pTbl->outClkFrq == sPllVoInst.outClkFrq) )
		{
			// write_ats(1,0xb8, 0x00); // source for Pll1 is Xtal ref, not working yet
			SiiDrvCraWrReg8(craInst, 0xb8, 0x00); // source for Pll0 is RX0 ref

			SiiDrvCraWrReg8(craInst, 0xb1, pTbl->pllPar0.pllNr); // NR-1 divider
			SiiDrvCraWrReg16(craInst, 0xb2, pTbl->pllPar0.pllNf); // NF -1 multiplier
			SiiDrvCraWrReg8(craInst,0xb4, pTbl->pllPar0.pllOd); // OD -1 post divide
			SiiDrvCraWrReg16(craInst,0xb6, pTbl->pllPar0.pllPd); // NB -1

			SiiDrvCraWrReg8(craInst,0xb9, 0x01);  // no post divider
			SiiDrvCraWrReg8(craInst,0xba, 0x01);  // update request

			//Select vidgen input clock source as XTAL
			SiiDrvCraWrReg8(craInst, 0x12, 0x05);
			SiiDrvCraWrReg8(craInst, 0xb0, 0x20); //PLL saturation behavior
 
			SiiDrvCraWrReg8(craInst, 0xba, 0x01); //video update

			sPllVoInst.voErrors = SII_DRV_PLL_VO_ERROR__NONE;
			return;
		}

		pTbl++;
	}
	/* return error if no match has been found */
	sPllVoInst.voErrors = SII_DRV_PLL_VO_EEROR__FREQ_NOT_FOUND;

}

/** END of File *********************************************************/
