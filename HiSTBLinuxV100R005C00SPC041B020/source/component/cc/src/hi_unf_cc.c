#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#include "cc608.h"
#include "cc708.h"
#include "cc608_obj.h"
#include "cc708_obj.h"
#include "hi_cc608_def.h"
#include "hi_cc708_def.h"
#include "cc_queue.h"
#include "aribcc.h"
#include "hi_unf_cc.h"
#include "cc_debug.h"
#include "cc_timer.h"

#ifndef HI_ADVCA_FUNCTION_RELEASE
static const HI_CHAR s_szCCVersion[] __attribute__((used)) = USER_VERSION_STRING;
#endif

#define CC_MAX_MODULE_NUM 0x01
#define CC_HANDLE_BASE  ((HI_ID_CC << 16) & 0xFFFF0000)

static HI_VOID* s_ahCC[CC_MAX_MODULE_NUM];

static HI_BOOL s_bCCInit = HI_FALSE;
HI_BOOL s_bCCStartFlag = HI_FALSE;
HI_BOOL s_bCCEnableFlag = HI_FALSE;
static pthread_t u32CharFlashThread;

#define CheckCCInit() \
    if (HI_FALSE == s_bCCInit)\
    {\
        HI_ERR_CC("Not Init\n");\
        return HI_FAILURE;\
    }

#define CheckCCHandle(pstParam)\
    do{\
        HI_U8 i = 0; \
        for (i = 0; i < CC_MAX_MODULE_NUM; i++)\
        { \
            if ((s_ahCC[i] == pstParam) && (HI_NULL != pstParam))\
            { \
                break; \
            } \
        } \
        if (i >= CC_MAX_MODULE_NUM)\
        { \
            HI_ERR_CC("handle invalid!\n"); \
            return HI_FAILURE; \
        } \
    } while (0)

typedef struct tagCC_LOCAL_PARAM_S
{
    HI_HANDLE h608CC;
    HI_HANDLE h708CC;
    HI_VOID* hAribCC;
    HI_UNF_CC_DATA_TYPE_E enCCType;

    pthread_t u32CC608Thread;
    pthread_t u32CC708Thread;
}CC_LOCAL_PARAM_S;



static HI_U16 CC_GetHandleIndex(HI_HANDLE hCC)
{
	HI_U16 u16HandleIndex = hCC & 0x0000FFFF;

	return u16HandleIndex;
}


static HI_HANDLE CC_GetNewHandle(HI_U16 u16HandleIndex)
{
	HI_HANDLE hCC = HI_NULL;

	hCC = CC_HANDLE_BASE | u16HandleIndex;

	return hCC;
}

static HI_VOID* CC_GetParamAddr(HI_HANDLE hCC)
{
    HI_U16 u16HandleIndex = CC_GetHandleIndex(hCC);

    if (u16HandleIndex >= CC_MAX_MODULE_NUM)
    {
        return HI_NULL;
    }

    if (CC_HANDLE_BASE == (hCC & 0xFFFF0000))
    {
        return s_ahCC[u16HandleIndex];
    }
    else
    {
        return HI_NULL;
    }
}


static HI_S32 CC_GetFreeIndex(HI_U16* pu16FreeIndex)
{
    HI_U16  i = 0;

    for (i = 0; i < CC_MAX_MODULE_NUM; i++)
    {
        if (HI_NULL == s_ahCC[i])
        {
            break;
        }
    }

    if (i >= CC_MAX_MODULE_NUM)
    {
        HI_ERR_CC("instance num > %d\n", CC_MAX_MODULE_NUM);

        return HI_FAILURE;
    }

	*pu16FreeIndex = i;

    return HI_SUCCESS;
}

