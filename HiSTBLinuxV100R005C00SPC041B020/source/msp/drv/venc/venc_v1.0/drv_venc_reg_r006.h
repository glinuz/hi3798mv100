//******************************************************************************
//  Copyright (C), 2016-2020, Hisilicon Technologies Co., Ltd.
//******************************************************************************
// File name     : venc_reg.h
// Version       : 2.0
// Author        : XXX
// Created       : 2010-12-30
// Last Modified :
// Description   :  The C union definition file for XXX
// Function List :
// History       :
// 1 Date        :
// Author        : XXX
// Modification  : Create file
//******************************************************************************
#ifndef __VEDU_REGR006_H__
#define __VEDU_REGR006_H__

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

#define VEDU_PILOT_REG_EN

// Define the union U_VEDU_INTSTAT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPic          : 1   ; // [0]
        unsigned int    VencBufFull           : 1   ; // [1]
        unsigned int    rserved5              : 1   ; // [2]
        unsigned int    VencPbitOverflow      : 1   ; // [3]
        unsigned int    reserved4             : 4   ; // [7..4]
        unsigned int    VdecEndOfPic          : 1   ; // [8]
        unsigned int    VdecStreamError       : 1   ; // [9]
        unsigned int    reserved1             : 6   ; // [15..10]
        unsigned int    VeduBreakPoint        : 1   ; // [16]
        unsigned int    VeduStep              : 1   ; // [17]
        unsigned int    reserved3             : 9   ; // [26..18]
        unsigned int    VeduTimeout           : 1   ; // [27]
        unsigned int    reserved2             : 2   ; // [29..28]
        unsigned int    cfgErr                : 1   ; // [30]
        unsigned int    reserved0             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_INTSTAT;

// Define the union U_VEDU_INTMASK
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPicMask      : 1   ; // [0]
        unsigned int    VencBufFullMask       : 1   ; // [1]
        unsigned int    reserved5             : 1   ; // [2]
        unsigned int    VencPbitOverflow      : 1   ; // [3]
        unsigned int    reserved4             : 4   ; // [7..4]
        unsigned int    VdecEndOfPicMask      : 1   ; // [8]
        unsigned int    VdecStreamErrorMask   : 1   ; // [9]
        unsigned int    reserved1             : 6   ; // [15..10]
        unsigned int    VeduBreakPointMask    : 1   ; // [16]
        unsigned int    VeduStepMask          : 1   ; // [17]
        unsigned int    reserved3             : 9   ; // [26..18]
        unsigned int    VeduTimeoutMask       : 1   ; // [27]
        unsigned int    reserved2             : 2   ; // [29..28]
        unsigned int    cfgErrMask            : 1   ; // [30]
        unsigned int    reserved0             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_INTMASK;

// Define the union U_VEDU_RAWINT
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPic          : 1   ; // [0]
        unsigned int    VencBufFull           : 1   ; // [1]
        unsigned int    reserved5             : 1   ; // [2]
        unsigned int    VencPbitOverflow      : 1   ; // [3]
        unsigned int    reserved4             : 4   ; // [7..4]
        unsigned int    VdecEndOfPic          : 1   ; // [8]
        unsigned int    VdecStreamError       : 1   ; // [9]
        unsigned int    reserved1             : 6   ; // [15..10]
        unsigned int    VeduBreakPoint        : 1   ; // [16]
        unsigned int    VeduStep              : 1   ; // [17]
        unsigned int    reserved3             : 9   ; // [26..18]
        unsigned int    VeduTimeout           : 1   ; // [27]
        unsigned int    reserved2             : 2   ; // [29..28]
        unsigned int    cfgErr                : 1   ; // [30]
        unsigned int    reserved0             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_RAWINT;

// Define the union U_VEDU_INTCLR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VencEndOfPicClr       : 1   ; // [0]
        unsigned int    VencBufFullClr        : 1   ; // [1]
        unsigned int    reserved5             : 1   ; // [2]
        unsigned int    VencPbitOverflow      : 1   ; // [3]
        unsigned int    reserved4             : 4   ; // [7..4]
        unsigned int    VdecEndOfPicClr       : 1   ; // [8]
        unsigned int    VdecStreamErrorClr    : 1   ; // [9]
        unsigned int    reserved1             : 6   ; // [15..10]
        unsigned int    VeduBreakPointClr     : 1   ; // [16]
        unsigned int    VeduStepClr           : 1   ; // [17]
        unsigned int    reserved3             : 9   ; // [26..18]
        unsigned int    VeduTimeoutClr        : 1   ; // [27]
        unsigned int    reserved2             : 2   ; // [29..28]
        unsigned int    cfgErrClr             : 1   ; // [30]
        unsigned int    reserved0             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_INTCLR;

// Define the union U_VEDU_START
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    VStart                : 1   ; // [0]
        unsigned int    Reserved1             : 15  ; // [15..1]
        unsigned int    Step                  : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_START;

// Define the union U_VEDU_MODE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vedsel                : 1   ; // [0]
        unsigned int    vencEn                : 1   ; // [1]                    //del     change to reserved
        unsigned int    osdEn                 : 1   ; // [2]
        unsigned int    stdmod                : 2   ; // [4..3]
        unsigned int    lowDlyMod             : 1   ; // [5]                    //del     change to reserved
        unsigned int    reserved2             : 1   ; // [6]
        unsigned int    reserved3             : 3   ; // [9..7]
        unsigned int    timeEn                : 2   ; // [11..10]
        unsigned int    accesslockEn          : 1   ; // [12]
        unsigned int    clkGateEn             : 2   ; // [14..13]
        unsigned int    memClkGateEn          : 1   ; // [15]
        unsigned int    strFmt                : 1   ; // [16]
        unsigned int    packageSel            : 8   ; // [24..17]
        unsigned int    regCfgMode            : 1   ; // [25]
        unsigned int    reserved4             : 4   ; // [29..26]
        unsigned int    bkpEn                 : 1   ; // [30]
        unsigned int    testMode              : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MODE;

