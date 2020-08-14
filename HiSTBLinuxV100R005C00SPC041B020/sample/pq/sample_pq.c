/******************************************************************************
*
* Copyright (C) 2014 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
*  and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
*****************************************************************************

  File Name     : sample_pq.c
  Version       : Initial Draft
  Author        : p00203646
  Created       : 2014/01/16
  Description   : pq sample

******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "hi_unf_pq.h"

#ifndef CONFIG_SUPPORT_CA_RELEASE
#define Printf  printf
#else
#define Printf(x...)
#endif

#define RANGECHECK(x, min, max) \
    do { \
        if ((x<min)||(max<x)) \
        { \
            Printf ("%d not in [%d, %d]\n", x, min, max); \
            HelpMsg();\
            return HI_FAILURE; \
        } \
    }while (0)

static HI_S32 gSetType ;   /* 0:get;  1:set */
static HI_S32 gChan ;      /* 0:chan 0;  1: chan 1*/
static HI_S32 gBright;
static HI_S32 gImageMode;
static HI_S32 gDefaultMode;
static HI_S32 gContrast;
static HI_S32 gHue;
static HI_S32 gSaturation;
static HI_S32 gColorTempe;
static HI_S32 gDynContrast;
static HI_S32 g6BaseColor[6];
static HI_S32 gFleshTone;
static HI_S32 gColorMode;
static HI_S32 gSharpness;
static HI_S32 gDenoise;
static HI_S32 gFilmMode;
static HI_S32 gSaveFlash;
static HI_S32 gSetDefaultParam;
static HI_S32 gSetDemo;
static HI_S32 gDemoMode;
static HI_S32 gSetDemoDispMode;
static HI_S32 gDemoDispMode;
static HI_S32 gDemoOnOff;
static HI_S32 gSetDemoCoordinate;
static HI_S32 gSetPQModule;
static HI_S32 gPQModule;
static HI_S32 gModuleOnOff;
static HI_S32 gColorGain;
static HI_S32 gTnrLevel;
static HI_S32 gDnrLevel;
static HI_S32 gVideoBaseParam[4];
static HI_S32 gGraphBaseParam[4];



const char* gpGetOption = NULL;

HI_UNF_PQ_OPT_CHANS_S  pstChanOption_obj ;
HI_UNF_PQ_OPT_CHANS_S* pstChanOption = &pstChanOption_obj;

HI_UNF_PQ_IMAGE_MODE_E penImageMode_obj ;
HI_UNF_PQ_IMAGE_MODE_E* penImageMode = &penImageMode_obj;

HI_UNF_PQ_OPT_COMMON_S pstCommOption_obj ;
HI_UNF_PQ_OPT_COMMON_S* pstCommOption = &pstCommOption_obj;


static void HelpMsg(void)
{
    Printf("\n");
    Printf("PQ sample options:\n");
    Printf("   -h,  --help                          Show this help message\n");
    Printf("   -s,  --set             <option>      Set the PQ options\n");
    Printf("   -g,  --get             <option>      Get the PQ options\n");
    Printf("   -ch, --chan            <0/1>         Set the chan(0:SD/1:HD)\n");
    Printf("   -bn, --brightness      <0~100>       Change the brightness\n");
    Printf("   -co, --contrast        <0~100>       Change the contrast\n");
    Printf("   -hu, --hue             <0~100>       Change the hue\n");
    Printf("   -sa, --saturation      <0~100>       Change the saturation (0~100)\n");
    Printf("   -dc, --dyncontrast     <0~100>       Change the dyncontrast (0~100)\n");
    Printf("   -6b, --6baseenhance    <0~100>       Change the 6basecolorenhance (Red,Green,Blue,Cyan,Magenta,Yellow)\n");
    Printf("   -vo, --video           <0~100>       Change the video base param (brightness,contrast,hue,saturation)\n");
    Printf("   -gp, --graph           <0~100>       Change the graph base param (brightness,contrast,hue,saturation)\n");
    Printf("   -ft, --fleshtone       <0/1/2/3>     Change the flesh tone (0:off/1:low/2:mid/3:high)\n");
    Printf("   -cm, --colormode       <0/1/2/3>     Change the color mode (0:recommend/1:blue/2:green/3:BG)\n");
    Printf("   -sh, --sharpness       <0~100>       Change the sharpness\n");
    Printf("   -cg, --colorgain       <0~100>       Change the color gain\n");
    Printf("   -nr, --tnr             <0~100>       Change the tnr level\n");
    Printf("   -dnr,--dnr             <0~100>       Change the dnr level\n");
    Printf("   -dm, --demo     <type> <OnOff>       Change the demo type(0:sharpness/1:dci/2:color/3:sr/4:tnr/5:dei/6:dbm/7:snr/8:all),OnOff(0:off/1:on)\n");
    Printf("   -ddm, --demomode       <mode>        Change the demo mode(0:fixed right/1:fixed left/2:scroll right/3:scroll left)\n");
    Printf("   -dmc, --dmcoor         <0~100>       Change the demo coordinate\n");
    Printf("   -pe, --pqenable <type> <OnOff>       Enable the pq module(0:sharpness/1:dci/2:color/3:sr/4:tnr/5:dbm/6:snr/7:mcdi/8:all),OnOff(0:off/1:on)\n");
    Printf("   -dp, --defaultparam                  Set the default PQ configuration for video parameter test\n");
    Printf("   -a,  --all                           Show all PQ options in current image mode\n");
    Printf("\n");


    //Printf("   -df, --default       <0/1/2/3>     Init image mode (0:natural/1:person/2:film/3:UD)\n");
    //Printf("   -im, --imagemode     <0/1/2/3>     Change the image mode (0:natural/1:person/2:film/3:UD)\n");
    //Printf("   -ct, --colortempe    <0/1/2>       Change the colortempe (0:cold/1:middle/2:warm)\n");
    //Printf("   -dn, --denoise       <0/1>         Change the denoise (0:disable/1:auto)\n");
    //Printf("   -fm, --filmmode      <0/1>         Change the filmmode (0:disable/1:auto)\n");
    //Printf("   -sf, --saveflash                   Save all PQ options to flash\n");
    //Printf("   -dp, --defaultparam                Set the default PQ configuration for video parameter test\n");

}

