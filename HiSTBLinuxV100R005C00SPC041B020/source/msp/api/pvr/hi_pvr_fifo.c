/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_fifo.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   : BUFFER module
  History       :
  1.Date        : 2008/04/24
    Author      : q46153
    Modification: Created file

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mount.h>
#include <fcntl.h>

#include "hi_type.h"
#include "pvr_debug.h"
#include "hi_pvr_fifo.h"
#include "hi_adpt_thread.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define PACKET_IDENTIFIER(a, b)         ((HI_U32)((((a) & 0x1f) << 8) | ((b) & 0xff)))

#define PVR_HISI_DEFAULT_FILE_NODE_SIZE (4000LLU*1024*1024)
/** */
typedef enum hiPVR_FILE_NAME_FMT_E
{
    PVR_FILE_NAME_FMT_HISI,
    PVR_FILE_NAME_FMT_XX_DOT_TS,
    PVR_FILE_NAME_FMT_XX_DOT_ENC_DOT_TS,
    PVR_FILE_NAME_FMT_XX_DOT_AD_DOT_TS,

    PVR_FILE_NAME_FMT_BUTT
} PVR_FILE_NAME_FMT_E;

typedef struct hiPVR_FILE_S
{
    HI_BOOL bOpened;
    int     systemFd;
    int     openMode;

    HI_U64  u64CycMaxSize;

    HI_U64  u64FileNodeSize;
    HI_U64  u64StartOfCurFd;   /* = u64FileNodeSize * nodIdx, prevent data from calculating again, save it */
    HI_U64  u64SeekOffset;
    HI_U64  u64LastWPtrEnd;
    HI_CHAR szFileName[PVR_MAX_FILENAME_LEN];
    PVR_FILE_NAME_FMT_E enFileNameFmt;
} PVR_FILE_S;

static PVR_FILE_S g_stPvrFiles[PVR_FILE_MAX_FILE];
HI_U64 g_u64PVR_FileNodeMaxSize = 4000LLU*1024*1024;

#define PVRFileGetNodeIdx(offset, nodeSize)  ((offset)/nodeSize)
#define PVRFileGetNodeOffset(offset, nodeSize)  ((offset)%nodeSize)
#define PVRFileOffsetMatchNode(offset, nodeStartOffset, nodeSize) ((offset >= nodeStartOffset) && (offset < (nodeStartOffset + nodeSize)) )

#define PVRFileGetRealFNameHisi(fileName, realName, nodeIdx) do {\
        if (0 == nodeIdx)\
        {\
            memset(realName, 0x00, PVR_MAX_FILENAME_LEN);\
            snprintf(realName, PVR_MAX_FILENAME_LEN - 1,"%s", fileName); \
        }\
        else\
        {\
            memset(realName, 0x00, PVR_MAX_FILENAME_LEN);\
            snprintf(realName, PVR_MAX_FILENAME_LEN - 1,"%s.%04d.ts", fileName, nodeIdx); \
        }\
    }while(0)

#define PVRFileGetRealFNameXxTs(fileName, realName, nodeIdx) do {\
        memset(realName, 0x00, PVR_MAX_FILENAME_LEN);\
        snprintf(realName,PVR_MAX_FILENAME_LEN - 1, "%s%02d.ts", fileName, nodeIdx); \
    }while(0)

#define PVRFileGetRealFNameXxEncTs(fileName, realName, nodeIdx) do {\
        memset(realName, 0x00, PVR_MAX_FILENAME_LEN);\
        snprintf(realName, PVR_MAX_FILENAME_LEN - 1,"%s%02d.enc.ts", fileName, nodeIdx); \
    }while(0)

#define PVRFileGetRealFNameXxAdTs(fileName, realName, nodeIdx) do {\
        memset(realName, 0x00, PVR_MAX_FILENAME_LEN);\
        snprintf(realName, PVR_MAX_FILENAME_LEN - 1,"%s%02d.ad.ts", fileName, nodeIdx); \
    }while(0)



