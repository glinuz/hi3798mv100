/***********************************************************************************
*              Copyright 2009 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName   : mpi_demux.c
* Description: media program interface implementation
*
* History    :
* Version   Date         Author     DefectNum    Description
* main\1    2006-07-18   Jianglei   NULL         Create this file.
***********************************************************************************/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <pthread.h>

#ifdef HI_ADVCA_FUNCTION_RELEASE
#define NDEBUG
#endif
#include <assert.h>

#include "hi_type.h"
#include "hi_debug.h"

#include "hi_drv_struct.h"
#include "hi_module.h"
#include "mpi_mmz.h"

#include "demux_debug.h"
#include "hi_mpi_demux.h"
#include "drv_demux_config.h"
#include "drv_demux_ioctl.h"
#include "hi_mpi_mem.h"

#define DMX_DEFAULT_CHANBUF_SIZE        (16 * 1024)
#define DMX_DEFAULT_TSBUF_SIZE          (2 * 1024 * 1024)

/********************** Global Variable define **************************/

#define DMX_COPY_PES 1

HI_S32 g_s32DmxFd = -1;     /* the file discreption for DEMUX module */

static const HI_CHAR    DmxDevName[]  = "/dev/" UMAP_DEVNAME_DEMUX;

static DMX_BUF_S    g_stPoolBuf;

static struct {
    HI_U32 RamPortCnt;
    DMX_BUF_S *BufDesc; 
} g_stTsBuf = {0};

typedef struct {
    HI_HANDLE Handle;
    HI_UNF_DMX_CB_DESC_S Raw;
    pthread_t Context;
}DMX_CB_DESC_S;

static struct {
    HI_U32 DmxCnt;
    HI_U32 ChnCnt;
    DMX_BUF_S *BufDesc;

    DMX_CB_DESC_S *CbDescEx;
    struct {
        pthread_t Context;
        HI_U32     Ref;
    } SharedChanCbCxt;
    pthread_mutex_t CbLock;
}g_stChanBuf = {0};

static struct {
    HI_U32 DmxCnt;
    HI_U32 RecCnt;
    DMX_BUF_S *BufDesc;
}g_stRecBuf = {0};

#if DMX_COPY_PES 
static struct {
    HI_U32 DmxCnt;
    HI_U32 ChnCnt;
    HI_U32 *BufDesc;
}g_stPesBuf = {0};
#endif

static pthread_mutex_t g_stDmxMutex = PTHREAD_MUTEX_INITIALIZER;

static pthread_mutex_t g_CbHookLock = PTHREAD_MUTEX_INITIALIZER;

#define MPIDmxCheckDeviceFd()           \
    do                                  \
    {                                   \
        if (-1 == g_s32DmxFd)           \
        {                               \
            HI_ERR_DEMUX("Dmx not init!\n"); \
            return HI_ERR_DMX_NOT_INIT; \
        }                               \
    } while (0)

#define MPIDmxCheckPointer(p)           \
    do                                  \
    {                                   \
        if (HI_NULL == p)               \
        {                               \
            HI_ERR_DEMUX("Null Pointer!\n"); \
            return HI_ERR_DMX_NULL_PTR; \
        }                               \
    } while (0)

#define DMX_BUFFERHANDLE2PORTID(BufferHandle)   (BufferHandle & 0xff)
#define DMX_PORTID2BUFFERHANDLE(PortId)         (PortId | 0x00000400 | (HI_ID_DEMUX << 16))
#define DMX_CHECK_BUFFERHANDLE(BufferHandle)    \
do{\
    if(((BufferHandle & 0xffffff00) != DMX_PORTID2BUFFERHANDLE(0)) \
        || (DMX_BUFFERHANDLE2PORTID(BufferHandle) >= g_stTsBuf.RamPortCnt))\
    {\
        HI_ERR_DEMUX("Invalid buffer handle:0x%x\n", BufferHandle); \
        return HI_ERR_DMX_INVALID_PARA;\
    }\
}while(0)


/************************************************************************************
 ************************************************************************************

                         Initialize  Module

 ************************************************************************************
 *************************************************************************************/
#if  DMX_COPY_PES

/**
 \brief check whether pes pUserMsg is valid or not.
return -1 if invalid.
return HI_UNF_DMX_TYPE_WHOLE if included all the pes
return HI_UNF_DMX_DATA_TYPE_HEAD if just only included the head of pes.
return HI_UNF_DMX_DATA_TYPE_TAIL if just only included the tail of pes
return HI_UNF_DMX_DATA_TYPE_BODY if not included head and tail

 \attention
 \none
 \param[in] pUserMsg
 \param[in] u32PmsgLen

 \retval none
 \return none

 \see
 \li ::
 */
static HI_S32 MPICheckPesUserMsg(const HI_UNF_DMX_DATA_S* pstBuf, HI_U32 u32BufSize)
{
    HI_U32 u32HeadType, u32TailType;

    if (pstBuf[0].enDataType == HI_UNF_DMX_DATA_TYPE_WHOLE)
    {
        return HI_UNF_DMX_DATA_TYPE_WHOLE;
    }

    u32HeadType = pstBuf[0].enDataType;
    u32TailType = pstBuf[u32BufSize - 1].enDataType;
    if (u32HeadType == HI_UNF_DMX_DATA_TYPE_HEAD)
    {
        if (u32TailType == HI_UNF_DMX_DATA_TYPE_TAIL)
        {
            return HI_UNF_DMX_DATA_TYPE_WHOLE;
        }
        else
        {
            return HI_UNF_DMX_DATA_TYPE_HEAD;
        }
    }
    else
    {
        if (u32TailType == HI_UNF_DMX_DATA_TYPE_TAIL)
        {
            return HI_UNF_DMX_DATA_TYPE_TAIL;
        }
        else
        {
            return HI_UNF_DMX_DATA_TYPE_BODY;
        }
    }
}

/**
 \brief return  the total length of pes
 \attention
\none
 \param[in] pu8Dst
 \param[in] pUserMsg
 \param[in] u32PmsgLen

 \retval none
 \return none

 \see
 \li ::
 */
static HI_U32 MPICopyPesTogether(HI_U8* pu8Dst, HI_UNF_DMX_DATA_S* pstBuf, HI_U32 u32BufSize)
{
    HI_U32 u32CopyedLen = 0;
    HI_U32 i;

    for (i = 0; i < u32BufSize; i++)
    {
        memcpy(pu8Dst + u32CopyedLen, pstBuf[i].pu8Data, pstBuf[i].u32Size);
        u32CopyedLen += pstBuf[i].u32Size;
    }

    return u32CopyedLen;
}

/**
 \brief malloc for pes, return HI_SUCCESS if successful. return HI_FAILURE if failure
 \attention
\none
 \param[in] hChannel
 \param[in] u32Size
 \param[out] pu8PesAddr

 \retval none
 \return none

 \see
 \li ::
 */
static HI_S32 MPIPesMemMalloc(HI_HANDLE hChannel, HI_U32 u32Size, HI_U8** pu8PesAddr)
{
    HI_UNF_DMX_CHAN_ATTR_S stChAttr;
    HI_U32 u32DmxId, u32ChnID;
    stChAttr.u32BufSize= 0;
    
    u32DmxId  = DMX_DMXID(hChannel);
    u32ChnID = DMX_CHANID(hChannel);

    assert(u32DmxId < g_stPesBuf.DmxCnt);
    assert(u32ChnID < g_stPesBuf.ChnCnt);
    
    HI_MPI_DMX_GetChannelAttr(hChannel, &stChAttr);
    *pu8PesAddr = (HI_U8*)HI_MALLOC(HI_ID_DEMUX, u32Size);
    if (!(*pu8PesAddr))
    {
        HI_ERR_DEMUX("Pes mem malloc failed!\n");
        *pu8PesAddr = HI_NULL;
        return HI_FAILURE;
    }

    g_stPesBuf.BufDesc[u32DmxId * g_stPesBuf.ChnCnt + u32ChnID] += u32Size;
    return HI_SUCCESS;
}

/**
 \brief free pes buffer
 \attention
none
 \param[in] hChannel
 \param[in] u32Size
 \param[in] pu8PesAddr

 \retval none
 \return none

 \see
 \li ::
 */
static HI_S32 MPIPesMemFree(HI_HANDLE hChannel, HI_U32 u32Size, HI_U8* pu8PesAddr)
{
    HI_U32 u32DmxId, u32ChnID;
    
    MPIDmxCheckPointer(pu8PesAddr);

    u32DmxId  = DMX_DMXID(hChannel);
    u32ChnID = DMX_CHANID(hChannel);

    assert(u32DmxId < g_stPesBuf.DmxCnt);
    assert(u32ChnID < g_stPesBuf.ChnCnt);

    HI_FREE(HI_ID_DEMUX, pu8PesAddr);

    g_stPesBuf.BufDesc[u32DmxId * g_stPesBuf.ChnCnt + u32ChnID] -= u32Size;

    return HI_SUCCESS;
}

static HI_BOOL MPIPesAccEnable(HI_HANDLE hChannel)
{
    HI_S32 ret = HI_FAILURE;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr = {0};
    
    ret = HI_MPI_DMX_GetChannelAttr(hChannel, &stChAttr);
    if (HI_SUCCESS != ret)
    {
        return HI_FALSE;
    }

    if (HI_UNF_DMX_CHAN_TYPE_PES == stChAttr.enChannelType && HI_UNF_DMX_SECURE_MODE_NONE == stChAttr.enSecureMode)
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

#endif /* #if DMX_COPY_PES */

#ifndef HI_SMMU_SUPPORT
/* define weak symbol for avoid compile depend HI_SMMU_SUPPORT */
static inline HI_VOID* HI_MPI_SMMU_Map(HI_U32 SmmuAddr, HI_BOOL bCached)
{
    HI_ERR_DEMUX("HI_SMMU_SUPPORT not configured.\n");

    return HI_NULL;
}

static inline HI_S32 HI_MPI_SMMU_Unmap(HI_U32 SmmuAddr)
{
    HI_ERR_DEMUX("HI_SMMU_SUPPORT not configured.\n");

    return HI_FAILURE;
}
#endif

static HI_VOID* DmxMmap(const HI_U32 PhyAddr, const DMX_BUF_FLAG_E Flag)
{  
    switch(Flag)
    {
        case DMX_MMZ_BUF:
            return HI_MPI_MMZ_Map(PhyAddr, HI_FALSE);
        case DMX_MMU_BUF:
            return HI_MPI_SMMU_Map(PhyAddr, HI_FALSE);
        case DMX_SECURE_BUF:
            return (HI_VOID *)(unsigned long)PhyAddr;
            
    }

    return HI_NULL;
}

static HI_S32 DmxMunmap(const HI_U32 PhyAddr, const DMX_BUF_FLAG_E Flag)
{
    switch(Flag)
    {
        case DMX_MMZ_BUF:
            return HI_MPI_MMZ_Unmap(PhyAddr);
        case DMX_MMU_BUF:
            return HI_MPI_SMMU_Unmap(PhyAddr);
        case DMX_SECURE_BUF:
            return HI_SUCCESS;
    }

    return HI_FAILURE;
}

static HI_S32 DmxGetPortTypeAndID(const HI_UNF_DMX_PORT_E Port, DMX_PORT_MODE_E *PortMode, HI_U32 *PortId)
{
    HI_S32 ret = HI_ERR_DMX_INVALID_PARA;
    HI_UNF_DMX_CAPABILITY_S CapInfo;

    ret = HI_MPI_DMX_GetCapability(&CapInfo);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("Get Demux Capability Info failed.\n");
        return ret;
    }

    switch((HI_MPI_DMX_PORT_E)Port)
    {
        case HI_MPI_DMX_PORT_RMX_0:
        {
            HI_U32 Id = (HI_U32)Port - (HI_U32)HI_MPI_DMX_PORT_RMX_0;
            
            *PortMode   = DMX_PORT_MODE_RMX;
            *PortId     = Id;

            return HI_SUCCESS;
        }

        default:
            break;
    }

    switch (Port)
    {
        case HI_UNF_DMX_PORT_IF_0 :
        case HI_UNF_DMX_PORT_IF_1 :
        case HI_UNF_DMX_PORT_IF_2 :
        case HI_UNF_DMX_PORT_IF_3 :
        case HI_UNF_DMX_PORT_IF_4 :
        case HI_UNF_DMX_PORT_IF_5 :
        case HI_UNF_DMX_PORT_IF_6 :
        case HI_UNF_DMX_PORT_IF_7 :
        case HI_UNF_DMX_PORT_IF_8 :
        case HI_UNF_DMX_PORT_IF_9 :
        case HI_UNF_DMX_PORT_IF_10 :
        case HI_UNF_DMX_PORT_IF_11 :
        case HI_UNF_DMX_PORT_IF_12 :
        case HI_UNF_DMX_PORT_IF_13 :
        case HI_UNF_DMX_PORT_IF_14 :
        {
            HI_U32 Id = (HI_U32)Port - (HI_U32)HI_UNF_DMX_PORT_IF_0;
            
            if (Id < CapInfo.u32IFPortNum)
            {
                *PortMode   = DMX_PORT_MODE_TUNER;
                *PortId     = Id;

                return HI_SUCCESS;
            }
            
            break;
        }
		
        case HI_UNF_DMX_PORT_TSI_0 :
        case HI_UNF_DMX_PORT_TSI_1 :
        case HI_UNF_DMX_PORT_TSI_2 :
        case HI_UNF_DMX_PORT_TSI_3 :
        case HI_UNF_DMX_PORT_TSI_4 :
        case HI_UNF_DMX_PORT_TSI_5 :
        case HI_UNF_DMX_PORT_TSI_6 :
        case HI_UNF_DMX_PORT_TSI_7 :
        case HI_UNF_DMX_PORT_TSI_8 :
        case HI_UNF_DMX_PORT_TSI_9 :
        case HI_UNF_DMX_PORT_TSI_10 :
        case HI_UNF_DMX_PORT_TSI_11 :
        case HI_UNF_DMX_PORT_TSI_12 :
        case HI_UNF_DMX_PORT_TSI_13 :
        case HI_UNF_DMX_PORT_TSI_14 :
        {
            HI_U32 Id = (HI_U32)Port - (HI_U32)HI_UNF_DMX_PORT_TSI_0;

            if (Id < CapInfo.u32TSIPortNum)
            {
                *PortMode   = DMX_PORT_MODE_TUNER;
                *PortId     = Id + CapInfo.u32IFPortNum;           

                return HI_SUCCESS;
            }
            
            break;
        }

        case HI_UNF_DMX_PORT_RAM_0 :
        case HI_UNF_DMX_PORT_RAM_1 :
        case HI_UNF_DMX_PORT_RAM_2 :
        case HI_UNF_DMX_PORT_RAM_3 :
        case HI_UNF_DMX_PORT_RAM_4 :
        case HI_UNF_DMX_PORT_RAM_5 :
        case HI_UNF_DMX_PORT_RAM_6 :
        case HI_UNF_DMX_PORT_RAM_7:
        case HI_UNF_DMX_PORT_RAM_8:
        case HI_UNF_DMX_PORT_RAM_9:
        case HI_UNF_DMX_PORT_RAM_10:
        case HI_UNF_DMX_PORT_RAM_11:
        case HI_UNF_DMX_PORT_RAM_12:
        case HI_UNF_DMX_PORT_RAM_13:
        {
            HI_U32 Id = (HI_U32)Port - (HI_U32)HI_UNF_DMX_PORT_RAM_0;

            if (Id < CapInfo.u32RamPortNum)
            {
                *PortMode   = DMX_PORT_MODE_RAM;
                *PortId     = Id;

                return HI_SUCCESS;
            }
            
            break;
        }

        default :
            break;
    }

    HI_ERR_DEMUX("Invalid Unknown port 0x%x\n", Port);
    
    return HI_ERR_DMX_INVALID_PARA;
}

