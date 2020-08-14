#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <string.h>   /* for NULL */
#include <unistd.h>

#include "hi_type.h"
#include "hi_adp.h"
#include "hi_adp_data.h"

static DB_FRONTEND_S DVBFEChan[MAX_FRONTEND_COUNT];
static DB_PROGRAM_S ProgList[MAX_PROG_COUNT];

static HI_S32 ProgTotalCount = 0;
static HI_S32 ProgCurNum = 0;
static HI_S32 DVBFEChanTotalCount = 0;

HI_S32  DB_GetDVBProgInfoByServiceID(HI_U16 u16ServiceID, DB_PROGRAM_S *proginfo )
{
    HI_S32 i = 0;

    if (NULL == proginfo)
    {
        return HI_FAILURE;
    }

    for (i = 0; i < ProgTotalCount; i++)
    {
        if (ProgList[i].u16ServiceID == u16ServiceID)
        {
            break;
        }
    }
    if(i >= ProgTotalCount)
    	return HI_FAILURE;

    memcpy((void *)proginfo, (void *)&ProgList[i], sizeof(DB_PROGRAM_S));

    return i;
}

HI_S32  DB_GetDVBProgInfo(HI_U32 prognum, DB_PROGRAM_S *proginfo )
{
    if ((prognum >= MAX_PROG_COUNT) || (NULL == proginfo))
    {
        return HI_FAILURE;
    }

    memcpy((void *)proginfo, (void *)&ProgList[prognum], sizeof(DB_PROGRAM_S));

    return HI_SUCCESS;
}

HI_S32  DB_SetDVBProgInfo(HI_U32 prognum, DB_PROGRAM_S *proginfo )
{
    if ((prognum >= MAX_PROG_COUNT) || (NULL == proginfo))
    {
        return HI_FAILURE;
    }

    memcpy((void *)&ProgList[prognum], (void *)proginfo, sizeof(DB_PROGRAM_S));

    return HI_SUCCESS;
}

HI_S32  DB_AddDVBProg(DB_PROGRAM_S *proginfo )
{
    HI_S32 i = 0;

    if ((NULL == proginfo) || (ProgTotalCount >= (MAX_PROG_COUNT - 1)))
    {
        return HI_FAILURE;
    }

    for (i = 0; i < ProgTotalCount; i++)
    {
        if (ProgList[i].u16ServiceID == proginfo->u16ServiceID)
        {
            return i;
        }
    }

    memcpy((void *)&ProgList[ProgTotalCount], (void *)proginfo, sizeof(DB_PROGRAM_S));
    ProgTotalCount++;

    return ProgTotalCount;
}

HI_S32  DB_GetProgTotalCount(void)
{
    return ProgTotalCount;
}

HI_S32  DB_GetFEChanInfo(HI_U32 channum, DB_FRONTEND_S *chaninfo )
{
    if ((channum >= MAX_FRONTEND_COUNT) || (NULL == chaninfo))
    {
        return HI_FAILURE;
    }

    memcpy((void *)chaninfo, (void *)&DVBFEChan[channum], sizeof(DB_FRONTEND_S));

    return HI_SUCCESS;
}

HI_S32  DB_SetFEChanInfo(HI_U32 channum, DB_FRONTEND_S *chaninfo )
{
    if ((channum >= MAX_FRONTEND_COUNT) || (NULL == chaninfo))
    {
        return HI_FAILURE;
    }

    memcpy((void *)&DVBFEChan[channum], (void *)chaninfo, sizeof(DB_FRONTEND_S));

    return HI_SUCCESS;
}

HI_S32  DB_AddFEChan(DB_FRONTEND_S *chaninfo)
{
    HI_U32 i;

    if ((DVBFEChanTotalCount >= (MAX_FRONTEND_COUNT)) || (NULL == chaninfo))
    {
        return HI_FAILURE;
    }

    if (FE_TYPE_RF == chaninfo->eFEType)
    {
        for (i = 0; i < DVBFEChanTotalCount; i++)
        {
            if ((DVBFEChan[i].unFEtype.sFEParaRf.u32Frequency == chaninfo->unFEtype.sFEParaRf.u32Frequency)
                && (DVBFEChan[i].unFEtype.sFEParaRf.u32Modulation == chaninfo->unFEtype.sFEParaRf.u32Modulation)
                && (DVBFEChan[i].unFEtype.sFEParaRf.u32SymbolRate == chaninfo->unFEtype.sFEParaRf.u32SymbolRate))
            {
                return i;
            }
        }
    }

    i = DVBFEChanTotalCount;
    memcpy((void *)&DVBFEChan[i], (void *)chaninfo, sizeof(DB_FRONTEND_S));
    DVBFEChanTotalCount++;

    return i;
}

HI_S32  DB_GetFEChanTotalCount(void)
{
    return DVBFEChanTotalCount;
}

HI_S32  DB_Reset(void)
{
    ProgCurNum = ProgTotalCount = 0;
    DVBFEChanTotalCount = 0;

    memset((void *)ProgList, 0, sizeof(ProgList[MAX_PROG_COUNT]));
    memset((void *)DVBFEChan, 0, sizeof(DVBFEChan[MAX_FRONTEND_COUNT]));

    return HI_SUCCESS;
}

HI_S32  DB_SaveToFile(FILE *filestream)
{
    HI_U32 u32ProgListSize, u32FEChanListSize;
    HI_S32 s32Ret;

    u32ProgListSize   = ProgTotalCount * sizeof(DB_PROGRAM_S);
    u32FEChanListSize = DVBFEChanTotalCount * sizeof(DB_FRONTEND_S);

    s32Ret = fwrite((void*)&u32ProgListSize, 1, sizeof(u32ProgListSize), filestream);

    //printf("\n  ret DB_Save 0x%x prog %d", s32Ret, u32ProgListSize);

    s32Ret += fwrite((void*)&u32FEChanListSize, 1, sizeof(u32FEChanListSize), filestream);

    //printf("\n  ret DB_Save 0x%x chan %d", s32Ret, u32FEChanListSize);

    s32Ret += fwrite((void*)&ProgList, 1, u32ProgListSize, filestream);
    s32Ret += fwrite((void*)&DVBFEChan, 1, u32FEChanListSize, filestream);

    sample_common_printf("\n DB_SaveToFile  0x%x \n", s32Ret);
    return HI_SUCCESS;
}

HI_S32  DB_RestoreFromFile(FILE *filestream)
{
    HI_U32 u32ProgListSize, u32FEChanListSize;
    HI_S32 s32Ret;

    s32Ret = fread((void*)&u32ProgListSize, 1, sizeof(u32ProgListSize), filestream);

    //printf("\n  ret DB_RestoreFromFile0x%x prog %d", s32Ret, u32ProgListSize);

    s32Ret += fread((void*)&u32FEChanListSize, 1, sizeof(u32FEChanListSize), filestream);

    //printf("\n  ret DB_RestoreFromFile0x%x chan %d", s32Ret, u32FEChanListSize);

    s32Ret += fread((void*)&ProgList, 1, u32ProgListSize, filestream);

    s32Ret += fread((void*)&DVBFEChan, 1, u32FEChanListSize, filestream);

    ProgTotalCount = u32ProgListSize / sizeof(DB_PROGRAM_S);
    DVBFEChanTotalCount = u32FEChanListSize / sizeof(DB_FRONTEND_S);

    sample_common_printf("\n  DB_RestoreFromFile 0x%x \n", s32Ret);

    return HI_SUCCESS;
}
