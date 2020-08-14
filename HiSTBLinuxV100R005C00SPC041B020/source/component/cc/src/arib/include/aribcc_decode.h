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

#ifndef __ARIBCC_DECODE_H__
#define __ARIBCC_DECODE_H__

#include  "aribcc_common.h"

/*Caption management data should be sent at least every 3 minutes*/
#define ARIBCC_MANAGEMENT_TIMEOUT   (60*3)    // 180 s

/* stream_id of cc */
#define ARIBCC_PES_PRIVATE_STREAM_1  0xBD
#define ARIBCC_PES_PRIVATE_STREAM_2  0xBF

/* PTS_DTS_flags */
#define ARIBCC_PES_PDF_NOTHING       0x0
#define ARIBCC_PES_PDF_DTS           0x1
#define ARIBCC_PES_PDF_PTS           0x2
#define ARIBCC_PES_PDF_PTS_DTS       0x3

/* data_identifier */
#define CCPES_DATA_IDENT_SYNC   0x80
#define CCPES_DATA_IDENT_ASYNC  0x81

/* private_stream_id */
#define CCPES_PRIVATE_STREAM_ID 0xFF

/* data group head length */
#define DG_HEADER_LENGTH        5

/* the max data group size */
#define DGS_MAX_SIZE            CCPES_MAX_SIZE

/* the max control code data size */
#define CONTROL_MAX_DATA_SIZE   16

/* TMD (Time control mode) */
#define TMD_FREE                0x0             /* 00: Playback time is not restricted to synchronize to the clock */
#define TMD_REAL_TIME           0x1             /* 01: Playback time is in accordance with the time of the clock */
#define TMD_OFFSET_TIME         0x2             /* 10: Playback time added with offset time should be the new playback time */

/* OTM: offset time */
#define OTM_FLD_LENGTH          5

/* the max size of data_unit_loop_length */
#define DUL_MAX_SIZE            CCPES_MAX_SIZE


#define DU_HEADER_LENGTH        5


/* STM: presentation start time */
#define STM_FLD_LENGTH          5


#define DU_MAX_SIZE             CCPES_MAX_SIZE

/* language_tag (Identification of language) */
#define LANG_ID_1ST             0x0             // 0: 1st language
#define LANG_ID_2ND             0x1             // 1: 2st language
#define LANG_ID_3RD             0x2             // 2: 3st language
#define LANG_ID_4TH             0x3             // 3: 4st language
#define LANG_ID_5TH             0x4             // 4: 5st language
#define LANG_ID_6TH             0x5             // 5: 6st language
#define LANG_ID_7TH             0x6             // 6: 7st language
#define LANG_ID_8TH             0x7             // 7: 8st language

/*
DMF (Display mode): This 4-bit field indicates the display mode of the caption statement.
Display mode is indicated in reception and recording playback in 2 bit each. The modes
controlled by DMF are listed in Table 9-5.
*/
/*display mode at a time of reception and recording playback*/
#define DMF_RECEPTION            0x0
#define DMF_REC_AND_PLAY         0x1

#define GET_DMF_RECEPTION(DMF)  ((((HI_U8)(DMF))&(0x0C))>>2)
#define GET_DMF_RECPLAY(DMF)    (((HI_U8)(DMF))&(0x03))

#define DMF_AUTOMATIC           0x0             // 00: Automatic display
#define DMF_NON_AUTOMATIC       0x1             // 01: Non-displayed automatically
#define DMF_SELECTION           0x2             // 10: Selectable display
#define DMF_SP_CONDITION        0x3             // 11: Automatic display/non-display under specific condition

#define DMF_AUTO_AND_AUTO       ((DMF_AUTOMATIC << 2) | DMF_AUTOMATIC)
#define DMF_AUTO_AND_NOT        ((DMF_AUTOMATIC << 2) | DMF_NON_AUTOMATIC)
#define DMF_AUTO_AND_SELECT     ((DMF_AUTOMATIC << 2) | DMF_SELECTION)
#define DMF_NON_AND_AUTO        ((DMF_NON_AUTOMATIC << 2) | DMF_AUTOMATIC)
#define DMF_NON_AND_NON         ((DMF_NON_AUTOMATIC << 2) | DMF_NON_AUTOMATIC)
#define DMF_NON_AND_SELECT      ((DMF_NON_AUTOMATIC << 2) | DMF_SELECTION)
#define DMF_SELECT_AND_AUTO     ((DMF_SELECTION << 2) | DMF_AUTOMATIC)
#define DMF_SELECT_AND_NON      ((DMF_SELECTION << 2) | DMF_NON_AUTOMATIC)
#define DMF_SELECT_AND_SELECT   ((DMF_SELECTION << 2) | DMF_SELECTION)
#define DMF_SPECIAL_AND_AUTO    ((DMF_SP_CONDITION << 2) | DMF_AUTOMATIC)
#define DMF_SPECIAL_AND_NON     ((DMF_SP_CONDITION << 2) | DMF_NON_AUTOMATIC)
#define DMF_SPECIAL_AND_SELECT  ((DMF_SP_CONDITION << 2) | DMF_SELECTION)

