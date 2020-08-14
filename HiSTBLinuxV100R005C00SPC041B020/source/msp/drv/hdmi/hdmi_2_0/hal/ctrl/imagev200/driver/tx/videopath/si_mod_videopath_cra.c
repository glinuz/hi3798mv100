/*************************************************************************
* si_drv_videopath_cra.c
*************************************************************************/

//#include "si_drv_videopath_cra.h"
#include "si_vidpath_regs.h"

void SiiModVideoPathRegRead(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t *ptrdata, size_t rSize)
{
	uint8_t i;

	for(i = 0; i < rSize; i++)
	{
		*(ptrdata + i) = SiiDrvCraRdReg8(inst, addr+i);
	}
}

// This function is implemeted as per the limitation of Video path register access
// Any change in the register takes effet only after the last byte is written
void SiiModVideoPathRegWrite(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t *ptrdata, size_t wSize)
{
	uint8_t i;

	for(i = 0; i < wSize; i++)
	{
		SiiDrvCraWrReg8(inst, addr+i,*(ptrdata+i));
	}
}