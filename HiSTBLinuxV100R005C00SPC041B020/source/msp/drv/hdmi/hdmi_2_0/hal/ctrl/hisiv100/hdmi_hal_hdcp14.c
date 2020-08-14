/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : hdmi_hal_hdcp14.c
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/08/13
  Description   :
  History       :
  Date          : 2015/08/13
  Author        : t00273561
  Modification  :
*******************************************************************************/

#include <linux/jiffies.h>
#include <linux/delay.h>

#include "hdmi_reg_aon.h"
#include "hdmi_reg_ctrl.h"


#include "hdmi_hal_ddc.h"
#include "hdmi_reg_hdcp.h"
#include "hdmi_hal_intf.h"

#include "hdmi_platform.h"
#include "hdmi_product_define.h"
#ifdef HDMI_TEE_SUPPORT
#include "drv_hdmi_tee.h"
#endif




/*
New value can be safely written into this register only after getting command done bit (reg. 0x0F9[0]) or waiting at least 0.125s.
That means that new value can be safely written into this register only after previous command has been completed. Otherwise;
it is possible to get either self-authentication #1 error status bit or to get BIST done bit without actually performing one of the new enabled BIST test.
*/
typedef enum
{
    HDCP14_KEY_CMD_CLR              =0x00,//b00000: Clear command register; no action will be taken ;
    //HDCP14_KEY_CMD_,//5'bxxx01: reserved ;
    //HDCP14_KEY_CMD_,//5'bxxx10: reserved ;
    HDCP14_KEY_CMD_ALL_BIST_ENABLE  =0x03,//5'bxxx11: will enable all BIST: CRC, self-authentication #1 and self_authentication #2 ;
    HDCP14_KEY_CMD_CRC_ENABLE       =0x04,//5'bxx100: will enable individual CRC ;
    HDCP14_KEY_CMD_SELF_AUTH1_ENABLE=0x05,//5'bx1x00: will enable individual self-authentication #1 ;
    HDCP14_KEY_CMD_SELF_AUTH2_ENABLE=0x06,//5'b1xx00: will enable individual self_authentication #2 ;
}HDCP14_KEY_CMD_E;

typedef enum
{
    HDCP14_CHK_STAT_ERR         = 0xb,
    HDCP14_CHK_STAT_ERR_RI_CMP  = 0xc,
}HDCP14_CHK_STAT_E;

#define  HDCP14_DEBUG_KEY 0

/* HDCP1.4 offset define (slave=0x74),see HDCP14 spec page 29 */
#define HDCP14_OFFSET_BKSV						0x00
#define HDCP14_OFFSET_BRi						0x08
#define HDCP14_OFFSET_BPj				        0x0a
#define HDCP14_OFFSET_AKSV      				0x10
#define HDCP14_OFFSET_Ainfo					0x15
#define HDCP14_OFFSET_An        			    0x18
#define HDCP14_OFFSET_BVH0       				0x20
#define HDCP14_OFFSET_BVH1       				0x24
#define HDCP14_OFFSET_BVH2       				0x28
#define HDCP14_OFFSET_BVH3       				0x2c
#define HDCP14_OFFSET_BVH4       				0x30
#define HDCP14_OFFSET_Bcaps       				0x40
#define HDCP14_OFFSET_Bstatus         			0x41
#define HDCP14_OFFSET_KSV_FIFO					0x43
#define HDCP14_OFFSET_DBG				        0xc0


#define HDCP14_SHA_WAIT_TIMEOUT                 30
#define HDCP14_VI_WAIT_TIMEOUT                  30

#define HDCP14_WAIT_LOAD_TIMEOUT                30
#define HDCP14_WAIT_CRC_TIMEOUT                 300
#define HDCP14_BR0_WAIT_TIMEOUT                 100
#define HDCP14_DEFUALT_INTERVAL                 10
#define HDCP14_PIII_Ri_NORMAL_INTERVAL          2000
#define HDCP14_PIII_Ri_ERROR_INTERVAL           10

#define HDCP14_REAUTH_WAIT_MS_AN_FAIL           12
#define HDCP14_REAUTH_WAIT_MS_R0_FAIL           2100

#ifndef HDMI_FPGA_SUPPORT
#define HDCP14_REAUTH_WAIT_MS_ENCYP             100
#define HDCP14_REAUTH_WAIT_MS_BKSV_FAIL         2100
#define HDCP14_BKSVLIST_TIMEOUT                 5000
#define HDCP14_REAUTH_WAIT_MS_Ri_FAIL           2130
#else
#define HDCP14_REAUTH_WAIT_MS_ENCYP             100     //FPGA value,un-test
#define HDCP14_REAUTH_WAIT_MS_Ri_FAIL           213     //FPGA value,1a-07a
#define HDCP14_BKSVLIST_TIMEOUT                 482     //FPGA value,1b-03
#define HDCP14_REAUTH_WAIT_MS_BKSV_FAIL         210     //FPGA value,1a-07
#endif



#define HDCP14_NULL_CHK(p) do{\
	if(HI_NULL == p)\
	{HDMI_WARN("%s is null pointer!return fail.\n",#p);\
	return HI_FAILURE;}\
}while(0)

#define HDCP14_INIT_CHK(pstHdcp14) do{\
	HDCP14_NULL_CHK(pstHdcp14);\
	if(pstHdcp14->stInit.bInit != HI_TRUE)\
	{HDMI_WARN("hdcp14 module didn't init!\n");\
	return HI_FAILURE;}\
}while(0)

#define HDCP14_CHK_FAILURE_NORET(s32Ret) do{\
	if(HI_SUCCESS != s32Ret)\
	{HDMI_WARN("retval=%d is not HI_SUCCESS!\n",s32Ret);\
	return ;}\
}while(0)


#define HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,u64ReauthMs) do{\
    if(HI_SUCCESS != s32Ret)\
	{\
        if(pstHdcp14->stAttr.bFailStopMach){HAL_HDMI_MachStop(pstHdcp14->stInit.u32MachId);}\
	    HDMI_ERR("retval=%d is not HI_SUCCESS!\n",s32Ret);\
        pstHdcp14->stRun.u64ReAuthWaitMs=u64ReauthMs;\
        pstHdcp14->stRun.enMachStatus = HDCP14_MACH_AUTHEN_READY;\
        pstHdcp14->stRun.u64MachStartStamp = HAL_HDMI_MachMsGet();\
        if (pstHdcp14->stInit.pfnEventCallBack && pstHdcp14->stInit.pvEventData){\
            pstHdcp14->stInit.pfnEventCallBack(pstHdcp14->stInit.pvEventData,HDMI_EVENT_HDCP_FAIL);\
        }\
        return ;\
    }\
}while(0)



#define HDCP14_TIMEOUT_REAUTHEN(pstHdcp14,u64TimeOutMs) do{\
    if (HAL_HDMI_MachMsGet() - pstHdcp14->stRun.u64MachStartStamp > u64TimeOutMs){\
        if(pstHdcp14->stAttr.bFailStopMach)HAL_HDMI_MachStop(pstHdcp14->stInit.u32MachId);\
        HDMI_ERR("Hdcp14 MACH status %u wait %u timeout!\n",pstHdcp14->stRun.enMachStatus,u64TimeOutMs);\
        pstHdcp14->stRun.enMachStatus = HDCP14_MACH_AUTHEN_READY;\
        if (pstHdcp14->stInit.pfnEventCallBack && pstHdcp14->stInit.pvEventData){\
            pstHdcp14->stInit.pfnEventCallBack(pstHdcp14->stInit.pvEventData,HDMI_EVENT_HDCP_FAIL);\
        }\
        return ;\
    }\
}while(0)


