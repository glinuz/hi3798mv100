#include<stdlib.h>
#include<pthread.h>

#include "string.h"
#include "hi_type.h"

#include "cc708.h"
#include "hi_cc708_def.h"
#include "cc708_dec.h"
#include "cc708_obj.h"
#include "cc708_osd.h"
#include "hi_unf_cc.h"

#define USERDATA_BUFFER_SIZE 256

#define CHECKDATARANGE(data,min,max) if (data < min || data >= max) { \
    HI_ERR_CC(#data" is invalid\n"); \
    return HI_FAILURE; \
}

#define CHECKDATA(data,vlue) if (data >= vlue) { \
    HI_ERR_CC(#data"is invalid\n"); \
    return HI_FAILURE;\
}

#define CHECKCLRRANGE(rgb) if( (rgb!= 0x00000000)&&(rgb!= 0x0000005f)&&(rgb!= 0x000000af)&&(rgb!= 0x000000ff) ){ \
    HI_ERR_CC("this color is invalid\n"); \
    return HI_FAILURE;\
}

#define CHECKCLR(color) do{ \
     CHECKCLRRANGE(((color&0x00ff0000)>>16)); \
     CHECKCLRRANGE(((color&0x0000ff00)>>8)); \
     CHECKCLRRANGE(((color&0x000000ff))); \
}while(0)



//static HI_U8 s_ModuleNum = 0;