static PVR_FILE_NAME_FMT_E PVR_Check_FileType(const HI_CHAR *filename)
{
    HI_CHAR  fNameReal[PVR_MAX_FILENAME_LEN] = {0};

    //lint -e506 -e774
    PVRFileGetRealFNameHisi(filename, fNameReal, 0);
    //lint +e506 +e774
    if (PVR_CHECK_FILE_EXIST(fNameReal))
    {
        return PVR_FILE_NAME_FMT_HISI;
    }

    PVRFileGetRealFNameXxTs(filename, fNameReal, 0);
    if (PVR_CHECK_FILE_EXIST(fNameReal))
    {
        return PVR_FILE_NAME_FMT_XX_DOT_TS;
    }

    PVRFileGetRealFNameXxEncTs(filename, fNameReal, 0);
    if (PVR_CHECK_FILE_EXIST(fNameReal))
    {
        return PVR_FILE_NAME_FMT_XX_DOT_ENC_DOT_TS;
    }

    PVRFileGetRealFNameXxAdTs(filename, fNameReal, 0);
    if (PVR_CHECK_FILE_EXIST(fNameReal))
    {
        return PVR_FILE_NAME_FMT_XX_DOT_AD_DOT_TS;
    }

    return PVR_FILE_NAME_FMT_BUTT;
}


/*
gongfu00.ts
gongfu01.ts
gongfu02.ts......
-------------
cctv00.enc.ts
cctv01.enc.ts
cctv02.enc.ts
*/
static HI_VOID PVRFileGetRealFName(const HI_CHAR *pFileName,  HI_CHAR *pRealName, HI_S32 nodeIdx)
{
    PVR_FILE_NAME_FMT_E fileFmt;

    fileFmt = PVR_Check_FileType(pFileName);

    switch (fileFmt)
    {
        case PVR_FILE_NAME_FMT_HISI:
            PVRFileGetRealFNameHisi(pFileName, pRealName, nodeIdx);
            break;
        case PVR_FILE_NAME_FMT_XX_DOT_TS:
            PVRFileGetRealFNameXxTs(pFileName, pRealName, nodeIdx);
            break;
        case PVR_FILE_NAME_FMT_XX_DOT_ENC_DOT_TS:
            PVRFileGetRealFNameXxEncTs(pFileName, pRealName, nodeIdx);
            break;
        case PVR_FILE_NAME_FMT_XX_DOT_AD_DOT_TS:
            PVRFileGetRealFNameXxAdTs(pFileName, pRealName, nodeIdx);
            break;
        default:
            PVRFileGetRealFNameHisi(pFileName, pRealName, nodeIdx);
    }

    return;
}

HI_S32 PVRFileGetOffsetFName(PVR_FILE64 file, HI_U64 offset, HI_CHAR *pRealName)
{
    int pvrFd = PVRFileGetPVRFd(file);
    HI_U32 nodeIdx;
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];

    nodeIdx = (HI_U32)PVRFileGetNodeIdx(offset, pPvrFile->u64FileNodeSize);
    PVRFileGetRealFName(pPvrFile->szFileName, pRealName, (HI_S32)nodeIdx);

    return HI_SUCCESS;
}



ssize_t PVR_READALL(void *buf, size_t size, PVR_FILE fd,  off64_t offset)
{
    ssize_t nread = 0, n;
    HI_U8 *pMem = HI_NULL;

    if ((0 == size) || (HI_NULL == buf))
    {
        return 0;
    }
    pMem = (HI_U8 *)buf;
    do {
        if ((n = (ssize_t)PVR_READ((void*)(pMem + nread), ((size) - (size_t)nread), fd, offset + (off64_t)nread)) == (-1))
        {
            //lint -e774
            if (NULL != &errno)
            {
                if (EINTR == errno)
                {
                    HI_WARN_PVR("read err @EINTR\n");
                    continue;
                }
                else
                {
                    return -1;
                }
            }
            //lint +e774
        }

        if (0 == n) /* EOF */
        {
            return (ssize_t)nread;
        }
        nread += n;
        if (nread < (ssize_t)size)
        {
            HI_WARN_PVR("read ok @ less\n");
        }
    }while(nread < (ssize_t)size);

    return (ssize_t)nread;
}

