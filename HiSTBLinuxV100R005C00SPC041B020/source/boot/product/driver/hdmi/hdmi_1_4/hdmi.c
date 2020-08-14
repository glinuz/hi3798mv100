/*******************************************************************************
 *              Copyright 2005 - 2006, Hisilicon Tech. Co., Ltd.
 *                           ALL RIGHTS RESERVED
 * FileName:    hi_hdmi.h
 * Description: This is internal include file for irda
 *
 * History:
 * Version      Date         Author       DefectNum    Description
 *  0.01     2010-08-06      luochen      NULL       Create this file
 ******************************************************************************/

#include "hi_unf_hdmi.h"
//#include "mpi_hdmi.h"
#include "txvidp.h"
#include "hdmitx.h"
#include "infofrm.h"
//#include "eedefs.h"
#include "hi_drv_hdmi.h"
#include "de.h"
#include "hi_reg_common.h"

#include "hi_common.h"

#ifdef HI_HDMI_4K_SUPPORT
#include "vsdb.h"
#endif

#ifndef HI_MINIBOOT_SUPPORT
#include "linux/time.h"
#endif
#include "phy.h"

#include "timer.h"

//static HI_BOOL g_bHDMIResetFlag = HI_FALSE;
static HI_U32 g_HDMICnt = 0;
#if 0
#ifdef HI_HDMI_EDID_SUPPORT
#include "si_edid.h"

HI_VOID Print_EDID(HI_UNF_EDID_BASE_INFO_S *pSinkCap)
{
    HI_U32 Index;
    HDMIPrint("Manufacture name:%s\n",pSinkCap->stMfrsInfo.u8MfrsName);
    HDMIPrint("SupportHdmi:%d\n",pSinkCap->bSupportHdmi);
    HDMIPrint("enNativeFormat:%d\n",pSinkCap->enNativeFormat);
    HDMIPrint("Support Format:");
    for(Index = 0; Index < HI_UNF_ENC_FMT_BUTT; Index++)
    {
        if(pSinkCap->bSupportFormat[Index])
        {
           // HDMIPrint("%d,",Index);
        }
    }
   // HDMIPrint("\n");
    HDMIPrint("3D Support:%d\n",pSinkCap->st3DInfo.bSupport3D);
    /*......*/
}

HI_S32 HI_UNF_HDMI_SetBaseParamByEDID(HI_VOID)
{
    HI_S32 s32Ret;
    HI_UNF_EDID_BASE_INFO_S SinkCap;
    HI_DISP_PARAM_S         stDispParam;
    s32Ret = HI_DRV_PDM_GetDispParam(HI_UNF_DISPLAY1, &stDispParam);
    if(HI_SUCCESS != s32Ret)
    {
        HDMIPrint("HI_PDM_GetGrcParam err! Ret = %x\n", s32Ret);
        return s32Ret;
    }

    s32Ret = HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0,&SinkCap);
    if(s32Ret != HI_SUCCESS)
    {
        HDMIPrint("get edid capability Failed!\n");
        return s32Ret;
    }
    Print_EDID(&SinkCap);

    if(SinkCap.bSupportHdmi)
    {
        stDispParam.enFormat = SinkCap.enNativeFormat;
    }
    else //DVI mode
    {
        stDispParam.enFormat = HI_UNF_ENC_FMT_BUTT;
    }

    //if native formatt == butt, so we need to set detailed timing every time
    stDispParam.stDispTiming.VFB = SinkCap.stPerferTiming.u32VFB;
    stDispParam.stDispTiming.VBB = SinkCap.stPerferTiming.u32VBB;
    stDispParam.stDispTiming.VACT= SinkCap.stPerferTiming.u32VACT;
    stDispParam.stDispTiming.HFB = SinkCap.stPerferTiming.u32HFB;
    stDispParam.stDispTiming.HBB = SinkCap.stPerferTiming.u32HBB;
    stDispParam.stDispTiming.HACT= SinkCap.stPerferTiming.u32HACT;
    stDispParam.stDispTiming.VPW = SinkCap.stPerferTiming.u32VPW;
    stDispParam.stDispTiming.HPW = SinkCap.stPerferTiming.u32HPW;
    stDispParam.stDispTiming.IDV = SinkCap.stPerferTiming.bIDV;
    stDispParam.stDispTiming.IHS = SinkCap.stPerferTiming.bIHS;
    stDispParam.stDispTiming.IVS = SinkCap.stPerferTiming.bIVS;
    stDispParam.stDispTiming.AspectRatioW = SinkCap.stPerferTiming.u32ImageWidth;
    stDispParam.stDispTiming.AspectRatioH = SinkCap.stPerferTiming.u32ImageHeight;
    stDispParam.stDispTiming.bInterlace = SinkCap.stPerferTiming.bInterlace;
    stDispParam.stDispTiming.PixFreq = SinkCap.stPerferTiming.u32PixelClk;

    stDispParam.stDispTiming.ClkPara0 = 0;
    stDispParam.stDispTiming.ClkPara1 = 0;
    stDispParam.stDispTiming.VertFreq = 0;

    s32Ret = HI_DRV_PDM_UpdateDispParam(HI_UNF_DISPLAY1,&stDispParam);
    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_HDMI("HI_PDM_Update Param err! Ret = %x\n", s32Ret);
        return s32Ret;
    }
    return HI_SUCCESS;
}