HI_S32 CC708_Init(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 CC708_DeInit(HI_VOID)
{
    return HI_SUCCESS;
}

HI_S32 CC708_Create(HI_HANDLE *hcc708)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 i = 0;
    CC708_OBJECT_S *pstCCObj = NULL;

    if(NULL == hcc708)
    {
        return HI_FAILURE;
    }

    for(i = 0; i < DTVCC_MAX_MODULE; i++ )
    {
        pstCCObj = CC708_OBJ_GetHandle(i);
        if(pstCCObj != NULL)
        {
            *hcc708 = i;
            break;
        }
    }

    if(DTVCC_MAX_MODULE == i)
    {
        HI_ERR_CC("error-module num over!\n");
        return HI_FAILURE;
    }


    if(pstCCObj->enModuleState == CC_READY)
    {
        return HI_FAILURE;
    }

    s32Ret = CC708_OBJ_Create(*hcc708);
    if(HI_FAILURE == s32Ret)
    {
        HI_ERR_CC("CC708_OBJ_Create failed!\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;

}

HI_S32 CC708_Destroy(HI_HANDLE hcc708)
{
    CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle((HI_U8)hcc708);
    if(hcc708 >= DTVCC_MAX_MODULE || pstCCObj==NULL)
    {
        HI_ERR_CC("Invalide module id:%d\n",hcc708);
        return HI_FAILURE;
    }

    EnterCriticalSection( &pstCCObj->mCriticalSection );

    CCQueue_Flush(&(pstCCObj->stUserdataQueue));
    (HI_VOID)CC708_DEC_Reset((HI_U8)hcc708);
    (HI_VOID)CC708_OSD_Reset();

    LeaveCriticalSection( &pstCCObj->mCriticalSection );
    CC708_OBJ_Destroy((HI_U8)hcc708);
    //s_ModuleNum = 0;

    return HI_SUCCESS;
}

HI_S32 CC708_Start(HI_U8 moduleID)
{
    CC708_OBJECT_S *pstCCObj = CC708_OBJ_GetHandle(moduleID);
    if(pstCCObj == NULL)
    {
        HI_ERR_CC("Invalide module id:%d\n",moduleID);
        return HI_FAILURE;
    }
    HI_INFO_CC("Start CC708 Decoder!\n");
    if(pstCCObj->u8IsStart)
    {
        return HI_SUCCESS;
    }

    EnterCriticalSection( &pstCCObj->mCriticalSection );

    pstCCObj->u8IsDelay = 0;
    pstCCObj->u8IsStart = 1;
    (HI_VOID)CC708_DEC_Start(moduleID);
    (HI_VOID)CC708_OSD_Start();
    CC708_OSD_ClrCCScreen();

    LeaveCriticalSection( &pstCCObj->mCriticalSection );

    return HI_SUCCESS;
}

HI_U8 CC708_IsStart(HI_U8 moduleID)
{
    HI_U8 u8value = 0;
    CC708_OBJECT_S *pstCCObj = CC708_OBJ_GetHandle(moduleID);
    if( pstCCObj == NULL )
    {
       HI_ERR_CC("Invalide module id:%d\n",moduleID);
       return 0;
    }
    EnterCriticalSection( &pstCCObj->mCriticalSection );

    u8value = pstCCObj->u8IsStart;

    LeaveCriticalSection( &pstCCObj->mCriticalSection );

    return u8value;
}
/*******************************************************************************
* Function: RETURN_TYPE CC_Config(int moduleid,DTVCC_CONFIG *pConfig)
* Description: CC module configure routine
*******************************************************************************/
HI_S32 CC708_Config(HI_U8 moduleid,HI_UNF_CC_708_CONFIGPARAM_S *pCC708Config)
{
    CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle(moduleid);
    HI_UNF_CC_RECT_S stScreenRect = {0,0,0,0};

    if( pstCCObj == NULL || pCC708Config == NULL )
    {
        HI_ERR_CC("Handle invalid!\n");
        return HI_FAILURE;
    }

    CHECKDATARANGE(pCC708Config->enCC708ServiceNum, HI_UNF_CC_708_SERVICE1, HI_UNF_CC_708_SERVICE_BUTT);
    CHECKDATA(pCC708Config->enCC708FontName,  HI_UNF_CC_FN_BUTT);
    CHECKDATA(pCC708Config->enCC708FontSize,  HI_UNF_CC_FONTSIZE_BUTT);
    CHECKCLR(pCC708Config->u32CC708TextColor);
    CHECKCLR(pCC708Config->u32CC708BgColor);
    CHECKDATA(pCC708Config->enCC708TextOpac,  HI_UNF_CC_OPACITY_BUTT);
    CHECKDATA(pCC708Config->enCC708BgOpac,  HI_UNF_CC_OPACITY_BUTT);
    CHECKDATA(pCC708Config->enCC708FontStyle,  HI_UNF_CC_FONTSTYLE_BUTT);
    CHECKCLR(pCC708Config->u32CC708WinColor);
    CHECKDATA(pCC708Config->enCC708WinOpac,  HI_UNF_CC_OPACITY_BUTT);
    CHECKDATA(pCC708Config->enCC708TextEdgeType,  HI_UNF_CC_EDGETYPE_BUTT);
    CHECKCLR(pCC708Config->u32CC708TextEdgeColor);
    CHECKDATA(pCC708Config->enCC708DispFormat,  HI_UNF_CC_DF_BUTT);

    if (pCC708Config->enCC708TextOpac != HI_UNF_CC_OPACITY_TRANSPARENT
        && pCC708Config->enCC708BgOpac != HI_UNF_CC_OPACITY_TRANSPARENT)
    {
        if ((pCC708Config->u32CC708TextColor != HI_UNF_CC_COLOR_DEFAULT && pCC708Config->u32CC708TextColor == pCC708Config->u32CC708BgColor)
            || (pCC708Config->u32CC708TextColor == HI_UNF_CC_COLOR_DEFAULT && pCC708Config->u32CC708BgColor == HI_UNF_CC_COLOR_WHITE)
            || (pCC708Config->u32CC708TextColor == HI_UNF_CC_COLOR_BLACK && pCC708Config->u32CC708BgColor == HI_UNF_CC_COLOR_DEFAULT))
        {
            HI_ERR_CC("CC708_Config color(0x%x 0x%x) error\n",pCC708Config->u32CC708TextColor,pCC708Config->u32CC708BgColor);
            return HI_FAILURE;
        }
    }

    EnterCriticalSection( &pstCCObj->mCriticalSection );
    if(pstCCObj->u32ServiceType != pCC708Config->enCC708ServiceNum)
    {
        LeaveCriticalSection( &pstCCObj->mCriticalSection );
        (HI_VOID)CC708_Reset(moduleid);
        EnterCriticalSection( &pstCCObj->mCriticalSection );
        pstCCObj->u32ServiceType = pCC708Config->enCC708ServiceNum;
    }
    pstCCObj->u32UserFontName      = pCC708Config->enCC708FontName;
    pstCCObj->u32UserFontSize      = pCC708Config->enCC708FontSize;
    pstCCObj->u32UserTextFGColor   = pCC708Config->u32CC708TextColor;
    pstCCObj->u32UserTextBgColor   = pCC708Config->u32CC708BgColor;
    pstCCObj->u32UserTextFGOpacity = pCC708Config->enCC708TextOpac;
    pstCCObj->u32UserTextBGOpacity = pCC708Config->enCC708BgOpac;
    pstCCObj->u32UserFontStyle     = pCC708Config->enCC708FontStyle;
    pstCCObj->u32UserWinColor      = pCC708Config->u32CC708WinColor;
    pstCCObj->u32UserWinOpac       = pCC708Config->enCC708WinOpac;
    pstCCObj->u32UserTextEdgetype  = pCC708Config->enCC708TextEdgeType;
    pstCCObj->u32UserTextEdgecolor = pCC708Config->u32CC708TextEdgeColor;

    switch (pCC708Config->enCC708DispFormat)
    {
        case HI_UNF_CC_DF_720X480:
            stScreenRect.width = 720;
            stScreenRect.height = 480;
            break;
        case HI_UNF_CC_DF_720X576:
            stScreenRect.width = 720;
            stScreenRect.height = 576;
            break;
        case HI_UNF_CC_DF_960X540:
            stScreenRect.width = 960;
            stScreenRect.height = 540;
            break;
        case HI_UNF_CC_DF_1280X720:
            stScreenRect.width = 1280;
            stScreenRect.height = 720;
            break;
        case HI_UNF_CC_DF_1920X1080:
            stScreenRect.width = 1920;
            stScreenRect.height = 1080;
            break;
        default:
            stScreenRect.width = 1024;
            stScreenRect.height = 768;
            break;
    }

    pstCCObj->stScreen.PhysicalRect       = stScreenRect;
    pstCCObj->stScreen.CaptionRect.x      = stScreenRect.width /10;
    pstCCObj->stScreen.CaptionRect.y      = stScreenRect.height /10;
    pstCCObj->stScreen.CaptionRect.width  = stScreenRect.width * 8 /10;
    pstCCObj->stScreen.CaptionRect.height = stScreenRect.height * 8 /10;
    (HI_VOID)CCDISP_Screen_SetSize(stScreenRect);

    LeaveCriticalSection( &pstCCObj->mCriticalSection );

    return HI_SUCCESS;
}

HI_S32 CC708_GetConfig(HI_U8 moduleid,HI_UNF_CC_708_CONFIGPARAM_S *pstCC708ConfigParam)
{
    CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle(moduleid);
    if (NULL == pstCCObj|| NULL == pstCC708ConfigParam)
    {
        HI_ERR_CC("CC708_GetConfig param error\n");
        return HI_FAILURE;
    }

    pstCC708ConfigParam->enCC708ServiceNum = (HI_UNF_CC_708_SERVICE_NUM_E)pstCCObj->u32ServiceType;
    pstCC708ConfigParam->u32CC708BgColor = pstCCObj->u32UserTextBgColor;
    pstCC708ConfigParam->enCC708BgOpac = (HI_UNF_CC_OPACITY_E)pstCCObj->u32UserTextBGOpacity;
    pstCC708ConfigParam->enCC708FontName = (HI_UNF_CC_FONTNAME_E)pstCCObj->u32UserFontName;
    pstCC708ConfigParam->enCC708FontSize = (HI_UNF_CC_FONTSIZE_E)pstCCObj->u32UserFontSize;
    pstCC708ConfigParam->enCC708FontStyle = (HI_UNF_CC_FONTSTYLE_E)pstCCObj->u32UserFontStyle;
    pstCC708ConfigParam->u32CC708TextColor = pstCCObj->u32UserTextFGColor;
    pstCC708ConfigParam->u32CC708TextEdgeColor = pstCCObj->u32UserTextEdgecolor;
    pstCC708ConfigParam->enCC708TextEdgeType = (HI_UNF_CC_EdgeType_E)pstCCObj->u32UserTextEdgetype;
    pstCC708ConfigParam->enCC708TextOpac = (HI_UNF_CC_OPACITY_E)pstCCObj->u32UserTextFGOpacity;
    pstCC708ConfigParam->u32CC708WinColor = pstCCObj->u32UserWinColor;
    pstCC708ConfigParam->enCC708WinOpac = (HI_UNF_CC_OPACITY_E)pstCCObj->u32UserWinOpac;


    if(pstCCObj->stScreen.PhysicalRect.width == 1280 && pstCCObj->stScreen.PhysicalRect.height == 720)
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_1280X720;
    }
    else if(pstCCObj->stScreen.PhysicalRect.width == 1920 && pstCCObj->stScreen.PhysicalRect.height == 1080)
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_1920X1080;
    }
    else if(pstCCObj->stScreen.PhysicalRect.width == 720 && pstCCObj->stScreen.PhysicalRect.height == 480)
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_720X480;
    }
    else if(pstCCObj->stScreen.PhysicalRect.width == 720 && pstCCObj->stScreen.PhysicalRect.height == 576)
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_720X576;
    }
    else if(pstCCObj->stScreen.PhysicalRect.width == 960 && pstCCObj->stScreen.PhysicalRect.height == 540)
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_960X540;
    }
    else
    {
        pstCC708ConfigParam->enCC708DispFormat = HI_UNF_CC_DF_BUTT;
    }

    return HI_SUCCESS;

}