/*****************************************************************************
 Prototype       : PVR_CHECK_FILE_EXIST
 Description     : check file exist
 Input           : pszFileName  **
 Output          : None
 Return Value    :
 Global Variable
    Read Only    :
    Read & Write :
  History
  1.Date         : 2008/4/26
    Author       : q46153
    Modification : Created function

*****************************************************************************/
HI_BOOL PVR_CHECK_FILE_EXIST(const HI_CHAR *pszFileName)
{
    HI_S32 fd = -1;
    fd = open(pszFileName, O_RDONLY);
    if (0 > fd)
    {
        return HI_FALSE;
    }
    else
    {
        close(fd);
        return HI_TRUE;
    }  
}

HI_BOOL PVR_CHECK_FILE_EXIST64(const HI_CHAR *pszFileName)
{
    HI_CHAR fNameReal[PVR_MAX_FILENAME_LEN] = {0};

    HI_S32 fd = -1;

    PVRFileGetRealFName(pszFileName, fNameReal, 0);
    fd = open(pszFileName, O_RDONLY);
    if (0 > fd)
    {
        return HI_FALSE;
    }
    else
    {
        close(fd);
        return HI_TRUE;
    } 
}


HI_VOID PVR_REMOVE_FILE64(const HI_CHAR *pszFileName)
{
    int nodeIdx = 0;

    HI_CHAR  fNameReal[PVR_MAX_FILENAME_LEN] = {0};

    PVRFileGetRealFName(pszFileName, fNameReal, nodeIdx);
    while(PVR_CHECK_FILE_EXIST(fNameReal))
    {
        (HI_VOID)PVR_REMOVE(fNameReal);

        nodeIdx++;
        PVRFileGetRealFName(pszFileName, fNameReal, nodeIdx);
    };

    /*
    perror("remove: ");
    bHave = PVR_CHECK_FILE_EXIST(pszFileName);

    HI_ERR_PVR("file:%s , exist:%d\n", pszFileName, bHave);
    */
}

HI_U64 PVR_FILE_GetFileSize(const HI_CHAR *pszFileName)
{
    HI_S64 size = 0;
    PVR_FILE fd;

    fd = pvr_open(pszFileName, PVR_FOPEN_MODE_DATA_READ, 0777);
    if (PVR_FILE_INVALID_FILE != fd)
    {
        size = (HI_S64)pvr_lseek(fd, 0, SEEK_END);
        pvr_close(fd);
    }

    return (HI_U64)size;
}

HI_U64 PVR_FILE_GetFileSize64(const HI_CHAR *pszFileName)
{
    HI_S64      size = 0;
    PVR_FILE    fd;
    int         nodeIdx = 0;
    HI_U64      nodeSize = 0;
    HI_CHAR  fNameReal[PVR_MAX_FILENAME_LEN] = {0};

    PVRFileGetRealFName(pszFileName, fNameReal, nodeIdx);
    while(PVR_CHECK_FILE_EXIST(fNameReal))
    {
        nodeSize = PVR_FILE_GetFileSize(fNameReal);
        size += (HI_S64)nodeSize;
        nodeIdx++;
        PVRFileGetRealFName(pszFileName, fNameReal, nodeIdx);
    };

    nodeIdx--;
    PVRFileGetRealFName(pszFileName, fNameReal, nodeIdx);
    fd = pvr_open(fNameReal, PVR_FOPEN_MODE_DATA_READ, 0777);
    if (PVR_FILE_INVALID_FILE != fd)
    {
        size += (HI_S64)pvr_lseek(fd, 0, (int)SEEK_END);
        size -= (HI_S64)nodeSize;
        pvr_close(fd);
    }


    return (HI_U64)size;
}

