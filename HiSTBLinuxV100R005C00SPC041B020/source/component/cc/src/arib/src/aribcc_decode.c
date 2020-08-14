
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include <unistd.h>
//#include <sys/prctl.h>

#include "cc_debug.h"
#include "aribcc_obj.h"
#include "aribcc_decode.h"
#include "ccdisp_api.h"


#define ARIBCC_PES_HEADER_LENGTH       (6)
#define ARIBCC_PES_OPT_HEADER_LENGTH   (3)
#define ARIBCC_PES_PD_HEADER_LENGTH    (3)

/* PES packet_code_prefix */
#define ARIBCC_PES_SC_PREFIX         0x000001

/* Ref : 9.3.2 Caption statement data */
typedef struct  tagAribCC_Statement_t
{
    HI_U8                 u8DataGroupID;
    HI_U8                 u8DataGroupVersion;
    HI_U8                 u8DataGroupLinkNumber;
    HI_U8                 u8LastDataGroupLinkNumber;
    HI_U8                 u8TMD;
    HI_U8                 u8Padding[3];
    HI_U16                au16STM[4];
//    HI_U16              fUnitGroupLoopLength;
//    AribCC_UnitGroup_t  fUnitGroup[1];
} AribCC_Statement_t;

typedef struct  tagAribCC_CaptionData_t
{
    ARIBCC_Management_S     fManagement;
    AribCC_Statement_t      fStatement;
} AribCC_CaptionData_t;

typedef struct tagARIBCC_DEC_PARAM_S
{
    AribCC_CaptionData_t stCaptionData;
    HI_U32               u32LastReceptionTime;
    struct timeval       stLastReceptionTimeValue;
    HI_BOOL              bReception; /*if received management data group*/
    HI_BOOL              bMute;
} ARIBCC_DEC_PARAM_S;

static ARIBCC_DEC_PARAM_S s_astDecParam[CCPES_TYPE_MAX];

//**********************************************************************************************************************

/* Ref : Table 7-18 Default macro code strings */
static const ARIBCC_DefaultMacro_S gDefaultMacro[16] =
{
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_ALPHANUMERIC },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_KATAKANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_1 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_MOSAIC_A },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_MOSAIC_C },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_MOSAIC_D },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_MOSAIC_A },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_MOSAIC_B },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_MOSAIC_D },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_MOSAIC_A },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_1 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_MOSAIC_D },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_DRCS_1 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_2 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_3 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_DRCS_4 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_5 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_6 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_DRCS_7 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_8 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_9 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_DRCS_10 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_11 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_12 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_DRCS_13 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_14 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_15 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_2 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_3 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KANJI },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_DRCS_4 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_KATAKANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_HIRAGANA },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_ALPHANUMERIC },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    },
    {
        {
            { ARIB_ESC_DES, ARIB_CELEMENT_G0, ARIB_GSET_ALPHANUMERIC },
            { ARIB_ESC_DES, ARIB_CELEMENT_G1, ARIB_GSET_MOSAIC_A },
            { ARIB_ESC_DES, ARIB_CELEMENT_G2, ARIB_GSET_DRCS_1 },
            { ARIB_ESC_DES, ARIB_CELEMENT_G3, ARIB_GSET_MACRO }
        },
        {
            ARIB_C0_LS0,
            ARIB_ESC_LS2R
        }
    }
};


HI_S32  _ARIBCC_DEC_DecodeDataGroup(ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8DataGroup, HI_U16 u16DataSize, HI_U64 u64Pts);

HI_S32  _ARIBCC_DEC_DecodeManagement(ARIBCC_CaptionType_E enCaptionType, ARIBCC_DataGroup_S *pstDataGroup,
                                     HI_U8 *pu8ManagementData, HI_U8 **ppu8DataUnitLoop, HI_U32 *pu32DataUnitLoopLength);

HI_S32  _ARIBCC_DEC_DecodeStatement (ARIBCC_CaptionType_E enCaptionType, ARIBCC_DataGroup_S *pstDataGroup,
                                     HI_U8 *pu8StatementData, HI_U8 **ppu8DataUnitLoop, HI_U32 *pu32DataUnitLoopLength);

HI_S32  _ARIBCC_DEC_DecodeDataUnitLoop (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8DataUnitLoop, HI_U32 s32DataUnitLoopLength,
                                        ARIBCC_UnitGroup_S **ppstUnitGroup, HI_U32 *pu32UnitGroupLength);

HI_S32  _ARIBCC_DEC_DecodeStatementBody (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8StatementBody, HI_U32 u32StatementBodySize,
                                         ARIBCC_StatementBody_S *pstStatementBody, HI_U32 *pu32StatementBodySize);

HI_S32  _ARIBCC_DEC_DecodeC0Control (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                                     ARIBCC_Control_S *pstControl, HI_U32 *pu32ControlSize);

HI_S32  _ARIBCC_DEC_DecodeC1Control (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                                     ARIBCC_Control_S *pstControl, HI_U32 *pu32ControlSize);

HI_S32  _ARIBCC_DEC_DecodeCsiControl (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                                      ARIBCC_Control_S *pstControl, HI_U32*pu32ControlSize);

HI_S32  _ARIBCC_DEC_DecodeDrcsData (ARIBCC_CaptionType_E enCaptionType, ARIBCC_DrcsType_E enDrcsType, HI_U8 *pu8InDrcsData,
                                    HI_U32 u32InDrcsDataSize, HI_U8 *pu8OutDrcsData, HI_U32 *pu32OutDrcsDataSize);

HI_S32  _ARIBCC_DEC_DecodeBitmapData (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8InBitmapData, HI_U32 u32InBitmapDataSize,
                                      HI_U8 *pu8OutBitmapData, HI_U32 *pu32OutBitmapDataSize);

HI_S32  _ARIBCC_DEC_GetPresentTimeFromPTS (ARIBCC_CaptionType_E enCaptionType, HI_U64 u64PtsFromPES90k, struct timeval *pstTimeValue);

HI_S32  _ARIBCC_DEC_CheckCCPESHeader (HI_U8 *pu8CcPesPtr, HI_U64 *pu64Pts, HI_U8 **ppu8PesDataByte);


