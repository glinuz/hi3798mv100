
/***********************************************************************
*
* Copyright (c) 2006 HUAWEI - All Rights Reserved
*
* File: $public.h$
* Date: $2006/11/30$
* Revision: $v1.0$
* Purpose: header file for interface & interface & storage management
*          module. this file only define internal information. the
*          interface related data is defined in 'vfmw.h'
*
*
* Change History:
*
* Date             Author            Change
* ====             ======            ======
* 2006/11/30       z56361            Original
*
*
* Dependencies:
*
************************************************************************/
#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#include "basedef.h"
#include "sysconfig.h"
#include "vfmw.h"
#include "scd_drv.h"
#include "vdm_hal.h"

#include "vfmw_osal_ext.h"

extern Vfmw_Osal_Func_Ptr vfmw_Osal_Func_Ptr_S;

/***********************************************************************
      VFMW的版本号
 ***********************************************************************/
#define  VFMW_VERSION_NUM       (2016081000)

/***********************************************************************
      constants
 ***********************************************************************/
#ifdef VFMW_BVT_SUPPORT
#define  SCD_SEG_BLANK_LEN      (4*1024)
#else
#define  SCD_SEG_BLANK_LEN      (128*1024)
#endif

#define  MIN_BOTTOM_SEG_LEN     (1*1024) //SCD_SEG_BLANK_LEN  //128K太大了吧，这么多码流拷贝一旦发生会耗时很长，给互斥实现带来麻烦
#define  SCD_AVS_SEG_BLANK_LEN  (1024*1024)
#define  MIN_BOTTOM_AVS_SEG_LEN (1023*1024)
#define  SM_MAX_SEG_BUF_SIZE    (2*1024*1024)
// 20130911: l232354, BVT 可能设置的最小SCD SEG BUF 为 32*1024，所以得更新相关判断条件
#ifdef VFMW_BVT_SUPPORT
#define  SM_MIN_SEG_BUF_SIZE    (32*1024)
#else
#define  SM_MIN_SEG_BUF_SIZE    (256*1024)
#endif

#define  VFMW_TRUE    (0)
#define  VFMW_FALSE   (1)

// debug message print control
#define  DEFAULT_PRINT_ENABLE   (0x0)          //(1<< PRN_DNR)//(1<<PRN_VDMREG)|(1<<PRN_DNMSG) //0x60 //0x0  //0x3B // 默认 'PRN_CTRL' & 'PRN_FATAL' 打开，其余关闭
#define  DEFAULT_PRINT_DEVICE   (DEV_SCREEN)   // 默认 'DEV_SCREEN'，即打印到屏幕

#define  MAX_USERDATA_NUM       (4)            // 最多支持4个，需保持与IMAGE结构体对应
/***********************************************************************
      enum
 ***********************************************************************/
typedef enum
{
    DEV_SCREEN = 1,
    DEV_SYSLOG,
    DEV_FILE,
    DEV_MEM
} PRINT_DEVICE_TYPE;