static void CC708DataThread(void *arg)
{
    HI_HANDLE h708cc = (HI_HANDLE)(intptr_t)arg;
    CC708_OBJECT_S *pCCObj= CC708_OBJ_GetHandle((HI_U8)h708cc);
    DTVCC_Element_S cc_element;
    TRI_QUEUE *puserdata_queue;


    if (HI_NULL == pCCObj)
    {
        return;
    }

    puserdata_queue = &(pCCObj->stUserdataQueue);

    while (s_bCCEnableFlag)
    {
        if (s_bCCStartFlag)
        {
            while(CCQueue_Get(puserdata_queue, &cc_element ) )
            {
                if(CC708_ParseUserData(h708cc, cc_element.pu8Buffer, cc_element.u32DataSize, (HI_BOOL)cc_element.u32UserData) == HI_SUCCESS)
                {
                    if(HI_FAILURE == CC708_ProcessData())
                    {
                        HI_INFO_CC("CC708_ProcessData error!\n");
                    }
                }
                else
                {
                    HI_INFO_CC("Userdata parse error!\n");
                }

                (HI_VOID)CCQueue_Remove(puserdata_queue,&cc_element);
            }

            /*when the userdata queue has no element,we can still process service block in service fifo*/
            if(HI_FAILURE == CC708_ProcessData())
            {
                HI_INFO_CC("CC708_ProcessData error!\n");
            }
        }
        usleep(10*1000);
    }
}

static void CC608Thread(void *arg)
{
    HI_HANDLE h608cc = (HI_HANDLE)(intptr_t)arg;
    CC608_OBJECT_S *pObject = CC608_OBJ_Get((HI_U8)h608cc);
    CC608_ELEMENT_S cc608_element;
    TRI_QUEUE *p608data_queue = &(pObject->stCCDataQueue);
    HI_U16 u16CCData = 0;
    HI_U8  u8FieldNum = 0;

    if(HI_NULL == p608data_queue)
    {
        HI_ERR_CC("p608data_queue is null\n");
        return;
    }

    while (s_bCCEnableFlag)
    {
        if (s_bCCStartFlag)
        {
            while(CCQueue_Get(p608data_queue, &cc608_element ) )
            {
                u8FieldNum = cc608_element.pu8Buffer[0];
                u16CCData = cc608_element.pu8Buffer[1] << 8 | cc608_element.pu8Buffer[2];
                if(CC608_IsStart((HI_U8)h608cc))
                {
                    if (CC608_Decode((HI_U8)h608cc, &u16CCData, u8FieldNum) != HI_SUCCESS)
                    {
                        HI_INFO_CC("CC608_Decode error!\n");
                    }
                }

                (HI_VOID)CCQueue_Remove(p608data_queue,&cc608_element);
            }
        }
        usleep(10*1000);
    }
}

static void CharFlashThread(void * arg)
{
     while(s_bCCEnableFlag)
     {
        if(s_bCCStartFlag)
        {
            CC708_CharFlash();
        }
        usleep(500*1000);
     }
}

static void StartCCDataThread(CC_LOCAL_PARAM_S *pstParam)
{
    s_bCCEnableFlag = HI_TRUE;

    if(0 == pstParam->u32CC708Thread)
    {
        if(pthread_create(&(pstParam->u32CC708Thread), HI_NULL, (void* (*)(void *))CC708DataThread,(void*)(intptr_t)pstParam->h708CC)!=0)
        {
            HI_ERR_CC("Can't create u32CC708Thread task,exit\n");
            return;
        }
    }
    if(0 == pstParam->u32CC608Thread)
    {
        if(pthread_create(&pstParam->u32CC608Thread, HI_NULL, (void* (*)(void *))CC608Thread,(void*)(intptr_t)pstParam->h608CC)!=0)
        {
            HI_ERR_CC("Can't create u32CC608Thread task,exit\n");
            return;
        }
    }
    if(0 == u32CharFlashThread)
    {
        if(pthread_create(&u32CharFlashThread,HI_NULL,(void* (*)(void *))CharFlashThread,HI_NULL)!=0)
        {
            HI_ERR_CC("Can't create u32CharFlashThread task,exit\n");
            return;
        }
    }
}