HI_S32 HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_EDID_BASE_INFO_S *pSinkCap)
{
    HI_S32 s32Ret  = 0;
    HI_U32 u32Cnt = 0;

    if(!g_bHDMIResetFlag)
    {
        HW_ResetHDMITX();
        SW_ResetHDMITX();
        g_bHDMIResetFlag = HI_TRUE;
    }

    for(u32Cnt = 0; u32Cnt < 5; u32Cnt++)
    {
        s32Ret = SI_GetSinkCapability(pSinkCap);
        HDMIPrint("Read edid times:%d\n",u32Cnt);
        if(s32Ret == HI_SUCCESS)
        {
            HDMIPrint("read edid over\n");
            break;
        }
    }
    return s32Ret;
}
#endif
#endif
#ifdef HI_HDMI_CEC_SUPPORT
HI_S32 HDMI_SendCECCommand(HI_UNF_HDMI_CEC_CMD_S *pstCECCmd)
{
    HI_U32 u32Reg,u32Status,u32Cnt = 0;

    //0xc0:0x8e init
    WriteByteHDMITXCEC(0x8e,0x04);

    //0xc0:0x87 clear tx cec fifo
    WriteByteHDMITXCEC(0x87,0x80);

    //0xc0:0xa6 0xa7 clear cec interrupt
    WriteByteHDMITXCEC(0xa6,0x64);
    WriteByteHDMITXCEC(0xa7,0x02);

    //0xc0:0x88 CEC_TX_INIT  addr
    WriteByteHDMITXCEC(0x88,pstCECCmd->enSrcAdd);
    //0xc0:0x89 CEC_TX_DEST addr
    WriteByteHDMITXCEC(0x89,pstCECCmd->enDstAdd);
    //0xc0:0x8f CEC_TX_COMMAND
    WriteByteHDMITXCEC(0x8f,pstCECCmd->u8Opcode)


    //cec send data
    //HDMI_REG_WRITE(0xf8ce0a40,0);
    WriteByteHDMITXCEC(0x90,0)
    if(pstCECCmd->unOperand.stRawData.u8Length > 15)
    {
        HDMIPrint("cec data too long!\n");
        pstCECCmd->unOperand.stRawData.u8Length = 15;
    }
    for(u32Cnt = 0; u32Cnt < pstCECCmd->unOperand.stRawData.u8Length; u32Cnt++)
    {
        //u32Reg = u32Cnt * 4;
        WriteByteHDMITXCEC((0x90 + u32Cnt),pstCECCmd->unOperand.stRawData.u8Data[u32Cnt]);
    }

    //0xc0:0x9f bit[4] send cec cmd // dual mode document err
    WriteByteHDMITXCEC(0x9f,0x10 | pstCECCmd->unOperand.stRawData.u8Length);//startup

    return HI_SUCCESS;
}
#endif
HI_U32 HDMI_Display(HI_UNF_ENC_FMT_E enHdFmt, HI_UNF_HDMI_VIDEO_MODE_E einput, HI_UNF_HDMI_VIDEO_MODE_E eoutput)
{
    HI_U8  u8AviInfoFrameByte = 0;
    HI_U8  u8VideoPath[4];
    HI_U8  pu8AviInfoFrame[32];
    HI_U32 bVideoMode = 0, VidCode = 0, bRxVideoMode = 0, bTxVideoMode = 0;
    HI_U32 retval = HI_SUCCESS, hdmi_dvi_mode = 1;
    HI_BOOL Support4K = HI_FALSE;

    HI_U32 u32PixelRepetition;
    HI_UNF_HDMI_COLORSPACE_E enColorimetry;
    HI_UNF_HDMI_ASPECT_RATIO_E enAspectRate;
    
	HDMIPrint(">>> HDMI_Display in... \n");

    //U_PERI_CRG70    crg70;
#ifdef HI_HDMI_EDID_SUPPORT
    HI_UNF_EDID_BASE_INFO_S SinkCap;
    HI_S32 s32Ret  = HI_SUCCESS;
#endif
#ifdef HI_HDMI_CEC_SUPPORT
    HI_UNF_HDMI_CEC_CMD_S stCECCmd;
#endif
#if 0 /*--for debug used time,do not delete--*/
    HI_INFO_PRINT(HI_ID_HDMI,"hdmi Init \n");
    ResetTimer();
    GetTimer();
#endif

    /* Video Inforframe */
    enColorimetry      = HDMI_COLORIMETRY_ITU709;
    enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_16TO9;
    u32PixelRepetition = HI_FALSE;

#ifdef HI_HDMI_EDID_SUPPORT
    s32Ret = HI_UNF_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_0,&SinkCap);
    if(HI_SUCCESS == s32Ret)
    {
        if(SinkCap.bSupportHdmi)
        {
            hdmi_dvi_mode = 1;
        }
        else //dvi
        {
            hdmi_dvi_mode = 0;
        }
    }
