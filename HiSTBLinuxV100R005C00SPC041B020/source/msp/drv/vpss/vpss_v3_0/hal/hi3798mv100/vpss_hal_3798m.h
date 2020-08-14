#ifndef __VPSS_HAL_S40V300_H__
#define __VPSS_HAL_S40V300_H__
#include"hi_type.h"

#include "hi_drv_mmz.h"

#include"vpss_common.h"

#include "hi_drv_vpss.h"
#include "vpss_reg_3798m.h"
#include "vpss_sttinf.h"
#include "vpss_wbc.h"
#include "vpss_rwzb.h"
#include "vpss_his.h"
#include "drv_pq_ext.h"

#define  VPSS_ZME_HPREC        (1<<20)
#define  VPSS_ZME_HPREC_F        (1<<19)
#define  VPSS_ZME_HPREC_B        (1<<1)

#define  VPSS_ZME_VPREC        (1<<12)

#define HAL_VERSION_3798M 0x300

#define VPSS0_BASE_ADDR  0xf8cb0000
#define VPSS1_BASE_ADDR  0xffffffff
#define VPSS_REG_SIZE    ((sizeof(VPSS_REG_S) + 15) & 0xFFFFFFF0)

#define VPSS_ZME_COEF_SIZE (512)

#define VPSS_ZME_COEF_NUM 16

#define DEF_VPSS_HAL_REF_LIST_NUM 4

#define VPSS_REG_SIZE_CALC(start, end)\
    (offsetof(VPSS_REG_S, end) + sizeof(HI_U32) -\
     offsetof(VPSS_REG_S, start))
     
#define VPSS_HAL_CHECK_IP_VAILD(enIP) \
do{\
    if((enIP != VPSS_IP_0)&&(enIP != VPSS_IP_1))\
    {\
        VPSS_ERROR("VPSS IP%d, is Not Vaild\n", enIP);\
        return HI_FAILURE;\
    }\
}while(0)

#define VPSS_HAL_CHECK_NODE_ID_VAILD(enTaskNodeId) \
do{\
    if(enTaskNodeId >= VPSS_HAL_TASK_NODE_BUTT)\
    {\
        VPSS_ERROR("VPSS NODE ID%d, is Not Vaild\n", enTaskNodeId);\
        return HI_FAILURE;\
    }\
}while(0)

#define VPSS_HAL_CHECK_INIT(bInit) \
do{\
    if (HI_FALSE == bInit){\
        VPSS_ERROR("VPSS HAL Is Not Init\n");\
        return HI_FAILURE;\
    }\
}while(0)


#define VPSS_HAL_CHECK_NULL_PTR(ptr) \
do{\
    if (HI_NULL == ptr){\
        VPSS_ERROR("pointer is NULL!\n");\
        return HI_FAILURE;\
    }\
}while(0)

#define DEF_VPSS_HAL_PORT_NUM 2

typedef enum hiVPSS_IP_E
{
    VPSS_IP_0 = 0,
    VPSS_IP_1,
    VPSS_IP_BUTT
}VPSS_IP_E;

typedef enum hiVPSS_HAL_TASK_NODE_E
{
    VPSS_HAL_TASK_NODE_2D_FIELD = 0,
	VPSS_HAL_TASK_NODE_2D_FIELD_VIRTUAL,
    VPSS_HAL_TASK_NODE_2D,
	VPSS_HAL_TASK_NODE_SPLIT_L,
	VPSS_HAL_TASK_NODE_SPLIT_R,
	VPSS_HAL_TASK_NODE_2D_VIRTUAL,
    VPSS_HAL_TASK_NODE_3D_R,
    VPSS_HAL_TASK_NODE_3D_R_VIRTUAL,
    VPSS_HAL_TASK_NODE_P0_ZME_L2,
    VPSS_HAL_TASK_NODE_P1_ZME_L2,
    VPSS_HAL_TASK_NODE_P2_ZME_L2,
    VPSS_HAL_TASK_NODE_P0_RO_Y,
    VPSS_HAL_TASK_NODE_P0_RO_C,
    VPSS_HAL_TASK_NODE_P1_RO_Y,
    VPSS_HAL_TASK_NODE_P1_RO_C,
    VPSS_HAL_TASK_NODE_P2_RO_Y,
    VPSS_HAL_TASK_NODE_P2_RO_C,

    VPSS_HAL_TASK_NODE_2D_H265_INTERLACE,
    VPSS_HAL_TASK_NODE_2D_H265_DEI,
		
    VPSS_HAL_TASK_NODE_BUTT
}VPSS_HAL_TASK_NODE_E;