static void StopCCDataThread(CC_LOCAL_PARAM_S *pstParam)
{
    s_bCCEnableFlag = HI_FALSE;

    if(u32CharFlashThread)
    {
        (HI_VOID)pthread_join(u32CharFlashThread,HI_NULL);
        u32CharFlashThread = 0;
    }
    if (pstParam->u32CC708Thread)
    {
        (HI_VOID)pthread_join(pstParam->u32CC708Thread, HI_NULL);
        pstParam->u32CC708Thread = 0;
    }
    if (pstParam->u32CC608Thread)
    {
        (HI_VOID)pthread_join(pstParam->u32CC608Thread, HI_NULL);
        pstParam->u32CC608Thread = 0;
    }
}

HI_S32 HI_UNF_CC_Init(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;

    if( HI_FALSE == s_bCCInit )
    {
        s32Ret = CC708_Init();
        s32Ret |= CC608_Init();
        s32Ret |= AribCC_Init();
        s32Ret |= CCTimer_Init();

        memset(s_ahCC, 0, sizeof(s_ahCC));

        s_bCCInit = HI_TRUE;
    }

    return s32Ret;
}

HI_S32 HI_UNF_CC_DeInit(HI_VOID)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8Index = 0;

    if( HI_TRUE == s_bCCInit )
    {
        s32Ret = CC708_DeInit();
        s32Ret |= CC608_DeInit();
        s32Ret |= AribCC_DeInit();
        s32Ret |= CCTimer_DeInit();

        for(u8Index = 0; u8Index < CC_MAX_MODULE_NUM; u8Index ++)
        {
            if (s_ahCC[u8Index])
            {
                HI_HANDLE hCC = CC_GetNewHandle(u8Index);
                (HI_VOID)HI_UNF_CC_Destroy(hCC);
            }
        }

        memset(s_ahCC, 0, sizeof(s_ahCC));

        s_bCCInit = HI_FALSE;
    }

    return s32Ret;
}

