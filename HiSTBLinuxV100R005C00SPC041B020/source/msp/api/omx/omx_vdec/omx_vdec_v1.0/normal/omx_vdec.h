
/*========================================================================
  Open MAX   Component: Video Decoder
  This module contains the class definition for openMAX decoder component.
  Author: yangyichang 00226912
========================================================================*/

#ifndef __OMX_VDEC_H__
#define __OMX_VDEC_H__

//khrons standard head file
#include <pthread.h>
#include <semaphore.h>

#include "OMX_Component.h"
#include "OMX_Video.h"
#include "OMX_VideoExt.h"
#include "OMX_IndexExt.h"
#include "OMX_RoleNames.h"

#include "drv_omxvdec.h"
#include "omx_event_queue.h"
#include "omx_drv_ctx.h"


#define MAX_PORT_NUM                       (2)

#define DEFAULT_FPS                        (30)
#define DEFAULT_ALIGN_SIZE                 (4096)

#define DEFAULT_IN_BUF_SIZE                (2*1024*1024)

/***********************************************************************
   NOTICE!!
   IN_BUF_CNT & OUT_BUF_CNT should be no more than MIN(MAX_BUFF_NUM,32),
   MAX_BUFF_NUM is defined in channel.h
***********************************************************************/
#define DEF_MAX_IN_BUF_CNT                 (5)
#define DEF_MIN_IN_BUF_CNT                 (3)
#define DEF_MAX_OUT_BUF_CNT                (7)
#define DEF_MIN_OUT_BUF_CNT                (3)

#define MAX_IN_BUF_SLOT_NUM                (3)

//the max num can be recorded by component, default DEF_MAX_OUT_BUF_CNT + 2, can be changed if needed
#define MAX_OUT_BUF_SLOT_NUM               (8)

#define BITS_PER_LONG                      (32)

#define COUNTOF(x)                         (sizeof(x)/sizeof(x[0]))

