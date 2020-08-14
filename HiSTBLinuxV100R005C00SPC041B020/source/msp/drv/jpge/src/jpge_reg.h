/******************************************************************************
*
* Copyright (C) 2016 Hisilicon Technologies Co., Ltd.  All rights reserved.
*
* This program is confidential and proprietary to Hisilicon  Technologies Co., Ltd. (Hisilicon),
* and may not be copied, reproduced, modified, disclosed to others, published or used, in
* whole or in part, without the express prior written permission of Hisilicon.
*
******************************************************************************
File Name          : jpge_reg.h
Version            : Initial Draft
Author             :
Created            : 2016/03/03
Description        :
Function List      :


History           :
Date                  Author                Modification
2016/03/03            y00181162              Created file
******************************************************************************/
#ifndef __JPGE_REG_H__
#define __JPGE_REG_H__


/*********************************add include here******************************/


/*****************************************************************************/


/*****************************************************************************/

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/***************************** Macro Definition ******************************/


/*************************** Structure Definition ****************************/


/***************************  The enum of Jpeg image format  ******************/

// Define the union U_JPGE_INTSTAT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPic          : 1   ; // [0]
        unsigned int    VencBufFull           : 1   ; // [1]
        unsigned int    JpgeTimeout           : 1   ; // [2]
        unsigned int    cfgErr                : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_INTSTAT;

// Define the union U_JPGE_INTMASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPicMask      : 1   ; // [0]
        unsigned int    VencBufFullMask       : 1   ; // [1]
        unsigned int    JpgeTimeoutMask       : 1   ; // [2]
        unsigned int    cfgErrMask            : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_INTMASK;

// Define the union U_JPGE_RAWINT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPic          : 1   ; // [0]
        unsigned int    VencBufFull           : 1   ; // [1]
        unsigned int    JpgeTimeout           : 1   ; // [2]
        unsigned int    cfgErr                : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_RAWINT;

// Define the union U_JPGE_INTCLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPicClr       : 1   ; // [0]
        unsigned int    VencBufFullClr        : 1   ; // [1]
        unsigned int    JpgeTimeoutClr        : 1   ; // [2]
        unsigned int    cfgErrClr             : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_INTCLR;

// Define the union U_JPGE_PICFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    strFmt                : 2   ; // [1..0]
        unsigned int    yuvFmt                : 2   ; // [3..2]
        unsigned int    chnId                 : 8   ; // [11..4]
        unsigned int    rotationAngle         : 2   ; // [13..12]
        unsigned int    clkGateEn             : 2   ; // [15..14]
        unsigned int    memClkGateEn          : 1   ; // [16]
        unsigned int    configlockEn          : 1   ; // [17]
        unsigned int    timeoutEn             : 1   ; // [18]
        unsigned int    roundEn               : 1   ; // [19]
        unsigned int    packageSel            : 8   ; // [27..20]
        unsigned int    jcpi_clip_en          : 1   ; // [28]
        unsigned int    reserved2             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_PICFG;

// Define the union U_JPGE_ECSCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ecsSplit              : 1   ; // [0]
        unsigned int    ecsSize               : 20  ; // [20..1]
        unsigned int    reserved0             : 11  ; // [31..21]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_ECSCFG;

// Define the union U_JPGE_START
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VStart                : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_START;

// Define the union U_JPGE_VEDIMGSIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ImgWidthInPixelsMinus1  : 13  ; // [12..0]
        unsigned int    Reserved2             : 3   ; // [15..13]
        unsigned int    ImgHeightInPixelsMinus1  : 13  ; // [28..16]
        unsigned int    Reserved1             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_VEDIMGSIZE;

// Define the union U_JPGE_CLIP_THR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    jcpi_luma_min         : 8   ; // [7..0]
        unsigned int    jcpi_luma_max         : 8   ; // [15..8]
        unsigned int    jcpi_chr_min          : 8   ; // [23..16]
        unsigned int    jcpi_chr_max          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_CLIP_THR;

// Define the union U_JPGE_SSTRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    SrcYStride            : 16  ; // [15..0]
        unsigned int    SrcCStride            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_SSTRIDE;

// Define the union U_JPGE_STRMBUFLEN
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Reserved_0            : 6   ; // [5..0]
        unsigned int    BufferLength          : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_STRMBUFLEN;

// Define the union U_JPGE_OUTSTD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    outstd                : 3   ; // [2..0]
        unsigned int    Reserved_0            : 29  ; // [31..3]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_OUTSTD;

// Define the union U_VPP_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    reserved1             : 5   ; // [4..0]
        unsigned int    osdEn                 : 1   ; // [5]
        unsigned int    reserved0             : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_CFG;

