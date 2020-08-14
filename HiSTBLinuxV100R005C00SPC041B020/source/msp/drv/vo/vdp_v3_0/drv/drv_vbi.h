#ifndef __DRV_VBI_H__
#define __DRV_VBI_H__


#include "hi_type.h"

#ifndef __DISP_PLATFORM_BOOT__
#include "hi_drv_mem.h"
#include "hi_drv_mmz.h"
#include <linux/list.h>
#include "drv_disp_hal.h"
#include "drv_disp_isr.h"
#endif

#include "hi_drv_disp.h"

/* ckeck the validity of VBI channel */
#define VbiCheckType(enVBIType) \
do {                             \
    if (enVBIType >= HI_DRV_DISP_VBI_TYPE_BUTT) \
    {                               \
        HI_ERR_DISP("VBI: Type is not support!.\n"); \
        return HI_ERR_DISP_INVALID_PARA;\
    }                               \
} while (0)



#define  BUFF_TIMES   2

#define    BUFFER_LEN_7K		  (0x1C00)
#define    BUFFER_LEN_16K		  (BUFF_TIMES*0x4000)

#define    TTX_BLOCK_NUM          (BUFF_TIMES*20)
#define    TTX_PES_MIN_LEN		  92
#define    TTX_PES_MAX_LEN        (1656-6)
#define    TTX_RAW_PACKET_LENGTH  46
#define    TTX_PACKET_LENGTH      176

#define    CC_BLOCK_NUM           20
#define    WSS_BLOCK_NUM          20
#define    VBI_PES_MIN_LEN        15
#define    VBI_PES_MAX_LEN		  (184-6)

typedef enum tagCC_SEND_STEP_E
{
    CC_SENDSTEP_START  = 0x0000,
    CC_SENDSTEP_PROCESSING,       
    CC_SENDSTEP_END
} CC_SEND_STEP_E;

typedef enum hiDISP_VBI_TYPE_E
{
    DISP_VBI_TYPE_TTX = 0,                   /**<teltext type*//**<CNcomment:teltext类型*/
    DISP_VBI_TYPE_CC,                        /**<closed caption type*//**<CNcomment: 隐藏字幕类型*/
    DISP_VBI_TYPE_VCHIP,                     /**<v-chip type*//**<CNcomment: v-chip类型*/
    DISP_VBI_TYPE_WSS,                       /**<wide screen signal*//**<CNcomment:宽屏信令*/
    DISP_VBI_TYPE_BUTT
}DISP_VBI_TYPE_E;

typedef struct hiWSS_DATA_BLOCK_S
{
#ifndef __DISP_PLATFORM_BOOT__
	struct list_head List;
	HI_U16   TopData;
#endif
}WSS_DATA_BLOCK_S;

typedef struct hiTTX_DATA_BLOCK_S
{
#ifndef __DISP_PLATFORM_BOOT__
	struct list_head List;
    HI_U8    *pTtxDataVirtAddr;             /* virtual address of BUFFER kernel */
    HI_U32   TtxStartAddr;                  /* start physical address of TTX data */
    HI_U32   TtxEndAddr;                    /* end physical address of TTX data */
    HI_U32   TtxDataLine;                   /* transform line of TTX data */
	HI_U32   TtxDataExtLine;                /* extended transform line of TTX data */
	HI_BOOL  TtxSeqFlag;                    /* sending order of TTX data, HI_TRUE is sent from MSB */
    HI_U8    TtxPackConut;
#endif
}TTX_DATA_BLOCK_S;

typedef struct hiCC_DATA_BLOCK_S
{
#ifndef __DISP_PLATFORM_BOOT__
	struct list_head List;
	HI_BOOL  TopFlag;
	HI_BOOL  BottomFlag;
	HI_U16   TopLine;
	HI_U16   BottomLine;
	HI_U16   TopData;
	HI_U16   BottomData;
#endif
}CC_DATA_BLOCK_S;

typedef struct hiDISP_VBI_STATUS_S
{
    DISP_VBI_TYPE_E enVBIType;
    HI_BOOL         bOpen;
    HI_HANDLE   u32Handle;
}HI_DISP_VBI_STATUS_S;

typedef struct hiDISP_VBI_WSS_DATA_S
{
    HI_BOOL bEnable;                            /**<WSS configure enable HI_TRUE: enable,HI_FALSE: disnable*//**<CNcomment:WSS配置使能。HI_TRUE：使能；HI_FALSE：禁止*/
    HI_U16  u16Data;                            /**<Wss data */ /**<CNcomment:Wss数据*/
}WSS_DATA_S;

typedef struct TTX_SEND_S
{
#ifndef __DISP_PLATFORM_BOOT__
    MMZ_BUFFER_S       TtxPesBuf;
    HI_U8              *pPesBufVirtAddr;
    HI_U32             TtxPesBufWrite;

    MMZ_BUFFER_S       TtxDataBuf;
    HI_U8              *pTtxBufVirtAddr;

    struct list_head   TtxBlockFreeHead;
    struct list_head   TtxBlockBusyHead;
    TTX_DATA_BLOCK_S   TtxDataBlock[TTX_BLOCK_NUM];
    struct list_head   *pTtxBusyList;
    TTX_DATA_BLOCK_S   *pTtxBusyBlock;
    HI_BOOL            TtxBusyFlag;
    HI_BOOL            TtxListFlag;
#endif
}TTX_SEND_S;

typedef struct VBI_SEND_S
{
#ifndef __DISP_PLATFORM_BOOT__
    MMZ_BUFFER_S           VbiPesBuf;
    HI_U8                  *pPesBufVirtAddr;
    HI_U32                 VbiPesBufWrite;

    struct list_head       CcBlockFreeHead;
    struct list_head       CcBlockBusyHead;
    CC_DATA_BLOCK_S        CcDataBlock[CC_BLOCK_NUM];
    struct list_head       *pCcBusyList;
    CC_DATA_BLOCK_S        *pCcBusyBlock;
    HI_BOOL                CcListFlag;
    CC_SEND_STEP_E           CcSendStep;

    struct list_head       WssBlockFreeHead;
    struct list_head       WssBlockBusyHead;
    WSS_DATA_BLOCK_S       WssDataBlock[WSS_BLOCK_NUM];
    struct list_head       *pWssBusyList;
    WSS_DATA_BLOCK_S       *pWssBusyBlock;
    HI_BOOL                WssListFlag;

    WSS_DATA_S  WssData;
#endif
}VBI_SEND_S;

typedef struct CC_SEND_S
{
   HI_BOOL TopFlag;
   HI_BOOL BottomFlag;
   HI_U16 TopData;
   HI_U16 BottomData; 
   HI_U16 TopLine;
   HI_U16 BottomLine;
}CC_SEND_S;


HI_S32 DRV_VBI_CreateChannel(HI_DRV_DISP_VBI_CFG_S* pstCfg, HI_HANDLE *phVbi);

HI_S32 DRV_VBI_DestoryChannel(HI_HANDLE hVbi);

HI_S32 DRV_VBI_SendData(HI_HANDLE hVbi, HI_DRV_DISP_FMT_E enFmt,HI_DRV_DISP_VBI_DATA_S* pstVbiData);

HI_S32 DRV_VBI_Init(HI_VOID);

HI_S32 DRV_VBI_DeInit(HI_VOID);

#endif
