#ifndef __VPSS_WBC_MCDEI_H__
#define __VPSS_WBC_MCDEI_H__

#include "hi_drv_mmz.h"
#include "vpss_common.h"
#include "hi_drv_vpss.h"
#include <linux/list.h>


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VPSS_RGME_WBC_MAX_NODE 4

typedef struct hiVPSS_WBC_MCDEI_DATA_S
{
    HI_DRV_VIDEO_FRAME_S stWbcFrame;  //帧信息
    LIST node;                        //链表节点
} VPSS_WBC_MCDEI_DATA_S;
typedef enum hiVPSS_RGME_MODE_E{       
    VPSS_RGME_MODE_4FIELD = 4,        //MCDEI目前固定4场模式，回写队列开辟4个帧存节点
    VPSS_RGME_MODE_BUTT
}VPSS_RGME_MODE_E;

typedef enum hiVPSS_RGME_PREIMG_MODE_E{
    VPSS_RGME_PREIMG_MODE_INIT = 1,        //获取参考帧时，当回写帧次数不够时，参考帧全部赋值为无效   
    VPSS_RGME_PREIMG_MODE_WORK = 2,        //获取参考帧时，当回写帧次数不够时，未回写的参考帧赋值为无效，回写的参考帧正常赋值
    VPSS_RGME_PREIMG_MODE_BUTT
}VPSS_RGME_PREIMG_MODE_E;

typedef struct hiVPSS_RGME_ATTR_S{
    HI_U32    u32Width;                //帧信息宽
    HI_U32    u32Height;               //帧信息高 
    HI_DRV_PIX_FORMAT_E ePixFormat;        //帧信息像素格式
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;  //帧信息图像bit位,10 bit or 8 bit
    VPSS_RGME_MODE_E enMode;                       //回写队列工作模式
    VPSS_RGME_PREIMG_MODE_E enRefMode;        //模式值使用模式
    HI_BOOL bSecure;
}VPSS_RGME_ATTR_S;

typedef struct hiVPSS_RGME_S{
    HI_BOOL bInit;                     //初始化标识    
    HI_U32 u32CompleteCount;           //回写链表轮转次数
    VPSS_WBC_MCDEI_DATA_S stDataList[VPSS_RGME_WBC_MAX_NODE];      //回写链表结构
    VPSS_RGME_ATTR_S stRgmeAttr;         //回写链表属性
#ifdef HI_VPSS_SMMU_SUPPORT
    SMMU_BUFFER_S stMMUBuf;            //帧存信息
#else
    MMZ_BUFFER_S stMMZBuf;            //帧存信息
#endif


    SMMU_BUFFER_S stTEEBuf;    


    LIST* pstFirstRef;                  //指向链表第一个参考帧
}VPSS_RGME_S;

HI_S32 VPSS_RGME_Init(VPSS_RGME_S *pstRgme,VPSS_RGME_ATTR_S *pstAttr);
HI_S32 VPSS_RGME_DeInit(VPSS_RGME_S* pstRgme);
//获取回写队列中的参考帧
HI_S32 VPSS_RGME_GetRefInfo(VPSS_RGME_S *pstRgme,
                               HI_DRV_VIDEO_FRAME_S **pstDataList);

//获取回写队列中的回写帧
HI_S32 VPSS_RGME_GetWbcInfo(VPSS_RGME_S *pstRgme,HI_DRV_VIDEO_FRAME_S **pstData);
//设置回写队列轮转
HI_S32 VPSS_RGME_CompleteImage(VPSS_RGME_S* pstRgme);
HI_S32 VPSS_RGME_Reset(VPSS_RGME_S* pstRgme);

//////////////////////////////////////////////////////////////////////

#define VPSS_BLEND_WBC_MAX_NODE 2

typedef enum hiVPSS_BLEND_MODE_E{       
    VPSS_BLEND_MODE_4FIELD = 4,        //MCDEI目前固定4场模式
    VPSS_BLEND_MODE_BUTT
}VPSS_BLEND_MODE_E;

typedef enum hiVPSS_BLEND_BUFMODE_E{
    VPSS_BLEND_BUFMODE_LOWMEM = 1,       //节省内存模式，开辟一场地址空间  
    VPSS_BLEND_BUFMODE_NORMAL = 2,        //正常使用模式，开辟两场地址空间
    VPSS_BLEND_BUFMODE_BUTT
}VPSS_BLEND_BUFMODE_E;

typedef struct hiVPSS_BLEND_ATTR_S{
    HI_U32    u32Width;                //帧信息宽
    HI_U32    u32Height;               //帧信息高 
    HI_DRV_PIX_FORMAT_E ePixFormat;        //帧信息像素格式
    HI_DRV_PIXEL_BITWIDTH_E  enBitWidth;  //帧信息图像bit位,10 bit or 8 bit
    VPSS_BLEND_MODE_E enMode;                       //回写队列工作模式
    VPSS_BLEND_BUFMODE_E enBufMode;        //使用模式
    HI_BOOL bSecure;        //安全模式
}VPSS_BLEND_ATTR_S;

typedef struct hiVPSS_BLEND_S{
    HI_BOOL bInit;                     //初始化标识    
    HI_U32 u32CompleteCount;           //回写链表轮转次数
    VPSS_WBC_MCDEI_DATA_S stDataList[VPSS_BLEND_WBC_MAX_NODE];      //回写链表结构
    VPSS_BLEND_ATTR_S stBlendAttr;         //回写链表属性
#ifdef HI_VPSS_SMMU_SUPPORT
    SMMU_BUFFER_S stMMUBuf;            //帧存信息
#else
    MMZ_BUFFER_S stMMZBuf;            //帧存信息
#endif


    SMMU_BUFFER_S stTEEBuf;

    LIST* pstFirstRef;                  //指向链表第一个参考帧
}VPSS_BLEND_S;

HI_S32 VPSS_BLEND_Init(VPSS_BLEND_S *pstBlend,VPSS_BLEND_ATTR_S *pstAttr);

HI_S32 VPSS_BLEND_DeInit(VPSS_BLEND_S* pstBlend);

//获取回写队列中的参考帧
HI_S32 VPSS_BLEND_GetRefInfo(VPSS_BLEND_S *pstBlend,
                              HI_DRV_VIDEO_FRAME_S **pstDataList);
//获取回写队列中的回写帧
HI_S32 VPSS_BLEND_GetWbcInfo(VPSS_BLEND_S *pstBlend,HI_DRV_VIDEO_FRAME_S **pstData);
//设置回写队列轮转
HI_S32 VPSS_BLEND_CompleteImage(VPSS_BLEND_S* pstBlend);
HI_S32 VPSS_BLEND_Reset(VPSS_BLEND_S* pstBlend);

///////////////////////////////////////////////////////////////////



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__VPSS_WBC_MCDEI_H__*/