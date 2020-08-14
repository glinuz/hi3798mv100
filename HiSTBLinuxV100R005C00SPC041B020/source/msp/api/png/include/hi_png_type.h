/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
 ******************************************************************************
 File Name     : hi_png_type.h
Version       : Initial Draft
Author        : z00141204
Created       : 2010/10/09
Last Modified :
Description   : PNG public type
Function List :
History       :
1.Date        : 2010/10/09
Author      : z00141204
Modification: Created file

 ******************************************************************************/
 
#ifndef __HI_PNG_TYPE_H__
#define __HI_PNG_TYPE_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#include "hi_type.h"



typedef HI_S32                  HI_PNG_HANDLE;

/* macro define for data convert */
#define HI_PNG_TRANSFORM_ADJUSTPIXELORDER 0x1	/* modify pixel order  */
#define HI_PNG_TRANSFORM_GRAY124TO8   0x2	/* 1/2/4bit gray bitmap to 8bit */
#define HI_PNG_TRANSFORM_GRAY2RGB 0x4		/* gray to RGB*/
#define HI_PNG_TRANSFORM_ADDALPHA 0x8		/* add alpha value*/
#define HI_PNG_TRANSFORM_STRIPALPHA 0x10	/* remove alpha valude */
#define HI_PNG_TRANSFORM_BGR2RGB  0x20		/* switch R and B */
#define HI_PNG_TRANSFORM_SWAPALPHA 0x40		/* switch Alpha and R, G, B */
#define HI_PNG_TRANSFORM_16TO8 0x80		/* 16 bit to 8bit */
#define HI_PNG_TRANSFORM_8TO4 0x100		/* 8bit to 4bit */
#define HI_PNG_TRANSFORM_CLUT2RGB 0x200         /* clut to RGB */
#define HI_PNG_TRANSFORM_PREMULTI_ALPHA 0x400 /*premulti alpha*/
/* enumerations of png color format */
typedef enum hiPNG_IMAGEFMT_E
{
	HI_PNG_IMAGEFMT_GRAY = 0x0,		/* gray bitmap */
	HI_PNG_IMAGEFMT_RGB = 0x2,		/* RGB bitmap */
	HI_PNG_IMAGEFMT_CLUT = 0x3,		/* clut */
	HI_PNG_IMAGEFMT_AGRAY = 0x4,	        /* gray bitmap with alpha */
	HI_PNG_IMAGEFMT_ARGB = 0x6,		/* RGB bitmap with alpha */
	HI_PNG_IMAGEFMT_BUTT
}HI_PNG_IMAGEFMT_E;

/* struction for png picture information */
typedef struct hiPNG_INFO_S
{
	HI_U32 u32Width;				/* picture width */
	HI_U32 u32Height;				/* picture height */
	HI_U8 u8BitDepth;				/* picture bitdepth */
        HI_U8 u8Reserved1;
        HI_U8 u8Reserved2;
        HI_U8 u8Reserved3;
	HI_PNG_IMAGEFMT_E eColorFmt;	/* color format of png */
	//HI_U8 u8InterlaceType;			/* type of picture's interlace */
}HI_PNG_INFO_S;

/*enumeration of  the modes that convert  1/2/4bit gray bitmap to 8bit */
typedef enum hiPNG_124TO8_MODE_E
{
	HI_PNG_124TO8_BRIEF = 0x0,		/* fill 0 */
	HI_PNG_124TO8_ACCURATE,		/* fill the low bits by high bit*/
	HI_PNG_124TO8_BUTT
}HI_PNG_124TO8_MODE_E;

/* enumeration of  the modes that convert 16bit to 8bit */
typedef enum hiPNG_16TO8_MODE_E
{
    HI_PNG_16TO8_BRIEF = 0x0,		/* throw away the lower 8 bits */
    HI_PNG_16TO8_ACCURATE,			/* by formula:H8 + ((L8 - H8 > 128)? 1 : 0)   H8 is higher 8 bit£¬L8 is lower 8 bit */
    HI_PNG_16TO8_BUTT
}HI_PNG_16TO8_MODE_E;