HI_S32 HI_UNF_CC_GetDefaultAttr(HI_UNF_CC_ATTR_S *pstDefaultAttr)
{
    if (HI_NULL == pstDefaultAttr)
    {
        HI_ERR_CC("params invalid\n");
        return HI_FAILURE;
    }

    if (HI_UNF_CC_DATA_TYPE_608 == pstDefaultAttr->enCCDataType)
    {
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.bLeadingTailingSpace = HI_FALSE;
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.enCC608DataType = HI_UNF_CC_608_DATATYPE_CC1;
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.enCC608DispFormat = HI_UNF_CC_DF_1280X720;

        pstDefaultAttr->unCCConfig.stCC608ConfigParam.u32CC608TextColor = HI_UNF_CC_COLOR_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.enCC608TextOpac = HI_UNF_CC_OPACITY_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.u32CC608BgColor = HI_UNF_CC_COLOR_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC608ConfigParam.enCC608BgOpac = HI_UNF_CC_OPACITY_DEFAULT;

        pstDefaultAttr->unCCConfig.stCC608ConfigParam.enCC608FontStyle = HI_UNF_CC_FONTSTYLE_DEFAULT;
    }
    else if (HI_UNF_CC_DATA_TYPE_708 == pstDefaultAttr->enCCDataType)
    {
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708ServiceNum = HI_UNF_CC_708_SERVICE1;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708DispFormat = HI_UNF_CC_DF_1280X720;

        pstDefaultAttr->unCCConfig.stCC708ConfigParam.u32CC708WinColor = HI_UNF_CC_COLOR_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.u32CC708BgColor = HI_UNF_CC_COLOR_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.u32CC708TextColor = HI_UNF_CC_COLOR_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.u32CC708TextEdgeColor = HI_UNF_CC_COLOR_DEFAULT;

        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708WinOpac = HI_UNF_CC_OPACITY_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708BgOpac = HI_UNF_CC_OPACITY_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708TextOpac = HI_UNF_CC_OPACITY_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708TextEdgeType = HI_UNF_CC_EDGETYPE_DEFAULT;

        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708FontName = HI_UNF_CC_FN_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708FontSize = HI_UNF_CC_FONTSIZE_DEFAULT;
        pstDefaultAttr->unCCConfig.stCC708ConfigParam.enCC708FontStyle = HI_UNF_CC_FONTSTYLE_DEFAULT;

    }
    else if (HI_UNF_CC_DATA_TYPE_ARIB == pstDefaultAttr->enCCDataType)
    {
        pstDefaultAttr->unCCConfig.stCCARIBConfigParam.u32BufferSize = 0x80000; /*512 Kbyte*/
    }
    else
    {
        HI_ERR_CC("enCCDataType : %d not support\n", pstDefaultAttr->enCCDataType);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CC_Create( HI_UNF_CC_PARAM_S *pstCCParam, HI_HANDLE *phCC)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U8 u8ModuleId = 0;
    CC_LOCAL_PARAM_S *pstParam = HI_NULL;
    HI_U16 u16Index = 0;
    CCDISP_INIT_PARAM_S stDispInitParm;

    if (HI_NULL == phCC || HI_NULL == pstCCParam)
    {
        HI_ERR_CC("params invalid\n");
        return HI_FAILURE;
    }
    if (pstCCParam->stCCAttr.enCCDataType >= HI_UNF_CC_DATA_TYPE_BUTT)
    {
        HI_ERR_CC("enCCDataType : %d not support\n", pstCCParam->stCCAttr.enCCDataType);
        return HI_FAILURE;
    }

    CheckCCInit();

	s32Ret = CC_GetFreeIndex(&u16Index);
	if (HI_SUCCESS != s32Ret)
	{
		return HI_FAILURE;
	}


    stDispInitParm.stOpt.pfnGetPts = pstCCParam->pfnCCGetPts;
    stDispInitParm.stOpt.pfnDisplay = pstCCParam->pfnCCDisplay;
    stDispInitParm.stOpt.pfnGetTextSize = pstCCParam->pfnCCGetTextSize;
    stDispInitParm.stOpt.pfnBlit = pstCCParam->pfnBlit;
    stDispInitParm.stOpt.pfnVBIOutput = pstCCParam->pfnVBIOutput;
    stDispInitParm.stOpt.pfnXDSOutput = pstCCParam->pfnXDSOutput;
    stDispInitParm.stOpt.pUserData = pstCCParam->pUserData;
    (HI_VOID)CCDISP_Init(&stDispInitParm);

    pstParam = (CC_LOCAL_PARAM_S *)malloc(sizeof(CC_LOCAL_PARAM_S));
    if (HI_NULL == pstParam)
    {
        HI_ERR_CC("Malloc error\n");
        return HI_FAILURE;
    }
	
    memset(pstParam,0,sizeof(CC_LOCAL_PARAM_S));

    if (pstCCParam->stCCAttr.enCCDataType == HI_UNF_CC_DATA_TYPE_608
        || pstCCParam->stCCAttr.enCCDataType == HI_UNF_CC_DATA_TYPE_708)
    {
        s32Ret = CC708_Create(&pstParam->h708CC);
        u8ModuleId = (HI_U8)(pstParam->h708CC);

        if (pstCCParam->stCCAttr.enCCDataType == HI_UNF_CC_DATA_TYPE_608)
        {
            s32Ret = CC608_Create(u8ModuleId);
            pstParam->h608CC = u8ModuleId;
            pstParam->enCCType = HI_UNF_CC_DATA_TYPE_608;

            s32Ret = CC608_Config(u8ModuleId,&pstCCParam->stCCAttr.unCCConfig.stCC608ConfigParam);
        }
        else
        {
            pstParam->enCCType = HI_UNF_CC_DATA_TYPE_708;
            s32Ret = CC708_Config(u8ModuleId,&pstCCParam->stCCAttr.unCCConfig.stCC708ConfigParam);
        }
    }
    else if (pstCCParam->stCCAttr.enCCDataType == HI_UNF_CC_DATA_TYPE_ARIB)
    {
        pstParam->hAribCC = AribCC_Create(&pstCCParam->stCCAttr.unCCConfig.stCCARIBConfigParam);
        pstParam->enCCType = HI_UNF_CC_DATA_TYPE_ARIB;
    }

	s_ahCC[u16Index] = pstParam;
    *phCC = CC_GetNewHandle(u16Index);

    return s32Ret;
}

HI_S32 HI_UNF_CC_Destroy(HI_HANDLE hCC)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_U16 u16HandleIndex = CC_GetHandleIndex(hCC);
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCInit();
    CheckCCHandle(pstParam);

    if(HI_UNF_CC_DATA_TYPE_608 == pstParam->enCCType
        || HI_UNF_CC_DATA_TYPE_708 == pstParam->enCCType)
    {
        StopCCDataThread(pstParam);

        s32Ret = CC708_Destroy(pstParam->h708CC);
        s32Ret |= CC608_Destroy(pstParam->h608CC);
    }
    else if(HI_UNF_CC_DATA_TYPE_ARIB == pstParam->enCCType)
    {
        s32Ret = AribCC_Destroy(pstParam->hAribCC);
    }

    free((void *)pstParam);
    pstParam = HI_NULL;

    (HI_VOID)CCDISP_DeInit();


	s_ahCC[u16HandleIndex] = HI_NULL;

    return s32Ret;
}