#endif

    if(HI_UNF_ENC_FMT_1080P_60 == enHdFmt)
    {
        VidCode = 16;
        bVideoMode = 0x0b;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080P_60\n");
    }
    else if(HI_UNF_ENC_FMT_1080P_50 == enHdFmt)
    {
        VidCode = 31;
        bVideoMode = 0x18;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080P_50\n");
    }
    else if(HI_UNF_ENC_FMT_1080P_30 == enHdFmt)
    {
        VidCode = 34;
        bVideoMode = 0x1b;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080P_30\n");
    }
    else if(HI_UNF_ENC_FMT_1080P_25 == enHdFmt)
    {
        VidCode = 33;
        bVideoMode = 0x1a;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080P_25\n");
    }
    else if(HI_UNF_ENC_FMT_1080P_24 == enHdFmt)
    {
        VidCode = 32;
        bVideoMode = 0x19;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080P_24\n");
    }
    else if(HI_UNF_ENC_FMT_1080i_60 == enHdFmt)
    {
        VidCode = 5;
        bVideoMode = 0x03;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080i_60\n");
    }
    else if(HI_UNF_ENC_FMT_1080i_50 == enHdFmt)
    {
        VidCode = 20;
        bVideoMode = 0x0e;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_1080i_50\n");
    }
    else if(HI_UNF_ENC_FMT_720P_60 == enHdFmt)
    {
        VidCode = 4;
        bVideoMode = 0x02;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_720P_60\n");
    }
    else if(HI_UNF_ENC_FMT_720P_50 == enHdFmt)
    {
        VidCode = 19;
        bVideoMode = 0x0d;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_720P_50\n");
    }
    else if(HI_UNF_ENC_FMT_576P_50 == enHdFmt)
    {
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        VidCode = 17;
        bVideoMode = 0x0c;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_576P_50\n");
    }
    else if(HI_UNF_ENC_FMT_480P_60 == enHdFmt)
    {
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        VidCode = 2;
        bVideoMode = 0x01;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_480P_60\n");
    }
    else if(HI_UNF_ENC_FMT_PAL == enHdFmt)
    {
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
        VidCode = 21;
        bVideoMode = 0x0f;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_PAL\n");
    }
    else if(HI_UNF_ENC_FMT_NTSC == enHdFmt)
    {
        enColorimetry = HDMI_COLORIMETRY_ITU601;
        enAspectRate  = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_TRUE;
        VidCode = 0x06;
        bVideoMode = 0x04;
        HDMIPrint("HDMI input format is HI_UNF_ENC_FMT_NTSC\n");
    }
    // for surpport VGA format bootlogo
    else if(HI_UNF_ENC_FMT_861D_640X480_60 == enHdFmt)
    {
        HDMIPrint("Set 640X480P_60000 enTimingMode\n");
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;
        VidCode = 0x01;

    }
#ifdef HI_HDMI_4K_SUPPORT
    else if (Is4KFmt(enHdFmt))
    {
        HDMIPrint("Set 4K enTimingMode\n");
        VidCode = 0;
        bVideoMode = 0;
    }
#endif
    else if ((HI_UNF_ENC_FMT_VESA_800X600_60 <= enHdFmt) && (HI_UNF_ENC_FMT_BUTT > enHdFmt))
    {
        //hdmi_dvi_mode = 0;
        HDMIPrint("DVI timing mode enTimingMode\n");
        enColorimetry      = HDMI_COLORIMETRY_ITU601;
        enAspectRate       = HI_UNF_HDMI_ASPECT_RATIO_4TO3;
        u32PixelRepetition = HI_FALSE;
    }
    //OpenHdmiDevice();

    //WriteDefaultConfigToEEPROM(); /* eeprom.c */
#if !(defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)|| defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420))
#if 0
    if(!g_bHDMIResetFlag)
    {
        HW_ResetHDMITX();
        SW_ResetHDMITX();
    }
    HDMIPrint("g_bHDMIResetFlag:%d\n",g_bHDMIResetFlag);
#else
    HW_ResetHDMITX();
    SW_ResetHDMITX();
#endif
#endif
    

    UpdateTX_656(bVideoMode);
    //SetIClk( ReadByteEEPROM(EE_TX_ICLK_ADDR) );
    SetIClk(0);

    WakeUpHDMITX();
#ifdef HI_HDMI_CEC_SUPPORT
    memset(&stCECCmd, 0, sizeof(HI_UNF_HDMI_CEC_CMD_S));
    stCECCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_BROADCAST;
    stCECCmd.enSrcAdd = 0x03;
    stCECCmd.u8Opcode = 0x0d;
    stCECCmd.unOperand.stUIOpcode = 0x0;
    stCECCmd.unOperand.stRawData.u8Length = 0;
    HDMI_SendCECCommand(&stCECCmd);
#ifdef HI_HDMI_EDID_SUPPORT
    stCECCmd.enDstAdd = HI_UNF_CEC_LOGICALADD_TV;
    HDMI_SendCECCommand(&stCECCmd);
#endif
#endif

#if 0 /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/
    //之前的一个规避方案。音频需要打开时钟，输出0电平。
    // 部分电视，如果只开启了hdmi mode，而没有开启audio，会有杂音。
    crg70.u32 = g_pstRegCrg->PERI_CRG70.u32;

    // In 3716Cv200 series chip,when aiao_mclk_sel == 0,audio mclock is 99xMhz
    crg70.bits.aiao_cken        = 1;
    crg70.bits.aiao_srst_req    = 0;
    crg70.bits.aiao_clk_sel     = 0;
    crg70.bits.aiaoclk_skipcfg  = 0;
    crg70.bits.aiaoclk_loaden   = 0;
    crg70.bits.aiao_mclk_sel    = 0;

    g_pstRegCrg->PERI_CRG70.u32 = crg70.u32;

    //config AO =>HDMI mclk to 12.288Mhz
    //HDMI Audio clk Frequency division
    HDMI_REG_WRITE(0xf8cd0158, 0x333333);
    HDMI_REG_WRITE(0xf8cd015c, 0x133);

    HDMI_REG_WRITE(0xf8ce040c, 0x0);
    HDMI_REG_WRITE(0xf8ce0410, 0x18);
    HDMI_REG_WRITE(0xf8ce0414, 0x0);

    //I2S通路打开，用于计算cts
    HDMI_REG_WRITE(0xf8ce0450,0x10);
