/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon. Co., Ltd.

******************************************************************************
File Name	: hi_png_api.c
Version		: Initial Draft
Author		: z00141204
Created		: 2010/10/09
Description	: implemention of PNG application interface
Function List 	:

History       	:
Date				Author        		Modification
2010/10/09		z00141204		Created file
******************************************************************************/
#ifdef HIPNG_ACCELERATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>
#include <assert.h>

#include "hi_png_api.h"
#include "hi_png_errcode.h"

#ifdef HIPNG_V100
#include "hi_png_ioctl.h"
#else
#include "hi_drv_png.h"
#endif

#ifdef HIGFX_COMMON
#include "hi_gfx_comm.h"
#endif

#ifdef DEBUG
#define PNG_API_ASSERT(EXP) assert(EXP)
#else
#define PNG_API_ASSERT(EXP)
#endif

/* PNG device path */
static const HI_CHAR *g_pPngDevPath = "/dev/hi_png";

#ifdef CONFIG_GFX_MEM_MMZ
/* memory device path */
static const HI_CHAR *g_pMemDevPath = "/dev/mem";
#endif

/* PNG device description symbol */
static HI_S32 g_s32PngFd = -1;

/* memory device description symbol */
static HI_S32 g_s32MemFd = -1;

/* PNG device quoted count */
static HI_U32 g_u32PngRef = 0;

/* PNG device lock */
static pthread_mutex_t g_PngMutex = PTHREAD_MUTEX_INITIALIZER;

/* check if PNG device is open */
#define PNG_CHECK_DEVSTATE() do \
    {\
        if (g_s32PngFd == -1)\
        {\
            return HI_ERR_PNG_DEV_NOOPEN; \
        } \
    } while (0)

#define PNG_CHECK_HANDLE(handle) do \
	{\
		if ((handle <= 0) || (handle > PNG_MAX_HANDLE) || !gs_PngApiInstance[handle - 1].bValid)\
		{\
			return HI_ERR_PNG_INVALID_HANDLE;\
		}\
	} while(0)

typedef struct hiPNG_STREAM_BUF_S
{
    HI_U32 u32PhyAddr;          /* physical addr of the buffer*/
    HI_VOID *pVir;                          /* virtual addr of the buffer*/
    HI_U32 u32Size;                 /* buffer size */
	HI_U32 u32Len;					/*stream size*/
	HI_VOID *pMemHandle;
}HIPNG_STREAM_BUF_S;

/* list struct, mapping physical address /virtual address */
typedef struct tagPNG_MEM_NODE_S
{
    HIPNG_STREAM_BUF_S stBuf;                   /* memory information */
    struct tagPNG_MEM_NODE_S *pNext;      /* next node */
} PNG_MEM_NODE_S;

/* buf r/w info struct */
typedef struct tagPNG_READ_INFO_S
{
    HI_U32 u32Read;        	/* length of code by read */
	PNG_MEM_NODE_S *pstMemNode;
} PNG_READ_INFO_S;

/* user decode instance, maintain the relation on mapping  physical addr and user virtual addr */
typedef struct tagPNG_API_INSTANCE_S
{
    pthread_mutex_t stLock;     /* Instance lock */
    PNG_MEM_NODE_S *pMemHead;   /* list head of memory */
	PNG_MEM_NODE_S *pMemTail;
    PNG_READ_INFO_S *pReadParam;    /* read parameter */
	HI_BOOL bValid;
} PNG_API_INSTANCE_S;

/* user decoder instance array */
static PNG_API_INSTANCE_S gs_PngApiInstance[PNG_MAX_HANDLE];

HI_U32 HIPNG_Read(HI_UCHAR *pBuf, HI_U32 u32Len, HI_PNG_HANDLE s32Handle);

#ifdef HIPNG_STREAMBUF_KERNEL
static HI_VOID *PNG_Map(HI_U32 u32Phyaddr, HI_U32 u32Size);
static HI_VOID PNG_UnMap(HI_U32 u32Phyaddr, HI_VOID *pVir, HI_U32 u32Size);
#endif

