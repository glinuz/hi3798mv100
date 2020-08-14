
#include "vsdb.h"
#include "infofrm.h"



HI_BOOL Is4KFmt(HI_UNF_ENC_FMT_E enHdFmt)
{
    if((enHdFmt == HI_UNF_ENC_FMT_3840X2160_24)
     || (enHdFmt == HI_UNF_ENC_FMT_3840X2160_25)
     || (enHdFmt == HI_UNF_ENC_FMT_3840X2160_30)
     || (enHdFmt == HI_UNF_ENC_FMT_4096X2160_24))
    {
        return HI_TRUE;
    }

    return HI_FALSE;
}

HI_U8  Setting4K(HI_U32 u324KFormat)
{
    switch(u324KFormat)
    {
        case HI_UNF_ENC_FMT_3840X2160_24:
        case HI_UNF_ENC_FMT_3840X2160_25:
        case HI_UNF_ENC_FMT_3840X2160_30:
        case HI_UNF_ENC_FMT_4096X2160_24:
            // 4k setting
            Config4KInfoFrame(u324KFormat);
            break;
        default:
            HDMIPrint("Unknown 4K Mode \n");
            return HI_SUCCESS;
    }

    EnableInfoFrame(VENDORSPEC_TYPE);

    return HI_SUCCESS;
}

void Config4KInfoFrame(HI_U32 u324KFormat)
{
    HI_U8 VendorBody[20], offset = 0;
    HI_U8 U8HDMI_Video_Format = 0, u8HDMI_VIC = 0;

    HI_U32 i = 0;

    HDMIPrint("4K Infoframe Mode,fmt:0x%03x \n",u324KFormat);
    offset = 0;
    //IEEE
    VendorBody[offset++] = 0x03;
    VendorBody[offset++] = 0x0c;
    VendorBody[offset++] = 0x00;

    U8HDMI_Video_Format = 0x01;//Extended resolution format (e.g. used for 4K x 2K video)

    if(u324KFormat == HI_UNF_ENC_FMT_3840X2160_30)
    {
        u8HDMI_VIC = 0X01;   //2160P30
    }
    else if(u324KFormat == HI_UNF_ENC_FMT_3840X2160_25)
    {
        u8HDMI_VIC = 0X02;   //2160P25
    }
    else if(u324KFormat == HI_UNF_ENC_FMT_3840X2160_24)
    {
        u8HDMI_VIC = 0X03;   //2160P24
    }
    else if(u324KFormat == HI_UNF_ENC_FMT_4096X2160_24)
    {
        u8HDMI_VIC = 0X04;   //4096*2160@24
    }

    VendorBody[offset++] = (U8HDMI_Video_Format & 0x3) << 5;
    VendorBody[offset++] = u8HDMI_VIC;

    HDMIPrint("offset: %d \n",offset);

    for(i = 0;i < offset;i++)
    {
        HDMIPrint("VendorBody[%d]:0x%02x \n",i,VendorBody[i]);
    }

    Send4KInfoFrame(VendorBody,offset);
}


