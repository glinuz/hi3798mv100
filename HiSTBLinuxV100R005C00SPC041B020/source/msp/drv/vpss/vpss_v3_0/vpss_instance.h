#ifndef __VPSS_INSTANCE_H__
#define __VPSS_INSTANCE_H__

#include "hi_type.h"
#include"drv_vpss_ext.h"
#include "hi_drv_mmz.h"
#include "vpss_fb.h"
#include "linux/list.h"
#include "vpss_osal.h"
#include "vpss_in_intf.h"
#include "vpss_info.h"
#include "drv_vdec_ext.h"
#include "hi_unf_video.h"
#include "drv_pq_ext.h"
#include "vpss_alg_scd.h"

#define VPSS_PORT_MAX_NUMB 3 


typedef struct hiVPSS_PORT_PRC_S{
    HI_S32 s32PortId;
    HI_BOOL bEnble;

    VPSS_FB_STATE_S stFbPrc;
    HI_DRV_VPSS_BUFLIST_CFG_S stBufListCfg;
    HI_DRV_PIX_FORMAT_E eFormat; 
    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;
    HI_DRV_COLOR_SPACE_E eDstCS;
    
    HI_DRV_ASPECT_RATIO_S stDispPixAR;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    HI_BOOL b3Dsupport;
    HI_BOOL   bInterlaced;                
    HI_RECT_S stScreen;                  
    HI_U32 u32MaxFrameRate;  
    HI_U32 u32OutCount;    

    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    HI_BOOL  bTunnelEnable;  
    HI_S32  s32SafeThr;
    
    HI_DRV_VPSS_ROTATION_E enRotation;
    HI_BOOL bHoriFlip;
    HI_BOOL bVertFlip;
}VPSS_PORT_PRC_S;

typedef enum hiVPSS_INSTANCE_STATE_E{
    INSTANCE_STATE_STOP = 0,
    INSTANCE_STATE_WORING,
    INSTANCE_STATE_BUTT
}VPSS_INSTANCE_STATE_E;

typedef struct hiVPSS_PORT_S{
    HI_S32 s32PortId;
    HI_BOOL bEnble;

    VPSS_FB_INFO_S stFrmInfo;
    
    HI_DRV_PIX_FORMAT_E eFormat; 
    HI_S32  s32OutputWidth;
    HI_S32  s32OutputHeight;
    HI_DRV_COLOR_SPACE_E eDstCS;

	/*
	 *[0]:Level
	 *[1]:OriW/H
	 *[2]:ReviseW/H
	 *[3]:OriARW/H
	 *[4]:ReviseARW/H
	 */	
	HI_RECT_S stLevelOutRect[3][5];
    
    HI_DRV_ASPECT_RATIO_S stDispPixAR;
    HI_DRV_ASP_RAT_MODE_E eAspMode;
    HI_DRV_ASPECT_RATIO_S stCustmAR;
    
    HI_BOOL b3Dsupport;

    HI_BOOL   bInterlaced;               
    HI_RECT_S stScreen;                  

    HI_U32 u32MaxFrameRate;  
    HI_U32 u32OutCount;     

    HI_DRV_VPSS_PORT_PROCESS_S stProcCtrl;

    HI_BOOL  bTunnelEnable;  
    HI_S32  s32SafeThr; 

    HI_BOOL bOnlyKeyFrame;      /* 配合硬件FRC工作，电影模式源需要输出非重复帧，TV独有 */
    HI_BOOL bLBDCropEn;         /* 动态CROP已检测到的黑边，TV独有 */
    HI_RECT_S stVideoRect;      /* TV LBX需求 */

    HI_RECT_S stInRect;
    HI_BOOL   bUseCropRect;
    HI_DRV_CROP_RECT_S stCropRect;
    
    HI_DRV_PIXEL_BITWIDTH_E  enOutBitWidth;

    HI_DRV_VPSS_ROTATION_E enRotation;
    HI_BOOL bHoriFlip;
    HI_BOOL bVertFlip;
	HI_U32  u32cmpflag; // default 0, cmp on 1, cmp off 2
	HI_BOOL bCurDropped;
	HI_BOOL bPassThrough;
	
#ifdef ZME_2L_TEST
	HI_BOOL bNeedZME2L;
	HI_BOOL bNeedRotate;
	HI_U32  u32ZME1LWidth;
	HI_U32  u32ZME1LHeight;
#endif	
}VPSS_PORT_S;

