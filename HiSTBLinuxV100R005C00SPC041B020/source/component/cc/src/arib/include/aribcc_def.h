
/******************************************************************************

Copyright (C), 2012-2014, HiSilicon Technologies Co., Ltd.
******************************************************************************
File Name     :
Version       : Initial draft
Author        :
Created Date   :
Last Modified by:
Description   :
Function List :
Change History:
******************************************************************************/

#ifndef _ARIB_CC_DEF_H_
#define _ARIB_CC_DEF_H_

#include "hi_unf_cc.h"

#if defined(__cplusplus) || defined(__CPLUSPLUS__)
extern "c"
{
#endif


#define  GET_PADDING_SIZE(l)  ((((HI_U32)(l))%4)?(4-(((HI_U32)(l))%4)):0)

/* private PES */
#define KB                      0x400                           //  1 Kbyte =         1,024 byte
#define MB                      0x100000                        //  1 MByte =     1,048,576 byte
#define CCPES_MAX_SIZE          (32*KB)                         // 32 Kbyte =        32,768 byte
#define CCPES_BUFFER_SIZE       (CCPES_MAX_SIZE*2)              // 64 Kbyte =        65,536 byte
#define CCPES_TYPE_MAX          2
#define ARIBCC_PESDATA_BLOCK    128

/* data_group_id (Data group identification; DGI) */
#define DGI_TYPE_MANAGE         0x00            // 0x00: Caption management
#define DGI_TYPE_1ST_LANG       0x01            // 0x01: Caption statement (1st language)
#define DGI_TYPE_2ND_LANG       0x02            // 0x02: Caption statement (2nd language)
#define DGI_TYPE_3RD_LANG       0x03            // 0x03: Caption statement (3rd language)
#define DGI_TYPE_4TH_LANG       0x04            // 0x04: Caption statement (4rd language)
#define DGI_TYPE_5TH_LANG       0x05            // 0x05: Caption statement (5rd language)
#define DGI_TYPE_6TH_LANG       0x06            // 0x06: Caption statement (6rd language)
#define DGI_TYPE_7TH_LANG       0x07            // 0x07: Caption statement (7rd language)
#define DGI_TYPE_8TH_LANG       0x08            // 0x08: Caption statement (8rd language)
#define DGI_GROUP_A             0x00            // 0x00: Group A
#define DGI_GROUP_B             0x20            // 0x20: Group B

/* number of languages */
#define CC_LANG_MAX             2

/* display format */
#define DF_HORI_SD              0x0  /*Horizontal writing in standard density*/
#define DF_VERT_SD              0x1  /*Vertical writing in standard density*/
#define DF_HORI_HD              0x2  /*Horizontal writing in high density*/
#define DF_VERT_HD              0x3  /*Vertical writing in high density*/
#define DF_HORI_WEST_LANG       0x4  /*Horizontal writing of Western language*/
#define DF_HORI_1920X1080       0x6  /*Horizontal writing in 1920 x 1080*/
#define DF_VERT_1920X1080       0x7  /*Vertical writing in 1920 x 1080*/
#define DF_HORI_960X540         0x8  /*Horizontal writing in 960 x 540*/
#define DF_VERT_960X540         0x9  /*Vertical writing in 960 x 540*/
#define DF_HORI_720X480         0xA  /*Horizontal writing in 720 x 480*/
#define DF_VERT_720X480         0xB  /*Vertical writing in 720 x 480*/
#define DF_HORI_1280X720        0xC  /*Horizontal writing in 1280 x 720*/
#define DF_VERT_1280X720        0xD  /*Vertical writing in 1280 x 720*/

/*
TCS (Character coding): This 2-bit field indicates the type of character coding. Character coding
is listed in Table 9-8
*/
#define TCS_8UNIT               0x0
#define TCS_UCS                 0x1

/*
rollup_mode: This 2-bit field indicates whether the caption data is transmitted in the roll-up
mode or not. The applicable values are shown in Table 9-9.
*/
#define RM_NON_ROLLUP           0x0
#define RM_ROLLUP               0x1

/*
data_unit_data_byte (Data unit data): Data unit data to be transmitted.
Assignment of data unit to data group is listed in Table 9-13.
*/
#define DATA_UNIT_STATE_BODY    0x20
#define DATA_UNIT_GEOMETRIC     0x28
#define DATA_UNIT_ADD_SOUND     0x2C
#define DATA_UNIT_1BYTE_DRCS    0x30
#define DATA_UNIT_2BYTE_DRCS    0x31
#define DATA_UNIT_COLORMAP      0x34
#define DATA_UNIT_BITMAP        0x35

/* code element */
typedef enum    tagARIBCC_CodeElement_E
{
    ARIB_CELEMENT_G0    = 0x28,
    ARIB_CELEMENT_G1,
    ARIB_CELEMENT_G2,
    ARIB_CELEMENT_G3
} ARIBCC_CodeElement_E;

/* graphic set for AribCC */
/* Ref : Table 7-2 Designation of graphic sets Table, 7-3 Classification of code set and Final Byte */
typedef enum    tagARIBCC_GraphSet_E
{
    // 1-byte code (0x0000¡Á0x00FF)
    ARIB_GSET_HIRAGANA                  = 0x0030,
    ARIB_GSET_KATAKANA                  = 0x0031,
    ARIB_GSET_MOSAIC_A                  = 0x0032,
    ARIB_GSET_MOSAIC_B                  = 0x0033,
    ARIB_GSET_MOSAIC_C                  = 0x0034,
    ARIB_GSET_MOSAIC_D                  = 0x0035,
    ARIB_GSET_P_ALPHANUMERIC            = 0x0036,
    ARIB_GSET_P_HIRAGANA                = 0x0037,
    ARIB_GSET_P_KATAKANA                = 0x0038,
    ARIB_GSET_JIS_X0201_KATAKANA        = 0x0049,
    ARIB_GSET_ALPHANUMERIC              = 0x004A,
    ARIB_GSET_LATIN_EXTENSION           = 0x004B,
    ARIB_GSET_SPECIAL                   = 0x004C,

    // 2-byete code (0x0100¡Á0x01FF)
    ARIB_GSET_KANJI_2                   = 0x0139,
    ARIB_GSET_JIS_INTERCHANGE_KANJI_1   = ARIB_GSET_KANJI_2,
    ARIB_GSET_KANJI_3                   = 0x013A,
    ARIB_GSET_JIS_INTERCHANGE_KANJI_2   = ARIB_GSET_KANJI_3,
    ARIB_GSET_KANJI_4                   = 0x013B,
    ARIB_GSET_ADDITION_SIGN             = ARIB_GSET_KANJI_4,
    ARIB_GSET_KANJI                     = 0x0142,

    // 1-byte code DRCS (0x0240¡Á0x02FF)
    ARIB_GSET_DRCS_1                    = 0x0241,
    ARIB_GSET_DRCS_2                    = 0x0242,
    ARIB_GSET_DRCS_3                    = 0x0243,
    ARIB_GSET_DRCS_4                    = 0x0244,
    ARIB_GSET_DRCS_5                    = 0x0245,
    ARIB_GSET_DRCS_6                    = 0x0246,
    ARIB_GSET_DRCS_7                    = 0x0247,
    ARIB_GSET_DRCS_8                    = 0x0248,
    ARIB_GSET_DRCS_9                    = 0x0249,
    ARIB_GSET_DRCS_10                   = 0x024A,
    ARIB_GSET_DRCS_11                   = 0x024B,
    ARIB_GSET_DRCS_12                   = 0x024C,
    ARIB_GSET_DRCS_13                   = 0x024D,
    ARIB_GSET_DRCS_14                   = 0x024E,
    ARIB_GSET_DRCS_15                   = 0x024F,
    ARIB_GSET_MACRO                     = 0x02F0,

    // 2-byte code DRCS (0x0340¡Á0x03FF)
    ARIB_GSET_DRCS_0                    = 0x0340
} ARIBCC_GraphSet_E;

/* control set (instruction) for AribCC */
/* Ref : Table 7-14 Control function character set code table */
typedef enum    tagARIBCC_CtrlSet_E
{
    /* C0 control set */
    ARIB_C0_NUL     = 0x00000000,
    ARIB_C0_BEL     = 0x00070000,
    ARIB_C0_APB     = 0x00080000,
    ARIB_C0_APF     = 0x00090000,
    ARIB_C0_APD     = 0x000A0000,
    ARIB_C0_APU     = 0x000B0000,
    ARIB_C0_CS      = 0x000C0000,
    ARIB_C0_APR     = 0x000D0000,
    ARIB_C0_LS1     = 0x000E0000,
    ARIB_C0_LS0     = 0x000F0000,
    ARIB_C0_CAN     = 0x00180000,
    ARIB_C0_SS2     = 0x00190000,
    ARIB_ESC_LS2    = 0x001B6E00,
    ARIB_ESC_LS3    = 0x001B6F00,
    ARIB_ESC_LS3R   = 0x001B7C00,
    ARIB_ESC_LS2R   = 0x001B7D00,
    ARIB_ESC_LS1R   = 0x001B7E00,
    ARIB_C0_SS3     = 0x001D0000,
    ARIB_C0_RS      = 0x001E0000,
    ARIB_C0_US      = 0x001F0000,

    ARIB_SP_SP      = 0x00200000,
    ARIB_SP_DEL     = 0x007F0000,

    /* C1 control set */
    ARIB_C1_BKF     = 0x00800000,
    ARIB_C1_RDF     = 0x00810000,
    ARIB_C1_GRF     = 0x00820000,
    ARIB_C1_YLF     = 0x00830000,
    ARIB_C1_BLF     = 0x00840000,
    ARIB_C1_MGF     = 0x00850000,
    ARIB_C1_CNF     = 0x00860000,
    ARIB_C1_WHF     = 0x00870000,
    ARIB_C1_SSZ     = 0x00880000,
    ARIB_C1_MSZ     = 0x00890000,
    ARIB_C1_NSZ     = 0x008A0000,
    ARIB_C1_SPL     = 0x00990000,
    ARIB_C1_STL     = 0x009A0000,
    ARIB_CSI_PLD    = 0x009B5B00,
    ARIB_CSI_PLU    = 0x009B5C00,
    ARIB_CSI_SCS    = 0x009B6F00,

    ARIB_C0_PAPF    = 0x01160000,
    ARIB_C0_ESC     = 0x011B0000,
    ARIB_C1_SZX     = 0x018B0000,

    ARIB_C1_COL_1   = 0x01900000,
    ARIB_C1_COL_2   = 0x01900001,
    ARIB_C1_FLC     = 0x01910000,
    ARIB_C1_CDC_1   = 0x01920000,
    ARIB_C1_CDC_2   = 0x01920001,
    ARIB_C1_POL     = 0x01930000,
    ARIB_C1_WMM     = 0x01940000,
    ARIB_C1_MACRO   = 0x01950000,
    ARIB_C1_HLC     = 0x01970000,
    ARIB_C1_RPC     = 0x01980000,
    ARIB_C1_CSI_1   = 0x019B0000,

    ARIB_CSI_SWF_1  = 0x019B5300,
    ARIB_CSI_CCC    = 0x019B5400,
    ARIB_CSI_SHS    = 0x019B5800,
    ARIB_CSI_SVS    = 0x019B5900,
    ARIB_CSI_GAA    = 0x019B5D00,
    ARIB_CSI_MDF    = 0x019B6400,
    ARIB_CSI_CFS    = 0x019B6500,
    ARIB_CSI_XCS    = 0x019B6600,
    ARIB_CSI_PRA    = 0x019B6800,
    ARIB_CSI_ACS    = 0x019B6900,
    ARIB_CSI_RCS    = 0x019B6E00,
    ARIB_C1_TIME_1  = 0x019D0000,
    ARIB_C1_TIME_2  = 0x019D0001,
    ARIB_ESC_DES    = 0x021B0000,
    ARIB_C0_APS     = 0x021C0000,

    ARIB_C1_CSI_2   = 0x029B0001,
    ARIB_CSI_GSM    = 0x029B4200,
    ARIB_CSI_SDF    = 0x029B5600,
    ARIB_CSI_SSM    = 0x029B5700,
    ARIB_CSI_SRC    = 0x029B5E00,
    ARIB_CSI_SDP    = 0x029B5F00,
    ARIB_CSI_ACPS   = 0x029B6100,
    ARIB_CSI_ORN    = 0x029B6300,
    ARIB_CSI_SCR    = 0x029B6700,
    ARIB_CSI_TCC    = 0x039B6200,

    ARIB_C1_TIME_6  = 0x039D0005,
    ARIB_CSI_SWF_2  = 0x049B5301,
    ARIB_C1_TIME_3  = 0x049D0002,
    ARIB_C1_TIME_4  = 0x049D0003,
    ARIB_C1_TIME_5  = 0x049D0004,
    ARIB_C1_TIME_7  = 0x049D0006
} ARIBCC_CtrlSet_E;

/* Colour Controls parameter */
/* Ref : control code "COL" in Table 7-16 C1 control set */
typedef enum  tagARIBCC_ColParam_E
{
    COL_FORE_0    = 0x40,         // Palette Index( CMLA0 )
    COL_FORE_1,                   // Palette Index( CMLA1 )
    COL_FORE_2,                   // Palette Index( CMLA2 )
    COL_FORE_3,                   // Palette Index( CMLA3 )
    COL_FORE_4,                   // Palette Index( CMLA4 )
    COL_FORE_5,                   // Palette Index( CMLA5 )
    COL_FORE_6,                   // Palette Index( CMLA6 )
    COL_FORE_7,                   // Palette Index( CMLA7 )
    COL_FORE_8,                   // Palette Index( CMLA8 )
    COL_FORE_9,                   // Palette Index( CMLA9 )
    COL_FORE_10,                  // Palette Index( CMLA10 )
    COL_FORE_11,                  // Palette Index( CMLA11 )
    COL_FORE_12,                  // Palette Index( CMLA12 )
    COL_FORE_13,                  // Palette Index( CMLA13 )
    COL_FORE_14,                  // Palette Index( CMLA14 )
    COL_FORE_15,                  // Palette Index( CMLA15 )

    COL_BACK_0    = 0x50,         // Palette Index( CMLA0 )
    COL_BACK_1,                   // Palette Index( CMLA1 )
    COL_BACK_2,                   // Palette Index( CMLA2 )
    COL_BACK_3,                   // Palette Index( CMLA3 )
    COL_BACK_4,                   // Palette Index( CMLA4 )
    COL_BACK_5,                   // Palette Index( CMLA5 )
    COL_BACK_6,                   // Palette Index( CMLA6 )
    COL_BACK_7,                   // Palette Index( CMLA7 )
    COL_BACK_8,                   // Palette Index( CMLA8 )
    COL_BACK_9,                   // Palette Index( CMLA9 )
    COL_BACK_10,                  // Palette Index( CMLA10 )
    COL_BACK_11,                  // Palette Index( CMLA11 )
    COL_BACK_12,                  // Palette Index( CMLA12 )
    COL_BACK_13,                  // Palette Index( CMLA13 )
    COL_BACK_14,                  // Palette Index( CMLA14 )
    COL_BACK_15,                  // Palette Index( CMLA15 )

    COL_FORE_MIDDLE_0    = 0x60,  // Palette Index( CMLA0 )
    COL_FORE_MIDDLE_1,            // Palette Index( CMLA1 )
    COL_FORE_MIDDLE_2,            // Palette Index( CMLA2 )
    COL_FORE_MIDDLE_3,            // Palette Index( CMLA3 )
    COL_FORE_MIDDLE_4,            // Palette Index( CMLA4 )
    COL_FORE_MIDDLE_5,            // Palette Index( CMLA5 )
    COL_FORE_MIDDLE_6,            // Palette Index( CMLA6 )
    COL_FORE_MIDDLE_7,            // Palette Index( CMLA7 )
    COL_FORE_MIDDLE_8,            // Palette Index( CMLA8 )
    COL_FORE_MIDDLE_9,            // Palette Index( CMLA9 )
    COL_FORE_MIDDLE_10,           // Palette Index( CMLA10 )
    COL_FORE_MIDDLE_11,           // Palette Index( CMLA11 )
    COL_FORE_MIDDLE_12,           // Palette Index( CMLA12 )
    COL_FORE_MIDDLE_13,           // Palette Index( CMLA13 )
    COL_FORE_MIDDLE_14,           // Palette Index( CMLA14 )
    COL_FORE_MIDDLE_15,           // Palette Index( CMLA15 )

    COL_BACK_MIDDLE_0    = 0x70,  // Palette Index( CMLA0 )
    COL_BACK_MIDDLE_1,            // Palette Index( CMLA1 )
    COL_BACK_MIDDLE_2,            // Palette Index( CMLA2 )
    COL_BACK_MIDDLE_3,            // Palette Index( CMLA3 )
    COL_BACK_MIDDLE_4,            // Palette Index( CMLA4 )
    COL_BACK_MIDDLE_5,            // Palette Index( CMLA5 )
    COL_BACK_MIDDLE_6,            // Palette Index( CMLA6 )
    COL_BACK_MIDDLE_7,            // Palette Index( CMLA7 )
    COL_BACK_MIDDLE_8,            // Palette Index( CMLA8 )
    COL_BACK_MIDDLE_9,            // Palette Index( CMLA9 )
    COL_BACK_MIDDLE_10,           // Palette Index( CMLA10 )
    COL_BACK_MIDDLE_11,           // Palette Index( CMLA11 )
    COL_BACK_MIDDLE_12,           // Palette Index( CMLA12 )
    COL_BACK_MIDDLE_13,           // Palette Index( CMLA13 )
    COL_BACK_MIDDLE_14,           // Palette Index( CMLA14 )
    COL_BACK_MIDDLE_15            // Palette Index( CMLA15 )
} ARIBCC_ColParam_E;

/* Character Size Controls */
/* Ref : control code "SZX" in Table 7-16 C1 control set */
typedef enum  tagARIBCC_SzxParam_E
{
    SZX_LENGTH_TWICE      = 0x41, /*Double height*/
    SZX_SIDE_TWICE        = 0x44, /*Double width*/
    SZX_LENGTH_SIDE_TWICE = 0x45, /*Double height and width*/
    SZX_MICRO             = 0x60, /*Tiny size*/
    SZX_SPECIAL_2         = 0x64,
    SZX_SPECIAL_1         = 0x6B
} ARIBCC_SzxParam_E;

/* Flashing control */
/* Ref : control code "FLC" in Table 7-16 C1 control set */
typedef enum  tagARIBCC_FlcParam_E
{
    FLC_NORMAL       = 0x40, /*Start normal phase flashing*/
    FLC_REVERSING    = 0x47, /*Start inverted phase flashing*/
    FLC_END          = 0x4F  /*Stop flashing*/
} ARIBCC_FlcParam_E;

/* Pattern Polarity Controls */
/* Ref : control code "POL" in Table 7-16 C1 control set */
typedef enum  tagARIBCC_PolParam_E
{
    POL_NORMAL      = 0x40, /*normal polarity*/
    POL_REVERSING_1, /*inverted polarity 1,Foreground and background colours are inverted in the whole display block*/
    POL_REVERSING_2  /*inverted polarity 2,Foreground and background colours are inverted in the design frame*/
} ARIBCC_PolParam_E;

/* highlighting character block */
/* Ref : control code "HLC" in Table 7-16 C1 control set */
typedef enum  tagARIBCC_HlcParam_E
{
    HLC_PATTERN_0    = 0x40,
    HLC_PATTERN_1,
    HLC_PATTERN_2,
    HLC_PATTERN_3,
    HLC_PATTERN_4,
    HLC_PATTERN_5,
    HLC_PATTERN_6,
    HLC_PATTERN_7,
    HLC_PATTERN_8,
    HLC_PATTERN_9,
    HLC_PATTERN_10,
    HLC_PATTERN_11,
    HLC_PATTERN_12,
    HLC_PATTERN_13,
    HLC_PATTERN_14,
    HLC_PATTERN_15
} ARIBCC_HlcParam_E;

/* Time Controls */
/* Ref : control code "TIME" in Table Table 7-16 C1 control set */
typedef enum  tagARIBCC_TimeParam_E
{
    TIME_WAIT                 = 0x20, /*Wait for process*/
    TIME_CONTROL_MODE_FREE    = 0x40,
    TIME_CONTROL_MODE_REAL,
    TIME_CONTROL_MODE_OFFSET,
    TIME_CONTROL_MODE_ORIGINAL
} ARIBCC_TimeParam_E;

/* Conceal Display Controls */
/* Ref : control code "CDC" in Table Table 7-16 C1 control set */
typedef enum  tagARIBCC_CdcParam_E
{
    CDC_REPLACEMENT_0  = 0x20,  /*Simple replacing conceal start*/
    CDC_REPLACEMENT_1,
    CDC_REPLACEMENT_2,
    CDC_REPLACEMENT_3,
    CDC_REPLACEMENT_4,
    CDC_REPLACEMENT_5,
    CDC_REPLACEMENT_6,
    CDC_REPLACEMENT_7,
    CDC_REPLACEMENT_8,
    CDC_REPLACEMENT_9,
    CDC_REPLACEMENT_10,
    CDC_START          = 0x40, /*Start conceal*/
    CDC_END            = 0x4F  /*Stop conceal*/
} ARIBCC_CdcParam_E;

/* Set Writing Format */
/* Ref : control code "SWF" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_SwfParam_E
{
    SWF_FORMAT_NORMAL_DENCITY_SIDE   = 0x00,
    SWF_FORMAT_NORMAL_DENCITY_LENGTH,
    SWF_FORMAT_HIGH_DENCITY_SIDE,
    SWF_FORMAT_HIGH_DENCITY_LENGTH,
    SWF_FORMAT_ROMAN_SIDE,
    SWF_FORMAT_1920_1080_SIDE,
    SWF_FORMAT_1920_1080_LENGTH,
    SWF_FORMAT_960_540_SIDE,
    SWF_FORMAT_960_540_LENGTH,
    SWF_FORMAT_720_480_SIDE,
    SWF_FORMAT_720_480_LENGTH,
    SWF_FORMAT_1280_720_SIDE,
    SWF_FORMAT_1280_720_LENGTH,
} ARIBCC_SwfParam_E;

/* Character composition dot designation */
/* Ref : control code "SSM" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_SsmParam_E
{
    SSM_SIZE_16    = 16,        // 16*16
    SSM_SIZE_20    = 20,        // 20*20
    SSM_SIZE_24    = 24,        // 24*24
    SSM_SIZE_30    = 30,        // 30*30
    SSM_SIZE_36    = 36         // 36*36
} ARIBCC_SsmParam_E;

/* Ornament Control */
/* Ref : control code "ORN" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_OrnParam_E
{
    ORN_NOTHING            = 0x00, /*without character decoration*/
    ORN_HEMMING,                   /*with hemming*/
    ORN_SHADOW,                    /*with shade*/
    ORN_HOLLOW                     /*with hollow*/
} ARIBCC_OrnParam_E;

