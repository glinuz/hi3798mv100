#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <string.h>   /* for NULL */
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "hi_type.h"

#include "hi_unf_demux.h"
#include "hi_unf_avplay.h"
#include "hi_adp.h"
#include "hi_adp_demux.h"
#include "hi_adp_search.h"

static HI_S32 DMX_DataRead(HI_HANDLE hChannel, HI_U32 u32TimeOutms, HI_U8 *pBuf ,
                           HI_U32* pAcquiredNum , HI_U32 * pBuffSize);

HI_S32 DMX_SectionStartDataFilter(HI_U32 u32DmxId, DMX_DATA_FILTER_S * pstDataFilter)
{
    HI_U32 i;
    HI_U8 *p;
    HI_UNF_DMX_CHAN_ATTR_S stChanAttr;
    HI_HANDLE hChan, hFilter = HI_NULL;
    HI_S32 s32Ret;
    HI_UNF_DMX_FILTER_ATTR_S stFilterAttr;
    HI_U32 u32AquiredNum = 0;
    HI_U8 u8DataBuf[MAX_SECTION_LEN * MAX_SECTION_NUM];
    HI_U32 u32BufSize[MAX_SECTION_NUM];

    HI_UNF_DMX_GetChannelDefaultAttr(&stChanAttr);

    stChanAttr.u32BufSize    = 16 * 1024;
    stChanAttr.enChannelType = HI_UNF_DMX_CHAN_TYPE_SEC;
    stChanAttr.enCRCMode     = HI_UNF_DMX_CHAN_CRC_MODE_BY_SYNTAX_AND_DISCARD;
    stChanAttr.enOutputMode  = HI_UNF_DMX_CHAN_OUTPUT_MODE_PLAY;
    s32Ret = HI_UNF_DMX_CreateChannel(u32DmxId, &stChanAttr, &hChan);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_CreateChannel fail 0x%x", s32Ret);
        goto DMX_EXIT;
    }

    s32Ret = HI_UNF_DMX_SetChannelPID(hChan, pstDataFilter->u32TSPID);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_SetChannelPID fail 0x%x", s32Ret);
        goto DMX_DESTROY;
    }

    stFilterAttr.u32FilterDepth = pstDataFilter->u16FilterDepth;
    memcpy(stFilterAttr.au8Match, pstDataFilter->u8Match, DMX_FILTER_MAX_DEPTH);
    memcpy(stFilterAttr.au8Mask, pstDataFilter->u8Mask, DMX_FILTER_MAX_DEPTH);
    memcpy(stFilterAttr.au8Negate, pstDataFilter->u8Negate, DMX_FILTER_MAX_DEPTH);

    s32Ret = HI_UNF_DMX_CreateFilter(u32DmxId, &stFilterAttr, &hFilter);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_CreateFilter fail 0x%x", s32Ret);
        goto DMX_DESTROY;
    }

    s32Ret = HI_UNF_DMX_SetFilterAttr(hFilter, &stFilterAttr);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_SetFilterAttr fail 0x%x", s32Ret);
        goto DMX_FILTER_DESTROY;
    }

    s32Ret = HI_UNF_DMX_AttachFilter(hFilter, hChan);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_AttachFilter fail 0x%x", s32Ret);
        goto DMX_FILTER_DESTROY;
    }

    s32Ret = HI_UNF_DMX_OpenChannel(hChan);
    if (HI_SUCCESS != s32Ret)
    {
        sample_common_printf("call HI_UNF_DMX_OpenChannel fail 0x%x", s32Ret);
        goto DMX_FILTER_DETACH;
    }

    memset(u8DataBuf, 0, sizeof(u8DataBuf));
    memset(u32BufSize, 0, sizeof(u32BufSize));

    s32Ret = DMX_DataRead(hChan, pstDataFilter->u32TimeOut, u8DataBuf, &u32AquiredNum , u32BufSize);
    if (HI_SUCCESS == s32Ret)
    {
        //multi-SECTION parse
         p = u8DataBuf;
        for(i=0;i<u32AquiredNum;i++)
        {
            pstDataFilter->funSectionFunCallback(p, u32BufSize[i], pstDataFilter->pSectionStruct);
            p = p+MAX_SECTION_LEN;
        }
    }

    HIAPI_RUN(HI_UNF_DMX_CloseChannel(hChan),s32Ret);
    
