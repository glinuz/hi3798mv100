#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "hi_unf_pdm.h"
#include "hi_mpi_pdm.h"

#define PDM_FASTPLAY_DEF_NAME "fastplay"
#define PDM_FASTPLAYBAK_DEF_NAME "fastplaybak"

HI_S32  HI_UNF_PDM_GetBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData)
{
    return HI_MPI_PDM_GetBaseParam(enType, pData);
}

HI_S32  HI_UNF_PDM_UpdateBaseParam(HI_UNF_PDM_BASEPARAM_TYPE_E enType, HI_VOID *pData)
{
    return HI_MPI_PDM_UpdateBaseParam(enType, pData);
}

HI_S32  HI_UNF_PDM_GetLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam)
{
    return HI_MPI_PDM_GetLogoParam(pstLogoParam);
}

HI_S32  HI_UNF_PDM_UpdateLogoParam(HI_UNF_MCE_LOGO_PARAM_S *pstLogoParam)
{
    return HI_MPI_PDM_UpdateLogoParam(pstLogoParam);
}

HI_S32  HI_UNF_PDM_GetLogoContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    return HI_MPI_PDM_GetLogoContent(pu8Content, u32Size);
}

HI_S32  HI_UNF_PDM_UpdateLogoContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    return HI_MPI_PDM_UpdateLogoContent(pu8Content, u32Size);
}

HI_S32  HI_UNF_PDM_GetPlayParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam)
{
    return HI_MPI_PDM_GetPlayParam(PDM_FASTPLAY_DEF_NAME, pstPlayParam);
}

HI_S32  HI_UNF_PDM_UpdatePlayParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam)
{
    return HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAY_DEF_NAME, pstPlayParam);
}

HI_S32  HI_UNF_PDM_GetPlayContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    return HI_MPI_PDM_GetPlayContent(PDM_FASTPLAY_DEF_NAME, pu8Content, u32Size);
}

HI_S32  HI_UNF_PDM_UpdatePlayContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_MCE_PLAY_PARAM_S stPlayParam;

    s32Ret = HI_MPI_PDM_GetPlayParam(PDM_FASTPLAY_DEF_NAME, &stPlayParam);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    stPlayParam.bContentValid = HI_FALSE;

    s32Ret = HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAY_DEF_NAME, &stPlayParam);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = HI_MPI_PDM_UpdatePlayContent(PDM_FASTPLAY_DEF_NAME, pu8Content, u32Size);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    stPlayParam.bContentValid = HI_TRUE;

    s32Ret = HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAY_DEF_NAME, &stPlayParam);

    return s32Ret;
}

HI_S32  HI_UNF_PDM_GetPlayBakParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam)
{
    return HI_MPI_PDM_GetPlayParam(PDM_FASTPLAYBAK_DEF_NAME, pstPlayParam);
}

HI_S32  HI_UNF_PDM_UpdatePlayBakParam(HI_UNF_MCE_PLAY_PARAM_S *pstPlayParam)
{
    return HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAYBAK_DEF_NAME, pstPlayParam);
}

HI_S32  HI_UNF_PDM_GetPlayBakContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    return HI_MPI_PDM_GetPlayContent(PDM_FASTPLAYBAK_DEF_NAME, pu8Content, u32Size);
}

HI_S32  HI_UNF_PDM_UpdatePlayBakContent(HI_U8 *pu8Content, HI_U32 u32Size)
{
    HI_S32 s32Ret = HI_SUCCESS;
    HI_UNF_MCE_PLAY_PARAM_S stPlayParam;

    s32Ret = HI_MPI_PDM_GetPlayParam(PDM_FASTPLAYBAK_DEF_NAME, &stPlayParam);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    stPlayParam.bContentValid = HI_FALSE;

    s32Ret = HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAYBAK_DEF_NAME, &stPlayParam);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    s32Ret = HI_MPI_PDM_UpdatePlayContent(PDM_FASTPLAYBAK_DEF_NAME, pu8Content, u32Size);

    if (s32Ret != HI_SUCCESS)
    {
        return s32Ret;
    }

    stPlayParam.bContentValid = HI_TRUE;

    s32Ret = HI_MPI_PDM_UpdatePlayParam(PDM_FASTPLAYBAK_DEF_NAME, &stPlayParam);

    return s32Ret;
}

HI_S32 HI_UNF_PDM_GetDeviceInfo(HI_CHAR * pstrCfgName, HI_CHAR * pstrCfgValue, HI_U32 u32Size)
{
    return HI_MPI_PDM_GetDeviceInfo(pstrCfgName, pstrCfgValue, u32Size);
}

