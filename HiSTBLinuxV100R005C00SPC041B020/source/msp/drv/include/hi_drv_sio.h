/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : mpi_priv_sio.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2009/5/5
  Description   : 
  History       :
  1.Date        : 2009/5/5
    Author      : p00123320
    Modification: Created file 
******************************************************************************/

#ifndef  __MKP_SIO_H__
#define  __MKP_SIO_H__

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* End of #ifdef __cplusplus */

//#include "mkp_ioctl.h"

#include "hi_debug.h"
#include "hi_unf_sio.h"
#include "drv_aiao_ext.h"
#include "hi_error_mpi.h"

typedef struct hiAIO_FRAME_INFO_S
{
	HI_U64 u64Pts;
	HI_U32 u32Seq;
	HI_U32 u32Len;
	void * pdata;
    HI_U32 u32PaddingForU64Align;
}AIO_FRAME_INFO_S;

typedef struct hiAI_FRAME_COMBINE_S
{
	AIO_FRAME_INFO_S *pstAiFrame;
	AIO_FRAME_INFO_S *pstRefFrame;
	HI_BOOL 		 bRefDataValid;
}AI_FRAME_COMBINE_S;

typedef struct hiAI_ENABLE_AEC_S
{
	AUDIO_DEV 	AoDevId;
	AO_CHN 		AoChn;
}AI_ENABLE_AEC_S;

typedef struct hiAO_BIND_AI_ARGS_S
{
    AUDIO_DEV 	AiDev;
	AI_CHN 		AiChn;
} AO_BIND_AI_ARGS_S;

typedef struct hiAIAO_BIND_ARGS_S
{
    AUDIO_DEV 	AiDev;
	AI_CHN 		AiChn;
    AUDIO_DEV 	AoDev;
	AO_CHN 		AoChn;
} AIAO_BIND_ARGS_S;

typedef struct hiADECAO_BIND_ARGS_S
{
    ADEC_CHN    AdChn;
    AUDIO_DEV 	AoDev;
	AO_CHN 		AoChn;
} ADECAO_BIND_ARGS_S;

#define CHECK_NULL_PTR(ptr)\
do{\
    if(NULL == ptr )\
        return HI_ERR_SIO_AI_NULL_PTR;\
}while(0)


#define CHECK_AIO_DEV(dev)\
do{\
     if((dev >= SIO_MAX_NUM) || (dev < 0))\
     {\
        HI_TRACE_SIO(HI_DBG_ERR, "aio device %d is invalid\n",dev);\
        return HI_ERR_SIO_AI_INVALID_DEVID;\
     }\
}while(0)

#define CHECK_AIO_CHN(chn)\
do{\
     if((chn >= AIO_MAX_CHN_NUM) || (chn < 0))\
     {\
        HI_TRACE_SIO(HI_DBG_ERR, "aio channel %d is invalid\n",chn);\
        return HI_ERR_SIO_AI_INVALID_CHNID;\
     }\
}while(0)

#define SIO_CHECK_DEV(s32DevId)\
 do{\
     if(s32DevId < 0 || s32DevId >= SIO_MAX_NUM)\
     {\
        HI_TRACE_SIO(HI_DBG_ERR, "aio dev %d is invalid\n",s32DevId);\
        return HI_ERR_SIO_AI_INVALID_DEVID;\
     }\
}while(0)
     
                   
#define AI_CHECK_CHNID(s32ChnId)\
do{\
    if(s32ChnId < 0 || s32ChnId >= SIO_MAX_NUM*AIO_MAX_CHN_NUM)\
    {\
        HI_TRACE_SIO(HI_DBG_ERR, "ai chnid %d is invalid\n",s32ChnId);\
        return HI_ERR_SIO_AI_INVALID_CHNID;\
    }\
}while(0)
       
#define AO_CHECK_CHNID(s32ChnId)\
 do{\
     if(s32ChnId < 0 || s32ChnId >= SIO_MAX_NUM*AIO_MAX_CHN_NUM)\
     {\
        HI_TRACE_SIO(HI_DBG_ERR, "ao chnid %d is invalid\n",s32ChnId);\
        return HI_ERR_SIO_AO_INVALID_CHNID;\
     }\
}while(0)

     
#define AIO_GET_CHNID(dev,i)          ((dev)*AIO_MAX_CHN_NUM + (i))
#define AIO_GETDEV_BY_CHNID(s32ChnId) ((s32ChnId)/AIO_MAX_CHN_NUM)
#define AIO_GETCHN_BY_CHNID(s32ChnId) ((s32ChnId)%AIO_MAX_CHN_NUM)