typedef enum
{
    PRN_FATAL = 0,       // 0.  致命异常（fatal error），比如未知异常，VDM挂死等信息
    PRN_ERROR,           // 1.  一般异常（error），比如语法错误
    PRN_CTRL,            // 2.  控制信息, 比如状态灯，内部状态机跳转信息等
    PRN_VDMREG,          // 3.  VDM寄存器，包括控制寄存器、状态寄存器

    PRN_DNMSG,           // 4.  解码下行消息，各消息字的值打印
    PRN_RPMSG,           // 5.  修补消息，修补消息各个消息字
    PRN_UPMSG,           // 6.  上行消息，上行消息各个消息字
    PRN_STREAM,          // 7.  码流信息，如每个码流包的信息（地址，长度等）

    PRN_STR_HEAD,        // 8.  码流头字节，（前8字节，用于查找比对）
    PRN_STR_TAIL,        // 9.  码流尾字节，（后8字节，用于查找比对）
    PRN_STR_BODY,        // 10. 码流中部字节，（前后8字节之外的码流）
    PRN_IMAGE,           // 11. 解码图像信息（图像的id，地址，属性等）

    PRN_QUEUE,           // 12. 解码图队列信息（队列的图像个数，位置等）
    PRN_REF,             // 13. 参考图信息（H264打印LIST相关信息，MPEG打印参考图及其维护的信息）
    PRN_DPB,             // 14. DPB类信息（与dpb相关的各种信息）
    PRN_POC,             // 15. POC类信息（POC的解码和计算）

    PRN_MARK_MMCO,       // 16. MARK & MMCO操作信息（MARK和MMCO操作的信息，可表现执行的过程）
    PRN_SEQ,             // 17. 序列级重要语法信息，H264打印SPS，MPEG2打印sequence信息，MPEG4打印VOL及以上层信息
    PRN_PIC,             // 18. 图像级重要语法信息，H264打印PPS，MPEG2打印picture，MPEG4打印VOP/SHVOP头
    PRN_SLICE,           // 19. slice级重要语法信息：仅H264有意义

    PRN_SEI,             // 20. SEI信息，仅H264有意义
    PRN_SE,              // 21. H264语法信息,仅H264有意义
    PRN_DBG,             // 22. 调试信息
    PRN_BLOCK,           // 23. 解码线程阻塞信息

    PRN_SCD_REGMSG,      // 24.
    PRN_SCD_STREAM,      // 25.
    PRN_SCD_INFO,        // 26. 打印SCD一些运行状态的信息
    PRN_INT_TIME,        // 27. VDH中断响应时间

    PRN_DNR,             // 28. 打印DNR及其驱动程序相关信息
    PRN_PTS,             // 29. 打印与PTS处理相关的信息
    PRN_DEC_MODE,        // 30. 解码模式切换信息
    PRN_FS,              // 31. 动态帧存打印l00273086
    PRN_ALWS = 32        // 32. 不受控打印
} PRINT_MSG_TYPE;

/* state type */
typedef enum
{
    STATE_RCV_RAW = 0,           //0. 接收RAW码流
    STATE_SCD_START,             //1. 启动SCD
    STATE_SCD_INTERRUPT,        //2. SCD中断

    STATE_DECSYNTAX_SEG,        //3. 语法解析SEG码流
    STATE_GENERATE_DECPARAM,    //4. 生成解码参数

    STATE_VDH_START,             //5. 启动VDH
    STATE_VDH_INTERRUPT,         //6. VDH中断
    STATE_VDH_REPAIR,            //7. VDH修补
    STATE_1D_TO_QUEUE,           //8. 1D图像输出到队列

    STATE_DNR_START,             //9. 启动DNR
    STATE_DNR_INTERRUPT,         //10. DNR中断
    STATE_2D_TO_QUEUE,           //11. 2D图像输出到队列

    STATE_VO_RCV_IMG,            //12. VO读取图像
    STATE_VO_REL_IMG             //13. VO释放图像
} VFMW_STATE_TYPE_E;

//#define    SAVE_PIC_YUV       0x1
/***********************************************************************
      structures
 ***********************************************************************/

/* MPEG2, MPEG4 and DIVX311 dedode interface need code stream info from control module.
   Struct 'STREAM_PARAM' defines the stream info format. */
typedef struct
{
    UINT8    *VirAddr;    // 码流的虚拟地址
    UADDR     PhyAddr;    // 码流的物理地址
    SINT32    Length;     // 码流长度（字节数）
} STREAM_PARAM;

/* MPEG2/4的码流暂存缓冲 */
typedef struct
{
    UINT8    *buf_vir_addr;
    SINT8     filled_new_data_flag;
    UADDR     buf_phy_addr;
    SINT32    buf_size;
    SINT32    length;
} STRM_LOCAL_BUF;

/* VDM 驱动的信息汇总 */
typedef struct
{
    SINT32    DecTaskState;
} VDMCMN_DECTASK_INFO_S;


