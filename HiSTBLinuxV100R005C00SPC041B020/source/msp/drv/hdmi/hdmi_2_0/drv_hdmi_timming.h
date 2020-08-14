/******************************************************************************

  Copyright (C), 2014-2024, Hisilicon Tech. Co., Ltd.

 ******************************************************************************
  File Name     : drv_hdmi_timming.h
  Version       : Initial Draft
  Author        : Hisilicon multimedia software group
  Created       : 2015/02/01
  Description   :
  History       :
  Date          : 2015/02/01
  Author        : t00273561
  Modification  :
*******************************************************************************/
#ifndef __DRV_HDMI_TIMMING_H__
#define __DRV_HDMI_TIMMING_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#if defined(CONFIG_HDMI_STB_SDK)


#include "hi_type.h"
#include "hi_drv_disp.h"
#include "drv_hdmi_common.h"
#include "drv_hdmi_infoframe.h"
#include "hi_drv_hdmi.h"






/* Define the union HDMI_U_DHD0_CTRL */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;
    /* Define the struct bits */
    struct
    {
        unsigned int    regup                 : 1   ; /* [0]  */
        unsigned int    disp_mode             : 3   ; /* [3..1]  */
        unsigned int    iop                   : 1   ; /* [4]  */
        unsigned int    intf_ivs              : 1   ; /* [5]  */
        unsigned int    intf_ihs              : 1   ; /* [6]  */
        unsigned int    intf_idv              : 1   ; /* [7]  */
        unsigned int    reserved_0            : 1   ; /* [8]  */
        unsigned int    hdmi420c_sel          : 1   ; /* [9]  */
        unsigned int    hdmi420_en            : 1   ; /* [10]  */
        unsigned int    uf_offline_en         : 1   ; /* [11]  */
        unsigned int    gmm_mode              : 1   ; /* [12]  */
        unsigned int    gmm_en                : 1   ; /* [13]  */
        unsigned int    hdmi_mode             : 1   ; /* [14]  */
        unsigned int    twochn_debug          : 1   ; /* [15]  */
        unsigned int    reserved_1            : 1   ; /* [16]  */
        unsigned int    mirror_en             : 1   ; /* [17]  */
        unsigned int    sin_en                : 1   ; /* [18]  */
        unsigned int    trigger_en            : 1   ; /* [19]  */
        unsigned int    fpga_lmt_width        : 7   ; /* [26..20]  */
        unsigned int    fpga_lmt_en           : 1   ; /* [27]  */
        unsigned int    p2i_en                : 1   ; /* [28]  */
        unsigned int    cbar_sel              : 1   ; /* [29]  */
        unsigned int    cbar_en               : 1   ; /* [30]  */
        unsigned int    intf_en               : 1   ; /* [31]  */
    } bits;

    

} HDMI_U_DHD0_CTRL;

/* Define the union HDMI_U_DHD0_VSYNC */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;
    /* Define the struct bits */
    struct
    {
        unsigned int    vact                  : 12  ; /* [11..0]  */
        unsigned int    vbb                   : 10  ; /* [21..12]  */
        unsigned int    vfb                   : 10  ; /* [31..22]  */
    } bits;

} HDMI_U_DHD0_VSYNC;

/* Define the union HDMI_U_DHD0_HSYNC1 */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;
    /* Define the struct bits */
    struct
    {
        unsigned int    hact                  : 16  ; /* [15..0]  */
        unsigned int    hbb                   : 16  ; /* [31..16]  */
    } bits;

} HDMI_U_DHD0_HSYNC1;

/* Define the union HDMI_U_DHD0_HSYNC2 */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;

    /* Define the struct bits */
    struct
    {
        unsigned int    hfb                   : 16  ; /* [15..0]  */
        unsigned int    hmid                  : 16  ; /* [31..16]  */
    } bits;


} HDMI_U_DHD0_HSYNC2;

/* Define the union HDMI_U_DHD0_VPLUS */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;

    /* Define the struct bits */
    struct
    {
        unsigned int    bvact                 : 12  ; /* [11..0]  */
        unsigned int    bvbb                  : 10  ; /* [21..12]  */
        unsigned int    bvfb                  : 10  ; /* [31..22]  */
    } bits;

} HDMI_U_DHD0_VPLUS;

/* Define the union HDMI_U_DHD0_PWR */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;

    /* Define the struct bits */
    struct
    {
        unsigned int    hpw                   : 16  ; /* [15..0]  */
        unsigned int    vpw                   : 8   ; /* [23..16]  */
        unsigned int    reserved_0            : 8   ; /* [31..24]  */
    } bits;

} HDMI_U_DHD0_PWR;

/* Define the union HDMI_U_DHD0_VTTHD3 */
typedef union
{
    /* Define an unsigned member */
    unsigned int    u32;

    /* Define the struct bits */
    struct
    {
        unsigned int    vtmgthd3              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 2   ; /* [14..13]  */
        unsigned int    thd3_mode             : 1   ; /* [15]  */
        unsigned int    vtmgthd4              : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    thd4_mode             : 1   ; /* [31]  */
    } bits;

} HDMI_U_DHD0_VTTHD3;

/* Define the union HDMI_U_DHD0_VTTHD */
typedef union
{

    /* Define an unsigned member */
    unsigned int    u32;
    /* Define the struct bits */
    struct
    {
        unsigned int    vtmgthd1              : 13  ; /* [12..0]  */
        unsigned int    reserved_0            : 2   ; /* [14..13]  */
        unsigned int    thd1_mode             : 1   ; /* [15]  */
        unsigned int    vtmgthd2              : 13  ; /* [28..16]  */
        unsigned int    reserved_1            : 2   ; /* [30..29]  */
        unsigned int    thd2_mode             : 1   ; /* [31]  */
    } bits;

} HDMI_U_DHD0_VTTHD;

typedef struct
{
    HDMI_VIC_E                  enVic          ;
    HI_DRV_DISP_FMT_E           enDisFmt       ;
    HI_U32                      u32PixRepeat   ;
    
    HI_U32                      DHD0_VTTHD                        ; /* 0xc01c */
    HI_U32                      DHD0_VTTHD3                       ; /* 0xc018 */
    HI_U32                      DHD0_PWR                          ; /* 0xc014 */
    HI_U32                      DHD0_VPLUS                        ; /* 0xc010 */
    HI_U32                      DHD0_HSYNC2                       ; /* 0xc00c */
    HI_U32                      DHD0_HSYNC1                       ; /* 0xc008 */
    HI_U32                      DHD0_VSYNC                        ; /* 0xc004 */
    HI_U32                      DHD0_CTRL                         ; /* 0xc000 */

}HDMI_VDP_TIMMING_S;


HI_S32  DRV_HDMI_TimmingSet(HDMI_DEVICE_ID_E enHdmiId,HDMI_VIC_E enVic);
HI_S32  DRV_HDMI_DispAttrSet(HDMI_DEVICE_ID_E enHdmiId,HDMI_VIDEO_ATTR_S *pstDispAttr);


#endif /* CONFIG_HDMI_STB_SDK */


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif  /* __DRV_HDMI_TIMMING_H__ */  