static void getChanOption(HI_UNF_DISP_E enChan, HI_UNF_PQ_OPT_CHANS_S* pstChansOption)
{
    if ((enChan > HI_UNF_DISPLAY1) || (NULL == pstChansOption))
    {   return;  }

    pstChansOption->enChan = enChan;
    HI_UNF_PQ_GetChanOption(pstChansOption);
    return;
}

static void getCommOption(HI_UNF_PQ_OPT_COMMON_S* pstCommOpt)
{
    if (NULL == pstCommOpt)
    {
        return;
    }
    HI_UNF_PQ_GetCommOption(pstCommOpt);
}

HI_S32 setImageMode(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_IMAGE_MODE_E mode = (HI_UNF_PQ_IMAGE_MODE_E)val ;
    /*HD  high definition */
    s32Ret = HI_UNF_PQ_SetImageMode(enChan, mode);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetImageMode fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d Set Image Mode:%d (0:natural/1:person/2:film/3:UD)\n", enChan, mode);
    return s32Ret ;
}

HI_S32 setDefault(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;

    HI_UNF_PQ_IMAGE_MODE_E mode = (HI_UNF_PQ_IMAGE_MODE_E)val ;
    s32Ret = HI_UNF_PQ_InitImageMode(enChan, mode);
    if (HI_FAILURE == s32Ret)
    {
        Printf("InitImageMode fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d Init Image Mode:%d (0:natural/1:person/2:film/3:UD)\n", enChan, mode);
    return s32Ret ;
}

HI_S32 setDefaultParam(void)
{
    HI_S32 s32Ret = HI_UNF_PQ_SetDefaultParam();
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetDefaultParam fail\n");
        return HI_FAILURE ;
    }

    Printf("Set PQ default param success\n");
    return s32Ret ;
}