/* 低延迟各模块时间统计信息 */
typedef struct
{
    SINT32    lowdly_enable;             /* 低延迟使能标志 */
    SINT32    chan_id;                   /* 通道 ID */
    SINT32    receive_raw;               /* 记录收到 RAW 包时间 */
    SINT32    scd_start;                 /* SCD 启动时间 */
    SINT32    scd_return;                /* SCD 中断返回 */
    SINT32    scd_count;                 /* SCD 工作一次所消耗的时间 */
    SINT32    vdh_start;                 /* VDH 启动时间 */
    SINT32    vdh_return;                /* VDH 中断返回 */
    SINT32    vdh_count;                 /* VDH 工作一次所消耗的时间 */
    SINT32    vo_read_img;               /* VO 成功读走图像的时间 */
    SINT32    raw_to_scd_return;         /* 从VFMW收到码流到解出一帧图像的时间, 只有按帧送码流时才较为准确 */
    SINT32    raw_to_img;                /* 从VFMW收到码流到解出一帧图像的时间, 只有按帧送码流时才较为准确 */
    SINT32    raw_to_vo;                 /* 从VFMW收到码流到解出一帧图像的时间，只有按帧送码流时才较为准确 */
    SINT32    OneTimeFlag;
} LOWDLY_INFO_S;

typedef enum 
{
    VFMW_STRING_VDH_IRQ = 0,
    VFMW_STRING_VDH1_IRQ,
    VFMW_STRING_VDH2_IRQ,
    VFMW_STRING_SCD_IRQ,
    VFMW_STRING_SCD1_IRQ,
    VFMW_STRING_CHAN_CTX,
    VFMW_STRING_CHAN_VDH,
    VFMW_STRING_CHAN_SCD,
    VFMW_STRING_DSPCTXMEM,
    VFMW_STRING_DNR_IRQ,
    VFMW_STRING_BTL_IRQ,
    VFMW_STRING_VDH_HAL_MEM,
    VFMW_STRING_SCD_MSG_MEM,
    VFMW_STRING_RAW_SAVE_PATH,
    VFMW_STRING_SEG_SAVE_PATH,
    VFMW_STRING_YUV_SAVE_PATH,
    VFMW_STRING_YUV_2D_SAVE_NAME,
    VFMW_STRING_YUV_1D_SAVE_NAME,
    VFMW_STRING_TRACER,
    VFMW_STRING_BIGTILE_ID_YUV,
    VFMW_STRING_BUTT
}VFMW_STRING_E;

/***********************************************************************
      global vars
 ***********************************************************************/
//extern  VFMW_STATE        g_VfmwState[2];
//extern  VFMW_STATE        *g_pVfmwState;

extern  UINT32            g_StateBkup_008;   // VDM_STATE

extern  SINT32            g_LogEnable;

extern  UINT32            g_ScreenPrint;
extern  UINT32            g_WriteLog;
extern  UINT32            g_PrintEnable;
extern  UINT32            g_PrintDevice;
extern  SINT32            (*g_StateHandler)( SINT32 type, UINT8 *p_args, SINT32 arg_len );
extern  UINT32            g_RunState;

extern  UINT32            g_LastActiveTime;

extern  SINT32            (*g_event_report)( SINT32 InstID, SINT32 type, VOID *p_args, UINT32 len );

extern  SINT32            g_TraceCtrl;
extern  SINT32            g_TraceBsPeriod;
extern  SINT32            g_TraceFramePeriod;
extern  SINT32            g_TraceImgPeriod;

extern  LOWDLY_INFO_S     g_stLowdlyInfo[MAX_CHAN_NUM];

// register address offset from register base address
#define    REG_VDM_STATE                              (0x01C>>2)    // VDM_STATE
#define    REG_INTERPRT_CLEAN                         (0X020>>2)    // INT
#define    REG_INTERPRT_MASK                          (0x024>>2)    // INT_MASK


// map first. take care!!!!

// debug related state definition
#define    DSTATE_WAIT_STREAM             (1<<1)
#define    DSTATE_WAIT_VDMFINISH          (1<<2)
#define    DSTATE_WAIT_FRAMESTORE         (1<<3)
#define    DSTATE_WAIT_VDMPROPERTY        (1<<4)

