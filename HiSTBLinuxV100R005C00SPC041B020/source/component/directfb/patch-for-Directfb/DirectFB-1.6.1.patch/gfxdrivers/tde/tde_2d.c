/**
\file
\brief  2D acceleration adapter
            CNcomment:DIRECTFB硬件加速适配
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author memmng
\date 2010-7-30
*/
#include <config.h>

#include <directfb.h>

#include <direct/debug.h>
#include <direct/memcpy.h>
#include <direct/messages.h>

#include <core/state.h>
#include <core/surface.h>
#include <core/system.h>

#include <gfx/convert.h>

#include "hi_tde_type.h"
#include "hi_tde_api.h"
#include "hi_tde_errcode.h"
#include "tde_driver.h"
#include "tde_2d.h"
#include "tde_gfxdriver.h"
#include "SoftOperate.h"

/***************************** Macro Definition ******************************/


D_DEBUG_DOMAIN( TDE, "TDE/2D", "TDE 2D Acceleration" );

/*
 * State validation flags.
 *
 * There's no prefix because of the macros below.
 */

enum {
     BLITINGFLAGS      = 0x00000001, /* use: blit flags */
     DRAWINGFLAGS      = 0x00000002, /* use: draw flags */
     CLIP              = 0x00000004, /* use: blit and draw clip rect*/
     COLOR             = 0x00000008, /* use: draw color and modulate color*/
     SRC_BLEND         = 0x00000010, /* use: src blend function , only for port-duff srcover*/
     DST_BLEND         = 0x00000020, /* use: dst blend function , only for port-duff srcover*/
     SRC_COLORKEY      = 0x00000040, /* use: for src colorkey value */
     DST_COLORKEY      = 0x00000080, /* use: for dst colorkey value */
     DESTINATION       = 0x00000100, /* use: for dst surface*/
     SOURCE            = 0x00000200, /* no use: for src surface*/
     SOURCE_MASK       = 0x00000400, /* no use: for src surface mask*/
     SOURCE_MASKVALUE  = 0x00000800, /* no use: for src mask value */
     INDEX_TRANSLATION = 0x00001000, /* no use: clut format key value */
     COLORLEY          = 0x00002000, /* no use: protect key*/
     RENDER_OPTS       = 0x00004000, /* no use: render operation*/
     MATRIX            = 0x00008000, /* no use: matix, only for render operate*/
     ALL               = 0x0000ffff  /* all invalid*/
};
/*
 * State handling macros.
 */

#define TDE_VALIDATE(flags)        do { tdev->v_flags |=  (flags); } while (0)
#define TDE_INVALIDATE(flags)      do { tdev->v_flags &= ~(flags); } while (0)

#define TDE_CHECK_VALIDATE(flag)   do {                                               \
                                       if ((tdev->v_flags & flag) != flag)           \
                                            tde_validate_##flag( tdev, state ); \
                                  } while (0)

/*Use 2d acceleration when resolution is larger than MAX_HD_PIXEL*/
/*CNComment:只有分辨率大于一定的尺寸才能使用硬件操作.*/
//#define MAX_HD_PIXEL 14400      
/*#define IS_NEED_HDOPT(width, height) \
        ((width * height) > MAX_HD_PIXEL)    */
#define IS_NEED_HDOPT(width, height) HI_TRUE 
#define TIMEOUT 10
//HI_BOOL g_SynFlag = HI_TRUE;
HI_BOOL g_SynFlag = HI_FALSE;

#define  INFLEXTION    (130*130)                                  
static HI_U32 s_u32SrcVirAddr = 0;				
static HI_U32 s_u32SrcPhysAddr = 0;				
static HI_U32 s_u32DstVirAddr = 0;				
static HI_U32 s_u32DstPhysAddr = 0;				

#define TDE_FLAG g_SynFlag



/** now clut format is not supported*/
#define CHECK_TDE_FORMAT(format) \
do \
{ \
    switch(format) \
    { \
        case DSPF_ARGB1555: \
        case DSPF_ARGB4444: \
        case DSPF_ARGB: \
        case DSPF_RGB555: \
        case DSPF_RGB24: \
        case DSPF_RGB444: \
        case DSPF_RGB16: \
           break; \
        default: \
           D_DEBUG_AT( TDE, "  -> unsupported destination format %s\n", \
                       dfb_pixelformat_name(format) ); \
           return; \
   }\
} \
while(0);