static HDCP14_INFO_S  s_Hdcp14Info[HDMI_DEVICE_ID_BUTT];
static HDCP14_INFO_S* Hdcp14InfoGet(HDMI_DEVICE_ID_E enHdmiId)
{
    if (enHdmiId < HDMI_DEVICE_ID_BUTT)
    {
        return &s_Hdcp14Info[enHdmiId];
    }
    return HI_NULL;

}

#if HDCP14_DEBUG_KEY

HI_U32 s_au32HdcpSrcKey[512]={
    0x00, 0xc2, 0x43, 0xfc,     0xb1, 0xd8, 0x78, 0x6d,     0xa8, 0xad, 0x29, 0x9a,     0x79, 0x63, 0xd8, 0xf6,
    0xd5, 0xd8, 0x5e, 0x14,     0xe1, 0xb2, 0x97, 0xed,     0x9d, 0x3a, 0x13, 0xd3,    0x27, 0xe9, 0xc8, 0x33,
    0xf5, 0x40, 0x76, 0x21,     0x20, 0x44, 0x21, 0xea,     0x70, 0x4a, 0x60, 0x21,     0xa3, 0x18, 0x4e, 0x6d,
    0x78, 0x0a, 0x01, 0x91,     0x7f, 0xfd, 0x7e, 0x76,     0x1b, 0xd5, 0x83, 0x2c,     0x5e, 0x82, 0x6e, 0xf6,
    0x68, 0xd4, 0x14, 0x1e,     0xe9, 0x37, 0x56, 0xff,     0x6d, 0x24, 0xbc, 0x0e,     0x1b, 0xcf, 0xb1, 0x88,
    0xc8, 0xbe, 0x47, 0x9c,     0x8a, 0x8e, 0xa5, 0x97,     0xff, 0xf7, 0xfe, 0x2d,     0xc3, 0x78, 0x97, 0x0d,
    0x69, 0xc6, 0x35, 0xc0,     0x42, 0xb2, 0x92, 0x90,     0xa0, 0x1a, 0x59, 0xe7,     0x51, 0xb4, 0x06, 0xfa,
    0x7d, 0x9c, 0x1c, 0xa0,     0x27, 0x89, 0xa3, 0x98,     0xcb, 0x80, 0x5b, 0x1c,     0x65, 0x95, 0x12, 0x2d,

    0xf1, 0x8f, 0x55, 0xf9,     0x68, 0xe5, 0x36, 0x45,     0x9f, 0x6d, 0x21, 0xf8,     0xc2, 0x93, 0x70, 0x54,
    0x95, 0xb6, 0xc9, 0x3b,     0x6f, 0x59, 0xec, 0xdb,     0x40, 0x90, 0x23, 0xac,     0xf0, 0xc3, 0x48, 0x10,
    0x37, 0x92, 0x58, 0x1e,     0xa6, 0x09, 0xd0, 0x0b,     0xcb, 0x45, 0x5b, 0xa4,     0xef, 0xef, 0x0c, 0x48,
    0xfc, 0x9c, 0x89, 0x0f,     0x6f, 0x11, 0xb9, 0x63,     0x7d, 0xf1, 0xda, 0x11,     0x59, 0x61, 0x7e, 0x92,
    0xb0, 0xf2, 0xe4, 0x03,     0x94, 0xae, 0x3f, 0x9b,     0x70, 0xeb, 0xd8, 0x00,     0x41, 0x8a, 0x18, 0x6e,
    0x17, 0x25, 0x7e, 0x49,     0x5d, 0x77, 0x54, 0xec,     0xd8, 0xc9, 0x8e, 0x4c,     0xaf, 0xfb, 0xe1, 0xaf,
    0x6b, 0xed, 0x2a, 0xac,     0xa0, 0x39, 0x56, 0x65,     0xdc, 0x10, 0xb8, 0x62,     0xfd, 0x88, 0x73, 0x84,
    0xad, 0x34, 0xff, 0x31,     0x6c, 0x3b, 0x7e, 0x38,     0xda, 0xca, 0xb9, 0x63,     0x9e, 0xd9, 0xa1, 0x7a,

    0xbf, 0xd1, 0xe6, 0x83,     0x68, 0x64, 0x30, 0xdd,     0xa7, 0x27, 0x68, 0xaf,     0x8a, 0x70, 0x39, 0x5a,
    0xe0, 0x96, 0x64, 0x44,     0x31, 0x85, 0x0d, 0x44,     0xb9, 0x28, 0xb5, 0x39,     0x7a, 0x65, 0x72, 0x16,
    0x55, 0x14, 0xf7, 0x61,     0x03, 0xb7, 0x59, 0x45,     0xe3, 0x0c, 0x7d, 0xb4,     0x45, 0x19, 0xea, 0x8f,
    0xd2, 0x89, 0xee, 0xbd,     0x90, 0x21, 0x8b, 0x05,     0xe0, 0x4e, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,

    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0xff
};


int  Hdcp14LoadDebugSrcKey(void)
{
    HI_U32  i = 0;
    HI_U8   k = 0;
    HI_U32   u32DebugKeyAddr = 0;

     u32DebugKeyAddr =0xf8ce3400;

    for(i=0;i<512;i=i+4)
    {
        for(k=0;k<4;k++)
        {
            HDMIRegWriteU32(0xf8ce0854,k);
            msleep(1);
            HDMIRegWriteU32( u32DebugKeyAddr ,s_au32HdcpSrcKey[i+k]);
        }
         u32DebugKeyAddr = u32DebugKeyAddr + 4;
    }
    return 0;
}
#endif


/******************* private interface ****************************/
#if 0
static HI_VOID Hdcp14DataMuteEnable(HI_BOOL bEnable)
{
    HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlSet(bEnable);
    HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlSet(bEnable);
    return ;
}
#endif
static HI_VOID Hdcp14ModeSet(HDMI_DEVICE_ID_E enHdmiId)
{
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S stParam;
    HI_BOOL         bFunSelHdcp2x = HI_FALSE;

    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd    = HI_HDMI_HDCP14_INIT;
    stParam.pvData      = &bFunSelHdcp2x;
    stParam.u32Size     = sizeof(bFunSelHdcp2x);
    DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);
#else
    HDMI_HDCP_FUN_SEL_hdcp_fun_selSet(0);
#endif

    return ;
}

static HI_VOID Hdcp14LoadKeySet(HI_VOID)
{
	//bCmdDone clear
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet(HI_TRUE);
	//load key
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_loadSet(HI_TRUE);

	return ;
}

static HI_BOOL Hdcp14LoadFinishCheck(HI_VOID)
{
    if(HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet())
    {
        /* write 1 to clear */
        HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet(HI_TRUE);
        return HI_TRUE;
    }
    else
    {
        return HI_FALSE;
    }
}

static HI_VOID Hdcp14CrcCheckSet(HI_VOID)
{
    //HI_U32      u32Vaule = 0;
	//clear bist Error
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errSet(HI_TRUE);
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errSet(HI_TRUE);
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errSet(HI_TRUE);

	/* CRC check type select */
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet(HDCP14_KEY_CMD_CRC_ENABLE);
    /* check enable */
	HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmd_validSet(HI_TRUE);

	//u32Vaule = (HDCP14_KEY_CMD_CRC_ENABLE << 1) | (1<<6);
    //HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet(u32Vaule);
	return ;
}