/***********************************************************************
      macro
 ***********************************************************************/
//注意:有很多函数前面对空指针的判断就可以去掉了
//add for check null point parament
#define CHECK_NULL_PTR_ReturnValue(parPoint,ReturnValue)                        \
    do                                                  \
    {                                                   \
        if (NULL == parPoint)                           \
        {                                               \
            dprint(PRN_FATAL, "NULL pointer: %s, L%d\n", __FILE__, __LINE__);       \
            return ReturnValue;                 \
        }                                               \
    }while(0)

#define CHECK_NULL_PTR_Return(parPoint)                         \
    do                                                  \
    {                                                   \
        if (NULL == parPoint)                           \
        {                                               \
            dprint(PRN_FATAL,"NULL pointer: %s, L%d\n", __FILE__, __LINE__);       \
            return;                 \
        }                                               \
    }while(0)
//end

#if 0
#define DefaultMarkValue -1
//add for check array beyond the mark
//如果不需要DeflautMark将其设为-1
//如果数组定义为int array[65] 那么FirstMark = 0; LastMark = 64
#define CHECK_ARRAY_BEYOND_ReturnValue( FirstMark, LastMark, InputMark, DefaultMark, ReturnValue)       \
    do                                                 \
    {                                                   \
        if ((InputMark < FirstMark) || (InputMark > LastMark))                          \
        {                                               \
            DPRINT("Array mark beyond %s, L%d\n", __FUNCTION__, __LINE__);    \
            if (DefaultMark != -1)                      \
                InputMark = DefaultMark;                \
            return ReturnValue;                 \
        }                                               \
    }while(0)

#define CHECK_ARRAY_BEYOND_Return( FirstMark, LastMark, InputMark, DefaultMark )            \
    do                                                 \
    {                                                   \
        if ((InputMark < FirstMark) || (InputMark > LastMark))                          \
        {                                               \
            DPRINT("Array mark beyond %s, L%d\n", __FUNCTION__, __LINE__);    \
            if (DefaultMark != -1)                      \
                InputMark = DefaultMark;                \
            return;                 \
        }                                               \
    }while(0)
#endif
//end


/*********************** 调用外挂函数对调试状态进行处理的宏封装 *************************/
/* state=1	VFMW开始等待码流(读码流失败)	无参数   */
#define HANDLE_WAIT_STREAM()                                              \
    do{                                                                       \
    }while(0)

/* state=2	VFMW结束等待码流(得到码流)	无参数 */
#define HANDLE_GET_STREAM()                                               \
    do{                                                                       \
    }while(0)

/* state=3	VFMW开始等待VDM的操作权	无参数 */
#define HANDLE_WAIT_VDMPROPERTY()                                         \
    do{                                                                       \
    }while(0)

/* state=4	VFMW结束等待VDM操作权（已获得）	无参数 */
#define HANDLE_GET_VDMPROPERTY()                                          \
    do{                                                                       \
    }while(0)

/* state=5	VFMW开始等待VDM解码完成	无参数 */
#define HANDLE_WAIT_VDMFINISH()                                           \
    do{                                                                       \
    }while(0)

/* state=6	VFMW结束等待VDM解码完成（VDM已完成） 无参数 */
#define HANDLE_GET_VDMFINISH()                                            \
    do{                                                                       \
    }while(0)

/* state=7	VFMW开始等待空闲帧存  无参数 */
#define HANDLE_WAIT_FRAMESTORE()                                          \
    do{                                                                       \
    }while(0)

/* state=8	VFMW结束等待空闲帧存（已获得帧存）	无参数 */
#define HANDLE_GET_FRAMESTORE()                                           \
    do{                                                                       \
    }while(0)

/* state=100	输出调试信息	p_args: 调试字符串，arg_len: 字符串长度 */
#define HANDLE_OUT_STRING( str, len )                                     \
    do{                                                                       \
    }while(0)


