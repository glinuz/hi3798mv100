/*--------------------------------------------------------------------------------------------------------------------------*/
/*!!Warning: This is a key information asset of Huawei Tech Co.,Ltd                                                         */
/*CODEMARK:kOyQZYzjDpyGdBAEC2GaWinjiDDUykL9e8pckESWBbMVmSWkBuyJO01cTiy3TdzKxGk0oBQa
mSMf7J4FkTpfv2bO4a86Ra1cP5w/R608AW9TGITFV3+8N3VYYiVf+Q2SydsXizP+UzuonkR7
tfVi+z2ragFK+Bn+WvtfaaSj5i7OEoOIgw5VlaR6fMfv0O57/9BnaDhfjToiSUrH6Ry3+v7F
dEtWrRzjBzMAiQZu2witl2eW7N0nBt5XzNdfVyfX8ec03FfZv9L0kVBbldJj3Q==*/
/*--------------------------------------------------------------------------------------------------------------------------*/
/******************************************************************************

  Copyright (C), 2007-2015, Hisilicon Technologies Co., Ltd.

 ******************************************************************************
  File Name     : hi_audsp_aoe.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/02/28
  Description   :
  History       :
  1.Date        : 2013/02/28
    Author      : zgjie
    Modification: Created file

 *******************************************************************************/

#ifndef  __HI_AUDSP_AOE__H__
#define  __HI_AUDSP_AOE__H__

#include "hi_type.h"
#include "hi_audsp_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/

#define AOE_RUNNING_FLAG 0xa5a5a5a5


/**Defines the  status of an ADSP.*/
typedef enum
{
    ADSP_SYSCOM_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    ADSP_SYSCOM_CMD_STOP,     /**<Start*//**<CNcomment: 运行 */

    ADSP_SYSCOM_CMD_BUTT
} ADSP_SYSCOM_CMD_E;

typedef enum
{
    ADSP_SYSCOM_CMD_DONE = 0,
    ADSP_SYSCOM_CMD_ERR_START_FAIL = 1,
    ADSP_SYSCOM_CMD_ERR_STOP_FAIL = 2,
    ADSP_SYSCOM_CMD_ERR_CREATE_FAIL = 3,
    ADSP_SYSCOM_CMD_ERR_UNKNOWCMD =  4,
    ADSP_SYSCOM_CMD_ERR_TIMEOUT = 5,
    ADSP_SYSCOM_CMD_ERR_BUTT
} ADSP_SYSCOM_CMD_RET_E;

typedef enum
{
    ADSP_AOE_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    ADSP_AOE_CMD_STOP,     /**<Start*//**<CNcomment: 运行 */
    ADSP_AOE_CMD_BUTT
} ADSP_AOESYS_CMD_E;

typedef enum
{
    ADSP_AOE_CMD_DONE = 0,
    ADSP_AOE_CMD_ERR_START_FAIL = 1,
    ADSP_AOE_CMD_ERR_STOP_FAIL = 2,
    ADSP_AOE_CMD_ERR_CREATE_FAIL = 3,
    ADSP_AOE_CMD_ERR_UNKNOWCMD =  4,
    ADSP_AOE_CMD_ERR_TIMEOUT = 5,
    ADSP_AOE_CMD_ERR_BUTT
} ADSP_AOESYS_CMD_RET_E;

typedef enum
{
    AOE_FORMAT_LPCM = 0,           /**<linear pcm*//**<CNcomment: LPCM格式 */
    AOE_FORMAT_IEC61937 = 1,       /**<iec61937 raw data*//**<CNcomment: iec61937协议透传数据 */
    AOE_FORMAT_IEC61937_COMP = 2,  /**<compressed iec61937 raw data*//**<CNcomment: 经压缩后的iec61937协议透传数据 */
    AOE_FORMAT_BUTT = 3
} AOE_DATA_FORMAT_E;

#define AOE_REG_LENGTH  (0xB00)   // 0xB00 reg for aoe

