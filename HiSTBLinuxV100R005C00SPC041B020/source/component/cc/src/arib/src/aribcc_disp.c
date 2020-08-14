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


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
//#include <sys/prctl.h>
#include <string.h>

#include "hi_type.h"
#include "hi_debug.h"
#include "cc_debug.h"

#include "aribcc_obj.h"
#include "aribcc_def.h"
#include "aribcc_osd.h"
#include "aribcc_decode.h"
#include "aribcc_common.h"

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/
#define INIT_STATE
#define ROLL_UP_STATE

#define PALETTE_NUM_0 0
#define PALETTE_NUM_7 7


#define PAPF_NUMBER_MAX 63
#define APS_NUMBER_MAX  63
#define TIME_NUMBER_MIN  0
#define TIME_NUMBER_MAX  63
#define RPC_NUMBER_MAX  63

#define CAP_INIT_DISP_PICTURE   0x00008000
#define CAP_INIT_ADD_SOUND      0x00004000
#define CAP_INIT_DEFINE_DATA_1  0x00002000
#define CAP_INIT_DEFINE_DATA_2  0x00001000
#define CAP_INIT_DECLARE_DATA   0x00000800
#define CAP_INIT_DESIGN_CODE_1  0x00000400
#define CAP_INIT_DESIGN_CODE_2  0x00000200
#define CAP_INIT_DESIGN_OPE_1   0x00000100
#define CAP_INIT_DESIGN_OPE_2   0x00000080
#define CAP_INIT_DESIGN_STATE_1 0x00000040
#define CAP_INIT_DESIGN_STATE_2 0x00000020
#define CAP_INIT_DESIGN_STATE_3 0x00000010
#define CAP_INIT_DESIGN_STATE_4 0x00000008
#define CAP_INIT_DESIGN_STATE_5 0x00000004
#define CAP_INIT_DESIGN_STATE_6 0x00000002
#define CAP_INIT_DESIGN_STATE_7 0x00000001

#define CAP_INIT_DEFINE_DATA    ( CAP_INIT_DEFINE_DATA_1  | CAP_INIT_DEFINE_DATA_2  )
#define CAP_INIT_DESIGN_CODE    ( CAP_INIT_DESIGN_CODE_1  | CAP_INIT_DESIGN_CODE_2  )
#define CAP_INIT_DESIGN_OPE     ( CAP_INIT_DESIGN_OPE_1   | CAP_INIT_DESIGN_OPE_2   )
#define CAP_INIT_DESIGN_STATE   ( CAP_INIT_DESIGN_STATE_1 | CAP_INIT_DESIGN_STATE_2 | CAP_INIT_DESIGN_STATE_3 \
                                | CAP_INIT_DESIGN_STATE_4 | CAP_INIT_DESIGN_STATE_5 | CAP_INIT_DESIGN_STATE_6 \
                                | CAP_INIT_DESIGN_STATE_7 )

#define CAP_INIT_REVISE_CONTROL ( CAP_INIT_DISP_PICTURE   | CAP_INIT_ADD_SOUND      | CAP_INIT_DEFINE_DATA_1  \
                                | CAP_INIT_DECLARE_DATA   | CAP_INIT_DESIGN_STATE_1 | CAP_INIT_DESIGN_STATE_4 \
                                | CAP_INIT_DESIGN_STATE_5 )


#define CAP_INIT_RECEIVE_DATA   ( CAP_INIT_DEFINE_DATA_2  | CAP_INIT_DECLARE_DATA   | CAP_INIT_DESIGN_STATE_2 \
                                | CAP_INIT_DESIGN_STATE_3 )

#define CAP_INIT_STATE_BODY     ( CAP_INIT_DESIGN_CODE_1  | CAP_INIT_DESIGN_OPE_1   | CAP_INIT_DESIGN_STATE_1 \
                                | CAP_INIT_DESIGN_STATE_2 | CAP_INIT_DESIGN_STATE_4 | CAP_INIT_DESIGN_STATE_5 \
                                | CAP_INIT_DESIGN_STATE_6 )


#define CAP_INIT_GEOMETRIC      ( CAP_INIT_DESIGN_CODE_2  | CAP_INIT_DESIGN_OPE_2   | CAP_INIT_DESIGN_STATE_7 )


#define CAP_INIT_C0_CS          ( CAP_INIT_DISP_PICTURE   | CAP_INIT_DESIGN_CODE    | CAP_INIT_DESIGN_OPE     \
                                | CAP_INIT_DESIGN_STATE_1 | CAP_INIT_DESIGN_STATE_2 | CAP_INIT_DESIGN_STATE_3 \
                                | CAP_INIT_DESIGN_STATE_4 | CAP_INIT_DESIGN_STATE_5 | CAP_INIT_DESIGN_STATE_6 \
                                | CAP_INIT_DESIGN_STATE_7 )


#define CAP_INIT_CSI_SWF        ( CAP_INIT_DESIGN_CODE    | CAP_INIT_DESIGN_OPE     | CAP_INIT_DESIGN_STATE_1 \
                                | CAP_INIT_DESIGN_STATE_2 | CAP_INIT_DESIGN_STATE_4 | CAP_INIT_DESIGN_STATE_5 \
                                | CAP_INIT_DESIGN_STATE_6 | CAP_INIT_DESIGN_STATE_7 )

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/
typedef enum    tagARIBCC_ThreadState_E
{
    CC_THREAD_STOP      = 0x00,
    CC_THREAD_CREATE,
    CC_THREAD_WAIT,
    CC_THREAD_WAIT_DISP_START,
    CC_THREAD_WAIT_DISP_STOP,
    CC_THREAD_EXECUTE,
    CC_THREAD_EXIT,
    CC_THREAD_EXIT_COMPLETION,
    CC_THREAD_FAULT
} ARIBCC_ThreadState_E;


typedef struct  tagARIBCC_ManagementBodyInfo_S
{
    HI_U8   u8Format;
    HI_U8   u8Dummy[1];
    HI_U16  u16Width;
    HI_U16  u16Height;
    HI_U16  u16X;
    HI_U16  u16Y;
    HI_U16  u16Size;
    HI_U16  u16IntervalShs;
    HI_U16  u16IntervalSvs;
} ARIBCC_ManagementBody_S;

typedef struct  tagARIBCC_SwitchInfo_S
{
    HI_U8  u8Designate[4];
    HI_U8  u8Invoke[2];
    HI_U8  u8Dummy[2];
} ARIBCC_SwitchInfo_S;

typedef struct  tagARIBCC_CaptionInfo_S
{
    HI_BOOL                  bScrollFlg;
    HI_U8                    u8Dummy[1];
    HI_U8                    u8Format;
    HI_U16                   u16PaletteNum;
    ARIBCC_Management_S      stManagement;
    ARIBCC_ManagementBody_S  stManagementBody;
    ARIBCC_SwitchInfo_S      stSwitch;
} ARIBCC_CaptionInfo_S;


static HI_BOOL                 g_bDataFlg[CCPES_TYPE_MAX];/*if we receive caption data*/
static HI_BOOL                 g_bRcvManageFlg[CCPES_TYPE_MAX];/*if we receive management data group*/
static ARIBCC_CaptionInfo_S   gCaptionInfo[CCPES_TYPE_MAX];
static ARIBCC_ThreadState_E    s_enDispState[CCPES_TYPE_MAX] = {CC_THREAD_STOP, CC_THREAD_STOP};

typedef struct  tagAribCC_Macro_S
{
    HI_U8  au8Designate[4];
    HI_U8  au8Invoke[2];
}AribCC_Macro_S;

//CLUT    Ref[ARIB-TR-B14v2_8-vol3-p3-2-E2.pdf] Appendix-1,page 3-369
const HI_UNF_CC_COLOR_S g_stClut[8][16] =
{
    //palette 0,Index 0-15
    {{0,0,0,255},  {255,0,0,255},  {0,255,0,255},  {255,255,0,255},   //black,red,green,Yellow
     {0,0,255,255},{255,0,255,255},{0,255,255,255},{255,255,255,255}, //blue,Magenta,Cyan,White
     {0,0,0,0},    {170,0,0,255},  {0,170,0,255},  {170,170,0,255},   //Transparent,Half brightness Red,Half brightness Green,Half brightness Yellow
     {0,0,170,255},{170,0,170,255},{0,170,170,255},{170,170,170,255}},//Half brightness Blue,Half brightness magenta,Half brightness Cyan,Half brightness White(Gray)

    //palette 1,Index 16-31
    {{0,0,85,255},   {0,85,0,255},  {0,85,85,255},  {0,85,170,255},
     {0,85,255,255}, {0,170,85,255},{0,170,255,255},{0,255,85,255},
     {0,255,170,255},{85,0,0,255},  {85,0,85,255},  {85,0,170,255},
     {85,0,255,255}, {85,85,0,255}, {85,85,85,255}, {85,85,170,255}},

    //palette 2,Index 32-47
    {{85,85,255,255}, {85,170,0,255},  {85,170,85,255}, {85,170,170,255},
     {85,170,255,255},{85,255,0,255},  {85,255,85,255}, {85,255,170,255},
     {85,255,255,255},{170,0,85,255},  {170,0,255,255}, {170,85,0,255},
     {170,85,85,255}, {170,85,170,255},{170,85,255,255},{170,170,85,255}},

    //palette 3,Index 48-63
    {{170,170,255,255},{170,255,0,255},  {170,255,85,255}, {170,255,170,255},
     {170,255,255,255},{255,0,85,255},   {255,0,170,255},  {255,85,0,255},
     {255,85,85,255},  {255,85,170,255}, {255,85,255,255}, {255,170,0,255},
     {255,170,85,255}, {255,170,170,255},{255,170,255,255},{255,255,85,255}},

    //palette 4,Index 64-79
    {{255,255,170,255},{0,0,0,128},    {255,0,0,128},  {0,255,0,128},     //,,Black,Red,Green
     {255,255,0,128},  {0,0,255,128},  {255,0,255,128},{0,255,255,128},   //Yellow,blue,Magenta,Cyan
     {255,255,255,128},{170,0,0,128},  {0,170,0,128},  {170,170,0,128},   //White,Half brightness Red ,Half brightness Green,Half brightness Yellow
     {0,0,170,128},    {170,0,170,128},{0,170,170,128},{170,170,170,128}},//Half brightness Blue ,Half brightness magenta,Half brightness Cyan,Half brightness White(Gray)

    //palette 5,Index 80-95
    {{0,0,85,128},   {0,85,0,128},  {0,85,85,128},  {0,85,170,128},
     {0,85,255,128}, {0,170,85,128},{0,170,255,128},{0,255,85,128},
     {0,255,170,128},{85,0,0,128},  {85,0,85,128},  {85,0,170,128},
     {85,0,255,128}, {85,85,0,128}, {85,85,85,128}, {85,85,170,128}},

    //palette 6,Index:96-111
    {{85,85,255,128}, {85,170,0,128},  {85,170,85,128}, {85,170,170,128},
     {85,170,255,128},{85,255,0,128},  {85,255,85,128}, {85,255,170,128},
     {85,255,255,128},{170,0,85,128},  {170,0,255,128}, {170,85,0,128},
     {170,85,85,128}, {170,85,170,128},{170,85,255,128},{170,170,85,128}},

    //palette 7,Index 112-127
    {{170,170,255,128},{170,255,0,128},  {170,255,85,128}, {170,255,170,128},
     {170,255,255,128},{255,0,85,128},   {255,0,170,128},  {255,85,0,128},
     {255,85,85,128},  {255,85,170,128}, {255,85,255,128}, {255,170,0,128},
     {255,170,85,128}, {255,170,170,128},{255,170,255,128},{255,255,85,128}}
};

/*Ref[B24]  page 101*/
static  AribCC_Macro_S g_stMacro[16] =
{
    {{0x01,0x02,0x03,0x1D},{0x02,0x00}},// 6/0
    {{0x01,0x04,0x03,0x1D},{0x02,0x00}},// 6/1
    {{0x01,0x0E,0x03,0x1D},{0x02,0x00}},// 6/2
    {{0x05,0x07,0x08,0x1D},{0x02,0x00}},// 6/3
    {{0x05,0x06,0x08,0x1D},{0x02,0x00}},// 6/4
    {{0x05,0x0E,0x08,0x1D},{0x02,0x00}},// 6/5
    {{0x0E,0x0F,0x10,0x1D},{0x02,0x00}},// 6/6
    {{0x11,0x12,0x13,0x1D},{0x02,0x00}},// 6/7
    {{0x14,0x15,0x16,0x1D},{0x02,0x00}},// 6/8
    {{0x17,0x18,0x19,0x1D},{0x02,0x00}},// 6/9
    {{0x1A,0x1B,0x1C,0x1D},{0x02,0x00}},// 6/10
    {{0x01,0x0F,0x03,0x1D},{0x02,0x00}},// 6/11
    {{0x01,0x10,0x03,0x1D},{0x02,0x00}},// 6/12
    {{0x01,0x11,0x03,0x1D},{0x02,0x00}},// 6/13
    {{0x04,0x03,0x02,0x1D},{0x02,0x00}},// 6/14
    {{0x02,0x03,0x0E,0x1D},{0x02,0x00}},// 6/15
};

/**************************************************************************************
  static function
**************************************************************************************/