// Define the union U_VEDU_PICFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ChnId                 : 8   ; // [7..0]
        unsigned int    ptbitsEn              : 1   ; // [8]
        unsigned int    cabacInitIdc          : 2   ; // [10..9]
        unsigned int    cabacByteStuff        : 1   ; // [11]
        unsigned int    ConstIntra            : 1   ; // [12]
        unsigned int    Ipicture              : 1   ; // [13]
        unsigned int    TransMode             : 2   ; // [15..14]
        unsigned int    NalRefIdc             : 2   ; // [17..16]
        unsigned int    numRefIndex           : 1   ; // [18]
        unsigned int    EntropyEncMode        : 1   ; // [19]
        unsigned int    dblkIdc               : 2   ; // [21..20]
        unsigned int    dblkAlpha             : 4   ; // [25..22]
        unsigned int    dblkBeta              : 4   ; // [29..26]
        unsigned int    reserved3             : 2   ; // [31..30]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG0;

// Define the union U_VEDU_PICFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    SliceSize             : 16  ; // [15..0]
        unsigned int    SliceSpiltMod         : 1   ; // [16]
        unsigned int    SliceSplit            : 1   ; // [17]
        unsigned int    Reserved_0            : 14  ; // [31..18]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG1;

// Define the union U_VEDU_PICFG2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    IPCMPredEn            : 1   ; // [0]
        unsigned int    Intra4x4PredEn        : 1   ; // [1]
        unsigned int    Intra8x8PredEn        : 1   ; // [2]
        unsigned int    Intra16x16PredEn      : 1   ; // [3]
        unsigned int    intra4ReducedModeEn   : 1   ; // [4]
        unsigned int    Reserved_0            : 26  ; // [30..5]
        unsigned int    IntraLowPowEn         : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG2;

// Define the union U_VEDU_PICFG3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Inter8x8PredEn        : 1   ; // [0]
        unsigned int    Inter8x16PredEn       : 1   ; // [1]
        unsigned int    Inter16x8PredEn       : 1   ; // [2]
        unsigned int    Inter16x16PredEn      : 1   ; // [3]
        unsigned int    pskEn                 : 1   ; // [4]
        unsigned int    Reserved_0            : 24  ; // [28..5]
        unsigned int    fracLowPowEn          : 1   ; // [29]
        unsigned int    intpLowPowEn          : 1   ; // [30]
        unsigned int    extedgeEn             : 1   ; // [31]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG3;

// Define the union U_VEDU_PICFG4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vcpi_clip_en          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG4;

// Define the union U_VEDU_PICFG5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    vcpi_luma_min         : 8   ; // [7..0]
        unsigned int    vcpi_luma_max         : 8   ; // [15..8]
        unsigned int    vcpi_chr_min          : 8   ; // [23..16]
        unsigned int    vcpi_chr_max          : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICFG5;

// Define the union U_VEDU_ENCUPDATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    iMBNum                : 14  ; // [13..0]
        unsigned int    startMb               : 18  ; // [31..14]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ENCUPDATE;

// Define the union U_VEDU_VEDIMGSIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ImgWidthInPixMinus1   : 13  ; // [12..0]
        unsigned int    Reserved2             : 3   ; // [15..13]
        unsigned int    ImgHeightInPixMinus1  : 13  ; // [28..16]
        unsigned int    Reserved1             : 3   ; // [31..29]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_VEDIMGSIZE;

// Define the union U_VEDU_SLCHDRPARA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    markistall            : 1   ; // [0]
        unsigned int    IDRind                : 1   ; // [1]
        unsigned int    Reserved2             : 6   ; // [7..2]
        unsigned int    ParaWd                : 7   ; // [14..8]
        unsigned int    Reserved1             : 1   ; // [15]
        unsigned int    ReorderWd             : 6   ; // [21..16]
        unsigned int    MarkingWd             : 6   ; // [27..22]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_SLCHDRPARA;

// Define the union U_VEDU_OUTSTD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    outstdNum             : 4   ; // [3..0]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_OUTSTD;

// Define the union U_VEDU_SSTRIDE
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

} U_VEDU_SSTRIDE;

// Define the union U_VEDU_RSTRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    RYStride              : 16  ; // [15..0]
        unsigned int    RCStride              : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_RSTRIDE;

// Define the union U_VEDU_STRMBUFLEN
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

} U_VEDU_STRMBUFLEN;

// Define the union U_VEDU_MECFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    HSWSize               : 4   ; // [3..0]
        unsigned int    VSWSize               : 3   ; // [6..4]
        unsigned int    reserved1             : 9   ; // [15..7]
        unsigned int    fracRealMvThr         : 4   ; // [19..16]
        unsigned int    Reserved_0            : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MECFG0;

// Define the union U_VEDU_MECFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Rect0Mod              : 1   ; // [0]
        unsigned int    Rect1Mod              : 1   ; // [1]
        unsigned int    Rect2Mod              : 1   ; // [2]
        unsigned int    Rect3Mod              : 1   ; // [3]
        unsigned int    Reserved_0            : 28  ; // [31..4]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MECFG1;

// Define the union U_VEDU_MERECTCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    RectWidth             : 8   ; // [7..0]
        unsigned int    RectHeight            : 7   ; // [14..8]
        unsigned int    reserved2             : 1   ; // [15]
        unsigned int    RectHstep             : 1   ; // [16]
        unsigned int    reserved1             : 7   ; // [23..17]
        unsigned int    RectVstep             : 1   ; // [24]
        unsigned int    Reserved_0            : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MERECTCFG;

