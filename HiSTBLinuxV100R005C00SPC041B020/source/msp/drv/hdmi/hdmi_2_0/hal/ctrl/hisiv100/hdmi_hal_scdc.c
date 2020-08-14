/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_scdc.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/


//#include <linux/kernel.h>

#ifndef HDMI_BUILD_IN_BOOT
#include "drv_hdmi_platform.h"
#include "drv_hdmi_intf.h"
#endif
#include "hdmi_product_define.h"
#include "hdmi_hal_phy.h"
#include "hdmi_hal_scdc.h"
#include "hdmi_hal_ddc.h"
#include "hdmi_hal_machine.h"
#include "hdmi_hal_intf.h"

#include "hdmi_reg_tx.h"

#define SCDC_HDMI20_VERSION         1
#define SCDC_UPDATE_TIME_INTERVAL   150


typedef struct {
    HI_BOOL     bSrcScramble;
    HI_BOOL     bSinkScramble;
    HI_BOOL     bTmdsClkRatio40x;
}SCDC_SCRAMBLE_S;


#define SCDC_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define SCDC_INIT_CHK(pstScdcInfo) do{\
	SCDC_NULL_CHK(pstScdcInfo);\
	if(pstScdcInfo->stStatus.bInit != HI_TRUE)\
	{HDMI_WARN("hdmi scdc module didn't init!\n");\
	return HI_FAILURE;}\
}while(0)

/****************************  private data *****************************/


static SCDC_INFO_S s_stScdcInfo[HDMI_DEVICE_ID_BUTT];


/****************************  private interface *****************************/

static SCDC_INFO_S *ScdcInfoPtrGet(HDMI_DEVICE_ID_E enHdmiId)
{
    if (enHdmiId < HDMI_DEVICE_ID_BUTT)
    {
        return &s_stScdcInfo[enHdmiId];
    }
    return HI_NULL;

}

static HI_S32 ScdcScrambleSet(HDMI_DEVICE_ID_E enHdmiId,SCDC_SCRAMBLE_S *pstScramble)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    SCDC_TMDS_CONFIG_U      u8DdcConfigByte;
    DDC_CFG_S               stDdcSrcCfg ;

    SCDC_NULL_CHK(pstScramble);

    /* source scramble */ 
    //HDMI_HDMI_ENC_CTRL_enc_hdmi2_onSet(pstScrambleStat->bSinkScramble);
    HDMI_HDMI_ENC_CTRL_enc_scr_onSet(pstScramble->bSrcScramble);
    
    /* sink scramble */
    u8DdcConfigByte.u8.bzScreamblingEnable     = pstScramble->bSinkScramble ? HI_TRUE : HI_FALSE;
    u8DdcConfigByte.u8.bzTmdsBitClockRatio     = pstScramble->bTmdsClkRatio40x ? HI_TRUE : HI_FALSE;

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcSrcCfg);
    stDdcSrcCfg.enFuncType                  = DDC_FUNC_TYPE_SCDC;
    stDdcSrcCfg.enIssueMode                 = DDC_MODE_WRITE_MUTIL_ACK;
    stDdcSrcCfg.u8Offset                    = SCDC_OFFSET_TMDS_CONFIG;
    stDdcSrcCfg.pu8Data                     = &u8DdcConfigByte.u8Byte;
    stDdcSrcCfg.s32DataSize                 = 1;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSrcCfg);
    
    return (s32Ret==stDdcSrcCfg.s32DataSize) ? HI_SUCCESS : HI_FAILURE;
}