/*Display condition designation*/
#define DC_RAIN_ATTENUATION     0x00


/* timing: display timing */
#define DT_ASYNC                0x0
#define DT_PROGRAM_SYNC         0x1
#define DT_TIME_SYNC            0x2


/* code element */
#define CODE_ELEMENT_G0         0x28
#define CODE_ELEMENT_G1         0x29
#define CODE_ELEMENT_G2         0x2A
#define CODE_ELEMENT_G3         0x2B


#define GSET_CLASS_1BYTE        0x00
#define GSET_CLASS_2BYTE        0x01
#define GSET_CLASS_GSET         0x00
#define GSET_CLASS_DRCS         0x01


#define GSET_HIRAGANA           0x30
#define GSET_KATAKANA           0x31
#define GSET_MOSAIC_A           0x32
#define GSET_MOSAIC_B           0x33
#define GSET_MOSAIC_C           0x34
#define GSET_MOSAIC_D           0x35
#define GSET_P_ALPHANUMERIC     0x36
#define GSET_P_HIRAGANA         0x37
#define GSET_P_KATAKANA         0x38
#define GSET_KANJI_2            0x39
#define GSET_JIS_INTER_KANJI_1  0x39
#define GSET_KANJI_3            0x3A
#define GSET_JIS_INTER_KANJI_2  0x3A
#define GSET_KANJI_4            0x3B
#define GSET_ADDITION_SIGN      0x3B
#define GSET_KANJI              0x42
#define GSET_JIS_X0201_KATAKANA 0x49
#define GSET_ALPHANUMERIC       0x4A

// DRCS
#define GSET_DRCS_0             0x40
#define GSET_DRCS_1             0x41
#define GSET_DRCS_2             0x42
#define GSET_DRCS_3             0x43
#define GSET_DRCS_4             0x44
#define GSET_DRCS_5             0x45
#define GSET_DRCS_6             0x46
#define GSET_DRCS_7             0x47
#define GSET_DRCS_8             0x48
#define GSET_DRCS_9             0x49
#define GSET_DRCS_10            0x4A
#define GSET_DRCS_11            0x4B
#define GSET_DRCS_12            0x4C
#define GSET_DRCS_13            0x4D
#define GSET_DRCS_14            0x4E
#define GSET_DRCS_15            0x4F
#define GSET_MACRO              0x70


/* C0 control set: 00/0¡Á01/15 */
#define C0_NULL                 0x00
#define C0_BEL                  0x07
#define C0_APB                  0x08
#define C0_APF                  0x09
#define C0_APD                  0x0A
#define C0_APU                  0x0B
#define C0_CS                   0x0C
#define C0_APR                  0x0D
#define C0_LS1                  0x0E
#define C0_LS0                  0x0F
#define C0_PAPF                 0x16
#define C0_CAN                  0x18
#define C0_SS2                  0x19
#define C0_ESC                  0x1B
#define C0_APS                  0x1C
#define C0_SS3                  0x1D
#define C0_RS                   0x1E
#define C0_US                   0x1F


/* C1 control set: 08/0¡Á09/15 */
#define C1_BKF                  0x80
#define C1_RDF                  0x81
#define C1_GRF                  0x82
#define C1_YLF                  0x83
#define C1_BLF                  0x84
#define C1_MGF                  0x85
#define C1_CNF                  0x86
#define C1_WHF                  0x87
#define C1_SSZ                  0x88
#define C1_MSZ                  0x89
#define C1_NSZ                  0x8A
#define C1_SZX                  0x8B
#define C1_COL                  0x90
#define C1_FLC                  0x91
#define C1_CDC                  0x92
#define C1_POL                  0x93
#define C1_WMM                  0x94
#define C1_MACRO                0x95
#define C1_HLC                  0x97
#define C1_RPC                  0x98
#define C1_SPL                  0x99
#define C1_STL                  0x9A
#define C1_CSI                  0x9B
#define C1_TIME                 0x9D