/********************************************************************************************
* func:	Open PNG device
* in:	none
* out:	none
* ret:	HI_SUCCESS	Open device successfully
*		HI_ERR_PNG_DEV_NOEXIST	device is not exist
* others:	when using PNG module, open device at first: support multi_course and multi-thread
********************************************************************************************/
HI_S32 HI_PNG_Open(HI_VOID)
{
    HI_S32 s32Ret;
    
    s32Ret = pthread_mutex_lock(&g_PngMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return HI_ERR_PNG_DEV_BUSY;
    }

    /* device opened, return afrer adding ref */
    if (g_s32PngFd != -1)
    {
        g_u32PngRef++;
        pthread_mutex_unlock(&g_PngMutex);
        return HI_SUCCESS;
    }

    g_s32PngFd = open(g_pPngDevPath, O_RDWR | O_SYNC, 0);
    if (-1 == g_s32PngFd)
    {
        pthread_mutex_unlock(&g_PngMutex);
        return HI_ERR_PNG_DEV_NOEXIST;
    }

#ifdef CONFIG_GFX_MEM_MMZ
    g_s32MemFd = open(g_pMemDevPath, O_RDWR, 0);
    if (-1 == g_s32MemFd)
    {
        close(g_s32PngFd);
        g_s32PngFd = -1;
        pthread_mutex_unlock(&g_PngMutex);
        return HI_ERR_PNG_DEV_NOEXIST;
    }
#else
    g_s32MemFd = gfx_mem_open();
    if (g_s32MemFd < 0)
    {
        close(g_s32PngFd);
        g_s32PngFd = -1;
        pthread_mutex_unlock(&g_PngMutex);
        return HI_ERR_PNG_DEV_NOEXIST;
    }
#endif
    g_u32PngRef++;

	memset(gs_PngApiInstance, 0, sizeof(gs_PngApiInstance));
	
    pthread_mutex_unlock(&g_PngMutex);

    return HI_SUCCESS;
}

/********************************************************************************************
* func:	close PNG device
* in:	none
* out:	none
* ret:	none
* others:	pair wirh HI_PNG_Open
********************************************************************************************/
HI_VOID HI_PNG_Close(HI_VOID)
{
    HI_S32 s32Ret;

    s32Ret = pthread_mutex_lock(&g_PngMutex);
    if (HI_SUCCESS != s32Ret)
    {
        return;
    }

    /* device is not open,return */
    if (-1 == g_s32PngFd)
    {
        pthread_mutex_unlock(&g_PngMutex);
        return;
    }

    if (--g_u32PngRef == 0)
    {
#ifdef CONFIG_GFX_MEM_MMZ
	    close(g_s32MemFd);
#else
        gfx_mem_close(g_s32MemFd);
#endif
        g_s32MemFd = -1;
        close(g_s32PngFd);
        g_s32PngFd = -1;
        pthread_mutex_unlock(&g_PngMutex);

        return;
    }

    pthread_mutex_unlock(&g_PngMutex);

    return;
}

/********************************************************************************************
* func:	create decoder
* in:	none
* out:	ps32Handle decoder handle pointer
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_NOHANDLE	no decoder resource
* ret:       HI_ERR_PNG_NULLPTR  Null pointer
* others:	support create 32 decoder at most
*********************************************************************************************/
HI_S32 HI_PNG_CreateDecoder(HI_PNG_HANDLE *ps32Handle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_NULL == ps32Handle)
    {
        return HI_ERR_PNG_NULLPTR;
    }

    /* check if device is open */
    PNG_CHECK_DEVSTATE();

    s32Ret = ioctl(g_s32PngFd, PNG_CREATE_DECODER, ps32Handle);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    pthread_mutex_init(&gs_PngApiInstance[*ps32Handle - 1].stLock, HI_NULL);
    gs_PngApiInstance[*ps32Handle - 1].pMemHead = HI_NULL;
	gs_PngApiInstance[*ps32Handle - 1].pMemTail = HI_NULL;
    gs_PngApiInstance[*ps32Handle - 1].pReadParam = HI_NULL;
	gs_PngApiInstance[*ps32Handle - 1].bValid = HI_TRUE;

    return HI_SUCCESS;
}