static HI_S32 ScdcScrambleGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_SCRAMBLE_S *pstScramble)
{
    HI_S32                  s32Ret = HI_SUCCESS;
    SCDC_SCRAMBLER_STATUS_U u8DdcSrambleByte;
    SCDC_TMDS_CONFIG_U      u8DdcConfigByte;
    DDC_CFG_S               stDdcSinkCfg;
    SCDC_INFO_S             *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScramble);

    /* source scrambled */ 
    pstScramble->bSrcScramble = HDMI_HDMI_ENC_CTRL_enc_scr_onGet() ? HI_TRUE : HI_FALSE;

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcSinkCfg);
    /* sink scrambled status */
    stDdcSinkCfg.enFuncType                  = DDC_FUNC_TYPE_SCDC;
    stDdcSinkCfg.enIssueMode                 = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcSinkCfg.s32DataSize                 = 1;
    
    stDdcSinkCfg.u8Offset                    = SCDC_OFFSET_SCRAMBLER_STATUS;
    u8DdcSrambleByte.u8Byte                  = 0;
    stDdcSinkCfg.pu8Data                     = &u8DdcSrambleByte.u8Byte;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSinkCfg);
    pstScramble->bSinkScramble  = u8DdcSrambleByte.u8.bzScreamblingStatus ? HI_TRUE : HI_FALSE;
    pstScdcInfo->stStatus.astRegUpdate[pstScdcInfo->stStatus.u32RegUpdatePtr].u8ScrambleStatus.u8Byte = u8DdcSrambleByte.u8Byte;
    
    /* sink tmds bit clock ratio & scramble cfg */
    u8DdcSrambleByte.u8Byte                 = 0;
    stDdcSinkCfg.pu8Data                    = &u8DdcConfigByte.u8Byte;
    stDdcSinkCfg.u8Offset                   = SCDC_OFFSET_TMDS_CONFIG;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcSinkCfg);
    pstScramble->bTmdsClkRatio40x    = u8DdcConfigByte.u8.bzTmdsBitClockRatio ? HI_TRUE : HI_FALSE;
    pstScdcInfo->stStatus.stRegStatic.u8TmdsConfig0.u8Byte = u8DdcConfigByte.u8Byte;
 
    return s32Ret==stDdcSinkCfg.s32DataSize ? HI_SUCCESS : HI_FAILURE;
}

HI_VOID ScdcMachPollCallBack(HI_VOID *pData)
{
    HI_U8                       u8DdcByte = 0;
    DDC_CFG_S                   stDdcCfg = {0};
    SCDC_INFO_S                 *pstScdcInfo = (SCDC_INFO_S *)pData;
    SCDC_REG_UPDATE_STATUS_S    *pstRegUpdate = HI_NULL;
    HDMI_DEVICE_ID_E            enHdmiId = 0;

    if(!pstScdcInfo)
    {
        HDMI_WARN("pstScdcInfo is null\n");
        return ; 
    }
    if(!pstScdcInfo->stStatus.bInit)
    {
        HDMI_WARN("module hdcp14 didn't init\n");
        return ;
    }
    
    enHdmiId = pstScdcInfo->stStatus.enHdmiId;
    
    if (!pstScdcInfo->stAttr.bSinkReadQuest)
    {
        pstRegUpdate  = &pstScdcInfo->stStatus.astRegUpdate[pstScdcInfo->stStatus.u32RegUpdatePtr];
        pstScdcInfo->stStatus.u32RegUpdatePtr = (pstScdcInfo->stStatus.u32RegUpdatePtr+1) % SCDC_MAX_UPDATE_RECORD_NUM;
 
        HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
        stDdcCfg.s32DataSize    = 1;
        stDdcCfg.pu8Data        = &u8DdcByte;
        stDdcCfg.enFuncType     = DDC_FUNC_TYPE_SCDC;
        stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_ACK;
     
        /* read Update */
        stDdcCfg.u8Offset       = SCDC_OFFSET_UPDATE_0;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u8Update0.u8Byte = u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_STATUS_FLAG_0;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u8StatusFlag0.u8Byte= u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_SCRAMBLER_STATUS;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u8ScrambleStatus.u8Byte= u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_CHKSUM;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u8ErrChkSum= u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_0_H;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch0Err.u16Word = u8DdcByte;
        pstRegUpdate->u16Ch0Err.u16Word <<= 8;
        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_0_L;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch0Err.u16Word |= u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_1_H;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch1Err.u16Word = u8DdcByte;
        pstRegUpdate->u16Ch1Err.u16Word <<= 8;
        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_1_L;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch1Err.u16Word |= u8DdcByte;

        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_2_H;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch1Err.u16Word = u8DdcByte;
        pstRegUpdate->u16Ch1Err.u16Word <<= 8;
        stDdcCfg.u8Offset       = SCDC_OFFSET_DRR_DET_2_L;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        pstRegUpdate->u16Ch1Err.u16Word |= u8DdcByte;

    }

    return ;
}