typedef struct hiVPSS_STREAM_ORIGINFO_S{   
    HI_U32 u32StreamInRate;
    HI_BOOL u32StreamTopFirst;
    HI_BOOL u32StreamProg; 
    HI_DRV_PIX_FORMAT_E ePixFormat;     
}VPSS_STREAM_ORIGINFO_S;

#define VPSS_LBX_DET_NODE_NUM 32
typedef struct hiVPSS_LBX_DET_S
{
    HI_U32 m_top[VPSS_LBX_DET_NODE_NUM];
    HI_U32 m_bot[VPSS_LBX_DET_NODE_NUM];
    HI_U32 m_left[VPSS_LBX_DET_NODE_NUM];
    HI_U32 m_right[VPSS_LBX_DET_NODE_NUM];

    HI_U32 u32NodeIndex;

    HI_U32 u32valid_top;
    HI_U32 u32valid_bot;
    HI_U32 u32valid_left;
    HI_U32 u32valid_right;
}VPSS_LBX_DET_S;


typedef struct hiVPSS_INSTANCE_S
{
    HI_S32  ID;                       //实例ID
    HI_S32  CtrlID;                //实例隶属的CTRL实体ID
	VPSS_CAPABILITY_U enCapability; //vpss处理能力集
    VPSS_INSTANCE_STATE_E enState;  //实例状态 WORK/STOP
    VPSS_OSAL_LOCK stInstLock;      //实例资源锁，线程与接口读写保护
    VPSS_OSAL_SPIN stUsrSetSpin;
    VPSS_REG_S stPqRegData;             //PQ初始化数据
    PQ_VPSS_WBC_REG_S stPqWbcReg;             //PQ初始化数据
    HI_PQ_VPSS_MODULE_S stPQModule;
    HI_PQ_IFMD_PLAYBACK_S stIfmdRls;    //IFMD数据
    HI_PQ_PFMD_PLAYBACK_S stPfmdRls;    //PFMD数据
    VPSS_LBX_DET_S stLbxDet;
	HI_BOOL bStorePrivData;
	/*
    enable -> disable  OK 
    disable -> enable  reset wbc
	*/
	HI_BOOL bEnableTNR;
                                    
    HI_DRV_VIDEO_FRAME_S stSrcImage;
    /*用户接口配置*/
    HI_BOOL bCfgNew;           //用户配置更新
	HI_BOOL bTvpFirstCfg;
    HI_DRV_VPSS_CFG_S stUsrInstCfg;    //用户配置数据
    HI_DRV_VPSS_PORT_CFG_S  stUsrPortCfg[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];
    HI_DRV_VPSS_PROCESS_S stProcCtrl; //看是否保留
    
    /*低延时总处理最新一帧*/
    HI_BOOL bAlwaysFlushSrc;

    HI_BOOL bSecure;

    VPSS_SRCIN_S stSrcIn;
    
    /*debug proc 信息*/
    VPSS_DBG_S stDbgCtrl;

    VPSS_RWZB_S stRwzbInfo;
    
    VPSS_IN_ENTITY_S stInEntity;
	
    VPSS_PORT_S stPort[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];

    HI_BOOL abNodeVaild[VPSS_HAL_TASK_NODE_BUTT];
    
    /*用户注册回调*/
    HI_HANDLE hDst;
    PFN_VPSS_CALLBACK pfUserCallBack;

    /*与前级交互模式*/
    HI_DRV_VPSS_SOURCE_MODE_E eSrcImgMode;
    HI_DRV_VPSS_SOURCE_FUNC_S stSrcFuncs;

	HI_U32 u32UhdLevelW;
	HI_U32 u32UhdLevelH;

	HI_U32 u32UhdUsrLevelW;
	HI_U32 u32UhdUsrLevelH;

	atomic_t atmUhdFmt; 

    HI_U32 u32CheckRate;
    HI_U32 u32CheckSucRate;
    HI_U32 u32CheckCnt;
    HI_U32 u32CheckSucCnt;
    HI_U32 u32LastCheckTime;

    HI_U32 u32ImgRate;
    HI_U32 u32ImgSucRate;
    HI_U32 u32ImgCnt;
    HI_U32 u32ImgSucCnt;
    HI_U32 u32ImgLastCnt;
    HI_U32 u32ImgLastSucCnt;
    
    HI_U32 u32SrcRate;
    HI_U32 u32SrcSucRate;
    HI_U32 u32SrcCnt;
    HI_U32 u32SrcSucCnt;
    
    HI_U32 u32BufRate;
    HI_U32 u32BufSucRate;
    HI_U32 u32BufCnt;
    HI_U32 u32BufSucCnt;
    
    HI_U32 u32ScenceChgCnt;
#if defined(CHIP_TYPE_hi3716mv410)||defined(CHIP_TYPE_hi3716mv420)||defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)
    VPSS_HAL_RefList_S stRefList;
#endif
}VPSS_INSTANCE_S;