#endif /*--NO MODIFY : COMMENT BY CODINGPARTNER--*/

    //close N/CTS packet
    WriteByteHDMITXP1(ACR_CTRL_ADDR,0X04);

    /* Set Video Path */
    HDMIPrint("einput:%d, eoutput:%d\n", einput, eoutput);
    if(HI_UNF_HDMI_VIDEO_MODE_RGB444 == einput)
    {
        bRxVideoMode = 0;  /* inRGB24[] */
        if(HI_UNF_HDMI_VIDEO_MODE_RGB444 == eoutput)
        {
            bTxVideoMode = 0;
        }
        else
        {
            HDMIPrint("Error output mode when input RGB444\n");
            bTxVideoMode = 0;
        }
    }
    else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == einput)
    {
        bRxVideoMode = 2; /* inYCbCr24[] */
        if(HI_UNF_HDMI_VIDEO_MODE_RGB444 == eoutput)
        {
            bTxVideoMode = 0;
        }
        else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == eoutput)
        {
            bTxVideoMode = 1;
        }
        else
        {
            HDMIPrint("Error output mode when input YCbCr444\n");
            bTxVideoMode = 0;
        }
    }
    else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR422 == einput)
    {
        bRxVideoMode = 3; /* inYC24[] */
        if(HI_UNF_HDMI_VIDEO_MODE_RGB444 == eoutput)
        {
            bTxVideoMode = 0;
        }
        else if(HI_UNF_HDMI_VIDEO_MODE_YCBCR444 == eoutput)
        {
            bTxVideoMode = 1;
        }
        else
        {
            bTxVideoMode = 2;
        }
    }
    u8VideoPath[0] = bRxVideoMode;
    u8VideoPath[1] = bTxVideoMode;

#if    defined(CHIP_TYPE_hi3716cv200)   \
    || defined(CHIP_TYPE_hi3716mv400)   \
    || defined(CHIP_TYPE_hi3718cv100)   \
    || defined(CHIP_TYPE_hi3719cv100)
    //for 3716Cv200 series the clk is Edge need select to falling edge
    //the default value for 0x72:0x08 is 0x37,and the clock has been holded in falling edge
    HDMIPrint("Set 0x08:0x35 \n");
    u8VideoPath[2] = 0x00;//0x35
#elif defined(CHIP_TYPE_hi3719mv100) || defined(CHIP_TYPE_hi3718mv100)
    //default 0x37 not 0x35
    HDMIPrint("Set 0x08:0x37 \n");
    u8VideoPath[2] = 0x01;//0x37
#endif

    u8VideoPath[3] = 0xFF;

    siiSetVideoPath(bVideoMode, u8VideoPath);

    /* HDMI AVI Infoframe is use Version = 0x02 in HDMI1.3 */
    /* Fill Data Byte 1 */
    u8AviInfoFrameByte=0;
    /* Scan information bits 0-1:S0,S1 */
    /*
           S1 S0 Scan Information
           0   0    No Data
           0   1   overscanned
           1   0   underscanned
           1   1   Future
    */
    u8AviInfoFrameByte |= (HI_U8)0x00;
    /* Bar Information bits 2-3:B0,B1 */
    /*
         B1 B0  Bar Info
         0   0  not valid
         0   1  Vert. Bar Info valid
         1   0  Horiz.Bar Info Valid
         1   1  Vert. and Horiz. Bar Info valid
    */
    u8AviInfoFrameByte |= (HI_U8) 0x00;
    /* Active information bit 4:A0 */
    /*
         A0 Active Format Information Present
         0        No Data
         1      Active Format(R0...R3) Information valid
    */
    u8AviInfoFrameByte |= (HI_U8)0x10;
    /* Output Type bits 5-6:Y0,Y1 */
    /*
         Y1 Y0  RGB orYCbCr
         0  0   RGB (default)
         0  1   YCbCr 4:2:2
         1  0   YCbCr 4:4:4
         1  1    Future
    */
    switch (eoutput)
    {
        case HI_UNF_HDMI_VIDEO_MODE_RGB444 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR422 :
            u8AviInfoFrameByte |= (HI_U8)0x20;
            break;
        case HI_UNF_HDMI_VIDEO_MODE_YCBCR444 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        default :
            HDMIPrint("Error Output format *******\n");
            retval = (HI_U32)HI_FAILURE;
            break;
    }
    pu8AviInfoFrame[0]= (HI_U8)(u8AviInfoFrameByte&0x7F);

    /* Fill Data byte 2 */
    u8AviInfoFrameByte=0;
    /* Active Format aspect ratio bits 0-3:R0...R3 */
    /*
        R3 R2 R1 R0  Active Format Aspect Ratio
        1  0  0  0   Same as picture aspect ratio
        1  0  0  1   4:3 (Center)
        1  0  1  0   16:9 (Center)
        1  0  1  1   14:9 (Center)
    */