// Define the union U_VPP_OSD_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osdRgbfmt             : 1   ; // [0]
        unsigned int    Reserved_0            : 15  ; // [15..1]
        unsigned int    osd0GlobalEn          : 1   ; // [16]
        unsigned int    osd1GlobalEn          : 1   ; // [17]
        unsigned int    osd2GlobalEn          : 1   ; // [18]
        unsigned int    osd3GlobalEn          : 1   ; // [19]
        unsigned int    osd4GlobalEn          : 1   ; // [20]
        unsigned int    osd5GlobalEn          : 1   ; // [21]
        unsigned int    osd6GlobalEn          : 1   ; // [22]
        unsigned int    osd7GlobalEn          : 1   ; // [23]
        unsigned int    osd0En                : 1   ; // [24]
        unsigned int    osd1En                : 1   ; // [25]
        unsigned int    osd2En                : 1   ; // [26]
        unsigned int    osd3En                : 1   ; // [27]
        unsigned int    osd4En                : 1   ; // [28]
        unsigned int    osd5En                : 1   ; // [29]
        unsigned int    osd6En                : 1   ; // [30]
        unsigned int    osd7En                : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_CFG;

// Define the union U_VPP_OSD01_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd0Alpha0            : 8   ; // [7..0]
        unsigned int    osd0Alpha1            : 8   ; // [15..8]
        unsigned int    osd1Alpha0            : 8   ; // [23..16]
        unsigned int    osd1Alpha1            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD01_ALPHA;

// Define the union U_VPP_OSD23_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd2Alpha0            : 8   ; // [7..0]
        unsigned int    osd2Alpha1            : 8   ; // [15..8]
        unsigned int    osd3Alpha0            : 8   ; // [23..16]
        unsigned int    osd3Alpha1            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD23_ALPHA;

// Define the union U_VPP_OSD45_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd4Alpha0            : 8   ; // [7..0]
        unsigned int    osd4Alpha1            : 8   ; // [15..8]
        unsigned int    osd5Alpha0            : 8   ; // [23..16]
        unsigned int    osd5Alpha1            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD45_ALPHA;

// Define the union U_VPP_OSD67_ALPHA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd6Alpha0            : 8   ; // [7..0]
        unsigned int    osd6Alpha1            : 8   ; // [15..8]
        unsigned int    osd7Alpha0            : 8   ; // [23..16]
        unsigned int    osd7Alpha1            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD67_ALPHA;

// Define the union U_VPP_OSD_ALPHA0123
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd0GlobalAlpha       : 8   ; // [7..0]
        unsigned int    osd1GlobalAlpha       : 8   ; // [15..8]
        unsigned int    osd2GlobalAlpha       : 8   ; // [23..16]
        unsigned int    osd3GlobalAlpha       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_ALPHA0123;

// Define the union U_VPP_OSD_ALPHA4567
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd4GlobalAlpha       : 8   ; // [7..0]
        unsigned int    osd5GlobalAlpha       : 8   ; // [15..8]
        unsigned int    osd6GlobalAlpha       : 8   ; // [23..16]
        unsigned int    osd7GlobalAlpha       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_ALPHA4567;

// Define the union U_VPP_OSD_POS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osdStartX             : 13  ; // [12..0]
        unsigned int    reserved1             : 3   ; // [15..13]
        unsigned int    osdStartY             : 13  ; // [28..16]
        unsigned int    reserved0             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_POS;

// Define the union U_VPP_OSD_SIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osdW                  : 13  ; // [12..0]
        unsigned int    reserved1             : 3   ; // [15..13]
        unsigned int    osdH                  : 13  ; // [28..16]
        unsigned int    reserved0             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_SIZE;

// Define the union U_VPP_OSD_STRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osdStride             : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_STRIDE;

// Define the union U_VPP_OSD_LAYERID
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd0LayerId           : 3   ; // [2..0]
        unsigned int    reserved7             : 1   ; // [3]
        unsigned int    osd1LayerId           : 3   ; // [6..4]
        unsigned int    reserved6             : 1   ; // [7]
        unsigned int    osd2LayerId           : 3   ; // [10..8]
        unsigned int    reserved5             : 1   ; // [11]
        unsigned int    osd3LayerId           : 3   ; // [14..12]
        unsigned int    reserved4             : 1   ; // [15]
        unsigned int    osd4LayerId           : 3   ; // [18..16]
        unsigned int    reserved3             : 1   ; // [19]
        unsigned int    osd5LayerId           : 3   ; // [22..20]
        unsigned int    reserved2             : 1   ; // [23]
        unsigned int    osd6LayerId           : 3   ; // [26..24]
        unsigned int    reserved1             : 1   ; // [27]
        unsigned int    osd7LayerId           : 3   ; // [30..28]
        unsigned int    reserved0             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_LAYERID;