/* enumeration of the mode that adding alpha */
typedef enum hiPNG_ADDALPHA_MODE_E
{
	HI_PNG_ADDALPHA_BRIEF = 0x0,	/* use the alpha that user assigned*/
	HI_PNG_ADDALPHA_TRNS,			/* adding alpha value by transparent */
	HI_PNG_ADDALPHA_BUTT
}HI_PNG_ADDALPHA_MODE_E;

/* enumeration of destination output format */
typedef enum hiPNG_COLORFMT_E
{
	HI_PNG_COLORFMT_ARGB4444 = 0x0,	/* ARGB4444 */
	HI_PNG_COLORFMT_ARGB1555,		/* ARGB1555 */
	HI_PNG_COLORFMT_RGB565,			/* RGB565 */
	HI_PNG_COLORFMT_RGB555,			/* RGB555 */
	HI_PNG_COLORFMT_RGB444,			/* RGB444 */
	HI_PNG_COLORFMT_BUTT
}HI_PNG_COLORFMT_E;

/*struction for transparent information*/
typedef struct hiPNG_TRNSINFO_S
{
	HI_U16 u16Red;		/* red part*/
	HI_U16 u16Green;	/* green part*/
	HI_U16 u16Blue;		/* blue part or gray value*/
}HI_PNG_TRNSINFO_S;

/* struction for data convert */
typedef struct hiPNG_TRANSFORM_S
{
	HI_U32 u32Transform;						/* operation of data convert,could set the value like  HI_PNG_TRANSFORM_***  | HI_PNG_TRANSFORM_*** */
	HI_PNG_124TO8_MODE_E e124To8Mode;			/* the mode that convert  1/2/4bit gray bitmap to 8bit  */
	HI_PNG_16TO8_MODE_E e16To8Mode;			/*  the mode that convert 16bit to 8bit */
	HI_PNG_ADDALPHA_MODE_E eAddAlphaMode;	/* the mode that adding alpha */
	HI_PNG_TRNSINFO_S sTrnsInfo;				/* transparent value */
	HI_PNG_COLORFMT_E eOutFmt;					/* 8bit to 4 bit,destination output format */
	HI_U32 u32ClutPhyaddr;						/* physical addr for palette */
        HI_BOOL bClutAlpha;                                             /* whether alpha channel is valid in palette*/
        HI_U8 u8Reserved1;
        HI_U16 u16Filler;								/* the value to fill when adding alpha part */
}HI_PNG_TRANSFORM_S;

/* struction fot decoding  */
typedef struct hiPNG_DECODE_S
{
	HI_BOOL bSync;						/* whether is synchronization */
        HI_U8 u8Reserved1;
        HI_U8 u8Reserved2;
        HI_U8 u8Reserved3;
	HI_U32 u32Phyaddr;					/* physical addr for destination buffer*/
	HI_U32 u32Stride;					/* stride for  destination buffer*/
	HI_PNG_INFO_S	stPngInfo;			/*png picture information */
	HI_PNG_TRANSFORM_S stTransform;	    /* data convert */
}HI_PNG_DECINFO_S;

/* enumeraton of status for png decoding */
typedef enum hiPNG_STATE_E
{
    HI_PNG_STATE_NOSTART = 0x0,  /* no start decod or abort,such as destroying decoder when decoding */
    HI_PNG_STATE_DECING,         /* decoding */
    HI_PNG_STATE_FINISH,         /* finish decoding */
    HI_PNG_STATE_ERR,            /* failed for decoding */
    HI_PNG_STATE_BUTT
}HI_PNG_STATE_E;

/* IO read function,return value is the byte that read */
typedef HI_U32 (* HI_PNG_READ_FN)(HI_UCHAR *pBuf, HI_U32 u32Len, HI_PNG_HANDLE s32Handle);

/* IO read parameters */
typedef struct hiPNG_IOPTR_S
{
    HI_VOID *pStart;		/* virtual addr for buf */
    HI_VOID *pRead;		/* the virtual addr that will be read next time */
    HI_U32 u32Len;		/* the remainder size */
}HI_PNG_IOPTR_S;

/*struction of buffer information */
typedef struct hiPNG_BUF_S
{
    HI_U32 u32PhyAddr;          /* physical addr of the buffer*/
    HI_VOID *pVir;                          /* virtual addr of the buffer*/
    HI_U32 u32Size;                 /* buffer size */
}HI_PNG_BUF_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */

#endif	/* __HI_PNG_TYPE_H__ */