// Define the union U_VEDU_METHR0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    SymcSTh1              : 16  ; // [15..0]
        unsigned int    SymcSTh2              : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_METHR0;

// Define the union U_VEDU_METHR1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    IntraThr              : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_METHR1;

// Define the union U_VEDU_MERDOCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    LmdaOff16             : 8   ; // [7..0]
        unsigned int    LmdaOff16x8           : 8   ; // [15..8]
        unsigned int    LmdaOff8x16           : 8   ; // [23..16]
        unsigned int    LmdaOff8              : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MERDOCFG;

// Define the union U_VEDU_CREFLDMODE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    crefldBur8En          : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_CREFLDMODE;

// Define the union U_VEDU_MDCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mdDelta               : 14  ; // [13..0]
        unsigned int    reserved1             : 2   ; // [15..14]
        unsigned int    rdoEn                 : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG;

// Define the union U_VEDU_MDCFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    itr8LmdOff            : 16  ; // [15..0]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG0;

// Define the union U_VEDU_MDCFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    itr4LmdOff            : 16  ; // [15..0]
        unsigned int    itr16LmdOff           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG1;

// Define the union U_VEDU_MDCFG2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    itr16LmdOff1          : 16  ; // [15..0]
        unsigned int    itr16LmdOff2          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG2;

// Define the union U_VEDU_MDCFG3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    itr16LmdOff3          : 16  ; // [15..0]
        unsigned int    itr16LmdOff4          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG3;

// Define the union U_VEDU_MDCFG4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    itr16LmdOff5          : 16  ; // [15..0]
        unsigned int    itr16LmdOff6          : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG4;

// Define the union U_VEDU_MDCFG5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mdStillThr1           : 16  ; // [15..0]
        unsigned int    mdStillThr2           : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG5;

// Define the union U_VEDU_MDCFG6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mdMadiThr1            : 16  ; // [15..0]
        unsigned int    mdMadiThr2            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MDCFG6;

// Define the union U_VEDU_MCTFCFG0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    MctfStillEn           : 1   ; // [0]
        unsigned int    MctfMovEn             : 1   ; // [1]
        unsigned int    Reserved_0            : 6   ; // [7..2]
        unsigned int    mctfStillMvThr        : 4   ; // [11..8]
        unsigned int    mctfRealMvThr         : 4   ; // [15..12]
        unsigned int    MctfStillCostThr      : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MCTFCFG0;

// Define the union U_VEDU_MCTFCFG1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    MctfLog2mctf          : 2   ; // [1..0]
        unsigned int    reserved1             : 6   ; // [7..2]
        unsigned int    MctfLumaDiffThr       : 8   ; // [15..8]
        unsigned int    MctfChrDiffThr        : 8   ; // [23..16]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MCTFCFG1;

// Define the union U_VEDU_MCTFCFG2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mctfChrDeltaThr       : 8   ; // [7..0]
        unsigned int    mctfStiMadiThr1       : 8   ; // [15..8]
        unsigned int    mctfStiMadiThr2       : 8   ; // [23..16]
        unsigned int    mctfMovMadiThr        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MCTFCFG2;

// Define the union U_VEDU_MCTFCFG3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mctfMovMad1_m         : 4   ; // [3..0]
        unsigned int    mctfMovMad1_n         : 4   ; // [7..4]
        unsigned int    mctfMovMad2_m         : 4   ; // [11..8]
        unsigned int    mctfMovMad2_n         : 4   ; // [15..12]
        unsigned int    mctfStiMad1_m         : 4   ; // [19..16]
        unsigned int    mctfStiMad1_n         : 4   ; // [23..20]
        unsigned int    mctfStiMad2_m         : 4   ; // [27..24]
        unsigned int    mctfStiMad2_n         : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MCTFCFG3;

// Define the union U_VEDU_ROICFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    region0En             : 1   ; // [0]
        unsigned int    region1En             : 1   ; // [1]
        unsigned int    region2En             : 1   ; // [2]
        unsigned int    region3En             : 1   ; // [3]
        unsigned int    region4En             : 1   ; // [4]
        unsigned int    region5En             : 1   ; // [5]
        unsigned int    region6En             : 1   ; // [6]
        unsigned int    region7En             : 1   ; // [7]
        unsigned int    bAbsQp0               : 1   ; // [8]
        unsigned int    bAbsQp1               : 1   ; // [9]
        unsigned int    bAbsQp2               : 1   ; // [10]
        unsigned int    bAbsQp3               : 1   ; // [11]
        unsigned int    bAbsQp4               : 1   ; // [12]
        unsigned int    bAbsQp5               : 1   ; // [13]
        unsigned int    bAbsQp6               : 1   ; // [14]
        unsigned int    bAbsQp7               : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ROICFG;

// Define the union U_VEDU_ROIQP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ROIQp0                : 7   ; // [6..0]
        unsigned int    ROIQp1                : 7   ; // [13..7]
        unsigned int    ROIQp2                : 7   ; // [20..14]
        unsigned int    ROIQp3                : 7   ; // [27..21]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ROIQP0;

// Define the union U_VEDU_ROIQP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ROIQp4                : 7   ; // [6..0]
        unsigned int    ROIQp5                : 7   ; // [13..7]
        unsigned int    ROIQp6                : 7   ; // [20..14]
        unsigned int    ROIQp7                : 7   ; // [27..21]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ROIQP1;

// Define the union U_VEDU_ROISIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ROIWidth              : 10  ; // [9..0]
        unsigned int    ROIHeight             : 10  ; // [19..10]
        unsigned int    Reserved_0            : 12  ; // [31..20]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ROISIZE;

