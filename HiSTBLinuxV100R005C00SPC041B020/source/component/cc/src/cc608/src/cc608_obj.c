/*******************************************************************************
*                       Include files
*******************************************************************************/
#include <string.h>

#include "hi_type.h"
#include "hi_cc608_def.h"
#include "cc608.h"
#include "cc608_obj.h"
#include "cc_debug.h"

static CC608_OBJECT_S cc608Object[MAX_CC608OBJECT_NUM];

/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/
/*****************************************************************************
* function name: CC608_OBJ_Create(void)                                     *
* descripton   : Initial CC608_OBJECT_S                                        *
* arguments    :                                                            *
*                                                                           *
****************************************************************************/
void CC608_OBJ_Create(HI_U8 module_id)
{
    HI_S32 width = 19,heigth = 0;
    HI_U16 buf[1];

    if( module_id >= MAX_CC608OBJECT_NUM )
    {
        return;
    }
    cc608Object[module_id].module_id   = module_id;
    cc608Object[module_id].enCurMode    = CC608_MODE_CAPTION;
    cc608Object[module_id].enVbiMode    = CC608_MODE_CAPTION;
    cc608Object[module_id].u8IsModeModified = 1;
    cc608Object[module_id].enDispStyle       = CC608_DISP_ROLLUP;
    cc608Object[module_id].u8IsTextShown = 1;

    cc608Object[module_id].enWindowId = CCDISP_WINDOW_ID_0;
    cc608Object[module_id].stScreen.stPhyScreen.x      = 0;
    cc608Object[module_id].stScreen.stPhyScreen.y      = 0;
    cc608Object[module_id].stScreen.stPhyScreen.width  = 1280;
    cc608Object[module_id].stScreen.stPhyScreen.height = 720;
    cc608Object[module_id].stScreen.stStaScreen.x      = 128;
    cc608Object[module_id].stScreen.stStaScreen.y      = 72;
    cc608Object[module_id].stScreen.stStaScreen.width  = 1280*8/10;
    cc608Object[module_id].stScreen.stStaScreen.height = 720*8/10;

    cc608Object[module_id].stScreen.u16CurTextWidth     = 0;
    cc608Object[module_id].stScreen.u8CurRow           = 14;
    cc608Object[module_id].stScreen.u8CurColumn        = 0;
    cc608Object[module_id].stScreen.u8BaseRow          = 14;
    cc608Object[module_id].stScreen.u8TopRow           = 13;
    cc608Object[module_id].stScreen.u8RowHeight        = cc608Object[module_id].stScreen.stStaScreen.height / CC608ROWS;
    cc608Object[module_id].stScreen.u8CharWidth        = cc608Object[module_id].stScreen.stStaScreen.width / CC608COLUMNS;
    //cc608Object[module_id].screen.char_width        = cc608Object[module_id].screen.sta_screen.width / CC608ROWCHARS;
    cc608Object[module_id].stPenAttr.u32FgColor        = HI_UNF_CC_COLOR_WHITE;
    cc608Object[module_id].stPenAttr.enFgOpacity      = HI_UNF_CC_OPACITY_SOLID;
    cc608Object[module_id].stPenAttr.u32BgColor        = HI_UNF_CC_COLOR_BLACK;
    cc608Object[module_id].stPenAttr.enBgOpacity      = HI_UNF_CC_OPACITY_SOLID;
    cc608Object[module_id].stPenAttr.bIsItalic       = HI_FALSE;
    cc608Object[module_id].stPenAttr.bIsUnderline    = HI_FALSE;
    cc608Object[module_id].stPenAttr.bIsFlash        = HI_FALSE;

    cc608Object[module_id].stUserPenAttr.u32FgColor        = HI_UNF_CC_COLOR_DEFAULT;
    cc608Object[module_id].stUserPenAttr.enFgOpacity      = HI_UNF_CC_OPACITY_DEFAULT;
    cc608Object[module_id].stUserPenAttr.u32BgColor        = HI_UNF_CC_COLOR_DEFAULT;
    cc608Object[module_id].stUserPenAttr.enBgOpacity      = HI_UNF_CC_OPACITY_DEFAULT;
    cc608Object[module_id].stUserPenAttr.bIsItalic       = HI_FALSE;
    cc608Object[module_id].stUserPenAttr.bIsUnderline    = HI_FALSE;

    cc608Object[module_id].bLeadingTailingSpace     = HI_FALSE;

    cc608Object[module_id].stOnBuffer.u32CurSeq = 1;
    cc608Object[module_id].stOffBuffer.u32CurSeq = 1;
    cc608Object[module_id].stTextBuffer.u32CurSeq = 1;

    cc608Object[module_id].bIgnoreBACWithoutSpace = HI_TRUE;
    cc608Object[module_id].bIgnoreExtCharWithoutStdChar = HI_TRUE;
    cc608Object[module_id].bOnlyUseSolidBlackBGIn608 = HI_FALSE;

    memset(cc608Object[module_id].au8CharWidth,0,2);
    cc608Object[module_id].enCCNum                   = HI_UNF_CC_608_DATATYPE_CC1;
    cc608Object[module_id].u8IsDecStart                 = 0;
    cc608Object[module_id].u8CmdCount = 0;
    InitializeCriticalSection( &(cc608Object[module_id].stCriticalSection) );
    CCQueue_Init(&(cc608Object[module_id].stCCDataQueue),CC608DATA_QUEUE_SIZE,sizeof(CC608_ELEMENT_S));

    buf[0] = 0x57;/*W,calc the max width of the code set*/
    (HI_VOID)CCDISP_Text_GetSize(CCDISP_WINDOW_ID_0,buf,1,&width,&heigth);
    if (0 == width)
    {
        HI_ERR_CC("text size is error, char_width = %d\n", width);
    }

    cc608Object[module_id].au8CharWidth[0] = width;
    cc608Object[module_id].au8CharWidth[1] = width +1;

    (HI_VOID)CCDISP_Screen_SetSize(cc608Object[module_id].stScreen.stPhyScreen);
    (HI_VOID)CCDISP_Screen_SetClipArea(cc608Object[module_id].stScreen.stStaScreen);
    (HI_VOID)CCDISP_Window_Create(cc608Object[module_id].enWindowId, cc608Object[module_id].stScreen.stStaScreen);
    return;
}

/*******************************************************************************
* Function: HI_BOOL CC608_OBJ_Destroys(HI_U8 id)
* Description: destroy the CC object
*******************************************************************************/
void CC608_OBJ_Destroy(HI_U8 module_id)
{
    if(module_id>= MAX_CC608OBJECT_NUM )
    {
        return;
    }
    DeleteCriticalSection(&(cc608Object[module_id].stCriticalSection));
    CCQueue_Destroy(&cc608Object[module_id].stCCDataQueue);
    memset(&cc608Object[module_id], 0, sizeof(CC608_OBJECT_S));
    return;
}

/*****************************************************************************
* function name: cc608_getobj_by_id(HI_U8 module_id)               *
* descripton   : Get CC608_OBJECT_S pointer according to module_id             *
* arguments    :                                                            *
*                return CC608_OBJECT_S Pointer                                 *
****************************************************************************/
CC608_OBJECT_S *CC608_OBJ_Get(HI_U8 module_id)
{
    if( module_id >= MAX_CC608OBJECT_NUM )
    {
        return NULL;
    }
    return &(cc608Object[module_id]);
}

/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/

