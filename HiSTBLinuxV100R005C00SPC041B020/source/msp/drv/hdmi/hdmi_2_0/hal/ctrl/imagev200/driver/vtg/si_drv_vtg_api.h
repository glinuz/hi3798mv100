/*************************************************************************
* si_drv_vtg_api.h
*************************************************************************/
#ifndef __SI_DRV_VTG_API_H__
#define __SI_DRV_VTG_API_H__

#include "si_datatypes.h"
#include "si_lib_video_api.h"

/************************************************************************/
/*      Type Definitions                                                */
/************************************************************************/
typedef enum
{
	SII_DRV_SCALER_ERROR__NONE
} SiiDrvVtgError_t;

/************************************************************************/
/*	Function Prototype                                                  */
/************************************************************************/
//-------------------------------------------------------------------------------------------------
//! @brief      Standard module initialization.
//-------------------------------------------------------------------------------------------------
SiiInst_t SiiDrvVtgCreate( SiiInst_t inst );

//-------------------------------------------------------------------------------------------------
//! @brief      Standard module closure.
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgDelete( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Horizontal squeeze control.
//!
//! @param[in]  squeeze  : Number of inserting output pixels on each side
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgHorizontalSqueezeSet( uint16_t squeeze );
uint16_t SiiDrvVtgHorizontalSqueezeGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Vertical squeeze control.
//!
//! @param[in]  squeeze  : Number of inserting input lines on each side
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgVerticalSqueezeSet( uint16_t squeeze );
uint16_t SiiDrvVtgVerticalSqueezeGet( void );

//-------------------------------------------------------------------------------------------------
//! @brief      Scaler bypass control.
//!
//! @param[in]  isTrue
//!             - true : scaler function is disabled (output format equals input format)
//!             - false: scaler is enabled
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgBypassSet( bool_t isTrue );
bool_t SiiDrvVtgBypassIs( void );


//-------------------------------------------------------------------------------------------------
//! @brief      Force clock to be lock or freerun
//!
//! @param[in]  
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgForceLockSet( bool_t isTrue );
bool_t SiiDrvVtgForceLockGet( void );


//-------------------------------------------------------------------------------------------------
//! @brief      Set the TPG video output
//!
//! @param[in]  
//-------------------------------------------------------------------------------------------------
void SiiDrvVtgOutputVideoTimingSet( const SiiLibVideoTiming_t* pTiming );

#endif /* __SI_DRV_VTG_API_H__ */