HI_S32 setBright(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetBrightness(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setBright fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set Brightness:%d\n", enChan, val);
    return s32Ret ;
}

HI_S32 setContrast(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetContrast(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("set Contrast fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set Contrast:%d\n", enChan, val);
    return s32Ret ;
}

HI_S32 setHue(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetHue(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setHue fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set Hue:%d\n", enChan, val);
    return s32Ret ;

}

HI_S32 setSaturation(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetSaturation(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setSaturation fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set Saturation:%d\n", enChan, val);
    return s32Ret ;

}

HI_S32 setColorTemperature(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    getChanOption(enChan, pstChanOption);
    pstChanOption->enChan = enChan;
    pstChanOption->stChanOpt.u32Colortemperature = val;
    s32Ret = HI_UNF_PQ_SetChanOption(pstChanOption);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setColorTemperature fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set ColorTemperature:%d (0:cold/1:middle/2:warm)\n", enChan, val);
    return s32Ret ;

}

HI_S32 setDynamicContrast(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;

    s32Ret = HI_UNF_PQ_SetDynamicContrast(val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setDynamicContrast fail\n");
        return HI_FAILURE ;
    }

    Printf("Chan %d set Dynamic Contrast:%d \n", enChan, val);
    return s32Ret ;

}


HI_S32 set6BaseColorEnhance(HI_UNF_DISP_E enChan,  HI_S32 val[])
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType = HI_UNF_PQ_COLOR_ENHANCE_SIX_BASE;
    stColorEnhanceParam.unColorGain.stSixBase.u32Red     = val[0];
    stColorEnhanceParam.unColorGain.stSixBase.u32Green   = val[1];
    stColorEnhanceParam.unColorGain.stSixBase.u32Blue    = val[2];
    stColorEnhanceParam.unColorGain.stSixBase.u32Cyan    = val[3];
    stColorEnhanceParam.unColorGain.stSixBase.u32Magenta = val[4];
    stColorEnhanceParam.unColorGain.stSixBase.u32Yellow  = val[5];

    s32Ret = HI_UNF_PQ_SetColorEnhanceParam(stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("set 6BaseColorEnhance fail!\n");
        return HI_FAILURE ;
    }

    Printf("set 6BaseColorEnhance:\nRed:%d \nGreen:%d \nBlue:%d \nCyan:%d \nMagenta:%d \nYellow:%d\n", \
           val[0], val[1], val[2], val[3], val[4], val[5]);
    return s32Ret ;

}


HI_S32 setBaseParam(HI_UNF_PQ_IMAGE_TYPE_E enType, HI_UNF_DISP_E enChan,  HI_S32 val[])
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_IMAGE_PARAM_S stParam;

    stParam.u32Brightness = val[0];
    stParam.u32Contrast   = val[1];
    stParam.u32Hue        = val[2];
    stParam.u32Saturation = val[3];

    s32Ret = HI_UNF_PQ_SetBasicImageParam(enType, enChan, stParam );
    if (HI_FAILURE == s32Ret)
    {
        Printf("set BasicImageParam fail!\n");
        return HI_FAILURE ;
    }

    Printf("set set BasicImageParam:\nBrightness:%d \nContrast:%d \nHue:%d \nSaturation:%d \n", \
           val[0], val[1], val[2], val[3]);
    return s32Ret ;

}

HI_S32 setFleshTone(HI_UNF_DISP_E enChan,  HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType       = HI_UNF_PQ_COLOR_ENHANCE_FLESHTONE;
    stColorEnhanceParam.unColorGain.enFleshtone  = (HI_UNF_PQ_FLESHTONE_E)val;


    s32Ret = HI_UNF_PQ_SetColorEnhanceParam(stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("set FleshTone fail!\n");
        return HI_FAILURE ;
    }

    Printf("set FleshTone:%d (0:off/1:low/2:mid/3:high)\n", val);
    return s32Ret ;

}

HI_S32 setColorEnhanceMode(HI_UNF_DISP_E enChan,  HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType       = HI_UNF_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE;
    stColorEnhanceParam.unColorGain.enColorMode  = (HI_UNF_PQ_COLOR_SPEC_MODE_E)val;


    s32Ret = HI_UNF_PQ_SetColorEnhanceParam(stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("set ColorMode fail!\n");
        return HI_FAILURE ;
    }

    Printf("set ColorMode:%d (0:recommend/1:blue/2:green/3:BG)\n", val);
    return s32Ret ;

}


HI_S32 setSharpness( HI_U32 val)
{
    HI_S32 s32Ret;

    s32Ret = HI_UNF_PQ_SetSharpness(HI_UNF_DISPLAY1, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setSharpness fail\n");
        return HI_FAILURE ;
    }

    Printf("set Sharpness:%d\n", val);
    return s32Ret ;

}


HI_S32 SetColorGain( HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetColorGain(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetColorGain fail\n");
        return HI_FAILURE ;
    }

    Printf("Set ColorGain:%d\n", val);
    return s32Ret ;

}

HI_S32 SetTnrLevel( HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetNR(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetTNR fail\n");
        return HI_FAILURE ;
    }

    Printf("Set TNR Level:%d\n", val);
    return s32Ret ;

}

HI_S32 SetDnrLevel( HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_SetDNR(enChan, val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetDNR fail\n");
        return HI_FAILURE ;
    }

    Printf("Set DNR Level:%d\n", val);
    return s32Ret ;
}

HI_S32 setDenoise(HI_U32 val)
{
    HI_S32 s32Ret;
    getCommOption(pstCommOption);
    pstCommOption->u32Denoise = val;//0:Closed 1:open
    s32Ret = HI_UNF_PQ_SetCommOption(pstCommOption);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setDenoise fail\n");
        return HI_FAILURE ;
    }

    Printf("set Denoise:%d (0:disable/1:auto)\n", val);
    return s32Ret ;

}

HI_S32 setFilmMode(HI_U32 val)
{
    HI_S32 s32Ret;
    getCommOption(pstCommOption);
    pstCommOption->u32FilmMode = val;//0:Closed 1:open
    s32Ret = HI_UNF_PQ_SetCommOption(pstCommOption);
    if (HI_FAILURE == s32Ret)
    {
        Printf("setFilmMode fail\n");
        return HI_FAILURE ;
    }

    Printf("set Film Mode:%d (0:disable/1:auto)\n", val);
    return s32Ret ;

}

HI_S32 setDemo(HI_U32 val, HI_U32 u32OnOff)
{
    HI_S32 s32Ret;
    HI_UNF_PQ_DEMO_E enFlags;
    enFlags = (HI_UNF_PQ_DEMO_E)val;

    s32Ret = HI_UNF_PQ_SetDemo( enFlags, u32OnOff);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetDemo fail\n");
        return HI_FAILURE ;
    }

    Printf("Set Demo %d :%s (0:sharpness/1:dci/2:color/3:sr/4:tnr/5:dei/6:dbm/7:snr/8:all)\n", val, u32OnOff ? "on" : "off");
    return s32Ret ;

}

HI_S32 setDemoDispMode(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;
    HI_UNF_PQ_DEMO_MODE_E enMode;
    enMode = (HI_UNF_PQ_DEMO_MODE_E)val;

    s32Ret = HI_UNF_PQ_SetDemoMode(enChan, enMode);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetDemoMode fail\n");
        return HI_FAILURE ;
    }

    Printf("Set Demo Disp Mode:%d (0:fixed right/1:fixed left/2:scroll right/3:scroll left)\n", val);
    return s32Ret ;

}

HI_S32 setDemoCoordinate(HI_UNF_DISP_E enChan, HI_U32 val)
{
    HI_S32 s32Ret;

    s32Ret = HI_UNF_PQ_SetDemoCoordinate(val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetDemoCoordinate fail\n");
        return HI_FAILURE ;
    }

    Printf("Set Demo Coordinate: %d \n", val);
    return s32Ret ;
}

HI_S32 setPQModule(HI_U32 val, HI_U32 u32OnOff)
{
    HI_S32 s32Ret;
    HI_UNF_PQ_MODULE_E enFlags;
    enFlags = (HI_UNF_PQ_MODULE_E)val;

    if (HI_UNF_PQ_MODULE_BUTT <= enFlags)
    {
        Printf("PQModule error\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_PQ_SetPQModule( enFlags, u32OnOff);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetPQModule fail\n");
        return HI_FAILURE ;
    }

    Printf("Set PQ Module %d :%s(0:sharpness/1:dci/2:color/3:sr/4:tnr/5:dbm/6:snr/7:mcdi/8:all)\n", val, u32OnOff ? "on" : "off");
    return s32Ret ;

}