static HI_BOOL Hdcp14CrcFinishCheck(HI_VOID)
{
    HI_S32      s32Err = 0;
    HI_BOOL     bRet = HI_FALSE;

    if (HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet())
    {
    	s32Err |= HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist0_errGet();
    	s32Err |= HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist1_errGet();
    	s32Err |= HDMI_HDCP1X_MEM_CTRL_hdcp1x_bist2_errGet();
        HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneSet(HI_TRUE);
        HDMI_HDCP1X_MEM_CTRL_hdcp1x_key_cmdSet(HI_FALSE);//crc finished ,clr keycmd.(all cmd disable)
        //HDMI_ERR(" crc cmd done, cmd done=%d\n",HDMI_HDCP1X_MEM_CTRL_hdcp1x_cmd_doneGet());
        bRet = s32Err ? HI_FALSE : HI_TRUE;
    }
    else
    {
        bRet = HI_FALSE;
    }

    return bRet;
}


static HI_S32 Hdcp14AnGenerateSent(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8AnData)
{
    HI_S32   	    u32An0 = 0;
    HI_S32   	    u32An1 = 0;
    HI_U32          i = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};

    for (i = 0; i < HDCP14_AN_SIZE_8BYTES; i++)
    {
        HDCP14_NULL_CHK(&pu8AnData[i]);
    }

    HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopSet(HI_FALSE);
    msleep(1);
    HDMI_HDCP1X_ENG_CTRL_hdcp1x_an_stopSet(HI_TRUE);
    //usleep(1);
    msleep(1);

    u32An0 = HDMI_HDCP1X_ENG_GEN_AN0_hdcp1x_eng_gen_an0Get();
    u32An1 = HDMI_HDCP1X_ENG_GEN_AN1_hdcp1x_eng_gen_an1Get();


    for(i=0;i<4;i++)
    {
        pu8AnData[i] = u32An0 & 0xff;
        u32An0 = u32An0>>8;
    }
    for(i=4;i<8;i++)
    {
        pu8AnData[i] = u32An1 & 0xff;
        u32An1 = u32An1>>8;
    }

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_WRITE_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_AN_SIZE_8BYTES;
    stDdcCfg.pu8Data        = pu8AnData;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_An;
    stDdcCfg.enMasterMode   = DDC_MASTER_MODE_PWD;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

    if (s32Ret == HDCP14_AN_SIZE_8BYTES)
    {
        return HI_SUCCESS;
    }
    else
    {
    	return HI_FAILURE;
    }
}

static HI_BOOL  Hdcp14KsvCheck(HI_U8 *pu8KsvData)
{
    HI_U8       i = 0,j = 0;
    HI_U8       u8Byte = 0;
    HI_U32      u32SetBitCnt = 0;
    HI_U8       *pu8Tmp = HI_NULL;
    HI_BOOL     bRet = HI_FALSE;

    for (pu8Tmp = pu8KsvData; pu8Tmp < pu8KsvData + HDCP14_KSV_SIZE_5BYTES; pu8Tmp++)
    {
        HDCP14_NULL_CHK(pu8Tmp);
    }

    for (i = 0,u32SetBitCnt = 0; i < HDCP14_KSV_SIZE_5BYTES; i++)
    {
        u8Byte = pu8KsvData[i];
        for (j = 0;j < 8;j++)
        {
            if (u8Byte & 0x01)
            {
                u32SetBitCnt++;
            }
            u8Byte>>=1;
        }
    }
    //HDMI_ERR("u32SetBitCnt==%u\n",u32SetBitCnt);

    bRet = u32SetBitCnt==20 ? HI_TRUE : HI_FALSE;

    return bRet;

}

static HI_S32 Hdcp14AksvSent(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8KsvData)
{
    HI_U32          i = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};

    for (i = 0; i < HDCP14_KSV_SIZE_5BYTES; i++)
    {
        HDCP14_NULL_CHK(&pu8KsvData[i]);
    }

	pu8KsvData[0] = HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte0Get();
	pu8KsvData[1] = HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte1Get();
	pu8KsvData[2] = HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte2Get();
	pu8KsvData[3] = HDMI_HDCP1X_KEY_AKSV0_hdcp1x_key_aksv_byte3Get();
	pu8KsvData[4] = HDMI_HDCP1X_KEY_AKSV1_hdcp1x_key_aksv_byte4Get();

	if(!Hdcp14KsvCheck(pu8KsvData))
	{
	    HDMI_ERR("hdcp14 aksv check fail!\n");
        return HI_FAILURE;
	}

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_WRITE_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_KSV_SIZE_5BYTES;
    stDdcCfg.pu8Data        = pu8KsvData;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_AKSV;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

	if (s32Ret==HDCP14_KSV_SIZE_5BYTES)
    {
        return HI_SUCCESS;
    }
    else
    {
    	return HI_FAILURE;
	}
}

static HI_S32 Hdcp14BksvRead(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8KsvData)
{
	HI_U32          i = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};

    for (i = 0; i < HDCP14_KSV_SIZE_5BYTES; i++)
    {
        HDCP14_NULL_CHK(&pu8KsvData[i]);
    }

	HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_KSV_SIZE_5BYTES;
    stDdcCfg.pu8Data        = pu8KsvData;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_BKSV;
    HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

    if(!Hdcp14KsvCheck(pu8KsvData))
    {
        HDMI_WARN("hdcp14 bksv check fail!\n");
        s32Ret = HI_FAILURE;
    }
    else
    {
        s32Ret = HI_SUCCESS;
    }

	return s32Ret;
}

static HI_S32 Hdcp14BksvSet(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8KsvData)
{
    HI_S32          s32Err = 0;
#ifndef HDMI_TEE_SUPPORT
    //Bksv Copy to Reg
    HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte0Set(pu8KsvData[0]);
    HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte1Set(pu8KsvData[1]);
    HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte2Set(pu8KsvData[2]);
    HDMI_HDCP1X_ENG_BKSV0_hdcp1x_key_bksv_byte3Set(pu8KsvData[3]);
    HDMI_HDCP1X_ENG_BKSV1_hdcp1x_key_bksv_byte4Set(pu8KsvData[4]);
#else
    HDMI_TEE_PARAM_S stParam;
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd    = HI_HDMI_HDCP14_BKSV_SET;
    stParam.pvData      = (HI_VOID *)pu8KsvData;
    stParam.u32Size     = HDCP14_KSV_SIZE_5BYTES;
    s32Err = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);
    if(HI_SUCCESS != s32Err)
    {
        return HI_FAILURE;
    }
#endif
	//Bksv eeror check
	s32Err = HDMI_HDCP1X_ENG_STATUS_hdcp1x_bksv_errGet();

    return s32Err ?   HI_FAILURE : HI_SUCCESS;
}

static HI_S32 Hdcp14BcapsRead(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8BcapsData)
{
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};

	HDCP14_NULL_CHK(pu8BcapsData);

	s32Ret |= HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = 1;
    stDdcCfg.pu8Data        = pu8BcapsData;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_Bcaps;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

    if (s32Ret==1)
    {
        return HI_SUCCESS;
    }
    else
    {
    	return HI_FAILURE;
	}

}