#define AOE_MAX_ENGINE_NUM  6
#define AOE_MAX_AOP_NUM     8
#define AOE_MAX_AIP_NUM     8
#define AOE_MAX_CAST_NUM    4     // to do Cast num

#define AOE_COM_REG_OFFSET      0x0000
#define AOE_ENGINE_REG_OFFSET   0x0040
#define AOE_AIP_REG_OFFSET      0x0340
#define AOE_AOP_REG_OFFSET      0x0640
#define AOE_CAST_REG_OFFSET     0x0A40

#define AOE_COM_REG_BANDSIZE    sizeof(S_AOE_REGS_TYPE)      //length=0x40
#define AOE_AIP_REG_BANDSIZE    sizeof(S_AIP_REGS_TYPE)      //length=0x60*8=0x300
#define AOE_ENGINE_REG_BANDSIZE sizeof(S_MIXER_REGS_TYPE)    //length=0x80*6=0x300
#define AOE_AOP_REG_BANDSIZE    sizeof(S_AOP_REGS_TYPE)      //length=0x80*8=0x400
#define AOE_CAST_REG_BANDSIZE   sizeof(S_CAST_REGS_TYPE)     //length=0x30*4=0x0C0


#define AOE_COM_REG_BASE (DSP0_SHARESRAM_BASEADDR + AOE_COM_REG_OFFSET)

#define AOE_AIP_VOL_6dB    (0x7f)
#define AOE_AIP_VOL_0dB    (0x79)
#define AOE_AIP_VOL_infdB  (0x28)
#define AOE_AIP_VOL_MAX_dB (AOE_AIP_VOL_6dB)
#define AOE_AIP_VOL_MIN_dB (AOE_AIP_VOL_infdB)
#ifndef HI_SND_DSP_HW_SUPPORT
#define AIP_FIFO_LATENCYMS_DEFAULT    40
#else
#define AIP_FIFO_LATENCYMS_DEFAULT    20
#endif
#define AIP_CHANNEL_MODE_STEREO       0x0
#define AIP_CHANNEL_MODE_DOUBLE_MONO  0x1
#define AIP_CHANNEL_MODE_DOUBLE_LEFT  0x2
#define AIP_CHANNEL_MODE_DOUBLE_RIGHT 0x3
#define AIP_CHANNEL_MODE_EXCHANGE     0x4
#define AIP_CHANNEL_MODE_ONLY_RIGHT   0x5
#define AIP_CHANNEL_MODE_ONLY_LEFT    0x6
#define AIP_CHANNEL_MODE_MUTED        0x7
#define AIP_CHANNEL_MODE_BUTT         0x8

#define AOE_AEF_MAX_CHAN_NUM     4   //用户态音效从Aef mask的bit[AOE_AEF_MAX_CHAN_NUM]计起，DSP音效从Aef mask的bit[0]记起

#define AOE_ENGINE_PROC_SAMPLES  256 //The samples for each process of engine
#define AOE_AEF_PERIODBUF_NUM    6   //Aef Period Buffer Number(the same for inbuf & outbuf)
#define AOE_FIFODELAY_FRAME_NUM  16  //The frame num of fifo delay(consider of all aef delay)


typedef enum
{
    AOE_AIP_TYPE_PCM_ALSA = 0, /**<ALSA Pcm*//**<CNcomment: alsa pcm */
    AOE_AIP_TYPE_PCM_SLAVE,    /**2.0 Normal Pcm for slave track*//**<CNcomment: 2.0 普通 PCM */
    AOE_AIP_TYPE_PCM_DMX,      /**downmixer pcm for master track*//**<CNcomment: 下混pcm流 */
    AOE_AIP_TYPE_PCM_MC,       /**multi chn pcm for master track*//**<CNcomment: 多通道音频流 */
    AOE_AIP_TYPE_PCM_ASSOC,    /**assoc chn pcm for master track only when double decoder*//**<CNcomment: 双路解码之辅助通道音频流 */
    AOE_AIP_TYPE_PCM_BUTT
} AOE_AIP_TYPE_PCM_E;