static HI_S32 DmxReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum, HI_UNF_DMX_DATA_S *pstBuf)
{
    DMX_RelMsg_S Param;
    HI_U8   *UsrAddr;
    HI_U32 u32DmxId, u32ChId;
    HI_U32 i;

    u32DmxId = DMX_DMXID(hChannel);
    u32ChId = DMX_CHANID(hChannel);

    assert(u32DmxId < g_stChanBuf.DmxCnt);
    assert(u32ChId < g_stChanBuf.ChnCnt);
    
    for (i = 0; i < u32ReleaseNum; i++)
    {
        UsrAddr = pstBuf[i].pu8Data;
        if ((UsrAddr >= g_stPoolBuf.VirAddr)
            && ((UsrAddr - g_stPoolBuf.VirAddr) < g_stPoolBuf.Size))
        {
            pstBuf[i].pu8Data = (HI_U8*)((UsrAddr - g_stPoolBuf.VirAddr)
                                         + g_stPoolBuf.PhyAddr);
        }
        else if ((UsrAddr >= g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr)
                 && ((UsrAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr) < g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].Size))
        {
            pstBuf[i].pu8Data = (HI_U8*)((UsrAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr)
                                         + g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr);
        }
        else
        {
            HI_ERR_DEMUX("Invalid addr of channel data:ChanId=%d\n", u32ChId);
            return HI_ERR_DMX_INVALID_PARA;
        }
    }

    Param.hChannel = hChannel;
    Param.u32ReleaseNum = u32ReleaseNum;
    Param.pstBuf = pstBuf;

    /*given the address and length of buffer for this interface, given data in kernel mode */
    return ioctl(g_s32DmxFd, CMD_DEMUX_RELEASE_MSG, &Param);
}

static HI_S32 DmxDestroyTSBuffer(HI_U32 PortId)
{
    HI_S32 ret = HI_FAILURE;

    if (0 == g_stTsBuf.BufDesc[PortId].PhyAddr || !(PortId < g_stTsBuf.RamPortCnt))
    {
        HI_ERR_DEMUX("invalid buffer addr!\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    ret = DmxMunmap(g_stTsBuf.BufDesc[PortId].PhyAddr, g_stTsBuf.BufDesc[PortId].Flag);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("TS buffer unmap failed\n");

        return HI_ERR_DMX_MUNMAP_FAILED;
    }

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_DEINIT, &PortId);
    if (HI_SUCCESS == ret)
    {
        memset(&g_stTsBuf.BufDesc[PortId], 0, sizeof(DMX_BUF_S));
    }

    return ret;
}

#define DEFAULT_CHAN_BUF_CB_RCV_MSG_NUM (16)     /* keep sync with DMX_DEFAULT_BUF_NUM from drv_demux_define.h. */
static HI_VOID* DmxSharedChanCbFunc(void *args)
{
    HI_S32 ret = HI_FAILURE;
    DMX_CB_DESC_S *CbDescEx = HI_NULL;
    HI_HANDLE *WatchHandleSet = HI_NULL, *ValidHandleSet = HI_NULL;
    HI_U32 WatchHandleCnt = 0, ValidHandleCnt = 0;
    HI_UNF_DMX_CAPABILITY_S Cap = {0};

    ret = HI_MPI_DMX_GetCapability(&Cap);
    if (HI_SUCCESS != ret)
    {
        HI_ERR_DEMUX("get demux capability failed.\n");
        goto out0;
    }

    WatchHandleSet = HI_MALLOC(HI_ID_DEMUX, sizeof(HI_HANDLE) * Cap.u32ChannelNum);
    if (!WatchHandleSet)
    {
        HI_ERR_DEMUX("malloc watch handle set failed.\n");
        goto out0;
    }

    ValidHandleSet = HI_MALLOC(HI_ID_DEMUX, sizeof(HI_HANDLE) * Cap.u32ChannelNum);
    if (!ValidHandleSet)
    {
        HI_ERR_DEMUX("malloc valid handle set failed.\n");
        goto out1;
    }
    
    for(;;)
    {
        HI_U32 index;
        HI_UNF_DMX_DATA_S AcqedMsgBuf[DEFAULT_CHAN_BUF_CB_RCV_MSG_NUM];
        HI_U32 AcqedMsgNum = 0;

        WatchHandleCnt = 0;
        
        if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
        {
            for (index = 0; index < g_stChanBuf.DmxCnt * g_stChanBuf.ChnCnt; index ++)
            {
                CbDescEx = &g_stChanBuf.CbDescEx[index];
                
                if (HI_SUCCESS == __DMX_CHECK_CHANHANDLE(CbDescEx->Handle) &&
                               HI_UNF_DMX_CB_CONTEXT_TYPE_SHARED == CbDescEx->Raw.enContextType)
                {
                    WatchHandleSet[WatchHandleCnt] = CbDescEx->Handle;

                    WatchHandleCnt ++;
                }
            }

            if(pthread_mutex_unlock(&g_stChanBuf.CbLock))
            {
                HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
            }
        }
        else
        {
            HI_ERR_DEMUX("pthread_mutex_lock failed, continue...\n");
            continue;
        }

        if (0 == WatchHandleCnt)
        {
            break;
        }

        /* [!!!WARNNING!!!]: 
          * HI_MPI_DMX_GetDataHandle and HI_MPI_DMX_SelectDataHandle will produce conflict when both works together at same time. 
          */
        ret = HI_MPI_DMX_SelectDataHandle(WatchHandleSet, WatchHandleCnt, ValidHandleSet, &ValidHandleCnt, 200);
        if (HI_SUCCESS != ret)
        {
            continue;
        }

        for (index = 0; index < ValidHandleCnt ; index ++)
        {
            HI_HANDLE Handle;
            HI_UNF_DMX_CB_CONTEXT_TYPE_E ContextType;
            HI_UNF_DMX_CHAN_BUF_CB_FUNC CbFunc;
            HI_VOID *UserData;

            HI_U32 DmxId = DMX_DMXID(ValidHandleSet[index]);
            HI_U32 ChnId = DMX_CHANID(ValidHandleSet[index]);

            if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
            {
                CbDescEx = &g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId];

                Handle = CbDescEx->Handle;
                ContextType = CbDescEx->Raw.enContextType;
                CbFunc = CbDescEx->Raw.pfnChanBufCb;
                UserData = CbDescEx->Raw.pUserData;

                if (pthread_mutex_unlock(&g_stChanBuf.CbLock))
                {
                    HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
                }
            }
            else
            {
                HI_ERR_DEMUX("pthread_mutex_lock failed, continue...\n");
                continue;
            }

            if (HI_SUCCESS == __DMX_CHECK_CHANHANDLE(Handle) &&
                    HI_UNF_DMX_CB_CONTEXT_TYPE_SHARED == ContextType &&
                    CbFunc && ValidHandleSet[index] == Handle)
            {
                ret = HI_MPI_DMX_AcquireBuf(Handle, DEFAULT_CHAN_BUF_CB_RCV_MSG_NUM, &AcqedMsgNum, AcqedMsgBuf, 0);
                if (HI_SUCCESS != ret)
                {
                    HI_ERR_DEMUX("acquire cb data failed(0x%x).\n", ret);
                    continue;
                }

                /* undefined behavior if cb unseted. */
                CbFunc(Handle, AcqedMsgNum, AcqedMsgBuf, UserData);

                ret = HI_MPI_DMX_ReleaseBuf(Handle, AcqedMsgNum, AcqedMsgBuf);
                if (HI_SUCCESS != ret)
                {
                    HI_ERR_DEMUX("release cb data failed(0x%x).\n", ret);
                }
            }
        }
    }

    HI_FREE(HI_ID_DEMUX, ValidHandleSet);
out1:
    HI_FREE(HI_ID_DEMUX, WatchHandleSet);
out0:
    pthread_exit((HI_VOID*)HI_SUCCESS);
    
    return HI_NULL;
}

static HI_VOID* DmxPrivateChanCbFunc(void *args)
{
    HI_S32 ret = HI_FAILURE;
    DMX_CB_DESC_S *CbDescEx = (DMX_CB_DESC_S *)args; 
    HI_UNF_DMX_DATA_S AcqedMsgBuf[DEFAULT_CHAN_BUF_CB_RCV_MSG_NUM];
    HI_U32 AcqedMsgNum = 0;

    for(;;)
    {
        HI_HANDLE Handle;
        HI_UNF_DMX_CB_CONTEXT_TYPE_E ContextType;
        HI_UNF_DMX_CHAN_BUF_CB_FUNC CbFunc;
        HI_VOID *UserData;
    
        if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
        {
            Handle = CbDescEx->Handle;
            ContextType = CbDescEx->Raw.enContextType;
            CbFunc = CbDescEx->Raw.pfnChanBufCb;
            UserData = CbDescEx->Raw.pUserData;

            if (pthread_mutex_unlock(&g_stChanBuf.CbLock))
            {
                HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
            }
        }
        else
        {
            HI_ERR_DEMUX("pthread_mutex_lock failed, continue...\n");
            continue;
        }

        if (HI_SUCCESS == __DMX_CHECK_CHANHANDLE(Handle) &&
                    HI_UNF_DMX_CB_CONTEXT_TYPE_PRIVATE == ContextType && CbFunc)
        {
            ret = HI_MPI_DMX_AcquireBuf(Handle, DEFAULT_CHAN_BUF_CB_RCV_MSG_NUM, &AcqedMsgNum, AcqedMsgBuf, 200);
            if (HI_SUCCESS != ret)
            {
                continue;
            }

            /* undefined behavior if cb unseted. */
            CbFunc(Handle, AcqedMsgNum, AcqedMsgBuf, UserData);

            ret = HI_MPI_DMX_ReleaseBuf(Handle, AcqedMsgNum, AcqedMsgBuf);
            if (HI_SUCCESS != ret)
            {
                HI_ERR_DEMUX("release cb data failed(0x%x).\n", ret);
            }
        }
        else
        {
            break;
        }
    }

    pthread_exit((HI_VOID*)HI_SUCCESS);
    
    return HI_NULL;
}

static HI_S32 DmxGetChanBufCb(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *CbDesc)
{
    HI_S32 ret = HI_FAILURE;
    HI_U32 DmxId = DMX_DMXID(hChannel);
    HI_U32 ChnId = DMX_CHANID(hChannel);

    if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
    {
        CbDesc->enContextType = g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId].Raw.enContextType;
        CbDesc->pfnChanBufCb = g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId].Raw.pfnChanBufCb;
        CbDesc->pUserData = g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId].Raw.pUserData;

        if (pthread_mutex_unlock(&g_stChanBuf.CbLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }

        ret = HI_SUCCESS;
    }
    else
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        ret = HI_FAILURE;
    }

    return ret;
}