/* release all mappings of user coder buffer*/
/* only use in release decoder and code stream buffer, it's suit to release parameter by read */
HI_VOID PNG_UnMap_DecoderBuf(HI_PNG_HANDLE s32Handle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PNG_MEM_NODE_S *pTemp = HI_NULL;

    do
    {
        s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
    } while (s32Ret != HI_SUCCESS);

    while (gs_PngApiInstance[s32Handle - 1].pMemHead != HI_NULL)
    {
        pTemp = gs_PngApiInstance[s32Handle - 1].pMemHead->pNext;
#ifdef HIPNG_STREAMBUF_KERNEL
        PNG_UnMap(gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32PhyAddr,
                  gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.pVir, gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32Size);
#else
    #ifdef CONFIG_GFX_MEM_MMZ
		HI_GFX_Unmap(gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32PhyAddr, HI_TRUE);
		HI_GFX_FreeMem(gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32PhyAddr, HI_TRUE);
    #else
		HI_GFX_Unmap(g_s32MemFd, gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32PhyAddr, gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.pMemHandle);
		HI_GFX_FreeMem(g_s32MemFd, gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.u32PhyAddr, gs_PngApiInstance[s32Handle - 1].pMemHead->stBuf.pMemHandle, HI_TRUE);
    #endif	
#endif
		free(gs_PngApiInstance[s32Handle - 1].pMemHead);
        gs_PngApiInstance[s32Handle - 1].pMemHead = pTemp;
    }

    if (gs_PngApiInstance[s32Handle - 1].pReadParam)
    {
        free(gs_PngApiInstance[s32Handle - 1].pReadParam);
        gs_PngApiInstance[s32Handle - 1].pReadParam = HI_NULL;
    }

    pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

    return;
}

/********************************************************************************************
* func:	destroy decoder
* in:	s32Handle	decoder handle
* out:	none
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
* others:	pair with HI_PNG_CreateDecoder
********************************************************************************************/
HI_S32 HI_PNG_DestroyDecoder(HI_PNG_HANDLE s32Handle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    PNG_CHECK_DEVSTATE();

	PNG_CHECK_HANDLE(s32Handle);
	
    PNG_UnMap_DecoderBuf(s32Handle);

	gs_PngApiInstance[s32Handle - 1].bValid = HI_FALSE;
	
    s32Ret = ioctl(g_s32PngFd, PNG_DESTROY_DECODER, &s32Handle);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    return HI_SUCCESS;
}

/* default stream buffer size*/
#define PNG_BUF_DEFAULTSIZE (128 * 1024)

/********************************************************************************************
* func:	Alloc code stream buffer
* in:	s32Handle	decoder handle
* in:	pstBuf->u32Len	apply the size of buffer
* out:	pstBuf	return buf information,include  basic physical addr and the size of buffer
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
* ret:	HI_ERR_PNG_NOMEM	memory is not enough
* ret:	HI_ERR_PNG_NULLPTR	pointer is null
* others:
			Get code stream buffer, write into code steam data;
			if pstBuf->u32Len = 0, alloc default size in memory,
			otherwise alloc by the size of pstBuf->u32Len.
			while output, the value of pstBuf->u32Len equal to its used
********************************************************************************************/
HI_S32 HI_PNG_AllocBuf(HI_PNG_HANDLE s32Handle, HI_PNG_BUF_S *pstBuf)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PNG_GETBUF_CMD_S stCmd = {0};
    PNG_MEM_NODE_S *pstMemNode = HI_NULL;
#ifndef HIPNG_STREAMBUF_KERNEL
	HI_U32 u32Size;
#endif
	HI_VOID *pMemHandle = HI_NULL;
	HI_BOOL bMmu = HI_TRUE;
#ifdef CONFIG_GFX_MMU_SUPPORT
	HI_CHAR *pZoneName = "iommu";
#else
	HI_CHAR *pZoneName = NULL;
#endif

    if (HI_NULL == pstBuf)
    {
        return HI_ERR_PNG_NULLPTR;
    }

    PNG_CHECK_DEVSTATE();

#ifndef HIPNG_STREAMBUF_KERNEL
	u32Size = (pstBuf->u32Size > PNG_BUF_DEFAULTSIZE)?pstBuf->u32Size:PNG_BUF_DEFAULTSIZE;
	u32Size = ((u32Size + 0xf) & 0xfffffff0);
    #ifdef CONFIG_GFX_MEM_MMZ
	pstBuf->u32PhyAddr = HI_GFX_AllocMem(u32Size, 16, pZoneName, "HIPNG_STREAM_BUF", &bMmu);
    #else
	pstBuf->u32PhyAddr = HI_GFX_AllocMem(g_s32MemFd, u32Size, 16, pZoneName, "HIPNG_STREAM_BUF", &pMemHandle, &bMmu);
    #endif
	if (0 == pstBuf->u32PhyAddr)
	{
		return HI_ERR_PNG_NOMEM;
	}