typedef enum
{
    AOE_AIP_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_AIP_CMD_STOP,    /**<Start*//**<CNcomment: 运行 */
    AOE_AIP_CMD_PAUSE,
    AOE_AIP_CMD_FLUSH,

    AOE_AIP_CMD_BUTT
} AOE_AIP_CMD_E;

typedef enum
{
    AOE_AIP_CMD_DONE = 0,
    AOE_AIP_CMD_ERR_START_FAIL = 1,
    AOE_AIP_CMD_ERR_STOP_FAIL   = 2,
    AOE_AIP_CMD_ERR_CREATE_FAIL = 3,
    AOE_AIP_CMD_ERR_UNKNOWCMD = 4,
    AOE_AIP_CMD_ERR_TIMEOUT = 5,

    AOE_AIP_CMD_ERR_BUTT
} AOE_AIP_CMD_RET_E;

/**Defines the  status of an ENGINE.*/
typedef enum
{
    AOE_ENGINE_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_ENGINE_CMD_STOP,     /**<Start*//**<CNcomment: 运行 */

    AOE_ENGINE_CMD_BUTT
} AOE_ENGINE_CMD_E;


typedef enum
{
    AOE_ENGINE_CMD_DONE = 0,
    AOE_ENGINE_CMD_ERR_START_FAIL = 1,
    AOE_ENGINE_CMD_ERR_STOP_FAIL = 2,
    AOE_ENGINE_CMD_ERR_CREATE_FAIL = 3,
    AOE_ENGINE_CMD_ERR_UNKNOWCMD =  4,
    AOE_ENGINE_CMD_ERR_TIMEOUT = 5,
    AOE_ENGINE_CMD_ERR_BUTT
} AOE_ENGINE_CMD_RET_E;


/**Defines the  status of an AOP.*/
typedef enum
{
    AOE_AOP_CMD_START = 0, /**<Stop*//**<CNcomment: 停止 */
    AOE_AOP_CMD_STOP, /**<Start*//**<CNcomment: 运行 */

    AOE_AOP_CMD_BUTT
} AOE_AOP_CMD_E;

typedef enum
{
    AOE_AOP_CMD_DONE = 0,
    AOE_AOP_CMD_ERR_START_FAIL = 1,
    AOE_AOP_CMD_ERR_STOP_FAIL   = 2,
    AOE_AOP_CMD_ERR_CREATE_FAIL = 3,
    AOE_AOP_CMD_ERR_UNKNOWCMD = 4,
    AOE_AOP_CMD_ERR_TIMEOUT = 5,
    AOE_AOP_CMD_ERR_BUTT
} AOE_AOP_CMD_RET_E;


/* Define the union U_AOE_STATUS1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    excute_timeout_cnt    : 16  ; /* [15..0]  */
        unsigned int    schedule_timeout_cnt  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOE_STATUS1;

/* Define the union U_ENGINE_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    precision             : 2   ; /* [1..0]  */
        unsigned int    ch                    : 2   ; /* [3..2]  */
        unsigned int    fs                    : 4   ; /* [7..4]  */
        unsigned int    format                : 2   ; /* [9..8]  */
        unsigned int    aef_delay             : 6   ; /* [15..10]  */
        unsigned int    geq_bandnum           : 4   ; /* [19..16]  */
        unsigned int    aef_support_mc        : 1   ; /* [20]  */
        unsigned int    Reserved_1            : 11  ; /* [31..21]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_ATTR;

/* Define the union U_ENGINE_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmd                   : 2   ; /* [1..0]  */
        unsigned int    cmd_done              : 1   ; /* [2]  */
        unsigned int    cmd_return_value      : 4   ; /* [6..3]  */
        unsigned int    ena_profile           : 1   ; /* [7]  */
        unsigned int    ena_geq               : 1   ; /* [8]  */
        unsigned int    geq_change_attr       : 1   ; /* [9]  */
        unsigned int    ena_sendmute          : 1   ; /* [10]  */
        unsigned int    ena_avc               : 1   ; /* [11]  */
        unsigned int    avc_change_attr       : 1   ; /* [12]  */
        unsigned int    Reserved_2            : 19  ; /* [31..13]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_CTRL;

/* Define the union U_ENGINE_MIX_SRC */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aip_fifo_ena          : 16  ; /* [15..0]  */
        unsigned int    Reserved_3            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_MIX_SRC;

