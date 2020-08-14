/* **************************************************** */
/*!
   @file	MN_DMD_types.h
   @brief	Panasonic Demodulator Driver
   @author	R.Mori
   @date	2011/8/5
   @param
		(c)	Panasonic
   */
/* **************************************************** */


#ifndef MN_DMD_TYPES_H
#define MN_DMD_TYPES_H

/* **************************************************** */
/* types */
/* **************************************************** */


typedef unsigned char DMD_u8_t;
typedef char DMD_s8_t;
typedef	unsigned int  DMD_u32_t;
typedef	int			  DMD_s32_t;
typedef	char*		  DMD_text_t;
typedef	void*              DMD_Handle_t;
#define	DMD_API

#ifdef DMD_FLOATING_FUNCTION
typedef float DMD_float_t;	
#else
typedef int DMD_float_t;			//temporary
#endif


#endif