static HI_S32 __DmxSetChanBufCb(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *CbDesc)
{
    HI_S32 ret = HI_FAILURE;
    DMX_CB_DESC_S *CbDescEx = HI_NULL;
    HI_U32 DmxId = DMX_DMXID(hChannel);
    HI_U32 ChnId = DMX_CHANID(hChannel);
    
    CbDescEx = &g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId];

    if (HI_SUCCESS == __DMX_CHECK_CHANHANDLE(CbDescEx->Handle) ||
                           CbDescEx->Raw.enContextType < HI_UNF_DMX_CB_CONTEXT_TYPE_BUTT ||
                           CbDescEx->Raw.pfnChanBufCb)
    {
        HI_ERR_DEMUX("failed to repeat set cb for channel.\n");
        ret = HI_ERR_DMX_BUSY;
        goto out0;
    }

    CbDescEx->Handle = hChannel;
    CbDescEx->Raw.enContextType = CbDesc->enContextType;
    CbDescEx->Raw.pfnChanBufCb = CbDesc->pfnChanBufCb;
    CbDescEx->Raw.pUserData = CbDesc->pUserData;

    /* prepare context */
    if (HI_UNF_DMX_CB_CONTEXT_TYPE_SHARED == CbDescEx->Raw.enContextType)
    {
        if (-1 == g_stChanBuf.SharedChanCbCxt.Context)
        {
            assert(0 == g_stChanBuf.SharedChanCbCxt.Ref);
            
            if (0 != pthread_create(&g_stChanBuf.SharedChanCbCxt.Context, HI_NULL, DmxSharedChanCbFunc, HI_NULL))
            {
                g_stChanBuf.SharedChanCbCxt.Context = -1;
                                
                HI_ERR_DEMUX("Create shared callback thread failed.\n\n");
                ret = HI_FAILURE;
                goto out1;
            }
        }

        g_stChanBuf.SharedChanCbCxt.Ref ++;
    }
    else if (HI_UNF_DMX_CB_CONTEXT_TYPE_PRIVATE == CbDescEx->Raw.enContextType)
    {
        assert(-1 == CbDescEx->Context);
        
        if (0 != pthread_create(&CbDescEx->Context, HI_NULL, DmxPrivateChanCbFunc, (HI_VOID*)CbDescEx))
        {
            HI_ERR_DEMUX("Create private callback thread failed.\n\n");
            ret = HI_FAILURE;
            goto out1;
        }
    }

    return HI_SUCCESS;

out1:
    CbDescEx->Handle = HI_INVALID_HANDLE;
    CbDescEx->Context = -1;
    CbDescEx->Raw.enContextType = HI_UNF_DMX_CB_CONTEXT_TYPE_BUTT;
    CbDescEx->Raw.pfnChanBufCb = HI_NULL;
    CbDescEx->Raw.pUserData = HI_NULL;
out0:
    return ret;
}

static HI_S32 DmxSetChanBufCb(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *CbDesc)
{
    HI_S32 ret = HI_FAILURE;
    HI_UNF_DMX_CHAN_ATTR_S stChAttr = {0}; 
    
    ret = HI_MPI_DMX_GetChannelAttr(hChannel, &stChAttr);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    if (!(HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY & stChAttr.enOutputMode) || 
                    (HI_UNF_DMX_CHAN_TYPE_AUD == stChAttr.enChannelType || HI_UNF_DMX_CHAN_TYPE_VID == stChAttr.enChannelType))
    {
        HI_ERR_DEMUX("failed to set cb for vid/aud play channel(%d, %d).\n",  stChAttr.enChannelType, stChAttr.enOutputMode);
        ret = HI_ERR_DMX_INVALID_PARA;
        goto out;
    }

    if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
    {
        ret = __DmxSetChanBufCb(hChannel, CbDesc);
        
        if (pthread_mutex_unlock(&g_stChanBuf.CbLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
    }
    else
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        ret = HI_FAILURE;
    }

out:
    return ret;
}

static HI_S32 __DmxUnsetChanBufCb(HI_HANDLE hChannel, pthread_t *SavedSharedContext, pthread_t *SavedPrivContext)
{
    DMX_CB_DESC_S *CbDescEx = HI_NULL;
    
    HI_U32 DmxId = DMX_DMXID(hChannel);
    HI_U32 ChnId = DMX_CHANID(hChannel);
    
    CbDescEx = &g_stChanBuf.CbDescEx[DmxId * g_stChanBuf.ChnCnt + ChnId];

    if (HI_SUCCESS == __DMX_CHECK_CHANHANDLE(CbDescEx->Handle) &&
                    HI_UNF_DMX_CB_CONTEXT_TYPE_SHARED == CbDescEx->Raw.enContextType &&
                    CbDescEx->Raw.pfnChanBufCb)
    {
        g_stChanBuf.SharedChanCbCxt.Ref --;
        
        if (0 == g_stChanBuf.SharedChanCbCxt.Ref)
        {                          
            *SavedSharedContext = g_stChanBuf.SharedChanCbCxt.Context;

            g_stChanBuf.SharedChanCbCxt.Context = -1;
        }
    }
    
    *SavedPrivContext = CbDescEx->Context;

    CbDescEx->Handle = HI_INVALID_HANDLE;
    CbDescEx->Context = -1;
    CbDescEx->Raw.enContextType = HI_UNF_DMX_CB_CONTEXT_TYPE_BUTT;
    CbDescEx->Raw.pfnChanBufCb = HI_NULL;
    CbDescEx->Raw.pUserData = HI_NULL;

    return HI_SUCCESS;
}

static HI_S32 DmxUnsetChanBufCb(HI_HANDLE hChannel)
{    
    HI_S32 ret = HI_FAILURE;
    pthread_t SavedSharedContext = -1, SavedPrivContext = -1;
    
    if (0 == pthread_mutex_lock(&g_stChanBuf.CbLock))
    {
        ret = __DmxUnsetChanBufCb(hChannel, &SavedSharedContext, &SavedPrivContext);

        if (pthread_mutex_unlock(&g_stChanBuf.CbLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
    }
    else
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        ret = HI_FAILURE;
    }

    /* avoid deadlock */
    if (!(-1 == SavedSharedContext))
    {
        pthread_join(SavedSharedContext, HI_NULL);
    }
    
    if (!(-1 == SavedPrivContext))
    {
        pthread_join(SavedPrivContext, HI_NULL);
    }

    return ret;
}

static HI_S32 DmxDestroyChannel(HI_HANDLE hChannel)
{
    HI_S32 ret = HI_FAILURE;
    DMX_BUF_S *BufDesc = HI_NULL;

    HI_U32 DmxId = DMX_DMXID(hChannel);
    HI_U32 ChnId = DMX_CHANID(hChannel);
                
    assert(DmxId < g_stChanBuf.DmxCnt);
    assert(ChnId < g_stChanBuf.ChnCnt);

    BufDesc = &g_stChanBuf.BufDesc[DmxId * g_stChanBuf.ChnCnt + ChnId];

    /*the record-only channel doesn't have channel buffer */
    if (0 != BufDesc->PhyAddr)
    {
        if (HI_SUCCESS != DmxMunmap(BufDesc->PhyAddr, BufDesc->Flag))
        {
            HI_ERR_DEMUX("channel %u buffer unmap failed\n", ChnId);
            ret = HI_ERR_DMX_MUNMAP_FAILED;
            goto out;
        }

        BufDesc->VirAddr = HI_NULL;
        BufDesc->PhyAddr = 0;
        BufDesc->Size = 0;
    }

    if (0 == pthread_mutex_lock(&g_CbHookLock))
    {
        DmxUnsetChanBufCb(hChannel);

        ret = ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_DEL, &hChannel);

        if (pthread_mutex_unlock(&g_CbHookLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
    }
    else
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        ret = HI_FAILURE;
    }

out:
    return ret;
}

/***********************************************************************************
* Function:      HI_MPI_DMX_Init
* Description:   Open DEMUX device.
* Data Accessed: (Optional)
* Data Updated:  (Optional)
* Input:
* Output:
* Return:        HI_SUCCESS:                         Success
*                HI_FAILURE                             failure
* Others:
***********************************************************************************/
HI_S32 HI_MPI_DMX_Init(HI_VOID)
{
    int fd;
    HI_S32 ret;
    HI_UNF_DMX_CAPABILITY_S Cap = {0};
    DMX_PoolBuf_Attr_S PoolBufParam = {0};
    HI_U32 index;

    if ( 0 != pthread_mutex_lock(&g_stDmxMutex))
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        return HI_FAILURE;
    }

    if (g_s32DmxFd == -1)
    {
        fd = open (DmxDevName, O_RDWR, 0);
        if (fd < 0)
        {
            HI_ERR_DEMUX("Cannot open '%s'\n", DmxDevName);       
            ret = HI_FAILURE;
            goto out0;
        }

        g_s32DmxFd = fd;

        ret = HI_MPI_DMX_GetCapability(&Cap);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_DEMUX("Get demux capability failed.\n");
            goto out1;
        }

        /* save ts buff desc info */
        g_stTsBuf.RamPortCnt = Cap.u32RamPortNum;
        g_stTsBuf.BufDesc = HI_MALLOC(HI_ID_DEMUX, sizeof(DMX_BUF_S) * g_stTsBuf.RamPortCnt);
        if (!g_stTsBuf.BufDesc)
        {
            HI_ERR_DEMUX("malloc ram buf desc failed.\n");
            ret = HI_FAILURE;
            goto out1;
        }
        memset(g_stTsBuf.BufDesc, 0, sizeof(DMX_BUF_S) * g_stTsBuf.RamPortCnt);

        /* save channel buf desc and handle info */
        g_stChanBuf.DmxCnt = Cap.u32DmxNum;       
        g_stChanBuf.ChnCnt = Cap.u32ChannelNum;     
        g_stChanBuf.BufDesc = HI_MALLOC(HI_ID_DEMUX, sizeof(DMX_BUF_S) * (g_stChanBuf.DmxCnt * g_stChanBuf.ChnCnt));
        if (!g_stChanBuf.BufDesc)
        {
            HI_ERR_DEMUX("malloc channel buf desc failed.\n");
            ret = HI_FAILURE;
            goto out2;
        }
        memset(g_stChanBuf.BufDesc, 0, sizeof(DMX_BUF_S) * (g_stChanBuf.DmxCnt * g_stChanBuf.ChnCnt));

        /* save call back desc. */
        g_stChanBuf.CbDescEx = HI_MALLOC(HI_ID_DEMUX, sizeof(DMX_CB_DESC_S) * (g_stChanBuf.DmxCnt * g_stChanBuf.ChnCnt));
        if (!g_stChanBuf.CbDescEx)
        {
            HI_ERR_DEMUX("malloc channel cb desc failed.\n");
            ret = HI_FAILURE;
            goto out3;
        }
        
        for (index = 0; index < g_stChanBuf.DmxCnt * g_stChanBuf.ChnCnt; index ++)
        {
            DMX_CB_DESC_S *CbDescEx = &g_stChanBuf.CbDescEx[index];

            CbDescEx->Handle = HI_INVALID_HANDLE;
            CbDescEx->Context = -1;
            CbDescEx->Raw.enContextType = HI_UNF_DMX_CB_CONTEXT_TYPE_BUTT;
            CbDescEx->Raw.pfnChanBufCb = HI_NULL;
            CbDescEx->Raw.pUserData = HI_NULL;
        }

        g_stChanBuf.SharedChanCbCxt.Context = -1;
        g_stChanBuf.SharedChanCbCxt.Ref  = 0;
        pthread_mutex_init(&g_stChanBuf.CbLock, HI_NULL);
        
        /* save rec buf desc info */
        g_stRecBuf.DmxCnt = Cap.u32DmxNum;
        g_stRecBuf.RecCnt = Cap.u32RecChnNum;
        g_stRecBuf.BufDesc = HI_MALLOC(HI_ID_DEMUX, sizeof(DMX_BUF_S) * (g_stRecBuf.DmxCnt * g_stRecBuf.RecCnt));
        if (!g_stRecBuf.BufDesc)
        {
            HI_ERR_DEMUX("malloc rec buf desc failed.\n");
            ret = HI_FAILURE;
            goto out4;
        }
        memset(g_stRecBuf.BufDesc, 0, sizeof(DMX_BUF_S) * (g_stRecBuf.DmxCnt * g_stRecBuf.RecCnt));

#if DMX_COPY_PES 
        /* save pes buf desc info */
        g_stPesBuf.DmxCnt = Cap.u32DmxNum;
        g_stPesBuf.ChnCnt = Cap.u32ChannelNum;
        g_stPesBuf.BufDesc = HI_MALLOC(HI_ID_DEMUX, sizeof(HI_U32) * (g_stPesBuf.DmxCnt * g_stPesBuf.ChnCnt));
        if (!g_stPesBuf.BufDesc)
        {
            HI_ERR_DEMUX("malloc pes buf desc failed.\n");
            ret = HI_FAILURE;
            goto out5;
        }
        memset(g_stPesBuf.BufDesc, 0, sizeof(HI_U32) * (g_stPesBuf.DmxCnt * g_stPesBuf.ChnCnt));
#endif

        ret = ioctl(g_s32DmxFd, CMD_DEMUX_GET_POOLBUF_ADDR, &PoolBufParam);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_DEMUX("Demux init error\n");
            ret = HI_FAILURE;
            goto out6;
        }

        g_stPoolBuf.PhyAddr = PoolBufParam.BufPhyAddr;
        g_stPoolBuf.Size = PoolBufParam.BufSize;     
        g_stPoolBuf.Flag = PoolBufParam.BufFlag;

        g_stPoolBuf.VirAddr = DmxMmap(g_stPoolBuf.PhyAddr, g_stPoolBuf.Flag);
        if (0 == g_stPoolBuf.VirAddr)
        {
            HI_ERR_DEMUX("Pool buffer mmap error\n");
            ret =  HI_ERR_DMX_MMAP_FAILED;
            goto out6;
        }

        ret = HI_SUCCESS;
    }
    else
    {
        ret = HI_SUCCESS; //again open it, so return success
    } 

    if (0 != pthread_mutex_unlock(&g_stDmxMutex))
    {
        HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
    }

    return ret;