PVR_FILE64 PVR_OPEN64(const HI_CHAR *filename, int mode)
{
    HI_S32 pvrFd;
    int sysFd;
    HI_U64   fileNodeSize;
    HI_CHAR  fNameOpen[PVR_MAX_FILENAME_LEN] = {0};
    PVR_FILE_NAME_FMT_E fileFmt;
    HI_U32 u32FileNameLen = 255;
    
    if (PVR_FOPEN_MODE_DATA_READ == mode)
    {
        fileFmt = PVR_Check_FileType(filename);
        if (PVR_FILE_NAME_FMT_BUTT == fileFmt)
        {
            HI_ERR_PVR("can NOT tell File '%s' format, open failed.\n", filename);
            return -1;
        }
    }
    else
    {
        fileFmt = PVR_FILE_NAME_FMT_HISI;
    }

    for (pvrFd = 0; pvrFd < PVR_FILE_MAX_FILE; pvrFd++)
    {
        if (HI_FALSE == g_stPvrFiles[pvrFd].bOpened)
        {
            g_stPvrFiles[pvrFd].bOpened = HI_TRUE;
            break;
        }
    }

    if (pvrFd >= PVR_FILE_MAX_FILE)  /* NO fd */
    {
        HI_ERR_PVR("PVR file: can NOT open more file.\n");
        return -1;
    }

    PVRFileGetRealFName(filename, fNameOpen, 0);

    g_stPvrFiles[pvrFd].enFileNameFmt = fileFmt;
    if (PVR_FILE_NAME_FMT_HISI == fileFmt)
    {
        fileNodeSize = PVR_HISI_DEFAULT_FILE_NODE_SIZE;
    }
    else
    {
        fileNodeSize = PVR_FILE_GetFileSize(fNameOpen);
    }

    g_stPvrFiles[pvrFd].u64FileNodeSize = fileNodeSize;
    HI_PRINT("PVR file: fmt:%d, NodeSize:%llu.\n", fileFmt, fileNodeSize);

    sysFd = PVR_OPEN(fNameOpen, mode);

    if (-1 == sysFd)
    {
        g_stPvrFiles[pvrFd].bOpened = HI_FALSE;
        g_stPvrFiles[pvrFd].systemFd = PVR_FILE_INVALID_FILE;
        HI_ERR_PVR("PVR file: can NOT open '%s', mode:%#x.\n", fNameOpen, mode);
        return -1;
    }

    memset(g_stPvrFiles[pvrFd].szFileName, '\0', sizeof(g_stPvrFiles[pvrFd].szFileName));
    if (u32FileNameLen > strlen(filename))
    {
        u32FileNameLen = (HI_U32)strlen(filename);
    }
    strncpy(g_stPvrFiles[pvrFd].szFileName, filename, u32FileNameLen);

    g_stPvrFiles[pvrFd].systemFd = sysFd;
    g_stPvrFiles[pvrFd].openMode = mode;
    g_stPvrFiles[pvrFd].u64StartOfCurFd = 0;
    g_stPvrFiles[pvrFd].u64SeekOffset = 0;
    g_stPvrFiles[pvrFd].u64CycMaxSize = 0;
    g_stPvrFiles[pvrFd].u64LastWPtrEnd = 0;

    return pvrFd + PVR_FILE_FD_BASE;
}

int PVR_CLOSE64(PVR_FILE64 file)
{
    int pvrFd = PVRFileGetPVRFd(file);

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);
    g_stPvrFiles[pvrFd].bOpened = HI_FALSE;

    return PVR_CLOSE(g_stPvrFiles[pvrFd].systemFd);
}

HI_VOID PVR_SET_MAXFILE_SIZE(PVR_FILE64 file, HI_U64 maxSize)
{
    int pvrFd = PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    if (pvrFd >= PVR_FILE_MAX_FILE)
    {
        return ;
    }

    if (g_stPvrFiles[pvrFd].bOpened != HI_TRUE)
    {
        return;
    }

    pPvrFile = &g_stPvrFiles[pvrFd];
    pPvrFile->u64CycMaxSize = maxSize;

    return;
}

