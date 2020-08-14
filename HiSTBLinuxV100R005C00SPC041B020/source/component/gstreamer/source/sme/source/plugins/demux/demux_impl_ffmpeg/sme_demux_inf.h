/**************************************************************************//**

  Copyright (C), 2001-2012, Huawei Tech. Co., Ltd.

 ******************************************************************************
 * @file    sme_demux_inf.h
 * @brief   demux 接口定义
 * @author  liurongliang(l00180035)
 * @date    2014/3/25
 * @version VFP xxxxxxxxx
 * History:
 *
 * Number : V1.00
 * Date   : 2014/3/25
 * Author : liurongliang(l00180035)
 * Desc   : Created file
 *
******************************************************************************/

#ifndef __SME_DEMUX_INF_H__
#define __SME_DEMUX_INF_H__

#ifdef __cplusplus
extern "C"{
#endif //__cplusplus

#include "osal_type.h"
#include "sme_media_type.h"


typedef V_VOID* SMEDEMUXHDL;
typedef V_VOID* SMEDEMUXFMT;
typedef const V_VOID* CSMEDEMUXFMT;

// same to avformat.h in ffmpeg
#define AVSEEK_FLAG_BACKWARD 1   // seek key frame backward of the target seek point
#define AVSEEK_FLAG_SYNC     16  // seek to only keyframe

typedef enum _tagSmeDemuxRet
{
    E_SME_DEMUX_RET_SUCCESS = ICS_SUCCESS,//成功
    E_SME_DEMUX_RET_UNSUPPORT,//不支持
    E_SME_DEMUX_RET_INVALID_ARG,//非法参数
    E_SME_DEMUX_RET_NOMEM,//分配内存错误
    E_SME_DEMUX_RET_INVALID_OPERATION,//非法操作
    E_SME_DEMUX_RET_NO_DATA,//没有读取到数据
    E_SME_DEMUX_RET_EOS,//码流结束
    E_SME_DEMUX_RET_NOT_OPENNED,//demux没有打开
    E_SME_DEMUX_RET_FATAL,//致命错误，不可恢复
    E_SME_DEMUX_RET_OPEN_FAIL, //码流打开失败
    E_SME_DEMUX_RET_UN_SUPPORT_STREAM,     //stream not supported.
    E_SME_DEMUX_RET_STOP_READ_DATA,
}E_SME_DEMUX_RET;

typedef enum _tagSmeSeekFlgs
{
    E_SME_SEEK_FLGS_BACKWARD = 0,
    E_SME_SEEK_FLGS_FORWARD,
    E_SME_SEEK_FLGS_NEAREST,
}E_SME_SEEK_FLGS;

typedef enum _tagSmeInvokeIds
{
    E_SME_INVOKE_SET_USERAGENT = 0,
    E_SME_INVOKE_SET_REFERER,
    E_SME_INVOKE_SET_COOKIE,
    E_SME_INVOKE_SET_HEADERS,
}E_SME_INVOKE_IDS;

//same with ffmpeg2.8.x
typedef struct _tagSmeIOInterruptCB{
    V_INT32 (*callback)(void*, V_INT32, V_INT32, void*);
    V_VOID *opaque;
} ST_SME_IOINTERRUPTCB;

typedef struct _tagSmeIOProto
{
    const V_CHAR* pstName;
    V_INT32 (*SME_OpenIOProto)(INOUT V_VOID* pvUrlCtx, IN const V_CHAR* pszUrl, IN V_INT32 i32Flags);
    V_INT32 (*SME_Read)(INOUT V_VOID* pvUrlCtx, INOUT V_UINT8* pu8Buf, IN V_INT32 i32Size);
    V_INT32 (*SME_Write)(INOUT V_VOID* pvUrlCtx, IN const V_UINT8* pu8Buf, IN V_INT32 i32Size);
    V_INT64 (*SME_Seek)(INOUT V_VOID* pvUrlCtx, IN V_INT64 i64Pos, IN V_INT32 i32Where);
	V_INT32 (*SME_Close)(INOUT V_VOID* pvUrlCtx);
}ST_SME_IOPROTO, *PST_SME_IOPROTO;

typedef struct _tagSmeDemuxInf
{
    SMEDEMUXHDL (*SME_CreateDemux)(
        IN const ST_SME_IOINTERRUPTCB* pstIOInterruptCb);
    V_VOID (*SME_DestroyDemux)(INOUT SMEDEMUXHDL phdl);

    //同步过程
    E_SME_DEMUX_RET (*SME_OpenDemux)(IN SMEDEMUXHDL hdlDemux,
                        IN const V_CHAR* pszUrl,
                        IN const ST_SME_IOPROTO* pstIOProto,
                        IN SMEDEMUXFMT demuxFmt);

    V_VOID (*SME_CloseDemux)(INOUT SMEDEMUXHDL hdlDemux);

    //i32StreamIdx,-1表示又demux决定输出哪条流的帧。
    E_SME_DEMUX_RET (*SME_ReadFrame)(IN SMEDEMUXHDL hdlDemux,
                        IN V_INT32 i32StreamIdx,
                        INOUT ST_SME_FRAMEBUFFER* pstFrameInfo);

    //i32StreamIdx 为流索引，-1为所有流,返回<0失败，其他为实际seek到得时间点
    V_INT32 (*SME_SeekTo)(IN SMEDEMUXHDL hdlDemux,
                        IN V_INT32 i32SeekToms,
                        IN V_INT32 i32StreamIdx,
                        IN V_INT32 i32Flags);

    //clean the data in ffmpeg and ffmpeg adapter
    void (*SME_Flush)(IN SMEDEMUXHDL hdlDemux);

    //open成功之后调用
    E_SME_DEMUX_RET (*SME_GetStreamInfo)(IN SMEDEMUXHDL hdlDemux, INOUT ST_SME_STREAMINFO* pstInfo);

    //open成功之后调用
    CSMEDEMUXFMT (*SME_GetCurrentDemuxCtx)(IN SMEDEMUXHDL hdlDemux);

    //返回值>=0为score值, <0出错
    V_INT32 (*SME_ProbeByData)(IN CSMEDEMUXFMT demuxCtx, IN const V_UINT8* pu8Data, IN V_UINT32 u32Size);
    const V_CHAR* (*SME_GetDemuxName)(IN CSMEDEMUXFMT demuxCtx);
    const V_CHAR* (*SME_GetDemuxDescription)(IN CSMEDEMUXFMT demuxCtx);

    //第一个时demuxCtx输入NULL。
    CSMEDEMUXFMT (*SME_GetNextDemux)(IN CSMEDEMUXFMT demuxCtx);

    V_INT32 (*SME_IsSeekable)(IN SMEDEMUXHDL hdlDemux,
                        IN V_INT32 i32StreamIdx);

    E_SME_DEMUX_RET (*SME_InvokeDemux)(IN SMEDEMUXHDL hdlDemux,
                        IN E_SME_INVOKE_IDS eInvokeId,
                        INOUT const V_CHAR* pszData);

    //stop reading data from ffmpeg
    void (*SME_StopRead)(IN SMEDEMUXHDL hdlDemux);
}ST_SME_DEMUX_INF, *PST_SME_DEMUX_INF;

//单例模式
const ST_SME_DEMUX_INF* SME_GetDemuxInf();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__SME_DEMUX_INF_H__