HI_S32 getPQModule(HI_U32 val)
{
    HI_S32 s32Ret;
    HI_UNF_PQ_MODULE_E enFlags;
    HI_U32 u32OnOff;
    enFlags = (HI_UNF_PQ_MODULE_E)val;

    if (HI_UNF_PQ_MODULE_BUTT <= enFlags)
    {
        Printf("PQModule error\n");
        return HI_FAILURE;
    }

    s32Ret = HI_UNF_PQ_GetPQModule( enFlags, &u32OnOff);
    if (HI_FAILURE == s32Ret)
    {
        Printf("SetPQModule fail\n");
        return HI_FAILURE ;
    }

    return u32OnOff ;
}


/* --------------- get --------------------- */
static HI_S32 getImageMode(HI_UNF_DISP_E enChan)
{
    HI_UNF_PQ_IMAGE_MODE_E   enImageMode;

    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetImageMode(enChan, &enImageMode);

    return (HI_U32)enImageMode;
}


static HI_S32 getBright(HI_UNF_DISP_E enChan)
{
    HI_U32 u32Brightness = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetBrightness(enChan, &u32Brightness);
    return u32Brightness;
}

static HI_S32 getContrast(HI_UNF_DISP_E enChan)
{
    HI_U32 u32Contrast = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetContrast(enChan, &u32Contrast);
    return u32Contrast;
}

static HI_S32 getHue(HI_UNF_DISP_E enChan)
{
    HI_U32 u32Hue = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetHue(enChan, &u32Hue);
    return u32Hue;
}

static HI_S32 getSaturation(HI_UNF_DISP_E enChan)
{
    HI_U32 u32Saturation = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetSaturation(enChan, &u32Saturation);
    return u32Saturation;
}

static HI_S32  getColorTemperature(HI_UNF_DISP_E enChan)
{
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    getChanOption(enChan, pstChanOption);
    return pstChanOption->stChanOpt.u32Colortemperature;
}

static HI_S32  getDynamicContrast(HI_UNF_DISP_E enChan)
{
    HI_U32 u32DCILevel = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_GetDynamicContrast(&u32DCILevel);
    return u32DCILevel;
}


static HI_S32  get6BaseColorEnhance(HI_UNF_DISP_E enChan, HI_S32 val[])
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    if (HI_NULL == val)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType = HI_UNF_PQ_COLOR_ENHANCE_SIX_BASE;

    s32Ret = HI_UNF_PQ_GetColorEnhanceParam(&stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("get 6BaseColorEnhance fail!\n");
        return HI_FAILURE ;
    }

    val[0] = stColorEnhanceParam.unColorGain.stSixBase.u32Red;
    val[1] = stColorEnhanceParam.unColorGain.stSixBase.u32Green;
    val[2] = stColorEnhanceParam.unColorGain.stSixBase.u32Blue;
    val[3] = stColorEnhanceParam.unColorGain.stSixBase.u32Cyan;
    val[4] = stColorEnhanceParam.unColorGain.stSixBase.u32Magenta;
    val[5] = stColorEnhanceParam.unColorGain.stSixBase.u32Yellow;

    Printf("get 6BaseColorEnhance:\nRed:%d \nGreen:%d \nBlue:%d \nCyan:%d \nMagenta:%d \nYellow:%d\n", \
           val[0], val[1], val[2], val[3], val[4], val[5]);

    return s32Ret ;
}

static HI_S32 getfleshtone(HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType = HI_UNF_PQ_COLOR_ENHANCE_FLESHTONE;

    s32Ret = HI_UNF_PQ_GetColorEnhanceParam(&stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("get fleshtone fail!\n");
        return HI_FAILURE ;
    }

    return stColorEnhanceParam.unColorGain.enFleshtone;
}

static HI_S32 getColorEnhanceMode(HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    HI_UNF_PQ_COLOR_ENHANCE_S stColorEnhanceParam;
    stColorEnhanceParam.enColorEnhanceType = HI_UNF_PQ_COLOR_ENHANCE_SPEC_COLOR_MODE;

    s32Ret = HI_UNF_PQ_GetColorEnhanceParam(&stColorEnhanceParam);
    if (HI_FAILURE == s32Ret)
    {
        Printf("get colormode fail!\n");
        return HI_FAILURE ;
    }

    return stColorEnhanceParam.unColorGain.enColorMode;
}


static HI_S32 getSharpness( void)
{
    HI_U32 u32Sharpeness = 0;

    HI_UNF_PQ_GetSharpness(HI_UNF_DISPLAY1, &u32Sharpeness);
    return u32Sharpeness;
}

static HI_S32 GetColorGain( HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    HI_U32 val = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_GetColorGain(enChan, &val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("GetColorGain fail\n");
        return HI_FAILURE ;
    }

    return val ;
}

static HI_S32 GetTnrLevel( HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    HI_U32 val = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_GetNR(enChan, &val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("GetTNR fail\n");
        return HI_FAILURE ;
    }

    return val ;
}

static HI_S32 GetDnrLevel( HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    HI_U32 val = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_GetDNR(enChan, &val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("GetDNR fail\n");
        return HI_FAILURE ;
    }

    return val ;
}

