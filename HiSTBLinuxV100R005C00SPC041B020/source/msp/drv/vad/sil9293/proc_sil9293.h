/******************************************************************************

  Copyright (C), 2001-2011, Hisilicon Tech. Co., Ltd.

******************************************************************************
  File Name     : proc_sil9293_str.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2013/04/07
  Description   :
  History       :
  1.Date        : 2013/04/07
    Author      : j00169368
    Modification: Created file

******************************************************************************/

#ifndef __PROC_SIL9293_STR_H__
#define __PROC_SIL9293_STR_H__

inline HI_U8 * EnIpStateToStr(HDMIRX_IP_STATE_E enIpState)
{
    switch (enIpState)
    {
    case HDMIRX_IP_STATE_WAITCONNET:
        return "NoClk";
    case HDMIRX_IP_STATE_NOSTABLE:
        return "NoStab";
    case HDMIRX_IP_STATE_STABLE:
        return "Stable";
    default:
        return "err";
    }
}

inline HI_U8 * EnHdcpStateToStr(HI_HDCP_STATE_E enHdcpState)
{
    switch (enHdcpState)
    {
    case HI_HDCP_STATE_FAILED:
        return "Failed";
    case HI_HDCP_STATE_TRYING:
        return "Trying";
    case HI_HDCP_STATE_DECRYPT:
        return "Decrypt";
    case HI_HDCP_STATE_SUCCESS:
        return "OK";
    default:
        return "err";
    }
}

inline HI_U8 * EnSigStateToStr(HI_SIG_STATUS_E enSigStatus)
{
    switch (enSigStatus)
    {
    case HI_SIG_SUPPORT:
        return "OK";
    case HI_SIG_NO_SIGNAL:
        return "NoSig";
    case HI_SIG_NOT_SUPPORT:
        return "NoSpt";
    case HI_SIG_UNSTABLE:
        return "UnStab";
    default:
        return "err";
    }
}

inline HI_U8 * BooleanToStr(HI_BOOL bValue)
{
    switch (bValue)
    {
    case HI_TRUE:
        return "Yes";
    case HI_FALSE:
        return "NO";
    default:
        return "err";
    }
}

inline HI_U8 * En3dFmtToStr(HI_VIDEO_3D_FORMAT_E en3dFmt)
{
    switch (en3dFmt)
    {
    case HI_VIDEO_3D_FORMAT_2D:
        return "2D";
    case HI_VIDEO_3D_FORMAT_SBS:
        return "3D_SBS";
    case HI_VIDEO_3D_FORMAT_TB:
        return "3D_TB";
    case HI_VIDEO_3D_FORMAT_FP:
        return "3D_FP";
    default:
        return "err";
    }
}

inline HI_U8 * EnClrSpcToStr(HI_COLOR_SPACE_E enColorSpace)
{
    switch (enColorSpace)
    {
    case HI_COLOR_SPACE_YCBCR_601:
        return "Y601";
    case HI_COLOR_SPACE_YCBCR_709:
        return "Y709";
    case HI_COLOR_SPACE_RGB:
        return "RGB";
    default:
        return "err";
    }
}

inline HI_U8 * EnPixFmtToStr(HI_PIXEL_FORMAT_E enPixelFmt)
{
    switch (enPixelFmt)
    {
    case HI_PIXEL_FORMAT_RGB_444:
        return "R444";
    case HI_PIXEL_FORMAT_YUV_420:
        return "Y420";
    case HI_PIXEL_FORMAT_YUV_422:
        return "Y422";
    case HI_PIXEL_FORMAT_YUV_444:
        return "Y444";
    default:
        return "err";
    }
}

inline HI_U8 * EnPixBWToStr(HI_PIXEL_BITWIDTH_E enBitWidth)
{
    switch (enBitWidth)
    {
    case HI_PIXEL_BITWIDTH_8BIT:
        return "8Bit";
    case HI_PIXEL_BITWIDTH_10BIT:
        return "10Bit";
    case HI_PIXEL_BITWIDTH_12BIT:
        return "12Bit";
    default:
        return "err";
    }
}

inline HI_U8 * EnOverSmpMdToStr(HI_OVERSAMPLE_MODE_E enOverSampleMode)
{
    switch (enOverSampleMode)
    {
    case HI_OVERSAMPLE_1X:
        return "1X";
    case HI_OVERSAMPLE_2X:
        return "2X";
    case HI_OVERSAMPLE_4X:
        return "4X";
    default:
        return "err";
    }
}

#endif // __PROC_SIL9293_STR_H__