#ifdef CONFIG_GFX_MEM_MMZ
	pstBuf->pVir = HI_GFX_MapCached(pstBuf->u32PhyAddr, bMmu);
#else
	pstBuf->pVir = HI_GFX_MapCached(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle);
#endif
	if (NULL == pstBuf->pVir)
	{
#ifdef CONFIG_GFX_MEM_MMZ
		HI_GFX_FreeMem(pstBuf->u32PhyAddr, bMmu);
#else
		HI_GFX_FreeMem(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle, bMmu);
#endif
		return HI_ERR_PNG_NOMEM;
	}

	pstBuf->u32Size = u32Size;
#endif

    stCmd.s32Handle = s32Handle;
    //memcpy(&stCmd.stBuf, pstBuf, sizeof(HI_PNG_BUF_S));
    stCmd.stBuf.u32PhyAddr = pstBuf->u32PhyAddr;
	stCmd.stBuf.u32Size = pstBuf->u32Size;
	stCmd.stBuf.u64Vir = (HI_U64)(unsigned long)(pstBuf->pVir);

    s32Ret = ioctl(g_s32PngFd, PNG_ALLOC_BUF, &stCmd);
    if (s32Ret < 0)
    {
#ifndef HIPNG_STREAMBUF_KERNEL
    #ifdef CONFIG_GFX_MEM_MMZ
		HI_GFX_Unmap(pstBuf->u32PhyAddr, bMmu);
		HI_GFX_FreeMem(pstBuf->u32PhyAddr, bMmu);
    #else
    	HI_GFX_Unmap(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle);
		HI_GFX_FreeMem(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle, bMmu);
    #endif
#endif
        return s32Ret;
    }

#ifdef HIPNG_STREAMBUF_KERNEL
    //memcpy(pstBuf, &stCmd.stBuf, sizeof(HI_PNG_BUF_S));
	pstBuf->u32PhyAddr = stCmd.stBuf.u32PhyAddr;
	pstBuf->pVir = (HI_VOID *)(stCmd.stBuf.u64Vir);
	pstBuf->u32Size = stCmd.stBuf.u32Size;

    /* memory map, create mapping list */
    pstBuf->pVir = PNG_Map(pstBuf->u32PhyAddr, pstBuf->u32Size);
    if (HI_NULL == pstBuf->pVir)
    {
        return HI_ERR_PNG_SYS;
    }
#endif

    pstMemNode = (PNG_MEM_NODE_S *)malloc(sizeof(PNG_MEM_NODE_S));
    if (HI_NULL == pstMemNode)
    {
#ifdef HIPNG_STREAMBUF_KERNEL
        PNG_UnMap(pstBuf->u32PhyAddr, pstBuf->pVir, pstBuf->u32Size);
#else
    #ifdef CONFIG_GFX_MEM_MMZ
		HI_GFX_Unmap(pstBuf->u32PhyAddr, bMmu);
		HI_GFX_FreeMem(pstBuf->u32PhyAddr, bMmu);
    #else
		HI_GFX_Unmap(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle);
		HI_GFX_FreeMem(g_s32MemFd, pstBuf->u32PhyAddr, pMemHandle, bMmu);
    #endif
#endif
        return HI_ERR_PNG_NOMEM;
    }

    memcpy(&pstMemNode->stBuf, pstBuf, sizeof(HI_PNG_BUF_S));
	pstMemNode->stBuf.u32Len = 0;
	pstMemNode->stBuf.pMemHandle = pMemHandle;
	pstMemNode->pNext = HI_NULL;

    do
    {
        s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
    } while (s32Ret != HI_SUCCESS);

    /* insert node into list head */
	if (gs_PngApiInstance[s32Handle - 1].pMemHead)
	{
		gs_PngApiInstance[s32Handle - 1].pMemTail->pNext = pstMemNode;
	}
	else
	{
		gs_PngApiInstance[s32Handle - 1].pMemHead = pstMemNode;
	}
	gs_PngApiInstance[s32Handle - 1].pMemTail = pstMemNode;

    pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

    return HI_SUCCESS;
}

