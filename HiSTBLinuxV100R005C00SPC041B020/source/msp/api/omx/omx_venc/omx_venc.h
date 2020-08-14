/*========================================================================
Open MAX   Component: Video Encoder
This module contains the class definition for openMAX decoder component.
==========================================================================*/
#ifndef __OMX_VENC_H__
#define __OMX_VENC_H__

//khrons standard head file
#include "OMX_Component.h"
#include "OMX_Video.h"
//#include "OMX_Custom.h"


#include "OMX_Types.h"

#include "hi_drv_venc.h"

#include "omx_allocator.h"
#include "omx_venc_queue.h"
#include "omx_venc_drv.h"

//specific-version NO.1.1
#define KHRONOS_1_1                                  //??
#define OMX_SPEC_VERSION			0x00000101       //°æ±¾ºÅºê¶¨Òå

#define MAX_PORT_NUM				2

#define MAX_BUF_NUM_INPUT			6//30
#define MAX_BUF_NUM_OUTPUT			20//30

#define MAX_FRAME_WIDTH				1920
#define MAX_FRAME_HEIGHT			1088

#define DEFAULT_FPS				    30
#define DEFAULT_BITRATE             (5*1024*1024)
#define DEFAULT_ALIGN_SIZE			4096                      //?? ´«ËµÖĞµÄ4K±ß½ç±ß½ç?

#define DEF_MAX_IN_BUF_CNT			1  //3
#define DEF_MIN_IN_BUF_CNT			1

#define DEF_MAX_OUT_BUF_CNT		    2
#define DEF_MIN_OUT_BUF_CNT		    1   //6

#define DEF_OUT_BUFFER_SIZE         (1000000)

#define ENABLE_BUFFER_LOCK
/**
* A pointer to this struct is passed to the OMX_SetParameter when the extension
* index for the 'OMX.google.android.index.enableAndroidNativeBuffers' extension
* is given.
* The corresponding extension Index is OMX_GoogleIndexUseAndroidNativeBuffer.
* This will be used to inform OMX about the presence of gralloc pointers
instead
* of virtual pointers
*/

typedef struct StoreMetaDataInBuffersParams
{
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_U32 nPortIndex;
    OMX_BOOL bStoreMetaData;
} StoreMetaDataInBuffersParams;

struct PrependSPSPPSToIDRFramesParams {
    OMX_U32 nSize;
    OMX_VERSIONTYPE nVersion;
    OMX_BOOL bEnable;
};

typedef struct AutoRequestIFrmParams
{
    OMX_U32 nSize;
    OMX_U32 nRes;
    OMX_BOOL bEnable;
} AutoRequestIFrmParams;
////////////////////////////////////////////////////////////////////////// add end
#define BITS_PER_LONG				32

//#define ESHUTDOWN					108
//#define ERESTARTSYS					512

/*ÉèÖÃÊäÈë²ÎÊıÖĞµÄ °æ±¾ºÅºÍ½á¹¹´óĞ¡*/
#define CONFIG_VERSION_SIZE(pParam,Type)  do{\
        pParam->nVersion.nVersion = OMX_SPEC_VERSION;\
        pParam->nSize             = sizeof(Type);\
    }while(0)


#define COUNTOF(x) (sizeof(x)/sizeof(x[0]))                  //Ëã³öÊı×éÖĞº¬ÓĞµÄÔªËØ¸öÊı


#define OMX_CHECK_ARG_RETURN(a)  do{if ((a)){DEBUG_PRINT_ERROR("[%s],line: %d", __func__,__LINE__);return OMX_ErrorBadParameter;}}while(0)

#define ALIGN_UP(val, align) ( (val+((align)-1))&~((align)-1) )

#define FRAME_SIZE(w , h)  (((w) * (h) * 2))  //(((w) * (h) * 3) / 2)