static HI_BOOL Hdcp14R0ReadCmp(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8ARiData,HI_U8 *pu8BRiData)
{
    HI_U32          i = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S stParam;
#endif

    for (i = 0; i < HDCP14_Ri_SIZE_2BYTES; i++)
    {
        HDCP14_NULL_CHK(&pu8ARiData[i]);
    }
    for (i = 0; i < HDCP14_Ri_SIZE_2BYTES; i++)
    {
        HDCP14_NULL_CHK(&pu8BRiData[i]);
    }

    /* Ri' */
    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_Ri_SIZE_2BYTES;
    stDdcCfg.pu8Data        = pu8BRiData;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_BRi;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

#ifndef HDMI_TEE_SUPPORT
    /* Ri */
    if(!HDMI_HDCP1X_ENG_STATUS_hdcp1x_ri_rdyGet())
    {
        HDMI_ERR("hdcp14 hdcp1x_ri_rdy=0\n");
    }
    pu8ARiData[0] = HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri0Get();
    pu8ARiData[1] = HDMI_HDCP1X_ENG_RI_hdcp1x_eng_ri1Get();


    if (s32Ret != HDCP14_Ri_SIZE_2BYTES)
    {
        HDMI_ERR("hdcp14 Ri' Read error!\n");
    }

    if((pu8ARiData[0]!=pu8BRiData[0]) || (pu8ARiData[1]!=pu8BRiData[1]))
    {
        HDMI_ERR("pu8ARiData[0]=0x%02x\n",pu8ARiData[0]);
        HDMI_ERR("pu8BRiData[0]=0x%02x\n",pu8BRiData[0]);
        HDMI_ERR("pu8ARiData[1]=0x%02x\n",pu8ARiData[1]);
        HDMI_ERR("pu8BRiData[1]=0x%02x\n\n",pu8BRiData[1]);
        return HI_FALSE;
    }
    else
    {
        //HDMI_ERR("pu8ARiData[0]=0x%02x\n",pu8ARiData[0]);
        //HDMI_ERR("pu8BRiData[0]=0x%02x\n",pu8BRiData[0]);
        //HDMI_ERR("pu8ARiData[1]=0x%02x\n",pu8ARiData[1]);
        //HDMI_ERR("pu8BRiData[1]=0x%02x\n\n",pu8BRiData[1]);
        return  HI_TRUE ;
    }
#else
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd  = HI_HDMI_HDCP14_R0_VERIFY;
    stParam.pvData  = (HI_VOID *)pu8BRiData;
    stParam.u32Size = HDCP14_Ri_SIZE_2BYTES;
    s32Ret = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);
    return (HI_SUCCESS != s32Ret) ? HI_FALSE : HI_TRUE;

#endif

}


static HI_S32 Hdcp14EncrypEnable(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable)
{
#ifndef HDMI_TEE_SUPPORT
    HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_enc_enSet(bEnable);
    return HI_SUCCESS;

#else
    HI_S32           s32Ret = HI_SUCCESS;
    HDMI_TEE_PARAM_S stParam;
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd    = HI_HDMI_HDCP14_ENC_ENABLE;
    stParam.pvData      = &bEnable;
    stParam.u32Size     = sizeof(bEnable);
    s32Ret = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);

    return (HI_SUCCESS != s32Ret) ? HI_FAILURE : HI_SUCCESS;

#endif

}

static HI_S32 Hdcp14RepeaterEnable(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable)
{
#ifndef HDMI_TEE_SUPPORT
    HDMI_HDCP1X_SEC_CFG_hdcp1x_sec_rpt_onSet(bEnable);
    return HI_SUCCESS;

#else
    HI_S32           s32Ret = HI_SUCCESS;
    HDMI_TEE_PARAM_S stParam;
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd    = HI_HDMI_HDCP14_RPT_ENABLE;
    stParam.pvData      = &bEnable;
    stParam.u32Size     = sizeof(bEnable);
    s32Ret = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);

    return (HI_SUCCESS != s32Ret) ? HI_FAILURE : HI_SUCCESS;

#endif

}


static HI_S32 Hdcp14BstatusRead(HDMI_DEVICE_ID_E enHdmiId,HI_U16 *pu16Bstauts)
{
    HI_U8           au8Byte[HDCP14_Bstatus_SIZE_2BYTES] = {0};
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};

	HDCP14_NULL_CHK(pu16Bstauts);

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_Bstatus_SIZE_2BYTES;
    stDdcCfg.pu8Data        = au8Byte;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_Bstatus;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);

    *pu16Bstauts = au8Byte[1] & 0xff;
    *pu16Bstauts = (*pu16Bstauts)<<8;
    *pu16Bstauts |= au8Byte[0] & 0xff;

    HDMI_HDCP1X_RPT_BSTATUS_hdcp1x_bstatusSet(*pu16Bstauts);

	return s32Ret>0 ? HI_SUCCESS : HI_FAILURE;

}


static HI_S32 Hdcp14BksvListRead(HDMI_DEVICE_ID_E enHdmiId,HI_U8 **ppu8KsvList,HI_S32 s32Size)
{
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};
    HI_U8           *pu8KsvList = HI_NULL;
#ifndef HDMI_TEE_SUPPORT
    HI_U32          i = 0;
#else
    HDMI_TEE_PARAM_S stParam;
#endif

    HDCP14_NULL_CHK(ppu8KsvList);

    if (*ppu8KsvList)
    {
        HDMI_KFREE(*ppu8KsvList);
    }

    pu8KsvList = (HI_U8 *)HDMI_KMALLOC(s32Size);
    HDCP14_NULL_CHK(pu8KsvList);
    *ppu8KsvList = pu8KsvList;

	s32Ret |= HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = s32Size;
    stDdcCfg.pu8Data        = pu8KsvList;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_KSV_FIFO;
    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
    if(s32Size != s32Ret)
        return HI_FAILURE;

#ifndef HDMI_TEE_SUPPORT

    for (i = 0; i<s32Size ;i++)
    {
        HDMI_HDCP1X_RPT_KLIST_hdcp1x_rpt_klistSet(pu8KsvList[i]);
    }

    return HI_SUCCESS ;

#else
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd  = HI_HDMI_HDCP14_KSVLIST_SET;
    stParam.pvData  = (HI_VOID *)pu8KsvList;
    stParam.u32Size = (HI_U32)s32Size;
    s32Ret = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);

    return (HI_SUCCESS != s32Ret) ? HI_FAILURE : HI_SUCCESS;

#endif
}


static HI_VOID Hdcp14ShaCalculateEanble(HI_BOOL bShaEnable,HI_BOOL bNoDownStream)
{
	HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_no_dsSet(bNoDownStream);
	HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startSet(bShaEnable);
	return ;
}

static HI_BOOL Hdcp14ShaReadyCheck(HI_VOID)
{
	if ( HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_readyGet() )
	{
        return HI_TRUE;
	}
	else
	{
        return HI_FALSE;
    }
}

