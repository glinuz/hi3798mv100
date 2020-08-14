#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#if defined (ANDROID)
#include <sys/syscall.h>
#define msgget(key, msgflg)                     syscall(__NR_msgget,key,msgflg)
#define msgctl(msqid,cmd,buf)                   syscall(__NR_msgctl,msqid,cmd, buf)
#define msgsnd(msqid,msgp,msgsz,msgflg)         syscall(__NR_msgsnd,msqid,msgp,msgsz,msgflg)
#define msgrcv(msqid,msgp,msgsz,msgtyp,msgflg)  syscall(__NR_msgrcv,msqid,msgp,msgsz,msgtyp,msgflg)
#else
#include <sys/msg.h>
#endif

#include "cc_debug.h"

#include "aribcc_decode.h"
#include "aribcc_disp.h"
#include "aribcc_osd.h"
#include "aribcc_common.h"
#include "aribcc_utils.h"

#define CCDEC_MSG_KEY   101
#define CCDEC_MSG_KEY2  102
#define CCDISP_MSG_KEY  103
#define CCDISP_MSG_KEY2 104



//**********************************************************************************************************************

/* pes data manager */
typedef struct  tagARIBCC_PesData_S
{
    HI_U8                   *pu8Ptr;
    HI_U32                  u32Size;
} ARIBCC_PesData_S;

typedef struct  tagARIBCC_Pes_S
{
    HI_U8                   *pu8Area;
    HI_U32                  u32AreaSize;
    HI_U16                  u16Rp;
    HI_U16                  u16Wp;
    ARIBCC_PesData_S        stPesData[ARIBCC_PESDATA_BLOCK];
} ARIBCC_Pes_S;

typedef struct tagARIBCC_Msgbuf_S
{
    HI_S32 s32Type;
    HI_U8  au8Data[ARIBCC_MSGBUFF_SIZE];
} ARIBCC_Msgbuf_S;

typedef struct  tagARIBCC_TimerData_S
{
    HI_S32                 s32MsgType;
    ARIBCC_Management_S    *pstManagement;
    ARIBCC_UnitGroup_S     *pstUnitGroup;
    struct timeval          stTimeValue;
} ARIBCC_TimerData_S;

typedef struct  tagARIBCC_Timer_S
{
    volatile HI_U16         u16Rp;
    volatile HI_U16         u16Wp;
    ARIBCC_TimerData_S      astTimerData[ARIBCC_PESDATA_BLOCK];
} ARIBCC_Timer_S;


static ARIBCC_Pes_S gAribCcPes[CCPES_TYPE_MAX];
static ARIBCC_Timer_S gAribCcTimer[CCPES_TYPE_MAX];
static struct timeval s_astDispTimeValue[CCPES_TYPE_MAX];

//ZWY@20160715 DTS2016071104163
#define ARIBCC_MSGDATA_SIZE 520 //在ARIBCC_MSGBUFF_SIZE基础上，还要额外增加4字节传递消息类型，干脆520吧
#define ARIBCC_MSGQUEUE_SIZE_INIT 10240 //消息队列初始化大小，10K足够了，取消息的循环执行得很快
static HI_U8  s_aribcc_msgdata_send[ARIBCC_MSGDATA_SIZE];
static HI_U8  s_aribcc_msgdata_rcv[ARIBCC_MSGDATA_SIZE];
static HI_U8* s_aribcc_msgqueue_baseaddr = NULL; //存消息的Buff基址
static ARIBCC_SEGMENT_QUEUE_S s_aribcc_msgqueue_handle; //消息队列Handle

HI_S32 ARIBCC_Com_Init (void)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = ARIBCC_DEC_Init (ARIB_CAP_CAPTION);

    //it seems SBTVD hasn't SUPERIMPOSE
    //s32Ret = ARIBCC_DEC_Init (ARIB_CAP_SUPERIMPOSE);

    memset (gAribCcPes, 0, sizeof(gAribCcPes));
    memset (gAribCcTimer, 0, sizeof(gAribCcTimer));
    memset (s_astDispTimeValue, 0, sizeof(s_astDispTimeValue));

    s32Ret |= ARIBCC_Com_MessageInit();

    return s32Ret;
}