static HI_S32 GetDemoMode( HI_UNF_DISP_E enChan)
{
    HI_S32 s32Ret;
    HI_U32 val = 0;
    if (enChan > HI_UNF_DISPLAY1)
    { return HI_FAILURE; }

    s32Ret = HI_UNF_PQ_GetDemoMode(enChan, &val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("GetDemoMode fail\n");
        return HI_FAILURE ;
    }

    return val ;
}

static HI_S32 GetDemoCoordinate(HI_VOID)
{
    HI_S32 s32Ret;
    HI_U32 val = 0;

    s32Ret = HI_UNF_PQ_GetDemoCoordinate(&val);
    if (HI_FAILURE == s32Ret)
    {
        Printf("GetDemoCoordinate fail\n");
        return HI_FAILURE ;
    }

    return val ;
}

static HI_S32 getDenoise(void)
{
    getCommOption(pstCommOption);
    return pstCommOption->u32Denoise ;
}

static HI_S32 getFilmMode(void)
{
    getCommOption(pstCommOption);
    return pstCommOption->u32FilmMode ;
}

/* --------------- save --------------------- */

HI_S32 setSaveAll(void)
{
    HI_S32 s32Ret;
    s32Ret = HI_UNF_PQ_UpdatePqParam();
    if (HI_SUCCESS != s32Ret)
    {
        Printf("Save flash error\n");
        return HI_FAILURE;
    }

    Printf("Save flash success\n");
    return s32Ret;
}