int PVR_FSYNC64(PVR_FILE64 file)
{
    int pvrFd = PVRFileGetPVRFd(file);

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    return PVR_FSYNC(g_stPvrFiles[pvrFd].systemFd);
}

ssize_t PVR_PREAD64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file, HI_U64 offset)
{
    HI_U32 nodeIdx;
    int sysFd;
    int pvrFd = PVRFileGetPVRFd(file);
    HI_U32      readLen1 = 0;
    HI_U32      readLen2 = 0;
    HI_U32      readed;
    PVR_FILE_S *pPvrFile;
    
    if ((0 == size) || (HI_NULL == pMem))
    {
        return 0;
    }
    
    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];
    

    if (PVRFileOffsetMatchNode(offset, pPvrFile->u64StartOfCurFd, pPvrFile->u64FileNodeSize))
    {
        sysFd = pPvrFile->systemFd;
        readLen1 = (offset + size >= pPvrFile->u64StartOfCurFd + pPvrFile->u64FileNodeSize)
                    ?  (HI_U32)(pPvrFile->u64StartOfCurFd + pPvrFile->u64FileNodeSize - offset)
                    : size;

        readed = (HI_U32)PVR_READ((void *)pMem, (size_t)readLen1, sysFd, (off64_t)(offset - pPvrFile->u64StartOfCurFd));

        if (readed != readLen1)
        {
            //perror("PVR_READ...222...,hi_pvr_fifo.c");
            if(readed > 0)
            {
                pPvrFile->u64SeekOffset = offset +  (HI_U64)readed;
                PVR_USLEEP(100000);
            }
            return (ssize_t)readed;
        }
        offset += (HI_U64)readLen1;
    }

    if (readLen1 != size) /* need another file */
    {
        HI_CHAR  fNameOpen[PVR_MAX_FILENAME_LEN] = {0};
        PVR_FSYNC(pPvrFile->systemFd);

        PVR_CLOSE(pPvrFile->systemFd);
        nodeIdx = (HI_U32)PVRFileGetNodeIdx(offset, pPvrFile->u64FileNodeSize);
        PVRFileGetRealFName(pPvrFile->szFileName, fNameOpen, (HI_S32)nodeIdx);
        sysFd = PVR_OPEN(fNameOpen, pPvrFile->openMode);

        if (-1 == sysFd)
        {
            HI_ERR_PVR("[%s] PVR can't open file:'%s' for read.offset=%lld, u64FileNodeSize=%lld.\n",
                __FUNCTION__, fNameOpen, offset, pPvrFile->u64FileNodeSize);
            return (ssize_t)readLen1;
        }
        pPvrFile->systemFd = sysFd;
        pPvrFile->u64StartOfCurFd = nodeIdx;
        pPvrFile->u64StartOfCurFd *= pPvrFile->u64FileNodeSize;

        readLen2 = size - readLen1;
        readed = (HI_U32)PVR_READ((HI_VOID *)(pMem+readLen1), (size_t)readLen2, sysFd,(off64_t)(offset - pPvrFile->u64StartOfCurFd));
        if (-1 == (HI_S32)readed)
        {
            //perror("PVR_READ...333...");
            return -1;
        }
        else //if (readed != readLen2)
        {
            pPvrFile->u64SeekOffset = offset + (HI_U64)readed + (HI_U64)readLen1;
            return (ssize_t)(readed + readLen1);
        }
    }

    pPvrFile->u64SeekOffset = offset +  size;
    return (ssize_t)size;
}


ssize_t PVR_PWRITE64(const void *pMem,  HI_U32 size, PVR_FILE64 file, HI_U64 offset)
{
    HI_U32 nodeIdx;
    int sysFd;
    int pvrFd = PVRFileGetPVRFd(file);
    size_t      readLen1 = 0;
    size_t      readLen2 = 0;
    HI_S64     writen;
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];
    if (0 == size)
    {
        return 0;
    }