static HI_S32 Hdcp14ViCmpSet(HDMI_DEVICE_ID_E enHdmiId,HI_U8 *pu8AViData,HI_U8 *pu8BViData )
{
    HI_U8           *pu8Tmp = HI_NULL;
	HI_U32          i = 0;
    HI_U32          u32Tmp = 0;
    HI_S32          s32Ret = HI_SUCCESS;
    DDC_CFG_S       stDdcCfg = {0};
#ifdef HDMI_TEE_SUPPORT
    HDMI_TEE_PARAM_S stParam;
#endif


    for(i=0,pu8Tmp=pu8AViData;i<HDCP14_Vi_SIZE_20BYTES;i++,pu8Tmp++)
    {
        HDCP14_NULL_CHK(pu8Tmp);
    }
    for(i=0,pu8Tmp=pu8BViData;i<HDCP14_Vi_SIZE_20BYTES;i++,pu8Tmp++)
    {
        HDCP14_NULL_CHK(pu8Tmp);
    }

    u32Tmp = HDMI_HDCP1X_SHA_V0_hdcp1x_rpt_vh0Get();
    pu8AViData[0] = (u32Tmp) & 0xff;
    pu8AViData[1] = (u32Tmp >> 8) & 0xff;
    pu8AViData[2] = (u32Tmp >> 16) & 0xff;
    pu8AViData[3] = (u32Tmp >> 24) & 0xff;

    u32Tmp = HDMI_HDCP1X_SHA_V1_hdcp1x_rpt_vh1Get();
    pu8AViData[4] = (u32Tmp) & 0xff;
    pu8AViData[5] = (u32Tmp >> 8) & 0xff;
    pu8AViData[6] = (u32Tmp >> 16) & 0xff;
    pu8AViData[7] = (u32Tmp >> 24) & 0xff;


    u32Tmp = HDMI_HDCP1X_SHA_V2_hdcp1x_rpt_vh2Get();
    pu8AViData[8] = (u32Tmp) & 0xff;
    pu8AViData[9] = (u32Tmp >> 8) & 0xff;
    pu8AViData[10] = (u32Tmp >> 16) & 0xff;
    pu8AViData[11] = (u32Tmp >> 24) & 0xff;

    u32Tmp = HDMI_HDCP1X_SHA_V3_hdcp1x_rpt_vh3Get();
    pu8AViData[12] = (u32Tmp) & 0xff;
    pu8AViData[13] = (u32Tmp >> 8) & 0xff;
    pu8AViData[14] = (u32Tmp >> 16) & 0xff;
    pu8AViData[15] = (u32Tmp >> 24) & 0xff;

    u32Tmp = HDMI_HDCP1X_SHA_V4_hdcp1x_rpt_vh4Get();
    pu8AViData[16] = (u32Tmp) & 0xff;
    pu8AViData[17] = (u32Tmp >> 8) & 0xff;
    pu8AViData[18] = (u32Tmp >> 16) & 0xff;
    pu8AViData[19] = (u32Tmp >> 24) & 0xff;

    HAL_HDMI_DdcDefaultCfgGet(enHdmiId,&stDdcCfg);
    stDdcCfg.enFuncType     = DDC_FUNC_TYPE_HDCP;
    stDdcCfg.enIssueMode    = DDC_MODE_READ_MUTIL_NO_ACK;
    stDdcCfg.s32DataSize    = HDCP14_Vi_SIZE_20BYTES;
    stDdcCfg.u8Offset       = HDCP14_OFFSET_BVH0;
    stDdcCfg.pu8Data        = &pu8BViData[0];

    s32Ret = HAL_HDMI_DdcIssue(enHdmiId,&stDdcCfg);
    if (s32Ret != stDdcCfg.s32DataSize)
    {
        HDMI_ERR("hdcp14 Vi' read fail,ddc size=%d\n",s32Ret);
        return HI_FAILURE;
    }
    else
    {
        s32Ret = HI_SUCCESS;
    }

#ifndef HDMI_TEE_SUPPORT
    u32Tmp = pu8BViData[19];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[18];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[17];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[16];
    HDMI_HDCP1X_RPT_V4_hdcp1x_rpt_calc_vh4Set(u32Tmp);

    u32Tmp = pu8BViData[15];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[14];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[13];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[12];
    HDMI_HDCP1X_RPT_V3_hdcp1x_rpt_calc_vh3Set(u32Tmp);

    u32Tmp = pu8BViData[11];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[10];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[9];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[8];
    HDMI_HDCP1X_RPT_V2_hdcp1x_rpt_calc_vh2Set(u32Tmp);

    u32Tmp = pu8BViData[7];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[6];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[5];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[4];
    HDMI_HDCP1X_RPT_V1_hdcp1x_rpt_calc_vh1Set(u32Tmp);

    u32Tmp = pu8BViData[3];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[2];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[1];
    u32Tmp <<= 8;
    u32Tmp |= pu8BViData[0];
    HDMI_HDCP1X_RPT_V0_hdcp1x_rpt_calc_vh0Set(u32Tmp);

    /* enable logic to cmp Vi & Vi' */
    HDMI_HDCP1X_SHA_CHECK_hdcp1x_sha_checkSet(HI_TRUE);

#else
    HDMI_MEMSET(&stParam, 0, sizeof(stParam));
    stParam.enTeeCmd  = HI_HDMI_HDCP14_VI_VERIFY;
    stParam.pvData  = (HI_VOID *)pu8BViData;
    stParam.u32Size = HDCP14_Vi_SIZE_20BYTES;
    s32Ret = DRV_HDMI_TeeCmdSend(enHdmiId, &stParam);
#endif
	return s32Ret;
}

static HI_S32 Hdcp14ViCmpGet(HI_U8 *pu8AViData,HI_U8 *pu8BViData )
{
    HI_S32  s32Ret = HI_FAILURE;
    HDCP14_NULL_CHK(pu8AViData);
    HDCP14_NULL_CHK(pu8BViData);

    if ((!HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_invalidGet())
        && HDMI_HDCP1X_SHA_RESULT_hdcp1x_sha_okGet())
    {
        s32Ret = HI_SUCCESS;
    }
    else
    {
        s32Ret = HI_FAILURE;
    }
    return s32Ret;
}


static HI_VOID Hdcp14RiCheckEanble(HI_BOOL bEnalbe)
{
    HDMI_HDCP1X_CHK_CTRL_hdcp1x_auto_check_enSet(bEnalbe);
    return ;
}


static HI_U32 Hdcp14EncrypFrameCntGet(HI_VOID)
{
    HI_U32 u32Status = 0;

    u32Status = HDMI_HDCP1X_ENG_STATUS_hdcp1x_i_cntGet();

    return u32Status;
}

static HI_S32 Hdcp14RiMonitor(HDCP14_INFO_S *pstHdcp14)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HI_U32              u32RiChkErr = 0;
    HDCP14_RUN_STAT_S   *pstRun = HI_NULL;
    HDCP14_PART_III_S   *pstPartIII = HI_NULL;

    HDCP14_NULL_CHK(pstHdcp14);
    pstRun = &pstHdcp14->stRun;

    pstPartIII  = &pstHdcp14->stStatus.stPartIII[pstRun->u32PartIIIPtr];
    pstPartIII->u32EncrypFrameCnt = Hdcp14EncrypFrameCntGet();
    u32RiChkErr = HDMI_HDCP1X_CHK_ERR_hdcp1x_nodone_err_cntGet();
    u32RiChkErr |= HDMI_HDCP1X_CHK_ERR_hdcp1x_notchg_err_cntGet();
    u32RiChkErr |= HDMI_HDCP1X_CHK_ERR_hdcp1x_000frm_err_cntGet();
    u32RiChkErr |= HDMI_HDCP1X_CHK_ERR_hdcp1x_127frm_err_cntGet();
    if (u32RiChkErr)
    {
        HDMI_ERR("hdcp14 Ri cnt fail,error code=0x%x\n",u32RiChkErr);
        s32Ret = HI_FAILURE;
    }
    u32RiChkErr = HDMI_HDCP1X_CHK_STATE_hdcp1x_fsm_stateGet();
    if( (HDCP14_CHK_STAT_ERR == u32RiChkErr)
        || (HDCP14_CHK_STAT_ERR_RI_CMP ==u32RiChkErr) )
    {
       HDMI_ERR("hdcp14 Ri check fail,error code=0x%x\n",u32RiChkErr);
        s32Ret = HI_FAILURE;
    }
    pstRun->u32PartIIIPtr = (pstRun->u32PartIIIPtr + 1) % HDCP14_MAX_III_RECORD_NUM;

    return s32Ret;
}


HI_VOID Hdcp14RethenHwReset(HDMI_DEVICE_ID_E enHdmiId)
{
    Hdcp14RepeaterEnable(enHdmiId,HI_FALSE);
    Hdcp14EncrypEnable(enHdmiId,HI_FALSE);
    Hdcp14ShaCalculateEanble(HI_FALSE,HI_FALSE);
    Hdcp14RiCheckEanble(HI_FALSE);
    return ;
}