/* special code: 02/0,07/15(,10/0,15/15 */
#define SP_SP                   0x20
#define SP_DEL                  0x7F


/* Final byte of control sequence introducer */
#define CSI_GSM                 0x42
#define CSI_SWF                 0x53
#define CSI_CCC                 0x54
#define CSI_SDF                 0x56
#define CSI_SSM                 0x57
#define CSI_SHS                 0x58
#define CSI_SVS                 0x59
#define CSI_PLD                 0x5B
#define CSI_PLU                 0x5C
#define CSI_GAA                 0x5D
#define CSI_SRC                 0x5E
#define CSI_SDP                 0x5F
#define CSI_ACPS                0x61
#define CSI_TCC                 0x62
#define CSI_ORN                 0x63
#define CSI_MDF                 0x64
#define CSI_CFS                 0x65
#define CSI_XCS                 0x66
#define CSI_SCR                 0x67
#define CSI_PRA                 0x68
#define CSI_ACS                 0x69
#define CSI_RCS                 0x6E
#define CSI_SCS                 0x6F


/* code extension */
#define ESC_LS2                 0x6E
#define ESC_LS3                 0x6F
#define ESC_LS1R                0x7E
#define ESC_LS2R                0x7D
#define ESC_LS3R                0x7C


/* transmission mode */
#define TM_2_GRADATION          0x0
#define TM_MULTI_GRADATION      0x1
#define TM_2_COLOR_COMPRESS     0x2
#define TM_MULTI_COLOR_COMPRESS 0x3


#define BMP_FLC_COLOR_MAX       16


#define PNG_SIG_CODE            "\x89PNG\r\n\x1A\n"
#define PNG_SIG_SIZE            8


#define CHUNK_TYPE_IHDR         "IHDR"
#define CHUNK_TYPE_IDAT         "IDAT"
#define CHUNK_TYPE_IEND         "IEND"
#define CHUNK_TYPE_PLTE         "PLTE"


#define IHDR_PALETTE            3
#define IHDR_DEFLATE_INFLATE    0
#define IHDR_NON_FILTER         0
#define IHDR_NON_INTERLACE      0


/* data_group_id: data group identification */
#define GET_DGI_TYPE(DGI)       (((HI_U8)(DGI))&(0x0F))
#define GET_DGI_GROUP(DGI)      (((HI_U8)(DGI))&(0x30))

#define IS_DGI_TYPE_STATE(DGI)  ((GET_DGI_TYPE(DGI)>=DGI_TYPE_1ST_LANG)&&(GET_DGI_TYPE(DGI)<=DGI_TYPE_8TH_LANG))


/* 8-bit code */
#define B8TOI(C)                (((HI_U8)(C))<0x40 ? ((HI_U8)(C) & 0x0F) : ((HI_U8)(C) & 0x3F))


#define IS_GSET_1B_DRCS(GS)     ((((HI_U8)(GS))>=GSET_DRCS_1) && (((HI_U8)(GS))<=GSET_DRCS_15))
#define IS_GSET_2B_DRCS(GS)     (((HI_U8)(GS))==GSET_DRCS_0)
#define IS_GSET_DRCS(GS)        ((((HI_U8)(GS))>=GSET_DRCS_0) && (((HI_U8)(GS))<=GSET_DRCS_15))

#define IS_CS_2B_DRCS_CELL(CS)  ((((HI_U8)(CS))>=0x21) && (((HI_U8)(CS))<=0x7E))
#define IS_CS_2B_DRCS_ROW(CS)   ((((HI_U8)(CS))>=0x21) && (((HI_U8)(CS))<=0x7E))

#define IS_GSET_MACRO(GS)       (((HI_U8)(GS))==GSET_MACRO)

