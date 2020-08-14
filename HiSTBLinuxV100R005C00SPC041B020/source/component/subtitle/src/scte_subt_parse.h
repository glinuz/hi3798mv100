#ifndef __SCTE_SUBT_PARSE_H__
#define __SCTE_SUBT_PARSE_H__

#include "hi_type.h"
#include "hi_unf_subt.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tagSCTE_SUBT_OUTLINE_STYLE_E
{
    SCTE_SUBT_OUTLINE_DROPSHADOW = 0,
    SCTE_SUBT_OUTLINE_OUTLINED,
    SCTE_SUBT_OUTLINE_RESERVED
} SCTE_SUBT_OUTLINE_STYLE_E;

typedef enum tagSCTE_SUBT_BACKGROUND_STYLE_E
{
    SCTE_SUBT_BACKGROUD_FRAMED,
    SCTE_SUBT_BACKGROUD_TRANSPARENT
} SCTE_SUBT_BACKGROUND_STYLE_E;

typedef struct tagSCTE_SUBT_BACKGROUND_FRAMED_S
{
    HI_U32 u32FrameColor;
    HI_U32 u32TopXPos;
    HI_U32 u32TopYPos;
    HI_U32 u32BottomXPos;
    HI_U32 u32BottomYPos;
} SCTE_SUBT_BACKGROUND_FRAMED_S;

typedef struct tagSCTE_SUBT_OUTLINE_DROPSHADOW_S
{
    HI_U32 u32ShadowColor;
    HI_U16 u32ShadowRight;
    HI_U16 u32ShadowBottom;
} SCTE_SUBT_OUTLINE_DROPSHADOW_S;

typedef struct tagSCTE_SUBT_OUTLINE_OUTLINED_S
{
    HI_U32 u32OutlineColor;
    HI_U16 u16OutlineThickness;
} SCTE_SUBT_OUTLINE_OUTLINED_S;

typedef enum tagSCTE_DISPLAY_STANDARD_E
{
    STANDARD_720_480_30 = 0x0,
    STANDARD_720_576_25,
    STANDARD_1280_720_60,
    STANDARD_1920_1080_60
}SCTE_DISPLAY_STANDARD_E;

/** Defines the type of display mode */ /** CNcomment: 定义SCTE subtitle显示的方式 */
typedef enum tagSCTE_SUBT_DISP_E
{
    SCTE_SUBT_DISP_NORMAL = 0, /**<Normal Display *//**<CNcomment:正常显示 */
    SCTE_SUBT_DISP_PRECLEAR /**<Erase Display *//**<CNcomment:擦除显示 */
} SCTE_SUBT_DISP_E;

typedef struct tagSCTE_SUBT_OUTPUT_S
{
    SCTE_SUBT_DISP_E      enDISP;
    SCTE_SUBT_OUTLINE_STYLE_E    enOutlineStyle;
    SCTE_SUBT_BACKGROUND_STYLE_E enBackgroundStyle;
    SCTE_DISPLAY_STANDARD_E enDispStandard;

    HI_U32 u32TopXPos;
    HI_U32 u32TopYPos;
    HI_U32 u32BottomXPos;
    HI_U32 u32BottomYPos;

    HI_U32 u32SubtColor;
    HI_U32 u32PTS;
    HI_U32 u32Duration;
    HI_U32 u32BitWidth;

    union
    {
        SCTE_SUBT_OUTLINE_DROPSHADOW_S stDropshadow;
        SCTE_SUBT_OUTLINE_OUTLINED_S stOutline;
    } unOutlineStyle;

    SCTE_SUBT_BACKGROUND_FRAMED_S stFramed;

    HI_U32 u32BitmapDataLen;
    HI_U8  *pu8SCTESubtData;
} SCTE_SUBT_OUTPUT_S;

HI_S32 SCTE_SUBT_Parse_Init(HI_VOID);

HI_S32 SCTE_SUBT_Parse_DeInit(HI_VOID);

HI_S32 SCTE_SUBT_Parse_Create(HI_VOID* hDisplay, HI_VOID** pphParse);

HI_S32 SCTE_SUBT_Parse_Destroy(HI_VOID* hParse);

HI_S32 SCTE_SUBT_Parse_RegGetPtsCb(HI_VOID* hParse, HI_UNF_SUBT_GETPTS_FN pfnGetPts, HI_VOID* pUserData);

HI_S32 SCTE_SUBT_Parse_ParseSection(HI_VOID* hParse, HI_U8 *pu8DataSrc, HI_U32 u32DataLen);

#ifdef __cplusplus
}
#endif

#endif