/****************************  public interface *****************************/

HI_S32 HAL_HDMI_ScdcInit(HDMI_DEVICE_ID_E enHdmiId)
{

    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
#ifndef HDMI_BUILD_IN_BOOT
    HDMI_MACH_CTRL_S    stMachCtrl = {0};
#endif
    SCDC_NULL_CHK(pstScdcInfo);

    if (!pstScdcInfo->stStatus.bInit)
    {
#ifndef HDMI_BUILD_IN_BOOT
        stMachCtrl.pCbData      = pstScdcInfo;
        stMachCtrl.u64Interval  = SCDC_DEFAULT_POLL_INTERVAL;
        stMachCtrl.pfnCallBack  = ScdcMachPollCallBack;
        stMachCtrl.pu8Name      = "SCDC"; 
#endif        
        HDMI_MEMSET(pstScdcInfo, 0 , sizeof(SCDC_INFO_S));
        pstScdcInfo->stStatus.bInit             = HI_TRUE;
#ifndef HDMI_BUILD_IN_BOOT
        pstScdcInfo->stStatus.enHdmiId          = enHdmiId;
        pstScdcInfo->stStatus.u64MachInterval   = stMachCtrl.u64Interval;
        s32Ret = HAL_HDMI_MachRegister(&stMachCtrl,&pstScdcInfo->stStatus.u32MachId);
#endif
    }

    return s32Ret;
}

HI_S32 HAL_HDMI_ScdcDeinit(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
    
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);
#ifndef HDMI_BUILD_IN_BOOT
    s32Ret = HAL_HDMI_MachUnRegister(pstScdcInfo->stStatus.u32MachId);
#endif
    HDMI_MEMSET(pstScdcInfo, 0 , sizeof(SCDC_INFO_S));
    pstScdcInfo->stStatus.bInit = HI_FALSE;
    return s32Ret;
}


