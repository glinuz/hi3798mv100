/**
\file
\brief TDE adapt    CNcomment:TDEÊÊÅä
\copyright Shenzhen Hisilicon Co., Ltd.
\version draft
\author memmng
\date 2010-10-12
*/

#ifndef __TDE_DRIVER_H__
#define __TDE_DRIVER_H__

/* add include here */


#ifdef __cplusplus
extern "C" {
#endif
/***************************** Macro Definition ******************************/

#define RGB32TO4444(c) \
    (((c & 0xf0000000) >> 16) | ((c & 0xf00000) >> 12) | ((c & 0xf000) >> 8) | ((c & 0xf0) >> 4))

#define RGB32TO1555(c) \
    (((c & 0x80000000) >> 16) | ((c & 0xf80000) >> 9) | ((c & 0xf800) >> 6) | ((c & 0xf8) >> 3))

#define RGB32TO565(c) \
    (((c & 0xf80000) >> 8) | ((c & 0xfc00) >> 5) | ((c & 0xf8) >> 3))

#define RGB32TO8565(c) \
    ((c & 0xff000000) >> 8 | ((c & 0xf80000) >> 8) | ((c & 0xfc00) >> 5) | ((c & 0xf8) >> 3))
  
#define RGB4444TO32(c) (((c & 0xf000) << 16) | \
                        ((c & 0x0f00) << 12) | \
                        ((c & 0x00f0) << 8) | \
                        ((c & 0x000f) << 4))

#define RGB0444TO32(c) (0xff000000 | ((c & 0x0f00) << 12) | \
                        ((c & 0x00f0) << 8) | \
                        ((c & 0x000f) << 4))

#define RGB565TO32(c) ((0xff000000 | (c & 0xf800) << 8) | \
                       ((c & 0x7e0) << 5) | \
                       ((c & 0x1f) << 3))
#define RGB8565TO32(c) (((c & 0x00ff0000)<<8) | ((c & 0xf800) << 8) | \
                       ((c & 0x7e0) << 5) | \
                       ((c & 0x1f) << 3))
                       
#define RGB555TO32(c) (0xff000000 | \
                       ((c & 0x7c00) << 9) | \
                       ((c & 0x3e0) << 6) | \
                       ((c & 0x1f) << 3))

#define RGB1555TO32(c) (((c & 0x8000) ? 0xff000000 : 0x0) | \
                        ((c & 0x7c00) << 9) | \
                        ((c & 0x3e0) << 6) | \
                        ((c & 0x1f) << 3))
/*************************** Structure Definition ****************************/



/********************** Global Variable declaration **************************/



/******************************* API declaration *****************************/
HI_S32 ADP_DFBSurfaceToTDESurface(CoreSurfaceBufferLock *pSurLock, CoreSurfaceConfig *pConfig, TDE2_SURFACE_S* pTDESurface);

HI_S32 ADP_GenerateTDEOpt(void *dev, TDE2_OPT_S* pTDEOpt, HI_BOOL bScale);

HI_S32 ADP_GenerateTDEFillOpt(void *dev, TDE2_OPT_S* pTDEOpt, HI_BOOL bScale);

#ifdef __cplusplus
}
#endif
#endif /* __TDE_DRIVER_H__ */