// Define the union U_VEDU_ROIPOS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ROIstartX             : 9   ; // [8..0]
        unsigned int    ROIstartY             : 9   ; // [17..9]
        unsigned int    Reserved_0            : 14  ; // [31..18]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ROIPOS;

// Define the union U_VEDU_QPTHR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    minQp                 : 6   ; // [5..0]
        unsigned int    reserved0             : 2   ; // [7..6]
        unsigned int    maxQp                 : 6   ; // [13..8]
        unsigned int    reserved1             : 2   ; // [15..14]
        unsigned int    startQp               : 6   ; // [21..16]
        unsigned int    chromaQpOff           : 5   ; // [26..22]
        unsigned int    secondChromaQpOff     : 5   ; // [31..27]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_QPTHR;

// Define the union U_VEDU_RC1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rcQpDelta             : 6   ; // [5..0]
        unsigned int    reserved0             : 2   ; // [7..6]
        unsigned int    rcMadpDelta           : 8   ; // [15..8]
        unsigned int    reserved1             : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_RC1;

// Define the union U_VEDU_QPDELTATHR0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rcQpDeltaThr0         : 8   ; // [7..0]
        unsigned int    rcQpDeltaThr1         : 8   ; // [15..8]
        unsigned int    rcQpDeltaThr2         : 8   ; // [23..16]
        unsigned int    rcQpDeltaThr3         : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_QPDELTATHR0;

// Define the union U_VEDU_QPDELTATHR1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rcQpDeltaThr4         : 8   ; // [7..0]
        unsigned int    rcQpDeltaThr5         : 8   ; // [15..8]
        unsigned int    rcQpDeltaThr6         : 8   ; // [23..16]
        unsigned int    rcQpDeltaThr7         : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_QPDELTATHR1;

// Define the union U_VEDU_QPDELTATHR2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rcQpDeltaThr8         : 8   ; // [7..0]
        unsigned int    rcQpDeltaThr9         : 8   ; // [15..8]
        unsigned int    rcQpDeltaThr10        : 8   ; // [23..16]
        unsigned int    rcQpDeltaThr11        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_QPDELTATHR2;

// Define the union U_VEDU_MEANQP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    meanQp                : 6   ; // [5..0]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MEANQP;

// Define the union U_VEDU_OSDQP_CFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    reserved1             : 8   ; // [7..0]
        unsigned int    bAbsQp0               : 1   ; // [8]
        unsigned int    bAbsQp1               : 1   ; // [9]
        unsigned int    bAbsQp2               : 1   ; // [10]
        unsigned int    bAbsQp3               : 1   ; // [11]
        unsigned int    bAbsQp4               : 1   ; // [12]
        unsigned int    bAbsQp5               : 1   ; // [13]
        unsigned int    bAbsQp6               : 1   ; // [14]
        unsigned int    bAbsQp7               : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_OSDQP_CFG;

// Define the union U_VEDU_OSD_QP0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    OSDQp0                : 7   ; // [6..0]
        unsigned int    OSDQp1                : 7   ; // [13..7]
        unsigned int    OSDQp2                : 7   ; // [20..14]
        unsigned int    OSDQp3                : 7   ; // [27..21]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_OSD_QP0;

// Define the union U_VEDU_OSD_QP1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    OSDQp4                : 7   ; // [6..0]
        unsigned int    OSDQp5                : 7   ; // [13..7]
        unsigned int    OSDQp6                : 7   ; // [20..14]
        unsigned int    OSDQp7                : 7   ; // [27..21]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_OSD_QP1;

// Define the union U_VEDU_MODLAMBDA
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    ModLambda0            : 12  ; // [11..0]
        unsigned int    ModLambda1            : 12  ; // [23..12]
        unsigned int    Reserved_0            : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MODLAMBDA;

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

// Define the union U_VEDU_MBBP
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    BreakpMbx             : 9   ; // [8..0]
        unsigned int    reserved1             : 7   ; // [15..9]
        unsigned int    BreakpMby             : 9   ; // [24..16]
        unsigned int    reserved0             : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MBBP;

// Define the union U_VEDU_MBPARA0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    CBP                   : 6   ; // [5..0]
        unsigned int    QP                    : 6   ; // [11..6]
        unsigned int    PredMod               : 4   ; // [15..12]
        unsigned int    dbgstgRunning         : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MBPARA0;

// Define the union U_VEDU_MBPARA1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mbx                   : 9   ; // [8..0]
        unsigned int    reserved1             : 7   ; // [15..9]
        unsigned int    mby                   : 9   ; // [24..16]
        unsigned int    reserved0             : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MBPARA1;

// Define the union U_VEDU_PICINFO0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    pskipMbNum            : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO0;

// Define the union U_VEDU_PICINFO1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    IPCMMbNum             : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO1;

// Define the union U_VEDU_PICINFO2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    inter16x16MbNum       : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO2;

// Define the union U_VEDU_PICINFO3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    inter16x8MbNum        : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO3;

// Define the union U_VEDU_PICINFO4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    inter8x16MbNum        : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO4;

// Define the union U_VEDU_PICINFO5
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    inter8x8MbNum         : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO5;

// Define the union U_VEDU_PICINFO6
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Intra16MbNum          : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO6;

// Define the union U_VEDU_PICINFO7
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Intra8MbNum           : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO7;

// Define the union U_VEDU_PICINFO8
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    Intra4MbNum           : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO8;

// Define the union U_VEDU_PICINFO9
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    multimvMbNum          : 19  ; // [18..0]
        unsigned int    Reserved_0            : 13  ; // [31..19]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_PICINFO9;