/* Define the union U_ENGINE_ROU_DST */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aop_buf_ena           : 16  ; /* [15..0]  */
        unsigned int    Reserved_4            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_ROU_DST;

/* Define the union U_ENGINE_PP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    pp_cmd                : 2   ; /* [1..0]  */
        unsigned int    pp_cmd_done           : 1   ; /* [2]  */
        unsigned int    dst_pp_enable_flag    : 16  ; /* [18..3]  */
        unsigned int    Reserved_5            : 13  ; /* [31..19]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_PP_CTRL;

/* Define the union U_ENGINE_STATUS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aver_mcps             : 8   ; /* [7..0]  */
        unsigned int    peak_mcps             : 8   ; /* [15..8]  */
        unsigned int    Reserved_7            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_STATUS0;

/* Define the union U_ENGINE_ATT_AEF */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aef_att_ena           : 16  ; /* [15..0]  */
        unsigned int    Reserved_8            : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_ATT_AEF;


/* Define the union U_ENGINE_AVC_TIME_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    attack_time           : 13  ; /* [12..0]  */
        unsigned int    release_time          : 13  ; /* [25..13]  */
        unsigned int    Reserved_9            : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_AVC_TIME_ATTR;

/* Define the union U_ENGINE_AVC_LEVEL_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    limiter_level         : 6   ; /* [5..0]  */
        unsigned int    threshold_level       : 6   ; /* [11..6]  */
        unsigned int    gain                  : 4   ; /* [15..12]  */
        unsigned int    Reserved_10           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_AVC_LEVEL_ATTR;

/* Define the union U_ENGINE_AEF_BUF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_size          : 24  ; /* [23..0]  */
        unsigned int    periond_num           : 4   ; /* [27..24]  */
        unsigned int    Reserved_13           : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_AEF_BUF_SIZE;

/* Define the union U_ENGINE_AEF_BUF_WRITEIDX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_write_idx     : 8   ; /* [7..0]  */
        unsigned int    periond_write_wrap    : 1   ; /* [8]  */
        unsigned int    Reserved_14           : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_AEF_BUF_WRITEIDX;

/* Define the union U_ENGINE_AEF_BUF_READIDX */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    periond_read_idx      : 8   ; /* [7..0]  */
        unsigned int    periond_read_wrap     : 1   ; /* [8]  */
        unsigned int    Reserved_15           : 23  ; /* [31..9]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_ENGINE_AEF_BUF_READIDX;


/* Define the union U_AIP_BUFF_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aip_precision         : 2   ; /* [1..0]  */
        unsigned int    aip_ch                : 3   ; /* [4..2]  */
        unsigned int    aip_fs                : 4   ; /* [8..5]  */
        unsigned int    aip_format            : 2   ; /* [10..9]  */
        unsigned int    aip_pcm_type          : 3   ; /* [13..11]  */
        unsigned int    aip_priority          : 1   ; /* [14]  */
        unsigned int    Reserved_21           : 17  ; /* [31..15]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_BUFF_ATTR;

/* Define the union U_AIP_FIFO_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fifo_precision        : 2   ; /* [1..0]  */
        unsigned int    fifo_ch               : 2   ; /* [3..2]  */
        unsigned int    fifo_fs               : 4   ; /* [7..4]  */
        unsigned int    fifo_format           : 2   ; /* [9..8]  */
        unsigned int    fifo_latency          : 6   ; /* [15..10]  */
        unsigned int    fifo_latency_real     : 6   ; /* [21..16]  */
        unsigned int    fifo_bypass           : 1   ; /* [22]  */
        unsigned int    Reserved_22           : 9   ; /* [31..23]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_FIFO_ATTR;

/* Define the union U_AIP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmd                   : 2   ; /* [1..0]  */
        unsigned int    Reserved_23           : 1   ; /* [2]  */
        unsigned int    cmd_done              : 1   ; /* [3]  */
        unsigned int    cmd_return_value      : 4   ; /* [7..4]  */
        unsigned int    volume                : 7   ; /* [14..8]  */
        unsigned int    fade_en               : 1   ; /* [15]  */
        unsigned int    fade_in_rate          : 4   ; /* [19..16]  */
        unsigned int    fade_out_rate         : 4   ; /* [23..20]  */
        unsigned int    dst_fs_adj_step       : 5   ; /* [28..24]  */
        unsigned int    dst_fs_adj_dir        : 2   ; /* [30..29]  */
        unsigned int    ena_profile           : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_CTRL;