HI_S32 Hdcp14RethenHwStatusGet(HDCP14_REAL_HW_S *pstRealHw)
{
    HDCP14_NULL_CHK(pstRealHw);

    pstRealHw->bRepeaterEnalbe = HDMI_HDCP1X_ENG_CTRL_hdcp1x_rpt_onGet() ?  HI_TRUE : HI_FALSE ;
    pstRealHw->bEncryption  = HDMI_HDCP1X_ENG_CTRL_hdcp1x_enc_enGet()  ?  HI_TRUE : HI_FALSE ;
    pstRealHw->bShaEnable   = HDMI_HDCP1X_SHA_CTRL_hdcp1x_sha_startGet()  ?  HI_TRUE : HI_FALSE ;
    pstRealHw->bHdcp14Mode  = (!HDMI_HDCP_FUN_SEL_hdcp_fun_selGet()) ? HI_TRUE : HI_FALSE ;
    pstRealHw->bAvmute      = (HDMI_HDCP_MUTE_CTRL_hdcp1x_amute_ctrlGet() && HDMI_HDCP_MUTE_CTRL_hdcp1x_vmute_ctrlGet()) ? HI_TRUE : HI_FALSE ;

    return HI_SUCCESS;
}


HI_VOID Hdcp14MachCallBack(HI_VOID *pData)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HI_BOOL             bNoDownStream = HI_FALSE;
    HDCP14_INFO_S       *pstHdcp14 = (HDCP14_INFO_S *)pData;
    HDCP14_MACH_E       *penMachStatus = HI_NULL;
    HI_U64              *pu64StartStamp = HI_NULL;

    if((!pstHdcp14) || (!pstHdcp14->stInit.bInit))
    {
        HDMI_WARN("pstHdcp14 is null OR module hdcp14 didn't init!\n");
        return ;
    }

    penMachStatus   = &pstHdcp14->stRun.enMachStatus;
    pu64StartStamp  = &pstHdcp14->stRun.u64MachStartStamp;

    switch(pstHdcp14->stRun.enMachStatus)
    {
        /* READY */
        case HDCP14_MACH_AUTHEN_READY:
            Hdcp14ModeSet(pstHdcp14->stInit.enHdmiId);
            Hdcp14RethenHwReset(pstHdcp14->stInit.enHdmiId);
            if (HAL_HDMI_MachMsGet()-(*pu64StartStamp) > pstHdcp14->stRun.u64ReAuthWaitMs)
            {
                *penMachStatus = HDCP14_MACH_PI_START;
                *pu64StartStamp = HAL_HDMI_MachMsGet();
            }
            else
            {
                break;
            }
            //break;

        /* Part I */
        case HDCP14_MACH_PI_START:
            s32Ret = Hdcp14BksvRead(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.au8Bksv);
            pstHdcp14->stStatus.stPartI.enBksvRead = HDCP14_PROCESS_FAIL;
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,HDCP14_REAUTH_WAIT_MS_BKSV_FAIL);
            pstHdcp14->stStatus.stPartI.enBksvRead = HDCP14_PROCESS_OK;

            *penMachStatus = HDCP14_MACH_PI_BCAP;
            *pu64StartStamp = HAL_HDMI_MachMsGet();
            break;

        case HDCP14_MACH_PI_BCAP:
            s32Ret = Hdcp14BcapsRead(pstHdcp14->stInit.enHdmiId,&pstHdcp14->stStatus.stPartI.unBCaps.u8Byte);
            pstHdcp14->stStatus.stPartI.enBCapsRead = HDCP14_PROCESS_FAIL;
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);
            pstHdcp14->stStatus.stPartI.enBCapsRead = HDCP14_PROCESS_OK;

            *penMachStatus = HDCP14_MACH_PI_AKSV;
            *pu64StartStamp = HAL_HDMI_MachMsGet();
            break;


        case HDCP14_MACH_PI_AKSV:
            s32Ret = Hdcp14AnGenerateSent(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.au8An);
            pstHdcp14->stStatus.stPartI.enAnGenerateSent = HDCP14_PROCESS_FAIL;
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,HDCP14_REAUTH_WAIT_MS_AN_FAIL);
            pstHdcp14->stStatus.stPartI.enAnGenerateSent = HDCP14_PROCESS_OK;

            s32Ret = Hdcp14AksvSent(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.au8Aksv);
            pstHdcp14->stStatus.stPartI.enAksvSent = HDCP14_PROCESS_FAIL;
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);
            pstHdcp14->stStatus.stPartI.enAksvSent = HDCP14_PROCESS_OK;

            Hdcp14RepeaterEnable(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.unBCaps.u8.bzRepeater);

            *penMachStatus = HDCP14_MACH_PI_BKSV_SET;
            *pu64StartStamp = HAL_HDMI_MachMsGet();
            break;

        case HDCP14_MACH_PI_BKSV_SET:
            s32Ret= Hdcp14BksvSet(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.au8Bksv);
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);

            *penMachStatus = HDCP14_MACH_PI_R0_WAIT;
            *pu64StartStamp = HAL_HDMI_MachMsGet();
            //break;

        case HDCP14_MACH_PI_R0_WAIT:
            if (HAL_HDMI_MachMsGet()-(*pu64StartStamp) > pstHdcp14->stAttr.u32R0ReadyTime)
            {
                *penMachStatus = HDCP14_MACH_PI_DONE;
                *pu64StartStamp = HAL_HDMI_MachMsGet();
            }
            break;

        case HDCP14_MACH_PI_DONE:
            if (Hdcp14R0ReadCmp(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartI.au8AR0,pstHdcp14->stStatus.stPartI.au8BR0))
            {
                pstHdcp14->stStatus.stPartI.enAR0BR0Cmp = HDCP14_PROCESS_OK;
                HDMI_INFO("hdcp14 PARTI done success!\n");

                pstHdcp14->stStatus.stPartI.enEncrypEnable = HDCP14_PROCESS_FAIL;
                Hdcp14EncrypEnable(pstHdcp14->stInit.enHdmiId,HI_TRUE);

                pstHdcp14->stRun.enMachStatus = HDCP14_MACH_PI_WAIT_ENCRP;
                *pu64StartStamp = HAL_HDMI_MachMsGet();
            }
            else
            {
                Hdcp14EncrypEnable(pstHdcp14->stInit.enHdmiId,HI_FALSE);
                pstHdcp14->stStatus.stPartI.enAR0BR0Cmp = HDCP14_PROCESS_FAIL;
                HDMI_ERR("hdcp14 PARTI done fail!(AR0!=BR0)\n");
                HDCP14_FAILURE_REAUTHEN(pstHdcp14,HI_FAILURE,HDCP14_REAUTH_WAIT_MS_R0_FAIL);
            }
            break;

        case HDCP14_MACH_PI_WAIT_ENCRP:
            if (HAL_HDMI_MachMsGet()-(*pu64StartStamp) > HDCP14_REAUTH_WAIT_MS_ENCYP)
            {
                pstHdcp14->stStatus.stPartI.enEncrypEnable = HDCP14_PROCESS_OK;
                *pu64StartStamp = HAL_HDMI_MachMsGet();
                pstHdcp14->stRun.enMachStatus = pstHdcp14->stStatus.stPartI.unBCaps.u8.bzRepeater ? \
                                                HDCP14_MACH_PII_START : HDCP14_MACH_PIII_START;
            }
            break;

        /* Part II */
        case HDCP14_MACH_PII_START:
            s32Ret = Hdcp14BcapsRead(pstHdcp14->stInit.enHdmiId,&pstHdcp14->stStatus.stPartII.unBcaps.u8Byte);
            pstHdcp14->stStatus.stPartII.enBlistReady = HDCP14_PROCESS_FAIL;
            if(s32Ret!=HI_SUCCESS)
            {
                HDMI_ERR("HDCP14 PII's Bcaps read fail!\n");
            }
            //HDMI_ERR("pstHdcp14->stStatus.stPartII.unBcaps.u8.bzKsvFifoReady=%d\n",pstHdcp14->stStatus.stPartII.unBcaps.u8.bzKsvFifoReady);
            if (pstHdcp14->stStatus.stPartII.unBcaps.u8.bzKsvFifoReady)
            {
                pstHdcp14->stStatus.stPartII.enBlistReady = HDCP14_PROCESS_OK;
                s32Ret = Hdcp14BstatusRead(pstHdcp14->stInit.enHdmiId,&pstHdcp14->stStatus.stPartII.unBstatus.u16Word);
                HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);
                if (( pstHdcp14->stStatus.stPartII.unBstatus.u16.bzMaxDevsExceeded )
                    ||( pstHdcp14->stStatus.stPartII.unBstatus.u16.bzMaxCasCadeExceeded ))
                {
                    Hdcp14EncrypEnable(pstHdcp14->stInit.enHdmiId,HI_FALSE);
                    HDMI_ERR("hdcp14 MaxDevsExceeded MaxCasCadeExceeded,Fail!\n");
                    HDCP14_FAILURE_REAUTHEN(pstHdcp14,HI_FAILURE,HDCP14_REAUTH_WAIT_MS_ENCYP);
                    break;
                }

                pstHdcp14->stStatus.stPartII.u32BksvListSize = pstHdcp14->stStatus.stPartII.unBstatus.u16.bzDeviceCnt * HDCP14_KSV_SIZE_5BYTES ;
                bNoDownStream = pstHdcp14->stStatus.stPartII.unBstatus.u16.bzDepth > 1 ? HI_FALSE : HI_TRUE ;

                //HDMI_ERR("cnt=%d,pstHdcp14->stStatus.stPartII.u32BksvListSize=%d\n",
                //        pstHdcp14->stStatus.stPartII.unBstatus.u16.bzDeviceCnt,
                //        pstHdcp14->stStatus.stPartII.u32BksvListSize);

                Hdcp14ShaCalculateEanble(HI_TRUE,bNoDownStream);
                if (pstHdcp14->stStatus.stPartII.u32BksvListSize)
                    s32Ret = Hdcp14BksvListRead(pstHdcp14->stInit.enHdmiId,&pstHdcp14->stStatus.stPartII.pu8BksvListData,pstHdcp14->stStatus.stPartII.u32BksvListSize);
                pstHdcp14->stStatus.stPartII.enBksvListChk = HDCP14_PROCESS_FAIL;
                HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);
                pstHdcp14->stStatus.stPartII.enBksvListChk = HDCP14_PROCESS_OK;

                *pu64StartStamp = HAL_HDMI_MachMsGet();
                *penMachStatus = HDCP14_MACH_PII_SHA_WAIT;
            }
            else
            {
                HDCP14_TIMEOUT_REAUTHEN(pstHdcp14,pstHdcp14->stAttr.u32BlistReadyTime);
            }
            break;

        case HDCP14_MACH_PII_SHA_WAIT:
            pstHdcp14->stStatus.stPartII.enHashDone = HDCP14_PROCESS_FAIL;
            if (Hdcp14ShaReadyCheck())
            {
                pstHdcp14->stStatus.stPartII.enHashDone = HDCP14_PROCESS_OK;
                s32Ret = Hdcp14ViCmpSet(pstHdcp14->stInit.enHdmiId,pstHdcp14->stStatus.stPartII.au8AVi,pstHdcp14->stStatus.stPartII.au8BVi);
                HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,0);

                *pu64StartStamp = HAL_HDMI_MachMsGet();
                *penMachStatus = HDCP14_MACH_PII_VI_CMP;
            }
            else
            {
                HDCP14_TIMEOUT_REAUTHEN(pstHdcp14,HDCP14_SHA_WAIT_TIMEOUT);
            }
            break;

        case HDCP14_MACH_PII_VI_CMP:
            pstHdcp14->stStatus.stPartII.enViReadCmp = HDCP14_PROCESS_FAIL;
            s32Ret = Hdcp14ViCmpGet(pstHdcp14->stStatus.stPartII.au8AVi,pstHdcp14->stStatus.stPartII.au8BVi);
            HDCP14_FAILURE_REAUTHEN(pstHdcp14, s32Ret, 0);
            pstHdcp14->stStatus.stPartII.enViReadCmp = HDCP14_PROCESS_OK;
            HDMI_INFO("hdcp14 PARTII done success!\n");
            *penMachStatus = HDCP14_MACH_PIII_START;
            //break;

        /* Part III */
        case HDCP14_MACH_PIII_START:
            if (pstHdcp14->stInit.pfnEventCallBack && pstHdcp14->stInit.pvEventData)
            {
                pstHdcp14->stInit.pfnEventCallBack(pstHdcp14->stInit.pvEventData,HDMI_EVENT_HDCP_SUCCESS);
            }
            //Hdcp14IrqMask(HI_TRUE);
            Hdcp14RiCheckEanble(HI_TRUE);
            pstHdcp14->stRun.u64PIIIRiInterval = HDCP14_PIII_Ri_NORMAL_INTERVAL;
            *pu64StartStamp = HAL_HDMI_MachMsGet();
            *penMachStatus = HDCP14_MACH_PIII_RI_MONITOR;
            break;

        case HDCP14_MACH_PIII_RI_MONITOR:
            s32Ret = Hdcp14RiMonitor(pstHdcp14);
            HDCP14_FAILURE_REAUTHEN(pstHdcp14,s32Ret,HDCP14_REAUTH_WAIT_MS_Ri_FAIL);
            break;

        default:
            HDMI_ERR("hdcp14 error status machine!\n");
            break;
    }

    return ;
}

