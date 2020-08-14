
/*******************************************************************************
*                       Include files
*******************************************************************************/

#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

#include <string.h>
#include <semaphore.h>
#include <sys/ioctl.h>

#include "cc708.h"
#include "hi_cc708_def.h"
#include "cc708_obj.h"
#include "ccdisp_api.h"
#include "cc_debug.h"

static CC708_OBJECT_S CCObj[DTVCC_MAX_MODULE];

/*******************************************************************************
* Function: HI_S32  CC708_OBJ_Create(HI_U8 id)
* Description: initalize the CC object
*******************************************************************************/
HI_S32  CC708_OBJ_Create(HI_U8 u8ModuleID)
{
    if (u8ModuleID >= DTVCC_MAX_MODULE)
    {
        return HI_FAILURE;
    }

    CCObj[u8ModuleID].u8ModuleID = u8ModuleID;
    CCObj[u8ModuleID].u8IsStart = 0;
    CCObj[u8ModuleID].u8IsDelay = 0;
    CCObj[u8ModuleID].enModuleState = CC_READY;
    InitializeCriticalSection( &CCObj[u8ModuleID].mCriticalSection );

    /*User set options*/
    EnterCriticalSection(&CCObj[u8ModuleID].mCriticalSection);
    CCObj[u8ModuleID].u32ServiceType         = HI_UNF_CC_708_SERVICE1;
    LeaveCriticalSection(&CCObj[u8ModuleID].mCriticalSection);
    CCObj[u8ModuleID].u32UserWinOpac         = HI_UNF_CC_OPACITY_DEFAULT;
    CCObj[u8ModuleID].u32UserWinColor        = HI_UNF_CC_COLOR_DEFAULT;
    CCObj[u8ModuleID].u32UserTextBgColor     = HI_UNF_CC_COLOR_DEFAULT;
    CCObj[u8ModuleID].u32UserTextBGOpacity   = HI_UNF_CC_OPACITY_DEFAULT;
    CCObj[u8ModuleID].u32UserTextFGOpacity   = HI_UNF_CC_OPACITY_DEFAULT;
    CCObj[u8ModuleID].u32UserTextFGColor     = HI_UNF_CC_COLOR_DEFAULT;
    CCObj[u8ModuleID].u32UserFontName        = HI_UNF_CC_FN_DEFAULT;
    CCObj[u8ModuleID].u32UserFontSize        = HI_UNF_CC_FONTSIZE_DEFAULT;
    CCObj[u8ModuleID].u32UserFontStyle       = HI_UNF_CC_FONTSTYLE_DEFAULT;

    memset(&CCObj[u8ModuleID].stCMDBuffer, 0, sizeof(DTVCC_CMDBuffer_S));
    memset(&CCObj[u8ModuleID].stPacket, 0, sizeof(DTVCC_Packet_S));
    memset(&CCObj[u8ModuleID].stServiceBlock, 0, sizeof(DTVCC_ServiceBlock_S));
    CCObj[u8ModuleID].s32LastSeqenceNo       = -1;
    CCObj[u8ModuleID].bServiceBlockEnded     = HI_TRUE;

    CCObj[u8ModuleID].u8ScrollTimes = 6;


    CCQueue_Init(&(CCObj[u8ModuleID].stUserdataQueue),USERDATA_QUEUE_SIZE,sizeof(DTVCC_Element_S));
    CCQueue_Init(&(CCObj[u8ModuleID].stServiceQueue),SERVICE_QUEUE_SIZE,sizeof(DTVCC_ServiceElement_S));

    CCObj[u8ModuleID].stScreen.PhysicalRect.x          = 0;
    CCObj[u8ModuleID].stScreen.PhysicalRect.y          = 0;
    CCObj[u8ModuleID].stScreen.PhysicalRect.width      = 1280;
    CCObj[u8ModuleID].stScreen.PhysicalRect.height     = 720;
    CCObj[u8ModuleID].stScreen.CaptionRect.x           = CCObj[u8ModuleID].stScreen.PhysicalRect.width/10;
    CCObj[u8ModuleID].stScreen.CaptionRect.y           = CCObj[u8ModuleID].stScreen.PhysicalRect.height/10;
    CCObj[u8ModuleID].stScreen.CaptionRect.width       = CCObj[u8ModuleID].stScreen.PhysicalRect.width * 8 / 10;
    CCObj[u8ModuleID].stScreen.CaptionRect.height      = CCObj[u8ModuleID].stScreen.PhysicalRect.height* 8 / 10;
    CCObj[u8ModuleID].stScreen.u16MaxHorizonalCells    = MAX_CC_ABSOLUTE_ANCHOR_HORZ_16B9;
    CCObj[u8ModuleID].stScreen.u16MaxColumnNum         = MAX_CC_COLUMNS_4B3;
    (HI_VOID)CCDISP_Screen_SetSize(CCObj[u8ModuleID].stScreen.PhysicalRect);

    return HI_SUCCESS;
}

/*******************************************************************************
* Function: HI_BOOL DTVCCObjDestroy(HI_U8 id)
* Description: destroy the CC object
*******************************************************************************/
HI_BOOL CC708_OBJ_Destroy(HI_U8 id)
{
    if(id >= DTVCC_MAX_MODULE )
    {
        return HI_FALSE;
    }
    CCQueue_Destroy(&CCObj[id].stServiceQueue);
    CCQueue_Destroy(&CCObj[id].stUserdataQueue);
    memset(&CCObj[id], 0, sizeof(CC708_OBJECT_S));
    CCObj[id].enModuleState = CC_INIT;
    return HI_TRUE;
}

/*******************************************************************************
* Function: DTVCCOBJ * DTVCCGetObjHandle(HI_U8 id)
* Description: Get the CC Object handle
*******************************************************************************/
CC708_OBJECT_S * CC708_OBJ_GetHandle(HI_U8 ModuleId)
{
    CC708_OBJECT_S * pObject = NULL;
    if( ModuleId >= DTVCC_MAX_MODULE)
    {
        HI_ERR_CC("Invalide module id:%d!\n",ModuleId);
        return NULL;
    }

    pObject = &CCObj[ModuleId];
    return pObject;
}
/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/