/* Ref : Figure 7-1 Code Table */
#define IS_AREA_C0(CS)          (((HI_U8)(CS))<=C0_US)
#define IS_AREA_GL(CS)          ((((HI_U8)(CS))>=0x21) && (((HI_U8)(CS))<=0x7E))
#define IS_AREA_CSI(CS)         (((HI_U8)(CS))==C1_CSI)
#define IS_AREA_C1(CS)          ((((HI_U8)(CS))>=C1_BKF) && (((HI_U8)(CS))<=0x9F))
#define IS_AREA_GR(CS)          ((((HI_U8)(CS))>=0xA1) && (((HI_U8)(CS))<=0xFE))
#define IS_AREA_SP(CS)          ((((HI_U8)(CS))==0x20) || (((HI_U8)(CS))==0x7F) || (((HI_U8)(CS))==0xA0) || (((HI_U8)(CS))==0xFF))
#define IS_AREA_CONTROL(CS)     (IS_AREA_C0(CS) || IS_AREA_C1(CS) || IS_AREA_SP(CS))
#define IS_SINGLE_SHIFT(CS)     ((((HI_U8)(CS))==C0_SS2) || (((HI_U8)(CS))==C0_SS3))
#define GET_MACRO_CODE(CS)      (((HI_U8)(CS))&0x7F)
#define IS_DEFAULT_MACRO(CS)    ((GET_MACRO_CODE(CS)>=0x60) && (GET_MACRO_CODE(CS)<=0x6F))
#define GET_MACRO_ID(CS)        (GET_MACRO_CODE(CS)-0x60)