/****************************** public interface ***************************************/

HI_S32  HAL_HDMI_Hdcp14Init(HDMI_DEVICE_ID_E enHdmiId,HDMI_HAL_INIT_S *pstHalInit)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_MACH_CTRL_S    stMachCtrl = {0};
    HDCP14_INFO_S       *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_NULL_CHK(pstHalInit);

    if (!pstHdcp14->stInit.bInit)
    {
        s32Ret |= HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_Init();

        HDMI_MEMSET( pstHdcp14, 0 , sizeof(HDCP14_INFO_S));
        pstHdcp14->stInit.bInit             = HI_TRUE;
        pstHdcp14->stRun.enMachStatus       = HDCP14_MACH_AUTHEN_READY;
        pstHdcp14->stInit.pfnEventCallBack  = pstHalInit->pfnEventCallBack;
        pstHdcp14->stInit.pvEventData       = pstHalInit->pvEventData;
        pstHdcp14->stInit.enHdmiId          = enHdmiId;

        pstHdcp14->stAttr.bFailStopMach     = HI_FALSE;
        pstHdcp14->stAttr.u32R0ReadyTime    = HDCP14_BR0_WAIT_TIMEOUT;
        pstHdcp14->stAttr.u32BlistReadyTime = HDCP14_BKSVLIST_TIMEOUT;
        pstHdcp14->stAttr.u32MachInterval   = HDCP14_DEFUALT_INTERVAL;

        stMachCtrl.u64Interval              = HDCP14_DEFUALT_INTERVAL;
        stMachCtrl.pCbData                  = pstHdcp14;
        stMachCtrl.pfnCallBack              = Hdcp14MachCallBack;
        stMachCtrl.pu8Name                  = "HDCP14";
        s32Ret |= HAL_HDMI_MachRegister(&stMachCtrl,&pstHdcp14->stInit.u32MachId);
    }

    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp14DeInit(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    s32Ret |= HAL_HDMI_MachUnRegister(pstHdcp14->stInit.u32MachId);
    if (pstHdcp14->stStatus.stPartII.pu8BksvListData)
    {
        HDMI_KFREE(pstHdcp14->stStatus.stPartII.pu8BksvListData);
        pstHdcp14->stStatus.stPartII.pu8BksvListData = HI_NULL;
    }
    HDMI_MEMSET( pstHdcp14, 0 , sizeof(HDCP14_INFO_S));
    pstHdcp14->stInit.bInit = HI_FALSE;

    s32Ret |= HDMI_TX_S_tx_hdcp_reg_REGS_TYPE_DeInit();

    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp14Reset(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    s32Ret |= HAL_HDMI_MachStop(pstHdcp14->stInit.u32MachId);
    Hdcp14RethenHwReset(enHdmiId);
    if (pstHdcp14->stStatus.stPartII.pu8BksvListData)
    {
        HDMI_KFREE(pstHdcp14->stStatus.stPartII.pu8BksvListData);
        pstHdcp14->stStatus.stPartII.pu8BksvListData = HI_NULL;
    }
    HDMI_MEMSET( &pstHdcp14->stRun, 0 , sizeof(HDCP14_RUN_STAT_S));
    HDMI_MEMSET( &pstHdcp14->stStatus, 0 , sizeof(HDCP14_STAUTS_S));

    return HI_SUCCESS;
}


HI_S32  HAL_HDMI_Hdcp14StatusGet(HDMI_DEVICE_ID_E enHdmiId,HDCP14_STAUTS_S *pstStauts)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstStauts);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    s32Ret = Hdcp14RethenHwStatusGet(&pstHdcp14->stStatus.stRealHw);
    HDMI_MEMCPY(pstStauts,&pstHdcp14->stStatus,sizeof(HDCP14_STAUTS_S));

    return s32Ret;
}