#define IS_HAVE_CLIPRECT(pDev, pRect)\
    (pDev->clip_region.s32Xpos   != (pRect)->s32Xpos) || \
    (pDev->clip_region.s32Ypos   != (pRect)->s32Ypos) || \
    (pDev->clip_region.u32Width  != (pRect)->u32Width) || \
    (pDev->clip_region.u32Height != (pRect)->u32Height)    

static inline void CONVERT_RECT_TO_REGION (TDE2_RECT_S *pRect, DFBRegion *pRegion)
{
    pRegion->x1 = pRect->s32Xpos; 
    pRegion->x2 = pRect->s32Xpos + pRect->u32Width - 1; 
    pRegion->y1 = pRect->s32Ypos; 
    pRegion->y2 = pRect->s32Ypos + pRect->u32Height -1; 
    return;
}

static inline void  CONVERT_REGION_TO_RECT (DFBRegion *pRegion, TDE2_RECT_S *pRect) \
{
    pRect->s32Xpos   = pRegion->x1; 
    pRect->u32Width  = pRegion->x2 - pRegion->x1 + 1; 
    pRect->s32Ypos   = pRegion->y1; 
    pRect->u32Height = pRegion->y2 - pRegion->y1 + 1;     
    return;
}   

static inline int tde_check_rect(TDE2_RECT_S *pRect, TDE2_SURFACE_S *pSurface) 
{
    DFBRegion  SrcRegion, DstRegion, ResRegion;
    TDE2_RECT_S Rect;
    
    Rect.s32Xpos = Rect.s32Ypos = 0;
    Rect.u32Width =  pSurface->u32Width;
    Rect.u32Height =  pSurface->u32Height;
    CONVERT_RECT_TO_REGION(pRect, &SrcRegion);
    CONVERT_RECT_TO_REGION(&Rect, &DstRegion);
    
	ResRegion.x1 =  (SrcRegion.x1 > DstRegion.x1)?SrcRegion.x1:DstRegion.x1;
	ResRegion.y1 =  (SrcRegion.y1 > DstRegion.y1)?SrcRegion.y1:DstRegion.y1;	
	ResRegion.x2 =  (SrcRegion.x2 < DstRegion.x2)?SrcRegion.x2:DstRegion.x2;
	ResRegion.y2 =  (SrcRegion.y2 < DstRegion.y2)?SrcRegion.y2:DstRegion.y2;	

	if ((ResRegion.x1 > ResRegion.x2) || (ResRegion.y1 > ResRegion.y2))
		return -1;
	CONVERT_REGION_TO_RECT(&ResRegion, pRect);
	return 0;
}
/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */

static inline void
tde_validate_SRC_BLEND( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->src_blend = state->src_blend;     

     /* Set the flag. */
     TDE_VALIDATE( SRC_BLEND );
}

/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_DST_BLEND( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->dst_blend = state->dst_blend;     

     /* Set the flag. */
     TDE_VALIDATE( DST_BLEND );
}

static unsigned long convert2ARGBcolor(DFBSurfacePixelFormat format, unsigned long src_color)
{
    switch(format) 
    { 
        case DSPF_ARGB1555: 
            return  RGB1555TO32(src_color);
        case DSPF_ARGB4444: 
            return  RGB4444TO32(src_color);
        case DSPF_ARGB: 
            return  src_color;
        case DSPF_RGB555:
            return  RGB555TO32(src_color);
        case DSPF_RGB24: 
            return  src_color;
        case DSPF_RGB444: 
            return  RGB0444TO32(src_color);
        case DSPF_RGB16:
            return  RGB565TO32(src_color);
        default:
            break;
    }

    return  src_color;    
}
/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_DST_COLORKEY( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->dst_colorkey = convert2ARGBcolor(state->destination->config.format, state->dst_colorkey);     

     /* Set the flag. */
     TDE_VALIDATE( DST_COLORKEY );
}

/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_SRC_COLORKEY( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->src_colorkey =      tdev->dst_colorkey = convert2ARGBcolor(state->source->config.format, state->src_colorkey);     

     /* Set the flag. */
     TDE_VALIDATE( SRC_COLORKEY );
}
/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_DRAWINGFLAGS( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->drawingflags = state->drawingflags;
     /* Set the flag. */
     TDE_VALIDATE( DRAWINGFLAGS);
}

/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_BLITINGFLAGS( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->blittingflags = state->blittingflags;
     /* Set the flag. */
     TDE_VALIDATE( BLITINGFLAGS);
}
/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_CLIP( TDEDeviceData *tdev,
                             CardState        *state )
{
     /* Remember destination parameters for usage in rendering functions. */
     tdev->clip_region.s32Xpos  = state->clip.x1;
     tdev->clip_region.s32Ypos  = state->clip.y1;
     tdev->clip_region.u32Width = state->clip.x2 - state->clip.x1 + 1;
     tdev->clip_region.u32Height= state->clip.y2 - state->clip.y1 + 1;
     /* Set the flag. */
     TDE_VALIDATE( CLIP );
}


