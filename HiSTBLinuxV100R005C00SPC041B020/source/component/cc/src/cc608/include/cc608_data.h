

#ifndef _CC608_DATA_H_
#define _CC608_DATA_H_

#include "hi_type.h"
#include "hi_cc608_def.h"
#include "hi_unf_cc.h"

#if defined __cplusplus || defined __cplusplus__
extern "C" {
#endif

/*****************************************************************************
*                    Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                    Type Definitions
*****************************************************************************/

/*****************************************************************************
*                    Extern Data Declarations
*****************************************************************************/

 /*****************************************************************************
*                    Extern Function Prototypes
*****************************************************************************/

void CC608_DATA_Create(HI_U8 module_id);
void CC608_DATA_Reset(HI_U8 module_id);

/*caption and it's attributes functions*/
void CC608_DATA_SetBgColor(HI_U8 module_id, HI_U32 u32BgColor, HI_UNF_CC_OPACITY_E enBgOpct);
void CC608_DATA_SetTextAttr(HI_U8 module_id, HI_U32 u32TxtColor, HI_U8 u8IsUnderline);
void CC608_DATA_GetTextAttr(HI_U8 module_id, HI_U32 *pu32TxtColor, HI_U8 *pu8IsUnderline);
void CC608_DATA_SetTextItalic(HI_U8 module_id, HI_U8 u8IsItalic);

void CC608_DATA_SetDefaultAttr(HI_U8 module_id);

void CC608_DATA_Caption(HI_U8 module_id, HI_U16 *pu16CaptionData, HI_U8 u8Len);

void CC608_DATA_SetRow(HI_U8 module_id, HI_U8 u8Row);
void CC608_DATA_SetColumn(HI_U8 module_id, HI_U8 u8Column);
void CC608_DATA_SetDefaultRowColumn(HI_U8 module_id, CC608_MODE_E enMode, CC608_DISP_STYLE_E enStyle);
HI_S32 CC608_DATA_CheckRowColumnValidation(HI_U8 module_id);
void CC608_DATA_SetPacCode(HI_U8 module_id, HI_U8 u8Pac);

HI_S32 CC608_DATA_Substitute(HI_U8 module_id);

/*control code command*/
HI_S32 CC608_DATA_ResumeCL(HI_U8 module_id);   /*Resume caption loading, enter pop on style*/
HI_S32 CC608_DATA_BackSpace(HI_U8 module_id);  /*backspace*/
HI_S32 CC608_DATA_AlarmOff(HI_U8 module_id);   /*reserved */
HI_S32 CC608_DATA_AlarmOn(HI_U8 module_id);    /*reserved */
HI_S32 CC608_DATA_DeleteToER(HI_U8 module_id); /*Delete to End of Row*/
HI_S32 CC608_DATA_Rollup(HI_U8 module_id, HI_U8 u8RollupRows);  /*rollup text rows*/
HI_S32 CC608_DATA_FlashOn(HI_U8 module_id, HI_U8 u8IsFlash);  /*caption flash on*/
HI_S32 CC608_DATA_ResumeDC(HI_U8 module_id);        /*Resume Direct Captioning, Enter Paint-on style*/


/*text mode functions*/
HI_S32 CC608_DATA_TextRestart(HI_U8 module_id);   /*text mode start*/
HI_S32 CC608_DATA_TextDisplay(HI_U8 module_id);   /*display text*/

HI_S32 CC608_DATA_EraseDM(HI_U8 module_id);       /*erase displayed memory(on screen)*/
HI_S32 CC608_DATA_CarriageReturn(HI_U8 module_id); /*change line*/
HI_S32 CC608_DATA_EraseNM(HI_U8 module_id);        /*Erase non-display memory*/
HI_S32 CC608_DATA_FlipMemory(HI_U8 module_id);     /*end of caption*/
HI_S32 CC608_DATA_Tab(HI_U8 module_id, HI_U8 u8Columns);

void CC608_DATA_ResumeLastAttr(HI_U8 module_id); /*set the last attr of one row*/

HI_S32 CC608_DATA_ClearSTA(HI_U8 module_id);

HI_S32 CC608_TimeoutErase_TimerInit(HI_U8 module_id);
void   CC608_TimeoutErase_TimerStop(HI_U8 module_id);
HI_S32 CC608_TimeoutErase_Start(HI_U8 module_id, HI_U32 u32Seconds);

#if defined __cplusplus || defined __cplusplus__
}
#endif

#endif

/*****************************************************************************
*                    End Of File

*****************************************************************************/