/********************************************************************************************
* func:	release code stream buffer
* in:	s32Handle	decoder handle
* out:      none
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
* others:
*********************************************************************************************/
HI_S32 HI_PNG_ReleaseBuf(HI_PNG_HANDLE s32Handle)
{
    HI_S32 s32Ret = HI_SUCCESS;

    PNG_CHECK_DEVSTATE();

    /* the two steps of release buffer:
    1. release map in kerneled and memory
    2. release map in user */

    s32Ret = ioctl(g_s32PngFd, PNG_RELEASE_BUF, &s32Handle);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    /* Unmap in user */
    PNG_UnMap_DecoderBuf(s32Handle);

    return HI_SUCCESS;
}

/********************************************************************************************
* func:	set code stream size
* in:	        s32Handle	decoder handle
* in:	        pstBuf	 info of code stream buffer
* out:
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	unlawful decoder handle
* ret:	HI_ERR_PNG_INVALID_PARAM	invalid code stream size
* ret:  HI_ERR_PNG_NULLPTR  point is null
* others:
		set the size of code stream, after get code stream buffer and write code stream data
		the size of code stream is not more than the size of buffer
*********************************************************************************************/
HI_S32 HI_PNG_SetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 u32Len)
{
    PNG_SETSTREAM_CMD_S stCmd = {0};
	HI_S32 s32Ret;
	PNG_MEM_NODE_S *pTemp = HI_NULL;

    PNG_CHECK_DEVSTATE();

    stCmd.s32Handle = s32Handle;
    stCmd.u32Phyaddr = u32Phyaddr;
    stCmd.u32Len = u32Len;

	s32Ret = ioctl(g_s32PngFd, PNG_SET_STREAMLEN, &stCmd);
	if (s32Ret < 0)
	{
		return s32Ret;
	}

    do
    {
        s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
    } while (s32Ret != HI_SUCCESS);

	if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pMemHead)
	{
		pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
		return HI_ERR_PNG_INVALID_PARAM;
	}

	pTemp = gs_PngApiInstance[s32Handle - 1].pMemHead; 
    while (pTemp)
    {
        if (pTemp->stBuf.u32PhyAddr == u32Phyaddr)
			break;
		pTemp = pTemp->pNext;
	}

	if ((HI_NULL == pTemp) || (pTemp->stBuf.u32Size < u32Len))
	{
		pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
		return HI_ERR_PNG_INVALID_PARAM;
	}
	else
	{
		pTemp->stBuf.u32Len = u32Len;
	}

    pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

    return HI_SUCCESS;
}

/********************************************************************************************
* func:	Get code stream size
* in:	        s32Handle	decoder handle
* in:	        u32Phyaddr	physical size of buffer
* out:	pu32Len length of code stream
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
* ret:  HI_ERR_PNG_NULLPTR  pointer is null
* others:
*********************************************************************************************/
HI_S32 HI_PNG_GetStreamLen(HI_PNG_HANDLE s32Handle, HI_U32 u32Phyaddr, HI_U32 *pu32Len)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PNG_SETSTREAM_CMD_S stCmd = {0};

    if (pu32Len == HI_NULL)
    {
        return HI_ERR_PNG_NULLPTR;
    }

    PNG_CHECK_DEVSTATE();

    stCmd.s32Handle = s32Handle;
    stCmd.u32Phyaddr = u32Phyaddr;

    s32Ret = ioctl(g_s32PngFd, PNG_GET_STREAMLEN, &stCmd);
    if (s32Ret < 0)
    {
        return s32Ret;
    }

    *pu32Len = stCmd.u32Len;

    return HI_SUCCESS;
}