#ifdef ROLL_UP_STATE
static HI_S32 _ARIBCC_GetRollupState(ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_BOOL *pbRollup)
{
    HI_S32      s32Ret  = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8       u8LanguageIndex = 0;
    HI_S32      s32Format = 0;
    HI_S32      RollupMode = 0;

    if((pstManagement == HI_NULL) || (pbRollup == HI_NULL))
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    if(enCaptionType == ARIB_CAP_SUPERIMPOSE)
    {
         *pbRollup = HI_FALSE;
    }
    else if(enCaptionType == ARIB_CAP_CAPTION)
    {
        s32Ret  = ARIBCC_DEC_GetLanguageIndex(enCaptionType,pstObject->acISO639LanguageCode,&u8LanguageIndex);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to ARIBCC_DEC_GetLanguageIndex\n");
        }
        else
        {
            if(u8LanguageIndex > (CC_LANG_MAX-1))
            {
                HI_ERR_CC("u8LanguageIndex : %d\n", u8LanguageIndex);
                return HI_FAILURE;
            }
            s32Format = pstManagement->stLanguageData[u8LanguageIndex].u8Format;
            if( (s32Format == DF_HORI_960X540) || (s32Format == DF_HORI_720X480) )
            {
                RollupMode = pstManagement->stLanguageData[u8LanguageIndex].u8RollupMode;
                if(RollupMode == RM_ROLLUP)
                {
                    *pbRollup = HI_TRUE;
                }
                else
                {
                    *pbRollup = HI_FALSE;
                }
            }
            else
            {
                 *pbRollup = HI_FALSE;
            }
        }
    }
    else
    {
        HI_ERR_CC("enCaptionType : %d\n", enCaptionType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}
#endif

#ifdef INIT_STATE
static HI_S32 _ARIBCC_InitDispPicture (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_BOOL bRollupFlg  = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    if((enCaptionType == ARIB_CAP_CAPTION) && (u32Operation == CAP_INIT_REVISE_CONTROL))
    {
        s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to _ARIBCC_GetRollupState\n");
            return HI_FAILURE;
        }

        if( bRollupFlg != HI_FALSE)
        {
            ARIBCC_OSD_SetRollupMode(1);
        }
        else
        {
            ARIBCC_OSD_SetRollupMode(0);
        }
    }

    ARIBCC_OSD_SetWinColor((HI_S32)enCaptionType,0,0,0,0);

    return HI_SUCCESS;
}


static HI_S32 _ARIBCC_InitDefineData1 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    ARIBCC_OSD_DrcsClear((HI_S32)enCaptionType, 0);

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDefineData2 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    ARIBCC_OSD_DrcsClear((HI_S32)enCaptionType, 2);

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignCode1 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_U8              Designate[4];
    HI_U8              Invoke[2];

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    gCaptionInfo[enCaptionType].stSwitch.u8Designate[0] = Designate[0] = ARIBCC_CHARSET_ALPHANUMERIC;   // G0 :  Alphanumeric
    gCaptionInfo[enCaptionType].stSwitch.u8Designate[1] = Designate[1] = ARIBCC_CHARSET_ALPHANUMERIC;   // G1 :  Alphanumeric
    gCaptionInfo[enCaptionType].stSwitch.u8Designate[2] = Designate[2] = ARIBCC_CHARSET_LATIN;          // G2 :  Latin Extension
    gCaptionInfo[enCaptionType].stSwitch.u8Designate[3] = Designate[3] = ARIBCC_CHARSET_SPECIAL;        // G3 : Special character
    gCaptionInfo[enCaptionType].stSwitch.u8Invoke[0]   = Invoke[0]    = ARIBCC_CODESET_G2;              // GR : G2(LS2R)
    gCaptionInfo[enCaptionType].stSwitch.u8Invoke[1]   = Invoke[1]    = ARIBCC_CODESET_G0;              // GL : G0(LS0)

    ARIBCC_OSD_SetCharacterCode((HI_S32)enCaptionType,&Designate[0],&Invoke[0]);

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignCode2 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignOpe1 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_BOOL bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_GetRollupState\n");
        return HI_FAILURE;
    }

    if( ((u32Operation == CAP_INIT_STATE_BODY) && (bRollupFlg != HI_FALSE)) ||
        ((u32Operation == CAP_INIT_C0_CS)      && (bRollupFlg != HI_FALSE))  )
    {
        HI_INFO_CC("Roolup Flag ON. Not Initialize APS&RPC\n");
    }
    else
    {
        ARIBCC_OSD_SetCursorPosition((HI_S32)enCaptionType,7,0,0,0,0);
    }

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignOpe2 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState1 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8   u8LanguageIndex = 0;
    HI_U8   s32Format = 0xff;
    HI_BOOL bSetFlg = HI_FALSE;
    HI_BOOL bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_GetRollupState\n");
        return HI_FAILURE;
    }

    if( (u32Operation == CAP_INIT_STATE_BODY) && (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Roolup Flag ON. Not Initialize SWF\n");
        return HI_SUCCESS;
    }

    if((u32Operation == CAP_INIT_STATE_BODY) || (u32Operation == CAP_INIT_C0_CS))
    {
        if(gCaptionInfo[enCaptionType].stManagementBody.u8Format != 0xFF)
        {
            s32Format = gCaptionInfo[enCaptionType].stManagementBody.u8Format;
            bSetFlg = HI_TRUE;
        }
        else
        {
            bSetFlg = HI_FALSE;
        }
    }

    if( (u32Operation  == CAP_INIT_REVISE_CONTROL)
        || ((u32Operation == CAP_INIT_STATE_BODY)  && (bSetFlg == HI_FALSE))
        || ((u32Operation == CAP_INIT_C0_CS)       && (bSetFlg == HI_FALSE)) )
    {
        s32Ret = ARIBCC_DEC_GetLanguageIndex(enCaptionType, pstObject->acISO639LanguageCode,&u8LanguageIndex);
        if (s32Ret == HI_SUCCESS)
        {
            if(u8LanguageIndex > (CC_LANG_MAX-1))
            {
                HI_ERR_CC("u8LanguageIndex : %d\n", u8LanguageIndex);
                return HI_FAILURE;
            }
            switch( pstManagement->stLanguageData[u8LanguageIndex].u8Format )
            {
                case DF_HORI_960X540:
                    s32Format = 0;
                    break;
                case DF_VERT_960X540:
                    s32Format = 1;
                    break;
                case DF_HORI_720X480:
                    s32Format = 2;
                    break;
                case DF_VERT_720X480:
                    s32Format = 3;
                    break;
                default:
                    HI_ERR_CC("format = %d not support\n", pstManagement->stLanguageData[u8LanguageIndex].u8Format);
                    s32Ret = HI_FAILURE;
                    break;
            }
        }
    }

    if (s32Ret == HI_SUCCESS)
    {
        if( u32Operation == CAP_INIT_CSI_SWF )
        {
            HI_INFO_CC("CAP_INIT_CSI_SWF:Not Initialize SWF\n");
        }
        else
        {
            ARIBCC_OSD_SetWritingFormat((HI_S32)enCaptionType,s32Format);
        }
    }

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState2 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    HI_BOOL bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_GetRollupState\n");
        return HI_FAILURE;
    }

    if( bRollupFlg != HI_FALSE )
    {
        HI_INFO_CC("Roolup Flag ON. Not Initialize SCR\n");
        return HI_SUCCESS;
    }

    ARIBCC_OSD_SetScrollInfo((HI_S32)enCaptionType,0,0);
    gCaptionInfo[enCaptionType].bScrollFlg = HI_FALSE;

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState3 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState4 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8   u8LanguageIndex = 0;
    HI_U8   s32Format = 0xFF;
    HI_S32  IntervalShs = -1;
    HI_S32  IntervalSvs = -1;
    HI_BOOL bSetFlg = HI_FALSE;
    HI_BOOL bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_GetRollupState\n");
        return HI_FAILURE;
    }

    if( (u32Operation == CAP_INIT_STATE_BODY) && (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Roolup Flag ON. Not Initialize SHS&SVS\n");
        return HI_SUCCESS;
    }

    if((u32Operation == CAP_INIT_STATE_BODY) || (u32Operation == CAP_INIT_C0_CS) || (u32Operation == CAP_INIT_CSI_SWF))
    {
        if( (gCaptionInfo[enCaptionType].stManagementBody.u16IntervalShs != 0xFFFF) &&
            (gCaptionInfo[enCaptionType].stManagementBody.u16IntervalSvs != 0xFFFF)  )
        {
            IntervalShs = (HI_S32)gCaptionInfo[enCaptionType].stManagementBody.u16IntervalShs;
            IntervalSvs = (HI_S32)gCaptionInfo[enCaptionType].stManagementBody.u16IntervalSvs;
            bSetFlg = HI_TRUE;
        }
        else
        {
            bSetFlg = HI_FALSE;
        }
    }

    if( (u32Operation == CAP_INIT_REVISE_CONTROL)
        || ((u32Operation == CAP_INIT_STATE_BODY) && (bSetFlg == HI_FALSE))
        || ((u32Operation == CAP_INIT_C0_CS) && (bSetFlg == HI_FALSE))
        || ((u32Operation == CAP_INIT_CSI_SWF) && (bSetFlg == HI_FALSE)) )
    {
        s32Ret = ARIBCC_DEC_GetLanguageIndex(enCaptionType, pstObject->acISO639LanguageCode,&u8LanguageIndex);
        if (s32Ret == HI_SUCCESS)
        {
            if(u8LanguageIndex > (CC_LANG_MAX-1))
            {
                HI_ERR_CC("u8LanguageIndex : %d\n", u8LanguageIndex);
                return HI_FAILURE;
            }

            s32Format = pstManagement->stLanguageData[u8LanguageIndex].u8Format;

            if(u32Operation == CAP_INIT_CSI_SWF)
            {
                if( gCaptionInfo[enCaptionType].u8Format == 0 )
                {
                    s32Format = DF_HORI_960X540;
                }
                else if( gCaptionInfo[enCaptionType].u8Format == 1 )
                {
                    s32Format = DF_VERT_960X540;
                }
                else if( gCaptionInfo[enCaptionType].u8Format == 2 )
                {
                    s32Format = DF_HORI_720X480;
                }
                else if( gCaptionInfo[enCaptionType].u8Format == 3 )
                {
                    s32Format = DF_VERT_720X480;
                }
                else
                {
                    //HI_ERR_CC("SWF s32Format Error:CcType=%d SWF=%d\n", enCaptionType, s32Format);
                }
            }

            switch( s32Format )
            {
                case DF_HORI_960X540:
                    IntervalShs = 4;
                    IntervalSvs = 24;
                    break;
                case DF_VERT_960X540:
                    IntervalShs = 12;
                    IntervalSvs = 24;
                    break;
                case DF_HORI_720X480:
                    IntervalShs = 4;
                    IntervalSvs = 16;
                    break;
                case DF_VERT_720X480:
                    IntervalShs = 8;
                    IntervalSvs = 24;
                    break;
                default:
                    HI_ERR_CC("s32Format = %d not support\n",s32Format);
                    s32Ret = HI_FAILURE;
                    break;
            }
        }
    }

    if (s32Ret == HI_SUCCESS)
    {
        ARIBCC_OSD_SetHorizontalSpacing((HI_S32)enCaptionType,IntervalShs);

        ARIBCC_OSD_SetVerticalSpacing((HI_S32)enCaptionType,IntervalSvs);
    }

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState5 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8   u8LanguageIndex = 0;
    HI_U16  u16X     = 0xFF;
    HI_U16  u16Y     = 0xFF;
    HI_U16  u16Size  = 0xFF;
    HI_S32  Width    = -1;
    HI_S32  Height   = -1;
    HI_BOOL bSetFlg  = HI_FALSE;
    HI_BOOL bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed  to _ARIBCC_GetRollupState\n");
        return HI_FAILURE;
    }

    if( (u32Operation == CAP_INIT_STATE_BODY) && (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("[%4.4d]Roolup Flag ON. Not Initialize SDF&SDP&SSM\n", __LINE__);
        return HI_SUCCESS;
    }

    if(u32Operation == CAP_INIT_REVISE_CONTROL)
    {
        u16Size = SSM_SIZE_36;
        u16X = u16Y = 0;
    }

    if((u32Operation == CAP_INIT_STATE_BODY) || (u32Operation == CAP_INIT_C0_CS) || (u32Operation == CAP_INIT_CSI_SWF))
    {
        if( (gCaptionInfo[enCaptionType].stManagementBody.u16Width    != 0xFFFF)  &&
            (gCaptionInfo[enCaptionType].stManagementBody.u16Height   != 0xFFFF)  )
        {
            Width   = (HI_S32)gCaptionInfo[enCaptionType].stManagementBody.u16Width;
            Height  = (HI_S32)gCaptionInfo[enCaptionType].stManagementBody.u16Height;
            bSetFlg  = HI_TRUE;
        }
        else
        {
            bSetFlg = HI_FALSE;
        }

        if( (gCaptionInfo[enCaptionType].stManagementBody.u16X != 0xFFFF)  &&
            (gCaptionInfo[enCaptionType].stManagementBody.u16Y != 0xFFFF)  )
        {
            u16X = gCaptionInfo[enCaptionType].stManagementBody.u16X;
            u16Y = gCaptionInfo[enCaptionType].stManagementBody.u16Y;
            bSetFlg  = HI_TRUE;
        }
        else
        {
            u16X = u16Y = 0;
        }

        if( gCaptionInfo[enCaptionType].stManagementBody.u16Size != 0xFFFF  )
        {
            u16Size = gCaptionInfo[enCaptionType].stManagementBody.u16Size;
        }
        else
        {
            u16Size    = SSM_SIZE_36;
        }
    }

    if( (u32Operation == CAP_INIT_REVISE_CONTROL)                           ||
        ((u32Operation == CAP_INIT_STATE_BODY)  && (bSetFlg == HI_FALSE))   ||
        ((u32Operation == CAP_INIT_C0_CS)       && (bSetFlg == HI_FALSE))   ||
        ((u32Operation == CAP_INIT_CSI_SWF)     && (bSetFlg == HI_FALSE))   )
    {
        s32Ret = ARIBCC_DEC_GetLanguageIndex(enCaptionType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
        if(u8LanguageIndex > (CC_LANG_MAX-1))
        {
            HI_ERR_CC("u8LanguageIndex : %d\n", u8LanguageIndex);
            return HI_FAILURE;
        }
        if (s32Ret == HI_SUCCESS)
        {
            HI_U32 U32Format = pstManagement->stLanguageData[u8LanguageIndex].u8Format;
            switch( U32Format )
            {
                case DF_HORI_960X540:
                case DF_VERT_960X540:
                    Width   = 960;
                    Height  = 540;
                    break;
                case DF_HORI_720X480:
                case DF_VERT_720X480:
                    Width   = 720;
                    Height  = 480;
                    break;
                default:
                    HI_ERR_CC("U32Format = %d not support\n",U32Format);
                    s32Ret = HI_FAILURE;
                    break;
            }
        }
    }

    if (s32Ret == HI_SUCCESS)
    {
        if (u16X == 0 || u16Y == 0)
        {
            u16X   = Width / 10;
            u16Y   = Height / 10;
            Width  = Width * 8 / 10;
            Height = Height * 8 / 10;
        }
        DISP_TRACE("\tInit window, CCType=%d u16X=%d,u16Y=%d,Width=%d,Height=%d\n",
                       enCaptionType, u16X, u16Y, Width, Height);
        ARIBCC_OSD_SetWindowPos((HI_S32)enCaptionType,(HI_S32)u16X,(HI_S32)u16Y);
        ARIBCC_OSD_SetWindowSize((HI_S32)enCaptionType,Width,Height);
        ARIBCC_OSD_SetCharacterSize((HI_S32)enCaptionType,(HI_S32)u16Size);
    }

    return HI_SUCCESS;
}

static HI_S32 _ARIBCC_InitDesignState6 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32   s32Ret = HI_SUCCESS;
    HI_BOOL  bRollupFlg = HI_FALSE;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("Null Pointer Error\n");
        return HI_FAILURE;
    }


    s32Ret = _ARIBCC_GetRollupState(enCaptionType, pstManagement, &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("[%4.4d]Get Rollup Status Error : ComErrCode=%ld\n", __LINE__, s32Ret);
        return s32Ret;
    }

    ARIBCC_OSD_SetSize((HI_S32)enCaptionType,0);
    gCaptionInfo[enCaptionType].u16PaletteNum = 0;
    ARIBCC_OSD_SetTextColor((HI_S32)enCaptionType,0,255,255,255,255);
    ARIBCC_OSD_SetTextColor((HI_S32)enCaptionType,1,170,170,170,255);

    ARIBCC_OSD_SetTextColor((HI_S32)enCaptionType,2,0,0,0,0);
    ARIBCC_OSD_SetTextColor((HI_S32)enCaptionType,3,85,85,85,255);
    ARIBCC_OSD_SetOrnament((HI_S32)enCaptionType,0,0,0,0,0);

    if(bRollupFlg != HI_FALSE)
    {
        HI_INFO_CC("[%4.4d]Roolup Flag ON. Not Initialize FLC&SPL&HLC&POL\n", __LINE__);
    }
    else
    {
        /* Initialize FLC&SPL&HLC&POL */
        ARIBCC_OSD_SetFlash((HI_S32)enCaptionType,0);
        ARIBCC_OSD_Underline((HI_S32)enCaptionType,HI_FALSE);

        ARIBCC_OSD_SetCharBox((HI_S32)enCaptionType,HI_FALSE,HI_FALSE,HI_FALSE,HI_FALSE);
        ARIBCC_OSD_SetPatternPolarity((HI_S32)enCaptionType,0);

    }

    return s32Ret;
}