/* FUNCTIONS FOR CTRL */
HI_S32 VPSS_INST_SyncUsrCfg(VPSS_INSTANCE_S *pstInstance);
HI_S32 VPSS_INST_CheckInstAvailable(VPSS_INSTANCE_S *pstInstance);
HI_S32 VPSS_INST_CompleteImage(VPSS_INSTANCE_S *pstInstance);
HI_S32 VPSS_INST_GetFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,
                    HI_DRV_VPSS_BUFLIST_CFG_S* pstBufCfg,VPSS_BUFFER_S *pstBuffer,
                    HI_U32 u32StoreH,HI_U32 u32StoreW);
HI_S32 VPSS_INST_CheckFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE  hPort,
                                HI_BOOL *bAvailable); 

HI_S32 VPSS_INST_RelFrmBuffer(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE  hPort,
                                HI_DRV_VPSS_BUFLIST_CFG_S   *pstBufCfg,
                                VPSS_MEM_S *pstMemBuf);
HI_S32 VPSS_INST_ReportNewFrm(VPSS_INSTANCE_S* pstInstance,
                                VPSS_HANDLE  hPort,HI_DRV_VIDEO_FRAME_S *pstFrm);
HI_BOOL VPSS_INST_CheckIsDropped(VPSS_INSTANCE_S *pstInstance,HI_U32 u32OutRate,HI_U32 u32OutCount);

VPSS_HAL_NODE_TYPE_E VPSS_INST_Check2DNodeType(VPSS_INSTANCE_S* pstInst);
VPSS_HAL_NODE_TYPE_E VPSS_INST_Check3DNodeType(VPSS_INSTANCE_S* pstInst);
HI_VOID VPSS_INST_SetHalFrameInfo(HI_DRV_VIDEO_FRAME_S *pstFrame,
    VPSS_HAL_FRAME_S *pstHalFrm, HI_DRV_BUF_ADDR_E enBufLR);
HI_VOID VPSS_INST_SetOutFrameInfo(VPSS_INSTANCE_S* pstInst, HI_U32 PortId, VPSS_BUFFER_S *pstBuf,
    HI_DRV_VIDEO_FRAME_S *pstFrm, HI_DRV_BUF_ADDR_E enBufLR);
HI_VOID VPSS_INST_SetRotationOutFrameInfo(VPSS_INSTANCE_S* pstInst, HI_U32 PortId, VPSS_BUFFER_S* pstBuf,
                                  HI_DRV_VIDEO_FRAME_S* pstFrm, HI_DRV_BUF_ADDR_E enBufLR);

HI_VOID VPSS_INST_GetInCrop(VPSS_INSTANCE_S* pstInst, HI_U32 PortId, HI_RECT_S *pstInCropRect);
HI_VOID VPSS_INST_GetVideoRect(VPSS_INSTANCE_S* pstInst, 
								HI_U32 PortId, 
								HI_RECT_S *pstInCropRect, 
								HI_RECT_S *pstVideoRect,
								HI_RECT_S *pstOutCropRect);
HI_VOID VPSS_INST_GetLbxInfo(VPSS_INSTANCE_S* pstInst, HI_U32 PortId, HI_RECT_S *pstLbx);
HI_S32 VPSS_INST_GetPortPrc(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,VPSS_PORT_PRC_S *pstPortPrc);
HI_VOID VPSS_INST_GetRotate(VPSS_INSTANCE_S* pstInst, HI_U32 PortId, VPSS_HAL_PORT_INFO_S *pstHalPortInfo, HI_DRV_VIDEO_FRAME_S *pstFrm);