/********************************************************************************************
* func:	start decode
* in:	        s32Handle	decoder handle
* in:	        pstDecInfo	decoder setting information
* out:
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
* ret:  HI_ERR_PNG_NULLPTR pointer is null
* ret:  HI_ERR_PNG_NOSTREAM  no code stream
* ret:  HI_PNG_ERR_INTERNAL  decode error
* others:
*********************************************************************************************/
HI_S32 HI_PNG_Decode(HI_PNG_HANDLE s32Handle,  const HI_PNG_DECINFO_S *pstDecInfo)
{
    PNG_DECODE_CMD_S stCmd = {0};

#ifndef CONFIG_GFX_MEM_MMZ
	PNG_MEM_NODE_S *pTemp = HI_NULL;
#endif

    if (HI_NULL == pstDecInfo)
    {
        return HI_ERR_PNG_NULLPTR;
    }

    PNG_CHECK_DEVSTATE();

	PNG_CHECK_HANDLE(s32Handle);

#ifndef HIPNG_STREAMBUF_KERNEL
    #ifdef CONFIG_GFX_MEM_MMZ
	    HI_GFX_Flush(0, HI_TRUE);
    #else
    	pTemp = gs_PngApiInstance[s32Handle - 1].pMemHead; 
        while (pTemp)
        {
            HI_GFX_Flush(g_s32MemFd, 0, pTemp->stBuf.pMemHandle);
    		pTemp = pTemp->pNext;
    	}
    #endif
#endif

    stCmd.s32Handle = s32Handle;

    memcpy(&stCmd.stDecInfo, pstDecInfo, sizeof(HI_PNG_DECINFO_S));

    return ioctl(g_s32PngFd, PNG_DECODE, &stCmd);
}

/********************************************************************************************
* func:	Get decode result
* in:	s32Handle	decoder handle
* in:	bBlock		if block
* out:	pstDecResult	decode result
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
* ret:	HI_ERR_PNG_NULLPTR	pointer is null
* others:
********************************************************************************************/
HI_S32 HI_PNG_GetResult(HI_PNG_HANDLE s32Handle, HI_BOOL bBlock, HI_PNG_STATE_E * peDecState)
{
    HI_S32 s32Ret = HI_SUCCESS;
    PNG_DECRESULT_CMD_S stCmd = {0};

    if (HI_NULL == peDecState)
    {
        return HI_ERR_PNG_NULLPTR;
    }

    PNG_CHECK_DEVSTATE();

    stCmd.s32Handle = s32Handle;
    stCmd.bBlock = bBlock;

    s32Ret = ioctl(g_s32PngFd, PNG_GET_DECRESULT, &stCmd);

    *peDecState = stCmd.eDecResult;

    return s32Ret;
}

/********************************************************************************************
* func:	Get read function pointer
* in:	s32Handle	decoder handle
* out:	pReadFunc 	read fuction
* ret:	HI_SUCCESS
* ret:	HI_ERR_PNG_NOOPEN	device is not open
* ret:	HI_ERR_PNG_INVALID_HANDLE	invalid decoder handle
* ret:	HI_ERR_PNG_NULLPTR	pointer is null
* others:
*********************************************************************************************/
HI_S32 HI_PNG_GetReadPtr(HI_PNG_HANDLE s32Handle, HI_PNG_READ_FN *ppReadFunc)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if (HI_NULL == ppReadFunc)
    {
        return HI_ERR_PNG_NULLPTR;
    }

	PNG_CHECK_DEVSTATE();

	PNG_CHECK_HANDLE(s32Handle);
	
    do
    {
        s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
    } while (s32Ret != HI_SUCCESS);

	if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pMemHead)
	{
		pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
		*ppReadFunc = HI_NULL;
		return HI_SUCCESS;
	}

    if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pReadParam)
    {
        gs_PngApiInstance[s32Handle - 1].pReadParam = (PNG_READ_INFO_S *)malloc(sizeof(PNG_READ_INFO_S));
        if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pReadParam)
        {
            pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
            return HI_ERR_PNG_NOMEM;
        }
        gs_PngApiInstance[s32Handle - 1].pReadParam->u32Read = 0;
        gs_PngApiInstance[s32Handle - 1].pReadParam->pstMemNode = gs_PngApiInstance[s32Handle - 1].pMemHead;
    }

    pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

    *ppReadFunc = HIPNG_Read;

    return HI_SUCCESS;
}