/*
 * Called by SetState() to ensure that the destination registers are properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_DESTINATION( TDEDeviceData *tdev,
                             CardState        *state )
{
	s_u32DstVirAddr = (HI_U32)state->dst.addr;
	/* Remember destination parameters for usage in rendering functions. */
	ADP_DFBSurfaceToTDESurface(&state->dst, &state->destination->config, &tdev->dst_surface);
	s_u32DstPhysAddr = (&state->dst)->phys;
	/* Set the flag. */
	TDE_VALIDATE( DESTINATION );
}

/*
 * Called by SetState() to ensure that the color register is properly set
 * for execution of rendering functions.
 */
static inline void
tde_validate_COLOR( TDEDeviceData *tdev,
                       CardState        *state )
{
     tdev->color_pixel = PIXEL_ARGB( state->color.a,
                                     state->color.r,
                                     state->color.g,
                                     state->color.b );
     /* Set the flag. */
     TDE_VALIDATE( COLOR );
}

/*
 * Called by SetState() to ensure that the source registers are properly set
 * for execution of blitting functions.
 */
static inline void
tde_validate_SOURCE( TDEDeviceData *tdev,
                        CardState        *state )
{
	s_u32SrcVirAddr = (HI_U32)state->src.addr;
     /* Remember source parameters for usage in rendering functions. */
     ADP_DFBSurfaceToTDESurface(&state->src, &state->source->config, &tdev->src_surface);
     s_u32SrcPhysAddr = (&state->src)->phys;

     /* Set the flag. */
     TDE_VALIDATE( SOURCE );
}

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

/*
 * Wait for the blitter to be idle.
 *
 * This function is called before memory that has been written to by the hardware is about to be
 * accessed by the CPU (software driver) or another hardware entity like video encoder (by Flip()).
 * It can also be called by applications explicitly, e.g. at the end of a benchmark loop to include
 * execution time of queued commands in the measurement.
 */
DFBResult TDEEngineSync   ( void  *drv, void  *dev )
{
//return DFB_OK;
    int Ret;
    /* wait all the task finish */
    Ret = HI_TDE2_WaitAllDone();
    if (Ret != HI_SUCCESS)
    {
    	D_ERROR("TDE sync Engine failed\n");
        return DFB_FAILURE;
    }
    D_DEBUG_AT(TDE, "tde sync ok\n");
    return DFB_OK;
}

/*
 * Reset the graphics engine.
 */
void      TDEEngineReset  ( void *drv, void  *dev )
{
    /** no need to realize*/
    D_DEBUG_AT( TDE, "%s()\n", __FUNCTION__ );
    return ;
}
void

/*
 * Start processing of queued commands if required.
 *
 * This function is called before returning from the graphics core to the application.
 * Usually that's after each rendering function. The only functions causing multiple commands
 * to be queued with a single emition at the end are DrawString(), TileBlit(), BatchBlit(),
 * DrawLines() and possibly FillTriangle() which is emulated using multiple FillRectangle() calls.
 */
 TDEEmitCommands ( void *drv,    void *dev )              
{
    /** no need to realize: all the tde commond is submit immediately*/
	
    return ;    
}                          