// Define the union U_VPP_OSD_CINVCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    osd0CinvEn            : 1   ; // [0]
        unsigned int    osd1CinvEn            : 1   ; // [1]
        unsigned int    osd2CinvEn            : 1   ; // [2]
        unsigned int    osd3CinvEn            : 1   ; // [3]
        unsigned int    osd4CinvEn            : 1   ; // [4]
        unsigned int    osd5CinvEn            : 1   ; // [5]
        unsigned int    osd6CinvEn            : 1   ; // [6]
        unsigned int    osd7CinvEn            : 1   ; // [7]
        unsigned int    osdCinvWidth          : 2   ; // [9..8]
        unsigned int    osdCinvHeight         : 2   ; // [11..10]
        unsigned int    reserved1             : 4   ; // [15..12]
        unsigned int    osdCinvThr            : 8   ; // [23..16]
        unsigned int    osdCinvMode           : 1   ; // [24]
        unsigned int    reserved0             : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VPP_OSD_CINVCFG;

// Define the union U_JPGE_QPT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    QP0                   : 8   ; // [7..0]
        unsigned int    QP1                   : 8   ; // [15..8]
        unsigned int    QP2                   : 8   ; // [23..16]
        unsigned int    QP3                   : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_JPGE_QPT;

//==============================================================================
// Define the global struct
typedef struct
{
    volatile U_JPGE_INTSTAT         JPGE_INTSTAT;
    volatile U_JPGE_INTMASK         JPGE_INTMASK;
    volatile U_JPGE_RAWINT          JPGE_RAWINT;
    volatile U_JPGE_INTCLR          JPGE_INTCLR;
    volatile U_JPGE_PICFG           JPGE_PICFG;
    volatile U_JPGE_ECSCFG          JPGE_ECSCFG;
    volatile unsigned int           RESERVED0;
    volatile U_JPGE_START           JPGE_START;
    volatile U_JPGE_VEDIMGSIZE      JPGE_VEDIMGSIZE;
    volatile U_JPGE_CLIP_THR        JPGE_CLIP_THR;
    volatile unsigned int           RESERBED1[2];
    volatile unsigned int           JPGE_SRCYADDR;
    volatile unsigned int           JPGE_SRCCADDR;
    volatile unsigned int           JPGE_SRCVADDR;
    volatile U_JPGE_SSTRIDE         JPGE_SSTRIDE;
    volatile unsigned int           JPGE_STRMADDR;
    volatile unsigned int           JPGE_SRPTRADDR;
    volatile unsigned int           JPGE_SWPTRADDR;
    volatile U_JPGE_STRMBUFLEN      JPGE_STRMBUFLEN;
    volatile unsigned int           RESERVED3[8];
    volatile unsigned int           JPGE_PTS0;
    volatile unsigned int           JPGE_PTS1;
    volatile unsigned int           JPGE_PTS2;
    volatile unsigned int           JPGE_PTS3;
    volatile unsigned int           JPGE_TIMEOUT;
    volatile unsigned int           JPGE_TIMER;
    volatile unsigned int           RESERVED4[7];
    volatile U_JPGE_OUTSTD          JPGE_OUTSTD;
    volatile unsigned int           RESERVED4_1[21];
    volatile unsigned int           JPGE_VERSION;
    volatile unsigned int           RESERVED9[256];
    volatile U_VPP_CFG              VPP_CFG;
    volatile unsigned int           RESERVED5[3];
    volatile U_VPP_OSD_CFG          VPP_OSD_CFG;
    volatile U_VPP_OSD01_ALPHA      VPP_OSD01_ALPHA;
    volatile U_VPP_OSD23_ALPHA      VPP_OSD23_ALPHA;
    volatile U_VPP_OSD45_ALPHA      VPP_OSD45_ALPHA;
    volatile U_VPP_OSD67_ALPHA      VPP_OSD67_ALPHA;
    volatile U_VPP_OSD_ALPHA0123    VPP_OSD_ALPHA0123;
    volatile U_VPP_OSD_ALPHA4567    VPP_OSD_ALPHA4567;
    volatile unsigned int           RESERVED6;
    volatile U_VPP_OSD_POS          VPP_OSD_POS[8];
    volatile U_VPP_OSD_SIZE         VPP_OSD_SIZE[8];
    volatile unsigned int           VPP_OSD_ADDR[8];
    volatile U_VPP_OSD_STRIDE       VPP_OSD_STRIDE[8];
    volatile unsigned int           RESERVED8[4];
    volatile U_VPP_OSD_LAYERID      VPP_OSD_LAYERID;
    volatile unsigned int           RESERVED7[3];
    volatile U_VPP_OSD_CINVCFG      VPP_OSD_CINVCFG;
    volatile unsigned int           RESERVED8_1[651];
    volatile U_JPGE_QPT             JPGE_QPT[48];
} S_JPGE_REGS_TYPE;


/********************** Global Variable declaration **************************/

/******************************* API declaration *****************************/

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /*__JPGE_REG_H__ */