// state transactions pending bits
#if 0
enum                                     /*ÔÚcomponentË½ÓĞ½á¹¹ÖĞ£¬×÷Îª¸÷ÖÖ´¦ÀíµÄ±êÖ¾£¬´¦ÀíÍê¸ÃÊÂ¼şºó£¬°Ñ¸ÃbitÎ»ÖÃ»Ø0*/
{
    OMX_STATE_IDLE_PENDING				= 0x1,               //idle(¿ÕÏĞ)µÈ´ı±êÖ¾,¸Ã±êÖ¾Îª1Ê±£¬ÔÚÏàÓ¦ÊÂ¼ş´¦Àíº¯ÊıÖĞ´¦Àí¸ÃµÈ´ıµÄ²Ù×÷£¬´¦ÀíÍêºó°Ñ¸ÃÎ» ÖÃ»Ø0
    OMX_STATE_LOADING_PENDING			= 0x2,
    OMX_STATE_INPUT_ENABLE_PENDING		= 0x3,
    OMX_STATE_OUTPUT_ENABLE_PENDING		= 0x4,
    OMX_STATE_OUTPUT_DISABLE_PENDING	= 0x5,
    OMX_STATE_INPUT_DISABLE_PENDING		= 0x6,
    OMX_STATE_OUTPUT_FLUSH_PENDING		= 0x7,
    OMX_STATE_INPUT_FLUSH_PENDING		= 0x8,
    OMX_STATE_PAUSE_PENDING				= 0x9,
    OMX_STATE_EXECUTE_PENDING			= 0xA
};
#endif

// Deferred callback identifiers
/*enum {                                                        //ÓÃÓÚ±íÊ¾¸÷¸ö CMD ÀàĞÍ,ÒÔ±ãÔÚ²åÈë¶ÓÁĞµÄÊ±ºòÅĞ¶ÏÒª°ÑÏûÏ¢µÄ´¦ÀíÊÇÒª²åÈëÄÄ¸ö¶ÓÁĞ
	OMX_GENERATE_COMMAND_DONE			= 0x1,
	OMX_GENERATE_FTB					= 0x2,
	OMX_GENERATE_ETB					= 0x3,
	OMX_GENERATE_COMMAND				= 0x4,
	OMX_GENERATE_EBD					= 0x5,
	OMX_GENERATE_FBD					= 0x6,
	OMX_GENERATE_FLUSH_INPUT_DONE		= 0x7,
	OMX_GENERATE_FLUSH_OUTPUT_DONE		= 0x8,
	OMX_GENERATE_START_DONE				= 0x9,
	OMX_GENERATE_PAUSE_DONE				= 0xA,
	OMX_GENERATE_RESUME_DONE			= 0xB,
	OMX_GENERATE_STOP_DONE				= 0xC,
	OMX_GENERATE_EOS_DONE				= 0xD,
	OMX_GENERATE_HARDWARE_ERROR			= 0xE,
	OMX_GENERATE_IMAGE_SIZE_CHANGE		= 0xF,
	OMX_GENERATE_CROP_RECT_CHANGE		= 0x10,
	OMX_GENERATE_UNUSED					= 0x11
};*/

enum
{
    INPUT_PORT_INDEX	= 0,
    OUTPUT_PORT_INDEX	= 1
};

struct port_property                                           //×÷Îª¶Ë¿ÚË½ÓĞ½á¹¹µÄ³ÉÔ±±äÁ¿Ö®Ò»,ÃèÊöÁË¶Ë¿ÚÌØĞÔ
{
    OMX_U32 port_dir;                                     //¶Ë¿ÚµÄ·½Ïò:in /out
    OMX_U32 min_count;                                    //¶ÔÓ¦ÓÚ±ê×¼OMX_PARAM_PORTDEFINITIONTYPEÀàĞÍ¶¨ÒåÖĞµÄnBufferCountMin±äÁ¿£¬±êÊ¶The minimum number of buffers this port requires
    OMX_U32 max_count;                                    //¶ÔÓ¦ÓÚ±ê×¼OMX_PARAM_PORTDEFINITIONTYPEÀàĞÍ¶¨ÒåÖĞµÄnBufferCountActual±äÁ¿£¬±êÊ¶The actual number of buffers allocated on this port
    OMX_U32 buffer_size;                                  //¶ÔÓ¦ÓÚ±ê×¼OMX_PARAM_PORTDEFINITIONTYPEÀàĞÍ¶¨ÒåÖĞµÄnBufferSize£¬±êÊ¶Size, in bytes, for buffers to be used for this channel
    OMX_U32 alignment;                                    //ÓÃÓÚÉêÇëÄÚ´æÖ®ÀàµÄ¶¯×÷!!4K¶ÔÆë
};