/*Composite Character Composition*/
/* Ref : control code "CCC" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_CccParam_E
{
    CCC_END       = 0x00,  /*composition ends*/
    CCC_START_OR  = 0x02,  /*OR composition starts*/
    CCC_START_AND,         /*AND composition starts*/
    CCC_START_XOR          /*XOR composition starts*/
} ARIBCC_CccParam_E;

/*Colouring block*/
/* Ref : control code "GAA" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_GaaParam_E
{
    GAA_ALL     = 0x00,  /*whole display block*/
    GAA_DESIGN_FRAM      /*design frame*/
} ARIBCC_GaaParam_E;

/*Switch control*/
/*switching mode designation*/
/* Ref : control code "TCC" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_TccParam1_E
{
    TCC_GROUP_CUT          = 0x00,  /*character group, cut*/
    TCC_GROUP_DISSOLVE,             /*character group, dissolve*/
    TCC_GROUP_WIPE,                 /*character group, wipe*/
    TCC_GROUP_ROLL,                 /*character group, roll*/
    TCC_GROUP_SLIDE,                /*character group, slide*/
    TCC_CHARACTER_CUT,              /*each character, cut*/
    TCC_CHARACTER_DISSOLVE,         /*each character, dissolve*/
    TCC_CHARACTER_WIPE,             /*each character, wipe*/
    TCC_CHARACTER_ROLL,             /*each character, roll*/
    TCC_CHARACTER_SLIDE,            /*each character, slide*/
} ARIBCC_TccParam1_E;

