/* **************************************************** */
/*!
   @file	MN_I2C.h
   @brief	I2C Infterface for MN_DMD_Driver
   @author	STB
   @date	2015/4/15
   @param
		(c)	Hisilicon
   */
/* **************************************************** */


#ifndef MN8847X_I2C_H
#define MN8847X_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* **************************************************** */
/*  System dependence functions */
/* **************************************************** */
/* I2C Bus Functions */
/* these function is defined by MN_DMD_I2C_(system).c */
extern DMD_ERROR_t MN8847X_DMD_I2C_open(void);
extern DMD_ERROR_t MN8847X_DMD_I2C_Write(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t  data );
extern DMD_ERROR_t MN8847X_DMD_I2C_Read(DMD_u8_t	slvadr , DMD_u8_t adr , DMD_u8_t* data );
extern DMD_ERROR_t MN8847X_DMD_I2C_WriteRead(DMD_u8_t slvadr , DMD_u8_t adr , DMD_u8_t* wdata , DMD_u32_t wlen , DMD_u8_t* rdata , DMD_u32_t rlen);
extern DMD_ERROR_t MN8847X_DMD_I2C_term(void);
extern DMD_ERROR_t MN8847X_DMD_wait( DMD_u32_t	msecond);
extern DMD_ERROR_t MN8847X_DMD_timer(DMD_u32_t* tim);

#ifdef DMD_I2C_DEBUG
extern DMD_u32_t	dmd_i2c_debug_flag;
#endif


#ifdef __cplusplus
}
#endif

#endif