HI_S32 HAL_HDMI_ScdcReset(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32          bInitBack = HI_FALSE;
    HI_U32          u32MachIdBack = 0;
    SCDC_SCRAMBLE_S stScramble = {0};
    SCDC_INFO_S     *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
    
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_MachStop(pstScdcInfo->stStatus.u32MachId);
#endif
    bInitBack       = pstScdcInfo->stStatus.bInit;
    u32MachIdBack   = pstScdcInfo->stStatus.u32MachId;
    HDMI_MEMSET(pstScdcInfo,0,sizeof(SCDC_INFO_S));
    pstScdcInfo->stStatus.bInit     = bInitBack;
    pstScdcInfo->stStatus.u32MachId = u32MachIdBack;
    pstScdcInfo->stStatus.enHdmiId  = enHdmiId;

    stScramble.bSrcScramble     = HI_FALSE;
    stScramble.bSinkScramble    = HI_FALSE;
    stScramble.bTmdsClkRatio40x = HI_FALSE;
    ScdcScrambleSet(enHdmiId, &stScramble);
    
    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_ScdcAttrSet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr)
{
    HI_U8                   u8DdcByte = 0;
    HI_S32                  s32Ret = HI_SUCCESS;
    HI_BOOL                 bOeEabnle = HI_FALSE;
    HI_U32                  u32TimeOut = 0 ;
    SCDC_SCRAMBLE_S         stScramble = {0};
    DDC_CFG_S               stDdcCfg = {0};
#ifndef HDMI_BUILD_IN_BOOT
    SCDC_ERROR_RECORD_S     *pstErrCord = HI_NULL;
#endif
    SCDC_INFO_S             *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pstScdcAttr);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    if ((pstScdcAttr->u32ScrambleInterval < SCDC_DEFAULT_SCRAMBLE_INTERVAL)
        || (pstScdcAttr->u32ScrambleInterval > SCDC_DEFAULT_SCRAMBLE_TIMEOUT))
    {
        pstScdcAttr->u32ScrambleInterval = SCDC_DEFAULT_SCRAMBLE_INTERVAL;
    }
    
    if (pstScdcAttr->u32ScrambleTimeout < SCDC_DEFAULT_SCRAMBLE_TIMEOUT )
    {
        pstScdcAttr->u32ScrambleTimeout = SCDC_DEFAULT_SCRAMBLE_TIMEOUT;
    }
    else if (pstScdcAttr->u32ScrambleTimeout > SCDC_MAX_SCRAMBLE_TIMEOUT )

    {
        pstScdcAttr->u32ScrambleTimeout = SCDC_MAX_SCRAMBLE_TIMEOUT;
    }
    else
    {
        //nothing
    }
    
    HDMI_MEMCPY(&pstScdcInfo->stAttr,pstScdcAttr,sizeof(SCDC_ATTR_S));

    /* read sink version */
    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.s32DataSize    = 1;
    stDdcCfg.pu8Data        = &u8DdcByte;
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_SCDC;
    stDdcCfg.u8Offset       = SCDC_OFFSET_SINK_VERSION;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_ACK;
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
#endif
    pstScdcInfo->stStatus.stRegStatic.u8SinkVersion = u8DdcByte;
    if (u8DdcByte != SCDC_HDMI20_VERSION)
    {
        pstScdcInfo->stStatus.stErrCnt.u32SinkUnReadCnt++;
        //pstErrCord                  = &pstScdcInfo->stStatus.astErrRecord[pstScdcInfo->stStatus.u32ErrRecordPtr];
        //pstErrCord->enErrType       = SCDC_ERROR_SINK_UNREAD;
        //pstErrCord->u64TimeStamp    = HAL_HDMI_MachMsGet();
        //HDMI_MEMCPY(&pstErrCord->stErrAttr, &pstScdcInfo->stAttr ,sizeof(SCDC_ATTR_S) );
        //pstScdcInfo->stStatus.u32ErrRecordPtr = (1+pstScdcInfo->stStatus.u32ErrRecordPtr) % SCDC_MAX_ERROR_RECORD_NUM;
        HDMI_WARN("scdc sink version is not %u\n",SCDC_HDMI20_VERSION);
    }

    /* RR_Enable = disable */
    pstScdcInfo->stAttr.bSinkReadQuest = HI_FALSE;
    pstScdcInfo->stStatus.stRegStatic.u8Config0.u8Byte  = 0;
    pstScdcInfo->stStatus.stRegStatic.u8Config0.u8.bzRrEnable = HI_FALSE;
    stDdcCfg.u8Offset       = SCDC_OFFSET_CONFIG_0;
    stDdcCfg.enIssueMode    = DDC_MODE_WRITE_MUTIL_ACK;
    stDdcCfg.pu8Data        = &pstScdcInfo->stStatus.stRegStatic.u8Config0.u8Byte;
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
#endif
    pstScdcInfo->stStatus.stRegStatic.u8RrTestCfg.u8Byte = 0;
    stDdcCfg.u8Offset       = SCDC_OFFSET_TEST_CONFIG_0;
    stDdcCfg.enIssueMode    = DDC_MODE_WRITE_MUTIL_ACK;
    stDdcCfg.pu8Data        = &pstScdcInfo->stStatus.stRegStatic.u8RrTestCfg.u8Byte;
#ifndef HDMI_BUILD_IN_BOOT
    HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
