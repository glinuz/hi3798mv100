/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hi_pvr_index.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2008/04/24
  Description   :
  History       :
  1.Date        : 2010/06/17
    Author      : j40671
    Modification: Created file

******************************************************************************/

#ifndef __PVR_SCD_H__
#define __PVR_SCD_H__

#include "hi_unf_demux.h"
#include "hi_mpi_demux.h"

#include "drv_demux_index.h"
#include "drv_demux_define.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* start code type definition(data from SCD buffer) */
#define DMX_INDEX_SC_TYPE_TS         0x1      /* ts packet header */
#define DMX_INDEX_SC_TYPE_PTS        0x2      /* pes packet header */
#define DMX_INDEX_SC_TYPE_PAUSE      0x3      /* pause flag */
#define DMX_INDEX_SC_TYPE_PIC        0x4      /* the start 00 00 01 of frame data */
#define DMX_INDEX_SC_TYPE_PIC_SHORT  0x5      /* the short head 00 01 of frame data */
#define DMX_INDEX_SC_TYPE_PES_ERR    0xf      /* the header of PES syntax error */

#define HEVC_DATA_OF_SC_OFFSET 4   /* 00 00 01 xx */
#define HEVC_DATA_OF_SC_TOTAL_LEN (16 * 1024)
#define HEVC_DATA_OF_SC_SAVED_LEN 8  /* this bytes has saved in PVR_SCDDmxIdxToPvrIdx. */
#define HEVC_DUP_DATA_CMP_LEN (HEVC_DATA_OF_SC_OFFSET + HEVC_DATA_OF_SC_SAVED_LEN)
#define HEVC_DUP_DATA_TOTAL_LEN (HEVC_DATA_OF_SC_OFFSET + HEVC_DATA_OF_SC_TOTAL_LEN)

HI_VOID DmxRecUpdateFrameInfo(HI_U32 *Param, FRAME_POS_S *IndexInfo);
HI_S32  DmxScdToVideoIndex(HI_BOOL bUseTimeStamp,const DMX_IDX_DATA_S *ScData, FINDEX_SCD_S *pstFidx);
HI_S32  DmxFixupHevcIndex(Dmx_Set_S *DmxSet, DMX_RecInfo_S  *RecInfo, FINDEX_SCD_S *pstFidx);
HI_S32  DmxScdToAudioIndex(HI_MPI_DMX_REC_INDEX_S *LastFrame, const DMX_IDX_DATA_S *ScData);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

