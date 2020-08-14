/******************************************************************************
*              Copyright 2004 - 2014, Hisilicon Tech. Co., Ltd.
*                           ALL RIGHTS RESERVED
* FileName: hi_gfx2d_errcode.h
* Description: Graphic 2D engine errcode declaration
*
* History:
* Version   Date         Author     DefectNum    Description
* main\1    2013-10-23              NULL         Create this file.
*******************************************************************************/
#ifndef _HI_GFX2D_ERRCODE_H_
#define _HI_GFX2D_ERRCODE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#define HI_ERR_GFX2D_BASE 0x80000001

enum
{
    HI_ERR_GFX2D_DEV_NOT_OPEN = HI_ERR_GFX2D_BASE,  /**<  device not open yet */
    HI_ERR_GFX2D_DEV_PERMISSION,              /*< device operation failed */
    HI_ERR_GFX2D_INVALID_DEVID,
    HI_ERR_GFX2D_NULL_PTR,                        /**<  parameters contain null ptr */
    HI_ERR_GFX2D_INVALID_COMPOSECNT,      /**< invalid composor count(>7 or =0) */
    HI_ERR_GFX2D_INVALID_SURFACE_TYPE,        /**< invalid surface info:colorfmt,phyaddr,stride,resolution... */
    HI_ERR_GFX2D_INVALID_SURFACE_RESO,
    HI_ERR_GFX2D_INVALID_SURFACE_FMT,
    HI_ERR_GFX2D_INVALID_SURFACE_ADDR,
    HI_ERR_GFX2D_INVALID_SURFACE_STRIDE,
    HI_ERR_GFX2D_INVALID_SURFACE_CMPTYPE,
    HI_ERR_GFX2D_INVALID_RECT,            /**< invalid opt rect:1.no intersection with surface */
    HI_ERR_GFX2D_INVALID_RESIZE_FILTERMODE,          /**< invalid resize info:1.invalid resize raito */
    HI_ERR_GFX2D_INVALID_RESIZE_RATIO,
    HI_ERR_GFX2D_INVALID_CLIP_MODE,                /**< invalid clip info */
    HI_ERR_GFX2D_INVALID_CLIP_RECT,
    HI_ERR_GFX2D_UNSUPPORT,           /**<  unsupported operation */
    HI_ERR_GFX2D_NO_MEM,                          /**<  lack of memory  */
    HI_ERR_GFX2D_TIMEOUT,                     /**<  sync task timeout */
    HI_ERR_GFX2D_INTERRUPT,               /*sync task interrupted by system*/
    HI_ERR_GFX2D_SYS,
};

#ifdef __cplusplus
#if __cplusplus
}
#endif  /*__cplusplus*/
#endif  /*__cplusplus*/

#endif /*_HI_GFX2D_ERRCODE_H_*/