HI_S32 HI_UNF_CC_Start(HI_HANDLE hCC)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    switch(pstParam->enCCType)
    {
        case HI_UNF_CC_DATA_TYPE_608:
            s32Ret = CC608_Start(pstParam->h608CC);
            break;
        case HI_UNF_CC_DATA_TYPE_708:
            s32Ret = CC708_Start(pstParam->h708CC);
            break;
        case HI_UNF_CC_DATA_TYPE_ARIB:
            s32Ret = AribCC_Start(pstParam->hAribCC);
            break;
        default:
            s32Ret = HI_FAILURE;
            break;
    }

    if(HI_SUCCESS != s32Ret)
    {
        HI_ERR_CC("cc start fail\n");
        return HI_FAILURE;
    }

    s_bCCStartFlag = HI_TRUE;

    if (pstParam->enCCType == HI_UNF_CC_DATA_TYPE_608
        || pstParam->enCCType == HI_UNF_CC_DATA_TYPE_708)
    {
        StartCCDataThread(pstParam);
    }

    return HI_SUCCESS;
}

HI_S32 HI_UNF_CC_Stop(HI_HANDLE hCC)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    DEC_TRACE("HI_UNF_CC_Stop, begin, enCCType = 0x%x!\n", pstParam->enCCType);

    switch(pstParam->enCCType)
    {
        case HI_UNF_CC_DATA_TYPE_608:
            s32Ret = CC608_Stop(pstParam->h608CC);
            break;
        case HI_UNF_CC_DATA_TYPE_708:
            s32Ret = CC708_Stop(pstParam->h708CC);
            break;
        case HI_UNF_CC_DATA_TYPE_ARIB:
            s32Ret = AribCC_Stop(pstParam->hAribCC);
            break;
        default:
            s32Ret = HI_FAILURE;
            break;
    }

    if(HI_SUCCESS != s32Ret)
    {
        DEC_TRACE("cc stop fail\n");
    }

    s_bCCStartFlag = HI_FALSE;

    usleep(100*1000);

    DEC_TRACE("HI_UNF_CC_Stop, end!\n");


    return HI_SUCCESS;
}

HI_S32 HI_UNF_CC_Reset(HI_HANDLE hCC)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if(HI_UNF_CC_DATA_TYPE_608 == pstParam->enCCType
        || HI_UNF_CC_DATA_TYPE_708 == pstParam->enCCType)
    {
        s32Ret = CC708_Reset(pstParam->h708CC);/*user data in 708 module*/
        s32Ret |= CC608_Reset(pstParam->h608CC);
    }
    else if(HI_UNF_CC_DATA_TYPE_ARIB == pstParam->enCCType)
    {
        s32Ret = AribCC_Reset(pstParam->hAribCC);
    }
    else
    {
        s32Ret = HI_FAILURE;
    }

    return s32Ret;

}

HI_S32 HI_UNF_CC_InjectUserData(HI_HANDLE hCC, HI_UNF_CC_USERDATA_S *pstUserData)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if (HI_NULL == pstUserData)
    {
        HI_ERR_CC("user data is null !\n");
        return HI_FAILURE;
    }

    if(HI_FALSE == s_bCCStartFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = CC708_Userdata_Inject(pstParam->h708CC, pstUserData->pu8userdata, pstUserData->u32dataLen, pstUserData->bTopFieldFirst);

    return s32Ret;
}