/* Check if the function 'accel' can be accelerated with the 'state'. If that's true, the
function sets the 'accel' bit in 'state->accel'. Otherwise the function just returns, no
need to clear the bit.
attention: state include : source format, dst format, accele and accele flags*/
void      TDECheckState   ( void *drv, void *dev, CardState *state, DFBAccelerationMask  accel )
{

     D_DEBUG_AT( TDE, "%s( state %p, accel 0x%08x ) <- dest %p [%lu]\n", __FUNCTION__,
                 state, accel, state->destination, state->dst.offset );

     /* step 1: check operation. Return if the desired function is not supported at all. */
     if (accel & ~(TDE_SUPPORTED_DRAWINGFUNCTIONS | TDE_SUPPORTED_BLITTINGFUNCTIONS)) {
          D_DEBUG_AT( TDE, "  -> unsupported function\n" );
          return;
     }

     /* Return if the source format is not supported. */

     /* Return if the destination format is not supported. */
      CHECK_TDE_FORMAT(state->destination->config.format);
     /* step 2: Check if drawing or blitting is requested. */
     if (DFB_DRAWING_FUNCTION( accel )) {
          /* Return if unsupported drawing flags are set. */
          if (state->drawingflags &~ TDE_SUPPORTED_DRAWINGFLAGS) {
               D_DEBUG_AT( TDE, "  -> unsupported drawing flags 0x%08x\n", state->drawingflags );
               return;
          }
          /** when the pixel is too small , don't use TDE*/
          if (!IS_NEED_HDOPT(state->destination->config.size.w, state->destination->config.size.h))
          {
              return;
          }

     }
     else {
          /* Return if the source format is not supported. */
          CHECK_TDE_FORMAT (state->source->config.format) 

          /* Return if unsupported blitting flags are set. */
          if (state->blittingflags & ~TDE_SUPPORTED_BLITTINGFLAGS) {
               D_DEBUG_AT( TDE, "  -> unsupported blitting flags 0x%08x\n", state->blittingflags );
               return;
          }
#if 0         
          //only support alpha blend with port-duff, can be changed in the future;
          if (state->blittingflags & DSBLIT_BLEND_ALPHACHANNEL)
          {
              if ((state->src_blend != DSBF_ONE)  || (state->dst_blend != DSBF_INVSRCCOLOR))
              {
                   return;
              }
          }
#endif
     }

     /* Enable acceleration of the function. */
     state->accel |= accel;

     D_DEBUG_AT( TDE, "  => OK\n" );

}

/*
    Program card for execution of the function 'accel' with the 'state'. 'state->modified'
contains information about changed entries. This function has to set at least 'accel' in
'state->set'. The driver should remember 'state->modified' and clear it. The driver may
modify 'funcs' depending on 'state' settings.
attention: don't chang the fields , if accel and field is not changged, else change the fields;
*/
void      TDESetState( void  *drv, void *dev, GraphicsDeviceFuncs *funcs, CardState  *state, DFBAccelerationMask  accel )
{
     //TDEDriverData           *tdrv     = drv;
     TDEDeviceData           *tdev     = dev;
     StateModificationFlags  modified = state->mod_hw;

     D_DEBUG_AT( TDE, "%s( state %p, accel 0x%08x ) <- dest %p, modified 0x%08x\n", __FUNCTION__,
                 state, accel, state->destination, modified );

     /*
      * 1) Invalidate hardware states
      *
      * Each modification to the hw independent state invalidates one or more hardware states.
      */

     /* Simply invalidate all */
     if (modified == SMF_ALL) {
          TDE_INVALIDATE( ALL );
     }
     else if (modified) 
     {

          /* no draw flags */
          if (modified & SMF_BLITTING_FLAGS)
               TDE_INVALIDATE( BLITINGFLAGS);

          if (modified & SMF_DRAWING_FLAGS)
               TDE_INVALIDATE( DRAWINGFLAGS);

          if (modified & SMF_CLIP)
               TDE_INVALIDATE( CLIP );

          if (modified & SMF_COLOR)
               TDE_INVALIDATE( COLOR );

          if (modified & (SMF_SRC_BLEND ))
               TDE_INVALIDATE( SRC_BLEND);

          if (modified & (SMF_DST_BLEND ))
               TDE_INVALIDATE( DST_BLEND);

          if (modified & SMF_SRC_COLORKEY)
               TDE_INVALIDATE( SRC_COLORKEY );

          if (modified & SMF_DST_COLORKEY)
               TDE_INVALIDATE( DST_COLORKEY );
              
          if (modified & SMF_DESTINATION)
               TDE_INVALIDATE( DESTINATION );

          if (modified & SMF_SOURCE)
               TDE_INVALIDATE( SOURCE );

          /* others variable no use, so don't need to add: 
                   SMF_MATRIX, SMF_RENDER_OPTIONS,
                   SMF_MATRIX, SMF_SOURCE_MASK, SMF_SOURCE_MASK_VALS, and so on*/  

     }


     /*
      * 2) Validate hardware states
      *
      * Each function has its own set of states that need to be validated.
      */

     /* Always requiring valid destination... */
     TDE_CHECK_VALIDATE( DESTINATION );
     TDE_CHECK_VALIDATE( CLIP );
     /* Depending on the function... */
     if (DFB_DRAWING_FUNCTION( accel ))
     {
        /** check draw color */
        TDE_CHECK_VALIDATE(COLOR);
        TDE_CHECK_VALIDATE(DRAWINGFLAGS);
        /** CHECK blend function, only support srcover opt, now*/
        TDE_CHECK_VALIDATE( SRC_BLEND);
        TDE_CHECK_VALIDATE( DST_BLEND);
     }
     else if (DFB_BLITTING_FUNCTION( accel ))
     {
        /** check blit function */
        TDE_CHECK_VALIDATE( BLITINGFLAGS);
        /** check blit function */
        TDE_CHECK_VALIDATE( SOURCE);
        /** CHECK blend function, only support srcover opt, now*/
        TDE_CHECK_VALIDATE( SRC_BLEND);
        TDE_CHECK_VALIDATE( DST_BLEND);
        /** check color  for gloal alpha*/
        TDE_CHECK_VALIDATE(COLOR);
        /** check color key*/
        TDE_CHECK_VALIDATE(SRC_COLORKEY);
        /** check dst color key*/
        TDE_CHECK_VALIDATE(DST_COLORKEY);
        
     }
     else
     {
        D_DEBUG_AT( TDE, "  -> unsupported operate\n");
     }

     return;
}
  