#if 0
    switch (enAspectRate)
    {
        case HI_UNF_HDMI_ASPECT_RATIO_4TO3 :
            u8AviInfoFrameByte |= (HI_U8) 0x09;
            break;
        case HI_UNF_HDMI_ASPECT_RATIO_16TO9 :
            u8AviInfoFrameByte |= (HI_U8) 0x0A;
            break;
        case HI_UNF_HDMI_ASPECT_RATIO_14TO9:
            u8AviInfoFrameByte |= (HI_U8) 0x0B;
            break;
        default :
            u8AviInfoFrameByte |= (HI_U8) 0x08;
            break;
     }
#else
    HDMIPrint("Active Format aspect ratio  set to 0x1000:Same as picture aspect ratio\n");
    u8AviInfoFrameByte |= (HI_U8) 0x08;
#endif
    /* Picture aspect ratio bits 4-5:M0.M1 */
    /*
        M1 M0     Picture Aspect Ratio
        0  0        No Data
        0  1        4:3
        1  0        16:9
        1  1        Future
    */
    switch (enAspectRate)
    {
        case HI_UNF_HDMI_ASPECT_RATIO_4TO3 :
             u8AviInfoFrameByte |= (HI_U8) 0x10;
             break;
        case HI_UNF_HDMI_ASPECT_RATIO_16TO9 :
             u8AviInfoFrameByte |= (HI_U8) 0x20;
                break;
            default :
             u8AviInfoFrameByte |=  (HI_U8) 0x00;
             break;
     }

    /* Colorimetry bits 6-7 of data byte2:C0,C1 */
    /*
        C1 C0    Colorim
        0   0    No Data
        0   1    SMPTE 170M[1] ITU601 [5]
        1   0    ITU709 [6] 1 0 16:9
        1   1    Extended Colorimetry Information Valid (colorimetry indicated in bits EC0, EC1,
                     EC2. See Table 11)
    */
    switch (enColorimetry)
    {
        case HDMI_COLORIMETRY_ITU601 :
            u8AviInfoFrameByte |= (HI_U8)0x40;
            break;
        case HDMI_COLORIMETRY_ITU709 :
            u8AviInfoFrameByte |= (HI_U8)0x80;
            break;
        case HDMI_COLORIMETRY_XVYCC_601 :
        case HDMI_COLORIMETRY_XVYCC_709 :
        case HDMI_COLORIMETRY_EXTENDED :
            u8AviInfoFrameByte |= (HI_U8)0xC0;
            break;
        default :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
    }
    pu8AviInfoFrame[1] = (HI_U8)(u8AviInfoFrameByte&0XFF);

    /* Fill data Byte 3: Picture Scaling bits 0-1:SC0,SC1 */
    u8AviInfoFrameByte=0;
    /*
       SC1  SC0   Non-Uniform Picture Scaling
       0     0    No Known non-uniform Scaling
       0     1    Picture has been scaled horizontally
       1     0    Picture has been scaled vertically
       1     1    Picture has been scaled horizontally and vertically
    */
    u8AviInfoFrameByte |= (HI_U8)0x00;
    /* Fill data Byte 3: RGB quantization range bits 2-3:Q0,Q1 */
    /*
        Q1  Q0  RGB Quantization Range
        0   0   Default (depends on video format)
        0   1   Limited Range
        1   0   Full Range
        1   1   Reserved
    */
    u8AviInfoFrameByte |= (HI_U8)0x00;
    /* Fill data Byte 3: Extended colorimtery range bits 4-6:EC0,EC1,EC2 */
    /*
        EC2 EC1 EC0   Extended Colorimetry
        0   0   0      xvYCC601
        0   0   1      xvYCC709
        -   -   -      All other values reserved
    */
    /*
        xvYCC601 is based on the colorimetry defined in ITU-R BT.601.
        xvYCC709 is based on the colorimetry defined in ITU-R BT.709.
    */
    switch (enColorimetry)
    {
        case HDMI_COLORIMETRY_XVYCC_601 :
            u8AviInfoFrameByte |= (HI_U8)0x00;
            break;
        case HDMI_COLORIMETRY_XVYCC_709 :
            u8AviInfoFrameByte |= (HI_U8)0x10;
            break;
        default:
            break;
    }
    /* Fill data Byte 3: IT content bit 7:ITC */
    /*
        ITC  IT content
        0    No data
        1    IT content
    */

    u8AviInfoFrameByte &= ~0x80;
    pu8AviInfoFrame[2] = (HI_U8)(u8AviInfoFrameByte&0XFF);

    /* Fill Data byte 4: Video indentification data Code, Bit0~7:VIC0 ~ VIC6 */
    u8AviInfoFrameByte=0;
    pu8AviInfoFrame[3] = (HI_U8)(VidCode & 0x7F);

    /* Fill Data byte 5: Pixel repetition, Bit0~3:PR0~PR3 */
    /*
        PR3 PR2 PR1 PR0 Pixel Repetition Factor
        0   0   0    0   No Repetition (i.e., pixel sent once)
        0   0   0    1   pixel sent 2 times (i.e., repeated once)
        0   0   1    0   pixel sent 3 times
        0   0   1    1   pixel sent 4 times
        0   1   0    0   pixel sent 5 times
        0   1   0    1   pixel sent 6 times
        0   1   1    0   pixel sent 7 times
        0   1   1    1   pixel sent 8 times
        1   0   0    0   pixel sent 9 times
        1   0   0    1   pixel sent 10 times
        0Ah-0Fh          Reserved
    */
    pu8AviInfoFrame[4]= (HI_U8)(u32PixelRepetition& 0x0F);

     /* Fill Data byte 6  */
    pu8AviInfoFrame[5] = 0x00;

    /* Fill Data byte 7  */
    pu8AviInfoFrame[6] = 0x00;

    /* Fill Data byte 8  */
    pu8AviInfoFrame[7] = 0x00;

    /* Fill Data byte 9  */
    pu8AviInfoFrame[8] = 0x00;

    /* Fill Data byte 10  */
    pu8AviInfoFrame[9] = 0x00;

    /* Fill Data byte 11  */
    pu8AviInfoFrame[10] = 0x00;

    /* Fill Data byte 12  */
    pu8AviInfoFrame[11] = 0x00;

    /* Fill Data byte 13  */
    pu8AviInfoFrame[12] = 0x00;

    /*
    720X480P_60000  :0x51,0x59,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    720X576P_50000  :0x51,0x59,0x00,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1280X720P_50000 :0x51,0xaa,0x00,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1280X720P_60000 :0x51,0xaa,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080i_50000:0x51,0xaa,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080i_60000:0x51,0xaa,0x00,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080P_24000:0x51,0xaa,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080P_25000:0x51,0xaa,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080P_30000:0x51,0xaa,0x00,0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080P_50000:0x51,0xaa,0x00,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    1920X1080P_60000:0x51,0xaa,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    */