#endif
    /* scrameble cfg */
    u32TimeOut = pstScdcInfo->stAttr.u32ScrambleInterval;
    do{
        /* oe disable */
        HAL_HDMI_PhyOeSet(HI_FALSE);
        msleep(5);
        HAL_HDMI_PhyOeGet(&bOeEabnle);
#ifndef HDMI_BUILD_IN_BOOT
        if (bOeEabnle && (pstScdcInfo->stStatus.stErrCnt.u32OeDisableFailCnt < SCDC_ERROR_MAX_NUM_OE_DISABLE) )
        {
            pstScdcInfo->stStatus.stErrCnt.u32OeDisableFailCnt++;
            pstErrCord                  = &pstScdcInfo->stStatus.astErrRecord[pstScdcInfo->stStatus.u32ErrRecordPtr];
            pstErrCord->enErrType       = SCDC_ERROR_OE_DISABLE;
            pstErrCord->u64TimeStamp    = HAL_HDMI_MachMsGet();
            HDMI_MEMCPY(&pstErrCord->stErrAttr, &pstScdcInfo->stAttr ,sizeof(SCDC_ATTR_S) );
            pstScdcInfo->stStatus.u32ErrRecordPtr = (1+pstScdcInfo->stStatus.u32ErrRecordPtr) % SCDC_MAX_ERROR_RECORD_NUM;
             
            HDMI_WARN("scdc oe bOeEnable=%u(expected 0)\n",bOeEabnle);
        }
        else if (bOeEabnle)
        {
            HDMI_ERR("scdc oe disable fail!\n");
            s32Ret = HI_FAILURE;
        }
#endif
        /* sink & source scramble config */
        stScramble.bSrcScramble     = pstScdcInfo->stAttr.bSrcScramble;
        stScramble.bSinkScramble    = pstScdcInfo->stAttr.bSinkScramble;
        stScramble.bTmdsClkRatio40x = pstScdcInfo->stAttr.bTmdsClkRatio40x;
        ScdcScrambleSet(enHdmiId,&stScramble);

        /* oe enable */
        msleep(u32TimeOut);
        HAL_HDMI_PhyOeSet(HI_TRUE);
        HAL_HDMI_PhyOeGet(&bOeEabnle);
#ifndef HDMI_BUILD_IN_BOOT
        if ( (!bOeEabnle) && (pstScdcInfo->stStatus.stErrCnt.u32OeEnableFailCnt < SCDC_ERROR_MAX_NUM_OE_ENABLE) )
        {
            pstScdcInfo->stStatus.stErrCnt.u32OeEnableFailCnt++;
            pstErrCord                  = &pstScdcInfo->stStatus.astErrRecord[pstScdcInfo->stStatus.u32ErrRecordPtr];
            pstErrCord->enErrType       = SCDC_ERROR_OE_ENABLE;
            pstErrCord->u64TimeStamp    = HAL_HDMI_MachMsGet();
            HDMI_MEMCPY(&pstErrCord->stErrAttr, &pstScdcInfo->stAttr ,sizeof(SCDC_ATTR_S) );
            pstScdcInfo->stStatus.u32ErrRecordPtr = (1+pstScdcInfo->stStatus.u32ErrRecordPtr) % SCDC_MAX_ERROR_RECORD_NUM;
            HDMI_WARN("scdc oe bOeEnable=%u(expected 1)\n",bOeEabnle);
        }
        else if(!bOeEabnle)
        {
            HDMI_ERR("scdc oe enable fail!\n");
            s32Ret = HI_FAILURE;
        }
#endif
        /*poll status*/
        ScdcScrambleGet(enHdmiId,&stScramble);        
        msleep(1);
        /*  Compatibility try again */                            
        if ((stScramble.bTmdsClkRatio40x == pstScdcInfo->stAttr.bTmdsClkRatio40x))
            //&&(stScramble.bSinkScramble == pstScdcInfo->stAttr.bSinkScramble) )
        {
            s32Ret = HI_SUCCESS;
        }
        else 
        {
#ifndef HDMI_BUILD_IN_BOOT
            pstScdcInfo->stStatus.stErrCnt.u32SrambleFailCnt++;
            pstErrCord                  = &pstScdcInfo->stStatus.astErrRecord[pstScdcInfo->stStatus.u32ErrRecordPtr];
            pstErrCord->enErrType       = SCDC_ERROR_SCRAMBLE;
            pstErrCord->u64TimeStamp    = HAL_HDMI_MachMsGet();
            HDMI_MEMCPY(&pstErrCord->stErrAttr, &pstScdcInfo->stAttr ,sizeof(SCDC_ATTR_S) );
            pstScdcInfo->stStatus.u32ErrRecordPtr = (1+pstScdcInfo->stStatus.u32ErrRecordPtr) % SCDC_MAX_ERROR_RECORD_NUM;
#endif
            s32Ret = HI_FAILURE;
        }

        u32TimeOut  += pstScdcInfo->stAttr.u32ScrambleInterval;

    }while((s32Ret != HI_SUCCESS) && (u32TimeOut <= pstScdcInfo->stAttr.u32ScrambleTimeout));


    HDMI_INFO("\noe=%u,u32TimeOut=%u,bSrcScarmble=%d,bSinkScramble=%u,bTmdsClkRatio40x=%u\n",
                                bOeEabnle,
                                u32TimeOut,
                                stScramble.bSrcScramble,
                                stScramble.bSinkScramble,
                                stScramble.bTmdsClkRatio40x);
    
    if (s32Ret != HI_SUCCESS)
    {
        HDMI_ERR("scdc scramble %s fail!\n",pstScdcInfo->stAttr.bSinkScramble ? "ON" : "OFF");
    }
    else
    {
        HDMI_INFO("scdc scramble %s success!\n",pstScdcInfo->stAttr.bSinkScramble ? "ON" : "OFF");
    }
