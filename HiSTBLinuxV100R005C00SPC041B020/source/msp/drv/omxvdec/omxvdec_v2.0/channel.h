
/*
 * Copyright (c) (2014 - ...) Device Chipset Source Algorithm and Chipset Platform Development Dept
 * Hisilicon. All rights reserved.
 *
 * File:    channel.h
 *
 * Purpose: omxvdec channel interface
 *
 * Author:  yangyichang 00226912
 *
 * Date:    26, 11, 2014
 *
 */

#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#include <asm/div64.h>
#include "omxvdec.h"
#include "message.h"


#define  LAST_FRAME_FLAG_NULL        (0)
#define  DECODER_REPORT_LAST_FRAME   (1)
#define  PROCESSOR_GOT_LAST_FRAME    (2)
#define  DECODER_REPORT_FAKE_FRAME   (4)
#define  REPORT_LAST_FRAME_SUCCESS   (0)
#define  REPORT_LAST_FRAME_FAIL      (1)
#define  REPORT_LAST_FRAME_WITH_ID   (2)

#define  MAX_DFS_BUF_NUM             (32)

#ifdef HI_OMX_TEE_SUPPORT
#define OMXVDEC_SEC_ZONE                "SEC-MMZ"
#endif

#define VDEC_INIT_MUTEX(pSem)             \
do {                                      \
    sema_init(pSem, 1);                   \
}while(0)

#define VDEC_DOWN_INTERRUPTIBLE(pSem)     \
do {                                      \
    if(down_interruptible(pSem))          \
    {                                     \
        OmxPrint(OMX_FATAL, "%s down_interruptible failed!\n", __func__); \
        return HI_FAILURE;                \
    }                                     \
}while(0)

#define VDEC_DOWN_INTERRUPTIBLE_RETURN(pSem,errRet)     \
do {                                      \
    if(down_interruptible(pSem))          \
    {                                     \
        OmxPrint(OMX_FATAL, "%s down_interruptible failed!\n", __func__); \
        return errRet;                \
    }                                     \
}while(0)

#define VDEC_UP_INTERRUPTIBLE(pSem)       \
do {                                      \
    up(pSem);                             \
}while(0)

typedef enum {
    CHAN_STATE_INVALID = 0,
    CHAN_STATE_IDLE,
    CHAN_STATE_WORK,
    CHAN_STATE_PAUSE,
    CHAN_STATE_PAUSE_PENDING,
}eCHAN_STATE;

typedef enum {
    BUF_STATE_INVALID = 0,
    BUF_STATE_IDLE,
    BUF_STATE_QUEUED,
    BUF_STATE_USING,
}eBUFFER_STATE;

typedef enum {
    DFS_INIT = 0,
    DFS_WAIT_RESPONSE_0,
    DFS_WAIT_INSERT,
    DFS_WAIT_BIND,
    DFS_WAIT_FILL,
    DFS_WAIT_ACTIVATE,
    DFS_WAIT_RESPONSE_1,
    DFS_WAIT_CONFIG,
    DFS_DONE,
    DFS_WAIT_ALLOC,
}eDFS_STATE;

typedef struct
{
    HI_U32 BufFd;
    HI_U8* pu8StartVirAddr;
    HI_U8* pu8DmaBuf;
    HI_U32 u32Offset;
} OMXVDEC_ION_BUF_S;

typedef struct {

    eBUFFER_TYPE       buf_type;
    eBUFFER_STATE      status;
    HI_U32             phy_addr;
    HI_U32             private_phy_addr;//VPSS 假4K输出需要metabuf传递额外信息
    HI_U32             buf_len;
    HI_U32             act_len;
    HI_U32             private_len;
    HI_U32             offset;
    HI_U32             flags;
    HI_U32             buf_id;
    HI_S64             time_stamp;
    HI_U32             usr_tag;
    struct list_head   list;

    HI_VOID           *user_vaddr;
    HI_VOID           *kern_vaddr;
    HI_VOID           *client_data;
    HI_VOID           *private_vaddr;   /*kernel viradd of metadata buffer*/
    HI_U32             u32FrameRate;
    HI_U32             u32SrcFrameRate;

    OMXVDEC_ION_BUF_S  Buf_meta_priv;
}OMXVDEC_BUF_S;

typedef struct {
    HI_U32 ETB;
    HI_U32 EBD;
    HI_U32 FTB;
    HI_U32 FBD;
    HI_U32 DecoderIn;
    HI_U32 DecoderOut;
    HI_U32 ProcessorIn;
    HI_U32 ProcessorOut;
} OMXVDEC_CHAN_STATINFO;