#define IS_AI_EXTERN(pstAttr)\
    ((pstAttr)->enBitwidth == HI_UNF_BIT_DEPTH_8 && (pstAttr)->u32EXFlag )

typedef enum hiIOC_NR_AI_E
{
	IOC_NR_AI_BINDFD = 0,
    IOC_NR_AI_SET_ATTR, 
    IOC_NR_AI_GET_ATTR,
    IOC_NR_AI_EN_DEV,
    IOC_NR_AI_DIS_DEV,
    IOC_NR_AI_EN_AEC,
    IOC_NR_AI_DIS_AEC,
    IOC_NR_AI_EN_CHN,
    IOC_NR_AI_DIS_CHN,
    IOC_NR_AI_PAUSE_CHN,
    IOC_NR_AI_RESUME_CHN,
    IOC_NR_AI_GET_FRM,
    IOC_NR_AI_EN_RESMP,
    IOC_NR_AI_DIS_RESMP,
    
    IOC_NR_AI_BUTT
} IOC_NR_AI_E;

typedef enum hiIOC_NR_AO_E
{
	IOC_NR_AO_BINDFD = 0,
    IOC_NR_AO_SET_ATTR, 
    IOC_NR_AO_GET_ATTR,
    IOC_NR_AO_EN_DEV,
    IOC_NR_AO_DIS_DEV,
    IOC_NR_AO_EN_AEC,
    IOC_NR_AO_DIS_AEC,
    IOC_NR_AO_EN_CHN,
    IOC_NR_AO_DIS_CHN,
    IOC_NR_AO_PAUSE_CHN,
    IOC_NR_AO_RESUME_CHN,
    IOC_NR_AO_PUT_FRM,
    IOC_NR_AO_BIND_ADEC,
    IOC_NR_AO_UNBIND_ADEC,
    IOC_NR_AO_BIND_AI,
    IOC_NR_AO_UNBIND_AI,
    IOC_NR_AO_GET_BIND_AI,
    IOC_NR_AO_GET_BIND_ADEC,
    IOC_NR_AO_EN_RESMP,
    IOC_NR_AO_DIS_RESMP,
    IOC_NR_AO_CLEAR_BUF,
    
    IOC_NR_AO_BUTT
} IOC_NR_AO_E;

#define IOC_TYPE_AI    'Z'
#define IOC_TYPE_AO    'X'
#define IOC_TYPE_SIO   'S'

#define AI_BIND_CHANNEL2FD      _IOW(IOC_TYPE_AI, IOC_NR_AI_BINDFD, HI_U32)
#define GET_AI_ATTR             _IOR(IOC_TYPE_AI, IOC_NR_AI_GET_ATTR,AIO_ATTR_S)/* get device attribute */
#define SET_AI_ATTR             _IOW(IOC_TYPE_AI, IOC_NR_AI_SET_ATTR,AIO_ATTR_S)/* set device attribute */
#define ENABLE_AI_TRANS         _IO(IOC_TYPE_AI, IOC_NR_AI_EN_DEV)/* enalbe device */
#define DISABLE_AI_TRANS        _IO(IOC_TYPE_AI, IOC_NR_AI_DIS_DEV)/* disable device */
#define GET_AI_FRAME		    _IOWR(IOC_TYPE_AI, IOC_NR_AI_GET_FRM,AI_FRAME_COMBINE_S)/* get AI frame */
#define AI_ENABLE_AEC			_IOW(IOC_TYPE_AI, IOC_NR_AI_EN_AEC,AI_ENABLE_AEC_S)/* enable echo */
#define AI_DISABLE_AEC			_IO(IOC_TYPE_AI, IOC_NR_AI_DIS_AEC)/* disable echo */
#define ENABLE_AI_CHN           _IO(IOC_TYPE_AI, IOC_NR_AI_EN_CHN)/* enable AI input channel */
#define DISABLE_AI_CHN          _IO(IOC_TYPE_AI, IOC_NR_AI_DIS_CHN)/* disable AI input channel */
#define AI_ENABLE_RESMP         _IOW(IOC_TYPE_AI, IOC_NR_AI_EN_RESMP, AUDIO_RESAMPLE_ATTR_S)/* enable resample */
#define AI_DISABLE_RESMP        _IO(IOC_TYPE_AI, IOC_NR_AI_DIS_RESMP)/* disable resample */