/*struct omx_hisi_extern_index
{
	OMX_S8 index_name[OMX_MAX_STRINGNAME_SIZE];
	OMX_HISI_EXTERN_INDEXTYPE index_type;
};*/

struct codec_info                                              //±àÂëÆ÷Ğ­ÒéÀàĞÍ½á¹¹Ìå,¾ßÌå°üº¬:
{
    const OMX_STRING role_name;                                     //±àÂëÀàĞÍÃû³Æ(ucharĞÍÊı×é)
    OMX_VIDEO_CODINGTYPE compress_fmt;                         //OMX±ê×¼Í·ÎÄ¼şomx_video.hÖĞ¶¨ÒåµÄÊÓÆµĞ­ÒéÃ¶¾ÙÀàĞÍ,×îÖÕ¸³Öµ¸øOMX_COMPONENT_PRIVATEÀàĞÍµÄm_dec_fmt£¬OMX_VIDEO_PARAM_PORTFORMATTYPE»òOMX_VIDEO_PORTDEFINITIONTYPEÀàĞÍÖĞµÄeCompressionFormat
    //enum venc_codec_type codec_type;                           //OMX±ê×¼Í·ÎÄ¼şomx_video.hÖĞ¶¨ÒåµÄÊÓÆµĞ­ÒéÃ¶¾ÙÀàĞÍ,×îÖÕ¸³Öµ¸øOMX_COMPONENT_PRIVATEÀàĞÍµÄdrv_ctx.venc_chan_attr[VENC_MAX_CHN].chan_cfg.protocol,¾ßÌå¶¨ÒåÔÚhisi_venc.h£¬Ó¦ÓëÔ­À´hi_unf_common.hÖĞ¶¨ÒåÒ»ÖÂ
    HI_UNF_VCODEC_TYPE_E codec_type;
};

struct codec_profile_level                                   //±àÂë²»ĞèÒª¿ ÔİÊ±±£Áô
{
    OMX_S32 profile;
    OMX_S32 level;
};/**/


struct frame_info                                            //ÒÑ°üº¬ÔÚ±àÂëÍ¨µÀÊôĞÔÖĞ
{
    OMX_U32 frame_width;
    OMX_U32 frame_height;
    OMX_U32 stride;
    //OMX_U32 scan_lines;
    //OMX_U32 crop_top;
    //OMX_U32 crop_left;
    //OMX_U32 crop_width;
    //OMX_U32 crop_height;
};/**/