/********************************************************************************************
* func:	Read data form code stream buffer in driver
* in:	u32Len  lenth
* in:   s32Handle decoder handle
* out:	get code stream data
* ret:  bytes read in fact
* others:
********************************************************************************************/
HI_U32 HIPNG_Read(HI_UCHAR *pBuf, HI_U32 u32Len, HI_PNG_HANDLE s32Handle)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UCHAR *pViraddr = HI_NULL;
    HI_U32 u32ReadLen = 0;
	PNG_READ_INFO_S *pReadParam = gs_PngApiInstance[s32Handle - 1].pReadParam;

    if ((HI_NULL == pBuf) || (HI_NULL == pReadParam)
		|| (HI_NULL == pReadParam->pstMemNode))
    {
        return 0;
    }

    do
    {
        s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
    } while (s32Ret != HI_SUCCESS);

    pViraddr = pReadParam->pstMemNode->stBuf.pVir;

    while (pViraddr)
    {
        if (u32Len <= (pReadParam->pstMemNode->stBuf.u32Len - pReadParam->u32Read))
        {
            memcpy(pBuf + u32ReadLen, pViraddr+pReadParam->u32Read, u32Len);
            pReadParam->u32Read += u32Len;
            u32ReadLen += u32Len;
            pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
            return u32ReadLen;
        }
        else
        {
			HI_U32 u32TmpLen = pReadParam->pstMemNode->stBuf.u32Len - pReadParam->u32Read;
            memcpy(pBuf + u32ReadLen, pViraddr+pReadParam->u32Read,u32TmpLen);
            u32ReadLen += u32TmpLen;
            u32Len -= u32TmpLen;

            pReadParam->u32Read = 0;
			pReadParam->pstMemNode = pReadParam->pstMemNode->pNext;
            if (NULL == pReadParam->pstMemNode)
            {
                pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
                return u32ReadLen;
            }

            pViraddr = pReadParam->pstMemNode->stBuf.pVir;
        }
    }

    pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

    return u32ReadLen;
}

#ifdef HIPNG_STREAMBUF_KERNEL
HI_VOID *PNG_Map(HI_U32 u32Phyaddr, HI_U32 u32Size)
{
    HI_CHAR *pVir;

    pVir = (HI_CHAR *)mmap(NULL, u32Size + (u32Phyaddr & 0xfff), PROT_READ | PROT_WRITE, MAP_SHARED, g_s32MemFd,
                           (long)(u32Phyaddr & 0xfffff000));
    if (pVir == MAP_FAILED)
    {
        return HI_NULL;
    }

    return (HI_VOID *)(pVir+(u32Phyaddr & 0xfff));
}

HI_VOID PNG_UnMap(HI_U32 u32Phyaddr, HI_VOID *pVir, HI_U32 u32Size)
{
    PNG_API_ASSERT((pVir != HI_NULL));
    PNG_API_ASSERT((u32Phyaddr != 0));

    munmap((HI_CHAR *)pVir-(u32Phyaddr&0xfff), u32Size+(u32Phyaddr & 0xfff));

    return;
}
#endif

HI_VOID HI_PNG_GetStream(HI_PNG_HANDLE s32Handle, HI_VOID **ppStream, HI_U32 *pu32Len)
{
	PNG_READ_INFO_S *pReadParam;
	HI_S32 s32Ret;

	do
	{
		s32Ret = pthread_mutex_lock(&gs_PngApiInstance[s32Handle - 1].stLock);
	} while (s32Ret != HI_SUCCESS);

	if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pMemHead)
	{
		pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
		*ppStream = NULL;
		*pu32Len = 0;
		return;
	}
	
    if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pReadParam)
    {
        gs_PngApiInstance[s32Handle - 1].pReadParam = (PNG_READ_INFO_S *)malloc(sizeof(PNG_READ_INFO_S));
        if (HI_NULL == gs_PngApiInstance[s32Handle - 1].pReadParam)
        {
            pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
			*ppStream = NULL;
			*pu32Len = 0;
			return;
        }
        gs_PngApiInstance[s32Handle - 1].pReadParam->u32Read = 0;
        gs_PngApiInstance[s32Handle - 1].pReadParam->pstMemNode = gs_PngApiInstance[s32Handle - 1].pMemHead;
    }

	pReadParam = gs_PngApiInstance[s32Handle - 1].pReadParam;
	if (NULL == pReadParam->pstMemNode)
	{
		pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);
		*ppStream = NULL;
		*pu32Len = 0;
		return;
	}
	*ppStream = pReadParam->pstMemNode->stBuf.pVir;
	*pu32Len = pReadParam->pstMemNode->stBuf.u32Len;	

	pReadParam->pstMemNode = pReadParam->pstMemNode->pNext;

	pthread_mutex_unlock(&gs_PngApiInstance[s32Handle - 1].stLock);

	return;
}
#endif