/* FUNCTIONS FOR INTF */
HI_S32 VPSS_INST_Init(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_INST_DelInit(VPSS_INSTANCE_S *pstInstance);
HI_S32 VPSS_INST_GetDefInstCfg(HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_S32 VPSS_INST_SetInstCfg(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);
HI_U32 VPSS_INST_GetInstCfg(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_CFG_S *pstVpssCfg);

HI_S32 VPSS_INST_CreatePort(VPSS_INSTANCE_S *pstInstance,HI_DRV_VPSS_PORT_CFG_S *pstPortCfg,VPSS_HANDLE *phPort);
HI_S32 VPSS_INST_DestoryPort(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort);
HI_U32 VPSS_INST_GetDefPortCfg(HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);
HI_S32 VPSS_INST_CheckPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,HI_DRV_VPSS_PORT_CFG_S *pstVpssPortCfg);
HI_S32 VPSS_INST_GetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);
HI_S32 VPSS_INST_SetPortCfg(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort, HI_DRV_VPSS_PORT_CFG_S *pstPortCfg);

HI_S32 VPSS_INST_RelPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_INST_GetPortFrame(VPSS_INSTANCE_S *pstInstance,VPSS_HANDLE hPort,HI_DRV_VIDEO_FRAME_S *pstFrame);
HI_S32 VPSS_INST_EnablePort(VPSS_INSTANCE_S * pstInstance, VPSS_HANDLE hPort,HI_BOOL bEnPort);

HI_S32 VPSS_INST_SetUserActiveMode(VPSS_INSTANCE_S * pstInstance);

HI_S32 VPSS_INST_ReplyUserCommand(VPSS_INSTANCE_S * pstInstance,
                                    HI_DRV_VPSS_USER_COMMAND_E eCommand,
                                    HI_VOID *pArgs);

HI_S32 VPSS_INST_SetCallBack(VPSS_INSTANCE_S *pstInstance,HI_HANDLE hDst, PFN_VPSS_CALLBACK pfVpssCallback);

HI_S32 VPSS_INST_GetPortListState(VPSS_INSTANCE_S* pstInstance,VPSS_HANDLE hPort,HI_DRV_VPSS_PORT_BUFLIST_STATE_S *pstListState);

HI_S32 VPSS_INST_GetSrcListState(VPSS_INSTANCE_S* pstInstance,VPSS_IN_IMAGELIST_STATE_S *pstListState);

HI_S32 VPSS_INST_SetUhdLevel(VPSS_INSTANCE_S* pstInstance,HI_U32 u32WidthLevel,HI_U32 u32HeightLevel);

HI_S32 VPSS_INST_LevelRectInit(HI_RECT_S pMatrixRect[][5]);

HI_S32 VPSS_INST_LevelRectSetOriRect(HI_RECT_S pMatrixRect[][5],
									HI_S32 s32OriWidth,
									HI_S32 s32OriHeight,
									HI_S32 s32OriARw,
									HI_S32 s32OriARh);

HI_S32 VPSS_INST_LevelRectSetReviseRect(HI_RECT_S pMatrixRect[][5],
									HI_RECT_S stLevelRect,
									HI_S32 s32ReviseWidth,
									HI_S32 s32ReviseHeight,
									HI_S32 s32ReviseARw,
									HI_S32 s32ReviseARh);

HI_S32 VPSS_INST_LevelRectGetReviseRect(HI_RECT_S pMatrixRect[][5],
									HI_RECT_S stLevelRect,
									HI_S32 *ps32ReviseWidth,
									HI_S32 *ps32ReviseHeight,
									HI_S32 *ps32ReviseARw,
									HI_S32 *ps32ReviseARh);

HI_S32 VPSS_INST_LevelRectGetOriRect(HI_RECT_S pMatrixRect[][5],
									HI_RECT_S stLevelRect,
									HI_S32 *ps32OriWidth,
									HI_S32 *ps32OriHeight,
									HI_S32 *ps32OriARw,
									HI_S32 *ps32OriARh);

HI_S32 VPSS_INST_EnableStorePriv(VPSS_INSTANCE_S* pstInstance,HI_BOOL bStore);
HI_S32 VPSS_INST_SyncTvpCfg(VPSS_INSTANCE_S *pstInstance);

HI_BOOL VPSS_INST_CheckPassThrough(VPSS_INSTANCE_S *pstInstance,HI_DRV_VIDEO_FRAME_S *pstInImage);

HI_S32 VPSS_INST_SetBandPolicy(VPSS_INSTANCE_S * pstInstance);
#endif
