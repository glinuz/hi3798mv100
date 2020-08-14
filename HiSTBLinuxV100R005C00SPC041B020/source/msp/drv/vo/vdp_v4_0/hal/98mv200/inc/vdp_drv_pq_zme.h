#ifndef __DRV_PQ_ZME_H__
#define __DRV_PQ_ZME_H__
#if EDA_TEST
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#endif

#include "vdp_drv_comm.h"


#ifndef VDP_PQ_STRATEGY
typedef enum
{
    HI_PQ_ZME_COEF_TYPE_LH = 0,
    HI_PQ_ZME_COEF_TYPE_LV    ,
    HI_PQ_ZME_COEF_TYPE_CH    ,
    HI_PQ_ZME_COEF_TYPE_CV    ,
    
    HI_PQ_ZME_COEF_TYPE_BUTT
} HI_PQ_ZME_COEF_TYPE_E;

typedef enum
{
    HI_PQ_ZME_COEF_1         = 0,
    HI_PQ_ZME_COEF_E1           ,
    HI_PQ_ZME_COEF_075          ,
    HI_PQ_ZME_COEF_05           ,
    HI_PQ_ZME_COEF_033          ,
    HI_PQ_ZME_COEF_025          ,
    HI_PQ_ZME_COEF_0            ,

    HI_PQ_ZME_COEF_RATIO_BUTT
} HI_PQ_ZME_COEF_RATIO_E;


typedef enum
{
    HI_PQ_ZME_TAP_8T32P = 0,
    HI_PQ_ZME_TAP_6T32P    ,
    HI_PQ_ZME_TAP_5T32P    ,
    HI_PQ_ZME_TAP_4T32P    ,
    HI_PQ_ZME_TAP_3T32P    ,
    HI_PQ_ZME_TAP_2T32P    ,
    HI_PQ_ZME_TAP_8T2P     ,
    HI_PQ_ZME_TAP_6T2P     ,

    HI_PQ_ZME_TAP_BUTT
} HI_PQ_ZME_TAP_E;
#endif


typedef enum hiVDP_VID_ZOOM_COEF_E
{
    VDP_ZOOM_COEF_1  = 0,
    VDP_ZOOM_COEF_EQU_1,
    VDP_ZOOM_COEF_075,
    VDP_ZOOM_COEF_05,
    VDP_ZOOM_COEF_033,
    VDP_ZOOM_COEF_025,
    VDP_ZOOM_COEF_0,
    VDP_ZOOM_COEF_BUTT

} VDP_ZOOM_COEF_E;

typedef enum hiVDP_ZOOM_TAP_E
{
    VDP_ZOOM_TAP_8LH = 0,
    VDP_ZOOM_TAP_6LV ,
    VDP_ZOOM_TAP_8CH ,
    VDP_ZOOM_TAP_6CV ,
    VDP_ZOOM_TAP_4CH ,
    VDP_ZOOM_TAP_4CV ,
    VDP_ZOOM_TAP_2CV ,
    VDP_TiHf_ZOOM_TAP_2LH,
    VDP_TiHf_ZOOM_TAP_2LV,
    VDP_TiHf_ZOOM_TAP_2CH,
    VDP_TiHf_ZOOM_TAP_2CV,
    VDP_ZOOM_TAP_5LV ,
    VDP_ZOOM_TAP_3CV ,
    VDP_ZOOM_TAP_BUTT
} VDP_ZOOM_TAP_E;
typedef struct
{
    HI_U32 coef_addr_tap_l8c4_1;   //18*8+18*4
    HI_U32 coef_addr_tap_l8c4_equal1;
    HI_U32 coef_addr_tap_l8c4_075 ;
    HI_U32 coef_addr_tap_l8c4_05  ;
    HI_U32 coef_addr_tap_l8c4_033 ;
    HI_U32 coef_addr_tap_l8c4_025 ;
    HI_U32 coef_addr_tap_l8c4_0;

    HI_U32 coef_addr_tap_l8c8_1;   //18*8+18*8
    HI_U32 coef_addr_tap_l8c8_equal1;
    HI_U32 coef_addr_tap_l8c8_075 ;
    HI_U32 coef_addr_tap_l8c8_05  ;
    HI_U32 coef_addr_tap_l8c8_033 ;
    HI_U32 coef_addr_tap_l8c8_025 ;
    HI_U32 coef_addr_tap_l8c8_0;

    HI_U32 coef_addr_tap_l6c6_1;   //18*6+18*6
    HI_U32 coef_addr_tap_l6c6_equal1;
    HI_U32 coef_addr_tap_l6c6_075 ;
    HI_U32 coef_addr_tap_l6c6_05  ;
    HI_U32 coef_addr_tap_l6c6_033 ;
    HI_U32 coef_addr_tap_l6c6_025 ;
    HI_U32 coef_addr_tap_l6c6_0;

    HI_U32 coef_addr_tap_l6c4_1;
    HI_U32 coef_addr_tap_l6c4_equal1;
    HI_U32 coef_addr_tap_l6c4_075;
    HI_U32 coef_addr_tap_l6c4_05;
    HI_U32 coef_addr_tap_l6c4_033;
    HI_U32 coef_addr_tap_l6c4_025;
    HI_U32 coef_addr_tap_l6c4_0;

    HI_U32 coef_addr_tap_l4c4_1;
    HI_U32 coef_addr_tap_l4c4_equal1 ;
    HI_U32 coef_addr_tap_l4c4_075;
    HI_U32 coef_addr_tap_l4c4_05;
    HI_U32 coef_addr_tap_l4c4_033;
    HI_U32 coef_addr_tap_l4c4_025;
    HI_U32 coef_addr_tap_l4c4_0;

    HI_U32 coef_addr_tap_l4c2_1;
    HI_U32 coef_addr_tap_l4c2_equal1;
    HI_U32 coef_addr_tap_l4c2_075;
    HI_U32 coef_addr_tap_l4c2_05;
    HI_U32 coef_addr_tap_l4c2_033;
    HI_U32 coef_addr_tap_l4c2_025;
    HI_U32 coef_addr_tap_l4c2_0;



    HI_U32 coef_addr_vc2tap_1   ;
    HI_U32 coef_addr_vc2tap_equal1 ;
    HI_U32 coef_addr_vc2tap_075 ;
    HI_U32 coef_addr_vc2tap_05  ;
    HI_U32 coef_addr_vc2tap_033 ;
    HI_U32 coef_addr_vc2tap_025 ;
    HI_U32 coef_addr_vc2tap_0   ;
} FIR_COEF_ADDR_S;