typedef struct hiVPSS_HAL_RefList
{
    VPSS_MEM_S stRefListBuf_0;
    HI_DRV_VID_FRAME_ADDR_S stRefListAddr[DEF_VPSS_HAL_REF_LIST_NUM];
    HI_BOOL					abRefNodeValid[DEF_VPSS_HAL_REF_LIST_NUM];
    HI_BOOL					bRefListValid;
    HI_U32 					u32RefListHead;
}VPSS_HAL_RefList_S;


typedef struct hiVPSS_HAL_CTX_S
{
    HI_BOOL  bInit;
    HI_BOOL  bClockEn;
    HI_U32   u32LogicVersion;
    
    HI_U32   u32BaseRegPhy;
    HI_U32   u32BaseRegVir;
    
    HI_U32   au32AppPhy[VPSS_HAL_TASK_NODE_BUTT];
    HI_U32   au32AppVir[VPSS_HAL_TASK_NODE_BUTT];
    MMZ_BUFFER_S stRegBuf;

    HI_BOOL  abUsed[VPSS_ZME_COEF_NUM];
    HI_U32   au32ZmeCoefPhy[VPSS_ZME_COEF_NUM][4];
    HI_U32   au32ZmeCoefVir[VPSS_ZME_COEF_NUM][4];
    MMZ_BUFFER_S stZmeCoefBuf;
    MMZ_BUFFER_S stLowDelayBuf;
} VPSS_HAL_CTX_S;


typedef struct hiVPSS_HAL_ZME_PARAM_S
{
    HI_BOOL bYUV;
    HI_U32 u32YHRatio;
    HI_U32 u32CHRatio;
    HI_U32 u32YVRatio;
    HI_U32 u32CVRatio;
    ZME_FORMAT_E enInFmt;
    ZME_FORMAT_E enOutFmt;    
}VPSS_HAL_ZME_PARAM_S;

typedef struct hiVPSS_HAL_FRAME_S{
    HI_DRV_FRAME_TYPE_E      eFrmType;
    HI_U32 u32Width;
    HI_U32 u32Height;
    HI_DRV_PIX_FORMAT_E enFormat;
    HI_DRV_FIELD_MODE_E enFieldMode;
    HI_BOOL bProgressive;
    HI_DRV_VID_FRAME_ADDR_S stAddr;
    HI_BOOL                  bCompressd;
	HI_BOOL                  bUVInver;
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;
    HI_U32 u32TunnelAddr;
    HI_BOOL  bTopFirst;
	HI_BOOL  bSecure;
}VPSS_HAL_FRAME_S;


typedef struct hiVPSS_HAL_PORT_INFO_S
{
    HI_BOOL    bEnable;
    HI_RECT_S  stInCropRect; /* PORT CROP信息 */
    HI_RECT_S  stVideoRect; /* 真实显示区域 */
    HI_RECT_S   stOutCropRect;
    HI_DRV_VPSS_ROTATION_E enRotation; /* 旋转信息 */
    HI_BOOL bNeedFlip;
    HI_BOOL bNeedMirror;

	HI_BOOL bCmpLoss;
	HI_BOOL bConfig;

    VPSS_HAL_FRAME_S stOutInfo; /* PORT输出信息 */
} VPSS_HAL_PORT_INFO_S;


typedef struct hiVPSS_DIE_INFO_S
{
	VPSS_DIESTCFG_S stDieStCfg;
    HI_BOOL bBottom_first;

	//:TODO: 插值选择信息
	
}VPSS_DIE_INFO_S;

typedef struct hiVPSS_NR_INFO_S
{
    HI_BOOL bNrEn;
	VPSS_NRMADCFG_S stNrMadCfg;
}VPSS_NR_INFO_S;

typedef struct hiVPSS_CCCL_INFO_S
{
    HI_BOOL bCCCLEn;
	VPSS_HAL_FRAME_S stInRefInfo[2];
	VPSS_CCCLCNTCFG_S stCCCLCntCfg;
}VPSS_CCCL_INFO_S;