/*********************** 发送消息的宏封装 *************************/

/* type=1	图像的显示区域发生了变化 */
#define REPORT_DISP_AREA_CHANGE(chan_id,w,h,center_x,center_y)  \
    do{                                                         \
        if( NULL != g_event_report )                         \
        {                                                       \
            UINT16  para[4];                                    \
            para[0] = (UINT16)(w);                              \
            para[1] = (UINT16)(h);                              \
            para[2] = (UINT16)(center_x);                      \
            para[3] = (UINT16)(center_y);                      \
            g_event_report( chan_id, EVNT_DISP_EREA, (UINT8*)para, sizeof(para));   \
        }                                                       \
    }while(0)

/* type=2, 图像宽高发生了变化 */
#define REPORT_IMGSIZE_CHANGE( chan_id, oldw, oldh, neww, newh )	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT16  para[4];                                    \
            para[0] = (UINT16)(oldw);                        	\
            para[1] = (UINT16)(oldh);                          	\
            para[2] = (UINT16)(neww);                          	\
            para[3] = (UINT16)(newh);                          	\
            g_event_report( chan_id, EVNT_IMG_SIZE_CHANGE, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=3, 帧率发生了变化 */
#define REPORT_FRMRATE_CHANGE( chan_id, newfr )    		\
    do{                                                      	\
        if( NULL != g_event_report )                 		\
        {                                                       \
            UINT32  para[1];                                    \
            para[0] = (UINT32)(newfr);                         	\
            g_event_report( chan_id, EVNT_FRMRATE_CHANGE, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=4, 图像的逐行/隔行信息变化，无参数 */
#define REPORT_SCAN_CHANGE( chan_id, newscan ) 			\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT8  para[1];                                     \
            para[0] = (UINT8)(newscan);                 		\
            g_event_report( chan_id, EVNT_SCAN_CHANGE, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=5, 有一帧图像进入队列 */
#define REPORT_IMGRDY( chan_id, pImage )            		\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_NEW_IMAGE, pImage, sizeof(IMAGE)); \
        }                                                       \
    }while(0)

/* type=6, 发现了user data */
#define REPORT_USRDAT( chan_id, p_usrdat )              	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_USRDAT, (VDEC_USRDAT_S*)p_usrdat, sizeof(VDEC_USRDAT_S));	 \
        }                                                       \
    }while(0)

/* type=7  发现宽高比发生变化 */
#define REPORT_ASPR_CHANGE( chan_id, oldasp, newasp )  	    \
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT8  para[2];                                     \
            para[0] = (UINT8)(oldasp);                        	\
            para[1] = (UINT8)(newasp);                       	\
            g_event_report( chan_id, EVNT_ASPR_CHANGE, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=8  即将输出一个尺寸与之前不同的帧 */
#define REPORT_OUTPUT_IMG_SIZE_CHANGE( chan_id, oldw, oldh, neww, newh )	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT16  para[4];                                    \
            para[0] = (UINT16)(oldw);                           \
            para[1] = (UINT16)(oldh);                           \
            para[2] = (UINT16)(neww);                           \
            para[3] = (UINT16)(newh);                           \
            g_event_report( chan_id, EVNT_OUTPUT_IMG_SIZE_CHANGE, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)


/* type=20  发现一个I帧，准备解码此I帧 */
#define REPORT_FIND_IFRAME( chan_id, stream_size )          \
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[1];                                    \
            para[0] = stream_size;                              \
            g_event_report( chan_id, EVNT_FIND_IFRAME, (UINT8*)para, sizeof(para));  \
        }                                                       \
    }while(0)
/* type=21  capture抓图完成 */
#define REPORT_CAPTURE_BTL_OVER( chan_id, pImage )          \
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            IMAGE*  para[1];                                    \
            para[0] = pImage;                              \
            g_event_report( chan_id, EVNT_CAPTURE_BTL_OVER, (IMAGE*)para, sizeof(IMAGE));  \
        }                                                       \
    }while(0)