HI_S32 HI_UNF_CC_InjectPESData(HI_HANDLE hCC, HI_U8 *pu8PesData, HI_U32 u32DataLen)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if((HI_NULL == pu8PesData) || (0 == u32DataLen))
    {
        HI_ERR_CC("Invalid pes data !\n");
        return HI_FAILURE;
    }

    if(HI_FALSE == s_bCCStartFlag)
    {
        return HI_SUCCESS;
    }

    s32Ret = AribCC_InjectPESData(pstParam->hAribCC, pu8PesData, u32DataLen);

    return s32Ret;
}

HI_S32 HI_UNF_CC_GetAttr(HI_HANDLE hCC, HI_UNF_CC_ATTR_S *pstCCAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if(HI_NULL == pstCCAttr)
    {
        HI_ERR_CC("Invalid Param!\n");
        return HI_FAILURE;
    }

    pstCCAttr->enCCDataType = pstParam->enCCType;
    if(HI_UNF_CC_DATA_TYPE_608 == pstCCAttr->enCCDataType)
    {
        s32Ret = CC608_GetConfig(pstParam->h608CC,&(pstCCAttr->unCCConfig.stCC608ConfigParam));
    }
    else if (HI_UNF_CC_DATA_TYPE_708 == pstCCAttr->enCCDataType)
    {
        s32Ret = CC708_GetConfig(pstParam->h708CC,&(pstCCAttr->unCCConfig.stCC708ConfigParam));
    }
    else if(HI_UNF_CC_DATA_TYPE_ARIB == pstCCAttr->enCCDataType)
    {
        s32Ret = AribCC_GetConfig(pstParam->hAribCC,&(pstCCAttr->unCCConfig.stCCARIBConfigParam));
    }

    return s32Ret;
}

HI_S32 HI_UNF_CC_SetAttr(HI_HANDLE hCC, HI_UNF_CC_ATTR_S *pstCCAttr)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if(HI_NULL == pstCCAttr)
    {
        HI_ERR_CC("Invalid Param!\n");
        return HI_FAILURE;
    }

    if(pstCCAttr->enCCDataType != pstParam->enCCType)
    {
        HI_INFO_CC("Data type invalid\n");
        return HI_FAILURE;
    }

    switch(pstCCAttr->enCCDataType)
    {
        case HI_UNF_CC_DATA_TYPE_608:
            s32Ret = CC608_Config(pstParam->h608CC,&(pstCCAttr->unCCConfig.stCC608ConfigParam));
            break;

        case HI_UNF_CC_DATA_TYPE_708:
            s32Ret = CC708_Config(pstParam->h708CC,&(pstCCAttr->unCCConfig.stCC708ConfigParam));
            break;

        case HI_UNF_CC_DATA_TYPE_ARIB:
            //TODU:we do not support config buffer size dynamically
            break;
        default:
            break;
    }

    return s32Ret;
}

HI_S32 HI_UNF_CC_GetARIBCCInfo(HI_HANDLE hCC,HI_UNF_CC_ARIB_INFO_S *pstCCAribInfo)
{
    HI_S32 s32Ret = HI_SUCCESS;
    CC_LOCAL_PARAM_S *pstParam = (CC_LOCAL_PARAM_S *)CC_GetParamAddr(hCC);

    CheckCCHandle(pstParam);

    if(HI_NULL == pstCCAribInfo)
    {
        HI_ERR_CC("Invalid Param!\n");
        return HI_FAILURE;
    }

    if(HI_UNF_CC_DATA_TYPE_ARIB != pstParam->enCCType)
    {
        HI_ERR_CC("Invalid CC type!\n");
        return HI_FAILURE;
    }

   memset(pstCCAribInfo,0,sizeof(HI_UNF_CC_ARIB_INFO_S));
   s32Ret=AribCC_GetCaptionInfo(pstParam->hAribCC,pstCCAribInfo);

    return s32Ret;
}