HI_S32  HAL_HDMI_Hdcp14RunStatusGet(HDMI_DEVICE_ID_E enHdmiId,HDCP14_RUN_STAT_S *pstRunStat)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstRunStat);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    HDMI_MEMCPY(pstRunStat,&pstHdcp14->stRun,sizeof(HDCP14_RUN_STAT_S));

    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp14AttrSet(HDMI_DEVICE_ID_E enHdmiId,HDCP14_ATTR_S *pstAttr)
{
    HI_S32              s32Ret = HI_SUCCESS;
    HDMI_MACH_CTRL_S    stMachCtrl = {0};
    HDCP14_INFO_S       *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstAttr);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    HDMI_MEMCPY(&pstHdcp14->stAttr,pstAttr,sizeof(HDCP14_ATTR_S));

    if (pstHdcp14->stAttr.u32R0ReadyTime < HDCP14_BR0_WAIT_TIMEOUT)
    {
        pstHdcp14->stAttr.u32R0ReadyTime = HDCP14_BR0_WAIT_TIMEOUT;
    }
    if (pstHdcp14->stAttr.u32BlistReadyTime < HDCP14_BKSVLIST_TIMEOUT)
    {
        pstHdcp14->stAttr.u32BlistReadyTime = HDCP14_BKSVLIST_TIMEOUT;
    }

    s32Ret |= HAL_HDMI_MachCfgGet(pstHdcp14->stInit.u32MachId,&stMachCtrl);
    stMachCtrl.u64Interval = pstAttr->u32MachInterval;
    s32Ret |= HAL_HDMI_MachCfgSet(pstHdcp14->stInit.u32MachId,&stMachCtrl);



    return s32Ret;
}


HI_S32  HAL_HDMI_Hdcp14AttrGet(HDMI_DEVICE_ID_E enHdmiId,HDCP14_ATTR_S *pstAttr)
{
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstAttr);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    HDMI_MEMCPY(pstAttr,&pstHdcp14->stAttr,sizeof(HDCP14_ATTR_S));

    return HI_SUCCESS;
}

HI_S32  HAL_HDMI_Hdcp14MachEnable(HDMI_DEVICE_ID_E enHdmiId,HI_BOOL bEnable)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    if (bEnable)
    {
        s32Ret |= HAL_HDMI_Hdcp14Reset(enHdmiId);
        pstHdcp14->stRun.bEnable            = HI_TRUE;
        pstHdcp14->stRun.u64MachStartStamp  = HAL_HDMI_MachMsGet();
        pstHdcp14->stRun.enMachStatus       = HDCP14_MACH_AUTHEN_READY;
        pstHdcp14->stRun.u64ReAuthWaitMs    = pstHdcp14->stAttr.u64StartWaitMs;
        s32Ret |= HAL_HDMI_Hdcp14LoadKey(enHdmiId);
        s32Ret |= HAL_HDMI_MachStart(pstHdcp14->stInit.u32MachId);
    }
    else
    {
        pstHdcp14->stRun.bEnable = HI_FALSE;
        s32Ret = HAL_HDMI_MachStop(pstHdcp14->stInit.u32MachId);
        Hdcp14RethenHwReset(enHdmiId);
    }

    return s32Ret;
}

HI_S32  HAL_HDMI_Hdcp14Support(HDMI_DEVICE_ID_E enHdmiId, HI_BOOL *pbSupport)
{
    HI_S32          s32Ret = HI_SUCCESS;
    HI_U8           au8Bksv[HDCP14_KSV_SIZE_5BYTES];
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pbSupport);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);

    s32Ret = Hdcp14BksvRead(enHdmiId,au8Bksv);

    *pbSupport = (s32Ret==HI_SUCCESS) ? HI_TRUE : HI_FALSE;

    return HI_SUCCESS;
}


HI_S32 HAL_HDMI_Hdcp14LoadKey(HDMI_DEVICE_ID_E enHdmiId)
{
    HI_S32          s32Ret = 0;
    HI_U32          u32TimeOut = 0;
    HDCP14_INFO_S   *pstHdcp14 = HI_NULL;

    pstHdcp14 = Hdcp14InfoGet(enHdmiId);
    HDCP14_NULL_CHK(pstHdcp14);
    HDCP14_INIT_CHK(pstHdcp14);
#if HDCP14_DEBUG_KEY
    /* just for DEBUG */
     Hdcp14LoadDebugSrcKey();
#endif
    Hdcp14LoadKeySet();
    u32TimeOut = 0;
    while ((!Hdcp14LoadFinishCheck()) && (u32TimeOut++ < 5) )
    {
        msleep(1);
    }
    pstHdcp14->stStatus.stPartI.enLoadKey = HDCP14_PROCESS_OK;
    if (u32TimeOut >= 5)
    {
        pstHdcp14->stStatus.stPartI.enLoadKey = HDCP14_PROCESS_FAIL;
        HDMI_ERR("hdcp14 loadkey  err!\n");
        s32Ret |= HI_FAILURE;
    }

    Hdcp14CrcCheckSet();
    u32TimeOut = 0;
    while (!Hdcp14CrcFinishCheck() && (u32TimeOut++ < 5) )
    {
        msleep(1);
    }
    pstHdcp14->stStatus.stPartI.enCrcCheck = HDCP14_PROCESS_OK;
    if (u32TimeOut >= 5)
    {
        pstHdcp14->stStatus.stPartI.enCrcCheck = HDCP14_PROCESS_FAIL;
        HDMI_ERR("hdcp14 crc err!\n");
        s32Ret |= HI_FAILURE;
    }

    return s32Ret;
}