out6:
#if DMX_COPY_PES     
    HI_FREE(HI_ID_DEMUX, g_stPesBuf.BufDesc);
    g_stPesBuf.BufDesc = HI_NULL;
#endif    
out5:
    HI_FREE(HI_ID_DEMUX, g_stRecBuf.BufDesc);
    g_stRecBuf.BufDesc = HI_NULL;
out4:
    HI_FREE(HI_ID_DEMUX, g_stChanBuf.CbDescEx);
    g_stChanBuf.CbDescEx = HI_NULL;
out3:
    HI_FREE(HI_ID_DEMUX, g_stChanBuf.BufDesc);
    g_stChanBuf.BufDesc = HI_NULL;
out2:
    HI_FREE(HI_ID_DEMUX, g_stTsBuf.BufDesc);
    g_stTsBuf.BufDesc = HI_NULL;
out1:
    close(g_s32DmxFd);
    g_s32DmxFd = -1;
out0:
    if (0 != pthread_mutex_unlock(&g_stDmxMutex))
    {
        HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
    }
    
    return ret;
}

/***********************************************************************************
* Function:      HI_MPI_DMX_DeInit
* Description:   Close DEMUX device.
* Data Accessed: (Optional)
* Data Updated:  (Optional)
* Input:
* Output:
* Return:        HI_SUCCESS:                         Success
*                HI_FAILURE                          failure
* Others:
***********************************************************************************/
HI_S32 HI_MPI_DMX_DeInit(HI_VOID)
{
    HI_S32 ret;
    HI_U32 i, j;
    HI_UNF_DMX_CAPABILITY_S Cap = {0};

    if (0 != pthread_mutex_lock(&g_stDmxMutex))
    {
        HI_ERR_DEMUX("pthread_mutex_lock failed.\n");
        return HI_FAILURE;
    }
        
    if (g_s32DmxFd != -1)
    {
        ret = HI_MPI_DMX_GetCapability(&Cap);
        if (HI_SUCCESS != ret)
        {
            if (0 != pthread_mutex_unlock(&g_stDmxMutex))
            {
                HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
            }
            
            HI_ERR_DEMUX("Get demux capability failed.\n");
            return ret;
        }

        ret = DmxMunmap(g_stPoolBuf.PhyAddr, g_stPoolBuf.Flag);
        if (HI_SUCCESS != ret)
        {
            if (0 != pthread_mutex_unlock(&g_stDmxMutex))
            {
                HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
            }

            HI_ERR_DEMUX("Pool buffer unmap failed\n");
            return HI_ERR_DMX_MUNMAP_FAILED;
        }
  
        for (i = 0; i < Cap.u32RamPortNum; i++)
        {
            if (g_stTsBuf.BufDesc[i].VirAddr)
            {
                ret = DmxDestroyTSBuffer(i);
                if (HI_SUCCESS != ret)
                {
                    if (0 != pthread_mutex_unlock(&g_stDmxMutex))
                    {
                        HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
                    }
                    
                    HI_ERR_DEMUX("TS buffer destroy failed:PortId=%d\n", i);
                    return HI_FAILURE;
                }
            }
        }

        HI_FREE(HI_ID_DEMUX, g_stTsBuf.BufDesc);
        g_stTsBuf.BufDesc = HI_NULL;

#if DMX_COPY_PES 
        HI_FREE(HI_ID_DEMUX, g_stPesBuf.BufDesc);
        g_stPesBuf.BufDesc = HI_NULL;
#endif

        for (i = 0; i < g_stRecBuf.DmxCnt; i++)
        {
            for(j = 0; j < g_stRecBuf.RecCnt; j++)
            {
                if (g_stRecBuf.BufDesc[i * g_stRecBuf.RecCnt + j].PhyAddr)
                {
                    HI_HANDLE Handle = DMX_RECHANDLE(i , j);
                    
                    ret = HI_MPI_DMX_StopRecChn(Handle);
                    if (HI_SUCCESS != ret)
                    {
                        if (0 != pthread_mutex_unlock(&g_stDmxMutex))
                        {
                            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
                        }

                        HI_ERR_DEMUX("stop rec failed 0x%x\n", ret);

                        return ret;
                    }

                    ret = HI_MPI_DMX_DestroyRecChn(Handle);
                    if (HI_SUCCESS != ret)
                    {
                        if (0 != pthread_mutex_unlock(&g_stDmxMutex))
                        {
                            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
                        }

                        HI_ERR_DEMUX("destroy rec failed 0x%x\n", ret);

                        return ret;
                    }
                }
            }
        }

        HI_FREE(HI_ID_DEMUX, g_stRecBuf.BufDesc);
        g_stRecBuf.BufDesc = HI_NULL;

        for (i = 0; i< g_stChanBuf.DmxCnt; i++)
        {
            for (j = 0; j < g_stChanBuf.ChnCnt; j++)
            {
                if (g_stChanBuf.BufDesc[i * g_stChanBuf.ChnCnt +j].PhyAddr)
                {
                    HI_HANDLE Handle = DMX_CHANHANDLE(i , j);

                    ret = DmxDestroyChannel(Handle);
                    if (HI_SUCCESS != ret)
                    {
                        if (0 != pthread_mutex_unlock(&g_stDmxMutex))
                        {
                            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
                        }

                        HI_ERR_DEMUX("Channel delete failed 0x%x\n", ret);

                        return ret;
                    }
                }
            }
        }

        HI_FREE(HI_ID_DEMUX, g_stChanBuf.BufDesc);
        g_stChanBuf.BufDesc = HI_NULL;
        
        HI_FREE(HI_ID_DEMUX, g_stChanBuf.CbDescEx);
        g_stChanBuf.CbDescEx = HI_NULL;

        ret = close(g_s32DmxFd);
        if (ret != 0)
        {
            if (0 != pthread_mutex_unlock(&g_stDmxMutex))
            {
                HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
            }
            
            HI_FATAL_DEMUX("Cannot close '%s'\n", DmxDevName);
            return HI_FAILURE;
        }

        g_s32DmxFd = -1;
        if (0 != pthread_mutex_unlock(&g_stDmxMutex))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
        return HI_SUCCESS;
    }
    else
    {
        if (0 != pthread_mutex_unlock(&g_stDmxMutex))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
        return HI_SUCCESS;
    }
}

HI_S32 HI_MPI_DMX_GetCapability(HI_UNF_DMX_CAPABILITY_S *pstCap)
{
    HI_S32                  ret;
    HI_UNF_DMX_CAPABILITY_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstCap);

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_GET_CAPABILITY, &Param);
    if (HI_SUCCESS == ret)
    {
        pstCap->u32IFPortNum 	= Param.u32IFPortNum;
        pstCap->u32TSIPortNum 	= Param.u32TSIPortNum;
        pstCap->u32TSOPortNum 	= Param.u32TSOPortNum;
        pstCap->u32RamPortNum   = Param.u32RamPortNum;
        pstCap->u32DmxNum       = Param.u32DmxNum;
        pstCap->u32ChannelNum   = Param.u32ChannelNum;
        pstCap->u32AVChannelNum = Param.u32AVChannelNum;
        pstCap->u32FilterNum    = Param.u32FilterNum;
        pstCap->u32KeyNum       = Param.u32KeyNum;
        pstCap->u32RecChnNum    = Param.u32RecChnNum;
        pstCap->u32TagPortNum = Param.u32TagPortNum;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    HI_S32              ret;
    DMX_Port_GetAttr_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PORT_GET_ATTR, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstAttr, &Param.PortAttr, sizeof(HI_UNF_DMX_PORT_ATTR_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_SetTSPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_PORT_ATTR_S *pstAttr)
{
    HI_S32              ret;
    DMX_Port_SetAttr_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    memcpy(&Param.PortAttr, pstAttr, sizeof(Param.PortAttr));

    return ioctl(g_s32DmxFd, CMD_DEMUX_PORT_SET_ATTR, &Param);
}

HI_S32 HI_MPI_DMX_GetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr)
{
    HI_S32              ret;
    DMX_TSO_Port_Attr_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    Param.PortId = (HI_U32)enPortId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_TSO_PORT_GET_ATTR, &Param);
    if(ret == HI_SUCCESS)
    {
    	memcpy(pstAttr,&Param.PortAttr, sizeof(Param.PortAttr));
    }   
    
    return ret;
}

HI_S32 HI_MPI_DMX_SetTSOPortAttr(HI_UNF_DMX_TSO_PORT_E enPortId, const HI_UNF_DMX_TSO_PORT_ATTR_S *pstAttr)
{
    DMX_TSO_Port_Attr_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);
    
    Param.PortId = (HI_U32)enPortId;
    memcpy(&Param.PortAttr, pstAttr, sizeof(Param.PortAttr));

    return ioctl(g_s32DmxFd, CMD_DEMUX_TSO_PORT_SET_ATTR, &Param);
}

#if 0
HI_S32 HI_MPI_DMX_GetTagPortAttr(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    HI_S32  ret;
    DMX_Tag_GetAttr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS == ret && DMX_PORT_MODE_TAG == Param.PortMode )
    {
        ret = ioctl(g_s32DmxFd, CMD_DEMUX_DMX_GET_TAG_ATTR, &Param);
        if (HI_SUCCESS == ret)
        {
            memcpy(pstAttr, &Param.TagAttr, sizeof(HI_UNF_DMX_TAG_ATTR_S));
        }
    }
    else
    {
        ret = HI_ERR_DMX_INVALID_PARA;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_SetTagPortAttr(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_TAG_ATTR_S *pstAttr)
{
    HI_S32 ret;
    DMX_Tag_GetAttr_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS == ret && DMX_PORT_MODE_TAG == Param.PortMode)
    {
        ret = ioctl(g_s32DmxFd, CMD_DEMUX_DMX_SET_TAG_ATTR, &Param);;
    }
    else
    {
        ret = HI_ERR_DMX_INVALID_PARA;
    }

    return ret;
}
#endif

HI_S32 HI_MPI_DMX_AttachTSPort(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E enPortId)
{
    HI_S32              ret;
    DMX_Port_Attach_S   Param;

    MPIDmxCheckDeviceFd();

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    Param.DmxId = u32DmxId;

    return ioctl(g_s32DmxFd, CMD_DEMUX_PORT_ATTACH, &Param);
}

HI_S32 HI_MPI_DMX_DetachTSPort(HI_U32 u32DmxId)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_PORT_DETACH, &u32DmxId);
}

HI_S32 HI_MPI_DMX_GetTSPortId(HI_U32 u32DmxId, HI_UNF_DMX_PORT_E *penPortId)
{
    HI_S32              ret;
    HI_UNF_DMX_CAPABILITY_S Cap;
    DMX_Port_GetId_S    Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(penPortId);

    ret = HI_MPI_DMX_GetCapability(&Cap);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }

    Param.DmxId = u32DmxId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PORT_GETID, &Param);
    if (HI_SUCCESS == ret)
    {
        *penPortId = (HI_UNF_DMX_PORT_E)Param.PortId;
        if ((DMX_PORT_MODE_TUNER  == Param.PortMode) && (*penPortId  >= Cap.u32IFPortNum))
        {
            /*pay attention: if DMX_IFPORT_CNT == 0,then  (Param.PortId == 0)   corresponding with (HI_UNF_DMX_PORT_TSI_0==0x20)*/
            *penPortId = (HI_UNF_DMX_PORT_E)(HI_UNF_DMX_PORT_TSI_0 + Param.PortId - Cap.u32IFPortNum);
        }
        
        if (DMX_PORT_MODE_RAM == Param.PortMode)
        {
            *penPortId = (HI_UNF_DMX_PORT_E)(HI_UNF_DMX_PORT_RAM_0 + Param.PortId);
        }

        if (DMX_PORT_MODE_RMX == Param.PortMode)
        {
            *penPortId = (HI_UNF_DMX_PORT_E)(HI_MPI_DMX_PORT_RMX_0 + Param.PortId);
        }
    }

out:
    return ret;
}