HI_S32 ARIBCC_DEC_Init (ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    memset(s_astDecParam + enCaptionType, 0, sizeof(ARIBCC_DEC_PARAM_S));
    s_astDecParam[enCaptionType].u32LastReceptionTime = 0xffffffff;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_DeInit (ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    memset(s_astDecParam + enCaptionType, 0, sizeof(ARIBCC_DEC_PARAM_S));
    s_astDecParam[enCaptionType].u32LastReceptionTime = 0xffffffff;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_Create (ARIBCC_CaptionType_E enCaptionType)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    s32Ret = ARIBCC_Com_InitPESPtr (enCaptionType, pstObject->u32PesBufSize);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to ARIBCC_Com_InitPESPtr !\n");
        return HI_FAILURE;
    }

    (HI_VOID)ARIBCC_Com_ClearTimer (enCaptionType);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_Destroy (ARIBCC_CaptionType_E enCaptionType)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    s32Ret = ARIBCC_Com_DeInitPESPtr (enCaptionType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to ARIBCC_Com_DeInitPESPtr\n");
        return HI_FAILURE;
    }

    (HI_VOID)ARIBCC_Com_ClearTimer (enCaptionType);

    pstDecParam->u32LastReceptionTime = 0xffffffff;
    pstDecParam->bReception = HI_FALSE;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_Start (ARIBCC_CaptionType_E enCaptionType)
{
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_Stop (ARIBCC_CaptionType_E enCaptionType)
{
    HI_S32  s32Ret = HI_SUCCESS;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    s32Ret = ARIBCC_Com_ClearTimer (enCaptionType);

    s32Ret |= ARIBCC_Com_ClearPESPtr (enCaptionType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to ARIBCC_Com_ClearPESPtr\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_Reset(ARIBCC_CaptionType_E enCaptionType)
{
    HI_S32  s32Ret = HI_SUCCESS;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }
    s_astDecParam[enCaptionType].bReception = HI_FALSE;
    s_astDecParam[enCaptionType].u32LastReceptionTime = 0xffffffff;

    s32Ret = ARIBCC_Com_ClearTimer (enCaptionType);
    s32Ret |= ARIBCC_Com_ClearPESPtr (enCaptionType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to ARIBCC_Com_ClearPESPtr\n");
        return HI_FAILURE;
    }
    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_DecodePesData (HI_U8 *pu8CcPes, HI_S32 s32Size)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_CaptionType_E   enCaptionType;
    HI_U8   u8StreamID = 0;
    HI_U8   *pu8PesData = pu8CcPes;
    HI_U8   *pu8PesDataByte = HI_NULL;
    HI_U32  u32PesDataSize = 0;
    HI_U64  u64Pts = 0;


    DEC_TRACE("DecodePesData"
                   "PES = %p, Size = %d\n", pu8CcPes, s32Size);

    if ((HI_NULL == pu8CcPes)
        || (s32Size < ARIBCC_PES_HEADER_LENGTH)
        || (s32Size > CCPES_MAX_SIZE))
    {
        HI_ERR_CC("s32Size : %d invalid\n", s32Size);
        return HI_FAILURE;
    }

    u8StreamID = (HI_U8) GET_BIT (pu8PesData, 24, 8);
    if (ARIBCC_PES_PRIVATE_STREAM_1 == (u8StreamID))
    {
        enCaptionType = ARIB_CAP_CAPTION;
    }
    else if (ARIBCC_PES_PRIVATE_STREAM_2 == (u8StreamID))
    {
        enCaptionType = ARIB_CAP_SUPERIMPOSE;
    }
    else
    {
        HI_ERR_CC("u8StreamID : %#x invalid \n", u8StreamID);
        return HI_FAILURE;
    }

    u64Pts         = 0;
    pu8PesDataByte = HI_NULL;
    s32Ret = _ARIBCC_DEC_CheckCCPESHeader (pu8PesData, &u64Pts, &pu8PesDataByte);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_DEC_CheckCCPESHeader\n");
        return HI_FAILURE;
    }

    if(pu8PesDataByte == NULL)
    {
        HI_ERR_CC("pes data invalid\n");
        return HI_FAILURE;
    }

    u32PesDataSize = (HI_U32) (GET_BIT (pu8PesDataByte, 24, 16) + DG_HEADER_LENGTH);

    DEC_TRACE("\t PTS : %lld, Pes data size : %d \n", u64Pts, u32PesDataSize);

    s32Ret = _ARIBCC_DEC_DecodeDataGroup (enCaptionType, pu8PesDataByte, u32PesDataSize, u64Pts);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_DEC_DecodeDataGroup\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_VOID ARIBCC_DEC_Cycle (HI_VOID)
{
    HI_S32  s32Ret = HI_SUCCESS;
    ARIBCC_DEC_PARAM_S *pstDecParam = s_astDecParam + ARIB_CAP_CAPTION;
    struct timeval  TimeValue;
    struct timezone TimeZone;

    if (pstDecParam->u32LastReceptionTime == 0xffffffff)
    {
        return;
    }

    s32Ret = gettimeofday (&TimeValue, &TimeZone);
    if (s32Ret == -1 )
    {
        HI_ERR_CC("\"ARIBCC_DEC_Cycle\" Check(1) Error!\n");
        return;
    }

    pstDecParam->u32LastReceptionTime ++;

    if ((pstDecParam->u32LastReceptionTime >= (ARIBCC_MANAGEMENT_TIMEOUT * 10))
        || (((TimeValue.tv_sec - pstDecParam->stLastReceptionTimeValue.tv_sec) * 1000)
            + ((TimeValue.tv_usec - pstDecParam->stLastReceptionTimeValue.tv_usec) / 1000)
           >= (ARIBCC_MANAGEMENT_TIMEOUT * 1000)))
    {
        s32Ret = ARIBCC_Com_MessageSend (ARIB_CAP_CAPTION, MSGTYPE_CC_DISP_TIMEOUT, HI_NULL, 0);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("\"ARIBCC_DEC_Cycle\" Check(2) Error!\n");
            return;
        }

        pstDecParam->u32LastReceptionTime = 0xffffffff;
        pstDecParam->bReception = HI_FALSE;
    }

    return;
}

HI_S32 ARIBCC_DEC_GetLanguage (ARIBCC_CaptionType_E enCaptionType,
                        HI_CHAR acISO639LanguageCode[][3], HI_U32 *pu32Count)
{
    HI_U8      u8NumLanguages, i;
    HI_U32     u32CaptionCount = 0;
    ARIBCC_Language_S  *LanguageData;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if ((acISO639LanguageCode == HI_NULL) || (pu32Count == HI_NULL))
    {
        HI_ERR_CC("param is error\n");
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    if (HI_FALSE == pstDecParam->bReception)
    {
        HI_ERR_CC("management not received\n");
        return HI_FAILURE;
    }

    u32CaptionCount = *pu32Count;
    *pu32Count = 0;

    LanguageData = pstDecParam->stCaptionData.fManagement.stLanguageData;
    u8NumLanguages = pstDecParam->stCaptionData.fManagement.u8NumLanguages;
    for (i = 0; i < u8NumLanguages; i++)
    {
        if (i >= u32CaptionCount)
        {
            break;
        }

        strncpy(acISO639LanguageCode[i],LanguageData[i].cISO639LanguageCode,3);
    }

    *pu32Count = i;

    return HI_SUCCESS;
}


HI_S32 ARIBCC_DEC_SetLanguage  (ARIBCC_CaptionType_E enCaptionType, HI_CHAR acLanguage[3])
{
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8  u8LanguageIndex1 = 0, u8LanguageIndex2 = 0;
    HI_U8  u8DMF = 0;
    ARIBCC_Management_S *pstManagementPtr = HI_NULL;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if (acLanguage[0] == pstObject->acISO639LanguageCode[0]
        && acLanguage[1] == pstObject->acISO639LanguageCode[1]
        && acLanguage[2] == pstObject->acISO639LanguageCode[2])
    {
        return HI_SUCCESS;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    if (pstDecParam->bReception)
    {
        s32Ret = ARIBCC_DEC_GetLanguageIndex (enCaptionType, pstObject->acISO639LanguageCode, &u8LanguageIndex1);
        s32Ret |= ARIBCC_DEC_GetLanguageIndex (enCaptionType, acLanguage, &u8LanguageIndex2);
        if (s32Ret != HI_SUCCESS)
        {
            HI_ERR_CC("failed to ARIBCC_DEC_GetLanguageIndex\n");
            return HI_FAILURE;
        }

        if (u8LanguageIndex1 != u8LanguageIndex2)
        {
            if(u8LanguageIndex1 > (CC_LANG_MAX-1))
            {
                HI_ERR_CC("buffer overflow");
                return HI_FAILURE;
            }

            pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);
            u8DMF = GET_DMF_RECEPTION (pstManagementPtr->stLanguageData[u8LanguageIndex1].u8DMF);
            if (DMF_SELECTION == (u8DMF))
            {
                s32Ret = ARIBCC_Com_MessageSend (enCaptionType, MSGTYPE_CC_DISP_CLEAR, HI_NULL, 0);
                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_CC("failed to ARIBCC_Com_MessageSend\n");
                    return HI_FAILURE;
                }
            }
        }
    }

    strncpy(pstObject->acISO639LanguageCode,acLanguage,3);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_GetLanguageIndex (ARIBCC_CaptionType_E enCaptionType, HI_CHAR acLanguage[3], HI_U8 *pu8Index)
{
    ARIBCC_Management_S *pstManagementPtr = HI_NULL;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;
    pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);

    //for brazil there are in total 1 language for one pes.
    if( pstManagementPtr->u8NumLanguages > 0 )
    {
        //mush lest than CC_LANG_MAX
        if (pu8Index)
        {
            *pu8Index = 0;
        }
    }
    else
    {
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_SetMute (ARIBCC_CaptionType_E enCaptionType, HI_BOOL bMute)
{
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8  u8LanguageIndex = 0;
    HI_U8  u8DMF = 0;
    ARIBCC_Management_S *pstManagementPtr = HI_NULL;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    if (((HI_TRUE == bMute) && (HI_FALSE == pstDecParam->bMute))
        || ((HI_FALSE == bMute) && (HI_TRUE == pstDecParam->bMute)))
    {
        pstDecParam->bMute = bMute;

        if ((pstDecParam->bMute) && (pstDecParam->bReception))
        {
            s32Ret = ARIBCC_DEC_GetLanguageIndex (enCaptionType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
            if (s32Ret!= HI_SUCCESS)
            {
                HI_ERR_CC("failed to ARIBCC_DEC_GetLanguageIndex\n");
                return HI_FAILURE;
            }

            if(u8LanguageIndex > (CC_LANG_MAX-1))
            {
                HI_ERR_CC("u8LanguageIndex1=%d is overflow",u8LanguageIndex);
                return HI_FAILURE;
            }

            pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);
            u8DMF = GET_DMF_RECEPTION (pstManagementPtr->stLanguageData[u8LanguageIndex].u8DMF);
            if (DMF_SELECTION == (u8DMF))
            {
                s32Ret = ARIBCC_Com_MessageSend (enCaptionType, MSGTYPE_CC_DISP_CLEAR, HI_NULL, 0);
                if (s32Ret != HI_SUCCESS)
                {
                    HI_ERR_CC("failed to ARIBCC_Com_MessageSend\n");
                    return HI_FAILURE;
                }
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_DEC_GetCaptionInfo (HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo)
{
    HI_U8 i;
    ARIBCC_Management_S *pstManagementPtr = HI_NULL;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    pstDecParam = s_astDecParam + ARIB_CAP_CAPTION;
    pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);

    pstCCAribInfo->enCCAribTMD=pstManagementPtr->u8TMD;
    pstCCAribInfo->u32NumLanguage=pstManagementPtr->u8NumLanguages;

    for(i=0;i<pstCCAribInfo->u32NumLanguage;i++)
    {
            pstCCAribInfo->stCCAribInfonode[i].u8LanguageTag=pstManagementPtr->stLanguageData[i].u8LanguageTag;
            strncpy(pstCCAribInfo->stCCAribInfonode[i].acISO639LanguageCode,pstManagementPtr->stLanguageData[i].cISO639LanguageCode,3);
            pstCCAribInfo->stCCAribInfonode[i].acISO639LanguageCode[3]='\0';
            pstCCAribInfo->stCCAribInfonode[i].enCCAribDF=pstManagementPtr->stLanguageData[i].u8Format;

            if((0x3==pstManagementPtr->stLanguageData[i].u8DMF)||(0x7==pstManagementPtr->stLanguageData[i].u8DMF)
                ||(0xb==pstManagementPtr->stLanguageData[i].u8DMF)||(0xf==pstManagementPtr->stLanguageData[i].u8DMF))
            {
                pstCCAribInfo->stCCAribInfonode[i].enCCAribDMF=HI_UNF_CC_ARIB_DMF_BUTT;
            }
            else
            {
                pstCCAribInfo->stCCAribInfonode[i].enCCAribDMF=pstManagementPtr->stLanguageData[i].u8DMF;
            }

            pstCCAribInfo->stCCAribInfonode[i].enCCAribRollup=
                pstManagementPtr->stLanguageData[i].u8RollupMode<0x2?pstManagementPtr->stLanguageData[i].u8RollupMode:HI_UNF_CC_ARIB_ROLLUP_BUTT;
            pstCCAribInfo->stCCAribInfonode[i].enCCAribTCS=
                pstManagementPtr->stLanguageData[i].u8TCS<0x1?pstManagementPtr->stLanguageData[i].u8TCS:HI_UNF_CC_ARIB_TCS_BUTT;

    }

    return HI_SUCCESS;
}


HI_S32 _ARIBCC_DEC_DecodeDataGroup (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8DataGroup, HI_U16 u16DataSize, HI_U64 u64Pts)
{
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8  u8DataGroupID = 0, u8DataGroupVersion = 0;
    HI_U8  u8DataGroupLinkNumber = 0, u8LastDataGroupLinkNumber = 0;
    HI_U16 u16DataGroupSize = 0;
    HI_U8  u8DataGroupIDPrev = 0;
    HI_U32 u32UnitGroupLength = 0;
    HI_U8  *u8DataGroupDataByte = HI_NULL, *pu8DataUnitLoop = HI_NULL;
    HI_U32 u32DataUnitLoopLength = 0;
    HI_S32 s32MsgType = 0;
    ARIBCC_DataGroup_S   stDataGroup;
    ARIBCC_Management_S  *pstManagementPtr = HI_NULL;
    ARIBCC_UnitGroup_S   *pstUnitGroup = HI_NULL;
    struct timeval       TimeValue;
    struct timezone      TimeZone;
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;
    static HI_S32      s32PreCMPesPacketSize[CCPES_TYPE_MAX];
    HI_U8              u8CMPesSizeChanged = 0;
    HI_U8              u8TMD = 0;
    HI_U64             u64Stm = 0;
    HI_U64             u64MaxPts = 0x200000000LL;

    DEC_TRACE("DecodeDataGroup "
                   "(enCaptionType = %s, u16DataSize = %#x)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"), u16DataSize);

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (pu8DataGroup == HI_NULL)
        || ((u16DataSize == 0) || (u16DataSize > DGS_MAX_SIZE))
        || ((enCaptionType == ARIB_CAP_SUPERIMPOSE) && (u64Pts > (HI_U64)0x1FFFFFFFFLL)))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    u8DataGroupID             = GET_BIT(pu8DataGroup,  0,  6);
    u8DataGroupVersion        = GET_BIT(pu8DataGroup,  6,  2);
    u8DataGroupLinkNumber     = (HI_U8)GET_BIT(pu8DataGroup,  8,  8);
    u8LastDataGroupLinkNumber = (HI_U8)GET_BIT(pu8DataGroup, 16,  8);
    u16DataGroupSize          = (HI_U16)GET_BIT(pu8DataGroup, 24, 16);
    u8DataGroupDataByte       = pu8DataGroup + DG_HEADER_LENGTH;
    u8TMD                     = GET_BIT (u8DataGroupDataByte, 0, 2);

    DEC_TRACE("\t u8DataGroupID : %d, u8DataGroupVersion : %d, "
               "u8DataGroupLinkNumber : %d, u8LastDataGroupLinkNumber : %d\n",
               u8DataGroupID, u8DataGroupVersion, u8DataGroupLinkNumber, u8LastDataGroupLinkNumber);

    /* data_group_id (Data group identification; DGI) */
    /* Ref : Table 9-2 Correspondence to caption data and data group identification */
    if ((DGI_TYPE_MANAGE != GET_DGI_TYPE(u8DataGroupID)) && (!IS_DGI_TYPE_STATE(u8DataGroupID)))
    {
        HI_ERR_CC("Data Group (Type) : 0x%2X invalid\n", GET_DGI_TYPE(u8DataGroupID));
        return HI_FAILURE;
    }
    else if ((DGI_GROUP_A != GET_DGI_GROUP(u8DataGroupID)) && (DGI_GROUP_B != GET_DGI_GROUP(u8DataGroupID)))
    {
        HI_ERR_CC("Data Group (Group) : 0x%2X invalid\n", GET_DGI_GROUP(u8DataGroupID));
        return HI_FAILURE;
    }

    if ((u8DataGroupLinkNumber != 0x00) || (u8LastDataGroupLinkNumber != 0x00))
    {
        HI_ERR_CC("Data Group Link Number      : %u\n", u8DataGroupLinkNumber);
        HI_ERR_CC("Last Data Group Link Number : %u\n", u8LastDataGroupLinkNumber);
        return HI_FAILURE;
    }

    if ((u16DataGroupSize == 0) || (u16DataGroupSize > DGS_MAX_SIZE))
    {
        HI_ERR_CC("Data Group Size : %u\n", u16DataGroupSize);
        return HI_FAILURE;
    }

    if (DGI_TYPE_MANAGE == GET_DGI_TYPE(u8DataGroupID))
    {
        s32Ret = gettimeofday (&TimeValue, &TimeZone);
        if (s32Ret == -1)
        {
            HI_ERR_CC("failed to gettimeofday\n");
            return HI_FAILURE;
        }
        pstDecParam->u32LastReceptionTime = 0;
        pstDecParam->stLastReceptionTimeValue = TimeValue;
        if( s32PreCMPesPacketSize[enCaptionType] != u16DataSize )
        {
            u8CMPesSizeChanged = 1;
            s32PreCMPesPacketSize[enCaptionType] = u16DataSize;
        }
    }

    u8DataGroupIDPrev  = 0x0;
    pstManagementPtr   = HI_NULL;
    pstUnitGroup       = HI_NULL;
    u32UnitGroupLength = 0;

    if (pstDecParam->bReception)
    {
        /* check if management is retransmit */
        u8DataGroupIDPrev = GET_DGI_GROUP(pstDecParam->stCaptionData.fManagement.u8DataGroupID);
        if ((DGI_TYPE_MANAGE == GET_DGI_TYPE(u8DataGroupID))
            && (u8DataGroupIDPrev == GET_DGI_GROUP(u8DataGroupID))
            && (u8CMPesSizeChanged == 0)
            && (u8DataGroupVersion) == pstDecParam->stCaptionData.fManagement.u8DataGroupVersion)
        {
            HI_WARN_CC("retransmit management\n");
            return HI_SUCCESS;
        }

        if (IS_DGI_TYPE_STATE (u8DataGroupID))
        {
            HI_U8 u8LanguageIndex = 0;

            s32Ret = ARIBCC_DEC_GetLanguageIndex (enCaptionType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
            if (s32Ret == HI_SUCCESS)
            {
                pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);
                if (GET_DGI_TYPE (u8DataGroupID) != (pstManagementPtr->stLanguageData[u8LanguageIndex].u8LanguageTag + 1))
                {
                    HI_WARN_CC("language difference\n");
                    return HI_SUCCESS;
                }
            }
            else
            {
                HI_ERR_CC("failed to ARIBCC_DEC_GetLanguageIndex\n");
                return HI_FAILURE;
            }

            /* check OTM of management */
            if (TMD_OFFSET_TIME == (u8TMD))
            {
                HI_U8 *pu8Ptr = HI_NULL, i = 0;

                pu8Ptr = u8DataGroupDataByte + 1;
                for (i = 0; i < 9; i++)
                {
                    /* using nine 4-bit binary coded decimals (BCD). */
                    if (GET_BIT (pu8Ptr, i * 4, 4) > 9)
                    {
                        HI_ERR_CC("OTM is error\n");
                        return HI_FAILURE;
                    }
                }

                u64Stm = ((HI_U64)(HI_U32)((GET_BIT (pu8Ptr,  0, 4) *  10) +  GET_BIT (pu8Ptr,  4, 4)) * 324000000)
                       + ((HI_U64)(HI_U32)((GET_BIT (pu8Ptr,  8, 4) *  10) +  GET_BIT (pu8Ptr, 12, 4)) * 5400000)
                       + ((HI_U64)(HI_U32)((GET_BIT (pu8Ptr, 16, 4) *  10) +  GET_BIT (pu8Ptr, 20, 4)) * 90000)
                       + ((HI_U64)(HI_U32)((GET_BIT (pu8Ptr, 24, 4) * 100) + (GET_BIT (pu8Ptr, 28, 4) * 10) + GET_BIT (pu8Ptr, 32, 4)) * 90);

                if (enCaptionType == ARIB_CAP_SUPERIMPOSE)
                {
                    u64Pts = u64Stm;
                }
            }
        }
    }
    else if (IS_DGI_TYPE_STATE(u8DataGroupID))
    {
        HI_WARN_CC("management not received\n");
        return HI_SUCCESS;
    }

    if ((IS_DGI_TYPE_STATE(u8DataGroupID)) && (GET_DGI_GROUP(u8DataGroupID) != u8DataGroupIDPrev))
    {
        /* group change, Group A <-> Group B */
        s32MsgType = MSGTYPE_CC_DISP_CLEAR;
        HI_INFO_CC("MSGTYPE_CC_DISP_CLEAR\n");
    }
    else
    {
        s32MsgType = MSGTYPE_CC_DISP_PARSE;
        HI_INFO_CC("MSGTYPE_CC_DISP_PARSE\n");

        memset(&stDataGroup, 0, sizeof(stDataGroup));
        stDataGroup.u8DataGroupID             = u8DataGroupID;
        stDataGroup.u8DataGroupVersion        = u8DataGroupVersion;
        stDataGroup.u8DataGroupLinkNumber     = u8DataGroupLinkNumber;
        stDataGroup.u8LastDataGroupLinkNumber = u8LastDataGroupLinkNumber;
        stDataGroup.u16DataGroupSize           = u16DataGroupSize;

        pu8DataUnitLoop       = HI_NULL;
        u32DataUnitLoopLength = 0;

        /* decode caption_management_data */
        if (DGI_TYPE_MANAGE == GET_DGI_TYPE(u8DataGroupID))
        {
            s32Ret = _ARIBCC_DEC_DecodeManagement(enCaptionType, &stDataGroup, u8DataGroupDataByte,
                                                       &pu8DataUnitLoop, &u32DataUnitLoopLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeManagement !\n");
                return HI_FAILURE;
            }
        }
        else if (IS_DGI_TYPE_STATE (u8DataGroupID))
        {
            s32Ret = _ARIBCC_DEC_DecodeStatement (enCaptionType, &stDataGroup, u8DataGroupDataByte,
                                                      &pu8DataUnitLoop, &u32DataUnitLoopLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeStatement !\n");
                return HI_FAILURE;
            }
        }

        if (u32DataUnitLoopLength)
        {
            s32Ret = _ARIBCC_DEC_DecodeDataUnitLoop (enCaptionType, pu8DataUnitLoop, u32DataUnitLoopLength,
                                                         &pstUnitGroup, &u32UnitGroupLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeDataUnitLoop !\n");
                return HI_FAILURE;
            }
        }
        else
        {
            HI_INFO_CC("no Data Unit !\n");
            return HI_SUCCESS;
        }
    }

    if (u64Pts < u64MaxPts)
    {
        s32Ret = _ARIBCC_DEC_GetPresentTimeFromPTS (enCaptionType, u64Pts, &TimeValue);
        if (s32Ret != HI_SUCCESS)
        {
            if (pstUnitGroup != HI_NULL)
            {
                ARIBCC_Com_FreePESPtr (enCaptionType, (HI_U8*) pstUnitGroup);
            }
            HI_ERR_CC("failed to _ARIBCC_DEC_GetPresentTimeFromPTS !\n");
            return HI_FAILURE;
        }
    }
    else
    {
        s32Ret = gettimeofday (&TimeValue, &TimeZone);
        if (s32Ret == -1)
        {
            if (pstUnitGroup != HI_NULL)
            {
                ARIBCC_Com_FreePESPtr (enCaptionType, (HI_U8*) pstUnitGroup);
            }
            HI_ERR_CC("failed to gettimeofday !\n");
            return HI_FAILURE;
        }
    }

    if (s32MsgType == MSGTYPE_CC_DISP_PARSE)
    {
        pstManagementPtr = &(pstDecParam->stCaptionData.fManagement);
        s32Ret = ARIBCC_Com_RegistTimer (enCaptionType, s32MsgType, pstManagementPtr, pstUnitGroup, &TimeValue);
    }
    else
    {
        s32Ret = ARIBCC_Com_RegistTimer (enCaptionType, s32MsgType, HI_NULL, HI_NULL, &TimeValue);
    }
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("decode data group error !\n");
        if (s32MsgType == MSGTYPE_CC_DISP_PARSE && pstUnitGroup)
        {
            ARIBCC_Com_FreePESPtr (enCaptionType, (HI_U8*) pstUnitGroup);
        }
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

/* Ref : 9.3.1 Caption management data */
HI_S32 _ARIBCC_DEC_DecodeManagement(ARIBCC_CaptionType_E enCaptionType, ARIBCC_DataGroup_S *pstDataGroup,
                                    HI_U8 *pu8ManagementData, HI_U8 **ppu8DataUnitLoop, HI_U32 *pu32DataUnitLoopLength)
{
    HI_U8  u8TMD = 0;
    HI_U16 au16OTM[4];
    HI_U8  u8NumLanguages = 0, u8LanguageTag = 0, u8DMF = 0;
    HI_U8  u8DC = 0, u8Format = 0, u8TCS = 0, u8RollupMode = 0, i = 0;
    HI_U32 u32DataUnitLoopLength = 0;
    HI_U8  *pu8Ptr = HI_NULL;
    HI_U8  *pu8ISO639LanguageCode = HI_NULL;
    ARIBCC_Language_S  astLanguageData[CC_LANG_MAX];
    ARIBCC_Management_S *pstManagement = HI_NULL;
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    DEC_TRACE("DecodeManagement "
                   "(enCaptionType = %s, pstDataGroup = %p, pu8ManagementData = %p)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pstDataGroup, pu8ManagementData);

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (pstDataGroup == HI_NULL)
        || (pu8ManagementData == HI_NULL)
        || (ppu8DataUnitLoop == HI_NULL)
        || (pu32DataUnitLoopLength == HI_NULL))
    {
        HI_ERR_CC("CCDEC_NULL_POINTER !\n");
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    *ppu8DataUnitLoop       = HI_NULL;
    *pu32DataUnitLoopLength = 0;

    /* Time control mode */
    u8TMD = GET_BIT(pu8ManagementData, 0, 2);

    if ((enCaptionType == ARIB_CAP_CAPTION) && (TMD_FREE != (u8TMD)))
    {
        HI_ERR_CC("u8TMD : 0x%2X\n", u8TMD);
        return HI_FAILURE;
    }
    else if ((enCaptionType == ARIB_CAP_SUPERIMPOSE) && ((TMD_FREE!=(u8TMD)) && (TMD_REAL_TIME!=(u8TMD))))
    {
        HI_ERR_CC("u8TMD : 0x%2X\n", u8TMD);
        return HI_FAILURE;
    }

    pu8Ptr = pu8ManagementData + 1;

    /* Offset time */
    memset (au16OTM, 0, sizeof(au16OTM));
    #if 0 /* not support offset time yet */
    if (TMD_OFFSET_TIME == (u8TMD))
    {
        /* Offset time is coded in the order of hour, minute, second and millisecond */
        /* using nine 4-bit binary coded decimals (BCD). */
        for (i = 0; i < 9; i++)
        {
            if (GET_BIT(pu8Ptr, (i * 4), 4) > 9)
            {
                HI_ERR_CC("OTM decode error !\n");
                return HI_FAILURE;
            }
        }

        au16OTM[0] = (GET_BIT (pu8Ptr,  0, 4) *  10) +  GET_BIT (pu8Ptr,  4, 4);
        au16OTM[1] = (GET_BIT (pu8Ptr,  8, 4) *  10) +  GET_BIT (pu8Ptr, 12, 4);
        au16OTM[2] = (GET_BIT (pu8Ptr, 16, 4) *  10) +  GET_BIT (pu8Ptr, 20, 4);
        au16OTM[3] = (GET_BIT (pu8Ptr, 24, 4) * 100) + (GET_BIT (pu8Ptr, 28, 4) * 10) + GET_BIT (pu8Ptr, 32, 4);
        pu8Ptr += OTM_FLD_LENGTH;
    }
    #endif

    /* Number of languages */
    u8NumLanguages = (HI_U8) GET_BIT(pu8Ptr, 0, 8);
    DEC_TRACE("\t num_languages = %d\n",u8NumLanguages);

    if ((u8NumLanguages == 0) || (u8NumLanguages > CC_LANG_MAX))
    {
        HI_ERR_CC("Num Languages : %u\n", u8NumLanguages);
        return HI_FAILURE;
    }
    pu8Ptr ++;

    memset (astLanguageData, 0, sizeof(astLanguageData));
    for (i = 0; i < u8NumLanguages; i++)
    {
        /* language_tag (Identification of language): Numbers to identify the language. */
        /* 0 means the 1st language, and 7, the 8th language, and so on. */
        u8LanguageTag  = GET_BIT (pu8Ptr, 0, 3);
        if (u8LanguageTag > LANG_ID_2ND)
        {
            HI_ERR_CC("Language Tag (%u) : %u\n", i, u8LanguageTag);
            s32Ret = HI_FAILURE;
            break;
        }
        DEC_TRACE("\t language_tag = %d\n",u8LanguageTag);

        /* DMF (Display mode) */
        u8DMF = GET_BIT (pu8Ptr, 4, 4);
        if ((u8DMF != DMF_AUTO_AND_AUTO) && (u8DMF != DMF_AUTO_AND_SELECT) && (u8DMF != DMF_SELECT_AND_SELECT))
        {
            HI_ERR_CC("u8DMF (%u) : 0x%2X\n", i, u8DMF);
            s32Ret = HI_FAILURE;
            break;
        }
        pu8Ptr ++;
        DEC_TRACE("\t DMF = %#x\n",u8DMF);

        /* ISO_639_language_code (Language code) */
        pu8ISO639LanguageCode = pu8Ptr;
        DEC_TRACE("\t ISO639Language = %c%c%c\n",pu8ISO639LanguageCode[0],pu8ISO639LanguageCode[1],pu8ISO639LanguageCode[2]);

        /* format (display format) */
        u8Format = GET_BIT(pu8Ptr, 24,  4);
        if ((DF_HORI_960X540 != (u8Format)) && (DF_HORI_720X480 != (u8Format)))
        {
            HI_ERR_CC("u8Format (%u) : 0x%2X\n", i, u8Format);
            s32Ret = HI_FAILURE;
            break;
        }

        /* TCS (Character coding) */
        u8TCS = GET_BIT(pu8Ptr, 28,  2);
        if (TCS_8UNIT != (u8TCS))
        {
            HI_ERR_CC("u8TCS (%u) : 0x%2X\n", i, u8TCS);
            s32Ret = HI_FAILURE;
            break;
        }

        /* rollup_mode */
        u8RollupMode = GET_BIT(pu8Ptr, 30,  2);
        if (((enCaptionType == ARIB_CAP_SUPERIMPOSE) && ((RM_NON_ROLLUP != (u8RollupMode)) && (RM_ROLLUP != (u8RollupMode))))
            || (enCaptionType == ARIB_CAP_SUPERIMPOSE))
        {
            u8RollupMode = RM_NON_ROLLUP;
        }

        astLanguageData[i].u8LanguageTag           = u8LanguageTag;
        astLanguageData[i].u8DMF                   = u8DMF;
        astLanguageData[i].u8DC                    = u8DC;
        memcpy(astLanguageData[i].cISO639LanguageCode, pu8ISO639LanguageCode, 3);
        astLanguageData[i].cISO639LanguageCode[3] = '\0';
        astLanguageData[i].u8Format                = u8Format;
        astLanguageData[i].u8TCS                   = u8TCS;
        astLanguageData[i].u8RollupMode            = u8RollupMode;

        pu8Ptr += 4;
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("decode languages error !\n");
        return HI_FAILURE;
    }

    /* data_unit_loop_length */
    u32DataUnitLoopLength = (HI_U32)GET_BIT(pu8Ptr, 0, 24);
    if (u32DataUnitLoopLength > DUL_MAX_SIZE)
    {
        HI_ERR_CC("Data Unit Loop Length : %u\n", u32DataUnitLoopLength);
        return HI_FAILURE;
    }

    pu8Ptr += 3;

    pstManagement = &(pstDecParam->stCaptionData.fManagement);
    memset(pstManagement, 0, sizeof(ARIBCC_Management_S));
    pstManagement->u8DataGroupID             = pstDataGroup->u8DataGroupID;
    pstManagement->u8DataGroupVersion        = pstDataGroup->u8DataGroupVersion;
    pstManagement->u8DataGroupLinkNumber     = pstDataGroup->u8DataGroupLinkNumber;
    pstManagement->u8LastDataGroupLinkNumber = pstDataGroup->u8LastDataGroupLinkNumber;
    pstManagement->u8TMD                     = u8TMD;
    pstManagement->u16OTM[0]                  = au16OTM[0]; //hour
    pstManagement->u16OTM[1]                  = au16OTM[1]; //minute
    pstManagement->u16OTM[2]                  = au16OTM[2]; //second
    pstManagement->u16OTM[3]                  = au16OTM[3]; //millisecond
    pstManagement->u8NumLanguages            = u8NumLanguages;
    memcpy (pstManagement->stLanguageData, astLanguageData, sizeof(pstManagement->stLanguageData));

    pstDecParam->bReception = HI_TRUE;

    *ppu8DataUnitLoop       = pu8Ptr;
    *pu32DataUnitLoopLength = u32DataUnitLoopLength;


    return HI_SUCCESS;
}

/* Ref : 9.3.2 Caption statement data */
HI_S32 _ARIBCC_DEC_DecodeStatement (ARIBCC_CaptionType_E enCaptionType, ARIBCC_DataGroup_S *pstDataGroup,
                                    HI_U8 *pu8ManagementData, HI_U8 **ppu8DataUnitLoop, HI_U32 *pu32DataUnitLoopLength)
{
    ARIBCC_OBJECT_S *pstObject = ARIBCC_OBJ_Get();
    HI_U8  u8TMD = 0;
    HI_U16 au16STM[4] = {0,0,0,0};
    HI_U8  u8LanguageIndex = 0;
    HI_U8  u8DMF = 0;
    HI_U32 u32DataUnitLoopLength = 0, i;
    HI_U8 *pu8Ptr = HI_NULL;
    ARIBCC_Management_S *pstManagement = HI_NULL;
    AribCC_Statement_t  *pstStatement = HI_NULL;
    HI_S32 s32Ret = HI_SUCCESS;
    ARIBCC_DEC_PARAM_S *pstDecParam = HI_NULL;

    DEC_TRACE("DecodeStatement "
                   "(enCaptionType = %s, pstDataGroup = %p, pu8ManagementData = %p) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pstDataGroup, pu8ManagementData);

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (pstDataGroup == HI_NULL)
        || (pu8ManagementData == HI_NULL)
        || (ppu8DataUnitLoop == HI_NULL)
        || (pu32DataUnitLoopLength == HI_NULL))
    {
        HI_ERR_CC("parameter error\n");
        return HI_FAILURE;
    }

    pstDecParam = s_astDecParam + enCaptionType;

    *ppu8DataUnitLoop       = HI_NULL;
    *pu32DataUnitLoopLength = 0;

    u8TMD = GET_BIT(pu8ManagementData, 0, 2);

    if ((enCaptionType == ARIB_CAP_CAPTION) && (TMD_FREE != (u8TMD)))
    {
        HI_ERR_CC("u8TMD : 0x%2X\n", u8TMD);
        return HI_FAILURE;
    }
    else if ((enCaptionType == ARIB_CAP_SUPERIMPOSE) && ((TMD_FREE != (u8TMD)) && (TMD_REAL_TIME != (u8TMD))))
    {
        HI_ERR_CC("u8TMD : 0x%2X\n", u8TMD);
        return HI_FAILURE;
    }

    pu8Ptr = pu8ManagementData + 1;

    if (TMD_REAL_TIME == (u8TMD) || TMD_OFFSET_TIME == (u8TMD))
    {
        /* Presentation start time is coded in the order of hour, minute, second and millisecond */
        /* using nine 4-bit binary coded decimals (BCD). */
        for (i = 0; i < 9; i++)
        {
            if (GET_BIT (pu8Ptr, i * 4, 4) > 9)
            {
                HI_ERR_CC("STM decode error !\n");
                return HI_FAILURE;
            }
        }

        au16STM[0] = (GET_BIT (pu8Ptr,  0, 4) *  10) +  GET_BIT (pu8Ptr,  4, 4);
        au16STM[1] = (GET_BIT (pu8Ptr,  8, 4) *  10) +  GET_BIT (pu8Ptr, 12, 4);
        au16STM[2] = (GET_BIT (pu8Ptr, 16, 4) *  10) +  GET_BIT (pu8Ptr, 20, 4);
        au16STM[3] = (GET_BIT (pu8Ptr, 24, 4) * 100) + (GET_BIT (pu8Ptr, 28, 4) * 10) + GET_BIT (pu8Ptr, 32, 4);

        pu8Ptr += STM_FLD_LENGTH;
    }

    u32DataUnitLoopLength = (HI_U32) GET_BIT (pu8Ptr, 0, 24);
    if ((u32DataUnitLoopLength == 0) || (u32DataUnitLoopLength > DUL_MAX_SIZE))
    {
        HI_ERR_CC("Data Unit Loop Length : %u\n", (unsigned int) u32DataUnitLoopLength);
        return HI_FAILURE;
    }
    pu8Ptr += 3;

    u8LanguageIndex = 0;
    pstManagement = &(pstDecParam->stCaptionData.fManagement);

    s32Ret = ARIBCC_DEC_GetLanguageIndex (enCaptionType, pstObject->acISO639LanguageCode, &u8LanguageIndex);
    if (s32Ret == HI_SUCCESS)
    {
        if(u8LanguageIndex > (CC_LANG_MAX-1))
        {
            HI_ERR_CC("u8LanguageIndex : %d\n", u8LanguageIndex);
            return HI_FAILURE;
        }
        if (GET_DGI_TYPE (pstDataGroup->u8DataGroupID) !=
            (pstManagement->stLanguageData[u8LanguageIndex].u8LanguageTag + 1))
        {
            HI_ERR_CC("data_group_id is error\n");
            return HI_FAILURE;
        }
    }
    else
    {
        HI_ERR_CC("failed to ARIBCC_DEC_GetLanguageIndex\n");
        return HI_FAILURE;
    }

    u8DMF = pstManagement->stLanguageData[u8LanguageIndex].u8DMF;
    if ((u8DMF != DMF_AUTO_AND_AUTO) && (u8DMF != DMF_AUTO_AND_SELECT) && (u8DMF != DMF_SELECT_AND_SELECT))
    {
        HI_ERR_CC("DMF = 0x%2X is error\n", u8DMF);
        return HI_FAILURE;
    }
    if (DMF_SELECT_AND_SELECT == u8DMF)
    {
        if (pstDecParam->bMute)
        {
            HI_ERR_CC("mute, no display\n");
            return HI_SUCCESS;
        }
    }


    pstStatement = &(pstDecParam->stCaptionData.fStatement);
    pstStatement->u8DataGroupID             = pstDataGroup->u8DataGroupID;
    pstStatement->u8DataGroupVersion        = pstDataGroup->u8DataGroupVersion;
    pstStatement->u8DataGroupLinkNumber     = pstDataGroup->u8DataGroupLinkNumber;
    pstStatement->u8LastDataGroupLinkNumber = pstDataGroup->u8LastDataGroupLinkNumber;
    pstStatement->u8TMD                     = u8TMD;
    memcpy(pstStatement->au16STM,au16STM,sizeof(pstStatement->au16STM));

    *ppu8DataUnitLoop       = pu8Ptr;
    *pu32DataUnitLoopLength = u32DataUnitLoopLength;


    return HI_SUCCESS;
}

/* Ref : 9.4 Structure of data unit */
HI_S32 _ARIBCC_DEC_DecodeDataUnitLoop (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8DataUnitLoop, HI_U32 s32DataUnitLoopLength,
                                ARIBCC_UnitGroup_S **ppstUnitGroup, HI_U32 *pu32UnitGroupLength)
{
    HI_U32 u32CcPesSize = 0;
    HI_U8  u8UnitSeparator = 0, u8DataUnitParameter = 0;
    HI_U32 u32UnitGroupLength = 0, u32UnitLength, u32DataUnitSize = 0, PaddingLength;
    HI_U8  *pu8DataUnit = HI_NULL, *pu8DataUnitDataByte = HI_NULL, *PaddingPtr=HI_NULL;
    ARIBCC_UnitGroup_S *pstUnitGroup = HI_NULL;
    ARIBCC_Unit_S      *pstUnit = HI_NULL;
    HI_S32      s32Ret = HI_SUCCESS;;

    DEC_TRACE("DecodeDataUnitLoop (enCaptionType = %s, "
                   "pu8DataUnitLoop = %p, u32DataUnitLoopLength = %u) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8DataUnitLoop, s32DataUnitLoopLength);

    if ((pu8DataUnitLoop == HI_NULL)
        || (s32DataUnitLoopLength == 0)
        || (ppstUnitGroup == HI_NULL)
        || (pu32UnitGroupLength == HI_NULL))
    {
        HI_ERR_CC("parameter error : %#x %#x!\n", pu8DataUnitLoop, s32DataUnitLoopLength);
        return HI_FAILURE;
    }

    if (s32DataUnitLoopLength > DUL_MAX_SIZE)
    {
        HI_ERR_CC("s32DataUnitLoopLength : %d !\n", s32DataUnitLoopLength);
        return HI_FAILURE;
    }

    *ppstUnitGroup       = HI_NULL;
    *pu32UnitGroupLength = 0;

    u32CcPesSize  = (s32DataUnitLoopLength * 8);
    if (u32CcPesSize < 256)
    {
        u32CcPesSize = 256;
    }
    else if (u32CcPesSize > CCPES_BUFFER_SIZE)
    {
        u32CcPesSize = CCPES_BUFFER_SIZE;
    }

    s32Ret = ARIBCC_Com_AllocPESPtr (enCaptionType, (HI_U8**) &pstUnitGroup, u32CcPesSize);
    if((s32Ret != HI_SUCCESS) || (pstUnitGroup == NULL))
    {
        HI_ERR_CC("failed to ARIBCC_Com_AllocPESPtr\n");
        return HI_FAILURE;
    }

    pstUnit = pstUnitGroup->stUnit;
    u32UnitGroupLength = 0;
    for (pu8DataUnit = pu8DataUnitLoop;
         pu8DataUnit < (pu8DataUnitLoop + s32DataUnitLoopLength);
         pu8DataUnit += (u32DataUnitSize + DU_HEADER_LENGTH))
    {
        u8UnitSeparator     = (HI_U8) GET_BIT(pu8DataUnit,  0,  8);
        u8DataUnitParameter = (HI_U8) GET_BIT(pu8DataUnit,  8,  8);
        u32DataUnitSize     = (HI_U32) GET_BIT(pu8DataUnit, 16, 24);

        /* Data unit separator code should be 0x1F */
        if (u8UnitSeparator != 0x1F)
        {
            HI_ERR_CC("Unit Separator : %u\n", u8UnitSeparator);
            s32Ret = HI_FAILURE;
            break;
        }
        else if ((DATA_UNIT_STATE_BODY != (u8DataUnitParameter)) && (DATA_UNIT_1BYTE_DRCS != (u8DataUnitParameter))
                 && (DATA_UNIT_2BYTE_DRCS != (u8DataUnitParameter)) && (DATA_UNIT_BITMAP != (u8DataUnitParameter)))
        {
            HI_ERR_CC("Data Unit Parameter : 0x%2X\n", u8DataUnitParameter);
            s32Ret = HI_FAILURE;
            break;
        }
        else if (u32DataUnitSize > DU_MAX_SIZE)
        {
            HI_ERR_CC("Data Unit Size : %u\n", (unsigned int) u32DataUnitSize);
            s32Ret = HI_FAILURE;
            break;
        }

        pu8DataUnitDataByte = pu8DataUnit + DU_HEADER_LENGTH;
        u32UnitLength       = 0;

        pstUnit->pu8UnitData = ((HI_U8*) pstUnit) + sizeof(ARIBCC_Unit_S);
        if (DATA_UNIT_STATE_BODY == (u8DataUnitParameter))
        {
            s32Ret = _ARIBCC_DEC_DecodeStatementBody (enCaptionType, pu8DataUnitDataByte, u32DataUnitSize,
                                                          (ARIBCC_StatementBody_S*) pstUnit->pu8UnitData, &u32UnitLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeStatementBody !\n");
            }
        }
        else if (DATA_UNIT_1BYTE_DRCS == (u8DataUnitParameter))
        {
            s32Ret = _ARIBCC_DEC_DecodeDrcsData (enCaptionType, ARIB_DRCS_1BYTE, pu8DataUnitDataByte, u32DataUnitSize,
                                                     pstUnit->pu8UnitData, &u32UnitLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeDrcsData !\n");
            }
        }
        else if (DATA_UNIT_2BYTE_DRCS == (u8DataUnitParameter))
        {
            s32Ret = _ARIBCC_DEC_DecodeDrcsData (enCaptionType, ARIB_DRCS_2BYTE, pu8DataUnitDataByte, u32DataUnitSize,
                                                     pstUnit->pu8UnitData, &u32UnitLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeDrcsData !\n");
            }
        }
        else if (DATA_UNIT_BITMAP == (u8DataUnitParameter))
        {
            s32Ret = _ARIBCC_DEC_DecodeBitmapData (enCaptionType, pu8DataUnitDataByte, u32DataUnitSize,
                                                       pstUnit->pu8UnitData, &u32UnitLength);
            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_DEC_DecodeBitmapData !\n");
            }
        }
        if (s32Ret != HI_SUCCESS)
        {
            break;
        }

        pstUnit->u32UnitId     = (HI_U32)u8DataUnitParameter;
        pstUnit->u32UnitLength = u32UnitLength;

        PaddingPtr           = ((HI_U8*) pstUnit) + sizeof(ARIBCC_Unit_S) + u32UnitLength;
        PaddingLength        = GET_PADDING_SIZE(sizeof(ARIBCC_Unit_S) + u32UnitLength);
        if (PaddingLength != 0)
        {
            memset (PaddingPtr, 0, PaddingLength);
        }

        pstUnit = (ARIBCC_Unit_S*) (PaddingPtr + PaddingLength);
        u32UnitGroupLength += (sizeof(ARIBCC_Unit_S) + u32UnitLength + PaddingLength);
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("decode data_unit error !\n");
        ARIBCC_Com_FreePESPtr (enCaptionType, (HI_U8*) pstUnitGroup);
        return HI_FAILURE;
    }

    pstUnitGroup->u32UnitGroupLength = u32UnitGroupLength;

    *ppstUnitGroup       = pstUnitGroup;
    *pu32UnitGroupLength = u32UnitGroupLength + sizeof(HI_U32);


    return HI_SUCCESS;
}


HI_S32 _ARIBCC_DEC_DecodeStatementBody (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8StatementBody, HI_U32 u32StatementBodySize,
                                 ARIBCC_StatementBody_S *pstStatementBody, HI_U32 *pu32StatementBodySize)
{
    HI_U32  u32InvocationArea = 0, u32DesignArea = 0;
    HI_U32  u32ControlSize = 0, u32ControlCount = 0, u32ControlLength = 0;
    HI_U32  u32CodeSize = 0, u32DataLength = 0;
    HI_U32  u32PaddingLength = 0;
    HI_U8   u8Code = 0, u8MacroId = 0;
    HI_U8   *pu8CodePtr = HI_NULL, *PaddingPtr = HI_NULL, *pu8PrevPtr = HI_NULL;
    HI_S32  as32Control[5] = {0};
    HI_S32  i;
    ARIBCC_StatementBody_S *pstStatementBodyPtr = HI_NULL;
    ARIBCC_Control_S       *pstControlPtr = HI_NULL;
    ARIBCC_Data_S          *pstDataPtr = HI_NULL;
    ARIBCC_GraphSet_E       aenDesignation[4]; /* Designation of graphic sets */
    ARIBCC_CodeElement_E    enInvocationPrev;
    ARIBCC_CodeElement_E    aenInvocation[2];
    HI_BOOL               bSingleShift;
    HI_S32 s32Ret = HI_SUCCESS;

    DEC_TRACE("DecodeStatementBody (enCaptionType = %s, "
                   "pu8StatementBody = %p, u32StatementBodySize = %u) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8StatementBody, u32StatementBodySize);

    if ((pu8StatementBody == HI_NULL) || (pstStatementBody == HI_NULL) || (pu32StatementBodySize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if (u32StatementBodySize > DUL_MAX_SIZE)
    {
        HI_ERR_CC("u32StatementBodySize : %d !\n", u32StatementBodySize);
        return HI_FAILURE;
    }

    //memset (pstStatementBody, 0, sizeof(AribCC_StatementBody_t));
    *pu32StatementBodySize = 0;

    aenDesignation[0] = ARIB_GSET_ALPHANUMERIC;                     // G0: Alphanumeric
    aenDesignation[1] = ARIB_GSET_ALPHANUMERIC;                     // G1: Alphanumeric
    aenDesignation[2] = ARIB_GSET_LATIN_EXTENSION;                  // G2: Latin Extension
    aenDesignation[3] = ARIB_GSET_SPECIAL;                          // G3: Special character
    aenInvocation[0]  = ARIB_CELEMENT_G0;                           // GL : G0(LS0)
    aenInvocation[1]  = ARIB_CELEMENT_G2;                           // GR : G2(LS2R)
    bSingleShift      = HI_FALSE;
    enInvocationPrev  = aenInvocation[0];

    pu8CodePtr        = pu8StatementBody;
    pstStatementBodyPtr  = pstStatementBody;
    pu8PrevPtr           = HI_NULL;

    while (pu8CodePtr < (pu8StatementBody + u32StatementBodySize))
    {
        u32ControlCount  = 0;
        u32ControlLength = 0;
        pstControlPtr    = pstStatementBodyPtr->stControl;
        u32DataLength    = 0;

        while (pu8CodePtr < (pu8StatementBody + u32StatementBodySize))
        {
            if ((pu8CodePtr == pu8PrevPtr) && (u32DataLength != 0))
            {
                s32Ret = HI_FAILURE;
                break;
            }
            pu8PrevPtr = pu8CodePtr;

            /* GL/GR */
            if ((IS_AREA_GL(*pu8CodePtr)) || (IS_AREA_GR(*pu8CodePtr)))
            {
                u8Code = *pu8CodePtr;
                u32InvocationArea = IS_AREA_GL(u8Code) ? 0 : 1;
                /* (G0/G1/G2/G3) */
                u32DesignArea = (HI_U32)aenInvocation[u32InvocationArea] - (HI_U32)ARIB_CELEMENT_G0;

                if (ARIB_GSET_MACRO == aenDesignation[u32DesignArea])
                {
                    if (IS_DEFAULT_MACRO (u8Code))
                    {
                        pstDataPtr = (ARIBCC_Data_S*) (((HI_U8*) pstControlPtr) + u32ControlLength);
                        pstDataPtr->pu8Data = ((HI_U8*) pstDataPtr) + sizeof(ARIBCC_Data_S);
                        *(pstDataPtr->pu8Data + u32DataLength) = u8Code;
                        u32DataLength ++;

                        u8MacroId = GET_MACRO_ID(u8Code);
                        for (i = 0; i < 4; i++)
                        {
                            aenDesignation[i] = gDefaultMacro[u8MacroId].astDesignation[i].enGraphSet;
                        }
                        for (i = 0; i < 2; i++)
                        {
                            aenInvocation[i] = CTRL_TO_INVOKE_ELEM (gDefaultMacro[u8MacroId].aenInvocationId[i]);
                        }
                    }

                    pu8CodePtr ++;
                }
                else
                {
                    pstDataPtr = (ARIBCC_Data_S*) (((HI_U8*) pstControlPtr) + u32ControlLength);
                    pstDataPtr->pu8Data = ((HI_U8*) pstDataPtr) + sizeof(ARIBCC_Data_S);
                    *(pstDataPtr->pu8Data + u32DataLength) = u8Code;
                    pu8CodePtr ++;
                    u32DataLength ++;

                    if ((aenDesignation[u32DesignArea]) & 0x0100) /* 2-byete code */
                    {
                        *(pstDataPtr->pu8Data + u32DataLength) = *pu8CodePtr;
                        pu8CodePtr ++;
                        u32DataLength ++;
                    }
                }

                if ((IS_AREA_GL (u8Code)) && bSingleShift)
                {
                    aenInvocation[0] = enInvocationPrev;
                    bSingleShift = HI_FALSE;
                }
            }
            else if (IS_SINGLE_SHIFT (*pu8CodePtr))
            {
                u8Code    = *pu8CodePtr;
                pstDataPtr = (ARIBCC_Data_S*) (((HI_U8*) pstControlPtr) + u32ControlLength);
                pstDataPtr->pu8Data = ((HI_U8*) pstDataPtr) + sizeof(ARIBCC_Data_S);
                *(pstDataPtr->pu8Data + u32DataLength) = u8Code;
                pu8CodePtr ++;
                u32DataLength ++;

                enInvocationPrev = aenInvocation[0];
                if (u8Code == C0_SS2)
                {
                    aenInvocation[0] = ARIB_CELEMENT_G2;
                }
                else
                {
                    aenInvocation[0] = ARIB_CELEMENT_G3;
                }
                bSingleShift = HI_TRUE;
            }
            else if (IS_AREA_SP (*pu8CodePtr))
            {
                if ((*pu8CodePtr != SP_SP) && (*pu8CodePtr != SP_DEL))
                {
                    s32Ret = HI_FAILURE;
                    break;
                }

                pstDataPtr = (ARIBCC_Data_S*) (((HI_U8*) pstControlPtr) + u32ControlLength);
                pstDataPtr->pu8Data = ((HI_U8*) pstDataPtr) + sizeof(ARIBCC_Data_S);
                *(pstDataPtr->pu8Data + u32DataLength) = *pu8CodePtr;
                pu8CodePtr ++;
                u32DataLength ++;
            }
            else if (IS_AREA_CONTROL (*pu8CodePtr))
            {
                if (u32DataLength != 0)
                {
                    break;
                }

                u32CodeSize    = (HI_U32) (pu8StatementBody + u32StatementBodySize - pu8CodePtr);
                u32ControlSize = sizeof(as32Control);

                if (IS_AREA_C0 (*pu8CodePtr))
                {
                    s32Ret = _ARIBCC_DEC_DecodeC0Control (enCaptionType, pu8CodePtr, &u32CodeSize,
                                                              (ARIBCC_Control_S*) as32Control, &u32ControlSize);
                }
                else if (IS_AREA_CSI(*pu8CodePtr))
                {
                    s32Ret = _ARIBCC_DEC_DecodeCsiControl (enCaptionType, pu8CodePtr, &u32CodeSize,
                                                              (ARIBCC_Control_S*) as32Control, &u32ControlSize);
                }
                else if (IS_AREA_C1 (*pu8CodePtr))
                {
                    s32Ret = _ARIBCC_DEC_DecodeC1Control (enCaptionType, pu8CodePtr, &u32CodeSize,
                                                              (ARIBCC_Control_S*) as32Control, &u32ControlSize);
                }

                if (s32Ret != HI_SUCCESS)
                {
                    break;
                }

                if (IS_AREA_C0 (*pu8CodePtr))
                {
                    if (IS_ARIB_INVOCATION (as32Control[0]))
                    {
                        u32InvocationArea = (HI_U32) CTRL_TO_INVOKE_AREA (as32Control[0]);
                        aenInvocation[u32InvocationArea] = CTRL_TO_INVOKE_ELEM (as32Control[0]);
                    }
                    else if (ARIB_ESC_DES == as32Control[0])
                    {
                        aenDesignation[as32Control[1] - (HI_U32)ARIB_CELEMENT_G0] = (ARIBCC_GraphSet_E) as32Control[2];
                    }
                }

                pu8CodePtr += u32CodeSize;
                memcpy (((HI_U8*)pstControlPtr) + u32ControlLength, as32Control, u32ControlSize);

                if (u32ControlSize != 0)
                {
                    u32ControlCount ++;
                }
                u32ControlLength += u32ControlSize;
            }
        }
        if (s32Ret != HI_SUCCESS)
        {
            break;
        }

        pstStatementBodyPtr->u32ControlCount  = u32ControlCount;
        pstStatementBodyPtr->u32ControlLength = u32ControlLength;

        pstDataPtr = (ARIBCC_Data_S*) (((HI_U8*) pstControlPtr) + u32ControlLength);
        pstDataPtr->u32DataLength = u32DataLength;
        pstDataPtr->pu8Data = ((HI_U8*) pstDataPtr) + sizeof(ARIBCC_Data_S);
        PaddingPtr = ((HI_U8*) pstDataPtr->pu8Data) + u32DataLength;
        u32PaddingLength = GET_PADDING_SIZE (u32DataLength);
        if (u32PaddingLength != 0)
        {
            memset (PaddingPtr, 0, u32PaddingLength);
        }

        pstStatementBodyPtr = (ARIBCC_StatementBody_S*) (PaddingPtr + u32PaddingLength);
    }
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("decode statement body error !\n");
        return HI_FAILURE;
    }

    *pu32StatementBodySize = (HI_U32) (((HI_U8*) pstStatementBodyPtr) - ((HI_U8*) pstStatementBody));


    return HI_SUCCESS;
}

/* Ref : Table 7-15 C0 control set */
HI_S32 _ARIBCC_DEC_DecodeC0Control (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                             ARIBCC_Control_S *pstControl, HI_U32 *pu32ControlSize)
{
    HI_U8    u8Class1= 0, u8Class2 = 0;
    HI_U8    *pu8CodePtr = HI_NULL, *pu8FinalBytePtr = HI_NULL;
    HI_U32   u32CodeSize = 0, u32ControlSize = 0, u32ControlDataSize = 0, i = 0;
    HI_S32   as32ControlData[4];
    ARIBCC_CtrlSet_E        enControlId = ARIB_C0_NUL;
    ARIBCC_GraphSet_E       enGraphSet;
    ARIBCC_CodeElement_E    enCodeElement = ARIB_CELEMENT_G0;
    HI_S32  s32Ret = HI_SUCCESS;

    DEC_TRACE("DecodeC0Control (enCaptionType = %s, "
                   "pu8Code = %p) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8Code);

    if ((pu8Code == HI_NULL) || (pu32CodeSize == HI_NULL) || (pstControl == HI_NULL) || (pu32ControlSize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if ((*pu32CodeSize == 0) || (*pu32CodeSize > DUL_MAX_SIZE) || (*pu32ControlSize == 0))
    {
        HI_ERR_CC("invalid size !\n");
        return HI_FAILURE;
    }

    u32CodeSize    = *pu32CodeSize;
    u32ControlSize = *pu32ControlSize;
    //memset (pstControl, 0, sizeof(AribCC_Control_t));
    *pu32CodeSize    = 0;
    *pu32ControlSize = 0;

    enControlId = ARIB_C0_NUL;
    pu8CodePtr   = pu8Code;
    memset (as32ControlData, 0, sizeof(as32ControlData));
    u32ControlDataSize = 0;
    switch (*pu8CodePtr)
    {
        case C0_NULL:
            break;

        case C0_BEL:
            s32Ret = HI_FAILURE;
            break;

        case C0_APB:
            enControlId = ARIB_C0_APB;
            break;

        case C0_APF:
            enControlId = ARIB_C0_APF;
            break;

        case C0_APD:
            enControlId = ARIB_C0_APD;
            break;

        case C0_APU:
            enControlId = ARIB_C0_APU;
            break;

        case C0_CS:
            enControlId = ARIB_C0_CS;
            break;

        case C0_APR:
            enControlId = ARIB_C0_APR;
            break;

        case C0_CAN:
            s32Ret = HI_FAILURE;
            break;

//        case C0_SS2:
//            enControlId = ARIB_C0_SS2;
//            break;

        case C0_LS1:
            enControlId = ARIB_C0_LS1;
            break;

        case C0_LS0:
            enControlId = ARIB_C0_LS0;
            break;

        case C0_PAPF:
            enControlId = ARIB_C0_PAPF;
            pu8CodePtr ++;
            if ((*pu8CodePtr < 0x40) || (*pu8CodePtr > 0x7F))
            {
                s32Ret = HI_FAILURE;
                break;
            }
            as32ControlData[0] = B8TOI (*pu8CodePtr);
            break;

        case C0_ESC:
            pu8CodePtr ++;
            if (ESC_LS2 == *pu8CodePtr)
            {
                enControlId = ARIB_ESC_LS2;
                break;
            }
            else if (ESC_LS3 == *pu8CodePtr)
            {
                enControlId = ARIB_ESC_LS3;
                break;
            }
            else if (ESC_LS1R == *pu8CodePtr)
            {
                enControlId = ARIB_ESC_LS1R;
                break;
            }
            else if (ESC_LS2R == *pu8CodePtr)
            {
                enControlId = ARIB_ESC_LS2R;
                break;
            }
            else if (ESC_LS3R == *pu8CodePtr)
            {
                enControlId = ARIB_ESC_LS3R;
                break;
            }

            pu8FinalBytePtr = pu8CodePtr;
            while (!IS_DES_FINAL_BYTE (*pu8FinalBytePtr))
            {
                if (pu8FinalBytePtr > pu8Code + u32CodeSize - 2)
                {
                    s32Ret = HI_FAILURE;
                    break;
                }
                pu8FinalBytePtr ++;
            }
            if (s32Ret != HI_SUCCESS)
            {
                break;
            }

            if (0x24 == (*pu8CodePtr))
            {
                u8Class1 = GSET_CLASS_2BYTE;
                pu8CodePtr ++;
            }
            else
            {
                u8Class1 = GSET_CLASS_1BYTE;
            }

            if (0x20 == (*(pu8FinalBytePtr-1)))
            {
                u8Class2 = GSET_CLASS_DRCS;
            }
            else
            {
                u8Class2 = GSET_CLASS_GSET;
            }

            if (CODE_ELEMENT_G0 == (*pu8CodePtr))
            {
                enCodeElement = ARIB_CELEMENT_G0;
            }
            else if (CODE_ELEMENT_G1 == (*pu8CodePtr))
            {
                enCodeElement = ARIB_CELEMENT_G1;
            }
            else if (CODE_ELEMENT_G2 == (*pu8CodePtr))
            {
                enCodeElement = ARIB_CELEMENT_G2;
            }
            else if (CODE_ELEMENT_G3 == (*pu8CodePtr))
            {
                enCodeElement = ARIB_CELEMENT_G3;
            }
            else if ((u8Class1 == GSET_CLASS_2BYTE) && (u8Class2 == GSET_CLASS_GSET) && (pu8CodePtr == pu8FinalBytePtr))
            {
                enCodeElement = ARIB_CELEMENT_G0;
            }
            else
            {
                s32Ret = HI_FAILURE;
                break;
            }

            enGraphSet = (ARIBCC_GraphSet_E) TO_ARIB_GSET (u8Class1, u8Class2, (*pu8FinalBytePtr));

            enControlId = ARIB_ESC_DES;
            as32ControlData[0] = enCodeElement;
            as32ControlData[1] = enGraphSet;
            pu8CodePtr = pu8FinalBytePtr;

            break;

        case C0_APS:
            enControlId = ARIB_C0_APS;
            if (u32CodeSize < 3)
            {
                s32Ret = HI_FAILURE;
                break;
            }
            for (i = 0; i < GET_CS_NUM_OF_PARAM(enControlId); i++)
            {
                pu8CodePtr ++;
                //if(i>4-1)
                 //      break;
                if ((*pu8CodePtr < 0x40) || (*pu8CodePtr > 0x7F))
                {
                    s32Ret = HI_FAILURE;
                    break;
                }
                as32ControlData[i] = B8TOI (*pu8CodePtr);
            }
            break;

//        case C0_SS3:
//            enControlId = ARIB_C0_SS3;
//            break;

        case C0_RS:
            s32Ret = HI_FAILURE;
            break;

        case C0_US:
            s32Ret = HI_FAILURE;
            break;

        default:
            s32Ret = HI_FAILURE;
            break;
    }
    if (enControlId == ARIB_C0_NUL)
    {
        if (s32Ret == HI_SUCCESS)
        {
            *pu32CodeSize = sizeof(HI_U8);
        }
        return s32Ret;
    }

    u32ControlDataSize = GET_CS_NUM_OF_PARAM(enControlId) * sizeof(HI_S32);
    if (sizeof(enControlId) + u32ControlDataSize > u32ControlSize)
    {
        HI_ERR_CC("u32ControlDataSize : %d, u32ControlSize : %d\n", u32ControlDataSize, u32ControlSize);
        return HI_FAILURE;
    }

    pstControl->enControlId = enControlId;
    if(u32ControlDataSize > CONTROL_MAX_DATA_SIZE)
    {
        u32ControlDataSize = CONTROL_MAX_DATA_SIZE;
    }
    memcpy (pstControl->u8ControlData, as32ControlData, u32ControlDataSize);
    *pu32CodeSize    = (HI_U32) (pu8CodePtr - pu8Code + 1);
    *pu32ControlSize = sizeof(enControlId) + u32ControlDataSize;


    return HI_SUCCESS;
}

/* Ref : Table 7-16 C1 control set */
HI_S32 _ARIBCC_DEC_DecodeC1Control (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                             ARIBCC_Control_S *pstControl, HI_U32 *pu32ControlSize)
{
    HI_U8       *pu8CodePtr = HI_NULL;
    //HI_U32      u32CodeSize = 0;
    HI_U32      u32ControlSize = 0;
    HI_U32      u32ControlDataSize = 0;
    HI_S32      as32ControlData[4];
    ARIBCC_CtrlSet_E    enControlId;
    HI_S32      s32Ret = HI_SUCCESS;

    DEC_TRACE("DecodeC1Control (enCaptionType = %s, "
                   "pu8Code = %p) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8Code);

    if ((pu8Code == HI_NULL) || (pu32CodeSize == HI_NULL) || (pstControl == HI_NULL) || (pu32ControlSize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if ((*pu32CodeSize == 0) || (*pu32CodeSize > DUL_MAX_SIZE) || (*pu32ControlSize == 0))
    {
        HI_ERR_CC("invalid size !\n");
        return HI_FAILURE;
    }

    enControlId   = ARIB_C0_NUL;
    //u32CodeSize    = *pu32CodeSize;
    u32ControlSize = *pu32ControlSize;
    *pu32CodeSize    = 0;
    *pu32ControlSize = 0;

    pu8CodePtr   = pu8Code;
    memset (as32ControlData, 0, sizeof(as32ControlData));

    switch (*pu8CodePtr)
    {
        case C1_BKF:
            enControlId = ARIB_C1_BKF;
            break;
        case C1_RDF:
            enControlId = ARIB_C1_RDF;
            break;
        case C1_GRF:
            enControlId = ARIB_C1_GRF;
            break;
        case C1_YLF:
            enControlId = ARIB_C1_YLF;
            break;
        case C1_BLF:
            enControlId = ARIB_C1_BLF;
            break;
        case C1_MGF:
            enControlId = ARIB_C1_MGF;
            break;
        case C1_CNF:
            enControlId = ARIB_C1_CNF;
            break;
        case C1_WHF:
            enControlId = ARIB_C1_WHF;
            break;
        case C1_SSZ:
            enControlId = ARIB_C1_SSZ;
            break;
        case C1_MSZ:
            enControlId = ARIB_C1_MSZ;
            break;
        case C1_NSZ:
            enControlId = ARIB_C1_NSZ;
            break;
        case C1_SZX:
            enControlId = ARIB_C1_SZX;
            pu8CodePtr ++;// P1
            switch(*pu8CodePtr)
            {
                case 0x41:
                    as32ControlData[0] = SZX_LENGTH_TWICE;
                    break;
                case 0x44:
                    as32ControlData[0] = SZX_SIDE_TWICE;
                    break;
                case 0x45:
                    as32ControlData[0] = SZX_LENGTH_SIDE_TWICE;
                    break;
//                case 0x60:
//                case 0x64:
//                case 0x6B:
                default:
                    HI_ERR_CC("SZX Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                    break;
            }
            break;
        case C1_COL:
            pu8CodePtr ++;// P1
            if(*pu8CodePtr == 0x20)
            {
                enControlId = ARIB_C1_COL_2;
                pu8CodePtr ++;// P2
                if((*pu8CodePtr < 0x40) || (*pu8CodePtr > 0x47))
                {
                    HI_ERR_CC("COL Palette Number Error! P2=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                    break;
                }
                as32ControlData[0] = B8TOI (*pu8CodePtr);
                if((as32ControlData[0] < 0) || (as32ControlData[0] > 7))
                {
                    HI_ERR_CC("COL Palette Number Error! P2=%d\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                }
            }
            else if((*pu8CodePtr & 0xF0) == 0x40)
            {
                enControlId = ARIB_C1_COL_1;
                if ((*pu8CodePtr >= COL_FORE_8) && (*pu8CodePtr <= COL_FORE_15))
                {
                    as32ControlData[0] = *pu8CodePtr;
                }
                else
                {
                    HI_ERR_CC("COL Fore Color Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                }
            }
            else if((*pu8CodePtr & 0xF0) == 0x50)
            {
                enControlId = ARIB_C1_COL_1;
                if ((*pu8CodePtr >= COL_BACK_0) && (*pu8CodePtr <= COL_BACK_15))
                {
                    as32ControlData[0] = *pu8CodePtr;
                }
                else
                {
                    HI_ERR_CC("COL Back Color Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                }
            }
            else if((*pu8CodePtr & 0xF0) == 0x60)
            {
                enControlId = ARIB_C1_COL_1;
                if ((*pu8CodePtr >= COL_FORE_MIDDLE_0) && (*pu8CodePtr <= COL_FORE_MIDDLE_15))
                {
                    as32ControlData[0] = *pu8CodePtr;
                }
                else
                {
                    HI_ERR_CC("COL Fore-Middle Color Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                }
            }
            else if((*pu8CodePtr & 0xF0) == 0x70)
            {
                enControlId = ARIB_C1_COL_1;
                if ((*pu8CodePtr >= COL_BACK_MIDDLE_0) && (*pu8CodePtr <= COL_BACK_MIDDLE_15))
                {
                    as32ControlData[0] = *pu8CodePtr;
                }
                else
                {
                    HI_ERR_CC("COL Back-Middle Color Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                }
            }
            else
            {
                HI_ERR_CC("COL Error! P1=0x%02x\n", *pu8CodePtr);
                s32Ret = HI_FAILURE;
            }
            break;
        case C1_FLC:
            enControlId = ARIB_C1_FLC;
            pu8CodePtr ++;// P1
            //as32ControlData[0] = *pu8CodePtr;
            switch(*pu8CodePtr)
            {
                case 0x40:
                    as32ControlData[0] = FLC_NORMAL;
                    break;
                //case 0x47:
                    //as32ControlData[0] = FLC_REVERSING;
                    //break;
                case 0x4F:
                    as32ControlData[0] = FLC_END;
                    break;
                default:
                    s32Ret = HI_FAILURE;
                    HI_ERR_CC("FLC Error! P1=0x%02x\n", *pu8CodePtr);
                    break;
            }
            break;
        case C1_CDC:
#if 0
            pu8CodePtr ++;// P1
            if(*pu8CodePtr == 0x20)
            {
                enControlId = ARIB_C1_CDC_2;
                pu8CodePtr ++;// P2
                switch(*pu8CodePtr)
                {
                    case 0x40:
                        as32ControlData[0] = CDC_REPLACEMENT_0;
                        break;
                    case 0x41:
                        as32ControlData[0] = CDC_REPLACEMENT_1;
                        break;
                    case 0x42:
                        as32ControlData[0] = CDC_REPLACEMENT_2;
                        break;
                    case 0x43:
                        as32ControlData[0] = CDC_REPLACEMENT_3;
                        break;
                    case 0x44:
                        as32ControlData[0] = CDC_REPLACEMENT_4;
                        break;
                    case 0x45:
                        as32ControlData[0] = CDC_REPLACEMENT_5;
                        break;
                    case 0x46:
                        as32ControlData[0] = CDC_REPLACEMENT_6;
                        break;
                    case 0x47:
                        as32ControlData[0] = CDC_REPLACEMENT_7;
                        break;
                    case 0x48:
                        as32ControlData[0] = CDC_REPLACEMENT_8;
                        break;
                    case 0x49:
                        as32ControlData[0] = CDC_REPLACEMENT_9;
                        break;
                    case 0x4A:
                        as32ControlData[0] = CDC_REPLACEMENT_10;
                        break;
                    default:
                        s32Ret = HI_FAILURE;
                        break;
                }
            }
            else
            {
                enControlId = ARIB_C1_CDC_1;
                switch(*pu8CodePtr)
                {
                    case 0x40:
                        as32ControlData[0] = CDC_START;
                        break;
                    case 0x4F:
                        as32ControlData[0] = CDC_END;
                        break;
                    default:
                        s32Ret = HI_FAILURE;
                        break;
            }
#endif
            HI_ERR_CC("CDC Error!\n");
            s32Ret = HI_FAILURE;
            break;
        case C1_POL:
            enControlId = ARIB_C1_POL;
            pu8CodePtr ++;// P1
//            as32ControlData[0] = *pu8CodePtr;
            switch(*pu8CodePtr)
            {
                case 0x40:
                    as32ControlData[0] = POL_NORMAL;
                    break;
                case 0x41:
                    as32ControlData[0] = POL_REVERSING_1;
                    break;
//                case 0x42:
//                    as32ControlData[0] = POL_REVERSING_2;
//                    break;
                default:
                    HI_ERR_CC("POL Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                    break;
            }
            break;
        case C1_WMM:
#if 0
            enControlId = ARIB_C1_WMM;
            pu8CodePtr ++;// P1
            switch(*pu8CodePtr)
            {
                case 0x40:
                    as32ControlData[0] = WMM_MODE_FORE_BACK;
                    break;
                case 0x44:
                    as32ControlData[0] = WMM_MODE_FORE;
                    break;
                case 0x45:
                    as32ControlData[0] = WMM_MODE_BACK;
                    break;
                default:
                    s32Ret = HI_FAILURE;
                    break;
            }
#endif
            HI_ERR_CC("WMM Error!\n");
            s32Ret = HI_FAILURE;
            break;
        case C1_MACRO:
//            enControlId = ARIB_C1_MACRO;
            HI_ERR_CC("MACRO Error!\n");
            s32Ret = HI_FAILURE;
            break;
        case C1_HLC:
            enControlId = ARIB_C1_HLC;
            pu8CodePtr ++;// P1
//            as32ControlData[0] = *pu8CodePtr;
            if ((*pu8CodePtr >= HLC_PATTERN_0) && (*pu8CodePtr <= HLC_PATTERN_15))
            {
                as32ControlData[0] = *pu8CodePtr;
            }
            else
            {
                HI_ERR_CC("HLC Error! P1=0x%02x\n", *pu8CodePtr);
                s32Ret = HI_FAILURE;
            }
            break;
        case C1_RPC:
            enControlId = ARIB_C1_RPC;
            pu8CodePtr ++;// P1
            if((*pu8CodePtr < 0x40) || (*pu8CodePtr > 0x7f))
            {
                HI_ERR_CC("RPC Repeat Number Error!(1) P1=0x%02x\n", *pu8CodePtr);
                s32Ret = HI_FAILURE;
                break;
            }
            as32ControlData[0] = B8TOI (*pu8CodePtr);
            if((as32ControlData[0] < 0) || (as32ControlData[0] > 63))
            {
                HI_ERR_CC("RPC Repeat Number Error!(2) P1=%d\n", as32ControlData[0]);
                s32Ret = HI_FAILURE;
            }
            break;
        case C1_SPL:
            enControlId = ARIB_C1_SPL;
            break;
        case C1_STL:
            enControlId = ARIB_C1_STL;
            break;
        case C1_TIME:
            pu8CodePtr ++;// P1(P)
            switch(*pu8CodePtr)
            {
                case 0x20:
                    enControlId = ARIB_C1_TIME_1;
                    pu8CodePtr ++;// P2
                    if((*pu8CodePtr < 0x40) || (*pu8CodePtr > 0x7f))
                    {
                        HI_ERR_CC("TIME Wait Time Error!(1) P2=0x%02x\n", *pu8CodePtr);
                        s32Ret = HI_FAILURE;
                        break;
                    }
                    as32ControlData[0] = B8TOI (*pu8CodePtr);
                    if((as32ControlData[0] < 0) || (as32ControlData[0] > 63))
                    {
                        HI_ERR_CC("TIME Wait Time Error!(2) P2=%d\n", as32ControlData[0]);
                        s32Ret = HI_FAILURE;
                    }
                    break;
#if 0
                case 0x28:
                    enControlId = ARIB_C1_TIME_2;
                    pu8CodePtr ++;// P2
                    if(*pu8CodePtr == 0x40)
                    {
                        as32ControlData[0] = TIME_CONTROL_MODE_FREE;
                    }
                    else if(*pu8CodePtr == 0x41)
                    {
                        as32ControlData[0] = TIME_CONTROL_MODE_REAL;
                    }
                    else if(*pu8CodePtr == 0x42)
                    {
                        as32ControlData[0] = TIME_CONTROL_MODE_OFFSET;
                    }
                    else if(*pu8CodePtr == 0x43)
                    {
                        as32ControlData[0] = TIME_CONTROL_MODE_ORIGINAL;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                case 0x29:
                    for(i=j=0,TempBuff=0 ;i < u32CodeSize ;i++ )
                    {
                        pu8CodePtr++;
                        if((*pu8CodePtr & 0xF0) == 0x40)
                        {
                            switch (*pu8CodePtr)
                            {
                                case 0x40:
                                    enControlId = ARIB_C1_TIME_3;
                                    break;
                                case 0x41:
                                    enControlId = ARIB_C1_TIME_5;
                                    break;
                                case 0x42:
                                    enControlId = ARIB_C1_TIME_6;
                                    break;
                                case 0x43:
                                    enControlId = ARIB_C1_TIME_7;
                                    break;
                                default:
                                    s32Ret = HI_FAILURE;
                                    break;
                            }
                        }
                        else if((*pu8CodePtr == 0x20) || (*pu8CodePtr == 0x3B))
                        {
                            TempBuff /= 10;
                            as32ControlData[j] = TempBuff;
                            j++;
                        }
                        else if((*pu8CodePtr & 0xF0) == 0x30)
                        {
                            TempBuff += B8TOI(*pu8CodePtr);
                            TempBuff *= 10;
                        }
                        else
                        {
                            s32Ret = HI_FAILURE;
                            break;
                        }
                    }
#endif
                default:
                    HI_ERR_CC("TIME Error! P1=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                    break;
            }
            break;
        default:
            HI_ERR_CC("C1 Control u8Code Error! C1=0x%02x\n", *pu8CodePtr);
            s32Ret = HI_FAILURE;
            break;
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("Function Error!\n");
//        *pu32CodeSize = sizeof(HI_U8);
        *pu32CodeSize    = (HI_U32)(pu8CodePtr - pu8Code + 1);
        return HI_FAILURE;
    }

    u32ControlDataSize = GET_CS_NUM_OF_PARAM(enControlId) * sizeof(HI_S32);
    if (sizeof(enControlId) + u32ControlDataSize > u32ControlSize)
    {
        HI_ERR_CC("Output Data Size Check Error! "
                   "sizeof(enControlId)=%d u32ControlDataSize=%ld u32ControlSize=%ld\n",
                   sizeof(enControlId), u32ControlDataSize, u32ControlSize);
        return HI_FAILURE;
    }

    *pu32CodeSize    = (HI_U32)(pu8CodePtr - pu8Code + 1);
    if ((enControlId == ARIB_C1_TIME_1) && (as32ControlData[0] == 0))
    {
        pstControl->enControlId = ARIB_C0_NUL;
        *pu32ControlSize = 0;
    }
    else
    {
        pstControl->enControlId = enControlId;
        *pu32ControlSize = sizeof(enControlId) + u32ControlDataSize;
        if(u32ControlDataSize > CONTROL_MAX_DATA_SIZE)
        {
            u32ControlDataSize = CONTROL_MAX_DATA_SIZE;
        }
        memcpy (pstControl->u8ControlData, as32ControlData, u32ControlDataSize);
    }

    return HI_SUCCESS;
}

/* Ref : Table 7-17 Extension control code (CSI) */
HI_S32 _ARIBCC_DEC_DecodeCsiControl (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8Code, HI_U32 *pu32CodeSize,
                             ARIBCC_Control_S *pstControl, HI_U32 *pu32ControlSize)
{
    HI_U8       *pu8CodePtr;
//    HI_U8     OrnAdr[4];
    HI_U32      u32CodeSize;
    HI_U32      u32ControlSize;
    HI_U32      u32ControlDataSize = 0;
    HI_U32      i,j;
    HI_S32      as32ControlData[4];
    HI_S32      TempBuff;
    ARIBCC_CtrlSet_E    enControlId = ARIB_C0_NUL;
    HI_S32      s32Ret = HI_SUCCESS;

    DEC_TRACE("DecodeCsiControl (enCaptionType = %s, "
                   "pu8Code = %p) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8Code);

    if ((pu8Code == HI_NULL) || (pu32CodeSize == HI_NULL) || (pstControl == HI_NULL) || (pu32ControlSize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if ((*pu32CodeSize == 0) || (*pu32CodeSize > DUL_MAX_SIZE) || (*pu32ControlSize == 0))
    {
        HI_ERR_CC("invalid size !\n");
        return HI_FAILURE;
    }

    u32CodeSize    = *pu32CodeSize;
    u32ControlSize = *pu32ControlSize;
    *pu32CodeSize    = 0;
    *pu32ControlSize = 0;

    pu8CodePtr   = pu8Code;
    memset (as32ControlData, 0, sizeof(as32ControlData));

    for(i=j=0,TempBuff=0; i < u32CodeSize; i++ )
    {
        pu8CodePtr++;
        if(((*pu8CodePtr & 0xF0) == 0x40) || ((*pu8CodePtr & 0xF0) == 0x50) || ((*pu8CodePtr & 0xF0) == 0x60))
        {
            switch (*pu8CodePtr)
            {
                case CSI_GSM:
                    HI_ERR_CC("GSM Error!\n");
//                    enControlId = ARIB_CSI_GSM;
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_SWF:
                    if((as32ControlData[0] >= SWF_FORMAT_960_540_SIDE) && (as32ControlData[0] <= SWF_FORMAT_720_480_LENGTH))
                    {
                        enControlId = ARIB_CSI_SWF_1;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_CCC:
#if 0
                    if((as32ControlData[0] == CCC_END)      || (as32ControlData[0] == CCC_START_OR) ||
                       (as32ControlData[0] == CCC_START_AND)|| (as32ControlData[0] == CCC_START_XOR) )
                    {
                        enControlId = ARIB_CSI_CCC;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("CCC Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_SDF:
                    enControlId = ARIB_CSI_SDF;
                    break;
                case CSI_SSM:
                    if(as32ControlData[0] == as32ControlData[1])
                    {
                        if((as32ControlData[0] == SSM_SIZE_16) || (as32ControlData[0] == SSM_SIZE_20) ||
                           (as32ControlData[0] == SSM_SIZE_24) || (as32ControlData[0] == SSM_SIZE_30) ||
                           (as32ControlData[0] == SSM_SIZE_36)                                     )
                        {
                            enControlId = ARIB_CSI_SSM;
                        }
                        else
                        {
                            s32Ret = HI_FAILURE;
                        }
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_SHS:
                    enControlId = ARIB_CSI_SHS;
                    break;
                case CSI_SVS:
                    enControlId = ARIB_CSI_SVS;
                    break;
                case CSI_PLD:
//                    enControlId = ARIB_CSI_PLD;
                    HI_ERR_CC("PLD Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_PLU:
                    HI_ERR_CC("PLU Error!\n");
//                    enControlId = ARIB_CSI_PLU;
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_GAA:
#if 0
                    if((as32ControlData[0] == GAA_ALL) || (as32ControlData[0] == GAA_DESIGN_FRAM))
                    {
                        enControlId = ARIB_CSI_GAA;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("GAA Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_SRC:
#if 0
                    if((as32ControlData[0] >= SRC_BOXING) && (as32ControlData[0] <= SRC_SHADOW))
                    {
                        enControlId = ARIB_CSI_SRC;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("SRC Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_SDP:
                    enControlId = ARIB_CSI_SDP;
                    break;
                case CSI_ACPS:
                    enControlId = ARIB_CSI_ACPS;
                    break;
                case CSI_TCC:
#if 0
                    if((as32ControlData[0] >= TCC_GROUP_CUT) && (as32ControlData[0] <= TCC_CHARACTER_SLIDE))
                    {
                        enControlId = ARIB_CSI_TCC;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    if((as32ControlData[1] >= TCC_LEFT_TO_RIGHT) && (as32ControlData[1] <= TCC_DOWN_TO_UP))
                    {
                        enControlId = ARIB_CSI_TCC;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("TCC Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_ORN:
                    if((as32ControlData[0] >= ORN_NOTHING) && (as32ControlData[0] <= ORN_HEMMING))
                    {
                        if((as32ControlData[1] >= ORN_INDEX_MIN) && (as32ControlData[1] <= ORN_INDEX_MAX))
                        {
                            enControlId = ARIB_CSI_ORN;
                        }
                        else
                        {
                            s32Ret = HI_FAILURE;
                        }
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_MDF:
#if 0
                    if((as32ControlData[0] >= MDF_NORMAL) && (as32ControlData[0] <= MDF_BOLD_OBLIQUE))
                    {
                        enControlId = ARIB_CSI_MDF;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("MDF Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_CFS:
                    HI_ERR_CC("CFS Error!\n");
//                    enControlId = ARIB_CSI_CFS;
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_XCS:
#if 0
                    if((as32ControlData[0] == XCS_START) || (as32ControlData[0] == XCS_END))
                    {
                        enControlId = ARIB_CSI_XCS;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("XCS Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_SCR:
                    if((as32ControlData[0] >= SCR_MODE_FIXED) && (as32ControlData[0] <= SCR_MODE_SCREEN_ROLLOUT))
                    {
                        enControlId = ARIB_CSI_SCR;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_PRA:
                    if((as32ControlData[0] >= PRA_NEWS_FLUSH_CHIME_1) && (as32ControlData[0] <= PRA_ALERT))
                    {
                        enControlId = ARIB_CSI_PRA;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_ACS:
#if 0
                    if((as32ControlData[0] >= ACS_START) && (as32ControlData[0] <= ACS_END_DEFINITION_2))
                    {
                        enControlId = ARIB_CSI_ACS;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
#endif
                    HI_ERR_CC("ACS Error!\n");
                    s32Ret = HI_FAILURE;
                    break;
                case CSI_RCS:
                    if((as32ControlData[0] >= RCS_INDEX_MIN) && (as32ControlData[0] <= RCS_INDEX_MAX))
                    {
                        enControlId = ARIB_CSI_RCS;
                    }
                    else
                    {
                        s32Ret = HI_FAILURE;
                    }
                    break;
                case CSI_SCS:
                    HI_ERR_CC("SCS Error!\n");
//                    enControlId = ARIB_CSI_SCS;
                    s32Ret = HI_FAILURE;
                    break;
                default:
                    HI_ERR_CC("CSI End u8Code Error! F=0x%02x\n", *pu8CodePtr);
                    s32Ret = HI_FAILURE;
                    break;
            }
            break;
        }
        else if((*pu8CodePtr == 0x20) || (*pu8CodePtr == 0x3B))
        {
            TempBuff /= 10;
            if(j>4-1)
            {
                HI_ERR_CC("buffer overflow\n");
                return HI_FAILURE;
            }
            as32ControlData[j] = TempBuff;
            j++;
            TempBuff = 0;
        }
        else if((*pu8CodePtr >= 0x30) && (*pu8CodePtr <= 0x39))
        {
            TempBuff += B8TOI(*pu8CodePtr);
            TempBuff *= 10;
            //HI_ERR_CC("P=0x%02x TempBuff=%d\n", *pu8CodePtr, TempBuff);
        }
        else
        {
            //HI_ERR_CC("CSI Parameter u8Code Error! Param=0x%02x\n", *pu8CodePtr);
            s32Ret = HI_FAILURE;
            break;
        }
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("Function Error!\n");
//        *pu32CodeSize = sizeof(HI_U8);
        *pu32CodeSize    = (HI_U32)(pu8CodePtr - pu8Code + 1);
        return HI_FAILURE;
    }

    u32ControlDataSize = GET_CS_NUM_OF_PARAM(enControlId) * sizeof(HI_S32);
    if (sizeof(enControlId) + u32ControlDataSize > u32ControlSize)
    {
        HI_ERR_CC("Output Data Size Check Error! "
                   "sizeof(enControlId)=%d u32ControlDataSize=%ld u32ControlSize=%ld\n",
                       sizeof(enControlId), u32ControlDataSize, u32ControlSize);
        return HI_FAILURE;
    }

    pstControl->enControlId = enControlId;
    if(u32ControlDataSize > CONTROL_MAX_DATA_SIZE)
    {
        u32ControlDataSize = CONTROL_MAX_DATA_SIZE;
    }
    memcpy (pstControl->u8ControlData, as32ControlData, u32ControlDataSize);
    *pu32CodeSize    = (HI_U32)(pu8CodePtr - pu8Code + 1);
    *pu32ControlSize = sizeof(enControlId) + u32ControlDataSize;

    return HI_SUCCESS;
}


HI_S32 _ARIBCC_DEC_DecodeDrcsData (ARIBCC_CaptionType_E enCaptionType, ARIBCC_DrcsType_E enDrcsType, HI_U8 *pu8InDrcsData,
                                   HI_U32 u32InDrcsDataSize, HI_U8 *pu8OutDrcsData, HI_U32 *pu32OutDrcsDataSize)
{
    HI_U8   u8NumberOfCode = 0, u8NumberOfFont = 0;
    HI_U8   Mode, Depth, Width, Height, i, j;
    HI_U8   *pu8DrcsData = HI_NULL, *pu8DrcsCode = HI_NULL, *pu8CharacterCode = HI_NULL;
    HI_U8   *pu8DrcsFont = HI_NULL, *pu8PatternData = HI_NULL;
    HI_U32  u32PatternSize = 0;
    HI_U32  u32BitCount = 0;
    HI_S32  s32Ret = HI_SUCCESS;

    HI_ERR_CC("DecodeCsiControl (enCaptionType = %s, "
                   "enDrcsType = %d, pu8InDrcsData = %p, u32InDrcsDataSize = %u) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                   enDrcsType, pu8InDrcsData, u32InDrcsDataSize);

    if ((pu8InDrcsData == HI_NULL) || (pu8OutDrcsData == HI_NULL) || (pu32OutDrcsDataSize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if ((u32InDrcsDataSize < (sizeof(HI_U8) * 8)) || (u32InDrcsDataSize > DUL_MAX_SIZE))
    {
        HI_ERR_CC("invalid size !\n");
        return HI_FAILURE;
    }

    *pu32OutDrcsDataSize = 0;

    pu8DrcsData     = pu8InDrcsData;
    u8NumberOfCode = (HI_U8) GET_BIT (pu8DrcsData, 0, 8);
    pu8DrcsCode     = pu8DrcsData + sizeof(HI_U8);

    if (u8NumberOfCode == 0)
    {
        HI_ERR_CC(" u8NumberOfCode = %d\n", u8NumberOfCode);
        return HI_FAILURE;
    }

    for (i = 0; i < u8NumberOfCode; i++)
    {
        pu8CharacterCode = pu8DrcsCode;
        u8NumberOfFont  = (HI_U8) GET_BIT (pu8DrcsCode, 16, 8);
        pu8DrcsFont      = pu8DrcsCode + sizeof(HI_U16) + sizeof(HI_U8);

        if (enDrcsType == ARIB_DRCS_1BYTE)
        {
            if (!IS_GSET_1B_DRCS (pu8CharacterCode[0]))
            {
                HI_ERR_CC("drcs data is error, %#x is not 1B\n",pu8CharacterCode[0]);
                s32Ret = HI_FAILURE;
                break;
            }
            else if (!IS_AREA_GL (pu8CharacterCode[1]))
            {
                HI_ERR_CC("drcs data is error, %#x is not GL\n",pu8CharacterCode[1]);
                s32Ret = HI_FAILURE;
                break;
            }
        }
        else
        {
            if (!IS_CS_2B_DRCS_CELL (pu8CharacterCode[0]))
            {
                HI_ERR_CC("drcs data is error, %#x is not 2B_DRCS_CELL\n",pu8CharacterCode[0]);
                s32Ret = HI_FAILURE;
                break;
            }
            else if (!IS_CS_2B_DRCS_ROW (pu8CharacterCode[1]))
            {
                HI_ERR_CC("drcs data is error, %#x is not 2B_DRCS_ROW\n",pu8CharacterCode[1]);
                s32Ret = HI_FAILURE;
                break;
            }
        }

        if (u8NumberOfFont == 0)
        {
            HI_ERR_CC("drcs data is error, number of font is %d\n", u8NumberOfFont);
            s32Ret = HI_FAILURE;
            break;
        }

        for (j = 0; j < u8NumberOfFont; j++)
        {
            //u8FontID      = GET_BIT (pu8DrcsFont,  0, 4);
            Mode        = GET_BIT (pu8DrcsFont,  4, 4);
            Depth       = (HI_U8) GET_BIT (pu8DrcsFont,  8, 8);
            Width       = (HI_U8) GET_BIT (pu8DrcsFont, 16, 8);
            Height      = (HI_U8) GET_BIT (pu8DrcsFont, 24, 8);
            pu8PatternData = pu8DrcsFont + (sizeof(HI_U8) * 4);

            if (! ((Mode&0x0F) == TM_MULTI_GRADATION))
            {
                HI_ERR_CC("drcs data is error, %#x is not MULTI_GRADATION\n",Mode);
                s32Ret = HI_FAILURE;
                break;
            }
            else if (GET_DG_GRADATION (Depth) != 4)
            {
                HI_ERR_CC("drcs data is error, Depth is %d\n", Depth);
                s32Ret = HI_FAILURE;
                break;
            }
            else if ((!IS_CS_STANDARD_SIZE (Width)) && (!IS_CS_HALF_SIZE (Width)))
            {
                HI_ERR_CC("drcs data is error, font size is invalid\n");
                s32Ret = HI_FAILURE;
                break;
            }
            else if ((!IS_CS_STANDARD_SIZE (Height)) && (!IS_CS_HALF_SIZE (Height)))
            {
                HI_ERR_CC("drcs data is error, font size is invalid\n");
                s32Ret = HI_FAILURE;
                break;
            }

            u32BitCount = 1;
            while ((1 << u32BitCount) < (Depth + 2))
            {
                u32BitCount ++;
            }
            u32PatternSize = ((u32BitCount * Width * Height) + 7) / 8;

            pu8DrcsFont    = pu8PatternData + u32PatternSize;
        }
        if (s32Ret != HI_SUCCESS)
        {
            break;
        }

        pu8DrcsCode = pu8DrcsFont;
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to decode drcs data\n");
        return HI_FAILURE;
    }

    memcpy (pu8OutDrcsData, pu8InDrcsData, u32InDrcsDataSize);
    *pu32OutDrcsDataSize = u32InDrcsDataSize;


    return HI_SUCCESS;
}


HI_S32 _ARIBCC_DEC_DecodeBitmapData (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8InBitmapData, HI_U32 u32InBitmapDataSize,
                              HI_U8 *pu8OutBitmapData, HI_U32 *pu32OutBitmapDataSize)
{
    //HI_S16  s16XPosition = 0, s16YPosition = 0;
    HI_U8   u8NumOfFlcColors = 0, u8BitDepth = 0, u8ColorType = 0;
    HI_U8   u8Compression = 0, u8Filtering = 0, u8Interlace = 0;
    HI_U8   u8CMF, u8FLG, u8CINFO, u8CM, u8FLEVEL, u8FDICT, u8FCHECK;
    HI_U8   *pu8ColorIndex = HI_NULL, *pu8BitmapData = HI_NULL;
    //HI_U8   *pu8PosHeader = HI_NULL;
    HI_U8   *pu8FlcHeader = HI_NULL, *pu8PngData = HI_NULL, *pu8Chunk = HI_NULL, *pu8ChunkType = HI_NULL, *pu8ChunkData = HI_NULL;
    HI_U32  u32PngLength = 0, u32ChunkLength = 0;
    //HI_U32  u32Width = 0, u32Height = 0;
    HI_U32  *pu32ChunkCRC __attribute__((unused)) = HI_NULL;
    HI_BOOL bIHDR;
    //HI_BOOL bIEND;
    HI_S32  s32Ret = HI_SUCCESS;

    HI_ERR_CC("DecodeCsiControl (enCaptionType = %s, "
                   "pu8InBitmapData = %p, u32InBitmapDataSize = %u) is Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    pu8InBitmapData, u32InBitmapDataSize);

    if ((pu8InBitmapData == HI_NULL) || (pu8OutBitmapData == HI_NULL) || (pu32OutBitmapDataSize == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }
    else if ((u32InBitmapDataSize < ((sizeof(HI_S16) * 2) + sizeof(HI_U8))) || (u32InBitmapDataSize > DUL_MAX_SIZE))
    {
        HI_ERR_CC("invalid size !\n");
        return HI_FAILURE;
    }

//    memset (pu8OutBitmapData, 0, sizeof(AribCC_BitmapData_t));
    *pu32OutBitmapDataSize = 0;

    pu8BitmapData = pu8InBitmapData;
    //pu8PosHeader  = pu8BitmapData;
    pu8FlcHeader  = pu8BitmapData + (sizeof(HI_S16) * 2);

    //s16XPosition      = (HI_S16) GET_BIT (pu8PosHeader, 0, 16);
    //s16YPosition      = (HI_S16) GET_BIT (pu8PosHeader, 16, 16);
    u8NumOfFlcColors = (HI_U8) GET_BIT (pu8FlcHeader, 0, 8);
    pu8ColorIndex     = pu8FlcHeader + sizeof(HI_U8);

    if (u8NumOfFlcColors > BMP_FLC_COLOR_MAX)
    {
        HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(4) Error!\n");
        HI_ERR_CC("  %p :     u8NumOfFlcColors = %u\n",
                       pu8FlcHeader - pu8InBitmapData, u8NumOfFlcColors);
        return HI_FAILURE;
    }

    pu8PngData   = pu8ColorIndex + u8NumOfFlcColors;
    u32PngLength = u32InBitmapDataSize - ((sizeof(HI_S16) * 2) + sizeof(HI_U8) + u8NumOfFlcColors);

    if (memcmp (pu8PngData, PNG_SIG_CODE, PNG_SIG_SIZE) != 0)
    {
        HI_ERR_CC("%p :     Signature      = "
                  "0x%2X %2X %2X %2X %2X %2X %2X %2X\n",
                  pu8PngData - pu8InBitmapData, *pu8PngData, *(pu8PngData + 1), *(pu8PngData + 2), *(pu8PngData + 3),
                  *(pu8PngData + 4), *(pu8PngData + 5), *(pu8PngData + 6), *(pu8PngData + 7));
        return HI_FAILURE;
    }

    s32Ret     = HI_FAILURE;
    bIHDR      = HI_FALSE;
    //bIEND      = HI_FALSE;
    //u32Width      = 0;
    //u32Height     = 0;
    //u8BitDepth = 0;
    for (pu8Chunk = pu8PngData + PNG_SIG_SIZE; pu8Chunk < pu8PngData + u32PngLength; pu8Chunk += (u32ChunkLength + (sizeof(HI_U32) * 3)))
    {
        u32ChunkLength = (HI_U32) GET_BIT (pu8Chunk, 0, 32);

        pu8ChunkType = pu8Chunk + sizeof(HI_U32);
        pu8ChunkData = pu8ChunkType + sizeof(HI_U32);
        pu32ChunkCRC  = (HI_U32*) (pu8ChunkData + u32ChunkLength);

        if (memcmp (pu8ChunkType, CHUNK_TYPE_IHDR, strlen(CHUNK_TYPE_IHDR)) == 0)
        {
            if (pu8Chunk != pu8PngData + PNG_SIG_SIZE)
            {
                HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(5) Error!\n");
                break;
            }

            bIHDR = HI_TRUE;

            //u32Width       = (HI_U32) GET_BIT (pu8ChunkData,  0, 32);
            //u32Height      = (HI_U32) GET_BIT (pu8ChunkData, 32, 32);
            u8BitDepth    = (HI_U8) GET_BIT (pu8ChunkData, 64,  8);
            u8ColorType   = (HI_U8) GET_BIT (pu8ChunkData, 72,  8);
            u8Compression = (HI_U8) GET_BIT (pu8ChunkData, 80,  8);
            u8Filtering   = (HI_U8) GET_BIT (pu8ChunkData, 88,  8);
            u8Interlace   = (HI_U8) GET_BIT (pu8ChunkData, 96,  8);

            if ((u8BitDepth != 1) && (u8BitDepth != 2) && (u8BitDepth != 4) && (u8BitDepth != 8))
            {
                HI_ERR_CC("%p :         u8BitDepth   = %u\n",
                               pu8ChunkData + (sizeof(HI_U32) * 2) - pu8InBitmapData, u8BitDepth);
                break;
            }
            else if (u8ColorType != IHDR_PALETTE)
            {
                HI_ERR_CC("%p :         u8ColorType  = %u\n",
                               pu8ChunkData + (sizeof(HI_U32) * 2) + sizeof(HI_U8) - pu8InBitmapData, u8ColorType);
                break;
            }
            else if (u8Compression != IHDR_DEFLATE_INFLATE)
            {
                HI_ERR_CC("%p :         u8Compression = %u\n",
                               pu8ChunkData + (sizeof(HI_U32) * 2) + (sizeof(HI_U8) * 2) - pu8InBitmapData, u8Compression);
                break;
            }
            else if (u8Filtering != IHDR_NON_FILTER)
            {
                HI_ERR_CC("%p :         u8Filtering   = %u\n",
                               pu8ChunkData + (sizeof(HI_U32) * 2) + (sizeof(HI_U8) * 3) - pu8InBitmapData, u8Filtering);
                break;
            }
            else if (u8Interlace != IHDR_NON_INTERLACE)
            {
                HI_ERR_CC("%p :         u8Interlace   = %u\n",
                               pu8ChunkData + (sizeof(HI_U32) * 2) + (sizeof(HI_U8) * 4) - pu8InBitmapData, u8Interlace);
                break;
            }
            continue;
        }

        if (!bIHDR)
        {
            HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(11) Error!\n");
            break;
        }

        //if (bIEND)
        //{
        //    HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(12) Error!\n");
        //    s32Ret = HI_FAILURE;
        //    break;
       // }

        if (memcmp (pu8ChunkType, CHUNK_TYPE_IEND, strlen(CHUNK_TYPE_IEND)) == 0)
        {
            HI_ERR_CC("%p :       CRC          = 0x%8x\n",
                           ((HI_U8*) pu32ChunkCRC) - pu8InBitmapData, ((unsigned int) (*pu32ChunkCRC)));

            if (pu8Chunk + (u32ChunkLength + (sizeof(HI_U32) * 3)) != pu8PngData + u32PngLength)
            {
                HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(13) Error!\n");
            }

            s32Ret = HI_SUCCESS;
            //bIEND = HI_TRUE;

            break;
        }

        if (memcmp (pu8ChunkType, CHUNK_TYPE_IDAT, sizeof(HI_U8)) == 0)
        {
            u8CINFO  = GET_BIT (pu8ChunkData,  0, 4);
            u8CM     = GET_BIT (pu8ChunkData,  4, 4);
            u8FLEVEL = GET_BIT (pu8ChunkData,  8, 2);
            u8FDICT  = GET_BIT (pu8ChunkData, 10, 1);
            u8FCHECK = GET_BIT (pu8ChunkData, 11, 5);

            u8CMF = ((u8CINFO << 4) | u8CM);
            u8FLG = ((u8FLEVEL << 6) | (u8FDICT << 5) | u8FCHECK);

            if ((u8CINFO != 0x7) || (u8CM != 0x8) || ((((u8CMF * 256) + u8FLG) % 31) != 0))
            {
                HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(14) Error!\n");
                HI_ERR_CC("  %p :         u8CMF        = 0x%2X\n",
                               pu8ChunkData - pu8InBitmapData, u8CMF);
                HI_ERR_CC("                         u8CINFO    = 0x%1.1X\n", u8CINFO);
                HI_ERR_CC("                         u8CM       = 0x%1.1X\n", u8CM);
                HI_ERR_CC("  %p :         u8FLG        = 0x%2X\n",
                               pu8ChunkData + sizeof(HI_U8) - pu8InBitmapData, u8FLG);
                HI_ERR_CC("                         u8FLEVEL   = 0x%1.1X\n", u8FLEVEL);
                HI_ERR_CC("                         u8FDICT    = 0x%1.1X\n", u8FDICT);
                HI_ERR_CC("                         u8FCHECK   = 0x%1.1X\n", u8FCHECK);
                break;
            }
        }
        else
        {
            HI_ERR_CC("  %p :       CRC          = 0x%8x\n",
                           ((HI_U8*) pu32ChunkCRC) - pu8InBitmapData, ((unsigned int) (*pu32ChunkCRC)));
        }
    }
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("\"_ARIBCC_DEC_DecodeBitmapData\" Check(16) Error!\n");
        return HI_FAILURE;
    }

    memcpy (pu8OutBitmapData, pu8InBitmapData, u32InBitmapDataSize);
    *pu32OutBitmapDataSize = u32InBitmapDataSize;


    return HI_SUCCESS;
}

HI_S32 _ARIBCC_DEC_GetPresentTimeFromPTS (ARIBCC_CaptionType_E enCaptionType, HI_U64 u64PtsFromPES90k, struct timeval *pstTimeValue)
{
    static HI_S64 s64LastPts = 0;
    HI_S64 s64CurPts = 0;
    HI_U64 u64DiffTime90k = 0, u64CurPts90k = 0;
    HI_U32 U32DiffTime1k = 0;
    struct timeval      TimeValue;
    struct timezone     TimeZone;
    HI_S32 s32Ret = HI_SUCCESS;

    if ((pstTimeValue == HI_NULL)
        || (u64PtsFromPES90k > (HI_U64)0x1FFFFFFFFLL))
    {
        HI_ERR_CC("parameter error\n");
        return HI_FAILURE;
    }

    pstTimeValue->tv_sec = 0;
    pstTimeValue->tv_usec= 0;

    s32Ret = CCDISP_GetCurPts(&s64CurPts);
    if(s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    #define PLAYER_TIME_JUMP_JUDGE  (5 * 1000)
    /* CurPts less than LastPts, need to Clear Timer */
    if ((-1 != s64CurPts) && ((s64CurPts + PLAYER_TIME_JUMP_JUDGE) < s64LastPts))
    {
        //(HI_VOID)ARIBCC_Com_ClearTimer(enCaptionType);
        //(HI_VOID)ARIBCC_Com_ClearPESPtr (enCaptionType);
    }
    s64LastPts = s64CurPts;

    /*pts from avplay is in unit of 1K*/
    u64CurPts90k = s64CurPts* 90;

    if (u64PtsFromPES90k >= u64CurPts90k)
    {
        u64DiffTime90k = u64PtsFromPES90k - u64CurPts90k;
    }
    else
    {
        u64DiffTime90k = 0;
    }

    U32DiffTime1k = 0;
    if (u64DiffTime90k)
    {
        U32DiffTime1k = (HI_U32) (u64DiffTime90k / 90);
    }

    s32Ret = gettimeofday (&TimeValue, &TimeZone);
    if (s32Ret == -1)
    {
        HI_ERR_CC("failed to gettimeofday\n");
        return HI_FAILURE;
    }

    if ((U32DiffTime1k < 100) || (U32DiffTime1k > (1000 * ARIBCC_MANAGEMENT_TIMEOUT)))
    {
        HI_WARN_CC("U32DiffTime1k = %d \n", U32DiffTime1k);
        pstTimeValue->tv_sec = TimeValue.tv_sec;
        pstTimeValue->tv_usec = TimeValue.tv_usec;
        return HI_SUCCESS;
    }

    TimeValue.tv_usec += ((U32DiffTime1k % 1000) * 1000);
    TimeValue.tv_sec  += (U32DiffTime1k / 1000);

    if (TimeValue.tv_usec / 1000000)
    {
        TimeValue.tv_sec  += (TimeValue.tv_usec / 1000000);
        TimeValue.tv_usec %= 1000000;
    }

    pstTimeValue->tv_sec  = TimeValue.tv_sec;
    pstTimeValue->tv_usec = TimeValue.tv_usec;

    return HI_SUCCESS;
}

HI_S32 _ARIBCC_DEC_CheckCCPESHeader (HI_U8 *pu8CcPesPtr, HI_U64 *pu64Pts, HI_U8 **ppu8PesDataByte)
{
    HI_U32  u32PacketStartCodePrefix = 0;
    HI_U8   u8StreamID = 0, u8PtsDtsFlags = 0, u8PesHeaderDataLength = 0;
    HI_U8   u8DataIdentifier = 0, u8PrivateStreamID = 0;
    HI_U8   u8PesDataPacketHeaderLength = 0;
    HI_U64  u64Pts = 0;
    HI_U8   *pu8OptionalPesHeader = HI_NULL, *pu8PtsField = HI_NULL;
    HI_U8   *pu8PesPacketDataByte = HI_NULL, *pu8PesDataPrivateDataByte = HI_NULL;
    HI_U16  U16PesPacketLength = 0;

    if ((pu8CcPesPtr == HI_NULL) || (pu64Pts == HI_NULL) || (ppu8PesDataByte == HI_NULL))
    {
        HI_ERR_CC("parameter error !\n");
        return HI_FAILURE;
    }

    *pu64Pts         = 0;
    *ppu8PesDataByte = HI_NULL;

    /* check pes start code prefix */
    u32PacketStartCodePrefix = (HI_U32) GET_BIT (pu8CcPesPtr,  0, 24);
    u8StreamID = (HI_U8) GET_BIT (pu8CcPesPtr, 24,  8);
    U16PesPacketLength = (HI_U16) GET_BIT (pu8CcPesPtr, 32, 16);

    if ((u32PacketStartCodePrefix != ARIBCC_PES_SC_PREFIX)
         || (U16PesPacketLength == 0)
         || (U16PesPacketLength > (CCPES_MAX_SIZE - ARIBCC_PES_HEADER_LENGTH)))
    {
        HI_ERR_CC("pes data error!\n");
        return HI_FAILURE;
    }

    if (ARIBCC_PES_PRIVATE_STREAM_1 == u8StreamID)
    {
        /* PES packets for caption */
        pu8OptionalPesHeader = pu8CcPesPtr + ARIBCC_PES_HEADER_LENGTH;

        u8PtsDtsFlags            = GET_BIT (pu8OptionalPesHeader,  8, 2);
        u8PesHeaderDataLength    = (HI_U8) GET_BIT (pu8OptionalPesHeader, 16, 8);

        pu8PesPacketDataByte = pu8OptionalPesHeader + ARIBCC_PES_OPT_HEADER_LENGTH + u8PesHeaderDataLength;

        /* check pts */
        if (u8PtsDtsFlags & ARIBCC_PES_PDF_PTS)
        {
            pu8PtsField = pu8OptionalPesHeader + ARIBCC_PES_OPT_HEADER_LENGTH;

            u64Pts = GET_BIT (pu8PtsField, 4, 3);
            u64Pts = (u64Pts << 15) + GET_BIT (pu8PtsField,  8, 15);
            u64Pts = (u64Pts << 15) + GET_BIT (pu8PtsField, 24, 15);
        }
        else
        {
            HI_WARN_CC("have no pts!\n");
        }
    }
    else if (ARIBCC_PES_PRIVATE_STREAM_2 == u8StreamID)
    {
        /* PES packets for superimpose */
        pu8PesPacketDataByte = pu8CcPesPtr + ARIBCC_PES_HEADER_LENGTH;
    }
    else
    {
        HI_ERR_CC("pes data have no cc \n");
        return HI_FAILURE;
    }

    u8DataIdentifier            = (HI_U8) GET_BIT (pu8PesPacketDataByte,  0, 8);
    u8PrivateStreamID           = (HI_U8) GET_BIT (pu8PesPacketDataByte,  8, 8);
    u8PesDataPacketHeaderLength = GET_BIT (pu8PesPacketDataByte, 20, 4);

    if ((ARIBCC_PES_PRIVATE_STREAM_1 == u8StreamID) && (CCPES_DATA_IDENT_SYNC != (u8DataIdentifier)))
    {
        HI_ERR_CC("  Data Identifier   : 0x%2X\n", u8DataIdentifier);
        return HI_FAILURE;
    }
    else if ((ARIBCC_PES_PRIVATE_STREAM_2 == u8StreamID) && (CCPES_DATA_IDENT_ASYNC != (u8DataIdentifier)))
    {
        HI_ERR_CC("  Data Identifier   : 0x%2X\n", u8DataIdentifier);
        return HI_FAILURE;
    }
    if (u8PrivateStreamID != CCPES_PRIVATE_STREAM_ID)
    {
        HI_ERR_CC("  Private Stream ID : 0x%2X\n", u8PrivateStreamID);
        return HI_FAILURE;
    }

    *pu64Pts                  = u64Pts;
    pu8PesDataPrivateDataByte = pu8PesPacketDataByte + ARIBCC_PES_PD_HEADER_LENGTH;
    *ppu8PesDataByte          = pu8PesDataPrivateDataByte + u8PesDataPacketHeaderLength;

    return HI_SUCCESS;
}