/*--------------------------------------------------------------*/
/* type=100 发现码流错误 p_args[3..0]：错误编号*/
#define REPORT_STRMERR(chan_id, err_code )          		\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[1];                                    \
            para[0] = (UINT32)(err_code);                     	\
            g_event_report( chan_id, EVNT_STREAM_ERR, (UINT8*)para, sizeof(para));		\
        }                                                       \
    }while(0)

/* type=101 VDM不响应 */
#define REPORT_VDMERR(chan_id)                           	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_VDM_ERR, (UINT8*)0, 0); 	 \
        }                                                       \
    }while(0)

/* type=102 发现不支持的规格 无 */
#define REPORT_UNSUPPORT(chan_id)                       	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_UNSUPPORT, (UINT8*)0, 0);  	 \
        }                                                       \
    }while(0)

/* type=103	码流的语法错误	无 */
#define REPORT_SE_ERR(chan_id)                           	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_SE_ERR, (UINT8*)0, 0); 		 \
        }                                                       \
    }while(0)

/* type=104  图像错误率超过ref_error_thr	无 */
#define REPORT_OVER_REFTHR(chan_id, CurrRate, RefRate)      \
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            UINT32	para[2];									\
            para[0] = (UINT32)(CurrRate);						\
            para[1] = (UINT32)(RefRate);					    \
            g_event_report( chan_id, EVNT_OVER_REFTHR, (UINT8*)para, sizeof(para));  \
        }														\
    }while(0)

/* type=105  图像错误率超过out_error_thr	无 */
#define REPORT_OVER_OUTTHR(chan_id)                     	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            g_event_report( chan_id, EVNT_OVER_OUTTHR, (UINT8*)0, 0);  \
        }                                                       \
    }while(0)

/* type=106  参考帧个数超过了设定值 p_args[3..0]：参考帧个数， p_args[7..4]：设定值 */
#define REPORT_REF_NUM_OVER(chan_id, RefNum, MaxRefNum)    	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[2];                                    \
            para[0] = RefNum;                                   \
            para[1] = MaxRefNum;                                \
            g_event_report( chan_id, EVNT_REF_NUM_OVER, (UINT8*)para, sizeof(para));  \
        }                                                       \
    }while(0)

/* type=107, 图像宽高超过阈值 */
#define REPORT_IMGSIZE_OVER( chan_id, w, h, maxw, maxh )	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT16  para[4];                                    \
            para[0] = (UINT16)(w);                        	    \
            para[1] = (UINT16)(h);                          	\
            para[2] = (UINT16)(maxw);                          	\
            para[3] = (UINT16)(maxh);                          	\
            g_event_report( chan_id, EVNT_SIZE_OVER, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=108, slice个数超过阈值 */
#define REPORT_SLICE_NUM_OVER( chan_id, SliceNum, MaxSliceNum )	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[2];                                    \
            para[0] = SliceNum;                         	    \
            para[1] = MaxSliceNum;                          	\
            g_event_report( chan_id, EVNT_SLICE_NUM_OVER, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=109, sps个数超过阈值 */
#define REPORT_SPS_NUM_OVER( chan_id, SpsNum, MaxSpsNum )	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[2];                                    \
            para[0] = SpsNum;                           	    \
            para[1] = MaxSpsNum;                            	\
            g_event_report( chan_id, EVNT_SPS_NUM_OVER, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=110, pps个数超过阈值 */
#define REPORT_PPS_NUM_OVER( chan_id, PpsNum, MaxPpsNum )	\
    do{                                                         \
        if( NULL != g_event_report )                    		\
        {                                                       \
            UINT32  para[2];                                    \
            para[0] = PpsNum;                            	    \
            para[1] = MaxPpsNum;                             	\
            g_event_report( chan_id, EVNT_PPS_NUM_OVER, (UINT8*)para, sizeof(para)); \
        }                                                       \
    }while(0)

/* type=111  发现I帧解码错误    无 */
#define REPORT_IFRAME_ERR(chan_id)                     	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            g_event_report( chan_id, EVNT_IFRAME_ERR, (UINT8*)0, 0);  \
        }                                                       \
    }while(0)

/* type=112  发现新的帧结束符    无 */
#define REPORT_MEET_NEWFRM(chan_id)                     	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            g_event_report( chan_id, EVNT_MEET_NEWFRM, (UINT8*)0, 0);  \
        }                                                       \
    }while(0)