HI_S32 HI_MPI_DMX_GetTSPortPacketNum(HI_UNF_DMX_PORT_E enPortId, HI_UNF_DMX_PORT_PACKETNUM_S *sPortStat)
{
    HI_S32              ret;
    DMX_PortPacketNum_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(sPortStat);

    ret = DmxGetPortTypeAndID(enPortId, &Param.PortMode, &Param.PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PORT_GETPACKETNUM, &Param);
    if (HI_SUCCESS == ret)
    {
        sPortStat->u32TsPackCnt     = Param.TsPackCnt;
        sPortStat->u32ErrTsPackCnt  = Param.ErrTsPackCnt;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetTSBufferDefaultAttr(HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr)
{
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstTsBufAttr);
    
    pstTsBufAttr->enSecureMode     = HI_UNF_DMX_SECURE_MODE_NONE;
    pstTsBufAttr->u32BufSize       = DMX_DEFAULT_TSBUF_SIZE; /* 2MiB */

    return HI_SUCCESS;
}

/* 
    multi-times request in process, return success directly
    the first time use it in this process, need to map user mode address.
 */
HI_S32 HI_MPI_DMX_CreateTSBuffer(HI_UNF_DMX_PORT_E enPortId, const HI_UNF_DMX_TSBUF_ATTR_S *pstTsBufAttr, HI_HANDLE *phTsBuffer)
{
    HI_S32          ret;
    HI_U32          PortId;
    DMX_PORT_MODE_E PortMode;
    DMX_TsBufInit_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstTsBufAttr);
    MPIDmxCheckPointer(phTsBuffer);

    ret = DmxGetPortTypeAndID(enPortId, &PortMode, &PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    if (DMX_PORT_MODE_TUNER == PortMode || !(PortId < g_stTsBuf.RamPortCnt))
    {
        HI_ERR_DEMUX("Invalid port mode:%d!\n",PortMode);
        return HI_ERR_DMX_NOT_SUPPORT;
    }

    Param.PortId = PortId;
    memcpy(&Param.Attr, pstTsBufAttr, sizeof(HI_UNF_DMX_TSBUF_ATTR_S));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_INIT, &Param);
    if (HI_SUCCESS == ret)
    {
        HI_U8 *UsrAddr;

        UsrAddr = DmxMmap(Param.BufPhyAddr, Param.BufFlag);
        if (HI_NULL == UsrAddr)
        {
            HI_ERR_DEMUX("Ts buffer mmap error: PortId=%d\n", enPortId);

            if (-1 == ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_DEINIT, &PortId))
            {
                HI_ERR_DEMUX("Ts buffer deinit failed: PortId=%d.\n", enPortId);
            }

            return HI_ERR_DMX_MMAP_FAILED;
        }

        g_stTsBuf.BufDesc[PortId].VirAddr   = UsrAddr;
        g_stTsBuf.BufDesc[PortId].PhyAddr   = Param.BufPhyAddr;
        g_stTsBuf.BufDesc[PortId].Size      = Param.BufSize;
        g_stTsBuf.BufDesc[PortId].Flag      = Param.BufFlag;

        *phTsBuffer = DMX_PORTID2BUFFERHANDLE(PortId);
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyTSBuffer(HI_HANDLE hTsBuffer)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);

    return DmxDestroyTSBuffer(DMX_BUFFERHANDLE2PORTID(hTsBuffer));
}

HI_S32 HI_MPI_DMX_GetTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ReqLen,
                              HI_UNF_STREAM_BUF_S *pstData, HI_U32 *pu32PhyAddr, HI_U32 u32TimeOutMs)
{
    HI_S32          ret;
    DMX_TsBufGet_S  Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);
    MPIDmxCheckPointer(pstData);
    MPIDmxCheckPointer(pu32PhyAddr);

    Param.PortId    = DMX_BUFFERHANDLE2PORTID(hTsBuffer);
    Param.ReqLen    = u32ReqLen;
    Param.TimeoutMs = u32TimeOutMs;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        pstData->u32Size = Param.BufSize;
        pstData->pu8Data = (HI_U8*)(g_stTsBuf.BufDesc[Param.PortId].VirAddr
                                    + Param.BufPhyAddr
                                    - g_stTsBuf.BufDesc[Param.PortId].PhyAddr);

        *pu32PhyAddr = Param.BufPhyAddr;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_PushTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData)
{
    DMX_TsBufPush_S Param;
    HI_U32 PortId = DMX_BUFFERHANDLE2PORTID(hTsBuffer);

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);
    MPIDmxCheckPointer(pstData);

    Param.PortId = PortId;
    Param.BufSize = pstData->u32Size;
    Param.BufPhyAddr = (HI_U32)(pstData->pu8Data - g_stTsBuf.BufDesc[PortId].VirAddr +
        g_stTsBuf.BufDesc[PortId].PhyAddr);
    
    return ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_PUSH, &Param);
}

HI_S32 HI_MPI_DMX_ReleaseTSBuffer(HI_HANDLE hTsBuffer, const HI_UNF_STREAM_BUF_S *pstData)
{
    DMX_TsBufRel_S Param;
    HI_U32 PortId = DMX_BUFFERHANDLE2PORTID(hTsBuffer);

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);
    MPIDmxCheckPointer(pstData);

    Param.PortId = PortId;
    Param.BufSize = pstData->u32Size;
    Param.BufPhyAddr = (HI_U32)(pstData->pu8Data - g_stTsBuf.BufDesc[PortId].VirAddr +
        g_stTsBuf.BufDesc[PortId].PhyAddr);
    
    return ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_RELEASE, &Param);
}

/*u32ValidDataLen is the valid length from u32StartPos */
HI_S32 HI_MPI_DMX_PutTSBuffer(HI_HANDLE hTsBuffer, HI_U32 u32ValidDataLen, HI_U32 u32StartPos)
{
    DMX_TsBufPut_S  Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);

    Param.PortId        = DMX_BUFFERHANDLE2PORTID(hTsBuffer);
    Param.ValidDataLen  = u32ValidDataLen;
    Param.StartPos      = u32StartPos;

    return ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_PUT, &Param);
}

HI_S32 HI_MPI_DMX_ResetTSBuffer(HI_HANDLE hTsBuffer)
{
    HI_U32 PortId = DMX_BUFFERHANDLE2PORTID(hTsBuffer);

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);

    return ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_RESET, &PortId);
}

/* get the status without limiting for muliti-process */
HI_S32 HI_MPI_DMX_GetTSBufferStatus(HI_HANDLE hTsBuffer, HI_UNF_DMX_TSBUF_STATUS_S *pStatus)
{
    HI_S32              ret;
    DMX_TsBufStaGet_S   Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);
    MPIDmxCheckPointer(pStatus);

    Param.PortId = DMX_BUFFERHANDLE2PORTID(hTsBuffer);

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_TS_BUFFER_GET_STATUS, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pStatus, &Param.Status, sizeof(HI_UNF_DMX_TSBUF_STATUS_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetTSBufferPortId(HI_HANDLE hTsBuffer, HI_UNF_DMX_PORT_E *penPortId)
{
    HI_U32 PortId = DMX_BUFFERHANDLE2PORTID(hTsBuffer);

    MPIDmxCheckDeviceFd();
    DMX_CHECK_BUFFERHANDLE(hTsBuffer);
    MPIDmxCheckPointer(penPortId);

    if (0 == g_stTsBuf.BufDesc[PortId].PhyAddr)
    {
        HI_ERR_DEMUX("TS buffer handle invalid, buffer not created:PortId=%d\n", PortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    *penPortId = (HI_UNF_DMX_PORT_E)(HI_UNF_DMX_PORT_RAM_0 + PortId);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DMX_GetTSBufferHandle(HI_UNF_DMX_PORT_E enPortId, HI_HANDLE *phTsBuffer)
{
    HI_S32 ret;
    HI_U32 PortId;
    DMX_PORT_MODE_E PortMode;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phTsBuffer);

    ret = DmxGetPortTypeAndID(enPortId, &PortMode, &PortId);
    if (HI_SUCCESS != ret)
    {
        return ret;
    }

    if (DMX_PORT_MODE_TUNER == PortMode  || !(PortId < g_stTsBuf.RamPortCnt))
    {
        HI_ERR_DEMUX("The Port %u not support TS buffer\n", enPortId);
        return HI_ERR_DMX_INVALID_PARA;
    }

    if (0 == g_stTsBuf.BufDesc[PortId].PhyAddr)
    {
        HI_ERR_DEMUX("TS Buffer not created\n");
        return HI_ERR_DMX_INVALID_PARA;
    }

    *phTsBuffer = DMX_PORTID2BUFFERHANDLE(PortId);

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DMX_GetPortMode(HI_U32 u32DmxId, HI_UNF_DMX_PORT_MODE_E *penPortMod)
{
    HI_S32 ret;
    HI_UNF_DMX_PORT_E PortId;
    HI_UNF_DMX_PORT_ATTR_S  PortAttr;

    MPIDmxCheckDeviceFd();  
    MPIDmxCheckPointer(penPortMod);

    ret = HI_MPI_DMX_GetTSPortId(u32DmxId, &PortId);
    if (HI_SUCCESS == ret)
    {
        ret = HI_MPI_DMX_GetTSPortAttr(PortId, &PortAttr);
        if (HI_SUCCESS == ret)
        {
            *penPortMod = PortAttr.enPortMod;
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetChannelDefaultAttr(HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstChAttr);

    pstChAttr->u32BufSize       = DMX_DEFAULT_CHANBUF_SIZE;
    pstChAttr->enCRCMode        = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
    pstChAttr->enChannelType    = HI_UNF_DMX_CHAN_TYPE_SEC;
    pstChAttr->enOutputMode     = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
    pstChAttr->enSecureMode     = HI_UNF_DMX_SECURE_MODE_NONE;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_DMX_CreateChannel(HI_U32 u32DmxId, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr,
                                HI_HANDLE *phChannel)
{
    HI_S32 ret;
    DMX_ChanNew_S Param;
    HI_U32 ChanId;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstChAttr);
    MPIDmxCheckPointer(phChannel);

    Param.u32DemuxId = u32DmxId;
    memcpy(&Param.stChAttr, pstChAttr, sizeof(HI_UNF_DMX_CHAN_ATTR_S));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_NEW, &Param);
    if (HI_SUCCESS == ret)
    {
        *phChannel = Param.hChannel;
        ChanId = DMX_CHANID(*phChannel);

        assert(u32DmxId == DMX_DMXID(*phChannel));
        assert(ChanId < g_stChanBuf.ChnCnt);

        if (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == pstChAttr->enOutputMode)
        {
            HI_U8 *UsrAddr;

            UsrAddr = DmxMmap(Param.BufPhyAddr, Param.BufFlag);
            if (HI_NULL == UsrAddr)
            {
                HI_ERR_DEMUX("Channel buffer mmap error: ChanId=%d\n", ChanId);

                if (HI_SUCCESS != ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_DEL, phChannel))
                {
                    HI_ERR_DEMUX("delete channel failed:ChId=%d.\n", ChanId);
                }

                return HI_ERR_DMX_MMAP_FAILED;
            }

            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].VirAddr = UsrAddr;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].PhyAddr = Param.BufPhyAddr;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].Size    = Param.BufSize;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].Flag    = Param.BufFlag;
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_CreateChannelWithPID(HI_U32 u32DmxId, HI_U32 u32Pid, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr, 
                            HI_HANDLE *phChannel)
{
    HI_S32 ret;
    DMX_ChanNew_S Param;
    HI_U32 ChanId;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstChAttr);
    MPIDmxCheckPointer(phChannel);

    Param.u32DemuxId = u32DmxId;
    Param.u32Pid = u32Pid;
    memcpy(&Param.stChAttr, pstChAttr, sizeof(HI_UNF_DMX_CHAN_ATTR_S));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_NEW2, &Param);
    if (HI_SUCCESS == ret)
    {
        *phChannel = Param.hChannel;
        ChanId = DMX_CHANID(*phChannel);
        
        assert(u32DmxId == DMX_DMXID(*phChannel));
        assert(ChanId < g_stChanBuf.ChnCnt);

        if (   (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY == pstChAttr->enOutputMode)
            || (HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY_REC == pstChAttr->enOutputMode) )
        {
            HI_U8 *UsrAddr;

            UsrAddr = DmxMmap(Param.BufPhyAddr, Param.BufFlag);
            if (HI_NULL == UsrAddr)
            {
                HI_ERR_DEMUX("Channel buffer mmap error: ChanId=%d\n", ChanId);

                if (HI_SUCCESS != ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_DEL, phChannel))
                {
                    HI_ERR_DEMUX("delete channel failed:ChId=%d.\n", ChanId);
                }

                return HI_ERR_DMX_MMAP_FAILED;
            }

            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].VirAddr   = UsrAddr;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].PhyAddr   = Param.BufPhyAddr;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].Size      = Param.BufSize;
            g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + ChanId].Flag      = Param.BufFlag;
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyChannel(HI_HANDLE hChannel)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);

    return DmxDestroyChannel(hChannel);
}

HI_S32 HI_MPI_DMX_GetChannelAttr(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    HI_S32 ret;
    DMX_GetChan_Attr_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstChAttr);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_ATTR_GET, &Param);

    if (HI_SUCCESS == ret)
    {
        memcpy(pstChAttr, &Param.stChAttr, sizeof(HI_UNF_DMX_CHAN_ATTR_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_SetChannelAttr(HI_HANDLE hChannel, const HI_UNF_DMX_CHAN_ATTR_S *pstChAttr)
{
    DMX_SetChan_Attr_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstChAttr);

    Param.hChannel = hChannel;
    memcpy(&Param.stChAttr, pstChAttr, sizeof(HI_UNF_DMX_CHAN_ATTR_S));

    /*set disable CRC in non-section mode */
    if ((HI_UNF_DMX_CHAN_TYPE_PES == Param.stChAttr.enChannelType)
        || (HI_UNF_DMX_CHAN_TYPE_POST == Param.stChAttr.enChannelType)
        || (HI_UNF_DMX_CHAN_TYPE_VID == Param.stChAttr.enChannelType)
        || (HI_UNF_DMX_CHAN_TYPE_AUD == Param.stChAttr.enChannelType))
    {
        Param.stChAttr.enCRCMode = HI_UNF_DMX_CHAN_CRC_MODE_FORBID;
    }

    return ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_ATTR_SET, &Param);
}

HI_S32 HI_MPI_DMX_SetChannelPID(HI_HANDLE hChannel, HI_U32 u32Pid)
{
    DMX_ChanPIDSet_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);

    Param.hChannel = hChannel;
    Param.u32Pid = u32Pid;
    return ioctl(g_s32DmxFd, CMD_DEMUX_PID_SET, &Param);
}