#if 0
    HI_U8 Index;

    HDMIPrint("The Content of the AVI version2 info frame is:\n");
    for (Index=0; Index<17; Index++)
    {
        HDMIPrint("0x%02x,",pu8AviInfoFrame[Index]);
    }
    HDMIPrint("\n");
#endif

    //BlockWriteEEPROM( 13, EE_TXAVIINFO_ADDR, pu8AviInfoFrame);
    if(hdmi_dvi_mode){
        SendAVIInfoFrame(pu8AviInfoFrame,13);
        EnableInfoFrame(AVI_TYPE);
    }

#ifdef HI_HDMI_4K_SUPPORT
    if (Is4KFmt(enHdFmt))
    {
        Support4K = HI_TRUE;

        HDMIPrint("Is 4K fmt \n");

        TX_PHY_4KRisingTime(HI_TRUE);
        Setting4K(enHdFmt);
        EnableInfoFrame(VENDORSPEC_TYPE);
    }
    else
    {
        HDMIPrint("Is not 4K fmt \n");
    }
#endif

    if ((enHdFmt <= HI_UNF_ENC_FMT_720P_50) || Support4K)
    {
        // HD TV Fmt Atribute
        TX_PHY_HighBandwidth(HI_TRUE);
    }
    else
    {
        TX_PHY_HighBandwidth(HI_FALSE);
    }

    if(Support4K)
    {
        TX_PHY_SwingCtrl(SWING_300M);
    }
    else if((enHdFmt == HI_UNF_ENC_FMT_861D_640X480_60)
        || (enHdFmt == HI_UNF_ENC_FMT_576P_50)
        || (enHdFmt == HI_UNF_ENC_FMT_480P_60)

        || (enHdFmt == HI_UNF_ENC_FMT_PAL)
        || (enHdFmt == HI_UNF_ENC_FMT_PAL_N)
        || (enHdFmt == HI_UNF_ENC_FMT_PAL_Nc)

        || (enHdFmt == HI_UNF_ENC_FMT_NTSC)
        || (enHdFmt == HI_UNF_ENC_FMT_NTSC_J)
        || (enHdFmt == HI_UNF_ENC_FMT_NTSC_PAL_M)

        || (enHdFmt == HI_UNF_ENC_FMT_SECAM_SIN)
        || (enHdFmt == HI_UNF_ENC_FMT_SECAM_COS))
    {
        TX_PHY_SwingCtrl(SWING_27M);
    }
    else
    {
        TX_PHY_SwingCtrl(SWING_NOMAL);
    }

    TX_SetHDMIMode(hdmi_dvi_mode);    //for hdmi

    //SendCP_Packet(OFF);
    {
        HI_U8 RegVal;
        WriteByteHDMITXP1( CP_IF_ADDR, BIT_CP_AVI_MUTE_CLEAR);
        RegVal = ReadByteHDMITXP1( INF_CTRL2 );
        WriteByteHDMITXP1(INF_CTRL2, RegVal | BIT_CP_ENABLE | BIT_CP_REPEAT);
    }

    TX_PHY_EnableHdmiOutput();

#if 0 /*--for debug used time--*/
    HI_INFO_PRINT(HI_ID_HDMI,"leave hdmi display \n");
    GetTimer();
#endif

    HDMIPrint(">>> HDMI_Display out... \n");

    return retval;
}

HI_S32 HI_UNF_HDMI_Init()
{
    HDMIPrint(">>> HI_UNF_HDMI_Init in... \n");

    HW_ResetHDMITX();
    SW_ResetHDMITX();
    TX_PHY_PowerDown(HI_TRUE);

    HDMIPrint("<<< HI_UNF_HDMI_Init out... \n");

    return HI_SUCCESS;
}

