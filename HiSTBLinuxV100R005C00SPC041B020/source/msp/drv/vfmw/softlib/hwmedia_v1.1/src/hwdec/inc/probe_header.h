
#ifndef __IMEDIA_PROBE_HEADER_H__
#define __IMEDIA_PROBE_HEADER_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

INT32 H264_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 MPEG2_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 MPEG4_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 H263_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 WMV3_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 VC1_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 VP6_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 VP6A_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

INT32 Sorenson_Spark_ProbeHeader(UINT8 *pucBuf, UINT32 uiNumBytes, UINT32 *puiBytesConsumed, INT32 *piStatus, STRU_IVIDDEC_META_DATA *pstMetaData, STRU_IVIDEO_STREAM_INFO *pstParams);

#ifdef __cplusplus
#if __cplusplus
}
#endif    /* __cpluscplus*/
#endif    /* __cpluscplus*/

#endif /**< __IMEDIA_PROBE_HEADER_H__ */