DMX_FILTER_DETACH:
    HIAPI_RUN(HI_UNF_DMX_DetachFilter(hFilter, hChan),s32Ret);    
DMX_FILTER_DESTROY:
    HIAPI_RUN(HI_UNF_DMX_DestroyFilter(hFilter),s32Ret);
DMX_DESTROY:
    HIAPI_RUN(HI_UNF_DMX_DestroyChannel(hChan),s32Ret);
DMX_EXIT:
    return s32Ret;
}

/*
    Timeout:in millisecond
 */
HI_S32 DMX_DataRead(HI_HANDLE hChannel, HI_U32 u32TimeOutms, HI_U8 *pBuf, HI_U32* pAcquiredNum , HI_U32 * pBuffSize)
{
    HI_U8  u8tableid;
    HI_UNF_DMX_DATA_S sSection[32];
    HI_U32 num, i = 0;
    HI_U32 count = 0 ;
    HI_U32 u32Times = 0;
    HI_U32 u32SecTotalNum=0, u32SecNum = 0;
    HI_U32 RequestNum = 32;
    HI_U8 u8SecGotFlag[MAX_SECTION_NUM];

    u32Times = u32TimeOutms / 10;

    memset(u8SecGotFlag, 0, sizeof(u8SecGotFlag[MAX_SECTION_NUM]));

    while (--u32Times)
    {
        num = 0;
        if ((HI_SUCCESS == HI_UNF_DMX_AcquireBuf(hChannel, RequestNum, &num, sSection, u32TimeOutms)) && (num > 0))
        {
            for (i = 0; i < num; i++)
            {
                if (sSection[i].enDataType == HI_UNF_DMX_DATA_TYPE_WHOLE)
                {
                    u8tableid = sSection[i].pu8Data[0];
                    if( ((EIT_TABLE_ID_SCHEDULE_ACTUAL_LOW <= u8tableid) && ( u8tableid <= EIT_TABLE_ID_SCHEDULE_ACTUAL_HIGH))
                        ||((EIT_TABLE_ID_SCHEDULE_OTHER_LOW <= u8tableid) && ( u8tableid <= EIT_TABLE_ID_SCHEDULE_OTHER_HIGH)))
                    {
                        u32SecNum = sSection[i].pu8Data[6]>>3;
                        u32SecTotalNum = (sSection[i].pu8Data[7]>>3) +1;
                    }
                    else if ( TDT_TABLE_ID == u8tableid ||TOT_TABLE_ID == u8tableid)
                    {
                        u32SecNum = 0;
                        u32SecTotalNum = 1;
                    }
                    else
                    {
                        u32SecNum = sSection[i].pu8Data[6];
                        u32SecTotalNum = sSection[i].pu8Data[7] + 1;
                    }

                    if(u8SecGotFlag[u32SecNum] == 0)
                    {
                        memcpy((void *)(pBuf + u32SecNum * MAX_SECTION_LEN), sSection[i].pu8Data, sSection[i].u32Size);
                        u8SecGotFlag[u32SecNum] = 1;
                        pBuffSize[  u32SecNum] = sSection[i].u32Size;
                        count++;
                    }
                }
            }

            HI_UNF_DMX_ReleaseBuf(hChannel, num, sSection);

            /* to check if all sections are received*/
            if(u32SecTotalNum == count)
                break;
        }
        else
        {
            sample_common_printf("HI_UNF_DMX_AcquireBuf time out\n");
            return HI_FAILURE;
        }
    }

    if (u32Times == 0)
    {
        sample_common_printf("HI_UNF_DMX_AcquireBuf time out\n");
        return HI_FAILURE;
    }

    if(pAcquiredNum != HI_NULL)
        *pAcquiredNum = u32SecTotalNum;

    return HI_SUCCESS;
}

