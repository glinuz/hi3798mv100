/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved. 
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon), 
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name           : hi_adp_gfx.c
Version             : Initial Draft
Author              : 
Created             : 2016/07/18
Description         : display initial
Function List       : 

                                          
History             :
Date                        Author                  Modification
2016/07/18                  y00181162               Created file        
******************************************************************************/

/*********************************add include here******************************/
#ifdef HI_DIRECTFB_SUPPORT
#include "hi_adp_gfx.h"
#endif

/*****************************************************************************/


/***************************** Macro Definition ******************************/
#ifdef HI_DIRECTFB_SUPPORT
#define DFBCHECK(x...) \
     {                                                                \
          err = x;                                                    \
          if (err != DFB_OK) {                                        \
               SAMPLE_TRACE("%s <%d>:\n\t", __FILE__, __LINE__ );     \
               DirectFBErrorFatal( #x, err );                         \
          }                                                           \
     }
#endif

/*************************** Structure Definition ****************************/

/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef HI_DIRECTFB_SUPPORT
static HI_S32 SAMPLE_Disp_Init(HI_UNF_ENC_FMT_E enFormat,HI_U32 u32ScreenW,HI_U32 u32ScreenH)
{
    HI_S32                      Ret;
    HI_UNF_DISP_BG_COLOR_S      BgColor;
    HI_UNF_DISP_INTF_S          DispIntf;
    HI_UNF_DISP_OFFSET_S        offset;
    HI_UNF_ENC_FMT_E            SdFmt = HI_UNF_ENC_FMT_PAL;

    Ret = HI_UNF_DISP_Init();
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Init failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_HDMI;
    DispIntf.unIntf.enHdmi = HI_UNF_HDMI_ID_0;

    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

#if defined(HI_DAC_YPBPR_Y) && defined(HI_DAC_YPBPR_PB) && defined(HI_DAC_YPBPR_PR)
    DispIntf.enIntfType = HI_UNF_DISP_INTF_TYPE_YPBPR;
    DispIntf.unIntf.stYPbPr.u8DacY  = HI_DAC_YPBPR_Y;
    DispIntf.unIntf.stYPbPr.u8DacPb = HI_DAC_YPBPR_PB;
    DispIntf.unIntf.stYPbPr.u8DacPr = HI_DAC_YPBPR_PR;

    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY1, &DispIntf, 1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

#ifdef HI_DAC_CVBS
    DispIntf.enIntfType            = HI_UNF_DISP_INTF_TYPE_CVBS;
    DispIntf.unIntf.stCVBS.u8Dac   = HI_DAC_CVBS;
    Ret = HI_UNF_DISP_AttachIntf(HI_UNF_DISPLAY0, &DispIntf, 1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_AttachIntf failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }
#endif

    Ret = HI_UNF_DISP_Attach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Attach failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY1, enFormat);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_SetFormat failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    switch (enFormat)
    {
        case HI_UNF_ENC_FMT_3840X2160_60 :
        case HI_UNF_ENC_FMT_3840X2160_30 :
        case HI_UNF_ENC_FMT_3840X2160_24 :
        case HI_UNF_ENC_FMT_1080P_60 :
        case HI_UNF_ENC_FMT_1080P_30 :
        case HI_UNF_ENC_FMT_1080i_60 :
        case HI_UNF_ENC_FMT_720P_60 :
        case HI_UNF_ENC_FMT_480P_60 :
        case HI_UNF_ENC_FMT_NTSC :
            SdFmt = HI_UNF_ENC_FMT_NTSC;
            break;

        case HI_UNF_ENC_FMT_3840X2160_50 :
        case HI_UNF_ENC_FMT_3840X2160_25 :
        case HI_UNF_ENC_FMT_1080P_50 :
        case HI_UNF_ENC_FMT_1080P_25 :
        case HI_UNF_ENC_FMT_1080i_50 :
        case HI_UNF_ENC_FMT_720P_50 :
        case HI_UNF_ENC_FMT_576P_50 :
        case HI_UNF_ENC_FMT_PAL :
            SdFmt = HI_UNF_ENC_FMT_PAL;
            break;

        default:
            break;
    }

    Ret = HI_UNF_DISP_SetFormat(HI_UNF_DISPLAY0, SdFmt);
    if(HI_SUCCESS != Ret){
        SAMPLE_TRACE("call HI_UNF_DISP_SetFormat failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

	/**
	 **放在这里，刚好底下display操作有延时了，使之配置生效
	 **/
    Ret = HI_UNF_DISP_SetVirtualScreen(HI_UNF_DISPLAY1, u32ScreenW, u32ScreenH);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_SetVirtualScreen failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    offset.u32Left      = 0;
    offset.u32Top       = 0;
    offset.u32Right     = 0;
    offset.u32Bottom    = 0;
    Ret = HI_UNF_DISP_SetScreenOffset(HI_UNF_DISPLAY1, &offset);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    /*set display0 screen offset*/
    Ret = HI_UNF_DISP_SetScreenOffset(HI_UNF_DISPLAY0, &offset);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    BgColor.u8Red   = 0;
    BgColor.u8Green = 0;
    BgColor.u8Blue  = 0;
    Ret = HI_UNF_DISP_SetBgColor(HI_UNF_DISPLAY1, &BgColor);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_SetBgColor failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Open DISPLAY1 failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HI_UNF_DISP_Open(HI_UNF_DISPLAY0);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Open DISPLAY0 failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    Ret = HIADP_HDMI_Init(HI_UNF_HDMI_ID_0);
    if(HI_SUCCESS != Ret){
        SAMPLE_TRACE("call HIADP_HDMI_Init failed, Ret=%#x.\n", Ret);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
        HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
        HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
        HI_UNF_DISP_DeInit();
        return Ret;
    }

    return HI_SUCCESS;
}

static HI_S32 SAMPLE_Disp_DeInit(HI_VOID)
{
    HI_S32 Ret;

    Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Close failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_Close(HI_UNF_DISPLAY0);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Close failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_Detach(HI_UNF_DISPLAY0, HI_UNF_DISPLAY1);
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_Detach failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    Ret = HI_UNF_DISP_DeInit();
    if(Ret != HI_SUCCESS){
        SAMPLE_TRACE("call HI_UNF_DISP_DeInit failed, Ret=%#x.\n", Ret);
        return Ret;
    }

    HIADP_HDMI_DeInit(HI_UNF_HDMI_ID_0);

    return HI_SUCCESS;
}
/*****************************************************************************
* func          : df_printSurfaceProp
* description   : CNcomment: 打印surface属性信息 CNend\n
* param[in]     : pSurface CNcomment:CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
HI_VOID df_printSurfaceProp(IDirectFBSurface * pSurface)
{
	int	err =0 ;
	DFBSurfacePixelFormat pf = 0;

	err = pSurface->GetPixelFormat(pSurface , &pf);
	if(DFB_OK != err ){
		SAMPLE_TRACE("printSurfaceProp Failed: Line %d  \n" , __LINE__ );
		return;
	}else{
		SAMPLE_TRACE("Surface PixelFormat 0x%x ,DSPF_ARGB 0x%x , DSPF_ARGB1555 0x%x  \n" ,
		          pf , DSPF_ARGB , DSPF_ARGB1555 );
	}

	return;
}

/*****************************************************************************
* func          : df_clearScreen
* description   : CNcomment: 清屏幕 CNend\n
* param[in]     : layer CNcomment:CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
HI_VOID df_clearScreen(IDirectFBDisplayLayer  *layer)
{

	DFBResult err = DFB_FAILURE;

	/**
	 **modify the cooperative level to admin. this is very importtant if you
	 **want to revise some attribute
	 **/
	DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE ));

	//directly set the layer's background color, no need other surface or window object.
	DFBCHECK(layer->SetBackgroundColor( layer, 0, 0, 0, 0));
	DFBCHECK(layer->SetBackgroundMode ( layer, DLBM_COLOR ));

	//restore the cooperative level.
	DFBCHECK(layer->SetCooperativeLevel( layer, DLSCL_SHARED ));

	return;
	
}