/*Switch control*/
/*switching, direction*/
/* Ref : control code "TCC" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_TccParam2_E
{
    TCC_LEFT_TO_RIGHT    = 0x00,  /*left to right*/
    TCC_RIGHT_TO_LEFT,            /*right to left*/
    TCC_UP_TO_DOWN,               /*up to down*/
    TCC_DOWN_TO_UP                /*down to up*/
} ARIBCC_TccParam2_E;

/*Font*/
/* Ref : control code "MDF" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_MdfParam_E
{
    MDF_NORMAL       = 0x00,  /*standard*/
    MDF_BOLD,                 /*bold character*/
    MDF_OBLIQUE,              /*slanted character*/
    MDF_BOLD_OBLIQUE          /*bold slanted character*/
} ARIBCC_MdfParam_E;

/*Raster Colour Designation*/
/* Ref : control code "SRC" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_SrcParam_E
{
    SRC_BOXING  = 0x00,  /*background is Raster colour and boxing display*/
    SRC_SIMPLE,          /*background is transparent and simple superimpose*/
    SRC_FRAMING,         /*background is transparent and superimposed with hemming*/
    SRC_SHADOW           /*background is transparent and superimposed with shadow*/
} ARIBCC_SrcParam_E;

/*External Character Set*/
/* Ref : control code "XCS" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_XcsParam_E
{
    XCS_START = 0x00,  /*definition starts*/
    XCS_END            /*definition ends*/
} ARIBCC_XcsParam_E;

