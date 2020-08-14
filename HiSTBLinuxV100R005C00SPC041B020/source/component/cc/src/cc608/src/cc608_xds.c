

/*******************************************************************************
*                       Include files
*******************************************************************************/

#include <string.h>
#include <stdio.h>

#include "cc608_xds.h"
#include "hi_cc608_def.h"
#include "cc608.h"
#include "cc608_obj.h"
#include "cc_debug.h"
#include "cc_timer.h"


/*****************************************************************************
*                       Macro Definitions
*****************************************************************************/
#define CCXDS_MUTEX_LOCK(x)  \
    do{\
        int ret = pthread_mutex_lock(x);\
        if(ret != 0){\
            HI_ERR_CC("CC call pthread_mutex_lock(XDS) failure,ret = 0x%x\n",ret);\
        }\
    }while(0)

#define CCXDS_MUTEX_UNLOCK(x)  \
    do{\
        int ret = pthread_mutex_unlock(x);\
        if(ret != 0){\
            HI_ERR_CC("CC call pthread_mutex_unlock(XDS) failure, ret = 0x%x\n",ret);\
        }\
    }while(0)

#define CCXDS_MUTEX_INIT(x)  \
    do{ \
        int ret = pthread_mutex_init(x,NULL); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_init(XDS) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)
    
#define CCXDS_MUTEX_DESTORY(x)  \
    do{ \
        int ret = pthread_mutex_destroy(x); \
        if(ret != 0){ \
            HI_ERR_CC("CC call pthread_mutex_destroy(XDS) failure,ret = 0x%x\n",ret); \
        } \
    }while(0)

#define CHECK_XDS_PACKET_CURFUT_CLASS(enXDSClass) \
if((enXDSClass != HI_UNF_CC_XDS_CLASS_CUR) && (enXDSClass != HI_UNF_CC_XDS_CLASS_FUT)) \
{ \
    HI_ERR_CC("Invalid packet class:%d\n",enXDSClass); \
    return HI_FAILURE; \
}
#define CHECK_XDS_PACKET_CLASS(enXDSClass, eCorrectClass) \
if(enXDSClass != eCorrectClass) \
{ \
   HI_ERR_CC("Invalid packet class:%d\n",enXDSClass); \
   return HI_FAILURE; \
}
/*****************************************************************************
*                       Type Definitions
*****************************************************************************/

/*******************************************************************************
*                       Extern Data Definition
*******************************************************************************/

/*******************************************************************************
*                       Static Data Definition
*******************************************************************************/

static XDS_OBJECT_S s_stXDSObject[MAX_XDS_OBJECT_NUM];

static XDS_OBJECT_S *_CC608_XDS_GetObject(HI_U8 u8ModuleID)
{
    if( u8ModuleID >= MAX_XDS_OBJECT_NUM )
    {
        return HI_NULL;
    }
    return &(s_stXDSObject[u8ModuleID]);
}

/*******************************************************************************
*                       Static Function Definition
*******************************************************************************/

static void _CC608_XDS_EndXDSPacket(HI_U8 u8ModuleID)
{
    XDS_OBJECT_S *pXDSObject = _CC608_XDS_GetObject(u8ModuleID);
    HI_UNF_CC_XDS_CLASS_E enXDSClass;
    HI_U8 u8XDSType;

    if (HI_NULL == pXDSObject)
    {
        return;
    }
    enXDSClass = pXDSObject->enCurXDSClass;
    u8XDSType = pXDSObject->u8CurXDSType;

    if((enXDSClass >= HI_UNF_CC_XDS_CLASS_BUTT) || (u8XDSType >= CC608_XDS_MAX_TYPE))
    {
        HI_WARN_CC("Invalid XDS packet class %d or type %d.\n",enXDSClass,u8XDSType);
        return;
    }

    XDS_TRACE("\nEnd XDS packet class %d, type %d, data len %d\n",enXDSClass,u8XDSType,pXDSObject->u8PacketLen);

    if( (pXDSObject->u8PacketLen > 0) && (pXDSObject->u8IsPacketStarted == 1))
    {
        (HI_VOID)CCDISP_OutputXDSData((HI_U8)enXDSClass,u8XDSType,pXDSObject->au8PacketData,pXDSObject->u8PacketLen);
    }

    if( (pXDSObject->u8PacketLen > 0) && (pXDSObject->u8IsPacketStarted == 1))
    {
        pXDSObject->bUpdated[enXDSClass][u8XDSType] = HI_TRUE;
    }
    return;
}

/*******************************************************************************
*                       Export Function Definition
*******************************************************************************/

HI_S32 CC608_XDS_Create(HI_U8 u8ModuleID)
{
    XDS_OBJECT_S *pstObject = _CC608_XDS_GetObject(u8ModuleID);
    HI_S32 i,j;

    if(HI_NULL == pstObject)
    {
        return HI_FAILURE;
    }
    // initial XDS mutex
    CCXDS_MUTEX_INIT(&pstObject->XDSMutex);

    for(i=0; i<HI_UNF_CC_XDS_CLASS_BUTT; i++)
    {
        for(j=0; j<CC608_XDS_MAX_TYPE; j++)
        {
            pstObject->bUpdated[i][j] = HI_FALSE;
        }
    }

    return HI_SUCCESS;
}

HI_S32 CC608_XDS_Destroy(HI_U8 u8ModuleID)
{
    XDS_OBJECT_S *pstObject = _CC608_XDS_GetObject(u8ModuleID);

    if (HI_NULL != pstObject)
    {
        CCXDS_MUTEX_DESTORY(&pstObject->XDSMutex);
    }
    return HI_SUCCESS;
}