static HI_S32 ADP_ColorExpand(TDE2_COLOR_FMT_E PFmt, unsigned long Color, HI_U32 *pFillData)
{
    switch (PFmt)
    {
    case TDE2_COLOR_FMT_RGB444:
    case TDE2_COLOR_FMT_ARGB4444:
        *pFillData = (HI_U32)RGB32TO4444(Color);
        break;
    case TDE2_COLOR_FMT_RGB555:
    case TDE2_COLOR_FMT_ARGB1555:
        *pFillData = (HI_U32)RGB32TO1555(Color);
        break;
    case TDE2_COLOR_FMT_RGB888:
    case TDE2_COLOR_FMT_ARGB8888:
        *pFillData = (HI_U32)Color;
        break;
    case TDE2_COLOR_FMT_RGB565:
        *pFillData = RGB32TO565(Color);
        break;
    default:
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

bool      TDEFillRectangle( void *drv, void *dev, DFBRectangle *rect)
{
	HI_U32 u32FillData;
	HI_S32 s32Ret;
	TDE2_RECT_S Rect = {rect->x, rect->y, rect->w, rect->h};
	TDE_HANDLE s32Handle;
	TDEDeviceData *pDev;

	/**check tde operation*/

	if (!IS_NEED_HDOPT(rect->w, rect->h))
	{
		return HI_FALSE;
	}

	pDev =  (TDEDeviceData *)dev;
#if 0
	/**********add  by wangsuijun**********/
	if((Rect.u32Height*Rect.u32Width  <= INFLEXTION)&&(pDev->drawingflags == DSDRAW_NOFX))
	{
		TDE2_OPT_S TDEOpt;
		memset(&TDEOpt, 0, sizeof(TDE2_OPT_S));

		s32Ret = ADP_GenerateTDEFillOpt(pDev, &TDEOpt, HI_FALSE);
		if (HI_SUCCESS != s32Ret)
		{
			return HI_FALSE;
		}

		s32Ret = ADP_ColorExpand(pDev->dst_surface.enColorFmt, pDev->color_pixel, &u32FillData);
		if (s32Ret != HI_SUCCESS)
		{
			return HI_FALSE;
		}

		pDev->dst_surface.u32PhyAddr = s_u32DstVirAddr;
		s32Ret = Hi_Soft_FillRect(&pDev->dst_surface,  &Rect, u32FillData,&TDEOpt);
		if(!s32Ret)
		{
			return HI_TRUE;
		}
		else
		{
			pDev->dst_surface.u32PhyAddr = s_u32DstPhysAddr;
		}
	}
	/**********add  by wangsuijun**********/
#endif
	if ((pDev->drawingflags == DSDRAW_NOFX) && !(IS_HAVE_CLIPRECT(pDev, &Rect)))
	{
		/** color convertion */
		s32Ret = ADP_ColorExpand(pDev->dst_surface.enColorFmt, pDev->color_pixel, &u32FillData);
		if (s32Ret != HI_SUCCESS)
		{
			return HI_FALSE;
		}

		/** quick fill */
		s32Handle = HI_TDE2_BeginJob();
		if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
		{
			D_DEBUG_AT(TDE, "begin job start ");
			return HI_FALSE;
		}

		s32Ret = HI_TDE2_QuickFill(s32Handle, &pDev->dst_surface, &Rect, u32FillData);
		if (s32Ret != HI_SUCCESS)
		{
			D_DEBUG_AT(TDE, "do job start ");
			(HI_VOID)HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
			return HI_FALSE;
		}

		s32Ret = HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
		if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
		{
			D_DEBUG_AT(TDE, "do job end ");
			return HI_FALSE;
		}
	}
	else
	{
		TDE2_OPT_S TDEOpt;
		TDE2_FILLCOLOR_S FillColor = {TDE2_COLOR_FMT_ARGB8888, pDev->color_pixel};
		memset(&TDEOpt, 0, sizeof(TDE2_OPT_S));
		/** using solidDraw do alpha blending */

		s32Ret = ADP_ColorExpand(pDev->dst_surface.enColorFmt, pDev->color_pixel, &u32FillData);
		if(s32Ret != HI_SUCCESS){
			return HI_FALSE;
		}
		FillColor.enColorFmt   = pDev->dst_surface.enColorFmt;
		FillColor.u32FillColor = u32FillData;
		
		/** prepare alpha,key,rop opt */ 
		s32Ret = ADP_GenerateTDEFillOpt(pDev, &TDEOpt, HI_FALSE);
		if (HI_SUCCESS != s32Ret)
		{
			return HI_FALSE;
		}

		s32Handle = HI_TDE2_BeginJob();
		if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
		{
			D_DEBUG_AT(TDE, "begin job failed\n");
			return HI_FALSE;
		}
		if (IS_HAVE_CLIPRECT(pDev, &Rect))
		{
			TDEOpt.enClipMode = TDE2_CLIPMODE_INSIDE;
			TDEOpt.stClipRect.s32Xpos = pDev->clip_region.s32Xpos;
			TDEOpt.stClipRect.s32Ypos = pDev->clip_region.s32Ypos;
			TDEOpt.stClipRect.u32Width = pDev->clip_region.u32Width;
			TDEOpt.stClipRect.u32Height = pDev->clip_region.u32Height;
		}
		s32Ret = HI_TDE2_SolidDraw(s32Handle, &pDev->dst_surface, &Rect, &pDev->dst_surface, &Rect, &FillColor, &TDEOpt);
		if (s32Ret != HI_SUCCESS)
		{
			D_DEBUG_AT(TDE, "solid draw failed\n");
			(HI_VOID)HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
			return HI_FALSE;
		}

		s32Ret = HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
		if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
		{
			D_DEBUG_AT(TDE, "solid draw failed\n");
			return HI_FALSE;
		}
	}

	return HI_TRUE;
}

bool      TDEDrawRectangle( void *drv, void *dev, DFBRectangle *rect)
{
    DFBRectangle Rect0, Rect1, Rect2, Rect3;

    Rect0.x = rect->x;
    Rect0.y = rect->y;
    Rect0.w = rect->w;
    Rect0.h = 1;
    if ( HI_TRUE != TDEFillRectangle(drv, dev, &Rect0));
    {
        D_DEBUG_AT(TDE, "draw rectangle failed\n");
        return HI_FALSE;
    }
    Rect1.x = rect->x;
    Rect1.y = rect->y;
    Rect1.w = 1;
    Rect1.h = rect->h;
    if ( HI_TRUE != TDEFillRectangle(drv, dev, &Rect1));
    {
        D_DEBUG_AT(TDE, "draw rectangle failed\n");
        return HI_FALSE;
    }
    Rect2.x = rect->x + rect->w -1;
    Rect2.y = rect->y;
    Rect2.w = 1;
    Rect2.h = rect->h;
    if ( HI_TRUE != TDEFillRectangle(drv, dev, &Rect2));
    {
        D_DEBUG_AT(TDE, "draw rectangle failed\n");
        return HI_FALSE;
    }
    Rect3.x = rect->x;
    Rect3.y = rect->y + rect->h -1;
    Rect3.w = rect->w;
    Rect3.h = 1;
    if ( HI_TRUE != TDEFillRectangle(drv, dev, &Rect3));
    {
        D_DEBUG_AT(TDE, "draw rectangle failed\n");
        return HI_FALSE;
    }
    return HI_TRUE;
}                           

bool      TDEDrawLine (void  *drv, void *dev, DFBRegion *line )
{
    /** no need to relize*/
    return HI_FALSE;
}                           

bool      TDEFillTriangle ( void *drv, void *dev,  DFBTriangle *tri )
{
    /** no need to relize*/
    return HI_FALSE;
} 

bool      TDEBlit         ( void *drv, void *dev, DFBRectangle *srect, int dx, int dy )
{
	TDEDeviceData *pDev;
	HI_S32 s32Ret = HI_SUCCESS;
	TDE_HANDLE s32Handle;
	TDE2_RECT_S SrcRect = {srect->x, srect->y, (HI_U32)srect->w, (HI_U32)srect->h};
	TDE2_RECT_S DstRect = {dx, dy, (HI_U32)srect->w, (HI_U32)srect->h};
	TDE2_OPT_S stOpt;
	TDE2_OPT_S tmpstOpt;
	HI_BOOL bWithOpt = HI_FALSE, bHaveClip = HI_FALSE;

	/** when the pixel is too small , don't use TDE*/
	if (!IS_NEED_HDOPT(srect->w, srect->h))
	{
		return HI_FALSE;
	}

	memset(&stOpt, 0, sizeof(TDE2_OPT_S));
	memset(&tmpstOpt, 0, sizeof(TDE2_OPT_S));
	/** RGB 2 RGB: attention no support clut blit*/
	pDev = (TDEDeviceData *)dev;
	if(tde_check_rect(&SrcRect, &pDev->src_surface) < 0)
	{
		return HI_FALSE;
	}
	
	if(tde_check_rect(&DstRect, &pDev->dst_surface) < 0)
	{
		return HI_FALSE;
	}
	/** prepare alpha,key,rop opt */ 
	s32Ret = ADP_GenerateTDEOpt(pDev, &stOpt, HI_FALSE);
	if (HI_SUCCESS != s32Ret)
	{
		return HI_FALSE;
	}
#if 0
	/**********add  by wangsuijun**********/
	if(DstRect.u32Height*DstRect.u32Width  <= INFLEXTION)
	{
		if (0 == memcmp(&stOpt, &tmpstOpt, sizeof(TDE2_OPT_S)))
		{
			pDev->src_surface.u32PhyAddr = s_u32SrcVirAddr;
			pDev->dst_surface.u32PhyAddr = s_u32DstVirAddr;
			s32Ret = Hi_Soft_Blit(&pDev->src_surface,  &SrcRect,  &pDev->dst_surface,  &DstRect,  &stOpt);
			if(!s32Ret)
			{
				return HI_TRUE;
			}
			else
			{
				pDev->src_surface.u32PhyAddr = s_u32SrcPhysAddr;
				pDev->dst_surface.u32PhyAddr = s_u32DstPhysAddr;
			}
		}
	}
	/**********add  by wangsuijun**********/
#endif
	/** do tde job*/
	s32Handle = HI_TDE2_BeginJob();
	if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
	{
		D_DEBUG_AT(TDE, "beging job failued\n");
		return HI_FALSE;
	}
	if (0 != memcmp(&stOpt, &tmpstOpt, sizeof(TDE2_OPT_S)))
	{
		bWithOpt = HI_TRUE;
	}

	if (IS_HAVE_CLIPRECT(pDev, &DstRect))
	{
		bHaveClip =  HI_TRUE;
		stOpt.enClipMode = TDE2_CLIPMODE_INSIDE;
		stOpt.stClipRect.s32Xpos = pDev->clip_region.s32Xpos;
		stOpt.stClipRect.s32Ypos = pDev->clip_region.s32Ypos;
		stOpt.stClipRect.u32Width = pDev->clip_region.u32Width;
		stOpt.stClipRect.u32Height = pDev->clip_region.u32Height;
	}

	if (!bWithOpt && !bHaveClip && (pDev->dst_surface.enColorFmt == pDev->src_surface.enColorFmt))
	{
		s32Ret = HI_TDE2_QuickCopy(s32Handle, &pDev->src_surface, &SrcRect, &pDev->dst_surface,&DstRect); // do memcpy        
	}
	else if (bWithOpt) //if with operation (alpha or ckey) or clip
	{
		s32Ret = HI_TDE2_Bitblit(s32Handle, &pDev->dst_surface, &DstRect, &pDev->src_surface, &SrcRect, &pDev->dst_surface,
									&DstRect, &stOpt);            
	}
	else // without clip and operation
	{
		s32Ret = HI_TDE2_Bitblit(s32Handle, NULL, NULL, &pDev->src_surface, &SrcRect, &pDev->dst_surface, //do simple blit
									&DstRect, &stOpt);
	}

	if (s32Ret != HI_SUCCESS)
	{
		D_DEBUG_AT(TDE, "beging job failued\n");
		(HI_VOID)HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
		return HI_FALSE;
	}
	s32Ret = HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
	if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
	{
		D_DEBUG_AT(TDE, "end job failued\n");
		return HI_FALSE;
	}
	return HI_TRUE;
}                           

bool      TDEStretchBlit  ( void *drv, void *dev, DFBRectangle *srect, DFBRectangle  *drect )
{
	TDEDeviceData *pDev;
	HI_S32 s32Ret = HI_SUCCESS;
	TDE_HANDLE s32Handle;
	TDE2_RECT_S SrcRect = {srect->x, srect->y, (HI_U32)srect->w, (HI_U32)srect->h};
	TDE2_RECT_S DstRect = {drect->x, drect->y, (HI_U32)drect->w, (HI_U32)drect->h};
	TDE2_OPT_S stOpt, tmpstOpt;

	memset(&stOpt, 0, sizeof(TDE2_OPT_S));
	memset(&tmpstOpt, 0, sizeof(TDE2_OPT_S));
	if (!IS_NEED_HDOPT(srect->w, srect->h))
	{
		return HI_FALSE;
	}

	if (!IS_NEED_HDOPT(drect->w, drect->h))
	{
		return HI_FALSE;
	}
#if 0
	// tde cann't support 16 scale, can be modify in pilot
	if ((srect->w > (drect->w * 15)) || (srect->h > (drect->h * 15)))
	{
	return HI_FALSE;
	}
#endif    
	//  printf("src width:%d, height:%d, dst widht:%d, height:%d\n", state->source->config.size.w, state->source->config.size.h, state->destination->config.size.w ,state->destination->config.size.h);

	/** RGB 2 RGB: attention no support clut blit*/
	pDev = (TDEDeviceData *)dev;
	/** prepare alpha,key,rop opt */ 
	s32Ret = ADP_GenerateTDEOpt(dev, &stOpt, HI_TRUE);
	if (HI_SUCCESS != s32Ret)
	{
		return HI_FALSE;
	}
	if(tde_check_rect(&SrcRect, &pDev->src_surface) < 0)
	{
		return HI_FALSE;
	}
	
	if(tde_check_rect(&DstRect, &pDev->dst_surface) < 0)
	{
		return HI_FALSE;
	}
#if 0
	/**********add  by wangsuijun**********/
	if(DstRect.u32Height*DstRect.u32Width  <= INFLEXTION)
	{
		if (0 == memcmp(&stOpt, &tmpstOpt, sizeof(TDE2_OPT_S)))
		{
			pDev->src_surface.u32PhyAddr = s_u32SrcVirAddr;
			pDev->dst_surface.u32PhyAddr = s_u32DstVirAddr;
			Hi_Soft_StretchBlit( &pDev->src_surface,  &SrcRect,   &pDev->dst_surface,  &DstRect,  &stOpt);
			if(!s32Ret)
			{
				return HI_TRUE;
			}
			else
			{
				pDev->src_surface.u32PhyAddr = s_u32SrcPhysAddr;
				pDev->dst_surface.u32PhyAddr = s_u32DstPhysAddr;
			}
		}
	}
	/**********add  by wangsuijun**********/
#endif	
	/** do tde job*/
	s32Handle = HI_TDE2_BeginJob();
	if (s32Handle == HI_ERR_TDE_INVALID_HANDLE)
	{
		D_DEBUG_AT(TDE, "beging job failued\n");
		return HI_FALSE;
	}
	if (IS_HAVE_CLIPRECT(pDev, &DstRect))
	{
		stOpt.enClipMode = TDE2_CLIPMODE_INSIDE;
		stOpt.stClipRect.s32Xpos = pDev->clip_region.s32Xpos;
		stOpt.stClipRect.s32Ypos = pDev->clip_region.s32Ypos;
		stOpt.stClipRect.u32Width = pDev->clip_region.u32Width;
		stOpt.stClipRect.u32Height = pDev->clip_region.u32Height;
	}
	if (0 != memcmp(&stOpt, &tmpstOpt, sizeof(TDE2_OPT_S))) 
	{
		s32Ret = HI_TDE2_Bitblit(s32Handle, &pDev->dst_surface, &DstRect, &pDev->src_surface, &SrcRect, &pDev->dst_surface,
									&DstRect, &stOpt);
	}
	else
	{
		s32Ret = HI_TDE2_Bitblit(s32Handle, NULL, NULL, &pDev->src_surface, &SrcRect, &pDev->dst_surface, &DstRect, &stOpt);
	}

	if (s32Ret != HI_SUCCESS)
	{
		D_DEBUG_AT(TDE, "beging job failued\n");
		(HI_VOID)HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
		return HI_FALSE;
	}
	s32Ret = HI_TDE2_EndJob(s32Handle, HI_FALSE, TDE_FLAG, TIMEOUT);
	if ((HI_SUCCESS != s32Ret) && (HI_ERR_TDE_JOB_TIMEOUT != s32Ret))
	{
		D_DEBUG_AT(TDE, "end job failued\n");
		return HI_FALSE;
	}

	return HI_TRUE;
}                           