HI_S32 HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_ENC_FMT_E enFormat)
{
    HI_S32  Ret;
    HI_BOOL Support4K = HI_FALSE;

    HDMIPrint(">>> HI_UNF_HDMI_Open in... \n");

#if defined(CHIP_TYPE_hi3798mv100) || defined(CHIP_TYPE_hi3796mv100) || defined(CHIP_TYPE_hi3716dv100)|| defined(CHIP_TYPE_hi3716mv410) || defined(CHIP_TYPE_hi3716mv420)
    if (!TX_RSEN_Status() || g_HDMICnt)
    {
        HDMIPrint("Rsen: %d, g_HDMICnt: %d\n", TX_RSEN_Status(), g_HDMICnt);
        return HI_FAILURE;
    }
    g_HDMICnt++;
#endif

#ifdef HI_HDMI_4K_SUPPORT
    Support4K = Is4KFmt(enFormat);
#endif

    TX_PHY_PowerDown(HI_FALSE);
    if ((enFormat <= HI_UNF_ENC_FMT_720P_50_FRAME_PACKING) || Support4K)
    {
        Ret = HDMI_Display(enFormat, HI_UNF_HDMI_VIDEO_MODE_YCBCR444, HI_UNF_HDMI_VIDEO_MODE_YCBCR444);
    }
    else
    {
        Ret = HDMI_Display(enFormat, HI_UNF_HDMI_VIDEO_MODE_RGB444, HI_UNF_HDMI_VIDEO_MODE_RGB444);
    }

    HDMIPrint("<<< HI_UNF_HDMI_Open out... \n");

    return Ret;
}

HI_S32 HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMIPrint(">>> HI_UNF_HDMI_Start in... \n");

    HDMIPrint("<<< HI_UNF_HDMI_Start out... \n");
    return HI_SUCCESS;
}

HI_S32 HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_E enHdmi)
{
    HDMIPrint(">>> HI_UNF_HDMI_Close in... \n");

    HDMIPrint("<<< HI_UNF_HDMI_Close out... \n");

    return HI_SUCCESS;
}

HI_S32 HI_UNF_HDMI_DeInit(void)
{
    HDMIPrint(">>> HI_UNF_HDMI_DeInit in... \n");

    HDMIPrint("<<< HI_UNF_HDMI_DeInit out... \n");

    return HI_SUCCESS;
}

HI_UNF_ENC_FMT_E hdmi_Disp2EncFmt(HI_DRV_DISP_FMT_E SrcFmt)
{
    HI_UNF_ENC_FMT_E dstFmt = HI_UNF_ENC_FMT_BUTT;

    HDMIPrint(">>> hdmi_Disp2EncFmt in... \n");

    switch(SrcFmt)
    {
        case HI_DRV_DISP_FMT_1080P_60 :
            dstFmt = HI_UNF_ENC_FMT_1080P_60;
            break;
        case HI_DRV_DISP_FMT_1080P_50 :
            dstFmt = HI_UNF_ENC_FMT_1080P_50;
            break;
        case HI_DRV_DISP_FMT_1080P_30 :
            dstFmt = HI_UNF_ENC_FMT_1080P_30;
            break;
        case HI_DRV_DISP_FMT_1080P_25 :
            dstFmt = HI_UNF_ENC_FMT_1080P_25;
            break;
        case HI_DRV_DISP_FMT_1080P_24 :
        case HI_DRV_DISP_FMT_1080P_24_FP:
            dstFmt = HI_UNF_ENC_FMT_1080P_24;
            break;
        case HI_DRV_DISP_FMT_1080i_60 :
            dstFmt = HI_UNF_ENC_FMT_1080i_60;
            break;
        case HI_DRV_DISP_FMT_1080i_50 :
            dstFmt = HI_UNF_ENC_FMT_1080i_50;
            break;
        case HI_DRV_DISP_FMT_720P_60 :
        case HI_DRV_DISP_FMT_720P_60_FP:
            dstFmt = HI_UNF_ENC_FMT_720P_60;
            break;
        case HI_DRV_DISP_FMT_720P_50 :
        case HI_DRV_DISP_FMT_720P_50_FP:
            dstFmt = HI_UNF_ENC_FMT_720P_50;
            break;
        case HI_DRV_DISP_FMT_576P_50 :
            dstFmt = HI_UNF_ENC_FMT_576P_50;
            break;
        case HI_DRV_DISP_FMT_480P_60 :
            dstFmt = HI_UNF_ENC_FMT_480P_60;
            break;
        case HI_DRV_DISP_FMT_PAL:
        case HI_DRV_DISP_FMT_PAL_B:
        case HI_DRV_DISP_FMT_PAL_B1:
        case HI_DRV_DISP_FMT_PAL_D:
        case HI_DRV_DISP_FMT_PAL_D1:
        case HI_DRV_DISP_FMT_PAL_G:
        case HI_DRV_DISP_FMT_PAL_H:
        case HI_DRV_DISP_FMT_PAL_K:
        case HI_DRV_DISP_FMT_PAL_I:
        case HI_DRV_DISP_FMT_PAL_M:
        case HI_DRV_DISP_FMT_PAL_N:
        case HI_DRV_DISP_FMT_PAL_Nc:
        case HI_DRV_DISP_FMT_PAL_60:
        case HI_DRV_DISP_FMT_1440x576i_50:

        case HI_DRV_DISP_FMT_SECAM_SIN:
        case HI_DRV_DISP_FMT_SECAM_COS:
        case HI_DRV_DISP_FMT_SECAM_L:
        case HI_DRV_DISP_FMT_SECAM_B:
        case HI_DRV_DISP_FMT_SECAM_G:
        case HI_DRV_DISP_FMT_SECAM_D:
        case HI_DRV_DISP_FMT_SECAM_K:
        case HI_DRV_DISP_FMT_SECAM_H:
            dstFmt = HI_UNF_ENC_FMT_PAL;
            break;
        case HI_DRV_DISP_FMT_NTSC :
        case HI_DRV_DISP_FMT_NTSC_J :
        case HI_DRV_DISP_FMT_NTSC_443 :
        case HI_DRV_DISP_FMT_1440x480i_60:
            dstFmt = HI_UNF_ENC_FMT_NTSC;
            break;
        case HI_DRV_DISP_FMT_861D_640X480_60 :
            dstFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            break;
        case HI_DRV_DISP_FMT_VESA_800X600_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_800X600_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1024X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1024X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X720_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X720_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X800_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X800_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1280X1024_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1280X1024_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1360X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1360X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1366X768_60 :
            dstFmt = HI_UNF_ENC_FMT_VESA_1366X768_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1400X1050_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1400X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1440X900_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1440X900_60_RB:
            dstFmt = HI_UNF_ENC_FMT_VESA_1440X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X900_60_RB:
            dstFmt = HI_UNF_ENC_FMT_VESA_1600X900_60_RB;
            break;
        case HI_DRV_DISP_FMT_VESA_1600X1200_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1600X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1680X1050_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1680X1050_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1080_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1920X1080_60;
            break;
        case HI_DRV_DISP_FMT_VESA_1920X1200_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_1920X1200_60;
            break;
        case HI_DRV_DISP_FMT_VESA_2048X1152_60:
            dstFmt = HI_UNF_ENC_FMT_VESA_2048X1152_60;
            break;
        case HI_DRV_DISP_FMT_VESA_2560X1440_60_RB:
        case HI_DRV_DISP_FMT_VESA_2560X1600_60_RB:
        case HI_DRV_DISP_FMT_CUSTOM:
            dstFmt = HI_UNF_ENC_FMT_861D_640X480_60;
            break;
        case HI_DRV_DISP_FMT_3840X2160_24:
            dstFmt = HI_UNF_ENC_FMT_3840X2160_24;
            break;
        case HI_DRV_DISP_FMT_3840X2160_25:
            dstFmt = HI_UNF_ENC_FMT_3840X2160_25;
            break;
        case HI_DRV_DISP_FMT_3840X2160_30:
            dstFmt = HI_UNF_ENC_FMT_3840X2160_30;
            break;
        case HI_DRV_DISP_FMT_4096X2160_24:
            dstFmt = HI_UNF_ENC_FMT_4096X2160_24;
            break;
        default:
            dstFmt = HI_UNF_ENC_FMT_BUTT;
            break;
    }

    HDMIPrint("<<< hdmi_Disp2EncFmt out... \n");

    return dstFmt;
}