HI_S32 HI_MPI_DMX_GetChannelPID(HI_HANDLE hChannel, HI_U32 *pu32Pid)
{
    HI_S32 ret;
    DMX_ChanPIDGet_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pu32Pid);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PID_GET, &Param);

    if (HI_SUCCESS == ret)
    {
        *pu32Pid = Param.u32Pid;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_OpenChannel(HI_HANDLE hChannel)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);

    return ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_OPEN, &hChannel);
}

HI_S32 HI_MPI_DMX_CloseChannel(HI_HANDLE hChannel)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    return ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_CLOSE, &hChannel);
}

HI_S32 HI_MPI_DMX_GetChannelStatus(HI_HANDLE hChannel, HI_UNF_DMX_CHAN_STATUS_S *pstStatus)
{
    HI_S32 ret;
    DMX_ChanStatusGet_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstStatus);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_GET_CHAN_STATUS, &Param);

    if (HI_SUCCESS == ret)
    {
        memcpy(pstStatus, &Param.stStatus, sizeof(HI_UNF_DMX_CHAN_STATUS_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetChannelHandle(HI_U32 u32DmxId, HI_U32 u32Pid, HI_HANDLE *phChannel)
{
    HI_S32 ret;
    DMX_ChannelIdGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phChannel);

    Param.u32DmxId  = u32DmxId;
    Param.u32Pid    = u32Pid;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_CHANID_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        *phChannel = Param.hChannel;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetFreeChannelCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    HI_S32 ret;
    DMX_FreeChanGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pu32FreeCount);

    Param.u32DmxId = u32DmxId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_FREECHAN_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        *pu32FreeCount = Param.u32FreeCount;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetScrambledFlag(HI_HANDLE hChannel, HI_UNF_DMX_SCRAMBLED_FLAG_E *penScrambleFlag)
{
    HI_S32 ret;
    DMX_ScrambledFlagGet_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(penScrambleFlag);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_SCRAMBLEFLAG_GET, &Param);

    if (HI_SUCCESS == ret)
    {
        *penScrambleFlag = Param.enScrambleFlag;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_SetChannelEosFlag(HI_HANDLE hChannel)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);

    return ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_SET_EOS_FLAG, &hChannel);
}

HI_S32 HI_MPI_DMX_GetChannelTsCount(HI_HANDLE hChannel, HI_U32 *pu32TsCount)
{
    HI_S32 ret;
    DMX_ChanChanTsCnt_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pu32TsCount);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_GET_CHAN_TSCNT, &Param);

    if (HI_SUCCESS == ret)
    {
        *pu32TsCount = Param.u32ChanTsCnt;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_CreateFilter(HI_U32 u32DmxId, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr, HI_HANDLE *phFilter)
{
    HI_S32          ret;
    DMX_NewFilter_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstFilterAttr);
    MPIDmxCheckPointer(phFilter);

    Param.DmxId = u32DmxId;
    memcpy(&Param.FilterAttr, pstFilterAttr, sizeof(HI_UNF_DMX_FILTER_ATTR_S));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_FLT_NEW, &Param);
    if (HI_SUCCESS == ret)
    {
        *phFilter = Param.Filter;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyFilter(HI_HANDLE hFilter)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_FLT_DEL, &hFilter);
}

HI_S32 HI_MPI_DMX_DeleteAllFilter(HI_HANDLE hChannel)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_FLT_DELALL, &hChannel);
}

HI_S32 HI_MPI_DMX_SetFilterAttr(HI_HANDLE hFilter, const HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr)
{
    DMX_FilterSet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstFilterAttr);

    Param.Filter = hFilter;
    memcpy(&Param.FilterAttr, pstFilterAttr, sizeof(HI_UNF_DMX_FILTER_ATTR_S));

    return ioctl(g_s32DmxFd, CMD_DEMUX_FLT_SET, &Param);
}

HI_S32 HI_MPI_DMX_GetFilterAttr(HI_HANDLE hFilter, HI_UNF_DMX_FILTER_ATTR_S *pstFilterAttr)
{
    HI_S32          ret;
    DMX_FilterGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstFilterAttr);

    Param.Filter = hFilter;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_FLT_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstFilterAttr, &Param.FilterAttr, sizeof(HI_UNF_DMX_FILTER_ATTR_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_AttachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    DMX_FilterAttach_S Param;

    MPIDmxCheckDeviceFd();

    Param.Filter    = hFilter;
    Param.Channel   = hChannel;

    return ioctl(g_s32DmxFd, CMD_DEMUX_FLT_ATTACH, &Param);
}

HI_S32 HI_MPI_DMX_DetachFilter(HI_HANDLE hFilter, HI_HANDLE hChannel)
{
    DMX_FilterDetach_S Param;

    MPIDmxCheckDeviceFd();

    Param.Filter    = hFilter;
    Param.Channel   = hChannel;

    return ioctl(g_s32DmxFd, CMD_DEMUX_FLT_DETACH, &Param);
}

HI_S32 HI_MPI_DMX_GetFilterChannelHandle(HI_HANDLE hFilter, HI_HANDLE *phChannel)
{
    HI_S32                      ret;
    DMX_FilterChannelIDGet_S    Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phChannel);

    Param.Filter = hFilter;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_FLT_CHANID_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        *phChannel = Param.Channel;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetFreeFilterCount(HI_U32 u32DmxId, HI_U32 *pu32FreeCount)
{
    HI_S32              ret;
    DMX_FreeFilterGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pu32FreeCount);

    Param.DmxId = u32DmxId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_FREEFLT_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        *pu32FreeCount = Param.FreeCount;
    }

    return ret;
}

/* return the data case of all the channel, excepted audio and video channel, the number of handle will not more than what user want */
HI_S32  HI_MPI_DMX_GetDataHandle(HI_HANDLE *phChannel, HI_U32 *pu32ChNum,
                                 HI_U32 u32TimeOutMs)
{
    DMX_GetDataFlag_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phChannel);
    MPIDmxCheckPointer(pu32ChNum);

    if (0 == *pu32ChNum)
    {
        HI_ERR_DEMUX("Invalid channel number:%d\n", *pu32ChNum);
        return HI_ERR_DMX_INVALID_PARA;
    }

    Param.WatchChannelNum = *pu32ChNum;
    Param.ValidChannel = phChannel;
    Param.ValidChannelNum = pu32ChNum;
    Param.u32TimeOutMs = u32TimeOutMs;

    return ioctl(g_s32DmxFd, CMD_DEMUX_GET_DATA_FLAG, &Param);
}

HI_S32  HI_MPI_DMX_SelectDataHandle(HI_HANDLE *phWatchChannel, HI_U32 u32WatchNum,
                                    HI_HANDLE *phDataChannel, HI_U32 *pu32ChNum, HI_U32 u32TimeOutMs)
{
    DMX_SelectDataFlag_S Param;
    HI_U32 i;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phDataChannel);
    MPIDmxCheckPointer(phWatchChannel);
    MPIDmxCheckPointer(pu32ChNum);

    if (0 == u32WatchNum)
    {
        HI_ERR_DEMUX("Invalid channel number:%d\n", u32WatchNum);
        return HI_ERR_DMX_INVALID_PARA;
    }
    
    for (i = 0; i < u32WatchNum; i++)
    {
        DMX_CHECK_CHANHANDLE(phWatchChannel[i]);
    }

    Param.WatchChannel = phWatchChannel;
    Param.WatchChannelNum = u32WatchNum;
    Param.ValidChannel = phDataChannel;
    Param.ValidChannelNum = pu32ChNum;
    Param.u32TimeOutMs = u32TimeOutMs;

    /* get whether the pointed channel data has flag or not */
    return ioctl(g_s32DmxFd, CMD_DEMUX_SELECT_DATA_FLAG, &Param);
}

HI_S32  HI_MPI_DMX_AcquireBuf(HI_HANDLE hChannel, HI_U32 u32AcquireNum,
                              HI_U32 *pu32AcquiredNum, HI_UNF_DMX_DATA_S *pstBuf,
                              HI_U32 u32TimeOutMs)
{
    HI_S32 ret;
    DMX_AcqMsg_S Param;
    HI_U32 i;
    HI_U32 u32PhyAddr;
    HI_U32 u32DmxId, u32ChId;
#if  DMX_COPY_PES
    HI_UNF_DMX_DATA_S stBufTmp[16];
#endif /* #if DMX_COPY_PES */

    HI_UNF_DMX_DATA_S *pstBufTmp;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pu32AcquiredNum);
    MPIDmxCheckPointer(pstBuf);
    pstBufTmp = pstBuf;
    
#if  DMX_COPY_PES
    if (HI_TRUE == MPIPesAccEnable(hChannel))
    {
        pstBufTmp = stBufTmp;
    }
#endif

    u32DmxId = DMX_DMXID(hChannel);
    u32ChId = DMX_CHANID(hChannel);

    assert(u32DmxId < g_stChanBuf.DmxCnt);
    assert(u32ChId < g_stChanBuf.ChnCnt);
    
    Param.hChannel = hChannel;
    Param.u32AcquireNum = u32AcquireNum;
    Param.pstBuf = pstBufTmp;
    Param.u32TimeOutMs = u32TimeOutMs;

    /* given the address and length of the buffer, the buffer address array will be copied out in kernel mode */
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_ACQUIRE_MSG, &Param);
    if ((HI_SUCCESS != ret) || (Param.u32AcquiredNum == 0))
    {
        *pu32AcquiredNum = 0;
        return ret;
    }

    HI_INFO_DEMUX("acquired %d: firstaddr:0x%x, poolAddr:0x%x, poolvir:0x%llx, poolzise:0x%x\n",
                  Param.u32AcquiredNum, pstBufTmp[0].pu8Data, g_stPoolBuf.PhyAddr,
                  g_stPoolBuf.VirAddr, g_stPoolBuf.Size);

    for (i = 0; i < Param.u32AcquiredNum; i++)
    {
        u32PhyAddr = (HI_U32)(unsigned long)pstBufTmp[i].pu8Data;
        if ((u32PhyAddr >= g_stPoolBuf.PhyAddr)
            && ((u32PhyAddr - g_stPoolBuf.PhyAddr) < g_stPoolBuf.Size))
        {
            pstBufTmp[i].pu8Data = (HI_U8*)((u32PhyAddr - g_stPoolBuf.PhyAddr)
                                         + g_stPoolBuf.VirAddr);
        }
        else if ((u32PhyAddr >= g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr)
                 && ((u32PhyAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr) < g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].Size))
        {
            pstBufTmp[i].pu8Data = (HI_U8*)((u32PhyAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr)
                                         + g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr);
        }
        else
        {
            HI_ERR_DEMUX("Invalid Phy addr of channel data:ChanId=%d\n", u32ChId);
            *pu32AcquiredNum = 0;
            return HI_FAILURE;
        }
    }

    *pu32AcquiredNum = Param.u32AcquiredNum;
    
    /*
    for PES channel every block of data is 8K, but customer want get large size of PES block at one time,
    so ,we alloc PES buffer at here and copy all acquired data block into it ,then throw it up .     
    */
#if  DMX_COPY_PES
    if (HI_TRUE == MPIPesAccEnable(hChannel))
    {
        HI_S32 s32PesType, s32Ret;
        HI_U32 u32PesLen = 0;
        HI_U32 u32MallocLen = 0;
        HI_U8* pu8PesMem = HI_NULL;

        s32PesType = MPICheckPesUserMsg(pstBufTmp, *pu32AcquiredNum);

        for (i = 0; i < *pu32AcquiredNum; i++)
        {
            u32MallocLen += pstBufTmp[i].u32Size;
        }
        s32Ret = MPIPesMemMalloc(hChannel, u32MallocLen, &pu8PesMem);
        if (HI_SUCCESS == s32Ret)
        {
            u32PesLen = MPICopyPesTogether(pu8PesMem, pstBufTmp, *pu32AcquiredNum);
        }

        /*free kernle buffer after copying */
        s32Ret |= DmxReleaseBuf(hChannel, *pu32AcquiredNum, pstBufTmp);
        if (HI_SUCCESS == s32Ret)
        {
            pstBuf[0].pu8Data = pu8PesMem;
            pstBuf[0].enDataType = (HI_UNF_DMX_DATA_TYPE_E)s32PesType;
            pstBuf[0].u32Size = u32PesLen;
            *pu32AcquiredNum = 1;
        }
        else
        {
            MPIPesMemFree(hChannel, u32MallocLen, pu8PesMem);
        }

        return s32Ret;
    }
#endif /* #if DMX_COPY_PES */


    return HI_SUCCESS;
}

HI_S32  HI_MPI_DMX_ReleaseBuf(HI_HANDLE hChannel, HI_U32 u32ReleaseNum,
                              HI_UNF_DMX_DATA_S *pstBuf)
{
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstBuf);

    if (0 == u32ReleaseNum)
    {
        return HI_SUCCESS;
    }

#if  DMX_COPY_PES
    if (HI_TRUE == MPIPesAccEnable(hChannel))
    {
        return MPIPesMemFree(hChannel, pstBuf[0].u32Size, pstBuf[0].pu8Data);
    }
#endif /* #if DMX_COPY_PES */

    return DmxReleaseBuf(hChannel, u32ReleaseNum, pstBuf);
}

HI_S32 HI_MPI_DMX_CreatePcrChannel(HI_U32 u32DmxId, HI_U32 *pu32PcrChId)
{
    HI_S32 ret;
    DMX_NewPcr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pu32PcrChId);

    Param.u32DmxId = u32DmxId;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PCR_NEW, &Param);
    if (HI_SUCCESS == ret)
    {
        *pu32PcrChId = Param.u32PcrId;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyPcrChannel(HI_U32 u32PcrChId)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_PCR_DEL, &u32PcrChId);
}