typedef enum hiVPSS_HAL_NODE_TYPE_E
{
    VPSS_HAL_NODE_2D_FRAME = 0,
    VPSS_HAL_NODE_2D_5Field,
    VPSS_HAL_NODE_2D_3Field,
    VPSS_HAL_NODE_2D_Field,
    VPSS_HAL_NODE_3D_FRAME_R, //用于配置读取偏移，在解码源为SBS/TAB，暂时不考虑拆分之后还有隔行的情况
    VPSS_HAL_NODE_PZME, //对应隔行，单场的源的类型
    VPSS_HAL_NODE_UHD, // 4K*2K场景，后面看是否有UHD非标的特殊场景，再增加类型
    VPSS_HAL_NODE_UHD_HIGH_SPEED,// 4K*2K场景，DTV  4k@60场景,一拍两像素配置
    VPSS_HAL_NODE_UHD_SPLIT_L,
    VPSS_HAL_NODE_UHD_SPLIT_R,
    VPSS_HAL_NODE_UHD_HALF,
    VPSS_HAL_NODE_3DDET,// 3D检测通路，只需要Y分量
    VPSS_HAL_NODE_ZME_2L,// 2级缩放节点
    VPSS_HAL_NODE_ROTATION_Y,
    VPSS_HAL_NODE_ROTATION_C,
    VPSS_HAL_NODE_2D_H265_STEP1_INTERLACE,
    VPSS_HAL_NODE_2D_H265_STEP2_DEI,
    VPSS_HAL_NODE_BUTT
} VPSS_HAL_NODE_TYPE_E;

typedef struct hiVPSS_HAL_INFO_S
{
    VPSS_REG_S *pstPqCfg;
    VPSS_HAL_NODE_TYPE_E enNodeType; 
	VPSS_HAL_FRAME_S stInInfo;             //输入源信息
    VPSS_RWZB_INFO_S stRwzbInfo;
    
	/*VPSS V2_0*/
    VPSS_HAL_FRAME_S stInRefInfo[4];       //参考帧信息
    VPSS_HAL_FRAME_S stInWbcInfo;          //回写信息
    HI_U32 u32stt_w_phy_addr;
    HI_U32 u32stt_w_vir_addr;
	VPSS_NR_INFO_S stNrInfo;
	VPSS_CCCL_INFO_S stCCCLInfo;
	
	/*VPSS V1_0*/
    HI_DRV_VID_FRAME_ADDR_S stFieldAddr[6];
    VPSS_HIS_ADDR_S stHisAddr;
	
    VPSS_DIE_INFO_S stDieInfo; 

    HI_U32 u32ScdValue;
    HI_BOOL bOutLowDelay;
    VPSS_HAL_PORT_INFO_S astPortInfo[DEF_HI_DRV_VPSS_PORT_MAX_NUMBER];

    VPSS_HAL_RefList_S *pstRefList;
} VPSS_HAL_INFO_S;


HI_S32 VPSS_HAL_Init(VPSS_IP_E enIP);
HI_S32 VPSS_HAL_DelInit(VPSS_IP_E enIP);

HI_S32 VPSS_HAL_SetClockEn(VPSS_IP_E enIP, HI_BOOL bClockEn);
HI_S32 VPSS_HAL_GetClockEn(VPSS_IP_E enIP, HI_BOOL *pbClockEn);

HI_S32 VPSS_HAL_GetIntState(VPSS_IP_E enIP, HI_U32* pu32IntState);
HI_S32 VPSS_HAL_ClearIntState(VPSS_IP_E enIP, HI_U32 u32IntState);

HI_S32 VPSS_HAL_SetNodeInfo(VPSS_IP_E enIP,
     VPSS_HAL_INFO_S *pstHalInfo,  VPSS_HAL_TASK_NODE_E enTaskNodeId);

HI_S32 VPSS_HAL_StartLogic(VPSS_IP_E enIP, 
    HI_BOOL abNodeVaild[VPSS_HAL_TASK_NODE_BUTT]);

HI_S32 VPSS_HAL_GetSCDInfo(HI_U32 u32AppAddr,HI_S32 s32SCDInfo[32]);

HI_VOID VPSS_HAL_GetDetPixel(VPSS_IP_E enIP,HI_U32 BlkNum, HI_U8* pstData);

HI_S32 VPSS_HAL_GetBaseRegAddr(VPSS_IP_E enIP,
                                 HI_U32 *pu32PhyAddr,
                                 HI_U32 *pu32VirAddr);

HI_S32 VPSS_HAL_GetCycleCnt(VPSS_IP_E enIP,HI_U32 *pu32Cycle);
HI_S32 VPSS_HAL_RefListInit(VPSS_HAL_RefList_S *pstRefList, HI_S32 s32Width, HI_S32 s32Height, HI_DRV_PIX_FORMAT_E 
enFormat, HI_BOOL bSecure);
HI_S32 VPSS_HAL_RefListDeInit(VPSS_HAL_RefList_S *pstRefList);
#endif

