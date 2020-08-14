#ifndef __VPSS_WBC_H__
#define __VPSS_WBC_H__

#include "hi_drv_mmz.h"
#include "vpss_common.h"
#include "hi_drv_vpss.h"
#include <linux/list.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VPSS_WBC_MAX_NODE 5

typedef struct hiVPSS_WBC_DATA_S
{
    HI_DRV_VIDEO_FRAME_S stWbcFrame;  //帧信息
    LIST node;                        //链表节点
} VPSS_WBC_DATA_S;

typedef enum hiVPSS_WBC_MODE_E
{
    VPSS_WBC_MODE_NORMAL = 2,        //回写帧模式，回写队列开辟2个帧存节点
    VPSS_WBC_MODE_3FIELD = 3,        //回写三场模式，回写队列开辟3个帧存节点
    VPSS_WBC_MODE_4FIELD = 4,        //回写四场模式，回写队列开辟4个帧存节点
    VPSS_WBC_MODE_5FIELD = 5,        //回写五场模式，回写队列开辟5个帧存节点
    VPSS_WBC_MODE_BUTT
} VPSS_WBC_MODE_E;

typedef enum hiVPSS_WBC_REF_MODE_E
{
    VPSS_WBC_REF_MODE_INIT = 1,        //获取参考帧时，当回写帧次数不够时，未回写的参考帧赋值为初始值，回写的参考帧正常赋值
    VPSS_WBC_REF_MODE_NULL = 2,        //获取参考帧时，当回写帧次数不够时，未回写的参考帧赋值为无效，回写的参考帧正常赋值
    VPSS_WBC_REF_MODE_BUTT
} VPSS_WBC_REF_MODE_E;

typedef struct hiVPSS_WBC_ATTR_S
{
    VPSS_WBC_MODE_E enMode;               //回写队列工作模式
    VPSS_WBC_REF_MODE_E enRefMode;        //模式值使用模式
    HI_U32    u32Width;                   //帧信息宽
    HI_U32    u32Height;                  //帧信息高
    HI_DRV_PIX_FORMAT_E ePixFormat;        //帧信息像素格式
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;  //帧信息图像bit位,10 bit or 8 bit
    HI_U32                   u32FrameIndex; //第一帧INDEX值
    HI_U32                   u32Pts;        //第一帧PTS值
} VPSS_WBC_ATTR_S;

typedef struct hiVPSS_WBC_S
{
    HI_BOOL bInit;                     //初始化标识
    HI_U32 u32CompleteCount;           //回写链表轮转次数    
    VPSS_WBC_DATA_S stDataList[VPSS_WBC_MAX_NODE];     //回写链表结构
    VPSS_WBC_ATTR_S stWbcAttr;         //回写链表属性
    MMZ_BUFFER_S stMMZBuf;             //帧存信息
    LIST* pstFirstRef;                  //指向链表第一个参考帧
} VPSS_WBC_S;


HI_S32 VPSS_WBC_Init(VPSS_WBC_S* pstWbc, VPSS_WBC_ATTR_S* pstAttr);

HI_S32 VPSS_WBC_DeInit(VPSS_WBC_S* pstWbc);

HI_S32 VPSS_WBC_GetRefInfo(VPSS_WBC_S* pstWbc,
                           HI_DRV_VIDEO_FRAME_S** pstDataList
                          );

HI_S32 VPSS_WBC_GetWbcInfo(VPSS_WBC_S* pstWbc, HI_DRV_VIDEO_FRAME_S** pstData);

HI_S32 VPSS_WBC_Complete(VPSS_WBC_S* pstWbc);

HI_S32 VPSS_WBC_Reset(VPSS_WBC_S* pstWbc);

HI_S32 VPSS_WBC_GetCurInfo(VPSS_WBC_S* pstWbc,HI_DRV_VIDEO_FRAME_S **ppstCurImage);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__VPSS_WBC_H__*/
