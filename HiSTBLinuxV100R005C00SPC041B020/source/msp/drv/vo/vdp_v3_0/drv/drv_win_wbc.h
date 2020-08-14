#ifndef __DRV_WIN_WBC_H__
#define __DRV_WIN_WBC_H__

#include "hi_drv_win.h"
#include "drv_win_buffer.h"

#ifdef __cplusplus
 #if __cplusplus
extern "C" {
 #endif
#endif /* __cplusplus */


typedef enum hiWBC_INIT_FLAG
{
    WBC_INITFLAG = 0xbeefdead,
    WBC_NOT_INIT_FLAG = 0xfffffff0,
    WBC_INIT_FLAG_BUTT
}WBC_INIT_FLAG;


#define WBC_FRAME_WRITE_FLAG  0xdeadbeaf

/*define wbc buf number.*/
#ifdef  MEM_RICH_FLAG 
#define WBC_BUF_NUM          3
#else
#define WBC_BUF_NUM          1
#endif

#define WBC_RECT_WIDTH       720
#define WBC_RECT_HEIGHT      576
#define WBC_MAX_STRIDE       720 
#define WBC_PERPIXEL_BYTES   2
#define WBC_FRAME_CREATE_FRAMEMODE
#define FRAME_IN_USE 0
#define FRAME_FREE   1
#define  WBC_TIMER_INTERVAL_MS 20
#define WBC_PARTFINISH_PERCENT_0 0


/* define the whole wbc's infor.
  * since window/videolayer has definitions, and only wbc added   
  * maybe a linker needed.
  */
typedef struct hiDRV_WIN_WBC_Linker_S{
	/*wbc in field mode, frame,field,top or bottom.*/
	HI_DRV_FIELD_MODE_E  eWbcFieldMode_In;

	/*wbc out field mode, decided by the out buffer.*/
	HI_DRV_FIELD_MODE_E  eWbcFieldMode_Out;

	/*whether open the p2i.*/
	HI_BOOL bOpenP2I;
	
	/*the current frame infor, for sd output.
	 *	as a output of wbc.
	 */
	HI_DRV_VIDEO_FRAME_S *pstCurrentFrame;

	/*the input size for wbc's zme.
	 * already processed by p2i.
	 */
	HI_RECT_S	 stinputSize;
	
	/*the outsize for V3, consideration of */	
	HI_DRV_WIN_ATTR_S stUsingAttr;	

	HI_DISP_DISPLAY_INFO_S stDispInfo;

	/*indicate the position of wbc.*/
	VDP_LAYER_CONN_E eWbcConnection;

	HI_DRV_COLOR_SPACE_E eSrcCS;
	HI_DRV_COLOR_SPACE_E eDstCS;
	
	/*indicates the wether the wbc has been inited.*/
	WBC_INIT_FLAG	 eInitalizeFlag;
	HI_BOOL bWbcTakeEffect;
	HI_BOOL  bEnable;
	
	/*for wbc*/
	WB_POOL_S stWinBP;
	
	HI_U32  u32StartTime;
	struct task_struct	*kThreadWbcPartFinish;
		
}HI_DRV_WIN_WBC_Linker_S;

typedef  struct hi_DISP_WBC_S
{
	HI_DRV_PIX_FORMAT_E ewbcInputPixel;
	HI_DRV_PIX_FORMAT_E ewbcOutputPixel;

	HI_DRV_FIELD_MODE_E  eWbcFieldMode_In;

	/*wbc out field mode, decided by the out buffer.*/
	HI_DRV_FIELD_MODE_E  eWbcFieldMode_Out;
	
}HI_DISP_WBC_S;

/*1  define  the wbc's infor.*/
typedef struct{
	/*indicates current wbc point */
	VDP_LAYER_CONN_E eCurrentWbcPoint;
	
	/* total wbc switch counts */
	HI_U32 u32WbcPointSwitchCounts;
	
	/*whether the wbc's P2I opened or not. */
	HI_BOOL u32WbcP2IOpenedOrNot;

	/*generate wbc infor's times. */
	HI_U32	u32WbcGenarateSuccessCounts;
	
	/*wbc  config try  times*/
	HI_U32	u32WbcConfigTryCounts;
	
	/*wbc  config succeed times*/
	HI_U32	u32WbcConfigOkCounts;
	
	/*wbc  retrieve node counts from sd window.*/
	HI_U32	u32WbcRetriveFromSdNodeCounts;
	
	/*hd's current line when config wbc.*/
	HI_U32	u32DhdCurrentLineWhenConfigWbc;

	/*field mode  of dhd when config wbc.*/
	HI_U32	u32DhdFieldModeWhenConfigWbc;

	/*wbc's input field mode.*/
	HI_U32	u32WbcInputFiledMode;
	
	/*wbc's output field mode.*/
	HI_U32	u32WbcOutputFiledMode;

	/*wbc's speed.*/
	HI_U32	u32WbcConfigSpeed;
	
}WIN_WBC_RUNNING_INFOR_S;

typedef  struct Linker{
	HI_BOOL bEnable;
	HI_VOID* hSdWinHanle;
}Linker_INFOR_S;


typedef struct{
	HI_U32	u32WbcTotalNodeCounts;
	HI_U32	u32WbcFreeNodesCounts;
	HI_U32	u32WbcFullNodesCounts;
	/*normally, only one frame: the pstConfig Frame.*/
	HI_U32	U32WbcConfiguringNodeCounts;
} WIN_WBC_FRAME_BUFFER_STATUS_S;

typedef enum {
	PARTFINISH_THREAD_STATUS_NO_FRAME = 0,
	PARTFINISH_THREAD_STATUS_NOT_FINISH,	
	PARTFINISH_THREAD_STATUS_PUSH_FRAME,
	PARTFINISH_THREAD_STATUS_BUTT,	
}WIN_ISO_PARTFINISH_THREADSTATUS_E;


/*1. define wbc proc infor struct.*/
typedef struct{
	WIN_WBC_FRAME_BUFFER_STATUS_S stWbcBufferStatus;
	WIN_WBC_RUNNING_INFOR_S  stWbcRunningInfor; 
}WIN_ISO_WBC_PROC_INFOR_S;

/*2. define partfinish thread proc infor struct.*/
typedef struct{ 
	/*partfinish try push times.*/
	HI_U32	u32WbcPartFinishTrycounts;
	
	/*partfish push succeed times*/
	HI_U32	u32WbcPartFinishPushFrameCnts;

	
	/*partfish push full list node counts*/
	HI_U32	u32WbcPartFinishNodesCnts;
	
	/*partfish schedule intervals*/
	HI_U32	u32WbcPartFinishIntervals;
	
	/*wbc  generate speed.*/
	HI_U32	u32WbcPartFinishSpeed;

	HI_U32	u32WbcStartTime;	
	HI_U32	u32WbcCurrentTime;
	HI_U32	u32WbcHighLantencyTimes;
	
	WIN_ISO_PARTFINISH_THREADSTATUS_E  ePartFinishThreadState;	
}WIN_ISO_PARTFINISH_THREAD_PROC_S;

/*3. define sd window's proc infor struct.*/
typedef struct{

	/*sd total frame counts.*/
	HI_U32 u32SdWinTotalFrameCounts;
	
	/*to be displayed frame counts.*/
	HI_U32 u32SdToBedisplayedCounts;
	
	/*been displayed frame counts.*/
	HI_U32 u32SdDisplayingFrameCounts;
	
	/*total return back frame counts to wbc.*/
	HI_U32 u32SdReturnBackFrameCounts;
	
	/*to be added:	total displayed count..*/
	
	/*underload counts.*/
	HI_U32 u32SdUnderloadCounts;
	
	/*discard counts.*/
	HI_U32 u32SdDiscardsCounts;
	
	/*field mismatch counts.*/
	HI_U32 u32SdFiledMisMatchCounts;
	
	/*frame field.*/
	HI_U32 u32SdFrameFieldMode;
	
	/*dsd  field.*/
	HI_U32 u32DsdFieldMode;
	
	/*dsd  field.*/
	HI_U32 u32DsdCurrentLine;
	
	HI_U32 u32FrameLantencyTimeMs;	
	HI_U32 u32FrameExpectWbcTimeMs;	
	HI_U32 u32FrameLinkerProcessTimeMs;
	HI_U32 u32FrameSdReceiveTimeMs;
	HI_U32 u32FrameSdDisplayTimeMs;
	
}WIN_ISO_SD_WIN_PROC_S;

typedef struct{
	HI_BOOL    bWbcInit;
	WIN_ISO_WBC_PROC_INFOR_S stWbcProcInfor;
	WIN_ISO_PARTFINISH_THREAD_PROC_S stPartFinishThreadProcInfor;
	WIN_ISO_SD_WIN_PROC_S  stSdWinProcInfor;
}WIN_WBCISOGENY_PROC_INFOR_S;

typedef struct {
	HI_BOOL bLowDelay;
	HI_VOID*  pstFrameAddr;
	HI_U32  u32FrameStatus;
	HI_U32  bFrameValid;
	unsigned long long ullExpectWbcFinishTimeMs;
	unsigned long long ullLinkerProcessTimeMs;
	unsigned long long ullSdReceiveTimeMs;
	unsigned long long ullSdDisplayTimeMs;	
}WBC_FRAME_PRIVATE_S;

WIN_WBCISOGENY_PROC_INFOR_S WIN_GetWbcIsoInfor(HI_VOID);
HI_VOID WbcLinker_Proc(HI_VOID);
Linker_INFOR_S *Thread_Get_LinkerStatus(HI_VOID);





#ifdef __cplusplus
 #if __cplusplus
}
 #endif
#endif /* __cplusplus */


#endif