#if 0
    /* machine */
    HAL_HDMI_MachCfgGet(pstScdcInfo->stStatus.u32MachId,&stMachCtrl);
    if (pstScdcAttr->u64MachPollInterval < SCDC_DEFAULT_POLL_INTERVAL)
    {
        stMachCtrl.u64Interval                  = SCDC_DEFAULT_POLL_INTERVAL;
        pstScdcInfo->stAttr.u64MachPollInterval = SCDC_DEFAULT_POLL_INTERVAL;
    }
    else
    {
        stMachCtrl.u64Interval                  = pstScdcAttr->u64MachPollInterval;
        pstScdcInfo->stAttr.u64MachPollInterval = pstScdcAttr->u64MachPollInterval;
    
    }
    HAL_HDMI_MachCfgGet(pstScdcInfo->stStatus.u32MachId,&stMachCtrl);
#endif
#ifndef HDMI_BUILD_IN_BOOT

    if ( (s32Ret==HI_SUCCESS) && pstScdcInfo->stAttr.bSinkScramble)
    {
        //s32Ret |= HAL_HDMI_MachStart(pstScdcInfo->stStatus.u32MachId);
        //pstScdcInfo->stStatus.bMachEnable = HI_TRUE;
    }
    else
    {
        s32Ret |= HAL_HDMI_MachStop(pstScdcInfo->stStatus.u32MachId);
        pstScdcInfo->stStatus.bMachEnable = HI_FALSE;
    }
#endif
    return s32Ret;
}


HI_S32 HAL_HDMI_ScdcAttrGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_ATTR_S *pstScdcAttr)
{
    HI_S32              s32Ret = HI_SUCCESS;
    SCDC_SCRAMBLE_S     stScramble = {0};
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
    
    SCDC_NULL_CHK(pstScdcAttr);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    ScdcScrambleGet(enHdmiId, &stScramble);
    pstScdcInfo->stAttr.bSinkScramble      = stScramble.bSinkScramble;
    pstScdcInfo->stAttr.bSrcScramble       = stScramble.bSrcScramble;
    pstScdcInfo->stAttr.bTmdsClkRatio40x   = stScramble.bTmdsClkRatio40x;

    HDMI_MEMCPY(pstScdcAttr,&pstScdcInfo->stAttr,sizeof(SCDC_ATTR_S));

    return s32Ret;
}