static HI_S32 ParseCommandLine( int argc, char* argv[] )
{
    HI_S32 n;

    gSetType = 0;  /* 0:get;  1:set ; default: get*/
    gChan = 1;      /* 0:chan 0;  1: chan 1; default: chan 1*/
    gImageMode = -1;
    gDefaultMode = -1;
    gBright = -1;
    gContrast = -1;
    gHue = -1;
    gSaturation = -1;
    gColorTempe = -1;
    gDynContrast = -1;
    g6BaseColor[5] = -1;
    gVideoBaseParam[3] = -1;
    gGraphBaseParam[3] = -1;
    gFleshTone = -1;
    gColorMode = -1;
    gSharpness = -1;
    gDenoise = -1;
    gFilmMode = -1;
    gSaveFlash = -1;
    gSetDefaultParam = -1;
    gSetDemo = -1;
    gDemoMode = 0;
    gSetDemoDispMode = -1;
    gDemoDispMode = 0;
    gSetDemoCoordinate = -1;
    gDemoOnOff = 1;
    gSetPQModule = -1;
    gPQModule = 0;
    gModuleOnOff = 1;
    gColorGain = -1;
    gTnrLevel  = -1;
    gDnrLevel  = -1;


    for (n = 1; n < argc; n++)
    {
        const char* arg = argv[n];

        if (strncmp (arg, "-s", strlen(arg)) == 0 || strncmp (arg, "--set", strlen(arg)) == 0)
        {
            gSetType = 1;
            continue;
        }

        if (strncmp (arg, "-g", strlen(arg)) == 0 || strncmp (arg, "--get", strlen(arg)) == 0)
        {
            gSetType = 0;
            if (++n == argc)
            { return HI_FAILURE; }

            gpGetOption = argv[n];

            continue;
        }

        if (strncmp (arg, "-h", strlen(arg)) == 0 || strncmp (arg, "--help", strlen(arg)) == 0)
        {
            HelpMsg();
            HI_UNF_PQ_DeInit();
            exit(0);
        }

        if (strncmp (arg, "-ch", strlen(arg)) == 0 || strncmp (arg, "--chan", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gChan = atoi(argv[n]);
            RANGECHECK(gChan, 0, 1);
            continue;
        }

        if (strncmp (arg, "-df", strlen(arg)) == 0 || strncmp (arg, "--default", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gDefaultMode = atoi(argv[n]);
            RANGECHECK(gDefaultMode, 0, 3);
            continue;
        }

        if (strncmp (arg, "-im", strlen(arg)) == 0 || strncmp (arg, "--imagemode", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gImageMode = atoi(argv[n]);
            RANGECHECK(gImageMode, 0, 3);
            continue;
        }

        if (strncmp (arg, "-bn", strlen(arg)) == 0 || strncmp (arg, "--brightness", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gBright = atoi(argv[n]);
            RANGECHECK(gBright, 0, 100);
            continue;
        }

        if (strncmp (arg, "-co", strlen(arg)) == 0 || strncmp (arg, "--contrast", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gContrast = atoi(argv[n]);
            RANGECHECK(gContrast, 0, 100);
            continue;
        }

        if (strncmp (arg, "-hu", strlen(arg)) == 0 || strncmp (arg, "--hue", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gHue = atoi(argv[n]);
            RANGECHECK(gHue, 0, 100);
            continue;
        }

        if (strncmp (arg, "-sa", strlen(arg)) == 0 || strncmp (arg, "--saturation", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gSaturation = atoi(argv[n]);
            RANGECHECK(gSaturation, 0, 100);
            continue;
        }

        if (strncmp (arg, "-ct", strlen(arg)) == 0 || strncmp (arg, "--colortempe", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gColorTempe = atoi(argv[n]);
            RANGECHECK(gColorTempe, 0, 2);
            continue;
        }

        if (strncmp (arg, "-dc", strlen(arg)) == 0 || strncmp (arg, "--dyncontrast", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gDynContrast = atoi(argv[n]);
            RANGECHECK(gDynContrast, 0, 100);
            continue;
        }

        if (strncmp (arg, "-6b", strlen(arg)) == 0 || strncmp (arg, "--6baseenhance", strlen(arg)) == 0)
        {
            HI_S32 i;

            if (0 == gSetType)
            { continue; }

            for (i = 0; i < 6; i++)
            {
                if (++n == argc)
                { return HI_FAILURE; }
                g6BaseColor[i] = atoi(argv[n]);
                RANGECHECK(g6BaseColor[i], 0, 100);
            }

            continue;
        }

        if (strncmp (arg, "-vo", strlen(arg)) == 0 || strncmp (arg, "--video", strlen(arg)) == 0)
        {
            HI_S32 i;

            if (0 == gSetType)
            { continue; }

            for (i = 0; i < 4; i++)
            {
                if (++n == argc)
                { return HI_FAILURE; }
                gVideoBaseParam[i] = atoi(argv[n]);
                RANGECHECK(gVideoBaseParam[i], 0, 100);
            }

            continue;
        }

        if (strncmp (arg, "-gp", strlen(arg)) == 0 || strncmp (arg, "--graph", strlen(arg)) == 0)
        {
            HI_S32 i;

            if (0 == gSetType)
            { continue; }

            for (i = 0; i < 4; i++)
            {
                if (++n == argc)
                { return HI_FAILURE; }
                gGraphBaseParam[i] = atoi(argv[n]);
                RANGECHECK(gGraphBaseParam[i], 0, 100);
            }

            continue;
        }

        if (strncmp (arg, "-ft", strlen(arg)) == 0 || strncmp (arg, "--fleshtone", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gFleshTone = atoi(argv[n]);
            RANGECHECK(gFleshTone, 0, 3);
            continue;
        }

        if (strncmp (arg, "-cm", strlen(arg)) == 0 || strncmp (arg, "--colormode", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gColorMode = atoi(argv[n]);
            RANGECHECK(gColorMode, 0, 3);
            continue;
        }

        if (strncmp (arg, "-sh", strlen(arg)) == 0 || strncmp (arg, "--sharpness", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gSharpness = atoi(argv[n]);
            RANGECHECK(gSharpness, 0, 100);
            continue;
        }

        if (strncmp (arg, "-dn", strlen(arg)) == 0 || strncmp (arg, "--denoise", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gDenoise = atoi(argv[n]);
            RANGECHECK(gDenoise, 0, 1);
            continue;
        }
        if (strncmp (arg, "-fm", strlen(arg)) == 0 || strncmp (arg, "--filmmode", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gFilmMode = atoi(argv[n]);
            RANGECHECK(gFilmMode, 0, 1);
            continue;
        }

        if (strncmp (arg, "-sf", strlen(arg)) == 0 || strncmp (arg, "--saveflash", strlen(arg)) == 0)
        {
            gSaveFlash = 1;
            continue;
        }

        if (strncmp (arg, "-dp", strlen(arg)) == 0 || strncmp (arg, "--defaultparam", strlen(arg)) == 0)
        {
            gSetDefaultParam = 1;
            continue;
        }

        if (strncmp (arg, "-dm", strlen(arg)) == 0 || strncmp (arg, "--demo", strlen(arg)) == 0)
        {
            gSetDemo = 1;

            if (++n == argc)
            { return HI_FAILURE; }
            gDemoMode = atoi(argv[n]);
            RANGECHECK(gDemoMode, 0, 8);

            if (++n == argc)
            { return HI_FAILURE; }
            gDemoOnOff = atoi(argv[n]);
            RANGECHECK(gDemoOnOff, 0, 1);

            continue;
        }

        if (strncmp (arg, "-ddm", strlen(arg)) == 0 || strncmp (arg, "--demomode", strlen(arg)) == 0)
        {
            gSetDemoDispMode = 1;

            if (++n == argc)
            { return HI_FAILURE; }
            gDemoDispMode = atoi(argv[n]);
            RANGECHECK(gDemoDispMode, 0, 3);

            continue;
        }
        if (strncmp (arg, "-dmc", strlen(arg)) == 0 || strncmp (arg, "--dmcoor", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gSetDemoCoordinate = atoi(argv[n]);
            RANGECHECK(gSetDemoCoordinate, 0, 100);

            continue;
        }

        if (strncmp (arg, "-pe", strlen(arg)) == 0 || strncmp (arg, "--pqenable", strlen(arg)) == 0)
        {
            gSetPQModule = 1;

            if (++n == argc)
            { return HI_FAILURE; }
            gPQModule = atoi(argv[n]);
            RANGECHECK(gPQModule, 0, 8);

            if (++n == argc)
            { return HI_FAILURE; }
            gModuleOnOff = atoi(argv[n]);
            RANGECHECK(gModuleOnOff, 0, 1);

            continue;
        }
        if (strncmp (arg, "-cg", strlen(arg)) == 0 || strncmp (arg, "--colorgain", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gColorGain = atoi(argv[n]);
            RANGECHECK(gColorGain, 0, 100);
            continue;
        }
        if (strncmp (arg, "-nr", strlen(arg)) == 0 || strncmp (arg, "--tnr", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gTnrLevel = atoi(argv[n]);
            RANGECHECK(gTnrLevel, 0, 100);
            continue;
        }
        if (strncmp (arg, "-dnr", strlen(arg)) == 0 || strncmp (arg, "--dnr", strlen(arg)) == 0)
        {
            if (++n == argc)
            { return HI_FAILURE; }
            gDnrLevel = atoi(argv[n]);
            RANGECHECK(gDnrLevel, 0, 100);
            continue;
        }

        HelpMsg();
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 main(int argc, char* argv[])
{
    HI_S32 s32Ret ;

    HI_UNF_PQ_Init();

    s32Ret = ParseCommandLine(argc, argv);
    if (HI_SUCCESS != s32Ret)
    {
        Printf("Parse command line error!\n");
        HI_UNF_PQ_DeInit();
        return HI_FAILURE;
    }

    /*get option */
    if (0 == gSetType)
    {
        if (NULL == gpGetOption)
        {
            HI_UNF_PQ_DeInit();
            HelpMsg();
            return HI_FAILURE;
        }
        else if (strncmp (gpGetOption, "-im", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--imagemode", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  image mode: %d (0:natural/1:person/2:film/3:UD)\n", gChan, getImageMode((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-bn", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--brightness", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  bright ness: %d\n", gChan, getBright((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-co", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--contrast", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  contrast: %d\n", gChan, getContrast((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-hu", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--hue", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  hue: %d\n", gChan, getHue((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-sa", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--saturation", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  saturation: %d\n", gChan, getSaturation((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-ct", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--colortempe", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d  color temperature: %d (0:cold/1:middle/2:warm)\n", gChan, getColorTemperature((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-dc", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--dyncontrast", strlen(gpGetOption)) == 0)
        {
            Printf("dynamic contrast: %d \n", getDynamicContrast((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-6b", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--6baseenhance", strlen(gpGetOption)) == 0)
        {
            get6BaseColorEnhance((HI_UNF_DISP_E)gChan, g6BaseColor);
        }
        else if (strncmp (gpGetOption, "-ft", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--fleshtone", strlen(gpGetOption)) == 0)
        {
            Printf("fleshtone: %d (0:off/1:low/2:mid/3:high)\n", getfleshtone((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-cm", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--colormode", strlen(gpGetOption)) == 0)
        {
            Printf("colormode: %d (0:recommend/1:blue/2:green/3:BG)\n", getColorEnhanceMode((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-sh", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--sharpness", strlen(gpGetOption)) == 0)
        {
            Printf("sharpness: %d\n",  getSharpness());
        }
        else if (strncmp (gpGetOption, "-dn", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--denoise", strlen(gpGetOption)) == 0)
        {
            Printf("denoise: %d (0:disable/1:auto)\n",  getDenoise());
        }
        else if (strncmp (gpGetOption, "-fm", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--filmmode", strlen(gpGetOption)) == 0)
        {
            Printf("film mode: %d (0:disable/1:auto)\n",  getFilmMode());
        }
        else if (strncmp (gpGetOption, "-cg", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--colorgain", strlen(gpGetOption)) == 0)
        {
            Printf("color gain: %d\n",  GetColorGain((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-nr", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--tnr", strlen(gpGetOption)) == 0)
        {
            Printf("TNR Level: %d\n",  GetTnrLevel((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-dnr", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--dnr", strlen(gpGetOption)) == 0)
        {
            Printf("DNR Level: %d\n",  GetDnrLevel((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-ddm", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--demomode", strlen(gpGetOption)) == 0)
        {
            Printf("Demo Mode: %d (0:fixed right/1:fixed left/2:scroll right/3:scroll left)\n", \
                   GetDemoMode((HI_UNF_DISP_E)gChan));
        }
        else if (strncmp (gpGetOption, "-dmc", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--dmcoor", strlen(gpGetOption)) == 0)
        {
            Printf("demo coordinate: %d \n", GetDemoCoordinate());
        }
        else if (strncmp (gpGetOption, "-pe", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--pqenable", strlen(gpGetOption)) == 0)
        {
            Printf("Module Sharpness: %s\n",  getPQModule(HI_UNF_PQ_MODULE_SHARPNESS) ? "on" : "off");
            Printf("Module DCI      : %s\n",  getPQModule(HI_UNF_PQ_MODULE_DCI) ? "on" : "off");
            Printf("Module Color    : %s\n",  getPQModule(HI_UNF_PQ_MODULE_COLOR) ? "on" : "off");
            Printf("Module SR       : %s\n",  getPQModule(HI_UNF_PQ_MODULE_SR) ? "on" : "off");
            Printf("Module TNR      : %s\n",  getPQModule(HI_UNF_PQ_MODULE_TNR) ? "on" : "off");
            Printf("Module DBM      : %s\n",  getPQModule(HI_UNF_PQ_MODULE_DBM) ? "on" : "off");
            Printf("Module SNR      : %s\n",  getPQModule(HI_UNF_PQ_MODULE_SNR) ? "on" : "off");
            Printf("Module MCDI     : %s\n",  getPQModule(HI_UNF_PQ_MODULE_MCDI) ? "on" : "off");
        }
        else if (strncmp (gpGetOption, "-a", strlen(gpGetOption)) == 0 || strncmp (gpGetOption, "--all", strlen(gpGetOption)) == 0)
        {
            Printf("chan %d:\n", gChan);
            Printf("bright ness:        %d\n", getBright((HI_UNF_DISP_E)gChan));
            Printf("contrast:           %d\n", getContrast((HI_UNF_DISP_E)gChan));
            Printf("hue:                %d\n", getHue((HI_UNF_DISP_E)gChan));
            Printf("saturation:         %d\n", getSaturation((HI_UNF_DISP_E)gChan));

            if (HI_UNF_DISPLAY1 == gChan)
            {
                Printf("dynamic contrast:   %d\n", getDynamicContrast((HI_UNF_DISP_E)gChan));
            }
            Printf("fleshtone:          %d (0:off/1:low/2:mid/3:high)\n", getfleshtone((HI_UNF_DISP_E)gChan));
            Printf("colormode:          %d (0:recommend/1:blue/2:green/3:BG)\n", getColorEnhanceMode((HI_UNF_DISP_E)gChan));
            Printf("demo mode:          %d (0:fixed right/1:fixed left/2:scroll right/3:scroll left)\n", GetDemoMode((HI_UNF_DISP_E)gChan));
            Printf("sharpness:          %d\n",  getSharpness());
            Printf("color gain:         %d\n",  GetColorGain((HI_UNF_DISP_E)gChan));
            Printf("Module Sharpness:   %s\n",  getPQModule(HI_UNF_PQ_MODULE_SHARPNESS) ? "on" : "off");
            Printf("Module DCI:         %s\n",  getPQModule(HI_UNF_PQ_MODULE_DCI) ? "on" : "off");
            Printf("Module Color:       %s\n",  getPQModule(HI_UNF_PQ_MODULE_COLOR) ? "on" : "off");
            Printf("Module SR:          %s\n",  getPQModule(HI_UNF_PQ_MODULE_SR) ? "on" : "off");
            Printf("Module TNR:         %s\n",  getPQModule(HI_UNF_PQ_MODULE_TNR) ? "on" : "off");
            Printf("Module DBM          %s\n",  getPQModule(HI_UNF_PQ_MODULE_DBM) ? "on" : "off");
            Printf("Module SNR          %s\n",  getPQModule(HI_UNF_PQ_MODULE_SNR) ? "on" : "off");
            Printf("Module MCDI         %s\n",  getPQModule(HI_UNF_PQ_MODULE_MCDI) ? "on" : "off");
            Printf("\n");
            Printf("TNR Level           %d\n",  GetTnrLevel((HI_UNF_DISP_E)gChan));
            Printf("DNR Level           %d\n",  GetDnrLevel((HI_UNF_DISP_E)gChan));
        }
        else
        {
            HelpMsg();
        }
    }
    /*set option */
    else if (1 == gSetType)
    {
        if (gImageMode > -1)
        {
            setImageMode((HI_UNF_DISP_E)gChan, gImageMode);
        }
        if (gDefaultMode > -1)
        {
            setDefault((HI_UNF_DISP_E)gChan, gDefaultMode);
        }
        if (gBright > -1)
        {
            setBright((HI_UNF_DISP_E)gChan, gBright);
        }
        if (gContrast > -1)
        {
            setContrast((HI_UNF_DISP_E)gChan, gContrast);
        }
        if (gHue > -1)
        {
            setHue((HI_UNF_DISP_E)gChan, gHue);
        }
        if (gSaturation > -1)
        {
            setSaturation((HI_UNF_DISP_E)gChan, gSaturation);
        }
        if (gColorTempe > -1)
        {
            setColorTemperature((HI_UNF_DISP_E)gChan, gColorTempe);
        }
        if (gDynContrast > -1)
        {
            setDynamicContrast((HI_UNF_DISP_E)gChan, gDynContrast);
        }
        if (g6BaseColor[5] > -1)
        {
            set6BaseColorEnhance((HI_UNF_DISP_E)gChan, g6BaseColor);
        }
        if (gVideoBaseParam[3] > -1)
        {
            setBaseParam(HI_UNF_PQ_IMAGE_VIDEO, (HI_UNF_DISP_E)gChan, gVideoBaseParam);
        }
        if (gGraphBaseParam[3] > -1)
        {
            setBaseParam(HI_UNF_PQ_IMAGE_GRAPH, (HI_UNF_DISP_E)gChan, gGraphBaseParam);
        }
        if (gFleshTone > -1)
        {
            setFleshTone((HI_UNF_DISP_E)gChan, gFleshTone);
        }
        if (gColorMode > -1)
        {
            setColorEnhanceMode((HI_UNF_DISP_E)gChan, gColorMode);
        }
        if (gSharpness > -1)
        {
            setSharpness( gSharpness);
        }
        if (gDenoise > -1)
        {
            setDenoise( gDenoise);
        }
        if (gFilmMode > -1)
        {
            setFilmMode( gFilmMode);
        }
        if (gSaveFlash > -1)
        {
            setSaveAll();
        }
        if (gSetDefaultParam > -1)
        {
            setDefaultParam();
        }
        if (gSetDemo > -1)
        {
            setDemo(gDemoMode, gDemoOnOff);
        }
        if (gSetPQModule > -1)
        {
            setPQModule(gPQModule, gModuleOnOff);
        }
        if (gColorGain > -1)
        {
            SetColorGain((HI_UNF_DISP_E)gChan, gColorGain);
        }
        if (gTnrLevel > -1)
        {
            SetTnrLevel((HI_UNF_DISP_E)gChan, gTnrLevel);
        }
        if (gDnrLevel > -1)
        {
            SetDnrLevel((HI_UNF_DISP_E)gChan, gDnrLevel);
        }
        if (gSetDemoDispMode > -1)
        {
            setDemoDispMode((HI_UNF_DISP_E)gChan, gDemoDispMode);
        }
        if (gSetDemoCoordinate > -1)
        {
            setDemoCoordinate((HI_UNF_DISP_E)gChan, gSetDemoCoordinate);
        }


    }

    HI_UNF_PQ_DeInit();

    return s32Ret;
}



