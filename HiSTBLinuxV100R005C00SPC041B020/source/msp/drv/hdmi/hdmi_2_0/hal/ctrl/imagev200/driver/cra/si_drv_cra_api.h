/**
* @file si_drv_cra_api.h
*
* @brief Chip Register Access Driver
*
*****************************************************************************/

#ifndef __SI_DRV_CRA_API_H__
#define __SI_DRV_CRA_API_H__

#include "si_datatypes.h"
#include "platform_api.h"
#include "si_lib_obj_api.h"

/************************************************************************/
/*  Type Definitions                                                    */
/************************************************************************/

typedef uint16_t      SiiDrvCraAddr_t;
typedef uint16_t      SiiDrvCraSize_t;
typedef uint8_t       SiiDrvCraIdx_t;

typedef uint16_t      SiiDrvCraVirtAddr_t;
typedef uint16_t      SiiReg_t;

typedef int           prefuint_t; // Platform-specific efficient integers

typedef	void          (*notifyCraFailureCallback)(void);

typedef struct{
   uint16_t  baseAddr; 
   uint32_t  *pu32BaseAddrVirtual; 
  
   SiiPlatformInterface_t *pInterfaceInfo;
   SiiPlatformGPIO_t       intrPin;
   SiiPlatformGPIO_t       rstPin;

   notifyCraFailureCallback callBack;
  
}SiiDrvCraConfig_t;

SiiInst_t SiiDrvCraCreate(SiiDrvCraConfig_t *pcfg);
void SiiDrvCraDelete(SiiInst_t craInst);
uint32_t *SiiDrvVirtualAddrGet(SiiInst_t craInst);
bool_t SiiDrvCraHardwareReset(SiiInst_t craInst);

/*----------------------------------------------------------------------*/
/*  ABT methods                                                         */
/*----------------------------------------------------------------------*/
void                SiiDrvCraWrReg8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t val );
uint8_t             SiiDrvCraRdReg8(SiiInst_t inst,  SiiDrvCraAddr_t addr );
void                SiiDrvCraSetBit8(SiiInst_t inst,  SiiDrvCraAddr_t addr, uint8_t mask );
void                SiiDrvCraClrBit8(SiiInst_t inst,  SiiDrvCraAddr_t addr, uint8_t mask );
void                SiiDrvCraPutBit8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t mask, uint8_t val );

void                SiiDrvCraWrReg16(SiiInst_t inst, SiiDrvCraAddr_t addr, uint16_t val );
uint16_t            SiiDrvCraRdReg16(SiiInst_t inst, SiiDrvCraAddr_t addr );

void                SiiDrvCraWrReg24(SiiInst_t inst, SiiDrvCraAddr_t addr, uint32_t val );
uint32_t            SiiDrvCraRdReg24(SiiInst_t inst, SiiDrvCraAddr_t addr );

void                SiiDrvCraFifoWrite8(SiiInst_t inst, SiiDrvCraAddr_t addr, const uint8_t* pData, SiiDrvCraSize_t size );
void                SiiDrvCraFifoRead8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size );
void                SiiDrvCraBlockWrite8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size );
void                SiiDrvCraBlockRead8(SiiInst_t inst, SiiDrvCraAddr_t addr, uint8_t* pData, SiiDrvCraSize_t size );

bool_t				SiiDCardRegBlockRead(SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, uint8_t *pBuffer, uint16_t count);
bool_t				SiiDCardRegBlockWrite(SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, uint8_t *pBuffer, uint16_t count);
//bool_t				SimmonReadBlockI2c (SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, uint8_t *pBuffer, uint16_t count);
//bool_t				SimmonWriteBlockI2c (SiiInst_t inst, uint8_t deviceId, uint16_t regAddr, const uint8_t *pBuffer, uint16_t count);

bool_t SiiDrvCraIsInterruptRcvd(SiiInst_t inst);

#endif /* __SI_DRV_CRA_API_H__ */
