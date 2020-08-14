
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include "sample_tidx.h"


int FrameTypeChartoSINT32(char type)
{
    int g_FrameType;

    if(type == 'I')
    {
        g_FrameType = 0;    // 0 表示帧的类型为 I
    }
    else if(type == 'P')
    {
        g_FrameType = 1;    // 1 表示帧的类型为 P
    }
    else if(type == 'B')
    {
        g_FrameType = 2;    // 2 表示帧的类型为 B
    }
    else
    {
        g_FrameType = -1;
    }
    return g_FrameType;
}


/***********************************************************/
/*              复位存放理论输出帧信息数组                 */
/***********************************************************/
void ResetFrameInfoTidx(FRAME_INFO_S *pFrameInfo)
{
    int i;

    /*将上次文件信息中的信息清空*/
    for(i = 0 ; i < MAX_LOAD_TIDX_NUM ; i++)
    {
        memset(&(pFrameInfo[i]) , -1 , sizeof(FRAME_INFO_S));
    }
}


/***********************************************************/
/*             从.tidx文件中读取一组帧信息                 */
/***********************************************************/
int ReadFrameInfoFromtidx(STR_CONTEXT_S *pstContext)
{
    int i;
    char *pFrameInfoBuf,*pFrameInfo;
    char type;
    char fieldtype[2];

    /*将上次文件信息中的信息清空*/
    ResetFrameInfoTidx(pstContext->stFrameInfotidx);

    /*tidx文件为空*/
    if(NULL == pstContext->fptidxFile)
    {
        return PACK_ERR;
    }

    /*读理论真信息*/
    if(!feof(pstContext->fptidxFile))
    {
        pFrameInfoBuf = (char *)malloc(1024);
        for(i = 0 ; i < MAX_LOAD_TIDX_NUM ; i++)
        {
            fgets(pFrameInfoBuf,1024,pstContext->fptidxFile);
            pFrameInfo = pFrameInfoBuf;
            sscanf(pFrameInfo, "%d", &(pstContext->stFrameInfotidx[i].s32FrameNum));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo, "%x", &(pstContext->stFrameInfotidx[i].s32Frameoffset));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo, "%d", &(pstContext->stFrameInfotidx[i].s32FrameSize));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%ld", (long *)&(pstContext->stFrameInfotidx[i].FramePts));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%c", &type);
            pstContext->stFrameInfotidx[i].FrameType = FrameTypeChartoSINT32(type);
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%x", &(pstContext->stFrameInfotidx[i].FrameStructure));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%s", fieldtype);
            pstContext->stFrameInfotidx[i].TopFieldType = FrameTypeChartoSINT32(fieldtype[0]);
            pstContext->stFrameInfotidx[i].BtmFieldType = FrameTypeChartoSINT32(fieldtype[1]);
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%x", &(pstContext->stFrameInfotidx[i].TopFieldCrc));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%x", &(pstContext->stFrameInfotidx[i].BtmFieldCrc));
            while(*pFrameInfo != ' ')pFrameInfo++;
            while(*pFrameInfo == ' ')pFrameInfo++;
            sscanf(pFrameInfo,"%d", &(pstContext->stFrameInfotidx[i].NVOPFlag));

            if( feof(pstContext->fptidxFile))
            {
                break;
            }
        }
        /*释放FrameInfoBuf*/
        free(pFrameInfoBuf);
    }


    return PACK_OK;
}