void CC608_XDS_Reset(HI_U8 u8ModuleID)
{
    XDS_OBJECT_S *pstObject = _CC608_XDS_GetObject(u8ModuleID);
    HI_U8 i,j;

    if(HI_NULL == pstObject)
    {
        return;
    }
    CCXDS_MUTEX_LOCK(&pstObject->XDSMutex);

    for(i = 0; i < HI_UNF_CC_XDS_CLASS_BUTT; i++)
    {
        for(j = 0; j< CC608_XDS_MAX_TYPE; j++)
        {
            pstObject->bUpdated[i][j] = HI_FALSE;
        }
    }

    CCXDS_MUTEX_UNLOCK(&pstObject->XDSMutex);
}

HI_S32 CC608_XDS_XDSDecode(HI_U8 u8ModuleID, HI_U8 b1, HI_U8 b2)
{
    XDS_OBJECT_S *pstObject = _CC608_XDS_GetObject(u8ModuleID);
    HI_UNF_CC_XDS_CLASS_E eXDSClassTable[] =
    {
        HI_UNF_CC_XDS_CLASS_CUR,  HI_UNF_CC_XDS_CLASS_FUT,  HI_UNF_CC_XDS_CLASS_CHAN, HI_UNF_CC_XDS_CLASS_MISC,
        HI_UNF_CC_XDS_CLASS_PUB,  HI_UNF_CC_XDS_CLASS_RESV, HI_UNF_CC_XDS_CLASS_PRIV,
    };

    if (HI_NULL == pstObject)
    {
        return HI_FAILURE;
    }

    XDS_TRACE("XDS:code pair %02x  %02x\n",b1,b2);
    if(b1 == CLASS_PACKET_END)
    {
        _CC608_XDS_EndXDSPacket(u8ModuleID);
        pstObject->u8IsPacketStarted = 0;
        pstObject->u8CurXDSType = 0;
        pstObject->u8PacketLen = 0;
        memset( pstObject->au8PacketData, 0, MAX_XDS_PACKET_LEN );

        if(pstObject->u8IsBackupStarted)
        {
            memcpy(pstObject->au8PacketData,pstObject->au8BackupPacketData,pstObject->u8BackupPacketLen);
            pstObject->u8PacketLen = pstObject->u8BackupPacketLen;
            pstObject->enCurXDSClass = pstObject->enBackupXDSClass;
            pstObject->u8CurXDSType = pstObject->u8BackupXDSType;

            XDS_TRACE("Interleaved XDS packet!Recover first XDS packet from backup buffer!"
                    "pstObject->u8PacketLen:%d,Class:%d,Type:%d\n",
                    pstObject->u8PacketLen,pstObject->enCurXDSClass,pstObject->u8CurXDSType);

            memset(pstObject->au8BackupPacketData, 0, MAX_XDS_PACKET_LEN);
            pstObject->u8BackupPacketLen = 0;
            pstObject->u8IsBackupStarted = 0;
            pstObject->u8IsPacketStarted = 1;
        }
    }
    else if((b1 >= CLASS_CURRENT_START) && (b1 <= CLASS_PRIVATE_CONTINUE))
    {
        // if b1 is a start code
        if(b1 & 0x1)
        {
            if(pstObject->u8IsPacketStarted)
            {
                XDS_TRACE("Interleaved XDS packet!Save first one to backup buffer!"
                        "pstObject->u8PacketLen:%d,Class:%d,Type:%d\n",
                        pstObject->u8PacketLen,pstObject->enCurXDSClass,pstObject->u8CurXDSType);
                memcpy(pstObject->au8BackupPacketData,pstObject->au8PacketData,pstObject->u8PacketLen);
                pstObject->u8BackupPacketLen = pstObject->u8PacketLen;
                pstObject->enBackupXDSClass = pstObject->enCurXDSClass;
                pstObject->u8BackupXDSType = pstObject->u8CurXDSType;
                pstObject->u8IsBackupStarted = 1;
            }
            pstObject->u8PacketLen = 0;
            memset(pstObject->au8PacketData, 0, MAX_XDS_PACKET_LEN);

            XDS_TRACE("Packet starts!\n");
        }

        pstObject->u8IsPacketStarted  = 1;
        pstObject->enCurXDSClass       = eXDSClassTable[(b1-1)>>1];
        pstObject->u8CurXDSType        = b2;
    }
    else if( pstObject->u8IsPacketStarted == 1 ) /*XDS data*/
    {
        if((b1>0 && b1<=0x1f) ||(b2>0 && b2<=0x1f) )
        {
            HI_WARN_CC("Illegal xds data (%#x %#x)! should be 00,or 0x20-0x7F\n",b1,b2);
            return HI_FAILURE;
        }
        if( pstObject->u8PacketLen < MAX_XDS_PACKET_LEN - 2)
        {
            XDS_TRACE("xds packet len:%d\n",pstObject->u8PacketLen);
            pstObject->au8PacketData[pstObject->u8PacketLen++] = b1;
            pstObject->au8PacketData[pstObject->u8PacketLen++] = b2;
        }
        else
        {
            /*over flow, reset the packet*/
            pstObject->u8PacketLen = 0;
            memset(pstObject->au8PacketData, 0, MAX_XDS_PACKET_LEN);
            pstObject->u8IsPacketStarted = 0;
            HI_ERR_CC("Error!!!!!!  xds packet overflow!\n");
        }
    }

    return HI_SUCCESS;
}