// port private for omx
typedef struct OMX_PORT_PRIVATE                                 //¶Ë¿ÚµÄË½ÓĞ½á¹¹Ìå£¬×÷Îª×é¼şµÄË½ÓĞ½á¹¹ÌåµÄ³ÉÔ±Ö®Ò»£¬ÔÚOMX_COMPONENT_PRIVATEÀàĞÍÖĞ£¬¶ÔÓ¦ÊäÈë/Êä³ö¶Ë¿Ú¸÷ÓĞÒ»Ì×
{
    OMX_BUFFERHEADERTYPE** m_omx_bufhead;                       //¶ÔÓ¦OMX±ê×¼¶¨Òå£¬ÓÃÓÚ´æ·Å¸Ã¶Ë¿ÚÒª´¦ÀíµÄbuffer
    //struct port_property port_pro;                              //¶Ë¿ÚÌØĞÔ½á¹¹Ìå£¬ÆäÖĞ°üº¬µÄÄÚÈİ¶àÔÚ±ê×¼µÄ¶Ë¿ÚÀàĞÍOMX_PARAM_PORTDEFINITIONTYPEÖĞ¶ÔÓ¦

    OMX_PARAM_PORTDEFINITIONTYPE port_def;
    //OMX_U32	m_port_index;                                       //¶Ë¿ÚIDË÷ÒıºÅ   ÔÚOMX_PARAM_PORTDEFINITIONTYPEÄÇÀïÓĞ¶¨Òå£»

    OMX_U32 m_buf_cnt;                                          //Ïàµ±ÓÚ±êÊ¶¶Ë¿ÚµÚi¸öbufferÓĞÎŞÊı¾İµÄflag£¬m_buf_cntµÄÃ¿Ò»Î»Îª1£¬Ôò¸ÃÎ»¶ÔÓ¦µÄĞòºÅµÄbufferÓĞÊı¾İ£¬¿ÉÓÃÓÚÅĞ¶Ïport buffer µÄ¿ÕÂú£¬ÕÒ¿ÉĞ´ÈëµÄ¿ÕÏĞbufferµÈ²Ù×÷

    OMX_U32 m_usage_bitmap;                                     //ÓëÉÏÊöm_buf_cnt£¬ÓÃ·¨ÏàËÆ£¬Ö»ÊÇËüÓÃÓÚ mark buffer to be allocated(±êÊ¶×Ô¼ºÉêÇëµ½µÄbuffer£¬·½±ãÒÔºó×öfree²Ù×÷)
    OMX_U32	m_buf_pend_cnt;                                     //¼ÇÂ¼´ıÓÃ»§´¦ÀíµÄbufferÊıÄ¿(Êµ¼ÊÉÏ×é¼şÄÚ²¿ÒÑ¾­´¦ÀíÍê³É£¬Ö»ÊÇµÈ´ı¸æÖªÓÃ»§)~~Èç:fill_this_buffer_porxyÊ±++,ÔÚfill_this_buffer_doneÊ±--.

    //OMX_U32	m_port_enabled;                                     //¶Ë¿ÚÊ¹ÄÜ±êÖ¾Î»£¬Ò²¶ÔÓ¦ÓÚ¶Ë¿Ú±ê×¼¶¨ÒåOMX_PARAM_PORTDEFINITIONTYPEÖĞµÄbEnabledÎ»
    //OMX_U32	m_port_populated;                                   //¶Ë¿Ú³äÂú(?)±êÖ¾Î»£¬Ò²¶ÔÓ¦ÓÚ¶Ë¿Ú±ê×¼¶¨ÒåOMX_PARAM_PORTDEFINITIONTYPEÖĞµÄbPopulatedÎ»£¬¾ßÌåÃèÊö¼ûbPopulatedµÄ×¢ÊÍ¡¾±íÊ¾¶Ë¿ÚËùÉêÇëµÄbufferÊıÄ¿ÒÑ¾­´ïµ½×î´ó¡¿

    OMX_BOOL m_port_reconfig;                                   //¶Ë¿ÚÖØĞÂÅäÖÃ±êÖ¾Î»£¬Ä¬ÈÏÎªfalse£¬µ±´¦ÀíOMX_GENERATE_IMAGE_SIZE_CHANGEÃüÁîÊÂ¼şÊ±£¬°Ñ¸Ã±êÊ¶ÖÃ1£¬´ËÊ±²»ÄÜ½øĞĞfill/empty_this_bufferµÈ²Ù×÷£¬µÈµ½½Óµ½COMMON_DONE¼°OMX_CommandPortEnableµÄÊÂ¼ş´¦ÀíÊ±²Å»á°ÑËüÖÃ»Ø0
    OMX_BOOL m_port_flushing;                                   //¶Ë¿ÚbufferË¢ĞÂ±êÖ¾Î»£¬Ä¬ÈÏÎªfalse£¬µ±ÊÕµ½OMX_CommandFlushÃüÁîÊ±µ÷ÓÃhandle_command_flushº¯Êı´¦Àí£¬°Ñ¸Ã±êÊ¶ÖÃ1£¬´ËÊ±²»ÄÜ½øĞĞfill/empty_this_bufferµÈ²Ù×÷£¬µÈµ½½Óµ½OMX_GENERATE_FLUSH_INPUT_DONEµÄÊÂ¼ş´¦ÀíÊ±²Å»á°ÑËüÖÃ»Ø0


    //struct frame_info pic_info;               //ÔÚvenc_driver_contextÊı¾İÀàĞÍÖĞÓĞ¶¨Òå±àÂë¿í¶È¸ß¶ÈĞÅÏ¢£¬¿É·ñ¸´ÓÃ?   (Ä¿Ç°Ïàµ±ÓÚÊäÈëµÄÖ¡ĞÅÏ¢)

    venc_user_info** m_venc_bufhead;                 //??ÓÃÓÚ¶Ô±ê×¼m_omx_bufheadµÄ²¹³äbuffer??

} OMX_PORT_PRIVATE;