// Define the union U_VEDU_MV
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mvx0                  : 14  ; // [13..0]
        unsigned int    reserved1             : 2   ; // [15..14]
        unsigned int    mvy0                  : 12  ; // [27..16]
        unsigned int    Reserved_0            : 4   ; // [31..28]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MV;

// Define the union U_VEDU_VDERR_STATE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    h4BlkErrSta           : 3   ; // [2..0]
        unsigned int    h4PrsErrSta           : 3   ; // [5..3]
        unsigned int    h4Err                 : 2   ; // [7..6]
        unsigned int    SedStrmErr            : 1   ; // [8]
        unsigned int    Reserved_0            : 23  ; // [31..9]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_VDERR_STATE;

// Define the union U_VEDU_MVD
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    mvdx0                 : 10  ; // [9..0]
        unsigned int    reserved1             : 6   ; // [15..10]
        unsigned int    mvdy0                 : 9   ; // [24..16]
        unsigned int    Reserved_0            : 7   ; // [31..25]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_MVD;

// Define the union U_VEDU_AXITSTART
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axi_start             : 1   ; // [0]
        unsigned int    Reserved_0            : 31  ; // [31..1]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITSTART;

// Define the union U_VEDU_AXITCFG
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axit_cmd_num          : 5   ; // [4..0]
        unsigned int    axitEn                : 1   ; // [5]
        unsigned int    Reserved_0            : 26  ; // [31..6]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITCFG;

// Define the union U_VEDU_AXITWR
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitWr0               : 1   ; // [0]
        unsigned int    axitWr1               : 1   ; // [1]
        unsigned int    axitWr2               : 1   ; // [2]
        unsigned int    axitWr3               : 1   ; // [3]
        unsigned int    axitWr4               : 1   ; // [4]
        unsigned int    axitWr5               : 1   ; // [5]
        unsigned int    axitWr6               : 1   ; // [6]
        unsigned int    axitWr7               : 1   ; // [7]
        unsigned int    axitWr8               : 1   ; // [8]
        unsigned int    axitWr9               : 1   ; // [9]
        unsigned int    axitWr10              : 1   ; // [10]
        unsigned int    axitWr11              : 1   ; // [11]
        unsigned int    axitWr12              : 1   ; // [12]
        unsigned int    axitWr13              : 1   ; // [13]
        unsigned int    axitWr14              : 1   ; // [14]
        unsigned int    axitWr15              : 1   ; // [15]
        unsigned int    Reserved_0            : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITWR;

// Define the union U_VEDU_AXITSTRIDE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitCmdStride         : 16  ; // [15..0]
        unsigned int    axitBlkStride         : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITSTRIDE;

// Define the union U_VEDU_AXITBLKSIZE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBlkW              : 5   ; // [4..0]
        unsigned int    axitBlkH              : 5   ; // [9..5]
        unsigned int    Reserved_0            : 22  ; // [31..10]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITBLKSIZE;

// Define the union U_VEDU_AXITSTUTAS
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitEnd               : 1   ; // [0]
        unsigned int    axitChkFlg            : 1   ; // [1]
        unsigned int    Reserved_0            : 30  ; // [31..2]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITSTUTAS;

// Define the union U_VEDU_AXITWDATA0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseWdata0        : 8   ; // [7..0]
        unsigned int    axitBaseWdata1        : 8   ; // [15..8]
        unsigned int    axitBaseWdata2        : 8   ; // [23..16]
        unsigned int    axitBaseWdata3        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITWDATA0;

// Define the union U_VEDU_AXITWDATA1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseWdata4        : 8   ; // [7..0]
        unsigned int    axitBaseWdata5        : 8   ; // [15..8]
        unsigned int    axitBaseWdata6        : 8   ; // [23..16]
        unsigned int    axitBaseWdata7        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITWDATA1;

// Define the union U_VEDU_AXITWDATA2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseWdata8        : 8   ; // [7..0]
        unsigned int    axitBaseWdata9        : 8   ; // [15..8]
        unsigned int    axitBaseWdata10       : 8   ; // [23..16]
        unsigned int    axitBaseWdata11       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITWDATA2;

// Define the union U_VEDU_AXITWDATA3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseWdata12       : 8   ; // [7..0]
        unsigned int    axitBaseWdata13       : 8   ; // [15..8]
        unsigned int    axitBaseWdata14       : 8   ; // [23..16]
        unsigned int    axitBaseWdata15       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITWDATA3;

// Define the union U_VEDU_AXITRDATA0
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseRdata0        : 8   ; // [7..0]
        unsigned int    axitBaseRdata1        : 8   ; // [15..8]
        unsigned int    axitBaseRdata2        : 8   ; // [23..16]
        unsigned int    axitBaseRdata3        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITRDATA0;

// Define the union U_VEDU_AXITRDATA1
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseRdata4        : 8   ; // [7..0]
        unsigned int    axitBaseRdata5        : 8   ; // [15..8]
        unsigned int    axitBaseRdata6        : 8   ; // [23..16]
        unsigned int    axitBaseRdata7        : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITRDATA1;

// Define the union U_VEDU_AXITRDATA2
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseRdata8        : 8   ; // [7..0]
        unsigned int    axitBaseRdata9        : 8   ; // [15..8]
        unsigned int    axitBaseRdata10       : 8   ; // [23..16]
        unsigned int    axitBaseRdata11       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITRDATA2;

// Define the union U_VEDU_AXITRDATA3
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    axitBaseRdata12       : 8   ; // [7..0]
        unsigned int    axitBaseRdata13       : 8   ; // [15..8]
        unsigned int    axitBaseRdata14       : 8   ; // [23..16]
        unsigned int    axitBaseRdata15       : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_AXITRDATA3;