/* real sw */
HI_S32 HAL_HDMI_ScdcAllStatusGet(HDMI_DEVICE_ID_E enHdmiId,SCDC_STATUS_S *pstStatus)
{
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
    
    SCDC_NULL_CHK(pstStatus);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    HDMI_MEMCPY(pstStatus,&pstScdcInfo->stStatus,sizeof(SCDC_STATUS_S));

    return HI_SUCCESS;
}

HI_S32 HAL_HDMI_ScdcSupportGet(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL *pbSupport)
{
    HI_U8               u8DdcByte = 0;
    HI_S32              s32Ret = 0;
    HI_U32              u32ReReadCnt = 0;
    DDC_CFG_S           stDdcCfg = {0};    
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);

    SCDC_NULL_CHK(pbSupport);
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.s32DataSize    = 1;
    stDdcCfg.pu8Data        = &u8DdcByte;
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_SCDC;
    stDdcCfg.u8Offset       = SCDC_OFFSET_SRC_VERSION;

    do {
        /* write source version */    
        stDdcCfg.enIssueMode     = DDC_MODE_WRITE_MUTIL_ACK;
        u8DdcByte                = SCDC_HDMI20_VERSION ;
        HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
        
        /* read source version */
        stDdcCfg.enIssueMode     = DDC_MODE_READ_MUTIL_ACK;   
        u8DdcByte                = 0;
        s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

        u32ReReadCnt++;
    }while((s32Ret != stDdcCfg.s32DataSize) && (u32ReReadCnt < 2));

    if ((s32Ret==stDdcCfg.s32DataSize) && (u8DdcByte == SCDC_HDMI20_VERSION))
    {
        pstScdcInfo->stStatus.stRegStatic.u8SrcVersion = SCDC_HDMI20_VERSION;
        *pbSupport = HI_TRUE;
    }
    else
    {
        pstScdcInfo->stStatus.stRegStatic.u8SrcVersion = 0;
        *pbSupport = HI_FALSE;
    }
    
    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_ScdcScrambleForce(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable)
{

    SCDC_SCRAMBLE_S stScramble;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);   
    
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);
    
    if (bEnable)
    {
        stScramble.bSinkScramble                = HI_TRUE;
        stScramble.bTmdsClkRatio40x             = HI_TRUE;
        stScramble.bSrcScramble                 = HI_TRUE;

        pstScdcInfo->stAttr.bSinkScramble       = HI_TRUE;
        pstScdcInfo->stAttr.bSrcScramble        = HI_TRUE;
        pstScdcInfo->stAttr.bTmdsClkRatio40x    = HI_TRUE;
        
    }
    else
    {
        stScramble.bSinkScramble                = HI_FALSE;
        stScramble.bTmdsClkRatio40x             = HI_FALSE;
        stScramble.bSrcScramble                 = HI_FALSE;

        pstScdcInfo->stAttr.bSinkScramble       = HI_FALSE;
        pstScdcInfo->stAttr.bSrcScramble        = HI_FALSE;
        pstScdcInfo->stAttr.bTmdsClkRatio40x    = HI_FALSE;
    }

    return ScdcScrambleSet(enHdmiId,&stScramble);
}


HI_S32 HAL_HDMI_ScdcMachEnable(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable)
{

    HI_S32 s32Ret       = HI_SUCCESS;
    SCDC_INFO_S         *pstScdcInfo = ScdcInfoPtrGet(enHdmiId);
    
    SCDC_NULL_CHK(pstScdcInfo);
    SCDC_INIT_CHK(pstScdcInfo);
#ifndef HDMI_BUILD_IN_BOOT
    if (bEnable)
    {
        s32Ret = HAL_HDMI_MachStart(pstScdcInfo->stStatus.u32MachId);
    }
    else
    {
        s32Ret = HAL_HDMI_MachStop(pstScdcInfo->stStatus.u32MachId);
    }
#endif
    return s32Ret;
}