/* Define the union U_AIP_SRC_ATTR_EXT */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    fs_ext                : 24  ; /* [23..0]  */
        unsigned int    Reserved_24           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_SRC_ATTR_EXT;

/* Define the union U_AIP_BUF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    buff_size             : 24  ; /* [23..0]  */
        unsigned int    buff_flag             : 1   ; /* [24]  */
        unsigned int    Reserved_27           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_BUF_SIZE;

/* Define the union U_AIP_BUF_TRANS_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    tx_trans_size         : 24  ; /* [23..0]  */
        unsigned int    Reserved_32           : 8   ; /* [31..24]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_BUF_TRANS_SIZE;

/* Define the union U_AIP_EXT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    volume_l              : 8   ; /* [7..0]  */
        unsigned int    volume_r              : 8   ; /* [15..8]  */
        unsigned int    mute                  : 1   ; /* [16]  */
        unsigned int    channel_mode          : 3   ; /* [19..17]  */
        unsigned int    dre_dither_decbps     : 3   ; /* [22..20]  */
        unsigned int    dre                   : 2   ; /* [24..23]  */
        unsigned int    dre_thrhd             : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_EXT_CTRL;

/* Define the union U_AIP_STATUS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aver_mcps             : 8   ; /* [7..0]  */
        unsigned int    peak_mcps             : 8   ; /* [15..8]  */
        unsigned int    Reserved_34           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_STATUS0;

/* Define the union U_AIP_FIFO_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aip_fifo_size         : 16  ; /* [15..0]  */
        unsigned int    aip_fifo_underflow_cnt  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_FIFO_SIZE;

/* Define the union U_AIP_EXT2_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    prescale_int          : 8   ; /* [7..0]  */
        unsigned int    prescale_dec          : 3   ; /* [10..8]  */
        unsigned int    prescale_dec_dir      : 1   ; /* [11]  */
        unsigned int    dest_aop              : 4   ; /* [15..12]  */
        unsigned int    skip_ms               : 8   ; /* [23..16]  */
        unsigned int    is_duplicate          : 1   ; /* [24]  */
        unsigned int    Reserved_35           : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AIP_EXT2_CTRL;