#ifdef PVR_CHECK_USB_FILE
    if (HI_SUCCESS != PVR_CheckUsbStillPlugIn())
    {
        return -1;
    }
#endif

    if (PVRFileOffsetMatchNode(offset, pPvrFile->u64StartOfCurFd, pPvrFile->u64FileNodeSize))
    {
        sysFd = pPvrFile->systemFd;
        readLen1 = (offset + size >= pPvrFile->u64StartOfCurFd + pPvrFile->u64FileNodeSize)
                    ? (size_t)(pPvrFile->u64StartOfCurFd + pPvrFile->u64FileNodeSize - offset)
                    : (size_t)size;
        writen = (HI_S64)PVR_WRITE(pMem, readLen1, sysFd, offset - pPvrFile->u64StartOfCurFd);
        if(writen < 0)
        {
            HI_ERR_PVR("%s file:%d, want to write %d,  acturally %d,  offset:%lld,  u64StartOfCurFd: %lld \n",__func__, sysFd, readLen1, writen, offset, pPvrFile->u64StartOfCurFd);
            return -1;
        }
        else if(writen == 0)
        {
            return 0;
        }
        else
        {
            if ((size_t)writen != readLen1)
            {
                pPvrFile->u64SeekOffset = offset + (HI_U64)writen;
                return (ssize_t)writen;
            }
            offset += readLen1;
        }
    }

    if (readLen1 != size) /* need another file */
    {
        HI_CHAR fNameOpen[PVR_MAX_FILENAME_LEN] = {0};
        PVR_CLOSE(pPvrFile->systemFd);
        nodeIdx = (HI_U32)PVRFileGetNodeIdx(offset, pPvrFile->u64FileNodeSize);
        PVRFileGetRealFName(pPvrFile->szFileName, fNameOpen, (HI_S32)nodeIdx);
        sysFd = PVR_OPEN(fNameOpen, pPvrFile->openMode);
        if (-1 == sysFd)
        {
            HI_ERR_PVR("PVR can't open file:'%s' for write.\n", fNameOpen);
            return -1;
        }
        pPvrFile->systemFd = sysFd;
        pPvrFile->u64StartOfCurFd = nodeIdx;
        pPvrFile->u64StartOfCurFd *= pPvrFile->u64FileNodeSize;

        readLen2 = size - readLen1;
        writen = (HI_S64)PVR_WRITE((void*)((size_t)pMem+readLen1), readLen2, sysFd, (off64_t)(offset - pPvrFile->u64StartOfCurFd));
        if ( writen < 0)
        {
            return -1;
        }
        else if(writen == 0)
        {
            return 0;
        }
        else //if (writen != readLen2)
        {
            pPvrFile->u64SeekOffset = offset + (HI_U64)(writen + readLen1);
            return (ssize_t)writen + (ssize_t)readLen1;
        }
    }

    pPvrFile->u64SeekOffset = offset +  size;
    return (ssize_t)size;
}

#if 0 /*obsolete APIs*/
ssize_t PVR_WRITEALL(const void *buf, size_t len, PVR_FILE fd, off64_t offset)
{
    ssize_t n, sizeWriten = 0;

    do {

#ifdef PVR_CHECK_USB_FILE
        if (HI_SUCCESS != PVR_CheckUsbStillPlugIn())
        {
            return -1;
        }
#endif
        n = PVR_WRITE( &((const char *)buf)[sizeWriten],
                len - (size_t)sizeWriten, fd, offset + (off64_t)sizeWriten);
        if (-1 == n)
        {
            //lint -e774
            if (NULL != &errno)
            {
                if (EINTR == errno)
                {
                    continue;
                }
                else
                {
                    return -1;
                }
            }
            //lint +e774
        }

        sizeWriten += n;
    }while((size_t)sizeWriten < len);

    return sizeWriten;
}