#define IS_ARIB_INVOCATION(C)   ((((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS0)     ||(((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS1)   \
                                 || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS2) ||(((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS3)  \
                                 || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS1R)||(((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS2R) \
                                 || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS3R)||(((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS2)   \
                                 || (((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS3))

#define IS_ARIB_SINGLE_SHIFT(C) ((((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS2) || (((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS3))

#define CTRL_TO_INVOKE_AREA(C)  (((((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS0)    ||(((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS1)  \
                                  || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS2)||(((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS3) \
                                  || (((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS2) ||(((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS3)) \
                                  ? 0 : 1)
#define CTRL_TO_INVOKE_ELEM(C)  ((((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS0) ? ARIB_CELEMENT_G0 : \
                                 ((((ARIBCC_CtrlSet_E)(C))==ARIB_C0_LS1) \
                                  || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS1R)) ? ARIB_CELEMENT_G1 : \
                                 ((((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS2) || (((ARIBCC_CtrlSet_E)(C))==ARIB_ESC_LS2R) \
                                  || (((ARIBCC_CtrlSet_E)(C))==ARIB_C0_SS2)) ? ARIB_CELEMENT_G2 : ARIB_CELEMENT_G3)


#define IS_DES_FINAL_BYTE(CS)   ((((HI_U8)(CS))>0x2F) || (((HI_U8)(CS))<0x20))

#define TO_ARIB_GSET(C1,C2,GS)  ((((HI_U8)(C2))<<9) | (((HI_U8)(C1))<<8) | ((HI_U8)(GS)))


/* depth of gradation */
#define GET_DG_GRADATION(DG)    (((HI_U8)(DG))+2)

/* character size */
#define IS_CS_STANDARD_SIZE(CS) ((((HI_U8)(CS))==16) || (((HI_U8)(CS))==20) || (((HI_U8)(CS))==24) || (((HI_U8)(CS))==30) || \
                                (((HI_U8)(CS))==36))
#define IS_CS_HALF_SIZE(CS)     ((((HI_U8)(CS))==8) || (((HI_U8)(CS))==10) || (((HI_U8)(CS))==12) || (((HI_U8)(CS))==15) || \
                                (((HI_U8)(CS))==18))


#define GET_CS_NUM_OF_PARAM(CS) ((((HI_U32)(CS))&(0xFF000000))>>24)


typedef enum    tagARIBCC_DrcsType_E
{
    ARIB_DRCS_1BYTE,
    ARIB_DRCS_2BYTE
} ARIBCC_DrcsType_E;


/* Ref : 9.2 Structure of data group */
typedef struct  tagARIBCC_DataGroup_S
{
    HI_U8    u8DataGroupID;            /* Data group identification; DGI : Group A/Group B */
    HI_U8    u8DataGroupVersion;       /* Data group version */
    HI_U8    u8DataGroupLinkNumber;    /* Data group link number */
    HI_U8    u8LastDataGroupLinkNumber;/* Last data group link number */
    HI_U16   u16DataGroupSize;         /* the size of Data group data */
} ARIBCC_DataGroup_S;

typedef struct  tagARIBCC_Data_S
{
    HI_U32            u32DataLength;
    HI_U8             *pu8Data;
} ARIBCC_Data_S;

typedef struct  tagARIBCC_Control_S
{
    ARIBCC_CtrlSet_E  enControlId;
    HI_U8             u8ControlData[CONTROL_MAX_DATA_SIZE];
} ARIBCC_Control_S;

typedef struct  tagARIBCC_StatementBody_S
{
    HI_U32            u32ControlCount;
    HI_U32            u32ControlLength;
    ARIBCC_Control_S  stControl[1];
} ARIBCC_StatementBody_S;

/* Ref : 9.4 Structure of data unit */
typedef struct  tagARIBCC_Unit_S
{
    HI_U32           u32UnitLength;
    HI_U32           u32UnitId;
    HI_U8            *pu8UnitData;
} ARIBCC_Unit_S;

typedef struct  tagARIBCC_UnitGroup_S
{
    HI_U32           u32UnitGroupLength;
    ARIBCC_Unit_S    stUnit[1];
} ARIBCC_UnitGroup_S;

/* Ref : 9.3.1 Caption management data */
typedef struct  tagARIBCC_Language_S
{
    HI_U8    u8LanguageTag;  /* Identification of language */
    HI_U8    u8DMF;          /* Display mode */
    HI_U8    u8DC;           /* Display condition designation */
    HI_U8    u8Format;       /* display format */
    HI_CHAR  cISO639LanguageCode[4]; /* Language code */
    HI_U8    u8TCS;          /* Character coding */
    HI_U8    u8RollupMode;   /* rollup_mode */
    HI_U8    u8Padding[6];
} ARIBCC_Language_S;

/* Ref : 9.3.1 Caption management data */
typedef struct  tagARIBCC_Management_S
{
    HI_U8              u8DataGroupID;               /* Data group identification; DGI : Group A/Group B */
    HI_U8              u8DataGroupVersion;          /* Data group version */
    HI_U8              u8DataGroupLinkNumber;       /* Data group link number */
    HI_U8              u8LastDataGroupLinkNumber;   /* Last data group link number */
    HI_U8              u8TMD;                       /* Time control mode */
    HI_U16             u16OTM[4];                   /* Offset time */
    HI_U8              u8NumLanguages;              /* Number of languages */
    ARIBCC_Language_S  stLanguageData[CC_LANG_MAX]; /* Language info */
} ARIBCC_Management_S;

typedef struct  tagARIBCC_ManagementUnit_S
{
    ARIBCC_Management_S    *pstManagement;
    ARIBCC_UnitGroup_S     *pstUnitGroup;
} ARIBCC_ManagementUnit_S;

typedef struct  tagARIBCC_Designation_S
{
    ARIBCC_CtrlSet_E        enDesignationId;
    ARIBCC_CodeElement_E    enCodeElement;
    ARIBCC_GraphSet_E       enGraphSet;
} ARIBCC_Designation_S;

typedef struct  tagARIBCC_DefaultMacro_S
{
    ARIBCC_Designation_S    astDesignation[4];
    ARIBCC_CtrlSet_E        aenInvocationId[2];
} ARIBCC_DefaultMacro_S;


HI_S32 ARIBCC_DEC_Init(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_DeInit (ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_Create (ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_Destroy (ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_Start (ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_Stop (ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_Reset(ARIBCC_CaptionType_E enCaptionType);

HI_S32 ARIBCC_DEC_DecodePesData (HI_U8 *pu8CcPes, HI_S32 s32Size);

HI_VOID ARIBCC_DEC_Cycle (HI_VOID);

HI_S32 ARIBCC_DEC_GetLanguage (ARIBCC_CaptionType_E enCaptionType,
                        HI_CHAR acISO639LanguageCode[][3], HI_U32 *pu32Count);

HI_S32 ARIBCC_DEC_SetLanguage  (ARIBCC_CaptionType_E enCaptionType, HI_CHAR acLanguage[3]);

HI_S32 ARIBCC_DEC_SetMute (ARIBCC_CaptionType_E enCaptionType, HI_BOOL bMute);

HI_S32 ARIBCC_DEC_GetLanguageIndex (ARIBCC_CaptionType_E enCaptionType, HI_CHAR acLanguage[3], HI_U8 *pu8Index);

HI_S32 ARIBCC_DEC_GetCaptionInfo (HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo);

#endif