#define AO_BIND_CHANNEL2FD      _IOW(IOC_TYPE_AO, IOC_NR_AO_BINDFD, HI_U32)
#define GET_AO_ATTR             _IOR(IOC_TYPE_AO, IOC_NR_AO_GET_ATTR, AIO_ATTR_S)/* get device attribute */
#define SET_AO_ATTR             _IOW(IOC_TYPE_AO, IOC_NR_AO_SET_ATTR, AIO_ATTR_S)/* set device attribute */
#define ENABLE_AO_TRANS         _IO(IOC_TYPE_AO, IOC_NR_AO_EN_DEV)/* enable device */
#define DISABLE_AO_TRANS        _IO(IOC_TYPE_AO, IOC_NR_AO_DIS_DEV)/* disable device */
#define PUT_AO_FRAME		    _IOW(IOC_TYPE_AO, IOC_NR_AO_PUT_FRM,AIO_FRAME_INFO_S)/* send AO frame */
#define ENABLE_AO_CHN           _IO(IOC_TYPE_AO, IOC_NR_AO_EN_CHN)/* enable AO output channel */
#define DISABLE_AO_CHN          _IO(IOC_TYPE_AO, IOC_NR_AO_DIS_CHN)/* disable AO output channel */
#define PAUSE_AO_CHN            _IO(IOC_TYPE_AO, IOC_NR_AO_PAUSE_CHN)/* pause AO output channel */
#define RESUME_AO_CHN           _IO(IOC_TYPE_AO, IOC_NR_AO_RESUME_CHN)/* resume AO output channel */
#define AO_BIND_ADEC_CTRL       _IOW(IOC_TYPE_AO, IOC_NR_AO_BIND_ADEC, ADEC_CHN)
#define AO_UNBIND_ADEC_CTRL     _IOW(IOC_TYPE_AO, IOC_NR_AO_UNBIND_ADEC, ADEC_CHN)
#define AO_BIND_AI_CTRL         _IOW(IOC_TYPE_AO, IOC_NR_AO_BIND_AI, AO_BIND_AI_ARGS_S)
#define AO_UNBIND_AI_CTRL       _IOW(IOC_TYPE_AO, IOC_NR_AO_UNBIND_AI, AO_BIND_AI_ARGS_S)
#define AO_GET_BIND_AI_CTRL     _IOWR(IOC_TYPE_AO, IOC_NR_AO_GET_BIND_AI, AIAO_BIND_ARGS_S)
#define AO_GET_BIND_ADEC_CTRL   _IOWR(IOC_TYPE_AO, IOC_NR_AO_GET_BIND_ADEC, ADECAO_BIND_ARGS_S)
#define AO_ENABLE_RESMP         _IOW(IOC_TYPE_AO, IOC_NR_AO_EN_RESMP, AUDIO_RESAMPLE_ATTR_S)/* enable resample */
#define AO_DISABLE_RESMP        _IO(IOC_TYPE_AO, IOC_NR_AO_DIS_RESMP)/* disable resample */
#define AO_CLEAR_BUF_CTRL       _IO(IOC_TYPE_AO, IOC_NR_AO_CLEAR_BUF)/* clear buffer */

#if 0
HI_S32 MPI_AO_Init(HI_VOID);
HI_S32 MPI_AO_Exit(HI_VOID);
HI_S32 MPI_AO_EnableReSmp(HI_U32 u32ChnId, AUDIO_RESAMPLE_ATTR_S *pstAttr);
HI_S32 MPI_AO_DisableReSmp(HI_U32 u32ChnId);

HI_S32 MPI_AI_Init(HI_VOID);
HI_S32 MPI_AI_Exit(HI_VOID);
HI_S32 MPI_AI_EnableReSmp(HI_U32 u32ChnId, AUDIO_RESAMPLE_ATTR_S *pstAttr);
HI_S32 MPI_AI_DisableReSmp(HI_U32 u32ChnId);

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif/* End of #ifndef __MPI_AUDIO_H__*/