/* Define the union U_AOP_BUFF_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    buf_precision         : 2   ; /* [1..0]  */
        unsigned int    buf_ch                : 2   ; /* [3..2]  */
        unsigned int    buf_fs                : 4   ; /* [7..4]  */
        unsigned int    buf_format            : 2   ; /* [9..8]  */
        unsigned int    buf_priority          : 1   ; /* [10]  */
        unsigned int    buf_delay             : 9   ; /* [19..11]  */
        unsigned int    buf_latency           : 12   ; /* [31..20]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_BUFF_ATTR;

/* Define the union U_AOP_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    cmd                   : 2   ; /* [1..0]  */
        unsigned int    cmd_done              : 1   ; /* [2]  */
        unsigned int    cmd_return_value      : 4   ; /* [6..3]  */
        unsigned int    ena_profile           : 1   ; /* [7]  */
        unsigned int    ena_aef_bypass        : 1   ; /* [8]  */
        unsigned int    ena_drc               : 1   ; /* [9]  */
        unsigned int    ena_ad                : 1   ; /* [10]  */
        unsigned int    ena_peq               : 1   ; /* [11]  */
        unsigned int    peq_bandnum           : 4   ; /* [15..12]  */
        unsigned int    peq_change_attr       : 1   ; /* [16]  */
        unsigned int    drc_change_attr       : 1   ; /* [17]  */
        unsigned int    ena_add_mute          : 1   ; /* [18]  */
        unsigned int    cast_flag             : 1   ; /* [19]  */
        unsigned int    audiodelay            : 1   ; /* [20]  */
        unsigned int    realdelayms           : 9   ; /* [29..21]  */
        unsigned int    Reserved_38           : 2   ; /* [31..30]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_CTRL;

/* Define the union U_AOP_STATUS0 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    aver_mcps             : 8   ; /* [7..0]  */
        unsigned int    peak_mcps             : 8   ; /* [15..8]  */
        unsigned int    Reserved_39           : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_STATUS0;

/* Define the union U_AOP_EXT_CTRL */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    volume_l              : 8   ; /* [7..0]  */
        unsigned int    volume_r              : 8   ; /* [15..8]  */
        unsigned int    volumedec_l           : 3   ; /* [18..16]  */
        unsigned int    volumedec_dir_l       : 1   ; /* [19]  */
        unsigned int    volumedec_r           : 3   ; /* [22..20]  */
        unsigned int    volumedec_dir_r       : 1   ; /* [23]  */
        unsigned int    balance_val           : 6   ; /* [29..24]  */
        unsigned int    balance_dir           : 1   ; /* [30]  */
        unsigned int    mute                  : 1   ; /* [31]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_EXT_CTRL;

/* Define the union U_AOP_BUF_SIZE */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    buff_size             : 24  ; /* [23..0]  */
        unsigned int    buff_flag             : 1   ; /* [24]  */
        unsigned int    buff_underflow_cnt    : 7   ; /* [31..25]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_BUF_SIZE;

/* Define the union U_AOP_DRC_TIME_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    attack_time           : 13  ; /* [12..0]  */
        unsigned int    release_time          : 13  ; /* [25..13]  */
        unsigned int    Reserved_40           : 6   ; /* [31..26]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_DRC_TIME_ATTR;

/* Define the union U_AOP_DRC_LEVEL_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    start_level           : 7   ; /* [6..0]  */
        unsigned int    targert_level         : 7   ; /* [13..7]  */
        unsigned int    Reserved_41           : 18  ; /* [31..14]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_DRC_LEVEL_ATTR;

/* Define the union U_AOP_PEQ_BAND_TYPE1 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    band1_type            : 4   ; /* [3..0]  */
        unsigned int    band2_type            : 4   ; /* [7..4]  */
        unsigned int    band3_type            : 4   ; /* [11..8]  */
        unsigned int    band4_type            : 4   ; /* [15..12]  */
        unsigned int    band5_type            : 4   ; /* [19..16]  */
        unsigned int    band6_type            : 4   ; /* [23..20]  */
        unsigned int    band7_type            : 4   ; /* [27..24]  */
        unsigned int    band8_type            : 4   ; /* [31..28]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_PEQ_BAND_TYPE1;

/* Define the union U_AOP_PEQ_BAND_TYPE2 */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    band9_type            : 4   ; /* [3..0]  */
        unsigned int    band10_type           : 4   ; /* [7..4]  */
        unsigned int    Reserved_42           : 24  ; /* [31..8]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_AOP_PEQ_BAND_TYPE2;

/* Define the union U_EQ_BAND_ATTR */
typedef union
{
    /* Define the struct bits */
    struct
    {
        unsigned int    gain_value            : 7   ; /* [6..0]  */
        unsigned int    gain_dir              : 1   ; /* [7]  */
        unsigned int    q_value               : 8   ; /* [15..8]  */
        unsigned int    freq                  : 16  ; /* [31..16]  */
    } bits;

    /* Define an unsigned member */
    unsigned int    u32;

} U_EQ_BAND_ATTR;