// y00226912
/* type=113  码流解析错误*/
#define REPORT_DECSYNTAX_ERR(chan_id)                     	\
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            g_event_report( chan_id, EVNT_DECSYNTAX_ERR, (UINT8*)0, 0);  \
        }														\
    }while(0)

// y00226912
/* type=114  错误率非零上报 */
#define REPORT_RATIO_NOTZERO(chan_id, ErrRatio)                           \
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            UINT32  para[1];                                    \
            para[0] = ErrRatio;                                   \
            g_event_report( chan_id, EVNT_RATIO_NOTZERO, (UINT8*)para, sizeof(para));  \
        }														\
    }while(0)

/* type=115  最后一帧输出结果上报 */
#define REPORT_LAST_FRAME(chan_id, flag)                     	\
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            UINT32  para[1];                                    \
            para[0] = (UINT32)(flag);                     	\
            g_event_report( chan_id, EVNT_LAST_FRAME, (UINT8*)para, sizeof(para));  \
        }														\
    }while(0)

/* type=116  变分辨率重新分割帧存上报 */
#define REPORT_RESOLUTION_CHANGE(chan_id)                   \
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            g_event_report( chan_id, EVNT_RESOLUTION_CHANGE, (UINT8*)0, 0);  \
        }														\
    }while(0)

/* type=117  码流结束符上报 */
#define REPORT_STREAM_END_SYNTAX(chan_id)                   \
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            g_event_report( chan_id, EVNT_STREAM_END_SYNTAX, (UINT8*)0, 0);  \
        }														\
    }while(0)

/* type=118 码流信息上报划分帧存*/ //l00273086
#define REPORT_NEED_ARRANGE(chan_id, FrmNum, FrmSize, Width, Height)                     	\
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            UINT32  para[4];                                    \
            para[0] = (UINT32)(FrmNum);                         \
            para[1] = (UINT32)(FrmSize);                        \
            para[2] = (UINT32)(Width);                          \
            para[3] = (UINT32)(Height);                         \
            g_event_report(chan_id, EVNT_NEED_ARRANGE, (UINT8*)para, sizeof(para));  \
        }														\
    }while(0)

/* type=119 发现不支持的规格 上报类型及数值 */
#define REPORT_UNSUPPORT_SPEC(chan_id, unsupport_spec, data)                       	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT32  para[2];                                    \
            para[0] = unsupport_spec;                           \
            para[1] = data;                                     \
            g_event_report( chan_id, EVNT_UNSUPPORT_SPEC, (UINT8*)para, sizeof(para));    \
        }                                                       \
    }while(0)

/* type=120  变分辨率时输出假帧让后级还帧 */
#define REPORT_FAKE_FRAME(chan_id)                     	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                   \
            g_event_report( chan_id, EVNT_FAKE_FRAME, (UINT8*)0, 0);  \
        }                                                       \
    }while(0)

#define REPORT_VID_STD_ERROR(chan_id)                       \
    do{ 														\
        if( NULL != g_event_report )							\
        {														\
            g_event_report( chan_id, EVNT_VIDSTD_ERROR, (UINT8*)0, 0);  \
        }														\
    }while(0)

#ifdef Hi3716MV410
/* type=120 上报码流信息分配帧存 */
#define REPORT_ARRANGE_FRAME_BUF(chan_id, FrmNum, FrmSize, Width, Height)                     	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT32  para[4];                                    \
            para[0] = (UINT32)(FrmNum);                         \
            para[1] = (UINT32)(FrmSize);                        \
            para[2] = (UINT32)(Width);                          \
            para[3] = (UINT32)(Height);                         \
            g_event_report(chan_id, EVNT_ARRANGE_FRAME_BUF, (UINT8*)para, sizeof(para));  \
        }                                                       \
    }while(0)