static HI_S32 _ARIBCC_InitDesignState7 (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("[%4.4d]Null Pointer Error\n", __LINE__);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Ref[B24],part3,Chapter 8,Table 8-2 Initial status. */
static HI_S32 _ARIBCC_InitOperation (ARIBCC_CaptionType_E enCaptionType, ARIBCC_Management_S *pstManagement, HI_U32 u32Operation)
{
    HI_S32  s32Ret = HI_SUCCESS;

    if(pstManagement == HI_NULL)
    {
        HI_ERR_CC("Null Pointer Error\n");
        return HI_FAILURE;
    }

    if (u32Operation & CAP_INIT_DISP_PICTURE)
    {
        s32Ret |= _ARIBCC_InitDispPicture(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DEFINE_DATA_1)
    {
        s32Ret |= _ARIBCC_InitDefineData1(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DEFINE_DATA_2)
    {
        s32Ret |= _ARIBCC_InitDefineData2(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_CODE_1)
    {
        s32Ret |= _ARIBCC_InitDesignCode1(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_CODE_2)
    {
        s32Ret |= _ARIBCC_InitDesignCode2(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_OPE_1)
    {
        s32Ret |= _ARIBCC_InitDesignOpe1(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_OPE_2)
    {
        s32Ret |= _ARIBCC_InitDesignOpe2(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_1)
    {
        s32Ret |= _ARIBCC_InitDesignState1(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_2)
    {
        s32Ret |= _ARIBCC_InitDesignState2(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_3)
    {
        s32Ret |= _ARIBCC_InitDesignState3(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_4)
    {
        s32Ret |= _ARIBCC_InitDesignState4(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_5)
    {
        s32Ret |= _ARIBCC_InitDesignState5(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_6)
    {
        s32Ret |= _ARIBCC_InitDesignState6(enCaptionType, pstManagement, u32Operation);
    }

    if (u32Operation & CAP_INIT_DESIGN_STATE_7)
    {
        s32Ret |= _ARIBCC_InitDesignState7(enCaptionType, pstManagement, u32Operation);
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("Init operation failure\n");
    }

    return s32Ret;
}
#endif

static HI_S32 _ARIBCC_8BitCode_Position(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret          = HI_SUCCESS;
    HI_U8      u8CursorCmd     = 0xFF;
    HI_U8      u8PositionX     = 0xFF;
    HI_U8      u8PositionY     = 0XFF;
    HI_U8      u8PositionRow   = 0xFF;
    HI_U8      u8PositionCol   = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC("Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    /*only 'rollup_mode' field of caption management data is 0x01, the language is recognized as roll-up mode*/
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE) || (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Scroll Flag ON in Position Control!\n");
        return HI_SUCCESS;
    }
#endif
    switch( pstControl->enControlId )
    {
        case ARIB_C0_APB:   //active position back
            u8CursorCmd = SET_CURSOR_APB;
            break;
        case ARIB_C0_APF:   //active position forward
            u8CursorCmd = SET_CURSOR_APF;
            break;
        case ARIB_C0_APD:   //avctive position down
            u8CursorCmd = SET_CURSOR_APD;
            break;
        case ARIB_C0_APU:   //avtive position up
            u8CursorCmd = SET_CURSOR_APU;
            break;
        case ARIB_C0_PAPF:  // Parameterized active position forward
            u8CursorCmd = SET_CURSOR_PAPF;
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            u8PositionCol = (HI_U8)ps32ControlData[0];
            if(u8PositionCol > PAPF_NUMBER_MAX)
            {
                HI_ERR_CC("PAPF Number Error : CcType=%d u8FowardNum=%d\n",enInCCType,u8PositionCol);
                return HI_FAILURE;
            }
            break;
        case ARIB_C0_APR:   //Active position return
            u8CursorCmd = SET_CURSOR_APR;
            break;
        case ARIB_C0_APS:   //Active position set
            u8CursorCmd = SET_CURSOR_APS;
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            u8PositionRow = (HI_U8)ps32ControlData[0];
            u8PositionCol = (HI_U8)ps32ControlData[1];

            if((u8PositionRow > APS_NUMBER_MAX) || (u8PositionCol > APS_NUMBER_MAX))
            {
                HI_ERR_CC("APS Number Error : CcType=%d u8PositionRow=%d u8PositionCol=%d\n",enInCCType,u8PositionRow,u8PositionCol);
                return HI_FAILURE;
            }
            break;
        case ARIB_CSI_ACPS: //Active Coordinate Position Set
            u8CursorCmd = SET_CURSOR_ACPS;
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;
            u8PositionX = (HI_U8)ps32ControlData[0];
            u8PositionY = (HI_U8)ps32ControlData[1];

            break;
        default:
            HI_ERR_CC("Control Id error : %d!\n",pstControl->enControlId);
            return HI_FAILURE;
    }

    if(u8CursorCmd == SET_CURSOR_APR)/*active position return*/
    {
        ARIBCC_OSD_Show((HI_S32)enInCCType);
    }
    ARIBCC_OSD_SetCursorPosition((HI_S32)enInCCType,(HI_S32)u8CursorCmd,(HI_S32)u8PositionRow,(HI_S32)u8PositionCol,(HI_S32)u8PositionX,(HI_S32)u8PositionY);


    return s32Ret;
}


//*********************************************************************************************************************


//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Cs(ARIBCC_CaptionType_E enInCCType)
{
    ARIBCC_OSD_ClearScreen((HI_S32)enInCCType);
    return HI_SUCCESS;
}


//*********************************************************************************************************************


//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_SwitchControl(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      au8Designate[4];
    HI_U8      au8Invoke[2];
    HI_U16     i  = 0;
    HI_S32     *ps32ControlData = HI_NULL;

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if(bRollupFlg == HI_TRUE)
    {
        HI_INFO_CC("Scroll Flag ON in Position Control!\n");
        return HI_SUCCESS;
    }
#endif

    au8Designate[ARIBCC_CODESET_G0] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0];  //desinate G0
    au8Designate[ARIBCC_CODESET_G1] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1];  //desinate G1
    au8Designate[ARIBCC_CODESET_G2] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2];  //desinate G2
    au8Designate[ARIBCC_CODESET_G3] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3];  //desinate G3

    au8Invoke[ARIBCC_GR]    = gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR];     //invoke GR
    au8Invoke[ARIBCC_GL]    = gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL];     //invoke GL

    switch( pstControl->enControlId )
    {
        case ARIB_C0_LS0:   //invoke G0 to GL
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = 0x00;  // GL<-G0
            break;
        case ARIB_C0_LS1:   //invoke G1 to GL
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = 0x01;  // GL<-G1
            break;
        case ARIB_ESC_LS2:  //invoke G2 to GL
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = 0x02;  // GL<-G2
            break;
        case ARIB_ESC_LS3:  //invoke G3 to GL
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = 0x03;  // GL<-G3
            break;
        case ARIB_ESC_LS1R: //invoke G1 to GR
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR] = au8Invoke[ARIBCC_GR] = 0x01;  // GR<-G1
            break;
        case ARIB_ESC_LS2R: //invoke G2 to GR
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR] = au8Invoke[ARIBCC_GR] = 0x02;  // GR<-G2
            break;
        case ARIB_ESC_LS3R: //invoke G3 to GR
            gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR] = au8Invoke[ARIBCC_GR] = 0x03;  // GR<-G3
            break;
        case ARIB_ESC_DES:  //desinate code
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            //code set
            switch( *ps32ControlData )
            {
                case ARIB_CELEMENT_G0:// G0 code set
                    i = ARIBCC_CODESET_G0;
                    break;
                case ARIB_CELEMENT_G1:// G1 code set
                    i = ARIBCC_CODESET_G1;
                    break;
                case ARIB_CELEMENT_G2:// G2 code set
                    i = ARIBCC_CODESET_G2;
                    break;
                case ARIB_CELEMENT_G3://G3 code set
                    i = ARIBCC_CODESET_G3;
                    break;
                default:
                    HI_ERR_CC("ARIB_ESC_DES Error : CcType=%d ARIB_CELEMENT=0x%02x\n",enInCCType, *ps32ControlData);
                    return HI_FAILURE;
            }

            //charater set
            switch( *(ps32ControlData+1) )
            {
                case ARIB_GSET_KANJI:
                    au8Designate[i] = 0x01;
                    break;
                case ARIB_GSET_ALPHANUMERIC:
                    au8Designate[i] = 0x02;
                    break;
                case ARIB_GSET_HIRAGANA:
                    au8Designate[i] = 0x03;
                    break;
                case ARIB_GSET_KATAKANA:
                    au8Designate[i] = 0x04;
                    break;
                case ARIB_GSET_LATIN_EXTENSION:
                    au8Designate[i] = 0x21;
                    HI_INFO_CC("\n\n\n###### Latin extension #####\n\n\n");
                    break;
                case ARIB_GSET_SPECIAL:
                    au8Designate[i] = 0x22;
                    HI_INFO_CC("\n\n\n###### Special #####\n\n\n");
                    break;
#if 0
                case ARIB_GSET_MOSAIC_A:
                    au8Designate[i] = 0x05;
                    break;
                case ARIB_GSET_MOSAIC_B:
                    au8Designate[i] = 0x06;
                    break;
                case ARIB_GSET_MOSAIC_C:
                    au8Designate[i] = 0x07;
                    break;
                case ARIB_GSET_MOSAIC_D:
                    au8Designate[i] = 0x08;
                    break;
                case ARIB_GSET_P_ALPHANUMERIC:
                    au8Designate[i] = 0x09;
                    break;
                case ARIB_GSET_P_HIRAGANA:
                    au8Designate[i] = 0x0A;
                    break;
                case ARIB_GSET_P_KATAKANA:
                    au8Designate[i] = 0x0B;
                    break;
#endif
                case ARIB_GSET_JIS_X0201_KATAKANA:
                    au8Designate[i] = 0x0C;
                    break;
                case ARIB_GSET_DRCS_0:// DRCS-0
                    au8Designate[i] = 0x0D;
                    break;
                case ARIB_GSET_DRCS_1:// DRCS-1
                    au8Designate[i] = 0x0E;
                    break;
                case ARIB_GSET_DRCS_2:// DRCS-2
                    au8Designate[i] = 0x0F;
                    break;
                case ARIB_GSET_DRCS_3:// DRCS-3
                    au8Designate[i] = 0x10;
                    break;
                case ARIB_GSET_DRCS_4:// DRCS-4
                    au8Designate[i] = 0x11;
                    break;
                case ARIB_GSET_DRCS_5:// DRCS-5
                    au8Designate[i] = 0x12;
                    break;
                case ARIB_GSET_DRCS_6:// DRCS-6
                    au8Designate[i] = 0x13;
                    break;
                case ARIB_GSET_DRCS_7:// DRCS-7
                    au8Designate[i] = 0x14;
                    break;
                case ARIB_GSET_DRCS_8:// DRCS-8
                    au8Designate[i] = 0x15;
                    break;
                case ARIB_GSET_DRCS_9:// DRCS-9
                    au8Designate[i] = 0x16;
                    break;
                case ARIB_GSET_DRCS_10:// DRCS-10
                    au8Designate[i] = 0x17;
                    break;
                case ARIB_GSET_DRCS_11:// DRCS-11
                    au8Designate[i] = 0x18;
                    break;
                case ARIB_GSET_DRCS_12:// DRCS-12
                    au8Designate[i] = 0x19;
                    break;
                case ARIB_GSET_DRCS_13:// DRCS-13
                    au8Designate[i] = 0x1A;
                    break;
                case ARIB_GSET_DRCS_14:// DRCS-14
                    au8Designate[i] = 0x1B;
                    break;
                case ARIB_GSET_DRCS_15:// DRCS-15
                    au8Designate[i] = 0x1C;
                    break;
                case ARIB_GSET_MACRO:
                    au8Designate[i] = 0x1D;
                    break;
                case ARIB_GSET_JIS_INTERCHANGE_KANJI_1:
                    au8Designate[i] = 0x1E;
                    break;
                case ARIB_GSET_JIS_INTERCHANGE_KANJI_2:
                    au8Designate[i] = 0x1F;
                    break;
                case ARIB_GSET_ADDITION_SIGN:
                    au8Designate[i] = 0x20;
                    break;
                default:
                    HI_ERR_CC("ARIB_ESC_DES Error : CcType=%d ARIB_GSET=0x%04x\n",enInCCType,*(ps32ControlData+1));
                    return HI_FAILURE;
            }

            gCaptionInfo[enInCCType].stSwitch.u8Designate[i] = au8Designate[i];
            break;
        default:
            HI_ERR_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetCharacterCode((HI_S32)enInCCType,au8Designate,au8Invoke);  // CC Graphic API

    return s32Ret;
}

//*************************************************************************************
//see [B24,Vol 1,Part2,Chapter 7,Table 7-16]page 91 and page 92

//********************************************* ***************************************
static HI_S32 _ARIBCC_8BitCode_TxtColor(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret           = HI_SUCCESS;
    HI_U8      u8ColorPlace     = 0xFF;
    HI_U8      u8Red            = 0xFF;
    HI_U8      u8Green          = 0xFF;
    HI_U8      u8Blue           = 0xFF;
    HI_U8      u8Alpha          = 0xFF;
    HI_U8      u8CMLA           = 0;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    switch( pstControl->enControlId )
    {
        case ARIB_C1_BKF:
            u8CMLA = 0;
            break;
        case ARIB_C1_RDF:
            u8CMLA = 1;
            break;
        case ARIB_C1_GRF:
            u8CMLA = 2;
            break;
        case ARIB_C1_YLF:
            u8CMLA = 3;
            break;
        case ARIB_C1_BLF:
            u8CMLA = 4;
            break;
        case ARIB_C1_MGF:
            u8CMLA = 5;
            break;
        case ARIB_C1_CNF:
            u8CMLA = 6;
            break;
        case ARIB_C1_WHF:
            u8CMLA = 7;
            break;
        case ARIB_C1_COL_1://color control 1
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            switch( *ps32ControlData & (HI_S32)(0xF0) )
            {
                case 0x40://forground color
                    u8ColorPlace = FG_COLOR_PLACE;
                    break;
                case 0x50://background color
                    u8ColorPlace = BG_COLOR_PLACE;
                    break;
                case 0x60://half forground color
                    u8ColorPlace = FG_MID_COLOR_PLACE;
                    break;
                case 0x70://half background color
                    u8ColorPlace = BG_MID_COLOR_PLACE;
                    break;
                default:
                    HI_ERR_CC("ARIB_C1_COL_1 Error : CcType=%d Place=0x%08x\n",enInCCType, *ps32ControlData);
                    return HI_FAILURE;
            }
            u8CMLA =  (HI_U8)(*ps32ControlData & (HI_S32)0x0F);
            break;

        case ARIB_C1_COL_2:
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            if(*ps32ControlData <= PALETTE_NUM_7)
            {
                gCaptionInfo[enInCCType].u16PaletteNum = (HI_U8)*ps32ControlData;
            }
            else
            {
                HI_ERR_CC("ARIB_C1_COL_2 Palette Number Error : CcType=%d Number=%d\n",enInCCType, *ps32ControlData);
                return HI_FAILURE;
            }
            break;
        default:
            HI_ERR_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            return HI_FAILURE;
    }


    if(pstControl->enControlId != ARIB_C1_COL_1)
    {
        u8ColorPlace = FG_COLOR_PLACE;
    }

    u8Red     = g_stClut[gCaptionInfo[enInCCType].u16PaletteNum][u8CMLA].u8Red;
    u8Green   = g_stClut[gCaptionInfo[enInCCType].u16PaletteNum][u8CMLA].u8Green;
    u8Blue    = g_stClut[gCaptionInfo[enInCCType].u16PaletteNum][u8CMLA].u8Blue;
    u8Alpha   = g_stClut[gCaptionInfo[enInCCType].u16PaletteNum][u8CMLA].u8Alpha;

    ARIBCC_OSD_SetTextColor((HI_S32)enInCCType,u8ColorPlace,u8Red,u8Green,u8Blue,u8Alpha);// CC Graphic API

    return s32Ret;
}

//************************************************************************************

//************************************************************************************
static HI_S32 _ARIBCC_8BitCode_TextSize(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret         = HI_SUCCESS;
    HI_U16     u16Size        = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }


    if((pstControl->enControlId == ARIB_C1_SZX) && (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE))
    {
        HI_INFO_CC("Scroll Flag ON SZX: CcType=%d\n",enInCCType);
        return s32Ret;
    }
#ifdef ROLL_UP_STATE

    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#endif
    switch( pstControl->enControlId )
    {
        case ARIB_C1_NSZ://normal size
            u16Size = 0;
            break;
        case ARIB_C1_SSZ://small size
            u16Size = 1;
            break;
        case ARIB_C1_MSZ://middle size
            u16Size = 2;
            break;
        case ARIB_C1_SZX:// set character size
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;

            switch( *ps32ControlData )
            {
                case SZX_LENGTH_TWICE:      //double hight
                    u16Size = 3;
                    break;
                case SZX_SIDE_TWICE:        //double width
                    u16Size = 4;
                    break;
                case SZX_LENGTH_SIDE_TWICE: //double height and width
                    u16Size = 5;
                    break;
                default:
                    HI_ERR_CC("ARIB_C1_SZX Error : CcType=%d Size=0x%02x\n", enInCCType, *ps32ControlData);
                    return HI_FAILURE;
            }
            break;
        default:
            HI_ERR_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetSize((HI_S32)enInCCType,(HI_S32)u16Size);// CC Graphic API

    return s32Ret;
}

//****************************************************************************************
 // Flashing control

//*****************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Flc(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret           = HI_SUCCESS;
    HI_U16     u16FLCRequest    = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE) || (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Scroll Flag ON in Position Control!\n");
        return HI_SUCCESS;
    }

#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case FLC_END:
            u16FLCRequest = 0;
            break;
        case FLC_NORMAL:
            u16FLCRequest = 1;
            break;
        default:
            HI_ERR_CC("ARIB_C1_FLC Error : CcType=%d Request=0x%02x\n", enInCCType, *ps32ControlData);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetFlash((HI_S32)enInCCType,(HI_S32)u16FLCRequest);// CC Graphic API

    return s32Ret;
}

//*****************************************************************************************
//TIME code,only operate wait for process,Ref[B24,Vol 1],page 94
//*****************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Time(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_U32   u32WaitTime = 0;
    HI_S32   *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if(gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE)
    {
        HI_INFO_CC("Scroll Flag ON TIME code : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    switch( pstControl->enControlId )
    {
        //only operate wait for process
        case ARIB_C1_TIME_1:
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;
            if((*ps32ControlData < TIME_NUMBER_MIN) || ((*ps32ControlData > TIME_NUMBER_MAX)))
            {
                HI_ERR_CC("ARIB_C1_TIME_1 Wait Time Error CcType=%d WaitTime=%d[0.1s]\n",enInCCType, *ps32ControlData);
                return HI_FAILURE;
            }
            else
            {
                u32WaitTime = (HI_U32)(*ps32ControlData) * 100000UL;
                usleep(u32WaitTime);
            }
            break;

        default:
            HI_WARN_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            break;
    }

    return HI_SUCCESS;
}

//*************************************************************************************
//Pattern Polarity Controls
//**************************************************************************************
HI_S32 _ARIBCC_8BitCode_Pol(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret           = HI_SUCCESS;
    HI_U8      u8ColorPattern   = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( bRollupFlg != HI_FALSE )
    {
        HI_INFO_CC("Rollup Flag ON Pol : CcType=%d\n",enInCCType);
        return s32Ret;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case POL_NORMAL:
            u8ColorPattern = ARIBCC_COLORPATTERN_NORMAL_POL;
            break;
        case POL_REVERSING_1:
            u8ColorPattern = ARIBCC_COLORPATTERN_REVERSE_POL1;
            break;

        default:
            HI_ERR_CC("ARIB_C1_POL Error : CcType=%d ARIBCC_COLORPATTERN_E=0x%02x\n", enInCCType, *ps32ControlData);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetPatternPolarity((HI_S32)enInCCType,u8ColorPattern);    // CC Graphic API

    return s32Ret;
}

//********************************************************************************
//Highlighting Character Block
//********************************************************************************
static HI_S32 _ARIBCC_8BitCode_Hlc(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret           = HI_SUCCESS;
    HI_BOOL    bbottom          = HI_FALSE;
    HI_BOOL    bright           = HI_FALSE;
    HI_BOOL    btop             = HI_FALSE;
    HI_BOOL    bleft            = HI_FALSE;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE

    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE) || (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Scroll Flag ON in Position Control!\n");
        return HI_SUCCESS;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case HLC_PATTERN_0:
            bbottom = bright = btop = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_1:
            bbottom = HI_TRUE;
            bright = btop = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_2:
            bright = HI_TRUE;
            bbottom = btop = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_3:
            bbottom = bright = HI_TRUE;
            btop= bleft = HI_FALSE;
            break;
        case HLC_PATTERN_4:
            btop = HI_TRUE;
            bbottom = bright = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_5:
            bbottom = btop = HI_TRUE;
            bright = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_6:
            bright = btop= HI_TRUE;
            bbottom = bleft = HI_FALSE;
            break;
        case HLC_PATTERN_7:
            bbottom = bright = btop = HI_TRUE;
            bleft = HI_FALSE;
            break;
        case HLC_PATTERN_8:
            bleft = HI_TRUE;
            bbottom = bright = btop = HI_FALSE;
            break;
        case HLC_PATTERN_9:
            bbottom = bleft = HI_TRUE;
            bright = btop = HI_FALSE;
            break;
        case HLC_PATTERN_10:
            bright = bleft = HI_TRUE;
            bbottom = btop = HI_FALSE;
            break;
        case HLC_PATTERN_11:
            bbottom = bright = bleft = HI_TRUE;
            btop = HI_FALSE;
            break;
        case HLC_PATTERN_12:
            btop = bleft = HI_TRUE;
            bbottom = bright = HI_FALSE;
            break;
        case HLC_PATTERN_13:
            bbottom = btop = bleft = HI_TRUE;
            bright = HI_FALSE;
            break;
        case HLC_PATTERN_14:
            bright = btop= bleft = HI_TRUE;
            bbottom = HI_FALSE;
            break;
        case HLC_PATTERN_15:
            bbottom = bright = btop = bleft = HI_TRUE;
            break;
        default:
            HI_ERR_CC("ARIB_C1_HLC Error : CcType=%d Pattern=0x%02x\n", enInCCType, *ps32ControlData);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetCharBox((HI_S32)enInCCType,bbottom,bright,btop,bleft);// CC Graphic API

    return s32Ret;
}

//*********************************************************************************************************************
//Repeat Character
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Rpc(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8RepeatNumber   = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( bRollupFlg != HI_FALSE )
    {
        HI_ERR_CC("Rollup Flag ON Rpc : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;
    u8RepeatNumber = (HI_U8)*ps32ControlData;

    if(u8RepeatNumber > RPC_NUMBER_MAX)
    {
        return HI_FAILURE;
    }

    if(gCaptionInfo[enInCCType].bScrollFlg == HI_FALSE)
    {
        ARIBCC_OSD_SetRepeatNum((HI_S32)enInCCType,(HI_S32)u8RepeatNumber);
    }

    return s32Ret;
}

//********************************************************************************************
//STL SPL
//********************************************************************************************
static HI_S32 _ARIBCC_8BitCode_UnderLine(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_BOOL    bFlg   = HI_FALSE;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE) || (bRollupFlg != HI_FALSE) )
    {
        return HI_SUCCESS;
    }
#endif
    switch( pstControl->enControlId )
    {
        case ARIB_C1_SPL:
            bFlg = HI_FALSE;
            break;
        case ARIB_C1_STL:
            bFlg = HI_TRUE;
            break;
        default:
            HI_ERR_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            return HI_SUCCESS;
    }

    ARIBCC_OSD_Underline((HI_S32)enInCCType,bFlg);// CC Graphic API

    return s32Ret;
}

//*********************************************************************************************************************
//set write format
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Swf(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8Format = 0xFF;
    HI_BOOL    bApiCallFlg = HI_TRUE;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }


    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON SWF : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    switch( pstControl->enControlId )
    {
        case ARIB_CSI_SWF_1:
            ps32ControlData = (HI_S32*)pstControl->u8ControlData;
            switch( *ps32ControlData )
            {
#if 0
                case SWF_FORMAT_NORMAL_DENCITY_SIDE:
                    break;
                case SWF_FORMAT_NORMAL_DENCITY_LENGTH:
                    break;
                case SWF_FORMAT_HIGH_DENCITY_SIDE:
                    break;
                case SWF_FORMAT_HIGH_DENCITY_LENGTH:
                    break;
                case SWF_FORMAT_ROMAN_SIDE:
                    break;
                case SWF_FORMAT_1920_1080_SIDE:
                    break;
                case SWF_FORMAT_1920_1080_LENGTH:
                    break;
#endif
                case SWF_FORMAT_960_540_SIDE:           // horizontal writing form in 960 x 540
                    u8Format = PRINTH_960X540;
                    break;
                case SWF_FORMAT_960_540_LENGTH:         // vertical writing form in 960 x 540
                    u8Format = PRINTV_960X540;
                    break;
                case SWF_FORMAT_720_480_SIDE:           //horizontal writing form in 720 x 480
                    u8Format = PRINTH_720X480;
                    break;
                case SWF_FORMAT_720_480_LENGTH:         //vertical writing form in 720 x 480
                    u8Format = PRINTV_720X480;
                    break;
#if 0
                case SWF_FORMAT_1280_720_SIDE:
                    break;
                case SWF_FORMAT_1280_720_LENGTH:
                    break;
#endif
                default:
                    return HI_SUCCESS;
            }
            break;

        default:
            HI_ERR_CC ("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            bApiCallFlg = HI_FALSE;
            break;
    }


    if(g_bDataFlg[enInCCType] != HI_FALSE)
    {
        gCaptionInfo[enInCCType].stManagementBody.u8Format = u8Format;
    }

    else
    {
        gCaptionInfo[enInCCType].u8Format = u8Format;
    }

    if(bApiCallFlg != HI_FALSE)
    {
        // SWF initialize
#ifdef INIT_STATE
        s32Ret  = _ARIBCC_InitOperation(enInCCType, &(gCaptionInfo[enInCCType].stManagement),CAP_INIT_CSI_SWF);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC ("[%4.4d] ARIB_CSI_SWF_1 Init Error,CcType=%d ComErrCode=%ld\n",enInCCType,s32Ret);
            return s32Ret;
        }
#endif
        ARIBCC_OSD_SetWritingFormat((HI_S32)enInCCType,u8Format);
    }

    return s32Ret;
}

//***************************************************************************************
// Raster Colour command
//***************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Rcs(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32  s32Ret   = HI_SUCCESS;
    HI_U8   u8Index       = 0xFF;
    HI_U8   u8PaletteNum  = 0;
    HI_U8   u8PaletteIndex= 0;
    HI_U8   u8Red         = 0xFF;
    HI_U8   u8Green       = 0xFF;
    HI_U8   u8Blue        = 0xFF;
    HI_U8   u8Alpha       = 0xFF;
    HI_S32  *ps32ControlData = HI_NULL;

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( (gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE) || (bRollupFlg != HI_FALSE) )
    {
        HI_INFO_CC("Scroll Flag ON in RSC!\n");
        return HI_SUCCESS;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;
    u8Index        = (HI_U8)*ps32ControlData;

    if(u8Index > 127)
    {
        HI_ERR_CC("Index wrong in RCS!\n");
        return HI_FAILURE;
    }

    u8PaletteNum  = u8Index / 16;
    u8PaletteIndex= u8Index % 16;

    u8Red     = g_stClut[u8PaletteNum][u8PaletteIndex].u8Red;
    u8Green   = g_stClut[u8PaletteNum][u8PaletteIndex].u8Green;
    u8Blue    = g_stClut[u8PaletteNum][u8PaletteIndex].u8Blue;
    u8Alpha   = g_stClut[u8PaletteNum][u8PaletteIndex].u8Alpha;

    ARIBCC_OSD_SetWinColor((HI_S32)enInCCType,u8Red,u8Green,u8Blue,u8Alpha);// CC Graphic API

    return s32Ret;
}

//******************************************************************************************
//SET DISPLAY FORMAT
//dot numbers in horizontal direction
//dot numbers in vertical direction
//******************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Sdf(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_BOOL   bApiCallFlg      = HI_TRUE;
    HI_U8     u8LanguageIndex    = 0;
    HI_U16    u16DisplayWidth    = 0;
    HI_U16    u16DisplayWidthMax = 0;
    HI_U16    u16DisplayHeight   = 0;
    HI_U16    u16DisplayHeightMax= 0;
    HI_U32    U32Format          = 0;
    HI_S32    *ps32ControlData   = HI_NULL;
    ARIBCC_Management_S *pstManagement = &(gCaptionInfo[enInCCType].stManagement);


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON Sdf : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    s32Ret = ARIBCC_DEC_GetLanguageIndex(enInCCType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Language Index Error : CcType=%d\n", enInCCType);
        return s32Ret;
    }

    if(u8LanguageIndex > (CC_LANG_MAX-1))
    {
        HI_ERR_CC("language buffer overflow\n");
        return HI_FAILURE;
    }

    U32Format = pstManagement->stLanguageData[u8LanguageIndex].u8Format;
    switch( U32Format )
    {
        case DF_HORI_960X540:
        case DF_VERT_960X540:
            u16DisplayWidthMax   = 960;
            u16DisplayHeightMax  = 540;
            break;
        case DF_HORI_720X480:
        case DF_VERT_720X480:
            u16DisplayWidthMax   = 720;
            u16DisplayHeightMax  = 480;
            break;
        default:
            HI_ERR_CC("cType=%d U32Format=%d not support\n",enInCCType,U32Format);
            s32Ret = HI_FAILURE;
            bApiCallFlg = HI_FALSE;
            break;
    }

    if(bApiCallFlg != HI_FALSE)
    {
        ps32ControlData = (HI_S32*)pstControl->u8ControlData;
        /*TS20ST005_020b.trp service 77 SD have width 800 with resolution 720,
                 So we add the patch here added for fixing the QX RPT bug 221*/
        if( *ps32ControlData > u16DisplayWidthMax )
        {
            *ps32ControlData = u16DisplayWidthMax;
        }

        u16DisplayWidth  = (HI_U16)*ps32ControlData;
        u16DisplayHeight = (HI_U16)*(ps32ControlData+1);

        if(u16DisplayWidth < u16DisplayWidthMax && u16DisplayHeight < u16DisplayHeightMax)
        {
            if(g_bDataFlg[enInCCType] != HI_FALSE)
            {
                gCaptionInfo[enInCCType].stManagementBody.u16Width  = u16DisplayWidth;
                gCaptionInfo[enInCCType].stManagementBody.u16Height = u16DisplayHeight;
            }

            DISP_TRACE("\tSDF, window width=%d height=%d\n",u16DisplayWidth,u16DisplayHeight);
            ARIBCC_OSD_SetWindowSize((HI_S32)enInCCType,(HI_S32)u16DisplayWidth,(HI_S32)u16DisplayHeight);// CC Graphic API
        }
    }

    return s32Ret;
}

//*********************************************************************************************************************
//Set Display Position
//coordinates in horizontal direction
//coordinates in vertical direction
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Sdp(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32    s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8     u8LanguageIndex  = 0;
    HI_U16    u16X             = 0;
    HI_U16    u16XMax          = 0;
    HI_U16    u16Y             = 0;
    HI_U16    u16YMax          = 0;
    HI_U32    u32Format        = 0;
    HI_S32    *ps32ControlData = HI_NULL;
    ARIBCC_Management_S *pstManagement = &(gCaptionInfo[enInCCType].stManagement);

    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON Sdp : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    s32Ret  = ARIBCC_DEC_GetLanguageIndex(enInCCType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Language Index Error : CcType=%d\n", enInCCType);
        return s32Ret;
    }

    if(u8LanguageIndex > (CC_LANG_MAX-1))
    {
        HI_ERR_CC("language buffer overflow\n");
        return HI_FAILURE;
    }

    u32Format = pstManagement->stLanguageData[u8LanguageIndex].u8Format;
    switch( u32Format )
    {
        case DF_HORI_960X540:
        case DF_VERT_960X540:
            u16XMax = 960;
            u16YMax = 540;
            break;
        case DF_HORI_720X480:
        case DF_VERT_720X480:
            u16XMax = 720;
            u16YMax = 480;
            break;
        default:
            HI_ERR_CC("CcType=%d,u32Format=%d not support\n", enInCCType,u32Format);
            return HI_FAILURE;
    }

    ps32ControlData = (HI_S32*)pstControl->u8ControlData;
    u16X = (HI_U16)*ps32ControlData;
    u16Y = (HI_U16)*(ps32ControlData+1);

    if(u16X < u16XMax && u16Y<u16YMax)
    {
        if(g_bDataFlg[enInCCType] != HI_FALSE)
        {
            gCaptionInfo[enInCCType].stManagementBody.u16X = u16X;
            gCaptionInfo[enInCCType].stManagementBody.u16Y = u16Y;
        }

        DISP_TRACE("\tSDP, window x=%d y=%d\n",u16X,u16Y);
        ARIBCC_OSD_SetWindowPos((HI_S32)enInCCType,(HI_S32)u16X,(HI_S32)u16Y);// CC Graphic API
    }


    return s32Ret;
}

//*********************************************************************************************************************
//Character composition dot designation
//dot numbers in horizontal direction
//dot numbers in vertical direction

//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Ssm(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8Size        = 0xFF;
    HI_BOOL    bApiCallFlg  = HI_TRUE;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON Ssm : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case SSM_SIZE_16: // 16*16
            u8Size = 16;
            break;
        case SSM_SIZE_20: // 20*20
            u8Size = 20;
            break;
        case SSM_SIZE_24: // 24*24
            u8Size = 24;
            break;
        case SSM_SIZE_30: // 30*30
            u8Size = 30;
            break;
        case SSM_SIZE_36: // 36*36
            u8Size = 36;
            break;
        default:
            HI_ERR_CC("ARIB_CSI_SSM Normal Font u8Size Error : CcType=%d Size=%d\n", enInCCType, *ps32ControlData);
            s32Ret = HI_FAILURE;
            bApiCallFlg = HI_FALSE;
            break;
    }

    if(g_bDataFlg[enInCCType] != HI_FALSE)
    {
        gCaptionInfo[enInCCType].stManagementBody.u16Size = u8Size;
    }

    if(bApiCallFlg != HI_FALSE)
    {
        ARIBCC_OSD_SetCharacterSize((HI_S32)enInCCType,(HI_S32)u8Size);// CC Graphic API
    }

    return s32Ret;
}
//*********************************************************************************************************************
//Set Horizontal Spacing
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Shs(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32   s32Ret = HI_SUCCESS;
    HI_U8    u8HInterval    = 0;
    HI_S32   *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }


    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON Shs : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    ps32ControlData = (HI_S32*)pstControl->u8ControlData;
    u8HInterval = (HI_U8)*ps32ControlData;

    if(g_bDataFlg[enInCCType] != HI_FALSE)
    {
        gCaptionInfo[enInCCType].stManagementBody.u16IntervalShs = u8HInterval;
    }


    ARIBCC_OSD_SetHorizontalSpacing((HI_S32)enInCCType,(HI_S32)u8HInterval);// CC Graphic API

    return s32Ret;
}