//==============================================================================
/* Define the global struct */
typedef struct
{
    volatile unsigned int         AOE_MAGIC;
    volatile unsigned int         AOE_VERSION;
    volatile unsigned int         AOE_STATUS0;
    volatile U_AOE_STATUS1        AOE_STATUS1;
    volatile unsigned int         AOE_RESERVED[12];
} S_AOE_REGS_TYPE;

//MIXER struct
typedef struct
{
    volatile U_ENGINE_ATTR        ENGINE_ATTR;
    volatile U_ENGINE_CTRL        ENGINE_CTRL;
    volatile U_ENGINE_MIX_SRC     ENGINE_MIX_SRC;
    volatile U_ENGINE_ROU_DST     ENGINE_ROU_DST;
    volatile U_ENGINE_PP_CTRL     ENGINE_PP_CTRL;
    volatile unsigned int         ENGINE_PP_ADDR;
    volatile U_ENGINE_STATUS0     ENGINE_STATUS0;
    volatile U_ENGINE_ATT_AEF     ENGINE_ATT_AEF;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND1_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND2_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND3_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND4_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND5_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND6_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND7_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND8_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND9_ATTR;
    volatile U_EQ_BAND_ATTR       ENGINE_GEQ_BAND10_ATTR;
    volatile U_ENGINE_AVC_TIME_ATTR    ENGINE_AVC_TIME_ATTR;
    volatile U_ENGINE_AVC_LEVEL_ATTR   ENGINE_AVC_LEVEL_ATTR;
    S_AOE_ADDR_TYPE                    ENGINE_AEF_INBUF_ADDR;
    volatile U_ENGINE_AEF_BUF_SIZE     ENGINE_AEF_INBUF_SIZE;
    volatile U_ENGINE_AEF_BUF_WRITEIDX ENGINE_AEF_INBUF_WRITEIDX;
    volatile U_ENGINE_AEF_BUF_READIDX  ENGINE_AEF_INBUF_READIDX;
    S_AOE_ADDR_TYPE                    ENGINE_AEF_OUTBUF_ADDR;
    volatile U_ENGINE_AEF_BUF_SIZE     ENGINE_AEF_OUTBUF_SIZE;
    volatile U_ENGINE_AEF_BUF_WRITEIDX ENGINE_AEF_OUTBUF_WRITEIDX;
    volatile U_ENGINE_AEF_BUF_READIDX  ENGINE_AEF_OUTBUF_READIDX;
    volatile unsigned int              ENGINE_RESERVED[0x80 / 4 - 30];
} S_MIXER_REGS_TYPE;

//AIP struct
typedef struct
{
    volatile U_AIP_BUFF_ATTR      AIP_BUFF_ATTR;
    volatile U_AIP_FIFO_ATTR      AIP_FIFO_ATTR;
    volatile U_AIP_CTRL           AIP_CTRL;
    volatile U_AIP_SRC_ATTR_EXT   AIP_SRC_ATTR_EXT;
    S_AOE_ADDR_TYPE               AIP_BUF_ADDR;
    volatile U_AIP_BUF_SIZE       AIP_BUF_SIZE;
    S_AOE_ADDR_TYPE               AIP_BUF_WPTR;
    S_AOE_ADDR_TYPE               AIP_BUF_RPTR;
    volatile U_AIP_BUF_TRANS_SIZE AIP_BUF_TRANS_SIZE;
    volatile U_AIP_EXT_CTRL       AIP_EXT_CTRL;
    volatile unsigned int         AIP_BUF_PHYADDR;
    volatile U_AIP_STATUS0        AIP_STATUS0;
    volatile unsigned int         AIP_FIFO_ADDR;
    volatile U_AIP_FIFO_SIZE      AIP_FIFO_SIZE;
    volatile unsigned int         AIP_FIFO_WPTR;
    volatile unsigned int         AIP_FIFO_RPTR;
    volatile U_AIP_EXT2_CTRL      AIP_EXT2_CTRL;
    volatile unsigned int         AIP_FSADJ_CNT;
    volatile unsigned int         AIP_RESERVED[0x60 / 4 - 21];
} S_AIP_REGS_TYPE;