// Define the union U_VEDU_ACLEVEL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    level0                : 16  ; // [15..0]
        unsigned int    level1                : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_ACLEVEL;

// Define the union U_VEDU_DCLEVEL
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    level0                : 16  ; // [15..0]
        unsigned int    level1                : 16  ; // [31..16]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_DCLEVEL;

// Define the union U_VEDU_SCALE
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scale0                : 8   ; // [7..0]
        unsigned int    scale1                : 8   ; // [15..8]
        unsigned int    scale2                : 8   ; // [23..16]
        unsigned int    scale3                : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_SCALE;

// Define the union U_VEDU_SCALE4
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    scale0                : 8   ; // [7..0]
        unsigned int    scale1                : 8   ; // [15..8]
        unsigned int    scale2                : 8   ; // [23..16]
        unsigned int    scale3                : 8   ; // [31..24]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_VEDU_SCALE4;

// Define the union U_AXI_AWDLY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    awDelay               : 16  ; // [15..0]
        unsigned int    awDelayMode           : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AXI_AWDLY;

// Define the union U_AXI_ARDLY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    arDelay               : 16  ; // [15..0]
        unsigned int    arDelayMode           : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AXI_ARDLY;

// Define the union U_AXI_RDLY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    rDelay                : 16  ; // [15..0]
        unsigned int    rDelayMode            : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AXI_RDLY;

// Define the union U_AXI_WDLY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    wDelay                : 16  ; // [15..0]
        unsigned int    wDelayMode            : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AXI_WDLY;

// Define the union U_AXI_BDLY
typedef union
{
    // Define the struct bits
    struct
    {
        unsigned int    bDelay                : 16  ; // [15..0]
        unsigned int    bDelayMode            : 1   ; // [16]
        unsigned int    Reserved_0            : 15  ; // [31..17]
    } bits;

    // Define an unsigned member
    unsigned int    u32;

} U_AXI_BDLY;