typedef struct {
    HI_DRV_VIDEO_FRAME_TYPE_E   enSrcFrameType;
    HI_DRV_VIDEO_STD_E CodecType;
    HI_DRV_COLOUR_DESCRIPTION_INFO_S stColourInfo;
    union
    {
        HI_DRV_VIDEO_DOLBY_INFO_S     stDolbyInfo;    /*Dolby frame info.*/
        HI_DRV_VIDEO_HDR10_INFO_S     stHDR10Info;    /*HDR10 frame info.*/
        HI_DRV_VIDEO_HLG_INFO_S       stHLGInfo;    /*HLG frame info.*/
        HI_DRV_VIDEO_SLF_INFO_S       stSLFInfo;    /*SLF frame info.*/
        HI_DRV_VIDEO_TECHNICOLOR_INFO_S stTechnicolorInfo;    /*Technicolor frame info.*/
    } unHDRInfo;
} OMXVDEC_CHAN_HDR_INFO;

typedef struct {
    MMZ_BUFFER_S       frm_buf;
    MMZ_BUFFER_S       pmv_buf;
    eMEM_ALLOC         frm_buf_type;
    eMEM_ALLOC         pmv_buf_type;
    HI_BOOL            is_available;       /* 表示是不是可以配置给vfmw，空闲的帧存才能配置给vfmw */
    HI_BOOL            is_configured;
    HI_BOOL            is_CanRls;
    HI_BOOL            is_wait_release;
} DSF_SINGLE_BUF_S;

typedef struct {
    spinlock_t         dfs_lock;
    HI_BOOL            occupied_frm_exist;
    HI_U32             occupied_frm_phyaddr;
    HI_U32             need_frm_num;
    HI_U32             need_frm_size;
    HI_U32             need_pmv_num;
    HI_U32             need_pmv_size;
    HI_U32             image_num_only;
    HI_U32             image_size_only;
    HI_U32             alloc_frm_num;      /* already alloc  frm buf num */
    HI_U32             alloc_pmv_num;      /* already alloc  pmv buf num */
    HI_U32             config_frm_num;     /* already config frm buf num */
    HI_U32             config_pmv_num;     /* already config pmv buf num */
    HI_U32             delay_time;
    HI_U32             retry_count;
    HI_U32             alloc_frame_only;
    eDFS_STATE         dfs_state;
    DSF_SINGLE_BUF_S   single_buf[MAX_DFS_BUF_NUM];
} OMXVDEC_DFS_INFO;

typedef struct {

    HI_S32             channel_id;
    HI_S32             decoder_id;
    HI_S32             processor_id;
    HI_S32             port_id;
    HI_BOOL            m_use_native_buf;
    HI_U32             out_width;
    HI_U32             out_height;
    HI_U32             out_stride;
    HI_U32             protocol;
    HI_S32             rotation;
    HI_U32             bReversed;
    HI_U32             input_buf_num;
    HI_U32             output_buf_num;
    HI_U32             max_in_buf_num;
    HI_U32             max_out_buf_num;
    HI_U32             msg_queue_depth;
    HI_U32             raw_use_cnt;
    HI_U32             yuv_use_cnt;
    HI_U32             input_flush_pending:   1,
                       output_flush_pending:  1,
                       pause_pending:         1,
                       eos_in_list:           1,           // eos in list flag
                       eos_recv_flag:         1,           // end of stream flag
                       eof_send_flag:         1,           // last frame flag
                       recfg_flag:            1,           // reconfig flag
                       port_enable_flag:      1,
                       reset_pending:         1,           // reset flag
                       seek_pending:          1,
                       progress:              1;

    HI_U32             *file_dec;

    HI_BOOL            is_tvp;
    HI_BOOL            is_overlay;
    HI_BOOL            is_10bit;
    HI_BOOL            is_compress;
    HI_U8              last_frame_processor_got;
    HI_U8              last_frame_image_id;
                       /* 0 (0/1)  : vfmw 是否上报;
                          1 (0/1/2): vfmw 上报的类型
                                     0 success,
                                     1 fail,
                                     2+ report last frame image id */
    HI_U8              last_frame_info[2];
    HI_S32             bLowdelay;
    HI_BOOL            bVpssBypass;
    OMX_PIX_FORMAT_E   color_format;
    eCHAN_STATE        state;

    eMEM_ALLOC         eCTXMemAlloc;
    eMEM_ALLOC         eSCDMemAlloc;
    eMEM_ALLOC         eVDHMemAlloc;
    eMEM_ALLOC         eEXTRAMemAlloc;
    eMEM_ALLOC         eEOSMemAlloc;
    eMEM_ALLOC         eChanMemAlloc;

    HI_VOID           *in_buf_table;
    HI_VOID           *out_buf_table;
    msg_queue_s       *msg_queue;
    OMXVDEC_ENTRY     *vdec;

    struct list_head   chan_list;
    struct list_head   raw_queue;
    struct list_head   yuv_queue;
    OMXVDEC_BUF_S      last_frame;

    struct semaphore   chan_mutex;
    spinlock_t         chan_lock;
    spinlock_t         raw_lock;
    spinlock_t         yuv_lock;

    STREAM_INTF_S      stream_ops;
    IMAGE_INTF_S       image_ops;

    MMZ_BUFFER_S       channel_buf;
    MMZ_BUFFER_S       channel_extra_buf;
    MMZ_BUFFER_S       decoder_scd_buf;
    MMZ_BUFFER_S       decoder_vdh_buf;
    MMZ_BUFFER_S       decoder_eos_buf;

    OMXVDEC_DFS_INFO   dfs;

    OMXVDEC_CHAN_STATINFO  omx_chan_statinfo;
    OMXVDEC_LOWDELAY_PROC_RECORD* omx_vdec_lowdelay_proc_rec;
    HI_U32             TunnelModeEnable;
    OMXVDEC_CHAN_HDR_INFO  omx_chan_hdr_info;
}OMXVDEC_CHAN_CTX;

typedef struct tagVDEC_PREMMZ_NODE_S
{
    HI_U32  u32StartVirAddr;
    HI_U32  u32StartPhyAddr;
    HI_U32  u32Size;
    HI_U32  u32NodeState;/*0:have MMZ not used,1:have MMZ but used 2:invalid*/
}VDEC_PREMMZ_NODE_S;

#ifdef VFMW_VPSS_BYPASS_EN
typedef struct
{
    HI_U32 u32InputWidth;
    HI_U32 u32InputHeight;
    HI_DRV_PIX_FORMAT_E enInputPixFormat;
    HI_U32 u32InputFrameRate;
    HI_BOOL bVpssBypass;                  //输出参数
}PROCESSOR_BYPASSATTR_S;
#endif

HI_S32 channel_init(HI_VOID);

HI_VOID channel_exit(HI_VOID);

HI_S32 channel_create_inst(struct file *fd, OMXVDEC_DRV_CFG *pcfg);

HI_S32 channel_release_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_start_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_stop_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_reset_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_pause_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_resume_inst(OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_bind_user_buffer(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

HI_S32 channel_unbind_user_buffer(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

HI_S32 channel_empty_this_stream(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

HI_S32 channel_fill_this_frame(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

HI_S32 channel_flush_inst(OMXVDEC_CHAN_CTX *pchan, ePORT_DIR dir);

HI_S32 channel_get_message(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_MSG_INFO *pmsg);

HI_S32 channel_report_message(OMXVDEC_CHAN_CTX *pchan, HI_U32 msgcode, HI_VOID *priv);

HI_S32 channel_check_avaible_frame_buffer(OMXVDEC_CHAN_CTX *pchan);

OMXVDEC_CHAN_CTX* channel_find_inst_by_channel_id(OMXVDEC_ENTRY *vdec, HI_S32 handle);

OMXVDEC_CHAN_CTX* channel_find_inst_by_decoder_id(OMXVDEC_ENTRY *vdec, HI_S32 handle);

OMXVDEC_CHAN_CTX* channel_find_inst_by_processor_id(OMXVDEC_ENTRY *vdec, HI_S32 handle);

HI_VOID channel_add_lowdelay_tag_time(OMXVDEC_CHAN_CTX *pchan, UINT32 tag, UINT32 type, UINT32 time);

HI_VOID channel_proc_entry(struct seq_file *p, OMXVDEC_CHAN_CTX *pchan);

HI_S32 channel_alloc_buf(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

HI_S32 channel_release_buf(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);

#ifdef VFMW_VPSS_BYPASS_EN
HI_S32 channel_record_occupied_frame(OMXVDEC_CHAN_CTX *pchan);
HI_S32 channel_release_frame(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);
HI_BOOL channel_IsOccupiedFrm(HI_U32 TargetPhyAddr, HI_U32 *pIndex);
HI_S32 channel_find_nodeId_can_release(OMXVDEC_List_S* pList, HI_U32* pIndex);

#ifdef HI_TVOS_SUPPORT
HI_S32 channel_global_release_frame(OMXVDEC_CHAN_CTX *pchan, OMXVDEC_BUF_DESC *puser_buf);
#endif

HI_S32 channel_set_processor_bypass(OMXVDEC_CHAN_CTX *pchan);
HI_S32 channel_get_processor_bypass(OMXVDEC_CHAN_CTX *pchan,OMXVDEC_DRV_CFG *chan_cfg);
#endif
HI_S32 channel_Vdec_Report(HI_S32 chan_id, HI_U32 EventID, HI_S32 result, HI_VOID* PrivData);
/*================ EXTERN FUNCTION ================*/

#if (1 == PRE_ALLOC_VDEC_VDH_MMZ)
HI_S32 VDEC_Chan_FindPreMMZ(MMZ_BUFFER_S *pstMMZBuffer);
HI_S32 VDEC_Chan_ReleasePreMMZ(MMZ_BUFFER_S *pstMMZBuffer);
#endif

#endif