//AOP Struct
typedef struct
{
    volatile U_AOP_BUFF_ATTR      AOP_BUFF_ATTR;
    volatile U_AOP_CTRL           AOP_CTRL;
    volatile U_AOP_STATUS0        AOP_STATUS0;
    volatile U_AOP_EXT_CTRL       AOP_EXT_CTRL;
    S_AOE_ADDR_TYPE               AOP_BUF_ADDR;
    volatile U_AOP_BUF_SIZE       AOP_BUF_SIZE;
    S_AOE_ADDR_TYPE               AOP_BUF_WPTR;
    S_AOE_ADDR_TYPE               AOP_BUF_RPTR;
    volatile U_AOP_DRC_TIME_ATTR  AOP_DRC_TIME_ATTR;
    volatile U_AOP_DRC_LEVEL_ATTR AOP_DRC_LEVEL_ATTR;
    volatile U_AOP_PEQ_BAND_TYPE1 AOP_PEQ_BAND_TYPE1;
    volatile U_AOP_PEQ_BAND_TYPE2 AOP_PEQ_BAND_TYPE2;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND1_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND2_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND3_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND4_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND5_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND6_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND7_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND8_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND9_ATTR;
    volatile U_EQ_BAND_ATTR       AOP_PEQ_BAND10_ATTR;
    volatile unsigned int         AOP_ADDMUTE_CNT;
    volatile unsigned int         AOP_RESERVED[0x80 / 4 - 26];
} S_AOP_REGS_TYPE;

typedef struct
{
    volatile unsigned int         CAST_AOP_ID;
    volatile unsigned int         CAST_FIFO_SIZE;
    volatile unsigned int         CAST_FIFO_WPTR;
    volatile unsigned int         CAST_FIFO_RPTR;
    volatile unsigned int         CAST_FIFO_TRY_CNT;
    volatile unsigned int         CAST_FIFO_OK_CNT;
    volatile unsigned int         CAST_FIFO_FORCE_READ_CNT;
    volatile unsigned int         CAST_BUF_WRITEOK_CNT;
    volatile unsigned int         CAST_BUF_WRITEMUTE_CNT;
    volatile unsigned int         CAST_BUF_OVERWRITE_CNT;
    volatile unsigned int         CAST_BUF_WRITETOTAL_CNT;
    volatile unsigned int         CAST_RESERVED0;
} S_CAST_REGS_TYPE;

typedef struct
{
    HI_U32  u32MixDmxDataSize;   /**<Mix dowmmix data size */
    HI_U32  u32MixAssocDataSize;
    HI_U32  u32MixMcDataSize;
    HI_U32  u32BakMixDmxDataSize;
    HI_U32  u32SamplesPerFrame; /**<I/O, number of output samples per channel*/
    HI_U32  u32BitPerSample; /**<I/O, bit per sampling*/ /**<CNcomment:OUT. Bit per sample */
    HI_U32  u32Channels; /**<I/O, number of channels*/ /**<CNcomment:OUT. 输出声道数  */
    HI_U32  u32SampleRate; /**<I/O, sampling rate*/ /**<CNcomment:OUT. 输出采样频率 */
    HI_BOOL bFlush;
} AOE_AEF_FRAME_INFO_S;
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