HI_S32 PVR_GetTsFileFd(PVR_FILE64 file)
{
    HI_U32      pvrFd = (HI_U32)PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];

    return pPvrFile->systemFd;
}

/*
 * read ts without index file
 */
ssize_t PVR_TS_PREAD64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file, HI_U64 offset)
{
    HI_S32      pvrFd = PVRFileGetPVRFd(file);
    HI_S64      readed;
    PVR_FILE_S *pPvrFile;
    HI_U64      u64Tmp = 0;
    
    if ((0 == size) || (HI_NULL == pMem))
    {
        return 0;
    }
    
    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];

    readed = (HI_S64)PVR_READ((void *)pMem, (size_t)size, pPvrFile->systemFd, (off64_t)offset);
    if (readed < 0)
    {
        HI_ERR_PVR("[PVR_TS_PREAD64] PVR_READ error");
    }
    else if (readed > 0)
    {
        u64Tmp = (HI_U64)readed;
        pPvrFile->u64SeekOffset = offset + u64Tmp;
    }

    return (ssize_t)readed;
}

ssize_t PVR_READ64(HI_U8 *pMem, HI_U32 size, PVR_FILE64 file)
{
    ssize_t ret;
    int pvrFd = PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);
    pPvrFile = &g_stPvrFiles[pvrFd];

    ret = PVR_PREAD64(pMem, size, file, pPvrFile->u64SeekOffset);

    return ret;
}

ssize_t PVR_WRITE64(const void *pMem,  HI_U32 size, PVR_FILE64 file)
{
    ssize_t ret;
    int pvrFd = PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);
    pPvrFile = &g_stPvrFiles[pvrFd];

    ret = PVR_PWRITE64(pMem, size, file, pPvrFile->u64SeekOffset);

    return ret;
}

ssize_t PVR_SEEK64(PVR_FILE64 file, HI_S64 offset, int whence)
{
    int pvrFd = PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    pPvrFile = &g_stPvrFiles[pvrFd];

    switch (whence)
    {
        case SEEK_SET :
            if (offset < 0)
            {
                pPvrFile->u64SeekOffset = 0;
            }
            else
            {
                pPvrFile->u64SeekOffset = (HI_U64)offset;
            }
            break;
        case SEEK_CUR :
            pPvrFile->u64SeekOffset = (pPvrFile->u64SeekOffset + (HI_U64)offset);
            break;
        case SEEK_END:
            return -1;
            //break;
        default:
            return -1;
    }

    return (ssize_t)(pPvrFile->u64SeekOffset);
}