HI_S32 HI_DRV_HDMI_Init(void)
{
    HDMIPrint(">>> HI_DRV_HDMI_Init in... \n");

    HDMIPrint("<<< HI_DRV_HDMI_Init out... \n");
    
    return HI_SUCCESS;
}

HI_S32 HI_DRV_HDMI_Open(HI_DRV_DISP_FMT_E enFormat)
{
    HI_S32 s32RetVal = HI_SUCCESS;

    HDMIPrint(">>> HI_DRV_HDMI_Open in... \n");

    s32RetVal = HI_UNF_HDMI_Open(HI_UNF_HDMI_ID_0,hdmi_Disp2EncFmt(enFormat));
 
    HDMIPrint("<<< HI_DRV_HDMI_Open out... \n");

    return s32RetVal;
}

HI_S32 HI_DRV_HDMI_Start(void)
{
    HI_S32 s32RetVal = HI_SUCCESS;

    HDMIPrint(">>> HI_DRV_HDMI_Start in... \n");

    s32RetVal = HI_UNF_HDMI_Start(HI_UNF_HDMI_ID_0);

    HDMIPrint("<<< HI_DRV_HDMI_Start out... \n");

    return s32RetVal ;
}

HI_S32 HI_DRV_HDMI_DeInit(void)
{
    HI_S32 s32RetVal = HI_SUCCESS;

    HDMIPrint(">>> HI_DRV_HDMI_DeInit in... \n");

    s32RetVal = HI_UNF_HDMI_DeInit();

    HDMIPrint("<<< HI_DRV_HDMI_DeInit out... \n");

    return s32RetVal ;
}

HI_S32 HI_DRV_HDMI_Close(void)
{
    HI_S32 s32RetVal = HI_SUCCESS;

    HDMIPrint(">>> HI_DRV_HDMI_Close in... \n");

    s32RetVal = HI_UNF_HDMI_Close(HI_UNF_HDMI_ID_0);

    HDMIPrint("<<< HI_DRV_HDMI_Close out... \n");

    return s32RetVal ;
 }

#ifdef HI_HDMI_EDID_SUPPORT
HI_S32 HI_DRV_HDMI_GetSinkCapability(HI_UNF_HDMI_ID_E enHdmi,HI_UNF_EDID_BASE_INFO_S *pSinkCap)
{
    HI_S32 s32RetVal = HI_SUCCESS;

    HDMIPrint(">>> HI_DRV_HDMI_GetSinkCapability in... \n");

    s32RetVal = HI_UNF_HDMI_GetSinkCapability(enHdmi, pSinkCap);

    HDMIPrint("<<< HI_DRV_HDMI_GetSinkCapability out... \n");

    return s32RetVal ;
 }
#endif