HI_S32 CC708_Stop(HI_U8 moduleID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC708_OBJECT_S *pstCCObj = CC708_OBJ_GetHandle(moduleID);
    if(pstCCObj == NULL)
    {
        HI_ERR_CC("Invalide module id:%d\n",moduleID);
        return HI_FAILURE;
    }

    HI_INFO_CC("Stop CC708 Decoder!\n");

    if(pstCCObj->u8IsStart == 0)
    {
        return HI_SUCCESS;
    }

    EnterCriticalSection( &pstCCObj->mCriticalSection );

    CCQueue_Flush(&(pstCCObj->stUserdataQueue));
    s32Ret = CC708_DEC_Stop(moduleID);
    s32Ret |= CC708_OSD_Stop();
    s32Ret |= CC708_OSD_ClrCCScreen();
    pstCCObj->u8IsStart = 0;

    LeaveCriticalSection( &pstCCObj->mCriticalSection );

    return s32Ret;
}

/*******************************************************************************
* Function: HI_S32 CC708_Reset(HI_U8 moduleid)
* Description: CC module reset routine
*******************************************************************************/
HI_S32 CC708_Reset(HI_U8 moduleid)
{
    CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle(moduleid);

    if(pstCCObj==NULL)
    {
        HI_ERR_CC("Invalide module id:%d\n",moduleid);
        return HI_FAILURE;
    }

    if(pstCCObj->u8IsStart == 0)
    {
        return HI_SUCCESS;
    }

    HI_INFO_CC("CC708Reset\n");
    // enter the critical section
    EnterCriticalSection( &pstCCObj->mCriticalSection );

    CCQueue_Flush(&(pstCCObj->stUserdataQueue));

    (HI_VOID)CC708_DEC_Reset(moduleid);
    (HI_VOID)CC708_OSD_Reset();

    // leave the critical section
    LeaveCriticalSection( &pstCCObj->mCriticalSection );
    return HI_SUCCESS;
}