//*********************************************************************************************************************
//Set Vertical Spacing
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Svs(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8VInterval    = 0;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( gCaptionInfo[enInCCType].bScrollFlg != HI_FALSE )
    {
        HI_ERR_CC("Scroll Flag ON Svs : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }

    ps32ControlData = (HI_S32*)pstControl->u8ControlData;
    u8VInterval = (HI_U8)*ps32ControlData;

    if(g_bDataFlg[enInCCType] != HI_FALSE)
    {
        gCaptionInfo[enInCCType].stManagementBody.u16IntervalSvs = u8VInterval;
    }

    ARIBCC_OSD_SetVerticalSpacing ((HI_S32)enInCCType,(HI_S32)u8VInterval);// CC Graphic API


    return s32Ret;
}

//*********************************************************************************************************************
//Ornament Control
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Orn(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8EdgeType    = 0xFF;
    HI_U8      u8Index       = 0;
    HI_U8      u8PaletteNum  = 0;
    HI_U8      u8PaletteIndex= 0;
    HI_U8      u8Red         = 0xFF;
    HI_U8      u8Green       = 0xFF;
    HI_U8      u8Blue        = 0xFF;
    HI_U8      u8Alpha       = 0xFF;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case ORN_NOTHING:
            u8EdgeType = 0;
            break;
        case ORN_HEMMING:
            u8EdgeType = 1;
            u8Index = (HI_U8)(*(ps32ControlData+1));
            if(u8Index <= ORN_INDEX_MAX)
            {
                u8PaletteNum  = u8Index / 16;
                u8PaletteIndex= u8Index % 16;
                u8Red     = g_stClut[u8PaletteNum][u8PaletteIndex].u8Red;
                u8Green   = g_stClut[u8PaletteNum][u8PaletteIndex].u8Green;
                u8Blue    = g_stClut[u8PaletteNum][u8PaletteIndex].u8Blue;
                u8Alpha   = g_stClut[u8PaletteNum][u8PaletteIndex].u8Alpha;
            }
            else
            {
                HI_ERR_CC ("ARIB_CSI_ORN u8Index Error : CcType=%d u8Index=%d\n", enInCCType, u8Index);
                return HI_FAILURE;
            }
            break;
        default:
            HI_ERR_CC("ARIB_CSI_ORN Error : CcType=%d u8EdgeType=0x%02x\n", enInCCType, *ps32ControlData);
            return HI_FAILURE;
    }

    ARIBCC_OSD_SetOrnament((HI_S32)enInCCType,(HI_S32)u8EdgeType, u8Red, u8Green, u8Blue, u8Alpha);// CC Graphic API

    return s32Ret;
}

//*********************************************************************************************************************
//
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Scr(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      Type        = 0;
    HI_U8      Second      = 120;
    HI_BOOL    bApiCallFlg  = HI_TRUE;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( bRollupFlg != HI_FALSE )
    {
        HI_INFO_CC("Rollup Flag ON Scr : CcType=%d\n", enInCCType);
        return HI_SUCCESS;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
        case SCR_MODE_FIXED:
            Type = 0;
            gCaptionInfo[enInCCType].bScrollFlg = HI_FALSE;
            break;
        case SCR_MODE_LINE:
            Type = 1;
            break;
        case SCR_MODE_LINE_ROLLOUT:
            Type = 2;
            break;
        case SCR_MODE_SCREEN:
            Type = 3;
            break;
        case SCR_MODE_SCREEN_ROLLOUT:
            Type = 4;
            break;
        default:
            HI_ERR_CC("ARIB_CSI_SCR Scroll Type Error : CcType=%d Number=0x%02x\n", enInCCType, *ps32ControlData);
            s32Ret = HI_FAILURE;
            bApiCallFlg = HI_FALSE;
            break;
    }

    if(((Type == 1) || (Type == 2) || (Type == 3) || (Type == 4)) && (*(ps32ControlData+1) <= SCR_SPEED_MAX))
    {
        Second = (HI_U8)*(ps32ControlData+1);
    }
    else
    {
        if(Type != 0)
        {
            HI_ERR_CC ("[%4.4d] ARIB_CSI_SCR Scroll Scroll Speed Error : ", __LINE__);
            HI_ERR_CC ("CcType=%d Speed=%ld Max=%d\n",enInCCType, (HI_U32)*(ps32ControlData+1), SCR_SPEED_MAX);
            s32Ret = HI_FAILURE;
            bApiCallFlg = HI_FALSE;
        }
    }

    if(bApiCallFlg != HI_FALSE)
    {
        if( (Type == 1) || (Type == 2) || (Type == 3) || (Type == 4) )
        {
            ARIBCC_OSD_SetCursorPosition((HI_S32)enInCCType,7,0,0,0,0);
            ARIBCC_OSD_SetFlash((HI_S32)enInCCType,0);
            ARIBCC_OSD_Underline((HI_S32)enInCCType,HI_FALSE);
            ARIBCC_OSD_SetCharBox((HI_S32)enInCCType,HI_FALSE,HI_FALSE,HI_FALSE,HI_FALSE);
            ARIBCC_OSD_SetWinColor((HI_S32)enInCCType,0,0,0,0);
            gCaptionInfo[enInCCType].bScrollFlg = HI_TRUE;
        }

        ARIBCC_OSD_SetScrollInfo((HI_S32)enInCCType,(HI_S32)Type,(HI_S32)Second);       // CC Graphic API
    }

    return s32Ret;
}

//*********************************************************************************************************************
//
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Pra(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      Number      = 0xFF;
    HI_BOOL    bApiCallFlg  = HI_TRUE;
    HI_S32     *ps32ControlData = HI_NULL;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }
#ifdef ROLL_UP_STATE
    HI_BOOL bRollupFlg = HI_FALSE;
    s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement), &bRollupFlg);
    if(s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC ("Get Rollup Status Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    if( bRollupFlg != HI_FALSE )
    {
        HI_INFO_CC("Rollup Flag ON Pra : CcType=%d\n",enInCCType);
        return HI_SUCCESS;
    }
#endif
    ps32ControlData = (HI_S32*)pstControl->u8ControlData;

    switch( *ps32ControlData )
    {
//        case PRA_BEL:
//            break;
        case PRA_NEWS_FLUSH_CHIME_1:
            Number = 0;
            break;
        case PRA_NEWS_FLUSH_CHIME_2:
            Number = 1;
            break;
        case PRA_NEWS_FLUSH_CHIME_3:
            Number = 2;
            break;
        case PRA_NEWS_FLUSH_CHIME_4:
            Number = 3;
            break;
        case PRA_NEWS_FLUSH_CHIME_5:
            Number = 4;
            break;
        case PRA_BOTTUN_1:
            Number = 5;
            break;
        case PRA_BOTTUN_2:
            Number = 6;
            break;
        case PRA_BOTTUN_3:
            Number = 7;
            break;
        case PRA_BOTTUN_4:
            Number = 8;
            break;
        case PRA_BOTTUN_5:
            Number = 9;
            break;
        case PRA_BOTTUN_6:
            Number = 10;
            break;
        case PRA_BOTTUN_7:
            Number = 11;
            break;
        case PRA_BOTTUN_8:
            Number = 12;
            break;
        case PRA_ALERT:
            Number = 13;
            break;
        default:
            HI_ERR_CC ("ARIB_CSI_PRA Play Number Error : CcType=%d Number=0x%02x\n", enInCCType, *ps32ControlData);
            s32Ret = HI_FAILURE;
            bApiCallFlg = HI_FALSE;
            break;
    }

    if(bApiCallFlg != HI_FALSE)
    {
        ARIBCC_OSD_Pra((HI_S32)Number);// CC Graphic API
    }

    return s32Ret;
}

//*********************************************************************************************************************
//special
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Special(ARIBCC_CaptionType_E enInCCType,ARIBCC_Control_S *pstControl)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8Request = 0;


    if(pstControl == HI_NULL)
    {
        HI_ERR_CC(" Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    switch( pstControl->enControlId )
    {
        case ARIB_SP_SP:
            u8Request = 0;
            break;
        case ARIB_SP_DEL:
            u8Request = 1;
            break;
        default:
            HI_ERR_CC ("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
            break;
    }


    ARIBCC_OSD_SpecialCode((HI_S32)enInCCType, u8Request);// CC Graphic API

    return s32Ret;
}

//*********************************************************************************************************************
//text char
//*********************************************************************************************************************
static HI_S32 _ARIBCC_8BitCode_Text(ARIBCC_CaptionType_E enInCCType,ARIBCC_Data_S *pstInData)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      au8Designate[4];
    HI_U8      au8Invoke[2];
    HI_U8      InvokeTmp     = 0;
    HI_U8      u8Index       = 0;
    HI_U8      *pu8Data      = HI_NULL;
    HI_U8      *pu8Text      = HI_NULL;
    HI_U32     u32DataLength = 0;
    HI_U32     u32TextLength = 0;
    HI_U8      i = 0;


    if(pstInData == HI_NULL)
    {
        HI_ERR_CC("Null Pointer Error : CcType=%d\n", enInCCType);
        return HI_FAILURE;
    }
    if (pstInData->pu8Data != (((HI_U8*) pstInData) + sizeof(ARIBCC_Data_S)))
    {
        HI_ERR_CC("cc data error\n");
        return HI_FAILURE;
    }

    u32DataLength = pstInData->u32DataLength;
    pu8Data = pu8Text = pstInData->pu8Data;
    DISP_TRACE(">>>>>>>>>> _ARIBCC_8BitCode_Text start!,u32DataLength = %d\n",u32DataLength);

    for( i=0,u32TextLength=0; i < u32DataLength; i++)
    {
        //if( (*pu8Data == SP_SP) || (*pu8Data == SP_DEL) )
        if((*pu8Data == SP_DEL))
        {
            if(u32TextLength > 0)
            {
                ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,u32TextLength); // CC Graphic API
            }
            if( *pu8Data == SP_SP )
            {
                ARIBCC_OSD_SpecialCode((HI_S32)enInCCType, 0);// CC Graphic API
            }
            else
            {
                ARIBCC_OSD_SpecialCode((HI_S32)enInCCType, 1);// CC Graphic API
            }
            u32TextLength = 0;
            pu8Text = pu8Data + 1;
        }
        /*Single shift means to invoke one code following to it in the GL or GR area temporary.*/
        else if( (*pu8Data == C0_SS2) || (*pu8Data == C0_SS3) )
        {
            HI_S32 is_macro_decode = 0;

            if(u32TextLength > 0)
            {
                ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,u32TextLength); // CC Graphic API
            }

            au8Designate[ARIBCC_CODESET_G0] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0];
            au8Designate[ARIBCC_CODESET_G1] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1];
            au8Designate[ARIBCC_CODESET_G2] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2];
            au8Designate[ARIBCC_CODESET_G3] = gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3];
            au8Invoke[ARIBCC_GR]    = gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR];
            au8Invoke[ARIBCC_GL]    = gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL];
            InvokeTmp    = au8Invoke[ARIBCC_GL];/*store invoke information*/
            if( *pu8Data == C0_SS2 )
            {
                au8Invoke[ARIBCC_GL] = 0x02;//G2->GL
            }
            else
            {
                au8Invoke[ARIBCC_GL] = 0x03;//G3->GL
            }
            ARIBCC_OSD_SetCharacterCode((HI_S32)enInCCType,au8Designate,au8Invoke);   // CC Graphic API

            pu8Text = pu8Data + 1;

            if(au8Invoke[ARIBCC_GL] == 0x02)
            {
                /* G2 = ARIB_GSET_KANJI || ARIB_GSET_DRCS_0  || ARIB_GSET_JIS_INTERCHANGE_KANJI_1 ||
                            ARIB_GSET_JIS_INTERCHANGE_KANJI_2 || ARIB_GSET_ADDITION_SIGN,2-bytecode*/
                if( (au8Designate[ARIBCC_CODESET_G2] == 0x01) || (au8Designate[ARIBCC_CODESET_G2] == 0x0D) ||
                    (au8Designate[ARIBCC_CODESET_G2] == 0x1E) || (au8Designate[ARIBCC_CODESET_G2] == 0x1F) ||
                    (au8Designate[ARIBCC_CODESET_G2] == 0x20))
                {
                    ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,2); // CC Graphic API
                    pu8Data += 2;
                    i += 2;
                    pu8Text = pu8Data + 1;
                }
                /* G2 = ARIB_GSET_MACRO*/
                else if(au8Designate[ARIBCC_CODESET_G2] == 0x1D)
                {
                    if((*pu8Text & 0xF0) == 0xE0)
                    {
                        u8Index = (HI_U8)(*pu8Text - 0xE0);
                        if(u8Index > 15)
                        {
                            HI_WARN_CC("index overflow\n");
                            u8Index = 15;
                        }
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0] = au8Designate[ARIBCC_CODESET_G0] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G0];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1] = au8Designate[ARIBCC_CODESET_G1] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G1];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2] = au8Designate[ARIBCC_CODESET_G2] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G2];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3] = au8Designate[ARIBCC_CODESET_G3] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G3];
                        gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR] = au8Invoke[ARIBCC_GR]  = g_stMacro[u8Index].au8Invoke[ARIBCC_GR];
                        gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = g_stMacro[u8Index].au8Invoke[ARIBCC_GL];
                        is_macro_decode = 1;
                    }
                    pu8Data += 1;
                    i += 1;
                    pu8Text = pu8Data + 1;
                }
                /*1-bytecode*/
                else
                {
                    ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,1); // CC Graphic API
                    pu8Data += 1;
                    i += 1;
                    pu8Text = pu8Data + 1;
                }
            }
            else
            {
                /*G3 = ARIB_GSET_KANJI || ARIB_GSET_DRCS_0  || ARIB_GSET_JIS_INTERCHANGE_KANJI_1 ||
                            ARIB_GSET_JIS_INTERCHANGE_KANJI_2 || ARIB_GSET_ADDITION_SIGN,2-bytecode*/
                if( (au8Designate[ARIBCC_CODESET_G3] == 0x01) || (au8Designate[ARIBCC_CODESET_G3] == 0x0D) ||
                    (au8Designate[ARIBCC_CODESET_G3] == 0x1E) ||(au8Designate[ARIBCC_CODESET_G3] == 0x1F) ||
                    (au8Designate[ARIBCC_CODESET_G3] == 0x20))
                {
                    ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,2); // CC Graphic API
                    pu8Data += 2;
                    i += 2;
                    pu8Text = pu8Data + 1;
                }
                /* G3 = ARIB_GSET_MACRO*/
                else if(au8Designate[ARIBCC_CODESET_G3] == 0x1D)
                {
                    if((*pu8Text & 0xF0) == 0x60)
                    {
                        u8Index = (HI_U8)(*pu8Text - 0x60);
                        if(u8Index > 15)
                        {
                            HI_WARN_CC("index overflow\n");
                            u8Index = 15;
                        }
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0] = au8Designate[ARIBCC_CODESET_G0] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G0];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1] = au8Designate[ARIBCC_CODESET_G1] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G1];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2] = au8Designate[ARIBCC_CODESET_G2] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G2];
                        gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3] = au8Designate[ARIBCC_CODESET_G3] = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G3];
                        gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR] = au8Invoke[ARIBCC_GR]  = g_stMacro[u8Index].au8Invoke[ARIBCC_GR];
                        gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL] = au8Invoke[ARIBCC_GL] = g_stMacro[u8Index].au8Invoke[ARIBCC_GL];
                        is_macro_decode = 1;
                    }
                    pu8Data += 1;
                    i += 1;
                    pu8Text = pu8Data + 1;
                }
                /*1-bytecode*/
                else
                {
                    ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,1);
                    pu8Data += 1;
                    i += 1;
                    pu8Text = pu8Data + 1;
                }
            }
            if( is_macro_decode == 0 )
            {
                au8Invoke[1] = InvokeTmp;
            }
            ARIBCC_OSD_SetCharacterCode((HI_S32)enInCCType,au8Designate,au8Invoke);  // CC Graphic API
            u32TextLength = 0;
        }
        else if( (gCaptionInfo[enInCCType].stSwitch.u8Designate[(HI_U8)gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR]] == 0x1D) ||
                 (gCaptionInfo[enInCCType].stSwitch.u8Designate[(HI_U8)gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL]] == 0x1D)  )
        {
            // GR
            if(gCaptionInfo[enInCCType].stSwitch.u8Designate[(HI_U8)gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR]] == 0x1D)
            {
                if( ((*pu8Data & 0xF0) == 0xE0) )
                {
                    if(u32TextLength > 0)
                    {
                        ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,u32TextLength); // CC Graphic API
                    }
                    u8Index = (HI_U8)(*pu8Data - 0xE0);

                    if(u8Index > 15)
                    {
                        HI_WARN_CC("index overflow\n");
                        u8Index = 15;
                    }

                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G0];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G1];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G2];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G3];
                    gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR]     = g_stMacro[u8Index].au8Invoke[ARIBCC_GR];
                    gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL]     = g_stMacro[u8Index].au8Invoke[ARIBCC_GL];

                    u32TextLength = 0;
                    pu8Text = pu8Data + 1;
                }
            }
            // GL
            else
            {
                if( ((*pu8Data & 0xF0) == 0x60) )
                {
                    if(u32TextLength > 0)
                    {
                        ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,u32TextLength); // CC Graphic API
                    }
                    u8Index = (HI_U8)(*pu8Data - 0x60);
                    if(u8Index > 15)
                    {
                        HI_WARN_CC("index overflow\n");
                        u8Index = 15;
                    }
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G0]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G0];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G1]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G1];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G2]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G2];
                    gCaptionInfo[enInCCType].stSwitch.u8Designate[ARIBCC_CODESET_G3]    = g_stMacro[u8Index].au8Designate[ARIBCC_CODESET_G3];
                    gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GR]     = g_stMacro[u8Index].au8Invoke[ARIBCC_GR];
                    gCaptionInfo[enInCCType].stSwitch.u8Invoke[ARIBCC_GL]     = g_stMacro[u8Index].au8Invoke[ARIBCC_GL];

                    u32TextLength = 0;
                    pu8Text = pu8Data + 1;
                }
            }
        }
        else
        {
            u32TextLength++ ;
        }
        pu8Data++;
    }

    if(u32TextLength > 0)
    {
        ARIBCC_OSD_DrawText((HI_S32)enInCCType,pu8Text,u32TextLength); // CC Graphic API
    }

    return s32Ret;
}

