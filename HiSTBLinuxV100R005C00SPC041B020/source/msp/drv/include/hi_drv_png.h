/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name	: hi_png_ioctl.h
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/09
Description	: PNG ioctl
Function List	:

History       	:
Date				Author        		Modification
2010/10/09		z00141204		Created file      	
******************************************************************************/

#ifndef __HI_PNG_IOCTL_H__
#define __HI_PNG_IOCTL_H__

#include <linux/ioctl.h>

#include "hi_png_type.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif  /* __cplusplus */
#endif  /* __cplusplus */

/* support 16 decoder at most */
#define PNG_MAX_HANDLE 16

/* Use 's' as magic number */
#define PNG_IOC_MAGIC 's'

typedef struct hiPNG_BUF_IOC_S
{
    HI_U32 u32PhyAddr;          /* physical addr of the buffer*/
    HI_U32 u32Size;             /* buffer size */
    HI_U64 u64Vir;              /* virtual addr of the buffer*/
}HI_PNG_BUF_IOC_S;
 
/* get instructed struct of code stream buffer */
typedef struct tag_PNG_GETBUF_CMD_S
{
	HI_PNG_HANDLE s32Handle;		/* decode handle */
	HI_PNG_BUF_IOC_S stBuf;				/* buffer information */
}PNG_GETBUF_CMD_S;

/* set the length of instructed struct of code stream */
typedef struct tag_PNG_SETSTREAMLEN_CMD_S
{
	HI_PNG_HANDLE s32Handle;		/* decoder handle */
	//HI_PNG_BUF_S stBuf;		    /* code stream information */
	HI_U32 u32Phyaddr;              /* physical address of code stream buffer */
    HI_U32 u32Len;              	/* length of code stream */
}PNG_SETSTREAM_CMD_S;

/* start decoder instructed struct */
typedef struct tag_PNG_DECODE_CMD_S
{
	HI_PNG_HANDLE s32Handle;	    /* decode handle */
	HI_PNG_DECINFO_S stDecInfo;		/* decode control informarion */
}PNG_DECODE_CMD_S;

/* get decode result of command struct */
typedef struct tag_PNG_DECRESULT_CMD_S
{
	HI_PNG_HANDLE s32Handle;		/* decode handle */
	HI_BOOL bBlock;					/* if get by block */
	HI_PNG_STATE_E eDecResult;		/* decode state */
}PNG_DECRESULT_CMD_S;

/* Get struct of parameter command from buffer */
typedef struct tag_PNG_GETBUFPARAM_CMD_S
{
	HI_PNG_HANDLE s32Handle;		/* decode handle */
	HI_U32 u32PhyAddr;				/* physical address in buffer */
    HI_U32 u32Size;             	/* the size of buffer */
}PNG_GETBUFPARAM_CMD_S;

#define PNG_CREATE_DECODER	   _IOR(PNG_IOC_MAGIC, 1, HI_PNG_HANDLE)				/* create decoder,return PNG handle */
#define PNG_DESTROY_DECODER	   _IOW(PNG_IOC_MAGIC, 2, HI_PNG_HANDLE)				/* destroyed decoder, introduced PNG handle */
#define PNG_ALLOC_BUF	       _IOWR(PNG_IOC_MAGIC, 3, PNG_GETBUF_CMD_S)		    /* get code stream buffer */
#define PNG_RELEASE_BUF        _IOW(PNG_IOC_MAGIC, 4, HI_PNG_HANDLE)
#define PNG_SET_STREAMLEN      _IOW(PNG_IOC_MAGIC, 5, PNG_SETSTREAM_CMD_S)	    	/* set data size in buffer */
#define PNG_GET_STREAMLEN      _IOWR(PNG_IOC_MAGIC, 6, PNG_SETSTREAM_CMD_S)
#define PNG_DECODE	           _IOW(PNG_IOC_MAGIC, 7, PNG_DECODE_CMD_S)			    /* start PNG decode*/
#define PNG_GET_DECRESULT	   _IOR(PNG_IOC_MAGIC, 8, PNG_DECRESULT_CMD_S)		    /* get decode result; synchronization */
#define PNG_GET_GETBUFPARAM	   _IOR(PNG_IOC_MAGIC, 9, PNG_GETBUFPARAM_CMD_S)		/* get physical head address of first code stream buffer */

/* Struct of head of code stream in buffer */
typedef struct tag_PNG_BUF_HEAD_S
{
	HI_U32 u32NextPhyaddr;			/* physical address of next head in buffer */
	HI_VOID *pNextViraddr;	        /* virtual address of next head in buffer(in kernel) */
	HI_U32 u32BufSize;				/* buffer size ,not include the size of head information */
	HI_U32 u32StreamLen;	        /* stream length */
  
}PNG_BUF_HEAD_S;

#ifdef __cplusplus
#if __cplusplus
}
#endif  /* __cplusplus */
#endif  /* __cplusplus */


#endif  /* __HI_TDE_IOCTL_H__ */