HI_S32 CC708_ParseUserData(HI_U8 moduleID, HI_U8 *pu8userdata, HI_U32 u32dataLen, HI_BOOL bTopFieldFirst)
{
     HI_S32 S32Ret = HI_SUCCESS;
     CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle(moduleID);

     if(pstCCObj == NULL || HI_NULL == pu8userdata || 0 == u32dataLen)
     {
         HI_ERR_CC("CC708_ParseUserData param invalid!\n");
         return HI_FAILURE;
     }

     S32Ret = CC708_DEC_UserDataParse(pu8userdata, u32dataLen, bTopFieldFirst);
     if(HI_FAILURE == S32Ret)
     {
        HI_ERR_CC("CC708_DEC_UserDataParse failed!\n");
     }

    return S32Ret;
}

HI_S32 CC708_Userdata_Inject(HI_U8 moduleID, HI_U8 *pu8UserData, HI_U32 u32UsrDataLen, HI_BOOL bTopFieldFirst)
{
    HI_S32 S32Ret = HI_SUCCESS;
    DTVCC_Element_S stCCElement;
    TRI_QUEUE *pstUserdataQueue;
    HI_U8 n = 0;
    CC708_OBJECT_S *pstCCObj= CC708_OBJ_GetHandle(moduleID);
    if(pstCCObj == NULL || HI_NULL == pu8UserData || 0 == u32UsrDataLen)
    {
         HI_ERR_CC("Invalide param:%d\n",u32UsrDataLen);
         return HI_FAILURE;
    }

    pstUserdataQueue = &(pstCCObj->stUserdataQueue);

    if( u32UsrDataLen > USERDATA_BUFFER_SIZE )
    {
        HI_WARN_CC("User data size overflow!\n");
        u32UsrDataLen = USERDATA_BUFFER_SIZE;
    }

    n = CCQueue_GetHeadPos(pstUserdataQueue);
    stCCElement.u16BufferSize = USERDATA_BUFFER_SIZE;
    stCCElement.u32DataSize = u32UsrDataLen;
    stCCElement.u32UserData = (HI_U32)bTopFieldFirst;
    stCCElement.pu8Buffer = (HI_U8 *)pstCCObj->au8UserData[n];

    memcpy(stCCElement.pu8Buffer,pu8UserData,u32UsrDataLen);
    if(!CCQueue_Insert(pstUserdataQueue,&stCCElement))
    {
        HI_WARN_CC("Userdata_queue FULL,head:%d,tail:%d\n",pstUserdataQueue->Head,pstUserdataQueue->Tail);
        CCQueue_Flush(pstUserdataQueue);
    }

    return S32Ret;
}


HI_S32 CC708_ProcessData(HI_VOID)
{
   return CC708_DEC_ProcessServicefifo();
}

HI_S32 CC708_CharFlash(HI_VOID)
{
    return CC708_OSD_CharFlash();
}

/********************************************************************
*                      End of file
********************************************************************/