//component private for omx                                     //×é¼şµÄË½ÓĞ½á¹¹Ìå£¬Êµ¼ÊÓ¦ÓÃ¾ø´ó²¿·Ö¶¼ÊÇÕâ¸ö½á¹¹Ìå£¬ÆäÖĞ°üº¬±ê×¼¶¨ÒåÖĞµÄOMX_COMPONENTTYPEÀàĞÍÊı¾İÖ¸Õë
typedef struct OMX_COMPONENT_PRIVATE
{
    pthread_mutex_t m_lock;                                     //»¥³âÁ¿£¬ÔÚ¶Ô¹²Ïí×ÊÔ´µÄ¶ÁĞ´²Ù×÷Ê±£¬Ò»¶¨ÒªÓÃ»¥³âÁ¿¼ÓÒÔ±£»¤£¬ÕâÀïµÄ¹²Ïí×ÊÔ´Ö÷ÒªÊÇ:Èı¸ö¶ÓÁĞ£¬¹ÜµÀ¶ÁĞ´¶Ë
    OMX_COMPONENTTYPE* m_pcomp;                                 //×é¼ş½á¹¹Ìå
    OMX_STATETYPE m_state;                                      //×é¼ş×´Ì¬
    OMX_U32 m_flags;                                            //Ã¿Ò»Î»¶ÔÓ¦ÁËflags_bit_positionsÖĞ¶¨ÒåµÄÊÂ¼ş
    OMX_VIDEO_CODINGTYPE m_encoder_fmt;                         //Óëcodec_info¶ÔÓ¦
    OMX_S8 m_role[OMX_MAX_STRINGNAME_SIZE];                     //±àÂëÀàĞÍÃû×Ö±êÊ¶
    OMX_S8 m_comp_name[OMX_MAX_STRINGNAME_SIZE];                //±àÂëÆ÷Ãû×ÖÖ¸Õë
    OMX_PTR m_app_data;                                         //ÓÉÍâ²¿Ó¦ÓÃ³ÌĞòAPP´«½øÀ´µÄÒ»¸ö²ÎÊı£¬ÓÃÓÚ¸øAPPÇø·Ö²»Í¬µÄcomponent£¬ÔÚÓ¦ÓÃ³ÌĞòµ÷ÓÃOMX_GetHandleÊ±Óë»Øµ÷º¯Êı½á¹¹ÌåÖ¸ÕëÒ»Æğ´«Èë
    OMX_CALLBACKTYPE m_cb;                                      //ÓÉÍâ²¿Ó¦ÓÃ³ÌĞòAPP´«½øÀ´»Øµ÷º¯ÊıÖ¸Õë£¬ÔÚÓ¦ÓÃ³ÌĞòµ÷ÓÃOMX_GetHandleÊ±ÓëÉÏÊöm_app_data»Øµ÷º¯Êı½á¹¹ÌåÖ¸ÕëÒ»Æğ´«Èë

    OMX_TICKS m_pre_timestamp;                                  //pts Ê±¼ä´Á

    OMX_PORT_PRIVATE m_port[MAX_PORT_NUM];                      //portµÄË½ÓĞ½á¹¹
    OMX_BOOL m_use_native_buf;                                  //ÓÃ×ÔÉíbuffer±êÊ¶£¬ÔÚget/set_parameterµÄ OMX_GoogleIndexGetAndroidNativeBufferUsage  ÅĞ¶Ï·ÖÖ§ÖĞÓ¦ÓÃ

    OMX_BOOL m_store_metadata_buf;                              //ÓÃ×ÔÉíbuffer±êÊ¶£¬ÔÚget/set_parameterµÄ OMX_GoogleIndexStoreMetaDataInBuffers  ÅĞ¶Ï·ÖÖ§ÖĞÓ¦ÓÃ
    OMX_BOOL m_prepend_sps_pps;                                 //ÓÃÓÚ±êÊ¾µ±Ç°ÊÇ·ñÖ§³ÖÔÚÃ¿¸öIÖ¡Ç°¶¼·¢SPS/PPSÊı¾İ
    pthread_t msg_thread_id;                                    //ÏûÏ¢Ïß³Ì     ->write
    pthread_t event_thread_id;                                  //ÊÂ¼ş´¦ÀíÏß³Ì ->read

    volatile OMX_BOOL event_thread_exit;                                 //ÊÂ¼ş´¦ÀíÏß³ÌÍË³ö±êÖ¾
    volatile OMX_BOOL msg_thread_exit;                                   ////ÏûÏ¢Ïß³ÌÍË³ö±êÖ¾


    venc_drv_context drv_ctx;         //×÷ÎªComponentµÄË½ÓĞ½á¹¹£¬¶¨ÓÚÔÚomx_venc_drv.hÖĞ°üº¬±àÂëÆ÷Éè±¸ÎÄ¼ş±êÊ¶·ûºÅ¡¢±àÂëÆ÷Í¨µÀÊôĞÔ
    OMX_S32 m_pipe_in;                              //read this pipe
    OMX_S32 m_pipe_out;                             //write this pipe

    sem_t m_async_sem;                          //Òì²½ĞÅºÅÁ¿ £¬ÓÃÓÚ¶ÁÈ¡Í¨µÀÊ±¼ÓĞÅºÅÁ¿±£»¤
    sem_t m_cmd_lock;                           //ÃüÁîĞÅºÅÁ¿ (Ïàµ±ÓÚËø) ??
    #ifdef ENABLE_BUFFER_LOCK
    sem_t m_buf_lock;
    #endif
    omx_event_queue m_ftb_q;                    //Êä³öÊı¾İ¶ÓÁĞ           /*×¢Òâ:Ã¿´ÎĞ´Íê¶ÓÁĞÖ®ºó¶Ô¶¼»á¶Ô¹ÜµÀ½øĞĞÏàÓ¦µÄĞ´£¬ÔÚÊÂ¼ş´¦ÀíÏß³ÌÖĞ¶Ô¹ÜµÀ½øĞĞ¶Á²Ù×÷£¬¶Á³öÏûÏ¢´¦ÀíÍêºó±ã³ö¶ªÁĞ*/
    omx_event_queue m_cmd_q;                    //ÃüÁî¶ÓÁĞ
    omx_event_queue m_etb_q;                    //ÊäÈëÔ´¶ÓÁĞ
    void  *bufferaddr_RGB;     //ĞéÄâµØÖ·
    HI_U32 bufferaddr_Phy_RGB;
	HI_U32 buffer_size_RGB;    //buffer alloc size

    HI_HANDLE hTDE;
    //allocator_handle_t allocator_handle;        //·ÖÅäÄÚ´æ£¬ÓÃµ½IOHÉè±¸Ê±£¬ÓÃÓÚ¼ÇÂ¼ION¾ä±ú~
} OMX_COMPONENT_PRIVATE;