#define OMX_CHECK_ARG_RETURN(x) \
    if ((x)) \
    {\
         DEBUG_PRINT_ERROR("[%s]"#x"\n", __func__);\
         return OMX_ErrorBadParameter;\
    }

#define ALIGN_UP(val, align)               (((val) + ((align)-1)) & ~((align)-1))


// state transactions pending bits
enum flags_bit_positions{

    OMX_STATE_IDLE_PENDING                = 0x1,
    OMX_STATE_LOADING_PENDING            = 0x2,
    OMX_STATE_INPUT_ENABLE_PENDING        = 0x3,
    OMX_STATE_OUTPUT_ENABLE_PENDING        = 0x4,
    OMX_STATE_OUTPUT_DISABLE_PENDING    = 0x5,
    OMX_STATE_INPUT_DISABLE_PENDING        = 0x6,
    OMX_STATE_OUTPUT_FLUSH_PENDING        = 0x7,
    OMX_STATE_INPUT_FLUSH_PENDING        = 0x8,
    OMX_STATE_PAUSE_PENDING                = 0x9,
    OMX_STATE_EXECUTE_PENDING            = 0xA,

};

// Deferred callback identifiers
enum {

    OMX_GENERATE_COMMAND_DONE            = 0x1,
    OMX_GENERATE_FTB                    = 0x2,
    OMX_GENERATE_ETB                    = 0x3,
    OMX_GENERATE_COMMAND                = 0x4,
    OMX_GENERATE_EBD                    = 0x5,
    OMX_GENERATE_FBD                    = 0x6,
    OMX_GENERATE_FLUSH_INPUT_DONE        = 0x7,
    OMX_GENERATE_FLUSH_OUTPUT_DONE        = 0x8,
    OMX_GENERATE_START_DONE                = 0x9,
    OMX_GENERATE_PAUSE_DONE                = 0xA,
    OMX_GENERATE_RESUME_DONE            = 0xB,
    OMX_GENERATE_STOP_DONE                = 0xC,
    OMX_GENERATE_EOS_DONE                = 0xD,
    OMX_GENERATE_HARDWARE_ERROR            = 0xE,
    OMX_GENERATE_IMAGE_SIZE_CHANGE        = 0xF,
    OMX_GENERATE_DYNAMIC_ALLOC_ERROR    = 0x10,
    OMX_GENERATE_UNUSED                    = 0x11,

};

enum {

    INPUT_PORT_INDEX    = 0,
    OUTPUT_PORT_INDEX    = 1,

};

struct port_property {

    OMX_U32 port_dir;
    OMX_U32 min_count;
    OMX_U32 max_count;
    OMX_U32 buffer_size;
    OMX_U32 alignment;

};

struct omx_hisi_extern_index{

    OMX_S8 index_name[OMX_MAX_STRINGNAME_SIZE];
    OMX_HISI_EXTERN_INDEXTYPE index_type;

};

struct codec_info {

       const OMX_STRING role_name;
       OMX_S32 compress_fmt;
       VDEC_CODEC_TYPE codec_type;

};

struct codec_profile_level{

    OMX_S32 profile;
    OMX_S32 level;

};

struct frame_info {

    OMX_U32 frame_width;
    OMX_U32 frame_height;
    OMX_U32 max_frame_width;
    OMX_U32 max_frame_height;
    OMX_U32 stride;
    OMX_U32 scan_lines;
};

// port private for omx
typedef struct OMX_PORT_PRIVATE {

    OMX_BUFFERHEADERTYPE **m_omx_bufhead;
    struct port_property port_pro;

    OMX_U32    m_port_index;

    OMX_U32 m_cur_buf_num;

    OMX_BOOL m_port_enabled;
    OMX_BOOL m_port_populated;

    OMX_BOOL m_port_reconfig;
    OMX_BOOL m_port_flushing;

} OMX_PORT_PRIVATE;


// Decode Protocal Parameter
typedef union
{
    OMX_VIDEO_PARAM_AVCTYPE avc;
    OMX_VIDEO_PARAM_VC1TYPE vc1;
    OMX_VIDEO_PARAM_MPEG2TYPE mpeg2;
    OMX_VIDEO_PARAM_MPEG4TYPE mpeg4;
    OMX_VIDEO_PARAM_H263TYPE h263;
    OMX_VIDEO_PARAM_VP6TYPE vp6;
    OMX_VIDEO_PARAM_VP8TYPE vp8;
    OMX_VIDEO_PARAM_WMVTYPE wmv;
    OMX_VIDEO_PARAM_RVTYPE rv;
    OMX_VIDEO_PARAM_HEVCTYPE hevc;

}CodecParam;

//component private for omx
typedef struct OMX_COMPONENT_PRIVATE {

    pthread_mutex_t m_lock;
    OMX_COMPONENTTYPE *m_pcomp;
    OMX_STATETYPE m_state;
    OMX_U32 m_flags;

    CodecParam m_codec_param;

    OMX_VIDEO_CODINGTYPE m_dec_fmt;
    OMX_U8 m_role[OMX_MAX_STRINGNAME_SIZE];

    OMX_PTR m_app_data;
    OMX_CALLBACKTYPE m_cb;

    OMX_U32 m_frame_rate;
    OMX_TICKS m_pre_timestamp;

    OMX_PORT_PRIVATE m_port[MAX_PORT_NUM];
    OMX_BOOL m_use_native_buf;
    OMX_BOOL is_adaptive;

    pthread_t msg_thread_id;
    pthread_t event_thread_id;

    OMX_BOOL event_thread_exit;
    OMX_BOOL msg_thread_exit;

    OMXVDEC_DRV_CONTEXT drv_ctx;
    struct frame_info pic_info;

    int m_pipe_in;
    int m_pipe_out;

    sem_t m_cmd_lock;

    int m_async_pipe[2];

    omx_event_queue m_ftb_q;
    omx_event_queue m_cmd_q;
    omx_event_queue m_etb_q;

}OMX_COMPONENT_PRIVATE;

/*========================= bit operation functions =================================*/
static inline void bit_set(OMX_U32 *addr, OMX_U32 nr)
{
    addr[nr / BITS_PER_LONG] |= (1 << (nr % BITS_PER_LONG));
}

static inline void bit_clear(OMX_U32 *addr, OMX_U32 nr)
{
    addr[nr / BITS_PER_LONG] &= ~((OMX_U32)(1 << (nr % BITS_PER_LONG)));
}

static inline OMX_S32 bit_present(const OMX_U32 *addr, OMX_U32 nr)
{
    return ((1 << (nr % BITS_PER_LONG)) & (((OMX_U32 *)addr)[nr / BITS_PER_LONG])) != 0;
}

static inline OMX_S32 bit_absent(const OMX_U32 *addr, OMX_U32 nr)
{
    return !bit_present(addr, nr);
}


#endif // __OMX_VDEC_H__