/*alternative Ccharacter set*/
/* Ref : control code "ACS" in Table 7-17 Extension control code (CSI) */
typedef enum  tagARIBCC_AcsParam_E
{
    ACS_START = 0x00,          /*source character definition start*/
    ACS_END,                   /*source character definition end*/
    ACS_START_DEFINITION_1,    /*alternative character (alphabet, numeric and katakana) definition start*/
    ACS_END_DEFINITION_1,      /*alternative character (alphabet, numeric and katakana) definition end*/
    ACS_START_DEFINITION_2,    /*alternative character (for speech synthesis) definition start*/
    ACS_END_DEFINITION_2       /*alternative character (for speech synthesis) definition end*/
} ARIBCC_AcsParam_E;

/* Scroll designation */
/* Ref : control code "SCR" in Table Table 5-4 Added extension control code (CSI) */
typedef enum  tagARIBCC_ScrParam_E
{
    SCR_MODE_FIXED        = 0x00,  /*fixed display (without scroll)*/
    SCR_MODE_LINE,                 /*one line scroll to character direction (without roll out)*/
    SCR_MODE_LINE_ROLLOUT,         /*one line scroll to character direction (with roll out)*/
    SCR_MODE_SCREEN,               /*whole display scroll to line direction (without roll out)*/
    SCR_MODE_SCREEN_ROLLOUT        /*whole display scroll to line direction (with roll out)*/
} ARIBCC_ScrParam_E;

/* Built-in sound replay */
typedef enum  tagARIBCC_PraParam_E
{
    PRA_NEWS_FLUSH_CHIME_1  = 0x00,
    PRA_NEWS_FLUSH_CHIME_2,
    PRA_NEWS_FLUSH_CHIME_3,
    PRA_NEWS_FLUSH_CHIME_4,
    PRA_NEWS_FLUSH_CHIME_5,
    PRA_BOTTUN_1,
    PRA_BOTTUN_2,
    PRA_BOTTUN_3,
    PRA_BOTTUN_4,
    PRA_BOTTUN_5,
    PRA_BOTTUN_6,
    PRA_BOTTUN_7,
    PRA_BOTTUN_8,
    PRA_ALERT
} ARIBCC_PraParam_E;



typedef enum tagARIBCC_CaptionType_E
{
    ARIB_CAP_SUPERIMPOSE = 0,
    ARIB_CAP_CAPTION,
    ARIB_CAP_BUTT
} ARIBCC_CaptionType_E;


#if defined(__cplusplus) || defined(__CPLUSPLUS__)
}
#endif

#endif