/* type=121 上报后级模块占有的一帧 */
#define REPORT_POST_MODULE_OCCUPY_ONE_FRAME(ChanID, specialFramePhyAddr, specialFrameVirAddr, specialFrameSize, findSpecialFrame)                       	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT32  para[4];                                    \
            para[0] = specialFramePhyAddr;                           \
            para[1] = specialFrameVirAddr;                           \
            para[2] = specialFrameSize;                                     \
            para[3] = findSpecialFrame;                                     \
            g_event_report(ChanID, EVNT_POST_MODULE_OCCUPY_ONE_FRAME, (UINT8*)para, sizeof(para));    \
        }                                                       \
    }while(0)

/* type=122 上报刷新已划分帧存状态或者释放变分辨率前动态分配的帧存 */
#define REPORT_REFRESH_CONFIG_FRAME_BUF(ChanID, phyAddr)                       	\
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT32  para[1];                                    \
            para[0] = phyAddr;                                  \
            g_event_report(ChanID, EVNT_REFRESH_CONFIG_FRAME_BUF, (UINT8*)para, sizeof(para));    \
        }                                                       \
    }while(0)
#endif

#define REPORT_SCD_BUFFER_REMAP(ChanID, virAddr)                \
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT64  para[1];                                    \
            para[0] = virAddr;                                  \
            g_event_report(ChanID, EVNT_SCD_BUFFER_REMAP, (UINT8*)para, sizeof(para));    \
        }                                                       \
    }while(0)

#define REPORT_VDH_BUFFER_REMAP(ChanID, virAddr)                \
    do{                                                         \
        if( NULL != g_event_report )                            \
        {                                                       \
            UINT64  para[1];                                    \
            para[0] = virAddr;                                  \
            g_event_report(ChanID, EVNT_VDH_BUFFER_REMAP, (UINT8*)para, sizeof(para));    \
        }                                                       \
    }while(0)

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
      functions
 ***********************************************************************/
#ifndef HI_ADVCA_FUNCTION_RELEASE
#ifdef VFMW_DPRINT_SUPPORT
#ifdef ENV_ARMLINUX_KERNEL
#define dprint(type, fmt, arg...)  vfmw_Osal_Func_Ptr_S.pfun_Osal_dprint(type, fmt, ##arg)
#endif
#else
#define dprint(type, fmt, arg...)  vfmw_dprint_nothing()
#endif
#else
#define dprint(type, fmt, arg...)  vfmw_dprint_nothing()
#endif

VOID vfmw_dprint_nothing(VOID);
SINT32 IsDprintTypeEnable(UINT32 type);

char *VFMW_GetString(VFMW_STRING_E enStringName);

#ifdef ENV_ARMLINUX_KERNEL
#define  DBG_OK       (0)
#define  DBG_ERR     (-1)
SINT32 dprint_vfmw( UINT32 type, const SINT8 *format, ... );
SINT32 DBG_CreateTracer(VOID);
VOID   DBG_DestroyTracer(VOID);
VOID   DBG_AddTrace( SINT8 *pFuncName, SINT32 nLine, SINT32 Data );
VOID   DBG_GetTracerInfo( SINT32 *pMemPhyAddr, SINT32 *pMemLength, SINT32 *pTracePhyAddr, SINT32 *pMaxNum, SINT32 *pCurIdx );
VOID   DBG_PrintTracer( SINT32 TraceDepth );
#endif
SINT32 CHECK_REC_POS_ENABLE(SINT32 type);
#ifdef VFMW_SCD_LOWDLY_SUPPORT
VOID DBG_CountTimeInfo(SINT32 ChanID, VFMW_STATE_TYPE_E SateType, SINT32 LowdlyFlag);
#endif
VOID     qsort( VOID *base, UINT32 num, UINT32 width, SINT32(*comp)(const VOID *, const VOID *) );


//extern SINT32  g_BypassFod;

#ifdef __cplusplus
}
#endif

#endif