//*********************************************************************************************************************
//drcs
//*********************************************************************************************************************

static HI_S32 _ARIBCC_ParseDrcs(ARIBCC_CaptionType_E enInCCType ,HI_U32 u32InUnitId ,HI_U8 *pu8InDrcs)
{
    HI_S32     s32Ret = HI_SUCCESS;
    HI_U8      u8EntryType;
    HI_U8      u8NumberOfCode;
    HI_U8      u8NumberOfFont;
    HI_U8      u8Code1,u8Code2;
    HI_U8      u8FontId;
    HI_U8      u8Mode;
    HI_U8      u8Depth;
    HI_U8      u8Width;
    HI_U8      u8Height;
    HI_U8      *pu8Bitmap;
    HI_U8      i,j;
    HI_U32     u32BitCount;
    HI_U32     u32PatternDataSize;
    HI_S32     s32DrcsType;
    HI_U8      *pDrcsData;
    HI_U8      *pDrcsCode;
    HI_U8      *pDrcsFont;

    if(pu8InDrcs == HI_NULL)
    {
        HI_ERR_CC ("[%4.4d] Null Pointer Error : CcType=%d\n", __LINE__, enInCCType);
        return HI_FAILURE;
    }

    if(g_bDataFlg[enInCCType] != HI_FALSE)
    {
        u8EntryType = 1;
    }
    else
    {
        u8EntryType = 2;
    }

    pDrcsData   = pu8InDrcs;
    u8NumberOfCode = GET_BIT (pDrcsData, 0, 8);
    pDrcsCode     = pDrcsData + sizeof(HI_U8);

    for(i=1 ;i <= u8NumberOfCode; i++)
    {
        /*1BYTE_DRCS:
                    u8Code1:indicate which DRCS set(DRCS-1~ DRCS-15)
                    u8Code2:designate assigned code value of the character in DRCS set above.
                2BYTE_DRCS:
                    u8Code1 and u8Code2:designate assigned code value of the character in DRCS-0(Ganji Suppliment).
                */
        u8Code1 = GET_BIT (pDrcsCode, 0, 8);
        u8Code2 = GET_BIT (pDrcsCode, 8, 8);

        if(u32InUnitId == DATA_UNIT_1BYTE_DRCS)
        {
            s32DrcsType = 1;
            if((u8Code1 < 0x41) || (u8Code1 > 0x4F) || (u8Code2 < 0x21) || (u8Code2 > 0x7E))
            {
                HI_ERR_CC ("1BYTE DRCS CODE Error : CcType=%d i=%d u8Code1=%x u8Code2=%x\n", enInCCType, i, u8Code1, u8Code2);
                s32Ret = HI_FAILURE;
                break;
            }
        }
        else if(u32InUnitId == DATA_UNIT_2BYTE_DRCS)
        {
            s32DrcsType = 2;
            if((u8Code1 < 0x21) || (u8Code1 > 0x7E) || (u8Code2 < 0x21) || (u8Code2 > 0x7E))
            {
                HI_ERR_CC ("2BYTE DRCS CODE Error : CcType=%d i=%d u8Code1=%x u8Code2=%x\n",enInCCType, i, u8Code1, u8Code2);
                s32Ret = HI_FAILURE;
                break;
            }
        }
        else
        {
            HI_ERR_CC ("DRCS Unit Id Error : CcType=%d i=%d Id=0x%02x\n",enInCCType, i, (HI_S32) u32InUnitId);
            s32Ret = HI_FAILURE;
            break;
        }

        u8NumberOfFont = GET_BIT (pDrcsCode, 16, 8);
        pDrcsFont = pDrcsCode + sizeof(HI_U8) + sizeof(HI_U8);

        for(j=1 ;j <= u8NumberOfFont;j++)
        {
            u8FontId = GET_BIT (pDrcsFont,  0, 4);
            if(u8FontId > (HI_U8)15)
            {
                HI_ERR_CC ("u8FontId Error : CcType=%d j=%d u8FontId=0x%x\n",enInCCType, j, u8FontId);
                s32Ret = HI_FAILURE;
                break;
            }
            u8Mode = GET_BIT (pDrcsFont,  4, 4);
            if(u8Mode != (HI_U8)0x01)
            {
                HI_ERR_CC ("u8Mode Error : CcType=%d j=%d u8Mode=0x%x\n",enInCCType, j, u8Mode);
                s32Ret = HI_FAILURE;
                break;
            }
            u8Depth = GET_BIT (pDrcsFont,  8, 8);
            if(u8Depth != 2)
            {
                HI_ERR_CC ("u8Depth Error : CcType=%d j=%d u8Depth=0x%x\n",enInCCType, j, u8Depth);
                s32Ret = HI_FAILURE;
                break;
            }
            u8Width = GET_BIT (pDrcsFont, 16, 8);
            if( (u8Width != 36) && (u8Width != 18) && (u8Width != 30) && (u8Width != 15) &&
                (u8Width != 24) && (u8Width != 12) && (u8Width != 20) && (u8Width != 10) &&
                (u8Width != 16) && (u8Width != 8)                                      )
            {
                HI_ERR_CC ("u8Width Error : CcType=%d j=%d u8Width=%d\n",enInCCType, j, u8Width);
                s32Ret = HI_FAILURE;
                break;
            }
            u8Height = GET_BIT (pDrcsFont, 24, 8);
            if( (u8Height != 36) && (u8Height != 18) && (u8Height != 30) && (u8Height != 15) &&
                (u8Height != 24) && (u8Height != 12) && (u8Height != 20) && (u8Height != 10) &&
                (u8Height != 16) && (u8Height != 8)                                         )
            {
                HI_ERR_CC ("u8Height Error : CcType=%d j=%d u8Height=%d\n",enInCCType, j, u8Height);
                s32Ret = HI_FAILURE;
                break;
            }
            pu8Bitmap = pDrcsFont + (sizeof(HI_U8) * 4);

            ARIBCC_OSD_DrcsEntry((HI_S32)enInCCType, u8EntryType, s32DrcsType,u8Code1,u8Code2,
                                    u8Depth,u8Width,u8Height,pu8Bitmap); // CC Graphic API

            u32BitCount = 1;
            while ((1 << u32BitCount) < (u8Depth + 2))
            {
                u32BitCount ++;
            }
            u32PatternDataSize = ((u32BitCount * u8Width * u8Height) + 7) / 8;//??

            pDrcsFont = pu8Bitmap + u32PatternDataSize;
        }

        if(s32Ret != HI_SUCCESS)
        {
            break;
        }

        pDrcsCode = pDrcsFont;

    }

    return s32Ret;
}