//==============================================================================
// Define the global struct
typedef struct
{
    volatile U_VEDU_INTSTAT         VEDU_INTSTAT;
    volatile U_VEDU_INTMASK         VEDU_INTMASK;
    volatile U_VEDU_RAWINT          VEDU_RAWINT;
    volatile U_VEDU_INTCLR          VEDU_INTCLR;
    volatile U_VEDU_START           VEDU_START;
    volatile unsigned int           RESERVED0[3];
    volatile U_VEDU_MODE            VEDU_MODE;
    volatile U_VEDU_PICFG0          VEDU_PICFG0;
    volatile U_VEDU_PICFG1          VEDU_PICFG1;
    volatile U_VEDU_PICFG2          VEDU_PICFG2;
    volatile U_VEDU_PICFG3          VEDU_PICFG3;
    volatile U_VEDU_PICFG4          VEDU_PICFG4;
    volatile U_VEDU_PICFG5          VEDU_PICFG5;
    volatile unsigned int           VEDU_CURPOC;
    volatile U_VEDU_ENCUPDATE       VEDU_ENCUPDATE;
    volatile unsigned int           RESERVED2[3];
    volatile U_VEDU_VEDIMGSIZE      VEDU_VEDIMGSIZE;
    volatile unsigned int           RESERVED3;
    volatile unsigned int           VEDU_PTBITS;
    volatile unsigned int           RESERVED4;
    volatile unsigned int           VEDU_SLCHDRSTRM0;
    volatile unsigned int           VEDU_SLCHDRSTRM1;
    volatile unsigned int           VEDU_SLCHDRSTRM2;
    volatile unsigned int           VEDU_SLCHDRSTRM3;
    volatile unsigned int           VEDU_SLCHDRSTRM4;
    volatile unsigned int           VEDU_SLCHDRSTRM5;
    volatile unsigned int           VEDU_SLCHDRSTRM6;
    volatile unsigned int           VEDU_SLCHDRSTRM7;
    volatile U_VEDU_SLCHDRPARA      VEDU_SLCHDRPARA;
    volatile unsigned int           RESERVED5[3];
    volatile unsigned int           VEDU_PTS0;
    volatile unsigned int           VEDU_PTS1;
    volatile unsigned int           VEDU_PTS2;
    volatile unsigned int           VEDU_PTS3;
    volatile unsigned int           VEDU_TIMEOUT;
    volatile U_VEDU_OUTSTD          VEDU_OUTSTD;
    volatile unsigned int           VEDU_TIMER;
    volatile unsigned int           VEDU_IDLE_TIMER;
    volatile unsigned int           RESERVED6[18];
    volatile unsigned int           VEDU_VERSION0;
    volatile unsigned int           VEDU_VERSION1;
    volatile unsigned int           VEDU_SRCYADDR;
    volatile unsigned int           VEDU_SRCCADDR;
    volatile unsigned int           RESERVED7[2];
    volatile unsigned int           VEDU_RCNYADDR;
    volatile unsigned int           VEDU_RCNCADDR;
    volatile unsigned int           RESERVED8[2];
    volatile unsigned int           VEDU_REFYADDR;
    volatile unsigned int           VEDU_REFCADDR;
    volatile unsigned int           RESERVED9[2];
    volatile U_VEDU_SSTRIDE         VEDU_SSTRIDE;
    volatile U_VEDU_RSTRIDE         VEDU_RSTRIDE;
    volatile unsigned int           RESERVED10[2];
    volatile unsigned int           VEDU_STRMADDR;
    volatile unsigned int           VEDU_SRPTRADDR;
    volatile unsigned int           VEDU_SWPTRADDR;
    volatile U_VEDU_STRMBUFLEN      VEDU_STRMBUFLEN;
    volatile unsigned int           VEDU_TNBUFADDR;
    volatile unsigned int           VEDU_IPIXBUFADDR;
    volatile unsigned int           VEDU_DOWNMSG_ADDR;
    volatile unsigned int           RESERVED10_1;
    volatile unsigned int           VEDU_VDREF_YADDR[16];
    volatile unsigned int           VEDU_VDREF_CADDR[16];
    volatile unsigned int           VEDU_REGCFG_ADDR;
    volatile unsigned int           RESERVED11[7];
    volatile U_VEDU_MECFG0          VEDU_MECFG0;
    volatile U_VEDU_MECFG1          VEDU_MECFG1;
    volatile U_VEDU_MERECTCFG       VEDU_MERECTCFG[4];
    volatile unsigned int           RESERVED11_2[2];
    volatile U_VEDU_METHR0          VEDU_METHR0;
    volatile U_VEDU_METHR1          VEDU_METHR1;
    volatile U_VEDU_MERDOCFG        VEDU_MERDOCFG;
    volatile U_VEDU_CREFLDMODE      VEDU_CREFLDMODE;
    volatile U_VEDU_MDCFG           VEDU_MDCFG;
    volatile unsigned int           RESERVED13[2];
    volatile U_VEDU_MDCFG0          VEDU_MDCFG0;
    volatile U_VEDU_MDCFG1          VEDU_MDCFG1;
    volatile U_VEDU_MDCFG2          VEDU_MDCFG2;
    volatile U_VEDU_MDCFG3          VEDU_MDCFG3;
    volatile U_VEDU_MDCFG4          VEDU_MDCFG4;
    volatile U_VEDU_MDCFG5          VEDU_MDCFG5;
    volatile U_VEDU_MDCFG6          VEDU_MDCFG6;
    volatile unsigned int           RESERVED13_1[30];
    volatile U_VEDU_MCTFCFG0        VEDU_MCTFCFG0;
    volatile U_VEDU_MCTFCFG1        VEDU_MCTFCFG1;
    volatile U_VEDU_MCTFCFG2        VEDU_MCTFCFG2;
    volatile U_VEDU_MCTFCFG3        VEDU_MCTFCFG3;
    volatile unsigned int           RESERVED14[8];
    volatile U_VEDU_ROICFG          VEDU_ROICFG;
    volatile U_VEDU_ROIQP0          VEDU_ROIQP0;
    volatile U_VEDU_ROIQP1          VEDU_ROIQP1;
    volatile U_VEDU_ROISIZE         VEDU_ROISIZE[8];
    volatile U_VEDU_ROIPOS          VEDU_ROIPOS[8];
    volatile unsigned int           RESERVED15;
    volatile U_VEDU_QPTHR           VEDU_QPTHR;
    volatile U_VEDU_RC1             VEDU_RC1;
    volatile U_VEDU_QPDELTATHR0     VEDU_QPDELTATHR0;
    volatile U_VEDU_QPDELTATHR1     VEDU_QPDELTATHR1;
    volatile U_VEDU_QPDELTATHR2     VEDU_QPDELTATHR2;
    volatile unsigned int           VEDU_TARGETBITS;
    volatile unsigned int           RESERVED16[3];
    volatile unsigned int           VEDU_PICBITS;
    volatile unsigned int           VEDU_TOTALMBHBITS;
    volatile unsigned int           VEDU_TOTALTXTBITS;
    volatile unsigned int           VEDU_TOTALCOST0;
    volatile unsigned int           VEDU_TOTALCOST1;
    volatile U_VEDU_MEANQP          VEDU_MEANQP;
    volatile unsigned int           RESERVED17;
    volatile U_VEDU_OSDQP_CFG       VEDU_OSDQP_CFG;
    volatile U_VEDU_OSD_QP0         VEDU_OSD_QP0;
    volatile U_VEDU_OSD_QP1         VEDU_OSD_QP1;
    volatile unsigned int           RESERVED18;
    volatile U_VEDU_MODLAMBDA       VEDU_MODLAMBDA[20];
    volatile unsigned int           RESERVED19[68];
    volatile U_VPP_OSD_CFG          VPP_OSD_CFG;
    volatile U_VPP_OSD01_ALPHA      VPP_OSD01_ALPHA;
    volatile U_VPP_OSD23_ALPHA      VPP_OSD23_ALPHA;
    volatile U_VPP_OSD45_ALPHA      VPP_OSD45_ALPHA;
    volatile U_VPP_OSD67_ALPHA      VPP_OSD67_ALPHA;
    volatile U_VPP_OSD_ALPHA0123    VPP_OSD_ALPHA0123;
    volatile U_VPP_OSD_ALPHA4567    VPP_OSD_ALPHA4567;
    volatile unsigned int           RESERVED20;
    volatile U_VPP_OSD_POS          VPP_OSD_POS[8];
    volatile U_VPP_OSD_SIZE         VPP_OSD_SIZE[8];
    volatile unsigned int           VPP_OSD_ADDR[8];
    volatile U_VPP_OSD_STRIDE       VPP_OSD_STRIDE[8];
    volatile U_VPP_OSD_LAYERID      VPP_OSD_LAYERID;
    volatile unsigned int           RESERVED21[3];
    volatile U_VPP_OSD_CINVCFG      VPP_OSD_CINVCFG;
    volatile unsigned int           RESERVED21_1[19];
    volatile unsigned int           RESERVED22[256];
    volatile unsigned int           VEDU_FRAMENO;
    volatile U_VEDU_MBBP            VEDU_MBBP;
    volatile U_VEDU_MBPARA0         VEDU_MBPARA0;
    volatile U_VEDU_MBPARA1         VEDU_MBPARA1;
    volatile unsigned int           RESERVED23[4];
    volatile U_VEDU_PICINFO0        VEDU_PICINFO0;
    volatile U_VEDU_PICINFO1        VEDU_PICINFO1;
    volatile U_VEDU_PICINFO2        VEDU_PICINFO2;
    volatile U_VEDU_PICINFO3        VEDU_PICINFO3;
    volatile U_VEDU_PICINFO4        VEDU_PICINFO4;
    volatile U_VEDU_PICINFO5        VEDU_PICINFO5;
    volatile U_VEDU_PICINFO6        VEDU_PICINFO6;
    volatile U_VEDU_PICINFO7        VEDU_PICINFO7;
    volatile U_VEDU_PICINFO8        VEDU_PICINFO8;
    volatile U_VEDU_PICINFO9        VEDU_PICINFO9;
    volatile unsigned int           VEDU_PICINFO10;
    volatile unsigned int           RESERVED24;
    volatile unsigned int           VEDU_VCTRL_STATE[10];
    volatile unsigned int           VEDU_CREFLD_STATE[3];
    volatile unsigned int           RESERVED25[3];
    volatile unsigned int           VEDU_LDSTR_STATE[4];
    volatile unsigned int           VEDU_INTP_STATE[6];
    volatile unsigned int           RESERVED26[2];
    volatile unsigned int           VEDU_VLC_STATE;
    volatile unsigned int           RESERVED28[3];
    volatile unsigned int           VEDU_PLINK_STATE;
    volatile unsigned int           RESERVED29[3];
    volatile unsigned int           VEDU_INTRA_STATE;
    volatile unsigned int           RESERVED30[3];
    volatile unsigned int           VEDU_END_STATE;
    volatile unsigned int           VEDU_SED_STATE0;
    volatile unsigned int           VEDU_SED_STATE1;
    volatile unsigned int           RESERVED31;
    volatile U_VEDU_MV              VEDU_MV[4];
    volatile unsigned int           RESERVED32[4];
    volatile U_VEDU_VDERR_STATE     VEDU_VDERR_STATE;
    volatile unsigned int           VEDU_SED_PFMC;
    volatile unsigned int           RESERVED32_1[2];
    volatile unsigned int           VEDU_LOWPOW_CNT0;
    volatile unsigned int           VEDU_LOWPOW_CNT1;
    volatile unsigned int           VEDU_LOWPOW_CNT2;
    volatile unsigned int           RESERVED32_2;
    volatile U_VEDU_MVD             VEDU_MVD[4];
    volatile unsigned int           VEDU_WRPTR;
    volatile unsigned int           VEDU_RDPTR;
    volatile unsigned int           VEDU_CREFLD_MISSNUM;
    volatile unsigned int           VEDU_CREFLD_HITNUM;
    volatile unsigned int           RESERVED33[40];
    volatile U_VEDU_AXITSTART       VEDU_AXITSTART;
    volatile U_VEDU_AXITCFG         VEDU_AXITCFG;
    volatile U_VEDU_AXITWR          VEDU_AXITWR;
    volatile unsigned int           VEDU_AXITSTARTADDR;
    volatile U_VEDU_AXITSTRIDE      VEDU_AXITSTRIDE;
    volatile U_VEDU_AXITBLKSIZE     VEDU_AXITBLKSIZE[16];
    volatile U_VEDU_AXITSTUTAS      VEDU_AXITSTUTAS;
    volatile unsigned int           VEDU_AXITLOOPNUM;
    volatile unsigned int           VEDU_AXITINTERVAL;
    volatile U_VEDU_AXITWDATA0      VEDU_AXITWDATA0;
    volatile U_VEDU_AXITWDATA1      VEDU_AXITWDATA1;
    volatile U_VEDU_AXITWDATA2      VEDU_AXITWDATA2;
    volatile U_VEDU_AXITWDATA3      VEDU_AXITWDATA3;
    volatile U_VEDU_AXITRDATA0      VEDU_AXITRDATA0;
    volatile U_VEDU_AXITRDATA1      VEDU_AXITRDATA1;
    volatile U_VEDU_AXITRDATA2      VEDU_AXITRDATA2;
    volatile U_VEDU_AXITRDATA3      VEDU_AXITRDATA3;
    volatile unsigned int           RESERVED34[3296];
    volatile unsigned int           VEDU_FRACPDATA[64];
    volatile unsigned int           VEDU_PSKPDATA[64];
    volatile unsigned int           VEDU_CFRACPDATA[32];
    volatile unsigned int           VEDU_CPSKPDATA[32];
    volatile unsigned int           VEDU_ITRPDATA[160];
    volatile unsigned int           RESERVED35[32];
    volatile unsigned int           VEDU_ITR8PDATA[64];
    volatile U_VEDU_ACLEVEL         VEDU_ACLEVEL[192];
    volatile U_VEDU_DCLEVEL         VEDU_DCLEVEL[12];
    volatile unsigned int           RESERVED36[52];
    volatile unsigned int           VEDU_RCNDATA[96];
    volatile unsigned int           RESERVED37[32];
    volatile unsigned int           VEDU_CURDATA[96];
    volatile unsigned int           RESERVED38[992];
    volatile U_VEDU_SCALE           VEDU_SCALE[32];
    volatile U_VEDU_SCALE4          VEDU_SCALE4[24];
    volatile unsigned int           RESERVED39[10248];
    volatile U_AXI_AWDLY            AXI_AWDLY;
    volatile U_AXI_ARDLY            AXI_ARDLY;
    volatile U_AXI_RDLY             AXI_RDLY;
    volatile U_AXI_WDLY             AXI_WDLY;
    volatile U_AXI_BDLY             AXI_BDLY;
} S_VEDU_REGS_TYPE;

// Declare the struct pointor of the module VEDU
extern volatile S_VEDU_REGS_TYPE* gopVEDUAllReg;



//#endif                                   //del

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif
