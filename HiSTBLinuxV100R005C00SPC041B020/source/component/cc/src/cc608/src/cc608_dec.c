/*******************************************************************************
*                       Include files
*******************************************************************************/

#include <unistd.h>
#include <stdio.h>

#include "hi_cc608_def.h"
#include "cc608_data.h"
#include "cc608.h"
#include "cc608_dec.h"
#include "cc608_obj.h"
#include "cc608_xds.h"
#include "cc_debug.h"

/*****************************************************************************
*                       Macro Definitions
*****************************************************************************/

/*****************************************************************************
*                       Type Definitions
*****************************************************************************/

/*******************************************************************************
*                       Extern Data Definition
*******************************************************************************/

/*******************************************************************************
*                       Static Data Definition
*******************************************************************************/

static HI_U16 s_u16CC608Char2UnicodeMap[96] =
{
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0xE1, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0xE9, 0x5D, 0xED, 0xF3,
        0xFA, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0xE7, 0xF7, 0xD1, 0xF1, 0x2588,
};

static HI_UNF_CC_COLOR_E s_enCCDispColorMap[] =
{
    HI_UNF_CC_COLOR_WHITE,   // white
    HI_UNF_CC_COLOR_GREEN,   // green
    HI_UNF_CC_COLOR_BLUE,    // blue
    HI_UNF_CC_COLOR_CYAN,    // cyan
    HI_UNF_CC_COLOR_RED,     // red
    HI_UNF_CC_COLOR_YELLOW,  // yellow
    HI_UNF_CC_COLOR_MAGENTA, // magenta
    HI_UNF_CC_COLOR_BLACK,   // black
};

/*******************************************************************************
*                       Static Function Prototype
*******************************************************************************/
static inline HI_U16 _CC608_DEC_DecodeSpecialAscii(HI_U8 u8Caption);
static HI_S32 _CC608_DEC_ValidationCheck(HI_U8 b1, HI_U8 b2);
static HI_S32 _CC608_DEC_Decode(HI_U8 module_id,HI_U16 u16Data, HI_BOOL bCaptionMode, HI_S32 s32Channel,HI_S32 s32FieldNum);
static HI_U8 _CC608_DEC_HasChar(HI_U8 moduleID,HI_U8 u8Row,HI_U8 u8Column);
static HI_U8 _CC608_DEC_HasBAC(HI_U8 moduleID,HI_U8 u8Row,HI_U8 u8Column);
/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/

HI_S32 CC608_DEC_Decode(HI_U8 module_id,HI_U16 *pu16CCData,  HI_U8 u8FieldNum)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    static HI_S32 s32NoValideCount =0;

    if (HI_NULL == pstObject)
    {
        DEC_TRACE("pstObject is null\n");
        return HI_FAILURE;
    }

    if( pu16CCData == HI_NULL )
    {
        HI_ERR_CC("Param NULL!\n");
        return HI_FAILURE;
    }
    if (u8FieldNum == CC608_VBI_FIELD1)
    {
        s32NoValideCount = 0;
        switch (pstObject->enCCNum)
        {
            case HI_UNF_CC_608_DATATYPE_CC1:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_TRUE,0,CC608_VBI_FIELD1);
                break;
            case HI_UNF_CC_608_DATATYPE_CC2:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_TRUE,1,CC608_VBI_FIELD1);
                break;
            case HI_UNF_CC_608_DATATYPE_TEXT1:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_FALSE,0,CC608_VBI_FIELD1);
                break;
            case HI_UNF_CC_608_DATATYPE_TEXT2:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_FALSE,1,CC608_VBI_FIELD1);
                break;
            default:
                break;
        }
    }
    else if (u8FieldNum == CC608_VBI_FIELD2)
    {
        s32NoValideCount = 0;

        switch (pstObject->enCCNum)
        {
            case HI_UNF_CC_608_DATATYPE_CC3:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_TRUE,0,CC608_VBI_FIELD2);
                break;
            case HI_UNF_CC_608_DATATYPE_CC4:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_TRUE,1,CC608_VBI_FIELD2);
                break;
            case HI_UNF_CC_608_DATATYPE_TEXT3:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_FALSE,0,CC608_VBI_FIELD2);
                break;
            case HI_UNF_CC_608_DATATYPE_TEXT4:
                (HI_VOID)_CC608_DEC_Decode(module_id, *pu16CCData, HI_FALSE,1,CC608_VBI_FIELD2);
                break;
            default:
                break;
        }
    }
    else if (u8FieldNum == 0x0e)
    {
        s32NoValideCount++;

        if (s32NoValideCount==10)
        {
            DEC_TRACE("USERDATA NO valid data to clear!\n");
            s32NoValideCount = 0;
        }
    }
    else if (u8FieldNum == 0x0f)
    {
         s32NoValideCount = 0;
    }
    return HI_SUCCESS;
}


HI_S32  CC608_DEC_CheckXDS(HI_U8 moduleID, HI_U16 *pu16CCData,  HI_U8 u8FieldNum)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    HI_U8 b1,b2;

    if (HI_NULL == pstObject)
    {
        DEC_TRACE("pstObject is null\n");
        return -2;
    }
    if( pu16CCData == HI_NULL )
    {
        return -1;
    }

    if (u8FieldNum == CC608_VBI_FIELD2)
    {
        b1 = (*pu16CCData) & 0xff;
        b2 = ((*pu16CCData) >> 8) & 0xff;

        /*0x01-0x0f, xds channel data,0x0f:end of xds data*/
        if( b1 >= 0x01 && b1 <= 0x0e )/*xds begin or continue code*/
        {
            pstObject->u8IsXDSStarted = 1;
            (HI_VOID)CC608_XDS_XDSDecode(moduleID,b1,b2);
            return 2;
        }
        /*codes from 0x10 to 0x1F indicates Caption or Text data.*/
        else if(b1 >= 0x10 && b1 <= 0x1F)
        {
            pstObject->u8IsXDSStarted = 0;
            return 0;
        }
        else
        {
            if( pstObject->u8IsXDSStarted == 1 )  /*xds data*/
            {
                (HI_VOID)CC608_XDS_XDSDecode(moduleID,b1,b2);
                if(b1 == 0x0f)/*xds end code*/
                {
                    pstObject->u8IsXDSStarted = 0;
                }
                return 2;
            }
            else
            {
                return 0;
            }
        }
    }

    return 0;
}


/*****************************************************************************
* function name: CC608_DEC_PACDCD(...)                                      *
* description:   decode pac code                                            *
* note:          this function will be acted as two function and will be    *
*       called at two condition. so we use the flag to indicate the conditio*
*        flag = 0: called at received this code.                            *
*        flag = 1; called by pop_caption, at this circumstance we can't set *
*                  the cusor.just set the text attributes                   *
*                                                                           *
*               see CEA 608_D :Tabel 53,P100                                *
*****************************************************************************/
void CC608_DEC_PAC(HI_U8 moduleID,HI_U8 b1,HI_U8 b2,HI_U8 u8Flag)
{
    //CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    static HI_U8  u8RowData[] = {10,0,0,1,2,3,11,12,13,14,4,5,6,7,8,9};
    HI_S32 s32CurRow = 0, s32CurColumn = 0;
    HI_U8 u8IsUnderline = 0,u8IsItalic = 0;
    HI_U32 u32TxtColor = 0;

    s32CurRow = u8RowData[((b1<<1)&0x0E)|((b2>>5)&0x01)];
    u8IsUnderline = b2 & 0x1;

    DEC_TRACE("CC608_DEC_PAC: u8Flag=%d, code pair[%2x %2x]\n",u8Flag,b1,b2);

    if (b2&0x10)/*b2(50-5F, 70-7F)*/
    {
        s32CurColumn = (b2&0x0E) << 1;

        /*if the pac received after any displayable charactersit just specify the location*/
        if( u8Flag == 0 )
        {
            CC608_DATA_SetRow(moduleID,s32CurRow);
            CC608_DATA_SetColumn(moduleID,s32CurColumn);
            if( _CC608_DEC_HasChar(moduleID,s32CurRow,s32CurColumn) )
            {
                /*to Set the preview attributes*/
                CC608_DATA_ResumeLastAttr(moduleID);
                return;
            }
        }
    }
    else /*b2(40-4f,60-6f)*/
    {
        if(((b2&0x0E) >> 1) == 0x07)
        {
            u8IsItalic = 1;
            DEC_TRACE("Got set text italic command");
        }
        else
        {
            u32TxtColor = s_enCCDispColorMap[((b2&0x0E) >> 1)];
        }
    }

    DEC_TRACE("CC608_DEC_PAC: row=%d,column=%d,underline=%d,textcolor=0x%x,italic=%d\n",s32CurRow,s32CurColumn, u8IsUnderline,u32TxtColor,u8IsItalic);
    if( u8Flag == 0 )
    {
        CC608_DATA_SetRow(moduleID,s32CurRow);
        CC608_DATA_SetColumn(moduleID,s32CurColumn);
        CC608_DATA_SetPacCode(moduleID,b2);
    }

    /*any color mid-row code will turn off italics*/
    CC608_DATA_SetTextItalic(moduleID,u8IsItalic);
    /*any color italics mid-row code will turn off flash*/
    CC608_DATA_SetTextAttr(moduleID,u32TxtColor,u8IsUnderline);
    (HI_VOID)CC608_DATA_FlashOn(moduleID, 0);

    /*If no BAC is received before the first character of an empty row,
      the default background attribute is opaue black*/
    if(_CC608_DEC_HasBAC(moduleID,s32CurRow,s32CurColumn) == 0)
    {
         CC608_DATA_SetBgColor(moduleID,HI_UNF_CC_COLOR_BLACK,HI_UNF_CC_OPACITY_SOLID);
    }
}

/*************************************************************************
* function name: CC608_DEC_ColorDCD(...)                                *
* description  : Decode Background color or Text color                  *
*                decode 608 spec p17 Table 3                            *
*************************************************************************/
void CC608_DEC_Color(HI_U8 moduleID,HI_U8 b1, HI_U8 b2)
{
    HI_UNF_CC_OPACITY_E enBgOpacity = HI_UNF_CC_OPACITY_SOLID;
    HI_U32 u32BgColor,u32TxtColor;
    HI_U8 u8IsUnderline;

    if( (b1 & 0x07) == 0x07 ) /*b1 == 17 0r 1F*/
    {
        if( b2 == 0x2d )/*blackground transparent*/
        {
            enBgOpacity = HI_UNF_CC_OPACITY_TRANSPARENT;//CC608TRSP;
            DEC_TRACE("Get background transparent!\n");
            CC608_DATA_SetBgColor(moduleID, HI_UNF_CC_COLOR_BLACK, enBgOpacity);
        }
        else if( (b2 == 0x2e) || (b2 == 0x2f))/*foreground black*/
        {
            u32TxtColor = HI_UNF_CC_COLOR_BLACK;
            u8IsUnderline = b2 & 0x1;    /*b2=2f:underline*/
            DEC_TRACE("Got caption underline command!\n");
            CC608_DATA_SetTextAttr(moduleID,u32TxtColor,u8IsUnderline);
            return;
        }
    }
    else  /*b1 == 0x10 0r 0x18*/
    {
        u32BgColor = s_enCCDispColorMap[((b2&0x0E) >> 1)];
        enBgOpacity  = ((b2&0x1) == 0)? HI_UNF_CC_OPACITY_SOLID : HI_UNF_CC_OPACITY_TRANSLUCENT;  /*0:opaque(solid) 1:semi_transparent*/

        CC608_DATA_SetBgColor(moduleID, u32BgColor, enBgOpacity);
    }
}

/*************************************************************************
* function name: CC608_DEC_MidRowDCD(...)                                *
* description  : Decode mid row code                                     *
*                decode 608 spec p98 Table 51                            *
*************************************************************************/
void CC608_DEC_MidRow(HI_U8 moduleID, HI_U8 b1, HI_U8 b2)
{
    HI_U32 u32TxtColor = 0;
    HI_U8 u8IsItalic = 0;
    HI_U8 u8IsUnderline;

    /*the cursor moves automatically one column to the right after
      each character or mid-row code received*/
    u8IsUnderline = (b2 & 0x01);
    if( ((b2 & 0x0E) >> 1) == 0x07)/*b2 = 2e or 2f*/
    {
        u8IsItalic = 1;
        CC608_DATA_GetTextAttr(moduleID,&u32TxtColor,NULL);
    }
    else
    {
        u32TxtColor = s_enCCDispColorMap[((b2&0x0E) >> 1)];
    }

    DEC_TRACE("CC608_DEC_MidRow: underline=%d,u32TxtColor=0x%x,italic=%d\n", u8IsUnderline, u32TxtColor,u8IsItalic);

    /*any color mid-row code will turn off italics*/
    CC608_DATA_SetTextItalic(moduleID,u8IsItalic);
    CC608_DATA_SetTextAttr(moduleID,u32TxtColor,u8IsUnderline);

    /*any color italics mid-row code will turn off flash*/
    (HI_VOID)CC608_DATA_FlashOn(moduleID, 0);

    return;
}

/*************************************************************************
* function name: CC608_DEC_CommandDCD(...)
* description  : Decode miscellaneouse  control codes
*                decode 608 spec p98 Table 52
*************************************************************************/
void CC608_DEC_Command(HI_U8 moduleID, HI_U8 u8IsCaption, HI_U8 b1, HI_U8 b2)
{
    HI_U8 u8RollRow;
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    HI_U16 u16Buf[1];

    if (HI_NULL == pstObject)
    {
        DEC_TRACE("pstObject is null\n");
        return;
    }

    pstObject->bPACOverwrite = HI_FALSE;

    //DEC_TRACE("CC608_DEC_Command: caption = %d, code pair[%2x %2x]\n",u8IsCaption,b1,b2);

    if ( (b1&0x07) == 7) /*Tab offset*/
    {
        DEC_TRACE("\tTO%d\n",b2 - 0x20);
        if( HI_TRUE == pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(moduleID) == 0) )
        {
            (HI_VOID)CC608_DATA_Tab( moduleID, (b2 - 0x20) );
        }
        return;
    }

    switch (b2)
    {
        case 0x20: // (RCL)start pop on captioning
            DEC_TRACE("\tRCL\n");
            if(u8IsCaption)
            {
                pstObject->u8IsCaptureText = HI_TRUE;
                (HI_VOID)CC608_DATA_ResumeCL( moduleID );
            }
            pstObject->enVbiMode = CC608_MODE_CAPTION;
            break;
        case 0x21: //backspace
            DEC_TRACE("\tBackSpace\n");
            if( HI_TRUE == pstObject->u8IsCaptureText  && (CC608_DATA_CheckRowColumnValidation(moduleID) == 0))
            {
                (HI_VOID)CC608_DATA_BackSpace( moduleID );
            }
            break;
        case 0x22: //Alarm off
            DEC_TRACE("\tAlarm off\n");
            (HI_VOID)CC608_DATA_AlarmOff( moduleID );
            break;
        case 0x23: //alarm on
            DEC_TRACE("\tAlarm on\n");
            (HI_VOID)CC608_DATA_AlarmOn( moduleID );
            break;
        case 0x24: //(DER)delete to end of row
            DEC_TRACE("\tDER\n");
            if( HI_TRUE == pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(moduleID) == 0) )
            {
                (HI_VOID)CC608_DATA_DeleteToER( moduleID );
            }
            break;
            // reset screen becase we've gone into roll up Mode
        case 0x25:
        case 0x26: //3
        case 0x27: //4
            DEC_TRACE("\tRU%d\n",b2 - 0x23);
            if(u8IsCaption)
            {
                u8RollRow = b2 - 0x23;
                (HI_VOID)CC608_DATA_Rollup( moduleID, u8RollRow );
                pstObject->u8CmdCount = 0;
            }
            pstObject->enVbiMode = CC608_MODE_CAPTION;
            break;
        case 0x28: //flash on
            DEC_TRACE("\tFlash on\n");
            if( HI_TRUE == pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(moduleID) == 0) )
            {
                u16Buf[0] = 0xbbbb;
                CC608_DATA_Caption(moduleID,u16Buf,1);
            }
            break;
        case 0x29: //(RDC)resume direct caption
            DEC_TRACE("\tRDC\n");
            if(u8IsCaption )
            {
                (HI_VOID)CC608_DATA_ResumeDC( moduleID );
            }
            pstObject->enVbiMode = CC608_MODE_CAPTION;
            break;
        case 0x2A: //text restart
            DEC_TRACE("\tTR\n");
            if(!u8IsCaption)
            {
                (HI_VOID)CC608_DATA_TextRestart( moduleID );
            }
            break;
        case 0x2B: //(RTD)resume text display
            DEC_TRACE("\tRTD\n");
            if(!u8IsCaption)
            {
                (HI_VOID)CC608_DATA_TextDisplay( moduleID );
            }
            break;
        case 0x2C: //(EDM)erase displayed memory
            DEC_TRACE("\tEDM\n");
            (HI_VOID)CC608_DATA_EraseDM( moduleID );
            break;
        case 0x2D: //(CR)carriage return
            DEC_TRACE("\tCR\n");
            DEC_TRACE("pstObject->u8IsCaptureText=%d\n",pstObject->u8IsCaptureText);
            if( HI_TRUE == pstObject->u8IsCaptureText  && (CC608_DATA_CheckRowColumnValidation(moduleID) == 0) )
            {
                (HI_VOID)CC608_DATA_CarriageReturn( moduleID );
                /*An attribute will remain in effect until changed by another
                control code or until the end of the row is reached
                CC608_DATA_SetDefaultAttr(moduleID);*/
            }
            break;
        case 0x2E: //(ENM)erase non-displayed memory
            DEC_TRACE("\tENM\n");
            (HI_VOID)CC608_DATA_EraseNM( moduleID );
            break;
        case 0x2F: //(EOC)end caption + swap memory
            DEC_TRACE("\tEOC\n");
            if(u8IsCaption)
            {
                (HI_VOID)CC608_DATA_FlipMemory( moduleID );
            }
            pstObject->enVbiMode = CC608_MODE_CAPTION;
            break;
        default:
            break;
    }

    if(pstObject->enCurMode == pstObject->enVbiMode )
    {
        pstObject->u8IsCaptureText = HI_TRUE;
    }
    else
    {
        pstObject->u8IsCaptureText = HI_FALSE;
    }

    return;
}

HI_U16 CC608_DEC_ExtCharSet(HI_U8 moduleID, HI_U16 u16LastChar,HI_U8 b1, HI_U8 b2)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    if (HI_NULL == pstObject)
    {
        DEC_TRACE("pstObject is null\n");
        return 0xffff;
    }

    /*CEA-608-D P20*/
    static HI_U16 s_u16ExtendCharMap[3][32]=
    {
        /*table 49 P95*/
        {0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,
        0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,  0x20,
        0xAE,  0xBA,  0xBD,  0xBF,  0x2122,0xA2,  0xA3,  0x266a,
        0xE0,  0x1111,0xE8,  0xE2,  0xEA,  0xEE,  0xF4,  0xFB},
        /*transparent space CEA-608-D  table 5,table 6,table 7*/
        {0xC1,  0xC9,  0xD3,  0xDA,  0xDC,  0xFC,  0x2018,0xa1,
        0x2A,  0x27,  0x2500,0xA9,  0x2120,0x2022,0x201C,0x201D,
        0xC0,  0xC2,  0xC7,  0xC8,  0xCA,  0xCB,  0xEB,  0xCE,
        0xCF,  0xEF,  0xD4,  0xD9,  0xF9,  0xDB,  0xAB,  0xBB},
        /*CEA-608-D   table 8,table 9,table 10*/
        {0xC3,  0xE3,  0xCD,  0xCC,  0xEC,  0xD2,  0xF2,  0xD5,
        0xF5,  0x7B,  0x7D,  0x5c,  0x5E,  0x5F,  0x7C,  0x7E,
        0xC4,  0xE4,  0xD6,  0xF6,  0xDF,  0xA5,  0xA4,  0xA6,
        0xC5,  0xE5,  0xD8,  0xF8,  0x250c,0x2510,0x2514,0x2518},
    };
     /*substitute caption of 64 extend chars*/
     static HI_U8 s_u8ExtendSubstitute[2][32]=
     {
        {0x41, 0x45, 0x4f, 0x55, 0x55, 0x75, 0x27, 0x69,
         0x7f, 0x27, 0x7f, 0x43, 0x53, 0x6f, 0x22, 0x22,
         0x41, 0x41, 0x43, 0x45, 0x45, 0x45, 0x65, 0x49,
         0x49, 0x69, 0x4f, 0x55, 0x75, 0x55, 0x22, 0x22},

        {0x41, 0x61, 0x49, 0x49, 0x69, 0x4f, 0x6f, 0x4f,
         0x6f, 0x5b, 0x5d, 0x2f, 0x7f, 0x7f, 0x7f, 0x7f,
         0x41, 0x61, 0x4f, 0x6f, 0x73, 0x7f, 0x7f, 0x7f,
         0x41, 0x61, 0x4f, 0x6f, 0x7f, 0x7f, 0x7f, 0x7f},
     };
     /*the following is for fluke implementation*/
     static HI_U8 s_u8ExtendSubstituteFluke[2][32]=
     {
        {0x41, 0x45, 0x4f, 0x55, 0x55, 0x75, 0x27, 0x69,
        0x2b, 0x27, 0x2d, 0x63, 0x53, 0x6f, 0x22, 0x22,
        0x41, 0x41, 0x43, 0x45, 0x45, 0x45, 0x65, 0x49,
        0x49, 0x69, 0x4f, 0x55, 0x75, 0x55, 0x22, 0x22},

        {0x41, 0x61, 0x49, 0x49, 0x69, 0x4f, 0x6f, 0x4f,
        0x6f, 0x28, 0x29, 0x2f, 0x2d, 0x2d, 0x2f, 0x2d,
        0x41, 0x61, 0x4f, 0x6f, 0x73, 0x59, 0x2d, 0x2f,
        0x41, 0x61, 0x4f, 0x6f, 0x2b, 0x2b, 0x2b, 0x2b},
    };

     HI_U8 u8Row, u8Column;
     HI_U16 u16ExtCaption[1];
     HI_S32 ret = 0;

     u8Row = (b1 & 0x07) - 1;
     u8Column = b2 - 0x20;
     if( u8Row >= 3 || u8Column >= 32 )
     {
         HI_ERR_CC("Invalide extend char! b1=%d,b2=%d\n",b1,b2);
         return 0;
     }
     u16ExtCaption[0] = s_u16ExtendCharMap[u8Row][u8Column];

     DEC_TRACE("CC608_DEC_ExtCharSet, u16LastChar : %d, u16ExtCaption : %#x\n",u16LastChar,u16ExtCaption[0]);

     /*Each of the 64 Extended Chracters incorporates an automatic
     backspace. and before the extend char ,there should be ba normal char*/
     /*row 0 is for FCC special chars. it needn't backspace*/
     if ( u8Row != 0 )
     {
         /*extend char validation check*/
         if( u16LastChar != s_u8ExtendSubstitute[u8Row-1][u8Column] &&
             u16LastChar != s_u8ExtendSubstituteFluke[u8Row-1][u8Column] &&
             u16LastChar != 0x3f )
         {
             /*the last byte before extend char isn't the specific substitute char*/
             if(pstObject->bIgnoreExtCharWithoutStdChar)
             {
                return 0;  // for VG859
             }
         }
         ret = CC608_DATA_Substitute(moduleID);
     }
     if( ret == 0 )
     {
         /*backspace valide*/
         CC608_DATA_Caption(moduleID,u16ExtCaption,1);
     }

     return u16ExtCaption[0];
}

HI_S32 CC608_DEC_Convert2Unicode(HI_U8 *pu8CCChars, HI_U32 u32Len, HI_U16 *pu16UniChars)
{
    HI_U32 i;

    if((pu8CCChars == NULL) || (pu16UniChars == NULL))
    {
        HI_ERR_CC("%s:NULL pointer!\n",__FUNCTION__);
        return HI_FAILURE;
    }

    for(i=0; i<u32Len; i++)
    {
        if((pu8CCChars[i] < 0x20) ||(pu8CCChars[i] > 0x7f))
        {
            HI_ERR_CC("Invalid CC608 character:0x%x\n",pu8CCChars[i]);
            return HI_FAILURE;
        }

        pu16UniChars[i] = s_u16CC608Char2UnicodeMap[pu8CCChars[i] - 0x20];
    }

    return HI_SUCCESS;
}

/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/

static inline HI_U16 _CC608_DEC_DecodeSpecialAscii(HI_U8 u8Caption)
{
    return s_u16CC608Char2UnicodeMap[u8Caption - 0x20];
}

static HI_S32 _CC608_DEC_ValidationCheck(HI_U8 b1, HI_U8 b2)
{
    HI_S32 ret = 0;
    switch(b1)
    {
        case 0x10:
        case 0x18:
            if((b2>=0x20&&b2<=0x2f) || (b2>=0x40&&b2<=0x5f))
            {
               ret = 1;
            }
            break;
        case 0x11:
        case 0x19:
        case 0x12:
        case 0x1a:
        case 0x13:
        case 0x1b:
            if((b2>=0x20&&b2<=0x7f))
            {
                ret = 1;
            }
            break;
        case 0x14:
        case 0x1c:
        case 0x15:
        case 0x1d:
            if((b2>=0x20&&b2<=0x2f)||(b2>=0x40&&b2<=0x7f))
            {
                ret = 1;
            }

            break;
        case 0x16:
        case 0x1e:
            if((b2>=0x40&&b2<=0x7f))
            {
                ret = 1;
            }
            break;
        case 0x17:
        case 0x1f:
            if((b2>=0x21&&b2<=0x2f&&b2!=0x2b)||(b2>=0x40&&b2<=0x7f))
            {
                ret = 1;
            }
            break;
        default:
            break;
    }
    return ret;
}

static HI_S32 _CC608_DEC_Decode(HI_U8 module_id,HI_U16 u16Data, HI_BOOL bCaptionMode, HI_S32 s32Channel,HI_S32 s32FieldNum)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(module_id);
    HI_U16 b1, b2;
    HI_U16 buf[64];
    HI_U8  u8CharNum = 0;
    HI_U8  u8SkipOneByte=0;
    static HI_U8  u8RepeatFlag=0;
    static HI_U16 u16PreData = 0;
    static HI_U16 u16LastCode = 0xff;

    if (HI_NULL == pstObject)
    {
        DEC_TRACE("pstObject is null\n");
        return HI_FAILURE;
    }

    b2 = u16Data & 0x7f;
    b1 = (u16Data>>8) & 0x7f;

    // skip empty code
    if ((b1 == 0x0) && (b2 == 0x0))
    {
        DEC_TRACE("empty code\n");
        return 0;
    }
    else if(b1 == 0)
    {
        b1 = b2;
        b2 = 0;
        u8SkipOneByte=1;
    }

    if( b1 > 0 && b1 < 0x10 )
    {
        if( CC608_VBI_FIELD2 == s32FieldNum ) //0x01-0x0f, xds channel data
        {
            DEC_TRACE("XDS packet !\n");
            pstObject->u8IsCaptureText = HI_FALSE;
            pstObject->enVbiMode = CC608_MODE_XDS;
        }
        else
        {
            b1 = b2;
            b2 = 0;
        }
    }

    /*distinguish between caption mode and text mode with same channel ID and field number*/
    if(pstObject->enCurMode != pstObject->enVbiMode)
    {
        pstObject->u8IsCaptureText = HI_FALSE;
    }

    if( (b1 <= 0x1f  && b1 >= 0x10) && (b2>0x1F) && (u16LastCode == u16Data) )
    {
        u8RepeatFlag = (u8RepeatFlag == 0)?1:0;
    }
    else
    {
        u8RepeatFlag=0;
    }

    if(b1 >= 0x20 && b1 <= 0x7f) //if (b1 & 0x60)  // Begin Caption Text Code part
    {
        if(pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(module_id) == HI_SUCCESS))
        {
            buf[u8CharNum++] = _CC608_DEC_DecodeSpecialAscii((HI_U8)b1);
            u16PreData = b1;  // copy first character
            if(b2 > 31)
            {
                buf[u8CharNum++] = _CC608_DEC_DecodeSpecialAscii((HI_U8)b2);
                u16PreData = b2; // copy second character
            }
            if(b1==0x20 && b2==0 && u8SkipOneByte==1)
            {
                 u8CharNum = 0;
                 u8SkipOneByte=0;
            }
            if (u8CharNum > 0 )
            {
                //printf("\tTEXT : %#x %#x\n", buf[0], buf[1]);
                CC608_DATA_Caption(module_id,buf,u8CharNum);
                u8CharNum = 0;
            }
            pstObject->u8IsTextShown = 1;
        }
        else
        {
            return HI_SUCCESS;
        }
    }// End Caption Text Code part

    /*******************************/
    /******    Control Code      ***/
    /*******************************/
    //codes are always transmitted twice (ignore the second occurance)
    else if ((b1 <= 0x1f  && b1 >= 0x10) && (b2>0x1F) && ((u16Data != u16LastCode) || ((u16Data == u16LastCode)&&( 0 == u8RepeatFlag ))))
    {
        // if we have been asked for the other channel, need to ignore any text that follows
        // printf("Caption Control Code !\n");
        if(!_CC608_DEC_ValidationCheck(b1,b2))
        {
            DEC_TRACE("code pair not valid!\n");
            return HI_SUCCESS;
        }
        if(((b1>>3)&1) != s32Channel)/*cc1:channel 0,cc2:channel 1*/
        {
            pstObject->u8IsCaptureText = HI_FALSE;
            return HI_SUCCESS;
        }

        /*add the extend characters as the channel identification*/
        else if(pstObject->enCurMode == pstObject->enVbiMode)
        {
            pstObject->u8IsCaptureText = HI_TRUE;
        }

        // if in xds mode, now enter caption mode
        if(pstObject->enVbiMode == CC608_MODE_XDS)
        {
            pstObject->u8IsCaptureText = HI_TRUE;
        }

        if( (((b1&0x17)==0x14) || ((b1&0x17)==0x15) ) && (b2==0x2c) )
        {
            ;/*EDM Command*/
        }
        else
        {
            pstObject->u8IsTextShown = 1;
        }

        (HI_VOID)CC608_TimeoutErase_Start(module_id,CC608_AUTOERASE_TIMEOUT);

        if (b2 & 0x40)  //preamble address code (row & indent)
        {
            if( (b1 == 0x10) && (b2 >= 0x60) )
            {
                 return HI_SUCCESS;
            }
            if(pstObject->u8IsCaptureText)
            {
                DEC_TRACE("\tPAC (%#x %#x)\n",b1,b2);
                pstObject->u8CmdCount = 0;
                CC608_DEC_PAC(module_id,b1,b2,0);
            }
        }
        else
        {
            switch (b1 & 0x07)
            {
            case 0x00:  //(10 ..)or(18 ..)   P17.table 3-BAC
                if(pstObject->u8IsCaptureText && b2 >= 0x20 && b2 <= 0x2f
                   && (CC608_DATA_CheckRowColumnValidation(module_id) == 0)
                   && !pstObject->bOnlyUseSolidBlackBGIn608)
                {
                    DEC_TRACE("\tBAC (%#x)\n",u16PreData);
                    if(pstObject->enDispStyle == CC608_DISP_POPON)
                    {
                        pstObject->u8CmdCount += 1;
                    }

                    /*The service provider is expected to precede each
                    Background Attribute code with a standard space*/
                    if( (u16PreData == 0x20) || !pstObject->bIgnoreBACWithoutSpace )
                    {
                        //(HI_VOID)CC608_DATA_Substitute(module_id);
                        buf[0] = (b1<<8)|b2;
                        CC608_DATA_Caption(module_id,buf,1);
                    }
                }
                break;
            case 0x01:  //midrow or char (11 ..)or (19 ..)
                if(pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(module_id) == 0))
                {
                    switch (b2&0x70)
                    {
                        case 0x20:// for Mid_Row codes(table 51 P98)
                            DEC_TRACE("\tMidRow (%#x %#x)\n",b1,b2);
                            buf[0] = 0x1100|b2;
                            CC608_DATA_Caption(module_id,buf,1);
                            break;
                        case 0x30:  // for special character(table 49,P95)
                            DEC_TRACE("\tSpecial Characters (%#x %#x)\n",b1,b2);
                            (HI_VOID)CC608_DEC_ExtCharSet(module_id,u16PreData, b1,b2);
                            break;
                        default:
                            break;
                    }
                }
                break;
            case 0x02://(12 ..) or (1a ..)
            case 0x03://(13 ..) or (1b ..)
                    DEC_TRACE("\tSpecial Characters (%#x %#x)\n",b1,b2);
                    if(pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(module_id) == 0))
                    {
                        (HI_VOID)CC608_DEC_ExtCharSet(module_id, u16PreData, b1,b2);
                    }
                    break;
            case 0x04:  //(14 ..)or(1c ..) p98 table 52
                    if( b2 >= 0x20 && b2 <= 0x2f)
                    {
                        if(pstObject->enDispStyle == CC608_DISP_POPON)
                        {
                            pstObject->u8CmdCount += 1;
                        }
                        CC608_DEC_Command(module_id, bCaptionMode, b1, b2);
                    }
                    break;
            case 0x05:  //field 2 control code p32.8.4
                    if( b2 >= 0x20 && b2 <= 0x2f && s32FieldNum == CC608_VBI_FIELD2)
                    {
                        if(pstObject->enDispStyle == CC608_DISP_POPON)
                        {
                            pstObject->u8CmdCount += 1;
                        }
                        CC608_DEC_Command(module_id, bCaptionMode, b1, b2);
                    }
                    break;
            case 0x07:  //(17 ..)or(1f ..)
                    if( (b2>=0x21) && (b2<=0x23) )//
                    {
                        CC608_DEC_Command(module_id, bCaptionMode, b1, b2);
                    }
                    else if((b2>=0x2d) && (b2<=0x2f) &&!pstObject->bOnlyUseSolidBlackBGIn608)//BAC p17.table 3
                    {
                        if(pstObject->u8IsCaptureText && (CC608_DATA_CheckRowColumnValidation(module_id) == 0))
                        {
                            DEC_TRACE("\tBAC (%#x)\n",u16PreData);
                            if(pstObject->enDispStyle == CC608_DISP_POPON)
                            {
                                pstObject->u8CmdCount += 1;
                            }
                            /*The service provider is expected to precede each
                            Background Attribute code with a standard space*/
                            if( (u16PreData == 0x20) || !pstObject->bIgnoreBACWithoutSpace)
                            {
                                //(HI_VOID)CC608_DATA_Substitute(module_id);
                                buf[0] = (b1<<8)|b2;
                                CC608_DATA_Caption(module_id,buf,1);
                            }
                        }
                    }
                    else if((b2>=0x24) && (b2<=0x2A))//P18.table 4
                    {
                        DEC_TRACE("\tSpecial Assignments (0x%x)\n",b2);
                    }
                    break;
            default:
                break;
            }
        }

        /*the last byte are command*/
        u16PreData = 0;
    } //Control Codes End

    /*if two two pair bytes come we must receive it*/
    if( u16LastCode == u16Data )
    {
        DEC_TRACE("Repeat code !\n");
        //LastCode = -1;
    }
    else
    {
        u16LastCode = u16Data;
    }
    return 0;
}

/******************************************************************
* function name: _CC608_DEC_HasChar(...)                         *
* description:   check if there is any char before current column*
* return value:  0 there are some chars before column            *
*                1 there is no chars before column               *
******************************************************************/
static HI_U8 _CC608_DEC_HasChar(HI_U8 moduleID,HI_U8 u8Row,HI_U8 u8Column)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    CC608_BUFFER_S *pstBuffer;

    if( NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",moduleID);
        return 0;
    }

    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }

    if( pstBuffer->u8CharNum[u8Row] < 1 )
    {
        return 0;
    }
    /*if there is already a displayable character in the column immediately
      to the left, the new character assumes the attributes of that character*/
    if( pstBuffer->stCaption[u8Row][u8Column-1].u8IsChar == 1 )
    {
        return 1;
    }
    return 0;
}

/******************************************************************
* function name: _CC608_DEC_HasBAC(...)                         *
* description:   check if there is any BACchar before current column*
* return value:  1 there are some BAC before column            *
*                0 there is no BAC before column               *
******************************************************************/
static HI_U8 _CC608_DEC_HasBAC(HI_U8 moduleID,HI_U8 u8Row,HI_U8 u8Column)
{
    CC608_OBJECT_S *pstObject = CC608_OBJ_Get(moduleID);
    CC608_BUFFER_S *pstBuffer;
    HI_U16 u16Caption;
    HI_S32 i;

    if( NULL==pstObject )
    {
        HI_ERR_CC("Invalid module id:%d\n",moduleID);
        return 0;
    }
    if( pstObject->enDispStyle == CC608_DISP_POPON )
    {
        pstBuffer = &(pstObject->stOffBuffer);
    }
    else
    {
        pstBuffer = &(pstObject->stOnBuffer);
    }

    for( i = 0; i <= u8Column; i++ )
    {
        u16Caption = (pstBuffer->stCaption[u8Row][i].u16Text&0xf7ff);
        if( ((u16Caption >= 0x1020)&&(u16Caption <= 0x102f)) ||
            ((u16Caption >= 0x172d)&&(u16Caption <= 0x172f)))
        {
            return 1;
        }
    }

    return 0;
}