//*********************************************************************************************************************
//Bit map
//*********************************************************************************************************************
static HI_S32 _ARIBCC_ParseBitmap(ARIBCC_CaptionType_E enInCCType,HI_U8 *pu8InBitmap)
{
    HI_S32     s32Ret = HI_SUCCESS;

    if(pu8InBitmap == HI_NULL)
    {
        HI_ERR_CC ("Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    ARIBCC_OSD_DrawBitmap((HI_S32)enInCCType, pu8InBitmap);   // CC Graphic API

    return s32Ret;
}


//*********************************************************************************************************************

//*********************************************************************************************************************
static HI_S32 _ARIBCC_ParseStatementBody(ARIBCC_CaptionType_E enInCCType, ARIBCC_StatementBody_S *pstInStatment, HI_U32 u32UnitLength)
{
    HI_S32    s32Ret = HI_SUCCESS;
    ARIBCC_StatementBody_S  *pstStatementBody      = HI_NULL;
    ARIBCC_Control_S        *pstControl   = HI_NULL;
    ARIBCC_Data_S           *pstData      = HI_NULL;
    HI_U32    i              = 0;
    HI_U32    u32Length      = 0;
    HI_U32    u32PadingSize  = 0;
    HI_U32    u32ParamNum    = 0;
    HI_BOOL   bRollupFlg     = HI_FALSE;
    HI_U8     *pTmp          = HI_NULL;

    if(pstInStatment == HI_NULL)
    {
        HI_ERR_CC ("Null Pointer Error : CcType=%d\n", enInCCType);
        return HI_FAILURE;
    }

    if( ((pstInStatment->u32ControlCount == 0) && (pstInStatment->u32ControlLength != 0)) ||
        ((pstInStatment->u32ControlCount != 0) && (pstInStatment->u32ControlLength == 0)) )
    {
        HI_ERR_CC ("Body Size Error! CcType=%d ControlCount=%lu ControlLength=%lu\n",enInCCType, pstInStatment->u32ControlCount, pstInStatment->u32ControlLength);
        return HI_FAILURE;
    }

    DISP_TRACE("_ARIBCC_ParseStatementBody,statement length = %d\n",u32UnitLength);
    pstStatementBody = pstInStatment;

    while(u32Length < u32UnitLength)
    {
        pstControl = (ARIBCC_Control_S*)pstStatementBody->stControl;

        /*Have control code*/
        if(pstStatementBody->u32ControlCount > 0)
        {
            for(i=1 ;i <= (pstStatementBody->u32ControlCount);i++)
            {
                DISP_TRACE("\tpControl->fControlId= %#x\n",pstControl->enControlId);
                switch( pstControl->enControlId )
                {
                    case ARIB_C0_APB:
                    case ARIB_C0_APF:
                    case ARIB_C0_APD:
                    case ARIB_C0_APU:
                    case ARIB_C0_APR:
                    case ARIB_C0_APS:
                    case ARIB_C0_PAPF:
                    case ARIB_CSI_ACPS:
                        s32Ret = _ARIBCC_8BitCode_Position(enInCCType,pstControl);
                        break;

                    case ARIB_C0_CS:
#ifdef ROLL_UP_STATE
                        s32Ret = _ARIBCC_GetRollupState(enInCCType, &(gCaptionInfo[enInCCType].stManagement),&bRollupFlg);
                        if(s32Ret != HI_SUCCESS)
                        {
                            HI_ERR_CC ("Get Rollup Status Error,CcType=%d\n", enInCCType);
                            break;
                        }
#endif
                        /* Ref[B24],part3,Chapter 8,In the roll-up mode, no initializing operation should be done. */
                        if(bRollupFlg == HI_FALSE)
                        {
                       #ifdef INIT_STATE
                            s32Ret  = _ARIBCC_InitOperation(enInCCType, &(gCaptionInfo[enInCCType].stManagement),CAP_INIT_C0_CS);
                            if(s32Ret != HI_SUCCESS)
                            {
                                HI_ERR_CC ("ARIB_C0_CS Init Error,CcType=%d\n",enInCCType);
                                break;
                            }
                       #endif
                            s32Ret = _ARIBCC_8BitCode_Cs(enInCCType);
                        }
                        break;
                    case ARIB_C0_LS0:
                    case ARIB_C0_LS1:
                    case ARIB_ESC_LS2:
                    case ARIB_ESC_LS3:
                    case ARIB_ESC_LS1R:
                    case ARIB_ESC_LS2R:
                    case ARIB_ESC_LS3R:
                    //case ARIB_C0_SS2:
                    //case ARIB_C0_SS3:
                    case ARIB_ESC_DES:
                        s32Ret = _ARIBCC_8BitCode_SwitchControl(enInCCType,pstControl);
                        break;
                    case ARIB_C1_BKF:
                    case ARIB_C1_RDF:
                    case ARIB_C1_GRF:
                    case ARIB_C1_YLF:
                    case ARIB_C1_BLF:
                    case ARIB_C1_MGF:
                    case ARIB_C1_CNF:
                    case ARIB_C1_WHF:
                    case ARIB_C1_COL_1:
                    case ARIB_C1_COL_2:
                        s32Ret = _ARIBCC_8BitCode_TxtColor(enInCCType,pstControl);
                        break;
                    case ARIB_C1_NSZ:
                    case ARIB_C1_SSZ:
                    case ARIB_C1_MSZ:
                    case ARIB_C1_SZX:
                        s32Ret = _ARIBCC_8BitCode_TextSize(enInCCType,pstControl);
                        break;
                    case ARIB_C1_FLC:
                        s32Ret = _ARIBCC_8BitCode_Flc(enInCCType,pstControl);
                        break;
                    case ARIB_C1_TIME_1:
                    //case ARIB_C1_TIME_2:
                    //case ARIB_C1_TIME_3:
                    //case ARIB_C1_TIME_4:
                    //case ARIB_C1_TIME_5:
                    //case ARIB_C1_TIME_6:
                    //case ARIB_C1_TIME_7:
                        s32Ret = _ARIBCC_8BitCode_Time(enInCCType,pstControl);
                        break;
                    case ARIB_C1_POL:
                        s32Ret = _ARIBCC_8BitCode_Pol(enInCCType,pstControl);
                        break;
                    case ARIB_C1_HLC:
                        s32Ret = _ARIBCC_8BitCode_Hlc(enInCCType,pstControl);
                        break;
                    case ARIB_C1_RPC:
                        s32Ret = _ARIBCC_8BitCode_Rpc(enInCCType,pstControl);
                        break;
                    case ARIB_C1_SPL:
                    case ARIB_C1_STL:
                        s32Ret = _ARIBCC_8BitCode_UnderLine(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SWF_1:
                    //case ARIB_CSI_SWF_2:
                        s32Ret = _ARIBCC_8BitCode_Swf(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_RCS:
                        s32Ret = _ARIBCC_8BitCode_Rcs(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SDF:
                        s32Ret = _ARIBCC_8BitCode_Sdf(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SDP:
                        s32Ret = _ARIBCC_8BitCode_Sdp(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SSM:
                        s32Ret = _ARIBCC_8BitCode_Ssm(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SHS:
                        s32Ret = _ARIBCC_8BitCode_Shs(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SVS:
                        s32Ret = _ARIBCC_8BitCode_Svs(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_ORN:
                        s32Ret = _ARIBCC_8BitCode_Orn(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_SCR:
                        s32Ret = _ARIBCC_8BitCode_Scr(enInCCType,pstControl);
                        break;
                    case ARIB_CSI_PRA:
                        s32Ret = _ARIBCC_8BitCode_Pra(enInCCType,pstControl);
                        break;
                    case ARIB_SP_SP:
                    case ARIB_SP_DEL:
                        s32Ret = _ARIBCC_8BitCode_Special(enInCCType,pstControl);
                        break;
                    default:
                        HI_ERR_CC("Control Id Error : CcType=%d Id=0x%08x\n", enInCCType, pstControl->enControlId);
                        s32Ret = HI_FAILURE;
                        break;
                }

                if(s32Ret != HI_SUCCESS)
                {
                    HI_WARN_CC("8BitCode Parse Error1\n ", __LINE__);
                    break;
                }

                u32ParamNum = GET_CS_NUM_OF_PARAM(pstControl->enControlId);

                pTmp = (HI_U8*)pstControl;
                pTmp += ( sizeof(pstControl->enControlId) + (u32ParamNum*sizeof(ARIBCC_CtrlSet_E)) );
                pstControl = (ARIBCC_Control_S*)pTmp;

            }

            u32Length += pstStatementBody->u32ControlLength + sizeof(pstStatementBody->u32ControlCount) + sizeof(pstStatementBody->u32ControlLength);
        }
        else
        {
            u32Length += sizeof(pstStatementBody->u32ControlCount) + sizeof(pstStatementBody->u32ControlLength);
        }

        /******************************************************************************
                                         operate text
            *******************************************************************************/
        pstData = (ARIBCC_Data_S*)pstControl;
        u32PadingSize = GET_PADDING_SIZE( pstData->u32DataLength);

        if(pstData->u32DataLength > 0)
        {
            s32Ret = _ARIBCC_8BitCode_Text(enInCCType,pstData);
            u32Length += sizeof(ARIBCC_Data_S) + pstData->u32DataLength + u32PadingSize;
        }
        else
        {
            u32Length += sizeof(ARIBCC_Data_S) + u32PadingSize;
        }

        if(u32Length < u32UnitLength)
        {
            pstStatementBody = (ARIBCC_StatementBody_S*)((HI_U8*)pstData + sizeof(ARIBCC_Data_S) + pstData->u32DataLength + u32PadingSize);
        }
    }

    return s32Ret;
}

//*********************************************************************************************************************
//
//*********************************************************************************************************************
static HI_S32 _ARIBCC_ParseMessage(ARIBCC_CaptionType_E enInCCType,HI_U8 *pInMessage)
{
    HI_S32         s32Ret     = HI_SUCCESS;
    ARIBCC_ManagementUnit_S   stManagementUnit;
    ARIBCC_UnitGroup_S      *pstGroup         = HI_NULL;
    ARIBCC_Unit_S           *pstUnit          = HI_NULL;
    ARIBCC_StatementBody_S  *pstStatementBody = HI_NULL;
    HI_U8      *pu8Bitmap        = HI_NULL;
    HI_U8      *pDrcs            = HI_NULL;
    HI_U32     u32PaddingLength  = 0;
    HI_U32     u32Length         = 0;
    HI_U8      *pu8Tmp           = HI_NULL;

    DISP_TRACE("_ARIBCC_ParseMessage\n");

    if( pInMessage == HI_NULL)
    {
        HI_ERR_CC ("Null Pointer Error : CcType=%d\n",enInCCType);
        return HI_FAILURE;
    }

    memcpy(&stManagementUnit, pInMessage, sizeof(ARIBCC_ManagementUnit_S));

    if(stManagementUnit.pstManagement)
    {
        g_bRcvManageFlg[enInCCType] = g_bDataFlg[enInCCType] = HI_TRUE;

        memcpy( &gCaptionInfo[enInCCType].stManagement, stManagementUnit.pstManagement, sizeof(ARIBCC_Management_S));
        memset( &gCaptionInfo[enInCCType].stManagementBody, 0xFF, sizeof(ARIBCC_ManagementBody_S));

        if(gCaptionInfo[enInCCType].bScrollFlg)
        {
            ARIBCC_OSD_SetWinColor(enInCCType,0,0,0,0);
            ARIBCC_OSD_ClearScreen(enInCCType);
            ARIBCC_OSD_Show(enInCCType);
            gCaptionInfo[enInCCType].bScrollFlg = HI_FALSE;
        }
#ifdef INIT_STATE
        /*Initialize when management update,Ref [B14,Vol3,Section2,4.7.1],Ref[B24,part3,Chap8,Table8-1]*/
        s32Ret = _ARIBCC_InitOperation (enInCCType, stManagementUnit.pstManagement, CAP_INIT_REVISE_CONTROL);
        if(s32Ret != HI_SUCCESS)
        {
            return s32Ret;
        }
#endif
        if(stManagementUnit.pstUnitGroup->u32UnitGroupLength == 0)
        {
            return HI_SUCCESS;
        }
    }
    else
    {
        g_bDataFlg[enInCCType] = HI_FALSE;
        gCaptionInfo[enInCCType].u8Format = 0xFF;
#ifdef INIT_STATE

        /*Initialize when receive statement data group,Ref[B24,part3,Chap8,Table8-1]*/
        s32Ret = _ARIBCC_InitOperation (enInCCType, &gCaptionInfo[enInCCType].stManagement, CAP_INIT_RECEIVE_DATA);
        if(s32Ret != HI_SUCCESS)
        {
            return s32Ret;
        }
#endif
    }

    pstGroup = stManagementUnit.pstUnitGroup;
    pstUnit  = pstGroup->stUnit;
    if ( pstUnit->pu8UnitData != (((HI_U8*) pstUnit) + sizeof(ARIBCC_Unit_S)) )
    {
        HI_ERR_CC("Unit data error\n");
        return HI_FAILURE;
    }

   /*******************************************************************************

     *******************************************************************************/
    while( u32Length < pstGroup->u32UnitGroupLength )
    {
        switch( pstUnit->u32UnitId )
        {
            case DATA_UNIT_STATE_BODY:

                if(g_bDataFlg[enInCCType] == HI_FALSE)
                {
#ifdef INIT_STATE
                   /*Ref[B24,part3,Chap8,Table8-1],Data Unit |Text*/
                    s32Ret = _ARIBCC_InitOperation(enInCCType, &(gCaptionInfo[enInCCType].stManagement),CAP_INIT_STATE_BODY);
                    if(s32Ret != HI_SUCCESS)
                    {
                        HI_ERR_CC("Init CAP_INIT_STATE_BODY error\n");
                        return HI_FAILURE;
                    }
#endif

                }
                pstStatementBody = (ARIBCC_StatementBody_S*)pstUnit->pu8UnitData;
                s32Ret = _ARIBCC_ParseStatementBody(enInCCType, pstStatementBody ,pstUnit->u32UnitLength);
                break;

            case DATA_UNIT_GEOMETRIC:
            case DATA_UNIT_ADD_SOUND:
                break;

            case DATA_UNIT_1BYTE_DRCS:
            case DATA_UNIT_2BYTE_DRCS:
                pDrcs = (HI_U8*)pstUnit->pu8UnitData;
                s32Ret = _ARIBCC_ParseDrcs(enInCCType, pstUnit->u32UnitId ,pDrcs);
                break;

            case DATA_UNIT_COLORMAP:
                break;

            case DATA_UNIT_BITMAP:
                pu8Bitmap = (HI_U8*)pstUnit->pu8UnitData;
                s32Ret = _ARIBCC_ParseBitmap(enInCCType,pu8Bitmap);
                break;

            default:
                HI_ERR_CC ("Unit Id Error : CcType=%d Id=0x%02x\n",enInCCType, (HI_U32)pstUnit->u32UnitId);
                s32Ret = HI_FAILURE;
                break;
        }


        if( s32Ret != HI_SUCCESS )
        {
            HI_ERR_CC ("Message Parse Error:CcType=%d UnitId=0x%02x\n",enInCCType, (HI_U32)pstUnit->u32UnitId);
            break;
        }


        if((g_bDataFlg[enInCCType] == HI_FALSE) &&
            ((pstUnit->u32UnitId == DATA_UNIT_STATE_BODY) || (pstUnit->u32UnitId == DATA_UNIT_BITMAP)))
        {
            ARIBCC_OSD_Show(enInCCType);   // CC Graphic API
        }

        /*paddingLength used to alignment the memory */
        u32PaddingLength = GET_PADDING_SIZE (sizeof(ARIBCC_Unit_S) + pstUnit->u32UnitLength);
        u32Length += (sizeof(ARIBCC_Unit_S) + pstUnit->u32UnitLength + u32PaddingLength);

        /*if there are other data unit in this datagroup*/
        pu8Tmp = (HI_U8*)pstUnit;
        pu8Tmp += u32Length;
        pstUnit= (ARIBCC_Unit_S*)pu8Tmp;

     }

    return s32Ret;
}

//******************************************************************************************

//******************************************************************************************

HI_S32 ARIBCC_DISP_Create(ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    g_bRcvManageFlg[enCaptionType]               = HI_FALSE;
    memset( &gCaptionInfo[enCaptionType], 0xFF, sizeof(ARIBCC_CaptionInfo_S));
    gCaptionInfo[enCaptionType].bScrollFlg     = HI_FALSE;
    gCaptionInfo[enCaptionType].u16PaletteNum           = PALETTE_NUM_0;

    s_enDispState[enCaptionType] = CC_THREAD_CREATE;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DISP_Destroy(ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    s_enDispState[enCaptionType] = CC_THREAD_EXIT;

    /* send a msg to myself */
    (HI_VOID)ARIBCC_Com_MessageSend (enCaptionType, MSGTYPE_CC_DISP_END, HI_NULL, 0);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DISP_Start(ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    s_enDispState[enCaptionType] = CC_THREAD_WAIT;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DISP_Stop(ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if(g_bRcvManageFlg[enCaptionType] != HI_FALSE)
    {
        ARIBCC_OSD_SetWinColor(enCaptionType,0,0,0,0);
        ARIBCC_OSD_ClearScreen(enCaptionType);
        ARIBCC_OSD_Show(enCaptionType);
        ARIBCC_OSD_SetScrollInfo(enCaptionType, 0, 0);
    }

    s_enDispState[enCaptionType] = CC_THREAD_STOP;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DISP_Reset(ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if(g_bRcvManageFlg[enCaptionType] != HI_FALSE)
    {
        ARIBCC_OSD_SetWinColor(enCaptionType,0,0,0,0);
        ARIBCC_OSD_ClearScreen(enCaptionType);
        ARIBCC_OSD_Show(enCaptionType);
        ARIBCC_OSD_SetScrollInfo(enCaptionType, 0, 0);
    }
    return HI_SUCCESS;
}

HI_S32 ARIBCC_DISP_Cycle(ARIBCC_CaptionType_E enCaptionType)
{
    HI_S32         s32Ret = HI_SUCCESS;
    HI_S32         s32RcvSize = ARIBCC_MSGBUFF_SIZE;
    HI_S32         s32MsgType = 0;
    HI_U8          au8MsgData[ARIBCC_MSGBUFF_SIZE];


    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if ((s_enDispState[enCaptionType] > CC_THREAD_CREATE) && (s_enDispState[enCaptionType] < CC_THREAD_EXIT))
    {
        memset(au8MsgData, 0, sizeof(au8MsgData));
        s32Ret = ARIBCC_Com_MessageReceive(enCaptionType,&s32MsgType,au8MsgData,&s32RcvSize);
        if(s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("Message Recive Error!\n");
            return HI_FAILURE;
        }

        if (MSGTYPE_CC_DISP_PARSE == s32MsgType)
        {
            ARIBCC_ManagementUnit_S stRcvMsgData;

            s_enDispState[enCaptionType] = CC_THREAD_EXECUTE;
            s32Ret = _ARIBCC_ParseMessage(enCaptionType,au8MsgData);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("Disp parse Error : enCaptionType=%d\n", enCaptionType);
                return HI_FAILURE;
            }

            memcpy(&stRcvMsgData, au8MsgData, sizeof(ARIBCC_ManagementUnit_S));
            s32Ret = ARIBCC_Com_FreePESPtr (enCaptionType, (HI_U8*) stRcvMsgData.pstUnitGroup);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to ARIBCC_Com_FreePESPtr\n");
                return HI_FAILURE;
            }
        }
        else if (MSGTYPE_CC_DISP_END == s32MsgType)
        {
            HI_INFO_CC("MSGTYPE_CC_DISP_END\n");
        }
        else if (MSGTYPE_CC_DISP_STOP == s32MsgType)
        {
            HI_INFO_CC("MSGTYPE_CC_DISP_STOP\n");
        }
        else if (MSGTYPE_CC_DISP_TIMEOUT == s32MsgType)
        {
            if(g_bRcvManageFlg[enCaptionType] != HI_FALSE)
            {
                ARIBCC_OSD_SetWinColor(enCaptionType,0,0,0,0);
                ARIBCC_OSD_ClearScreen(enCaptionType);
                ARIBCC_OSD_Show(enCaptionType);
            }
        }
        else if (MSGTYPE_CC_DISP_CLEAR == s32MsgType)
        {
            if(g_bRcvManageFlg[enCaptionType] != HI_FALSE)
            {
                ARIBCC_OSD_SetWinColor(enCaptionType,0,0,0,0);
                ARIBCC_OSD_ClearScreen(enCaptionType);
                ARIBCC_OSD_Show(enCaptionType);
            }
        }
        else
        {
            HI_WARN_CC("Message Type Error : enCaptionType=%d mtype=%d\n",enCaptionType, s32MsgType);
            return HI_FAILURE;
        }
    }

    return HI_SUCCESS;
}