/*==========================================================================*/
// bit operation functions
static inline void bit_set(OMX_U32* addr, OMX_U32 nr)                          // ¶ÔÊı¾İµÄÄ³Ò»Î»ÉèÖÃÎª 1 (¸Ãº¯Êı¶Ô64Î»Í¬ÑùÓĞĞ§)
{
    addr[nr / BITS_PER_LONG] |= (1 << (nr % BITS_PER_LONG));
}

static inline void bit_clear(OMX_U32* addr, OMX_U32 nr)                         // ¶ÔÊı¾İµÄÄ³Ò»Î»ÉèÖÃÎªÇå0 (¸Ãº¯Êı¶Ô64Î»Í¬ÑùÓĞĞ§)
{
    addr[nr / BITS_PER_LONG] &= ~((OMX_U32)(1 << (nr % BITS_PER_LONG)));
}

static inline OMX_S32 bit_present(const OMX_U32* addr, OMX_U32 nr)         // ÅĞ¶ÏÊı¾İÄ³Ò»Î»ÊÇ·ñÎª 1;Èç¹ûÎª1,Ôò·µ»Ø1£¬·ñÔò·µ»Ø0 (¸Ãº¯Êı¶Ô64Î»Í¬ÑùÓĞĞ§)
{
    return ((1 << (nr % BITS_PER_LONG)) &
            (((OMX_U32*)addr)[nr / BITS_PER_LONG])) != 0;
}

static inline OMX_S32 bit_absent(const OMX_U32* addr, OMX_U32 nr)          // ÅĞ¶ÏÊı¾İÄ³Ò»Î»ÊÇ·ñÎª 0£»Èç¹ûÎª0,Ôò·µ»Ø1£¬·ñÔò·µ»Ø0
{
    return !bit_present(addr, nr);
}

#endif // __OMX_VENC_H__