HI_S32 HI_MPI_DMX_PcrPidSet(HI_U32 pu32PcrChId, HI_U32 u32Pid)
{
    DMX_PcrPidSet_S Param;

    MPIDmxCheckDeviceFd();

    Param.pu32PcrChId = pu32PcrChId;
    Param.u32Pid = u32Pid;

    return ioctl(g_s32DmxFd, CMD_DEMUX_PCRPID_SET, &Param);
}

HI_S32 HI_MPI_DMX_PcrPidGet(HI_U32 pu32PcrChId, HI_U32 *pu32Pid)
{
    HI_S32 ret;
    DMX_PcrPidGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pu32Pid);

    Param.pu32PcrChId = pu32PcrChId;
    
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PCRPID_GET, &Param);
    
    if (HI_SUCCESS == ret)
    {
        *pu32Pid = Param.u32Pid;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_PcrScrGet(HI_U32 pu32PcrChId, HI_U64 *pu64PcrMs, HI_U64 *pu64ScrMs)
{
    HI_S32 ret;
    DMX_PcrScrGet_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pu64PcrMs);
    MPIDmxCheckPointer(pu64ScrMs);

    Param.pu32PcrChId = pu32PcrChId;
    
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_CURPCR_GET, &Param);

    if (HI_SUCCESS == ret)
    {
        *pu64PcrMs = Param.u64PcrValue;
        *pu64ScrMs = Param.u64ScrValue;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_PcrSyncAttach(HI_U32 u32PcrChId, HI_U32 u32SyncHandle)
{
    HI_S32 ret;
    DMX_PCRSYNC_S Param;

    MPIDmxCheckDeviceFd();

    Param.u32PcrChId = u32PcrChId;
    Param.u32SyncHandle = u32SyncHandle;
    
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PCRSYN_ATTACH, &Param);

    return ret;
}

HI_S32 HI_MPI_DMX_PcrSyncDetach(HI_U32 u32PcrChId)
{
    HI_S32 ret;
    DMX_PCRSYNC_S Param;

    MPIDmxCheckDeviceFd();

    Param.u32PcrChId = u32PcrChId;
    
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PCRSYN_DETACH, &Param);

    return ret;
}

HI_S32 HI_MPI_DMX_GetPESBufferStatus(HI_HANDLE hChannel, HI_MPI_DMX_BUF_STATUS_S *pBufStat)
{
    HI_S32 ret;
    DMX_PesBufStaGet_S Param;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pBufStat);

    Param.hChannel = hChannel;
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_PES_BUFFER_GETSTAT, &Param);

    if (HI_SUCCESS == ret)
    {
        memcpy(pBufStat, &Param.stBufStat, sizeof(HI_MPI_DMX_BUF_STATUS_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_AcquireEs(HI_HANDLE hChannel, HI_UNF_ES_BUF_S *pEsBuf)
{
    HI_S32 ret;
    DMX_PesBufGet_S Param;
    HI_U32 u32PhyAddr;
    HI_U32 u32DmxId, u32ChId;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pEsBuf);

    Param.hChannel = hChannel;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_ES_BUFFER_GET, &Param);
    if (HI_SUCCESS == ret)
    {
        u32DmxId = DMX_DMXID(hChannel);
        u32ChId = DMX_CHANID(hChannel);
        
        assert(u32DmxId < g_stChanBuf.DmxCnt);
        assert(u32ChId < g_stChanBuf.ChnCnt);
    
        u32PhyAddr = (HI_U32)Param.BufPhyAddr;
        if ((u32PhyAddr >= g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr)
            && ((u32PhyAddr + Param.BufSize - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr) <= g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].Size))
        {
            pEsBuf->pu8Buf = (HI_U8*)((u32PhyAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr)
                                      + g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr);
            pEsBuf->u32BufLen = Param.BufSize;
            pEsBuf->u32PtsMs = Param.PtsMs;
        }
        else
        {
            HI_ERR_DEMUX("Invalid physical addr of AV es data:ChanId=%d\n", u32ChId);
            return HI_FAILURE;
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_ReleaseEs(HI_HANDLE hChannel, const HI_UNF_ES_BUF_S *pEsBuf)
{
    DMX_PesBufGet_S Param;
    HI_U8 *UsrAddr;
    HI_U32 u32DmxId, u32ChId;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pEsBuf);
    DMX_CHECK_CHANHANDLE(hChannel);

    u32DmxId = DMX_DMXID(hChannel);
    u32ChId = DMX_CHANID(hChannel);

    assert(u32DmxId < g_stChanBuf.DmxCnt);
    assert(u32ChId < g_stChanBuf.ChnCnt);

    UsrAddr = pEsBuf->pu8Buf;
    if ((UsrAddr >= g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr)
        && ((UsrAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr) < g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].Size))
    {
        Param.BufPhyAddr = (HI_U32)((UsrAddr - g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].VirAddr)
                                        + g_stChanBuf.BufDesc[u32DmxId * g_stChanBuf.ChnCnt + u32ChId].PhyAddr);
        Param.BufSize = pEsBuf->u32BufLen;
        Param.PtsMs = pEsBuf->u32PtsMs;
    }
    else
    {
        HI_ERR_DEMUX("Invalid user virtual addr of AV es data:ChanId=%d\n", u32ChId);
        return HI_FAILURE;
    }

    Param.hChannel = hChannel;
    return ioctl(g_s32DmxFd, CMD_DEMUX_ES_BUFFER_PUT, &Param);
}

HI_S32 HI_MPI_DMX_CreateRecChn(HI_UNF_DMX_REC_ATTR_S *pstRecAttr, HI_HANDLE *phRecChn)
{
    HI_S32 ret;
    DMX_Rec_CreateChan_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecAttr);
    MPIDmxCheckPointer(phRecChn);

    memcpy(&Param.RecAttr, pstRecAttr, sizeof(Param.RecAttr));

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_CREATE, &Param);
    if (HI_SUCCESS == ret)
    {
        HI_U8 *UsrAddr;

        UsrAddr = DmxMmap(Param.RecBufPhyAddr, Param.RecBufFlag);
        if (HI_NULL == UsrAddr)
        {
            if (HI_SUCCESS != ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_DESTROY, &Param.RecHandle))
            {
                HI_ERR_DEMUX("destroy rec failed\n");
            }

            ret = HI_ERR_DMX_MMAP_FAILED;
        }
        else
        {
            HI_U32 DmxId = DMX_DMXID(Param.RecHandle);
            HI_U32 RecId = DMX_RECID(Param.RecHandle);

            assert(DmxId < g_stRecBuf.DmxCnt);
            assert(RecId < g_stRecBuf.RecCnt);

            g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].PhyAddr   = Param.RecBufPhyAddr;
            g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].VirAddr   = UsrAddr;
            g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].Size      = Param.RecBufSize;
            g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].Flag      = Param.RecBufFlag;

            *phRecChn = Param.RecHandle;
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DestroyRecChn(HI_HANDLE hRecChn)
{
    HI_S32 ret;
    HI_U32 DmxId, RecId;
    DMX_BUF_S *BufDesc = HI_NULL;

    MPIDmxCheckDeviceFd();
    DMX_CHECK_RECHANDLE(hRecChn);

    DmxId = DMX_DMXID(hRecChn);
    RecId = DMX_RECID(hRecChn);

    assert(DmxId < g_stRecBuf.DmxCnt);
    assert(RecId < g_stRecBuf.RecCnt);

    BufDesc = &g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId];

    if (BufDesc->PhyAddr)
    {
        ret = DmxMunmap(BufDesc->PhyAddr, BufDesc->Flag);
        if (HI_SUCCESS != ret)
        {
            HI_ERR_DEMUX("rec buffer unmap failed.\n");
            ret = HI_ERR_DMX_MUNMAP_FAILED;
            goto out;
        }

        BufDesc->VirAddr = HI_NULL;
        BufDesc->PhyAddr = 0;
        BufDesc->Size = 0;
    }

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_DESTROY, &hRecChn);

out:
    return ret;
}

HI_S32 HI_MPI_DMX_AddRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid, HI_HANDLE *phChannel)
{
    HI_S32              ret;
    DMX_Rec_AddPid_S    Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(phChannel);

    Param.RecHandle = hRecChn;
    Param.Pid       = u32Pid;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_ADD_PID, &Param);
    if (HI_SUCCESS == ret)
    {
        *phChannel = Param.ChanHandle;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_DelRecPid(HI_HANDLE hRecChn, HI_HANDLE hChannel)
{
    DMX_Rec_DelPid_S Param;

    MPIDmxCheckDeviceFd();

    Param.RecHandle     = hRecChn;
    Param.ChanHandle    = hChannel;

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_DEL_PID, &Param);
}

HI_S32 HI_MPI_DMX_DelAllRecPid(HI_HANDLE hRecChn)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_DEL_ALL_PID, &hRecChn);
}

HI_S32 HI_MPI_DMX_AddExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid)
{
    DMX_Rec_ExcludePid_S Param;

    MPIDmxCheckDeviceFd();

    Param.RecHandle = hRecChn;
    Param.Pid       = u32Pid;

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_ADD_EXCLUDE_PID, &Param);
}

HI_S32 HI_MPI_DMX_DelExcludeRecPid(HI_HANDLE hRecChn, HI_U32 u32Pid)
{
    DMX_Rec_ExcludePid_S Param;

    MPIDmxCheckDeviceFd();

    Param.RecHandle = hRecChn;
    Param.Pid       = u32Pid;

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_DEL_EXCLUDE_PID, &Param);
}

HI_S32 HI_MPI_DMX_DelAllExcludeRecPid(HI_HANDLE hRecChn)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_CANCEL_EXCLUDE, &hRecChn);
}

HI_S32 HI_MPI_DMX_StartRecChn(HI_HANDLE hRecChn)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_START, &hRecChn);
}

HI_S32 HI_MPI_DMX_StopRecChn(HI_HANDLE hRecChn)
{
    MPIDmxCheckDeviceFd();

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_STOP, &hRecChn);
}

HI_S32 HI_MPI_DMX_AcquireRecData(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_S *pstRecData, HI_U32 u32TimeoutMs)
{
    HI_S32                  ret;
    DMX_Rec_AcquireData_S   Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecData);

    Param.RecHandle = hRecChn;
    Param.TimeoutMs = u32TimeoutMs;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_ACQUIRE_DATA, &Param);
    if (HI_SUCCESS == ret)
    {
        HI_U32 DmxId = DMX_DMXID(hRecChn);
        HI_U32 RecId = DMX_RECID(hRecChn);
        HI_U32 Offset;

        assert(DmxId < g_stRecBuf.DmxCnt);
        assert(RecId < g_stRecBuf.RecCnt);

        Offset = Param.BufPhyAddr - g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].PhyAddr;

        pstRecData->pDataAddr       = g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].VirAddr + Offset;
        pstRecData->u32DataPhyAddr  = Param.BufPhyAddr;
        pstRecData->u32Len          = Param.BufSize;
    }

    return ret;
}

HI_S32 HI_MPI_DMX_ReleaseRecData(HI_HANDLE hRecChn, const HI_MPI_DMX_REC_DATA_S *pstRecData)
{
    DMX_Rec_ReleaseData_S   Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecData);

    Param.RecHandle = hRecChn;
    Param.BufPhyAddr = pstRecData->u32DataPhyAddr;
    Param.BufSize = pstRecData->u32Len;

    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_RELEASE_DATA, &Param);
}

HI_S32 HI_MPI_DMX_AcquireRecIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_INDEX_S *pstRecIndex, HI_U32 u32TimeoutMs)
{
    HI_S32                  ret;
    DMX_Rec_AcquireIndex_S  Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecIndex);

    Param.RecHandle = hRecChn;
    Param.TimeoutMs = u32TimeoutMs;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_ACQUIRE_INDEX, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstRecIndex, &Param.IndexData, sizeof(HI_MPI_DMX_REC_INDEX_S));
    }

    return ret;
}

static HI_S32 DmxMixRecDataAndIndex(HI_MPI_DMX_REC_DATA_INDEX_S *Old, HI_MPI_DMX_REC_DATA_INDEX_S *New)
{
    assert(1 == New->u32RecDataCnt);
    assert(Old->u32IdxNum + New->u32IdxNum <= DMX_MAX_IDX_ACQUIRED_EACH_TIME);
    
    if (1 == Old->u32RecDataCnt)
    {
        if (Old->stRecData[0].u32DataPhyAddr + Old->stRecData[0].u32Len == New->stRecData[0].u32DataPhyAddr)
        {
            Old->stRecData[0].u32Len += New->stRecData[0].u32Len;
        }
        else
        {
            Old->u32RecDataCnt = 2;
                
            Old->stRecData[1].pDataAddr = New->stRecData[0].pDataAddr; 
            Old->stRecData[1].u32DataPhyAddr = New->stRecData[0].u32DataPhyAddr;
            Old->stRecData[1].u32Len = New->stRecData[0].u32Len;
        }

        memcpy(&Old->stIndex[Old->u32IdxNum], &New->stIndex[0], sizeof(HI_MPI_DMX_REC_INDEX_S) * New->u32IdxNum);   
        Old->u32IdxNum += New->u32IdxNum;
    }
    else
    {
        assert(2 == Old->u32RecDataCnt);

         if (Old->stRecData[1].u32DataPhyAddr + Old->stRecData[1].u32Len == New->stRecData[0].u32DataPhyAddr)
         {
            Old->stRecData[1].u32Len += New->stRecData[0].u32Len;
         }
         else
         {
            assert(0);
         }

        memcpy(&Old->stIndex[Old->u32IdxNum], &New->stIndex[0], sizeof(HI_MPI_DMX_REC_INDEX_S) * New->u32IdxNum);
        Old->u32IdxNum += New->u32IdxNum;
    }

    return HI_SUCCESS;
}

static HI_S32 DmxAcquireRecDataAndIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_INDEX_S* pstRecDataIdx)
{
    HI_S32 ret;
    HI_U32 DmxId, RecId;
    DMX_Rec_ProcessDataIndex_S Param = {
        .RecHandle = hRecChn,
        .RecDataIdx = pstRecDataIdx,
    };
    
    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_ACQUIRE_DATA_INDEX, &Param);
    if (HI_SUCCESS == ret)
    {
        HI_U32 Offset;

        DmxId = DMX_DMXID(hRecChn);
        RecId = DMX_RECID(hRecChn);
        
        assert(1 == pstRecDataIdx->u32RecDataCnt);

        Offset = pstRecDataIdx->stRecData[0].u32DataPhyAddr - g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].PhyAddr;
        
        pstRecDataIdx->stRecData[0].pDataAddr  = (HI_U8*)(g_stRecBuf.BufDesc[DmxId * g_stRecBuf.RecCnt + RecId].VirAddr + Offset);
    }    
    
    return ret;
}

HI_S32 HI_MPI_DMX_AcquireRecDataAndIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_INDEX_S* pstRecDataIdx)
{
    HI_S32 ret = HI_FAILURE;
    
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecDataIdx);

    ret = DmxAcquireRecDataAndIndex(hRecChn, pstRecDataIdx);
    if (HI_SUCCESS != ret)
    {
        goto out;
    }
    
    /* 
     * recv more until no available data. this policy maybe has some implicit risk.
     * DmxAcquireRecDataAndIndex always return success if stream comes too fast.
     * There may be result ts rec buffer overflow.
     * For example, define DMX_SUPPORT_SCRAMB_SOFT_IDX will enable soft index and 
     * this while will dead loop.
     */
    while(1)
    {
        HI_MPI_DMX_REC_DATA_INDEX_S RecDataIdx;
        
        ret = DmxAcquireRecDataAndIndex(hRecChn, &RecDataIdx);
        if (HI_SUCCESS != ret)
        {
            /* pstRecDataIdx include one index at least */
            if (0 == pstRecDataIdx->u32IdxNum)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        /*
         * FIXME:
         * in theory, we should backup RecDataidx if pstRecDataIdx.stIndex array overflow.
         * but it's too complicated, so we reserve 20% space slot for avoid overflow.
         */
        DmxMixRecDataAndIndex(pstRecDataIdx, &RecDataIdx);
        
        if (pstRecDataIdx->u32IdxNum > DMX_MAX_IDX_ACQUIRED_EACH_TIME * 80 / 100)
        {
            break;
        }
    }

    ret = HI_SUCCESS;
    
out:
    return ret;
}

HI_S32 HI_MPI_DMX_ReleaseRecDataAndIndex(HI_HANDLE hRecChn, HI_MPI_DMX_REC_DATA_INDEX_S* pstRecDataIdx)
{
    DMX_Rec_ProcessDataIndex_S Param = {
        .RecHandle = hRecChn,
        .RecDataIdx = pstRecDataIdx,
    };

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstRecDataIdx);
    
    return ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_RELEASE_DATA_INDEX, &Param);
}

HI_S32 HI_MPI_DMX_GetRecBufferStatus(HI_HANDLE hRecChn, HI_UNF_DMX_RECBUF_STATUS_S *pstBufStatus)
{
    HI_S32              ret;
    DMX_Rec_BufStatus_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstBufStatus);

    Param.RecHandle = hRecChn;

    ret = ioctl(g_s32DmxFd, CMD_DEMUX_REC_CHAN_GET_BUF_STATUS, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstBufStatus, &Param.BufStatus, sizeof(HI_UNF_DMX_RECBUF_STATUS_S));
    }

    return ret;
}

HI_S32 HI_MPI_DMX_Invoke(HI_UNF_DMX_INVOKE_TYPE_E enCmd, const HI_VOID *pCmdPara)
{
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pCmdPara);
    
    if (HI_UNF_DMX_INVOKE_TYPE_CHAN_CC_REPEAT_SET == enCmd)
    {
        DMX_SetChan_CC_REPEAT_S Param;
        memcpy(&Param.stChCCRepeatSet, (HI_UNF_DMX_CHAN_CC_REPEAT_SET_S*)pCmdPara, sizeof(HI_UNF_DMX_CHAN_CC_REPEAT_SET_S));

        return ioctl(g_s32DmxFd, CMD_DEMUX_CHAN_CC_REPEAT_SET, &Param);
    }

    if (HI_UNF_DMX_INVOKE_TYPE_PUSI_SET == enCmd)
    {
        HI_UNF_DMX_PUSI_SET_S Param;
        memcpy(&Param, (HI_UNF_DMX_PUSI_SET_S*)pCmdPara, sizeof(HI_UNF_DMX_PUSI_SET_S));

        return ioctl(g_s32DmxFd, CMD_DEMUX_SET_PUSI, &Param);
    }

    if (HI_UNF_DMX_INVOKE_TYPE_TEI_SET == enCmd)
    {
        HI_UNF_DMX_TEI_SET_S Param;
        memcpy(&Param, (HI_UNF_DMX_TEI_SET_S*)pCmdPara, sizeof(HI_UNF_DMX_TEI_SET_S));

        return ioctl(g_s32DmxFd, CMD_DEMUX_SET_TEI, &Param);
    }
    
    if (HI_UNF_DMX_INVOKE_TYPE_TSI_ATTACH_TSO == enCmd)
    {
        return HI_ERR_DMX_NOT_SUPPORT;
    }
    
    HI_ERR_DEMUX("unknow cmd:%d.\n",enCmd);
    return HI_ERR_DMX_INVALID_PARA;
   
}

HI_S32 HI_MPI_DMX_GetCallback(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *pstCbDesc)
{
    HI_S32 ret  = HI_FAILURE;
    
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstCbDesc);

    if (0 == pthread_mutex_lock(&g_CbHookLock))
    {
        ret = DmxGetChanBufCb(hChannel, pstCbDesc);

        if(pthread_mutex_unlock(&g_CbHookLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_RegisterCallback(HI_HANDLE hChannel, HI_UNF_DMX_CB_DESC_S *pstCbDesc)
{
    HI_S32 ret  = HI_FAILURE;
    
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    MPIDmxCheckPointer(pstCbDesc);

    if (0 == pthread_mutex_lock(&g_CbHookLock))
    {
        if (pstCbDesc->enContextType < HI_UNF_DMX_CB_CONTEXT_TYPE_BUTT && pstCbDesc->pfnChanBufCb)
        {
            ret = DmxSetChanBufCb(hChannel, pstCbDesc);           
        }
        else
        {
            ret = HI_ERR_DMX_INVALID_PARA;            
        }     

        if (pthread_mutex_unlock(&g_CbHookLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }
    }

    return ret;
}

HI_S32 HI_MPI_DMX_UnRegisterCallback(HI_HANDLE hChannel)
{
    HI_S32 ret  = HI_FAILURE;
    
    MPIDmxCheckDeviceFd();
    DMX_CHECK_CHANHANDLE(hChannel);
    
    if (0 == pthread_mutex_lock(&g_CbHookLock))
    {
        ret = DmxUnsetChanBufCb(hChannel);            

        if (pthread_mutex_unlock(&g_CbHookLock))
        {
            HI_ERR_DEMUX("pthread_mutex_unlock failed.\n");
        }    
    }

    return ret;
}

HI_S32 HI_MPI_DMX_GetResumeCount(HI_U32 *pCount)
{
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pCount);

    return ioctl(g_s32DmxFd, CMD_DEMUX_GET_RESUME_COUNT, pCount);
}

HI_S32 HI_MPI_RMX_GetDefaultAttr(HI_MPI_RMX_ATTR_S * pstAttr)
{
    MPIDmxCheckPointer(pstAttr);

    pstAttr->enOutPortId = HI_MPI_DMX_PORT_RMX_0;

    return HI_SUCCESS;
}

HI_S32 HI_MPI_RMX_Create(HI_MPI_RMX_ATTR_S * pstAttr, HI_HANDLE *phRmx)
{
    HI_S32 ret;
    RMX_Create_S Param;
    
    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);
    MPIDmxCheckPointer(phRmx);

    memcpy(&Param.Attr, pstAttr, sizeof(HI_MPI_RMX_ATTR_S));
    
    ret = ioctl(g_s32DmxFd, CMD_REMUX_CREATE, &Param);
    if (HI_SUCCESS == ret)
    {
        *phRmx = Param.Handle;
    }

    return ret;
}

HI_S32 HI_MPI_RMX_Destroy(HI_HANDLE hRmx)
{
    MPIDmxCheckDeviceFd();
    RMX_CHECK_HANDLE(hRmx);
    
    return ioctl(g_s32DmxFd, CMD_REMUX_DESTROY, &hRmx);
}

HI_S32 HI_MPI_RMX_GetAttr(HI_HANDLE hRmx, HI_MPI_RMX_ATTR_S * pstAttr)
{   
    HI_S32 ret;
    RMX_Attr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    Param.Handle = hRmx;

    ret = ioctl(g_s32DmxFd, CMD_REMUX_GET_ATTR, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstAttr, &Param.Attr, sizeof(HI_MPI_RMX_ATTR_S));
    }
    
    return ret;
}

HI_S32 HI_MPI_RMX_SetAttr(HI_HANDLE hRmx, HI_MPI_RMX_ATTR_S * pstAttr)
{
    RMX_Attr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    Param.Handle = hRmx;
    memcpy(&Param.Attr, pstAttr, sizeof(HI_MPI_RMX_ATTR_S));

    return ioctl(g_s32DmxFd, CMD_REMUX_SET_ATTR, &Param);
}

HI_S32 HI_MPI_RMX_Start(HI_HANDLE hRmx)
{
    MPIDmxCheckDeviceFd();
    RMX_CHECK_HANDLE(hRmx);

    return ioctl(g_s32DmxFd, CMD_REMUX_START, &hRmx);
}

HI_S32 HI_MPI_RMX_Stop(HI_HANDLE hRmx)
{
    MPIDmxCheckDeviceFd();
    RMX_CHECK_HANDLE(hRmx);

    return ioctl(g_s32DmxFd, CMD_REMUX_STOP, &hRmx);
}

HI_S32 HI_MPI_RMX_AddPump(HI_HANDLE hRmx, HI_MPI_RMX_PUMP_ATTR_S *pstAttr, HI_HANDLE *phRmxPump)
{
    HI_S32 ret;
    RMX_Add_Pump_S Param;
    
    MPIDmxCheckDeviceFd();
    RMX_CHECK_HANDLE(hRmx);
    MPIDmxCheckPointer(pstAttr);
    MPIDmxCheckPointer(phRmxPump);

    Param.RmxHandle = hRmx;
    memcpy(&Param.Attr, pstAttr, sizeof(HI_MPI_RMX_PUMP_ATTR_S));
    
    ret = ioctl(g_s32DmxFd, CMD_REMUX_ADD_PUMP, &Param);
    if (HI_SUCCESS == ret)
    {
        *phRmxPump = Param.PumpHandle;
    }

    return ret;
}

HI_S32 HI_MPI_RMX_DelPump(HI_HANDLE hRmxPump)
{
    MPIDmxCheckDeviceFd();
    RMX_CHECK_PUMPHANDLE(hRmxPump);

    return ioctl(g_s32DmxFd, CMD_REMUX_DEL_PUMP, &hRmxPump);
}

HI_S32 HI_MPI_RMX_GetPumpDefaultAttr(HI_MPI_RMX_PUMP_ATTR_S * pstAttr)
{
    HI_S32 ret;
    RMX_Pump_Attr_S Param ;
    
    MPIDmxCheckPointer(pstAttr);

    ret = ioctl(g_s32DmxFd, CMD_REMUX_GET_PUMP_DEFAULT_ATTR, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstAttr, &Param.Attr, sizeof(HI_MPI_RMX_PUMP_ATTR_S));
    }

    return ret;
}

HI_S32 HI_MPI_RMX_GetPumpAttr(HI_HANDLE hRmxPump, HI_MPI_RMX_PUMP_ATTR_S *pstAttr)
{
    HI_S32 ret;
    RMX_Pump_Attr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    Param.PumpHandle = hRmxPump;

    ret = ioctl(g_s32DmxFd, CMD_REMUX_GET_PUMP_ATTR, &Param);
    if (HI_SUCCESS == ret)
    {
        memcpy(pstAttr, &Param.Attr, sizeof(HI_MPI_RMX_PUMP_ATTR_S));
    }
    
    return ret;
}

HI_S32 HI_MPI_RMX_SetPumpAttr(HI_HANDLE hRmxPump, HI_MPI_RMX_PUMP_ATTR_S *pstAttr)
{
    RMX_Pump_Attr_S Param;

    MPIDmxCheckDeviceFd();
    MPIDmxCheckPointer(pstAttr);

    Param.PumpHandle = hRmxPump;
    memcpy(&Param.Attr, pstAttr, sizeof(HI_MPI_RMX_PUMP_ATTR_S));

    return ioctl(g_s32DmxFd, CMD_REMUX_SET_PUMP_ATTR, &Param);
}


