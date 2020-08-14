/**
\file
\brief      2D acceleration header
            CNcomment:DIRECTFB硬件加速适配头文件
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author memmng
\date 2010-7-30
*/

#ifndef __HI3720_2D_H__
#define __HI3720_2D_H__

/* add include here */


#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/
#ifdef MPW
#define TDE_SUPPORTED_DRAWINGFLAGS      (DSDRAW_NOFX|DSDRAW_BLEND)
#else
#define TDE_SUPPORTED_DRAWINGFLAGS      (DSDRAW_NOFX|DSDRAW_BLEND|DSDRAW_SRC_PREMULTIPLY|DSDRAW_DST_PREMULTIPLY|DSDRAW_XOR)
#endif
#define TDE_SUPPORTED_DRAWINGFUNCTIONS  (DFXL_FILLRECTANGLE|DFXL_DRAWRECTANGLE)
/** DFXL_DRAWLINE could not be support now,  */

#define TDE_SUPPORTED_BLITTINGFLAGS    (DSBLIT_BLEND_ALPHACHANNEL | \
                                        DSBLIT_BLEND_COLORALPHA | \
                                        DSBLIT_SRC_COLORKEY| \
                                        DSBLIT_DST_COLORKEY| \
                                        DSBLIT_DST_PREMULTIPLY| \
                                        DSBLIT_SRC_PREMULTIPLY| \
                                        DSBLIT_COLORIZE)
                                        
/*                                      DSBLIT_ROTATE90 | \
                                        DSBLIT_ROTATE180| \
                                        DSBLIT_ROTATE270 | \                                        
                                        DSBLIT_SRC_PREMULTCOLOR)
        
*/

#define TDE_SUPPORTED_BLITTINGFUNCTIONS (DFXL_BLIT | \
                                         DFXL_STRETCHBLIT)

/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/

DFBResult TDEEngineSync   ( void                *drv,
                           void                *dev );

void      TDEEngineReset  ( void                *drv,
                           void                *dev );

void      TDEEmitCommands ( void                *drv,
                           void                *dev );

void      TDECheckState   ( void                *drv,
                           void                *dev,
                           CardState           *state,
                           DFBAccelerationMask  accel );

void      TDESetState     ( void                *drv,
                           void                *dev,
                           GraphicsDeviceFuncs *funcs,
                           CardState           *state,
                           DFBAccelerationMask  accel );

bool      TDEFillRectangle( void                *drv,
                           void                *dev,
                           DFBRectangle        *rect );

bool      TDEDrawRectangle( void                *drv,
                           void                *dev,
                           DFBRectangle        *rect );

bool      TDEDrawLine     ( void                *drv,
                           void                *dev,
                           DFBRegion           *line );

bool      TDEFillTriangle ( void                *drv,
                           void                *dev,
                           DFBTriangle         *tri );

bool      TDEBlit         ( void                *drv,
                           void                *dev,
                           DFBRectangle        *srect,
                           int                  dx,
                           int                  dy );

bool      TDEStretchBlit  ( void                *drv,
                           void                *dev,
                           DFBRectangle        *srect,
                           DFBRectangle        *drect );



#ifdef __cplusplus
}
#endif
#endif /* __HI3720_2D_H__ */