/*****************************************************************************
* func          : df_dec_to_surface
* description   : CNcomment: 解图片数据到surface CNend\n
* param[in]     : CNcomment: CNend\n
* retval        : NA
* others:       : NA
*****************************************************************************/
HI_S32 df_dec_to_surface(const char* pcPicPath,HI_U32 u32Width,HI_U32 u32Height,IDirectFB *dfb, IDirectFBSurface **Surface)
{

	IDirectFBImageProvider *provider = NULL;
	DFBSurfaceDescription surfaceDesc;
	DFBResult err = DFB_FAILURE;
    
    if(dfb == NULL){
        SAMPLE_TRACE("dfb must be inited at first\n");
        return HI_FAILURE;
    }
    
    if(pcPicPath == NULL){
        SAMPLE_TRACE("FATAL ERROR: dfbShowPicture > picture path is NULL\n");
        return HI_FAILURE;
    }

    DFBCHECK(dfb->CreateImageProvider( dfb, pcPicPath, &provider ));

	 /**
	 **create surface
	 **/
	 DFBCHECK(provider->GetSurfaceDescription( provider, &surfaceDesc ));
	 surfaceDesc.flags      |= DSDESC_CAPS | DSDESC_PIXELFORMAT;
     surfaceDesc.pixelformat = DSPF_ARGB;
    //surfaceDesc.caps       = DSCAPS_SHARED;       /** blit **/
     surfaceDesc.caps        = DSCAPS_VIDEOONLY;    /** blit **/
    //surfaceDesc.caps       = DSCAPS_SYSTEMONLY;   /** soft **/
    
	 surfaceDesc.width  = u32Width;
	 surfaceDesc.height = u32Height;
     DFBCHECK(dfb->CreateSurface( dfb, &surfaceDesc, Surface ));
     
     DFBCHECK(provider->RenderTo( provider, *Surface, NULL ));
     
     DFBCHECK(provider->Release( provider ));
     
     provider = NULL;
    
    return HI_SUCCESS;
}

/*****************************************************************************
* func            : dfb_sample_init
* description     : init display
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
void __attribute__ ((constructor)) dfb_sample_init(void)
{
    //system("rmmod hi_fb.ko");
    //system("insmod /kmod/hi_fb.ko video=\"hifb:vram0_size:24300,vram1_size:24300\"\n");
    HI_SYS_Init();
    SAMPLE_Disp_Init(DISPLAY_FMT,SCREEN_WIDTH,SCREEN_HEIGHT);
    SAMPLE_TRACE("====disp init success\n");
    return;
}

/*****************************************************************************
* func            : dfb_sample_dinit
* description     : dinit display
* retval          : HI_SUCCESS
* retval          : HI_FAILURE
*****************************************************************************/
void __attribute__ ((destructor)) dfb_sample_dinit(void)
{
    SAMPLE_Disp_DeInit();
    HI_SYS_DeInit();
    SAMPLE_TRACE("====disp dinit success\n");
    //system("rmmod hi_fb.ko");
    //system("insmod /kmod/hi_fb.ko");
    return;
}
#endif
