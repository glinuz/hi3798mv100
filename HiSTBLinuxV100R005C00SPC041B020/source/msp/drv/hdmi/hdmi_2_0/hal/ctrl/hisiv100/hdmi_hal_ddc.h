/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_ddc.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __HDMI_HAL_DDC_H__
#define __HDMI_HAL_DDC_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#include "drv_hdmi_common.h"
#include "hdmi_reg_aon.h"

#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"

#define DDC_MAX_RECORD_NUM              100
#else
#define DDC_MAX_RECORD_NUM              1
#endif

#define DDC_DEFAULT_TIMEOUT_ACCESS      100
#define DDC_DEFAULT_TIMEOUT_HPD         100
#define DDC_DEFAULT_TIMEOUT_IN_PROG     20
#define DDC_DEFAULT_TIMEOUT_SCL         1
#define DDC_DEFAULT_TIMEOUT_SDA         30
#define DDC_DEFAULT_TIMEOUT_ISSUE       20

typedef enum
{
    DDC_FUNC_TYPE_EDID, //NO ACK
    DDC_FUNC_TYPE_HDCP, //NO ACK
    DDC_FUNC_TYPE_SCDC, //ACK
    DDC_FUNC_TYPE_BUTT,
    
}DDC_FUNC_TYPE_E;

typedef enum
{
	DDC_MASTER_MODE_PWD  = 0x0,
    DDC_MASTER_MODE_AON  = 0x1,
} DDC_MASTER_MODE_E;

typedef enum
{
    DDC_MODE_READ_SINGLE_NO_ACK,    //0b0000
    DDC_MODE_READ_SINGLE_ACK,       //0b0001
    DDC_MODE_READ_MUTIL_NO_ACK,     //0b0010
    DDC_MODE_READ_MUTIL_ACK,        //0b0011
    DDC_MODE_READ_SEGMENT_NO_ACK,   //0b0100
    DDC_MODE_READ_SEGMENT_ACK,      //0b0101
    DDC_MODE_WRITE_MUTIL_NO_ACK,    //0b0110
    DDC_MODE_WRITE_MUTIL_ACK,       //0b0111
    DDC_MODE_BUTT,
    
}DDC_ISSUE_MODE_E;

typedef struct 
{
    HI_BOOL                 bValid;
    U_DDC_MST_CTRL          DDC_MST_CTRL        ; /* 4040 */
    U_DDC_FIFO_RDATA        DDC_FIFO_RDATA      ; /* 4044 */
    U_DDC_FIFO_WDATA        DDC_FIFO_WDATA      ; /* 4048 */
    U_DDC_DATA_CNT          DDC_DATA_CNT        ; /* 404C */
    U_DDC_SLAVE_CFG         DDC_SLAVE_CFG       ; /* 4050 */
    U_DDC_MST_STATE         DDC_MST_STATE       ; /* 4054 */
    U_DDC_MST_CMD           DDC_MST_CMD         ; /* 4058 */
    U_DDC_MAN_CTRL          DDC_MAN_CTRL        ; /* 405C */
    U_DDC_STATE_CLR         DDC_STATE_CLR       ; /* 4060 */
    
}DDC_REG_STATUS_S;

typedef union
{    
    struct {
    HI_U32      bzAccessWaitTimeOut : 1 ;
    HI_U32      bzBusLowScl         : 1 ;
    HI_U32      bzBusLowSda         : 1 ;
    HI_U32      bzErrIsseuMode      : 1 ;
    HI_U32      bzIssueTimeOut      : 1 ;
    HI_U32      bzBusNoAck          : 1 ;
    HI_U32      bzInProgTimeOut     : 1 ;
    HI_U32      bzHpdNoExist        : 1 ;
    HI_U32      bzClrErrFail        : 1 ;
    HI_U32      bzReserved          : 23;
    }u32;

    HI_U32  u32Word;
}DDC_ERR_U;

typedef struct 
{
    HI_U8               u8Segment;
    HI_U8               u8Offset;
    DDC_FUNC_TYPE_E     enFuncType;
    DDC_ISSUE_MODE_E    enIssueMode;
    DDC_MASTER_MODE_E   enMasterMode;       /* AON MODE ONLY FOR DEBUG */
    HI_U32              u32Speed;           //TO DO

    HI_U32              u32AccessTimeOut;
    HI_U32              u32HpdTimeOut;
    HI_U32              u32InProgTimeOut;
    HI_U32              u32SclTimeOut;
    HI_U32              u32SdaTimeOut;
    HI_U32              u32IssueTimeOut;
    
    HI_S32              s32DataSize;
    HI_U8               *pu8Data;
}DDC_CFG_S;

typedef struct
{
	HI_BOOL				bInit;				/* is the DDC init */
	HI_U32				u32ElemPtr;         /* element pointer in the record array */
	HI_U32              u32TotalNum;        /* total record size,max value is DDC_MAX_RECORD_NUM */
	
}DDC_RUN_S;
 
typedef struct
{
    HI_U64              u64StartTime;
    HI_U64              u64TimeLen;
    DDC_ERR_U           unErr;
    DDC_REG_STATUS_S    stRegBefore;
    DDC_REG_STATUS_S    stRegAfter;
    DDC_CFG_S           stCfg;
    HI_S32              s32DataSize;
    HI_U8               *pu8Data;
}DDC_RECORD_ELEM_S;
 

typedef struct
{
    DDC_RUN_S           stDdcRun;                   /* DDC running control info */ 

    DDC_RECORD_ELEM_S   astDdcRecord[DDC_MAX_RECORD_NUM];
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MUTEX          stDdcWrMutex;           /* lock to avoid competition*/
#endif 
}DDC_INFO_S;



HI_S32 HAL_HDMI_DdcInit(HDMI_DEVICE_ID_E enHdmi);

HI_S32 HAL_HDMI_DdcDeInit(HDMI_DEVICE_ID_E enHdmi);

HI_S32 HAL_HDMI_DdcReset(HDMI_DEVICE_ID_E enHdmi);

HI_S32 HAL_HDMI_DdcErrClear(HDMI_DEVICE_ID_E enHdmi);

HI_S32 HAL_HDMI_DdcIssue(HDMI_DEVICE_ID_E enHdmi,DDC_CFG_S *pstCfg);

HI_S32 HAL_HDMI_DdcDefaultCfgGet(HDMI_DEVICE_ID_E enHdmi,DDC_CFG_S *pstCfg);

HI_S32 HAL_HDMI_DdcEdidRawGet(HDMI_DEVICE_ID_E enHdmi,HI_S32 s32Size,HI_U8 *pu8Data);

HI_S32 HAL_HDMI_DdcStatusGet(HDMI_DEVICE_ID_E enHdmi,DDC_RUN_S *pstDdcRun);

HI_S32 HAL_HDMI_DdcElemGet(HDMI_DEVICE_ID_E enHdmi,HI_U32 u32Idx,DDC_RECORD_ELEM_S *pstElem);

//HI_S32 HAL_HDMI_DdcLock(HDMI_DEVICE_ID_E enHdmi,HI_BOOL bEanble);



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __HDMI_HAL_DDC_H__ */   