/* 对位域的更小拆分，一方面是为了满足10bit内存排布；
   另一方面，位域如果进行跨int操作，会出现错误。
 */
typedef struct hiVO_ZOOM_BIT_S
{
    HI_S32	bits_0	:	10	;
    HI_S32	bits_1	:	10	;
    HI_S32	bits_2	:	10	;
    HI_S32	bits_32	:	2	;
    HI_S32	bits_38	:	8	;
    HI_S32	bits_4	:	10	;
    HI_S32	bits_5	:	10	;
    HI_S32	bits_64	:	4	;
    HI_S32	bits_66	:	6	;
    HI_S32	bits_7	:	10	;
    HI_S32	bits_8	:	10	;
    HI_S32	bits_96	:	6	;
    HI_S32	bits_94	:	4	;
    HI_S32	bits_10	:	10	;
    HI_S32	bits_11	:	10	;
    HI_S32	bits_12	:	8	;
} VDP_VID_ZOOM_BIT_S;

typedef struct hiVDP_VID_ZOOM_BITARRAY_S
{
    HI_U32        u32Size;
    VDP_VID_ZOOM_BIT_S stBit[12];
} VDP_VID_ZOOM_BITARRAY_S;



//HI_S32 DRV_PQ_GetVdpZmeCoef(HI_PQ_ZME_COEF_RATIO_E enCoefRatio, HI_PQ_ZME_COEF_TYPE_E enCoefType, HI_PQ_ZME_TAP_E enZmeTap, HI_U8* ps16Coef);
HI_VOID DRV_PQ_SetVdpZmeCoef(VDP_ZOOM_TAP_E enVdpZoomTap,HI_PQ_ZME_COEF_TYPE_E enCoefType,HI_U8  *VirAddr, HI_U8  *ps32Coef);
HI_S32 VDP_GetVdpZmeCoef(HI_PQ_ZME_COEF_RATIO_E enCoefRatio, HI_PQ_ZME_COEF_TYPE_E enCoefType, HI_PQ_ZME_TAP_E enZmeTap, HI_S16 *ps16Coef);
//HI_S32  DRV_PQ_GetVdpZmeCoef(HI_PQ_ZME_COEF_RATIO_E enCoefRatio, HI_PQ_ZME_COEF_TYPE_E enCoefType, HI_PQ_ZME_TAP_E enZmeTap, HI_S32* ps16Coef);
//static HI_S32  DRV_VDP_LoadZoomCoefVC(HI_S16* ps16Coef,VDP_ZOOM_TAP_E enChZoomTap, HI_U8 *pu8Addr);
//static HI_S32 DRV_VDP_LoadZoomCoefVH(HI_S16* ps16Coef,VDP_ZOOM_TAP_E enLZoomTap, HI_U8 *pu8Addr);
//static HI_U32 DRV_VDP_LoadZoomCoefHC(HI_S16* ps16Coef,VDP_ZOOM_TAP_E enChZoomTap, HI_U8 *pu8Addr);
//static HI_U32 DRV_VDP_LoadZoomCoefHL(HI_S16* ps16Coef,VDP_ZOOM_TAP_E enLZoomTap, HI_U8 *pu8Addr);
//static HI_U32 DRV_VDP_TranlateZoomCoef(const HI_S32 *ps32Coef, VDP_ZOOM_TAP_E enTap,VDP_VID_ZOOM_BITARRAY_S *pBitCoef);


#endif
