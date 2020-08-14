/* **************************************************** */
/*!
   @file	MN_I2C.h
   @brief	Tuner Bus controller
   @author	R.Mori
   @date	2011/7/12
   @param
		(c)	Panasonic
   */
/* **************************************************** */

#ifndef MN_TCB_H
#define MN_TCB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "MN_DMD_driver.h"

/* **************************************************** */
/*  System dependence functions */
/* **************************************************** */
/* I2C Bus Functions */
/* these function is defined by MN_DMD_I2C_(system).c */
extern DMD_ERROR_t MN8847X_DMD_TCB_Write(DMD_u8_t	slvadr,DMD_u8_t adr,DMD_u8_t  data);
extern DMD_ERROR_t MN8847X_DMD_TCB_Read (DMD_u8_t slvadr, DMD_u8_t adr,DMD_u8_t* data);

extern DMD_ERROR_t MN8847X_DMD_TCB_WriteRead(DMD_u8_t tuner_slvadr , DMD_u8_t tuner_reg_adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen);


/* '11/08/05 : OKAMOTO Implement "Through Mode". */
extern DMD_ERROR_t MN8847X_DMD_TCB_WriteAnyLength(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* data , DMD_u32_t wlen);
extern DMD_ERROR_t MN8847X_DMD_TCB_ReadAnyLength (DMD_u8_t	slvadr , DMD_u8_t* data , DMD_u8_t rlen);

#ifdef __cplusplus
}
#endif

#endif