HI_S32 PVRFileGetRealOffset(PVR_FILE64 file, HI_U64 *offset, HI_U64 *pu64OffsetLen)
{
    HI_U32  nodeIdx = 0 ;
    HI_U64  u64StartOfCurFd;
    int pvrFd = PVRFileGetPVRFd(file);
    PVR_FILE_S *pPvrFile;

    PVRFileCheckPVRFd(pvrFd);
    PVRFileCheckPVRFdOpen(pvrFd);

    if(NULL == offset)
    {
        HI_ERR_PVR("Enter parameter error!\n");
        return HI_FAILURE;
    }

    pPvrFile = &g_stPvrFiles[pvrFd];

    /*offset  in current node*/
    if(PVRFileOffsetMatchNode(*offset, pPvrFile->u64StartOfCurFd, pPvrFile->u64FileNodeSize))
    {
        if(NULL != pu64OffsetLen)
        {
            if(*pu64OffsetLen > 0)
            {
                if(PVRFileOffsetMatchNode((*offset + *pu64OffsetLen), pPvrFile->u64StartOfCurFd, pPvrFile->u64FileNodeSize))
                {
                    *offset -= pPvrFile->u64StartOfCurFd;
                }
                else
                {
                    nodeIdx = (HI_U32)PVRFileGetNodeIdx((*offset + *pu64OffsetLen), pPvrFile->u64FileNodeSize);
                    u64StartOfCurFd = (HI_U64)nodeIdx;
                    u64StartOfCurFd *= pPvrFile->u64FileNodeSize;
                    *pu64OffsetLen =*pu64OffsetLen - (*offset + *pu64OffsetLen - u64StartOfCurFd);
                }
            }
            else
            {
                *offset -= pPvrFile->u64StartOfCurFd;
                HI_WARN_PVR("pu64OffsetLen < 0!\n");
            }
        }
        else
        {
            *offset -= pPvrFile->u64StartOfCurFd;
            HI_INFO_PVR("pu64OffsetLen is null!\n");
        }
    }
    else if(*offset > pPvrFile->u64StartOfCurFd)
    {
        if(NULL == pu64OffsetLen)
        {
            nodeIdx = (HI_U32)PVRFileGetNodeIdx(*offset, pPvrFile->u64FileNodeSize);
            u64StartOfCurFd = nodeIdx;
            u64StartOfCurFd *= pPvrFile->u64FileNodeSize;
            *offset -= u64StartOfCurFd;
        }
    }
    else   /*offset  in last node , PVR_PREAD64  set  pPvrFile->u64StartOfCurFd already */
    {
        if((NULL != pu64OffsetLen)&&(0 != *offset))
        {
            nodeIdx = (HI_U32)PVRFileGetNodeIdx(*offset, pPvrFile->u64FileNodeSize);
            u64StartOfCurFd = nodeIdx;
            u64StartOfCurFd *= pPvrFile->u64FileNodeSize;
            *pu64OffsetLen =*pu64OffsetLen - (*offset + *pu64OffsetLen - pPvrFile->u64StartOfCurFd);
            *offset -= u64StartOfCurFd;
        }

        HI_WARN_PVR("offset is in last file!\n");
    }

    return HI_SUCCESS;
}

int PVR_Mount(const char *source, const char *target,
                 const char *filesystemtype, unsigned long mountflags,
                 const void *data)
{
    int ret = -1;
    HI_BOOL bVFAT = HI_TRUE;
    /*HI_BOOL bNTFS = HI_FALSE;*/ 
    HI_BOOL bHIFAT = HI_FALSE;
#ifdef SUPPORT_HIMOUNT
    if (HI_NULL != strcasestr(filesystemtype, "HIFAT"))
    {
        bHIFAT = HI_TRUE;
    }
#endif
    //lint -e774
    /* not supported */
    /*
    if (HI_TRUE == bNTFS && HI_TRUE == bHIFAT)  
    {
        bVFAT = HI_TRUE;
        bNTFS = HI_FALSE;
    }*/

    if (HI_TRUE == bVFAT)
    {
#ifdef SUPPORT_HIMOUNT
    if (HI_TRUE == bHIFAT)
    {
        ret = himount(source, target);
        HI_ERR_PVR("himount(%s,%s) return %d.\n", source, target, ret);
    }
    else
    {
        char system_mount_command[512];
        snprintf(system_mount_command, sizeof(system_mount_command)-1, "mount %s %s", source, target);
        system_mount_command[sizeof(system_mount_command)-1] = 0;
#if 0  /*for unsafe function check:  use #if 0 to disable below two lines*/      
        ret = sys_tem((const char*)system_mount_command);/*if want use this function ,modify the sys_tem to it's right */
        HI_ERR_PVR("\"%s\" return %d.\n", system_mount_command, ret);
#endif        
    }
#else
    ret = mount(source, target, "vfat", 0, NULL);
    HI_INFO_PVR("mount(%s,%s, \"vfat\") return %d.\n", source, target, ret);
#endif
    }
    mountflags = 0;
    data = NULL;
    //lint +e774

    HI_INFO_PVR("filesystemtype =%s\n",filesystemtype);
    UNUSED(bHIFAT);
    UNUSED(data);
    UNUSED(mountflags);
    return ret;
}

int PVR_Umount(const char *target)
{
    return pvr_umount(target);
}
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