HI_S32 ARIBCC_Com_DeInit (void)
{
    HI_S32 s32Ret = HI_SUCCESS;

    s32Ret = ARIBCC_DEC_DeInit (ARIB_CAP_CAPTION);
    s32Ret |= ARIBCC_DEC_DeInit (ARIB_CAP_SUPERIMPOSE);

    (HI_VOID)ARIBCC_Com_MessageDeInit();

    return s32Ret;
}

HI_S32 ARIBCC_Com_InitPESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U32 u32Size)
{
    HI_U8              *pu8Area = HI_NULL;
    ARIBCC_Pes_S       *pstPes = HI_NULL;
    ARIBCC_PesData_S   *pstPesData = HI_NULL;

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (u32Size == 0))
    {
        HI_ERR_CC("param error, enCaptionType : %d, u32Size : %d\n", enCaptionType, u32Size);
        return HI_FAILURE;
    }

    pstPes = gAribCcPes + enCaptionType;
    if (pstPes->pu8Area)
    {
        HI_WARN_CC("area is init already!\n");
        return HI_SUCCESS;
    }

    pu8Area = malloc (u32Size);
    if (pu8Area == NULL)
    {
        HI_ERR_CC("failed to malloc %d\n", u32Size);
        return HI_FAILURE;
    }

    pstPes->pu8Area     = pu8Area;
    pstPes->u32AreaSize = u32Size;
    pstPes->u16Rp       = 0;
    pstPes->u16Wp       = 0;
    pstPesData        = pstPes->stPesData;
    memset (pstPesData, 0, sizeof(pstPes->stPesData));
    pstPesData[0].pu8Ptr = pu8Area;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_ClearPESPtr (ARIBCC_CaptionType_E enCaptionType)
{
    HI_U8              *pu8Area = HI_NULL;
    ARIBCC_Pes_S       *pstPes = HI_NULL;
    ARIBCC_PesData_S   *pstPesData = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("param error, enCaptionType : %d\n", enCaptionType);
        return HI_FAILURE;
    }

    pstPes     = gAribCcPes + enCaptionType;
    pu8Area    = pstPes->pu8Area;
    pstPesData = pstPes->stPesData;

    if (pstPes->pu8Area == HI_NULL)
    {
        HI_ERR_CC("area is invalid!\n");
        return HI_FAILURE;
    }

    pstPes->u16Rp = 0;
    pstPes->u16Wp = 0;
    memset (pstPesData, 0, sizeof(pstPes->stPesData));
    pstPesData[0].pu8Ptr = pu8Area;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_AllocPESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U8 **ppu8CcPesPtr, HI_U32 u32CcPesSize)
{
    HI_U32             u32AreaSize = 0;
    HI_U16             u16Rp = 0, u16Wp= 0, u16NextWp = 0;
    HI_U8              *pu8Area = HI_NULL, *pu8RPtr = HI_NULL;
    HI_U8              *pu8WPtr = HI_NULL, *pu8NextWPtr = HI_NULL;
    ARIBCC_Pes_S       *pstPes = HI_NULL;
    ARIBCC_PesData_S   *pstPesData = HI_NULL;

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (ppu8CcPesPtr == HI_NULL)
        || (u32CcPesSize == 0))
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    *ppu8CcPesPtr = HI_NULL;

    pstPes      = gAribCcPes + enCaptionType;
    pstPesData  = pstPes->stPesData;

    pu8Area     = pstPes->pu8Area;
    u32AreaSize = pstPes->u32AreaSize;
    u16Rp       = pstPes->u16Rp;
    u16Wp       = pstPes->u16Wp;
    u16NextWp   = (u16Wp + 1) % (sizeof(pstPes->stPesData) / sizeof(ARIBCC_PesData_S));

    if (pstPes->pu8Area == HI_NULL)
    {
        HI_ERR_CC("area is invalid!\n");
        return HI_FAILURE;
    }
    else if (u16NextWp == u16Rp)
    {
        HI_ERR_CC("area is full!\n");
        return HI_FAILURE;
    }

    u32CcPesSize = (u32CcPesSize + 3) & (~3);

    pu8RPtr     = pstPesData[u16Rp].pu8Ptr;
    pu8WPtr     = pstPesData[u16Wp].pu8Ptr;
    pu8NextWPtr = pu8WPtr + u32CcPesSize;
    if ((pu8WPtr < pu8RPtr) && (pu8NextWPtr >= pu8RPtr))
    {
        HI_ERR_CC("area is full!\n");
        return HI_FAILURE;
    }
    if (pu8NextWPtr > (pu8Area + u32AreaSize))
    {
        pu8WPtr     = pu8Area;
        pu8NextWPtr = pu8WPtr + u32CcPesSize;
        if (pu8NextWPtr >= pu8RPtr)
        {
            HI_ERR_CC("area is full!\n");
            return HI_FAILURE;
        }
    }

    pstPes->u16Wp             = u16NextWp;
    pstPesData[u16Wp].pu8Ptr     = pu8WPtr;
    pstPesData[u16Wp].u32Size    = u32CcPesSize;
    pstPesData[u16NextWp].pu8Ptr = pu8NextWPtr;

    *ppu8CcPesPtr = pu8WPtr;

    HI_INFO_CC("AllocPESPtr (%s, %p, 0x%8X)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                    *ppu8CcPesPtr, u32CcPesSize);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_FreePESPtr (ARIBCC_CaptionType_E enCaptionType, HI_U8 *pu8CcPesPtr)
{
    HI_U16             u16Size = 0, u16Rp = 0, u16Wp = 0;
    HI_U16             u16FreeRp = 0, u16NextRp = 0;
    ARIBCC_Pes_S       *pstPes = HI_NULL;
    ARIBCC_PesData_S   *pstPesData = HI_NULL;

    HI_INFO_CC("FreePESPtr (%s, %p)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"), pu8CcPesPtr);

    if (((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || (pu8CcPesPtr == HI_NULL))
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    pstPes     = gAribCcPes + enCaptionType;
    pstPesData = pstPes->stPesData;
    u16Size    = sizeof(pstPes->stPesData);

    u16Rp      = pstPes->u16Rp;
    u16Wp      = pstPes->u16Wp;

    if (pstPes->pu8Area == HI_NULL)
    {
        HI_ERR_CC("area is invalid !\n");
        return HI_FAILURE;
    }
    else if (u16Rp == u16Wp)
    {
        HI_ERR_CC("area is empty !\n");
        return HI_FAILURE;
    }

    for (u16FreeRp = u16Rp; u16FreeRp != u16Wp; u16FreeRp = ((u16FreeRp + 1) % (u16Size / sizeof(ARIBCC_PesData_S))))
    {
        if (pstPesData[u16FreeRp].pu8Ptr == pu8CcPesPtr)
        {
            break;
        }
    }
    if (u16FreeRp == u16Wp)
    {
        HI_ERR_CC("can't free ptr !\n");
        return HI_FAILURE;
    }

    for (u16NextRp = u16Rp; u16NextRp != u16Wp; u16NextRp = ((u16NextRp + 1) % (u16Size / sizeof(ARIBCC_PesData_S))))
    {
        if ((pstPesData[u16NextRp].pu8Ptr != HI_NULL) && (u16NextRp != u16FreeRp))
        {
            break;
        }
    }

    pstPes->u16Rp             = u16NextRp;
    pstPesData[u16FreeRp].pu8Ptr = HI_NULL;

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_DeInitPESPtr (ARIBCC_CaptionType_E enCaptionType)
{
    HI_U16             u16Size = 0;
    HI_U8              *pu8Area = HI_NULL;
    ARIBCC_Pes_S       *pstPes = HI_NULL;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("param error\n");
        return HI_FAILURE;
    }

    pstPes     = gAribCcPes + enCaptionType;
    u16Size    = sizeof(pstPes->stPesData);

    pu8Area    = pstPes->pu8Area;

    if (pstPes->pu8Area == HI_NULL)
    {
        HI_ERR_CC("area is avalid!\n");
        return HI_FAILURE;
    }

    free (pu8Area);
    pu8Area = HI_NULL;
    pstPes->pu8Area     = HI_NULL;
    pstPes->u32AreaSize = 0;
    pstPes->u16Rp       = 0;
    pstPes->u16Wp       = 0;
    memset ( pstPes->stPesData, 0, u16Size);

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_ClearTimer (ARIBCC_CaptionType_E enCaptionType)
{
    HI_U16             u16Rp = 0, u16Wp = 0;
    HI_U16             u16Size = 0;
    ARIBCC_Timer_S     *pstTimer = HI_NULL;

    HI_INFO_CC("ClearTimer (CaptionType = %s)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"));

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    pstTimer     = gAribCcTimer + enCaptionType;
    u16Rp        = pstTimer->u16Rp;
    u16Wp        = pstTimer->u16Wp;
    u16Size      = sizeof(pstTimer->astTimerData);

    if ((u16Wp != u16Rp) && (( pstTimer->astTimerData[u16Rp].stTimeValue.tv_sec == 0) && ( pstTimer->astTimerData[u16Rp].stTimeValue.tv_usec== 0)))
    {
        s_astDispTimeValue[enCaptionType].tv_sec  = 0;
        s_astDispTimeValue[enCaptionType].tv_usec = 0;
    }

    pstTimer->u16Rp = 0;
    pstTimer->u16Wp = 0;
    memset (pstTimer->astTimerData, 0, u16Size);

    return HI_SUCCESS;
}


HI_S32 _ARIBCC_Com_CreateTimer (ARIBCC_CaptionType_E enCaptionType)
{
    HI_U16             u16Rp = 0, u16Wp = 0;
    ARIBCC_Timer_S     *pstTimer = HI_NULL;
    ARIBCC_TimerData_S *pstTimerData = HI_NULL;
    struct timeval      TimeValue;
    struct timezone     TimeZone;
    HI_U64              u64SystemTime = 0, u64PresentTime = 0;
    HI_S32              s32Ret = HI_FAILURE;

    HI_INFO_CC("CreateTimer (CaptionType=%s)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"));

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    pstTimer     = gAribCcTimer + enCaptionType;
    u16Rp        = pstTimer->u16Rp;
    u16Wp        = pstTimer->u16Wp;
    pstTimerData = pstTimer->astTimerData;

    if (u16Wp == u16Rp)
    {
        HI_ERR_CC("Invalid data !\n");
        return HI_FAILURE;
    }

    u16Rp = pstTimer->u16Rp;

    if ((pstTimerData[u16Rp].stTimeValue.tv_sec == 0) && (pstTimerData[u16Rp].stTimeValue.tv_usec == 0))
    {
        HI_INFO_CC("Timer Already exists !\n");
        return HI_SUCCESS;
    }

    s32Ret = gettimeofday (&TimeValue, &TimeZone);
    if (s32Ret == -1)
    {
        HI_ERR_CC("failed to gettimeofday !\n");
        return HI_FAILURE;
    }

    u64SystemTime  = (HI_U64)(((HI_U64)TimeValue.tv_sec * 1000) + ((HI_U64)TimeValue.tv_usec / 1000));
    u64PresentTime = (HI_U64)(((HI_U64)pstTimerData[u16Rp].stTimeValue.tv_sec * 1000) + ((HI_U64)pstTimerData[u16Rp].stTimeValue.tv_usec / 1000));
    if (u64PresentTime < u64SystemTime + 100)
    {
        s_astDispTimeValue[enCaptionType] = TimeValue;
    }
    else
    {
        s_astDispTimeValue[enCaptionType] = pstTimerData[u16Rp].stTimeValue;
    }

    pstTimerData[u16Rp].stTimeValue.tv_sec  = 0;
    pstTimerData[u16Rp].stTimeValue.tv_usec = 0;

    return HI_SUCCESS;
}

HI_S32 _ARIBCC_Com_NotifyTimer (ARIBCC_CaptionType_E enCaptionType)
{
    HI_U16             u16Rp = 0, u16Wp = 0, u16NextRp = 0, u16Size = 0;
    ARIBCC_Timer_S     *pstTimer = HI_NULL;
    ARIBCC_TimerData_S *pstTimerData = HI_NULL;
    ARIBCC_ManagementUnit_S  stMsgData;
    HI_S32             s32Ret = HI_SUCCESS;

    HI_INFO_CC("NotifyTimer (CaptionType=%s) Called!\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"));

    pstTimer     = gAribCcTimer + enCaptionType;
    u16Rp        = pstTimer->u16Rp;
    u16Wp        = pstTimer->u16Wp;
    pstTimerData = pstTimer->astTimerData;
    u16Size      = sizeof(pstTimer->astTimerData);

    if (u16Wp == u16Rp)
    {
        HI_ERR_CC("timer empty\n");
        return HI_FAILURE;
    }

    if (pstTimerData[u16Rp].s32MsgType == MSGTYPE_CC_DISP_PARSE)
    {
        stMsgData.pstManagement = pstTimerData[u16Rp].pstManagement;
        stMsgData.pstUnitGroup  = pstTimerData[u16Rp].pstUnitGroup;

        HI_INFO_CC("ARIBCC_Com_MessageSend (CC_QID, MSGTYPE_CC_DISP_PARSE, %p, %d)\n",
                        &stMsgData, sizeof(stMsgData));

        s32Ret = ARIBCC_Com_MessageSend (enCaptionType, pstTimerData[u16Rp].s32MsgType, (HI_U8*) &stMsgData, sizeof(stMsgData));
    }
    else
    {
        HI_INFO_CC("ARIBCC_Com_MessageSend MSGTYPE_CC_DISP_CLEAR\n");
        s32Ret = ARIBCC_Com_MessageSend (enCaptionType, pstTimerData[u16Rp].s32MsgType, HI_NULL, 0);
    }

    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to ARIBCC_Com_MessageSend\n");
        return HI_FAILURE;
    }

    u16NextRp     = (u16Rp + 1) % (u16Size / sizeof(ARIBCC_TimerData_S));
    pstTimer->u16Rp = u16NextRp;

    u16Wp        = pstTimer->u16Wp;

    if (u16Wp == u16NextRp)
    {
        HI_INFO_CC("timer is empty\n");
        return HI_SUCCESS;
    }

    s32Ret = _ARIBCC_Com_CreateTimer (enCaptionType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_Com_CreateTimer !\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_RegistTimer (ARIBCC_CaptionType_E enCaptionType, HI_S32 s32MsgType, HI_VOID *pvManagement,
                               HI_VOID *pvUnitGroup, struct timeval *pstTimeValue)
{
    HI_S32             s32Ret = HI_SUCCESS;
    HI_U16             u16Rp = 0, u16Wp = 0, u16NextWp = 0, u16Size = 0;
    ARIBCC_Timer_S     *pstTimer = HI_NULL;
    ARIBCC_TimerData_S *pstTimerData = HI_NULL;

    if ((pstTimeValue == HI_NULL)
        || ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
        || ((s32MsgType != MSGTYPE_CC_DISP_PARSE) && (s32MsgType != MSGTYPE_CC_DISP_CLEAR))
        || ((s32MsgType == MSGTYPE_CC_DISP_PARSE) && (pvUnitGroup == HI_NULL)))
    {
        HI_ERR_CC("parameter error!\n");
        HI_ERR_CC("enCaptionType : %d, MsgType : %d\n", enCaptionType, s32MsgType);
        HI_ERR_CC("pstManagement : %#x, pstUnitGroup : %#x\n", pvManagement, pvUnitGroup);
        return HI_FAILURE;
    }

    HI_INFO_CC("RegistTimer (CaptionType=%s, MsgType=%s, "
                   "Management=%p, UnitGroup=%p, TimeValue=%ld.%6ld)\n",
                   (enCaptionType==ARIB_CAP_CAPTION?"CAPTION":"SUPERIMPOSE"),
                   (s32MsgType==MSGTYPE_CC_DISP_PARSE?"DISP":"CLEAR"),
                    pvManagement,  pvUnitGroup,
                   pstTimeValue->tv_sec, pstTimeValue->tv_usec);

    pstTimer     = gAribCcTimer + enCaptionType;
    u16Rp        = pstTimer->u16Rp;
    u16Wp        = pstTimer->u16Wp;
    pstTimerData = pstTimer->astTimerData;
    u16Size      = sizeof(pstTimer->astTimerData);
    u16NextWp    = (u16Wp + 1) % (u16Size / sizeof(ARIBCC_TimerData_S));

    if (u16NextWp == u16Rp)
    {
        HI_ERR_CC("Buffer Too Small !\n");
        return HI_FAILURE;
    }

    pstTimerData[u16Wp].s32MsgType    = s32MsgType;
    pstTimerData[u16Wp].pstManagement = (ARIBCC_Management_S *)pvManagement;
    pstTimerData[u16Wp].pstUnitGroup  = (ARIBCC_UnitGroup_S *)pvUnitGroup;
    pstTimerData[u16Wp].stTimeValue  = *pstTimeValue;

    pstTimer->u16Wp = u16NextWp;

    s32Ret = _ARIBCC_Com_CreateTimer (enCaptionType);
    if (s32Ret != HI_SUCCESS)
    {
        HI_ERR_CC("failed to _ARIBCC_Com_CreateTimer !\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_TimerCheck (ARIBCC_CaptionType_E enCaptionType)
{
    struct timeval  TimeValue;
    struct timezone TimeZone;
    HI_U64 u64SystemTime = 0, u64PresentTime = 0;
    HI_S32 s32Ret = HI_SUCCESS;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }

    if ((s_astDispTimeValue[enCaptionType].tv_sec != 0) || (s_astDispTimeValue[enCaptionType].tv_usec != 0))
    {
        s32Ret = gettimeofday (&TimeValue, &TimeZone);
        if (s32Ret == -1)
        {
            HI_ERR_CC("failed to gettimeofday !\n");
            return HI_FAILURE;
        }

        u64SystemTime  = (HI_U64)(((HI_U64)TimeValue.tv_sec * 1000) + ((HI_U64)TimeValue.tv_usec / 1000));
        u64PresentTime = (HI_U64)(((HI_U64)s_astDispTimeValue[enCaptionType].tv_sec * 1000) + ((HI_U64)s_astDispTimeValue[enCaptionType].tv_usec / 1000));

        if (u64SystemTime >= (u64PresentTime - 200))
        {
            s_astDispTimeValue[enCaptionType].tv_sec  = 0;
            s_astDispTimeValue[enCaptionType].tv_usec = 0;

            s32Ret |= _ARIBCC_Com_NotifyTimer (enCaptionType);

            if (s32Ret != HI_SUCCESS)
            {
                HI_ERR_CC("failed to _ARIBCC_Com_NotifyTimer !\n");
                return HI_FAILURE;
            }
        }
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_MessageInit(HI_VOID)
{
    s_aribcc_msgqueue_baseaddr = (HI_U8 *)malloc(ARIBCC_MSGQUEUE_SIZE_INIT);
    if (s_aribcc_msgqueue_baseaddr == NULL)
    {
        return HI_FAILURE;
    }
    if (ARIBCC_PesQueue_Init(&s_aribcc_msgqueue_handle, s_aribcc_msgqueue_baseaddr, ARIBCC_MSGQUEUE_SIZE_INIT) != HI_SUCCESS)
    {
        HI_ERR_CC("create msg error !\n");
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_MessageDeInit(HI_VOID)
{
    if (s_aribcc_msgqueue_baseaddr != NULL)
    {
        free(s_aribcc_msgqueue_baseaddr);
        s_aribcc_msgqueue_baseaddr = NULL;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_MessageSend(ARIBCC_CaptionType_E enCaptionType, HI_S32 s32Type, HI_U8 *pu8Data, HI_S32 s32Size )
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_S32 s32MsgqID = 0;
    ARIBCC_Msgbuf_S stMsg;
    ARIBCC_SEGMENT_S local_segment;
    
    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }
    
    if( (0 > s32MsgqID)
        || (ARIBCC_MSGBUFF_SIZE < s32Size) )
    {
        HI_ERR_CC("s32MsgqID = %d, Maxbufsize = 512, request size = %d\n", s32MsgqID, s32Size);
        return HI_FAILURE;
    }

    stMsg.s32Type = s32Type;
    if (pu8Data && s32Size > 0)
    {
        memcpy(stMsg.au8Data, pu8Data, (size_t)s32Size );
    }
    else
    {
        memset(stMsg.au8Data, 0, sizeof(stMsg.au8Data));
        s32Size = sizeof(HI_S32);
    }

    memset(s_aribcc_msgdata_send, 0, ARIBCC_MSGDATA_SIZE);
    local_segment.pu8SegmentData = s_aribcc_msgdata_send;
    local_segment.u16DataLength = s32Size + sizeof(stMsg.s32Type);
    memcpy(local_segment.pu8SegmentData, &stMsg, local_segment.u16DataLength);
    
    s32Ret = ARIBCC_PesQueue_En(&s_aribcc_msgqueue_handle, &local_segment);
    if( s32Ret != HI_SUCCESS )
    {
        HI_ERR_CC("failed to msgsnd, s32MsgqID : %d\n", s32MsgqID);
        return HI_FAILURE;
    }

    return HI_SUCCESS;
}

HI_S32 ARIBCC_Com_MessageReceive(ARIBCC_CaptionType_E enCaptionType, HI_S32 *ps32Type, HI_U8 *pu8Data, HI_S32 *ps32Size )
{
    ARIBCC_Msgbuf_S stMsg;
    HI_S32 s32MsgqID = 0;
    ssize_t  rev_size = 0; //没有收到消息，就不要再做多余处理了
    ARIBCC_SEGMENT_S local_segment;

    if ((enCaptionType != ARIB_CAP_CAPTION) && (enCaptionType != ARIB_CAP_SUPERIMPOSE))
    {
        HI_ERR_CC("enCaptionType : %d not support !\n", enCaptionType);
        return HI_FAILURE;
    }
    
    if (HI_NULL == ps32Type || HI_NULL == pu8Data || HI_NULL == ps32Size)
    {
        HI_ERR_CC("param is invalid\n");
        return HI_FAILURE;
    }

    if( *ps32Size < 0 )
    {
        HI_ERR_CC("* ARIBCC_Com_MessageReceive: failed ! (size < 0)\n");
        return HI_FAILURE;
    }

    if( (0 > s32MsgqID) || (ARIBCC_MSGBUFF_SIZE < *ps32Size) )
    {
        HI_ERR_CC("* ARIBCC_Com_MessageReceive: failed ! (s32MsgqID = %d, Maxbufsize = 512, request size = %d)\n", s32MsgqID, *ps32Size);
        return HI_FAILURE;
    }
    
    memset(s_aribcc_msgdata_rcv, 0, ARIBCC_MSGDATA_SIZE);
    local_segment.pu8SegmentData = s_aribcc_msgdata_rcv;
    local_segment.pu8SegmentData = local_segment.pu8SegmentData;
    if (ARIBCC_PesQueue_De(&s_aribcc_msgqueue_handle, &local_segment) == HI_SUCCESS)
    {
        memcpy(&stMsg, local_segment.pu8SegmentData, (size_t)local_segment.u16DataLength);
        rev_size = local_segment.u16DataLength - sizeof(stMsg.s32Type);
    }
    
    if( rev_size < 0 )
    {
        *ps32Type = 0;
        *pu8Data = 0;
        HI_ERR_CC("failed to msgrcv ! (qid=%d)\n",s32MsgqID);
        return HI_FAILURE;
    }
    else if( rev_size > 0 )
    {
        *ps32Type =  stMsg.s32Type;
        memcpy( pu8Data, &stMsg.au8Data[0],(size_t)rev_size );
        *ps32Size = rev_size ;
    }

    return HI_SUCCESS;
}

/******************************************************************************
* Function:            getBits48
* Description:        读取buf中,偏移byte_offset 字节,从startbit位开始的bitlen位的值
* Input:         pBuf :数据指针
                    byte_offset :偏移字节
                    startbit  :开始位
                    bitlen:位长
* Output:
* Return:            从buf中取得的值
* Data Accessed: (Optional)
* Data Updated:  (Optional)
* Others:  (Optional) :该函数最多支持bitlen  <=48位。否则会造成越界
******************************************************************************/
static HI_U64 ARIBCC_GetBits48 (HI_U8 *buf, HI_S32 byte_offset, HI_S32 startbit, HI_S32 bitlen)
{
    HI_U8 *b;
    HI_U64 v;
    HI_U64 mask;
    HI_U64 tmp;

    if (bitlen > 48)
    {
        HI_ERR_CC(" Error: ARIBCC_GetBits48() request out of bound!!!! (report!!) \n");
        return 0xFEFEFEFEFEFEFEFELL;
    }


    if(buf ==(HI_U8*) 0)
    {
        HI_ERR_CC((" \n Error: [ARIBCC_GetBits()] buf is NULL  \n"));
        return  0xFEFEFEFEFEFEFEFELL;
    }
    if (0 == bitlen ) 
    {
        return 0;
    }

    b = &buf[byte_offset + (startbit / 8)];
    startbit %= 8;


    // -- safe is 48 bitlen
    tmp = (HI_U64)(
         ((HI_U64)*(b  )<<48) + ((HI_U64)*(b+1)<<40) +
         ((HI_U64)*(b+2)<<32) + ((HI_U64)*(b+3)<<24) +
         (*(b+4)<<16) + (*(b+5)<< 8) + *(b+6) );

    startbit = 56 - startbit - bitlen;
    tmp      = tmp >> (HI_U32)startbit;
    mask     = (1ULL << bitlen) - 1;	// 1ULL !!!
    v        = tmp & mask;

    return v;
}

/******************************************************************************
* Function:            getBits
* Description:        读取buf中,偏移byte_offset 字节,从startbit位开始的bitlen位的值
* Input:         pBuf :数据指针
                    byte_offset :偏移字节
                    startbit  :开始位
                    bitlen:位长
* Output:
* Return:            从buf中取得的值
* Data Accessed: (Optional)
* Data Updated:  (Optional)
* Others:  (Optional) :该函数最多支持bitlen  <=32位。否则会造成越界
******************************************************************************/
HI_U32 ARIBCC_GetBits (HI_U8 *buf, HI_S32 byte_offset, HI_S32 startbit, HI_S32 bitlen)
{
    HI_U8 *b = HI_NULL;
    HI_U32 v = 0;
    HI_U32 mask = 0;
    HI_U32 tmp_long = 0;
    HI_S32 bitHigh = 0;

    if ( bitlen > 32)
    {
        HI_ERR_CC(" \n Error: [ARIBCC_GetBits()] request out of bound!!!! (report!!) \n");
        return (HI_U32) 0xFEFEFEFE;
    }

    if(buf ==(unsigned char*) 0)
    {
        HI_ERR_CC(" \n Error: [ARIBCC_GetBits()] buf is NULL  \n");
        return (HI_U32) 0xFEFEFEFE;
    }
    if (0  == bitlen ) 
    {
        return 0;
    }

    b = &buf[byte_offset + (startbit >> 3)];
    startbit %= 8;

    switch ((bitlen-1) >> 3)
    {
        case -1:	// -- <=0 bits: always 0
            return 0L;

        case 0:		// -- 1..8 bit
            tmp_long = (HI_U32)(
            (*(b  )<< 8) +  *(b+1) );
            bitHigh = 16;
            break;

        case 1:		// -- 9..16 bit
            tmp_long = (HI_U32)(
            (*(b  )<<16) + (*(b+1)<< 8) +  *(b+2) );
            bitHigh = 24;
            break;

        case 2:		// -- 17..24 bit
            tmp_long = (HI_U32)(
            (*(b  )<<24) + (*(b+1)<<16) +
            (*(b+2)<< 8) +  *(b+3) );
            bitHigh = 32;
            break;

        case 3:		// -- 25..32 bit
            // -- to be safe, we need 32+8 bit as shift range 
            return (HI_U32) ARIBCC_GetBits48 (b, 0, startbit, bitlen);

        default:	// -- 33.. bits: fail, deliver constant fail value
            HI_ERR_CC(" Error: ARIBCC_GetBits() request out of bound!!!! (report!!) \n");
            return (HI_U32) 0xFEFEFEFE;
    }

    startbit = bitHigh - startbit - bitlen;
    tmp_long = tmp_long >> (HI_U32)startbit;
    mask     = (1ULL << bitlen) - 1;  // 1ULL !!!
    v        = tmp_long & mask;

    return v;
}

